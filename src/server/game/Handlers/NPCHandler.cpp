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
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SpellMgr.h"
#include "Player.h"
#include "GossipDef.h"
#include "UpdateMask.h"
#include "ObjectAccessor.h"
#include "Creature.h"
#include "Pet.h"
#include "BattlegroundMgr.h"
#include "Battleground.h"
#include "ScriptMgr.h"
#include "CreatureAI.h"
#include "SpellInfo.h"
#include "Guild.h"

enum StableResultCode
{
    STABLE_ERR_MONEY        = 0x01,                         // "you don't have enough money"
    STABLE_ERR_STABLE       = 0x06,                         // currently used in most fail cases
    STABLE_SUCCESS_STABLE   = 0x08,                         // stable success
    STABLE_SUCCESS_UNSTABLE = 0x09,                         // unstable/swap success
    STABLE_SUCCESS_BUY_SLOT = 0x0A,                         // buy slot success
    STABLE_ERR_EXOTIC       = 0x0C,                         // "you are unable to control exotic creatures"
};

void WorldSession::HandleTabardVendorActivateOpcode(WorldPacket& recvData)
{
    uint64 guid;
    recvData >> guid;

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TABARDDESIGNER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTabardVendorActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendTabardVendorActivate(guid);
}

void WorldSession::SendTabardVendorActivate(uint64 guid)
{
    ObjectGuid playerGuid = guid;
    WorldPacket data(SMSG_PLAYER_TABAR_VENDOR_SHOW);

    uint8 bitsOrder[8] = { 7, 0, 3, 6, 4, 1, 5, 2 };
    data.WriteBitInOrder(playerGuid, bitsOrder);

    uint8 bytesOrder[8] = { 6, 2, 5, 7, 1, 0, 4, 3 };
    data.WriteBytesSeq(playerGuid, bytesOrder);

    SendPacket(&data);
}

void WorldSession::HandleBankerActivateOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_BANKER_ACTIVATE");

    uint8 bitsOrder[8] = { 0, 2, 1, 6, 7, 3, 4, 5 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 7, 4, 0, 3, 2, 1, 5, 6 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_BANKER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBankerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendShowBank(guid);
}

void WorldSession::SendShowBank(uint64 guid)
{
    WorldPacket data(SMSG_SHOW_BANK);
    data.appendPackGUID(guid);

    SendPacket(&data);
}

void WorldSession::HandleTrainerListOpcode(WorldPacket & p_Packet)
{
    uint64 l_TrainerGUID = 0;

    p_Packet.readPackGUID(l_TrainerGUID);

    SendTrainerList(l_TrainerGUID);
}

void WorldSession::SendTrainerList(uint64 guid)
{
    std::string str = GetTrinityString(LANG_NPC_TAINER_HELLO);
    SendTrainerList(guid, str);
}

