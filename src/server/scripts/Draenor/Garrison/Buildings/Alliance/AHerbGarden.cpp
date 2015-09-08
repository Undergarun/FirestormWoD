////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "AHerbGarden.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison 
{

    std::vector<uint32> g_HerbsGobsEntry
    {
        235376, ///< HerbSpawnType::Frostweed
        235389, ///< HerbSpawnType::Starflower
        235387, ///< HerbSpawnType::Fireweed
        235391, ///< HerbSpawnType::TaladorOrchid
        235388, ///< HerbSpawnType::GrogrondFlytrap
        235390  ///< HerbSpawnType::NagrandArrowbloom
    };

    /// FarmSimulator/GoatSimulator positions
    std::vector<GatheringPlotInfos> g_HerbGardenFlowerPlot
    {
        /// Level 1
        { 1, 30.1702f, -15.1116f, -1.1398f, 1.8810f },
        { 1, 32.4670f, -14.9524f, -1.1400f, 1.8810f },
        { 1, 40.4576f,   0.8304f, -1.1388f, 1.8810f },
        { 1, 41.5929f,  -0.9677f, -1.0772f, 1.8810f },
        { 1, 40.2687f, -11.3029f, -1.1390f, 1.8810f },
        { 1, 41.8250f,  -9.6902f, -1.1411f, 1.8810f },
        /// Level 2

        /// Level 3
    };

    //////////////////////////////////////////////////////////////////////////
    /// 85514 - Olly Nimkip                                                ///
    //////////////////////////////////////////////////////////////////////////
    #pragma region 
    namespace npc_OllyNimkipData
    {
        /// Cosmetic init for level 1
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        /// Cosmetic init for level 2
        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        /// Cosmetic init for level 3
        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };
    }   ///< namespace npc_TharisStrongcastData

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_OllyNimkip::npc_OllyNimkip()
        : CreatureScript("npc_OllyNimkip_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_OllyNimkip::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->IsQuestRewarded(Quests::Alliance_ClearingTheGarden))
        {
            /// @TODO
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
            return true;
        }
        else
        {
            p_Player->SEND_GOSSIP_MENU(NPCTexts::OllyNimkipDefault, p_Creature->GetGUID());
            return true;
        }
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_OllyNimkip::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (!p_Player->IsQuestRewarded(Quests::Alliance_ClearingTheGarden))
            return true;

        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == CreatureScript::GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_OllyNimkip::GetAI(Creature * p_Creature) const
    {
        return new SimpleSequenceCosmeticScriptAI<&npc_OllyNimkipData::FnLevel1, &npc_OllyNimkipData::FnLevel2, &npc_OllyNimkipData::FnLevel3>(p_Creature);
    }
    #pragma endregion 

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

            for (uint32 l_I = 0; l_I < s_PositionCount; ++l_I)
            {
                SequencePosition const& l_CurrentPosition = s_Position[l_I];
                Creature * l_Creature = SummonRelativeCreature(NPCs::LunarfallRaccoon, l_CurrentPosition.X, l_CurrentPosition.Y, l_CurrentPosition.Z, 0, TEMPSUMMON_MANUAL_DESPAWN);
                if (l_Creature)
                    l_Creature->GetMotionMaster()->MoveRandom(10.f);
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On player quest rewarded
    /// @p_Player : Subject
    /// @p_Quest  : Rewarded quest template
    /// @p_Opt    : Quest reward option (eg: Loot choose)
    void npc_NaronBloomthistleAI::sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 p_Opt)
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
            me->GetCreatureListWithEntryInGrid(l_CreatureList, NPCs::LunarfallRaccoon, 65.f);

            for (Creature* l_Creature : l_CreatureList)
                l_Creature->DespawnOrUnsummon();

            GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());

            if (l_BuildingEntry)
                l_Owner->PlayStandaloneScene(l_BuildingEntry->BuiltScene[!l_Owner->GetGarrison()->GetGarrisonFactionIndex()], 0, s_CinematicLoc);

            l_Owner->GetGarrison()->ActivateBuilding(GetPlotInstanceID());

            InitGatheringPlots(HerbSpawnType::Random);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Select game object entry for a fresh gathering spawn
    /// @p_MiscData : Misc data
    uint32 npc_NaronBloomthistleAI::SelectGameObjectEntryForGatheringSpawn(uint32 p_MiscData)
    {
        if (p_MiscData == HerbSpawnType::Random)
            return g_HerbsGobsEntry[urand(0, HerbSpawnType::Random - 1)];

        return g_HerbsGobsEntry[p_MiscData];
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
    CreatureAI * npc_NaronBloomthistle::GetAI(Creature * p_Creature) const
    {
        return new npc_NaronBloomthistleAI(p_Creature);
    }

}   ///< namespace Garrison
}   ///< namespace MS