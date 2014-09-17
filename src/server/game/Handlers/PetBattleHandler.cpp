/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "PetBattle.h"
#include "WildBattlePet.h"
#include "AchievementMgr.h"

void WorldSession::SendPetBattleJournal()
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_ACCOUNT);
    stmt->setUInt32(0, GetAccountId());
    _petBattleJournalCallback = LoginDatabase.AsyncQuery(stmt);
}

bool WorldSession::SendPetBattleJournalCallback(PreparedQueryResult& p_Result)
{
    if (!_player || !_player->IsInWorld())
        return true;

    if (!p_Result)
    {
        for (uint32 l_I = 0; l_I < _player->OldPetBattleSpellToMerge.size(); l_I++)
        {
            BattlePet pet;
            pet.Slot = PETBATTLE_NULL_SLOT;
            pet.NameTimeStamp = 0;
            pet.Species = _player->OldPetBattleSpellToMerge[l_I];
            pet.DisplayModelID = 0;
            pet.Flags = 0;

            if (BattlePetTemplate const* temp = sObjectMgr->GetBattlePetTemplate(_player->OldPetBattleSpellToMerge[l_I]))
            {
                pet.Breed = temp->Breed;
                pet.Quality = temp->Quality;
                pet.Level = temp->Level;
            }
            else
            {
                pet.Breed = 3;
                pet.Quality = BATTLEPET_QUALITY_COMMON;
                pet.Level = 1;
            }

            // Calculate XP for level
            pet.XP = 0;
            if (pet.Level > 1 && pet.Level < 100)
                pet.XP = sGtBattlePetXPStore.LookupEntry(pet.Level - 2)->value * sGtBattlePetXPStore.LookupEntry(100 + pet.Level - 2)->value;

            // Calculate stats
            pet.UpdateStats();
            pet.Health = pet.InfoMaxHealth;

            pet.AddToPlayer(_player);
        }

        _player->OldPetBattleSpellToMerge.clear();
        return false;
    }

    std::vector<BattlePet>  l_Pets(p_Result->GetRowCount());
    uint32                  l_UnlockedSlotCount = _player->GetUnlockedPetBattleSlot();
    BattlePet*              l_PetSlots[3]       = { 0, 0, 0 };
    size_t                  l_PetID             = 0;

    std::vector<uint32> l_AlreadyKnownPet;

    uint32 l_MaxLevelCount = 0;
    do
    {
        l_Pets[l_PetID].Load(p_Result->Fetch());
        l_AlreadyKnownPet.push_back(l_Pets[l_PetID].Species);
             
        if (l_Pets[l_PetID].Slot >= 0 && l_Pets[l_PetID].Slot < (int32)l_UnlockedSlotCount)
            l_PetSlots[l_Pets[l_PetID].Slot] = &l_Pets[l_PetID];

        ++l_PetID;
    }
    while (p_Result->NextRow());

    bool l_OldPetAdded = false;
    for (uint32 l_I = 0; l_I < _player->OldPetBattleSpellToMerge.size(); l_I++)
    {
        if (std::find(l_AlreadyKnownPet.begin(), l_AlreadyKnownPet.end(), _player->OldPetBattleSpellToMerge[l_I]) != l_AlreadyKnownPet.end())
            continue;

        l_OldPetAdded = true;

        BattlePet pet;
        pet.Slot = PETBATTLE_NULL_SLOT;
        pet.NameTimeStamp = 0;
        pet.Species = _player->OldPetBattleSpellToMerge[l_I];
        pet.DisplayModelID = 0;
        pet.Flags = 0;

        if (BattlePetTemplate const* temp = sObjectMgr->GetBattlePetTemplate(_player->OldPetBattleSpellToMerge[l_I]))
        {
            pet.Breed = temp->Breed;
            pet.Quality = temp->Quality;
            pet.Level = temp->Level;
        }
        else
        {
            pet.Breed = 3;
            pet.Quality = BATTLEPET_QUALITY_COMMON;
            pet.Level = 1;
        }

        // Calculate XP for level
        pet.XP = 0;
        if (pet.Level > 1 && pet.Level < 100)
            pet.XP = sGtBattlePetXPStore.LookupEntry(pet.Level - 2)->value * sGtBattlePetXPStore.LookupEntry(100 + pet.Level - 2)->value;

        // Calculate stats
        pet.UpdateStats();
        pet.Health = pet.InfoMaxHealth;

        pet.AddToPlayer(_player);
        l_Pets.push_back(pet);
    }

    _player->OldPetBattleSpellToMerge.clear();

    if (l_OldPetAdded)
        return false;

    if (l_UnlockedSlotCount > 0)
        _player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_HAS_BATTLE_PET_TRAINING);

    if (l_MaxLevelCount == 30 && !_player->GetAchievementMgr().HasAchieved(6581))
        if (AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(6581))
            _player->CompletedAchievement(achievementEntry);

    if (l_Pets.size() == 50 && !_player->GetAchievementMgr().HasAchieved(6556))
        if (AchievementEntry const* achievementEntry = sAchievementMgr->GetAchievement(6556))
            _player->CompletedAchievement(achievementEntry);

    WorldPacket l_Packet(SMSG_PETBATTLE_FULL_JOURNAL, 1000);

    l_Packet.WriteBits(l_Pets.size(), 19);

    for (std::vector<BattlePet>::iterator l_It = l_Pets.begin(); l_It != l_Pets.end(); ++l_It)
    {
        ObjectGuid l_Guid = l_It->JournalID;

        l_Packet.WriteBit(l_Guid[7]);
        l_Packet.WriteBit(!l_It->Quality);
        l_Packet.WriteBit(l_Guid[3]);
        l_Packet.WriteBit(l_Guid[5]);
        l_Packet.WriteBit(l_Guid[1]);
        l_Packet.WriteBit(l_Guid[6]);
        l_Packet.WriteBit(0);
        l_Packet.WriteBit(!0);
        l_Packet.WriteBit(!l_It->Flags);
        l_Packet.WriteBit(0);                                   // Has Unk block
        l_Packet.WriteBits(l_It->Name.length(), 7);

        //if (unkBlock)
        //{
        //    data.WriteBit(unkGuid[1]);
        //    data.WriteBit(unkGuid[4]);
        //    data.WriteBit(unkGuid[3]);
        //    data.WriteBit(unkGuid[5]);
        //    data.WriteBit(unkGuid[7]);
        //    data.WriteBit(unkGuid[2]);
        //    data.WriteBit(unkGuid[6]);
        //    data.WriteBit(unkGuid[0]);
        //}

        l_Packet.WriteBit(l_Guid[0]);
        l_Packet.WriteBit(l_Guid[2]);
        l_Packet.WriteBit(l_Guid[4]);
    }

    l_Packet.WriteBit(1);                                     // unk
    l_Packet.WriteBits(3, 25);                                // Slots count 

    for (uint32 l_I = 0; l_I < 3; l_I++)
    {
        ObjectGuid l_Guid;

        if (l_PetSlots[l_I])
            l_Guid = l_PetSlots[l_I]->JournalID;

        l_Packet.WriteBit(!l_I);                                // Have slot ID
        l_Packet.WriteBit(!((l_I + 1) <= l_UnlockedSlotCount)); // Has unlocked slot
        l_Packet.WriteBit(!0);                                  // Has collar id
        l_Packet.WriteBit(l_Guid != 0);                         // Has Guid
        l_Packet.WriteBit(l_Guid[1]);
        l_Packet.WriteBit(l_Guid[4]);
        l_Packet.WriteBit(l_Guid[5]);
        l_Packet.WriteBit(l_Guid[6]);
        l_Packet.WriteBit(l_Guid[0]);
        l_Packet.WriteBit(l_Guid[2]);
        l_Packet.WriteBit(l_Guid[3]);
        l_Packet.WriteBit(l_Guid[7]);
    }

    l_Packet.FlushBits();

    for (uint32 l_I = 0; l_I < 3; l_I++)
    {
        ObjectGuid l_Guid;

        if (l_PetSlots[l_I])
            l_Guid = l_PetSlots[l_I]->JournalID;

        l_Packet.WriteByteSeq(l_Guid[2]);
        l_Packet.WriteByteSeq(l_Guid[1]);
        l_Packet.WriteByteSeq(l_Guid[3]);
        l_Packet.WriteByteSeq(l_Guid[6]);
        l_Packet.WriteByteSeq(l_Guid[7]);
        l_Packet.WriteByteSeq(l_Guid[4]);
        l_Packet.WriteByteSeq(l_Guid[5]);
        l_Packet.WriteByteSeq(l_Guid[0]);

        if (l_I)
            l_Packet << uint8(l_I);
    }

    for (std::vector<BattlePet>::iterator l_It = l_Pets.begin(); l_It != l_Pets.end(); ++l_It)
    {
        ObjectGuid l_Guid = l_It->JournalID;
        BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(l_It->Species);

        l_It->UpdateStats();

        //if (unkBlock)
        //{
        //    data.WriteBit(unkGuid[1]);
        //    data.WriteBit(unkGuid[3]);
        //    data.WriteBit(unkGuid[5]);
        //    data.WriteBit(unkGuid[2]);
        //    data.WriteBit(unkGuid[4]);
        //    data.WriteBit(unkGuid[7]);
        //    data << uint32(0);
        //    data << uint32(0);
        //    data.WriteBit(unkGuid[6]);
        //    data.WriteBit(unkGuid[0]);
        //}

        l_Packet << uint16(l_It->Level);
        l_Packet.WriteByteSeq(l_Guid[7]);
        l_Packet << int32(l_It->Health > l_It->InfoMaxHealth ? l_It->InfoMaxHealth : l_It->Health);
        l_Packet << uint32(l_It->Species);
        l_Packet << int32(l_It->InfoSpeed);
        l_Packet << int32(l_It->InfoPower);
        l_Packet << int32(l_It->InfoMaxHealth);
        l_Packet.WriteByteSeq(l_Guid[6]);
        l_Packet << uint32(l_SpeciesInfo ? l_SpeciesInfo->entry : 0);
        l_Packet.WriteByteSeq(l_Guid[4]);
        l_Packet.WriteByteSeq(l_Guid[2]);
        l_Packet.WriteByteSeq(l_Guid[3]);
        l_Packet.WriteByteSeq(l_Guid[0]);

        if (l_It->Flags)
            l_Packet << uint16(l_It->Flags);

        l_Packet << uint32(l_It->DisplayModelID);

        if (l_It->Quality)
            l_Packet << uint8(l_It->Quality);

        l_Packet << uint16(l_It->XP);
        l_Packet.WriteByteSeq(l_Guid[1]);
        l_Packet.WriteByteSeq(l_Guid[5]);
        l_Packet.WriteString(l_It->Name);
    }

    l_Packet << uint16(0); // unk

    SendPacket(&l_Packet);
    return true;
}

