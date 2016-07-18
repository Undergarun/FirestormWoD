////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "ALunarfallInn.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "../../../GarrisonScriptData.hpp"
#include "../../../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77368 - Madison Clark                                             ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_MadisonClark::npc_MadisonClark()
        : CreatureScript("npc_MadisonClark_Garr")
    {

    }

    /// Constructor
    npc_MadisonClarkAI::npc_MadisonClarkAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_MadisonClark::GetAI(Creature* p_Creature) const
    {
        return new npc_MadisonClarkAI(p_Creature);
    }

    void npc_MadisonClarkAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        if (Player* l_Player = GetOwner())
        {
            std::list<Creature*> l_CreatureList;
            MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            me->GetCreatureListInGrid(l_CreatureList, 30.0f);

            for (Creature* l_Creature : l_CreatureList)
            {
                if (std::find(TavernDatas::g_QuestGiverEntries.begin(), TavernDatas::g_QuestGiverEntries.end(), l_Creature->GetEntry()) != TavernDatas::g_QuestGiverEntries.end())
                    l_Creature->DespawnOrUnsummon();
            }

            std::vector<uint32>& l_Entries = l_GarrisonMgr->GetGarrisonDailyTavernDatas();

            if (l_Entries.empty())
            {
                l_GarrisonMgr->ResetGarrisonDailyTavernData();
                OnSetPlotInstanceID(p_PlotInstanceID);
                return;
            }

            switch (l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(MS::Garrison::Building::Type::Inn)))
            {
                case 1:
                {
                    if (l_Entries.size() > 1)
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverAlliancePositions[1], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());

                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[1], g_QuestGiverAlliancePositions[2], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());
                    }
                    else
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverAlliancePositions[0], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());
                    }

                    break;
                }
                case 2:
                {
                    if (l_Entries.size() > 1)
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverAlliancePositions[4], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());

                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[1], g_QuestGiverAlliancePositions[5], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());
                    }
                    else
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverAlliancePositions[3], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());
                    }

                    break;
                }
                case 3:
                {
                    if (l_Entries.size() > 1)
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverAlliancePositions[7], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());

                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[1], g_QuestGiverAlliancePositions[8], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());
                    }
                    else
                    {
                        if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverAlliancePositions[6], TEMPSUMMON_MANUAL_DESPAWN))
                            AddSummonGUID(l_Creature->GetGUID());
                    }

                    break;
                }
                default:
                    break;
            }
        }
    }

    void npc_MadisonClarkAI::OnDailyDataReset()
    {
        if (Player* l_Player = GetOwner())
        {
            MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

            if (l_GarrisonMgr == nullptr)
                return;

            OnSetPlotInstanceID(GetPlotInstanceID());
            l_GarrisonMgr->UpdatePlot(GetPlotInstanceID());
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_lysa_serion_garr::npc_lysa_serion_garr()
        : CreatureScript("npc_lysa_serion_garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_lysa_serion_garr::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr || l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(Building::Type::Inn)) < 2)
            return true;

        if (!p_Player->IsQuestRewarded(Quests::Alliance_TheHeadHunterHarverst))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_TheHeadHunterHarverst, 4);

        if (p_Player->GetQuestStatus(Quests::Alliance_TheHeadHunterHarverst) != QUEST_STATUS_NONE && l_GarrisonMgr->CanRecruitFollower())
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::FollowerRecruitment, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_lysa_serion_garr::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
    {
        if (p_Action == GOSSIP_ACTION_INFO_DEF)
        {
            GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

            if (l_AI == nullptr)
                return true;

            l_AI->SendFollowerRecruitmentUI(p_Player);
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_lysa_serion_garr::GetAI(Creature* p_Creature) const
    {
        return new npc_lysa_serion_garrAI(p_Creature);
    }

    /// Constructor
    npc_lysa_serion_garr::npc_lysa_serion_garrAI::npc_lysa_serion_garrAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_lysa_serion_garr::npc_lysa_serion_garrAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
    }

}   ///< namespace Garrison
}   ///< namespace MS
#endif
