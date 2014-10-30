/*
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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "ObjectAccessor.h"
#include "CreatureAI.h"
#include "Util.h"
#include "Pet.h"
#include "World.h"
#include "Group.h"
#include "SpellInfo.h"
#include "SpellAuraEffects.h"

void WorldSession::HandleDismissCritter(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_DISMISS_CRITTER for GUID " UI64FMTD, guid);

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, guid);

    if (!pet)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "Vanitypet (guid: %u) does not exist - player '%s' (guid: %u / account: %u) attempted to dismiss it (possibly lagged out)",
                uint32(GUID_LOPART(guid)), GetPlayer()->GetName(), GetPlayer()->GetGUIDLow(), GetAccountId());
        return;
    }

    if (m_Player->GetCritterGUID() == pet->GetGUID())
    {
         if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isSummon())
             pet->ToTempSummon()->UnSummon();
    }
}

void WorldSession::HandlePetAction(WorldPacket & recvData)
{
    ObjectGuid guid1;
    ObjectGuid guid2;
    uint32 data;
    float x, y, z;
    recvData >> data;

    // Position
    recvData >> x;
    recvData >> z;
    recvData >> y;

    guid1[0] = recvData.ReadBit();
    guid1[2] = recvData.ReadBit();
    guid1[6] = recvData.ReadBit();
    guid1[1] = recvData.ReadBit();
    guid2[6] = recvData.ReadBit();
    guid2[4] = recvData.ReadBit();
    guid2[0] = recvData.ReadBit();
    guid2[1] = recvData.ReadBit();
    guid2[3] = recvData.ReadBit();
    guid1[3] = recvData.ReadBit();
    guid2[2] = recvData.ReadBit();
    guid1[4] = recvData.ReadBit();
    guid2[5] = recvData.ReadBit();
    guid2[7] = recvData.ReadBit();
    guid1[5] = recvData.ReadBit();
    guid1[7] = recvData.ReadBit();
    recvData.FlushBits();

    recvData.ReadByteSeq(guid2[7]);
    recvData.ReadByteSeq(guid1[7]);
    recvData.ReadByteSeq(guid2[6]);
    recvData.ReadByteSeq(guid2[0]);
    recvData.ReadByteSeq(guid2[3]);
    recvData.ReadByteSeq(guid1[1]);
    recvData.ReadByteSeq(guid2[2]);
    recvData.ReadByteSeq(guid2[1]);
    recvData.ReadByteSeq(guid1[2]);
    recvData.ReadByteSeq(guid1[5]);
    recvData.ReadByteSeq(guid1[6]);
    recvData.ReadByteSeq(guid1[0]);
    recvData.ReadByteSeq(guid1[3]);
    recvData.ReadByteSeq(guid2[4]);
    recvData.ReadByteSeq(guid2[5]);
    recvData.ReadByteSeq(guid1[4]);

    uint32 spellid = UNIT_ACTION_BUTTON_ACTION(data);
    uint8 flag = UNIT_ACTION_BUTTON_TYPE(data);             //delete = 0x07 CastSpell = C1

    // used also for charmed creature
    Unit* pet= ObjectAccessor::GetUnit(*m_Player, guid1);
    sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet %u - flag: %u, spellid: %u, target: %u.", uint32(GUID_LOPART(guid1)), uint32(flag), spellid, uint32(GUID_LOPART(guid2)));

    if (!pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) doesn't exist for player '%s'", uint32(GUID_LOPART(guid1)), GetPlayer()->GetName());
        return;
    }

    if (pet != GetPlayer()->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) does not belong to player '%s'", uint32(GUID_LOPART(guid1)), GetPlayer()->GetName());
        return;
    }

    if (!pet->isAlive())
    {
        SpellInfo const* spell = (flag == ACT_ENABLED || flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(spellid) : NULL;
        if (!spell)
            return;
        if (!(spell->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            return;
    }

    //TODO: allow control charmed player?
    if (pet->GetTypeId() == TYPEID_PLAYER && !(flag == ACT_COMMAND && spellid == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(pet, guid1, spellid, flag, guid2, x, y ,z);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> controlled;
        for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
            if ((*itr)->GetEntry() == pet->GetEntry() && (*itr)->isAlive())
                controlled.push_back(*itr);
        for (std::vector<Unit*>::iterator itr = controlled.begin(); itr != controlled.end(); ++itr)
            HandlePetActionHelper(*itr, guid1, spellid, flag, guid2, x, y, z);
    }
}

void WorldSession::HandlePetStopAttack(WorldPacket &recvData)
{
    ObjectGuid guid;
    uint8 bitOrder[8] = {4, 2, 0, 6, 1, 7, 5, 3};
    recvData.ReadBitInOrder(guid, bitOrder);
    uint8 byteOrder[8] = {3, 2, 4, 0, 7, 6, 5, 1};
    recvData.ReadBytesSeq(guid, byteOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PET_STOP_ATTACK");

    Unit* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, guid);

    if (!pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetStopAttack: Pet %u does not exist", uint32(GUID_LOPART(guid)));
        return;
    }

    if (pet != GetPlayer()->GetPet() && pet != GetPlayer()->GetCharm())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetStopAttack: Pet GUID %u isn't a pet or charmed creature of player %s", uint32(GUID_LOPART(guid)), GetPlayer()->GetName());
        return;
    }

    if (!pet->isAlive())
        return;

    pet->AttackStop();
}

void WorldSession::HandlePetActionHelper(Unit* pet, uint64 guid1, uint32 spellid, uint16 flag, uint64 guid2, float x, float y, float z)
{
    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetAction(petGuid: " UI64FMTD ", tagGuid: " UI64FMTD ", spellId: %u, flag: %u): object (entry: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!",
            guid1, guid2, spellid, flag, pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    switch (flag)
    {
        case ACT_COMMAND:                                   //0x07
            switch (spellid)
            {
                case COMMAND_STAY:                          //flat=1792  //STAY
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MoveIdle();
                    charmInfo->SetCommandState(COMMAND_STAY);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                case COMMAND_FOLLOW:                        //spellid=1792  //FOLLOW
                    pet->AttackStop();
                    pet->InterruptNonMeleeSpells(false);
                    pet->GetMotionMaster()->MoveFollow(m_Player, PET_FOLLOW_DIST, pet->GetFollowAngle());
                    charmInfo->SetCommandState(COMMAND_FOLLOW);

                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(false);
                    charmInfo->SetIsReturning(true);
                    charmInfo->SetIsFollowing(false);
                    break;
                case COMMAND_ATTACK:                        //spellid=1792  //ATTACK
                {
                    // Can't attack if owner is pacified
                    if (m_Player->HasAuraType(SPELL_AURA_MOD_PACIFY))
                    {
                        //pet->SendPetCastFail(spellid, SPELL_FAILED_PACIFIED);
                        //TODO: Send proper error message to client
                        return;
                    }

                    // only place where pet can be player
                    Unit* TargetUnit = ObjectAccessor::GetUnit(*m_Player, guid2);
                    if (!TargetUnit)
                        return;

                    if (Unit* owner = pet->GetOwner())
                        if (!owner->IsValidAttackTarget(TargetUnit))
                            return;

                    // Not let attack through obstructions
                    if (sWorld->getBoolConfig(CONFIG_PET_LOS))
                    {
                        if (!pet->IsWithinLOSInMap(TargetUnit))
                            return;
                    }

                    pet->ClearUnitState(UNIT_STATE_FOLLOW);
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != TargetUnit || (pet->getVictim() == TargetUnit && !pet->GetCharmInfo()->IsCommandAttack()))
                    {
                        if (pet->getVictim())
                            pet->AttackStop();

                        // Summon gargoyle should attack the same target as ghoul
                        if (Unit* owner = pet->GetOwner())
                        {
                            if (owner->getClass() == CLASS_DEATH_KNIGHT)
                            {
                                for (Unit::ControlList::iterator itr = owner->m_Controlled.begin(); itr != owner->m_Controlled.end(); ++itr)
                                {
                                    if ((*itr)->GetEntry() == 27829 && !(*itr)->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                                    {
                                        owner->AddAura(49206, TargetUnit);
                                        break;
                                    }
                                }
                            }
                        }

                        if (pet->GetTypeId() != TYPEID_PLAYER && pet->ToCreature()->IsAIEnabled)
                        {
                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->ToCreature()->AI()->AttackStart(TargetUnit);

                            // Blink Strikes
                            /*if (pet->HasAura(130392))
                            {
                                pet->GetMotionMaster()->Clear();
                                pet->NearTeleportTo(TargetUnit->GetPositionX(), TargetUnit->GetPositionY(), TargetUnit->GetPositionZ(), TargetUnit->GetOrientation());
                            }*/

                            //10% chance to play special pet attack talk, else growl
                            if (pet->ToCreature()->isPet() && ((Pet*)pet)->getPetType() == SUMMON_PET && pet != TargetUnit && urand(0, 100) < 10)
                                pet->SendPetTalk((uint32)PET_TALK_ATTACK);
                            else
                            {
                                // 90% chance for pet and 100% chance for charmed creature
                                pet->SendPetAIReaction(guid1);
                            }
                        }
                        else                                // charmed player
                        {
                            if (pet->getVictim() && pet->getVictim() != TargetUnit)
                                pet->AttackStop();

                            charmInfo->SetIsCommandAttack(true);
                            charmInfo->SetIsAtStay(false);
                            charmInfo->SetIsFollowing(false);
                            charmInfo->SetIsReturning(false);

                            pet->Attack(TargetUnit, true);
                            pet->SendPetAIReaction(guid1);
                        }
                    }
                    break;
                }
                case COMMAND_ABANDON:                       // abandon (hunter pet) or dismiss (summoned pet)
                    if (pet->GetCharmerGUID() == GetPlayer()->GetGUID())
                        m_Player->StopCastingCharm();
                    else if (pet->GetOwnerGUID() == GetPlayer()->GetGUID())
                    {
                        ASSERT(pet->GetTypeId() == TYPEID_UNIT);
                        if (pet->isPet())
                        {
                            if (pet->ToPet()->getPetType() == HUNTER_PET)
                                GetPlayer()->RemovePet(pet->ToPet(), PET_SLOT_DELETED, false, pet->ToPet()->m_Stampeded);
                            else
                                //dismissing a summoned pet is like killing them (this prevents returning a soulshard...)
                                pet->setDeathState(CORPSE);
                        }
                        else if (pet->HasUnitTypeMask(UNIT_MASK_MINION))
                        {
                            ((Minion*)pet)->UnSummon();
                        }
                    }
                    break;
                case COMMAND_MOVE_TO:
                    pet->StopMoving();
                    pet->GetMotionMaster()->Clear(false);
                    pet->GetMotionMaster()->MoveIdle();
                    pet->GetMotionMaster()->MovePoint(0, x, y, z);

                    charmInfo->SetCommandState(COMMAND_MOVE_TO);
                    charmInfo->SetIsCommandAttack(false);
                    charmInfo->SetIsAtStay(true);
                    charmInfo->SetIsFollowing(false);
                    charmInfo->SetIsReturning(false);
                    charmInfo->SaveStayPosition();
                    break;
                default:
                    sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
            }
            break;
        case ACT_REACTION:                                  // 0x6
            switch (spellid)
            {
                case REACT_PASSIVE:                         //passive
                    pet->AttackStop();

                case REACT_DEFENSIVE:                       //recovery
                case REACT_HELPER:
                    if (pet->GetTypeId() == TYPEID_UNIT)
                        pet->ToCreature()->SetReactState(ReactStates(spellid));
                    break;
            }
            break;
        case ACT_DISABLED:                                  // 0x81    spell (disabled), ignore
        case ACT_PASSIVE:                                   // 0x01
        case ACT_ENABLED:                                   // 0xC1    spell
        {
            Unit* unit_target = NULL;

            if (guid2)
                unit_target = ObjectAccessor::GetUnit(*m_Player, guid2);

            // do not cast unknown spells
            SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellid);
            if (!spellInfo)
            {
                sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", spellid);
                return;
            }

            if (spellInfo->StartRecoveryCategory > 0)
                if (pet->GetCharmInfo() && pet->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
                    return;

            for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
            {
                if (spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_SRC_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_UNIT_DEST_AREA_ENEMY || spellInfo->Effects[i].TargetA.GetTarget() == TARGET_DEST_DYNOBJ_ENEMY)
                    return;
            }

            // do not cast not learned spells
            if (!pet->HasSpell(spellid) || spellInfo->IsPassive())
                return;

            //  Clear the flags as if owner clicked 'attack'. AI will reset them
            //  after AttackStart, even if spell failed
            if (pet->GetCharmInfo())
            {
                pet->GetCharmInfo()->SetIsAtStay(false);
                pet->GetCharmInfo()->SetIsCommandAttack(true);
                pet->GetCharmInfo()->SetIsReturning(false);
                pet->GetCharmInfo()->SetIsFollowing(false);
            }

            Spell* spell = new Spell(pet, spellInfo, TRIGGERED_NONE);

            SpellCastResult result = spell->CheckPetCast(unit_target);

            //auto turn to target unless possessed
            if (result == SPELL_FAILED_UNIT_NOT_INFRONT && !pet->isPossessed() && !pet->IsVehicle())
            {
                if (unit_target)
                {
                    pet->SetInFront(unit_target);
                    if (unit_target->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer((Player*)unit_target);
                }
                else if (Unit* unit_target2 = spell->m_targets.GetUnitTarget())
                {
                    pet->SetInFront(unit_target2);
                    if (unit_target2->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer((Player*)unit_target2);
                }

                if (Unit* powner = pet->GetCharmerOrOwner())
                    if (powner->GetTypeId() == TYPEID_PLAYER)
                        pet->SendUpdateToPlayer(powner->ToPlayer());

                result = SPELL_CAST_OK;
            }

            if (result == SPELL_CAST_OK)
            {
                pet->ToCreature()->AddCreatureSpellCooldown(spellid);

                unit_target = spell->m_targets.GetUnitTarget();

                //10% chance to play special pet attack talk, else growl
                //actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (pet->ToCreature()->isPet() && (((Pet*)pet)->getPetType() == SUMMON_PET) && (pet != unit_target) && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                {
                    pet->SendPetAIReaction(guid1);
                }

                if (unit_target && !GetPlayer()->IsFriendlyTo(unit_target) && !pet->isPossessed() && !pet->IsVehicle())
                {
                    // This is true if pet has no target or has target but targets differs.
                    if (pet->getVictim() != unit_target)
                    {
                        if (pet->getVictim())
                            pet->AttackStop();
                        pet->GetMotionMaster()->Clear();
                        if (pet->ToCreature()->IsAIEnabled)
                            pet->ToCreature()->AI()->AttackStart(unit_target);
                    }
                }

                spell->prepare(&(spell->m_targets));
            }
            else
            {
                if (pet->isPossessed() || pet->IsVehicle())
                    Spell::SendCastResult(GetPlayer(), spellInfo, 0, result);
                else
                    pet->SendPetCastFail(spellid, result);

                if (!pet->ToCreature()->HasSpellCooldown(spellid))
                    GetPlayer()->SendClearCooldown(spellid, pet);

                spell->finish(false);
                delete spell;

                // reset specific flags in case of spell fail. AI will reset other flags
                if (pet->GetCharmInfo())
                    pet->GetCharmInfo()->SetIsCommandAttack(false);
            }
            break;
        }
        default:
            sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET flag Action %i and spellid %i.", uint32(flag), spellid);
    }
}

