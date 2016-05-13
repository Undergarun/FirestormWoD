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

void WorldSession::HandlePetAction(WorldPacket & p_RecvPacket)
{
    uint64       l_PetGUID;    
    uint64       l_TargetGUID;
    G3D::Vector3 l_ActionPosition;
    uint32       l_Action;

    p_RecvPacket.readPackGUID(l_PetGUID);           ///< PetGUID
    l_Action = p_RecvPacket.read<uint32>();         ///< Action
    p_RecvPacket.readPackGUID(l_TargetGUID);        ///< TargetGUID
    p_RecvPacket.ReadVector3(l_ActionPosition);     ///< ActionPosition


    uint32 l_SpellID = UNIT_ACTION_BUTTON_ACTION(l_Action);
    uint8 l_Flag     = UNIT_ACTION_BUTTON_TYPE(l_Action);             //delete = 0x07 CastSpell = C1

    // used also for charmed creature
    Unit* l_Pet = ObjectAccessor::GetUnit(*m_Player, l_PetGUID);
    if (l_Pet == nullptr)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) doesn't exist for player '%s'", uint32(GUID_LOPART(l_PetGUID)), GetPlayer()->GetName());
        return;
    }

    if (l_Pet != GetPlayer()->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetAction: Pet (GUID: %u) does not belong to player '%s'", uint32(GUID_LOPART(l_PetGUID)), GetPlayer()->GetName());
        return;
    }

    if (!l_Pet->isAlive())
    {
        SpellInfo const* l_Spell = (l_Flag == ACT_ENABLED || l_Flag == ACT_PASSIVE) ? sSpellMgr->GetSpellInfo(l_SpellID) : NULL;
        if (!l_Spell)
            return;

        if (!(l_Spell->Attributes & SPELL_ATTR0_CASTABLE_WHILE_DEAD))
            return;
    }

    //TODO: allow control charmed player?
    if (l_Pet->IsPlayer() && !(l_Flag == ACT_COMMAND && l_SpellID == COMMAND_ATTACK))
        return;

    if (GetPlayer()->m_Controlled.size() == 1)
        HandlePetActionHelper(l_Pet, l_PetGUID, l_SpellID, l_Flag, l_TargetGUID, l_ActionPosition.x, l_ActionPosition.y, l_ActionPosition.z);
    else
    {
        //If a pet is dismissed, m_Controlled will change
        std::vector<Unit*> l_Controlled;
        for (Unit::ControlList::iterator l_Itr = GetPlayer()->m_Controlled.begin(); l_Itr != GetPlayer()->m_Controlled.end(); ++l_Itr)
            if ((*l_Itr)->GetEntry() == l_Pet->GetEntry() && (*l_Itr)->isAlive())
                l_Controlled.push_back(*l_Itr);

        for (std::vector<Unit*>::iterator l_Itr = l_Controlled.begin(); l_Itr != l_Controlled.end(); ++l_Itr)
            HandlePetActionHelper(*l_Itr, l_PetGUID, l_SpellID, l_Flag, l_TargetGUID, l_ActionPosition.x, l_ActionPosition.y, l_ActionPosition.z);
    }
}

