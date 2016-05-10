////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "PetBattle.h"
#include "WildBattlePet.h"
#include "AchievementMgr.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

void WorldSession::SendPetBattleSlotUpdates(bool p_NewSlotUnlocked)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    uint32                          l_UnlockedSlotCount = m_Player->GetUnlockedPetBattleSlot();
    BattlePet::Ptr                * l_PetSlots          = m_Player->GetBattlePetCombatTeam();

    if (l_UnlockedSlotCount > 0)
        m_Player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HAS_BATTLE_PET_TRAINING);

    WorldPacket l_Packet(SMSG_PET_BATTLE_SLOT_UPDATES, 100);
    l_Packet << uint32(MAX_PETBATTLE_SLOTS);                                                                ///< Slots count

    for (uint32 l_I = 0; l_I < MAX_PETBATTLE_SLOTS; l_I++)
    {
        uint64 l_Guid = 0;

        //bool l_IsLocked = false;

        //if (m_Player->HasBattlePetTraining() && (l_I + 1) <= l_UnlockedSlotCount)
        //    l_IsLocked = false;

        if (l_PetSlots[l_I])
            l_Guid = l_PetSlots[l_I]->JournalID;

        l_Packet.appendPackGUID(l_Guid);                                                                    ///< BattlePetGUID
        l_Packet << uint32(0);                                                                              ///< CollarID
        l_Packet << uint8(l_I);                                                                             ///< SlotIndex
        l_Packet.WriteBit(!((l_I + 1) <= l_UnlockedSlotCount));                                             ///< Locked
        l_Packet.FlushBits();
    }

    l_Packet.WriteBit(p_NewSlotUnlocked);                                                                   ///< NewSlotUnlocked
    l_Packet.WriteBit(true);                                                                                ///< AutoSlotted
    l_Packet.FlushBits();

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleRequestFailed(uint8 p_Reason)
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_REQUEST_FAILED, 2);
    l_Packet << p_Reason;

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattlePvPChallenge(PetBattleRequest* p_Request)
{
    uint32 l_UnkStatus = 0;

    WorldPacket l_Packet(SMSG_PET_BATTLE_PVPCHALLENGE, 2 + 16 + 4 + 4 + 4 + 4 + 4 + (3 * 4));

    l_Packet.appendPackGUID(p_Request->RequesterGuid);

    l_Packet << uint32(p_Request->LocationResult);
    l_Packet << p_Request->PetBattleCenterPosition[0];
    l_Packet << p_Request->PetBattleCenterPosition[1];
    l_Packet << p_Request->PetBattleCenterPosition[2];
    l_Packet << float(p_Request->BattleFacing);

    for (int l_I = 0; l_I < MAX_PETBATTLE_TEAM; l_I++)
        l_Packet << p_Request->TeamPosition[l_I][0] << p_Request->TeamPosition[l_I][1] << p_Request->TeamPosition[l_I][2];

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFinalizeLocation(PetBattleRequest* p_Request)
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_FINALIZE_LOCATION, 16 + 16 + 16 + 1 + 4 + 4);

    l_Packet << uint32(p_Request->LocationResult);
    l_Packet << p_Request->PetBattleCenterPosition[0];
    l_Packet << p_Request->PetBattleCenterPosition[1];
    l_Packet << p_Request->PetBattleCenterPosition[2];
    l_Packet << float(p_Request->BattleFacing);

    for (uint8 l_I = 0; l_I < MAX_PETBATTLE_TEAM; l_I++)
        l_Packet << p_Request->TeamPosition[l_I][0] << p_Request->TeamPosition[l_I][1] << p_Request->TeamPosition[l_I][2];

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleInitialUpdate(PetBattle* p_Battle)
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_INITIAL_UPDATE, 1000);

    uint64 l_Guid = 0;

    if (p_Battle->BattleType == PETBATTLE_TYPE_PVE)
        l_Guid = p_Battle->Teams[PETBATTLE_PVE_TEAM_ID]->OwnerGuid;

    uint32 l_NpcDisplayID = 0;
    uint32 l_NpcCreatureID = 0;
    uint16 l_WaitingForFrontPetsMaxSecs = 30;
    uint16 l_PvpMaxRoundTime = 30;
    uint8 l_CurPetBattleState = 1;
    bool l_CanAwardXP = p_Battle->BattleType != PETBATTLE_TYPE_PVP_DUEL;
    bool l_IsPVP = p_Battle->BattleType != PETBATTLE_TYPE_PVE;

    if (p_Battle->BattleType == PETBATTLE_TYPE_PVE && p_Battle->PveBattleType == PVE_PETBATTLE_TRAINER)
    {
        Creature* l_Trainer = ObjectAccessor::GetObjectInOrOutOfWorld(l_Guid, (Creature*)nullptr);

        if (l_Trainer)
        {
            l_NpcDisplayID = l_Trainer->GetDisplayId();
            l_NpcCreatureID = l_Trainer->GetEntry();
        }
    }

    for (uint8 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        bool l_IsPVP = p_Battle->BattleType != PETBATTLE_TYPE_PVE;
        uint16 l_PvpMaxRoundTime = l_IsPVP ? 30 : 0;

        ObjectGuid l_OwnerGuid = p_Battle->Teams[l_TeamID]->OwnerGuid;

        if (p_Battle->BattleType == PETBATTLE_TYPE_PVE && l_TeamID == PETBATTLE_PVE_TEAM_ID)
            l_OwnerGuid = 0;

        l_Packet.appendPackGUID(l_OwnerGuid);
        l_Packet << uint32(p_Battle->Teams[l_TeamID]->GetCatchAbilityID());
        l_Packet << uint32(l_TeamID == PETBATTLE_TEAM_1 ? 5 : 2); // dwordc
        l_Packet << uint16(l_PvpMaxRoundTime);

        if (l_TeamID == PETBATTLE_TEAM_1 || p_Battle->Teams[l_TeamID]->ActivePetID == PETBATTLE_NULL_ID)
            l_Packet << int8(p_Battle->Teams[l_TeamID]->ActivePetID);
        else
            l_Packet << int8(p_Battle->Teams[l_TeamID]->ActivePetID - (l_TeamID == PETBATTLE_TEAM_2 ? MAX_PETBATTLE_SLOTS : 0));
        l_Packet << uint8(6);

        l_Packet.WriteBits(p_Battle->Teams[l_TeamID]->TeamPetCount, 2);
        l_Packet.FlushBits();

        for (uint8 l_PetID = 0; l_PetID < p_Battle->Teams[l_TeamID]->TeamPetCount; l_PetID++)
        {
            std::shared_ptr<BattlePetInstance> l_Pet = p_Battle->Teams[l_TeamID]->TeamPets[l_PetID];

            ObjectGuid l_JournalID = (p_Battle->BattleType == PETBATTLE_TYPE_PVE && l_TeamID == PETBATTLE_PVE_TEAM_ID) ? 0 : l_Pet->JournalID;

            uint32 l_AbilityCount = 0;
            uint32 l_StatesCount = 0;

            for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
            {
                if (l_Pet->Abilities[l_AbilitySlot])
                    l_AbilityCount++;
            }

            for (int l_State = 0; l_State < NUM_BATTLEPET_STATES; ++l_State)
            {
                switch (l_State)
                {
                    case BATTLEPET_STATE_Stat_Power:
                    case BATTLEPET_STATE_Stat_Stamina:
                    case BATTLEPET_STATE_Stat_Speed:
                    case BATTLEPET_STATE_Stat_CritChance:
                    case BATTLEPET_STATE_Stat_Accuracy:
                        l_StatesCount++;
                        break;

                    default:
                        break;
                }
            }

            l_Packet.appendPackGUID(l_JournalID);
            l_Packet << uint32(l_Pet->Species);
            l_Packet << uint32(l_Pet->DisplayModelID);
            l_Packet << uint32(0);
            l_Packet << uint16(l_Pet->Level);
            l_Packet << uint16(l_Pet->XP);
            l_Packet << uint32(l_Pet->Health);
            l_Packet << uint32(l_Pet->InfoMaxHealth);
            l_Packet << uint32(l_Pet->InfoPower);
            l_Packet << uint32(l_Pet->InfoSpeed);
            l_Packet << uint32(0);
            l_Packet << uint16(l_Pet->Quality);
            l_Packet << uint16(l_Pet->Flags & ~PETBATTLE_FLAG_CAPTURED);
            l_Packet << uint8(l_PetID);
            l_Packet << uint32(l_AbilityCount);
            l_Packet << uint32(0);
            l_Packet << uint32(l_StatesCount);

            for (uint8 l_CurrentAbilitySlot = 0; l_CurrentAbilitySlot < MAX_PETBATTLE_ABILITIES; l_CurrentAbilitySlot++)
            {
                if (l_Pet->Abilities[l_CurrentAbilitySlot])
                {
                    l_Packet << uint32(l_Pet->Abilities[l_CurrentAbilitySlot]);
                    l_Packet << uint16(0/*l_Pet->Cooldowns[l_CurrentAbilitySlot]*/);    ///< Sending cooldown at -1 make client disable it
                    l_Packet << uint16(l_Pet->Lockdowns[l_CurrentAbilitySlot]);
                    l_Packet << uint8(l_CurrentAbilitySlot);
                    l_Packet << uint8(l_Pet->ID);
                }
            }

            for (int l_State = 0; l_State < NUM_BATTLEPET_STATES; ++l_State)
            {
                switch (l_State)
                {
                    case BATTLEPET_STATE_Stat_Power:
                    case BATTLEPET_STATE_Stat_Stamina:
                    case BATTLEPET_STATE_Stat_Speed:
                    case BATTLEPET_STATE_Stat_CritChance:
                    case BATTLEPET_STATE_Stat_Accuracy:
                        l_Packet << uint32(l_State);
                        l_Packet << int32(l_Pet->States[l_State]);
                        break;

                    default:
                        break;
                }
            }

            l_Packet.WriteBits(l_Pet->Name.length(), 7);
            l_Packet.FlushBits();

            l_Packet.WriteString(l_Pet->Name);
        }
    }

    for (int l_I = 0; l_I < 3; l_I++)
    {
        l_Packet << uint32(0);
        l_Packet << uint32(0);
    }

    l_Packet << uint16(l_WaitingForFrontPetsMaxSecs);
    l_Packet << uint16(l_PvpMaxRoundTime);
    l_Packet << uint32(p_Battle->Turn); ///< Turn ? always 0 here
    l_Packet << uint32(l_NpcCreatureID);
    l_Packet << uint32(l_NpcDisplayID);
    l_Packet << uint8(l_CurPetBattleState);
    l_Packet << uint8(p_Battle->GetForfeitHealthPenalityPct());

    l_Packet.WriteBit(l_IsPVP);
    l_Packet.WriteBit(l_CanAwardXP);
    l_Packet.FlushBits();

    l_Packet.appendPackGUID(l_Guid);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFirstRound(PetBattle* p_Battle)
{
    bool l_IsPVP = p_Battle->BattleType != PETBATTLE_TYPE_PVE;
    uint16 l_PvpMaxRoundTime = l_IsPVP ? 30 : 0;

    WorldPacket l_Packet(SMSG_PET_BATTLE_FIRST_ROUND, 100);

    uint32 l_CooldownCount = 0;

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1 || p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot] != 0)
                l_CooldownCount++;
        }
    }

    l_Packet << uint32(p_Battle->Turn);
    l_Packet << uint8(p_Battle->RoundResult);
    l_Packet << uint32(p_Battle->RoundEvents.size());

    for (uint32 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamInputFlags());  ///< NextInputFlags
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamTrapFlags());   ///< NextTrapStatus
        l_Packet << uint16(l_PvpMaxRoundTime);                              ///< RoundTimeSecs
    }

    l_Packet << uint32(l_CooldownCount);

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        l_Packet << uint32(l_EventIt->AbilityEffectID);
        l_Packet << uint16(l_EventIt->Flags);
        l_Packet << uint16(l_EventIt->BuffTurn);    ///< can be swap down
        l_Packet << uint16(l_EventIt->RoundTurn);   ///< can be swap up
        l_Packet << uint8(l_EventIt->EventType);
        l_Packet << uint8(l_EventIt->SourcePetID);
        l_Packet << uint8(l_EventIt->StackDepth);
        l_Packet << uint32(l_EventIt->Updates.size());

        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            l_Packet.WriteBits(l_UpdateIt->UpdateType, 3);
            l_Packet.FlushBits();

            l_Packet << uint8(l_UpdateIt->TargetPetID);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_BUFF)
            {
                l_Packet << uint32(l_UpdateIt->Buff.ID);
                l_Packet << uint32(l_UpdateIt->Buff.AbilityID);
                l_Packet << int32(l_UpdateIt->Buff.Duration);
                l_Packet << uint32(l_UpdateIt->Buff.Turn);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_STATE)
            {
                l_Packet << uint32(l_UpdateIt->State.ID);
                l_Packet << int32(l_UpdateIt->State.Value);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_HEALTH)
                l_Packet << int32(l_UpdateIt->Health);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_SPEED)
                l_Packet << int32(l_UpdateIt->Speed);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_TRIGGER)
                l_Packet << uint32(l_UpdateIt->TriggerAbilityId);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_ABILITY_CHANGE)
            {
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_NPC_EMOTE)
                l_Packet << int32(l_UpdateIt->NpcEmote.BroadcastTextID);
        }
    }

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1 || p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot] != 0)
            {
                l_Packet << uint32(p_Battle->Pets[l_CurrentPetslot]->Abilities[l_AbilitySlot]);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot]);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot]);
                l_Packet << uint8(l_AbilitySlot);
                l_Packet << uint8(p_Battle->Pets[l_CurrentPetslot]->ID);
            }
        }
    }

    l_Packet.WriteBits(p_Battle->RoundDeadPets.size(), 3);
    l_Packet.FlushBits();

    for (std::size_t l_I = 0; l_I < p_Battle->RoundDeadPets.size(); l_I++)
        l_Packet << uint8(p_Battle->RoundDeadPets[l_I]);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleRoundResult(PetBattle* p_Battle)
{
    bool l_IsPVP = p_Battle->BattleType != PETBATTLE_TYPE_PVE;
    uint16 l_PvpMaxRoundTime = l_IsPVP ? 30 : 0;

    WorldPacket l_Packet(SMSG_PET_BATTLE_ROUND_RESULT, 700);

    uint32 l_CooldownCount = 0;

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1 || p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot] != 0)
                l_CooldownCount++;
        }
    }

    l_Packet << uint32(p_Battle->Turn);
    l_Packet << uint8(p_Battle->RoundResult);
    l_Packet << uint32(p_Battle->RoundEvents.size());

    for (uint8 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamInputFlags());
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamTrapFlags());
        l_Packet << uint16(l_PvpMaxRoundTime);
    }

    l_Packet << uint32(l_CooldownCount);

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        l_Packet << uint32(l_EventIt->AbilityEffectID);
        l_Packet << uint16(l_EventIt->Flags);
        l_Packet << uint16(l_EventIt->BuffTurn);    ///< Can be swap down
        l_Packet << uint16(l_EventIt->RoundTurn);   ///< Can be swap up
        l_Packet << uint8(l_EventIt->EventType);
        l_Packet << uint8(l_EventIt->SourcePetID);
        l_Packet << uint8(l_EventIt->StackDepth);
        l_Packet << uint32(l_EventIt->Updates.size());

        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            l_Packet.WriteBits(l_UpdateIt->UpdateType, 3);
            l_Packet.FlushBits();

            l_Packet << uint8(l_UpdateIt->TargetPetID);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_BUFF)
            {
                l_Packet << uint32(l_UpdateIt->Buff.ID);
                l_Packet << uint32(l_UpdateIt->Buff.AbilityID);
                l_Packet << int32(l_UpdateIt->Buff.Duration);
                l_Packet << uint32(l_UpdateIt->Buff.Turn);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_STATE)
            {
                l_Packet << uint32(l_UpdateIt->State.ID);
                l_Packet << int32(l_UpdateIt->State.Value);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_HEALTH)
                l_Packet << int32(l_UpdateIt->Health);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_SPEED)
                l_Packet << int32(l_UpdateIt->Speed);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_TRIGGER)
                l_Packet << uint32(l_UpdateIt->TriggerAbilityId);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_ABILITY_CHANGE)
            {
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_NPC_EMOTE)
                l_Packet << int32(l_UpdateIt->NpcEmote.BroadcastTextID);
        }
    }

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1 || p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot] != 0)
            {
                l_Packet << uint32(p_Battle->Pets[l_CurrentPetslot]->Abilities[l_AbilitySlot]);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot]);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot]);
                l_Packet << uint8(l_AbilitySlot);
                l_Packet << uint8(p_Battle->Pets[l_CurrentPetslot]->ID);
            }
        }
    }

    l_Packet.WriteBits(p_Battle->RoundDeadPets.size(), 3);
    l_Packet.FlushBits();

    for (std::size_t l_I = 0; l_I < p_Battle->RoundDeadPets.size(); l_I++)
        l_Packet << uint8(p_Battle->RoundDeadPets[l_I]);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleReplacementMade(PetBattle* p_Battle)
{
    bool l_IsPVP = p_Battle->BattleType != PETBATTLE_TYPE_PVE;
    uint16 l_PvpMaxRoundTime = l_IsPVP ? 30 : 0;

    WorldPacket l_Packet(SMSG_PET_BATTLE_REPLACEMENTS_MADE, 700);

    uint32 l_CooldownCount = 0;

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1 || p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot] != 0)
                l_CooldownCount++;
        }
    }

    l_Packet << uint32(p_Battle->Turn);
    l_Packet << uint8(p_Battle->RoundResult);
    l_Packet << uint32(p_Battle->RoundEvents.size());

    for (uint8 l_TeamID = 0; l_TeamID < MAX_PETBATTLE_TEAM; l_TeamID++)
    {
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamInputFlags());
        l_Packet << uint8(p_Battle->Teams[l_TeamID]->GetTeamTrapFlags());
        l_Packet << uint16(l_PvpMaxRoundTime);
    }

    l_Packet << uint32(l_CooldownCount);

    for (std::list<PetBattleEvent>::iterator l_EventIt = p_Battle->RoundEvents.begin(); l_EventIt != p_Battle->RoundEvents.end(); ++l_EventIt)
    {
        l_Packet << uint32(l_EventIt->AbilityEffectID);
        l_Packet << uint16(l_EventIt->Flags);
        l_Packet << uint16(l_EventIt->BuffTurn);    ///< Can be swap down
        l_Packet << uint16(l_EventIt->RoundTurn);   ///< Can be swap up
        l_Packet << uint8(l_EventIt->EventType);
        l_Packet << uint8(l_EventIt->SourcePetID);
        l_Packet << uint8(l_EventIt->StackDepth);
        l_Packet << uint32(l_EventIt->Updates.size());

        for (std::list<PetBattleEventUpdate>::iterator l_UpdateIt = l_EventIt->Updates.begin(); l_UpdateIt != l_EventIt->Updates.end(); ++l_UpdateIt)
        {
            l_Packet.WriteBits(l_UpdateIt->UpdateType, 3);
            l_Packet.FlushBits();

            l_Packet << uint8(l_UpdateIt->TargetPetID);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_BUFF)
            {
                l_Packet << uint32(l_UpdateIt->Buff.ID);
                l_Packet << uint32(l_UpdateIt->Buff.AbilityID);
                l_Packet << int32(l_UpdateIt->Buff.Duration);
                l_Packet << uint32(l_UpdateIt->Buff.Turn);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_STATE)
            {
                l_Packet << uint32(l_UpdateIt->State.ID);
                l_Packet << int32(l_UpdateIt->State.Value);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_HEALTH)
                l_Packet << int32(l_UpdateIt->Health);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_SPEED)
                l_Packet << int32(l_UpdateIt->Speed);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_TRIGGER)
                l_Packet << uint32(l_UpdateIt->TriggerAbilityId);

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_ABILITY_CHANGE)
            {
                l_Packet << uint32(0);
                l_Packet << uint32(0);
                l_Packet << uint32(0);
            }

            if (l_UpdateIt->UpdateType == PETBATTLE_EVENT_UPDATE_NPC_EMOTE)
                l_Packet << int32(l_UpdateIt->NpcEmote.BroadcastTextID);
        }
    }

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        for (uint8 l_AbilitySlot = 0; l_AbilitySlot < MAX_PETBATTLE_ABILITIES; l_AbilitySlot++)
        {
            if (p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot] != -1 || p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot] != 0)
            {
                l_Packet << uint32(p_Battle->Pets[l_CurrentPetslot]->Abilities[l_AbilitySlot]);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Cooldowns[l_AbilitySlot]);
                l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Lockdowns[l_AbilitySlot]);
                l_Packet << uint8(l_AbilitySlot);
                l_Packet << uint8(p_Battle->Pets[l_CurrentPetslot]->ID);
            }
        }
    }

    l_Packet.WriteBits(p_Battle->RoundDeadPets.size(), 3);
    l_Packet.FlushBits();

    for (std::size_t l_I = 0; l_I < p_Battle->RoundDeadPets.size(); l_I++)
        l_Packet << uint8(p_Battle->RoundDeadPets[l_I]);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFinalRound(PetBattle* p_Battle)
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_FINAL_ROUND, 200);

    l_Packet.WriteBit(p_Battle->CombatResult == PETBATTLE_RESULT_ABANDON);
    l_Packet.WriteBit(p_Battle->BattleType != PETBATTLE_TYPE_PVE);

    for (uint8 l_TeamId = 0; l_TeamId < MAX_PETBATTLE_TEAM; ++l_TeamId)
        l_Packet.WriteBit(p_Battle->WinnerTeamId == l_TeamId);

    l_Packet.FlushBits();

    for (uint8 l_TeamId = 0; l_TeamId < MAX_PETBATTLE_TEAM; ++l_TeamId)
        l_Packet << uint32(0);

    l_Packet << uint32(p_Battle->TotalPetCount);

    for (uint8 l_CurrentPetslot = 0; l_CurrentPetslot < (MAX_PETBATTLE_TEAM * MAX_PETBATTLE_SLOTS); l_CurrentPetslot++)
    {
        if (!p_Battle->Pets[l_CurrentPetslot])
            continue;

        l_Packet.appendPackGUID(0);
        l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->Level);
        l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->XP);
        l_Packet << int32(p_Battle->Pets[l_CurrentPetslot]->Health);
        l_Packet << int32(p_Battle->Pets[l_CurrentPetslot]->InfoMaxHealth);
        l_Packet << uint16(p_Battle->Pets[l_CurrentPetslot]->OldLevel);
        l_Packet << uint8(p_Battle->Pets[l_CurrentPetslot]->ID);

        l_Packet.WriteBit(p_Battle->CatchedPetId == l_CurrentPetslot);
        l_Packet.WriteBit(false);                                           ///< Caged
        l_Packet.WriteBit(p_Battle->Pets[l_CurrentPetslot]->OldXP != p_Battle->Pets[l_CurrentPetslot]->XP);
        l_Packet.WriteBit(p_Battle->FightedPets.find(l_CurrentPetslot) != p_Battle->FightedPets.end());
        l_Packet.FlushBits();
    }

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleFinished(PetBattle* /*battle*/)
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_FINISHED, 0);
    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleChatRestricted()
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_CHAT_RESTRICTED, 0);
    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleMaxGameLenghtWarning()
{
    /// @TODO
}

