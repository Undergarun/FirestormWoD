////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "DBCStores.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "SpellMgr.h"
#include "Log.h"
#include "Opcodes.h"
#include "Spell.h"
#include "Totem.h"
#include "TemporarySummon.h"
#include "SpellAuras.h"
#include "CreatureAI.h"
#include "ScriptMgr.h"
#include "GameObjectAI.h"
#include "SpellAuraEffects.h"
#ifndef CROSS
#include "GarrisonMgr.hpp"
#endif /* not CROSS */

void WorldSession::HandleUseItemOpcode(WorldPacket& p_RecvPacket)
{
    // TODO: add targets.read() check
    Player* pUser = m_Player;

    // ignore for remote control state
    if (pUser->m_mover != pUser)
        return;

    if (pUser->GetEmoteState())
        pUser->SetEmoteState(0);

    std::string l_SrcTargetName;

    uint64 l_TargetItemGUID         = 0;
    uint64 l_SourceTargetGUID       = 0;
    uint64 l_DestinationTargetGUID  = 0;
    uint64 l_TargetGUID             = 0;
    uint64 l_UnkGUID                = 0;
    uint64 l_ItemGUID               = 0;

    float l_MissibleTrajectorySpeed = 0.00f;
    float l_MissibleTrajectoryPitch = 0.00f;

    uint8* l_SpellWeightType      = nullptr;
    uint32* l_SpellWeightID       = nullptr;
    uint32* l_SpellWeightQuantity = nullptr;

    uint32 l_SpellID            = 0;
    uint32 l_TargetFlags        = 0;
    uint32 l_NameLenght         = 0;
    uint32 l_SpellWeightCount   = 0;

    float l_UnkFloat = 0;

    uint8 l_CastCount       = 0;
    uint8 l_SendCastFlag    = 0;
    uint8 l_Slot            = 0;
    uint8 l_PackSlot        = 0;

    uint32 l_Misc[2];

    bool l_HasSourceTarget      = false;
    bool l_HasDestinationTarget = false;
    bool l_HasUnkFloat          = false;
    bool l_HasMovementInfos     = false;

    WorldLocation l_SourceTargetPosition;
    WorldLocation l_DestinationTargetPosition;

    p_RecvPacket >> l_PackSlot;
    p_RecvPacket >> l_Slot;
    p_RecvPacket.readPackGUID(l_ItemGUID);

    p_RecvPacket >> l_CastCount;

    for (int l_I = 0; l_I < 2; l_I++)
        p_RecvPacket >> l_Misc[l_I];

    p_RecvPacket >> l_SpellID;
    p_RecvPacket.read_skip<uint32>();   ///< Unk

    l_TargetFlags           = p_RecvPacket.ReadBits(23);
    l_HasSourceTarget       = p_RecvPacket.ReadBit();
    l_HasDestinationTarget  = p_RecvPacket.ReadBit();
    l_HasUnkFloat           = p_RecvPacket.ReadBit();
    l_NameLenght            = p_RecvPacket.ReadBits(7);
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

    l_SendCastFlag      = p_RecvPacket.ReadBits(5); ///< l_SendCastFlag is never read 01/18/16
    l_HasMovementInfos  = p_RecvPacket.ReadBit();
    l_SpellWeightCount  = p_RecvPacket.ReadBits(2);

    if (l_HasMovementInfos)
        HandleMovementOpcodes(p_RecvPacket);

    if (l_SpellWeightCount)
    {
        l_SpellWeightType       = new uint8[l_SpellWeightCount];
        l_SpellWeightID         = new uint32[l_SpellWeightCount];
        l_SpellWeightQuantity   = new uint32[l_SpellWeightCount];

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

    bool l_IsGlyph = false;

    if (sSpellMgr->GetSpellInfo(l_SpellID))
    {
        switch (sSpellMgr->GetSpellInfo(l_SpellID)->Effects[EFFECT_0].Effect)
        {
            case SPELL_EFFECT_APPLY_GLYPH:
                l_IsGlyph = true;
                break;
            case SPELL_EFFECT_TEACH_FOLLOWER_ABILITY:
            case SPELL_EFFECT_INCREASE_FOLLOWER_ITEM_LEVEL:
            case SPELL_EFFECT_FINISH_GARRISON_MISSION:
            case SPELL_EFFECT_INCREASE_FOLLOWER_EXPERIENCE:
                l_TargetGUID = pUser->GetGUID();
                break;
            default:
                break;
        }
    }

    if (l_IsGlyph && l_Misc[0] >= MAX_GLYPH_SLOT_INDEX)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    Item* pItem = pUser->GetUseableItemByPos(l_PackSlot, l_Slot);
    if (!pItem)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    if (pItem->GetGUID() != l_ItemGUID)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    ItemTemplate const* proto = pItem->GetTemplate();
    if (!proto)
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pItem, NULL);
        return;
    }

    // some item classes can be used only in equipped state
    if (proto->InventoryType != INVTYPE_NON_EQUIP && !pItem->IsEquipped())
    {
        pUser->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, pItem, NULL);
        return;
    }

    InventoryResult msg = pUser->CanUseItem(pItem);
    if (msg != EQUIP_ERR_OK)
    {
        pUser->SendEquipError(msg, pItem, NULL);
        return;
    }

    // only allow conjured consumable, bandage, poisons (all should have the 2^21 item flag set in DB)
    if (proto->Class == ITEM_CLASS_CONSUMABLE && !(proto->Flags & ITEM_FLAG_USEABLE_IN_ARENA) && (pUser->InArena() || pUser->InRatedBattleGround()))
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    // don't allow items banned in arena
    if (proto->Flags & ITEM_FLAG_NOT_USEABLE_IN_ARENA && (pUser->InArena() || pUser->InRatedBattleGround()))
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    if (pUser->isInCombat())
    {
        for (int i = 0; i < MAX_ITEM_PROTO_SPELLS; ++i)
        {
            if (SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(proto->Spells[i].SpellId))
            {
                if (!spellInfo->CanBeUsedInCombat())
                {
                    pUser->SendEquipError(EQUIP_ERR_NOT_IN_COMBAT, pItem, NULL);
                    return;
                }
            }
        }
    }

    // check also  BIND_WHEN_PICKED_UP and BIND_QUEST_ITEM for .additem or .additemset case by GM (not binded at adding to inventory)
    if (pItem->GetTemplate()->Bonding == BIND_WHEN_USE || pItem->GetTemplate()->Bonding == BIND_WHEN_PICKED_UP || pItem->GetTemplate()->Bonding == BIND_QUEST_ITEM)
    {
        if (!pItem->IsSoulBound())
        {
            pItem->SetState(ITEM_CHANGED, pUser);
            pItem->SetBinding(true);
        }
    }

