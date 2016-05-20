////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "Opcodes.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "Group.h"
#include "Formulas.h"
#include "ObjectAccessor.h"
#include "Battleground.h"
#include "BattlegroundMgr.hpp"
#include "MapManager.h"
#include "InstanceSaveMgr.h"
#include "MapInstanced.h"
#include "Util.h"
#include "LFGMgr.h"
#include "UpdateFieldFlags.h"
#include "LFGListMgr.h"

Roll::Roll(uint64 _guid, LootItem const& li) : itemGUID(_guid), itemid(li.itemid),
    itemRandomPropId(li.randomPropertyId), itemRandomSuffix(li.randomSuffix), itemCount(li.count),
    totalPlayersRolling(0), totalNeed(0), totalGreed(0), totalPass(0), itemSlot(0),
    rollVoteMask(ROLL_ALL_TYPE_NO_DISENCHANT), m_ItemBonuses(li.itemBonuses)
{
}

Roll::~Roll()
{
}

void Roll::setLoot(Loot* pLoot)
{
    link(pLoot, this);
}

Loot* Roll::getLoot()
{
    return getTarget();
}

Group::Group() : m_leaderGuid(0), m_leaderName(""), m_PartyFlags(PARTY_FLAG_NORMAL),
m_dungeonDifficulty(DifficultyNormal), m_raidDifficulty(DifficultyRaidNormal), m_LegacyRaidDifficuty(Difficulty10N),
    m_bgGroup(NULL), m_bfGroup(NULL), m_lootMethod(FREE_FOR_ALL), m_lootThreshold(ITEM_QUALITY_UNCOMMON), m_looterGuid(0),
    m_subGroupsCounts(NULL), m_guid(0), m_UpdateCount(0), m_maxEnchantingLevel(0), m_dbStoreId(0), m_readyCheckCount(0),
    m_membersInInstance(0), m_readyCheck(false), m_Team(0)
{
    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i] = 0;

    uint32 lowguid = sGroupMgr->GenerateGroupId();
    m_guid = MAKE_NEW_GUID(lowguid, 0, HIGHGUID_GROUP);

    m_RaidMarkers.resize(eRaidMarkersMisc::MaxRaidMarkers);
}

Group::~Group()
{
    if (m_bgGroup)
    {
        sLog->outDebug(LOG_FILTER_BATTLEGROUND, "Group::~Group: battleground group being deleted.");
        if (m_bgGroup->GetBgRaid(ALLIANCE) == this) m_bgGroup->SetBgRaid(ALLIANCE, NULL);
        else if (m_bgGroup->GetBgRaid(HORDE) == this) m_bgGroup->SetBgRaid(HORDE, NULL);
        else sLog->outError(LOG_FILTER_GENERAL, "Group::~Group: battleground group is not linked to the correct battleground.");
    }
    Rolls::iterator itr;
    while (!RollId.empty())
    {
        itr = RollId.begin();
        Roll *r = *itr;
        RollId.erase(itr);
        delete(r);
    }

    // it is undefined whether objectmgr (which stores the groups) or instancesavemgr
    // will be unloaded first so we must be prepared for both cases
    // this may unload some instance saves
    for (uint8 i = 0; i < Difficulty::MaxDifficulties; ++i)
        for (BoundInstancesMap::iterator itr2 = m_boundInstances[i].begin(); itr2 != m_boundInstances[i].end(); ++itr2)
            itr2->second.save->RemoveGroup(this);

    // Sub group counters clean up
    delete[] m_subGroupsCounts;
}

bool Group::Create(Player* leader)
{
    uint64 leaderGuid = leader->GetGUID();

    m_leaderGuid = leaderGuid;
    m_leaderName = leader->GetName();

    leader->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);

    m_PartyFlags  = (isBGGroup() || isBFGroup()) ? PARTY_FLAG_BGRAID : PARTY_FLAG_NORMAL;

    if (m_PartyFlags & PARTY_FLAG_RAID)
        _initRaidSubGroupsCounter();

    m_lootMethod = GROUP_LOOT;
    m_lootThreshold = ITEM_QUALITY_UNCOMMON;
    m_looterGuid = leaderGuid;

    m_dungeonDifficulty = DifficultyNormal;
    m_raidDifficulty = DifficultyRaidNormal;
    m_LegacyRaidDifficuty = Difficulty10N;

    m_Team = leader->GetTeam();

    if (!isBGGroup() && !isBFGroup())
    {
        m_dungeonDifficulty = leader->GetDungeonDifficultyID();

        bool l_NewLFR = leader->getLevel() == MAX_LEVEL;
        m_raidDifficulty = isLFGGroup() ? (l_NewLFR ? Difficulty::DifficultyRaidLFR : Difficulty::DifficultyRaidTool) : leader->GetLegacyRaidDifficultyID();

        if (l_NewLFR)
        {
            /// Loot options sniffed from new LFR type
            m_lootMethod = LootMethod::FREE_FOR_ALL;
            m_lootThreshold = ItemQualities::ITEM_QUALITY_POOR;
        }

        m_dbStoreId = sGroupMgr->GenerateNewGroupDbStoreId();

        sGroupMgr->RegisterGroupDbStoreId(m_dbStoreId, this);

        // Store group in database
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP);

        uint8 index = 0;

        stmt->setUInt32(index++, m_dbStoreId);
        stmt->setUInt32(index++, GUID_LOPART(m_leaderGuid));
        stmt->setUInt8(index++, uint8(m_lootMethod));
        stmt->setUInt32(index++, GUID_LOPART(m_looterGuid));
        stmt->setUInt8(index++, uint8(m_lootThreshold));
        stmt->setUInt32(index++, uint32(m_targetIcons[0]));
        stmt->setUInt32(index++, uint32(m_targetIcons[1]));
        stmt->setUInt32(index++, uint32(m_targetIcons[2]));
        stmt->setUInt32(index++, uint32(m_targetIcons[3]));
        stmt->setUInt32(index++, uint32(m_targetIcons[4]));
        stmt->setUInt32(index++, uint32(m_targetIcons[5]));
        stmt->setUInt32(index++, uint32(m_targetIcons[6]));
        stmt->setUInt32(index++, uint32(m_targetIcons[7]));
        stmt->setUInt8(index++, uint8(m_PartyFlags));
        stmt->setUInt32(index++, uint8(m_dungeonDifficulty));
        stmt->setUInt32(index++, uint8(m_raidDifficulty));
        stmt->setUInt32(index++, uint8(m_LegacyRaidDifficuty));

        CharacterDatabase.Execute(stmt);


        ASSERT(AddMember(leader)); // If the leader can't be added to a new group because it appears full, something is clearly wrong.

        Player::ConvertInstancesToGroup(leader, this, false);
    }
    else if (!AddMember(leader))
        return false;

    return true;
}

uint8 Group::GetPartyFlags() const
{
    return m_PartyFlags;
}
uint8 Group::GetPartyIndex() const
{
    return PARTY_INDEX_NORMAL;
    /// Need more work
    /// return (m_PartyFlags & PARTY_FLAG_MASK_INSTANCE) != 0 ? PARTY_INDEX_INSTANCE : PARTY_INDEX_NORMAL;
}
uint8 Group::GetPartyType() const
{
    return 0;
}

void Group::LoadGroupFromDB(Field* fields)
{
    m_dbStoreId = fields[15].GetUInt32();
    m_leaderGuid = MAKE_NEW_GUID(fields[0].GetUInt32(), 0, HIGHGUID_PLAYER);

    // group leader not exist
    if (!sObjectMgr->GetPlayerNameByGUID(fields[0].GetUInt32(), m_leaderName))
        return;

    m_lootMethod = LootMethod(fields[1].GetUInt8());
    m_looterGuid = MAKE_NEW_GUID(fields[2].GetUInt32(), 0, HIGHGUID_PLAYER);
    m_lootThreshold = ItemQualities(fields[3].GetUInt8());

    for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
        m_targetIcons[i] = fields[4+i].GetUInt32();

    m_PartyFlags  = PartyFlags(fields[12].GetUInt8());
    if (m_PartyFlags & PARTY_FLAG_RAID)
        _initRaidSubGroupsCounter();

    m_dungeonDifficulty = Player::CheckLoadedDungeonDifficultyID(Difficulty(fields[13].GetUInt8()));
    m_raidDifficulty = Player::CheckLoadedRaidDifficultyID(Difficulty(fields[14].GetUInt8()));

    if (m_PartyFlags & PARTY_FLAG_LFG)
        sLFGMgr->_LoadFromDB(fields, GetGUID());

    m_LegacyRaidDifficuty = Player::CheckLoadedLegacyRaidDifficultyID(Difficulty(fields[18].GetUInt8()));
}

void Group::LoadMemberFromDB(uint32 guidLow, uint8 memberFlags, uint8 subgroup, uint8 roles, uint8 playerClass, uint32 specId)
{
    MemberSlot member;
    member.guid = MAKE_NEW_GUID(guidLow, 0, HIGHGUID_PLAYER);

    // skip non-existed member
    if (!sObjectMgr->GetPlayerNameByGUID(member.guid, member.name))
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
        stmt->setUInt32(0, guidLow);
        CharacterDatabase.Execute(stmt);
        return;
    }

    member.group = subgroup;
    member.flags = memberFlags;
    member.roles = roles;
    member.playerClass = playerClass;
    member.specID = specId;

    m_memberSlots.push_back(member);

    SubGroupCounterIncrease(subgroup);

    if (isLFGGroup())
    {
        LfgDungeonSet Dungeons;
        Dungeons.insert(sLFGMgr->GetDungeon(GetGUID()));
        sLFGMgr->SetSelectedDungeons(member.guid, Dungeons);
        sLFGMgr->SetState(member.guid, sLFGMgr->GetState(GetGUID()));
    }
}

void Group::ChangeFlagEveryoneAssistant(bool apply)
{
    if (apply)
        m_PartyFlags = PartyFlags(m_PartyFlags | PARTY_FLAG_EVERYONE_IS_ASSISTANT);
    else
        m_PartyFlags = PartyFlags(m_PartyFlags &~ PARTY_FLAG_EVERYONE_IS_ASSISTANT);

    this->SendUpdate();
}

void Group::ConvertToLFG()
{
    m_PartyFlags = PartyFlags(m_PartyFlags | PARTY_FLAG_LFG | PARTY_FLAG_UNK1);
    m_lootMethod = NEED_BEFORE_GREED;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_PartyFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();
}

void Group::ConvertToRaid()
{
    m_PartyFlags = PartyFlags(m_PartyFlags | PARTY_FLAG_RAID);

    _initRaidSubGroupsCounter();

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_PartyFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

void Group::ConvertToGroup()
{
    if (m_memberSlots.size() > 5)
        return; // What message error should we send?

    m_PartyFlags = PartyFlags(PARTY_FLAG_NORMAL);

    if (m_subGroupsCounts)
    {
        delete[] m_subGroupsCounts;
        m_subGroupsCounts = NULL;
    }

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_TYPE);

        stmt->setUInt8(0, uint8(m_PartyFlags));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    SendUpdate();

    // update quest related GO states (quest activity dependent from raid membership)
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
        if (Player* player = ObjectAccessor::FindPlayer(citr->guid))
            player->UpdateForQuestWorldObjects();
}

bool Group::AddInvite(Player* player)
{
    if (!player || player->GetGroupInvite())
        return false;
    Group* group = player->GetGroup();
    if (group && (group->isBGGroup() || group->isBFGroup()))
        group = player->GetOriginalGroup();
    if (group)
        return false;

    RemoveInvite(player);

    m_inviteesLock.acquire();
    m_invitees.insert(player->GetGUID());
    m_inviteesLock.release();

    player->SetGroupInvite(this->GetGUID());

    sScriptMgr->OnGroupInviteMember(this, player->GetGUID());

    return true;
}

bool Group::AddLeaderInvite(Player* player)
{
    if (!AddInvite(player))
        return false;

    m_leaderGuid = player->GetGUID();
    m_leaderName = player->GetName();
    player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);
    return true;
}

void Group::RemoveInvite(Player* player)
{
    if (player)
    {
        m_inviteesLock.acquire();
        m_invitees.erase(player->GetGUID());
        m_inviteesLock.release();
        player->SetGroupInvite(0);
    }
}

void Group::RemoveAllInvites()
{
    m_inviteesLock.acquire();
    for (InvitesList::iterator itr=m_invitees.begin(); itr != m_invitees.end(); ++itr)
        if (Player* plr = sObjectAccessor->FindPlayer(*itr))
            plr->SetGroupInvite(0);

    m_invitees.clear();
    m_inviteesLock.release();
}

Player* Group::GetInvited(uint64 guid) const
{
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        if ((*itr) == guid)
            return sObjectAccessor->FindPlayer(*itr);
    }
    return NULL;
}

Player* Group::GetInvited(const std::string& name) const
{
    m_inviteesLock.acquire();
    for (InvitesList::const_iterator itr = m_invitees.begin(); itr != m_invitees.end(); ++itr)
    {
        Player* plr = sObjectAccessor->FindPlayer(*itr);
        if (!plr)
            continue;

        if (plr->GetName() == name)
        {
            m_inviteesLock.release();
            return plr;
        }
    }
    m_inviteesLock.release();
    return NULL;
}

