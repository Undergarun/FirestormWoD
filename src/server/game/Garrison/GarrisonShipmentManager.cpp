////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "Common.h"
#include "GarrisonMgr.hpp"
#include "GarrisonShipmentManager.hpp"
#include "Player.h"
#include "DatabaseEnv.h"
#include "ObjectMgr.h"
#include "ObjectAccessor.h"

namespace MS { namespace Garrison
{
    /// Constructor
    ShipmentManager::ShipmentManager()
    {

    }

    /// Destructor
    ShipmentManager::~ShipmentManager()
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Init shipment manager
    void ShipmentManager::Init()
    {
        std::map<uint32, std::set<uint32>> l_ShipmentPerBuildingType;

        for (uint32 l_I = 0; l_I < sCharShipmentStore.GetNumRows(); ++l_I)
        {
            CharShipmentEntry const* l_Entry = sCharShipmentStore.LookupEntry(l_I);

            if (!l_Entry || l_Entry->Flags & CharShipmentFlags::Quest || l_Entry->Flags & CharShipmentFlags::Internal)
                continue;

            /// Blizzard debug
            if (l_Entry->SpellID == 156944)
                continue;

            CharShipmentContainerEntry const* l_ContainerEntry = sCharShipmentContainerStore.LookupEntry(l_Entry->ShipmentContainerID);

            if (!l_ContainerEntry || (std::string(l_ContainerEntry->Name->Get(sWorld->GetDefaultDbcLocale())).empty() && std::string(l_ContainerEntry->Description->Get(sWorld->GetDefaultDbcLocale())).empty()))
                continue;

            l_ShipmentPerBuildingType[l_ContainerEntry->BuildingType].emplace(l_Entry->ID);
        }

        for (auto l_Pair : l_ShipmentPerBuildingType)
        {
            if (l_Pair.second.size() == 1)
                m_ShipmentPerBuildingType[l_Pair.first] = *l_Pair.second.begin();
            else if (l_Pair.second.size() > 1)
            {
                std::vector<uint32> l_Candidates;
                std::copy(l_Pair.second.begin(), l_Pair.second.end(), std::back_inserter(l_Candidates));

                /// @TODO Some conflict can appear if queue contains more than 1 ID Range
                ///auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                ///std::shuffle(l_Candidates.begin(), l_Candidates.end(), std::default_random_engine(l_Seed));

                m_ShipmentPerBuildingType[l_Pair.first] = l_Candidates.at(0);
            }
        }

        for (uint32 l_I = 0; l_I < sCharShipmentStore.GetNumRows(); ++l_I)
        {
            CharShipmentEntry const* l_Entry = sCharShipmentStore.LookupEntry(l_I);

            if (!l_Entry || l_Entry->Flags != CharShipmentFlags::Quest)
                continue;

            /// Blizzard debug
            if (l_Entry->SpellID == 156944)
                continue;

            CharShipmentContainerEntry const* l_ContainerEntry = sCharShipmentContainerStore.LookupEntry(l_Entry->ShipmentContainerID);

            if (!l_ContainerEntry || (std::string(l_ContainerEntry->Name->Get(sWorld->GetDefaultDbcLocale())).empty() && std::string(l_ContainerEntry->Description->Get(sWorld->GetDefaultDbcLocale())).empty()))
                continue;

            /// Some conflict can appear if queue contains more than 1 ID Range
            if (m_QuestShipmentPerBuildingType[l_ContainerEntry->BuildingType] != 0)
                continue;

            m_QuestShipmentPerBuildingType[l_ContainerEntry->BuildingType] = l_Entry->ID;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get shipment ID for specific building & player
    uint32 ShipmentManager::GetShipmentIDForBuilding(uint32 p_BuildingID, Player* p_Target, bool p_ForStartWorkOrder)
    {
        uint32 l_BuildingType = 0;

        const GarrBuildingEntry* l_Entry = sGarrBuildingStore.LookupEntry(p_BuildingID);

        if (!l_Entry)
        {
            sLog->outError(LOG_FILTER_WORLDSERVER, "MS::Garrison::ShipmentManager::GetShipmentIDForBuilding() Building ID invalid");
            return 0;
        }

        l_BuildingType = l_Entry->Type;

        if (l_BuildingType == BuildingType::TradingPost)
            return p_Target->GetCharacterWorldStateValue(GarrisonTradingPostDailyRandomShipment);
        else if (!p_ForStartWorkOrder)
            return m_ShipmentPerBuildingType[l_BuildingType];
        else
        {
            const CharShipmentEntry* l_ShipmentEntry = sCharShipmentStore.LookupEntry(m_QuestShipmentPerBuildingType[l_BuildingType]);

            if (!l_ShipmentEntry)
                return m_ShipmentPerBuildingType[l_BuildingType];

            uint32 l_QuestItem = l_ShipmentEntry->ResultItemID;

            std::vector<GarrisonWorkOrder> l_WorkOrders = p_Target->GetGarrison()->GetWorkOrders();

            for (auto l_QuestPair : sObjectMgr->QuestForItem[l_QuestItem])
            {
                if (p_Target->HasQuest(l_QuestPair.first))
                {
                    Quest const* l_Quest = sObjectMgr->GetQuestTemplate(l_QuestPair.first);

                    if (!l_Quest)
                        continue;

                    if (l_Quest->GetQuestObjectiveXIndex(l_QuestPair.second))
                    {
                        QuestObjective const* l_Objective = l_Quest->GetQuestObjectiveXIndex(l_QuestPair.second);

                        uint32 l_Count = (uint32)std::count_if(l_WorkOrders.begin(), l_WorkOrders.end(), [this, l_BuildingType](const GarrisonWorkOrder & p_Order) -> bool
                        {
                            return p_Order.ShipmentID == m_QuestShipmentPerBuildingType[l_BuildingType];
                        });

                        if (l_Objective->ObjectID == l_QuestItem && !p_Target->HasItemCount(l_QuestItem, 1, false) && !l_Count) ///< Comparison of integers of different signs: 'const uint32' (aka 'const unsigned int') and 'const int32' (aka 'const int')
                            return m_QuestShipmentPerBuildingType[l_BuildingType];
                    }
                }
            }

            return m_ShipmentPerBuildingType[l_BuildingType];
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS
#endif