void WorldSession::SendTrainerList(uint64 p_NpcGUID, const std::string& p_Title)
{
    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWith(p_NpcGUID, UNIT_NPC_FLAG_TRAINER);
    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(p_NpcGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // trainer list loaded at check;
    if (!l_Unit->isCanTrainingOf(m_Player, true))
        return;

    const CreatureTemplate * l_CreatureTemplate = l_Unit->GetCreatureTemplate();

    if (!l_CreatureTemplate)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - (GUID: %u) NO CREATUREINFO!", GUID_LOPART(p_NpcGUID));
        return;
    }

    const TrainerSpellData * l_TrainerSpells = l_Unit->GetTrainerSpells();

    if (!l_TrainerSpells)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - Training spells not found for creature (GUID: %u Entry: %u)",
            GUID_LOPART(p_NpcGUID), l_Unit->GetEntry());
        return;
    }

    ByteBuffer l_TrainerList;

    // reputation discount
    float l_DiscountMod = m_Player->GetReputationPriceDiscount(l_Unit);
    bool l_CanLearnPrimaryProfession = GetPlayer()->GetFreePrimaryProfessionPoints() > 0;

    uint32 l_TrainerSpellCount = 0;
    for (TrainerSpellMap::const_iterator itr = l_TrainerSpells->spellList.begin(); itr != l_TrainerSpells->spellList.end(); ++itr)
    {
        TrainerSpell const* l_TrainerSpell = &itr->second;

        bool l_Valid = true;
        bool l_PrimaryProfesionFirstRank = false;

        for (uint8 l_I = 0; l_I < MAX_SPELL_EFFECTS; ++l_I)
        {
            if (!l_TrainerSpell->learnedSpell[l_I])
                continue;

            if (!m_Player->IsSpellFitByClassAndRace(l_TrainerSpell->learnedSpell[l_I]))
            {
                l_Valid = false;
                break;
            }

            const SpellInfo * l_LearnedSpellInfo = sSpellMgr->GetSpellInfo(l_TrainerSpell->learnedSpell[l_I]);

            if (l_LearnedSpellInfo && l_LearnedSpellInfo->IsPrimaryProfessionFirstRank())
                l_PrimaryProfesionFirstRank = true;
        }

        if (!l_Valid)
            continue;

        TrainerSpellState state = m_Player->GetTrainerSpellState(l_TrainerSpell);

        l_TrainerList << uint32(l_TrainerSpell->spell);
        l_TrainerList << uint32(floor(l_TrainerSpell->spellCost * l_DiscountMod));
        l_TrainerList << uint32(l_TrainerSpell->reqSkill);
        l_TrainerList << uint32(l_TrainerSpell->reqSkillValue);

        //prev + req or req + 0
        uint8 l_MaxReq = 0;
        for (uint8 l_I = 0; l_I < MAX_SPELL_EFFECTS; ++l_I)
        {
            if (!l_TrainerSpell->learnedSpell[l_I])
                continue;

            if (uint32 prevSpellId = sSpellMgr->GetPrevSpellInChain(l_TrainerSpell->learnedSpell[l_I]))
            {
                l_TrainerList << uint32(prevSpellId);
                ++l_MaxReq;
            }

            if (l_MaxReq == 1)
                break;

            SpellsRequiringSpellMapBounds l_RequiredSpells = sSpellMgr->GetSpellsRequiredForSpellBounds(l_TrainerSpell->learnedSpell[l_I]);

            for (SpellsRequiringSpellMap::const_iterator l_It = l_RequiredSpells.first; l_It != l_RequiredSpells.second && l_MaxReq < 2; ++l_It)
            {
                l_TrainerList << uint32(l_It->second);
                ++l_MaxReq;
            }

            if (l_MaxReq == 1)
                break;
        }
        while (l_MaxReq < 1)
        {
            l_TrainerList << uint32(0);
            ++l_MaxReq;
        }
        l_TrainerList << uint32(0); // Profession Dialog or Profession Button
        l_TrainerList << uint32(0); // Profession Dialog or Profession Button

        l_TrainerList << uint8(state == TRAINER_SPELL_GREEN_DISABLED ? TRAINER_SPELL_GREEN : state);
        l_TrainerList << uint8(l_TrainerSpell->reqLevel);

        ++l_TrainerSpellCount;
    }

    WorldPacket l_Data(SMSG_TRAINER_LIST);

    l_Data.appendPackGUID(p_NpcGUID);

    l_Data << uint32(l_Unit->GetCreatureTemplate()->trainer_type);
    l_Data << uint32(1); // different value for each trainer, also found in CMSG_TRAINER_BUY_SPELL
    l_Data << uint32(l_TrainerSpellCount);

    l_Data.append(l_TrainerList);

    l_Data.WriteBits(p_Title.size(), 11);
    l_Data.FlushBits();
    l_Data.WriteString(p_Title);

    SendPacket(&l_Data);
}