#ifndef CROSS
    if (l_TargetFlags & SpellCastTargetFlags::TARGET_FLAG_GARRISON_MISSION && l_Misc[0])
    {
        if (MS::Garrison::Manager* l_GarrisonMgr = pUser->GetGarrison())
        {
            if (!l_GarrisonMgr->HasPendingMission(l_Misc[0]))
            {
                /// Need to find appropriate equip error
                pUser->SendEquipError(InventoryResult::EQUIP_ERR_ITEM_NOT_FOUND, nullptr, nullptr);
                return;
            }
        }
    }
#endif

    Unit* mover = pUser->m_mover;
    if (mover != pUser && mover->IsPlayer())
        return;

    SpellCastTargets targets;

    targets.Initialize(l_TargetFlags, l_TargetGUID, l_TargetItemGUID, l_DestinationTargetGUID, l_DestinationTargetPosition, l_SourceTargetGUID, l_SourceTargetPosition);
    targets.SetElevation(l_MissibleTrajectoryPitch);
    targets.SetSpeed(l_MissibleTrajectorySpeed);
    targets.Update(mover);

    // Note: If script stop casting it must send appropriate data to client to prevent stuck item in gray state.
    if (!sScriptMgr->OnItemUse(pUser, pItem, targets))
    {
        // no script or script not process request by self
        pUser->CastItemUseSpell(pItem, targets, l_CastCount, l_Misc[0], l_Misc[1]);
    }
}