void WorldSession::SendPetBattleQueueProposeMatch()
{
    WorldPacket l_Packet(SMSG_PET_BATTLE_QUEUE_PROPOSE_MATCH, 0);
    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleQueueStatus(uint32 p_TicketTime, uint32 p_TicketID, uint32 p_Status, uint32 p_AvgWaitTime)
{
    if (!m_Player || !m_Player->GetSession())
        return;

    ObjectGuid l_TicketRequesterGuid = m_Player->GetGUID();
    uint32 l_QueuedTime = time(nullptr) - p_TicketTime;

    WorldPacket l_Packet(SMSG_PET_BATTLE_QUEUE_STATUS, 50);

    l_Packet << uint32(p_Status);
    l_Packet << uint32(0);          ///< Pet status

    l_Packet.appendPackGUID(l_TicketRequesterGuid);
    l_Packet << uint32(p_TicketID);
    l_Packet << uint32(6);                              ///< Ticket type
    l_Packet << uint32(p_TicketTime);

    l_Packet.WriteBit(p_AvgWaitTime != 0);
    l_Packet.WriteBit(l_QueuedTime != 0);
    l_Packet.FlushBits();

    if (p_AvgWaitTime)
        l_Packet << uint32(p_Status != LFBUpdateStatus::LFB_LEAVE_QUEUE ? p_AvgWaitTime : 0);

    if (l_QueuedTime)
        l_Packet << uint32(p_Status != LFBUpdateStatus::LFB_LEAVE_QUEUE ? l_QueuedTime : 0);

    SendPacket(&l_Packet);
}

void WorldSession::SendPetBattleDebugQueueDumpResponse()
{
    /// @TODO
}

//////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void WorldSession::HandlePetBattleJoinQueue(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    // Player can't be already in battle
    if (m_Player->_petBattleId)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_IN_BATTLE);
        return;
    }

    // Player can't be in combat
    if (m_Player->isInCombat())
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_NOT_WHILE_IN_COMBAT);
        return;
    }

    BattlePetInstance::Ptr  l_PlayerPets[MAX_PETBATTLE_SLOTS];
    size_t                  l_PlayerPetCount = 0;
    uint32                  l_ErrorCode = PETBATTLE_REQUEST_CREATE_FAILED;

    // Temporary pet buffer
    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
        l_PlayerPets[l_CurrentPetSlot] = 0;

    // Load player pets
    BattlePet::Ptr * l_PetSlots = m_Player->GetBattlePetCombatTeam();

    uint32 l_DeadPetCount = 0;

    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        if (!l_PetSlots[l_CurrentPetSlot])
            continue;

        if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= m_Player->GetUnlockedPetBattleSlot())
            break;

        if (l_PetSlots[l_CurrentPetSlot]->Health == 0)
            l_DeadPetCount++;

        l_PlayerPets[l_PlayerPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
        l_PlayerPets[l_PlayerPetCount]->CloneFrom(l_PetSlots[l_CurrentPetSlot]);
        l_PlayerPets[l_PlayerPetCount]->Slot = l_PlayerPetCount;
        l_PlayerPets[l_PlayerPetCount]->OriginalBattlePet = l_PetSlots[l_CurrentPetSlot];

        ++l_PlayerPetCount;
    }

    if (l_DeadPetCount && l_DeadPetCount == l_PlayerPetCount)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_ALL_PETS_DEAD);
        return;
    }

    // Check player team
    if (!l_PlayerPetCount)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_NO_PETS_IN_SLOT);
        return;
    }

    sPetBattleSystem->JoinQueue(m_Player);
}

