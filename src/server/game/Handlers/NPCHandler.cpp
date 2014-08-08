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

void WorldSession::HandleTrainerListOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;

    uint8 bitsOrder[8] = { 5, 7, 6, 3, 1, 4, 0, 2 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 4, 7, 5, 2, 1, 3, 0, 6 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    SendTrainerList(guid);
}

void WorldSession::SendTrainerList(uint64 guid)
{
    std::string str = GetTrinityString(LANG_NPC_TAINER_HELLO);
    SendTrainerList(guid, str);
}

void WorldSession::SendTrainerList(uint64 guid, const std::string& strTitle)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList");

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    // trainer list loaded at check;
    if (!unit->isCanTrainingOf(m_Player, true))
        return;

    CreatureTemplate const* ci = unit->GetCreatureTemplate();

    if (!ci)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - (GUID: %u) NO CREATUREINFO!", GUID_LOPART(guid));
        return;
    }

    TrainerSpellData const* trainer_spells = unit->GetTrainerSpells();
    if (!trainer_spells)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: SendTrainerList - Training spells not found for creature (GUID: %u Entry: %u)",
            GUID_LOPART(guid), unit->GetEntry());
        return;
    }

    ByteBuffer dataBuffer;
    WorldPacket data(SMSG_TRAINER_LIST);
    ObjectGuid npcGuid = guid;

    data.WriteBit(npcGuid[0]);
    data.WriteBits(strTitle.size(), 11);
    data.WriteBit(npcGuid[5]);
    data.WriteBit(npcGuid[6]);
    data.WriteBit(npcGuid[1]);
    data.WriteBit(npcGuid[2]);
    data.WriteBit(npcGuid[7]);
    data.WriteBit(npcGuid[4]);
    data.WriteBit(npcGuid[3]);

    // reputation discount
    float fDiscountMod = m_Player->GetReputationPriceDiscount(unit);
    bool can_learn_primary_prof = GetPlayer()->GetFreePrimaryProfessionPoints() > 0;

    uint32 count = 0;
    for (TrainerSpellMap::const_iterator itr = trainer_spells->spellList.begin(); itr != trainer_spells->spellList.end(); ++itr)
    {
        TrainerSpell const* tSpell = &itr->second;

        bool valid = true;
        bool primary_prof_first_rank = false;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (!tSpell->learnedSpell[i])
                continue;
            if (!m_Player->IsSpellFitByClassAndRace(tSpell->learnedSpell[i]))
            {
                valid = false;
                break;
            }
            SpellInfo const* learnedSpellInfo = sSpellMgr->GetSpellInfo(tSpell->learnedSpell[i]);
            if (learnedSpellInfo && learnedSpellInfo->IsPrimaryProfessionFirstRank())
                primary_prof_first_rank = true;
        }
        if (!valid)
            continue;

        TrainerSpellState state = m_Player->GetTrainerSpellState(tSpell);

        dataBuffer << uint32(tSpell->reqSkillValue);
        dataBuffer << uint32(floor(tSpell->spellCost * fDiscountMod));
        dataBuffer << uint8(tSpell->reqLevel);

        //prev + req or req + 0
        uint8 maxReq = 0;
        for (uint8 i = 0; i < MAX_SPELL_EFFECTS; ++i)
        {
            if (!tSpell->learnedSpell[i])
                continue;
            if (uint32 prevSpellId = sSpellMgr->GetPrevSpellInChain(tSpell->learnedSpell[i]))
            {
                dataBuffer << uint32(prevSpellId);
                ++maxReq;
            }
            if (maxReq == 1)
                break;
            SpellsRequiringSpellMapBounds spellsRequired = sSpellMgr->GetSpellsRequiredForSpellBounds(tSpell->learnedSpell[i]);
            for (SpellsRequiringSpellMap::const_iterator itr2 = spellsRequired.first; itr2 != spellsRequired.second && maxReq < 2; ++itr2)
            {
                dataBuffer << uint32(itr2->second);
                ++maxReq;
            }
            if (maxReq == 1)
                break;
        }
        while (maxReq < 1)
        {
            dataBuffer << uint32(0);
            ++maxReq;
        }

        dataBuffer << uint32(0); // Profession Dialog or Profession Button
        dataBuffer << uint32(0); // Profession Dialog or Profession Button
        dataBuffer << uint32(tSpell->spell);
        dataBuffer << uint8(state == TRAINER_SPELL_GREEN_DISABLED ? TRAINER_SPELL_GREEN : state);
        dataBuffer << uint32(tSpell->reqSkill);

        ++count;
    }

    data.WriteBits(count, 19);
    data.FlushBits();

    if (dataBuffer.size() > 0)
        data.append(dataBuffer);

    data.WriteByteSeq(npcGuid[5]);
    data.WriteByteSeq(npcGuid[7]);
    data.WriteByteSeq(npcGuid[6]);

    if (strTitle.size() > 0)
        data.append(strTitle.c_str(), strTitle.size());

    data << uint32(1); // different value for each trainer, also found in CMSG_TRAINER_BUY_SPELL
    data.WriteByteSeq(npcGuid[2]);
    data.WriteByteSeq(npcGuid[3]);
    data.WriteByteSeq(npcGuid[1]);
    data.WriteByteSeq(npcGuid[0]);
    data.WriteByteSeq(npcGuid[4]);
    data << uint32(unit->GetCreatureTemplate()->trainer_type);

    SendPacket(&data);
}