void WorldSession::HandleOpenItemOpcode(WorldPacket& p_Packet)
{
    /// ignore for remote control state
    if (m_Player->m_mover != m_Player)
        return;

    uint8 l_PackSlot = 0;
    uint8 l_Slot = 0;

    p_Packet >> l_PackSlot;
    p_Packet >> l_Slot;

    Item* l_Item = m_Player->GetItemByPos(l_PackSlot, l_Slot);

    if (!l_Item)
    {
        m_Player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    const ItemTemplate* l_ItemTemplate = l_Item->GetTemplate();

    if (!l_ItemTemplate)
    {
        m_Player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, l_Item, NULL);
        return;
    }

    /// Verify that the bag is an actual bag or wrapped item that can be used "normally"
    if (!(l_ItemTemplate->Flags & ITEM_FLAG_OPENABLE) && !l_Item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_WRAPPED))
    {
        m_Player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, l_Item, NULL);
        sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [guid: %u] tried to open item [guid: %u, entry: %u] which is not openable!",
                m_Player->GetName(), m_Player->GetGUIDLow(), l_Item->GetGUIDLow(), l_ItemTemplate->ItemId);

        return;
    }

    InventoryResult l_Result = m_Player->CanUseItem(l_ItemTemplate);
    if (l_Result != InventoryResult::EQUIP_ERR_OK)
    {
        m_Player->SendEquipError(l_Result, l_Item, NULL);
        return;
    }

    /// Locked item
    uint32 l_LockID = l_ItemTemplate->LockID;

    if (!sScriptMgr->OnItemOpen(m_Player, l_Item))
    {
        if (l_LockID)
        {
            const LockEntry* l_LockInfo = sLockStore.LookupEntry(l_LockID);

            if (!l_LockInfo)
            {
                m_Player->SendEquipError(EQUIP_ERR_ITEM_LOCKED, l_Item, NULL);
                sLog->outError(LOG_FILTER_NETWORKIO, "WORLD::OpenItem: item [guid = %u] has an unknown lockId: %u!", l_Item->GetGUIDLow(), l_LockID);

                return;
            }

            /// Was not unlocked yet
            if (l_Item->IsLocked())
            {
                m_Player->SendEquipError(EQUIP_ERR_ITEM_LOCKED, l_Item, NULL);

                return;
            }
        }

        /// Wrapped?
        if (l_Item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FIELD_FLAG_WRAPPED))
        {
            PreparedStatement* l_Stmt = SessionRealmDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GIFT_BY_ITEM);
            l_Stmt->setUInt32(0, l_Item->GetRealGUIDLow());
            PreparedQueryResult l_Result = SessionRealmDatabase.Query(l_Stmt);

            if (l_Result)
            {
                Field* l_Fields = l_Result->Fetch();
                uint32 l_Entry = l_Fields[0].GetUInt32();
                uint32 l_Flags = l_Fields[1].GetUInt32();

                l_Item->SetGuidValue(ITEM_FIELD_GIFT_CREATOR, 0);
                l_Item->SetEntry(l_Entry);
                l_Item->SetUInt32Value(ITEM_FIELD_DYNAMIC_FLAGS, l_Flags);
                l_Item->SetState(ITEM_CHANGED, m_Player);
            }
            else
            {
                sLog->outError(LOG_FILTER_NETWORKIO, "Wrapped item %u don't have record in character_gifts table and will deleted", l_Item->GetGUIDLow());
                m_Player->DestroyItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);

                return;
            }

            l_Stmt = SessionRealmDatabase.GetPreparedStatement(CHAR_DEL_GIFT);
            l_Stmt->setUInt32(0, l_Item->GetRealGUIDLow());
            SessionRealmDatabase.Execute(l_Stmt);
        }
        else
            m_Player->SendLoot(l_Item->GetGUID(), LOOT_CORPSE);
    }
}

void WorldSession::HandleGameObjectUseOpcode(WorldPacket& recvData)
{
    uint64 l_GameObjectGUID;

    recvData.readPackGUID(l_GameObjectGUID);

    // ignore for remote control state
    if (m_Player->m_mover != m_Player)
        return;

    if (GameObject* obj = GetPlayer()->GetMap()->GetGameObject(l_GameObjectGUID))
        obj->Use(m_Player);
}

void WorldSession::HandleGameobjectReportUse(WorldPacket& recvPacket)
{
    uint64 l_GameObjectGUID;

    recvPacket.readPackGUID(l_GameObjectGUID);

    // ignore for remote control state
    if (m_Player->m_mover != m_Player)
        return;

    GameObject* go = GetPlayer()->GetMap()->GetGameObject(l_GameObjectGUID);

    if (!go)
        return;

    if (!go->IsWithinDistInMap(m_Player, INTERACTION_DISTANCE))
        return;

    if (go->AI()->GossipHello(m_Player))
        return;

    m_Player->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_USE_GAMEOBJECT, go->GetEntry());
}