bool Group::AddMember(Player* p_Player)
{
    // Get first not-full group
    uint8 l_SubGroup = 0;
    if (m_subGroupsCounts)
    {
        bool l_GroupFound = false;
        for (; l_SubGroup < MAX_RAID_SUBGROUPS; ++l_SubGroup)
        {
            if (m_subGroupsCounts[l_SubGroup] < MAXGROUPSIZE)
            {
                l_GroupFound = true;
                break;
            }
        }
        // We are raid group and no one slot is free
        if (!l_GroupFound)
            return false;
    }

    MemberSlot l_Member;
    l_Member.guid         = p_Player->GetGUID();
    l_Member.name         = p_Player->GetName();
    l_Member.group        = l_SubGroup;
    l_Member.flags        = 0;
    l_Member.roles        = 0;
    l_Member.playerClass  = p_Player->getClass();
    l_Member.specID       = p_Player->GetSpecializationId(p_Player->GetActiveSpec());
    m_memberSlots.push_back(l_Member);

    SubGroupCounterIncrease(l_SubGroup);

    p_Player->SetGroupInvite(0);
    if (p_Player->GetGroup() && (isBGGroup() || isBFGroup())) // if player is in group and he is being added to BG raid group, then call SetBattlegroundRaid()
        p_Player->SetBattlegroundOrBattlefieldRaid(this, l_SubGroup);
    else if (p_Player->GetGroup()) //if player is in bg raid and we are adding him to normal group, then call SetOriginalGroup()
        p_Player->SetOriginalGroup(this, l_SubGroup);
    else //if player is not in group, then call set group
        p_Player->SetGroup(this, l_SubGroup);

    // if the same group invites the player back, cancel the homebind timer
    InstanceGroupBind* l_Bind = GetBoundInstance(p_Player);
    if (l_Bind && l_Bind->save->GetInstanceId() == p_Player->GetInstanceId())
        p_Player->m_InstanceValid = true;

    if (sLFGListMgr->IsGroupQueued(this))
        sLFGListMgr->PlayerAddedToGroup(p_Player, this);

    if (!isRaidGroup())                                      // reset targetIcons for non-raid-groups
    {
        for (uint8 i = 0; i < TARGETICONCOUNT; ++i)
            m_targetIcons[i] = 0;
    }

    // insert into the table if we're not a battleground group
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_INS_GROUP_MEMBER);

        l_Statement->setUInt32(0, m_dbStoreId);
        l_Statement->setUInt32(1, GUID_LOPART(l_Member.guid));
        l_Statement->setUInt8(2, l_Member.flags);
        l_Statement->setUInt8(3, l_Member.group);
        l_Statement->setUInt8(4, l_Member.roles);
        l_Statement->setUInt8(5, l_Member.playerClass);
        l_Statement->setUInt32(6, l_Member.specID);

        CharacterDatabase.Execute(l_Statement);
    }

    SendUpdate();
    sScriptMgr->OnGroupAddMember(this, p_Player->GetGUID());

    if (p_Player->IsInWorld() && p_Player->GetMap() != nullptr)
    {
        WorldPacket l_Data;
        p_Player->GetSession()->BuildPartyMemberStatsChangedPacket(p_Player, &l_Data, 0);

        for (GroupReference* l_Itr = GetFirstMember(); l_Itr != NULL; l_Itr = l_Itr->next())
        {
            Player* l_Member = l_Itr->getSource();
            if (l_Member && l_Member != p_Player && !l_Member->IsWithinDist(p_Player, l_Member->GetSightRange(), false)
                && l_Member->IsInWorld() && l_Member->GetMap() != nullptr)
            {
                WorldPacket l_Data2;
                l_Member->GetSession()->BuildPartyMemberStatsChangedPacket(l_Member, &l_Data2, 0);

                l_Member->GetSession()->SendPacket(&l_Data);
                p_Player->GetSession()->SendPacket(&l_Data2);
            }
        }
    }

    if (!IsLeader(p_Player->GetGUID()) && !isBGGroup() && !isBFGroup())
    {
        // reset the new member's instances, unless he is currently in one of them
        // including raid/heroic instances that they are not permanently bound to!
        p_Player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, false, false);
        p_Player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true, false);
        p_Player->ResetInstances(INSTANCE_RESET_GROUP_JOIN, true, true);

        if (p_Player->getLevel() >= LEVELREQUIREMENT_HEROIC)
        {
            if (p_Player->GetDungeonDifficultyID() != GetDungeonDifficultyID())
            {
                p_Player->SetDungeonDifficultyID(GetDungeonDifficultyID());
                p_Player->SendDungeonDifficulty();
            }
            if (p_Player->GetRaidDifficultyID() != GetRaidDifficultyID())
            {
                p_Player->SetRaidDifficultyID(GetRaidDifficultyID());
                p_Player->SendRaidDifficulty(false);
            }
            if (p_Player->GetLegacyRaidDifficultyID() != GetLegacyRaidDifficultyID())
            {
                p_Player->SetLegacyRaidDifficultyID(GetLegacyRaidDifficultyID());
                p_Player->SendRaidDifficulty(true);
            }
        }
    }

    p_Player->SetGroupUpdateFlag(GROUP_UPDATE_FULL);
    UpdatePlayerOutOfRange(p_Player);

    // quest related GO state dependent from raid membership
    if (isRaidGroup())
        p_Player->UpdateForQuestWorldObjects();

    // Broadcast new player group member fields to rest of the group
    p_Player->SetFieldNotifyFlag(UF_FLAG_UNIT_ALL);

    UpdateData l_GroupData(p_Player->GetMapId());
    WorldPacket l_GroupDataPacket;

    // Broadcast group members' fields to player
    for (GroupReference* l_Itr = GetFirstMember(); l_Itr != NULL; l_Itr = l_Itr->next())
    {
        if (l_Itr->getSource() == p_Player)
            continue;

        if (Player* l_Member = l_Itr->getSource())
        {
            if (p_Player->HaveAtClient(l_Member))   // must be on the same map, or shit will break
            {
                l_Member->SetFieldNotifyFlag(UF_FLAG_UNIT_ALL);
                l_Member->BuildValuesUpdateBlockForPlayer(&l_GroupData, p_Player);
                l_Member->RemoveFieldNotifyFlag(UF_FLAG_UNIT_ALL);
            }

            if (l_Member->HaveAtClient(p_Player))
            {
                UpdateData l_NewData(p_Player->GetMapId());
                WorldPacket l_NewDataPacket;
                p_Player->BuildValuesUpdateBlockForPlayer(&l_NewData, l_Member);

                if (l_NewData.HasData())
                {
                    if (l_NewData.BuildPacket(&l_NewDataPacket))
                        l_Member->SendDirectMessage(&l_NewDataPacket);
                }
            }
        }
    }

    if (l_GroupData.HasData())
    {
        if (l_GroupData.BuildPacket(&l_GroupDataPacket))
            p_Player->SendDirectMessage(&l_GroupDataPacket);
    }

    p_Player->RemoveFieldNotifyFlag(UF_FLAG_UNIT_ALL);

    if (m_maxEnchantingLevel < p_Player->GetSkillValue(SKILL_ENCHANTING))
        m_maxEnchantingLevel = p_Player->GetSkillValue(SKILL_ENCHANTING);

    SendTargetIconList(p_Player->GetSession(), 0);

    return true;
}

bool Group::RemoveMember(uint64 p_Guid, RemoveMethod const& p_Method /*= GROUP_REMOVEMETHOD_DEFAULT*/, uint64 p_Kicker /*= 0*/, char const* p_Reason /*= NULL*/)
{
    BroadcastGroupUpdate();

    if (Player* l_Player = sObjectAccessor->FindPlayer(p_Guid))
        if (sLFGListMgr->IsGroupQueued(this))
            sLFGListMgr->PlayerRemoveFromGroup(l_Player, this);

    sScriptMgr->OnGroupRemoveMember(this, p_Guid, p_Method, p_Kicker, p_Reason);

    /// LFG group vote kick handled in scripts
    if (isLFGGroup() && p_Method == GROUP_REMOVEMETHOD_KICK)
        return m_memberSlots.size();

    /// Remove member and change leader (if need) only if strong more 2 members _before_ member remove (BG/BF allow 1 member group)
    if (sLFGListMgr->IsGroupQueued(this) || GetMembersCount() > ((isBGGroup() || isLFGGroup() || isBFGroup()) ? 1u : 2u))
    {
        Player * l_Player = ObjectAccessor::GetObjectInOrOutOfWorld(p_Guid, (Player*)NULL);

        if (l_Player)
        {
            /// Battleground group handling
            if (isBGGroup() || isBFGroup())
                l_Player->RemoveFromBattlegroundOrBattlefieldRaid();
            /// Regular group
            else
            {
                if (l_Player->GetOriginalGroup() == this)
                    l_Player->SetOriginalGroup(NULL);
                else
                    l_Player->SetGroup(NULL);

                /// Quest related GO state dependent from raid membership
                l_Player->UpdateForQuestWorldObjects();
            }

            WorldPacket l_Data;

            if (p_Method == GROUP_REMOVEMETHOD_KICK)
            {
                l_Data.Initialize(SMSG_GROUP_UNINVITE, 0);
                l_Player->GetSession()->SendPacket(&l_Data);
            }

            uint64 l_GroupGUID = GetGUID();
            uint64 l_LeaderGUID = GetLeaderGUID();

            uint32 l_MemberCount = 0;

            bool l_HasJamCliPartyLFGInfo = isLFGGroup(); ///< l_HasJamCliPartyLFGInfo is never read 01/18/16

            l_Data.Initialize(SMSG_PARTY_UPDATE, 200);
            l_Data << uint8(GetPartyFlags());
            l_Data << uint8(GetPartyIndex());
            l_Data << uint8(GetPartyType());
            l_Data << int32(-1);                ///< MyIndex
            l_Data.appendPackGUID(l_GroupGUID);
            l_Data << uint32(m_UpdateCount++);
            l_Data.appendPackGUID(l_LeaderGUID);
            l_Data << uint32(l_MemberCount);

            l_Data.WriteBit(false); ///< LfgInfos
            l_Data.WriteBit(false); ///< LootSettings
            l_Data.WriteBit(false);
            l_Data.FlushBits();

            l_Player->GetSession()->SendPacket(&l_Data);

            _homebindIfInstance(l_Player);

            /// Fix ghost group leader flag
            l_Player->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);
        }

        /// Remove player from group in DB
        if (!isBGGroup() && !isBFGroup())
        {
            PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER);
            stmt->setUInt32(0, GUID_LOPART(p_Guid));
            CharacterDatabase.Execute(stmt);
            DelinkMember(p_Guid);
        }

        /// Reevaluate group enchanter if the leaving player had enchanting skill or the player is offline
        if ((l_Player && l_Player->GetSkillValue(SKILL_ENCHANTING)) || !l_Player)
            ResetMaxEnchantingLevel();

        /// Remove player from loot rolls
        for (Rolls::iterator l_It = RollId.begin(); l_It != RollId.end(); ++l_It)
        {
            Roll* l_Roll = *l_It;
            Roll::PlayerVote::iterator l_RollPlayerVoteIT = l_Roll->playerVote.find(p_Guid);

            if (l_RollPlayerVoteIT == l_Roll->playerVote.end())
                continue;

            if (l_RollPlayerVoteIT->second == GREED || l_RollPlayerVoteIT->second == DISENCHANT)
                --l_Roll->totalGreed;
            else if (l_RollPlayerVoteIT->second == NEED)
                --l_Roll->totalNeed;
            else if (l_RollPlayerVoteIT->second == PASS)
                --l_Roll->totalPass;

            if (l_RollPlayerVoteIT->second != NOT_VALID)
                --l_Roll->totalPlayersRolling;

            l_Roll->playerVote.erase(l_RollPlayerVoteIT);

            CountRollVote(p_Guid, l_Roll->itemGUID, MAX_ROLL_TYPE);
        }

        /// Update subgroups
        member_witerator slot = _getMemberWSlot(p_Guid);
        if (slot != m_memberSlots.end())
        {
            SubGroupCounterDecrease(slot->group);
            m_memberSlots.erase(slot);
        }

        /// Pick new leader if necessary
        if (m_leaderGuid == p_Guid)
        {
            for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
            {
                if (ObjectAccessor::FindPlayer(itr->guid))
                {
                    ChangeLeader(itr->guid);
                    break;
                }
            }
        }

        SendUpdate();

        if (isLFGGroup() && GetMembersCount() == 1)
        {
            Player* l_Leader = ObjectAccessor::FindPlayer(GetLeaderGUID());
            const LFGDungeonEntry * l_Dungeon = sLFGDungeonStore.LookupEntry(sLFGMgr->GetDungeon(GetGUID()));

            if ((l_Leader && l_Dungeon && l_Leader->isAlive() && l_Leader->GetMapId() != uint32(l_Dungeon->map)) || !l_Dungeon)
            {
                Disband();
                return false;
            }
        }

        if ((!sLFGListMgr->IsGroupQueued(this) || !m_memberMgr.getSize()) && m_memberMgr.getSize() < ((isLFGGroup() || isBGGroup()) ? 1u : 2u))
            Disband();

        return true;
    }
    /// If group size before player removal <= 2 then disband it
    else
    {
        /// Don't display "You have been removed from group" if player removes himself
        Disband(p_Method == RemoveMethod::GROUP_REMOVEMETHOD_LEAVE);
        return false;
    }

    return true;
}

