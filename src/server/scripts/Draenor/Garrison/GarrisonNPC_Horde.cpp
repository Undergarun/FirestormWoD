////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "GarrisonNPC.hpp"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "GarrisonMgr.hpp"
#include "Sites/GarrisonSiteBase.hpp"
#include "GarrisonScriptData.hpp"

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
    bool npc_Skaggit::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
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
    bool npc_SeniorPeonII::OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 /*p_Sender*/, uint32 /*p_Action*/)
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
    void npc_SeniorPeonII::npc_SeniorPeonIIAI::UpdateAI(const uint32 /*p_Diff*/)
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
            if (Sites::GarrisonSiteBase* l_GarrisonSite = (Sites::GarrisonSiteBase*)me->GetInstanceScript())
            {
                if (Player* l_Player = l_GarrisonSite->GetOwner())
                {
                    l_Player->QuestObjectiveSatisfy(39015, 1, 14);
                    l_Player->QuestObjectiveSatisfy(39012, 1, 14);
                }
            }

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
    void npc_Gazlowe::npc_GazloweAI::UpdateAI(const uint32 /*p_Diff*/)
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
