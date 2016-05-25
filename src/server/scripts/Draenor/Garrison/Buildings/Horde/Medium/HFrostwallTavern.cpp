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

    void npc_MurgAI::OnSetPlotInstanceID(uint32 /*p_PlotInstanceID*/)
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

                std::vector<uint32>& l_Entries = l_GarrisonMgr->GetGarrisonTavernDatas();

                if (l_Entries.size() == 1)
                    SummonRelativeCreature(l_Entries[0],
                    g_QuestGiverHordePositions[0].X,
                    g_QuestGiverHordePositions[0].Y,
                    g_QuestGiverHordePositions[0].Z,
                    g_QuestGiverHordePositions[0].O,
                    TEMPSUMMON_MANUAL_DESPAWN);
                else if (l_Entries.size() > 1)
                {
                    SummonRelativeCreature(l_Entries[0],
                        g_QuestGiverHordePositions[1].X,
                        g_QuestGiverHordePositions[1].Y,
                        g_QuestGiverHordePositions[1].Z,
                        g_QuestGiverHordePositions[1].O,
                        TEMPSUMMON_MANUAL_DESPAWN);

                    SummonRelativeCreature(l_Entries[1],
                        g_QuestGiverHordePositions[2].X,
                        g_QuestGiverHordePositions[2].Y,
                        g_QuestGiverHordePositions[2].Z,
                        g_QuestGiverHordePositions[2].O,
                        TEMPSUMMON_MANUAL_DESPAWN);
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
        if (p_Player->HasQuest(Quests::Horde_YourFirstInscriptionWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstInscriptionWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstInscriptionWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Horde_YourFirstInscriptionWorkOrder) || p_Player->IsQuestRewarded(Quests::Horde_YourFirstInscriptionWorkOrder))
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        l_AI->SendFollowerRecruitmentUI(p_Player);

        //p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_akanja_garr::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
    {
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