void WorldSession::HandleTrainerBuySpellOpcode(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint32 spellId;
    int32 trainerId;

    recvData >> trainerId >> spellId;

    uint8 bitsOrder[8] = { 0, 5, 4, 6, 1, 2, 7, 3 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 3, 7, 2, 0, 5, 6, 1, 4 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_TRAINER_BUY_SPELL NpcGUID=%u, learn spell id is: %u", uint32(GUID_LOPART(guid)), spellId);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_TRAINER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleTrainerBuySpellOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!unit->isCanTrainingOf(m_Player, true))
    {
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // check present spell in trainer spell list
    TrainerSpellData const* trainer_spells = unit->GetTrainerSpells();
    if (!trainer_spells)
    {
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // not found, cheat?
    TrainerSpell const* trainer_spell = trainer_spells->Find(spellId);
    if (!trainer_spell)
    {
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // can't be learn, cheat? Or double learn with lags...
    if (m_Player->GetTrainerSpellState(trainer_spell) != TRAINER_SPELL_GREEN)
    {
        SendTrainerService(guid, spellId, 0);
        return;
    }

    // apply reputation discount
    uint32 nSpellCost = uint32(floor(trainer_spell->spellCost * m_Player->GetReputationPriceDiscount(unit)));

    // check money requirement
    if (!m_Player->HasEnoughMoney(uint64(nSpellCost)))
    {
        SendTrainerService(guid, spellId, 1);
        return;
    }

    m_Player->ModifyMoney(-int64(nSpellCost));

    unit->SendPlaySpellVisualKit(179, 0);       // 53 SpellCastDirected
    m_Player->SendPlaySpellVisualKit(362, 1);    // 113 EmoteSalute

    // learn explicitly or cast explicitly
    if (trainer_spell->IsCastable())
        m_Player->CastSpell(m_Player, trainer_spell->spell, true);
    else
        m_Player->learnSpell(spellId, false);

    SendTrainerService(guid, spellId, 2);

    // Archeology
    if (spellId == 78670)
    {
        m_Player->GetArchaeologyMgr().GenerateResearchSites();
        m_Player->GetArchaeologyMgr().GenerateResearchProjects();
    }
}

void WorldSession::SendTrainerService(uint64 guid, uint32 spellId, uint32 result)
{
    WorldPacket data(SMSG_TRAINER_SERVICE, 16);
    ObjectGuid npcGuid = guid;

    data << uint32(result);         // 2 == Success. 1 == "Not enough money for trainer service." 0 == "Trainer service %d unavailable."
    data << uint32(spellId);        // should be same as in packet from client

    uint8 bitsOrder[8] = { 0, 3, 6, 1, 2, 5, 7, 4 };
    data.WriteBitInOrder(npcGuid, bitsOrder);

    uint8 bytesOrder[8] = { 6, 2, 3, 5, 7, 4, 1, 0 };
    data.WriteBytesSeq(npcGuid, bytesOrder);

    SendPacket(&data);
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

void WorldSession::HandleSpiritHealerActivateOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: CMSG_SPIRIT_HEALER_ACTIVATE");

    ObjectGuid guid;

    uint8 bitsOrder[8] = { 7, 2, 1, 5, 6, 0, 3, 4 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 1, 7, 6, 0, 5, 2, 4, 3 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(guid, UNIT_NPC_FLAG_SPIRITHEALER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleSpiritHealerActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(guid)));
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

void WorldSession::HandleBinderActivateOpcode(WorldPacket& recvData)
{
    ObjectGuid npcGUID;

    uint8 bitsOrder[8] = { 6, 4, 2, 0, 3, 7, 5, 1 };
    recvData.ReadBitInOrder(npcGUID, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 2, 6, 0, 5, 3, 1, 7, 4 };
    recvData.ReadBytesSeq(npcGUID, bytesOrder);

    if (!GetPlayer()->IsInWorld() || !GetPlayer()->isAlive())
        return;

    Creature* unit = GetPlayer()->GetNPCIfCanInteractWith(npcGUID, UNIT_NPC_FLAG_INNKEEPER);
    if (!unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleBinderActivateOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(npcGUID)));
        return;
    }

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    SendBindPoint(unit);
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

void WorldSession::SendStablePetCallback(PreparedQueryResult result, uint64 guid)
{
    if (!GetPlayer())
        return;

    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Recv SMSG_PET_STABLE_LIST Send.");

    WorldPacket data(SMSG_PET_STABLE_LIST, 200);           // guessed size
    ObjectGuid npcGuid = guid;
    ByteBuffer dataBuffer;

    data.WriteBit(npcGuid[6]);
    data.WriteBit(npcGuid[1]);
    data.WriteBit(npcGuid[5]);
    data.WriteBit(npcGuid[4]);
    data.WriteBit(npcGuid[2]);
    data.WriteBits(result ? result->GetRowCount() : 0, 19);
    data.WriteBit(npcGuid[7]);

    if (result)
    {
        do
        {
            Field* fields = result->Fetch();

            std::string name = fields[5].GetString();

            data.WriteBits(name.size(), 8);

            dataBuffer << uint32(fields[3].GetUInt32());          // creature entry
            dataBuffer << uint32(fields[2].GetUInt32());          // petnumber
            dataBuffer << uint32(fields[4].GetUInt16());          // level
            dataBuffer << uint8(fields[1].GetUInt8() < uint8(PET_SLOT_STABLE_FIRST) ? 1 : 3);       // 1 = current, 2/3 = in stable (any from 4, 5, ... create problems with proper show)

            uint32 modelId = 0;

            if (CreatureTemplate const* cInfo = sObjectMgr->GetCreatureTemplate(fields[3].GetUInt32()))
                modelId = cInfo->Modelid1 ? cInfo->Modelid1 : cInfo->Modelid2;

            dataBuffer << uint32(fields[1].GetUInt8());           // slot
            dataBuffer << uint32(modelId);                        // creature modelid

            if (name.size())
                dataBuffer.append(name.c_str(), name.size());
        }
        while (result->NextRow());
    }

    data.WriteBit(npcGuid[3]);
    data.WriteBit(npcGuid[0]);
    data.FlushBits();

    if (dataBuffer.size())
        data.append(dataBuffer);

    data.WriteByteSeq(npcGuid[2]);
    data.WriteByteSeq(npcGuid[0]);
    data.WriteByteSeq(npcGuid[6]);
    data.WriteByteSeq(npcGuid[1]);
    data.WriteByteSeq(npcGuid[7]);
    data.WriteByteSeq(npcGuid[5]);
    data.WriteByteSeq(npcGuid[3]);
    data.WriteByteSeq(npcGuid[4]);

    SendPacket(&data);
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
