#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "GameObjectAI.h"
#include "Spell.h"
#include "Garrison.h"
#include "GarrisonScriptData.hpp"

#include <random>

/// Garrison Ford
class npc_GarrisonFord : public CreatureScript
{
    public:
        /// Constructor
        npc_GarrisonFord()
            : CreatureScript("npc_GarrisonFord")
        { 

        }

        /// Called when a player opens a gossip dialog with the creature.
        bool OnGossipHello(Player * p_Player, Creature * p_Creature) override
        {
            if (!p_Player->GetGarrison())
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Create me a garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            else
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Delete my garrison.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
            
            p_Player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, p_Creature->GetGUID());

            return true;
        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            if (!p_Player->GetGarrison())
            {
                p_Player->CLOSE_GOSSIP_MENU();
                p_Player->CreateGarrison();

                uint32 l_MovieID    = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->CreationMovie;
                uint32 l_MapID      = p_Player->GetGarrison()->GetGarrisonSiteLevelEntry()->MapID;
                uint32 l_TeamID     = p_Player->GetTeamId();

                p_Player->AddMovieDelayedTeleport(l_MovieID, l_MapID,   MS::Garrison::gGarrisonCreationCoords[l_TeamID][0],
                                                                        MS::Garrison::gGarrisonCreationCoords[l_TeamID][1],
                                                                        MS::Garrison::gGarrisonCreationCoords[l_TeamID][2],
                                                                        MS::Garrison::gGarrisonCreationCoords[l_TeamID][3]);
                p_Player->SendMovieStart(l_MovieID);

                if (l_TeamID == TEAM_ALLIANCE)
                {
                    p_Player->AddQuest(sObjectMgr->GetQuestTemplate(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A), p_Creature);
                    p_Player->CompleteQuest(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A);
                }
                else if (l_TeamID == TEAM_HORDE)
                {
                    p_Player->AddQuest(sObjectMgr->GetQuestTemplate(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_H), p_Creature);
                    p_Player->CompleteQuest(MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_H);
                }

                /// HACK until shadowmoon quest are done : add follower Qiana Moonshadow / Olin Umberhide
                p_Player->GetGarrison()->AddFollower(34);
                p_Player->GetGarrison()->AddFollower(89);
                p_Player->GetGarrison()->AddFollower(92);

                /// HACK until quest : add barracks plan
                p_Player->GetGarrison()->LearnBlueprint(26);
            }
            else
            {
                if (p_Player->GetCurrency(GARRISON_CURRENCY_ID, false))
                    p_Player->ModifyCurrency(GARRISON_CURRENCY_ID, -(int32)p_Player->GetCurrency(GARRISON_CURRENCY_ID, false));

                p_Player->DeleteGarrison();
            }

            return true;
        }
};

/// Garrison ROPE owner
class npc_GarrisonCartRope : public CreatureScript
{
    public:
        /// Constructor
        npc_GarrisonCartRope() 
            : CreatureScript("npc_GarrisonCartRope")
        { 

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_GarrisonCartRopeAI(p_Creature);
        }

        struct npc_GarrisonCartRopeAI : public CreatureAI
        {
            /// Constructor
            npc_GarrisonCartRopeAI(Creature * p_Creature) 
                : CreatureAI(p_Creature)
            {
                HasRope = false;
                me->SetFlag(UNIT_FIELD_FLAGS2, UNIT_FLAG2_DISABLE_TURN);
            }

            void Reset() override
            {
                HasRope = false;
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                if (me->GetEntry() == MS::Garrison::NPCs::NPC_GARRISON_ALLIANCE_CART && !HasRope)
                {
                    Creature * l_Horse = me->FindNearestCreature(MS::Garrison::NPCs::NPC_GARRISON_ALLIANCE_HORSE, 2.5f);

                    if (l_Horse)
                    {
                        me->CastSpell(l_Horse, MS::Garrison::Spells::SPELL_LEFT_ROPE, TRIGGERED_FULL_MASK);
                        HasRope = true;
                    }
                }
                else if (me->GetEntry() == MS::Garrison::NPCs::NPC_GARRISON_ALLIANCE_HORSE && !HasRope)
                {
                    Creature * l_Cart = me->FindNearestCreature(MS::Garrison::NPCs::NPC_GARRISON_ALLIANCE_CART, 4.0f);

                    if (l_Cart && l_Cart->GetEntry() == MS::Garrison::NPCs::NPC_GARRISON_ALLIANCE_CART)
                    {
                        me->CastSpell(l_Cart, MS::Garrison::Spells::SPELL_RIGHT_ROPE, TRIGGERED_FULL_MASK);
                        HasRope = true;
                    }
                }
            }