void WorldSession::SendPetBattleJournalBattleSlotUpdate()
{
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_BATTLESLOT_ACCOUNT);
    stmt->setUInt32(0, GetAccountId());
    _petBattleJournalBattleSlotCallback = LoginDatabase.AsyncQuery(stmt);
}

void WorldSession::SendPetBattleJournalBattleSlotUpdateCallback(PreparedQueryResult& p_Result)
{
    if (!_player || !_player->IsInWorld())
        return;

    std::vector<BattlePet>  l_Pets(p_Result ? p_Result->GetRowCount() : 0);
    uint32                  l_UnlockedSlotCount = _player->GetUnlockedPetBattleSlot();
    BattlePet*              l_PetSlots[3]       = { 0, 0, 0 };
    size_t                  l_PetID             = 0;

    if (l_UnlockedSlotCount > 0)
        _player->SetFlag(PLAYER_FLAGS, PLAYER_FLAGS_HAS_BATTLE_PET_TRAINING);

    if (p_Result)
    {
        do
        {
            l_Pets[l_PetID].Load(p_Result->Fetch());

            if (l_Pets[l_PetID].Slot >= 0 && l_Pets[l_PetID].Slot < (int32)l_UnlockedSlotCount)
                l_PetSlots[l_Pets[l_PetID].Slot] = &l_Pets[l_PetID];

            ++l_PetID;
        } while (p_Result->NextRow());
    }

    WorldPacket l_Packet(SMSG_PETBATTLE_UPDATE_BATTLESLOT, 100);

    l_Packet.WriteBits(l_UnlockedSlotCount, 25);              // Slots count 
    l_Packet.WriteBit(1);
    l_Packet.WriteBit(0);

    for (uint32 l_I = 0; l_I < l_UnlockedSlotCount; l_I++)
    {
        ObjectGuid l_Guid;

        if (l_PetSlots[l_I])
            l_Guid = l_PetSlots[l_I]->JournalID;

        l_Packet.WriteBit(l_Guid == 0);                       // Has Guid
        l_Packet.WriteBit(!0);
        l_Packet.WriteBit(l_Guid[3]);
        l_Packet.WriteBit(l_Guid[1]);
        l_Packet.WriteBit(l_Guid[5]);
        l_Packet.WriteBit(l_Guid[4]);
        l_Packet.WriteBit(l_Guid[6]);
        l_Packet.WriteBit(l_Guid[2]);
        l_Packet.WriteBit(l_Guid[7]);
        l_Packet.WriteBit(l_Guid[0]);
        l_Packet.WriteBit(!l_I);                              // Have slot ID
        l_Packet.WriteBit(0);
    }

    l_Packet.FlushBits();

    for (uint32 l_I = 0; l_I < l_UnlockedSlotCount; l_I++)
    {
        ObjectGuid l_Guid;

        if (l_PetSlots[l_I])
            l_Guid = l_PetSlots[l_I]->JournalID;

        l_Packet.WriteByteSeq(l_Guid[3]);
        l_Packet.WriteByteSeq(l_Guid[7]);
        l_Packet.WriteByteSeq(l_Guid[6]);
        l_Packet.WriteByteSeq(l_Guid[2]);
        l_Packet.WriteByteSeq(l_Guid[1]);
        l_Packet.WriteByteSeq(l_Guid[5]);
        l_Packet.WriteByteSeq(l_Guid[4]);
        l_Packet.WriteByteSeq(l_Guid[0]);

        if (l_I)
            l_Packet << uint8(l_I);
    }

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleRequestFailed(uint8 p_Reason)
{
    WorldPacket packet(SMSG_PETBATTLE_REQUEST_FAILED, 2);
    packet.WriteBit(!p_Reason);
    packet.FlushBits();

    if (p_Reason)
        packet << p_Reason;

    SendPacket(&packet);
}

void WorldSession::SendPetBattleFullUpdate(PetBattle* battle)
{
    WorldPacket packet(SMSG_PETBATTLE_FULL_UPDATE, 1000);

    ObjectGuid l_Guid = 0;

    if (battle->BattleType == PETBATTLE_TYPE_PVE)
        l_Guid = battle->Teams[1]->OwnerGuid;

    uint32 l_NpcDisplayID = 0;
    uint32 l_NpcCreatureID = 0;
    uint16 l_WaitingForFrontPetsMaxSecs = 30;
    uint16 l_PvpMaxRoundTime = 30;
    uint8 l_CurPetBattleState = 1;
    bool l_CanAwardXP = true;
    bool l_IsPVP = false;

    for (uint32 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        ObjectGuid l_OwnerGuid = battle->Teams[l_TeamID]->OwnerGuid;

        if (battle->BattleType == PETBATTLE_TYPE_PVE && l_TeamID == PETBATTLE_PVE_TEAM_ID)
            l_OwnerGuid = 0;

        packet.WriteBit(l_OwnerGuid[5]);
        packet.WriteBit(l_OwnerGuid[6]);
        packet.WriteBit(l_OwnerGuid[3]);
        packet.WriteBit(!true);                // As active team
        packet.WriteBit(!1);                   // dwordC
        packet.WriteBit(l_OwnerGuid[2]);
        packet.WriteBit(l_OwnerGuid[7]);
        packet.WriteBit(l_OwnerGuid[4]);
        packet.WriteBits(battle->Teams[l_TeamID]->TeamPetCount, 2);

        for (uint32 l_PetID = 0; l_PetID < battle->Teams[l_TeamID]->TeamPetCount; l_PetID++)
        {
            BattlePetInstance* l_Pet = battle->Teams[l_TeamID]->TeamPets[l_PetID];

            ObjectGuid l_JournalID = (battle->BattleType == PETBATTLE_TYPE_PVE && l_TeamID == PETBATTLE_PVE_TEAM_ID) ? 0 : l_Pet->JournalID;

            uint32 l_StatesCount = 0;
            uint32 l_AbilityCount = 0;

            for (uint32 l_State = 0; l_State < NUM_BATTLEPET_STATES; l_State++)
                if (l_State == 20 ||l_State == 19 ||l_State == 18 ||l_State == 40 || l_State == 49)
                    l_StatesCount++;

            for (uint32 l_Ability = 0; l_Ability < MAX_PETBATTLE_ABILITIES; l_Ability++)
                if (l_Pet->Abilities[l_Ability])
                    l_AbilityCount++;

            packet.WriteBit(l_JournalID[7]);
            packet.WriteBits(l_StatesCount, 21);
            packet.WriteBit(l_JournalID[5]);
            packet.WriteBits(l_AbilityCount, 20);
            packet.WriteBit(!true /*l_PetID*/);                             ///< Match pet id
            packet.WriteBit(l_JournalID[3]);
            packet.WriteBit(l_JournalID[0]);
            packet.WriteBit(l_JournalID[1]);
            packet.WriteBit(l_JournalID[6]);

            for (uint32 l_I = 0; l_I < l_AbilityCount; l_I++)
                packet.WriteBit(!true);     ///< Always send match pet id

            packet.WriteBit(l_JournalID[4]);
            packet.WriteBits(0, 21);
            packet.WriteBits(l_Pet->Name.length(), 7);
            packet.WriteBit(l_JournalID[2]);
            packet.WriteBit(!l_Pet->Flags);
        }

        packet.WriteBit(l_OwnerGuid[1]);
        packet.WriteBit(!0);
        packet.WriteBit(l_OwnerGuid[0]);
    }

    packet.WriteBit(!l_WaitingForFrontPetsMaxSecs);

    for (uint32 l_I = 0; l_I < 3; l_I++)
    {
        packet.WriteBits(0, 21);
        packet.WriteBits(0, 21);
    }

    packet.WriteBit(!l_Guid);
    packet.WriteBit(l_Guid[3]);
    packet.WriteBit(l_Guid[0]);
    packet.WriteBit(l_Guid[2]);
    packet.WriteBit(l_Guid[5]);
    packet.WriteBit(l_Guid[7]);
    packet.WriteBit(l_Guid[1]);
    packet.WriteBit(l_Guid[4]);
    packet.WriteBit(l_Guid[6]);
    packet.WriteBit(l_CanAwardXP);
    packet.WriteBit(!l_CurPetBattleState);
    packet.WriteBit(l_IsPVP);
    packet.WriteBit(!l_NpcDisplayID);
    packet.WriteBit(!l_NpcCreatureID);
    packet.WriteBit(!l_PvpMaxRoundTime);
    packet.WriteBit(!battle->GetForfeitHealthPenalityPct());

    packet.FlushBits();

    if (l_CurPetBattleState)
        packet << uint8(l_CurPetBattleState);

    for (uint32 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        ObjectGuid l_OwnerGuid = battle->Teams[l_TeamID]->OwnerGuid;

        if (battle->BattleType == PETBATTLE_TYPE_PVE && l_TeamID == PETBATTLE_PVE_TEAM_ID)
            l_OwnerGuid = 0;

        packet.WriteByteSeq(l_OwnerGuid[6]);
        packet.WriteByteSeq(l_OwnerGuid[1]);

        for (uint32 l_PetID = 0; l_PetID < battle->Teams[l_TeamID]->TeamPetCount; l_PetID++)
        {
            BattlePetInstance* l_Pet = battle->Teams[l_TeamID]->TeamPets[l_PetID];

            ObjectGuid l_JournalID = (battle->BattleType == PETBATTLE_TYPE_PVE && l_TeamID == PETBATTLE_PVE_TEAM_ID) ? 0 : l_Pet->JournalID;

            packet << uint32(0);
            packet.WriteByteSeq(l_JournalID[4]);
            packet << uint32(l_Pet->DisplayModelID);

            if (l_Pet->Flags)
                packet << uint16(l_Pet->Flags & ~PETBATTLE_FLAG_CAPTURED);

            packet.WriteByteSeq(l_JournalID[7]);

            for (int l_State = 0; l_State < NUM_BATTLEPET_STATES; l_State++)
                if (l_State == 20 || l_State == 19 || l_State == 18 || l_State == 40 || l_State == 49)
                    packet << uint32(l_State) << int32(l_Pet->States[l_State]);

            packet << uint32(0);

            for (uint32 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
            {
                if (l_Pet->Abilities[l_CurrentAbilitySlot])
                {
                    packet << uint32(l_Pet->Abilities[l_CurrentAbilitySlot]);
                    packet << uint16(0);
                    packet << uint16(0);
                    packet << uint8(l_CurrentAbilitySlot);

                    if (true)                           ///< Always send match pet id
                        packet << uint8(l_Pet->ID);
                }
            }

            packet << uint16(l_Pet->Quality);
            packet << uint32(l_Pet->Species);

            packet.WriteByteSeq(l_JournalID[3]);
            packet.WriteByteSeq(l_JournalID[1]);
            packet.WriteByteSeq(l_JournalID[2]);
            packet.WriteByteSeq(l_JournalID[0]);
            packet.WriteByteSeq(l_JournalID[5]);

            packet.WriteString(l_Pet->Name);

            packet << uint32(l_Pet->InfoSpeed);
            packet << uint16(l_Pet->Level);

            packet.WriteByteSeq(l_JournalID[6]);

            packet << uint32(l_Pet->Health);
            packet << uint32(l_Pet->InfoMaxHealth);
            packet << uint16(l_Pet->XP);
            packet << uint32(l_Pet->InfoPower / 100);

            if (true/*l_PetID*/)                                            ///< Match pet id
                packet << uint8(l_PetID);
        }

        packet.WriteByteSeq(l_OwnerGuid[0]);

        if (l_TeamID == PETBATTLE_TEAM_1)
            packet << uint8(battle->Teams[l_TeamID]->ActivePetID);
        else
            packet << uint8(battle->Teams[l_TeamID]->ActivePetID - (l_TeamID == PETBATTLE_TEAM_2 ? MAX_PETBATTLE_SLOTS : 0));

        packet << uint32(battle->Teams[l_TeamID]->GetCatchAbilityID());
        packet << uint32(l_TeamID == PETBATTLE_TEAM_1 ? 5 : 2); // dwordc

        packet.WriteByteSeq(l_OwnerGuid[4]);
        packet.WriteByteSeq(l_OwnerGuid[7]);

        packet << uint8(6);

        packet.WriteByteSeq(l_OwnerGuid[2]);
        packet.WriteByteSeq(l_OwnerGuid[3]);
        packet.WriteByteSeq(l_OwnerGuid[5]);
    }

    packet << uint32(battle->Turn); ///< Turn ? always 0 here

    packet.WriteByteSeq(l_Guid[5]);
    packet.WriteByteSeq(l_Guid[4]);
    packet.WriteByteSeq(l_Guid[0]);
    packet.WriteByteSeq(l_Guid[1]);
    packet.WriteByteSeq(l_Guid[2]);
    packet.WriteByteSeq(l_Guid[3]);
    packet.WriteByteSeq(l_Guid[6]);
    packet.WriteByteSeq(l_Guid[7]);

    if (battle->GetForfeitHealthPenalityPct())
        packet << uint8(battle->GetForfeitHealthPenalityPct());

    if (l_PvpMaxRoundTime)
        packet << uint16(l_PvpMaxRoundTime);

    if (l_NpcCreatureID)
        packet << uint32(l_NpcCreatureID);

    if (l_WaitingForFrontPetsMaxSecs)
        packet << uint16(l_WaitingForFrontPetsMaxSecs);

    if (l_NpcDisplayID)
        packet << uint32(l_NpcDisplayID);

    SendPacket(&packet);
}

void WorldSession::SendPetBattleFirstRound(PetBattle* p_Battle)
{
    WorldPacket l_Packet(SMSG_PETBATTLE_FIRST_ROUND, 100);

    l_Packet << uint32(p_Battle->Turn);

    for (uint32 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        l_Packet << uint16(0);                                              ///< RoundTimeSecs
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamTrapFlags());   ///< NextTrapStatus
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamInputFlags());  ///< NextInputFlags
    }

    l_Packet.WriteBits((uint32)p_Battle->RoundEvents.size(), 22);

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        l_Packet.WriteBit(!(l_EventIt->SourcePetID != -1));
        l_Packet.WriteBits((uint32)l_EventIt->Updates.size(), 25);
        l_Packet.WriteBit(!0);
        l_Packet.WriteBit(!(l_EventIt->Flags & 0xFFFF));
        l_Packet.WriteBit(!l_EventIt->EventType);
        l_Packet.WriteBit(!l_EventIt->AbilityEffectID);

        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            l_Packet.WriteBit(!(l_UpdateIt->TargetPetID != -1));
            l_Packet.WriteBits(l_UpdateIt->UpdateType, 3);

            /// updates are not implemented because only type TYPE_LOCK seed in this packet
        }

        l_Packet.WriteBit(!0);
        l_Packet.WriteBit(!0);
    }

    l_Packet.WriteBits(0, 20);
    l_Packet.WriteBit(!p_Battle->RoundResult);
    l_Packet.WriteBits(0, 3);
    l_Packet.FlushBits();

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            if (l_UpdateIt->TargetPetID != -1)
                l_Packet << uint8(l_UpdateIt->TargetPetID);

            /// updates are not implemented because only type TYPE_LOCK seed in this packet
        }

        if (l_EventIt->EventType)
            l_Packet << uint8(l_EventIt->EventType);

        if (l_EventIt->AbilityEffectID)
            l_Packet << uint32(l_EventIt->AbilityEffectID);

        if (l_EventIt->SourcePetID != -1)
            l_Packet << uint8(l_EventIt->SourcePetID);

        if (l_EventIt->Flags)
            l_Packet << uint16(l_EventIt->Flags);
    }

    if (p_Battle->RoundResult)
        l_Packet << uint8(p_Battle->RoundResult);

   SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFinalRound(PetBattle* p_Battle)
{
    WorldPacket l_Packet(SMSG_PETBATTLE_FINAL_ROUND, 200);

    for (size_t l_TeamId = 0; l_TeamId < MAX_PETBATTLE_TEAM; ++l_TeamId)
        l_Packet.WriteBit(p_Battle->WinnerTeamId == l_TeamId);

    l_Packet.WriteBit(p_Battle->CombatResult == PETBATTLE_RESULT_ABANDON);
    l_Packet.WriteBit(0);
    l_Packet.WriteBits(p_Battle->TotalPetCount, 20);

    for (uint32 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        l_Packet.WriteBit(!true);                                                                           // As XP
        l_Packet.WriteBit(!true);                                                                           // Old level
        l_Packet.WriteBit(!true);                                                                           // New level
        l_Packet.WriteBit(p_Battle->CatchedPetId == l_CurrentPetslot);
        l_Packet.WriteBit(p_Battle->Pets[l_CurrentPetslot]->Level != p_Battle->Pets[l_CurrentPetslot]->OldLevel);
        l_Packet.WriteBit(p_Battle->CatchedPetId == l_CurrentPetslot);
        l_Packet.WriteBit(p_Battle->FightedPets.find(l_CurrentPetslot) != p_Battle->FightedPets.end());
    }

    l_Packet.FlushBits();

    for (uint32 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        l_Packet << uint8(p_Battle->Pets[l_CurrentPetslot]->ID);
        l_Packet << int32(p_Battle->Pets[l_CurrentPetslot]->Health);

        //if (true)
            l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->XP);

        l_Packet << int32(p_Battle->Pets[l_CurrentPetslot]->InfoMaxHealth);

        //if (true)
            l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->OldLevel);

        //if (true)
            l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Level);
    }

    for (size_t l_TeamId = 0; l_TeamId < MAX_PETBATTLE_TEAM; ++l_TeamId)
        l_Packet << uint32(0);

    SendPacket(&l_Packet);
    
}

