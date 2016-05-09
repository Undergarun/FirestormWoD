////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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

    /// FarmSimulator/GoatSimulator positions
    std::vector<GatheringPlotInfos> g_HordeHerbGardenFlowerPlot
    {
        /// Level 1
        { 1, -32.8207f, 35.7624f, 3.0563f, 3.1370f },
        { 1, -30.1080f, 34.6427f, 2.8868f, 5.8427f },
        { 1, -36.1460f, 30.8179f, 2.8871f, 2.3398f },
        { 1, -34.6058f, 28.8384f, 2.8872f, 3.6122f },
        { 1, -42.7375f, 35.9599f, 2.8865f, 2.7247f },
        { 1, -41.5901f, 37.4636f, 3.3671f, 0.8404f }

        /// Level 2

        /// Level 3
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
    void npc_TarnonAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
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

            InitGatheringPlots(HerbSpawnType::Random);
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

        return g_HordeHerbsGobsEntry[(p_MiscData >= HerbSpawnType::Random) ? 0 : p_MiscData];
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// @p_Action : Action ID
    void npc_TarnonAI::DoAction(int32 p_Action)
    {
        SetGatheringMiscData(g_HordeHerbsGobsEntry[p_Action]);
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
        {
            if (p_Player->GetGarrison())
            {
                if (!p_Player->GetGarrison()->HasFollowerAbility(MS::Garrison::GarrisonAbilities::AbilityHerbalism))
                    p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());
                else
                {
                    p_Player->PlayerTalkClass->ClearMenus();
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "I want to browse your goods.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
///                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to pick what we plant next.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                    p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                }
            }
        }

        return true;
    }

    bool npc_Tarnon::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (!p_Player || !p_Creature || !l_GarrisonMgr || !l_AI)
            return true;

        p_Player->PlayerTalkClass->ClearMenus();

        switch (p_Action)
        {
            case GOSSIP_ACTION_INFO_DEF + 1:
            {
                p_Player->PlayerTalkClass->ClearMenus();

                /// Test for removing already selected option

                uint32 l_Type = reinterpret_cast<GatheringBuildingMaster<&g_HordeHerbGardenFlowerPlot>*>(p_Creature->AI())->GetGatheringMiscData();

                if (l_Type != g_AllyHerbsGobsEntry[HerbSpawnType::Frostweed])
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Frostweed.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                if (l_Type != g_AllyHerbsGobsEntry[HerbSpawnType::Starflower])
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Starflower.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                if (l_Type != g_AllyHerbsGobsEntry[HerbSpawnType::Fireweed])
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Fireweed.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                if (l_Type != g_AllyHerbsGobsEntry[HerbSpawnType::TaladorOrchid])
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Talador Orchid.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 5);
                if (l_Type != g_AllyHerbsGobsEntry[HerbSpawnType::GorgrondFlytrap])
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gorgrond Flytrap.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 6);
                if (l_Type != g_AllyHerbsGobsEntry[HerbSpawnType::NagrandArrowbloom])
                    p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Nagrand Arrowbloom.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 7);

                p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
                break;
            }
            case GOSSIP_ACTION_INFO_DEF + 2:
                l_AI->DoAction(HerbAction::Frostweed);
                p_Player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 3:
                l_AI->DoAction(HerbAction::Starflower);
                p_Player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 4:
                l_AI->DoAction(HerbAction::Fireweed);
                p_Player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 5:
                l_AI->DoAction(HerbAction::TaladorOrchid);
                p_Player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 6:
                l_AI->DoAction(HerbAction::GorgrondFlytrap);
                p_Player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_INFO_DEF + 7:
                l_AI->DoAction(HerbAction::NagrandArrowbloom);
                p_Player->CLOSE_GOSSIP_MENU();
                break;
            case GOSSIP_ACTION_TRADE:
                p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());
                break;
            default:
                break;
            }

        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Tarnon::GetAI(Creature* p_Creature) const
    {
        return new npc_TarnonAI(p_Creature);
    }
}   ///< namespace Garrison
}   ///< namespace MS