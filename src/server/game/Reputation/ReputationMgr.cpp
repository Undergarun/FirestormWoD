////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "DatabaseEnv.h"
#include "ReputationMgr.h"
#include "DBCStores.h"
#include "Player.h"
#include "WorldPacket.h"
#include "World.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#ifndef CROSS
#include "../Garrison/GarrisonMgrConstants.hpp"
#include "../Garrison/GarrisonMgr.hpp"
#endif /* not CROSS */

const int32 ReputationMgr::PointsInRank[MAX_REPUTATION_RANK] = {36000, 3000, 3000, 3000, 6000, 12000, 21000, 1000};
const int32 ReputationMgr::Reputation_Cap = 42999;
const int32 ReputationMgr::Reputation_Bottom = -42000;

ReputationRank ReputationMgr::ReputationToRank(int32 standing)
{
    int32 limit = Reputation_Cap + 1;
    for (int i = MAX_REPUTATION_RANK-1; i >= MIN_REPUTATION_RANK; --i)
    {
        limit -= PointsInRank[i];
        if (standing >= limit)
            return ReputationRank(i);
    }
    return MIN_REPUTATION_RANK;
}

bool ReputationMgr::IsAtWar(uint32 faction_id) const
{
    FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id);

    if (!factionEntry)
    {
        sLog->outError(LOG_FILTER_GENERAL, "ReputationMgr::IsAtWar: Can't get AtWar flag of %s for unknown faction (faction id) #%u.", _player->GetName(), faction_id);
        return 0;
    }

    return IsAtWar(factionEntry);
}

bool ReputationMgr::IsAtWar(FactionEntry const* factionEntry) const
{
    if (!factionEntry)
        return false;

    if (FactionState const* factionState = GetState(factionEntry))
        return (factionState->Flags & FACTION_FLAG_AT_WAR);
    return false;
}

int32 ReputationMgr::GetReputation(uint32 faction_id) const
{
    FactionEntry const* factionEntry = sFactionStore.LookupEntry(faction_id);

    if (!factionEntry)
    {
        sLog->outError(LOG_FILTER_GENERAL, "ReputationMgr::GetReputation: Can't get reputation of %s for unknown faction (faction id) #%u.", _player->GetName(), faction_id);
        return 0;
    }

    return GetReputation(factionEntry);
}

int32 ReputationMgr::GetBaseReputation(FactionEntry const* factionEntry) const
{
    if (!factionEntry)
        return 0;

    uint32 raceMask = _player->getRaceMask();
    uint32 classMask = _player->getClassMask();
    for (int i = 0; i < 4; i++)
    {
        if ((factionEntry->ReputationRaceMask[i] & raceMask ||
            (factionEntry->ReputationRaceMask[i] == 0 &&
                factionEntry->ReputationClassMask[i] != 0)) &&
            (factionEntry->ReputationClassMask[i] & classMask ||
                factionEntry->ReputationClassMask[i] == 0))

            return factionEntry->ReputationBase[i];
    }

    // in faction.dbc exist factions with (RepListId >=0, listed in character reputation list) with all BaseRepRaceMask[i] == 0
    return 0;
}

int32 ReputationMgr::GetReputation(FactionEntry const* factionEntry) const
{
    // Faction without recorded reputation. Just ignore.
    if (!factionEntry)
        return 0;

    if (FactionState const* state = GetState(factionEntry))
        return GetBaseReputation(factionEntry) + state->Standing;

    return 0;
}

ReputationRank ReputationMgr::GetRank(FactionEntry const* factionEntry) const
{
    int32 reputation = GetReputation(factionEntry);
    return ReputationToRank(reputation);
}

ReputationRank ReputationMgr::GetBaseRank(FactionEntry const* factionEntry) const
{
    int32 reputation = GetBaseReputation(factionEntry);
    return ReputationToRank(reputation);
}

void ReputationMgr::ApplyForceReaction(uint32 faction_id, ReputationRank rank, bool apply)
{
    if (apply)
        _forcedReactions[faction_id] = rank;
    else
        _forcedReactions.erase(faction_id);
}

