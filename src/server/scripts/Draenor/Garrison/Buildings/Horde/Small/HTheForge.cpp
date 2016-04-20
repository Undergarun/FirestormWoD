////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HTheForge.hpp"
#include "HTheForge_Level1Data.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

namespace MS { namespace Garrison
{
    //////////////////////////////////////////////////////////////////////////
    /// 79867 - Orgek Ironhand                                             ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_OrgekIronhandAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(OrgekIronhand_Level1::MovePointLoc, OrgekIronhand_Level1::Sequence, sizeof(OrgekIronhand_Level1::Sequence), OrgekIronhand_Level1::MovePointIDs::Anvil);

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Anvil] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Anvil, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Anvil - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(1, true);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Anvil, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Front] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Front, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push([p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Front - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Front, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Forge1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge1, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                 [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Forge1 - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS, [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge1, [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Forge2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge2, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                 [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Forge2 - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                             [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Forge2, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[OrgekIronhand_Level1::MovePointIDs::Chest] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Chest, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                                [p_This]() -> void { p_This->SetFacingBuildingRelative(OrgekIronhand_Level1::MovePointLoc[OrgekIronhand_Level1::MovePointIDs::Chest - OrgekIronhand_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
                p_This->AddTimedDelayedOperation(OrgekIronhand_Level1::DestPointDuration::Chest, [p_Me]() -> void { p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
            };

            p_This->DoNextSequenceAction();
        };

        InitSequenceFunction FnLevel2 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        InitSequenceFunction FnLevel3 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {

        };

        char ScriptName[] = "npc_OrgekIronhand_Garr";
    }

    //////////////////////////////////////////////////////////////////////////
    /// 79817 - Kinja                                                      ///
    //////////////////////////////////////////////////////////////////////////
    namespace npc_KinjaAIData
    {
        InitSequenceFunction FnLevel1 = [](GarrisonNPCAI* p_This, Creature* p_Me)
        {
            p_This->SetAIObstacleManagerEnabled(true);
            p_This->SetupActionSequence(Kinja_Level1::MovePointLoc, Kinja_Level1::Sequence, sizeof(Kinja_Level1::Sequence), Kinja_Level1::MovePointIDs::Anvil);

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Anvil] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Anvil, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                        [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Anvil - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                    [p_Me]() -> void
                {
                    p_Me->LoadEquipment(1, true);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
                });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Anvil, [p_Me]() -> void
                {
                    p_Me->LoadEquipment(0, true);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Front] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Front, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                        [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Front - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                    [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK); });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Front, [p_Me]() -> void
                {
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Forge1] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge1, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                         [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Forge1 - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                    p_Me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
                });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge1, [p_Me]() -> void
                {
                    p_Me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                    p_Me->HandleEmoteCommand(0);
                    p_Me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                });
            };

            p_This->m_OnPointReached[Kinja_Level1::MovePointIDs::Forge2] = [p_This, p_Me]() -> void
            {
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge2, [p_This]() -> void { p_This->DoNextSequenceAction(); });
                p_This->m_DelayedOperations.push(                                         [p_This]() -> void { p_This->SetFacingBuildingRelative(Kinja_Level1::MovePointLoc[Kinja_Level1::MovePointIDs::Forge2 - Kinja_Level1::MovePointIDs::Anvil].O); });

                p_This->AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [p_Me]() -> void { p_Me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
                p_This->AddTimedDelayedOperation(Kinja_Level1::DestPointDuration::Forge2, [p_Me]() -> void
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
    }

    /// Constructor
    npc_Kinja::npc_Kinja()
        : CreatureScript("npc_Kinja_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_Kinja::OnGossipHello(Player* p_Player, Creature* p_Creature)
    {
        if (p_Player->HasQuest(Quests::Horde_YourFirstBlacksmithingWorkOrder) && !p_Player->IsQuestRewarded(Quests::Horde_YourFirstBlacksmithingWorkOrder))
            p_Player->PlayerTalkClass->GetQuestMenu().AddMenuItem(Quests::Horde_YourFirstBlacksmithingWorkOrder, 4);

        if (p_Player->HasQuest(Quests::Horde_YourFirstBlacksmithingWorkOrder) || p_Player->IsQuestRewarded(Quests::Horde_YourFirstBlacksmithingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_Kinja::OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 p_Sender, uint32 p_Action)
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
    CreatureAI* npc_Kinja::GetAI(Creature* p_Creature) const
    {
        return new npc_KinjaAI(p_Creature);
    }

    /// Constructor
    npc_Kinja::npc_KinjaAI::npc_KinjaAI(Creature* p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

    void npc_Kinja::npc_KinjaAI::SetGUID(uint64 p_Guid, int32 p_Id)
    {
        if (p_Id == CreatureAIDataIDs::OwnerGuid)
            m_OwnerGuid = p_Guid;
    }

    void npc_Kinja::npc_KinjaAI::OnPlotInstanceUnload()
    {
        for (std::vector<uint64>::iterator l_Guid = m_Summons.begin(); l_Guid != m_Summons.end(); ++l_Guid)
        {
            if (Creature* l_Creature = HashMapHolder<Creature>::Find(*l_Guid))
                l_Creature->DespawnOrUnsummon();
        }

        m_Summons.clear();
    }

    void npc_Kinja::npc_KinjaAI::OnSetPlotInstanceID(uint32 p_PlotInstanceID)
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
                        case Buildings::TheForge_TheForge_Level3:
                            if (Creature* l_Creature = SummonRelativeCreature(l_GarrFollEntry->CreatureID[0], -4.1991f, -0.8694f, 0.8636f, 0.4778f, TEMPSUMMON_MANUAL_DESPAWN))
                            {
                                l_GarrisonMgr->InsertNewCreatureInPlotDatas(p_PlotInstanceID, l_Creature->GetGUID());
                                l_Creature->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                                m_Summons.push_back(l_Creature->GetGUID());
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