void WorldSession::SendPetBattleRoundResult(PetBattle* p_Battle)
{
    WorldPacket l_Packet(SMSG_PETBATTLE_ROUND_RESULT, 700);

    uint32 l_CooldownCount = 0;

    for (uint32 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint32 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1)
                l_CooldownCount++;
    }

    for (uint32 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamTrapFlags());
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamInputFlags());
        l_Packet << uint16(0);
    }

    l_Packet << uint32(p_Battle->Turn);

    l_Packet.WriteBit(!p_Battle->RoundResult);
    l_Packet.WriteBits(p_Battle->RoundDeadPets.size(), 3);
    l_Packet.WriteBits((uint32)p_Battle->RoundEvents.size(), 22);

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        l_Packet.WriteBit(!(l_EventIt->SourcePetID != -1));
        l_Packet.WriteBit(!l_EventIt->AbilityEffectID);
        l_Packet.WriteBit(!l_EventIt->BuffTurn);
        l_Packet.WriteBits((uint32)l_EventIt->Updates.size(), 25);
        l_Packet.WriteBit(!l_EventIt->StackDepth);
        l_Packet.WriteBit(!(l_EventIt->Flags & 0xFFFF));
        l_Packet.WriteBit(!l_EventIt->EventType);

        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            l_Packet.WriteBits(l_UpdateIt->UpdateType, 3);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_HEALTH)
                l_Packet.WriteBit(!l_UpdateIt->Health);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_BUFF)
            {
                l_Packet.WriteBit(!l_UpdateIt->Buff.ID);
                l_Packet.WriteBit(!l_UpdateIt->Buff.AbilityID);
                l_Packet.WriteBit(!l_UpdateIt->Buff.Turn);
                l_Packet.WriteBit(!l_UpdateIt->Buff.Duration);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_ABILITY_CHANGE)
            {
                l_Packet.WriteBit(!0);
                l_Packet.WriteBit(!0);
                l_Packet.WriteBit(!0);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_TRIGGER)
                l_Packet.WriteBit(!l_UpdateIt->TriggerAbilityId);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_STATE)
            {
                l_Packet.WriteBit(!l_UpdateIt->State.Value);
                l_Packet.WriteBit(!l_UpdateIt->State.ID);
            }

            l_Packet.WriteBit(!(l_UpdateIt->TargetPetID != -1));

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_SPEED)
                l_Packet.WriteBit(!l_UpdateIt->Speed);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_NPC_EMOTE)
                l_Packet.WriteBit(!l_UpdateIt->NpcEmote.BroadcastTextID);
        }

        l_Packet.WriteBit(!l_EventIt->RoundTurn);
    }

    l_Packet.WriteBits(l_CooldownCount, 20);

    for (uint32 l_I = 0; l_I < l_CooldownCount; l_I++)
        l_Packet.WriteBit(!true);   /// Has match pet id

    l_Packet.FlushBits();

    for (uint32 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint32 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1)
            {
                if (true)
                    l_Packet << uint8(p_Battle->Pets[l_CurrentPetslot]->ID);    /// Has match pet id

                l_Packet << uint32(p_Battle->Pets[l_CurrentPetslot]->Abilities[l_AbilitySlot]);
                l_Packet << uint8(l_AbilitySlot);

                l_Packet << uint16(0);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot]);
            }
        }
    }

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_STATE)
            {
                if (l_UpdateIt->State.Value)
                    l_Packet << int32(l_UpdateIt->State.Value);

                if (l_UpdateIt->State.ID)
                    l_Packet << uint32(l_UpdateIt->State.ID);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_ABILITY_CHANGE)
            {
                if (false)
                    l_Packet << uint32(0);

                if (false)
                    l_Packet << uint32(0);

                if (false)
                    l_Packet << uint32(0);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_BUFF)
            {
                if (l_UpdateIt->Buff.AbilityID)
                    l_Packet << uint32(l_UpdateIt->Buff.AbilityID);

                if (l_UpdateIt->Buff.Duration)
                    l_Packet << int32(l_UpdateIt->Buff.Duration);

                if (l_UpdateIt->Buff.Turn)
                    l_Packet << uint32(l_UpdateIt->Buff.Turn);

                if (l_UpdateIt->Buff.ID)
                    l_Packet << uint32(l_UpdateIt->Buff.ID);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_TRIGGER && l_UpdateIt->TriggerAbilityId)
                l_Packet << uint32(l_UpdateIt->TriggerAbilityId);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_SPEED && l_UpdateIt->Speed)
                l_Packet << int32(l_UpdateIt->Speed);

            if (l_UpdateIt->TargetPetID != -1)
                l_Packet << uint8(l_UpdateIt->TargetPetID);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_NPC_EMOTE)
                l_Packet << int32(l_UpdateIt->NpcEmote.BroadcastTextID);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_HEALTH && l_UpdateIt->Health)
                l_Packet << int32(l_UpdateIt->Health);
        }

        if (l_EventIt->StackDepth)
            l_Packet << uint8(l_EventIt->StackDepth);

        if (l_EventIt->RoundTurn)
            l_Packet << uint16(l_EventIt->RoundTurn);

        if (l_EventIt->Flags)
            l_Packet << uint16(l_EventIt->Flags);

        if (l_EventIt->SourcePetID != -1)
            l_Packet << uint8(l_EventIt->SourcePetID);

        if (l_EventIt->EventType)
            l_Packet << uint8(l_EventIt->EventType);

        if (l_EventIt->BuffTurn)
            l_Packet << uint16(l_EventIt->BuffTurn);

        if (l_EventIt->AbilityEffectID)
            l_Packet << uint32(l_EventIt->AbilityEffectID);
    }

    for (uint32 l_I = 0; l_I < p_Battle->RoundDeadPets.size(); l_I++)
        l_Packet << uint8(p_Battle->RoundDeadPets[l_I]);

    if (p_Battle->RoundResult)
        l_Packet << uint8(p_Battle->RoundResult);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFinalizeLocation(PetBattleRequest* p_Request)
{
    WorldPacket l_Packet(SMSG_PETBATTLE_FINALIZE_LOCATION, 16 + 16 + 16 + 1 + 4 + 4);

    l_Packet << p_Request->PetBattleCenterPosition[0];

    for (int l_I = 0; l_I < 2; l_I++)
        l_Packet << p_Request->TeamPosition[l_I][0] << p_Request->TeamPosition[l_I][1] << p_Request->TeamPosition[l_I][2];

    l_Packet << p_Request->PetBattleCenterPosition[1];
    l_Packet << p_Request->PetBattleCenterPosition[2];

    l_Packet.WriteBit(!p_Request->LocationResult);
    l_Packet.WriteBit(G3D::fuzzyEq(p_Request->BattleFacing, 0));
    l_Packet.FlushBits();

    if (p_Request->LocationResult)
        l_Packet << uint32(p_Request->LocationResult);

    if (!G3D::fuzzyEq(p_Request->BattleFacing, 0))
        l_Packet << float(p_Request->BattleFacing);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFinished(PetBattle* battle)
{
    WorldPacket packet(SMSG_PETBATTLE_FINISHED, 0);
    SendPacket(&packet);
}

//////////////////////////////////////////////////////////////////////////
void WorldSession::HandlePetBattleSetAbility(WorldPacket& p_RecvData)
{
    ObjectGuid l_PetJournalID;
    uint32 l_Flag = 0;
    uint8 l_Action = 0;

    p_RecvData >> l_Flag;

    l_PetJournalID[0]   = p_RecvData.ReadBit();
    l_Action            = p_RecvData.ReadBits(2);  // 0 add flag, 2 remove it
    l_PetJournalID[7]   = p_RecvData.ReadBit();
    l_PetJournalID[3]   = p_RecvData.ReadBit();
    l_PetJournalID[5]   = p_RecvData.ReadBit();
    l_PetJournalID[1]   = p_RecvData.ReadBit();
    l_PetJournalID[2]   = p_RecvData.ReadBit();
    l_PetJournalID[4]   = p_RecvData.ReadBit();
    l_PetJournalID[6]   = p_RecvData.ReadBit();

    p_RecvData.ReadByteSeq(l_PetJournalID[3]);
    p_RecvData.ReadByteSeq(l_PetJournalID[4]);
    p_RecvData.ReadByteSeq(l_PetJournalID[2]);
    p_RecvData.ReadByteSeq(l_PetJournalID[5]);
    p_RecvData.ReadByteSeq(l_PetJournalID[6]);
    p_RecvData.ReadByteSeq(l_PetJournalID[0]);
    p_RecvData.ReadByteSeq(l_PetJournalID[7]);
    p_RecvData.ReadByteSeq(l_PetJournalID[1]);

    PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_PETBATTLE_TOGGLE_FLAGS);
    l_Stmt->setUInt32(0, l_Flag);
    l_Stmt->setUInt32(1, l_Flag);
    l_Stmt->setUInt32(2, l_Flag);
    l_Stmt->setUInt64(3, l_PetJournalID);
    LoginDatabase.Execute(l_Stmt);
}