void WorldSession::HandleTrainerBuySpellOpcode(WorldPacket& p_Packet)
{
    uint64 l_TrainerGUID = 0;

    uint32 l_SpellID   = 0;
    int32  l_TrainerID = 0;

    p_Packet.readPackGUID(l_TrainerGUID);
    p_Packet >> l_TrainerID;
    p_Packet >> l_SpellID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_TrainerGUID, UNIT_NPC_FLAG_TRAINER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTrainerBuySpellOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_TrainerGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!l_Unit->isCanTrainingOf(m_Player, true))
    {
        SendTrainerService(l_TrainerGUID, l_SpellID, 0);
        return;
    }

    /// Check present spell in trainer spell list
    const TrainerSpellData * l_TrainerSpells = l_Unit->GetTrainerSpells();
    if (!l_TrainerSpells)
    {
        SendTrainerService(l_TrainerGUID, l_SpellID, 0);
        return;
    }

    /// Not found, cheat?
    const TrainerSpell * l_TrainerSpell = l_TrainerSpells->Find(l_SpellID);
    if (!l_TrainerSpell)
    {
        SendTrainerService(l_TrainerGUID, l_SpellID, 0);
        return;
    }

    /// Can't be learn, cheat? Or double learn with lags...
    if (m_Player->GetTrainerSpellState(l_TrainerSpell) != TRAINER_SPELL_GREEN)
    {
        SendTrainerService(l_TrainerGUID, l_SpellID, 0);
        return;
    }

    /// Apply reputation discount
    uint32 nSpellCost = uint32(floor(l_TrainerSpell->spellCost * m_Player->GetReputationPriceDiscount(l_Unit)));

    /// Check money requirement
    if (!m_Player->HasEnoughMoney(uint64(nSpellCost)))
    {
        SendTrainerService(l_TrainerGUID, l_SpellID, 1);
        return;
    }

    m_Player->ModifyMoney(-int64(nSpellCost));

    l_Unit->SendPlaySpellVisualKit(179, 0);       // 53 SpellCastDirected
    m_Player->SendPlaySpellVisualKit(362, 1);    // 113 EmoteSalute

    /// Learn explicitly or cast explicitly
    if (l_TrainerSpell->IsCastable())
        m_Player->CastSpell(m_Player, l_TrainerSpell->spell, true);
    else
        m_Player->learnSpell(l_SpellID, false);

    SendTrainerService(l_TrainerGUID, l_SpellID, 2);

    // Archeology
    if (l_SpellID == 78670)
    {
        m_Player->GetArchaeologyMgr().GenerateResearchSites();
        m_Player->GetArchaeologyMgr().GenerateResearchProjects();
    }
}

void WorldSession::SendTrainerService(uint64 p_Guid, uint32 p_SpellID, uint32 p_Result)
{
    if (p_Result == 2)
        return;

    WorldPacket l_Data(SMSG_TRAINER_SERVICE, 2 + 16 + 4 + 4);

    l_Data.appendPackGUID(p_Guid);
    l_Data << uint32(p_SpellID);        // should be same as in packet from client
    l_Data << uint32(p_Result);         // 2 == Success. 1 == "Not enough money for trainer service." 0 == "Trainer service %d unavailable."

    SendPacket(&l_Data);
}

void WorldSession::HandleGossipHelloOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GOSSIP_HELLO");

    uint64 guid;

    recvData.readPackGUID(guid);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_NONE);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGossipHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // set faction visible if needed
    if (FactionTemplateEntry const* factionTemplateEntry = sFactionTemplateStore.LookupEntry(unit->getFaction()))
        m_Player->GetReputationMgr().SetVisible(factionTemplateEntry);

    GetPlayer()->RemoveAurasWithInterruptFlags(AURA_INTERRUPT_FLAG_TALK);
    // remove fake death
    //if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
    //    GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (unit->isArmorer() || unit->isCivilian() || unit->isQuestGiver() || unit->isServiceProvider() || unit->isGuard())
        unit->StopMoving();

    // If spiritguide, no need for gossip menu, just put player into resurrect queue
    if (unit->isSpiritGuide())
    {
        Battleground* bg = m_Player->GetBattleground();
        if (bg)
        {
            bg->AddPlayerToResurrectQueue(unit->GetGUID(), m_Player->GetGUID());
            sBattlegroundMgr->SendAreaSpiritHealerQueryOpcode(m_Player, bg, unit->GetGUID());
            return;
        }
    }

    if (!sScriptMgr->OnGossipHello(m_Player, unit))
    {
//        _player->TalkedToCreature(unit->GetEntry(), unit->GetGUID());
        m_Player->PrepareGossipMenu(unit, unit->GetCreatureTemplate()->GossipMenuId, true);
        m_Player->SendPreparedGossip(unit);
    }
    unit->AI()->sGossipHello(m_Player);
}

