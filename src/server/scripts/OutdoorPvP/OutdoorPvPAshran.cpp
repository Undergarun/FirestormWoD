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

#include "OutdoorPvPAshran.h"

OutdoorPvPAshran::OutdoorPvPAshran()
{
    m_TypeId            = OUTDOOR_PVP_ASHRAN;
    m_WorldPvPAreaId    = ASHRAN_WORLD_PVP_AREA_ID;

    m_Guid = MAKE_NEW_GUID(m_WorldPvPAreaId, 0, HIGHGUID_TYPE_BATTLEGROUND);
    m_Guid |= BATTLEFIELD_TYPE_WORLD_PVP;

    for (uint8 l_Team = BG_TEAM_ALLIANCE; l_Team < BG_TEAMS_COUNT; ++l_Team)
    {
        m_PlayersInWar[l_Team].clear();
        m_InvitedPlayers[l_Team].clear();
        m_PlayersWillBeKick[l_Team].clear();
    }
}

bool OutdoorPvPAshran::SetupOutdoorPvP()
{
    RegisterZone(ASHRAN_ZONE_ID);
    return true;
}

void OutdoorPvPAshran::HandlePlayerEnterMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != ASHRAN_MAP_ID)
        return;

    if (!p_Player || p_Player->GetTeamId() >= 2 || p_Player->isInFlight())
        return;

    // If the player does not match minimal level requirements for the battlefield, kick him
    if (p_Player->getLevel() < PLAYER_MIN_LEVEL)
    {
        if (m_PlayersWillBeKick[p_Player->GetTeamId()].count(p_Player->GetGUID()) == 0)
            m_PlayersWillBeKick[p_Player->GetTeamId()][p_Player->GetGUID()] = time(NULL) + 10;
        return;
    }

    // Check if player is not already in war
    if (m_PlayersInWar[p_Player->GetTeamId()].count(p_Player->GetGUID()) || m_InvitedPlayers[p_Player->GetTeamId()].count(p_Player->GetGUID()))
        return;

    m_InvitedPlayers[p_Player->GetTeamId()][p_Player->GetGUID()] = time(NULL) + ASHRAN_TIME_FOR_INVITE;

    WorldPacket l_Data(SMSG_BFMGR_ENTRY_INVITE);
    l_Data << uint64(m_Guid);           ///< QueueID
    l_Data << uint32(ASHRAN_ZONE_ID);   ///< Zone Id
    l_Data << uint32(time(NULL) + 20);  ///< Invite lasts until

    ///< Sending the packet to player
    p_Player->SendDirectMessage(&l_Data);

    p_Player->CastSpell(p_Player, SPELL_LOOTABLE, true);
}

void OutdoorPvPAshran::HandlePlayerLeaveMap(Player* p_Player, uint32 p_MapID)
{
    if (p_MapID != ASHRAN_MAP_ID)
        return;

    if (p_Player->GetTeamId() < 2)
    {
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersInWar[p_Player->GetTeamId()].erase(p_Player->GetGUID());
        m_PlayersWillBeKick[p_Player->GetTeamId()].erase(p_Player->GetGUID());
    }

    SendRemoveWorldStates(p_Player);
    p_Player->GetSession()->SendBfLeaveMessage(m_Guid);

    p_Player->RemoveAura(SPELL_LOOTABLE);
}

void OutdoorPvPAshran::HandlePlayerEnterArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player->GetMapId() != ASHRAN_NEUTRAL_MAP_ID && p_Player->GetMapId() != ASHRAN_MAP_ID)
        return;

    if (p_AreaID == ASHRAN_PRE_AREA_HORDE || p_AreaID == ASHRAN_PRE_AREA_ALLIANCE)
        p_Player->SwitchToPhasedMap(ASHRAN_NEUTRAL_MAP_ID);

    if ((p_AreaID == ASHRAN_HORDE_BASE && p_Player->GetTeamId() == TEAM_HORDE) ||
        (p_AreaID == ASHRAN_ALLIANCE_BASE && p_Player->GetTeamId() == TEAM_ALLIANCE))
        p_Player->CastSpell(p_Player, SPELL_HOLD_YOUR_GROUND, true);
}