void WorldSession::HandlePetNameQuery(WorldPacket & p_RecvPacket)
{
    uint64 l_PetGUID;
    p_RecvPacket.readPackGUID(l_PetGUID);

    SendPetNameQuery(l_PetGUID);
}

void WorldSession::SendPetNameQuery(uint64 p_PetGUID)
{
    Pet* l_Pet = ObjectAccessor::GetPet(*m_Player, p_PetGUID);
    bool l_Allow    = l_Pet != nullptr;

    WorldPacket l_Data(SMSG_PET_NAME_QUERY_RESPONSE);
    l_Data.appendPackGUID(p_PetGUID);
    l_Data.WriteBit(l_Allow);

    if (l_Allow)
    {
        DeclinedName const* l_DeclinedNames = l_Pet->GetDeclinedNames();
        uint32 l_NameSize                   = strlen(l_Pet->GetName());
        bool l_HasDeclined                  = l_DeclinedNames != nullptr;

        l_Data.WriteBits(l_NameSize, 8);
        l_Data.WriteBit(l_HasDeclined);
        for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_Data.WriteBits(l_DeclinedNames != nullptr ? l_DeclinedNames->name[l_I].size() : 0, 7);

        if (l_HasDeclined)
        {
            for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
                if (l_DeclinedNames->name[l_I].size())
                    l_Data.WriteString(l_DeclinedNames->name[l_I]);
        }

        l_Data << uint32(l_Pet->GetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP));
        l_Data.WriteString(l_Pet->GetName());
    }

    m_Player->GetSession()->SendPacket(&l_Data);
}