            bool HasRope;
        };
};

/// Garrison assistant brightstone
class npc_AssistantBrightstone : public CreatureScript
{
    public:
        /// Constructor
        npc_AssistantBrightstone()
            : CreatureScript("npc_AssistantBrightstone")
        {

        }

        /// Called when a player opens a gossip dialog with the creature.
        bool OnGossipHello(Player * p_Player, Creature * p_Creature) override
        {
            if (p_Player->HasQuest(MS::Garrison::Quests::QUEST_KEEPING_IT_TOGETHER) && p_Player->GetQuestObjectiveCounter(273535) != 1)
            {
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Time to get back to work.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                p_Player->SEND_GOSSIP_MENU(MS::Garrison::NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
            }
            else
            {
                p_Player->SEND_GOSSIP_MENU(RAND(MS::Garrison::NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_1, MS::Garrison::NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_2, MS::Garrison::NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_3), p_Creature->GetGUID());
            }

            return true;
        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            if (p_Player->HasQuest(MS::Garrison::Quests::QUEST_KEEPING_IT_TOGETHER) && p_Player->GetQuestObjectiveCounter(273535) != 1)
            {
                p_Player->QuestObjectiveSatisfy(84455, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Creature->GetGUID());

                // @TODO move peon arround

                p_Player->CLOSE_GOSSIP_MENU();
                p_Player->SEND_GOSSIP_MENU(MS::Garrison::NPCTexts::NPC_TEXT_ASSISTANT_BRIGHTSTONE_TEXT_QUEST, p_Creature->GetGUID());
            }

            return true;
        }

};

/// Garrison shelly hamby
class npc_ShellyHamby : public CreatureScript
{
    public:
        /// Constructor
        npc_ShellyHamby()
            : CreatureScript("npc_ShellyHamby")
        {

        }

        /// Called when a player opens a gossip dialog with the creature.
        bool OnGossipHello(Player * p_Player, Creature * p_Creature) override
        {
            if (!p_Player->HasItemCount(MS::Garrison::Items::ITEM_SHELLY_HAMBY_REPORT, 1))
                p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Gather Shelly's report.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);

            p_Player->SEND_GOSSIP_MENU(MS::Garrison::NPCTexts::NPC_TEXT_SHELLY_HAMBY_TEXT_QUEST, p_Creature->GetGUID());

            return true;
        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            p_Player->CLOSE_GOSSIP_MENU();
            p_Creature->AI()->Talk(0);

            if (!p_Player->HasItemCount(MS::Garrison::Items::ITEM_SHELLY_HAMBY_REPORT, 1))
                p_Player->AddItem(MS::Garrison::Items::ITEM_SHELLY_HAMBY_REPORT, 1);

            return true;
        }

};

/// Garrison baros alexston
class npc_BarosAlexsom : public CreatureScript
{
    public:
        /// Constructor
        npc_BarosAlexsom()
            : CreatureScript("npc_BarosAlexsom")
        {

        }

        /// Called when a player accepts a quest from the creature.
        virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_KEEPING_IT_TOGETHER)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_START_CHAT);
            else if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_SHIP_SALVAGE)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_BAROS_SHIP_SALVAGE_START_CHAT);

            return false;
        }
        /// Called when a player completes a quest with the creature.
        virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_ETABLISH_YOUR_GARRISON_A)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_BAROS_ETABLISH_YOUR_GARRISON_CHAT);
            else if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_KEEPING_IT_TOGETHER)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_BAROS_KEEPING_IT_TOGETHER_END_CHAT);
            else if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_SHIP_SALVAGE)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_BAROS_SHIP_SALVAGE_END_CHAT);

            return false;
        }

};

