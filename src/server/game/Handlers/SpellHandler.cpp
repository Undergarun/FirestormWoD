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

void WorldSession::HandleClientCastFlags(WorldPacket& recvPacket, uint8 castFlags, SpellCastTargets& targets)
{
    if (castFlags & 0x8)   // Archaeology
    {
        uint32 count, entry, usedCount;
        uint8 type;
        recvPacket >> count;
        for (uint32 i = 0; i < count; ++i)
        {
            recvPacket >> type;
            switch (type)
            {
                case 2: // Keystones
                    recvPacket >> entry;        // Item id
                    recvPacket >> usedCount;    // Item count
                    GetPlayer()->GetArchaeologyMgr().AddProjectCost(entry, usedCount, false);
                    break;
                case 1: // Fragments
                    recvPacket >> entry;        // Currency id
                    recvPacket >> usedCount;    // Currency count
                    GetPlayer()->GetArchaeologyMgr().AddProjectCost(entry, usedCount, true);
                    break;
            }
        }
    }
}

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

    uint8   * l_SpellWeightType     = nullptr;
    uint32  * l_SpellWeightID       = nullptr;
    uint32  * l_SpellWeightQuantity = nullptr;

    uint32 l_SpellID            = 0;
    uint32 l_Misc               = 0;
    uint32 l_TargetFlags        = 0;
    uint32 l_NameLenght         = 0;
    uint32 l_SpellWeightCount   = 0;

    float l_UnkFloat = 0;

    uint8 l_CastCount       = 0;
    uint8 l_SendCastFlag    = 0;
    uint8 l_Slot            = 0;
    uint8 l_PackSlot        = 0;

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
    p_RecvPacket >> l_SpellID;
    p_RecvPacket >> l_Misc;

    l_TargetFlags           = p_RecvPacket.ReadBits(20);
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

    l_SendCastFlag      = p_RecvPacket.ReadBits(5);
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
        for (uint32 l_I = 0; l_I < l_SpellWeightCount; l_I++)
        {
            switch (l_SpellWeightType[l_I])
            {
                case SPELL_WEIGHT_ARCHEOLOGY_FRAGMENTS: // Fragments
                case SPELL_WEIGHT_ARCHEOLOGY_KEYSTONES: // Keystones
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

    if (l_Misc >= MAX_GLYPH_SLOT_INDEX)
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

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_USE_ITEM packet, bagIndex: %u, slot: %u, castCount: %u, spellId: %u, Item: %u, glyphIndex: %u, data length = %i", l_PackSlot, l_Slot, l_CastCount, l_SpellID, pItem->GetEntry(), l_Misc, (uint32)p_RecvPacket.size());

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
    if (proto->Class == ITEM_CLASS_CONSUMABLE && !(proto->Flags & ITEM_PROTO_FLAG_USEABLE_IN_ARENA) && (pUser->InArena() || pUser->InRatedBattleGround()))
    {
        pUser->SendEquipError(EQUIP_ERR_NOT_DURING_ARENA_MATCH, pItem, NULL);
        return;
    }

    // don't allow items banned in arena
    if (proto->Flags & ITEM_PROTO_FLAG_NOT_USEABLE_IN_ARENA && (pUser->InArena() || pUser->InRatedBattleGround()))
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

    Unit* mover = pUser->m_mover;
    if (mover != pUser && mover->GetTypeId() == TYPEID_PLAYER)
        return;

    SpellCastTargets targets;

    targets.Initialize(l_SendCastFlag, l_TargetGUID, l_TargetItemGUID, l_DestinationTargetGUID, l_DestinationTargetPosition, l_SourceTargetGUID, l_SourceTargetPosition);
    targets.SetElevation(l_MissibleTrajectoryPitch);
    targets.SetSpeed(l_MissibleTrajectorySpeed);
    targets.Update(mover);

    // Note: If script stop casting it must send appropriate data to client to prevent stuck item in gray state.
    if (!sScriptMgr->OnItemUse(pUser, pItem, targets))
    {
        // no script or script not process request by self
        pUser->CastItemUseSpell(pItem, targets, l_CastCount, l_Misc);
    }
}

void WorldSession::HandleOpenItemOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_OPEN_ITEM packet, data length = %i", (uint32)p_Packet.size());

    /// ignore for remote control state
    if (m_Player->m_mover != m_Player)
        return;

    uint8 l_PackSlot = 0;
    uint8 l_Slot = 0;

    p_Packet >> l_PackSlot;
    p_Packet >> l_Slot;

    sLog->outInfo(LOG_FILTER_NETWORKIO, "bagIndex: %u, slot: %u", l_PackSlot, l_Slot);

    Item* l_Item = m_Player->GetItemByPos(l_PackSlot, l_Slot);

    if (!l_Item)
    {
        m_Player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, NULL, NULL);
        return;
    }

    const ItemTemplate * l_ItemTemplate = l_Item->GetTemplate();

    if (!l_ItemTemplate)
    {
        m_Player->SendEquipError(EQUIP_ERR_ITEM_NOT_FOUND, l_Item, NULL);
        return;
    }

    /// Verify that the bag is an actual bag or wrapped item that can be used "normally"
    if (!(l_ItemTemplate->Flags & ITEM_PROTO_FLAG_OPENABLE) && !l_Item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
    {
        m_Player->SendEquipError(EQUIP_ERR_CLIENT_LOCKED_OUT, l_Item, NULL);
        sLog->outError(LOG_FILTER_NETWORKIO, "Possible hacking attempt: Player %s [guid: %u] tried to open item [guid: %u, entry: %u] which is not openable!",
                m_Player->GetName(), m_Player->GetGUIDLow(), l_Item->GetGUIDLow(), l_ItemTemplate->ItemId);

        return;
    }

    /// Locked item
    uint32 l_LockID = l_ItemTemplate->LockID;

    if (l_LockID)
    {
        const LockEntry * l_LockInfo = sLockStore.LookupEntry(l_LockID);

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
    if (l_Item->HasFlag(ITEM_FIELD_DYNAMIC_FLAGS, ITEM_FLAG_WRAPPED))
    {
        PreparedStatement* l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_CHARACTER_GIFT_BY_ITEM);

        l_Stmt->setUInt32(0, l_Item->GetGUIDLow());

        PreparedQueryResult l_Result = CharacterDatabase.Query(l_Stmt);

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

        l_Stmt = CharacterDatabase.GetPreparedStatement(CHAR_DEL_GIFT);

        l_Stmt->setUInt32(0, l_Item->GetGUIDLow());

        CharacterDatabase.Execute(l_Stmt);
    }
    else
        m_Player->SendLoot(l_Item->GetGUID(), LOOT_CORPSE);
}