uint32 ReputationMgr::GetDefaultStateFlags(FactionEntry const* factionEntry) const
{
    if (!factionEntry)
        return 0;

    uint32 raceMask = _player->getRaceMask();
    uint32 classMask = _player->getClassMask();
    for (int i = 0; i < 4; i++)
    {
        if ((factionEntry->ReputationRaceMask[i] & raceMask ||
            (factionEntry->ReputationRaceMask[i] == 0 &&
                factionEntry->ReputationClassMask[i] != 0)) &&
            (factionEntry->ReputationClassMask[i] & classMask ||
                factionEntry->ReputationClassMask[i] == 0))

            return factionEntry->ReputationFlags[i];
    }
    return 0;
}

void ReputationMgr::SendForceReactions()
{
    WorldPacket l_Data(SMSG_SET_FORCED_REACTIONS, 6 + _forcedReactions.size() *(4 + 4));

    l_Data.WriteBits(_forcedReactions.size(), 6);
    l_Data.FlushBits();

    for (ForcedReactions::const_iterator l_It = _forcedReactions.begin(); l_It != _forcedReactions.end(); ++l_It)
    {
        l_Data << uint32(l_It->first);                         // faction_id (Faction.dbc)
        l_Data << uint32(l_It->second);                        // reputation rank
    }

    _player->SendDirectMessage(&l_Data);
}

void ReputationMgr::SendState(FactionState const* p_Faction)
{
    auto l_BuildSinglePacket = [this](uint32 p_ReputationListID, uint32 p_Standing)
    {
        WorldPacket l_Data(SMSG_SET_FACTION_STANDING, 200);
        l_Data << float(0);               ///< ReferAFriendBonus
        l_Data << float(0);               ///< BonusFromAchievementSystem
        l_Data << uint32(1);

        l_Data << uint32(p_ReputationListID);
        l_Data << uint32(p_Standing);

        l_Data.WriteBit(true);            ///< ShowVisual
        l_Data.FlushBits();

        _player->SendDirectMessage(&l_Data);
    };

    uint32 count = 0;

    for (FactionStateList::iterator l_It = _factions.begin(); l_It != _factions.end(); ++l_It)
    {
        if (l_It->second.needSend)
        {
            if (l_It->second.ReputationListID != p_Faction->ReputationListID)
                count++;
            else
            {
                l_BuildSinglePacket(l_It->second.ReputationListID, l_It->second.Standing);
                l_It->second.needSend = false;
            }
        }
    }

    WorldPacket l_Data(SMSG_SET_FACTION_STANDING, 200);
    l_Data << float(0);               ///< ReferAFriendBonus
    l_Data << float(0);               ///< BonusFromAchievementSystem
    l_Data << uint32(count);

    for (FactionStateList::iterator l_It = _factions.begin(); l_It != _factions.end(); ++l_It)
    {
        if (l_It->second.needSend && l_It->second.ReputationListID != p_Faction->ReputationListID)
        {
            l_It->second.needSend = false;
            l_Data << uint32(l_It->second.ReputationListID);
            l_Data << uint32(l_It->second.Standing);
        }
    }

    l_Data.WriteBit(false);            ///< ShowVisual
    l_Data.FlushBits();

    _player->SendDirectMessage(&l_Data);

    _sendFactionIncreased = false; // Reset
}