void WorldSession::HandlePetBattleRename(WorldPacket& p_RecvData)
{
    DeclinedName    l_DeclinedNames;
    ObjectGuid      l_PetJournalID;
    bool            l_HaveDeclinedNames = false;
    uint32          l_NameLenght        = 0;
    std::string     l_Name;

    uint32 l_DeclinedNameLens[MAX_DECLINED_NAME_CASES];

    l_PetJournalID[3]   = p_RecvData.ReadBit();
    l_PetJournalID[5]   = p_RecvData.ReadBit();
    l_PetJournalID[1]   = p_RecvData.ReadBit();
    l_PetJournalID[4]   = p_RecvData.ReadBit();
    l_PetJournalID[0]   = p_RecvData.ReadBit();
    l_PetJournalID[6]   = p_RecvData.ReadBit();
    l_NameLenght        = p_RecvData.ReadBits(7);
    l_PetJournalID[2]   = p_RecvData.ReadBit();
    l_PetJournalID[7]   = p_RecvData.ReadBit();
    l_HaveDeclinedNames = p_RecvData.ReadBit();

    if (l_HaveDeclinedNames)
    {
        for (size_t l_I = 0 ; l_I < MAX_DECLINED_NAME_CASES ; ++l_I)
            l_DeclinedNameLens[l_I] = p_RecvData.ReadBits(7);
    }

    p_RecvData.ReadByteSeq(l_PetJournalID[1]);
    p_RecvData.ReadByteSeq(l_PetJournalID[7]);
    p_RecvData.ReadByteSeq(l_PetJournalID[3]);
    p_RecvData.ReadByteSeq(l_PetJournalID[4]);
    p_RecvData.ReadByteSeq(l_PetJournalID[0]);
    p_RecvData.ReadByteSeq(l_PetJournalID[5]);
    p_RecvData.ReadByteSeq(l_PetJournalID[2]);
    p_RecvData.ReadByteSeq(l_PetJournalID[6]);

    l_Name = p_RecvData.ReadString(l_NameLenght);

    if (l_HaveDeclinedNames)
    {
        for (size_t l_I = 0 ; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_DeclinedNames.name[l_I] = p_RecvData.ReadString(l_DeclinedNameLens[l_I]);
    }

    PetNameInvalidReason l_NameInvalidReason = sObjectMgr->CheckPetName(l_Name);
    if (l_NameInvalidReason != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(l_NameInvalidReason, l_Name, (l_HaveDeclinedNames) ? &l_DeclinedNames : NULL);
        return;
    }

    uint32 l_TimeStamp = l_Name.empty() ? 0 : time(0);

    PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_PETBATTLE_RENAME);
    l_Stmt->setString(0, l_Name);
    l_Stmt->setUInt32(1, l_TimeStamp);
    l_Stmt->setUInt32(2, GetAccountId());
    l_Stmt->setUInt64(3, l_PetJournalID);
    LoginDatabase.Execute(l_Stmt);

    _player->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, l_TimeStamp);

    Creature* l_Creature = _player->GetSummonedBattlePet();

    if (!l_Creature)
        return;

    l_Creature->SetName(l_Name);
    l_Creature->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, l_TimeStamp);

    //TODO: add declined names
}

