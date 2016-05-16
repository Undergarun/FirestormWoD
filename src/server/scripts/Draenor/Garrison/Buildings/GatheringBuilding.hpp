////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#ifndef GARRISON_GATHERING_BUILDING_HPP_GARRISON
#define GARRISON_GATHERING_BUILDING_HPP_GARRISON

#include "../GarrisonScriptData.hpp"
#include "../GarrisonNPC.hpp"
#include "GarrisonMgr.hpp"
#include "SharedDefines.h"
#include "../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    namespace GatheringSpawnState
    {
        enum Type : int32
        {
            Gathered = 0,
            NotSpawned = -1
        };
    }

    /// Flower plot location informations
    struct GatheringPlotInfos
    {
        int BuildingLevel;
        float X, Y, Z;
        float O;
    };

    /// Simple profession building NPC script
    /// @t_ScriptName   : Script name
    /// @t_Skill        : Building target skill type
    /// @t_QuestID      : Building work order quest
    template<std::vector<GatheringPlotInfos>* t_Plots>
    class GatheringBuildingMaster : public GarrisonNPCAI
    {
        public:
            /// Constructor
            /// @p_Creature : Creature instance
            GatheringBuildingMaster(Creature * p_Creature)
                : GarrisonNPCAI(p_Creature), m_GatheringPlotsAreSpawned(false), m_NextUpdate(1 * IN_MILLISECONDS)
            {

            }

            /// On AI Update
            /// @p_Diff : Time since last update
            virtual void UpdateAI(const uint32 p_Diff) override
            {
                m_NextUpdate -= p_Diff;

                if (m_NextUpdate < 0)
                {
                    UpdateGatheringPlots();
                    m_NextUpdate = MINUTE * IN_MILLISECONDS;
                }
            }

            /// Select game object entry for a fresh gathering spawn
            /// @p_MiscData : Misc data
            virtual uint32 SelectGameObjectEntryForGatheringSpawn(uint32 p_MiscData) = 0;

            /// Set gathering misc data
            /// @p_MiscID : Misc data value
            void SetGatheringMiscData(uint32 p_MiscID)
            {
                Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

                if (!l_GarrisonSite)
                    return;

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

                if (!l_BuildingEntry)
                    return;

                Player* l_Owner = l_GarrisonSite->GetOwner();

                if (!l_Owner)
                    return;

                Tokenizer l_Datas(l_Owner->GetGarrison()->GetBuildingGatheringData(GetPlotInstanceID()), ' ');

                if (l_Datas.size() < 4 || l_Owner->GetGarrison()->GetBuildingGatheringData(GetPlotInstanceID()) == "")
                    return;

                std::ostringstream l_Str;
                l_Str << p_MiscID;

                for (uint32 l_I = 1; l_I < l_Datas.size(); ++l_I)
                    l_Str << " " << l_Datas[l_I];

                l_Owner->GetGarrison()->SetBuildingGatheringData(GetPlotInstanceID(), l_Str.str());
            }

            /// Get gathering misc data
            uint32 GetGatheringMiscData()
            {
                Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

                if (!l_GarrisonSite)
                    return 0;

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

                if (!l_BuildingEntry)
                    return 0;

                Player* l_Owner = l_GarrisonSite->GetOwner();

                if (!l_Owner)
                    return 0;

                Tokenizer l_Datas(l_Owner->GetGarrison()->GetBuildingGatheringData(GetPlotInstanceID()), ' ');

                if (l_Datas.size() < 4 || l_Owner->GetGarrison()->GetBuildingGatheringData(GetPlotInstanceID()) == "")
                    return 0;

                return atol(l_Datas[0]);
            }

        protected:
            /// Init gathering plots
            /// @p_MiscData : Misc data
            void InitGatheringPlots(uint32 p_MiscData)
            {
                Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

                if (!l_GarrisonSite)
                    return;

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

                if (!l_BuildingEntry)
                    return;

                Player* l_Owner = l_GarrisonSite->GetOwner();

                if (!l_Owner)
                    return;

                /// Init spawning system
                std::ostringstream l_Str;
                l_Str << p_MiscData << " " << 0 << " " << 0 << " " << l_BuildingEntry->Level;

                for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                {
                    if (t_Plots->at(l_I).BuildingLevel != l_BuildingEntry->Level)
                        continue;

                    l_Str << " " << GatheringSpawnState::NotSpawned;
                }

                l_Owner->GetGarrison()->SetBuildingGatheringData(GetPlotInstanceID(), l_Str.str());
            }
            /// Update gathering plots
            void UpdateGatheringPlots(int p_Recursion = 0)
            {
                if (p_Recursion >= 10)
                    return;
                    
                Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

                if (!l_GarrisonSite)
                    return;

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

                if (!l_BuildingEntry)
                    return;

                Player* l_Owner = l_GarrisonSite->GetOwner();

                if (!l_Owner)
                    return;

                MS::Garrison::Manager* l_Manager = l_Owner->GetGarrison();
                if (!l_Manager)
                    return;

                Tokenizer l_Datas(l_Manager->GetBuildingGatheringData(GetPlotInstanceID()), ' ');

                if (l_Datas.size() < 4 || l_Manager->GetBuildingGatheringData(GetPlotInstanceID()) == "" || p_Recursion >= 5)
                {
                    InitGatheringPlots(0);
                    UpdateGatheringPlots(p_Recursion + 1);
                    return;
                }
                
                uint8 l_Index = 0;
                uint32 l_MiscData           = atol(l_Datas[l_Index++]);
                uint32 l_PrevSpawnTimeStamp = atol(l_Datas[l_Index++]);
                uint32 l_NextSpawnTimeStamp = atol(l_Datas[l_Index++]);
                uint32 l_BuildingLevel      = atol(l_Datas[l_Index++]);

                auto l_UpdateRecursive = [this, &l_Owner, &l_MiscData, &l_PrevSpawnTimeStamp, &l_NextSpawnTimeStamp, &l_BuildingEntry, p_Recursion]() -> void
                {
                    std::ostringstream l_Str;
                    l_Str << l_MiscData << " " << l_PrevSpawnTimeStamp << " " << l_NextSpawnTimeStamp << " " << l_BuildingEntry->Level;

                    for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                    {
                        if (t_Plots->at(l_I).BuildingLevel != l_BuildingEntry->Level)
                            continue;

                        l_Str << " " << SelectGameObjectEntryForGatheringSpawn(l_MiscData);
                    }

                    l_Owner->GetGarrison()->SetBuildingGatheringData(GetPlotInstanceID(), l_Str.str());
                    UpdateGatheringPlots(p_Recursion + 1);
                };

                if (l_BuildingLevel == l_BuildingEntry->Level)
                {
                    uint32 l_NextDailyQuestReset = sWorld->GetNextDailyQuestsResetTime();

                    if ((l_NextDailyQuestReset - l_PrevSpawnTimeStamp) > DAY || (l_PrevSpawnTimeStamp == 0 && l_NextSpawnTimeStamp == 0))
                    {
                        l_PrevSpawnTimeStamp = l_NextDailyQuestReset - DAY;
                        l_NextSpawnTimeStamp = l_NextDailyQuestReset;

                        l_UpdateRecursive();
                    }
                    else if (l_PrevSpawnTimeStamp < time(nullptr) && l_NextSpawnTimeStamp == l_NextDailyQuestReset && !m_GatheringPlotsAreSpawned)
                    {
                        for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                        {
                            GatheringPlotInfos& l_Plot = t_Plots->at(l_I);
                            if (l_Plot.BuildingLevel != l_BuildingEntry->Level)
                                continue;

                            uint32 l_StateOrGobEntry = atol(l_Datas[l_Index++]);
                    
                            if (l_StateOrGobEntry != 0)
                                SummonRelativeGameObject(l_StateOrGobEntry, l_Plot.X, l_Plot.Y, l_Plot.Z, l_Plot.O);
                        }

                        m_GatheringPlotsAreSpawned = true;
                    }
                }
                /// Update data
                else
                    l_UpdateRecursive();
            }

            void SetGUID(uint64 p_GUID, int32 p_ID) override
            {
                if (p_ID != CreatureAIDataIDs::GatheredPos)
                    return;

                Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

                if (!l_GarrisonSite)
                    return;

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

                if (!l_BuildingEntry)
                    return;

                Player* l_Owner = l_GarrisonSite->GetOwner();

                if (!l_Owner)
                    return;

                Tokenizer l_Datas(l_Owner->GetGarrison()->GetBuildingGatheringData(GetPlotInstanceID()), ' ');

                if (l_Datas.size() < 4 || l_Owner->GetGarrison()->GetBuildingGatheringData(GetPlotInstanceID()) == "")
                    return;

                uint8 l_Index = 0;
                uint32 l_MiscData = atol(l_Datas[l_Index++]);
                uint32 l_PrevSpawnTimeStamp = atol(l_Datas[l_Index++]);
                uint32 l_NextSpawnTimeStamp = atol(l_Datas[l_Index++]);
                uint32 l_BuildingLevel = atol(l_Datas[l_Index++]);

                float l_UnpackedX, l_UnpackedY, l_UnpackedZ = 0;

                *(uint32*)(&l_UnpackedX) = (p_GUID & 0x00000000FFFFFFFF) >> 0;
                *(uint32*)(&l_UnpackedY) = (p_GUID & 0xFFFFFFFF00000000) >> 32;

                Position l_Position
                {
                    l_UnpackedX, l_UnpackedY, l_UnpackedZ
                };

                std::ostringstream l_Str;
                l_Str << l_MiscData << " " << l_PrevSpawnTimeStamp << " " << l_NextSpawnTimeStamp << " " << l_BuildingEntry->Level;

                for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                {
                    GatheringPlotInfos& l_Plot = t_Plots->at(l_I);
                    if (l_Plot.BuildingLevel != l_BuildingEntry->Level)
                        continue;

                    uint32 l_StateOrGobEntry = atol(l_Datas[l_Index++]);

                    float l_PlotX = l_Plot.X;
                    float l_PlotY = l_Plot.Y;
                    float l_PlotZ = l_Plot.Z;

                    TransformCoord(l_PlotX, l_PlotY, l_PlotZ);

                    if (l_Position.GetExactDist2d(l_PlotX, l_PlotY) < 0.1)
                        l_Str << " " << GatheringSpawnState::Gathered;
                    else
                        l_Str << " " << l_StateOrGobEntry;
                }

                l_Owner->GetGarrison()->SetBuildingGatheringData(GetPlotInstanceID(), l_Str.str());
            }

        private:
            bool m_GatheringPlotsAreSpawned;    ///< Is gathering plots spawned
            int32 m_NextUpdate;                 ///< Last update

    };

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Simple profession building NPC script
    /// @t_ScriptName   : Script name
    /// @t_Skill        : Building target skill type
    /// @t_QuestID      : Building work order quest
    /// @t_SetupLevel1  : Function pour initializing sequence for level 1 building
    /// @t_SetupLevel2  : Function pour initializing sequence for level 2 building
    /// @t_SetupLevel3  : Function pour initializing sequence for level 3 building
    template<char const* t_ScriptName, uint32 t_QuestID, InitSequenceFunction* t_SetupLevel1, InitSequenceFunction* t_SetupLevel2, InitSequenceFunction* t_SetupLevel3>
    class GatheringBuilding_WorkOrderNPC : public SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>
    {
        public:
            /// Constructor
            GatheringBuilding_WorkOrderNPC()
                : SimpleSequenceCosmeticScript<t_ScriptName, t_SetupLevel1, t_SetupLevel2, t_SetupLevel3>()
            {

            }

            /// Called when a player opens a gossip dialog with the GameObject.
            /// @p_Player     : Source player instance
            /// @p_Creature   : Target GameObject instance
            virtual bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
            {
                if (p_Player->IsQuestRewarded(t_QuestID))
                    p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

                return true;
            }

            /// Called when a player selects a gossip item in the creature's gossip menu.
            /// @p_Player   : Source player instance
            /// @p_Creature : Target creature instance
            /// @p_Sender   : Sender menu
            /// @p_Action   : Action
            virtual bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/) override
            {
                if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName() && p_Player->IsQuestRewarded(t_QuestID))
                    static_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

                return true;
            }

    };

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GATHERING_BUILDING_HPP_GARRISON