void WorldSession::HandleCastSpellOpcode(WorldPacket& p_RecvPacket)
{
    std::string l_SrcTargetName;

    uint64 l_TargetItemGUID = 0;
    uint64 l_SourceTargetGUID = 0;
    uint64 l_DestinationTargetGUID = 0;
    uint64 l_TargetGUID = 0;
    uint64 l_UnkGUID = 0;

    float l_MissibleTrajectorySpeed = 0.00f;
    float l_MissibleTrajectoryPitch = 0.00f;

    uint8* l_SpellWeightType      = nullptr;
    uint32* l_SpellWeightID       = nullptr;
    uint32* l_SpellWeightQuantity = nullptr;

    uint32 l_SpellID            = 0;
    uint32 l_Misc[2]            = {0, 0};
    uint32 l_TargetFlags        = 0;
    uint32 l_NameLenght         = 0;
    uint32 l_SpellWeightCount   = 0;

    float l_UnkFloat = 0;

    uint8 l_CastCount = 0;
    uint8 l_SendCastFlag = 0;

    bool l_HasSourceTarget      = false;
    bool l_HasDestinationTarget = false;
    bool l_HasUnkFloat          = false;
    bool l_HasMovementInfos     = false;

    WorldLocation l_SourceTargetPosition;
    WorldLocation l_DestinationTargetPosition;

    p_RecvPacket >> l_CastCount;

    for (int l_I = 0; l_I < 2; l_I++)
        p_RecvPacket >> l_Misc[l_I];

    p_RecvPacket >> l_SpellID;
    p_RecvPacket.read_skip<uint32>(); // unk

    l_TargetFlags           = p_RecvPacket.ReadBits(23);
    l_HasSourceTarget       = p_RecvPacket.ReadBit();
    l_HasDestinationTarget  = p_RecvPacket.ReadBit();
    l_HasUnkFloat           = p_RecvPacket.ReadBit();
    l_NameLenght            = p_RecvPacket.ReadBits(7);
    p_RecvPacket.FlushBits();
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

    l_SendCastFlag      = p_RecvPacket.ReadBits(5); ///< l_SendCastFlag is never read 01/18/16
    l_HasMovementInfos  = p_RecvPacket.ReadBit();
    l_SpellWeightCount  = p_RecvPacket.ReadBits(2);

    if (l_HasMovementInfos)
        HandleMovementOpcodes(p_RecvPacket);

    if (l_SpellWeightCount)
    {
        l_SpellWeightType       = new uint8[l_SpellWeightCount];
        l_SpellWeightID         = new uint32[l_SpellWeightCount];
        l_SpellWeightQuantity   = new uint32[l_SpellWeightCount];

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

    // ignore for remote control state (for player case)
    Unit* mover = m_Player->m_mover;
    if (mover != m_Player && mover->IsPlayer())
    {
        p_RecvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(l_SpellID);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown spell id %u", l_SpellID);
        p_RecvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    // Override spell Id, client send base spell and not the overrided id
    uint8 l_Count = std::numeric_limits<uint8>::max();
    while (!spellInfo->OverrideSpellList.empty())
    {
        if (!l_Count)
            break;

        for (auto itr : spellInfo->OverrideSpellList)
        {
            if (!l_Count)
                break;

            if (m_Player->HasSpell(itr))
            {
                SpellInfo const* overrideSpellInfo = sSpellMgr->GetSpellInfo(itr);
                if (overrideSpellInfo)
                {
                    spellInfo = overrideSpellInfo;
                    l_SpellID = itr;
                }
                break;
            }

            --l_Count;
        }
    }

    if (mover->IsPlayer())
    {
        if (mover->ToPlayer()->GetEmoteState())
            mover->ToPlayer()->SetEmoteState(0);
    }

    if (spellInfo->IsPassive())
    {
        p_RecvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    Unit* caster = mover;
    if (caster->GetTypeId() == TYPEID_UNIT && !caster->ToCreature()->HasSpell(l_SpellID))
    {
        // If the vehicle creature does not have the spell but it allows the passenger to cast own spells
        // change caster to player and let him cast
        if (!m_Player->IsOnVehicle(caster) || spellInfo->CheckVehicle(m_Player) != SPELL_CAST_OK)
        {
            p_RecvPacket.rfinish(); // prevent spam at ignore packet
            return;
        }

        caster = m_Player;
    }

    if (caster->IsPlayer()
        && !caster->ToPlayer()->HasActiveSpell(l_SpellID)
        && !spellInfo->HasEffect(SPELL_EFFECT_LOOT_BONUS)
        && !spellInfo->HasCustomAttribute(SPELL_ATTR0_CU_ALWAYS_ACTIVE)
        && !spellInfo->IsRaidMarker())
    {
        // GameObject Use
        if (IS_GAMEOBJECT_GUID(l_TargetGUID))
        {
            if (!spellInfo->HasEffect(SPELL_EFFECT_OPEN_LOCK))
            {
                p_RecvPacket.rfinish(); // prevent spam at ignore packet
                return;
            }
        }
        else
        {
            Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_UnkGUID, UNIT_NPC_FLAG2_TRADESKILL_NPC);

            if (l_Unit)
            {
#ifndef CROSS
                uint32 l_ConditionID = l_Unit->AI()->GetData(MS::Garrison::CreatureAIDataIDs::HasRecipe + l_SpellID);
#else /* CROSS */
                uint32 l_ConditionID = 0;
#endif /* CROSS */
                if (l_ConditionID == (uint32)-1)
                {
                    p_RecvPacket.rfinish();
                    return;
                }

                if (l_ConditionID != 0 && !m_Player->EvalPlayerCondition(l_ConditionID).first)
                {
                    p_RecvPacket.rfinish();
                    return;
                }

                /// All is ok, we continue
            }
            // not have spell in spellbook
            // cheater? kick? ban?
            else if(!spellInfo->IsAbilityOfSkillType(SKILL_ARCHAEOLOGY) && !spellInfo->IsCustomArchaeologySpell())
            {
                bool l_ModNextSpell = false;
                Unit::AuraEffectList const& l_AurasModNextSpell = caster->GetAuraEffectsByType(SPELL_AURA_MOD_NEXT_SPELL);
                for (auto l_Aura : l_AurasModNextSpell)
                {
                    if (l_Aura->GetSpellEffectInfo()->TriggerSpell == l_SpellID)
                    {
                        l_ModNextSpell = true;
                        break;
                    }
                }

                if (!l_ModNextSpell)
                {
                    p_RecvPacket.rfinish(); // prevent spam at ignore packet
                    return;
                }
            }
        }
    }

    Unit::AuraEffectList swaps = mover->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS);
    Unit::AuraEffectList const& swaps2 = mover->GetAuraEffectsByType(SPELL_AURA_OVERRIDE_ACTIONBAR_SPELLS_2);
    if (!swaps2.empty())
        swaps.insert(swaps.end(), swaps2.begin(), swaps2.end());

    if (!swaps.empty())
    {
        for (Unit::AuraEffectList::const_iterator itr = swaps.begin(); itr != swaps.end(); ++itr)
        {
            if ((*itr)->IsAffectingSpell(spellInfo))
            {
                if (SpellInfo const* newInfo = sSpellMgr->GetSpellInfo((*itr)->GetAmount()))
                {
                    spellInfo = newInfo;
                    l_SpellID = newInfo->Id; ///< l_SpellID is never read 01/18/16
                }
                break;
            }
        }
    }

    // Client is resending autoshot cast opcode when other spell is casted during shoot rotation
    // Skip it to prevent "interrupt" message
    if (spellInfo->IsAutoRepeatRangedSpell() && caster->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)
        && caster->GetCurrentSpell(CURRENT_AUTOREPEAT_SPELL)->m_spellInfo == spellInfo)
    {
        p_RecvPacket.rfinish();
        return;
    }

    // can't use our own spells when we're in possession of another unit,
    if (m_Player->isPossessing())
    {
        p_RecvPacket.rfinish(); // prevent spam at ignore packet
        return;
    }

    // client provided targets
    SpellCastTargets targets;

    targets.Initialize(l_TargetFlags, l_TargetGUID, l_TargetItemGUID, l_DestinationTargetGUID, l_DestinationTargetPosition, l_SourceTargetGUID, l_SourceTargetPosition);
    targets.SetElevation(l_MissibleTrajectoryPitch);
    targets.SetSpeed(l_MissibleTrajectorySpeed);
    targets.Update(mover);

    // auto-selection buff level base at target level (in spellInfo)
    if (targets.GetUnitTarget())
    {
        SpellInfo const* actualSpellInfo = spellInfo->GetAuraRankForLevel(targets.GetUnitTarget()->getLevel());

        // if rank not found then function return NULL but in explicit cast case original spell can be casted and later failed with appropriate error message
        if (actualSpellInfo)
            spellInfo = actualSpellInfo;
    }

    switch (l_SpellID) ///< Hackfix for codex of Xerrath effect on Abyssal (Guardian) and Abyssal (Summon)
    {
        case 112921: //< Abyssal(Guardian)
            if (m_Player->HasAura(137206)) 
                spellInfo = sSpellMgr->GetSpellInfo(140763);
            break;
        case 157899: //< Abyssal (Summon)
            if (m_Player->HasAura(137206)) 
                spellInfo = sSpellMgr->GetSpellInfo(157904);
            break;
        default:
            break;
    }

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE, 0, false);
    spell->m_cast_count = l_CastCount;                       // set count of casts
    spell->m_Misc[0] = l_Misc[0];
    spell->m_Misc[1] = l_Misc[1];
    spell->prepare(&targets);
}