void OutdoorPvPAshran::HandlePlayerLeaveArea(Player* p_Player, uint32 p_AreaID)
{
    if (p_Player->GetMapId() != ASHRAN_NEUTRAL_MAP_ID && p_Player->GetMapId() != ASHRAN_MAP_ID)
        return;

    if (p_AreaID == ASHRAN_PRE_AREA_HORDE || p_AreaID == ASHRAN_PRE_AREA_ALLIANCE)
        p_Player->SwitchToPhasedMap(ASHRAN_MAP_ID);

    if (p_AreaID == ASHRAN_HORDE_BASE || p_AreaID == ASHRAN_ALLIANCE_BASE)
        p_Player->RemoveAura(SPELL_HOLD_YOUR_GROUND);
}

void OutdoorPvPAshran::HandlePlayerKilled(Player* p_Player)
{
    // Drop half of artifact fragments at player death
    // Even if he's killed by a creature
    if (uint32 l_ArtifactCount = p_Player->GetCurrency(CURRENCY_TYPE_ARTIFACT_FRAGEMENT, false))
        p_Player->ModifyCurrency(CURRENCY_TYPE_ARTIFACT_FRAGEMENT, -int32(l_ArtifactCount / 2));
}

bool OutdoorPvPAshran::Update(uint32 p_Diff)
{
    PlayerTimerMap l_TempList[BG_TEAMS_COUNT];

    for (uint8 l_Team = 0; l_Team < 2; ++l_Team)
    {
        l_TempList[l_Team] = m_InvitedPlayers[l_Team];

        for (PlayerTimerMap::iterator l_Iter = l_TempList[l_Team].begin(); l_Iter != l_TempList[l_Team].end(); ++l_Iter)
        {
            // Remove player after 20s if not accepted
            if ((*l_Iter).second <= time(NULL))
            {
                if (Player* l_Player = sObjectAccessor->FindPlayer((*l_Iter).first))
                {
                    if (l_Player->GetTeamId() == TEAM_HORDE)
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
                    else
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
                }
            }
        }

        l_TempList[l_Team] = m_PlayersWillBeKick[l_Team];

        for (PlayerTimerMap::iterator l_Iter = l_TempList[l_Team].begin(); l_Iter != l_TempList[l_Team].end(); ++l_Iter)
        {
            if ((*l_Iter).second <= time(NULL))
            {
                if (Player* l_Player = sObjectAccessor->FindPlayer((*l_Iter).first))
                {
                    if (l_Player->GetTeamId() == TEAM_HORDE)
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
                    else
                        l_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
                }
            }
        }
    }

    return true;
}