void WorldSession::HandleSpiritHealerActivateOpcode(WorldPacket& p_Packet)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_SPIRIT_HEALER_ACTIVATE");

    uint64 l_Healer;

    p_Packet.readPackGUID(l_Healer);

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_Healer, UNIT_NPC_FLAG_SPIRITHEALER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleSpiritHealerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_Healer)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendSpiritResurrect();
}

void WorldSession::SendSpiritResurrect()
{
    m_Player->ResurrectPlayer(0.5f, true);

    m_Player->DurabilityLossAll(0.25f, true);

    // get corpse nearest graveyard
    WorldSafeLocsEntry const* corpseGrave = NULL;
    Corpse* corpse = m_Player->GetCorpse();
    if (corpse)
        corpseGrave = sObjectMgr->GetClosestGraveYard(
            corpse->GetPositionX(), corpse->GetPositionY(), corpse->GetPositionZ(), corpse->GetMapId(), m_Player->GetTeam());

    // now can spawn bones
    m_Player->SpawnCorpseBones();

    // teleport to nearest from corpse graveyard, if different from nearest to player ghost
    if (corpseGrave)
    {
        WorldSafeLocsEntry const* ghostGrave = sObjectMgr->GetClosestGraveYard(
            m_Player->GetPositionX(), m_Player->GetPositionY(), m_Player->GetPositionZ(), m_Player->GetMapId(), m_Player->GetTeam());

        if (corpseGrave != ghostGrave)
            m_Player->TeleportTo(corpseGrave->map_id, corpseGrave->x, corpseGrave->y, corpseGrave->z, m_Player->GetOrientation());
        // or update at original position
        else
            m_Player->UpdateObjectVisibility();
    }
    // or update at original position
    else
        m_Player->UpdateObjectVisibility();
}

void WorldSession::HandleBinderActivateOpcode(WorldPacket& p_RecvData)
{
    uint64 l_NpcGUID;

    p_RecvData.readPackGUID(l_NpcGUID);

    if (!GetPlayer()->IsInWorld() || !GetPlayer()->isAlive())
        return;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_NpcGUID, UNIT_NPC_FLAG_INNKEEPER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBinderActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBindPoint(l_Unit);
}

void WorldSession::SendBindPoint(Creature* npc)
{
    // prevent set homebind to instances in any case
    if (GetPlayer()->GetMap()->Instanceable())
        return;

    uint32 bindspell = 3286;

    // update sql homebind
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PLAYER_HOMEBIND);
    stmt->setUInt16(0, m_Player->GetMapId());
    stmt->setUInt16(1, m_Player->GetAreaId());
    stmt->setFloat (2, m_Player->GetPositionX());
    stmt->setFloat (3, m_Player->GetPositionY());
    stmt->setFloat (4, m_Player->GetPositionZ());
    stmt->setUInt32(5, m_Player->GetGUIDLow());
    CharacterDatabase.Execute(stmt);

    m_Player->m_homebindMapId = m_Player->GetMapId();
    m_Player->m_homebindAreaId = m_Player->GetAreaId();
    m_Player->m_homebindX = m_Player->GetPositionX();
    m_Player->m_homebindY = m_Player->GetPositionY();
    m_Player->m_homebindZ = m_Player->GetPositionZ();

    // send spell for homebinding (3286)
    m_Player->CastSpell(m_Player, bindspell, true);

    SendTrainerService(npc->GetGUID(), bindspell, 2);
    m_Player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleListStabledPetsOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_LIST_STABLED_PETS");
    ObjectGuid npcGUID;

    uint8 bitsOrder[8] = { 0, 7, 2, 4, 5, 3, 1, 6 };
    recvData.ReadBitInOrder(npcGUID, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 0, 2, 3, 1, 7, 5, 6, 4 };
    recvData.ReadBytesSeq(npcGUID, bytesOrder);

    if (!CheckStableMaster(npcGUID))
        return;

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // remove mounts this fix bug where getting pet from stable while mounted deletes pet.
    if (GetPlayer()->IsMounted())
        GetPlayer()->RemoveAurasByType(SPELL_AURA_MOUNTED);

    SendStablePet(npcGUID);
}

