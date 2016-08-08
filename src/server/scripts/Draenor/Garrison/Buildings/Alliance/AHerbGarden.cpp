////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "AHerbGarden.hpp"
#include "../../GarrisonScriptData.hpp"
#include "GarrisonMgr.hpp"
#include "../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{

    std::vector<uint32> g_AllyHerbsGobsEntry
    {
        235376, ///< HerbSpawnType::Frostweed
        235389, ///< HerbSpawnType::Starflower
        235387, ///< HerbSpawnType::Fireweed
        235391, ///< HerbSpawnType::TaladorOrchid
        235388, ///< HerbSpawnType::GrogrondFlytrap
        235390  ///< HerbSpawnType::NagrandArrowbloom
    };

    /// Farm positions
    std::vector<GatheringPlotInfos> g_AllyHerbGardenFlowerPlot
    {
        /// Level 1
        { 1, 30.1702f, -15.1116f, -1.1398f, 1.8810f },
        { 1, 32.4670f, -14.9524f, -1.1400f, 1.8810f },
        { 1, 40.4576f,   0.8304f, -1.1388f, 1.8810f },
        { 1, 41.5929f,  -0.9677f, -1.0772f, 1.8810f },
        { 1, 40.2687f, -11.3029f, -1.1390f, 1.8810f },
        { 1, 41.8250f,  -9.6902f, -1.1411f, 1.8810f },

        /// Level 2
        { 2, 21.1405f,  -4.6047f, -1.2120f, 6.2356f },
        { 2, 21.9487f,  -2.2275f, -1.2128f, 5.9270f },
        { 2, 35.0556f,   4.2857f, -1.0858f, 4.2148f },
        { 2, 39.7550f,   1.7330f, -1.2128f, 3.9949f },
        { 2, 41.3125f,  -0.0295f, -1.2128f, 3.8653f },
        { 2, 42.3940f,  -2.1894f, -1.2126f, 3.5786f },
        { 2, 41.9220f,  -9.6817f, -1.2126f, 2.4712f },
        { 2, 40.0622f, -11.2737f, -1.2122f, 2.2435f },
        { 2, 32.2555f, -15.0528f, -1.2115f, 1.6034f },
        { 2, 29.9400f, -15.1374f, -1.2128f, 1.6073f },

        /// Level 3
        { 3, 21.3530f,  -4.7566f, -1.2117f, 6.1610f },
        { 3, 22.1297f,  -2.2733f, -1.2120f, 5.9607f },
        { 3, 23.4454f,   5.2645f, -0.3015f, 0.8870f },
        { 3, 22.1110f,   7.2890f, -1.0898f, 0.3333f },
        { 3, 22.2057f,   9.9572f, -1.2116f, 5.9175f },
        { 3, 27.4279f,  12.7284f, -1.0441f, 4.6609f },
        { 3, 29.7084f,  11.4011f, -1.2116f, 3.9037f },
        { 3, 30.7635f,   8.8447f, -1.2125f, 3.2204f },
        { 3, 34.8067f,   4.4143f, -1.1381f, 4.2572f },
        { 3, 39.8085f,   1.6521f, -1.2124f, 3.9430f },
        { 3, 41.4464f,   0.0478f, -1.2115f, 3.7781f },
        { 3, 42.5028f,  -1.9277f, -1.0000f, 3.6328f },
        { 3, 41.7187f,  -9.7533f, -1.2129f, 2.4272f },
        { 3, 40.2174f, -11.4180f, -0.5593f, 2.3722f },
        { 3, 32.3375f, -14.9437f, -1.2110f, 1.5750f },
        { 3, 30.3116f, -14.9523f, -0.0671f, 1.5750f }
    };

    std::vector<SequencePosition> g_AllianceTreeFruitsPosition = 
    {
        { 21.0904f, 14.4230f, -1.5216f, 1.4415f },
        { 18.8171f, 15.0089f, -1.2786f, 0.5972f },
        { 20.0032f, 18.1571f, -1.4552f, 5.9426f },
        { 24.4342f, 18.5986f, -1.5882f, 6.0919f },
        { 23.4849f, 14.0583f, -1.5540f, 1.5978f },
        { 26.3669f, 16.8630f, -1.4604f, 1.9709f }
    };

    //////////////////////////////////////////////////////////////////////////
    /// 85514 - Olly Nimkip                                                ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_OllyNimkipAIData
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

        char ScriptName[] = "npc_OllyNimkip_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_Creature : AI Owner
    npc_NaronBloomthistleAI::npc_NaronBloomthistleAI(Creature * p_Creature)
        : GatheringBuildingMaster(p_Creature)
    {
    }

    void npc_NaronBloomthistleAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea(NPCs::LunarfallRaccoon, 200.0f);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_NaronBloomthistle::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Alliance_ClearingTheGarden);

        if (!l_Quest)
            return true;

        if (p_Player->GetQuestStatus(Quests::Alliance_ClearingTheGarden) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Alliance_ClearingTheGarden) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());
        else
            p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());

        return true;
    }

    /// When the PlotInstance ID is set
    /// @p_BuildingID : Set plot instance ID
    void npc_NaronBloomthistleAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (!l_Owner)
            return;

        if (!l_Owner->IsQuestRewarded(Quests::Alliance_ClearingTheGarden))
        {
            /// Spawn quest NPCs
            static const SequencePosition s_Position[] =
            {
                { 15.8652f, -20.7036f,  0.1359f, 0.9460f },
                { 39.7740f,  -7.5587f, -1.4195f, 0.9460f },
                { 12.1007f,  27.7721f,  5.8273f, 0.9460f },
                { 39.6979f,  31.0361f, -0.8692f, 0.9460f },
                { 34.5776f,  10.7333f, -1.4195f, 0.9460f },
                { 33.8607f, -22.3020f, -1.5758f, 0.9460f },
                { 31.1975f, -11.2975f, -1.4195f, 0.9460f },
                { 22.3715f,  -4.6437f, -1.4195f, 0.9460f },
                { 50.0809f,  -5.9267f,  0.6608f, 0.9460f },
                { 14.6594f,   6.7840f, -1.0020f, 0.9460f },
                { 32.5566f,   1.1516f, -1.4195f, 0.9460f },
                { 30.5110f,  19.0472f, -1.4195f, 0.9460f },
                { 40.9513f,   0.7158f, -1.4195f, 0.9460f },
                { 54.4107f,   4.5363f, -1.7876f, 0.9460f },
                { 38.0261f,  33.8734f, -0.8520f, 0.9460f },
                { 44.1797f,  10.6116f, -1.4195f, 0.9460f }
            };
            static const uint32 s_PositionCount = sizeof(s_Position) / sizeof(s_Position[0]);

            std::list<Creature*> l_CreatureList;
            me->GetCreatureListWithEntryInGrid(l_CreatureList, NPCs::LunarfallRaccoon, 65.0f);

            for (Creature* l_Creature : l_CreatureList)
                l_Creature->DespawnOrUnsummon();

            for (uint32 l_I = 0; l_I < s_PositionCount; ++l_I)
            {
                SequencePosition const& l_CurrentPosition = s_Position[l_I];
                Creature * l_Creature = SummonRelativeCreature(NPCs::LunarfallRaccoon, l_CurrentPosition.X, l_CurrentPosition.Y, l_CurrentPosition.Z, 0, TEMPSUMMON_MANUAL_DESPAWN);
                if (l_Creature)
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
                        if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -6.5917f, -7.0602f, 0.1845f, 4.9248f, TEMPSUMMON_MANUAL_DESPAWN))
                        {
                            if (CreatureTemplate const* l_FollowerTemplate = sObjectMgr->GetCreatureTemplate(l_GarrFollEntry->CreatureID[1]))
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
    void npc_NaronBloomthistleAI::sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 /*p_Opt*/)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (l_Owner != p_Player)
            return;

        if (p_Quest->GetQuestId() == Quests::Alliance_ClearingTheGarden)
        {
            static const Position s_CinematicLoc = { 1847.9746f, 144.6517f, 78.40f, 0.803772f };

            std::list<Creature*> l_CreatureList;
            me->GetCreatureListWithEntryInGrid(l_CreatureList, NPCs::LunarfallRaccoon, 65.0f);

            for (Creature* l_Creature : l_CreatureList)
                l_Creature->DespawnOrUnsummon();

            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

            if (l_BuildingEntry)
                l_Owner->PlayStandaloneScene(l_Owner->GetGarrison()->GetGarrisonFactionIndex() ? l_BuildingEntry->HordeActivationScenePackageID : l_BuildingEntry->AllianceActivationScenePackageID, 0, s_CinematicLoc);

            l_Owner->GetGarrison()->ActivateBuilding(GetPlotInstanceID());
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Select game object entry for a fresh gathering spawn
    /// @p_MiscData : Misc data
    uint32 npc_NaronBloomthistleAI::SelectGameObjectEntryForGatheringSpawn(uint32 p_MiscData)
    {
        if (p_MiscData == HerbSpawnType::Random)
            return g_AllyHerbsGobsEntry[urand(0, HerbSpawnType::Random - 1)];

        return g_AllyHerbsGobsEntry[(p_MiscData >= HerbSpawnType::Random) ? 0 : p_MiscData];
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// @p_Action : Action ID
    void npc_NaronBloomthistleAI::DoAction(int32 p_Action)
    {
        ResetGatheringPlots(p_Action);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_NaronBloomthistle::npc_NaronBloomthistle()
        : CreatureScript("npc_NaronBloomthistle_Garr")
    {

    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_NaronBloomthistle::GetAI(Creature* p_Creature) const
    {
        return new npc_NaronBloomthistleAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS
#endif