void WorldSession::HandleGameObjectUseOpcode(WorldPacket& recvData)
{
    uint64 l_GameObjectGUID;

    recvData.readPackGUID(l_GameObjectGUID);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_GAMEOBJECT_USE Message [guid=%u]", GUID_LOPART(l_GameObjectGUID));

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

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recvd CMSG_GAMEOBJECT_REPORT_USE Message [in game guid: %u]", GUID_LOPART(l_GameObjectGUID));

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

    uint8   * l_SpellWeightType     = nullptr;
    uint32  * l_SpellWeightID       = nullptr;
    uint32  * l_SpellWeightQuantity = nullptr;

    uint32 l_SpellID            = 0;
    uint32 l_Misc               = 0;
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
    p_RecvPacket >> l_SpellID;
    p_RecvPacket >> l_Misc;

    l_TargetFlags           = p_RecvPacket.ReadBits(21);
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

    l_SendCastFlag      = p_RecvPacket.ReadBits(5);
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
        for (uint32 l_I = 0; l_I < l_SpellWeightCount; l_I++)
        {
            switch (l_SpellWeightType[l_I])
            {
                case SPELL_WEIGHT_ARCHEOLOGY_FRAGMENTS: // Fragments
                case SPELL_WEIGHT_ARCHEOLOGY_KEYSTONES: // Keystones
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

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: got cast spell packet, castCount: %u, spellId: %u, targetFlags: %u, data length = %u", l_CastCount, l_SpellID, l_TargetFlags, (uint32)p_RecvPacket.size());

    // ignore for remote control state (for player case)
    Unit* mover = m_Player->m_mover;
    if (mover != m_Player && mover->GetTypeId() == TYPEID_PLAYER)
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
    if (!spellInfo->OverrideSpellList.empty())
    {
        for (auto itr : spellInfo->OverrideSpellList)
        {
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
        }
    }

    if (mover->GetTypeId() == TYPEID_PLAYER)
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

    if (caster->GetTypeId() == TYPEID_PLAYER &&
        !caster->ToPlayer()->HasActiveSpell(l_SpellID) &&
         l_SpellID != 101603 && // Hack for Throw Totem, Echo of Baine
         l_SpellID != 1843 && !spellInfo->IsRaidMarker() && !IS_GAMEOBJECT_GUID(l_TargetGUID)) // Hack for disarm. Client sends the spell instead of gameobjectuse.
    {
        // not have spell in spellbook
        // cheater? kick? ban?
        if (!spellInfo->IsAbilityOfSkillType(SKILL_ARCHAEOLOGY) && !spellInfo->IsCustomArchaeologySpell() && !spellInfo->HasEffect(SPELL_EFFECT_LOOT_BONUS))
        {
            p_RecvPacket.rfinish(); // prevent spam at ignore packet
            return;
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
                    l_SpellID = newInfo->Id;
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

    Spell* spell = new Spell(caster, spellInfo, TRIGGERED_NONE, 0, false);
    spell->m_cast_count = l_CastCount;                       // set count of casts
    spell->m_glyphIndex = l_Misc;
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
    if (l_SpellInfo->IsChanneled())
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

void WorldSession::HandlePetCancelAuraOpcode(WorldPacket& recvPacket)
{
    ObjectGuid guid;
    uint32 spellId;

    recvPacket >> spellId;
    uint8 bitOrder[8] = {4, 6, 5, 2, 3, 1, 0, 7};
    recvPacket.ReadBitInOrder(guid, bitOrder);
    uint8 byteOrder[8] = {1, 3, 2, 5, 0, 6, 7, 4};
    recvPacket.ReadBytesSeq(guid, byteOrder);

    SpellInfo const* spellInfo = sSpellMgr->GetSpellInfo(spellId);
    if (!spellInfo)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WORLD: unknown PET spell id %u", spellId);
        return;
    }

    Creature* pet=ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, guid);

    if (!pet)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCancelAura: Attempt to cancel an aura for non-existant pet %u by player '%s'", uint32(GUID_LOPART(guid)), GetPlayer()->GetName());
        return;
    }

    if (pet != GetPlayer()->GetGuardianPet() && pet != GetPlayer()->GetCharm())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "HandlePetCancelAura: Pet %u is not a pet of player '%s'", uint32(GUID_LOPART(guid)), GetPlayer()->GetName());
        return;
    }

    if (!pet->isAlive())
    {
        pet->SendPetActionFeedback(FEEDBACK_PET_DEAD);
        return;
    }

    pet->RemoveOwnedAura(spellId, 0, 0, AURA_REMOVE_BY_CANCEL);

    pet->AddCreatureSpellCooldown(spellId);
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
    Unit * l_Mover = m_Player->m_mover;
    if (l_Mover != m_Player && l_Mover->GetTypeId() == TYPEID_PLAYER)
        return;

    l_Mover->InterruptSpell(CURRENT_CHANNELED_SPELL);
}