void WorldSession::SendStablePet(uint64 guid)
{
    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_SLOTS_DETAIL);

    stmt->setUInt32(0, m_Player->GetGUIDLow());
    stmt->setUInt8(1, PET_SLOT_HUNTER_FIRST);
    stmt->setUInt8(2, PET_SLOT_STABLE_LAST);

    _sendStabledPetCallback.SetParam(guid);
    _sendStabledPetCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::SendStablePetCallback(PreparedQueryResult p_QueryResult, uint64 p_Guid)
{
    if (m_Player == nullptr)
        return;

    uint64 l_StableMaster = p_Guid;
    uint32 l_PetsCount   = p_QueryResult ? p_QueryResult->GetRowCount() : 0;

    WorldPacket l_Data(SMSG_PET_STABLE_LIST, 200);
    l_Data.appendPackGUID(l_StableMaster);              ///< StableMaster
    l_Data << uint32(l_PetsCount);                      ///< Pets count 

    if (p_QueryResult)
    {
        do
        {
            Field* l_Fields = p_QueryResult->Fetch();

            uint32 l_PetSlot         = l_Fields[1].GetUInt8();
            uint32 l_PetNumber       = l_Fields[2].GetUInt32();
            uint32 l_CreatureID      = l_Fields[3].GetUInt32();
            uint32 l_ExperienceLevel = l_Fields[4].GetUInt16();
            uint8  l_PetFlag         = l_Fields[1].GetUInt8() < uint8(PET_SLOT_STABLE_FIRST) ? 1 : 3;

            uint32 l_DisplayID = 0;
            if (CreatureTemplate const* l_CreatureInfo = sObjectMgr->GetCreatureTemplate(l_CreatureID))
                l_DisplayID = l_CreatureInfo->Modelid1 ? l_CreatureInfo->Modelid1 : l_CreatureInfo->Modelid2;

            std::string l_PetName = l_Fields[5].GetString();

            l_Data << uint32(l_PetSlot);           ///< PetSlot
            l_Data << uint32(l_PetNumber);         ///< PetNumber
            l_Data << uint32(l_CreatureID);        ///< CreatureID
            l_Data << uint32(l_DisplayID);         ///< DisplayID
            l_Data << uint32(l_ExperienceLevel);   ///< ExperienceLevel
            l_Data << uint8(l_PetFlag);            ///< PetFlag
            l_Data.WriteBits(l_PetName.size(), 8); ///< PetName Length
            l_Data.WriteString(l_PetName);         ///< PetName
        } 
        while (p_QueryResult->NextRow());
    }


    SendPacket(&l_Data);
}

void WorldSession::SendStableResult(uint8 res)
{
    WorldPacket data(SMSG_STABLE_RESULT, 1);
    data << uint8(res);
    SendPacket(&data);
}

