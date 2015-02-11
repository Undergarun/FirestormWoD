////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"

#include <random>

namespace MS { namespace Garrison 
{
    /// Constructor
    npc_Skaggit::npc_Skaggit()
        : CreatureScript("npc_Skaggit")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_Skaggit::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->CLOSE_GOSSIP_MENU();
        p_Creature->AI()->Talk(0);

        if (p_Player->HasQuest(Quests::QUEST_WHAT_WE_GOT) && p_Player->GetQuestObjectiveCounter(273085) != 1)
        {
            p_Player->QuestObjectiveSatisfy(NPCs::NPC_SKAGGIT, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Creature->GetGUID());

            std::list<Creature*>    l_CreaturesList;
            std::vector<Creature*>  l_Creatures;
            p_Creature->GetCreatureListInGrid(l_CreaturesList, 13);

            for (Creature * l_Creature : l_CreaturesList)
                l_Creatures.push_back(l_Creature);

            std::random_shuffle(l_Creatures.begin(), l_Creatures.end());

            for (Creature * l_Creature : l_Creatures)
            {
                if (l_Creature->GetScriptName() != "npc_FrostwallPeon_Dynamic" || !l_Creature->AI())
                    continue;

                l_Creature->AI()->SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING);
            }
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_LadySena::npc_LadySena()
        : CreatureScript("npc_LadySena_Garrison")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_LadySena::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_LADY_SENA, p_Creature->GetGUID());
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_SergeantGrimjaw::npc_SergeantGrimjaw()
        : CreatureScript("npc_SergeantGrimjaw_Garrison")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_SergeantGrimjaw::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_SERGENT_GRIMJAW, p_Creature->GetGUID());
        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_SeniorPeonII::npc_SeniorPeonII()
        : CreatureScript("npc_SeniorPeonII_Garrison")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////


    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_SeniorPeonII::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::QUEST_WHAT_WE_GOT) && p_Player->GetQuestObjectiveCounter(275373) != 1)
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gazlowe needs you.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_SENIOR_PEON_II, p_Creature->GetGUID());
        }

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_SeniorPeonII::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player->HasQuest(Quests::QUEST_WHAT_WE_GOT) && p_Player->GetQuestObjectiveCounter(275373) != 1)
        {
            p_Player->QuestObjectiveSatisfy(NPCs::NPC_SENIOR_PEON_II, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Creature->GetGUID());
            p_Player->CLOSE_GOSSIP_MENU();

            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_SENIOR_PEON_II_WHAT_WE_GOT);

            p_Creature->SetWalk(false);
            p_Creature->LoadPath(Waypoints::MOVE_PATH_SENIOR_PEON_II);
            p_Creature->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
            p_Creature->GetMotionMaster()->Initialize();
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_SeniorPeonII::GetAI(Creature * p_Creature) const
    {
        return new npc_SeniorPeonIIAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_SeniorPeonII::npc_SeniorPeonIIAI::npc_SeniorPeonIIAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called at waypoint reached or point movement finished
    /// @p_Type : Movement Type
    /// @p_ID   : Misc ID
    void npc_SeniorPeonII::npc_SeniorPeonIIAI::MovementInform(uint32 p_Type, uint32 p_ID)
    {
        if (p_Type == WAYPOINT_MOTION_TYPE && p_ID == WaypointsPoints::MOVE_PATH_SENIOR_PEON_II_END)
        {
            m_DelayedOperations.push([this]() -> void
            {
                me->LoadPath(0);
                me->StopMoving();
                me->GetMotionMaster()->MoveIdle();

                if (me->GetWaypointPath() == Waypoints::MOVE_PATH_SENIOR_PEON_II)
                    me->SetFacingTo(1.66f);
                else if (me->GetWaypointPath() == Waypoints::MOVE_PATH_SENIOR_PEON_II_BACK)
                    me->SetFacingTo(1.28f);
            });
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_SeniorPeonII::npc_SeniorPeonIIAI::UpdateAI(const uint32 p_Diff)
    {
        while (!m_DelayedOperations.empty())
        {
            m_DelayedOperations.front()();
            m_DelayedOperations.pop();
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Gazlowe::npc_Gazlowe()
        : CreatureScript("npc_Gazlowe_Garrison")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Gazlowe::GetAI(Creature * p_Creature) const
    {
        return new npc_GazloweAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Gazlowe::npc_GazloweAI::npc_GazloweAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called at waypoint reached or point movement finished
    /// @p_Type : Movement Type
    /// @p_ID   : Misc ID
    void npc_Gazlowe::npc_GazloweAI::MovementInform(uint32 p_Type, uint32 p_ID)
    {
        if (p_Type == POINT_MOTION_TYPE && p_ID == MovePointIDs::MOVE_POINT_GAZLOWE_BARRACK_A)
        {
            m_DelayedOperations.push([this]() -> void
            {
                me->SetFacingTo(4.13f);
                me->GetMotionMaster()->MoveTargetedHome();
            });
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_Gazlowe::npc_GazloweAI::UpdateAI(const uint32 p_Diff)
    {
        while (!m_DelayedOperations.empty())
        {
            m_DelayedOperations.front()();
            m_DelayedOperations.pop();
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GrunLek::npc_GrunLek()
        : CreatureScript("npc_GrunLek_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_GrunLek::GetAI(Creature * p_Creature) const
    {
        return new npc_GrunLekAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GrunLek::npc_GrunLekAI::npc_GrunLekAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[GrunLek::MovePointIDs::Out] = [this]() -> void
        {
            AddTimedDelayedOperation(GrunLek::DestPointDuration::Out, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Out - GrunLek::MovePointIDs::Out][3]); });
        };

        m_OnPointReached[GrunLek::MovePointIDs::Carpet] = [this]() -> void
        {
            AddTimedDelayedOperation(GrunLek::DestPointDuration::Carpet, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Carpet - GrunLek::MovePointIDs::Out][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS, [this]() -> void { me->HandleEmoteCommand(EMOTE_ONESHOT_TALK); });
            AddTimedDelayedOperation(3 * IN_MILLISECONDS, [this]() -> void { me->HandleEmoteCommand(EMOTE_ONESHOT_TALK); });
        };

        m_OnPointReached[GrunLek::MovePointIDs::Chest] = [this]() -> void
        {
            AddTimedDelayedOperation(GrunLek::DestPointDuration::Chest, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Chest - GrunLek::MovePointIDs::Out][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(5 * IN_MILLISECONDS, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        m_OnPointReached[GrunLek::MovePointIDs::Table] = [this]() -> void
        {
            AddTimedDelayedOperation(GrunLek::DestPointDuration::Table, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GrunLek::MovePointLoc[GrunLek::MovePointIDs::Table - GrunLek::MovePointIDs::Out][3]); });

            AddTimedDelayedOperation(1  * IN_MILLISECONDS, [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(9  * IN_MILLISECONDS, [this]() -> void 
            {
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
                me->HandleEmoteCommand(EMOTE_ONESHOT_TALK);
            });
            AddTimedDelayedOperation(11 * IN_MILLISECONDS, [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(19 * IN_MILLISECONDS, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);        });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_GrunLek::npc_GrunLekAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(GrunLek::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = GrunLek::Sequence[m_SequencePosition] - GrunLek::MovePointIDs::Out;
            MoveBuildingRelative(GrunLek::Sequence[m_SequencePosition],   GrunLek::MovePointLoc[l_LocationID][0],
                                                                          GrunLek::MovePointLoc[l_LocationID][1], 
                                                                          GrunLek::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    
    /// Constructor
    npc_OrgekIronhand::npc_OrgekIronhand()
        : CreatureScript("npc_OrgekIronhand_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_OrgekIronhand::GetAI(Creature * p_Creature) const
    {
        return new npc_OrgekIronhandAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_OrgekIronhand::npc_OrgekIronhandAI::npc_OrgekIronhandAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[OrgekIronhand::MovePointIDs::Anvil] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Anvil, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand::MovePointLoc[OrgekIronhand::MovePointIDs::Anvil - OrgekIronhand::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [this]() -> void
            {
                me->LoadEquipment(1, true);
                me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
            });
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Anvil, [this]() -> void
            {
                me->LoadEquipment(0, true);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand::MovePointIDs::Front] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Front, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand::MovePointLoc[OrgekIronhand::MovePointIDs::Front - OrgekIronhand::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Front, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand::MovePointIDs::Forge1] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Forge1, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand::MovePointLoc[OrgekIronhand::MovePointIDs::Forge1 - OrgekIronhand::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                      [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
            });
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Forge1, [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand::MovePointIDs::Forge2] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Forge2, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand::MovePointLoc[OrgekIronhand::MovePointIDs::Forge2 - OrgekIronhand::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                      [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Forge2, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[OrgekIronhand::MovePointIDs::Chest] = [this]() -> void
        {
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Chest, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(OrgekIronhand::MovePointLoc[OrgekIronhand::MovePointIDs::Chest - OrgekIronhand::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                     [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(OrgekIronhand::DestPointDuration::Chest, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_OrgekIronhand::npc_OrgekIronhandAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(OrgekIronhand::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = OrgekIronhand::Sequence[m_SequencePosition] - OrgekIronhand::MovePointIDs::Anvil;
            MoveBuildingRelative(OrgekIronhand::Sequence[m_SequencePosition],   OrgekIronhand::MovePointLoc[l_LocationID][0],
                                                                                OrgekIronhand::MovePointLoc[l_LocationID][1], 
                                                                                OrgekIronhand::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Kinja::npc_Kinja()
        : CreatureScript("npc_Kinja_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_Kinja::GetAI(Creature * p_Creature) const
    {
        return new npc_KinjaAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_Kinja::npc_KinjaAI::npc_KinjaAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[Kinja::MovePointIDs::Anvil] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja::DestPointDuration::Anvil, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja::MovePointLoc[Kinja::MovePointIDs::Anvil - Kinja::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,             [this]() -> void
            {
                me->LoadEquipment(1, true);
                me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
            });
            AddTimedDelayedOperation(Kinja::DestPointDuration::Anvil, [this]() -> void
            {
                me->LoadEquipment(0, true);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[Kinja::MovePointIDs::Front] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja::DestPointDuration::Front, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja::MovePointLoc[Kinja::MovePointIDs::Front - Kinja::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,             [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(Kinja::DestPointDuration::Front, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[Kinja::MovePointIDs::Forge1] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja::DestPointDuration::Forge1, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja::MovePointLoc[Kinja::MovePointIDs::Forge1 - Kinja::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,              [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8);
                me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
            });
            AddTimedDelayedOperation(Kinja::DestPointDuration::Forge1, [this]() -> void
            {
                me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[Kinja::MovePointIDs::Forge2] = [this]() -> void
        {
            AddTimedDelayedOperation(Kinja::DestPointDuration::Forge2, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(Kinja::MovePointLoc[Kinja::MovePointIDs::Forge2 - Kinja::MovePointIDs::Anvil][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,              [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(Kinja::DestPointDuration::Forge2, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_Kinja::npc_KinjaAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(Kinja::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = Kinja::Sequence[m_SequencePosition] - Kinja::MovePointIDs::Anvil;
            MoveBuildingRelative(Kinja::Sequence[m_SequencePosition],   Kinja::MovePointLoc[l_LocationID][0],
                                                                                Kinja::MovePointLoc[l_LocationID][1], 
                                                                                Kinja::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallGrunt::npc_FrostWallGrunt()
        : CreatureScript("npc_FrostwallGrunt_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_FrostWallGrunt::GetAI(Creature * p_Creature) const
    {
        return new npc_FrostWallGruntAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
   
    /// Constructor
    npc_FrostWallGrunt::npc_FrostWallGruntAI::npc_FrostWallGruntAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_FrostWallGrunt::npc_FrostWallGruntAI::UpdateAI(const uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void npc_FrostWallGrunt::npc_FrostWallGruntAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::BuildingID && p_Value == Buildings::DwarvenBunker__WarMill_Level1)
            me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallSmith::npc_FrostWallSmith()
        : CreatureScript("npc_FrostWallSmith_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_FrostWallSmith::GetAI(Creature * p_Creature) const
    {
        return new npc_FrostWallSmithAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostWallSmith::npc_FrostWallSmithAI::npc_FrostWallSmithAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_FrostWallSmith::npc_FrostWallSmithAI::UpdateAI(const uint32 p_Diff)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void npc_FrostWallSmith::npc_FrostWallSmithAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::BuildingID && p_Value == Buildings::DwarvenBunker__WarMill_Level1)
        {
            me->LoadEquipment(1, true);
            me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    /// @p_ScriptName : Script name
    npc_FrostwallPeon::npc_FrostwallPeon(const std::string & p_ScriptName)
        : CreatureScript(p_ScriptName.c_str())
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_FrostwallPeon::GetAI(Creature * p_Creature) const
    {
        return new npc_FrostwallPeonAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_FrostwallPeon::npc_FrostwallPeonAI::npc_FrostwallPeonAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {
        me->SetFlag(UNIT_FIELD_NPC_FLAGS + 1, UNIT_NPC_FLAG2_AI_OBSTACLE);
        me->GetPosition(m_SpawnX, m_SpawnY, m_SpawnZ, m_SpawnO);
        me->SetWalk(true);

        m_IsDynamicScript       = me->GetScriptName() == "npc_FrostwallPeon_Dynamic" && me->GetInstanceScript();
        m_TalkInterval          = 60 + (rand() % 60);
        m_LastTalkTimer         = time(0) - (m_TalkInterval / 2);
        m_WoodCuttingCycleCount = 0;
        m_Initialised           = false;
        m_BackToSpawnPos        = false;
        m_ThreeID               = -1;

        if (m_IsDynamicScript && me->GetInstanceScript()->GetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED))
            SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called at waypoint reached or point movement finished
    /// @p_Type : Movement Type
    /// @p_ID   : Misc ID
    void npc_FrostwallPeon::npc_FrostwallPeonAI::MovementInform(uint32 p_Type, uint32 p_ID)
    {
        if (m_IsDynamicScript)
        {
            if (p_Type == WAYPOINT_MOTION_TYPE)
            {
                if (me->GetWaypointPath() == HordePeonData::MOVE_PATH_WOOD_CUTTING_A)
                {
                    if (p_ID == HordePeonData::MOVE_PATH_WOOD_CUTTING_A_MID)
                        me->RemoveAura(Spells::SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER);
                    else if (p_ID == HordePeonData::MOVE_PATH_WOOD_CUTTING_A_END)
                    {
                        if (!m_BackToSpawnPos)
                        {
                            m_DelayedOperations.push([this]() -> void
                            {
                                me->LoadPath(0);
                                me->StopMoving();
                                me->GetMotionMaster()->MoveIdle();

                                m_WoodCuttingCycleCount = -1;
                                SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING_DISENGAGE);
                                m_WoodCuttingRemainingTime = 200;
                            });
                        }
                        else if (m_BackToSpawnPos)
                        {
                            m_DelayedOperations.push([this]() -> void
                            {
                                me->LoadPath(0);
                                me->StopMoving();
                                me->GetMotionMaster()->MoveIdle();

                                me->LoadEquipment(0, true);

                                me->GetMotionMaster()->MovePoint(HordePeonData::MOVE_POINT_ID_HOME_POSITION, m_SpawnX, m_SpawnY, m_SpawnZ);
                            });
                        }
                    }
                }
            }
            else if (p_Type == POINT_MOTION_TYPE)
            {
                if (p_ID == HordePeonData::MOVE_POINT_ID_HOME_POSITION)
                {
                    m_DelayedOperations.push([this]() -> void
                    {
                        me->SetFacingTo(m_SpawnO);
                        me->AddAura(Spells::SPELL_COMESTIC_SLEEP, me);
                    });
                }
                else if (p_ID >= HordePeonData::MOVE_POINT_ID_THREE_FIRST && p_ID <= HordePeonData::MOVE_POINT_ID_THREE_END)
                {
                    m_DelayedOperations.push([this, p_ID]() -> void
                    {
                        me->SetFacingTo(HordePeonData::FrostWallTreePos[p_ID - HordePeonData::MOVE_POINT_ID_THREE_FIRST][3]);
                        me->LoadEquipment(1, true);
                        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
                    });
                }
                else if (p_ID == HordePeonData::MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A)
                {
                    m_DelayedOperations.push([this]() -> void
                    {
                        me->LoadPath(HordePeonData::MOVE_PATH_WOOD_CUTTING_A);
                        me->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                        me->GetMotionMaster()->Initialize();
                    });
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_FrostwallPeon::npc_FrostwallPeonAI::UpdateAI(const uint32 p_Diff)
    {
        if (m_IsDynamicScript && me->HasAura(MS::Garrison::Spells::SPELL_COMESTIC_SLEEP))
        {
            if ((time(0) - m_LastTalkTimer) > m_TalkInterval)
            {
                me->AI()->Talk(RAND(0,1,2,3));
                m_LastTalkTimer = time(0);
            }
        }

        if (m_IsDynamicScript)
        {
            if (!m_Initialised)
            {
                if (me->GetInstanceScript() && me->GetInstanceScript()->GetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED))
                    SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING);

                m_Initialised = true;
            }

            while (!m_DelayedOperations.empty())
            {
                m_DelayedOperations.front()();
                m_DelayedOperations.pop();
            }

            switch (m_Phase)
            {
                case HordePeonData::PHASE_WOODCUTTING:
                    m_WoodCuttingRemainingTime -= p_Diff;

                    if (m_WoodCuttingRemainingTime < 0)
                        SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING_DISENGAGE);
                    break;

                case HordePeonData::PHASE_WOODCUTTING_DISENGAGE:
                    m_WoodCuttingRemainingTime -= p_Diff;

                    if (m_WoodCuttingRemainingTime < 0 && m_WoodCuttingCycleCount < HordePeonData::WOODCUTTING_MAX_CYLE_COUNT)
                        SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING_REENGAGE);
                    else if (m_WoodCuttingRemainingTime < 0)
                        SetData(CreatureAIDataIDs::PeonWorking, HordePeonData::PHASE_WOODCUTTING_DEPOSIT);

                    break;

                default:
                    break;
            }
        }
    }
    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void npc_FrostwallPeon::npc_FrostwallPeonAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::Builder)
        {
            me->LoadEquipment(1, true);
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
        }
        else if (m_IsDynamicScript)
        {
            if (p_ID == CreatureAIDataIDs::PeonWorking)
            {
                if (p_Value == HordePeonData::PHASE_BACK_TO_HOME && !me->HasAura(Spells::SPELL_COMESTIC_SLEEP))
                {
                    m_BackToSpawnPos = true;
                }
                else
                {
                    me->RemoveAura(Spells::SPELL_COMESTIC_SLEEP);
                    m_Phase             = p_Value;
                    m_Initialised       = true;
                    m_BackToSpawnPos    = false;

                    switch (m_Phase)
                    {
                        case HordePeonData::PHASE_WOODCUTTING:
                            {
                                uint32 l_ThreeID = SelectRandomTree();

                                me->SetWalk(true);
                                me->GetInstanceScript()->SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + l_ThreeID, 1);
                                me->GetMotionMaster()->MovePoint(HordePeonData::MOVE_POINT_ID_THREE_FIRST + l_ThreeID,   HordePeonData::FrostWallTreePos[l_ThreeID][0],
                                                                                                                    HordePeonData::FrostWallTreePos[l_ThreeID][1],
                                                                                                                    HordePeonData::FrostWallTreePos[l_ThreeID][2]);

                                m_ThreeID                   = l_ThreeID;
                                m_WoodCuttingRemainingTime  = HordePeonData::WOODCUTTING_TIME;
                            }
                            break;

                        case HordePeonData::PHASE_WOODCUTTING_DISENGAGE:
                            {
                                me->LoadEquipment(0, true);
                                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
                                me->GetInstanceScript()->SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + m_ThreeID, 0);
                                me->SetFacingTo(me->GetOrientation() + M_PI);

                                m_WoodCuttingRemainingTime = HordePeonData::WOODCUTTING_DISENGAGE_TIME;

                                if (m_WoodCuttingCycleCount >= HordePeonData::WOODCUTTING_MAX_CYLE_COUNT)
                                    m_WoodCuttingRemainingTime += urand(0, 1500);

                                m_ThreeID = -1;
                            }
                            break;

                        case HordePeonData::PHASE_WOODCUTTING_REENGAGE:
                            {
                                uint32 l_ThreeID = SelectRandomTree();

                                me->SetWalk(true);
                                me->GetInstanceScript()->SetData(InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + l_ThreeID, 1);
                                me->GetMotionMaster()->MovePoint(HordePeonData::MOVE_POINT_ID_THREE_FIRST + l_ThreeID,   HordePeonData::FrostWallTreePos[l_ThreeID][0],
                                                                                                                    HordePeonData::FrostWallTreePos[l_ThreeID][1],
                                                                                                                    HordePeonData::FrostWallTreePos[l_ThreeID][2]);

                                m_ThreeID                   = l_ThreeID;
                                m_Phase                     = HordePeonData::PHASE_WOODCUTTING;
                                m_WoodCuttingRemainingTime  = HordePeonData::WOODCUTTING_TIME;
                                m_WoodCuttingCycleCount++;
                            }
                            break;

                        case HordePeonData::PHASE_WOODCUTTING_DEPOSIT:
                            {
                                me->SetWalk(true);
                                me->AddAura(Spells::SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER, me);
                                me->GetMotionMaster()->MovePoint(HordePeonData::MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A, HordePeonData::FrostWallWoodCuttingA[0],
                                                                                                                HordePeonData::FrostWallWoodCuttingA[1],
                                                                                                                HordePeonData::FrostWallWoodCuttingA[2]);
                            }
                            break;

                        default:
                            break;
                    }
                }
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Select a random tree ID
    uint32 npc_FrostwallPeon::npc_FrostwallPeonAI::SelectRandomTree()
    {
        std::vector<uint32> l_Candidates;

        for (uint32 l_I = 0; l_I < 7; l_I++)
        {
            if (l_I == m_ThreeID)
                continue;

            if (me->GetInstanceScript()->GetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + l_I))
                continue;

            l_Candidates.push_back(l_I);
        }

        if (!l_Candidates.size())
            return urand(0, 7);

        auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::shuffle(l_Candidates.begin(), l_Candidates.end(), std::default_random_engine(l_Seed));

        return l_Candidates[0];
    }

}   ///< namespace Garrison
}   ///< namespace MS