void WorldSession::HandleTotemDestroyed(WorldPacket& recvPacket)
{
    // ignore for remote control state
    if (m_Player->m_mover != m_Player)
        return;

    uint8 slotId;
    ObjectGuid totemGuid;

    recvPacket >> slotId;

    uint8 bitsOrder[8] = { 5, 3, 0, 7, 5, 6, 2, 1 };
    recvPacket.ReadBitInOrder(totemGuid, bitsOrder);

    recvPacket.FlushBits();

    uint8 bytesOrder[8] = { 7, 2, 0, 6, 5, 3, 4, 1 };
    recvPacket.ReadBytesSeq(totemGuid, bytesOrder);

    ++slotId;
    if (slotId >= MAX_TOTEM_SLOT)
        return;

    if (!m_Player->m_SummonSlot[slotId])
        return;

    Creature* totem = GetPlayer()->GetMap()->GetCreature(m_Player->m_SummonSlot[slotId]);
    if (totem && totem->isTotem())
        totem->ToTotem()->UnSummon();
}

void WorldSession::HandleSelfResOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_SELF_RES");                  // empty opcode

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
    l_TryAutoDismount = p_Packet.ReadBit();

    // this will get something not in world. crash
    Creature * l_Unit = ObjectAccessor::GetCreatureOrPetOrVehicle(*m_Player, l_NpcGuid);

    if (!l_Unit)
        return;

    // TODO: Unit::SetCharmedBy: 28782 is not in world but 0 is trying to charm it! -> crash
    if (!l_Unit->IsInWorld())
        return;

    l_Unit->HandleSpellClick(m_Player);
}