void WorldSession::HandleSummonCompanion(WorldPacket& recvData)
{
    ObjectGuid l_JournalID;

    l_JournalID[7] = recvData.ReadBit();
    l_JournalID[5] = recvData.ReadBit();
    l_JournalID[0] = recvData.ReadBit();
    l_JournalID[2] = recvData.ReadBit();
    l_JournalID[4] = recvData.ReadBit();
    l_JournalID[6] = recvData.ReadBit();
    l_JournalID[3] = recvData.ReadBit();
    l_JournalID[1] = recvData.ReadBit();

    recvData.ReadByteSeq(l_JournalID[4]);
    recvData.ReadByteSeq(l_JournalID[1]);
    recvData.ReadByteSeq(l_JournalID[0]);
    recvData.ReadByteSeq(l_JournalID[2]);
    recvData.ReadByteSeq(l_JournalID[6]);
    recvData.ReadByteSeq(l_JournalID[3]);
    recvData.ReadByteSeq(l_JournalID[7]);
    recvData.ReadByteSeq(l_JournalID[5]);

    if (_player->GetSummonedBattlePet() && _player->GetSummonedBattlePet()->GetUInt64Value(UNIT_FIELD_BATTLE_PET_COMPANION_GUID) == l_JournalID)
        _player->UnsummonCurrentBattlePetIfAny(false);
    else
    {
        _player->UnsummonCurrentBattlePetIfAny(false);
        _player->SummonBattlePet(l_JournalID);
    }
}