bool WorldSession::CheckStableMaster(uint64 guid)
{
    // spell case or GM
    if (guid == GetPlayer()->GetGUID())
    {
        if (!GetPlayer()->isGameMaster() && !GetPlayer()->HasAuraType(SPELL_AURA_OPEN_STABLE))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Player (GUID:%u) attempt open stable in cheating way.", GUID_LOPART(guid));
            return false;
        }
    }
    // stable master case
    else
    {
        if (!GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_STABLEMASTER))
        {
            sLog->outDebug(LOG_FILTER_NETWORKIO, "Stablemaster (GUID:%u) not found or you can't interact with him.", GUID_LOPART(guid));
            return false;
        }
    }
    return true;
}

void WorldSession::HandlePetSetAction(WorldPacket & recvData)
{
    sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetSetAction. CMSG_PET_SET_ACTION");

    ObjectGuid petguid;
    uint32 position;
    uint32 data;
    bool move_command = false;

    recvData >> data >> position;
    uint8 bitOrder[8] = {1, 7, 3, 5, 2, 6, 4, 0};
    recvData.ReadBitInOrder(petguid, bitOrder);
    uint8 byteOrder[8] = {0, 1, 2, 3, 7, 4, 6, 5};
    recvData.ReadBytesSeq(petguid, byteOrder);


    Unit* pet = ObjectAccessor::GetUnit(*m_Player, petguid);

    if (!pet || pet != m_Player->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", GUID_LOPART(petguid), m_Player->GetGUIDLow());
        return;
    }

    CharmInfo* charmInfo = pet->GetCharmInfo();
    if (!charmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", pet->GetGUIDLow(), pet->GetTypeId());
        return;
    }

    uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data);

    //ignore invalid position
    if (position >= MAX_UNIT_ACTION_BAR_INDEX)
        return;

    uint8 act_state_0 = UNIT_ACTION_BUTTON_TYPE(data);
    if ((act_state_0 == ACT_COMMAND && UNIT_ACTION_BUTTON_ACTION(data) != COMMAND_MOVE_TO) || act_state_0 == ACT_REACTION)
    {
        uint32 spell_id_0 = UNIT_ACTION_BUTTON_ACTION(data);
        UnitActionBarEntry const* actionEntry_1 = charmInfo->GetActionBarEntry(position);
        if (!actionEntry_1 || spell_id_0 != actionEntry_1->GetAction() ||
            act_state_0 != actionEntry_1->GetType())
            return;
    }

    uint32 spell_id = UNIT_ACTION_BUTTON_ACTION(data);
    //uint8 act_state = UNIT_ACTION_BUTTON_TYPE(data);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "Player %s has changed pet spell action. Position: %u, Spell: %u, State: 0x%X", m_Player->GetName(), position, spell_id, uint32(act_state));

    //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
    if (!((act_state == ACT_ENABLED || act_state == ACT_DISABLED || act_state == ACT_PASSIVE) && spell_id && !pet->HasSpell(spell_id)))
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spell_id))
        {
            //sign for autocast
            if (act_state == ACT_ENABLED)
            {
                if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isPet())
                    ((Pet*)pet)->ToggleAutocast(spellInfo, true);
                else
                    for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, true);
            }
            //sign for no/turn off autocast
            else if (act_state == ACT_DISABLED)
            {
                if (pet->GetTypeId() == TYPEID_UNIT && pet->ToCreature()->isPet())
                    ((Pet*)pet)->ToggleAutocast(spellInfo, false);
                else
                    for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, false);
            }
        }

        charmInfo->SetActionBar(position, spell_id, ActiveStates(act_state));
    }
}