void Group::ChangeLeader(uint64 newLeaderGuid)
{
    member_witerator slot = _getMemberWSlot(newLeaderGuid);

    if (slot == m_memberSlots.end())
        return;

    Player* newLeader = ObjectAccessor::FindPlayer(slot->guid);

    // Don't allow switching leader to offline players
    if (!newLeader)
        return;

    sScriptMgr->OnGroupChangeLeader(this, m_leaderGuid, newLeaderGuid);

    if (!isBGGroup() && !isBFGroup())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        // Remove the groups permanent instance bindings
        for (uint8 i = 0; i < Difficulty::MaxDifficulties; ++i)
        {
            for (BoundInstancesMap::iterator itr = m_boundInstances[i].begin(); itr != m_boundInstances[i].end();)
            {
                // Do not unbind saves of instances that already have map created (a newLeader entered)
                // forcing a new instance with another leader requires group disbanding (confirmed on retail)
                if (itr->second.perm && !sMapMgr->FindMap(itr->first, itr->second.save->GetInstanceId()))
                {
                    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_PERM_BINDING);
                    stmt->setUInt32(0, m_dbStoreId);
                    stmt->setUInt32(1, itr->second.save->GetInstanceId());
                    trans->Append(stmt);

                    itr->second.save->RemoveGroup(this);
                    m_boundInstances[i].erase(itr++);
                }
                else
                    ++itr;
            }
        }

        // Copy the permanent binds from the new leader to the group
        Player::ConvertInstancesToGroup(newLeader, this, true);

        // Update the group leader
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_LEADER);

        stmt->setUInt32(0, newLeader->GetGUIDLow());
        stmt->setUInt32(1, m_dbStoreId);

        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);
    }

    Player* oldLeader = ObjectAccessor::FindPlayer(m_leaderGuid);

    if (oldLeader)
        oldLeader->RemoveFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);

    m_leaderGuid = newLeader->GetGUID();
    m_leaderName = newLeader->GetName();

    newLeader->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GROUP_LEADER);

    ToggleGroupMemberFlag(slot, MEMBER_FLAG_ASSISTANT, false);

    uint8 l_PartyIndex = GetPartyIndex();
    std::string l_Name = slot->name;

    WorldPacket data(SMSG_GROUP_NEW_LEADER);
    data << uint8(l_PartyIndex);
    data.WriteBits(l_Name.length(), 6);
    data.FlushBits();
    data.WriteString(l_Name);

    BroadcastPacket(&data, true);
}

void Group::Disband(bool hideDestroy /* = false */)
{
    sScriptMgr->OnGroupDisband(this);
    sLFGListMgr->Remove(GetLowGUID(), nullptr, false);

    Player* player;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        player = HashMapHolder<Player>::Find(citr->guid);
        if (!player)
            continue;

        //we cannot call _removeMember because it would invalidate member iterator
        //if we are removing player from battleground raid
        if (isBGGroup() || isBFGroup())
        {
            if (player->IsInWorld())
                player->RemoveFromBattlegroundOrBattlefieldRaid();
        }
        else
        {
            //we can remove player who is in battleground from his original group
            if (player->GetOriginalGroup() == this)
                player->SetOriginalGroup(NULL);
            else
                player->SetGroup(NULL);
        }

        // quest related GO state dependent from raid membership
        if (isRaidGroup() && player->IsInWorld())
            player->UpdateForQuestWorldObjects();

        if (!player->GetSession())
            continue;

        WorldPacket l_Data;
        if (!hideDestroy)
        {
            l_Data.Initialize(SMSG_GROUP_DESTROYED, 0);
            player->GetSession()->SendPacket(&l_Data);
        }

        //we already removed player from group and in player->GetGroup() is his original group, send update
        if (Group* group = player->GetGroup())
        {
            group->SendUpdate();
        }
        else
        {
            uint64 l_GroupGUID = GetGUID();
            uint64 l_LeaderGUID = GetLeaderGUID();

            uint32 l_MemberCount = 0;

            bool l_HasJamCliPartyLFGInfo = isLFGGroup(); ///< l_HasJamCliPartyLFGInfo is never read 01/18/16

            l_Data.Initialize(SMSG_PARTY_UPDATE, 200);
            l_Data << uint8(GetPartyFlags());
            l_Data << uint8(GetPartyIndex());
            l_Data << uint8(GetPartyType());
            l_Data << int32(-1);
            l_Data.appendPackGUID(l_GroupGUID);
            l_Data << uint32(m_UpdateCount++);
            l_Data.appendPackGUID(l_LeaderGUID);
            l_Data << uint32(l_MemberCount);

            l_Data.WriteBit(false);
            l_Data.WriteBit(false);
            l_Data.WriteBit(false);
            l_Data.FlushBits();

            player->GetSession()->SendPacket(&l_Data);
        }

        _homebindIfInstance(player);
    }

    RollId.clear();
    m_memberSlots.clear();

    RemoveAllInvites();

    if (!isBGGroup() && !isBFGroup())
    {
        SQLTransaction trans = CharacterDatabase.BeginTransaction();

        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_MEMBER_ALL);
        stmt->setUInt32(0, m_dbStoreId);
        trans->Append(stmt);

        CharacterDatabase.CommitTransaction(trans);

        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, false, false, NULL);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, false, NULL);
        ResetInstances(INSTANCE_RESET_GROUP_DISBAND, true, true, NULL);

        stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_LFG_DATA);
        stmt->setUInt32(0, m_dbStoreId);
        CharacterDatabase.Execute(stmt);

        sGroupMgr->FreeGroupDbStoreId(this);
    }

    sGroupMgr->RemoveGroup(this);
    delete this;
}

/*********************************************************/
/***                   LOOT SYSTEM                     ***/
/*********************************************************/

void Group::SendLootStartRoll(uint32 p_CountDown, uint32 p_MapID, Roll const& p_Roll)
{
    WorldPacket l_Data(SMSG_LOOT_START_ROLL, 200);
    l_Data.appendPackGUID(p_Roll.lootedGUID);
    l_Data << uint32(p_MapID);                              ///< mapid

    l_Data.WriteBits(LOOT_ITEM_TYPE_ITEM, 2);               ///< Type
    l_Data.WriteBits(LOOT_SLOT_TYPE_ALLOW_LOOT, 3);         ///< Ui Type
    l_Data.WriteBit(true);                                  ///< Can Trade To Tap List
    l_Data.FlushBits();
    l_Data << uint32(p_Roll.itemCount);
    l_Data << uint8(LOOT_LIST_ITEM);
    l_Data << uint8(p_Roll.itemSlot);

    Item::BuildDynamicItemDatas(l_Data, p_Roll.itemid, p_Roll.m_ItemBonuses);

    l_Data << uint32(p_CountDown);                          ///< the countdown time to choose "need" or "greed"
    l_Data << uint8(p_Roll.rollVoteMask);                   ///< roll type mask
    l_Data << uint8(p_Roll.totalPlayersRolling);            ///< maybe the number of players rolling for it???

    for (Roll::PlayerVote::const_iterator itr=p_Roll.playerVote.begin(); itr != p_Roll.playerVote.end(); ++itr)
    {
        Player * l_Player = ObjectAccessor::FindPlayer(itr->first);
        if (!l_Player || !l_Player->GetSession())
            continue;

        if (itr->second == NOT_EMITED_YET)
            l_Player->GetSession()->SendPacket(&l_Data);
    }
}

void Group::SendLootStartRollToPlayer(uint32 p_CountDown, uint32 p_MapID, Player* p_Player, bool /*p_CanNeed*/, Roll const& p_Roll) ///< p_CanNeed is unused
{
    if (!p_Player || !p_Player->GetSession())
        return;

    WorldPacket l_Data(SMSG_LOOT_START_ROLL, 200);
    l_Data.appendPackGUID(p_Roll.lootedGUID);
    l_Data << uint32(p_MapID);                              ///< mapid

    l_Data.WriteBits(LOOT_ITEM_TYPE_ITEM, 2);               ///< Type
    l_Data.WriteBits(LOOT_SLOT_TYPE_OWNER, 3);              ///< Ui Type
    l_Data.WriteBit(true);                                  ///< Can Trade To Tap List
    l_Data.FlushBits();
    l_Data << uint32(p_Roll.itemCount);
    l_Data << uint8(LOOT_LIST_ITEM);
    l_Data << uint8(p_Roll.itemSlot);

    Item::BuildDynamicItemDatas(l_Data, p_Roll.itemid, p_Roll.m_ItemBonuses);

    l_Data << uint32(p_CountDown);                          ///< the countdown time to choose "need" or "greed"
    l_Data << uint8(p_Roll.rollVoteMask);                   ///< roll type mask
    l_Data << uint8(p_Roll.totalPlayersRolling);            ///< maybe the number of players rolling for it???

    p_Player->GetSession()->SendPacket(&l_Data);
}

void Group::SendLootRoll(uint64 /*p_TargetGUID*/, uint64 targetGuid, uint8 p_RollNumber, uint8 rollType, Roll const& p_Roll) ///< p_TargetGUID is unused
{
    WorldPacket l_Data(SMSG_LOOT_ROLL, 200);
    l_Data.appendPackGUID(p_Roll.lootedGUID);
    l_Data.appendPackGUID(targetGuid);

    l_Data.WriteBits(LOOT_ITEM_TYPE_ITEM, 2);               ///< Type
    l_Data.WriteBits(LOOT_SLOT_TYPE_ALLOW_LOOT, 3);         ///< Ui Type
    l_Data.WriteBit(true);                                  ///< Can Trade To Tap List
    l_Data.FlushBits();
    l_Data << uint32(p_Roll.itemCount);
    l_Data << uint8(LOOT_LIST_ITEM);
    l_Data << uint8(p_Roll.itemSlot);

    Item::BuildDynamicItemDatas(l_Data, p_Roll.itemid, p_Roll.m_ItemBonuses);

    l_Data << uint32(p_RollNumber);                         ///< 0: "Need for: [item name]" > 127: "you passed on: [item name]"      Roll number
    l_Data << uint8(rollType);                              ///< 0: "Need for: [item name]" 0: "You have selected need for [item name] 1: need roll 2: greed roll
    l_Data.WriteBit(false);                                 ///< 1: "You automatically passed on: %s because you cannot loot that item." - Possibly used in need befor greed
    l_Data.FlushBits();

    for (Roll::PlayerVote::const_iterator l_It = p_Roll.playerVote.begin(); l_It != p_Roll.playerVote.end(); ++l_It)
    {
        Player* l_Player = ObjectAccessor::FindPlayer(l_It->first);
        if (!l_Player || !l_Player->GetSession())
            continue;

        if (l_It->second != NOT_VALID)
            l_Player->GetSession()->SendPacket(&l_Data);
    }
}

void Group::SendLootRollWon(uint64 /*p_SourceGUID*/, uint64 p_TargetGUID, uint8 p_RollNumber, uint8 rollType, Roll const& p_Roll)
{
    WorldPacket l_Data(SMSG_LOOT_ROLL_WON, 200);

    l_Data.appendPackGUID(p_Roll.lootedGUID);

    l_Data.WriteBits(LOOT_ITEM_TYPE_ITEM, 2);               ///< Type
    l_Data.WriteBits(LOOT_SLOT_TYPE_ALLOW_LOOT, 3);         ///< Ui Type
    l_Data.WriteBit(true);                                  ///< Can Trade To Tap List
    l_Data.FlushBits();
    l_Data << uint32(p_Roll.itemCount);
    l_Data << uint8(LOOT_LIST_ITEM);
    l_Data << uint8(p_Roll.itemSlot);

    Item::BuildDynamicItemDatas(l_Data, p_Roll.itemid, p_Roll.m_ItemBonuses);

    l_Data.appendPackGUID(p_TargetGUID);

    l_Data << uint32(p_RollNumber);                         ///< rollnumber realted to SMSG_LOOT_ROLL
    l_Data << uint8(rollType);                              ///< rollType related to SMSG_LOOT_ROLL

    for (Roll::PlayerVote::const_iterator l_It = p_Roll.playerVote.begin(); l_It != p_Roll.playerVote.end(); ++l_It)
    {
        Player* l_Player = ObjectAccessor::FindPlayer(l_It->first);
        if (!l_Player || !l_Player->GetSession())
            continue;

        if (l_It->second != NOT_VALID)
            l_Player->GetSession()->SendPacket(&l_Data);
    }
}

void Group::SendLootAllPassed(Roll const& p_Roll)
{
    WorldPacket l_Data(SMSG_LOOT_ALL_PASSED, 200);
    l_Data.appendPackGUID(p_Roll.lootedGUID);

    l_Data.WriteBits(LOOT_ITEM_TYPE_ITEM, 2);               ///< Type
    l_Data.WriteBits(LOOT_SLOT_TYPE_ALLOW_LOOT, 3);         ///< Ui Type
    l_Data.WriteBit(true);                                  ///< Can Trade To Tap List
    l_Data.FlushBits();
    l_Data << uint32(p_Roll.itemCount);
    l_Data << uint8(LOOT_LIST_ITEM);
    l_Data << uint8(p_Roll.itemSlot);

    Item::BuildDynamicItemDatas(l_Data, p_Roll.itemid, p_Roll.m_ItemBonuses);

    for (Roll::PlayerVote::const_iterator l_It = p_Roll.playerVote.begin(); l_It != p_Roll.playerVote.end(); ++l_It)
    {
        Player * p_Player = ObjectAccessor::FindPlayer(l_It->first);

        if (!p_Player || !p_Player->GetSession())
            continue;

        if (l_It->second != NOT_VALID)
            p_Player->GetSession()->SendPacket(&l_Data);
    }
}