void WorldSession::HandlePetBattleLeaveQueue(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    sPetBattleSystem->LeaveQueue(m_Player);
}

void WorldSession::HandlePetBattleRequestWild(WorldPacket& p_RecvData)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    if (m_IsPetBattleJournalLocked)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_NO_ACCOUNT_LOCK);
        return;
    }

    PetBattleRequest* l_BattleRequest = sPetBattleSystem->CreateRequest(m_Player->GetGUID());
    uint64 l_OpponentGuid;

    p_RecvData.readPackGUID(l_OpponentGuid);
    p_RecvData >> l_BattleRequest->LocationResult;
    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[0];
    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[1];
    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[2];
    p_RecvData >> l_BattleRequest->BattleFacing;

    for (int l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; l_CurrentTeamID++)
        p_RecvData >> l_BattleRequest->TeamPosition[l_CurrentTeamID][0] >> l_BattleRequest->TeamPosition[l_CurrentTeamID][1] >> l_BattleRequest->TeamPosition[l_CurrentTeamID][2];

    l_BattleRequest->RequestType    = PETBATTLE_TYPE_PVE;
    l_BattleRequest->OpponentGuid   = l_OpponentGuid;

    eBattlePetRequests l_CanEnterResult = sPetBattleSystem->CanPlayerEnterInPetBattle(m_Player, l_BattleRequest);
    if (l_CanEnterResult != PETBATTLE_REQUEST_OK)
    {
        SendPetBattleRequestFailed(l_CanEnterResult);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    /// Wild should be for PetBattle and Player able to interact with it
    Creature* l_WildPet = m_Player->GetNPCIfCanInteractWith(l_BattleRequest->OpponentGuid, UNIT_NPC_FLAG_PETBATTLE);
    if (!l_WildPet)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_TARGET_NOT_CAPTURABLE);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    ////////////////////////////////
    ////////////////////////////////

    Creature* l_Wild = sObjectAccessor->GetCreature(*m_Player, l_BattleRequest->OpponentGuid);

    if (!l_Wild)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_TARGET_NOT_CAPTURABLE);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    ////////////////////////////////
    ////////////////////////////////

    /// Wild should be wild
    if (!sWildBattlePetMgr->IsWildPet(l_Wild))
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_TARGET_NOT_CAPTURABLE);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    ////////////////////////////////
    ////////////////////////////////

    BattlePetInstance::Ptr  l_PlayerPets[MAX_PETBATTLE_SLOTS];
    BattlePetInstance::Ptr  l_WildBattlePets[MAX_PETBATTLE_SLOTS];
    size_t                  l_PlayerPetCount = 0;
    PetBattle*              l_Battle;

    /// Temporary pet buffer
    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        l_PlayerPets[l_CurrentPetSlot] = nullptr;
        l_WildBattlePets[l_CurrentPetSlot] = nullptr;
    }

    ////////////////////////////////
    ////////////////////////////////

    BattlePetInstance::Ptr l_WildBattlePet = sWildBattlePetMgr->GetWildBattlePet(l_Wild);

    if (!l_WildBattlePet)
    {
        l_WildBattlePet = nullptr;

        SendPetBattleRequestFailed(PETBATTLE_REQUEST_TARGET_NOT_CAPTURABLE);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    l_WildBattlePets[0] = l_WildBattlePet;

    std::list<Unit*> l_NearCreature;
    JadeCore::AnyFriendlyUnitInObjectRangeCheck l_UCheck(l_Wild, l_Wild, 50.0f);
    JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(l_Wild, l_NearCreature, l_UCheck);
    l_Wild->VisitNearbyObject(40.0f, l_Searcher);

    uint32 l_WildsPetCount = 1;
    for (Unit* l_Current : l_NearCreature)
    {
        if (l_WildsPetCount >= MAX_PETBATTLE_SLOTS)
            break;

        if (!l_Current->ToCreature() || !l_Current->isAlive() || l_Current->GetGUID() == l_Wild->GetGUID() || !sWildBattlePetMgr->IsWildPet(l_Current->ToCreature()))
            continue;

        if (sWildBattlePetMgr->GetWildBattlePet(l_Current->ToCreature()) != nullptr && roll_chance_i(80))
        {
            l_WildBattlePets[l_WildsPetCount] = sWildBattlePetMgr->GetWildBattlePet(l_Current->ToCreature());
            l_WildBattlePets[l_WildsPetCount]->OriginalCreature = l_Current->GetGUID();
            l_WildsPetCount++;
        }
    }

    ////////////////////////////////
    ////////////////////////////////

    /// Load player pets
    BattlePet::Ptr * l_PetSlots = m_Player->GetBattlePetCombatTeam();

    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        if (!l_PetSlots[l_CurrentPetSlot])
            continue;

        if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= m_Player->GetUnlockedPetBattleSlot())
            break;

        l_PlayerPets[l_PlayerPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
        l_PlayerPets[l_PlayerPetCount]->CloneFrom(l_PetSlots[l_CurrentPetSlot]);
        l_PlayerPets[l_PlayerPetCount]->Slot                = l_PlayerPetCount;
        l_PlayerPets[l_PlayerPetCount]->OriginalBattlePet   = l_PetSlots[l_CurrentPetSlot];

        ++l_PlayerPetCount;
    }

    ////////////////////////////////
    ////////////////////////////////

    m_Player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC); ///< Immuned only to NPC
    m_Player->SetTarget(l_WildPet->GetGUID());

    SendPetBattleFinalizeLocation(l_BattleRequest);

    m_Player->SetFacingTo(m_Player->GetAngle(l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][0], l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][1]));
    m_Player->SetRooted(true);

    /// Ok, Create battle
    l_Battle = sPetBattleSystem->CreateBattle();

    /// Add player pets
    l_Battle->Teams[PETBATTLE_TEAM_1]->OwnerGuid    = m_Player->GetGUID();
    l_Battle->Teams[PETBATTLE_TEAM_1]->PlayerGuid   = m_Player->GetGUID();

    l_Battle->Teams[PETBATTLE_TEAM_2]->OwnerGuid = l_Wild->GetGUID();

    for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
    {
        if (l_PlayerPets[l_CurrentPetSlot])
            l_Battle->AddPet(PETBATTLE_TEAM_1, l_PlayerPets[l_CurrentPetSlot]);

        if (l_WildBattlePets[l_CurrentPetSlot])
        {
            l_Battle->AddPet(PETBATTLE_TEAM_2, l_WildBattlePets[l_CurrentPetSlot]);

            if (Creature* l_CurrrentCreature = sObjectAccessor->GetCreature(*m_Player, l_WildBattlePets[l_CurrentPetSlot]->OriginalCreature))
            {
                /// Ok, Despawn wild
                l_CurrrentCreature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_PC); // Immuned to all
                l_CurrrentCreature->SetTarget(m_Player->GetGUID());
                l_CurrrentCreature->SetControlled(true, UNIT_STATE_ROOT);
                l_CurrrentCreature->_petBattleId = l_Battle->ID;

                sWildBattlePetMgr->EnterInBattle(l_CurrrentCreature);
            }
        }
    }

    l_Battle->BattleType    = l_BattleRequest->RequestType;
    l_Battle->PveBattleType = PVE_PETBATTLE_WILD;

    /// Launch battle
    m_Player->_petBattleId = l_Battle->ID;
    l_Battle->Begin();

    sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);

    for (size_t l_CurrentPetID = 0; l_CurrentPetID < MAX_PETBATTLE_SLOTS; ++l_CurrentPetID)
    {
        if (l_PlayerPets[l_CurrentPetID])
            l_PlayerPets[l_CurrentPetID] = nullptr;

        if (l_WildBattlePets[l_CurrentPetID])
            l_WildBattlePets[l_CurrentPetID] = nullptr;
    }

    l_WildBattlePet = nullptr;
}