void WorldSession::HandleCancelCastOpcode(WorldPacket& recvPacket)
{
    uint32 spellId = 0;

    recvPacket >> spellId;
    recvPacket.read_skip<uint8>();                          // counter, increments with every CANCEL packet, don't use for now

    if (m_Player->IsNonMeleeSpellCasted(false))
        m_Player->InterruptNonMeleeSpells(false, spellId, false);
}

void WorldSession::HandleCancelAuraOpcode(WorldPacket& p_Packet)
{
    uint32 l_SpellID;
    uint64 l_CasterGUID;

    p_Packet >> l_SpellID;
    p_Packet.readPackGUID(l_CasterGUID);

    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellID);

    if (!l_SpellInfo)
        return;

    /// not allow remove spells with attr SPELL_ATTR0_CANT_CANCEL
    if (l_SpellInfo->Attributes & SPELL_ATTR0_CANT_CANCEL)
        return;

    /// channeled spell case (it currently casted then)
    if (l_SpellInfo->IsChanneled() && l_SpellInfo->Id != 157627) ///< Breath of the Serpent can be canceled
    {
        if (Spell* curSpell = m_Player->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
            if (curSpell->m_spellInfo->Id == l_SpellID)
                m_Player->InterruptSpell(CURRENT_CHANNELED_SPELL);
        return;
    }

    /// non channeled case:
    /// don't allow remove non positive spells
    /// don't allow cancelling passive auras (some of them are visible)
    if (!l_SpellInfo->IsPositive() || l_SpellInfo->IsPassive())
        return;

    m_Player->RemoveOwnedAura(l_SpellID, l_CasterGUID, 0, AURA_REMOVE_BY_CANCEL);
}