void WorldSession::HandlePetRename(WorldPacket & p_RecvPacket)
{
    uint64 l_PetNumber;
    uint32 l_Unknow;
    uint32 l_NewNameSize;
    bool l_HasDeclinedNames;
    DeclinedName l_DeclinedNames;
    uint8 l_DeclinedNamesLenght[MAX_DECLINED_NAME_CASES];
    std::string l_NewName;

    printf("l_Unknow : %u\n", l_Unknow);

    p_RecvPacket.readPackGUID(l_PetNumber);
    l_Unknow           = p_RecvPacket.read<uint32>();
    l_NewNameSize      = p_RecvPacket.ReadBits(8);
    l_HasDeclinedNames = p_RecvPacket.ReadBit();

    if (l_HasDeclinedNames)
    {
        for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; l_I++)
            l_DeclinedNamesLenght[l_I] = p_RecvPacket.ReadBits(7);

        for (uint8 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; l_I++)
            l_DeclinedNames.name[l_I] = p_RecvPacket.ReadString(l_DeclinedNamesLenght[l_I]);
    }

    l_NewName = p_RecvPacket.ReadString(l_NewNameSize);

    Pet* l_Pet = ObjectAccessor::GetPet(*m_Player, l_PetNumber);
    if (l_Pet == nullptr || l_Pet != m_Player->GetPet())
        return;
                                                            // check it!
    if (!l_Pet || !l_Pet->isPet() || ((Pet*)l_Pet)->getPetType()!= HUNTER_PET ||
        !l_Pet->HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED) ||
        l_Pet->GetOwnerGUID() != m_Player->GetGUID() || !l_Pet->GetCharmInfo())
        return;

    PetNameInvalidReason l_Reason = ObjectMgr::CheckPetName(l_NewName);
    if (l_Reason != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(l_Reason, l_NewName, NULL);
        return;
    }

    if (sObjectMgr->IsReservedName(l_NewName))
    {
        SendPetNameInvalid(PET_NAME_RESERVED, l_NewName, NULL);
        return;
    }

    l_Pet->SetName(l_NewName);

    Unit* l_Owner = l_Pet->GetOwner();
    if (l_Owner && (l_Owner->GetTypeId() == TYPEID_PLAYER) && l_Owner->ToPlayer()->GetGroup())
        l_Owner->ToPlayer()->SetGroupUpdateFlag(GROUP_UPDATE_FLAG_PET_NAME);

    l_Pet->RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 2, UNIT_CAN_BE_RENAMED);

    if (l_HasDeclinedNames)
    {
        std::wstring l_WName;
        Utf8toWStr(l_NewName, l_WName);
        if (!ObjectMgr::CheckDeclinedNames(l_WName, l_DeclinedNames))
        {
            SendPetNameInvalid(PET_NAME_DECLENSION_DOESNT_MATCH_BASE_NAME, l_NewName, &l_DeclinedNames);
            return;
        }
    }

    SQLTransaction l_Transaction = CharacterDatabase.BeginTransaction();
    if (l_HasDeclinedNames)
    {
        PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_DEL_CHAR_PET_DECLINEDNAME);
        l_Statement->setUInt32(0, l_Pet->GetCharmInfo()->GetPetNumber());
        l_Transaction->Append(l_Statement);

        l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_ADD_CHAR_PET_DECLINEDNAME);
        l_Statement->setUInt32(0, m_Player->GetGUIDLow());

        for (uint8 i = 0; i < MAX_DECLINED_NAME_CASES; i++)
            l_Statement->setString(i + 1, l_DeclinedNames.name[i]);

        l_Transaction->Append(l_Statement);
    }

    PreparedStatement* l_Statement = CharacterDatabase.GetPreparedStatement(CHAR_UPD_CHAR_PET_NAME);
    l_Statement->setString(0, l_NewName);
    l_Statement->setUInt32(1, m_Player->GetGUIDLow());
    l_Statement->setUInt32(2, l_Pet->GetCharmInfo()->GetPetNumber());
    l_Transaction->Append(l_Statement);

    CharacterDatabase.CommitTransaction(l_Transaction);

    l_Pet->SetUInt32Value(UNIT_FIELD_PET_NAME_TIMESTAMP, uint32(time(NULL))); // cast can't be helped
}

