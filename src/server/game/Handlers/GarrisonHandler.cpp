#include "Common.h"
#include "Language.h"
#include "DatabaseEnv.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Opcodes.h"
#include "Log.h"
#include "World.h"
#include "ObjectMgr.h"
#include "Player.h"
#include "UpdateMask.h"
#include "NPCHandler.h"
#include "Pet.h"
#include "MapManager.h"
#include "GarrisonMgr.hpp"
#include "CreatureAI.h"
#include "Chat.h"
#include "ScriptMgr.h"
#include "../../scripts/Draenor/Garrison/GarrisonScriptData.hpp"
#include "../../scripts/Draenor/Garrison/GarrisonNPC.hpp"

void WorldSession::HandleGetGarrisonInfoOpcode(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !l_Garrison->GetGarrisonSiteLevelEntry())
        return;
    
    std::vector<MS::Garrison::GarrisonPlotInstanceInfoLocation>   l_Plots             = l_Garrison->GetPlots();
    std::vector<MS::Garrison::GarrisonMission>                    l_CompletedMission  = l_Garrison->GetCompletedMissions();
    std::vector<MS::Garrison::GarrisonMission>                    l_Missions          = l_Garrison->GetMissions();
    std::vector<MS::Garrison::GarrisonBuilding>                   l_Buildings         = l_Garrison->GetBuildings();
    std::vector<MS::Garrison::GarrisonFollower>                   l_Followers         = l_Garrison->GetFollowers();

    /// @TODO: Update it to 6.1.2
    if (!m_Player->IsInGarrison())
    {
        WorldPacket l_Data(SMSG_GARRISON_REMOTE_INFO, 200);

        l_Data << uint32(1);                                                        ///< @TODO Site Count

        /// For
        /// {
            l_Data << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID);      ///< Site Level ID
            l_Data << uint32(l_Buildings.size());                                       ///< Buildings

            for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
            {
                l_Data << uint32(l_Buildings[l_I].PlotInstanceID);                      ///< Garr Plot Instance ID
                l_Data << uint32(l_Buildings[l_I].BuildingID);                          ///< Garr Building ID
            }
        /// }

        SendPacket(&l_Data);
    }

    WorldPacket l_Infos(SMSG_GET_GARRISON_INFO_RESULT, 5 * 1024);

    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteID);          ///< Site ID
    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID);     ///< Site Level ID
    l_Infos << int32(l_Garrison->GetGarrisonFactionIndex());                    ///< Faction Index
    
    l_Infos << uint32(l_Buildings.size());
    l_Infos << uint32(l_Plots.size());
    l_Infos << uint32(l_Followers.size());
    l_Infos << uint32(l_Missions.size());
    l_Infos << uint32(0);                                                       ///< Uint32 loop - ship related ? 6.2
    l_Infos << uint32(l_Missions.size());                                       ///< 6.2.0 IsMissionNavel ????
    l_Infos << uint32(l_CompletedMission.size());

    l_Infos << int32(l_Garrison->GetNumFollowerActivationsRemaining());

    for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
    {
        l_Infos << uint32(l_Buildings[l_I].PlotInstanceID);
        l_Infos << uint32(l_Buildings[l_I].BuildingID);
        l_Infos << uint32(l_Buildings[l_I].TimeBuiltStart);
        l_Infos << uint32(l_Buildings[l_I].SpecID);
        l_Infos << uint32(l_Buildings[l_I].TimeBuiltEnd);

        l_Infos.WriteBit(l_Buildings[l_I].Active);
        l_Infos.FlushBits();
    }

    for (uint32 l_I = 0; l_I < l_Plots.size(); ++l_I)
    {
        l_Infos << int32(l_Plots[l_I].PlotInstanceID);
        l_Infos << float(l_Plots[l_I].X);
        l_Infos << float(l_Plots[l_I].Y);
        l_Infos << float(l_Plots[l_I].Z);
        l_Infos << float(l_Plots[l_I].O);
        l_Infos << uint32(l_Garrison->GetPlotType(l_Plots[l_I].PlotInstanceID));
    }

    for (uint32 l_I = 0; l_I < l_Followers.size(); ++l_I)
    {
        l_Followers[l_I].Write(l_Infos);
    }

    for (uint32 l_I = 0; l_I < l_Missions.size(); ++l_I)
    {
        uint32 l_TravelDuration     = 0;
        uint32 l_MissionDuration    = 0;

        if (l_Missions[l_I].State == MS::Garrison::MissionStates::InProgress && sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID))
        {
            l_TravelDuration    = l_Garrison->GetMissionTravelDuration(l_Missions[l_I].MissionID);
            l_MissionDuration   = l_Garrison->GetMissionDuration(l_Missions[l_I].MissionID);
        }

        l_Infos << uint64(l_Missions[l_I].DatabaseID);
        l_Infos << uint32(l_Missions[l_I].MissionID);
        l_Infos << uint32(l_Missions[l_I].OfferTime);
        l_Infos << uint32(l_Missions[l_I].OfferMaxDuration);
        l_Infos << uint32(l_Missions[l_I].StartTime);
        l_Infos << uint32(l_TravelDuration);
        l_Infos << uint32(l_MissionDuration);
        l_Infos << uint32(l_Missions[l_I].State);
    }

    for (uint32 l_I = 0; l_I < l_CompletedMission.size(); ++l_I)
        l_Infos << int32(l_CompletedMission[l_I].MissionID);

    for (uint32 l_I = 0; l_I < l_Missions.size(); ++l_I)
        l_Infos.WriteBit(0);

    l_Infos.FlushBits();
    SendPacket(&l_Infos);

    std::vector<int32> l_KnownBlueprints        = l_Garrison->GetKnownBlueprints();
    std::vector<int32> l_KnownSpecializations   = l_Garrison->GetKnownSpecializations();

    WorldPacket l_Data(SMSG_GARRISON_BLUEPRINT_AND_SPECIALIZATION_DATA, 500);

    l_Data << uint32(l_KnownBlueprints.size());
    l_Data << uint32(l_KnownSpecializations.size());

    for (uint32 l_I = 0; l_I < l_KnownBlueprints.size(); ++l_I)
        l_Data << int32(l_KnownBlueprints[l_I]);

    for (uint32 l_I = 0; l_I < l_KnownSpecializations.size(); ++l_I)
        l_Data << int32(l_KnownSpecializations[l_I]);

    SendPacket(&l_Data);
}

