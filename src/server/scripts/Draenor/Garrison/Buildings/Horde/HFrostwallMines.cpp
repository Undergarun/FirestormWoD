////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
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
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Can you refine this draenic stone into ore for me?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
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
    bool npc_Gorsol::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->PlayerTalkClass->ClearMenus();
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();
        CreatureAI* l_AI = p_Creature->AI();

        if (l_AI == nullptr)
            return true;

        if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
                reinterpret_cast<GarrisonNPCAI*>(l_AI)->SendShipmentCrafterUI(p_Player);
        }

        return true;
    }

    bool npc_Gorsol::OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 p_Option)
    {
        if (p_Quest->GetQuestId() == Quests::Horde_ThingsAreNotGorenOurWay)
        {
            CreatureAI* l_AI = p_Creature->AI();

            if (l_AI == nullptr)
                return true;

            std::vector<uint32> l_CreatureEntries = { NPCs::NpcFrostwallGorenHatchling, NPCs::NpcFrostwallGoren, NPCs::NpcStonetooth };
            p_Creature->DespawnCreaturesInArea(l_CreatureEntries, 100.0f);

            std::list<Creature*> l_MinersList;
            p_Creature->GetCreatureListWithEntryInGrid(l_MinersList, NPCs::NpcHordeMiner, 150.0f);

            if (l_MinersList.empty())
            {
                for (SequencePosition l_Pos : g_MinersPositions)
                {
                    if (p_Player && p_Creature && p_Creature->GetScriptName() == CreatureScript::GetName())
                    {
                        if (Creature* l_Creature = reinterpret_cast<GarrisonNPCAI*>(l_AI)->SummonRelativeCreature(NPCs::NpcHordeMiner, l_Pos.X, l_Pos.Y, l_Pos.Z, 0, TEMPSUMMON_MANUAL_DESPAWN))
                            l_Creature->GetMotionMaster()->MoveRandom(7.0f);

                        if (Manager* l_Garrison = p_Player->GetGarrison())
                            l_Garrison->ActivateBuilding(reinterpret_cast<GarrisonNPCAI*>(l_AI)->GetPlotInstanceID());
                    }
                }
            }
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Gorsol::GetAI(Creature* p_Creature) const
    {
        return new npc_GorsolAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Gorsol::npc_GorsolAI::npc_GorsolAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_Gorsol::npc_GorsolAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
        }
        else
        {
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
    }

}   ///< namespace Garrison
}   ///< namespace MS