void ReputationMgr::SendInitialReputations()
{
    WorldPacket l_Data(SMSG_INITIALIZE_FACTIONS, 2 * 1024);

    RepListID l_A = 0;

    for (FactionStateList::iterator itr = _factions.begin(); itr != _factions.end(); ++itr)
    {
        // fill in absent fields
        for (; l_A != itr->first; ++l_A)
        {
            l_Data << uint8(0);                   ///< Faction Flags
            l_Data << uint32(0);                  ///< Faction Standings
        }

        l_Data << uint8(itr->second.Flags);       ///< Faction Flags
        l_Data << uint32(itr->second.Standing);   ///< Faction Standings

        itr->second.needSend = false;

        ++l_A;
    }

    for (; l_A != 256; ++l_A)
    {
        l_Data << uint8(0);                       ///< Faction Flags
        l_Data << uint32(0);                      ///< Faction Standings
    }

    // fill in absent fields
    for (l_A = 0 ; l_A != 256; ++l_A)
        l_Data.WriteBit(0);                       ///< Faction Has Bonus

    l_Data.FlushBits();

    _player->SendDirectMessage(&l_Data);
}

void ReputationMgr::SendStates()
{
    for (FactionStateList::iterator itr = _factions.begin(); itr != _factions.end(); ++itr)
        SendState(&(itr->second));
}

void ReputationMgr::SendVisible(FactionState const* faction, bool visible) const
{
    if (_player->GetSession()->PlayerLoading())
        return;

    // make faction visible in reputation list at client
    WorldPacket l_Data(visible ? SMSG_SET_FACTION_VISIBLE : SMSG_SET_FACTION_NOT_VISIBLE, 4);
    l_Data << faction->ReputationListID;
    _player->SendDirectMessage(&l_Data);
}

void ReputationMgr::Initialize()
{
    _factions.clear();
    _visibleFactionCount = 0;
    _honoredFactionCount = 0;
    _reveredFactionCount = 0;
    _exaltedFactionCount = 0;
    _sendFactionIncreased = false;

    for (unsigned int i = 1; i < sFactionStore.GetNumRows(); i++)
    {
        FactionEntry const* factionEntry = sFactionStore.LookupEntry(i);

        if (factionEntry && factionEntry->CanHaveReputation())
        {
            FactionState newFaction;
            newFaction.ID = factionEntry->ID;
            newFaction.ReputationListID = factionEntry->ReputationIndex;
            newFaction.Standing = 0;
            newFaction.Flags = GetDefaultStateFlags(factionEntry);
            newFaction.needSend = true;
            newFaction.needSave = true;

            if (newFaction.Flags & FACTION_FLAG_VISIBLE)
                ++_visibleFactionCount;

            UpdateRankCounters(REP_HOSTILE, GetBaseRank(factionEntry));

            _factions[newFaction.ReputationListID] = newFaction;
        }
    }
}

