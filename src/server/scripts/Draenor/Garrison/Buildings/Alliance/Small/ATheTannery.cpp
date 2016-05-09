////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ATheTannery.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77383 - Anders Longstitch                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AndersLongstitchAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };
        
        char ScriptName[] = "npc_AndersLongstitch_Garr";

        std::vector<RecipesConditions> Recipes
        { 
            { 171260,     0 },
            { 171261,     0 },
            { 171262,     0 },
            { 171263,     0 },
            { 171264,     0 },
            { 171265,     0 },
            { 171266, 28267 },
            { 171267, 28267 },
            { 171268, 28267 },
            { 171269, 28267 },
            { 171270, 28268 },
            { 171271, 28268 },
            { 171272, 28268 },
            { 171273, 28268 },
            { 171274, 28268 },
            { 171275, 28268 },
            { 171276, 28268 },
            { 171277, 28268 },
            { 171282, 28268 },
            { 171284, 28268 },
            { 171281, 28268 },
            { 171279, 28268 },
            { 171280, 28268 },
            { 171278, 28268 },
            { 171285, 28268 },
            { 171283, 28268 },
            { 178208,     0 },
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 78207 - Marianne Levine                                           ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_MarianneLevine::npc_MarianneLevine()
        : CreatureScript("npc_MarianneLevine_Garr")
    {

    }

    bool npc_MarianneLevine::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstLeatherworkingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Alliance_YourFirstLeatherworkingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_YourFirstLeatherworkingWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Alliance_YourFirstLeatherworkingWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstLeatherworkingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_MarianneLevine::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
    {
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_MarianneLevine::GetAI(Creature* p_Creature) const
    {
        return new npc_MarianneLevineAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


    /// Constructor
    npc_MarianneLevine::npc_MarianneLevineAI::npc_MarianneLevineAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_MarianneLevine::npc_MarianneLevineAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_MarianneLevine::npc_MarianneLevineAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Buildings::TheTannery_TheTannery_Level2:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], 0.0216f, 1.0433f, 1.0822f, 6.1718f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_TRADESKILL_NPC);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                AddSummonGUID(l_Creature->GetGUID());
                            }
                            break;
                        case Buildings::TheTannery_TheTannery_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -3.4031f, -2.0342f, 0.7733f, 6.1092f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_TRADESKILL_NPC);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
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
    }

}   ///< namespace Garrison
}   ///< namespace MS