void WorldSession::HandlePetAbandon(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 bitOrder[8] = {7, 0, 6, 3, 1, 2, 4, 5};
    recvData.ReadBitInOrder(guid, bitOrder);
    uint8 byteOrder[8] = {7, 4, 3, 0, 2, 6, 5, 1};
    recvData.ReadBytesSeq(guid, byteOrder);

    sLog->outInfo(LOG_FILTER_NETWORKIO, "HandlePetAbandon. CMSG_PET_ABANDON pet guid is %u", GUID_LOPART(guid));

    if (!m_Player->IsInWorld())
        return;

    // pet/charmed
    Creature* pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, guid);
    if (pet)
    {
        if (pet->isPet())
        {
            m_Player->RemovePet(pet->ToPet(), PET_SLOT_DELETED, false, pet->ToPet()->m_Stampeded);
            m_Player->GetSession()->SendStablePet(0);
        }
        else if (pet->GetGUID() == m_Player->GetCharmGUID())
            m_Player->StopCastingCharm();
    }
}

void WorldSession::HandlePetCastSpellOpcode(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_PET_CAST_SPELL");

    ObjectGuid casterGUID;
    ObjectGuid unkGUID1;
    ObjectGuid transportDstGUID;
    ObjectGuid transportSrcGUID;
    ObjectGuid targetGUID;
    ObjectGuid unkGUID2;
    bool hasDestPos;
    bool hasSrcPos;
    bool hasSpeed;
    bool hasSpell;
    bool hasGlyphIndex;
    bool hasTargetFlags;
    bool hasElevation;
    bool hasString;
    bool hasCastCount;
    bool hasUnk5bits;
    uint32 archeologyCounter = 0;
    WorldLocation dstLoc, srcLoc;
    float speed = 0.0f, elevation = 0.0f;
    uint32 targetFlags = 0;
    uint32 spellID = 0;
    uint32 stringLenght = 0;
    uint8 castCount = 0;

    casterGUID[3] = recvPacket.ReadBit();
    casterGUID[5] = recvPacket.ReadBit();
    hasDestPos = recvPacket.ReadBit();
    recvPacket.ReadBit();                   // unk bit
    hasElevation = !recvPacket.ReadBit();
    hasSrcPos = recvPacket.ReadBit();
    hasSpell = !recvPacket.ReadBit();
    casterGUID[0] = recvPacket.ReadBit();
    hasGlyphIndex = !recvPacket.ReadBit();
    casterGUID[7] = recvPacket.ReadBit();
    hasTargetFlags = !recvPacket.ReadBit();
    hasSpeed = !recvPacket.ReadBit();
    recvPacket.ReadBit();                   // has movement info
    hasString = !recvPacket.ReadBit();
    recvPacket.ReadBit();                   // !inverse bit, unk
    hasCastCount = !recvPacket.ReadBit();
    casterGUID[2] = recvPacket.ReadBit();
    casterGUID[4] = recvPacket.ReadBit();
    archeologyCounter = recvPacket.ReadBits(2);
    casterGUID[1] = recvPacket.ReadBit();
    hasUnk5bits = !recvPacket.ReadBit();

    for (uint32 i = 0; i < archeologyCounter; i++)
        recvPacket.ReadBits(2);             // archeology type

    casterGUID[6] = recvPacket.ReadBit();

    if (hasDestPos)
    {
        uint8 bitOrder[8] = {2, 7, 4, 0, 1, 6, 5, 3};
        recvPacket.ReadBitInOrder(transportDstGUID, bitOrder);
    }

    // movement block (disabled by patch client-side)

    if (hasSrcPos)
    {
        uint8 bitOrder[8] = {6, 2, 3, 1, 5, 4, 0, 7};
        recvPacket.ReadBitInOrder(transportSrcGUID, bitOrder);
    }

    if (hasUnk5bits)
        recvPacket.ReadBits(5);             // unk 5 bits

    // Target GUID
    {
        uint8 bitOrder[8] = {3, 5, 6, 2, 4, 1, 7, 0};
        recvPacket.ReadBitInOrder(targetGUID, bitOrder);
    }

    // unkGUID1
    {
        uint8 bitOrder[8] = {3, 1, 5, 2, 4, 7, 0, 6};
        recvPacket.ReadBitInOrder(unkGUID1, bitOrder);
    }

    if (hasTargetFlags)
        targetFlags = recvPacket.ReadBits(20);

    if (hasString)
        stringLenght = recvPacket.ReadBits(7);

    recvPacket.ReadByteSeq(casterGUID[0]);
    recvPacket.ReadByteSeq(casterGUID[4]);
    recvPacket.ReadByteSeq(casterGUID[5]);
    recvPacket.ReadByteSeq(casterGUID[1]);
    recvPacket.ReadByteSeq(casterGUID[2]);
    recvPacket.ReadByteSeq(casterGUID[3]);
    recvPacket.ReadByteSeq(casterGUID[7]);

    for (uint32 i = 0; i < archeologyCounter; i++)
    {
        recvPacket.read_skip<uint32>(); // entry
        recvPacket.read_skip<uint32>(); // counter
    }

    recvPacket.ReadByteSeq(casterGUID[6]);
    recvPacket.ReadByteSeq(unkGUID1[1]);
    recvPacket.ReadByteSeq(unkGUID1[5]);
    recvPacket.ReadByteSeq(unkGUID1[4]);
    recvPacket.ReadByteSeq(unkGUID1[2]);
    recvPacket.ReadByteSeq(unkGUID1[7]);
    recvPacket.ReadByteSeq(unkGUID1[3]);
    recvPacket.ReadByteSeq(unkGUID1[0]);

    if (hasSrcPos)
    {
        recvPacket.ReadByteSeq(transportSrcGUID[4]);
        srcLoc.m_positionY = recvPacket.read<float>();
        recvPacket.ReadByteSeq(transportSrcGUID[2]);
        recvPacket.ReadByteSeq(transportSrcGUID[6]);
        srcLoc.m_positionZ = recvPacket.read<float>();
        srcLoc.m_positionX = recvPacket.read<float>();
        recvPacket.ReadByteSeq(transportSrcGUID[1]);
        recvPacket.ReadByteSeq(transportSrcGUID[3]);
        recvPacket.ReadByteSeq(transportSrcGUID[5]);
        recvPacket.ReadByteSeq(transportSrcGUID[7]);
        recvPacket.ReadByteSeq(transportSrcGUID[0]);
    }

    // Target GUID
    {
        uint8 byteOrder[8] = {7, 4, 2, 6, 3, 0, 5, 1};
        recvPacket.ReadBytesSeq(targetGUID, byteOrder);
    }

    if (hasDestPos)
    {
        dstLoc.m_positionX = recvPacket.read<float>();
        recvPacket.ReadByteSeq(transportDstGUID[5]);
        recvPacket.ReadByteSeq(transportDstGUID[7]);
        recvPacket.ReadByteSeq(transportDstGUID[2]);
        recvPacket.ReadByteSeq(transportDstGUID[0]);
        recvPacket.ReadByteSeq(transportDstGUID[1]);
        recvPacket.ReadByteSeq(transportDstGUID[3]);
        recvPacket.ReadByteSeq(transportDstGUID[6]);
        dstLoc.m_positionZ = recvPacket.read<float>();
        dstLoc.m_positionY = recvPacket.read<float>();
        recvPacket.ReadByteSeq(transportDstGUID[4]);
    }

    if (hasGlyphIndex)
        recvPacket.read_skip<uint32>();     // glyph index

    if (hasElevation)
        elevation = recvPacket.read<float>();

    if (hasSpell)
        spellID = recvPacket.read<uint32>();

    if (hasCastCount)
        castCount = recvPacket.read<uint8>();

    if (hasString)
        recvPacket.ReadString(stringLenght);

    if (hasSpeed)
        speed = recvPacket.read<float>();

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_PET_CAST_SPELL, castCount: %u, spellId %u, targetFlags %u", castCount, spellID, targetFlags);

    // This opcode is also sent from charmed and possessed units (players and creatures)
    if (!m_Player->GetGuardianPet() && !m_Player->GetCharm())
        return;

    Unit* caster = ObjectAccessor::GetUnit(*m_Player, casterGUID);

    if (!caster || (caster != m_Player->GetGuardianPet() && caster != m_Player->GetCharm()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s .", uint32(GUID_LOPART(casterGUID)), GetPlayer()->GetName());
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellID);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", spellID);
        return;
    }

    if (spellInfo->StartRecoveryCategory > 0) // Check if spell is affected by GCD
        if (caster->GetTypeId() == TYPEID_UNIT && caster->GetCharmInfo() && caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(spellInfo))
        {
            caster->SendPetCastFail(spellID, SPELL_FAILED_NOT_READY);
            return;
        }

    // do not cast not learned spells
    if (!caster->HasSpell(spellID) || spellInfo->IsPassive())
        return;

    SpellCastTargets targets;
    targets.Initialize(targetFlags, targetGUID, unkGUID1, transportDstGUID, dstLoc, transportSrcGUID, srcLoc);
    targets.SetElevation(elevation);
    targets.SetSpeed(speed);
    targets.Update(caster);

    caster->ClearUnitState(UNIT_STATE_FOLLOW);

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE);
    spell->m_cast_count = castCount;                    // probably pending spell cast
    spell->m_targets = targets;

    // TODO: need to check victim?
    SpellCastResult result;
    if (caster->m_movedPlayer)
        result = spell->CheckPetCast(caster->m_movedPlayer->GetSelectedUnit());
    else
        result = spell->CheckPetCast(NULL);
    if (result == SPELL_CAST_OK)
    {
        if (caster->GetTypeId() == TYPEID_UNIT)
        {
            Creature* pet = caster->ToCreature();
            pet->AddCreatureSpellCooldown(spellID);
            if (pet->isPet())
            {
                Pet* p = (Pet*)pet;
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (p->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                    pet->SendPetAIReaction(spellID);
            }
        }

        spell->prepare(&(spell->m_targets));
    }
    else
    {
        caster->SendPetCastFail(spellID, result);
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
            if (!caster->ToPlayer()->HasSpellCooldown(spellID))
                GetPlayer()->SendClearCooldown(spellID, caster);
        }
        else
        {
            if (!caster->ToCreature()->HasSpellCooldown(spellID))
                GetPlayer()->SendClearCooldown(spellID, caster);
        }

        spell->finish(false);
        delete spell;
    }
}