void WorldSession::HandlePetBattleCagePet(WorldPacket& recvData)
{
    ObjectGuid l_Guid;

    l_Guid[6] = recvData.ReadBit();
    l_Guid[5] = recvData.ReadBit();
    l_Guid[0] = recvData.ReadBit();
    l_Guid[3] = recvData.ReadBit();
    l_Guid[4] = recvData.ReadBit();
    l_Guid[7] = recvData.ReadBit();
    l_Guid[2] = recvData.ReadBit();
    l_Guid[1] = recvData.ReadBit();

    recvData.ReadByteSeq(l_Guid[4]);
    recvData.ReadByteSeq(l_Guid[1]);
    recvData.ReadByteSeq(l_Guid[5]);
    recvData.ReadByteSeq(l_Guid[3]);
    recvData.ReadByteSeq(l_Guid[0]);
    recvData.ReadByteSeq(l_Guid[6]);
    recvData.ReadByteSeq(l_Guid[7]);
    recvData.ReadByteSeq(l_Guid[2]);

    if (_player->GetUInt64Value(PLAYER_FIELD_SUMMONED_BATTLE_PET_GUID) == l_Guid)
        _player->UnsummonCurrentBattlePetIfAny(false);


    PetBattle pet;
    //TODO: aync load
    //TODO: create item cage
}

void WorldSession::HandlePetBattleQueryName(WorldPacket& p_RecvData)
{
    ObjectGuid l_UnitGuid;
    ObjectGuid l_JournalGuid;

    l_JournalGuid[6] = p_RecvData.ReadBit();
    l_JournalGuid[3] = p_RecvData.ReadBit();
    l_JournalGuid[1] = p_RecvData.ReadBit();
    l_UnitGuid[0] = p_RecvData.ReadBit();
    l_UnitGuid[1] = p_RecvData.ReadBit();
    l_JournalGuid[4] = p_RecvData.ReadBit();
    l_JournalGuid[2] = p_RecvData.ReadBit();
    l_UnitGuid[3] = p_RecvData.ReadBit();
    l_UnitGuid[7] = p_RecvData.ReadBit();
    l_UnitGuid[4] = p_RecvData.ReadBit();
    l_JournalGuid[5] = p_RecvData.ReadBit();
    l_UnitGuid[6] = p_RecvData.ReadBit();
    l_UnitGuid[5] = p_RecvData.ReadBit();
    l_JournalGuid[0] = p_RecvData.ReadBit();
    l_UnitGuid[2] = p_RecvData.ReadBit();
    l_JournalGuid[7] = p_RecvData.ReadBit();

    p_RecvData.ReadByteSeq(l_JournalGuid[4]);
    p_RecvData.ReadByteSeq(l_JournalGuid[6]);
    p_RecvData.ReadByteSeq(l_JournalGuid[5]);
    p_RecvData.ReadByteSeq(l_UnitGuid[0]);
    p_RecvData.ReadByteSeq(l_UnitGuid[5]);
    p_RecvData.ReadByteSeq(l_JournalGuid[3]);
    p_RecvData.ReadByteSeq(l_UnitGuid[4]);
    p_RecvData.ReadByteSeq(l_JournalGuid[2]);
    p_RecvData.ReadByteSeq(l_JournalGuid[1]);
    p_RecvData.ReadByteSeq(l_UnitGuid[6]);
    p_RecvData.ReadByteSeq(l_JournalGuid[0]);
    p_RecvData.ReadByteSeq(l_UnitGuid[2]);
    p_RecvData.ReadByteSeq(l_UnitGuid[7]);
    p_RecvData.ReadByteSeq(l_UnitGuid[1]);
    p_RecvData.ReadByteSeq(l_UnitGuid[3]);
    p_RecvData.ReadByteSeq(l_JournalGuid[7]);

    Creature* l_Creature = Unit::GetCreature(*_player, l_UnitGuid);

    if (!l_Creature)
        return;

    WorldPacket l_Packet(SMSG_PETBATTLE_QUERY_NAME_RESPONSE, 0x40);

    l_Packet.WriteBit(l_Creature->GetName() ? true : false);

    if (l_Creature->GetName())
    {
        l_Packet.WriteBits(l_Creature->GetName() ? strlen(l_Creature->GetName()) : 0, 8);

        for (uint32 l_I = 0 ; l_I < 5 ; ++l_I)
            l_Packet.WriteBits(0, 7);

        l_Packet.WriteBit(0);   // unk maybe declined names
    }

    l_Packet.FlushBits();

    if (l_Creature->GetName())
        l_Packet.WriteString(l_Creature->GetName());

    l_Packet << uint64(l_JournalGuid);
    l_Packet << uint32(l_Creature->GetEntry());
    l_Packet << uint32(l_Creature->GetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP));

    _player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::HandleBattlePetSetBattleSlot(WorldPacket& p_RecvData)
{
    ObjectGuid  l_PetJournalID;
    uint8       l_DestSlot = 0;

    p_RecvData >> l_DestSlot;
    l_PetJournalID[7] = p_RecvData.ReadBit();
    l_PetJournalID[0] = p_RecvData.ReadBit();
    l_PetJournalID[6] = p_RecvData.ReadBit();
    l_PetJournalID[5] = p_RecvData.ReadBit();
    l_PetJournalID[2] = p_RecvData.ReadBit();
    l_PetJournalID[3] = p_RecvData.ReadBit();
    l_PetJournalID[4] = p_RecvData.ReadBit();
    l_PetJournalID[1] = p_RecvData.ReadBit();

    p_RecvData.ReadByteSeq(l_PetJournalID[3]);
    p_RecvData.ReadByteSeq(l_PetJournalID[2]);
    p_RecvData.ReadByteSeq(l_PetJournalID[4]);
    p_RecvData.ReadByteSeq(l_PetJournalID[7]);
    p_RecvData.ReadByteSeq(l_PetJournalID[5]);
    p_RecvData.ReadByteSeq(l_PetJournalID[1]);
    p_RecvData.ReadByteSeq(l_PetJournalID[6]);
    p_RecvData.ReadByteSeq(l_PetJournalID[0]);

    if (l_DestSlot >= MAX_PETBATTLE_SLOTS)
        return;

    PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE);
    l_Stmt->setUInt32(0, l_PetJournalID);
    _swapPetBattleSlot.SetFutureResult(LoginDatabase.AsyncQuery(l_Stmt));
    _swapPetBattleSlot.SetParam(l_DestSlot);
}

