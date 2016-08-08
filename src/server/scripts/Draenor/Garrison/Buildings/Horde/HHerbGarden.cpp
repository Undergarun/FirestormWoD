////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "HHerbGarden.hpp"
#include "GarrisonMgr.hpp"
#include "../../GarrisonScriptData.hpp"
#include "../../Sites/GarrisonSiteBase.hpp"
#include "../Alliance/AHerbGarden.hpp"

namespace MS { namespace Garrison
{

    std::vector<uint32> g_HordeHerbsGobsEntry
    {
        235376, ///< HerbSpawnType::Frostweed
        235389, ///< HerbSpawnType::Starflower
        235387, ///< HerbSpawnType::Fireweed
        235391, ///< HerbSpawnType::TaladorOrchid
        235388, ///< HerbSpawnType::GrogrondFlytrap
        235390  ///< HerbSpawnType::NagrandArrowbloom
    };

    /// Farm plot positions
    std::vector<GatheringPlotInfos> g_HordeHerbGardenFlowerPlot
    {
        /// Level 1
        { 1, -32.8207f, 35.7624f, 3.0563f, 3.1370f },
        { 1, -30.1080f, 34.6427f, 2.8868f, 5.8427f },
        { 1, -36.1460f, 30.8179f, 2.8871f, 2.3398f },
        { 1, -34.6058f, 28.8384f, 2.8872f, 3.6122f },
        { 1, -42.7375f, 35.9599f, 2.8865f, 2.7247f },
        { 1, -41.5901f, 37.4636f, 3.3671f, 0.8404f },

        /// Level 2
        { 2, -30.0838f, 34.7960f, 2.8899f, 1.1606f },
        { 2, -32.8646f, 35.8498f, 3.2592f, 1.3884f },
        { 2, -24.4076f, 35.8484f, 2.8864f, 3.7210f },
        { 2, -25.5345f, 37.3289f, 3.4044f, 3.9213f },
        { 2, -41.6903f, 37.3835f, 2.8861f, 5.5353f },
        { 2, -42.6684f, 36.1353f, 2.8881f, 5.7199f },
        { 2, -34.4991f, 28.6406f, 2.8856f, 4.0548f },
        { 2, -36.3694f, 31.0295f, 2.8902f, 3.3637f },
        { 2, -38.4641f, 25.0125f, 2.8900f, 0.7876f },
        { 2, -37.0768f, 23.9160f, 2.8894f, 0.9171f },

        /// Level 3
        { 3, -22.0056f, 32.6219f, 2.9004f, 3.7460f },
        { 3, -23.2427f, 34.4142f, 2.9112f, 3.8905f },
        { 3, -24.9024f, 36.7312f, 2.8858f, 3.8669f },
        { 3, -26.2620f, 38.1862f, 3.1133f, 3.8983f },
        { 3, -30.1770f, 34.8043f, 2.8866f, 1.0591f },
        { 3, -32.8142f, 35.6323f, 2.8878f, 1.4597f },
        { 3, -34.6969f, 35.6271f, 3.7698f, 1.5736f },
        { 3, -36.5496f, 32.8353f, 2.8866f, 3.2975f },
        { 3, -36.2112f, 30.9791f, 2.8891f, 3.4389f },
        { 3, -34.4445f, 28.8961f, 2.8870f, 4.0279f },
        { 3, -38.4482f, 24.9980f, 2.8897f, 0.8785f },
        { 3, -36.8310f, 23.7710f, 3.1038f, 0.9217f },
        { 3, -42.9678f, 35.9658f, 2.8863f, 5.7558f },
        { 3, -41.7365f, 37.5816f, 2.8879f, 5.6537f },
        { 3, -26.8490f, 21.7487f, 2.8885f, 1.2673f }
    };

    std::vector<SequencePosition> g_HordeTreeFruitsPosition =
    {
        { -29.4834f, 30.5917f, 2.5401f, 2.9513f },
        { -30.8693f, 29.7693f, 2.5401f, 2.3898f },
        { -32.2983f, 29.7496f, 2.5401f, 1.6201f },
        { -29.8297f, 29.5411f, 2.5410f, 2.4408f },
        { -28.0837f, 29.5566f, 2.5410f, 2.7204f },
        { -29.6035f, 28.4041f, 2.5410f, 2.0097f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 85783 - Nali Softsoil                                              ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_NaliSoftOilAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* /*p_This*/, Creature* /*p_Me*/)
        {

        };

        char ScriptName[] = "npc_NaliSoftOil_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Creature : AI Owner
    npc_TarnonAI::npc_TarnonAI(Creature* p_Creature)
        : GatheringBuildingMaster(p_Creature)
    {

    }

    void npc_TarnonAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(NPCs::NpcFrostwallNibbler, 200.0f);
    }

    /// When the PlotInstance ID is set
    /// @p_BuildingID : Set plot instance ID
    void npc_TarnonAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (!l_Owner)
            return;

