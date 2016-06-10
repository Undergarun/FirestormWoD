////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "../../Alliance/Medium/ALunarfallInn.hpp"
#include "HFrostwallTavern.hpp"
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
    /// 79828 - Murg                                                       ///
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Murg::npc_Murg()
        : CreatureScript("npc_Murg_Garr")
    {

    }

    /// Constructor
    npc_MurgAI::npc_MurgAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Murg::GetAI(Creature* p_Creature) const
    {
        return new npc_MurgAI(p_Creature);
    }

    void npc_MurgAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        if (Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript())
        {
            if (Player* l_Player = l_GarrisonSite->GetOwner())
            {
                std::list<Creature*> l_CreatureList;
                me->GetCreatureListInGrid(l_CreatureList, 30.0f);

                for (Creature* l_Creature : l_CreatureList)
                {
                    if (std::find(TavernDatas::g_QuestGiverEntries.begin(), TavernDatas::g_QuestGiverEntries.end(), l_Creature->GetEntry()) != TavernDatas::g_QuestGiverEntries.end())
                        l_Creature->DespawnOrUnsummon();
                }

                MS::Garrison::Manager* l_GarrisonMgr = l_Player->GetGarrison();

                if (l_GarrisonMgr == nullptr)
                    return;

                std::vector<uint32>& l_Entries = l_GarrisonMgr->GetGarrisonDailyTavernDatas();

                if (l_Entries.empty())
                {
                    l_GarrisonMgr->ResetGarrisonDailyTavernData();
                    OnSetPlotInstanceID(p_PlotInstanceID);
                    return;
                }

                switch (l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(MS::Garrison::BuildingType::Inn)))
                {
                    case 1:
                    {
                        if (l_Entries.size() > 1)
                        {
                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverHordePositions[1], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());

                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[1], g_QuestGiverHordePositions[2], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());
                        }
                        else
                        {
                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverHordePositions[0], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());
                        }

                        break;
                    }
                    case 2:
                    {
                        if (l_Entries.size() > 1)
                        {
                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverHordePositions[4], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());

                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[1], g_QuestGiverHordePositions[5], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());
                        }
                        else
                        {
                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverHordePositions[3], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());
                        }

                        break;
                    }
                    case 3:
                    {
                        if (l_Entries.size() > 1)
                        {
                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverHordePositions[7], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());

                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[1], g_QuestGiverHordePositions[8], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());
                        }
                        else
                        {
                            if (Creature* l_Creature = SummonRelativeCreature(l_Entries[0], g_QuestGiverHordePositions[6], TEMPSUMMON_MANUAL_DESPAWN))
                                AddSummonGUID(l_Creature->GetGUID());
                        }

                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }

    void npc_MurgAI::OnDailyDataReset()
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
    npc_akanja_garr::npc_akanja_garr()
        : CreatureScript("npc_akanja_garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_akanja_garr::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr || l_GarrisonMgr->GetBuildingLevel(l_GarrisonMgr->GetBuildingWithType(BuildingType::Inn)) < 2)
            return true;

        if (!p_Player->HasQuest(Quests::Horde_TheHeadHunterHarverst) && !p_Player->IsQuestRewarded(Quests::Horde_TheHeadHunterHarverst))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_TheHeadHunterHarverst, 4);

        if (p_Player->GetQuestStatus(Quests::Horde_TheHeadHunterHarverst) != QUEST_STATUS_NONE)
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::FollowerRecruitment, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_akanja_garr::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 p_Action)
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
    CreatureAI* npc_akanja_garr::GetAI(Creature* p_Creature) const
    {
        return new npc_akanja_garrAI(p_Creature);
    }

    /// Constructor
    npc_akanja_garr::npc_akanja_garrAI::npc_akanja_garrAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_akanja_garr::npc_akanja_garrAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
    }

}   ///< namespace Garrison
}   ///< namespace MS