// notify group members which player is the allowed looter for the given creature
void Group::SendLooter(Creature* creature, Player* p_GroupLooter)
{
    ASSERT(creature);

    WorldPacket l_Data(SMSG_LOOT_LIST);

    ObjectGuid l_Master = p_GroupLooter ? p_GroupLooter->GetGUID() : 0;

    l_Data.appendPackGUID(creature->GetGUID());   ///< Owner
    l_Data.WriteBit(l_Master);                    ///< Master
    l_Data.WriteBit(false);                       ///< RoundRobinWinner
    l_Data.FlushBits();

    if (l_Master)
        l_Data.appendPackGUID(l_Master);

    BroadcastPacket(&l_Data, false);
}

void Group::GroupLoot(Loot* loot, WorldObject* pLootedObject)
{
    std::vector<LootItem>::iterator i;
    ItemTemplate const* item;
    uint8 itemSlot = 0;

    for (i = loot->Items.begin(); i != loot->Items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //sLog->outDebug(LOG_FILTER_GENERAL, "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
            Roll* r = new Roll(newitemGUID, *i);
            r->lootedGUID = pLootedObject->GetGUID();

            //a vector is filled with only near party members
            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* member = itr->getSource();
                if (!member || !member->GetSession())
                    continue;
                if (i->AllowedForPlayer(member))
                {
                    if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                    {
                        r->totalPlayersRolling++;

                        if (member->GetPassOnGroupLoot())
                        {
                            r->playerVote[member->GetGUID()] = PASS;
                            r->totalPass++;
                            // can't broadcast the pass now. need to wait until all rolling players are known.
                        }
                        else
                            r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
                    }
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                loot->Items[itemSlot].is_blocked = true;

                // If there is any "auto pass", broadcast the pass now.
                if (r->totalPass)
                {
                    for (Roll::PlayerVote::const_iterator itr=r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                    {
                        Player* p = ObjectAccessor::FindPlayer(itr->first);
                        if (!p || !p->GetSession())
                            continue;

                        if (itr->second == PASS)
                            SendLootRoll(newitemGUID, p->GetGUID(), 128, ROLL_PASS, *r);
                    }
                }

                SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

                RollId.push_back(r);

                if (Creature* creature = pLootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = pLootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }

    for (i = loot->QuestItems.begin(); i != loot->QuestItems.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        if (!item)
        {
            //sLog->outDebug(LOG_FILTER_GENERAL, "Group::GroupLoot: missing item prototype for item with id: %d", i->itemid);
            continue;
        }

        uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
        Roll* r = new Roll(newitemGUID, *i);
        r->lootedGUID = pLootedObject->GetGUID();

        //a vector is filled with only near party members
        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* member = itr->getSource();
            if (!member || !member->GetSession())
                continue;

            if (i->AllowedForPlayer(member))
            {
                if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    r->totalPlayersRolling++;
                    r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
                }
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;

            loot->QuestItems[itemSlot - loot->Items.size()].is_blocked = true;

            SendLootStartRoll(60000, pLootedObject->GetMapId(), *r);

            RollId.push_back(r);

            if (Creature* creature = pLootedObject->ToCreature())
            {
                creature->m_groupLootTimer = 60000;
                creature->lootingGroupLowGUID = GetLowGUID();
            }
            else if (GameObject* go = pLootedObject->ToGameObject())
            {
                go->m_groupLootTimer = 60000;
                go->lootingGroupLowGUID = GetLowGUID();
            }
        }
        else
            delete r;
    }
}

void Group::NeedBeforeGreed(Loot* loot, WorldObject* lootedObject)
{
    ItemTemplate const* item;
    uint8 itemSlot = 0;
    for (std::vector<LootItem>::iterator i = loot->Items.begin(); i != loot->Items.end(); ++i, ++itemSlot)
    {
        if (i->freeforall)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);

        //roll for over-threshold item if it's one-player loot
        if (item->Quality >= uint32(m_lootThreshold))
        {
            uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
            Roll* r = new Roll(newitemGUID, *i);
            r->lootedGUID = lootedObject->GetGUID();

            for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
            {
                Player* playerToRoll = itr->getSource();
                if (!playerToRoll || !playerToRoll->GetSession())
                    continue;

                bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
                if (allowedForPlayer && playerToRoll->IsWithinDistInMap(lootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    r->totalPlayersRolling++;
                    if (playerToRoll->GetPassOnGroupLoot())
                    {
                        r->playerVote[playerToRoll->GetGUID()] = PASS;
                        r->totalPass++;
                        // can't broadcast the pass now. need to wait until all rolling players are known.
                    }
                    else
                        r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
                }
            }

            if (r->totalPlayersRolling > 0)
            {
                r->setLoot(loot);
                r->itemSlot = itemSlot;
                if (item->DisenchantID && m_maxEnchantingLevel >= item->RequiredDisenchantSkill)
                    r->rollVoteMask |= ROLL_FLAG_TYPE_DISENCHANT;

                if (item->Flags2 & ITEM_FLAG2_NEED_ROLL_DISABLED)
                    r->rollVoteMask &= ~ROLL_FLAG_TYPE_NEED;

                loot->Items[itemSlot].is_blocked = true;

                //Broadcast Pass and Send Rollstart
                for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
                {
                    Player* p = ObjectAccessor::FindPlayer(itr->first);
                    if (!p || !p->GetSession())
                        continue;

                    if (itr->second == PASS)
                        SendLootRoll(newitemGUID, p->GetGUID(), 128, ROLL_PASS, *r);
                    else
                        SendLootStartRollToPlayer(60000, lootedObject->GetMapId(), p, p->CanRollForItemInLFG(item, lootedObject) == EQUIP_ERR_OK, *r);
                }

                RollId.push_back(r);

                if (Creature* creature = lootedObject->ToCreature())
                {
                    creature->m_groupLootTimer = 60000;
                    creature->lootingGroupLowGUID = GetLowGUID();
                }
                else if (GameObject* go = lootedObject->ToGameObject())
                {
                    go->m_groupLootTimer = 60000;
                    go->lootingGroupLowGUID = GetLowGUID();
                }
            }
            else
                delete r;
        }
        else
            i->is_underthreshold = true;
    }

    for (std::vector<LootItem>::iterator i = loot->QuestItems.begin(); i != loot->QuestItems.end(); ++i, ++itemSlot)
    {
        if (!i->follow_loot_rules)
            continue;

        item = sObjectMgr->GetItemTemplate(i->itemid);
        uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
        Roll* r = new Roll(newitemGUID, *i);
        r->lootedGUID = lootedObject->GetGUID();

        for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
        {
            Player* playerToRoll = itr->getSource();
            if (!playerToRoll || !playerToRoll->GetSession())
                continue;

            bool allowedForPlayer = i->AllowedForPlayer(playerToRoll);
            if (allowedForPlayer && playerToRoll->IsWithinDistInMap(lootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                r->totalPlayersRolling++;
                r->playerVote[playerToRoll->GetGUID()] = NOT_EMITED_YET;
            }
        }

        if (r->totalPlayersRolling > 0)
        {
            r->setLoot(loot);
            r->itemSlot = itemSlot;

            loot->QuestItems[itemSlot - loot->Items.size()].is_blocked = true;

            //Broadcast Pass and Send Rollstart
            for (Roll::PlayerVote::const_iterator itr = r->playerVote.begin(); itr != r->playerVote.end(); ++itr)
            {
                Player* p = ObjectAccessor::FindPlayer(itr->first);
                if (!p || !p->GetSession())
                    continue;

                if (itr->second == PASS)
                    SendLootRoll(newitemGUID, p->GetGUID(), 128, ROLL_PASS, *r);
                else
                    SendLootStartRollToPlayer(60000, lootedObject->GetMapId(), p, p->CanRollForItemInLFG(item, lootedObject) == EQUIP_ERR_OK, *r);
            }

            RollId.push_back(r);

            if (Creature* creature = lootedObject->ToCreature())
            {
                creature->m_groupLootTimer = 60000;
                creature->lootingGroupLowGUID = GetLowGUID();
            }
            else if (GameObject* go = lootedObject->ToGameObject())
            {
                go->m_groupLootTimer = 60000;
                go->lootingGroupLowGUID = GetLowGUID();
            }
        }
        else
            delete r;
    }
}

void Group::MasterLoot(Loot* loot, WorldObject* p_LootedObject)
{
    for (auto& l_Iter : loot->Items)
    {
        if (l_Iter.freeforall)
            continue;

        l_Iter.is_blocked = !l_Iter.is_underthreshold;
    }

    for (auto& l_Iter : loot->QuestItems)
    {
        if (!l_Iter.follow_loot_rules)
            continue;

        l_Iter.is_blocked = !l_Iter.is_underthreshold;
    }

    uint32 l_Count = 0;

    for (GroupReference* l_It = GetFirstMember(); l_It != nullptr; l_It = l_It->next())
    {
        Player* l_Looter = l_It->getSource();

        if (!l_Looter->IsInWorld())
            continue;

        if (l_Looter->IsAtGroupRewardDistance(p_LootedObject))
            ++l_Count;
    }

    uint64 l_LootedGUID = p_LootedObject->GetGUID();
    sObjectMgr->setLootViewGUID(l_LootedGUID, p_LootedObject->GetGUID());

    WorldPacket l_Data(Opcodes::SMSG_MASTER_LOOT_CANDIDATE_LIST);
    l_Data.appendPackGUID(MAKE_NEW_GUID(p_LootedObject->GetGUIDLow(), 0, HIGHGUID_LOOT));
    l_Data << uint32(l_Count);

    for (GroupReference* l_It = GetFirstMember(); l_It != nullptr; l_It = l_It->next())
    {
        Player* l_Looter = l_It->getSource();

        if (!l_Looter->IsInWorld())
            continue;

        if (l_Looter->IsAtGroupRewardDistance(p_LootedObject))
            l_Data.appendPackGUID(l_Looter->GetGUID());
    }

    for (GroupReference* l_It = GetFirstMember(); l_It != nullptr; l_It = l_It->next())
    {
        Player* l_Looter = l_It->getSource();
        if (l_Looter->GetGUID() == GetLooterGuid())
            l_Looter->GetSession()->SendPacket(&l_Data);
    }
}

void Group::DoRollForAllMembers(ObjectGuid guid, uint8 slot, uint32 mapid, Loot* loot, LootItem& item, Player* player)
{
    uint64 newitemGUID = MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_ITEM), 0, HIGHGUID_ITEM);
    Roll* r = new Roll(newitemGUID, item);
    r->lootedGUID = guid;
    WorldObject* pLootedObject = nullptr;

    if (IS_CRE_OR_VEH_GUID(guid))
        pLootedObject = player->GetMap()->GetCreature(guid);
    else if (IS_GAMEOBJECT_GUID(guid))
        pLootedObject = player->GetMap()->GetGameObject(guid);

    if (!pLootedObject)
        return;

    //a vector is filled with only near party members
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* member = itr->getSource();
        if (!member || !member->GetSession())
            continue;

        if (item.AllowedForPlayer(member))
        {
            if (member->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                r->totalPlayersRolling++;
                r->playerVote[member->GetGUID()] = NOT_EMITED_YET;
            }
        }
    }

    if (r->totalPlayersRolling > 0)
    {
        r->setLoot(loot);
        r->itemSlot = slot;

        RollId.push_back(r);
    }

    SendLootStartRoll(180000, mapid, *r);
}

void Group::CountRollVote(uint64 playerGUID, uint8 slot, uint8 Choice)
{
    Rolls::iterator rollI = GetRoll(slot);
    if (rollI == RollId.end())
        return;
    Roll* roll = *rollI;

    Roll::PlayerVote::iterator itr = roll->playerVote.find(playerGUID);
    // this condition means that player joins to the party after roll begins
    if (itr == roll->playerVote.end())
        return;

    if (roll->getLoot())
        if (roll->getLoot()->Items.empty())
            return;

    switch (Choice)
    {
    case ROLL_PASS:                                     // Player choose pass
        SendLootRoll(0, playerGUID, 0, ROLL_PASS, *roll);
        ++roll->totalPass;
        itr->second = PASS;
        break;
    case ROLL_NEED:                                     // player choose Need
        SendLootRoll(0, playerGUID, 0, ROLL_NEED, *roll);
        ++roll->totalNeed;
        itr->second = NEED;
        break;
    case ROLL_GREED:                                    // player choose Greed
        SendLootRoll(0, playerGUID, 0, ROLL_GREED, *roll);
        ++roll->totalGreed;
        itr->second = GREED;
        break;
    case ROLL_DISENCHANT:                               // player choose Disenchant
        SendLootRoll(0, playerGUID, 0, ROLL_DISENCHANT, *roll);
        ++roll->totalGreed;
        itr->second = DISENCHANT;
        break;
    }

    if (roll->totalPass + roll->totalNeed + roll->totalGreed >= roll->totalPlayersRolling)
        CountTheRoll(rollI);
}

//called when roll timer expires
void Group::EndRoll(Loot* pLoot)
{
    for (Rolls::iterator itr = RollId.begin(); itr != RollId.end();)
    {
        if ((*itr)->getLoot() == pLoot) {
            CountTheRoll(itr);           //i don't have to edit player votes, who didn't vote ... he will pass
            itr = RollId.begin();
        }
        else
            ++itr;
    }
}