void WorldSession::HandlePetBattleWildLocationFail(WorldPacket& /*p_RecvData*/)
{

}

void WorldSession::HandlePetBattleRequestPvP(WorldPacket& p_RecvData)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    PetBattleRequest* l_BattleRequest = sPetBattleSystem->CreateRequest(m_Player->GetGUID());
    uint64 l_OpponentGuid = 0;

    p_RecvData.readPackGUID(l_OpponentGuid);

    p_RecvData >> l_BattleRequest->LocationResult;
    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[0];
    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[1];
    p_RecvData >> l_BattleRequest->PetBattleCenterPosition[2];
    p_RecvData >> l_BattleRequest->BattleFacing;

    for (int l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; l_CurrentTeamID++)
        p_RecvData >> l_BattleRequest->TeamPosition[l_CurrentTeamID][0] >> l_BattleRequest->TeamPosition[l_CurrentTeamID][1] >> l_BattleRequest->TeamPosition[l_CurrentTeamID][2];

    l_BattleRequest->RequestType    = PETBATTLE_TYPE_PVP_DUEL;
    l_BattleRequest->OpponentGuid   = l_OpponentGuid;

    // Player can't be already in battle
    if (m_Player->_petBattleId)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_IN_BATTLE);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    // Player can't be in combat
    if (m_Player->isInCombat())
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_NOT_WHILE_IN_COMBAT);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    // Check positions
    for (size_t l_CurrentTeamID = 0; l_CurrentTeamID < MAX_PETBATTLE_TEAM; ++l_CurrentTeamID)
    {
        if (m_Player->GetMap()->getObjectHitPos(m_Player->GetPhaseMask(), l_BattleRequest->PetBattleCenterPosition[0],        l_BattleRequest->PetBattleCenterPosition[1],        l_BattleRequest->PetBattleCenterPosition[2],
                                                                          l_BattleRequest->TeamPosition[l_CurrentTeamID][0],  l_BattleRequest->TeamPosition[l_CurrentTeamID][1],  l_BattleRequest->TeamPosition[l_CurrentTeamID][2],
                                                                          l_BattleRequest->TeamPosition[l_CurrentTeamID][0],  l_BattleRequest->TeamPosition[l_CurrentTeamID][1],  l_BattleRequest->TeamPosition[l_CurrentTeamID][2], 0.0f))
        {
            SendPetBattleRequestFailed(PETBATTLE_REQUEST_NOT_HERE_UNEVEN_GROUND);
            sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
            return;
        }
    }

    Player* l_Opposant = HashMapHolder<Player>::Find(l_OpponentGuid);
    if (!l_Opposant)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_TARGET_INVALID);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    // Player can't be already in battle
    if (l_Opposant->_petBattleId)
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_IN_BATTLE);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    // Player can't be in combat
    if (l_Opposant->isInCombat())
    {
        SendPetBattleRequestFailed(PETBATTLE_REQUEST_NOT_WHILE_IN_COMBAT);
        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);
        return;
    }

    l_BattleRequest->IsPvPReady[PETBATTLE_TEAM_1] = true;
    l_Opposant->GetSession()->SendPetBattlePvPChallenge(l_BattleRequest);
}

