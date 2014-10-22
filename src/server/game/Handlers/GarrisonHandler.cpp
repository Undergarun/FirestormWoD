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
#include "Garrison.h"

void WorldSession::HandleGetGarrisonInfoOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;
    
    std::vector<GarrisonPlotInstanceInfoLocation>   l_Plots             = l_Garrison->GetPlots();
    std::vector<GarrisonMission>                    l_CompletedMission  = l_Garrison->GetCompletedMissions();
    std::vector<GarrisonMission>                    l_Missions          = l_Garrison->GetMissions();
    std::vector<GarrisonBuilding>                   l_Buildings         = l_Garrison->GetBuildings();
    std::vector<GarrisonFollower>                   l_Followers         = l_Garrison->GetFollowers();

    WorldPacket l_Infos(SMSG_GET_GARRISON_INFO_RESULT, 200);

    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteID);          ///< Site ID
    l_Infos << int32(l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID);     ///< Site Level ID
    l_Infos << int32(1);                                                        ///< Unk
    
    l_Infos << uint32(l_Buildings.size());
    l_Infos << uint32(l_Plots.size());
    l_Infos << uint32(l_Followers.size());
    l_Infos << uint32(l_Missions.size());
    l_Infos << uint32(l_CompletedMission.size());

    l_Infos << int32(l_Garrison->GetGarrisonFactionIndex());                    ///< Faction Index

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
        l_Infos << uint64(l_Followers[l_I].DB_ID);
        l_Infos << uint32(l_Followers[l_I].FollowerID);
        l_Infos << uint32(l_Followers[l_I].Quality);
        l_Infos << uint32(l_Followers[l_I].Level);
        l_Infos << uint32(l_Followers[l_I].ItemLevelWeapon);
        l_Infos << uint32(l_Followers[l_I].ItemLevelArmor);
        l_Infos << uint32(l_Followers[l_I].XP);
        l_Infos << uint32(l_Followers[l_I].CurrentBuildingID);
        l_Infos << uint32(l_Followers[l_I].CurrentMissionID);

        l_Infos << uint32(l_Followers[l_I].Abilities.size());
        l_Infos << uint32(0);       ///< Unk

        for (uint32 l_Y = 0; l_Y < l_Followers[l_I].Abilities.size(); ++l_Y)
            l_Infos << int32(l_Followers[l_I].Abilities[l_Y]);
    }

    for (uint32 l_I = 0; l_I < l_Missions.size(); ++l_I)
    {
        uint32 l_TravelDuration     = 0;
        uint32 l_MissionDuration    = 0;

        if (l_Missions[l_I].State == GARRISON_MISSION_IN_PROGRESS && sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID))
        {
            //l_TravelDuration    = sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID)->TravelDuration;
            //l_MissionDuration   = sGarrMissionStore.LookupEntry(l_Missions[l_I].MissionID)->MissionDuration;
        }

        l_Infos << uint64(l_Missions[l_I].DB_ID);
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

    SendPacket(&l_Infos);

    std::vector<int32> l_KnownBlueprints = l_Garrison->GetKnownBlueprints();
    std::vector<int32> l_KnownSpecializations = l_Garrison->GetKnownSpecializations();

    WorldPacket l_Data(SMSG_GARRISON_BLUEPRINT_AND_SPECIALIZATION_DATA, 200);

    l_Data << uint32(l_KnownBlueprints.size());
    l_Data << uint32(l_KnownSpecializations.size());

    for (uint32 l_I = 0; l_I < l_KnownBlueprints.size(); ++l_I)
        l_Data << int32(l_KnownBlueprints[l_I]);

    for (uint32 l_I = 0; l_I < l_KnownSpecializations.size(); ++l_I)
        l_Data << int32(l_KnownSpecializations[l_I]);

    SendPacket(&l_Data);
}
void WorldSession::HandleRequestGarrisonUpgradeableOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    bool l_CanUpgrade = false;

    WorldPacket l_Data(SMSG_GARRISON_REQUEST_UPGRADEABLE_RESULT, 4);

    l_Data << uint32(!l_CanUpgrade);

    SendPacket(&l_Data);
}
void WorldSession::HandleRequestLandingPageShipmentInfoOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

}
void WorldSession::HandleGarrisonMissionNPCHelloOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_MISSION_NPC);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonMissionNPCHelloOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    SendGarrisonOpenMissionNpc(l_NpcGUID);
}
void WorldSession::HandleGarrisonRequestBuildingsOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    std::vector<GarrisonBuilding> l_Buildings = l_Garrison->GetBuildings();

    WorldPacket l_Data(SMSG_GARRISON_GET_BUILDINGS_DATA, 200);
    l_Data << uint32(l_Buildings.size());

    for (uint32 l_I = 0; l_I < l_Buildings.size(); ++l_I)
    {
        GarrisonPlotInstanceInfoLocation l_PlotLocation = l_Garrison->GetPlot(l_Buildings[l_I].PlotInstanceID);
        uint32 l_SiteLevelPlotInstanceID = 0;
        uint32 l_BuildingPlotInstanceID  = 0;

        for (uint32 l_Y = 0; l_Y < sGarrSiteLevelPlotInstStore.GetNumRows(); ++l_Y)
        {
            const GarrSiteLevelPlotInstEntry * l_Entry = sGarrSiteLevelPlotInstStore.LookupEntry(l_Y);

            if (l_Entry && l_Entry->PlotInstanceID == l_Buildings[l_I].PlotInstanceID && l_Entry->SiteLevelID == l_Garrison->GetGarrisonSiteLevelEntry()->SiteLevelID)
            {
                l_SiteLevelPlotInstanceID = l_Entry->ID;
                break;
            }
        }

        for (uint32 l_Y = 0; l_Y < sGarrBuildingPlotInstStore.GetNumRows(); ++l_Y)
        {
            const GarrBuildingPlotInstEntry * l_Entry = sGarrBuildingPlotInstStore.LookupEntry(l_Y);

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
void WorldSession::HandleGarrisonPurchaseBuildingOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_BuildingID     = 0;
    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;
    p_RecvData >> l_BuildingID;

    Creature* l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonPurchaseBuildingOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    GarrisonPurchaseBuildingResult l_Result = GARRISON_PURCHASE_BUILDING_OK;

    if (!sGarrBuildingStore.LookupEntry(l_BuildingID))
        l_Result = GARRISON_PURCHASE_BUILDING_INVALID_BUILDING_ID;

    if (!l_Result && !sGarrPlotInstanceStore.LookupEntry(l_PlotInstanceID))
        l_Result = GARRISON_PURCHASE_BUILDING_INVALID_PLOT;

    if (!l_Result && !l_Garrison->KnownBlueprint(l_BuildingID))
        l_Result = GARRISON_PURCHASE_BUILDING_REQUIRE_BLUE_PRINT;

    if (!l_Result && l_Garrison->GetBuilding(l_BuildingID).BuildingID != 0)
        l_Result = GARRISON_PURCHASE_BUILDING_BUILDING_EXIST;

    if (!l_Result && !l_Garrison->IsBuildingPlotInstanceValid(l_BuildingID, l_PlotInstanceID))
        l_Result = GARRISON_PURCHASE_BUILDING_INVALID_PLOT_BUILDING;

    if (!l_Result)
        l_Result = l_Garrison->CanPurchaseBuilding(l_BuildingID);

    WorldPacket l_PlaceResult(SMSG_GARRISON_PLACE_BUILDING_RESULT, 26);
    l_PlaceResult << uint32(l_Result);

    if (l_Result == GARRISON_PURCHASE_BUILDING_OK)
    {
        GarrisonBuilding l_Building = l_Garrison->PurchaseBuilding(l_BuildingID, l_PlotInstanceID);

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
void WorldSession::HandleGarrisonCancelConstructionOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    uint64 l_NpcGUID = 0;

    uint32 l_PlotInstanceID = 0;

    p_RecvData.readPackGUID(l_NpcGUID);
    p_RecvData >> l_PlotInstanceID;

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonCancelConstructionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    l_Garrison->CancelConstruction(l_PlotInstanceID);
}
void WorldSession::HandleGarrisonStartMissionOpcode(WorldPacket & p_RecvData)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

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

    Creature * l_Unit = GetPlayer()->GetNPCIfCanInteractWithFlag2(l_NpcGUID, UNIT_NPC_FLAG2_GARRISON_ARCHITECT);

    if (!l_Unit)
    {
        sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: HandleGarrisonStartMissionOpcode - Unit (GUID: %u) not found or you can not interact with him.", uint32(GUID_LOPART(l_NpcGUID)));
        return;
    }

    if (!l_Garrison->HaveMission(l_MissionID))
    {
        /// TODO ERROR dont have the mission
        return;
    }

    const GarrMissionEntry * l_MissionTemplate = sGarrMissionStore.LookupEntry(l_MissionID);

    if (l_Followers.size() < l_MissionTemplate->RequiredFollowersCount)
    {
        /// TODO ERROR not enought follower
        return;
    }

    std::vector<GarrisonFollower> l_GarrFollowers = l_Garrison->GetFollowers();
    
    for (uint32 l_I = 0; l_I < l_FollowerCount; ++l_I)
    {
        std::vector<GarrisonFollower>::iterator l_It = std::find_if(l_GarrFollowers.begin(), l_GarrFollowers.end(), [this, l_Followers, l_I](const GarrisonFollower p_Follower) -> bool
        {
            if (p_Follower.DB_ID == l_Followers[l_I])
                return true;

            return false;
        });

        if (l_It == l_GarrFollowers.end())
        {
            /// TODO ERROR follower not found
            return;
        }

        if (l_It->CurrentBuildingID != 0 || l_It->CurrentMissionID != 0)
        {
            /// TODO ERROR follower busy
            return;
        }

        uint32 l_FollowerItemLevel = (l_It->ItemLevelWeapon + l_It->ItemLevelArmor) / 2;

        if (l_FollowerItemLevel)
        {
            /// TODO ERROR follower busy
            return;
        }
    }

}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

void WorldSession::SendGarrisonOpenArchitect(uint64 p_CreatureGUID)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_ARCHITECT, 18);

    l_Data.appendPackGUID(p_CreatureGUID);

    SendPacket(&l_Data);
}
void WorldSession::SendGarrisonOpenMissionNpc(uint64 p_CreatureGUID)
{
    Garrison * l_Garrison = m_Player->GetGarrison();

    if (!l_Garrison)
        return;

    WorldPacket l_Data(SMSG_GARRISON_OPEN_MISSION_NPC, 18);
    l_Data << uint32(1);
    l_Data << uint32(0);
    l_Data.WriteBit(false);
    l_Data.FlushBits();

    SendPacket(&l_Data);
}