void Group::CountTheRoll(Rolls::iterator rollI)
{
    Roll* roll = *rollI;
    if (!roll->isValid())                                   // is loot already deleted ?
    {
        RollId.erase(rollI);
        delete roll;
        return;
    }

    //end of the roll
    if (roll->totalNeed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            uint64 maxguid  = (*roll->playerVote.begin()).first;
            Player* player;

            for (Roll::PlayerVote::const_iterator itr=roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != NEED)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(0, itr->first, randomN, ROLL_NEED, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                }
            }
            SendLootRollWon(0, maxguid, maxresul, ROLL_NEED, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED_ON_LOOT, roll->itemid, maxresul);

                ItemPosCountVec dest;
                LootItem* item = &(roll->itemSlot >= roll->getLoot()->Items.size() ? roll->getLoot()->QuestItems[roll->itemSlot - roll->getLoot()->Items.size()] : roll->getLoot()->Items[roll->itemSlot]);
                InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                if (msg == EQUIP_ERR_OK)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->UnlootedCount--;
                    AllowedLooterSet looters = item->GetAllowedLooters();

                    if (Item* l_Item = player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, looters))
                        l_Item->AddItemBonuses(item->itemBonuses);
                }
                else
                {
                    item->is_blocked = false;
                    player->SendEquipError(msg, NULL, NULL, roll->itemid);
                }
            }
        }
    }
    else if (roll->totalGreed > 0)
    {
        if (!roll->playerVote.empty())
        {
            uint8 maxresul = 0;
            uint64 maxguid = (*roll->playerVote.begin()).first;
            Player* player;
            RollVote rollvote = NOT_VALID;

            Roll::PlayerVote::iterator itr;
            for (itr = roll->playerVote.begin(); itr != roll->playerVote.end(); ++itr)
            {
                if (itr->second != GREED && itr->second != DISENCHANT)
                    continue;

                uint8 randomN = urand(1, 100);
                SendLootRoll(0, itr->first, randomN, itr->second, *roll);
                if (maxresul < randomN)
                {
                    maxguid  = itr->first;
                    maxresul = randomN;
                    rollvote = itr->second;
                }
            }
            SendLootRollWon(0, maxguid, maxresul, rollvote, *roll);
            player = ObjectAccessor::FindPlayer(maxguid);

            if (player && player->GetSession())
            {
                player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED_ON_LOOT, roll->itemid, maxresul);

                LootItem* item = &(roll->itemSlot >= roll->getLoot()->Items.size() ? roll->getLoot()->QuestItems[roll->itemSlot - roll->getLoot()->Items.size()] : roll->getLoot()->Items[roll->itemSlot]);

                if (rollvote == GREED)
                {
                    ItemPosCountVec dest;
                    InventoryResult msg = player->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, roll->itemid, item->count);
                    if (msg == EQUIP_ERR_OK)
                    {
                        item->is_looted = true;
                        roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                        roll->getLoot()->UnlootedCount--;
                        AllowedLooterSet looters = item->GetAllowedLooters();
                        Item* l_Item = player->StoreNewItem(dest, roll->itemid, true, item->randomPropertyId, looters);
                        if (l_Item)
                            l_Item->AddItemBonuses(item->itemBonuses);
                    }
                    else
                    {
                        item->is_blocked = false;
                        player->SendEquipError(msg, NULL, NULL, roll->itemid);
                    }
                }
                else if (rollvote == DISENCHANT)
                {
                    item->is_looted = true;
                    roll->getLoot()->NotifyItemRemoved(roll->itemSlot);
                    roll->getLoot()->UnlootedCount--;
                    ItemTemplate const* pProto = sObjectMgr->GetItemTemplate(roll->itemid);
                    player->AutoStoreLoot(pProto->DisenchantID, LootTemplates_Disenchant, true);
                    player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_CAST_SPELL, 13262); // Disenchant
                }
            }
        }
    }
    else
    {
        SendLootAllPassed(*roll);

        // remove is_blocked so that the item is lootable by all players
        LootItem* item = &(roll->itemSlot >= roll->getLoot()->Items.size() ? roll->getLoot()->QuestItems[roll->itemSlot - roll->getLoot()->Items.size()] : roll->getLoot()->Items[roll->itemSlot]);
        if (item)
            item->is_blocked = false;
    }

    RollId.erase(rollI);
    delete roll;
}

void Group::SetTargetIcon(uint8 p_Symbol, uint64 p_WhoGuid, uint64 p_TargetGuid, uint8 p_PartyIndex)
{
    if (p_Symbol >= TARGETICONCOUNT)
        return;

    // Clean other icons
    if (p_TargetGuid != 0)
    {
        for (int i = 0; i < TARGETICONCOUNT; ++i)
        {
            if (m_targetIcons[i] == p_TargetGuid)
                SetTargetIcon(i, 0, 0, p_PartyIndex);
        }
    }

    m_targetIcons[p_Symbol] = p_TargetGuid;

    uint64 l_ChangedBy = p_WhoGuid;
    uint64 l_Target     = p_TargetGuid;
    uint8  l_PartyIndex = p_PartyIndex;
    uint8  l_Symbol     = p_Symbol;

    WorldPacket l_Data(SMSG_SEND_RAID_TARGET_UPDATE_SINGLE, (1 + 16 + 1 + 16));
    l_Data << uint8(l_Symbol);
    l_Data << uint8(l_PartyIndex);
    l_Data.appendPackGUID(l_Target);
    l_Data.appendPackGUID(l_ChangedBy);

    BroadcastPacket(&l_Data, true);
}

void Group::SendTargetIconList(WorldSession* p_Session, uint8 p_PartyIndex)
{
    if (!p_Session)
        return;

    uint8 l_PartyIndex   = p_PartyIndex;
    uint32 l_TargetsSize = 0;

    for (uint8 l_I = 0; l_I < TARGETICONCOUNT; ++l_I)
    {
        if (m_targetIcons[l_I] == 0)
            continue;

        ++l_TargetsSize;
    }

    WorldPacket l_Data(SMSG_SEND_RAID_TARGET_UPDATE_ALL, 4 + 1 + (16 + 1) * TARGETICONCOUNT);
    l_Data << uint8(l_PartyIndex);
    l_Data << uint32(l_TargetsSize);

    for (uint8 l_I = 0; l_I < TARGETICONCOUNT; ++l_I)
    {
        uint64 l_Target = m_targetIcons[l_I];
        uint8  l_Symbol = l_I;
        if (l_Target == 0)
            continue;

        l_Data.appendPackGUID(l_Target);
        l_Data << uint8(l_Symbol);
    }

    p_Session->SendPacket(&l_Data);
}

void Group::SendUpdate()
{
    for (member_witerator witr = m_memberSlots.begin(); witr != m_memberSlots.end(); ++witr)
        SendUpdateToPlayer(witr->guid, &(*witr));
}

void Group::SendUpdateToPlayer(uint64 playerGUID, MemberSlot* slot)
{
    Player* player = ObjectAccessor::FindPlayer(playerGUID);

    if (!player || !player->GetSession() || player->GetGroup() != this)
        return;

    // if MemberSlot wasn't provided
    if (!slot)
    {
        member_witerator witr = _getMemberWSlot(playerGUID);

        if (witr == m_memberSlots.end()) // if there is no MemberSlot for such a player
            return;

        slot = &(*witr);
    }

    uint64 l_GroupGUID  = GetGUID();
    uint64 l_LeaderGUID = GetLeaderGUID();
    uint64 l_LooterGUID = GetLooterGuid();

    bool l_HasJamCliPartyLFGInfo            = isLFGGroup();
    bool l_HasJamCliPartyDifficultySettings = !isBGGroup();
    bool l_HasJamCliPartyLootSettings       = !isBGGroup();

    uint32 l_MemberCount = GetMembersCount();

    int l_MyPosition = -1;
    int l_I = 0;
    for (member_citerator l_MemberIT = m_memberSlots.begin(); l_MemberIT != m_memberSlots.end(); ++l_MemberIT)
    {
        if (l_MemberIT->guid == slot->guid)
        {
            l_MyPosition = l_I;
            break;
        }
        l_I++;
    }

    WorldPacket l_Data(SMSG_PARTY_UPDATE, 1 * 1024);
    l_Data << uint8(GetPartyFlags());
    l_Data << uint8(GetPartyIndex());
    l_Data << uint8(GetPartyType());
    l_Data << int32(l_MyPosition);
    l_Data.appendPackGUID(l_GroupGUID);
    l_Data << uint32(m_UpdateCount++);
    l_Data.appendPackGUID(l_LeaderGUID);
    l_Data << uint32(l_MemberCount);

    for (member_citerator l_MemberIT = m_memberSlots.begin(); l_MemberIT != m_memberSlots.end(); ++l_MemberIT)
    {
        Player * l_Member      = HashMapHolder<Player>::Find(l_MemberIT->guid);
        uint8    l_OnlineState = (l_Member) ? MEMBER_STATUS_ONLINE : MEMBER_STATUS_OFFLINE;

        l_OnlineState = l_OnlineState | ((isBGGroup() || isBFGroup()) ? MEMBER_STATUS_PVP : 0);

        l_Data.WriteBits(l_MemberIT->name.length(), 6);
        l_Data.FlushBits();
        l_Data.appendPackGUID(l_MemberIT->guid);
        l_Data << uint8(l_OnlineState);
        l_Data << uint8(l_MemberIT->group);
        l_Data << uint8(l_MemberIT->flags);
        l_Data << uint8(l_MemberIT->roles);
        l_Data << uint8(0);                     ///< @todo class!
        l_Data.WriteString(l_MemberIT->name);
    }

    l_Data.WriteBit(l_HasJamCliPartyLFGInfo);
    l_Data.WriteBit(l_HasJamCliPartyLootSettings);
    l_Data.WriteBit(l_HasJamCliPartyDifficultySettings);
    l_Data.FlushBits();

    if (l_HasJamCliPartyLFGInfo)
    {
        l_Data << uint8(0);                                       ///< MyLfgFlags
        l_Data << uint32(sLFGMgr->GetDungeon(m_guid, false));     ///< LfgSlot
        l_Data << uint32(0);                                      ///< MyLfgRandomSlot
        l_Data << uint8(0);                                       ///< MyLfgPartialClear
        l_Data << float(0.0f);                                    ///< MyLfgGearDiff
        l_Data << uint8(0);                                       ///< MyLfgStrangerCount
        l_Data << uint8(0);                                       ///< MyLfgKickVoteCount
        l_Data << uint8(0);                                       ///< LfgBootCount

        l_Data.WriteBit(false);                                   ///< LfgAborted
        l_Data.WriteBit(false);                                   ///< MyLfgFirstReward
        l_Data.FlushBits();
    }

    if (l_HasJamCliPartyLootSettings)
    {
        l_Data << uint8(m_lootMethod);
        l_Data.appendPackGUID(l_LooterGUID);
        l_Data << uint8(m_lootThreshold);
    }

    if (l_HasJamCliPartyDifficultySettings)
    {
        l_Data << uint32(GetDungeonDifficultyID());
        l_Data << uint32(GetRaidDifficultyID());
        l_Data << uint32(GetLegacyRaidDifficultyID());
    }

    player->GetSession()->SendPacket(&l_Data);
}

void Group::UpdatePlayerOutOfRange(Player* player)
{
    if (!player || !player->IsInWorld())
        return;

    WorldPacket data;
    player->GetSession()->BuildPartyMemberStatsChangedPacket(player, &data, player->GetGroupUpdateFlag());

    Player* member;
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        member = itr->getSource();
        if (member && !member->IsWithinDist(player, member->GetSightRange(), false))
            member->GetSession()->SendPacket(&data);
    }
}

void Group::BroadcastAddonMessagePacket(WorldPacket* packet, const std::string& prefix, bool ignorePlayersInBGRaid, int group, uint64 ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player || (ignore != 0 && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (WorldSession* session = player->GetSession())
            if (session && (group == -1 || itr->getSubGroup() == group))
                if (session->IsAddonRegistered(prefix))
                    session->SendPacket(packet);
    }
}

void Group::BroadcastPacket(WorldPacket* packet, bool ignorePlayersInBGRaid, int group, uint64 ignore)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player || (ignore != 0 && player->GetGUID() == ignore) || (ignorePlayersInBGRaid && player->GetGroup() != this))
            continue;

        if (player->GetSession() && (group == -1 || itr->getSubGroup() == group))
            player->GetSession()->SendPacket(packet);
    }
}

void Group::BroadcastReadyCheck(WorldPacket* packet)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (player && player->GetSession())
            if (IsLeader(player->GetGUID()) || IsAssistant(player->GetGUID()) || m_PartyFlags & PARTY_FLAG_EVERYONE_IS_ASSISTANT)
                player->GetSession()->SendPacket(packet);
    }
}

void Group::OfflineReadyCheck()
{
    for (member_citerator l_It = m_memberSlots.begin(); l_It != m_memberSlots.end(); ++l_It)
    {
        Player* l_Player = ObjectAccessor::FindPlayer(l_It->guid);

        if (!l_Player || !l_Player->GetSession())
        {
            bool l_IsIsReady = false;

            WorldPacket l_Response(SMSG_READY_CHECK_RESPONSE);

            l_Response.appendPackGUID(GetGUID());
            l_Response.appendPackGUID(l_It->guid);
            l_Response.WriteBit(l_IsIsReady);
            l_Response.FlushBits();

            BroadcastReadyCheck(&l_Response);

            m_readyCheckCount++;
        }
    }
}