void WorldSession::SendPetNameInvalid(uint32 p_Result, const std::string& p_NewName, DeclinedName *p_DeclinedName)
{
    Pet* l_Pet = m_Player->GetPet();
    if (l_Pet == nullptr)
        return;

    WorldPacket l_Data(SMSG_PET_NAME_INVALID);
    l_Data << uint8(p_Result);
    l_Data.appendPackGUID(l_Pet->GetGUID());
    l_Data << uint32(0);
    l_Data.WriteBits(p_NewName.size(), 8);
    l_Data.WriteBit(p_DeclinedName != nullptr);

    if (p_DeclinedName != nullptr)
    {
        for (uint32 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_Data.WriteBits(p_DeclinedName->name[l_I].size(), 7);

        for (uint32 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_Data.WriteString(p_DeclinedName->name[l_I]);
    }

    l_Data.WriteString(p_NewName);

    SendPacket(&l_Data);
}

void WorldSession::HandleLearnPetSpecialization(WorldPacket & recvData)
{
    uint32 index = recvData.read<uint32>();
    // GUID : useless =P
    recvData.rfinish();

    if (m_Player->isInCombat())
        return;

    uint32 specializationId = 0;

    switch(index)
    {
        case 0:
            specializationId = SPEC_PET_FEROCITY; // Férocité
            break;
        case 1:
            specializationId = SPEC_PET_TENACITY; // Ténacité
            break;
        case 2:
            specializationId = SPEC_PET_CUNNING; // Ruse
            break;
        default:
            break;
    }

    if (!specializationId)
        return;

    Pet* pet = m_Player->GetPet();
    if (!pet)
        return;

    if (pet->getPetType() != PetType::HUNTER_PET)
        return;

    if (pet->GetSpecializationId())
        pet->UnlearnSpecializationSpell();

    pet->SetSpecializationId(specializationId);
    pet->LearnSpecializationSpell();
    m_Player->SendTalentsInfoData(true);
}