/// Garrison vindicator maraad
class npc_VindicatorMaraad : public CreatureScript
{
    public:
        /// Constructor
        npc_VindicatorMaraad()
            : CreatureScript("npc_VindicatorMaraad")
        {

        }

        /// Called when a player accepts a quest from the creature.
        virtual bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_PALE_MOONLIGHT)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_START_CHAT);

            return false;
        }
        /// Called when a player completes a quest with the creature.
        virtual bool OnQuestComplete(Player * p_Player, Creature * p_Creature, const Quest * p_Quest) override
        {
            if (p_Quest && p_Quest->GetQuestId() == MS::Garrison::Quests::QUEST_PALE_MOONLIGHT)
                p_Creature->AI()->Talk(MS::Garrison::CreatureTexts::CREATURE_TEXT_VINDICATOR_MARAAD_PALE_MOONLIGHT_END_CHAT);

            return false;
        }

};

/// Lunarfall Laborer
class npc_LunarfallLaborer : public CreatureScript
{
    public:
        /// Constructor
        npc_LunarfallLaborer()
            : CreatureScript("npc_LunarfallLaborer")
        {

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_LunarfallLaborerAI(p_Creature);
        }

        struct npc_LunarfallLaborerAI : public CreatureAI
        {
            /// Constructor
            npc_LunarfallLaborerAI(Creature * p_Creature)
                : CreatureAI(p_Creature)
            {

            }

            void UpdateAI(const uint32 p_Diff) override
            {

            }

            void SetData(uint32 p_ID, uint32 p_Value) override
            {
                if (p_ID == GARRISON_CREATURE_AI_DATA_BUILDER)
                {
                    me->LoadEquipment(1, true);
                    me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
                }
            }
        };
};

namespace PeonData {

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

}

/// Frostwall Peon
class npc_FrostwallPeon : public CreatureScript
{
    public:
        /// Constructor
        npc_FrostwallPeon(const std::string & p_ScriptName)
            : CreatureScript(p_ScriptName.c_str())
        {

        }

        CreatureAI* GetAI(Creature * p_Creature) const
        {
            return new npc_FrostwallPeonAI(p_Creature);
        }

