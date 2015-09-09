////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
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
            virtual void UpdateAI(const uint32 p_Diff)
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
                l_Str << p_MiscData << " " << 0 << " " << 0 << " " << l_BuildingEntry->BuildingLevel;

                for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                {
                    if (t_Plots->at(l_I).BuildingLevel != l_BuildingEntry->BuildingLevel)
                        continue;

                    l_Str << " " << GatheringSpawnState::NotSpawned;
                }

                l_Owner->GetGarrison()->SetBuildingGatheringData(GetPlotInstanceID(), l_Str.str());
            }
            /// Update gathering plots
            void UpdateGatheringPlots()
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
                {
                    InitGatheringPlots(0);
                    UpdateGatheringPlots();
                    return;
                }
                
                uint8 l_Index = 0;
                uint32 l_MiscData           = atol(l_Datas[l_Index++]);
                uint32 l_PrevSpawnTimeStamp = atol(l_Datas[l_Index++]);
                uint32 l_NextSpawnTimeStamp = atol(l_Datas[l_Index++]);
                uint32 l_BuildingLevel      = atol(l_Datas[l_Index++]);

                auto l_UpdateRecursive = [this, &l_Owner, &l_MiscData, &l_PrevSpawnTimeStamp, &l_NextSpawnTimeStamp, &l_BuildingEntry]() -> void
                {
                    std::ostringstream l_Str;
                    l_Str << l_MiscData << " " << l_PrevSpawnTimeStamp << " " << l_NextSpawnTimeStamp << " " << l_BuildingEntry->BuildingLevel;

                    for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                    {
                        if (t_Plots->at(l_I).BuildingLevel != l_BuildingEntry->BuildingLevel)
                            continue;

                        l_Str << " " << SelectGameObjectEntryForGatheringSpawn(l_MiscData);
                    }

                    l_Owner->GetGarrison()->SetBuildingGatheringData(GetPlotInstanceID(), l_Str.str());
                    UpdateGatheringPlots();
                };

                if (l_BuildingLevel == l_BuildingEntry->BuildingLevel)
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
                            if (l_Plot.BuildingLevel != l_BuildingEntry->BuildingLevel)
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

                float l_UnpackedX = 0;
                float l_UnpackedY = 0;
                float l_UnpackedZ = 0;

                *(uint32*)(&l_UnpackedX) = (p_GUID & 0x00000000FFFFFFFF) >> 0;
                *(uint32*)(&l_UnpackedY) = (p_GUID & 0xFFFFFFFF00000000) >> 32;

                Position l_Position
                {
                    l_UnpackedX, l_UnpackedY, l_UnpackedZ
                };

                std::ostringstream l_Str;
                l_Str << l_MiscData << " " << l_PrevSpawnTimeStamp << " " << l_NextSpawnTimeStamp << " " << l_BuildingEntry->BuildingLevel;

                for (uint32 l_I = 0; l_I < t_Plots->size(); ++l_I)
                {
                    GatheringPlotInfos& l_Plot = t_Plots->at(l_I);
                    if (l_Plot.BuildingLevel != l_BuildingEntry->BuildingLevel)
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

}   ///< namespace Garrison
}   ///< namespace MS

#endif  ///< GARRISON_GATHERING_BUILDING_HPP_GARRISON