void OutdoorPvPAshran::FillInitialWorldStates(ByteBuffer& p_Data)
{
    /*
    *p_Data << uint32(521) << uint32(0);
    *p_Data << uint32(522) << uint32(0);
    *p_Data << uint32(523) << uint32(0);
    *p_Data << uint32(524) << uint32(0);
    *p_Data << uint32(WORLD_STATE_ORE_COLLECTED_ALLIANCE) << uint32(0);
    *p_Data << uint32(WORLD_STATE_ORE_COLLECTED_HORDE) << uint32(0);
    *p_Data << uint32(1723) << uint32(0);
    *p_Data << uint32(1724) << uint32(0);
    *p_Data << uint32(1941) << uint32(0);
    *p_Data << uint32(1942) << uint32(0);
    *p_Data << uint32(1943) << uint32(0);
    *p_Data << uint32(2259) << uint32(0);
    *p_Data << uint32(2260) << uint32(0);
    *p_Data << uint32(2261) << uint32(0);
    *p_Data << uint32(2262) << uint32(0);
    *p_Data << uint32(2263) << uint32(0);
    *p_Data << uint32(2264) << uint32(0);
    *p_Data << uint32(2265) << uint32(142);
    *p_Data << uint32(2851) << uint32(0);
    *p_Data << uint32(3085) << uint32(379);
    *p_Data << uint32(3191) << uint32(16);
    *p_Data << uint32(3327) << uint32(0);
    *p_Data << uint32(3426) << uint32(3);
    *p_Data << uint32(3600) << uint32(0);
    *p_Data << uint32(3601) << uint32(0);
    *p_Data << uint32(3610) << uint32(1);
    *p_Data << uint32(3695) << uint32(0);
    *p_Data << uint32(3710) << uint32(0);
    *p_Data << uint32(3781) << uint32(0);
    *p_Data << uint32(3801) << uint32(1);
    *p_Data << uint32(3826) << uint32(4);
    *p_Data << uint32(3901) << uint32(3);
    *p_Data << uint32(4020) << uint32(1094);
    *p_Data << uint32(4021) << uint32(7);
    *p_Data << uint32(4022) << uint32(3);
    *p_Data << uint32(4023) << uint32(4);
    *p_Data << uint32(4024) << uint32(825);
    *p_Data << uint32(4025) << uint32(269);
    *p_Data << uint32(4062) << uint32(0);
    *p_Data << uint32(4131) << uint32(60);
    *p_Data << uint32(4273) << uint32(0);
    *p_Data << uint32(4354) << uint32(time(NULL));
    *p_Data << uint32(4375) << uint32(0);
    *p_Data << uint32(4417) << uint32(1);
    *p_Data << uint32(4418) << uint32(50);
    *p_Data << uint32(4419) << uint32(0);
    *p_Data << uint32(4485) << uint32(0);
    *p_Data << uint32(4486) << uint32(0);
    *p_Data << uint32(4862) << uint32(1000);
    *p_Data << uint32(4863) << uint32(300);
    *p_Data << uint32(4864) << uint32(100);
    *p_Data << uint32(5037) << uint32(6);
    *p_Data << uint32(5071) << uint32(6);
    *p_Data << uint32(5115) << uint32(0);
    *p_Data << uint32(5192) << uint32(0);
    *p_Data << uint32(5193) << uint32(0);
    *p_Data << uint32(5194) << uint32(0);
    *p_Data << uint32(5195) << uint32(0);
    *p_Data << uint32(5196) << uint32(0);
    *p_Data << uint32(5332) << uint32(time(NULL));
    *p_Data << uint32(5333) << uint32(0);
    *p_Data << uint32(5334) << uint32(1);
    *p_Data << uint32(5344) << uint32(0);
    *p_Data << uint32(5360) << uint32(0);
    *p_Data << uint32(5361) << uint32(0);
    *p_Data << uint32(5508) << uint32(1);
    *p_Data << uint32(5677) << uint32(0);
    *p_Data << uint32(5678) << uint32(0);
    *p_Data << uint32(5679) << uint32(0);
    *p_Data << uint32(5684) << uint32(0);
    *p_Data << uint32(6078) << uint32(0);
    *p_Data << uint32(6095) << uint32(0);
    *p_Data << uint32(6164) << uint32(35);
    *p_Data << uint32(6174) << uint32(0);
    *p_Data << uint32(6267) << uint32(25);
    *p_Data << uint32(6306) << uint32(0);
    *p_Data << uint32(6436) << uint32(0);
    *p_Data << uint32(6895) << uint32(10);
    *p_Data << uint32(6897) << uint32(10);
    *p_Data << uint32(6898) << uint32(10);
    *p_Data << uint32(7022) << uint32(0);
    *p_Data << uint32(7242) << uint32(82);
    *p_Data << uint32(7243) << uint32(1);
    *p_Data << uint32(7244) << uint32(82);
    *p_Data << uint32(7245) << uint32(1);
    *p_Data << uint32(7511) << uint32(0);
    *p_Data << uint32(7617) << uint32(5);
    *p_Data << uint32(7618) << uint32(5);
    *p_Data << uint32(7671) << uint32(0);
    *p_Data << uint32(7738) << uint32(0);
    *p_Data << uint32(7752) << uint32(0);
    *p_Data << uint32(7774) << uint32(0);
    *p_Data << uint32(7796) << uint32(0);
    *p_Data << uint32(7797) << uint32(0);
    *p_Data << uint32(7876) << uint32(0);
    *p_Data << uint32(8012) << uint32(1);
    *p_Data << uint32(8295) << uint32(15);
    *p_Data << uint32(8306) << uint32(20);
    *p_Data << uint32(8307) << uint32(20);
    *p_Data << uint32(8391) << uint32(0);
    *p_Data << uint32(8524) << uint32(0);
    *p_Data << uint32(8525) << uint32(0);
    *p_Data << uint32(8526) << uint32(0);
    *p_Data << uint32(8527) << uint32(0);
    *p_Data << uint32(8528) << uint32(0);
    *p_Data << uint32(8529) << uint32(0);
    *p_Data << uint32(8712) << uint32(0);
    *p_Data << uint32(8722) << uint32(0);
    *p_Data << uint32(8859) << uint32(0);
    *p_Data << uint32(8860) << uint32(0);
    *p_Data << uint32(8861) << uint32(0);
    *p_Data << uint32(8862) << uint32(0);
    *p_Data << uint32(8863) << uint32(1);
    *p_Data << uint32(8890) << uint32(0);
    *p_Data << uint32(8892) << uint32(0);
    *p_Data << uint32(8911) << uint32(10);
    *p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_ALLIANCE) << uint32(65);
    *p_Data << uint32(WORLD_STATE_ENNEMIES_SLAIN_HORDE) << uint32(5);
    *p_Data << uint32(8935) << uint32(1);
    *p_Data << uint32(8938) << uint32(0);
    *p_Data << uint32(WORLD_STATE_NEXT_BATTLE_TIMESTAMP) << uint32(time(NULL));
    *p_Data << uint32(8946) << uint32(0);
    *p_Data << uint32(8949) << uint32(1);
    *p_Data << uint32(8950) << uint32(0);
    *p_Data << uint32(WORLD_STATE_ACTIVE_STAGE) << uint32(0);
    */
}