void WorldSession::HandleMirrorImageDataRequest(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_GET_MIRRORIMAGE_DATA");
    ObjectGuid guid;
    uint32 displayId = recvData.read<uint32>();

    uint8 bits[8] = { 0, 4, 1, 7, 6, 2, 5, 3 };
    recvData.ReadBitInOrder(guid, bits);

    uint8 bytes[8] = { 4, 0, 6, 7, 3, 2, 1, 5 };
    recvData.ReadBytesSeq(guid, bytes);

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

    WorldPacket data(SMSG_MIRROR_IMAGE_COMPONENTED_DATA, 68);

    if (creator->GetTypeId() == TYPEID_PLAYER)
    {
        Player* player = creator->ToPlayer();
        Guild* guild = NULL;

        if (uint32 guildId = player->GetGuildId())
            guild = sGuildMgr->GetGuildById(guildId);

        ObjectGuid guildGuid = guild ?  guild->GetGUID() : 0;

        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 3)); // haircolor
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 2)); // hair
        data << uint8(player->getRace());
        data << uint8(player->getGender());
        data << uint8(player->GetByteValue(PLAYER_FIELD_REST_STATE, 0));     // facialhair
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 0)); // skin
        data << uint32(player->GetDisplayId());
        data << uint8(player->GetByteValue(PLAYER_FIELD_HAIR_COLOR_ID, 1)); // face
        data << uint8(player->getClass());

        data.WriteBit(guid[7]);
        data.WriteBit(guildGuid[3]);
        data.WriteBit(guildGuid[1]);
        data.WriteBit(guildGuid[5]);
        data.WriteBit(guildGuid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(guildGuid[4]);
        data.WriteBit(guid[3]);
        data.WriteBit(guildGuid[6]);
        data.WriteBits(11, 22);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[4]);
        data.WriteBit(guildGuid[2]);
        data.WriteBit(guildGuid[0]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[2]);

        data.WriteByteSeq(guildGuid[5]);
        data.WriteByteSeq(guildGuid[2]);
        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[6]);

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
            EQUIPMENT_SLOT_BACK,
            EQUIPMENT_SLOT_TABARD,
            EQUIPMENT_SLOT_END
        };

        // Display items in visible slots
        for (EquipmentSlots const* itr = &itemSlots[0]; *itr != EQUIPMENT_SLOT_END; ++itr)
        {
            if (*itr == EQUIPMENT_SLOT_HEAD && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_HELM))
                data << uint32(0);
            else if (*itr == EQUIPMENT_SLOT_BACK && player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HIDE_CLOAK))
                data << uint32(0);
            else if (Item const* item = player->GetItemByPos(INVENTORY_SLOT_BAG_0, *itr))
            {
                // Display Transmogrifications on player's clone
                if (ItemTemplate const* proto = sObjectMgr->GetItemTemplate(item->GetDynamicUInt32Value(ITEM_DYNAMIC_MODIFIERS, 1)))
                    data << uint32(proto->DisplayInfoID);
                else
                    data << uint32(item->GetTemplate()->DisplayInfoID);
            }
            else
                data << uint32(0);
        }

        data.WriteByteSeq(guildGuid[1]);
        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[2]);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guildGuid[3]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guildGuid[4]);
        data.WriteByteSeq(guildGuid[0]);
        data.WriteByteSeq(guildGuid[6]);
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guildGuid[7]);
    }
    else
    {
        ObjectGuid guildGuid = 0;

        data << uint8(0);   // skin
        data << uint8(creator->getRace());
        data << uint8(0);   // face
        data << uint8(creator->getGender());
        data << uint8(0);   // hair
        data << uint8(0);   // haircolor
        data << uint32(creator->GetDisplayId());
        data << uint8(0);   // facialhair
        data << uint8(creator->getClass());

        data.WriteBit(guid[7]);
        data.WriteBit(guildGuid[3]);
        data.WriteBit(guildGuid[1]);
        data.WriteBit(guildGuid[5]);
        data.WriteBit(guildGuid[7]);
        data.WriteBit(guid[6]);
        data.WriteBit(guildGuid[4]);
        data.WriteBit(guid[3]);
        data.WriteBit(guildGuid[6]);
        data.WriteBits(0, 22);
        data.WriteBit(guid[5]);
        data.WriteBit(guid[1]);
        data.WriteBit(guid[4]);
        data.WriteBit(guildGuid[2]);
        data.WriteBit(guildGuid[0]);
        data.WriteBit(guid[0]);
        data.WriteBit(guid[2]);

        data.WriteByteSeq(guildGuid[5]);
        data.WriteByteSeq(guildGuid[2]);
        data.WriteByteSeq(guid[4]);
        data.WriteByteSeq(guid[6]);
        data.WriteByteSeq(guildGuid[1]);
        data.WriteByteSeq(guid[1]);
        data.WriteByteSeq(guid[3]);
        data.WriteByteSeq(guid[2]);
        data.WriteByteSeq(guid[7]);
        data.WriteByteSeq(guildGuid[3]);
        data.WriteByteSeq(guid[5]);
        data.WriteByteSeq(guildGuid[4]);
        data.WriteByteSeq(guildGuid[0]);
        data.WriteByteSeq(guildGuid[6]);
        data.WriteByteSeq(guid[0]);
        data.WriteByteSeq(guildGuid[7]);
    }

    SendPacket(&data);
}

void WorldSession::HandleUpdateProjectilePosition(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_UPDATE_PROJECTILE_POSITION");

    uint64 casterGuid;
    uint32 spellId;
    uint8 castCount;
    float x, y, z;    // Position of missile hit

    recvPacket >> casterGuid;
    recvPacket >> spellId;
    recvPacket >> castCount;
    recvPacket >> x;
    recvPacket >> y;
    recvPacket >> z;

    Unit* caster = ObjectAccessor::GetUnit(*m_Player, casterGuid);
    if (!caster)
        return;

    Spell* spell = caster->FindCurrentSpellBySpellId(spellId);
    if (!spell || !spell->m_targets.HasDst())
        return;

    Position pos = *spell->m_targets.GetDstPos();
    pos.Relocate(x, y, z);
    spell->m_targets.ModDst(pos);

    WorldPacket data(SMSG_SET_PROJECTILE_POSITION, 21);
    data << uint64(casterGuid);
    data << uint8(castCount);
    data << float(x);
    data << float(y);
    data << float(z);
    caster->SendMessageToSet(&data, true);
}