void WorldSession::HandleStableSetPetSlot(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv CMSG_SET_PET_SLOT.");
    ObjectGuid npcGuid;
    uint32 pet_number;
    uint8 new_slot;

    recvData >> pet_number >> new_slot;

    uint8 bitsOrder[8] = { 3, 2, 4, 6, 0, 1, 7, 5 };
    recvData.ReadBitInOrder(npcGuid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 5, 3, 2, 0, 1, 4, 7, 6 };
    recvData.ReadBytesSeq(npcGuid, bytesOrder);

    if (!CheckStableMaster(npcGuid))
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    if (new_slot > MAX_PET_STABLES)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    Pet* pet = m_Player->GetPet();

    //If we move the pet already summoned...
    if (pet && pet->GetCharmInfo() && pet->GetCharmInfo()->GetPetNumber() == pet_number)
        m_Player->RemovePet(pet, PET_SLOT_ACTUAL_PET_SLOT, false, pet->m_Stampeded);

    //If we move to the pet already summoned...
    if (pet && GetPlayer()->m_currentPetSlot == new_slot)
        m_Player->RemovePet(pet, PET_SLOT_ACTUAL_PET_SLOT, false, pet->m_Stampeded);

    PreparedStatement* stmt = CharacterDatabase.GetPreparedStatement(CHAR_SEL_PET_SLOT_BY_ID);

    stmt->setUInt32(0, m_Player->GetGUIDLow());
    stmt->setUInt32(1, pet_number);

    _setPetSlotCallback.SetParam(new_slot);
    _setPetSlotCallback.SetFutureResult(CharacterDatabase.AsyncQuery(stmt));
}

void WorldSession::HandleStableSetPetSlotCallback(PreparedQueryResult result, uint32 petId)
{
    if (!GetPlayer())
        return;

    if (!result)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    Field* fields = result->Fetch();

    uint32 slot         = fields[0].GetUInt8();
    uint32 petEntry     = fields[1].GetUInt32();
    uint32 pet_number   = fields[2].GetUInt32();

    if (!petEntry)
    {
        SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    CreatureTemplate const* creatureInfo = sObjectMgr->GetCreatureTemplate(petEntry);
    if (!creatureInfo || !creatureInfo->isTameable(m_Player->CanTameExoticPets()))
    {
        // if problem in exotic pet
        if (creatureInfo && creatureInfo->isTameable(true))
            SendStableResult(STABLE_ERR_EXOTIC);
        else
            SendStableResult(STABLE_ERR_STABLE);
        return;
    }

    SQLTransaction trans = CharacterDatabase.BeginTransaction();

    auto stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PET_DATA_OWNER);
    {
        stmt->setUInt32(0, petId);
        stmt->setUInt32(1, slot);
        stmt->setUInt32(2, GetPlayer()->GetGUIDLow());

        trans->Append(stmt);
    }
    stmt = CharacterDatabase.GetPreparedStatement(CHAR_UPD_PET_DATA_OWNER_ID);
    {
        stmt->setUInt32(0, slot);
        stmt->setUInt32(1, petId);
        stmt->setUInt32(2, GetPlayer()->GetGUIDLow());
        stmt->setUInt32(3, pet_number);

        trans->Append(stmt);
    }

    CharacterDatabase.CommitTransaction(trans);

    if (petId != 100)
    {
        // We need to remove and add the new pet to there diffrent slots
        // GetPlayer()->setPetSlotUsed((PetSlot)slot, false);
        m_Player->setPetSlotUsed((PetSlot)slot, false);
        m_Player->setPetSlotUsed((PetSlot)petId, true);
        SendStableResult(STABLE_SUCCESS_UNSTABLE);
    }
    else
        SendStableResult(STABLE_ERR_STABLE);
}

void WorldSession::HandleRepairItemOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_REPAIR_ITEM");

    uint64 npcGUID, itemGUID;
    bool guildBank;                                        // new in 2.3.2, bool that means from guild bank money

    recvData.readPackGUID(npcGUID);
    recvData.readPackGUID(itemGUID);
    guildBank = recvData.ReadBit();

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGUID, UNIT_NPC_FLAG_REPAIR);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleRepairItemOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(npcGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // reputation discount
    float discountMod = m_Player->GetReputationPriceDiscount(unit);

    if (itemGUID)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "ITEM: Repair item, itemGUID = %u, npcGUID = %u", GUID_LOPART(itemGUID), GUID_LOPART(npcGUID));

        Item* item = m_Player->GetItemByGuid(itemGUID);
        if (item)
            m_Player->DurabilityRepair(item->GetPos(), true, discountMod, guildBank);
    }
    else
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "ITEM: Repair all items, npcGUID = %u", GUID_LOPART(npcGUID));
        m_Player->DurabilityRepairAll(true, discountMod, guildBank);
    }
}