void WorldSession::HandleRequestGarrisonUpgradeableOpcode(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    bool l_CanUpgrade = l_Garrison->CanUpgrade();

    WorldPacket l_Data(SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT, 4);

    l_Data << uint32(!l_CanUpgrade);

    SendPacket(&l_Data);
}

void WorldSession::HandleUpgradeGarrisonOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeGarrisonOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    if (!l_Garrison->CanUpgrade())
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleUpgradeGarrisonOpcode - Can't upgrade");
        return;
    }

    l_Garrison->Upgrade();
}

void WorldSession::HandleRequestLandingPageShipmentInfoOpcode(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

}

void WorldSession::HandleGarrisonMissionNPCHelloOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
        l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_SHIPYARD_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    SendGarrisonOpenMissionNpc(l_NpcGUID);
}

void WorldSession::HandleGarrisonRequestSetMissionNPC(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonRequestSetMissionNPC - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    SendGarrisonSetMissionNpc(l_NpcGUID);
}

void WorldSession::HandleGarrisonRequestBuildingsOpcode(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !l_Garrison->GetGarrisonSiteLevelEntry())
        return;

    std::vector<MS::Garrison::GarrisonBuilding> l_Buildings = l_Garrison->GetBuildings();

    WorldPacket l_Data(SMSG_GARRISON_GET_BUILDINGS_DATA, 200);
    l_Data << uint32(l_Buildings.size());

    for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
    {
        MS::Garrison::GarrisonPlotInstanceInfoLocation l_PlotLocation = l_Garrison->GetPlot(l_Buildings[l_I].PlotInstanceID);
        uint32 l_SiteLevelPlotInstanceID = 0;
        uint32 l_BuildingPlotInstanceID  = 0;

        for (uint32 l_Y = 0; l_Y < sGarrSiteLevelPlotInstStore.GetNumRows(); ++l_Y)
        {
            const GarrSiteLevelPlotInstEntry* l_Entry = sGarrSiteLevelPlotInstStore.LookupEntry(l_Y);

            if (l_Entry && l_Entry->PlotInstanceID == l_Buildings[l_I].PlotInstanceID && l_Entry->SiteLevelID == l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID)
            {
                l_SiteLevelPlotInstanceID = l_Entry->ID;
                break;
            }
        }

        for (uint32 l_Y = 0; l_Y < sGarrBuildingPlotInstStore.GetNumRows(); ++l_Y)
        {
            const GarrBuildingPlotInstEntry* l_Entry = sGarrBuildingPlotInstStore.LookupEntry(l_Y);

            if (l_Entry && l_Entry->SiteLevelPlotInstID == l_SiteLevelPlotInstanceID && l_Entry->BuildingID == l_Buildings[l_I].BuildingID)
            {
                l_BuildingPlotInstanceID = l_Entry->ID;
                break;
            }
        }

        l_Data << uint32(l_BuildingPlotInstanceID);
        l_Data << float(l_PlotLocation.X);
        l_Data << float(l_PlotLocation.Y);
        l_Data << float(l_PlotLocation.Z);
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleGarrisonPurchaseBuildingOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_BuildingID     = 0;
    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;
    p_RecvData >> l_BuildingID;

    sGarrisonBuildingManager->LearnAllowedBuildings(m_Player, l_Garrison);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonPurchaseBuildingOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    MS::Garrison::PurchaseBuildingResults::Type l_Result = MS::Garrison::PurchaseBuildingResults::Ok;

    if (!sGarrBuildingStore.LookupEntry(l_BuildingID))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidBuildingID;

    if (!l_Result && !sGarrPlotInstanceStore.LookupEntry(l_PlotInstanceID))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidPlot;

    if (!l_Result && !l_Garrison->KnownBlueprint(l_BuildingID))
        l_Result = MS::Garrison::PurchaseBuildingResults::RequireBluePrint;

    if (!l_Result && l_Garrison->GetBuilding(l_BuildingID).BuildingID != 0)
        l_Result = MS::Garrison::PurchaseBuildingResults::BuildingExists;

    if (!l_Result && !l_Garrison->IsBuildingPlotInstanceValid(l_BuildingID, l_PlotInstanceID))
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidPlotBuilding;

    if (!l_Result)
        l_Result = l_Garrison->CanPurchaseBuilding(l_BuildingID);

    if (!sGarrisonBuildingManager->IsBluePrintAllowedForPurchasingBuilding(l_BuildingID, m_Player))
    {
        l_Result = MS::Garrison::PurchaseBuildingResults::InvalidBuildingID;

        std::string l_Message = "Building not available yet";
        switch (m_Player->GetSession()->GetSessionDbcLocale())
        {
            case LocaleConstant::LOCALE_frFR:
                l_Message = "Batiment non disponible";
                break;

            case LocaleConstant::LOCALE_esES:
            case LocaleConstant::LOCALE_esMX:
                l_Message = "Edificio no disponible";
                break;

            default:
                break;
        }

        WorldPacket l_Data(SMSG_PRINT_NOTIFICATION, 2 + l_Message.size());
        l_Data.WriteBits(l_Message.size(), 12);
        l_Data.FlushBits();
        l_Data.WriteString(l_Message);

        SendPacket(&l_Data);
    }

    WorldPacket l_PlaceResult(SMSG_GARRISON_PLACE_BUILDING_RESULT, 26);
    l_PlaceResult << uint32(l_Result);

    if (l_Result == MS::Garrison::PurchaseBuildingResults::Ok)
    {
        MS::Garrison::GarrisonBuilding l_Building = l_Garrison->PurchaseBuilding(l_BuildingID, l_PlotInstanceID);

        l_PlaceResult << uint32(l_PlotInstanceID);
        l_PlaceResult << uint32(l_BuildingID);
        l_PlaceResult << uint32(l_Building.TimeBuiltStart);
        l_PlaceResult << uint32(l_Building.SpecID);
        l_PlaceResult << uint32(l_Building.TimeBuiltEnd);
        l_PlaceResult.WriteBit(l_Building.Active);
        l_PlaceResult.FlushBits();
    }
    else
    {
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult << uint32(0);
        l_PlaceResult.WriteBit(false);
        l_PlaceResult.FlushBits();
    }

    l_PlaceResult.WriteBit(false);                      ///< Unk bit
    l_PlaceResult.FlushBits();

    SendPacket(&l_PlaceResult);
}

