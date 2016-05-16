////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ATheForge.hpp"
#include "ATheForge_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 77359 - Auria Irondreamer                                         ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_AuriaIrondreamerAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(AuriaIrondreamer_Level1::MovePointLoc, AuriaIrondreamer_Level1::Sequence, sizeof(AuriaIrondreamer_Level1::Sequence), AuriaIrondreamer_Level1::MovePointIDs::ForgeOre);

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeOre, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                      [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeOre - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                  [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeOre, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ForgeFront] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeFront, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                        [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ForgeFront - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                    [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ForgeFront, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::Workorder] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::Workorder, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                       [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::Workorder - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderDown, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                           [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::WorkorderDown - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                       [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderDown, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::WorkorderFront] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::WorkorderFront, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                            [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::WorkorderFront - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });
            };

            p_This->m_OnPointReached[AuriaIrondreamer_Level1::MovePointIDs::ChestLeft] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ChestLeft, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                       [p_This]() -> void { p_This->SetFacingBuildingRelative(AuriaIrondreamer_Level1::MovePointLoc[AuriaIrondreamer_Level1::MovePointIDs::ChestLeft - AuriaIrondreamer_Level1::MovePointIDs::ForgeOre].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                   [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(AuriaIrondreamer_Level1::DestPointDuration::ChestLeft, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        char ScriptName[] = "npc_AuriaIrondreamer_Garr";

        std::vector<RecipesConditions> Recipes
        {
            { 171692,     0 },
            { 171693,     0 },
            { 171691,     0 },
            { 171694, 28366 },
            { 171695, 28366 },
            { 171696, 28366 },
            { 171697, 28366 },
            { 171698, 28366 },
            { 171699, 28366 },
            { 171700, 28367 },
            { 171701, 28367 },
            { 171702, 28367 },
            { 171703, 28367 },
            { 171704, 28367 },
            { 171705, 28367 },
            { 171706, 28367 },
            { 171707, 28367 },
            { 173355, 28366 },
            { 178243, 28366 },
            { 178245, 28366 },
        };
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77792 - Yulia Samras                                              ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_YuliaSamrasAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI * p_This, Creature * /*p_Me*/)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(YuliaSamras_Level1::MovePointLoc, YuliaSamras_Level1::Sequence, sizeof(YuliaSamras_Level1::Sequence), YuliaSamras_Level1::MovePointIDs::Workorder);

            p_This->m_OnPointReached[YuliaSamras_Level1::MovePointIDs::Workorder] = [p_This]() -> void
            {
                p_This->AddTimedDelayedOperation(YuliaSamras_Level1::DestPointDuration::Workorder, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                  [p_This]() -> void { p_This->SetFacingBuildingRelative(YuliaSamras_Level1::MovePointLoc[YuliaSamras_Level1::MovePointIDs::Workorder - YuliaSamras_Level1::MovePointIDs::Workorder].O); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI * /*p_This*/, Creature * /*p_Me*/)
        {

        };
    }

    /// Constructor
    npc_YuliaSamras::npc_YuliaSamras()
        : CreatureScript("npc_YuliaSamras_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_YuliaSamras::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstBlacksmithingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Alliance_YourFirstBlacksmithingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_YourFirstBlacksmithingWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Alliance_YourFirstBlacksmithingWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstBlacksmithingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM_DB(GarrisonGossipMenus::MenuID::DefaultMenuGreetings, GarrisonGossipMenus::GossipOption::DefaultWorkOrder, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_YuliaSamras::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
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
    CreatureAI* npc_YuliaSamras::GetAI(Creature* p_Creature) const
    {
        return new npc_YuliaSamrasAI(p_Creature);
    }

    /// Constructor
    npc_YuliaSamras::npc_YuliaSamrasAI::npc_YuliaSamrasAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_YuliaSamras::npc_YuliaSamrasAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_YuliaSamras::npc_YuliaSamrasAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Buildings::TheForge_TheForge_Level2:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -1.6759f, -1.1615f, 0.7728f, 6.2240f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                AddSummonGUID(l_Creature->GetGUID());
                            }
                            break;
                        case Buildings::TheForge_TheForge_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -3.0387f, -0.3201f, 0.7725f, 0.1411f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
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
    }

}   ///< namespace Garrison
}   ///< namespace MS
