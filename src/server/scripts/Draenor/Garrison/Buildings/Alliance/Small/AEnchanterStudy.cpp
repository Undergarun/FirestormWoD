////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "AEnchanterStudy.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77354 - Ayada The White                                           ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AyadaTheWhiteAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {

        };

        char ScriptName[] = "npc_AyadaTheWhite_Garr";

        std::vector<RecipesConditions> Recipes
        {
            { 158877, 31228 },
            { 158907,     0 },
            { 158892, 31228 },
            { 158893, 31228 },
            { 158878, 31228 },
            { 158908,     0 },
            { 158879, 31228 },
            { 158894, 31228 },
            { 158909,     0 },
            { 158880, 31228 },
            { 158910,     0 },
            { 158895, 31228 },
            { 158896, 31228 },
            { 158911,     0 },
            { 158881, 31228 },
            { 162948,     0 },
            { 169091,     0 },
            { 159236,     0 },
            { 158914, 31228 },
            { 158899, 31229 },
            { 158884, 31229 },
            { 158915, 31228 },
            { 158900, 31229 },
            { 158885, 31229 },
            { 158916, 31228 },
            { 158901, 31229 },
            { 158886, 31229 },
            { 158917, 31228 },
            { 158902, 31229 },
            { 158887, 31229 },
            { 158918, 31228 },
            { 158903, 31229 },
            { 158889, 31229 },
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77781 - Garn                                                      ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_GarmAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };
    }

    /// Constructor
    npc_Garm::npc_Garm()
        : CreatureScript("npc_Garm_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_Garm::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstEnchantingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Alliance_YourFirstEnchantingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_YourFirstEnchantingWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Alliance_YourFirstEnchantingWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstEnchantingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_Garm::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        GarrisonNPCAI* l_AI = p_Creature->AI() ? static_cast<GarrisonNPCAI*>(p_Creature->AI()) : nullptr;

        if (l_AI == nullptr)
            return true;

        l_AI->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI* npc_Garm::GetAI(Creature* p_Creature) const
    {
        return new npc_GarmAI(p_Creature);
    }

    /// Constructor
    npc_Garm::npc_GarmAI::npc_GarmAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_Garm::npc_GarmAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_Garm::npc_GarmAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Buildings::EnchanterStudy_EnchanterStudy_Level2:
                        case Buildings::EnchanterStudy_EnchanterStudy_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], 1.7011f, 5.9887f, 0.7726f, 4.5753f, TEMPSUMMON_MANUAL_DESPAWN))
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