void WorldSession::HandleGarrisonCancelConstructionOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonCancelConstructionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->CancelConstruction(l_PlotInstanceID);
}

void WorldSession::HandleGarrisonStartMissionOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID        = 0;
    uint32 l_FollowerCount  = 0;
    uint32 l_MissionID      = 0;

    std::vector<uint64> l_Followers;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_FollowerCount;
    p_RecvData >> l_MissionID;

    for (uint32 l_I = 0; l_I < l_FollowerCount; ++l_I)
    {
        uint64 l_FollowerDBID = 0;
        p_RecvData >> l_FollowerDBID;

        l_Followers.push_back(l_FollowerDBID);
    }

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonStartMissionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->StartMission(l_MissionID, l_Followers);
}

void WorldSession::HandleGarrisonCompleteMissionOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID   = 0;
    uint32 l_MissionID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_MissionID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonCompleteMissionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }
    
    l_Garrison->CompleteMission(l_MissionID);
}

void WorldSession::HandleGarrisonMissionBonusRollOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID    = 0;
    uint32 l_MissionID  = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_MissionID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionBonusRollOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->DoMissionBonusRoll(l_NpcGUID, l_MissionID);
}

void WorldSession::HandleGarrisonGenerateRecruitsOpcode(WorldPacket& p_RecvData)
{
    if (m_Player == nullptr)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (l_Garrison == nullptr)
        return;

    uint64 l_GUID      = 0;
    uint32 l_TraitID   = 0;
    uint32 l_AbilityID = 0;

    p_RecvData.readPackGUID(l_GUID);
    p_RecvData >> l_AbilityID;
    p_RecvData >> l_TraitID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWith(l_GUID, 0);

    if (l_Unit == nullptr)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionBonusRollOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_GUID)));
        return;
    }

    if (l_Unit->AI())
        static_cast<MS::Garrison::GarrisonNPCAI*>(l_Unit->AI())->SendRecruitmentFollowersGenerated(m_Player, l_AbilityID ? l_AbilityID : l_TraitID, 0, l_TraitID ? true : false);
}

void WorldSession::HandleGarrisonSetRecruitmentPreferencesOpcode(WorldPacket& p_RecvData)
{
    if (m_Player == nullptr)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (l_Garrison == nullptr)
        return;
    

    uint64 l_GUID      = 0; ///< Unused ?
    uint32 l_TraitID   = 0;
    uint32 l_AbilityID = 0;

    p_RecvData.readPackGUID(l_GUID);

    p_RecvData >> l_AbilityID;
    p_RecvData >> l_TraitID;
}

void WorldSession::HandleGarrisonRecruitFollower(WorldPacket& p_RecvData)
{
    if (m_Player == nullptr)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (l_Garrison == nullptr)
        return;

    uint64 l_GUID       = 0;

    p_RecvData.readPackGUID(l_GUID);
    uint32 l_FollowerID = 0;
    p_RecvData >> l_FollowerID;

    Creature* l_Unit = m_Player->GetNPCIfCanInteractWith(l_GUID, 0);

    if (l_Unit == nullptr)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionBonusRollOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_GUID)));
        return;
    }

    WorldPacket l_RecruitmentResult(SMSG_GARRISON_RECRUIT_FOLLOWER_RESULT, 64);

    l_RecruitmentResult << uint32(MS::Garrison::PurchaseBuildingResults::Ok);

    std::vector<MS::Garrison::GarrisonFollower> l_WeeklyFollowers = l_Garrison->GetWeeklyFollowerRecruits(m_Player);

    for (MS::Garrison::GarrisonFollower l_Follower : l_WeeklyFollowers)
    {
        if (l_Follower.FollowerID == l_FollowerID)
        {
            l_Follower.Write(l_RecruitmentResult);
            l_Garrison->AddFollower(l_Follower);
            break;
        }
    }

    m_Player->SendDirectMessage(&l_RecruitmentResult);
    m_Player->PlayerTalkClass->SendCloseGossip();
}

void WorldSession::HandleGarrisonChangeFollowerActivationStateOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64  l_FollowerDBID  = 0;
    bool    l_Desactivate   = false;

    p_RecvData >> l_FollowerDBID;
    l_Desactivate = p_RecvData.ReadBit();

    l_Garrison->ChangeFollowerActivationState(l_FollowerDBID, !l_Desactivate);
}

void WorldSession::HandleGarrisonAssignFollowerToBuilding(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_GarrisonMgr = m_Player->GetGarrison();

    if (!l_GarrisonMgr)
        return;

    uint64 l_NpcGUID       = 0;
    uint64 l_FollowerDBID  = 0;
    int32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    l_PlotInstanceID = p_RecvData.read<int32>();
    p_RecvData >> l_FollowerDBID;

    Creature* l_Creature = m_Player->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonAssignFollowerToBuilding - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    if (l_PlotInstanceID)
    {
        l_GarrisonMgr->AssignFollowerToBuilding(l_FollowerDBID, (uint32)l_PlotInstanceID);

        WorldPacket l_Response(SMSG_GARRISON_ASSIGN_FOLLOWER_TO_BUILDING_RESULT, 1024);

        uint8 l_Result = 0; ///< Always 0 ?

        l_Response << uint64(l_FollowerDBID);
        l_Response << int32(l_Result);
        l_Response << int32(l_PlotInstanceID);

        SendPacket(&l_Response);
    }
}

void WorldSession::HandleGarrisonRemoveFollowerFromBuilding(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_GarrisonMgr = m_Player->GetGarrison();

    if (!l_GarrisonMgr)
        return;

    uint64 l_NpcGUID      = 0;
    uint64 l_FollowerDBID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_FollowerDBID;

    Creature* l_Creature = m_Player->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Creature)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonRemoveFollowerFromBuilding - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_GarrisonMgr->AssignFollowerToBuilding(l_FollowerDBID, 0);

    WorldPacket l_Response(SMSG_GARRISON_REMOVE_FOLLOWER_FROM_BUILDING_RESULT, 1024);

    uint8 l_Result = 0; ///< Always 0 ?

    l_Response << uint64(l_FollowerDBID);
    l_Response << int32(l_Result);

    SendPacket(&l_Response);
}

void WorldSession::HandleGarrisonGetShipmentInfoOpcode(WorldPacket& p_RecvData)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !m_Player->IsInGarrison())
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    uint32 l_ShipmentID     = 0;
    uint32 l_OrderAvailable = 0;
    uint32 l_PlotInstanceID = 0;

    l_PlotInstanceID = l_Garrison->GetCreaturePlotInstanceID(l_NpcGUID);

    if (!!l_PlotInstanceID)
    {
        l_OrderAvailable = l_Garrison->GetBuildingMaxWorkOrder(l_PlotInstanceID);

        uint32 l_BuildingID = l_Garrison->GetBuilding(l_PlotInstanceID).BuildingID;

        if (l_BuildingID)
        {
            if (l_Unit->AI())
                l_ShipmentID = l_Unit->AI()->OnShipmentIDRequest(m_Player);

            if (l_ShipmentID == -1) ///< Comparison of integers of different signs: 'uint32' (aka 'unsigned int') and 'int'
                l_ShipmentID = sGarrisonShipmentManager->GetShipmentIDForBuilding(l_BuildingID, m_Player, false);
        }
    }

    bool l_Success = !!l_ShipmentID && !!l_PlotInstanceID;

    WorldPacket l_Response(SMSG_GET_SHIPMENT_INFO_RESPONSE, 1024);
    l_Response.WriteBit(l_Success);
    l_Response.FlushBits();

    if (l_Success)
    {
        std::vector<MS::Garrison::GarrisonWorkOrder> l_WorkOrders = l_Garrison->GetWorkOrders();

        uint32 l_PendingWorkOrderCount = std::count_if(l_WorkOrders.begin(), l_WorkOrders.end(), [l_PlotInstanceID](const MS::Garrison::GarrisonWorkOrder & p_Order) -> bool
        {
            return p_Order.PlotInstanceID == l_PlotInstanceID;
        });


        l_Response << uint32(l_ShipmentID);
        l_Response << uint32(l_OrderAvailable);
        l_Response << uint32(l_PendingWorkOrderCount);
        l_Response << uint32(l_PlotInstanceID);

        for (uint32 l_I = 0; l_I < l_WorkOrders.size(); ++l_I)
        {
            if (l_WorkOrders[l_I].PlotInstanceID != l_PlotInstanceID)
                continue;

            uint32 l_Duration = 0;
        
            const CharShipmentEntry* l_Entry = sCharShipmentStore.LookupEntry(l_WorkOrders[l_I].ShipmentID);
        
            if (l_Entry)
                l_Duration = l_Entry->Duration;

            l_Response << uint32(l_WorkOrders[l_I].ShipmentID);
            l_Response << uint64(l_WorkOrders[l_I].DatabaseID);
            l_Response << uint64(0);                                    ///< 6.1.x FollowerID
            l_Response << uint32(l_WorkOrders[l_I].CreationTime);
            l_Response << uint32(l_Duration);
            l_Response << uint32(0);                                    ///< 6.1.x Rewarded XP
        }
    }
    else
    {
        l_Response << uint32(0);
        l_Response << uint32(0);
        l_Response << uint32(0);
        l_Response << uint32(0);
    }

    SendPacket(&l_Response);
}

void WorldSession::HandleGarrisonCreateShipmentOpcode(WorldPacket& p_RecvData)
{
    std::function<void(const std::string &)> l_OnError = [this](const std::string & p_Message) -> void
    {
        if (m_Player->GetSession()->GetSecurity() > SEC_PLAYER)
            ChatHandler(m_Player).PSendSysMessage("HandleGarrisonCreateShipmentOpcode => %s", p_Message.c_str());

        WorldPacket l_Ack(SMSG_CREATE_SHIPMENT_RESPONSE, 16);
        l_Ack << uint64(0);
        l_Ack << uint32(0);
        l_Ack << uint32(1); ///< 0 = success & 1 = error

        m_Player->SendDirectMessage(&l_Ack);
    };

    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !m_Player->IsInGarrison())
        return;

    uint64 l_NpcGUID = 0;
    uint32 l_Count = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_Count;

    /// Min 1 work order
    if (!l_Count)
        l_Count = 1;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    uint32 l_ShipmentID     = 0;
    uint32 l_OrderMax       = 0;
    uint32 l_PlotInstanceID = 0;

    l_PlotInstanceID = l_Garrison->GetCreaturePlotInstanceID(l_NpcGUID);

    if (!!l_PlotInstanceID)
    {
        l_OrderMax = l_Garrison->GetBuildingMaxWorkOrder(l_PlotInstanceID);

        uint32 l_BuildingID = l_Garrison->GetBuilding(l_PlotInstanceID).BuildingID;

        if (l_BuildingID)
        {
            l_ShipmentID = sGarrisonShipmentManager->GetShipmentIDForBuilding(l_BuildingID, m_Player, true);

            if (l_ShipmentID == MS::Garrison::Barn::ShipmentIDS::ShipmentFurredBeast)
            {
                if (l_Unit->AI())
                    l_ShipmentID = l_Unit->AI()->OnShipmentIDRequest(m_Player);
            }

            sScriptMgr->OnShipmentCreated(m_Player, l_Unit, l_BuildingID);
        }
    }

    if (!l_ShipmentID || !l_PlotInstanceID)
    {
        l_OnError("Invalid ShipmentID or PlotInstanceID");
        return;
    }

    for (uint32 l_OrderI = 0; l_OrderI < l_Count; ++l_OrderI)
    {
        if (((int32)l_OrderMax - (int32)l_Garrison->GetWorkOrderCount(l_PlotInstanceID)) < 1)
        {
            l_OnError("Max work order for this building reached");
            return;
        }

        const CharShipmentEntry * l_ShipmentEntry = sCharShipmentStore.LookupEntry(l_ShipmentID);

        if (!l_ShipmentEntry)
        {
            l_OnError("Shipment entry not found");
            return;
        }

        const SpellInfo * l_Spell = sSpellMgr->GetSpellInfo(l_ShipmentEntry->SpellID);

        if (!l_Spell)
        {
            l_OnError("Shipment spell not found");
            return;
        }

        bool l_HasReagents = true;
        for (uint32 l_I = 0; l_I < MAX_SPELL_REAGENTS; ++l_I)
        {
            uint32 l_ItemEntry = l_Spell->Reagent[l_I];
            uint32 l_ItemCount = l_Spell->ReagentCount[l_I];

            if (!l_ItemEntry || !l_ItemCount)
                continue;

            if (!m_Player->HasItemCount(l_ItemEntry, l_ItemCount))
                l_HasReagents = false;
        }

        if (l_Spell->CurrencyID)
        {
            if (!m_Player->HasCurrency(l_Spell->CurrencyID, l_Spell->CurrencyCount))
                l_HasReagents = false;
        }

        if (!l_HasReagents)
        {
            l_OnError("Doesn't have reagents");
            return;
        }

        for (uint32 l_I = 0; l_I < MAX_SPELL_REAGENTS; ++l_I)
        {
            uint32 l_ItemEntry = l_Spell->Reagent[l_I];
            uint32 l_ItemCount = l_Spell->ReagentCount[l_I];

            if (!l_ItemEntry || !l_ItemCount)
                continue;

            m_Player->DestroyItemCount(l_ItemEntry, l_ItemCount, true);
        }

        if (l_Spell->CurrencyID)
            m_Player->ModifyCurrency(l_Spell->CurrencyID, -int32(l_Spell->CurrencyCount), false);

        m_Player->CastSpell(m_Player, l_Spell, TRIGGERED_FULL_MASK);

        uint64 l_DatabaseID = l_Garrison->StartWorkOrder(l_PlotInstanceID, l_ShipmentID);

        WorldPacket l_Ack(SMSG_CREATE_SHIPMENT_RESPONSE, 16);
        l_Ack << uint64(l_DatabaseID);
        l_Ack << uint32(l_ShipmentID);
        l_Ack << uint32(l_DatabaseID != 0);

        m_Player->SendDirectMessage(&l_Ack);
    }
}