void WorldSession::HandlePetCancelAuraOpcode(WorldPacket& p_RecvPacket)
{
    uint32 l_SpellID;
    uint64 l_PetGUID;

    p_RecvPacket.readPackGUID(l_PetGUID);
    l_SpellID = p_RecvPacket.read<uint32>();

    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellID);
    if (!l_SpellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %u", l_SpellID);
        return;
    }

    Creature* l_Pet = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, l_PetGUID);
    if (!l_Pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCancelAura: Attempt to cancel an aura for non-existant pet %u by player '%s'", uint32(GUID_LOPART(l_PetGUID)), GetPlayer()->GetName());
        return;
    }

    if (l_Pet != GetPlayer()->GetGuardianPet() && l_Pet != GetPlayer()->GetCharm())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCancelAura: Pet %u is not a pet of player '%s'", uint32(GUID_LOPART(l_PetGUID)), GetPlayer()->GetName());
        return;
    }

    if (!l_Pet->isAlive())
    {
        l_Pet->SendPetActionFeedback(l_SpellID, FEEDBACK_PET_DEAD);
        return;
    }

    l_Pet->RemoveOwnedAura(l_SpellID, 0, 0, AURA_REMOVE_BY_CANCEL);
    l_Pet->AddCreatureSpellCooldown(l_SpellID);
}

void WorldSession::HandleCancelGrowthAuraOpcode(WorldPacket& /*recvPacket*/)
{
}

void WorldSession::HandleCancelAutoRepeatSpellOpcode(WorldPacket& /*recvPacket*/)
{
    // may be better send SMSG_CANCEL_AUTO_REPEAT?
    // cancel and prepare for deleting
    m_Player->InterruptSpell(CURRENT_AUTOREPEAT_SPELL);
}

void WorldSession::HandleCancelChanneling(WorldPacket& recvData)
{
    recvData.read_skip<uint32>();

    /// ignore for remote control state (for player case)
    Unit* l_Mover = m_Player->m_mover;
    if (l_Mover != m_Player && l_Mover->IsPlayer())
        return;

    l_Mover->InterruptSpell(CURRENT_CHANNELED_SPELL);
}

void WorldSession::HandleTotemDestroyed(WorldPacket& recvPacket)
{
    // ignore for remote control state
    if (m_Player->m_mover != m_Player)
        return;

    uint8 slotId;
    uint64 totemGuid;

    recvPacket >> slotId;
    recvPacket.readPackGUID(totemGuid);

    if ((SUMMON_SLOT_TOTEM + slotId) >= MAX_TOTEM_SLOT * 2)
        return;

    if (!m_Player->m_SummonSlot[SUMMON_SLOT_TOTEM + slotId])
        return;

    Creature* totem = GetPlayer()->GetMap()->GetCreature(m_Player->m_SummonSlot[SUMMON_SLOT_TOTEM + slotId]);
    if (totem && totem->isTotem())
        totem->ToTotem()->UnSummon();
}

void WorldSession::HandleSelfResOpcode(WorldPacket& /*recvData*/)
{
    if (m_Player->HasAuraType(SPELL_AURA_PREVENT_RESURRECTION))
        return; // silent return, client should display error by itself and not send this opcode

    if (m_Player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL))
    {
        SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(m_Player->GetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL));
        if (spellInfo)
            m_Player->CastSpell(m_Player, spellInfo, false, 0);

        m_Player->SetUInt32Value(PLAYER_FIELD_SELF_RES_SPELL, 0);
    }
}

void WorldSession::HandleSpellClick(WorldPacket& p_Packet)
{
    uint64 l_NpcGuid = 0;
    bool l_TryAutoDismount = false;

    p_Packet.readPackGUID(l_NpcGuid);
    l_TryAutoDismount = p_Packet.ReadBit(); ///< l_tryAutoDismount is never read 01/18/16

    // this will get something not in world. crash
    Creature* l_Unit = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, l_NpcGuid);

    if (!l_Unit)
        return;

    // TODO: Unit::SetCharmedBy: 28782 is not in world but 0 is trying to charm it! -> crash
    if (!l_Unit->IsInWorld())
        return;

    m_Player->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_USE);

    l_Unit->HandleSpellClick(m_Player);
}