bool Group::_setMembersGroup(uint64 guid, uint8 group)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return false;

    slot->group = group;

    SubGroupCounterIncrease(group);

    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt32(1, GUID_LOPART(guid));

        CharacterDatabase.Execute(stmt);
    }

    return true;
}

bool Group::SameSubGroup(Player const* member1, Player const* member2) const
{
    if (!member1 || !member2)
        return false;

    if (member1->GetGroup() != this || member2->GetGroup() != this)
        return false;
    else
        return member1->GetSubGroup() == member2->GetSubGroup();
}

// Allows setting sub groups both for online or offline members
void Group::ChangeMembersGroup(uint64 guid, uint8 group)
{
    // Only raid groups have sub groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Abort if the player is already in the target sub group
    uint8 prevSubGroup = GetMemberGroup(guid);
    if (prevSubGroup == group)
        return;

    // Update the player slot with the new sub group setting
    slot->group = group;

    // Increase the counter of the new sub group..
    SubGroupCounterIncrease(group);

    // ..and decrease the counter of the previous one
    SubGroupCounterDecrease(prevSubGroup);

    // Preserve new sub group in database for non-raid groups
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SUBGROUP);

        stmt->setUInt8(0, group);
        stmt->setUInt32(1, GUID_LOPART(guid));

        CharacterDatabase.Execute(stmt);
    }

    // In case the moved player is online, update the player object with the new sub group references
    if (Player* player = ObjectAccessor::FindPlayer(guid))
    {
        if (player->GetGroup() == this)
            player->GetGroupRef().setSubGroup(group);
        else
        {
            // If player is in BG raid, it is possible that he is also in normal raid - and that normal raid is stored in m_originalGroup reference
            prevSubGroup = player->GetOriginalSubGroup(); ///< prevSubGroup is never read 01/18/16
            player->GetOriginalGroupRef().setSubGroup(group);
        }
    }

    // Broadcast the changes to the group
    SendUpdate();
}

// Retrieve the next Round-Roubin player for the group
//
// No update done if loot method is Master or FFA.
//
// If the RR player is not yet set for the group, the first group member becomes the round-robin player.
// If the RR player is set, the next player in group becomes the round-robin player.
//
// If ifneed is true,
//      the current RR player is checked to be near the looted object.
//      if yes, no update done.
//      if not, he loses his turn.
void Group::UpdateLooterGuid(WorldObject* pLootedObject, bool ifneed)
{
    switch (GetLootMethod())
    {
        case MASTER_LOOT:
        case FREE_FOR_ALL:
            return;
        default:
            // round robin style looting applies for all low
            // quality items in each loot method except free for all and master loot
            break;
    }

    uint64 oldLooterGUID = GetLooterGuid();
    member_citerator guid_itr = _getMemberCSlot(oldLooterGUID);
    if (guid_itr != m_memberSlots.end())
    {
        if (ifneed)
        {
            // not update if only update if need and ok
            Player* looter = ObjectAccessor::FindPlayer(guid_itr->guid);
            if (looter && looter->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                return;
        }
        ++guid_itr;
    }

    // search next after current
    Player* pNewLooter = NULL;
    for (member_citerator itr = guid_itr; itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
            if (player->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
            {
                pNewLooter = player;
                break;
            }
    }

    if (!pNewLooter)
    {
        // search from start
        for (member_citerator itr = m_memberSlots.begin(); itr != guid_itr; ++itr)
        {
            if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
                if (player->IsWithinDistInMap(pLootedObject, sWorld->getFloatConfig(CONFIG_GROUP_XP_DISTANCE), false))
                {
                    pNewLooter = player;
                    break;
                }
        }
    }

    if (pNewLooter)
    {
        if (oldLooterGUID != pNewLooter->GetGUID())
        {
            SetLooterGuid(pNewLooter->GetGUID());
            SendUpdate();
        }
    }
    else
    {
        SetLooterGuid(0);
        SendUpdate();
    }
}

GroupJoinBattlegroundResult Group::CanJoinBattlegroundQueue(Battleground const* p_BgOrTemplate, MS::Battlegrounds::BattlegroundType::Type p_BgQueueTypeId, uint32 MinPlayerCount)
{
    // check if this group is LFG group
    if (isLFGGroup())
        return ERR_LFG_CANT_USE_BATTLEGROUND;

    BattlemasterListEntry const* bgEntry = sBattlemasterListStore.LookupEntry(p_BgOrTemplate->GetTypeID());
    if (!bgEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;            // shouldn't happen

    // check for min / max count
    uint32 memberscount = GetMembersCount();

    if (memberscount > bgEntry->RatedPlayers)                // no MinPlayerCount for battlegrounds
        return ERR_BATTLEGROUND_NONE;                        // ERR_GROUP_JOIN_BATTLEGROUND_TOO_MANY handled on client side

    // get a player as reference, to compare other players' stats to (arena team id, queue id based on level, etc.)
    Player* reference = GetFirstMember()->getSource();
    // no reference found, can't join this way
    if (!reference)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    MS::Battlegrounds::Bracket const* bracketEntry = MS::Battlegrounds::Brackets::FindForLevel(reference->getLevel());
    if (!bracketEntry)
        return ERR_BATTLEGROUND_JOIN_FAILED;

    uint32 team = reference->GetTeam();

    MS::Battlegrounds::BattlegroundType::Type bgQueueTypeIdRandom = MS::Battlegrounds::GetTypeFromId(BATTLEGROUND_RB, 0);

    // check every member of the group to be able to join
    memberscount = 0;
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next(), ++memberscount)
    {
        Player* member = itr->getSource();
        // offline member? don't let join
        if (!member)
            return ERR_BATTLEGROUND_JOIN_FAILED;
        // don't allow cross-faction join as group
        if (member->GetTeam() != team)
            return ERR_BATTLEGROUND_JOIN_TIMED_OUT;
        // not in the same battleground level bracket, don't let join
        MS::Battlegrounds::Bracket const* memberBracketEntry = MS::Battlegrounds::Brackets::FindForLevel(member->getLevel());
        if (memberBracketEntry != bracketEntry)
            return ERR_BATTLEGROUND_JOIN_RANGE_INDEX;
        // don't let join if someone from the group is already in that bg queue
        if (member->InBattlegroundQueueForBattlegroundQueueType(p_BgQueueTypeId))
            return ERR_BATTLEGROUND_JOIN_FAILED;            // not blizz-like
        // don't let join if someone from the group is in bg queue random
        if (member->InBattlegroundQueueForBattlegroundQueueType(bgQueueTypeIdRandom))
            return ERR_IN_RANDOM_BG;
        // don't let join to bg queue random if someone from the group is already in bg queue
        if (p_BgOrTemplate->GetTypeID() == BATTLEGROUND_RB && member->InBattlegroundQueue())
            return ERR_IN_NON_RANDOM_BG;
        // check for deserter debuff in case not arena queue
        if (p_BgOrTemplate->GetTypeID() != BATTLEGROUND_AA && !member->CanJoinToBattleground())
            return ERR_GROUP_JOIN_BATTLEGROUND_DESERTERS;
        // check if member can join any more battleground queues
        if (!member->HasFreeBattlegroundQueueId())
            return ERR_BATTLEGROUND_TOO_MANY_QUEUES;        // not blizz-like
        // check if someone in party is using dungeon system
        if (member->isUsingLfg())
            return ERR_LFG_CANT_USE_BATTLEGROUND;
        // check is someone in party is loading or teleporting
        if (member->GetSession()->PlayerLoading() || member->IsBeingTeleported())
            return ERR_BATTLEGROUND_JOIN_FAILED;
    }

    // only check for MinPlayerCount since MinPlayerCount == MaxPlayerCount for arenas...
    if (p_BgOrTemplate->isArena() && memberscount != MinPlayerCount)
        return ERR_ARENA_TEAM_PARTY_SIZE;

    return ERR_BATTLEGROUND_NONE;
}

//===================================================
//============== Roll ===============================
//===================================================

void Roll::targetObjectBuildLink()
{
    // called from link()
    getTarget()->addLootValidatorRef(this);
}

void Group::SetDungeonDifficultyID(Difficulty difficulty)
{
    m_dungeonDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_dungeonDifficulty));
        stmt->setUInt32(1, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetDungeonDifficultyID(difficulty);
        player->SendDungeonDifficulty();
    }

    SendUpdate();
}

void Group::SetRaidDifficultyID(Difficulty difficulty)
{
    m_raidDifficulty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_RAID_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_raidDifficulty));
        stmt->setUInt8(1, uint8(m_LegacyRaidDifficuty));
        stmt->setUInt32(2, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetRaidDifficultyID(difficulty);
        player->SendRaidDifficulty(false);
    }

    SendUpdate();
}
void Group::SetLegacyRaidDifficultyID(Difficulty difficulty)
{
    m_LegacyRaidDifficuty = difficulty;
    if (!isBGGroup() && !isBFGroup())
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_RAID_DIFFICULTY);

        stmt->setUInt8(0, uint8(m_raidDifficulty));
        stmt->setUInt8(1, uint8(m_LegacyRaidDifficuty));
        stmt->setUInt32(2, m_dbStoreId);

        CharacterDatabase.Execute(stmt);
    }

    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (!player->GetSession())
            continue;

        player->SetLegacyRaidDifficultyID(difficulty);
        player->SendRaidDifficulty(true);
    }

    SendUpdate();
}

bool Group::InCombatToInstance(uint32 instanceId)
{
    for (GroupReference* itr = GetFirstMember(); itr != NULL; itr = itr->next())
    {
        Player* player = itr->getSource();
        if (player && !player->getAttackers().empty() && player->GetInstanceId() == instanceId && (player->GetMap()->IsRaidOrHeroicDungeon()))
            for (std::set<Unit*>::const_iterator i = player->getAttackers().begin(); i != player->getAttackers().end(); ++i)
                if ((*i) && (*i)->GetTypeId() == TYPEID_UNIT && (*i)->ToCreature()->GetCreatureTemplate()->flags_extra & CREATURE_FLAG_EXTRA_INSTANCE_BIND)
                    return true;
    }
    return false;
}

Difficulty Group::GetDifficultyID(MapEntry const* p_MapEntry) const
{
    if (!p_MapEntry->IsRaid())
        return m_dungeonDifficulty;

    MapDifficulty const* defaultDifficulty = GetDefaultMapDifficulty(p_MapEntry->MapID);
    if (!defaultDifficulty)
        return m_LegacyRaidDifficuty;

    DifficultyEntry const* difficulty = sDifficultyStore.LookupEntry(defaultDifficulty->DifficultyID);
    if (!difficulty || difficulty->Flags & DIFFICULTY_FLAG_LEGACY)
        return m_LegacyRaidDifficuty;

    return m_raidDifficulty;
}

void Group::ResetInstances(uint8 method, bool isRaid, bool isLegacy, Player* SendMsgTo)
{
    if (isBGGroup() || isBFGroup())
        return;

    // method can be INSTANCE_RESET_ALL, INSTANCE_RESET_CHANGE_DIFFICULTY, INSTANCE_RESET_GROUP_DISBAND

    // we assume that when the difficulty changes, all instances that can be reset will be
    Difficulty diff = GetDungeonDifficultyID();
    if (isRaid)
    {
        if (!isLegacy)
            diff = GetRaidDifficultyID();
        else
            diff = GetLegacyRaidDifficultyID();
    }

    for (BoundInstancesMap::iterator itr = m_boundInstances[diff].begin(); itr != m_boundInstances[diff].end();)
    {
        InstanceSave* instanceSave = itr->second.save;
        const MapEntry* entry = sMapStore.LookupEntry(itr->first);
        if (!entry || entry->IsRaid() != isRaid || (!instanceSave->CanReset() && method != INSTANCE_RESET_GROUP_DISBAND))
        {
            ++itr;
            continue;
        }

        if (method == INSTANCE_RESET_ALL)
        {
            // the "reset all instances" method can only reset normal maps
            if (entry->instanceType == MAP_RAID || diff == DifficultyRaidHeroic)
            {
                ++itr;
                continue;
            }
        }

        bool isEmpty = true;
        // if the map is loaded, reset it
        Map* map = sMapMgr->FindMap(instanceSave->GetMapId(), instanceSave->GetInstanceId());
        if (map && map->IsDungeon() && !(method == INSTANCE_RESET_GROUP_DISBAND && !instanceSave->CanReset()))
        {
            if (instanceSave->CanReset())
                isEmpty = ((InstanceMap*)map)->Reset(method);
            else
                isEmpty = !map->HavePlayers();
        }

        if (SendMsgTo)
        {
            if (isEmpty)
                SendMsgTo->SendResetInstanceSuccess(instanceSave->GetMapId());
            else
                SendMsgTo->SendResetInstanceFailed(INSTANCE_RESET_FAILED_OFFLINE, instanceSave->GetMapId());
        }

        if (isEmpty || method == INSTANCE_RESET_GROUP_DISBAND || method == INSTANCE_RESET_CHANGE_DIFFICULTY)
        {
            // do not reset the instance, just unbind if others are permanently bound to it
            if (instanceSave->CanReset())
                instanceSave->DeleteFromDB();
            else
            {
                PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_INSTANCE);

                stmt->setUInt32(0, instanceSave->GetInstanceId());

                CharacterDatabase.Execute(stmt);
            }


            // i don't know for sure if hash_map iterators
            m_boundInstances[diff].erase(itr);
            itr = m_boundInstances[diff].begin();
            // this unloads the instance save unless online players are bound to it
            // (eg. permanent binds or GM solo binds)
            instanceSave->RemoveGroup(this);
        }
        else
            ++itr;
    }
}