        struct npc_FrostwallPeonAI : public CreatureAI
        {
            /// Constructor
            npc_FrostwallPeonAI(Creature * p_Creature)
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
                    SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING);
            }

            void UpdateAI(const uint32 p_Diff) override
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
                        if (me->GetInstanceScript() && me->GetInstanceScript()->GetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_PEON_ENABLED))
                            SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING);

                        m_Initialised = true;
                    }

                    while (!m_DelayedOperations.empty())
                    {
                        m_DelayedOperations.front()();
                        m_DelayedOperations.pop();
                    }

                    switch (m_Phase)
                    {
                        case PeonData::PHASE_WOODCUTTING:
                            m_WoodCuttingRemainingTime -= p_Diff;

                            if (m_WoodCuttingRemainingTime < 0)
                                SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING_DISENGAGE);
                            break;

                        case PeonData::PHASE_WOODCUTTING_DISENGAGE:
                            m_WoodCuttingRemainingTime -= p_Diff;

                            if (m_WoodCuttingRemainingTime < 0 && m_WoodCuttingCycleCount < PeonData::WOODCUTTING_MAX_CYLE_COUNT)
                                SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING_REENGAGE);
                            else if (m_WoodCuttingRemainingTime < 0)
                                SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING_DEPOSIT);

                            break;

                        default:
                            break;
                    }
                }
            }

            // Called at waypoint reached or point movement finished
            virtual void MovementInform(uint32 p_Type, uint32 p_ID)
            {
                if (m_IsDynamicScript)
                {
                    if (p_Type == WAYPOINT_MOTION_TYPE)
                    {
                        if (    (me->GetWaypointPath() == PeonData::MOVE_PATH_FROSTWALL_A && p_ID == PeonData::MOVE_PATH_FROSTWALL_A_POINT_NEAR_HOME && m_Phase == PeonData::PHASE_BACK_TO_HOME)
                            ||  (me->GetWaypointPath() == PeonData::MOVE_PATH_FROSTWALL_B && p_ID == PeonData::MOVE_PATH_FROSTWALL_B_POINT_NEAR_HOME && m_Phase == PeonData::PHASE_BACK_TO_HOME))
                        {
                            m_DelayedOperations.push([this]() -> void
                            {
                                m_BackToSpawnPos = true;
                                me->GetMotionMaster()->MovePoint(PeonData::MOVE_POINT_ID_HOME_POSITION, m_SpawnX, m_SpawnY, m_SpawnZ);
                            });
                        }
                        else if (me->GetWaypointPath() == PeonData::MOVE_PATH_WOOD_CUTTING_A)
                        {
                            if (p_ID == PeonData::MOVE_PATH_WOOD_CUTTING_A_MID)
                                me->RemoveAura(MS::Garrison::Spells::SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER);
                            else if (p_ID == PeonData::MOVE_PATH_WOOD_CUTTING_A_END)
                            {
                                m_DelayedOperations.push([this]() -> void
                                {
                                    me->LoadPath(0);
                                    me->StopMoving();
                                    me->GetMotionMaster()->MoveIdle();

                                    m_WoodCuttingCycleCount = -1;
                                    SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING_DISENGAGE);
                                    m_WoodCuttingRemainingTime = 200;
                                });
                            }
                        }
                    }
                    else if (p_Type == POINT_MOTION_TYPE)
                    {
                        if (p_ID == PeonData::MOVE_POINT_ID_HOME_POSITION)
                        {
                            m_DelayedOperations.push([this]() -> void
                            {
                                me->SetFacingTo(m_SpawnO);
                                me->AddAura(MS::Garrison::Spells::SPELL_COMESTIC_SLEEP, me);
                            });
                        }
                        else if (p_ID >= PeonData::MOVE_POINT_ID_THREE_FIRST && p_ID <= PeonData::MOVE_POINT_ID_THREE_END)
                        {
                            m_DelayedOperations.push([this, p_ID]() -> void
                            {
                                me->SetFacingTo(PeonData::FrostWallTreePos[p_ID - PeonData::MOVE_POINT_ID_THREE_FIRST][3]);
                                me->LoadEquipment(1, true);
                                me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_STATE_WORK);
                            });
                        }
                        else if (p_ID == PeonData::MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A)
                        {
                            m_DelayedOperations.push([this]() -> void
                            {
                                me->LoadPath(PeonData::MOVE_PATH_WOOD_CUTTING_A);
                                me->SetDefaultMovementType(WAYPOINT_MOTION_TYPE);
                                me->GetMotionMaster()->Initialize();
                            });
                        }
                    }
                }
            }
            //34824
            void SetData(uint32 p_ID, uint32 p_Value) override
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
                                case PeonData::PHASE_WOODCUTTING:
                                    {
                                        uint32 l_ThreeID = SelectRandomTree();

                                        me->SetWalk(true);
                                        me->GetInstanceScript()->SetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + l_ThreeID, 1);
                                        me->GetMotionMaster()->MovePoint(PeonData::MOVE_POINT_ID_THREE_FIRST + l_ThreeID,   PeonData::FrostWallTreePos[l_ThreeID][0],
                                                                                                                            PeonData::FrostWallTreePos[l_ThreeID][1],
                                                                                                                            PeonData::FrostWallTreePos[l_ThreeID][2]);

                                        m_ThreeID                   = l_ThreeID;
                                        m_WoodCuttingRemainingTime  = PeonData::WOODCUTTING_TIME;
                                    }
                                    break;

                                case PeonData::PHASE_WOODCUTTING_DISENGAGE:
                                    {
                                        me->LoadEquipment(0, true);
                                        me->SetUInt32Value(UNIT_FIELD_EMOTE_STATE, EMOTE_ONESHOT_NONE);
                                        me->GetInstanceScript()->SetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + m_ThreeID, 0);
                                        me->SetFacingTo(me->GetOrientation() + M_PI);

                                        m_WoodCuttingRemainingTime = PeonData::WOODCUTTING_DISENGAGE_TIME;

                                        if (m_WoodCuttingCycleCount >= PeonData::WOODCUTTING_MAX_CYLE_COUNT)
                                            m_WoodCuttingRemainingTime += urand(0, 1500);

                                        m_ThreeID = -1;
                                    }
                                    break;

                                case PeonData::PHASE_WOODCUTTING_REENGAGE:
                                    {
                                        uint32 l_ThreeID = SelectRandomTree();

                                        me->SetWalk(true);
                                        me->GetInstanceScript()->SetData(MS::Garrison::InstanceDataIDs::GARRISON_INSTANCE_DATA_THREE_FIRST + l_ThreeID, 1);
                                        me->GetMotionMaster()->MovePoint(PeonData::MOVE_POINT_ID_THREE_FIRST + l_ThreeID,   PeonData::FrostWallTreePos[l_ThreeID][0],
                                                                                                                            PeonData::FrostWallTreePos[l_ThreeID][1],
                                                                                                                            PeonData::FrostWallTreePos[l_ThreeID][2]);

                                        m_ThreeID                   = l_ThreeID;
                                        m_Phase                     = PeonData::PHASE_WOODCUTTING;
                                        m_WoodCuttingRemainingTime  = PeonData::WOODCUTTING_TIME;
                                        m_WoodCuttingCycleCount++;
                                    }
                                    break;

                                case PeonData::PHASE_WOODCUTTING_DEPOSIT:
                                    {
                                        me->SetWalk(true);
                                        me->AddAura(MS::Garrison::Spells::SPELL_GARRISON_ORC_MALE_CARRYNG_LUMBER, me);
                                        me->GetMotionMaster()->MovePoint(PeonData::MOVE_POINT_ID_WOODCUTTING_DEPOSIT_A, PeonData::FrostWallWoodCuttingA[0],
                                                                                                                        PeonData::FrostWallWoodCuttingA[1],
                                                                                                                        PeonData::FrostWallWoodCuttingA[2]);
                                    }
                                    break;

                                default:
                                    break;
                            }
                        }
                    }
                }
            }

            uint32 SelectRandomTree()
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

            uint32 m_LastTalkTimer;
            uint32 m_TalkInterval;
            uint32 m_Phase;
            int32 m_ThreeID;

            int32 m_WoodCuttingRemainingTime;
            int32 m_WoodCuttingCycleCount;

            float m_SpawnX, m_SpawnY, m_SpawnZ, m_SpawnO;
            bool m_BackToSpawnPos;
            bool m_IsDynamicScript;
            bool m_Initialised;

            std::queue<std::function<void()>> m_DelayedOperations;
        };
};