void WorldSession::HandleMirrorImageDataRequest(WorldPacket& recvData)
{
    uint64 guid;

    recvData.readPackGUID(guid);
    uint32 displayId = recvData.read<uint32>(); ///< displayId is never read 01/18/16

    // Get unit for which data is needed by client
    Unit* unit = ObjectAccessor::GetObjectInWorld(guid, (Unit*)NULL);
    if (!unit)
        return;

    if (!unit->HasAuraType(SPELL_AURA_CLONE_CASTER))
        return;

    // Get creator of the unit (SPELL_AURA_CLONE_CASTER does not stack)
    Unit* creator = unit->GetAuraEffectsByType(SPELL_AURA_CLONE_CASTER).front()->GetCaster();
    if (!creator)
        return;

    if (creator->GetSimulacrumTarget())
        creator = creator->GetSimulacrumTarget();

    WorldPacket data(SMSG_MIRROR_IMAGE_COMPONENTED_DATA, 76);

    if (creator->IsPlayer())
    {
        Player* player = creator->ToPlayer();
#ifndef CROSS
        Guild* guild = NULL;

        if (uint32 guildId = player->GetGuildId())
            guild = sGuildMgr->GetGuildById(guildId);
#endif /* not CROSS */

#ifndef CROSS
        uint64 guildGuid = guild ?  guild->GetGUID() : 0;
#else /* CROSS */
        uint64 guildGuid = player->GetGuildGUID();
#endif /* CROSS */

        data.appendPackGUID(guid);
        data << uint32(player->GetDisplayId());
        data << uint8(player->getRace());
        data << uint8(player->getGender());
        data << uint8(player->getClass());
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_SKIN_ID)); // skin
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_FACE_ID)); // face
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_STYLE_ID)); // hair
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, PLAYER_BYTES_OFFSET_HAIR_COLOR_ID)); // haircolor
        data << uint8(player->GetByteValue(PLAYER_FIELD_REST_STATE, PLAYER_BYTES_2_OFFSET_FACIAL_STYLE));     // facialhair
        data.appendPackGUID(guildGuid);

        data << uint32(11);

        static EquipmentSlots const itemSlots[] =
        {
            EQUIPMENT_SLOT_HEAD,
            EQUIPMENT_SLOT_SHOULDERS,
            EQUIPMENT_SLOT_BODY,
            EQUIPMENT_SLOT_CHEST,
            EQUIPMENT_SLOT_WAIST,
            EQUIPMENT_SLOT_LEGS,
            EQUIPMENT_SLOT_FEET,
            EQUIPMENT_SLOT_WRISTS,
            EQUIPMENT_SLOT_HANDS,
            EQUIPMENT_SLOT_TABARD,
            EQUIPMENT_SLOT_BACK,
            EQUIPMENT_SLOT_END
        };

        // Display items in visible slots
        for (EquipmentSlots const* itr = itemSlots; *itr != EQUIPMENT_SLOT_END; ++itr)
        {
            if (*itr == EQUIPMENT_SLOT_HEAD && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
                data << uint32(0);
            else if (*itr == EQUIPMENT_SLOT_BACK && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
                data << uint32(0);
            else if (ItemTemplate const* l_Proto = sObjectMgr->GetItemTemplate(player->GetUInt32Value(PLAYER_FIELD_VISIBLE_ITEMS + (*itr * 2))))
                data << uint32(l_Proto->DisplayInfoID);
            else
                data << uint32(0);
        }
    }
    else
    {
        ObjectGuid guildGuid = 0;

        data.appendPackGUID(guid);
        data << uint32(creator->GetDisplayId());
        data << uint8(creator->getRace());
        data << uint8(creator->getGender());
        data << uint8(creator->getClass());
        data << uint8(0);   // skin
        data << uint8(0);   // face
        data << uint8(0);   // hair
        data << uint8(0);   // haircolor
        data << uint8(0);   // facialhair
        data.appendPackGUID(guildGuid);

        data << uint32(0);
    }

    SendPacket(&data);
}

//////////////////////////////////////////////////////////////////////////
/// ToyBox
void WorldSession::HandleAddNewToyToBoxOpcode(WorldPacket& p_RecvData)
{
    uint64 l_ItemGuid = 0;
    p_RecvData.readPackGUID(l_ItemGuid);

    Item* l_Item = m_Player->GetItemByGuid(l_ItemGuid);
    if (!l_Item)
        return;

    if (m_Player->HasToy(l_Item->GetEntry()))
        return;

    m_Player->AddNewToyToBox(l_Item->GetEntry());
    m_Player->DestroyItem(l_Item->GetBagSlot(), l_Item->GetSlot(), true);
    m_Player->SendPlaySpellVisualKit(179, 0);   // SpellCastDirected
}

void WorldSession::HandleSetFavoriteToyOpcode(WorldPacket& p_RecvData)
{
    uint32 l_ItemID = p_RecvData.read<uint32>();
    bool l_Apply = p_RecvData.ReadBit();

    if (!m_Player->HasToy(l_ItemID))
        return;

    m_Player->SetFavoriteToy(l_Apply, l_ItemID);
}

