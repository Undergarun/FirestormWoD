////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "HFrostwallMines.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "../../GarrisonScriptData.hpp"
#include "../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    std::vector<uint32> g_DepositsEntry
    {
        232542, ///< GameObjects::GobBlackrockDeposit
        232543, ///< GameObjects::GobRichBlackrockDeposit
        232544, ///< GameObjects::GobTrueIronDeposit
        232545  ///< GameObjects::GobRichTrueIronDeposit
    };

    std::vector<GatheringPlotInfos> g_MineDeposits
    {
        /// Level 1
        { 1, -21.2715f,   -13.8611f, -59.2858f, 2.9218f },
        { 1,  38.9916f,    18.4105f, -43.6418f, 4.1906f },
        { 1, -30.3754f,    -2.0047f, -55.6653f, 4.4898f },
        { 1,  54.8163f,   -86.7227f, -22.4267f, 4.8856f },
        { 1, -15.7798f,   -82.9398f,  16.0685f, 1.2571f },
        { 1,   5.5319f,  -103.7457f,   1.8457f, 1.5045f },
        { 1,  43.0758f,   -75.4841f, -24.3843f, 2.3300f },
        { 1,  -5.3802f,     5.2938f, -32.1254f, 0.8919f },

        /// Level 2 Nodes
        { 2,  -0.1453f,   -83.1668f,  21.3184f, 2.8043f }, ///< Index 8
        { 2, -18.0635f,   -92.3047f,  12.1119f, 6.0205f },
        { 2,   9.0536f,  -104.4316f,  -0.4216f, 4.8896f },
        { 2,  41.9440f,  -118.8517f, -20.8364f, 2.2074f },
        { 2,  25.6644f,  -105.8075f, -20.5683f, 0.1418f },
        { 2,  44.0880f,   -75.4456f, -24.4706f, 0.2793f },
        { 2,  45.3556f,   -34.6418f, -29.5068f, 4.1670f },
        { 2,   9.4826f,     8.5928f, -55.4112f, 3.9274f },
        { 2, -11.7996f,     5.0582f, -31.3561f, 5.3922f },
        { 2,   6.0193f,    38.4071f, -41.1025f, 5.8870f },
        { 2,  24.5833f,     6.6041f, -39.6288f, 1.5752f },
        { 2,  23.2054f,    21.1458f, -38.8364f, 5.3097f },
        { 2, 129.4601f,   -74.0291f, -32.3973f, 3.2716f },
        { 2, 141.0421f,  -130.9598f, -42.5439f, 2.4783f },

        /// Level 2 Mine carts
        { 2,  19.2847f,  -108.0606f,  -4.4168f, 0.4119f }, ///< Index 22
        { 2, -15.5335f,     6.2892f, -56.3418f, 6.2199f },
        { 2, 122.6982f,   -78.7842f, -34.0165f, 1.7353f },

        /// Level 3 Nodes
        { 3,  -7.5575f,  -109.2435f,   7.9372f, 5.2578f }, ///< Index 25
        { 3,  21.5750f,  -104.3829f,  -4.0382f, 5.4109f },
        { 3,  41.6786f,  -119.8712f, -20.8772f, 1.9945f },
        { 3,  30.6119f,   -92.4000f, -20.0998f, 0.4354f },
        { 3,  70.5351f,   -67.0701f, -26.6804f, 0.6711f },
        { 3, 126.8150f,   -91.7694f, -33.3816f, 5.1635f },
        { 3, 118.0903f,   -56.0649f, -33.0110f, 0.2195f },
        { 3, 121.8157f,  -133.3610f, -42.1396f, 0.4394f },
        { 3,  40.9749f,   -21.6144f, -30.1959f, 1.1957f },
        { 3,  61.4366f,    -7.5749f, -29.4919f, 1.2310f },
        { 3,  57.4152f,     9.9456f, -31.5089f, 6.2065f },
        { 3,  90.7145f,     9.8223f, -32.5748f, 0.7873f },
        { 3,  95.8351f,    36.5286f, -33.3050f, 5.6332f },
        { 3,   7.4742f,    -6.0910f, -31.8558f, 2.1499f },
        { 3,  -8.8085f,     2.4314f, -31.3037f, 0.5909f },
        { 3,  12.5277f,    25.4946f, -38.7423f, 2.4759f },
        { 3,  32.6245f,    27.8861f, -41.0488f, 4.0899f },
        { 3,  24.0614f,     7.3652f, -39.5016f, 1.6983f },
        { 3, 139.4184f,  -119.9508f, -40.3710f, 3.0649f },
        { 3,  97.3166f,   -84.2670f, -36.3878f, 0.6773f },
        { 3, 112.4439f,    21.1500f, -32.8400f, 1.5295f },

        /// Level 3 Mine carts
        { 3,  103.002f,    35.7791f, -32.8620f, 0.7441f }, ///< Index 46
        { 3,   6.0586f,    11.4875f, -35.9979f, 1.0072f },
        { 3,  122.272f,   -77.8380f, -34.0436f, 4.8242f },
        { 3,  36.3170f,   -89.1779f, -21.0286f, 0.6145f }
    };

    std::vector<SequencePosition> g_PlaceholderHordeVector;

    //////////////////////////////////////////////////////////////////////////
    /// 77730 - Timothy Leens                                             ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Gorsol::npc_Gorsol()
        : CreatureScript("npc_Gorsol_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    std::vector<SequencePosition> npc_Gorsol::CalculateDepositsPositions()
    {
        std::vector<SequencePosition> l_DepositsPositions;

        for (SequencePosition l_Position : g_MineLeftSideDeposits)
        {
            if (l_DepositsPositions.size() < 10 && roll_chance_i(50))
                l_DepositsPositions.push_back(l_Position);
            else if (l_DepositsPositions.size() >= 10)
                break;
        }

        for (SequencePosition l_Position : g_MineRightSideDeposits)
        {
            if (l_DepositsPositions.size() >= 10 && l_DepositsPositions.size() < 20 && roll_chance_i(50))
                l_DepositsPositions.push_back(l_Position);
            else if (l_DepositsPositions.size() >= 20)
                break;
        }

        return l_DepositsPositions;
    }

    void npc_Gorsol::SummonDeposits(std::vector<SequencePosition> p_SpawnPos, GarrisonNPCAI* l_AI)
    {
        for (SequencePosition l_Position : p_SpawnPos)
        {
            if (roll_chance_i(50)) ///< Blackrock Deposit
                l_AI->SummonRelativeGameObject(roll_chance_i(20) ? GameObjects::GobRichBlackrockDeposit : GameObjects::GobBlackrockDeposit, l_Position.X, l_Position.Y, l_Position.Z, l_Position.O);
            else ///< True Iron Deposit
                l_AI->SummonRelativeGameObject(roll_chance_i(20) ? GameObjects::GobRichTrueIronDeposit : GameObjects::GobTrueIronDeposit, l_Position.X, l_Position.Y, l_Position.Z, l_Position.O);
        }
    }

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_Gorsol::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Quest const* l_Quest = sObjectMgr->GetQuestTemplate(Quests::Horde_ThingsAreNotGorenOurWay);

        if (l_Quest == nullptr)
            return true;

        if (p_Player->IsQuestRewarded(Quests::Horde_ThingsAreNotGorenOurWay) ||
            (p_Player->GetQuestStatus(Quests::Horde_ThingsAreNotGorenOurWay) == QUEST_STATUS_INCOMPLETE))
        {
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::MineDefaultOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());
        }
        else if (p_Player->GetQuestStatus(Quests::Horde_ThingsAreNotGorenOurWay) == QUEST_STATUS_NONE)
            p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_Quest, p_Creature->GetGUID());
        else if (p_Player->GetQuestStatus(Quests::Horde_ThingsAreNotGorenOurWay) == QUEST_STATUS_COMPLETE)
            p_Player->PlayerTalkClass->SendQuestGiverOfferReward(l_Quest, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_Gorsol::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        p_Player->PlayerTalkClass->ClearMenus();

        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Gorsol::GetAI(Creature* p_Creature) const
    {
        return new npc_GorsolAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GorsolAI::npc_GorsolAI(Creature* p_Creature)
        : GatheringBuildingMaster(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_GorsolAI::sQuestReward(Player* p_Player, Quest const* p_Quest, uint32 /*p_Option*/)
    {
        if (p_Quest->GetQuestId() == Quests::Horde_ThingsAreNotGorenOurWay)
        {
            std::vector<uint32> l_CreatureEntries = { NPCs::NpcFrostwallGoren, NPCs::NpcFrostwallGorenHatchling, NPCs::NpcStonetooth };
            me->DespawnCreaturesInArea(l_CreatureEntries, 100.0f);

            std::list<Creature*> l_MinersList;
            me->GetCreatureListWithEntryInGrid(l_MinersList, NPCs::NpcHordeMiner, 150.0f);

            if (l_MinersList.empty())
            {
                for (SequencePosition l_Pos : g_MinersPositions)
                {
                    if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcHordeMiner, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                        l_Creature->GetMotionMaster()->MoveRandom(7.0f);
                }

                GarrBuildingEntry const* l_BuildingEntry = sGarrBuildingStore.LookupEntry(GetBuildingID());
                Position l_Pos = *p_Player;

                if (l_BuildingEntry)
                    p_Player->PlayStandaloneScene(p_Player->GetGarrison()->GetGarrisonFactionIndex() ? l_BuildingEntry->HordeActivationScenePackageID : l_BuildingEntry->AllianceActivationScenePackageID, 0, l_Pos);

                if (Manager* l_Garrison = p_Player->GetGarrison())
                    l_Garrison->ActivateBuilding(GetPlotInstanceID());
            }
        }
    }

    void npc_GorsolAI::OnPlotInstanceUnload()
    {
        me->DespawnCreaturesInArea({ NPCs::NpcFrostwallGorenHatchling, NPCs::NpcFrostwallGoren, NPCs::NpcStonetooth }, 200.0f);
    }

    void npc_GorsolAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript();

        if (!l_GarrisonSite)
            return;

        Player* l_Owner = l_GarrisonSite->GetOwner();

        if (!l_Owner)
            return;

        if (!l_Owner->IsQuestRewarded(Quests::Horde_ThingsAreNotGorenOurWay))
        {
            for (SequencePosition l_Pos : g_GorenHatchlingPositions)
            {
                if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcFrostwallGorenHatchling, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                    l_Creature->GetMotionMaster()->MoveRandom(7.0f);
            }

            for (SequencePosition l_Pos : g_GorenPositions)
            {
                if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcFrostwallGoren, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                    l_Creature->GetMotionMaster()->MoveRandom(7.0f);
            }

            SummonRelativeCreature(NPCs::NpcStonetooth, 1.26916f, 3.68763f, -33.3223f, 4.37961f, TEMPSUMMON_MANUAL_DESPAWN);
        }
        else
        {
//             std::vector<SequencePosition> l_DepositsPositions = CalculateDepositsPositions();
//             SummonDeposits(l_DepositsPositions, this);

            std::vector<uint32> l_CreatureEntries = { NPCs::NpcFrostwallGorenHatchling, NPCs::NpcFrostwallGoren, NPCs::NpcStonetooth };
            me->DespawnCreaturesInArea(l_CreatureEntries, 100.0f);

            std::list<Creature*> l_MinersList;
            me->GetCreatureListWithEntryInGrid(l_MinersList, NPCs::NpcHordeMiner, 150.0f);

            if (l_MinersList.empty())
            {
                for (SequencePosition l_Pos : g_MinersPositions)
                {
                    if (Creature* l_Creature = SummonRelativeCreature(NPCs::NpcHordeMiner, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                        l_Creature->GetMotionMaster()->MoveRandom(3.0f);
                }
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
                    case Building::ID::LunarfallExcavation_FrostwallMines_Level2:
                    case Building::ID::LunarfallExcavation_FrostwallMines_Level3:
                        if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], 31.6474f, -61.9248f, 30.1793f, 5.0652f, TEMPSUMMON_MANUAL_DESPAWN))
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

    /// Select game object entry for a fresh gathering spawn
    /// @p_MiscData : Misc data
    uint32 npc_GorsolAI::SelectGameObjectEntryForGatheringSpawn(uint32 /*p_MiscData*/)
    {
        uint32 l_Entry = 0;

        if (roll_chance_i(50))
            l_Entry = roll_chance_i(20) ? GameObjects::GobRichBlackrockDeposit : GameObjects::GobBlackrockDeposit;
        else ///< True Iron Deposit
            l_Entry = roll_chance_i(20) ? GameObjects::GobRichTrueIronDeposit : GameObjects::GobTrueIronDeposit;

        return l_Entry;
    }

}   ///< namespace Garrison
}   ///< namespace MS
#endif