        if (!l_Owner->IsQuestRewarded(Quests::Horde_ClearingTheGarden))
        {
            /// Spawn quest NPCs
            static const SequencePosition s_Position[] =
            {
                { -8.4535f,  25.4862f,  2.9582f, 3.1754f },
                { -26.1947f, -1.4902f, -0.4178f, 3.1754f },
                { -26.9370f, -2.3312f, -0.5978f, 3.1754f },
                { -38.5197f, -8.5257f, -0.2278f, 0.8781f },
                { -36.0800f, 20.3173f,  2.7462f, 0.8781f },
                { -14.0748f, 37.7829f,  3.3492f, 0.8781f },
                { -36.7022f, 27.4449f,  2.5852f, 0.8781f },
                { -19.8449f, 22.4386f,  2.5632f, 0.8781f },
                { -10.5182f,  7.9501f,  0.7062f, 0.8781f },
                { -58.3114f, 22.8912f,  2.9522f, 0.8781f },
                { -58.4457f, 23.5773f,  3.1832f, 0.8781f },
                { -16.9457f, -9.7911f, -0.0078f, 0.8781f },
                { -30.6638f, 44.8115f,  2.7582f, 0.8781f },
                { -37.2964f,  0.5041f,  0.0732f, 0.8781f },
                { -26.7461f, 39.5790f,  2.5634f, 0.8781f },
                { -26.3856f, 14.5686f,  2.8292f, 0.8781f },
                { -38.7458f, 48.2973f,  2.7662f, 0.8781f },
                { -49.5151f, 29.0712f,  3.0252f, 0.8781f }
            };
            static const uint32 s_PositionCount = sizeof(s_Position) / sizeof(s_Position[0]);

            for (uint32 l_I = 0; l_I < s_PositionCount; ++l_I)
            {
                SequencePosition const& l_CurrentPosition = s_Position[l_I];

                if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcFrostwallNibbler, l_CurrentPosition.X, l_CurrentPosition.Y, l_CurrentPosition.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                    l_Creature->GetMotionMaster()->MoveRandom(10.0f);
            }
        }
        
        if (Manager* l_GarrisonMgr = l_Owner->GetGarrison())
        {
            if (l_GarrisonMgr->HasRequiredFollowerAssignedAbility(p_PlotInstanceID))
            {
                GarrisonFollower* l_Follower = l_GarrisonMgr->GetAssignedFollower(p_PlotInstanceID);

                if (l_Follower == nullptr)
                    return;

                GarrFollowerEntry const* l_GarrFollEntry = l_Follower->GetEntry();

                if (l_GarrFollEntry == nullptr)
                    return;

                switch (GetBuildingID())
                {
                    case Building::ID::HerbGarden_HerbGarden_Level2:
                    case Building::ID::HerbGarden_HerbGarden_Level3:
                        if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[0], -10.5386f, 2.3247f, 0.0986f, 3.12f, TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            if (CreatureTemplate const* l_FollowerTemplate = sObjectMgr->GetCreatureTemplate(l_GarrFollEntry->CreatureID[0]))
                                l_Creature->SetDisplayId(l_FollowerTemplate->Modelid1);

                            l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                            l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                            AddSummonGUID(l_Creature->GetGUID());
                        }
                        break;
                    default:
                        break;
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On player quest rewarded
    /// @p_Player : Subject
    /// @p_Quest  : Rewarded quest template
    /// @p_Opt    : Quest reward option (eg: Loot choose)
    void npc_TarnonAI::sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 /*p_Opt*/)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (l_Owner != p_Player)
            return;

        if (p_Quest->GetQuestId() == Quests::Horde_ClearingTheGarden)
        {
            static const Position l_Pos = *p_Player;

            std::list<Creature*> l_CreatureList;
            me->GetCreatureListWithEntryInGrid(l_CreatureList, NPCs::NpcFrostwallNibbler, 65.0f);

            for (Creature* l_Creature : l_CreatureList)
                l_Creature->DespawnOrUnsummon();

            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

            if (l_BuildingEntry)
                l_Owner->PlayStandaloneScene(l_Owner->GetGarrison()->GetGarrisonFactionIndex() ? l_BuildingEntry->HordeActivationScenePackageID : l_BuildingEntry->AllianceActivationScenePackageID, 0, l_Pos);

            l_Owner->GetGarrison()->ActivateBuilding(GetPlotInstanceID());
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Select game object entry for a fresh gathering spawn
    /// @p_MiscData : Misc data
    uint32 npc_TarnonAI::SelectGameObjectEntryForGatheringSpawn(uint32 p_MiscData)
    {
        if (p_MiscData == HerbSpawnType::Random)
            return g_HordeHerbsGobsEntry[urand(0, HerbSpawnType::Random - 1)];

        return g_HordeHerbsGobsEntry[(p_MiscData >= HerbSpawnType::Random) ? urand(0, HerbSpawnType::Random - 1) : p_MiscData];
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// @p_Action : Action ID
    void npc_TarnonAI::DoAction(int32 p_Action)
    {
        ResetGatheringPlots(p_Action);
    }

    /// Constructor
    npc_Tarnon::npc_Tarnon()
        : CreatureScript("npc_Tarnon_Garrison")
    {

    }

    bool npc_Tarnon::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Horde_ClearingTheGarden);

        if (!l_Quest)
            return true;

        if (p_Player->GetQuestStatus(Quests::Horde_ClearingTheGarden) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Horde_ClearingTheGarden) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());
        else
            p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());

        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Tarnon::GetAI(Creature* p_Creature) const
    {
        return new npc_TarnonAI(p_Creature);
    }
}   ///< namespace Garrison
}   ///< namespace MS
#endif