bool ReputationMgr::SetReputation(FactionEntry const* p_FactionEntry, int32 p_Standing, bool p_Incremental, bool p_NoSpillOver)
{
    sScriptMgr->OnPlayerReputationChange(_player, p_FactionEntry->ID, p_Standing, p_Incremental);
    bool l_Res = false;
    
    if (!p_NoSpillOver)
    {
        /// if spillover definition exists in DB, override DBC
        if (const RepSpilloverTemplate* l_ReputationTemplate = sObjectMgr->GetRepSpilloverTemplate(p_FactionEntry->ID))
        {
            for (uint32 l_I = 0; l_I < MAX_SPILLOVER_FACTIONS; ++l_I)
            {
                if (l_ReputationTemplate->faction[l_I])
                {
                    if (_player->GetReputationRank(l_ReputationTemplate->faction[l_I]) <= ReputationRank(l_ReputationTemplate->faction_rank[l_I]))
                    {
#ifndef CROSS
                        /// Algorithm to add Trading Post bonus (+20% from each reputation earning when you have the building lvl 3)
                        if (_player->GetGarrison() != nullptr)
                        {
                            if (_player->GetGarrison()->GetBuildingWithBuildingID(MS::Garrison::Building::ID::TradingPost_TradingPost_Level3).BuildingID)
                                p_Standing *= 1.2;
                        }
#endif

                        /// bonuses are already given, so just modify standing by rate
                        int32 l_SpilloverRep = int32(p_Standing * l_ReputationTemplate->faction_rate[l_I]);
                        SetOneFactionReputation(sFactionStore.LookupEntry(l_ReputationTemplate->faction[l_I]), l_SpilloverRep, p_Incremental);
                    }
                }
            }
        }
        else
        {
            float p_SpillOverRepOut = float(p_Standing);
            /// check for sub-factions that receive spillover
            std::vector<uint32> const* l_FactionList = GetFactionTeamList(p_FactionEntry->ID);
            /// if has no sub-factions, check for factions with same parent
            if (!l_FactionList && p_FactionEntry->ParentFactionID && p_FactionEntry->ParentFactionModOut != 0.0f)
            {
                p_SpillOverRepOut *= p_FactionEntry->ParentFactionModOut;
                if (FactionEntry const* l_ParentFactionEntry = sFactionStore.LookupEntry(p_FactionEntry->ParentFactionID))
                {
                    FactionStateList::iterator l_ParentState = _factions.find(l_ParentFactionEntry->ReputationIndex);

                    /// some team factions have own reputation standing, in this case do not spill to other sub-factions
                    if (l_ParentState != _factions.end() && (l_ParentState->second.Flags & FACTION_FLAG_SPECIAL))
                        SetOneFactionReputation(l_ParentFactionEntry, int32(p_SpillOverRepOut), p_Incremental);
                    else    ///< spill to "sister" factions
                        l_FactionList = GetFactionTeamList(p_FactionEntry->ParentFactionID);
                }
            }
            if (l_FactionList)
            {
                /// Spillover to affiliated factions
                for (std::vector<uint32>::const_iterator l_It = l_FactionList->begin(); l_It != l_FactionList->end(); ++l_It)
                {
                    if (FactionEntry const* l_FactionEntryCalc = sFactionStore.LookupEntry(*l_It))
                    {
                        if (l_FactionEntryCalc == p_FactionEntry || GetRank(l_FactionEntryCalc) > ReputationRank(l_FactionEntryCalc->ParentFactionCapIn))
                            continue;

                        int32 l_SpilloverRep = int32(p_SpillOverRepOut * l_FactionEntryCalc->ParentFactionModIn);

                        if (l_SpilloverRep != 0 || !p_Incremental)
                            l_Res = SetOneFactionReputation(l_FactionEntryCalc, l_SpilloverRep, p_Incremental);
                    }
                }
            }
        }
    }

    /// spillover done, update faction itself
    FactionStateList::iterator l_FactionIT = _factions.find(p_FactionEntry->ReputationIndex);
    if (l_FactionIT != _factions.end())
    {
        l_Res = SetOneFactionReputation(p_FactionEntry, p_Standing, p_Incremental);

        /// only this faction gets reported to client, even if it has no own visible standing
        SendState(&l_FactionIT->second);
    }
    return l_Res;
}

bool ReputationMgr::SetOneFactionReputation(FactionEntry const* factionEntry, int32 standing, bool incremental)
{
    FactionStateList::iterator itr = _factions.find(factionEntry->ReputationIndex);
    if (itr != _factions.end())
    {
        int32 BaseRep = GetBaseReputation(factionEntry);

        if (incremental)
        {
            // int32 *= float cause one point loss?
#ifndef CROSS
            standing = int32(floor((float)standing * sWorld->getRate(RATE_REPUTATION_GAIN) + 0.5f));

            if (_player->GetSession()->IsPremium())
                standing = int32(floor((float)standing * sWorld->getRate(RATE_REPUTATION_GAIN_PREMIUM) + 0.5f));
#else /* CROSS */
            if (InterRealmClient* client = _player->GetSession()->GetInterRealmClient())
            {
                // int32 *= float cause one point loss?
                standing = int32(floor((float)standing * client->GetReputationRate() + 0.5f));
 
                if (_player->GetSession()->IsPremium())
                    standing = int32(floor((float)standing * client->GetReputationPremiumRate() + 0.5f));
            }
#endif /* CROSS */

            standing += itr->second.Standing + BaseRep;
        }

        if (standing > Reputation_Cap)
            standing = Reputation_Cap;
        else if (standing < Reputation_Bottom)
            standing = Reputation_Bottom;

        ReputationRank old_rank = ReputationToRank(itr->second.Standing + BaseRep);
        ReputationRank new_rank = ReputationToRank(standing);

        itr->second.Standing = standing - BaseRep;
        itr->second.needSend = true;
        itr->second.needSave = true;

        SetVisible(&itr->second);

        if (new_rank <= REP_HOSTILE)
            SetAtWar(&itr->second, true);

        if (new_rank > old_rank)
            _sendFactionIncreased = true;

        UpdateRankCounters(old_rank, new_rank);

        _player->ReputationChanged(factionEntry);
        _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KNOWN_FACTIONS,          factionEntry->ID);
        _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GAIN_REPUTATION,         factionEntry->ID);
        _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GAIN_EXALTED_REPUTATION, factionEntry->ID);
        _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GAIN_REVERED_REPUTATION, factionEntry->ID);
        _player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_GAIN_HONORED_REPUTATION, factionEntry->ID);

        return true;
    }
    return false;
}