void OutdoorPvPAshran::SendRemoveWorldStates(Player* p_Player)
{
}

void OutdoorPvPAshran::HandleBFMGREntryInviteResponse(bool p_Accepted, Player* p_Player)
{
    if (p_Accepted)
    {
        m_PlayersInWar[p_Player->GetTeamId()].insert(p_Player->GetGUID());
        m_InvitedPlayers[p_Player->GetTeamId()].erase(p_Player->GetGUID());
    }
    else
    {
        if (p_Player->GetTeamId() == TEAM_HORDE)
            p_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_HordeTeleportPos.m_positionX, g_HordeTeleportPos.m_positionY, g_HordeTeleportPos.m_positionZ, g_HordeTeleportPos.m_orientation);
        else
            p_Player->TeleportTo(ASHRAN_NEUTRAL_MAP_ID, g_AllianceTeleportPos.m_positionX, g_AllianceTeleportPos.m_positionY, g_AllianceTeleportPos.m_positionZ, g_AllianceTeleportPos.m_orientation);
    }
}

class OutdoorPvP_Ashran : public OutdoorPvPScript
{
    public:

        OutdoorPvP_Ashran() : OutdoorPvPScript("outdoorpvp_ashran") { }

        OutdoorPvP* GetOutdoorPvP() const
        {
            return new OutdoorPvPAshran();
        }
};

void AddSC_OutdoorPvPAshran()
{
    new OutdoorPvP_Ashran();
}