void WorldSession::HandleBattlePetSetBattleSlotCallBack(PreparedQueryResult& p_Result, uint8 p_DestSlot)
{
    if (!p_Result)
        return;

    BattlePet l_Pet;
    l_Pet.Load(p_Result->Fetch());

    if (l_Pet.AccountID != GetAccountId())
        return;

    if (l_Pet.Slot >= 0)
    {
        PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_REPLACE_BATTLESLOT);
        l_Stmt->setInt32(0, l_Pet.Slot);
        l_Stmt->setUInt32(1, GetAccountId());
        l_Stmt->setInt32(2, p_DestSlot);
        LoginDatabase.AsyncQuery(l_Stmt);

        l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_SET_BATTLESLOT);
        l_Stmt->setInt32(0, p_DestSlot);
        l_Stmt->setUInt32(1, GetAccountId());
        l_Stmt->setUInt64(2, l_Pet.JournalID);
        LoginDatabase.AsyncQuery(l_Stmt);
    }
    else
    {
        PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_UNSET_BATTLESLOT);
        l_Stmt->setUInt32(0, GetAccountId());
        l_Stmt->setInt32(1, p_DestSlot);
        LoginDatabase.AsyncQuery(l_Stmt);

        l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_SET_BATTLESLOT);
        l_Stmt->setInt32(0, p_DestSlot);
        l_Stmt->setUInt32(1, GetAccountId());
        l_Stmt->setUInt64(2, l_Pet.JournalID);
        LoginDatabase.AsyncQuery(l_Stmt);
    }
}

void WorldSession::HandlePetBattleRequestWild(WorldPacket& p_RecvData)
{
    PetBattleRequest* l_BattleRequest = sPetBattleSystem->CreateRequest(_player->GetGUID());
    ObjectGuid l_OpponentGuid;

    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[1] >> l_BattleRequest->PetBattleCenterPosition[2];

    for (int l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; l_CurrentTeamID++)
        p_RecvData >> l_BattleRequest->TeamPosition[l_CurrentTeamID][1] >> l_BattleRequest->TeamPosition[l_CurrentTeamID][2] >> l_BattleRequest->TeamPosition[l_CurrentTeamID][0];

    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[0];

    l_OpponentGuid[4] = p_RecvData.ReadBit();
    l_OpponentGuid[1] = p_RecvData.ReadBit();
    l_OpponentGuid[0] = p_RecvData.ReadBit();
    l_OpponentGuid[5] = p_RecvData.ReadBit();
    bool l_HasLocationResult = !p_RecvData.ReadBit();
    bool l_HasBattleFacing = !p_RecvData.ReadBit();
    l_OpponentGuid[7] = p_RecvData.ReadBit();
    l_OpponentGuid[6] = p_RecvData.ReadBit();
    l_OpponentGuid[2] = p_RecvData.ReadBit();
    l_OpponentGuid[3] = p_RecvData.ReadBit();

    p_RecvData.ReadByteSeq(l_OpponentGuid[3]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[2]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[6]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[1]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[7]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[5]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[4]);
    p_RecvData.ReadByteSeq(l_OpponentGuid[0]);

    if (l_HasLocationResult)
        p_RecvData >> l_BattleRequest->LocationResult;

    if (l_HasBattleFacing)
        p_RecvData >> l_BattleRequest->BattleFacing;

    l_BattleRequest->RequestType    = PETBATTLE_TYPE_PVE;
    l_BattleRequest->OpponentGuid   = l_OpponentGuid;

    // Player can't be already in battle
    if (_player->_petBattleId)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_ALREADY_IN_PETBATTLE);
        return;
    }

    // Player can't be in combat
    if (_player->isInCombat())
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_ALREADY_IN_COMBAT);
        return;
    }

    // Check positions
    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if (_player->GetMap()->getObjectHitPos(_player->GetPhaseMask(), l_BattleRequest->PetBattleCenterPosition[0], l_BattleRequest->PetBattleCenterPosition[1], l_BattleRequest->PetBattleCenterPosition[2],
            l_BattleRequest->TeamPosition[l_CurrentTeamID][0], l_BattleRequest->TeamPosition[l_CurrentTeamID][1], l_BattleRequest->TeamPosition[l_CurrentTeamID][2],
            l_BattleRequest->TeamPosition[l_CurrentTeamID][0], l_BattleRequest->TeamPosition[l_CurrentTeamID][1], l_BattleRequest->TeamPosition[l_CurrentTeamID][2], 0.0f))
        {
            SendPetBattleRequestFailed(PETBATTLE_REQUEST_GROUND_NOT_ENOUGHT_SMOOTH);
            return;
        }
    }

    // Wild should be for PetBattle and Player able to interact with it
    Creature* l_WildPet = _player->GetNPCIfCanInteractWith(l_BattleRequest->OpponentGuid, UNIT_NPC_FLAG_PETBATTLE);
    if (!l_WildPet)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_INVALID_TARGET);
        return;
    }

    // Ok, Despawn wild
    l_WildPet->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC); // Immuned to all
    l_WildPet->SetTarget(_player->GetGUID());
    l_WildPet->SetControlled(true, UNIT_STATE_ROOT);

    _player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC); // Immuned only to NPC
    _player->SetTarget(l_WildPet->GetGUID());

    // Request player team
    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_PETBATTLE_TEAM);
    stmt->setUInt32(0, GetAccountId());
    _petBattleRequestWildCallback.SetParam(l_BattleRequest);
    _petBattleRequestWildCallback.SetFutureResult(LoginDatabase.AsyncQuery(stmt));
}

void WorldSession::HandlePetBattleRequestWildCallback(PreparedQueryResult& p_Result, PetBattleRequest* p_Request)
{
    BattlePetInstance*  l_PlayerPets[MAX_PETBATTLE_SLOTS];
    BattlePetInstance*  l_WildBattlePet;
    size_t              l_PlayerPetCount = 0;
    PetBattle*          l_Battle;
    uint32              l_ErrorCode = PETBATTLE_REQUEST_FAILED;

    if (!_player || !_player->IsInWorld())
        return;

    // Temporary pet buffer
    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
        l_PlayerPets[l_CurrentPetSlot] = 0;

    Creature* l_Wild = sObjectAccessor->GetCreature(*_player, p_Request->OpponentGuid);
    if (!l_Wild)
    {
        l_ErrorCode = PETBATTLE_REQUEST_INVALID_TARGET;
        goto WILD_BATTLE_FAILED;
    }

    // No team
    if (!p_Result)
    {
        l_ErrorCode = PETBATTLE_REQUEST_NEED_AT_LEAST_1_PET_IN_SLOT;
        goto WILD_BATTLE_FAILED;
    }

    // Load player pets
    do
    {
        if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= _player->GetUnlockedPetBattleSlot())
            break;

        Field* l_Fields = p_Result->Fetch();

        l_PlayerPets[l_PlayerPetCount] = new BattlePetInstance();
        l_PlayerPets[l_PlayerPetCount]->Load(l_Fields);
        l_PlayerPets[l_PlayerPetCount]->Slot = l_PlayerPetCount;

        ++l_PlayerPetCount;
    }
    while (p_Result->NextRow());

    // Check player team
    if (!l_PlayerPetCount)
    {
        l_ErrorCode = PETBATTLE_REQUEST_NEED_AT_LEAST_1_PET_IN_SLOT;
        goto WILD_BATTLE_FAILED;
    }

    // Wild should be wild
    if (!sWildBattlePetMgr->IsWildPet(l_Wild))
        goto WILD_BATTLE_FAILED;

    l_WildBattlePet = sWildBattlePetMgr->GetWildBattlePet(l_Wild);

    if (!l_WildBattlePet)
        goto WILD_BATTLE_FAILED;

    SendPetBattleFinalizeLocation(p_Request);

    _player->SetFacingTo(_player->GetAngle(p_Request->TeamPosition[PETBATTLE_TEAM_2][0], p_Request->TeamPosition[PETBATTLE_TEAM_2][1]));
    _player->SetRooted(true);

    // Ok, Create battle
    l_Battle = sPetBattleSystem->CreateBattle();

    // Add player pets
    l_Battle->Teams[PETBATTLE_TEAM_1]->OwnerGuid  = _player->GetGUID();
    l_Battle->Teams[PETBATTLE_TEAM_1]->PlayerGuid = _player->GetGUID();

    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        if (l_PlayerPets[l_CurrentPetSlot])
            l_Battle->AddPet(PETBATTLE_TEAM_1, l_PlayerPets[l_CurrentPetSlot]);
    }

    // Add wild pet
    //TODO: wild pets can group nearby battle pets with lesser or equal level
    l_Battle->Teams[PETBATTLE_TEAM_2]->OwnerGuid = l_Wild->GetGUID();

    l_Battle->BattleType = p_Request->RequestType;
    l_Battle->AddPet(PETBATTLE_TEAM_2, l_WildBattlePet);

    // Launch battle
    _player->_petBattleId = l_Battle->ID;
    l_Battle->Begin();

    sWildBattlePetMgr->EnterInBattle(l_Wild);
    sPetBattleSystem->RemoveRequest(p_Request->RequesterGuid);

    return;