void ReputationMgr::SetVisible(FactionTemplateEntry const*factionTemplateEntry)
{
    if (!factionTemplateEntry->Faction)
        return;

    if (FactionEntry const* factionEntry = sFactionStore.LookupEntry(factionTemplateEntry->Faction))
        // Never show factions of the opposing team
        if (!(factionEntry->ReputationRaceMask[1] & _player->getRaceMask() && factionEntry->ReputationBase[1] == Reputation_Bottom))
            SetVisible(factionEntry);
}

void ReputationMgr::SetVisible(FactionEntry const* factionEntry)
{
    if (!factionEntry->CanHaveReputation())
        return;

    FactionStateList::iterator itr = _factions.find(factionEntry->ReputationIndex);
    if (itr == _factions.end())
        return;

    SetVisible(&itr->second);
}

void ReputationMgr::SetVisible(FactionState* faction)
{
    // always invisible or hidden faction can't be make visible
    // except if faction has FACTION_FLAG_SPECIAL
    if (faction->Flags & (FACTION_FLAG_INVISIBLE_FORCED | FACTION_FLAG_HIDDEN) && !(faction->Flags & FACTION_FLAG_SPECIAL))
        return;

    // already set
    if (faction->Flags & FACTION_FLAG_VISIBLE)
        return;

    faction->Flags |= FACTION_FLAG_VISIBLE;
    faction->needSend = true;
    faction->needSave = true;

    ++_visibleFactionCount;

    SendVisible(faction);
}

void ReputationMgr::SetAtWar(RepListID repListID, bool on)
{
    FactionStateList::iterator itr = _factions.find(repListID);
    if (itr == _factions.end())
        return;

    // always invisible or hidden faction can't change war state
    if (itr->second.Flags & (FACTION_FLAG_INVISIBLE_FORCED | FACTION_FLAG_HIDDEN))
        return;

    SetAtWar(&itr->second, on);
}

void ReputationMgr::SetAtWar(FactionState* faction, bool atWar) const
{
    // not allow declare war to own faction
    if (atWar && (faction->Flags & FACTION_FLAG_PEACE_FORCED))
        return;

    // already set
    if (((faction->Flags & FACTION_FLAG_AT_WAR) != 0) == atWar)
        return;

    if (atWar)
        faction->Flags |= FACTION_FLAG_AT_WAR;
    else
        faction->Flags &= ~FACTION_FLAG_AT_WAR;

    faction->needSend = true;
    faction->needSave = true;
}

void ReputationMgr::SetInactive(RepListID repListID, bool on)
{
    FactionStateList::iterator itr = _factions.find(repListID);
    if (itr == _factions.end())
        return;

    SetInactive(&itr->second, on);
}

