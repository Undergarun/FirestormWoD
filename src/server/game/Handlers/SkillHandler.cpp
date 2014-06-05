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
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "Log.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "ObjectAccessor.h"
#include "UpdateMask.h"

void WorldSession::HandleSetSpecialization(WorldPacket& recvData)
{
    uint32 tab = recvData.read<uint32>();
    uint8 classId = m_Player->getClass();

    // Avoid cheat - hack
    if (m_Player->GetSpecializationId(m_Player->GetActiveSpec()))
        return;

    uint32 specializationId = 0;
    uint32 specializationSpell = 0;

    for (uint32 i = 0; i < sChrSpecializationsStore.GetNumRows(); i++)
    {
        ChrSpecializationsEntry const* specialization = sChrSpecializationsStore.LookupEntry(i);
        if (!specialization)
            continue;

        if (specialization->classId == classId && specialization->tabId == tab)
        {
            specializationId = specialization->entry;
            specializationSpell = specialization->specializationSpell;
            break;
        }
    }

    if (specializationId)
    {
        m_Player->SetSpecializationId(m_Player->GetActiveSpec(), specializationId);
        m_Player->SendTalentsInfoData(false);
        if (specializationSpell)
            m_Player->learnSpell(specializationSpell, false);
        m_Player->InitSpellForLevel();
        m_Player->UpdateMasteryPercentage();
    }
}

void WorldSession::HandleLearnTalents(WorldPacket& recvData)
{
    uint32 count = recvData.ReadBits(23);
    recvData.FlushBits();

    if (count > MAX_TALENT_SPELLS)
        return;

    if (count > m_Player->GetFreeTalentPoints())
        return;

    for (uint32 i = 0; i < count; i++)
    {
        uint16 talentId = recvData.read<uint16>();
        m_Player->LearnTalent(talentId);
    }

    m_Player->SendTalentsInfoData(false);
}

void WorldSession::HandleTalentWipeConfirmOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_CONFIRM_RESPEC_WIPE");

    uint8 specializationReset = recvData.read<uint8>();

    recvData.rfinish();

    // remove fake death
    if (GetPlayer()->HasUnitState(UNIT_STATE_DIED))
        GetPlayer()->RemoveAurasByType(SPELL_AURA_FEIGN_DEATH);

    if (!specializationReset)
    {
        if (!m_Player->ResetTalents())
        {
            WorldPacket data(SMSG_RESPEC_WIPE_CONFIRM, 8+4);    //you have not any talent
            data << uint8(0); // 0 guid bit
            data << uint32(0);
            data << uint8(0);
            SendPacket(&data);
            return;
        }
    }
    else
        m_Player->ResetSpec();

    m_Player->SendTalentsInfoData(false);

    if (Unit* unit = m_Player->GetSelectedUnit())
        unit->CastSpell(m_Player, 14867, true);                  //spell: "Untalent Visual Effect"
}

void WorldSession::HandleUnlearnSkillOpcode(WorldPacket& recvData)
{
    uint32 skillId;
    recvData >> skillId;

    if (!IsProfessionSkill(skillId))
        return;

    GetPlayer()->SetSkill(skillId, 0, 0, 0);
}

void WorldSession::HandleArcheologyRequestHistory(WorldPacket& recvPacket)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_REQUEST_RESEARCH_HISTORY");

    WorldPacket data(SMSG_RESEARCH_SETUP_HISTORY);

    CompletedProjectMap& projects = GetPlayer()->GetArchaeologyMgr().GetCompletedProjects();
    uint32 count = projects.size();

    data.WriteBits(count, 20);

    if (count > 0)
    {
        for (CompletedProjectMap::iterator itr = projects.begin(); itr != projects.end(); itr++)
        {
            if (ResearchProjectEntry const* project = sResearchProjectStore.LookupEntry((*itr).first))
            {
                data << uint32((*itr).first);
                data << uint32((*itr).second.count);
                data << uint32((*itr).second.first_date);
            }
            else
                data << uint32(0) << uint32(0) << uint32(0);
        }
    }

    SendPacket(&data);
}