WILD_BATTLE_FAILED:
    _player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC);
    _player->SetRooted(false);
    if (l_Wild)
    {
        l_Wild->SetTarget(0);
        l_Wild->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC);
        l_Wild->SetControlled(false, UNIT_STATE_ROOT);
    }

    for (size_t l_CurrentPetID = 0; l_CurrentPetID < MAX_PETBATTLE_SLOTS; ++l_CurrentPetID)
        if (l_PlayerPets[l_CurrentPetID])
            delete l_PlayerPets[l_CurrentPetID];

    SendPetBattleRequestFailed(l_ErrorCode);
    delete p_Request;
}

void WorldSession::HandlePetBattleRequestPvP(WorldPacket& p_RecvData)
{
}

void WorldSession::HandlePetBattleJoinQueue(WorldPacket& p_RecvData)
{
}

void WorldSession::HandlePetBattleRequestUpdate(WorldPacket& p_RecvData)
{
}

void WorldSession::HandlePetBattleCancelRequestPvPMatchmaking(WorldPacket& p_RecvData)
{
}

enum ePetBattleActions
{
    PETBATTLE_ACTION_REQUEST_LEAVE      = 0,
    PETBATTLE_ACTION_CAST               = 1,
    PETBATTLE_ACTION_SWAP_OR_PASS       = 2,
    PETBATTLE_ACTION_CATCH              = 3,
    PETBATTLE_ACTION_LEAVE_PETBATTLE    = 4
};

void WorldSession::HandlePetBattleInputNewFrontPet(WorldPacket& p_RecvData)
{
    if (!_player->_petBattleId)
    {
        SendPetBattleFinished(0);
        return;
    }

    PetBattle* l_PetBattle = sPetBattleSystem->GetBattle(_player->_petBattleId);
    if (!l_PetBattle || l_PetBattle->BattleStatus == PETBATTLE_STATUS_FINISHED)
    {
        SendPetBattleFinished(0);
        return;
    }

    uint8 l_NewFrontPetID = 0;
    uint32 l_PlayerTeamID = 0;

    p_RecvData >> l_NewFrontPetID;

    if (l_PetBattle->Teams[PETBATTLE_TEAM_2]->PlayerGuid == _player->GetGUID())
        l_PlayerTeamID = PETBATTLE_TEAM_2;

    // Skip if input already died
    if (l_PetBattle->Teams[l_PlayerTeamID]->Ready)
        return;

    l_NewFrontPetID = (l_PlayerTeamID == PETBATTLE_TEAM_2 ? MAX_PETBATTLE_SLOTS : 0) + l_NewFrontPetID;

    if (!l_PetBattle->Teams[l_PlayerTeamID]->CanSwap(l_NewFrontPetID))
        return;

    l_PetBattle->SwapPet(l_PlayerTeamID, l_NewFrontPetID);
    l_PetBattle->SwapPet(!l_PlayerTeamID, l_PetBattle->Teams[!l_PlayerTeamID]->ActivePetID);
}

void WorldSession::HandlePetBattleInput(WorldPacket& p_RecvData)
{
    uint8   l_Action        = 0;
    uint8   l_NewFrontPetID = 0;
    uint32  l_Ability       = 0;
    uint32  l_Turn          = 0;

    bool byte13             = !p_RecvData.ReadBit();
    bool byte14             = p_RecvData.ReadBit();
    bool l_HasAbility       = !p_RecvData.ReadBit();
    bool l_HasNewFrontPet   = !p_RecvData.ReadBit();
    bool byte12             = !p_RecvData.ReadBit();
    bool l_HasActionType    = !p_RecvData.ReadBit();
    bool l_HasTurn          = !p_RecvData.ReadBit();

    if (byte13)
    {
        uint8 l_Value;
        p_RecvData >> l_Value;
    }

    if (l_HasNewFrontPet)
        p_RecvData >> l_NewFrontPetID;

    if (l_HasTurn)
        p_RecvData >> l_Turn;

    if (l_HasAbility)
        p_RecvData >> l_Ability;

    if (byte12)
    {
        uint8 l_Value;
        p_RecvData >> l_Value;
    }

    if (l_HasActionType)
        p_RecvData >> l_Action;

    if (l_Action == PETBATTLE_ACTION_LEAVE_PETBATTLE)
    {
        SendPetBattleFinished(0);
        return;
    }

    if (!_player->_petBattleId)
    {
        SendPetBattleFinished(0);
        return;
    }

    PetBattle * l_PetBattle = sPetBattleSystem->GetBattle(_player->_petBattleId);
    if (!l_PetBattle || l_PetBattle->BattleStatus == PETBATTLE_STATUS_FINISHED)
    {
        SendPetBattleFinished(0);
        return;
    }

    ///// Check sync
    if ((l_Turn + 1) != l_PetBattle->Turn)
    {
        sPetBattleSystem->ForfeitBattle(l_PetBattle->ID, _player->GetGUID());
        return;
    }

    if (l_HasActionType)
    {
        uint32 l_PlayerTeamID = 0;

        if (l_PetBattle->Teams[PETBATTLE_TEAM_2]->PlayerGuid == _player->GetGUID())
            l_PlayerTeamID = PETBATTLE_TEAM_2;

        // Skip if input already died
        if (l_PetBattle->Teams[l_PlayerTeamID]->Ready)
            return;

        if (l_Action == PETBATTLE_ACTION_REQUEST_LEAVE)
        {
            sPetBattleSystem->ForfeitBattle(l_PetBattle->ID, _player->GetGUID());
        }
        else if (l_Action == PETBATTLE_ACTION_CAST)
        {
            if (l_PetBattle->CanCast(l_PlayerTeamID, l_Ability))
                l_PetBattle->PrepareCast(l_PlayerTeamID, l_Ability);
        }
        else if (l_Action == PETBATTLE_ACTION_CATCH)
        {
            uint32 l_CatchAbilityID = l_PetBattle->Teams[l_PlayerTeamID]->GetCatchAbilityID();

            if (l_PetBattle->Teams[l_PlayerTeamID]->CanCatchOpponentTeamFrontPet() == PETBATTLE_TEAM_CATCH_FLAG_ENABLE_TRAP)
                l_PetBattle->PrepareCast(l_PlayerTeamID, l_CatchAbilityID);
        }
        else if (l_Action == PETBATTLE_ACTION_SWAP_OR_PASS)
        {
            l_NewFrontPetID = (l_PlayerTeamID == PETBATTLE_TEAM_2 ? MAX_PETBATTLE_SLOTS : 0) + l_NewFrontPetID;

            if (!l_PetBattle->Teams[l_PlayerTeamID]->CanSwap(l_NewFrontPetID))
                return;

            l_PetBattle->SwapPet(l_PlayerTeamID, l_NewFrontPetID);
        }
    }
}