InstanceGroupBind* Group::GetBoundInstance(Player* player)
{
    uint32 mapid = player->GetMapId();
    MapEntry const* mapEntry = sMapStore.LookupEntry(mapid);
    return GetBoundInstance(mapEntry);
}

InstanceGroupBind* Group::GetBoundInstance(Map* aMap)
{
    return GetBoundInstance(aMap->GetEntry());
}

InstanceGroupBind* Group::GetBoundInstance(MapEntry const* mapEntry)
{
    if (!mapEntry)
        return NULL;

    Difficulty difficulty = GetDifficultyID(mapEntry);

    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(mapEntry->MapID, difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapEntry->MapID);
    if (itr != m_boundInstances[difficulty].end())
        return &itr->second;
    else
        return NULL;
}

InstanceGroupBind* Group::GetBoundInstance(Difficulty difficulty, uint32 mapId)
{
    // some instances only have one difficulty
    GetDownscaledMapDifficultyData(mapId, difficulty);

    BoundInstancesMap::iterator itr = m_boundInstances[difficulty].find(mapId);
    if (itr != m_boundInstances[difficulty].end())
        return &itr->second;
    else
        return NULL;
}

InstanceGroupBind* Group::BindToInstance(InstanceSave* save, bool permanent, bool load)
{
    if (!save || isBGGroup() || isBFGroup())
        return NULL;

    InstanceGroupBind& bind = m_boundInstances[save->GetDifficultyID()][save->GetMapId()];
    if (!load && (!bind.save || permanent != bind.perm || save != bind.save))
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_REP_GROUP_INSTANCE);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt32(1, save->GetInstanceId());
        stmt->setBool(2, permanent);

        CharacterDatabase.Execute(stmt);
    }

    if (bind.save != save)
    {
        if (bind.save)
            bind.save->RemoveGroup(this);
        save->AddGroup(this);
    }

    bind.save = save;
    bind.perm = permanent;
    if (!load)
        sLog->outDebug(LOG_FILTER_MAPS, "Group::BindToInstance: Group (guid: %u, storage id: %u) is now bound to map %d, instance %d, difficulty %d",
        GUID_LOPART(GetGUID()), m_dbStoreId, save->GetMapId(), save->GetInstanceId(), save->GetDifficultyID());

    return &bind;
}

void Group::UnbindInstance(uint32 p_MapID, uint8 p_DifficultyID, bool p_Unload)
{
    if (p_DifficultyID >= Difficulty::MaxDifficulties)
        return;

    if (m_boundInstances[p_DifficultyID].empty())
        return;

    if (m_boundInstances[p_DifficultyID].find(p_MapID) == m_boundInstances[p_DifficultyID].end())
        return;

    BoundInstancesMap::iterator l_Instance = m_boundInstances[p_DifficultyID].find(p_MapID);
    if (!l_Instance->second.save)
        return;

    if (!p_Unload)
    {
        PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GROUP_INSTANCE_BY_GUID);

        stmt->setUInt32(0, m_dbStoreId);
        stmt->setUInt32(1, l_Instance->second.save->GetInstanceId());

        CharacterDatabase.Execute(stmt);
    }

    l_Instance->second.save->RemoveGroup(this);                // save can become invalid
    m_boundInstances[p_DifficultyID].erase(l_Instance);
}

void Group::_homebindIfInstance(Player* player)
{
    if (player && !player->isGameMaster() && !player->IsInGarrison() && sMapStore.LookupEntry(player->GetMapId())->IsDungeon())
        player->m_InstanceValid = false;
}

void Group::BroadcastGroupUpdate(void)
{
    // FG: HACK: force flags update on group leave - for values update hack
    // -- not very efficient but safe
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        Player* pp = ObjectAccessor::FindPlayer(citr->guid);
        if (pp && pp->IsInWorld())
        {
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_SHAPESHIFT_FORM);
            pp->ForceValuesUpdateAtIndex(UNIT_FIELD_FACTION_TEMPLATE);
            sLog->outDebug(LOG_FILTER_GENERAL, "-- Forced group value update for '%s'", pp->GetName());
        }
    }
}

void Group::ResetMaxEnchantingLevel()
{
    m_maxEnchantingLevel = 0;
    Player* pMember = NULL;
    for (member_citerator citr = m_memberSlots.begin(); citr != m_memberSlots.end(); ++citr)
    {
        pMember = ObjectAccessor::FindPlayer(citr->guid);
        if (pMember && m_maxEnchantingLevel < pMember->GetSkillValue(SKILL_ENCHANTING))
            m_maxEnchantingLevel = pMember->GetSkillValue(SKILL_ENCHANTING);
    }
}

void Group::SetLootMethod(LootMethod method)
{
    m_lootMethod = method;
}

void Group::SetLooterGuid(uint64 guid)
{
    m_looterGuid = guid;
}

void Group::SetLootThreshold(ItemQualities threshold)
{
    m_lootThreshold = threshold;
}

void Group::SetLfgRoles(uint64 guid, const uint8 roles)
{
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    slot->roles = roles;
    SendUpdate();
}

bool Group::IsFull() const
{
    return isRaidGroup() ? (m_memberSlots.size() >= MAXRAIDSIZE) : (m_memberSlots.size() >= MAXGROUPSIZE);
}

bool Group::isLFGGroup() const
{
    return m_PartyFlags & PARTY_FLAG_LFG;
}

bool Group::isRaidGroup() const
{
    return m_PartyFlags & PARTY_FLAG_RAID;
}

bool Group::isBGGroup() const
{
    return m_bgGroup != NULL;
}

bool Group::isBFGroup() const
{
    return m_bfGroup != NULL;
}

bool Group::IsCreated() const
{
    return GetMembersCount() > 0;
}

bool Group::IsGuildGroup(uint32 p_GuildID, bool p_SameMap, bool p_SameInstanceID)
{
    uint32 l_MapID = 0;
    uint32 l_InstanceID = 0;
    uint32 l_Counter = 0;
    uint32 l_MembersCount = GetMembersCount();

    std::vector<Player*> l_Members;

    // First we populate the array
    for (GroupReference* l_Iter = GetFirstMember(); l_Iter != NULL; l_Iter = l_Iter->next()) // Loop trough all members
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (l_Player->GetGuildId() == p_GuildID) // Check if it has a guild
                l_Members.push_back(l_Player);
        }
    }

    bool l_IsOkay = false;
    l_Counter = l_Members.size();
    for (std::vector<Player*>::iterator l_Iter = l_Members.begin(); l_Iter != l_Members.end(); ++l_Iter) // Iterate through players
    {
        if (Player* l_Player = (*l_Iter))
        {
            if (l_MapID == 0)
                l_MapID = l_Player->GetMapId();

            if (l_InstanceID == 0)
                l_InstanceID = l_Player->GetInstanceId();

            if (l_Player->GetMap()->IsNonRaidDungeon() && !l_IsOkay)
            {
                if (l_Counter >= 3)
                    l_IsOkay = true;
            }

            if (l_Player->GetMap()->IsRaid() && !l_IsOkay)
            {
                switch (l_Player->GetMap()->GetDifficultyID())
                {
                    case Difficulty::Difficulty10N:
                    case Difficulty::Difficulty10HC:
                        if (l_Counter >= 8)
                            l_IsOkay = true;
                        break;
                    case Difficulty::Difficulty25N:
                    case Difficulty::Difficulty25HC:
                    case Difficulty::DifficultyRaidTool:
                    case Difficulty::DifficultyRaidLFR:
                        if (l_Counter >= 20)
                            l_IsOkay = true;
                        break;
                    case Difficulty::Difficulty40:
                        if (l_Counter >= 30)
                            l_IsOkay = true;
                        break;
                    case Difficulty::DifficultyRaidNormal:
                    case Difficulty::DifficultyRaidHeroic:
                        /// Handle scaled difficulties (from 10 to 30)
                        if (l_Counter >= 10 && l_Counter >= CalculatePct(l_MembersCount, 80))
                            l_IsOkay = true;
                        break;
                    case Difficulty::DifficultyRaidMythic:
                        if (l_Counter >= 16)
                            l_IsOkay = true;
                        break;
                    default:
                        break;
                }
            }

            if (l_Player->GetMap()->IsBattleArena() && !l_IsOkay)
            {
                if (l_Counter == GetMembersCount())
                    l_IsOkay = true;
            }

            if (l_Player->GetMap()->IsBattleground() && !l_IsOkay)
            {
                if (Battleground* l_Battleground = l_Player->GetBattleground())
                {
                    if (l_Counter >= uint32(l_Battleground->GetMaxPlayers() * 0.8f))
                        l_IsOkay = true;
                }
            }

            // ToDo: Check 40-player raids: 10/40
            if (p_SameMap && (l_MapID != l_Player->GetMapId()))
                return false;

            if (p_SameInstanceID && (l_InstanceID != l_Player->GetInstanceId()))
                return false;
        }
    }

    return l_IsOkay;
}

uint64 Group::GetLeaderGUID() const
{
    return m_leaderGuid;
}

uint64 Group::GetGUID() const
{
    return m_guid;
}

uint32 Group::GetLowGUID() const
{
    return GUID_LOPART(m_guid);
}

const char * Group::GetLeaderName() const
{
    return m_leaderName.c_str();
}

LootMethod Group::GetLootMethod() const
{
    return m_lootMethod;
}

uint64 Group::GetLooterGuid() const
{
    return m_looterGuid;
}

ItemQualities Group::GetLootThreshold() const
{
    return m_lootThreshold;
}

bool Group::IsMember(uint64 guid) const
{
    return _getMemberCSlot(guid) != m_memberSlots.end();
}

bool Group::IsLeader(uint64 guid) const
{
    return (GetLeaderGUID() == guid);
}

uint64 Group::GetMemberGUID(const std::string& name)
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->name == name)
            return itr->guid;
    return 0;
}

bool Group::IsAssistant(uint64 guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return false;
    return mslot->flags & MEMBER_FLAG_ASSISTANT;
}

bool Group::SameSubGroup(uint64 guid1, uint64 guid2) const
{
    member_citerator mslot2 = _getMemberCSlot(guid2);
    if (mslot2 == m_memberSlots.end())
        return false;
    return SameSubGroup(guid1, &*mslot2);
}

bool Group::SameSubGroup(uint64 guid1, MemberSlot const* slot2) const
{
    member_citerator mslot1 = _getMemberCSlot(guid1);
    if (mslot1 == m_memberSlots.end() || !slot2)
        return false;
    return (mslot1->group == slot2->group);
}

bool Group::HasFreeSlotSubGroup(uint8 subgroup) const
{
    return (m_subGroupsCounts && m_subGroupsCounts[subgroup] < MAXGROUPSIZE);
}

uint32 Group::GetActiveMarkers() const
{
    uint32 l_Mask = eRaidMarkersFlags::RaidMarkerNone;

    for (RaidMarker const& l_Marker : GetRaidMarkers())
    {
        if (l_Marker.Mask)
            l_Mask |= l_Marker.Mask;
    }

    return l_Mask;
}

uint32 Group::CountActiveMarkers() const
{
    uint32 l_Count = 0;

    for (RaidMarker const& l_Marker : GetRaidMarkers())
    {
        if (l_Marker.Mask)
            ++l_Count;
    }

    return l_Count;
}

void Group::SendRaidMarkersUpdate()
{
    std::vector<RaidMarker> const& l_RaidMarkers = GetRaidMarkers();
    WorldPacket l_Data(Opcodes::SMSG_RAID_MARKERS_CHANGED, 10);

    l_Data << uint8(0); ///< PartyIndex
    l_Data << uint32(GetActiveMarkers());

    l_Data.WriteBits(CountActiveMarkers(), 4);
    l_Data.FlushBits();

    for (RaidMarker const& l_Marker : l_RaidMarkers)
    {
        if (!l_Marker.Mask)
            continue;

        l_Data.appendPackGUID(0);   ///< TransportGuid
        l_Data << uint32(l_Marker.MapID);
        l_Data << float(l_Marker.PosX);
        l_Data << float(l_Marker.PosY);
        l_Data << float(l_Marker.PosZ);
    }

    BroadcastPacket(&l_Data, true);
}

