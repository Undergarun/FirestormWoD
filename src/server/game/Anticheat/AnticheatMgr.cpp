////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AnticheatMgr.h"
#include "AnticheatScripts.h"
#include "MapManager.h"

#define CLIMB_ANGLE 1.9f

AnticheatMgr::AnticheatMgr()
{
}

AnticheatMgr::~AnticheatMgr()
{
    m_Players.clear();
}

void AnticheatMgr::JumpHackDetection(Player* player, MovementInfo const& /*movementInfo*/, uint32 opcode)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & JUMP_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();

    if (m_Players[key].GetLastOpcode() == CMSG_MOVE_JUMP && opcode == CMSG_MOVE_JUMP)
    {
        BuildReport(player,JUMP_HACK_REPORT);
        //sLog->outError("AnticheatMgr:: Jump-Hack detected player GUID (low) %u",player->GetGUIDLow());
    }
}

void AnticheatMgr::WalkOnWaterHackDetection(Player* player, MovementInfo const& /*movementInfo*/)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & WALK_WATER_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_WATERWALKING))
        return;

    // if we are a ghost we can walk on water
    if (!player->isAlive())
        return;

    if (player->HasAuraType(SPELL_AURA_FEATHER_FALL) ||
        player->HasAuraType(SPELL_AURA_SAFE_FALL) ||
        player->HasAuraType(SPELL_AURA_WATER_WALK))
        return;

    //sLog->outError("AnticheatMgr:: Walk on Water - Hack detected player GUID (low) %u",player->GetGUIDLow());
    BuildReport(player,WALK_WATER_HACK_REPORT);

}

void AnticheatMgr::FlyHackDetection(Player* player, MovementInfo const& /*movementInfo*/)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & FLY_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();
    if (!m_Players[key].GetLastMovementInfo().HasMovementFlag(MOVEMENTFLAG_FLYING))
        return;

    if (player->HasAuraType(SPELL_AURA_FLY) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_MOUNTED_FLIGHT_SPEED) ||
        player->HasAuraType(SPELL_AURA_MOD_INCREASE_FLIGHT_SPEED))
        return;

    //sLog->outError("AnticheatMgr:: Fly-Hack detected player GUID (low) %u",player->GetGUIDLow());
    BuildReport(player,FLY_HACK_REPORT);
}

void AnticheatMgr::TeleportPlaneHackDetection(Player* player, MovementInfo const& movementInfo)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & TELEPORT_PLANE_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();

    if (m_Players[key].GetLastMovementInfo().pos.GetPositionZ() != 0 ||
        movementInfo.pos.GetPositionZ() != 0)
        return;

    if (movementInfo.HasMovementFlag(MOVEMENTFLAG_FALLING))
        return;

    //DEAD_FALLING was deprecated
    //if (player->getDeathState() == DEAD_FALLING)
    //    return;
    float x, y, z;
    player->GetPosition(x, y, z);
    float ground_Z = player->GetMap()->GetHeight(x, y, z);
    float z_diff = fabs(ground_Z - z);

    // we are not really walking there
    if (z_diff > 1.0f)
    {
        //sLog->outError("AnticheatMgr:: Teleport To Plane - Hack detected player GUID (low) %u",player->GetGUIDLow());
        BuildReport(player,TELEPORT_PLANE_HACK_REPORT);
    }
}


void AnticheatMgr::StartHackDetection(Player* p_Player, MovementInfo const& p_MoveInfos, uint32 p_Opcode)
{
    if (!sWorld->getBoolConfig(CONFIG_ANTICHEAT_ENABLE))
        return;

    if (p_Player->GetSession()->GetSecurity() > AccountTypes::SEC_PLAYER)
        return;

    uint32 key = p_Player->GetGUIDLow();

    if (p_Player->isInFlight() || p_Player->GetTransport() || p_Player->GetVehicle())
    {

        m_Players[key].SetLastMovementInfo(p_MoveInfos);

        m_Players[key].SetLastOpcode(p_Opcode);
        return;
    }

    SpeedHackDetection(p_Player,p_MoveInfos);
    FlyHackDetection(p_Player,p_MoveInfos);
    WalkOnWaterHackDetection(p_Player,p_MoveInfos);
    JumpHackDetection(p_Player,p_MoveInfos,p_Opcode);
    TeleportPlaneHackDetection(p_Player, p_MoveInfos);
    ClimbHackDetection(p_Player,p_MoveInfos,p_Opcode);

    m_Players[key].SetLastMovementInfo(p_MoveInfos);
    m_Players[key].SetLastOpcode(p_Opcode);
}