void ReputationMgr::SetInactive(FactionState* faction, bool inactive) const
{
    // always invisible or hidden faction can't be inactive
    if (inactive && ((faction->Flags & (FACTION_FLAG_INVISIBLE_FORCED | FACTION_FLAG_HIDDEN)) || !(faction->Flags & FACTION_FLAG_VISIBLE)))
        return;

    // already set
    if (((faction->Flags & FACTION_FLAG_INACTIVE) != 0) == inactive)
        return;

    if (inactive)
        faction->Flags |= FACTION_FLAG_INACTIVE;
    else
        faction->Flags &= ~FACTION_FLAG_INACTIVE;

    faction->needSend = true;
    faction->needSave = true;
}

void ReputationMgr::LoadFromDB(PreparedQueryResult result)
{
    // Set initial reputations (so everything is nifty before DB data load)
    Initialize();

    //QueryResult* result = CharacterDatabase.PQuery("SELECT faction, standing, flags FROM character_reputation WHERE guid = '%u'", GetGUIDLow());

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            FactionEntry const* factionEntry = sFactionStore.LookupEntry(fields[0].GetUInt16());
            if (factionEntry && (factionEntry->ReputationIndex >= 0))
            {
                FactionState* faction = &_factions[factionEntry->ReputationIndex];

                // update standing to current
                faction->Standing = fields[1].GetInt32();

                // update counters
                int32 BaseRep = GetBaseReputation(factionEntry);
                ReputationRank old_rank = ReputationToRank(BaseRep);
                ReputationRank new_rank = ReputationToRank(BaseRep + faction->Standing);
                UpdateRankCounters(old_rank, new_rank);

                uint32 dbFactionFlags = fields[2].GetUInt16();

                if (dbFactionFlags & FACTION_FLAG_VISIBLE)
                    SetVisible(faction);                    // have internal checks for forced invisibility

                if (dbFactionFlags & FACTION_FLAG_INACTIVE)
                    SetInactive(faction, true);              // have internal checks for visibility requirement

                if (dbFactionFlags & FACTION_FLAG_AT_WAR)  // DB at war
                    SetAtWar(faction, true);                 // have internal checks for FACTION_FLAG_PEACE_FORCED
                else                                        // DB not at war
                {
                    // allow remove if visible (and then not FACTION_FLAG_INVISIBLE_FORCED or FACTION_FLAG_HIDDEN)
                    if (faction->Flags & FACTION_FLAG_VISIBLE)
                        SetAtWar(faction, false);            // have internal checks for FACTION_FLAG_PEACE_FORCED
                }

                // set atWar for hostile
                if (GetRank(factionEntry) <= REP_HOSTILE)
                    SetAtWar(faction, true);

                // reset changed flag if values similar to saved in DB
                if (faction->Flags == dbFactionFlags)
                {
                    faction->needSend = false;
                    faction->needSave = false;
                }
            }
        }
        while (result->NextRow());
    }
}

void ReputationMgr::SaveToDB(SQLTransaction& trans)
{
    for (FactionStateList::iterator itr = _factions.begin(); itr != _factions.end(); ++itr)
    {
        if (itr->second.needSave)
        {
            trans->PAppend("DELETE FROM character_reputation WHERE guid = '%u' AND faction='%u'", _player->GetRealGUIDLow(), uint16(itr->second.ID));
            trans->PAppend("INSERT INTO character_reputation (guid, faction, standing, flags) VALUES ('%u', '%u', '%d', '%u')", _player->GetRealGUIDLow(), uint16(itr->second.ID), itr->second.Standing, uint16(itr->second.Flags));
            itr->second.needSave = false;
        }
    }
}

void ReputationMgr::UpdateRankCounters(ReputationRank old_rank, ReputationRank new_rank)
{
    if (old_rank >= REP_EXALTED)
        --_exaltedFactionCount;
    if (old_rank >= REP_REVERED)
        --_reveredFactionCount;
    if (old_rank >= REP_HONORED)
        --_honoredFactionCount;

    if (new_rank >= REP_EXALTED)
        ++_exaltedFactionCount;
    if (new_rank >= REP_REVERED)
        ++_reveredFactionCount;
    if (new_rank >= REP_HONORED)
        ++_honoredFactionCount;
}