void WorldSession::HandlePetStopAttack(WorldPacket &p_RecvPacket)
{
    uint64 l_PetGUID;
    p_RecvPacket.readPackGUID(l_PetGUID);

    Unit* l_Pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, l_PetGUID);
    if (!l_Pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetStopAttack: Pet %u does not exist", uint32(GUID_LOPART(l_PetGUID)));
        return;
    }

    if (l_Pet != GetPlayer()->GetPet() && l_Pet != GetPlayer()->GetCharm())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetStopAttack: Pet GUID %u isn't a pet or charmed creature of player %s", uint32(GUID_LOPART(l_PetGUID)), GetPlayer()->GetName());
        return;
    }

    if (!l_Pet->isAlive())
        return;

    l_Pet->AttackStop();
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
                    charmInfo->SetIsCommandFollow(false);
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
                    charmInfo->SetIsCommandFollow(true);
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
                            charmInfo->SetIsCommandFollow(false);
                            charmInfo->SetIsReturning(false);

                            pet->ToCreature()->AI()->AttackStart(TargetUnit);

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
                            charmInfo->SetIsCommandFollow(false);
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

            for (uint8 i = 0; i < spellInfo->EffectCount; ++i)
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
            spell->m_targets.SetUnitTarget(unit_target);

            SpellCastResult result = spell->CheckPetCast(unit_target);

            //auto turn to target unless possessed
            if (result == SPELL_FAILED_UNIT_NOT_INFRONT && !pet->isPossessed() && !pet->IsVehicle())
            {
                if (unit_target)
                {
                    pet->SetInFront(unit_target);
                    if (unit_target->IsPlayer())
                        pet->SendUpdateToPlayer((Player*)unit_target);
                }
                else if (Unit* unit_target2 = spell->m_targets.GetUnitTarget())
                {
                    pet->SetInFront(unit_target2);
                    if (unit_target2->IsPlayer())
                        pet->SendUpdateToPlayer((Player*)unit_target2);
                }

                if (Unit* powner = pet->GetCharmerOrOwner())
                    if (powner->IsPlayer())
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
                    pet->SendPetCastFail(spellid, result, 0);

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
        case ACT_DECIDE:
            return;
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

void WorldSession::HandlePetSetAction(WorldPacket & p_RecvPacket)
{
    bool l_SwapCommand = false;

    uint64 l_PetGUID;
    uint32 l_Action;
    uint32 l_Index;

    p_RecvPacket.readPackGUID(l_PetGUID);
    p_RecvPacket >> l_Index;
    p_RecvPacket >> l_Action;

    Unit* l_Pet = ObjectAccessor::GetUnit(*m_Player, l_PetGUID);
    if (!l_Pet || l_Pet != m_Player->GetFirstControlled())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetSetAction: Unknown pet (GUID: %u) or pet owner (GUID: %u)", GUID_LOPART(l_PetGUID), m_Player->GetGUIDLow());
        return;
    }

    CharmInfo* l_CharmInfo = l_Pet->GetCharmInfo();
    if (!l_CharmInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePetSetAction: object (GUID: %u TypeId: %u) is considered pet-like but doesn't have a charminfo!", l_Pet->GetGUIDLow(), l_Pet->GetTypeId());
        return;
    }

    /// ignore invalid position
    if (l_Index >= MAX_UNIT_ACTION_BAR_INDEX)
        return;

    uint32 l_SpellIdOrAction = UNIT_ACTION_BUTTON_ACTION(l_Action);
    uint8 l_ActState = UNIT_ACTION_BUTTON_TYPE(l_Action);

    /// in the normal case, command and reaction buttons can only be moved, not removed
    /// ignore attempt to remove command|reaction buttons (not possible at normal case)
    UnitActionBarEntry const* l_ActionEntry = l_CharmInfo->GetActionBarEntry(l_Index);
    if ((l_ActionEntry->GetType() == ACT_COMMAND || l_ActionEntry->GetType() == ACT_REACTION) && l_ActState == 0 && l_SpellIdOrAction == 0)
        l_SwapCommand = true;

    //if it's act for spell (en/disable/cast) and there is a spell given (0 = remove spell) which pet doesn't know, don't add
    if (!l_SwapCommand && !((l_ActState == ACT_ENABLED || l_ActState == ACT_DISABLED || l_ActState == ACT_PASSIVE) && l_SpellIdOrAction && !l_Pet->HasSpell(l_SpellIdOrAction)))
    {
        if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(l_SpellIdOrAction))
        {
            //sign for autocast
            if (l_ActState == ACT_ENABLED)
            {
                if (l_Pet->GetTypeId() == TYPEID_UNIT && l_Pet->ToCreature()->isPet())
                    ((Pet*)l_Pet)->ToggleAutocast(spellInfo, true);
                else
                    for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == l_Pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, true);
            }
            //sign for no/turn off autocast
            else if (l_ActState == ACT_DISABLED)
            {
                if (l_Pet->GetTypeId() == TYPEID_UNIT && l_Pet->ToCreature()->isPet())
                    ((Pet*)l_Pet)->ToggleAutocast(spellInfo, false);
                else
                    for (Unit::ControlList::iterator itr = GetPlayer()->m_Controlled.begin(); itr != GetPlayer()->m_Controlled.end(); ++itr)
                        if ((*itr)->GetEntry() == l_Pet->GetEntry())
                            (*itr)->GetCharmInfo()->ToggleCreatureAutocast(spellInfo, false);
            }
        }

        l_CharmInfo->SetActionBar(l_Index, l_SpellIdOrAction, ActiveStates(l_ActState));
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

    p_RecvPacket.readPackGUID(l_PetNumber);
    l_Unknow           = p_RecvPacket.read<uint32>(); ///< l_unknow is never read 01/18/16
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
    if (l_Owner && (l_Owner->IsPlayer()) && l_Owner->ToPlayer()->GetGroup())
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

