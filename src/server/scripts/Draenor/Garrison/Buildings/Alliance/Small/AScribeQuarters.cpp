////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "AScribeQuarters.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77372 - Eric Broadoak                                             ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_EricBroadoakAIData
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

        char ScriptName[] = "npc_EricBroadoak_Garr";

        std::vector<RecipesConditions> Recipes
        {
            { 175392,     0 },
            { 175390,     0 },
            { 175389,     0 },
            { 166669,     0 },
            { 166363, 31230 },
            { 166359, 31231 },
            { 166356, 31231 },
            { 165804, 31231 },
            { 163294, 31230 },
            { 178550,     0 }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77777 - KurtBroadoak                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KurtBroadoakAIData
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
    }

    /// Constructor
    npc_KurtBroadoak::npc_KurtBroadoak()
        : CreatureScript("npc_KurtBroadoak_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_KurtBroadoak::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstInscriptionWorkOrder) && !p_Player->IsQuestRewarded(Quests::Alliance_YourFirstInscriptionWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_YourFirstInscriptionWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Alliance_YourFirstInscriptionWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstInscriptionWorkOrder))
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_KurtBroadoak::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
    {
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_KurtBroadoak::GetAI(Creature* p_Creature) const
    {
        return new npc_KurtBroadoakAI(p_Creature);
    }

    /// Constructor
    npc_KurtBroadoak::npc_KurtBroadoakAI::npc_KurtBroadoakAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_KurtBroadoak::npc_KurtBroadoakAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_KurtBroadoak::npc_KurtBroadoakAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
    {
        if (Player* l_Owner = HashMapHolder<Player>::Find(m_OwnerGuid))
        {
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
                        case Buildings::ScribesQuarters_ScribesQuarters_Level2:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -1.5711f, -3.8109f, 0.7731f, 1.0316f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_TRADESKILL_NPC);
                                AddSummonGUID(l_Creature->GetGUID());
                            }
                            break;
                        case Buildings::ScribesQuarters_ScribesQuarters_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], 3.5999f, -3.0674f, 0.7729f, 1.2122f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_TRADESKILL_NPC);
                                AddSummonGUID(l_Creature->GetGUID());
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }

}   ///< namespace Garrison
}   ///< namespace MS
#endif