void Group::AddRaidMarker(uint8 p_Slot, uint32 p_MapID, float p_X, float p_Y, float p_Z)
{
    if (p_Slot >= eRaidMarkersMisc::MaxRaidMarkers)
        return;

    uint32 l_Mask = eRaidMarkersFlags::RaidMarkerNone;

    RaidMarker l_RaidMarker;
    l_RaidMarker.MapID  = p_MapID;
    l_RaidMarker.PosX   = p_X;
    l_RaidMarker.PosY   = p_Y;
    l_RaidMarker.PosZ   = p_Z;
    l_RaidMarker.Slot   = p_Slot;

    switch (p_Slot)
    {
        case eRaidMarkersMisc::SlotBlue:
            l_Mask = eRaidMarkersFlags::RaidMarkerBlue;
            break;
        case eRaidMarkersMisc::SlotGreen:
            l_Mask = eRaidMarkersFlags::RaidMarkerGreen;
            break;
        case eRaidMarkersMisc::SlotPurple:
            l_Mask = eRaidMarkersFlags::RaidMarkerPurple;
            break;
        case eRaidMarkersMisc::SlotRed:
            l_Mask = eRaidMarkersFlags::RaidMarkerRed;
            break;
        case eRaidMarkersMisc::SlotYellow:
            l_Mask = eRaidMarkersFlags::RaidMarkerYellow;
            break;
        case eRaidMarkersMisc::SlotOrange:
            l_Mask = eRaidMarkersFlags::RaidMarkerOrange;
            break;
        case eRaidMarkersMisc::SlotSilver:
            l_Mask = eRaidMarkersFlags::RaidMarkerSilver;
            break;
        case eRaidMarkersMisc::SlotWhite:
            l_Mask = eRaidMarkersFlags::RaidMarkerWhite;
            break;
        default:
            break;
    }

    l_RaidMarker.Mask = l_Mask;

    m_RaidMarkers[p_Slot] = l_RaidMarker;
    SendRaidMarkersUpdate();
}

void Group::RemoveRaidMarker(uint8 p_Slot)
{
    if (p_Slot >= eRaidMarkersMisc::MaxRaidMarkers)
        return;

    m_RaidMarkers[p_Slot] = RaidMarker();
    SendRaidMarkersUpdate();
}

void Group::RemoveAllRaidMarkers()
{
    for (uint8 l_Slot = 0; l_Slot < eRaidMarkersMisc::MaxRaidMarkers; ++l_Slot)
        m_RaidMarkers[l_Slot] = RaidMarker();

    SendRaidMarkersUpdate();
}

uint8 Group::GetMemberGroup(uint64 guid) const
{
    member_citerator mslot = _getMemberCSlot(guid);
    if (mslot == m_memberSlots.end())
        return (MAX_RAID_SUBGROUPS+1);
    return mslot->group;
}

void Group::SetBattlegroundGroup(Battleground* bg)
{
    m_bgGroup = bg;
}

void Group::SetBattlefieldGroup(Battlefield *bg)
{
    m_bfGroup = bg;
}

void Group::setGroupMemberRole(uint64 guid, uint32 role)
{
    for (auto member = m_memberSlots.begin(); member != m_memberSlots.end(); ++member)
    {
        if (member->guid == guid)
        {
            member->roles = role;
            break;
        }
    }
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_ROLE);
    if (stmt != nullptr)
    {
        stmt->setUInt8(0, role);
        stmt->setUInt32(1, GUID_LOPART(guid));
        CharacterDatabase.Execute(stmt);
    }
}

void Group::OnChangeMemberSpec(uint64 p_GUID, uint32 p_SpecId)
{
    for (auto l_Member = m_memberSlots.begin(); l_Member != m_memberSlots.end(); ++l_Member)
    {
        if (l_Member->guid == p_GUID)
        {
            l_Member->specID = p_SpecId;
            break;
        }
    }

    PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_SPEC);
    if (l_Stmt != nullptr)
    {
        l_Stmt->setUInt32(0, p_SpecId);
        l_Stmt->setUInt32(1, GUID_LOPART(p_GUID));
        CharacterDatabase.Execute(l_Stmt);
    }

}

uint32 Group::getGroupMemberRole(uint64 guid)
{
    for (auto member = m_memberSlots.begin(); member != m_memberSlots.end(); ++member)
        if (member->guid == guid)
            return member->roles;

    return 0;
}

void Group::SetGroupMemberFlag(uint64 guid, bool apply, GroupMemberFlags flag)
{
    // Assistants, main assistants and main tanks are only available in raid groups
    if (!isRaidGroup())
        return;

    // Check if player is really in the raid
    member_witerator slot = _getMemberWSlot(guid);
    if (slot == m_memberSlots.end())
        return;

    // Do flag specific actions, e.g ensure uniqueness
    switch (flag) {
    case MEMBER_FLAG_MAINASSIST:
        RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINASSIST);         // Remove main assist flag from current if any.
        break;
    case MEMBER_FLAG_MAINTANK:
        RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINTANK);           // Remove main tank flag from current if any.
        break;
    case MEMBER_FLAG_ASSISTANT:
        break;
    default:
        return;                                                      // This should never happen
    }

    // Switch the actual flag
    ToggleGroupMemberFlag(slot, flag, apply);

    // Preserve the new setting in the db
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_GROUP_MEMBER_FLAG);

    stmt->setUInt8(0, slot->flags);
    stmt->setUInt32(1, GUID_LOPART(guid));

    CharacterDatabase.Execute(stmt);

    // Broadcast the changes to the group
    SendUpdate();
}


Difficulty Group::GetDungeonDifficultyID() const
{
    return m_dungeonDifficulty;
}

Difficulty Group::GetRaidDifficultyID() const
{
    return m_raidDifficulty;
}
Difficulty Group::GetLegacyRaidDifficultyID() const
{
    return m_LegacyRaidDifficuty;
}

bool Group::isRollLootActive() const
{
    return !RollId.empty();
}

Group::Rolls::iterator Group::GetRoll(uint8 slot)
{
    Rolls::iterator iter;
    for (iter=RollId.begin(); iter != RollId.end(); ++iter)
        if ((*iter)->itemSlot == slot && (*iter)->isValid())
            return iter;
    return RollId.end();
}

void Group::LinkMember(GroupReference* pRef)
{
    m_memberMgr.insertFirst(pRef);
}

void Group::DelinkMember(uint64 guid)
{
    GroupReference* ref = m_memberMgr.getFirst();
    while (ref)
    {
        GroupReference* nextRef = ref->next();
        if (ref->getSource()->GetGUID() == guid)
        {
            ref->unlink();
            break;
        }
        ref = nextRef;
    }
}

Group::BoundInstancesMap& Group::GetBoundInstances(Difficulty difficulty)
{
    return m_boundInstances[difficulty];
}

void Group::_initRaidSubGroupsCounter()
{
    // Sub group counters initialization
    if (!m_subGroupsCounts)
        m_subGroupsCounts = new uint8[MAX_RAID_SUBGROUPS];

    memset((void*)m_subGroupsCounts, 0, (MAX_RAID_SUBGROUPS)*sizeof(uint8));

    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        ++m_subGroupsCounts[itr->group];
}

Group::member_citerator Group::_getMemberCSlot(uint64 Guid) const
{
    for (member_citerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

Group::member_witerator Group::_getMemberWSlot(uint64 Guid)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->guid == Guid)
            return itr;
    return m_memberSlots.end();
}

void Group::SubGroupCounterIncrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        ++m_subGroupsCounts[subgroup];
}

void Group::SubGroupCounterDecrease(uint8 subgroup)
{
    if (m_subGroupsCounts)
        --m_subGroupsCounts[subgroup];
}

void Group::RemoveUniqueGroupMemberFlag(GroupMemberFlags flag)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
        if (itr->flags & flag)
            itr->flags &= ~flag;
}

void Group::ToggleGroupMemberFlag(member_witerator slot, uint8 flag, bool apply)
{
    if (apply)
        slot->flags |= flag;
    else
        slot->flags &= ~flag;
}

void Group::OfflineMemberLost(uint64 guid, uint32 againstMatchmakerRating, uint8 slot, int32 MatchmakerRatingChange)
{
    // Called for offline player after ending rated arena match!
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (itr->guid == guid)
        {
            if (Player* p = ObjectAccessor::FindPlayer(guid))
            {
                // update personal rating
                int32 mod = Arena::GetRatingMod(p->GetArenaPersonalRating(slot), againstMatchmakerRating, false);
                p->SetArenaPersonalRating(slot, std::max(0, (int)p->GetArenaPersonalRating(slot) + mod));

                // update matchmaker rating
                p->SetArenaMatchMakerRating(slot, std::max(0, (int)p->GetArenaMatchMakerRating(slot) + MatchmakerRatingChange));

                // update personal played stats
                p->IncrementWeekGames(slot);
                p->IncrementSeasonGames(slot);
                return;
            }
        }
    }
}

void Group::MemberLost(Player* player, uint32 againstMatchmakerRating, uint8 slot, int32 MatchmakerRatingChange)
{
    // Called for each participant of a match after losing
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (itr->guid == player->GetGUID())
        {
            // Update personal rating
            int32 mod = Arena::GetRatingMod(player->GetArenaPersonalRating(slot), againstMatchmakerRating, false);
            player->SetArenaPersonalRating(slot, std::max(0, (int)player->GetArenaPersonalRating(slot) + mod));

            // Update matchmaker rating
            player->SetArenaMatchMakerRating(slot, std::max(0, (int)player->GetArenaMatchMakerRating(slot) + MatchmakerRatingChange));

            // Update personal played stats
            player->IncrementWeekGames(slot);
            player->IncrementSeasonGames(slot);
            return;
        }
    }
}

uint32 Group::GetRating(uint8 slot)
{
    uint32 rating = 0;
    uint32 count = 0;
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
        {
            rating += player->GetArenaPersonalRating(slot);
            ++count;
        }
    }

    if (!count)
        count = 1;

    rating /= count;
    return rating;
}

void Group::WonAgainst(uint32 Own_MMRating, uint32 Opponent_MMRating, int32& rating_change, uint8 slot)
{
    // Called when the team has won
    // Change in Matchmaker rating
    int32 mod = Arena::GetMatchmakerRatingMod(Own_MMRating, Opponent_MMRating, true);

    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
        {
            // Change in Team Rating
            rating_change = Arena::GetRatingMod(player->GetArenaPersonalRating(slot), Opponent_MMRating, true);

            if (player->GetArenaPersonalRating(slot) < 1000)
                rating_change = 96;

            if (player->GetBattleground())
                for (Battleground::BattlegroundScoreMap::const_iterator itr2 = player->GetBattleground()->GetPlayerScoresBegin(); itr2 != player->GetBattleground()->GetPlayerScoresEnd(); ++itr2)
                    if (itr2->first == itr->guid)
                        itr2->second->RatingChange = rating_change;

            player->SetArenaPersonalRating(slot, player->GetArenaPersonalRating(slot) + rating_change);
            player->SetArenaMatchMakerRating(slot, player->GetArenaMatchMakerRating(slot) + mod);

            player->IncrementWeekWins(slot);
            player->IncrementSeasonWins(slot);
            player->IncrementWeekGames(slot);
            player->IncrementSeasonGames(slot);
        }
    }
}

bool Group::CanEnterInInstance()
{
    uint8 maxplayers = 0;

    if (!isRaidGroup())
        maxplayers = 5;
    else
    {
        switch (GetLegacyRaidDifficultyID())
        {
            case Difficulty::Difficulty10N:
            case Difficulty::Difficulty10HC:
                maxplayers = 10;
                break;
            case Difficulty::Difficulty25N:
            case Difficulty::Difficulty25HC:
            case Difficulty::DifficultyRaidTool:
            case Difficulty::DifficultyRaidLFR:
                maxplayers = 25;
                break;
            case Difficulty::Difficulty40:
                maxplayers = 40;
                break;
        }
    }

    if (m_membersInInstance < maxplayers)
        return true;
    else
        return false;
}

void Group::LostAgainst(uint32 Own_MMRating, uint32 Opponent_MMRating, int32& rating_change, uint8 slot)
{
    // Called when the team has lost
    // Change in Matchmaker Rating
    int32 mod = Arena::GetMatchmakerRatingMod(Own_MMRating, Opponent_MMRating, false);

    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
        {
            // Change in Team Rating
            rating_change = Arena::GetRatingMod(player->GetArenaPersonalRating(slot), Opponent_MMRating, false);

            if (player->GetBattleground())
                for (Battleground::BattlegroundScoreMap::const_iterator itr2 = player->GetBattleground()->GetPlayerScoresBegin(); itr2 != player->GetBattleground()->GetPlayerScoresEnd(); ++itr2)
                    if (itr2->first == itr->guid)
                        itr2->second->RatingChange = rating_change;

            player->SetArenaPersonalRating(slot, std::max(0, (int)player->GetArenaPersonalRating(slot) + rating_change));
            player->SetArenaMatchMakerRating(slot, std::max(0, (int)player->GetArenaMatchMakerRating(slot) + mod));

            player->IncrementWeekGames(slot);
            player->IncrementSeasonGames(slot);
        }
    }
}

void Group::FinishGame(int32 rating_change, uint8 slot)
{
    for (member_witerator itr = m_memberSlots.begin(); itr != m_memberSlots.end(); ++itr)
    {
        if (Player* player = ObjectAccessor::FindPlayer(itr->guid))
        {
            if (player->GetBattleground())
                for (Battleground::BattlegroundScoreMap::const_iterator itr2 = player->GetBattleground()->GetPlayerScoresBegin(); itr2 != player->GetBattleground()->GetPlayerScoresEnd(); ++itr2)
                    if (itr2->first == itr->guid)
                        itr2->second->RatingChange = rating_change;

            player->SetArenaPersonalRating(slot, std::max(0, (int)player->GetArenaPersonalRating(slot) + rating_change));
            player->IncrementWeekGames(slot);
            player->IncrementSeasonGames(slot);
        }
    }
}