void WorldSession::HandlePetBattleRequestUpdate(WorldPacket& p_RecvData)
{
    uint64 l_TargetGUID = 0;
    bool l_Canceled = false;

    p_RecvData.readPackGUID(l_TargetGUID);
    l_Canceled = p_RecvData.ReadBit();

    PetBattleRequest* l_BattleRequest = sPetBattleSystem->GetRequest(l_TargetGUID);
    Player* l_Opposant = HashMapHolder<Player>::Find(l_TargetGUID);

    if (!l_Canceled && l_BattleRequest && l_Opposant)
    {
        m_Player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC); // Immuned only to NPC
        l_Opposant->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_PACIFIED | UNIT_FLAG_IMMUNE_TO_NPC); // Immuned only to NPC

        BattlePetInstance::Ptr  l_PlayerPets[MAX_PETBATTLE_SLOTS];
        BattlePetInstance::Ptr  l_PlayerOpposantPets[MAX_PETBATTLE_SLOTS];
        size_t                  l_PlayerPetCount = 0;
        size_t                  l_PlayerOpposantPetCount = 0;
        PetBattle*              l_Battle;
        uint32                  l_ErrorCode = PETBATTLE_REQUEST_CREATE_FAILED;

        // Temporary pet buffer
        for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
        {
            l_PlayerPets[l_CurrentPetSlot] = 0;
            l_PlayerOpposantPets[l_CurrentPetSlot] = 0;
        }

        // Load player pets
        BattlePet::Ptr * l_PetSlots = m_Player->GetBattlePetCombatTeam();

        for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
        {
            if (!l_PetSlots[l_CurrentPetSlot])
                continue;

            if (l_PlayerPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerPetCount >= m_Player->GetUnlockedPetBattleSlot())
                break;

            l_PlayerPets[l_PlayerPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
            l_PlayerPets[l_PlayerPetCount]->CloneFrom(l_PetSlots[l_CurrentPetSlot]);
            l_PlayerPets[l_PlayerPetCount]->Slot = l_PlayerPetCount;
            l_PlayerPets[l_PlayerPetCount]->OriginalBattlePet = l_PetSlots[l_CurrentPetSlot];

            ++l_PlayerPetCount;
        }

        BattlePet::Ptr * l_PetOpposantSlots = l_Opposant->GetBattlePetCombatTeam();

        for (size_t l_CurrentOpposantPetSlot = 0; l_CurrentOpposantPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentOpposantPetSlot)
        {
            if (!l_PetOpposantSlots[l_CurrentOpposantPetSlot])
                continue;

            if (l_PlayerOpposantPetCount >= MAX_PETBATTLE_SLOTS || l_PlayerOpposantPetCount >= m_Player->GetUnlockedPetBattleSlot())
                break;

            l_PlayerOpposantPets[l_PlayerOpposantPetCount] = BattlePetInstance::Ptr(new BattlePetInstance());
            l_PlayerOpposantPets[l_PlayerOpposantPetCount]->CloneFrom(l_PetOpposantSlots[l_CurrentOpposantPetSlot]);
            l_PlayerOpposantPets[l_PlayerOpposantPetCount]->Slot = l_PlayerOpposantPetCount;
            l_PlayerOpposantPets[l_PlayerOpposantPetCount]->OriginalBattlePet = l_PetOpposantSlots[l_CurrentOpposantPetSlot];

            ++l_PlayerOpposantPetCount;
        }

        if (!l_PlayerOpposantPetCount || !l_PlayerPetCount)
        {
            m_Player->GetSession()->SendPetBattleRequestFailed(PETBATTLE_REQUEST_NO_PETS_IN_SLOT);
            l_Opposant->GetSession()->SendPetBattleRequestFailed(PETBATTLE_REQUEST_NO_PETS_IN_SLOT);
            sPetBattleSystem->RemoveRequest(l_TargetGUID);
            return;
        }

        m_Player->GetSession()->SendPetBattleFinalizeLocation(l_BattleRequest);
        l_Opposant->GetSession()->SendPetBattleFinalizeLocation(l_BattleRequest);

        m_Player->SetFacingTo(m_Player->GetAngle(l_BattleRequest->TeamPosition[PETBATTLE_TEAM_1][0], l_BattleRequest->TeamPosition[PETBATTLE_TEAM_1][1]));
        l_Opposant->SetFacingTo(m_Player->GetAngle(l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][0], l_BattleRequest->TeamPosition[PETBATTLE_TEAM_2][1]));
        m_Player->SetRooted(true);
        l_Opposant->SetRooted(true);

        // Ok, Create battle
        l_Battle = sPetBattleSystem->CreateBattle();

        // Add player pets
        l_Battle->Teams[PETBATTLE_TEAM_1]->OwnerGuid = l_Opposant->GetGUID();
        l_Battle->Teams[PETBATTLE_TEAM_1]->PlayerGuid = l_Opposant->GetGUID();
        l_Battle->Teams[PETBATTLE_TEAM_2]->OwnerGuid = m_Player->GetGUID();
        l_Battle->Teams[PETBATTLE_TEAM_2]->PlayerGuid = m_Player->GetGUID();

        for (size_t l_CurrentPetSlot = 0; l_CurrentPetSlot < MAX_PETBATTLE_SLOTS; ++l_CurrentPetSlot)
        {
            if (l_PlayerOpposantPets[l_CurrentPetSlot])
                l_Battle->AddPet(PETBATTLE_TEAM_1, l_PlayerOpposantPets[l_CurrentPetSlot]);

            if (l_PlayerPets[l_CurrentPetSlot])
                l_Battle->AddPet(PETBATTLE_TEAM_2, l_PlayerPets[l_CurrentPetSlot]);
        }

        l_Battle->BattleType = l_BattleRequest->RequestType;

        // Launch battle
        m_Player->_petBattleId = l_Battle->ID;
        l_Opposant->_petBattleId = l_Battle->ID;
        l_Battle->Begin();

        sPetBattleSystem->RemoveRequest(l_BattleRequest->RequesterGuid);

        for (size_t l_CurrentPetID = 0; l_CurrentPetID < MAX_PETBATTLE_SLOTS; ++l_CurrentPetID)
        {
            if (l_PlayerPets[l_CurrentPetID])
                l_PlayerPets[l_CurrentPetID] = BattlePetInstance::Ptr();

            if (l_PlayerOpposantPets[l_CurrentPetID])
                l_PlayerOpposantPets[l_CurrentPetID] = BattlePetInstance::Ptr();
        }
    }
    else
    {
        if (l_Opposant)
            l_Opposant->GetSession()->SendPetBattleRequestFailed(PETBATTLE_REQUEST_DECLINED);
        sPetBattleSystem->RemoveRequest(l_TargetGUID);
    }
}