void WorldSession::HandlePetAbandon(WorldPacket& p_RecvPacket)
{
    uint64 l_PetGUID;
    p_RecvPacket.readPackGUID(l_PetGUID);

    if (!m_Player->IsInWorld())
        return;

    // pet/charmed
    Creature* l_Pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, l_PetGUID);
    if (l_Pet)
    {
        if (l_Pet->isPet())
        {
            m_Player->RemovePet(l_Pet->ToPet(), PET_SLOT_DELETED, false, l_Pet->ToPet()->m_Stampeded);
            m_Player->GetSession()->SendStablePet(0);
        }
        else if (l_Pet->GetGUID() == m_Player->GetCharmGUID())
            m_Player->StopCastingCharm();
    }
}

void WorldSession::HandlePetCastSpellOpcode(WorldPacket& p_RecvPacket)
{
    uint64 l_PetGUID;
    p_RecvPacket.readPackGUID(l_PetGUID);

    std::string l_SrcTargetName;

    uint64 l_TargetItemGUID = 0;
    uint64 l_SourceTargetGUID = 0;
    uint64 l_DestinationTargetGUID = 0;
    uint64 l_TargetGUID = 0;
    uint64 l_UnkGUID = 0;

    float l_MissibleTrajectorySpeed = 0.00f;
    float l_MissibleTrajectoryPitch = 0.00f;

    uint8   * l_SpellWeightType = nullptr;
    uint32  * l_SpellWeightID = nullptr;
    uint32  * l_SpellWeightQuantity = nullptr;

    uint32 l_SpellID = 0;
    uint32 l_Misc = 0;
    uint32 l_TargetFlags = 0;
    uint32 l_NameLenght = 0;
    uint32 l_SpellWeightCount = 0;

    float l_UnkFloat = 0;

    uint8 l_CastCount = 0;
    uint8 l_SendCastFlag = 0;

    bool l_HasSourceTarget = false;
    bool l_HasDestinationTarget = false;
    bool l_HasUnkFloat = false;
    bool l_HasMovementInfos = false;

    WorldLocation l_SourceTargetPosition;
    WorldLocation l_DestinationTargetPosition;

    p_RecvPacket >> l_CastCount;

    for (int l_I = 0; l_I < 2; l_I++)
        p_RecvPacket.read_skip<uint32>();

    p_RecvPacket >> l_SpellID;
    p_RecvPacket >> l_Misc;

    l_TargetFlags = p_RecvPacket.ReadBits(23);
    l_HasSourceTarget = p_RecvPacket.ReadBit();
    l_HasDestinationTarget = p_RecvPacket.ReadBit();
    l_HasUnkFloat = p_RecvPacket.ReadBit();
    l_NameLenght = p_RecvPacket.ReadBits(7);
    p_RecvPacket.readPackGUID(l_TargetGUID);
    p_RecvPacket.readPackGUID(l_TargetItemGUID);

    if (l_HasSourceTarget)
    {
        p_RecvPacket.readPackGUID(l_SourceTargetGUID);
        p_RecvPacket >> l_SourceTargetPosition.m_positionX;
        p_RecvPacket >> l_SourceTargetPosition.m_positionY;
        p_RecvPacket >> l_SourceTargetPosition.m_positionZ;
    }

    if (l_HasDestinationTarget)
    {
        p_RecvPacket.readPackGUID(l_DestinationTargetGUID);
        p_RecvPacket >> l_DestinationTargetPosition.m_positionX;
        p_RecvPacket >> l_DestinationTargetPosition.m_positionY;
        p_RecvPacket >> l_DestinationTargetPosition.m_positionZ;
    }

    if (l_HasUnkFloat)
        p_RecvPacket >> l_UnkFloat;

    l_SrcTargetName = p_RecvPacket.ReadString(l_NameLenght);

    p_RecvPacket >> l_MissibleTrajectoryPitch;
    p_RecvPacket >> l_MissibleTrajectorySpeed;

    p_RecvPacket.readPackGUID(l_UnkGUID);

    l_SendCastFlag = p_RecvPacket.ReadBits(5);  ///< l_SendCastFlag is never read 01/18/16
    l_HasMovementInfos = p_RecvPacket.ReadBit();
    l_SpellWeightCount = p_RecvPacket.ReadBits(2);

    if (l_HasMovementInfos)
        HandleMovementOpcodes(p_RecvPacket);

    if (l_SpellWeightCount)
    {
        l_SpellWeightType = new uint8[l_SpellWeightCount];
        l_SpellWeightID = new uint32[l_SpellWeightCount];
        l_SpellWeightQuantity = new uint32[l_SpellWeightCount];

        for (uint32 l_I = 0; l_I < l_SpellWeightCount; ++l_I)
        {
            l_SpellWeightType[l_I] = p_RecvPacket.ReadBits(2);
            p_RecvPacket >> l_SpellWeightID[l_I];
            p_RecvPacket >> l_SpellWeightQuantity[l_I];
        }
    }

    //////////////////////////////////////////////////////////////////////////

    if (l_SpellWeightCount)
    {
        GetPlayer()->GetArchaeologyMgr().ClearProjectCost();

        for (uint32 l_I = 0; l_I < l_SpellWeightCount; l_I++)
        {
            switch (l_SpellWeightType[l_I])
            {
                case SPELL_WEIGHT_ARCHEOLOGY_KEYSTONES: // Keystones
                    GetPlayer()->GetArchaeologyMgr().AddProjectCost(l_SpellWeightID[l_I], l_SpellWeightQuantity[l_I], false);
                    break;

                case SPELL_WEIGHT_ARCHEOLOGY_FRAGMENTS: // Fragments
                    GetPlayer()->GetArchaeologyMgr().AddProjectCost(l_SpellWeightID[l_I], l_SpellWeightQuantity[l_I], true);
                    break;

                default:
                    break;
            }
        }

        delete[] l_SpellWeightType;
        delete[] l_SpellWeightID;
        delete[] l_SpellWeightQuantity;
    }
    
    //////////////////////////////////////////////////////////////////////////
 
    // This opcode is also sent from charmed and possessed units (players and creatures)
    if (!m_Player->GetGuardianPet() && !m_Player->GetCharm())
        return;

    Unit* l_Caster = ObjectAccessor::GetUnit(*m_Player, l_PetGUID);
    if (!l_Caster || (l_Caster != m_Player->GetGuardianPet() && l_Caster != m_Player->GetCharm()))
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCastSpellOpcode: Pet %u isn't pet of player %s .", uint32(GUID_LOPART(l_PetGUID)), GetPlayer()->GetName());
        return;
    }

    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellID);
    if (!l_SpellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %i", l_SpellID);
        return;
    }

    if (l_SpellInfo->StartRecoveryCategory > 0) // Check if spell is affected by GCD
    {
        if (l_Caster->GetTypeId() == TYPEID_UNIT && l_Caster->GetCharmInfo() && l_Caster->GetCharmInfo()->GetGlobalCooldownMgr().HasGlobalCooldown(l_SpellInfo))
        {
            l_Caster->SendPetCastFail(l_SpellID, SPELL_FAILED_NOT_READY, l_CastCount);
            return;
        }
    }

    // do not cast not learned spells
    if (!l_Caster->HasSpell(l_SpellID) || l_SpellInfo->IsPassive())
        return;

    SpellCastTargets targets;
    targets.Initialize(l_TargetFlags, l_TargetGUID, l_UnkGUID, l_DestinationTargetGUID, l_DestinationTargetPosition, l_SourceTargetGUID, l_SourceTargetPosition);
    targets.SetElevation(l_MissibleTrajectoryPitch);
    targets.SetSpeed(l_MissibleTrajectorySpeed);
    targets.Update(l_Caster);

    l_Caster->ClearUnitState(UNIT_STATE_FOLLOW);

    Spell* l_Spell = new Spell(l_Caster, l_SpellInfo, TRIGGERED_NONE);
    l_Spell->m_cast_count = l_CastCount;                    // probably pending spell cast
    l_Spell->m_targets = targets;

    // TODO: need to check victim?
    SpellCastResult l_Result;
    if (l_Caster->m_movedPlayer)
        l_Result = l_Spell->CheckPetCast(l_Caster->m_movedPlayer->GetSelectedUnit());
    else
        l_Result = l_Spell->CheckPetCast(NULL);
    if (l_Result == SPELL_CAST_OK)
    {
        if (l_Caster->GetTypeId() == TYPEID_UNIT)
        {
            Creature* l_CreaturePet = l_Caster->ToCreature();
            l_CreaturePet->AddCreatureSpellCooldown(l_SpellID);
            if (l_CreaturePet->isPet())
            {
                Pet* l_Pet = (Pet*)l_CreaturePet;
                // 10% chance to play special pet attack talk, else growl
                // actually this only seems to happen on special spells, fire shield for imp, torment for voidwalker, but it's stupid to check every spell
                if (l_Pet->getPetType() == SUMMON_PET && (urand(0, 100) < 10))
                    l_Pet->SendPetTalk((uint32)PET_TALK_SPECIAL_SPELL);
                else
                    l_Pet->SendPetAIReaction(l_SpellID);
            }
        }

        l_Spell->prepare(&(l_Spell->m_targets));
    }
    else
    {
        l_Caster->SendPetCastFail(l_SpellID, l_Result, l_CastCount);
        if (l_Caster->IsPlayer())
        {
            if (!l_Caster->ToPlayer()->HasSpellCooldown(l_SpellID))
                GetPlayer()->SendClearCooldown(l_SpellID, l_Caster);
        }
        else
        {
            if (!l_Caster->ToCreature()->HasSpellCooldown(l_SpellID))
                GetPlayer()->SendClearCooldown(l_SpellID, l_Caster);
        }

        l_Spell->finish(false);
        delete l_Spell;
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

void WorldSession::HandleLearnPetSpecialization(WorldPacket & p_RecvData)
{
    uint64 l_PetGUID = 0;
    uint32 l_SpecGroupIndex = 0;

    p_RecvData.readPackGUID(l_PetGUID);
    p_RecvData >> l_SpecGroupIndex;

    if (m_Player->isInCombat())
        return;

    uint32 l_PetSpecializationId = 0;
    for (uint32 l_I = 0; l_I < sChrSpecializationsStore.GetNumRows(); l_I++)
    {
        ChrSpecializationsEntry const* l_ChrSpecialization = sChrSpecializationsStore.LookupEntry(l_I);
        if (l_ChrSpecialization == nullptr)
            continue;

        /// Pet specialization are the only one to have flags == 0/32 & classid == 0
        if ((l_ChrSpecialization->Flags != 0 && l_ChrSpecialization->Flags != 32) || l_ChrSpecialization->ClassID != 0)
            continue;

        if (l_SpecGroupIndex != l_ChrSpecialization->PetTalentType)
            continue;

        l_PetSpecializationId = l_ChrSpecialization->ID;
        break;
    }

    if (!l_PetSpecializationId)
        return;

    if (m_Player->HasAuraType(AuraType::SPELL_AURA_OVERRIDE_PET_SPECS))
    {
        switch (l_PetSpecializationId)
        {
            case SpecIndex::SPEC_PET_FEROCITY:
                l_PetSpecializationId = (uint32)SpecIndex::SPEC_ROGUE_FEROCITY_ADAPT;
                break;
            case SpecIndex::SPEC_PET_CUNNING:
                l_PetSpecializationId = (uint32)SpecIndex::SPEC_ROGUE_CUNNING_ADAPT;
                break;
            case SpecIndex::SPEC_PET_TENACITY:
                l_PetSpecializationId = (uint32)SpecIndex::SPEC_ROGUE_TENACIOUS_ADAPT;
                break;
            default:
                break;
        }
    }

    Pet* l_Pet = m_Player->GetPet();
    if (!l_Pet)
        return;

    if (l_Pet->getPetType() != PetType::HUNTER_PET)
        return;

    if (l_Pet->GetSpecializationId())
        l_Pet->UnlearnSpecializationSpell();

    l_Pet->SetSpecializationId(l_PetSpecializationId);
    l_Pet->LearnSpecializationSpell();
    m_Player->SendTalentsInfoData(true);
}
