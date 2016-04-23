////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonSiteBase.hpp"
#include "Group.h"

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    /// @p_Map : Target map
    GarrisonSiteBase::GarrisonSiteBase(Map* p_Map)
        : InstanceScript(p_Map), m_OwnerGUID(0)
    {

    }
    /// Destructor
    GarrisonSiteBase::~GarrisonSiteBase()
    {
        if (Player * l_Player = HashMapHolder<Player>::Find(m_OwnerGUID))
        {
            if (l_Player->GetGarrison())
                l_Player->GetGarrison()->_SetGarrisonScript(nullptr);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When a creature is created
    /// @p_Creature : Created creature instance
    void GarrisonSiteBase::OnCreatureCreate(Creature * p_Creature)
    {
        if (!p_Creature)
            return;

        uint32 l_CreatureEntry = p_Creature->GetEntry();

        if (m_CreaturesPerEntry.find(l_CreatureEntry) == m_CreaturesPerEntry.end())
            m_CreaturesPerEntry[l_CreatureEntry] = std::set<uint64>();

        std::set<uint64> * l_GUIDs = &m_CreaturesPerEntry[l_CreatureEntry];

        if (std::find(l_GUIDs->begin(), l_GUIDs->end(), p_Creature->GetGUID()) == l_GUIDs->end())
            l_GUIDs->emplace(p_Creature->GetGUID());
    }
    /// When a game object is created
    /// @p_Gameobject : Created game object instance
    void GarrisonSiteBase::OnGameObjectCreate(GameObject * p_Gameobject)
    {
        if (!p_Gameobject)
            return;

        uint32 l_GameObjectEntry = p_Gameobject->GetEntry();

        if (m_GameObjectsPerEntry.find(l_GameObjectEntry) == m_GameObjectsPerEntry.end())
            m_GameObjectsPerEntry[l_GameObjectEntry] = std::set<uint64>();

        std::set<uint64> * l_GUIDs = &m_GameObjectsPerEntry[l_GameObjectEntry];

        if (std::find(l_GUIDs->begin(), l_GUIDs->end(), p_Gameobject->GetGUID()) == l_GUIDs->end())
            l_GUIDs->emplace(p_Gameobject->GetGUID());
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get UInt32 value by ID
    /// @p_ID : Value ID
    uint32 GarrisonSiteBase::GetData(uint32 p_ID)
    {
        return m_Data32[p_ID];
    }
    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void GarrisonSiteBase::SetData(uint32 p_ID, uint32 p_Value)
    {
        m_Data32[p_ID] = p_Value;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player successfully enters the instance.
    /// @p_Player : Entered player instance
    void GarrisonSiteBase::OnPlayerEnter(Player * p_Player)
    {
        if (!p_Player->IsInWorld())
            return;

        p_Player->GetMap()->SetObjectVisibility(400.0f);

        InstanceScript::OnPlayerEnter(p_Player);

        m_Players.emplace(p_Player->GetGUID());

        Group * l_Group = p_Player->GetGroup();
        if (!l_Group || l_Group->IsLeader(p_Player->GetGUID()))
        {
            if (p_Player->GetGarrison())
            {
                p_Player->GetGarrison()->_SetGarrisonScript(this);

                m_OwnerGUID = p_Player->GetGUID();
                OnOwnerEnter(p_Player);
                OnOwnerLevelChange(p_Player->getLevel());

                if (p_Player->GetTeamId() == TEAM_ALLIANCE)
                {
                    if (p_Player->IsQuestRewarded(Quests::Alliance_ClearingTheGarden) && !p_Player->GetGarrison()->HasBuildingType(BuildingType::Farm)
                        && p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Farm, true).DatabaseID != 0)
                    {
                        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Farm, true).BuildingID);

                        if (l_Building && l_Building->Level <= 1)
                            p_Player->GetGarrison()->ActivateBuilding(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Farm, true).PlotInstanceID);
                    }

                    if (p_Player->IsQuestRewarded(Quests::Alliance_ThingsAreNotGorenOurWay) && !p_Player->GetGarrison()->HasBuildingType(BuildingType::Mine)
                        && p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Mine, true).DatabaseID != 0)
                    {
                        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Mine, true).BuildingID);

                        if (l_Building && l_Building->Level <= 1)
                            p_Player->GetGarrison()->ActivateBuilding(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Mine, true).PlotInstanceID);
                    }

                    if (p_Player->IsQuestRewarded(Quests::Alliance_AnglinInOurGarrison) && !p_Player->GetGarrison()->HasBuildingType(BuildingType::Fishing)
                        && p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Fishing, true).DatabaseID != 0)
                    {
                        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Fishing, true).BuildingID);

                        if (l_Building && l_Building->Level <= 1)
                            p_Player->GetGarrison()->ActivateBuilding(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Fishing, true).PlotInstanceID);
                    }
                }
                else
                {
                    if (p_Player->IsQuestRewarded(Quests::Horde_ClearingTheGarden) && !p_Player->GetGarrison()->HasBuildingType(BuildingType::Farm)
                        && p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Farm, true).DatabaseID != 0)
                    {
                        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Farm, true).BuildingID);

                        if (l_Building && l_Building->Level <= 1)
                            p_Player->GetGarrison()->ActivateBuilding(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Farm, true).PlotInstanceID);
                    }

                    if (p_Player->IsQuestRewarded(Quests::Horde_ThingsAreNotGorenOurWay) && !p_Player->GetGarrison()->HasBuildingType(BuildingType::Mine)
                        && p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Mine, true).DatabaseID != 0)
                    {
                        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Mine, true).BuildingID);

                        if (l_Building && l_Building->Level <= 1)
                            p_Player->GetGarrison()->ActivateBuilding(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Mine, true).PlotInstanceID);
                    }

                    if (p_Player->IsQuestRewarded(Quests::Horde_AnglinInOurGarrison) && !p_Player->GetGarrison()->HasBuildingType(BuildingType::Fishing)
                        && p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Fishing, true).DatabaseID != 0)
                    {
                        GarrBuildingEntry const* l_Building = sGarrBuildingStore.LookupEntry(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Fishing, true).BuildingID);

                        if (l_Building && l_Building->Level <= 1)
                            p_Player->GetGarrison()->ActivateBuilding(p_Player->GetGarrison()->GetBuildingWithType(BuildingType::Fishing, true).PlotInstanceID);
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// When the owner player change level
    /// @p_Level : New owner level
    void GarrisonSiteBase::OnOwnerLevelChange(uint32 p_Level)
    {
        for (uint32 l_I = 0; l_I < (sizeof(gGarrisonLevelUpdateCreatures) / sizeof(gGarrisonLevelUpdateCreatures[0])); ++l_I)
        {
            uint32 const l_Entry = gGarrisonLevelUpdateCreatures[l_I];

            if (m_CreaturesPerEntry.find(l_Entry) == m_CreaturesPerEntry.end())
                continue;

            std::set<uint64>& l_Guids = m_CreaturesPerEntry[l_Entry];

            for (auto l_It = l_Guids.begin(); l_It != l_Guids.end(); ++l_It)
            {
                uint64 l_Guid = *l_It;

                if (Creature * l_Creature = HashMapHolder<Creature>::Find(l_Guid))
                {
                    l_Creature->SetLevel(p_Level);
                    l_Creature->UpdateStatsForLevel();
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get owner player instance
    Player* GarrisonSiteBase::GetOwner()
    {
        return HashMapHolder<Player>::Find(m_OwnerGUID);
    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS
