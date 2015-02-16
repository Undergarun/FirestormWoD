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
    npc_GarrisonCartRope::npc_GarrisonCartRope()
        : CreatureScript("npc_GarrisonCartRope")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_GarrisonCartRope::GetAI(Creature * p_Creature) const
    {
        return new npc_GarrisonCartRopeAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GarrisonCartRope::npc_GarrisonCartRopeAI::npc_GarrisonCartRopeAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {
        HasRope = false;
        me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_DISABLE_TURN);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On reset
    void npc_GarrisonCartRope::npc_GarrisonCartRopeAI::Reset()
    {
        HasRope = false;
    }
    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_GarrisonCartRope::npc_GarrisonCartRopeAI::UpdateAI(const uint32 p_Diff)
    {
        if (me->GetEntry() == NPCs::NPC_GARRISON_ALLIANCE_CART && !HasRope)
        {
            Creature * l_Horse = me->FindNearestCreature(NPCs::NPC_GARRISON_ALLIANCE_HORSE, 2.5f);

            if (l_Horse)
            {
                me->CastSpell(l_Horse, Spells::SPELL_LEFT_ROPE, TRIGGERED_FULL_MASK);
                HasRope = true;
            }
        }
        else if (me->GetEntry() == NPCs::NPC_GARRISON_ALLIANCE_HORSE && !HasRope)
        {
            Creature * l_Cart = me->FindNearestCreature(NPCs::NPC_GARRISON_ALLIANCE_CART, 4.0f);

            if (l_Cart && l_Cart->GetEntry() == NPCs::NPC_GARRISON_ALLIANCE_CART)
            {
                me->CastSpell(l_Cart, Spells::SPELL_RIGHT_ROPE, TRIGGERED_FULL_MASK);
                HasRope = true;
            }
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AssistantBrightstone::npc_AssistantBrightstone()
        : CreatureScript("npc_AssistantBrightstone")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_AssistantBrightstone::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER) && p_Player->GetQuestObjectiveCounter(273535) != 1)
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Time to get back to work.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
        }
        else
        {
            p_Player->SEND_GOSSIP_MENU(RAND(NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_1, NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_2, NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_3), p_Creature->GetGUID());
        }

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_AssistantBrightstone::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player->HasQuest(Quests::QUEST_KEEPING_IT_TOGETHER) && p_Player->GetQuestObjectiveCounter(273535) != 1)
        {
            p_Player->QuestObjectiveSatisfy(NPCs::NPC_ASSISTANT_BRIGHTSTONE, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Creature->GetGUID());

            // @TODO move peon arround

            p_Player->CLOSE_GOSSIP_MENU();
            p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_VindicatorMaraad::npc_VindicatorMaraad()
        : CreatureScript("npc_VindicatorMaraad_GarrA")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player accepts a quest from the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Quest    : Accepted quest
    bool npc_VindicatorMaraad::OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
    {
        if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_PALE_MOONLIGHT)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT);

        return false;
    }
    /// Called when a player completes a quest with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Quest    : Completed quest
    bool npc_VindicatorMaraad::OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
    {
        if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_PALE_MOONLIGHT)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT);

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_ShellyHamby::npc_ShellyHamby()
        : CreatureScript("npc_ShellyHamby")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    bool npc_ShellyHamby::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (!p_Player->HasItemCount(Items::ITEM_SHELLY_HAMBY_REPORT, 1))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gather Shelly's report.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(NPCTexts::NPC_TEXT_SHELLY_HAMBY_TEXT_QUEST, p_Creature->GetGUID());

        return true;
    }
    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_ShellyHamby::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        p_Player->CLOSE_GOSSIP_MENU();
        p_Creature->AI()->Talk(0);

        if (!p_Player->HasItemCount(Items::ITEM_SHELLY_HAMBY_REPORT, 1))
            p_Player->AddItem(Items::ITEM_SHELLY_HAMBY_REPORT, 1);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_BarosAlexsom::npc_BarosAlexsom()
        : CreatureScript("npc_BarosAlexsom")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player accepts a quest from the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Quest    : Accepted quest
    bool npc_BarosAlexsom::OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
    {
        if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_KEEPING_IT_TOGETHER)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_START_CHAT);
        else if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_SHIP_SALVAGE)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_SHIP_SALVAGE_START_CHAT);

        return false;
    }
    /// Called when a player completes a quest with the creature.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Quest    : Completed quest
    bool npc_BarosAlexsom::OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
    {
        if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_ETABLISH_YOUR_GARRISON_A)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_ETABLISH_YOUR_GARRISON_CHAT);
        else if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_KEEPING_IT_TOGETHER)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_END_CHAT);
        else if (p_Quest && p_Quest->GetQuestId() == Quests::QUEST_SHIP_SALVAGE)
            p_Creature->AI()->Talk(CreatureTexts::CREATURE_TEXT_BAROS_SHIP_SALVAGE_END_CHAT);

        return false;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_LunarfallLaborer::npc_LunarfallLaborer()
        : CreatureScript("npc_LunarfallLaborer")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_LunarfallLaborer::GetAI(Creature * p_Creature) const
    {
        return new npc_LunarfallLaborerAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_LunarfallLaborer::npc_LunarfallLaborerAI::npc_LunarfallLaborerAI(Creature * p_Creature)
        : CreatureAI(p_Creature)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// On AI Update
    /// @p_Diff : Time since last update
    void npc_LunarfallLaborer::npc_LunarfallLaborerAI::UpdateAI(const uint32 p_Diff)
    {

    }
    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void npc_LunarfallLaborer::npc_LunarfallLaborerAI::SetData(uint32 p_ID, uint32 p_Value)
    {
        if (p_ID == CreatureAIDataIDs::Builder)
        {
            me->LoadEquipment(1, true);
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
        }
    }


    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GussofForgefire::npc_GussofForgefire()
        : CreatureScript("npc_GussofForgefire_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_GussofForgefire::GetAI(Creature * p_Creature) const
    {
        return new npc_GussofForgefireAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_GussofForgefire::npc_GussofForgefireAI::npc_GussofForgefireAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[GussofForgefire::MovePointIDs::Nothing] = [this]() -> void
        {
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Nothing, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GussofForgefire::MovePointLoc[GussofForgefire::MovePointIDs::Nothing - GussofForgefire::MovePointIDs::Nothing][3]); });
        };

        m_OnPointReached[GussofForgefire::MovePointIDs::Stairs1] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };
        m_OnPointReached[GussofForgefire::MovePointIDs::Stairs2] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };
        m_OnPointReached[GussofForgefire::MovePointIDs::Stairs3] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };

        m_OnPointReached[GussofForgefire::MovePointIDs::Stairs4] = [this]() -> void
        {
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Stairs4, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GussofForgefire::MovePointLoc[GussofForgefire::MovePointIDs::Stairs4 - GussofForgefire::MovePointIDs::Nothing][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                         [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE); });
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Stairs4, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[GussofForgefire::MovePointIDs::Anvil] = [this]() -> void
        {
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Anvil, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GussofForgefire::MovePointLoc[GussofForgefire::MovePointIDs::Anvil - GussofForgefire::MovePointIDs::Nothing][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                       [this]() -> void
            {
                me->LoadEquipment(1, true);
                me->HandleEmoteCommand(EMOTE_STATE_WORK_CHOPWOOD_GARR);
            });
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Anvil, [this]() -> void
            {
                me->LoadEquipment(0, true);
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[GussofForgefire::MovePointIDs::Canon] = [this]() -> void
        {
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Canon, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(GussofForgefire::MovePointLoc[GussofForgefire::MovePointIDs::Canon - GussofForgefire::MovePointIDs::Nothing][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                       [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE); });
            AddTimedDelayedOperation(GussofForgefire::DestPointDuration::Canon, [this]() -> void
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
    void npc_GussofForgefire::npc_GussofForgefireAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(GussofForgefire::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = GussofForgefire::Sequence[m_SequencePosition] - GussofForgefire::MovePointIDs::Nothing;
            MoveBuildingRelative( GussofForgefire::Sequence[m_SequencePosition],  GussofForgefire::MovePointLoc[l_LocationID][0],
                                                                                  GussofForgefire::MovePointLoc[l_LocationID][1],
                                                                                  GussofForgefire::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_KristenStoneforge::npc_KristenStoneforge()
        : CreatureScript("npc_KristenStoneforge_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_KristenStoneforge::GetAI(Creature * p_Creature) const
    {
        return new npc_KristenStoneforgAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_KristenStoneforge::npc_KristenStoneforgAI::npc_KristenStoneforgAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[KristenStoneforge::MovePointIDs::Table] = [this]() -> void
        {
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::Table, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(KristenStoneforge::MovePointLoc[KristenStoneforge::MovePointIDs::Table - KristenStoneforge::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                         [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::Table, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[KristenStoneforge::MovePointIDs::Stairs1] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };
        m_OnPointReached[KristenStoneforge::MovePointIDs::Stairs2] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };
        m_OnPointReached[KristenStoneforge::MovePointIDs::Stairs3] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };
        m_OnPointReached[KristenStoneforge::MovePointIDs::Stairs4] = [this]() -> void { m_DelayedOperations.push([this]() -> void { DoNextSequenceAction(); }); };

        m_OnPointReached[KristenStoneforge::MovePointIDs::Chest] = [this]() -> void
        {
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::Chest, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(KristenStoneforge::MovePointLoc[KristenStoneforge::MovePointIDs::Chest - KristenStoneforge::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                         [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::Chest, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        m_OnPointReached[KristenStoneforge::MovePointIDs::UpTable] = [this]() -> void
        {
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::UpTable, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(KristenStoneforge::MovePointLoc[KristenStoneforge::MovePointIDs::UpTable - KristenStoneforge::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                           [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::UpTable, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[KristenStoneforge::MovePointIDs::CanonBalls] = [this]() -> void
        {
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::CanonBalls, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(KristenStoneforge::MovePointLoc[KristenStoneforge::MovePointIDs::CanonBalls - KristenStoneforge::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                              [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(KristenStoneforge::DestPointDuration::CanonBalls, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_KristenStoneforge::npc_KristenStoneforgAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(KristenStoneforge::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = KristenStoneforge::Sequence[m_SequencePosition] - KristenStoneforge::MovePointIDs::Table;
            MoveBuildingRelative( KristenStoneforge::Sequence[m_SequencePosition],  KristenStoneforge::MovePointLoc[l_LocationID][0],
                                                                                    KristenStoneforge::MovePointLoc[l_LocationID][1],
                                                                                    KristenStoneforge::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_JonathanStephens::npc_JonathanStephens()
        : CreatureScript("npc_JonathanStephens_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_JonathanStephens::GetAI(Creature * p_Creature) const
    {
        return new npc_JonathanStephensAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_JonathanStephens::npc_JonathanStephensAI::npc_JonathanStephensAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[JonathanStephens::MovePointIDs::Table] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Table, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::Table - JonathanStephens::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                        [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Table, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[JonathanStephens::MovePointIDs::Chair] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Chair, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::Chair - JonathanStephens::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                        [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_SIT_CHAIR_LOW);    });
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Chair, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[JonathanStephens::MovePointIDs::Armory] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Armory, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::Armory - JonathanStephens::MovePointIDs::Table][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                         [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_USE_STANDING_NO_SHEATHE);    });
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Armory, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[JonathanStephens::MovePointIDs::Middle] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Middle, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::Middle - JonathanStephens::MovePointIDs::Table][3]); });

        };

        m_OnPointReached[JonathanStephens::MovePointIDs::Middle2] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Middle2, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::Middle2 - JonathanStephens::MovePointIDs::Table][3]); });

        };

        m_OnPointReached[JonathanStephens::MovePointIDs::BackBed] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::BackBed, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::BackBed - JonathanStephens::MovePointIDs::Table][3]); });

        };

        m_OnPointReached[JonathanStephens::MovePointIDs::Shields] = [this]() -> void
        {
            AddTimedDelayedOperation(JonathanStephens::DestPointDuration::Shields, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(JonathanStephens::MovePointLoc[JonathanStephens::MovePointIDs::Shields - JonathanStephens::MovePointIDs::Table][3]); });

        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_JonathanStephens::npc_JonathanStephensAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(JonathanStephens::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = JonathanStephens::Sequence[m_SequencePosition] - JonathanStephens::MovePointIDs::Table;
            MoveBuildingRelative(JonathanStephens::Sequence[m_SequencePosition],   JonathanStephens::MovePointLoc[l_LocationID][0],
                                                                                   JonathanStephens::MovePointLoc[l_LocationID][1],
                                                                                   JonathanStephens::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77359 - Auria Irondreamer                                         ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AuriaIrondreamer::npc_AuriaIrondreamer()
        : CreatureScript("npc_AuriaIrondreamer_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_AuriaIrondreamer::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I need you to do something for me.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_AuriaIrondreamer::GetAI(Creature * p_Creature) const
    {
        return new npc_AuriaIrondreamerAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_AuriaIrondreamer::npc_AuriaIrondreamerAI::npc_AuriaIrondreamerAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[AuriaIrondreamer::MovePointIDs::ForgeOre] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::ForgeOre, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer::MovePointLoc[AuriaIrondreamer::MovePointIDs::ForgeOre - AuriaIrondreamer::MovePointIDs::ForgeOre][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                           [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::ForgeOre, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        m_OnPointReached[AuriaIrondreamer::MovePointIDs::ForgeFront] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::ForgeFront, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer::MovePointLoc[AuriaIrondreamer::MovePointIDs::ForgeFront - AuriaIrondreamer::MovePointIDs::ForgeFront][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                             [this]() -> void { me->HandleEmoteCommand(EMOTE_STATE_READ_AND_TALK);    });
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::ForgeFront, [this]() -> void
            {
                me->HandleEmoteCommand(0);
                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, 0);
            });
        };

        m_OnPointReached[AuriaIrondreamer::MovePointIDs::Workorder] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::Workorder, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer::MovePointLoc[AuriaIrondreamer::MovePointIDs::Workorder - AuriaIrondreamer::MovePointIDs::Workorder][3]); });
        };

        m_OnPointReached[AuriaIrondreamer::MovePointIDs::WorkorderDown] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::WorkorderDown, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer::MovePointLoc[AuriaIrondreamer::MovePointIDs::WorkorderDown - AuriaIrondreamer::MovePointIDs::WorkorderDown][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                                [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::WorkorderDown, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        m_OnPointReached[AuriaIrondreamer::MovePointIDs::WorkorderFront] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::WorkorderFront, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer::MovePointLoc[AuriaIrondreamer::MovePointIDs::ForgeOre - AuriaIrondreamer::MovePointIDs::WorkorderFront][3]); });
        };

        m_OnPointReached[AuriaIrondreamer::MovePointIDs::ChestLeft] = [this]() -> void
        {
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::ChestLeft, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(AuriaIrondreamer::MovePointLoc[AuriaIrondreamer::MovePointIDs::ForgeOre - AuriaIrondreamer::MovePointIDs::ChestLeft][3]); });

            AddTimedDelayedOperation(0 * IN_MILLISECONDS,                            [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 8); });
            AddTimedDelayedOperation(AuriaIrondreamer::DestPointDuration::ChestLeft, [this]() -> void { me->SetUInt32Value(UNIT_FIELD_ANIM_TIER, 0); });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_AuriaIrondreamer::npc_AuriaIrondreamerAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(JonathanStephens::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = AuriaIrondreamer::Sequence[m_SequencePosition] - AuriaIrondreamer::MovePointIDs::ForgeOre;
            MoveBuildingRelative(AuriaIrondreamer::Sequence[m_SequencePosition],   AuriaIrondreamer::MovePointLoc[l_LocationID][0],
                                                                                   AuriaIrondreamer::MovePointLoc[l_LocationID][1],
                                                                                   AuriaIrondreamer::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77792 - Yulia Samras                                              ////
    //////////////////////////////////////////////////////////////////////////

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
    bool npc_YuliaSamras::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player->HasQuest(Quests::Alliance_YourFirstBlacksmithingWorkOrder) || p_Player->IsQuestRewarded(Quests::Alliance_YourFirstBlacksmithingWorkOrder))
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to place an order.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

        p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_YuliaSamras::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_YuliaSamras::GetAI(Creature * p_Creature) const
    {
        return new npc_YuliaSamrasAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_YuliaSamras::npc_YuliaSamrasAI::npc_YuliaSamrasAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature), m_SequencePosition(0xFF)
    {
        SetAIObstacleManagerEnabled(true);

        m_OnPointReached[YuliaSamras::MovePointIDs::Workorder] = [this]() -> void
        {
            AddTimedDelayedOperation(YuliaSamras::DestPointDuration::Workorder, [this]() -> void { DoNextSequenceAction(); });
            m_DelayedOperations.push([this]() -> void { SetFacingBuildingRelative(YuliaSamras::MovePointLoc[YuliaSamras::MovePointIDs::Workorder - YuliaSamras::MovePointIDs::Workorder][3]); });
        };

        DoNextSequenceAction();
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Do next sequence element
    void npc_YuliaSamras::npc_YuliaSamrasAI::DoNextSequenceAction()
    {
        if (m_SequencePosition >= sizeof(YuliaSamras::Sequence))
            m_SequencePosition = 0;

        m_DelayedOperations.push([this]() -> void
        {
            me->SetWalk(true);

            uint32 l_LocationID = YuliaSamras::Sequence[m_SequencePosition] -   YuliaSamras::MovePointIDs::Workorder;
            MoveBuildingRelative(YuliaSamras::Sequence[m_SequencePosition],     YuliaSamras::MovePointLoc[l_LocationID][0],
                                                                                YuliaSamras::MovePointLoc[l_LocationID][1],
                                                                                YuliaSamras::MovePointLoc[l_LocationID][2]);

            m_SequencePosition++;
        });
    }

    //////////////////////////////////////////////////////////////////////////
    /// 77730 - Timothy Leens                                             ////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TimothyLeens::npc_TimothyLeens()
        : CreatureScript("npc_TimothyLeens_Garr")
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player opens a gossip dialog with the GameObject.
    /// @p_Player     : Source player instance
    /// @p_Creature   : Target GameObject instance
    bool npc_TimothyLeens::OnGossipHello(Player * p_Player, Creature * p_Creature)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
        {
            uint32 l_BuildingID = reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->GetBuildingID();

            if (l_BuildingID && sGarrisonBuildingManager->MatchsConditionsForBuilding(l_BuildingID, p_Player))
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Can you refine this draenic stone into ore for me?", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        }

        p_Player->SEND_GOSSIP_MENU(NPCTexts::ThimothyLeens, p_Creature->GetGUID());

        return true;
    }

    /// Called when a player selects a gossip item in the creature's gossip menu.
    /// @p_Player   : Source player instance
    /// @p_Creature : Target creature instance
    /// @p_Sender   : Sender menu
    /// @p_Action   : Action
    bool npc_TimothyLeens::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action)
    {
        if (p_Player && p_Creature && p_Creature->AI() && p_Creature->GetScriptName() == GetName())
            reinterpret_cast<GarrisonNPCAI*>(p_Creature->AI())->SendShipmentCrafterUI(p_Player);

        return true;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a CreatureAI object is needed for the creature.
    /// @p_Creature : Target creature instance
    CreatureAI * npc_TimothyLeens::GetAI(Creature * p_Creature) const
    {
        return new npc_TimothyLeensAI(p_Creature);
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    npc_TimothyLeens::npc_TimothyLeensAI::npc_TimothyLeensAI(Creature * p_Creature)
        : GarrisonNPCAI(p_Creature)
    {
        SetAIObstacleManagerEnabled(true);
    }

}   ///< namespace Garrison
}   ///< namespace MS