////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "ATailoringEmporium.hpp"
#include "ATailoringEmporium_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison 
{
    //////////////////////////////////////////////////////////////////////////
    /// 77382 - Christopher Macdonald                                     ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_ChristopherMacdonaldData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(ChristopherMacdonald_Level1::MovePointLoc, ChristopherMacdonald_Level1::Sequence, sizeof(ChristopherMacdonald_Level1::Sequence), ChristopherMacdonald_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Home, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Home - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Home, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Chest - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Chest, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Armory] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Armory, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Armory - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Armory, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Front] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Front, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Front - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Front, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[ChristopherMacdonald_Level1::MovePointIDs::Materials] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Materials, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(ChristopherMacdonald_Level1::MovePointLoc[ChristopherMacdonald_Level1::MovePointIDs::Materials - ChristopherMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(ChristopherMacdonald_Level1::DestPointDuration::Materials, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        char gScriptName[] = "npc_ChristopherMacdonald_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77778 - Kaylie Macdonald                                          ////
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KaylieMacdonaldAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetupActionSequence(KaylieMacdonald_Level1::MovePointLoc, KaylieMacdonald_Level1::Sequence, sizeof(KaylieMacdonald_Level1::Sequence), KaylieMacdonald_Level1::MovePointIDs::Home);

            p_This->m_OnPointReached[KaylieMacdonald_Level1::MovePointIDs::Home] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Home, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(KaylieMacdonald_Level1::MovePointLoc[KaylieMacdonald_Level1::MovePointIDs::Home - KaylieMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);
                });
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Home, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[KaylieMacdonald_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(KaylieMacdonald_Level1::MovePointLoc[KaylieMacdonald_Level1::MovePointIDs::Chest - KaylieMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Chest, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[KaylieMacdonald_Level1::MovePointIDs::Body] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Body, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This, p_Me]() -> void
                {
                    p_This->SetFacingBuildingRelative(KaylieMacdonald_Level1::MovePointLoc[KaylieMacdonald_Level1::MovePointIDs::Body - KaylieMacdonald_Level1::MovePointIDs::Home].O);
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                });
                p_This->AddTimedDelayedOperation(KaylieMacdonald_Level1::DestPointDuration::Body, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_KaylieMacdonald::npc_KaylieMacdonald()
        : CreatureScript("npc_KaylieMacdonald_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_KaylieMacdonald::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstTailoringWorkOrder) && !p_Player->IsQuestRewarded(Quests::Alliance_YourFirstTailoringWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Alliance_YourFirstTailoringWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Alliance_YourFirstTailoringWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstTailoringWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_KaylieMacdonald::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
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
    CreatureAI* npc_KaylieMacdonald::GetAI(Creature* p_Creature) const
    {
        return new npc_KaylieMacdonaldAI(p_Creature);
    }

    /// Constructor
    npc_KaylieMacdonald::npc_KaylieMacdonaldAI::npc_KaylieMacdonaldAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_KaylieMacdonald::npc_KaylieMacdonaldAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_KaylieMacdonald::npc_KaylieMacdonaldAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Buildings::TailoringEmporium_TailoringEmporium_Level2:
                        case Buildings::TailoringEmporium_TailoringEmporium_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[1], -0.8347f, 4.7301f, 0.7607f, 5.2390f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_TRADESKILL_NPC);
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
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