void WorldSession::HandlePetBattleQuitNotify(WorldPacket& /*p_RecvData*/)
{
    /// @TODO
}

void WorldSession::HandlePetBattleFinalNotify(WorldPacket& /*p_RecvData*/)
{
    /// @TODO
}

/// [INTERNAL]
void WorldSession::HandlePetBattleScriptErrorNotify(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

void WorldSession::HandlePetBattleQueueProposeMatchResult(WorldPacket& p_RecvData)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    sPetBattleSystem->ProposalResponse(m_Player, p_RecvData.ReadBit());
}

/// [INTERNAL]
void WorldSession::HandlePetBattleFirstPet(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

enum ePetBattleActions
{
    PETBATTLE_ACTION_REQUEST_LEAVE      = 0,
    PETBATTLE_ACTION_CAST               = 1,
    PETBATTLE_ACTION_SWAP_OR_PASS       = 2,
    PETBATTLE_ACTION_CATCH              = 3,
    PETBATTLE_ACTION_LEAVE_PETBATTLE    = 4
};

void WorldSession::HandlePetBattleInput(WorldPacket& p_RecvData)
{
    uint8   l_Action = 0;
    uint8   l_NewFrontPetID = 0;
    uint32  l_Ability = 0;
    uint32  l_Turn = 0;

    p_RecvData >> l_Action;
    p_RecvData >> l_NewFrontPetID;
    p_RecvData.read_skip<uint8>();
    p_RecvData.read_skip<uint8>();
    p_RecvData >> l_Ability;
    p_RecvData >> l_Turn;

    p_RecvData.ReadBit();

    if (l_Action == PETBATTLE_ACTION_LEAVE_PETBATTLE)
    {
        SendPetBattleFinished(0);
        return;
    }

    if (!m_Player->_petBattleId)
    {
        SendPetBattleFinished(0);
        return;
    }

    PetBattle * l_PetBattle = sPetBattleSystem->GetBattle(m_Player->_petBattleId);

    if (!l_PetBattle || l_PetBattle->BattleStatus == PETBATTLE_STATUS_FINISHED)
    {
        SendPetBattleFinished(0);
        return;
    }

    ///// Check sync
    if ((l_Turn + 1) != l_PetBattle->Turn)
    {
        sPetBattleSystem->ForfeitBattle(l_PetBattle->ID, m_Player->GetGUID());
        return;
    }

    if (l_Action)
    {
        uint32 l_PlayerTeamID = 0;

        if (l_PetBattle->Teams[PETBATTLE_TEAM_2]->PlayerGuid == m_Player->GetGUID())
            l_PlayerTeamID = PETBATTLE_TEAM_2;

        // Skip if input already died
        if (l_PetBattle->Teams[l_PlayerTeamID]->Ready)
            return;

        if (l_Action == PETBATTLE_ACTION_REQUEST_LEAVE)
        {
            sPetBattleSystem->ForfeitBattle(l_PetBattle->ID, m_Player->GetGUID());
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

void WorldSession::HandlePetBattleReplaceFrontPet(WorldPacket& p_RecvData)
{
    if (!m_Player->_petBattleId)
    {
        SendPetBattleFinished(0);
        return;
    }

    PetBattle* l_PetBattle = sPetBattleSystem->GetBattle(m_Player->_petBattleId);

    if (!l_PetBattle || l_PetBattle->BattleStatus == PETBATTLE_STATUS_FINISHED)
    {
        SendPetBattleFinished(0);
        return;
    }

    uint8 l_NewFrontPetID = 0;
    uint32 l_PlayerTeamID = 0;

    p_RecvData >> l_NewFrontPetID;

    if (l_PetBattle->Teams[PETBATTLE_TEAM_2]->PlayerGuid == m_Player->GetGUID())
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

/// [INTERNAL]
void WorldSession::HandlePetBattleDebugQueueDump(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}