/// Garrison shelly hamby
class npc_Skaggit : public CreatureScript
{
    public:
        /// Constructor
        npc_Skaggit()
            : CreatureScript("npc_Skaggit")
        {

        }

        /// Called when a player selects a gossip item in the creature's gossip menu.
        bool OnGossipSelect(Player * p_Player, Creature * p_Creature, uint32 p_Sender, uint32 p_Action) override
        {
            p_Player->CLOSE_GOSSIP_MENU();
            p_Creature->AI()->Talk(0);

            if (p_Player->HasQuest(MS::Garrison::Quests::QUEST_WHAT_WE_GOT) && p_Player->GetQuestObjectiveCounter(273085) != 1)
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

                    l_Creature->AI()->SetData(GARRISON_CREATURE_AI_DATA_PEON_WORKING, PeonData::PHASE_WOODCUTTING);
                }
            }

            return true;
        }

};

void AddSC_Garrison_NPC()
{
    new npc_GarrisonFord;
    new npc_GarrisonCartRope;
    new npc_AssistantBrightstone;
    new npc_ShellyHamby;
    new npc_BarosAlexsom;
    new npc_VindicatorMaraad;
    new npc_LunarfallLaborer;
    new npc_FrostwallPeon("npc_FrostwallPeon_Dynamic");
    new npc_FrostwallPeon("npc_FrostwallPeon");
    new npc_Skaggit;
}