void WorldSession::HandleUseToyOpcode(WorldPacket& p_RecvData)
{
    uint32 l_ItemID = p_RecvData.read<uint32>();

    if (!m_Player->HasToy(l_ItemID))
    {
        p_RecvData.rfinish();
        return;
    }

    std::string l_SrcTargetName;

    uint64 l_TargetItemGUID = 0;
    uint64 l_SourceTargetGUID = 0;
    uint64 l_DestinationTargetGUID = 0;
    uint64 l_TargetGUID = 0;
    uint64 l_UnkGUID = 0;

    float l_MissibleTrajectorySpeed = 0.00f;
    float l_MissibleTrajectoryPitch = 0.00f;

    uint8* l_SpellWeightType      = nullptr;
    uint32* l_SpellWeightID       = nullptr;
    uint32* l_SpellWeightQuantity = nullptr;

    uint32 l_SpellID = 0;
    uint32 l_TargetFlags = 0;
    uint32 l_NameLenght = 0;
    uint32 l_SpellWeightCount = 0;

    uint32 l_Misc[2] = { 0, 0 };

    float l_UnkFloat = 0;

    uint8 l_CastCount = 0;
    uint8 l_SendCastFlag = 0;

    bool l_HasSourceTarget = false;
    bool l_HasDestinationTarget = false;
    bool l_HasUnkFloat = false;
    bool l_HasMovementInfos = false;

    WorldLocation l_SourceTargetPosition;
    WorldLocation l_DestinationTargetPosition;

    p_RecvData >> l_CastCount;

    for (int l_I = 0; l_I < 2; l_I++)
        p_RecvData >> l_Misc[l_I];

    p_RecvData >> l_SpellID;
    p_RecvData.read_skip<uint32>(); ///< Unk

    l_TargetFlags = p_RecvData.ReadBits(23);
    l_HasSourceTarget = p_RecvData.ReadBit();
    l_HasDestinationTarget = p_RecvData.ReadBit();
    l_HasUnkFloat = p_RecvData.ReadBit();
    l_NameLenght = p_RecvData.ReadBits(7);
    p_RecvData.FlushBits();
    p_RecvData.readPackGUID(l_TargetGUID);
    p_RecvData.readPackGUID(l_TargetItemGUID);

    if (l_HasSourceTarget)
    {
        p_RecvData.readPackGUID(l_SourceTargetGUID);
        p_RecvData >> l_SourceTargetPosition.m_positionX;
        p_RecvData >> l_SourceTargetPosition.m_positionY;
        p_RecvData >> l_SourceTargetPosition.m_positionZ;
    }

    if (l_HasDestinationTarget)
    {
        p_RecvData.readPackGUID(l_DestinationTargetGUID);
        p_RecvData >> l_DestinationTargetPosition.m_positionX;
        p_RecvData >> l_DestinationTargetPosition.m_positionY;
        p_RecvData >> l_DestinationTargetPosition.m_positionZ;
    }

    if (l_HasUnkFloat)
        p_RecvData >> l_UnkFloat;

    l_SrcTargetName = p_RecvData.ReadString(l_NameLenght);

    p_RecvData >> l_MissibleTrajectoryPitch;
    p_RecvData >> l_MissibleTrajectorySpeed;

    p_RecvData.readPackGUID(l_UnkGUID);

    l_SendCastFlag = p_RecvData.ReadBits(5); ///< l_SendCastFlag is never read 01/18/16
    l_HasMovementInfos = p_RecvData.ReadBit();
    l_SpellWeightCount = p_RecvData.ReadBits(2);

    if (l_HasMovementInfos)
        HandleMovementOpcodes(p_RecvData);

    if (l_SpellWeightCount)
    {
        l_SpellWeightType = new uint8[l_SpellWeightCount];
        l_SpellWeightID = new uint32[l_SpellWeightCount];
        l_SpellWeightQuantity = new uint32[l_SpellWeightCount];

        for (uint32 l_I = 0; l_I < l_SpellWeightCount; ++l_I)
        {
            l_SpellWeightType[l_I] = p_RecvData.ReadBits(2);
            p_RecvData >> l_SpellWeightID[l_I];
            p_RecvData >> l_SpellWeightQuantity[l_I];
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

    // ignore for remote control state (for player case)
    Unit* l_Mover = m_Player->m_mover;
    if (l_Mover != m_Player && l_Mover->IsPlayer())
    {
        p_RecvData.rfinish();
        return;
    }

    SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_SpellID);
    if (!l_SpellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown spell id %u", l_SpellID);
        p_RecvData.rfinish();
        return;
    }

    ItemTemplate const* l_ItemProto = sObjectMgr->GetItemTemplate(l_ItemID);
    if (!l_ItemProto)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown item id %u", l_ItemID);
        p_RecvData.rfinish();
        return;
    }

    bool l_Found = false;
    for (int l_I = 0; l_I < MAX_ITEM_PROTO_SPELLS; ++l_I)
    {
        if (l_ItemProto->Spells[l_I].SpellId == l_SpellID)
        {
            l_Found = true;
            break;
        }
    }

    /// Cheater?
    if (!l_Found)
    {
        sLog->outAshran("HandleUseToyOpcode: Player %s [%u] Trying to spoof packet and cast spell %u", m_Player->GetName(), m_Player->GetGUIDLow(), l_SpellID);
        p_RecvData.rfinish();
        return;
    }

    SpellCastTargets l_Targets;

    l_Targets.Initialize(l_TargetFlags, l_TargetGUID, l_TargetItemGUID, l_DestinationTargetGUID, l_DestinationTargetPosition, l_SourceTargetGUID, l_SourceTargetPosition);
    l_Targets.SetElevation(l_MissibleTrajectoryPitch);
    l_Targets.SetSpeed(l_MissibleTrajectorySpeed);
    l_Targets.Update(l_Mover);

    Spell* l_Spell = new Spell(l_Mover, l_SpellInfo, TRIGGERED_NONE, 0, false);
    l_Spell->m_cast_count = l_CastCount;
    l_Spell->m_CastItemEntry = l_ItemID;
    l_Spell->m_Misc[0] = l_Misc[0];
    l_Spell->m_Misc[1] = l_Misc[1];
    l_Spell->prepare(&l_Targets);
}
//////////////////////////////////////////////////////////////////////////
