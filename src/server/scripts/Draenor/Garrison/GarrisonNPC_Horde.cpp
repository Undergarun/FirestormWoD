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
    namespace HordePeonData 
    {
        enum
        {
            MOVE_POINT_ID_HOME_POSITION         = 100,
            MOVE_POINT_ID_THREE_FIRST           = 101,
            MOVE_POINT_ID_THREE_END             = 111,
            MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A = 112,

            MOVE_PATH_FROSTWALL_A                   = 600000,
            MOVE_PATH_FROSTWALL_A_POINT_NEAR_HOME   = 1,
            MOVE_PATH_FROSTWALL_B                   = 600001,
            MOVE_PATH_FROSTWALL_B_POINT_NEAR_HOME   = 1,
            MOVE_PATH_WOOD_CUTTING_A                = 600002,
            MOVE_PATH_WOOD_CUTTING_A_MID            = 10,
            MOVE_PATH_WOOD_CUTTING_A_END            = 20,

            PHASE_BACK_TO_HOME          = 0,
            PHASE_WOODCUTTING           = 1,
            PHASE_WOODCUTTING_DISENGAGE = 2,
            PHASE_WOODCUTTING_REENGAGE  = 3,
            PHASE_WOODCUTTING_DEPOSIT   = 4,

            WOODCUTTING_TIME            = 30 * IN_MILLISECONDS,
            WOODCUTTING_DISENGAGE_TIME  =  2 * IN_MILLISECONDS,

            WOODCUTTING_MAX_CYLE_COUNT  = 1,//4

        };

        float FrostWallTreePos[][4] = {
            { 5543.7100f, 4600.020f, 142.000f, 2.8211f },
            { 5558.2724f, 4608.380f, 138.500f, 1.8433f },
            { 5550.3200f, 4594.158f, 136.795f, 3.7440f },
            { 5548.6600f, 4583.240f, 136.640f, 5.4320f },
            { 5544.5209f, 4600.591f, 141.500f, 2.8604f },
            { 5541.5766f, 4584.437f, 137.900f, 1.7134f },
            { 5535.4940f, 4567.104f, 137.480f, 4.0602f },
            { 5561.4848f, 4616.658f, 139.720f, 2.8347f }
        };

        float FrostWallWoodCuttingA[3] = {
            5572.927f, 4602.003f, 136.58f
        };

    }   ///< namespace HordePeonData

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

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
            p_Player->QuestObjectiveSatisfy(80225, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Creature->GetGUID());

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

                l_Creature->AI()->SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING);
            }
        }

        return true;
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

        m_BackToSpawnPos = false;

        m_IsDynamicScript       = me->GetScriptName() == "npc_FrostwallPeon_Dynamic" && me->GetInstanceScript();
        m_TalkInterval          = 60 + (rand() % 60);
        m_LastTalkTimer         = time(0) - (m_TalkInterval / 2);
        m_WoodCuttingCycleCount = 0;
        m_Initialised           = false;
        m_ThreeID               = -1;

        if (m_IsDynamicScript && me->GetInstanceScript()->GetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED))
            SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING);
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
                if (    (me->GetWaypointPath() == HordePeonData::MOVE_PATH_FROSTWALL_A && p_ID == HordePeonData::MOVE_PATH_FROSTWALL_A_POINT_NEAR_HOME && m_Phase == HordePeonData::PHASE_BACK_TO_HOME)
                    ||  (me->GetWaypointPath() == HordePeonData::MOVE_PATH_FROSTWALL_B && p_ID == HordePeonData::MOVE_PATH_FROSTWALL_B_POINT_NEAR_HOME && m_Phase == HordePeonData::PHASE_BACK_TO_HOME))
                {
                    m_DelayedOperations.push([this]() -> void
                    {
                        m_BackToSpawnPos = true;
                        me->GetMotionMaster()->MovePoint(HordePeonData::MOVE_POINT_ID_HOME_POSITION, m_SpawnX, m_SpawnY, m_SpawnZ);
                    });
                }
                else if (me->GetWaypointPath() == HordePeonData::MOVE_PATH_WOOD_CUTTING_A)
                {
                    if (p_ID == HordePeonData::MOVE_PATH_WOOD_CUTTING_A_MID)
                        me->RemoveAura(Spells::SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER);
                    else if (p_ID == HordePeonData::MOVE_PATH_WOOD_CUTTING_A_END)
                    {
                        m_DelayedOperations.push([this]() -> void
                        {
                            me->LoadPath(0);
                            me->StopMoving();
                            me->GetMotionMaster()->MoveIdle();

                            m_WoodCuttingCycleCount = -1;
                            SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING_DISENGAGE);
                            m_WoodCuttingRemainingTime = 200;
                        });
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
                    SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING);

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
                        SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING_DISENGAGE);
                    break;

                case HordePeonData::PHASE_WOODCUTTING_DISENGAGE:
                    m_WoodCuttingRemainingTime -= p_Diff;

                    if (m_WoodCuttingRemainingTime < 0 && m_WoodCuttingCycleCount < HordePeonData::WOODCUTTING_MAX_CYLE_COUNT)
                        SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING_REENGAGE);
                    else if (m_WoodCuttingRemainingTime < 0)
                        SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, HordePeonData::PHASE_WOODCUTTING_DEPOSIT);

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
        if (p_ID == GARRISON_CREATURE_AI_DATA_BUILDER)
        {
            me->LoadEquipment(1, true);
            me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
        }
        else if (m_IsDynamicScript)
        {
            if (p_ID == GARRISON_CREATURE_AI_DATA_PEON_WORKING)
            {
                if (p_Value == 0)
                {
                    me->GetMotionMaster()->MovePath(0, false);
                    m_Phase = p_Value;
                }
                else
                {
                    me->RemoveAura(MS::Garrison::Spells::SPELL_COMESTIC_SLEEP);
                    m_Phase = p_Value;
                    m_Initialised = true;

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