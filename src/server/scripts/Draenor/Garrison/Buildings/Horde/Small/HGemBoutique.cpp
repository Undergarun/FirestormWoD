////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef CROSS
#include "HGemBoutique.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79832 - Dorogarr                                                  ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_DorogarrAIData
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

        char ScriptName[] = "npc_Dorogarr_Garr";

///        std::vector<RecipesConditions> Recipes = sObjectMgr->GetNpcRecipesConditions(79829);

       /* std::vector<RecipesConditions> Recipes
        {
            { 170701, 28179 },
            { 170706,     0 },
            { 170705,     0 },
            { 170704,     0 },
            { 170709,     0 },
            { 170708,     0 },
            { 170707,     0 },
            { 170712,     0 },
            { 170711,     0 },
            { 170710,     0 },
            { 170724, 28179 },
            { 170723, 28179 },
            { 170722, 28179 },
            { 170721, 28179 },
            { 170720, 28179 },
            { 170719, 28179 },
            { 170715, 28180 },
            { 170714, 28180 },
            { 170713, 28180 },
            { 170718, 28180 },
            { 170717, 28180 },
            { 170716, 28180 },
        };*/
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79830 - Elrondir Surrion                                          ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_ElrondirSurrion::npc_ElrondirSurrion()
        : CreatureScript("npc_ElrondirSurrion_Garr")
    {

    }

    bool npc_ElrondirSurrion::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Horde_YourFirstJewelcraftingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstJewelcraftingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstJewelcraftingWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Horde_YourFirstJewelcraftingWorkOrder) || p_Player->IsQuestRewarded(Quests::Horde_YourFirstJewelcraftingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_ElrondirSurrion::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
    {
        GarrisonNPCAI* l_AI = p_Creature->ToGarrisonNPCAI();

        if (l_AI == nullptr)
            return true;

        l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_ElrondirSurrion::GetAI(Creature* p_Creature) const
    {
        return new npc_ElrondirSurrionAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_ElrondirSurrion::npc_ElrondirSurrionAI::npc_ElrondirSurrionAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
    }

    void npc_ElrondirSurrion::npc_ElrondirSurrionAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_ElrondirSurrion::npc_ElrondirSurrionAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Building::ID::GemBoutique_GemBoutique_Level2:
                        case Building::ID::GemBoutique_GemBoutique_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(88546, -0.9046f, 5.7426f, 0.8908f, 4.7023f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
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