void WorldSession::HandleGarrisonGetShipmentsOpcode(WorldPacket& /*p_RecvData*/)
{
    if (!m_Player)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison || !l_Garrison->GetGarrisonSiteLevelEntry())
        return;

    std::vector<MS::Garrison::GarrisonWorkOrder> l_WorkOrders = l_Garrison->GetWorkOrders();

    WorldPacket l_Data(SMSG_GET_SHIPMENTS, 1024);
    l_Data << uint32(l_WorkOrders.size());

    for (uint32 l_I = 0; l_I < l_WorkOrders.size(); ++l_I)
    {
        uint32 l_Duration = 0;

        const CharShipmentEntry * l_Entry = sCharShipmentStore.LookupEntry(l_WorkOrders[l_I].ShipmentID);

        if (l_Entry)
            l_Duration = l_Entry->Duration;

        /// @TODO http://www.mmo-champion.com/content/4662-Patch-6-1-Iron-Horde-Scrap-Meltdown-Garrison-Vendor-Rush-Orders-Blue-Posts
        l_Data << uint32(l_WorkOrders[l_I].ShipmentID);
        l_Data << uint64(l_WorkOrders[l_I].DatabaseID);
        l_Data << uint64(0);                                    ///< FollowerID
        l_Data << uint32(l_WorkOrders[l_I].CreationTime);
        l_Data << uint32(l_Duration);
        l_Data << uint32(0);                                    ///< Rewarded XP
    }

    SendPacket(&l_Data);
}

void WorldSession::HandleGarrisonFollowerRename(WorldPacket& p_RecvData)
{
    uint64 l_DatabaseID;
    uint32 l_NameLen;
    std::string l_Name;

    p_RecvData >> l_DatabaseID;
    l_NameLen = p_RecvData.ReadBits(7);
    l_Name = p_RecvData.ReadString(l_NameLen);

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    l_Garrison->RenameFollower(l_DatabaseID, l_Name);
}

void WorldSession::HandleGarrisonDecommisionShip(WorldPacket& p_RecvData)
{
    uint64 l_Guid, l_DatabaseID;

    p_RecvData.readPackGUID(l_Guid);
    p_RecvData >> l_DatabaseID;

    Creature* l_NPC = m_Player->GetNPCIfCanInteractWithFlag2(l_Guid, UNIT_NPC_FLAG2_SHIPYARD_MISSION_NPC);

    if (!l_NPC)
        return;

    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    l_Garrison->RemoveFollower(l_DatabaseID);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void WorldSession::SendGarrisonOpenArchitect(uint64 p_CreatureGUID)
{
    MS::Garrison::Manager * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_ARCHITECT, 18);
    l_Data.appendPackGUID(p_CreatureGUID);

    SendPacket(&l_Data);
}
void WorldSession::SendGarrisonOpenMissionNpc(uint64 /*p_CreatureGUID*/)
{
    MS::Garrison::Manager* l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_MISSION_NPC, 9);

    l_Data << uint32(0);
    l_Data << uint32(0);
    l_Data.WriteBit(false);
    l_Data.FlushBits();

    SendPacket(&l_Data);
}

void WorldSession::SendGarrisonSetMissionNpc(uint64 p_CreatureGUID)
{
    Creature* l_Creature = sObjectAccessor->FindCreature(p_CreatureGUID);

    WorldPacket l_Data(SMSG_GARRISON_SET_MISSION_NPC, 22);
    l_Data.appendPackGUID(p_CreatureGUID);
    l_Data << uint32(l_Creature && l_Creature->HasFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_SHIPYARD_MISSION_NPC) ? MS::Garrison::FollowerType::Ship : MS::Garrison::FollowerType::NPC);
    SendPacket(&l_Data);
}