// basic detection
void AnticheatMgr::ClimbHackDetection(Player *player, MovementInfo const& movementInfo, uint32 opcode)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & CLIMB_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();

    if (opcode != CMSG_MOVE_HEARTBEAT ||
        m_Players[key].GetLastOpcode() != CMSG_MOVE_HEARTBEAT)
        return;

    // in this case we don't care if they are "legal" flags, they are handled in another parts of the Anticheat Manager.
    if (player->IsInWater() ||
        player->IsFlying() ||
        player->IsFalling())
        return;

    Position playerPos;
    player->GetPosition(&playerPos);

    float deltaZ = fabs(playerPos.GetPositionZ() - movementInfo.pos.GetPositionZ());
    float deltaXY = movementInfo.pos.GetExactDist2d(&playerPos);

    float angle = player->NormalizeOrientation(tan(deltaZ/deltaXY));

    if (angle > CLIMB_ANGLE)
    {
        //sLog->outError("AnticheatMgr:: Climb-Hack detected player GUID (low) %u", player->GetGUIDLow());
        BuildReport(player,CLIMB_HACK_REPORT);
    }
}

void AnticheatMgr::SpeedHackDetection(Player* player, MovementInfo const& movementInfo)
{
    if ((sWorld->getIntConfig(CONFIG_ANTICHEAT_DETECTIONS_ENABLED) & SPEED_HACK_DETECTION) == 0)
        return;

    uint32 key = player->GetGUIDLow();

    // We also must check the map because the movementFlag can be modified by the client.
    // If we just check the flag, they could always add that flag and always skip the speed hacking detection.
    // 369 == DEEPRUN TRAM
    if (player->GetMapId() == 369)
        return;

    uint32 distance2D = (uint32)movementInfo.pos.GetExactDist2d(&m_Players[key].GetLastMovementInfo().pos);
    uint8 moveType = 0;

    // we need to know HOW is the player moving
    // TO-DO: Should we check the incoming movement flags?
    if (player->HasUnitMovementFlag(MOVEMENTFLAG_SWIMMING))
        moveType = MOVE_SWIM;
    else if (player->IsFlying())
        moveType = MOVE_FLIGHT;
    else if (player->HasUnitMovementFlag(MOVEMENTFLAG_WALKING))
        moveType = MOVE_WALK;
    else
        moveType = MOVE_RUN;

    // how many yards the player can do in one sec.
    uint32 speedRate = (uint32)(player->GetSpeed(UnitMoveType(moveType)) + movementInfo.j_xyspeed);

    // how long the player took to move to here.
    uint32 timeDiff = getMSTimeDiff(m_Players[key].GetLastMovementInfo().time,movementInfo.time);

    if (!timeDiff)
        timeDiff = 1;

    // this is the distance doable by the player in 1 sec, using the time done to move to this point.
    uint32 clientSpeedRate = distance2D * 1000 / timeDiff;

    // we did the (uint32) cast to accept a margin of tolerance
    if (clientSpeedRate > speedRate)
    {
        BuildReport(player, SPEED_HACK_REPORT);
        //sLog->outError("AnticheatMgr:: Speed-Hack detected player GUID (low) %u",player->GetGUIDLow());
    }
}

#ifndef __clang_analyzer__
void AnticheatMgr::StartScripts()
{
    new AnticheatScripts();
}
#endif

void AnticheatMgr::HandlePlayerLogin(Player* /*player*/)
{
    /*
    // we must delete this to prevent errors in case of crash
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u",player->GetGUIDLow());
    // we initialize the pos of lastMovementPosition var.
    m_Players[player->GetGUIDLow()].SetPosition(player->GetPositionX(),player->GetPositionY(),player->GetPositionZ(),player->GetOrientation());
    QueryResult resultDB = CharacterDatabase.PQuery("SELECT * FROM daily_players_reports WHERE guid=%u;",player->GetGUIDLow());

    if (resultDB)
        m_Players[player->GetGUIDLow()].SetDailyReportState(true);*/
}

