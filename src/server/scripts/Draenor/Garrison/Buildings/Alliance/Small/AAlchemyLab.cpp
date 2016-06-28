////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "AAlchemyLab.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77363 - Mary Kearie                                               ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_MaryKearieAIData
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

        char ScriptName[] = "npc_MaryKearie_Garr";

        std::vector<RecipesConditions> Recipes
        {
            { 156582,     0 },
            { 156585,     0 },
            { 156577, 27405 },
            { 156578, 27405 },
            { 156579, 27405 },
            { 156580, 27405 },
            { 156581, 27405 },
            { 156561, 27406 },
            { 156563, 27406 },
            { 156564, 27406 },
            { 175869,     0 },
            { 175868,     0 },
            { 175867,     0 },
            { 175866,     0 },
            { 175865,     0 },
            { 175853,     0 },
            { 156568, 27406 }
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77791 - Peter Kearie                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_PeterKearieAIData
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
    npc_PeterKearie::npc_PeterKearie()
        : CreatureScript("npc_PeterKearie_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_PeterKearie::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstAlchemyWorkOrder) && !p_Player->IsQuestRewarded(Quests::Alliance_YourFirstAlchemyWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_YourFirstAlchemyWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Alliance_YourFirstAlchemyWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstAlchemyWorkOrder))
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF );

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_PeterKearie::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
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
    CreatureAI* npc_PeterKearie::GetAI(Creature* p_Creature) const
    {
        return new npc_PeterKearieAI(p_Creature);
    }

    /// Constructor
    npc_PeterKearie::npc_PeterKearieAI::npc_PeterKearieAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_PeterKearie::npc_PeterKearieAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_PeterKearie::npc_PeterKearieAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Buildings::AlchemyLab_AlchemyLab_Level2:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -1.6759f, -1.1615f, 0.7728f, 6.2240f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                                sObjectMgr->AddCreatureQuestRelationBounds(l_Creature->GetEntry(), 37270);
                                sObjectMgr->AddCreatureQuestInvolvedRelationBounds(l_Creature->GetEntry(), 37270);
                                AddSummonGUID(l_Creature->GetGUID());

                                /// inform client about quest status
                                if (l_Creature->AI())
                                    l_Creature->AI()->SetGUID(m_OwnerGuid, 1);
                            }
                            break;
                        case Buildings::AlchemyLab_AlchemyLab_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -0.8157f, -0.4505f, 0.7730f, 0.2894f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                                sObjectMgr->AddCreatureQuestRelationBounds(l_Creature->GetEntry(), 37270);
                                sObjectMgr->AddCreatureQuestInvolvedRelationBounds(l_Creature->GetEntry(), 37270);
                                AddSummonGUID(l_Creature->GetGUID());

                                /// inform client about quest status
                                if (l_Creature->AI())
                                    l_Creature->AI()->SetGUID(m_OwnerGuid, 1);
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