void AnticheatMgr::HandlePlayerLogout(Player* player)
{
    // TO-DO Make a table that stores the cheaters of the day, with more detailed information.

    // We must also delete it at logout to prevent have data of offline players in the db when we query the database (IE: The GM Command)
    CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u",player->GetGUIDLow());
    // Delete not needed data from the memory.
    m_Players.erase(player->GetGUIDLow());
}

void AnticheatMgr::SavePlayerData(Player* player)
{
    CharacterDatabase.PExecute("REPLACE INTO players_reports_status (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u);",player->GetGUIDLow(),m_Players[player->GetGUIDLow()].GetAverage(),m_Players[player->GetGUIDLow()].GetTotalReports(), m_Players[player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT),m_Players[player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT),m_Players[player->GetGUIDLow()].GetCreationTime());
}

uint32 AnticheatMgr::GetTotalReports(uint32 lowGUID)
{
    return m_Players[lowGUID].GetTotalReports();
}

float AnticheatMgr::GetAverage(uint32 lowGUID)
{
    return m_Players[lowGUID].GetAverage();
}

uint32 AnticheatMgr::GetTypeReports(uint32 lowGUID, uint8 type)
{
    return m_Players[lowGUID].GetTypeReports(type);
}

bool AnticheatMgr::MustCheckTempReports(uint8 type)
{
    if (type == JUMP_HACK_REPORT)
        return false;

    return true;
}

void AnticheatMgr::BuildReport(Player* p_Player, uint8 p_ReportType)
{
    if (p_Player->GetTransport() != nullptr)
        return;

    uint32 l_Key = p_Player->GetGUIDLow();

    if (MustCheckTempReports(p_ReportType))
    {
        uint32 l_ActualTime = getMSTime();

        if (!m_Players[l_Key].GetTempReportsTimer(p_ReportType))
            m_Players[l_Key].SetTempReportsTimer(l_ActualTime,p_ReportType);

        if (getMSTimeDiff(m_Players[l_Key].GetTempReportsTimer(p_ReportType),l_ActualTime) < 3000)
        {
            m_Players[l_Key].SetTempReports(m_Players[l_Key].GetTempReports(p_ReportType)+1,p_ReportType);

            if (m_Players[l_Key].GetTempReports(p_ReportType) < 3)
                return;
        }
        else
        {
            m_Players[l_Key].SetTempReportsTimer(l_ActualTime,p_ReportType);
            m_Players[l_Key].SetTempReports(1,p_ReportType);
            return;
        }
    }

    // generating creationTime for average calculation
    if (!m_Players[l_Key].GetTotalReports())
        m_Players[l_Key].SetCreationTime(getMSTime());

    // increasing total_reports
    m_Players[l_Key].SetTotalReports(m_Players[l_Key].GetTotalReports()+1);

    // increasing specific cheat report
    m_Players[l_Key].SetTypeReports(p_ReportType,m_Players[l_Key].GetTypeReports(p_ReportType)+1);

    // diff time for average calculation
    uint32 l_DiffTime = getMSTimeDiff(m_Players[l_Key].GetCreationTime(),getMSTime()) / IN_MILLISECONDS;

    if (l_DiffTime > 0)
    {
        // Average == Reports per second
        float average = float(m_Players[l_Key].GetTotalReports()) / float(l_DiffTime);
        m_Players[l_Key].SetAverage(average);
    }

    //if (sWorld->getIntConfig(CONFIG_ANTICHEAT_MAX_REPORTS_FOR_DAILY_REPORT) < m_Players[key].GetTotalReports())
    {
        if (!m_Players[l_Key].GetDailyReportState())
        {
            CharacterDatabase.PExecute("REPLACE INTO daily_players_reports (guid,average,total_reports,speed_reports,fly_reports,jump_reports,waterwalk_reports,teleportplane_reports,climb_reports,creation_time) VALUES (%u,%f,%u,%u,%u,%u,%u,%u,%u,%u);",p_Player->GetGUIDLow(),m_Players[p_Player->GetGUIDLow()].GetAverage(),m_Players[p_Player->GetGUIDLow()].GetTotalReports(), m_Players[p_Player->GetGUIDLow()].GetTypeReports(SPEED_HACK_REPORT),m_Players[p_Player->GetGUIDLow()].GetTypeReports(FLY_HACK_REPORT),m_Players[p_Player->GetGUIDLow()].GetTypeReports(JUMP_HACK_REPORT),m_Players[p_Player->GetGUIDLow()].GetTypeReports(WALK_WATER_HACK_REPORT),m_Players[p_Player->GetGUIDLow()].GetTypeReports(TELEPORT_PLANE_HACK_REPORT),m_Players[p_Player->GetGUIDLow()].GetTypeReports(CLIMB_HACK_REPORT),m_Players[p_Player->GetGUIDLow()].GetCreationTime());
            m_Players[l_Key].SetDailyReportState(true);
        }
    }

    m_Players[l_Key].AddReportToHistory();

    if (m_Players[l_Key].GetReportCountInLastSecs(sWorld->getIntConfig(CONFIG_ANTICHEAT_BAN_CHECK_TIME_RANGE)) > sWorld->getIntConfig(CONFIG_ANTICHEAT_MAX_REPORTS_BEFORE_BAN))
    {
        p_Player->GetSession()->KickPlayer();

        std::string l_Msg = "Player " + std::string(p_Player->GetName()) + " was kicked by the anti cheat";
        ChatHandler(p_Player).SendGlobalGMSysMessage(l_Msg.c_str());
    }
}

void AnticheatMgr::AnticheatGlobalCommand(ChatHandler* p_Handler)
{
    std::list<AnticheatData> l_TopCheatersList;
    for (auto l_CheatData : m_Players)
    {
        uint32 l_TotalReports = l_CheatData.second.GetTotalReports();
        if (l_TotalReports == 0)
            continue;

        l_TopCheatersList.push_back(l_CheatData.second);
    }

    l_TopCheatersList.sort([](AnticheatData const& p_CheatData1, AnticheatData const& p_CheatData2) -> bool
    {
        return p_CheatData1.GetTotalReports() > p_CheatData2.GetTotalReports();
    });

    if (l_TopCheatersList.empty())
        return;

    p_Handler->SendSysMessage("=============================");
    p_Handler->PSendSysMessage("Players with the more reports:");

    uint32 l_Counter = 0;
    for (auto l_CheatData : l_TopCheatersList)
    {

        if (Player* l_Player = sObjectMgr->GetPlayerByLowGUID(GUID_LOPART(l_CheatData.GetLastMovementInfo().guid)))
            p_Handler->PSendSysMessage("Player: %s Total Reports: %u Average: %f", l_Player->GetName(), l_CheatData.GetTotalReports(), l_CheatData.GetAverage());

        l_Counter++;
        if (l_Counter == 3)
            break;
    }
}

void AnticheatMgr::AnticheatDeleteCommand(uint32 guid)
{
    if (!guid)
    {
        for (AnticheatPlayersDataMap::iterator it = m_Players.begin(); it != m_Players.end(); ++it)
        {
            (*it).second.SetTotalReports(0);
            (*it).second.SetAverage(0);
            (*it).second.SetCreationTime(0);
            for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
            {
                (*it).second.SetTempReports(0,i);
                (*it).second.SetTempReportsTimer(0,i);
                (*it).second.SetTypeReports(i,0);
            }
        }
        CharacterDatabase.PExecute("DELETE FROM players_reports_status;");
    }
    else
    {
        m_Players[guid].SetTotalReports(0);
        m_Players[guid].SetAverage(0);
        m_Players[guid].SetCreationTime(0);
        for (uint8 i = 0; i < MAX_REPORT_TYPES; i++)
        {
            m_Players[guid].SetTempReports(0,i);
            m_Players[guid].SetTempReportsTimer(0,i);
            m_Players[guid].SetTypeReports(i,0);
        }
        CharacterDatabase.PExecute("DELETE FROM players_reports_status WHERE guid=%u;",guid);
    }
}

void AnticheatMgr::ResetDailyReportStates()
{
    for (AnticheatPlayersDataMap::iterator it = m_Players.begin(); it != m_Players.end(); ++it)
        m_Players[(*it).first].SetDailyReportState(false);
}
