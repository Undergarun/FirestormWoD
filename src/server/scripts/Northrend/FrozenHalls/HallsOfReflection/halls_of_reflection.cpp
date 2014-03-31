/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"
#include "Player.h"
#include "halls_of_reflection.h"

enum Text
{
    SAY_JAINA_INTRO_1                   = 0,
    SAY_JAINA_INTRO_2                   = 1,
    SAY_JAINA_INTRO_3                   = 2,
    SAY_JAINA_INTRO_4                   = 3,
    SAY_JAINA_INTRO_5                   = 4,
    SAY_JAINA_INTRO_6                   = 5,
    SAY_JAINA_INTRO_7                   = 6,
    SAY_JAINA_INTRO_8                   = 7,
    SAY_JAINA_INTRO_9                   = 8,
    SAY_JAINA_INTRO_10                  = 9,
    SAY_JAINA_INTRO_11                  = 10,
    SAY_JAINA_INTRO_END                 = 11,

    SAY_SYLVANAS_INTRO_1                = 0,
    SAY_SYLVANAS_INTRO_2                = 1,
    SAY_SYLVANAS_INTRO_3                = 2,
    SAY_SYLVANAS_INTRO_4                = 3,
    SAY_SYLVANAS_INTRO_5                = 4,
    SAY_SYLVANAS_INTRO_6                = 5,
    SAY_SYLVANAS_INTRO_7                = 6,
    SAY_SYLVANAS_INTRO_8                = 7,
    SAY_SYLVANAS_INTRO_END              = 8,

    SAY_UTHER_INTRO_A2_1                = 0,
    SAY_UTHER_INTRO_A2_2                = 1,
    SAY_UTHER_INTRO_A2_3                = 2,
    SAY_UTHER_INTRO_A2_4                = 3,
    SAY_UTHER_INTRO_A2_5                = 4,
    SAY_UTHER_INTRO_A2_6                = 5,
    SAY_UTHER_INTRO_A2_7                = 6,
    SAY_UTHER_INTRO_A2_8                = 7,
    SAY_UTHER_INTRO_A2_9                = 8,
    SAY_UTHER_INTRO_H2_1                = 9,
    SAY_UTHER_INTRO_H2_2                = 10,
    SAY_UTHER_INTRO_H2_3                = 11,
    SAY_UTHER_INTRO_H2_4                = 12,
    SAY_UTHER_INTRO_H2_5                = 13,
    SAY_UTHER_INTRO_H2_6                = 14,
    SAY_UTHER_INTRO_H2_7                = 15,

    SAY_LK_INTRO_1                      = 0,
    SAY_LK_INTRO_2                      = 1,
    SAY_LK_INTRO_3                      = 2,
    SAY_LK_JAINA_INTRO_END              = 3,
    SAY_LK_SYLVANAS_INTRO_END           = 4,

    SAY_FALRIC_INTRO_1                  = 5,
    SAY_FALRIC_INTRO_2                  = 6,

    SAY_MARWYN_INTRO_1                  = 4,
};

enum Events
{
    EVENT_WALK_INTRO1   = 1,
    EVENT_WALK_INTRO2,
    EVENT_START_INTRO,
    EVENT_SKIP_INTRO,

    EVENT_INTRO_A2_1,
    EVENT_INTRO_A2_2,
    EVENT_INTRO_A2_3,
    EVENT_INTRO_A2_4,
    EVENT_INTRO_A2_5,
    EVENT_INTRO_A2_6,
    EVENT_INTRO_A2_7,
    EVENT_INTRO_A2_8,
    EVENT_INTRO_A2_9,
    EVENT_INTRO_A2_10,
    EVENT_INTRO_A2_11,
    EVENT_INTRO_A2_12,
    EVENT_INTRO_A2_13,
    EVENT_INTRO_A2_14,
    EVENT_INTRO_A2_15,
    EVENT_INTRO_A2_16,
    EVENT_INTRO_A2_17,
    EVENT_INTRO_A2_18,
    EVENT_INTRO_A2_19,

    EVENT_INTRO_H2_1,
    EVENT_INTRO_H2_2,
    EVENT_INTRO_H2_3,
    EVENT_INTRO_H2_4,
    EVENT_INTRO_H2_5,
    EVENT_INTRO_H2_6,
    EVENT_INTRO_H2_7,
    EVENT_INTRO_H2_8,
    EVENT_INTRO_H2_9,
    EVENT_INTRO_H2_10,
    EVENT_INTRO_H2_11,
    EVENT_INTRO_H2_12,
    EVENT_INTRO_H2_13,
    EVENT_INTRO_H2_14,
    EVENT_INTRO_H2_15,

    EVENT_INTRO_LK_1,
    EVENT_INTRO_LK_2,
    EVENT_INTRO_LK_3,
    EVENT_INTRO_LK_4,
    EVENT_INTRO_LK_5,
    EVENT_INTRO_LK_6,
    EVENT_INTRO_LK_7,
    EVENT_INTRO_LK_8,
    EVENT_INTRO_LK_9,
    EVENT_INTRO_LK_10,
    EVENT_INTRO_LK_11,

    EVENT_INTRO_END,

    EVENT_OPEN_FROSTWORN_DOOR,
    EVENT_CLOSE_FROSTWORN_DOOR,
    EVENT_GUARD_FINISH,
};

enum eEnum
{
    ACTION_START_INTRO,
    ACTION_SKIP_INTRO,

    QUEST_DELIVRANCE_FROM_THE_PIT_A2              = 24710,
    QUEST_DELIVRANCE_FROM_THE_PIT_H2              = 24712,
    QUEST_WRATH_OF_THE_LICH_KING_A2               = 24500,
    QUEST_WRATH_OF_THE_LICH_KING_H2               = 24802,
};

enum Spells
{
    SPELL_CAST_VISUAL                  = 65633, // Jaina/Sylavana
    SPELL_BOSS_SPAWN_AURA              = 72712, // Falric and Marwyn
    SPELL_UTHER_DESPAWN                = 70693,
    SPELL_TAKE_FROSTMOURNE             = 72729,
    SPELL_FROSTMOURNE_DESPAWN          = 72726,
    SPELL_FROSTMOURNE_VISUAL           = 73220,
    SPELL_FROSTMOURNE_SOUNDS           = 70667,
    SPELL_JAINA_ICEBARRIER             = 69787,
    SPELL_JAINA_ICEPRISON              = 69708,
    SPELL_SYLVANAS_CLOAKOFDARKNESS     = 70188,
    SPELL_SYLVANAS_DARKBINDING         = 70194,
    SPELL_REMORSELESS_WINTER           = 69780,
    SPELL_JAINA_DESTROY_ICE_WALL       = 69784,
    SPELL_SYLVANAS_DESTROY_ICE_WALL    = 70225,
    SPELL_SYLVANAS_JUMP                = 68339,
    SPELL_SYLVANAS_SHADOWSTEP          = 69087,
    SPELL_FIRE_CANNON                  = 67461,
};

const Position HallsofReflectionLocs[]=
{
    {5283.234863f, 1990.946777f, 707.695679f, 0.929097f},   // 2 Loralen Follows
    {5408.031250f, 2102.918213f, 707.695251f, 0.792756f},   // 9 Sylvanas Follows
    {5401.866699f, 2110.837402f, 707.695251f, 0.800610f},   // 10 Loralen follows
};

const Position IntroPos              = {5265.89f, 1952.98f, 707.6978f, 0.0f}; // Jaina/Sylvanas Intro Start Position
const Position MoveThronePos         = {5306.952148f, 1998.499023f, 709.341431f, 1.277278f}; // Jaina/Sylvanas walks to throne
const Position MoveGuardPos          = {5284.392090f, 1989.915405f, 707.694824f, 0.553165f};
const Position UtherSpawnPos         = {5308.310059f, 2003.857178f, 709.341431f, 4.650315f};
const Position LichKingSpawnPos      = {5362.917480f, 2062.307129f, 707.695374f, 3.945812f};
const Position LichKingMoveThronePos = {5312.080566f, 2009.172119f, 709.341431f, 3.973301f}; // Lich King walks to throne
const Position LichKingMoveAwayPos   = {5400.069824f, 2102.7131689f, 707.69525f, 0.843803f}; // Lich King walks away
const Position GuardDiePos           = {5367.544434f, 2085.288086f, 707.695007f, 0.900312f}; // Lich King walks away
const Position FalricStartPos        = {5334.979980f, 1982.399536f, 709.320129f, 2.347014f}; // Falric start position
const Position MarwynStartPos        = {5283.878906f, 2030.459595f, 709.319641f, 5.506670f}; // Marwyn start position 
const Position ChestPos              = {5246.187500f, 1649.079468f, 784.301758f, 0.901268f}; // Chest position 

class npc_jaina_or_sylvanas_hor : public CreatureScript
{
    public:
        npc_jaina_or_sylvanas_hor() : CreatureScript("npc_jaina_or_sylvanas_hor") { }

    // AI of Part1
    struct npc_jaina_or_sylvanas_horAI : public ScriptedAI
    {
        npc_jaina_or_sylvanas_horAI(Creature* creature) : ScriptedAI(creature)
        {
            _instance = me->GetInstanceScript();
        }

        InstanceScript* _instance;
        uint64 _utherGUID;
        uint64 _lichkingGUID;

        EventMap _events;

        void sGossipSelect(Player* player, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();
            switch (action)
            {
                case 0:
                    player->CLOSE_GOSSIP_MENU();
                    _events.ScheduleEvent(EVENT_START_INTRO, 1000);
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP|UNIT_NPC_FLAG_QUESTGIVER);
                    break;
                case 1:
                    player->CLOSE_GOSSIP_MENU();
                    _events.ScheduleEvent(EVENT_SKIP_INTRO, 1000);
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP|UNIT_NPC_FLAG_QUESTGIVER);
                    break;
            }
        }

        void Reset()
        {
            _events.Reset();

            _utherGUID = 0;
            _lichkingGUID = 0;

            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP|UNIT_NPC_FLAG_QUESTGIVER);
            me->SetStandState(UNIT_STAND_STATE_STAND);
            _events.ScheduleEvent(EVENT_WALK_INTRO1, 3000);
        }

        void UpdateAI(uint32 const diff)
        {
            _events.Update(diff);
            switch (_events.ExecuteEvent())
            {
                case EVENT_WALK_INTRO1:
                    me->GetMotionMaster()->MovePoint(0, IntroPos);
                    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                    {
                        Talk(SAY_JAINA_INTRO_1);
                        _events.ScheduleEvent(EVENT_WALK_INTRO2, 7000);
                    }
                    else
                    {
                        Talk(SAY_SYLVANAS_INTRO_1);
                        _events.ScheduleEvent(EVENT_WALK_INTRO2, 9000);
                    }
                    break;
                case EVENT_WALK_INTRO2:
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP|UNIT_NPC_FLAG_QUESTGIVER);
                    break;
                case EVENT_START_INTRO:
                    me->GetMotionMaster()->MovePoint(0, MoveThronePos);
                    if (Creature* guard = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_LORALEN_OR_KORELN)))
                        guard->GetMotionMaster()->MovePoint(0, MoveGuardPos);
                    // Begining of intro is differents between fActions as the speech sequence and timers are differents.
                    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                        _events.ScheduleEvent(EVENT_INTRO_A2_1, 0);
                    else
                        _events.ScheduleEvent(EVENT_INTRO_H2_1, 0);
                    break;
                // A2 Intro Events
                case EVENT_INTRO_A2_1:
                    Talk(SAY_JAINA_INTRO_3);
                    _events.ScheduleEvent(EVENT_INTRO_A2_2, 7000);
                    break;
                case EVENT_INTRO_A2_2:
                    Talk(SAY_JAINA_INTRO_4);
                    _events.ScheduleEvent(EVENT_INTRO_A2_3, 10000);
                    break;
                case EVENT_INTRO_A2_3:
                    me->CastSpell(me, SPELL_CAST_VISUAL, false);
                    me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
                    _instance->HandleGameObject(_instance->GetData64(DATA_FROSTMOURNE), true);
                    _events.ScheduleEvent(EVENT_INTRO_A2_4, 10000);
                    break;
                case EVENT_INTRO_A2_4:
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->GetMotionMaster()->MoveIdle();
                        _utherGUID = uther->GetGUID();
                    }
                    _events.ScheduleEvent(EVENT_INTRO_A2_5, 2000);
                    break;
                case EVENT_INTRO_A2_5:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_1);
                    _events.ScheduleEvent(EVENT_INTRO_A2_6, 3000);
                    break;
                case EVENT_INTRO_A2_6:
                    Talk(SAY_JAINA_INTRO_5);
                    _events.ScheduleEvent(EVENT_INTRO_A2_7, 7000);
                    break;
                case EVENT_INTRO_A2_7:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_2);
                    _events.ScheduleEvent(EVENT_INTRO_A2_8, 7000);
                    break;
                case EVENT_INTRO_A2_8:
                    Talk(SAY_JAINA_INTRO_6);
                    _events.ScheduleEvent(EVENT_INTRO_A2_9, 1200);
                    break;
                case EVENT_INTRO_A2_9:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_3);
                    _events.ScheduleEvent(EVENT_INTRO_A2_10, 11000);
                    break;
                case EVENT_INTRO_A2_10:
                    Talk(SAY_JAINA_INTRO_7);
                    _events.ScheduleEvent(EVENT_INTRO_A2_11, 6000);
                    break;
                case EVENT_INTRO_A2_11:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_4);
                    _events.ScheduleEvent(EVENT_INTRO_A2_12, 12000);
                    break;
                case EVENT_INTRO_A2_12:
                    Talk(SAY_JAINA_INTRO_8);
                    _events.ScheduleEvent(EVENT_INTRO_A2_13, 6000);
                    break;
                case EVENT_INTRO_A2_13:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_5);
                    _events.ScheduleEvent(EVENT_INTRO_A2_14, 13000);
                    break;
                case EVENT_INTRO_A2_14:
                    Talk(SAY_JAINA_INTRO_9);
                    _events.ScheduleEvent(EVENT_INTRO_A2_15, 12000);
                    break;
                case EVENT_INTRO_A2_15:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_6);
                    _events.ScheduleEvent(EVENT_INTRO_A2_16, 25000);
                    break;
                case EVENT_INTRO_A2_16:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_7);
                    _events.ScheduleEvent(EVENT_INTRO_A2_17, 6000);
                    break;
                case EVENT_INTRO_A2_17:
                    Talk(SAY_JAINA_INTRO_10);
                    _events.ScheduleEvent(EVENT_INTRO_A2_18, 5000);
                    break;
                case EVENT_INTRO_A2_18:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                    {
                        uther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                        uther->AI()->Talk(SAY_UTHER_INTRO_A2_8);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_A2_19, 12000);
                    break;
                case EVENT_INTRO_A2_19:
                    Talk(SAY_JAINA_INTRO_11);
                    _events.ScheduleEvent(EVENT_INTRO_LK_1, 3000);
                    break;
                // H2 Intro Events
                case EVENT_INTRO_H2_1:
                    Talk(SAY_SYLVANAS_INTRO_2);
                    _events.ScheduleEvent(EVENT_INTRO_H2_3, 6000);
                    break;
                case EVENT_INTRO_H2_3:
                    Talk(SAY_SYLVANAS_INTRO_3);
                    me->CastSpell(me, SPELL_CAST_VISUAL, false);
                    me->CastSpell(me, SPELL_FROSTMOURNE_SOUNDS, true);
                    _instance->HandleGameObject(_instance->GetData64(DATA_FROSTMOURNE), true);
                    _events.ScheduleEvent(EVENT_INTRO_H2_4, 6000);
                    break;
                case EVENT_INTRO_H2_4:
                    // spawn UTHER during speach 2
                    if (Creature* uther = me->SummonCreature(NPC_UTHER, UtherSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        uther->GetMotionMaster()->MoveIdle();
                        _utherGUID = uther->GetGUID();
                    }
                    _events.ScheduleEvent(EVENT_INTRO_H2_5, 2000);
                    break;
                case EVENT_INTRO_H2_5:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_1);
                    _events.ScheduleEvent(EVENT_INTRO_H2_6, 11000);
                    break;
                case EVENT_INTRO_H2_6:
                    Talk(SAY_SYLVANAS_INTRO_4);
                    _events.ScheduleEvent(EVENT_INTRO_H2_7, 3000);
                    break;
                case EVENT_INTRO_H2_7:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_2);
                    _events.ScheduleEvent(EVENT_INTRO_H2_8, 6000);
                    break;
                case EVENT_INTRO_H2_8:
                    Talk(SAY_SYLVANAS_INTRO_5);
                    _events.ScheduleEvent(EVENT_INTRO_H2_9, 5000);
                    break;
                case EVENT_INTRO_H2_9:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_3);
                    _events.ScheduleEvent(EVENT_INTRO_H2_10, 19000);
                    break;
                case EVENT_INTRO_H2_10:
                    Talk(SAY_SYLVANAS_INTRO_6);
                    _events.ScheduleEvent(EVENT_INTRO_H2_11, 1500);
                    break;
                case EVENT_INTRO_H2_11:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_4);
                    _events.ScheduleEvent(EVENT_INTRO_H2_12, 19500);
                    break;
                case EVENT_INTRO_H2_12:
                    Talk(SAY_SYLVANAS_INTRO_7);
                    _events.ScheduleEvent(EVENT_INTRO_H2_13, 2000);
                    break;
                case EVENT_INTRO_H2_13:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                    {
                        uther->HandleEmoteCommand(EMOTE_ONESHOT_NO);
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_5);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_H2_14, 12000);
                    break;
                case EVENT_INTRO_H2_14:
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                        uther->AI()->Talk(SAY_UTHER_INTRO_H2_6);
                    _events.ScheduleEvent(EVENT_INTRO_H2_15, 8000);
                    break;
                case EVENT_INTRO_H2_15:
                    Talk(SAY_SYLVANAS_INTRO_8);
                    _events.ScheduleEvent(EVENT_INTRO_LK_1, 2000);
                    break;
                // Remaining Intro Events common for both faction
                case EVENT_INTRO_LK_1:
                    // Spawn LK in front of door, and make him move to the sword.
                    if (Creature* lichking = me->SummonCreature(NPC_LICH_KING_PART1, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        lichking->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                        lichking->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
                        lichking->SetFlag(UNIT_NPC_FLAGS, UNIT_FLAG_IMMUNE_TO_PC|UNIT_FLAG_IMMUNE_TO_NPC);
                        lichking->SetReactState(REACT_PASSIVE);
                        _lichkingGUID = lichking->GetGUID();
                        _events.ScheduleEvent(EVENT_OPEN_FROSTWORN_DOOR, 0);
                        _events.ScheduleEvent(EVENT_CLOSE_FROSTWORN_DOOR, 4000);
                    }
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                    {
                        uther->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_COWER);
                        if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            uther->AI()->Talk(SAY_UTHER_INTRO_A2_9);
                        else
                            uther->AI()->Talk(SAY_UTHER_INTRO_H2_7);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_2, 10000);
                    break;
                case EVENT_INTRO_LK_2:
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGUID))
                        lichking->AI()->Talk(SAY_LK_INTRO_1);
                    _events.ScheduleEvent(EVENT_INTRO_LK_3, 1000);
                    break;
                case EVENT_INTRO_LK_3:
                    // The Lich King banishes Uther to the abyss.
                    if (Creature* uther = me->GetCreature(*me, _utherGUID))
                    {
                        uther->CastSpell(uther, SPELL_UTHER_DESPAWN, true);
                        uther->DespawnOrUnsummon(5000);
                        _utherGUID = 0;
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_4, 9000);
                    break;
                case EVENT_INTRO_LK_4:
                    // He steps forward and removes the runeblade from the heap of skulls.
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGUID))
                    {
                        if (GameObject* frostmourne = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_FROSTMOURNE)))
                            frostmourne->SetPhaseMask(2, true);
                        lichking->CastSpell(lichking, SPELL_TAKE_FROSTMOURNE, true);
                        lichking->CastSpell(lichking, SPELL_FROSTMOURNE_VISUAL, true);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_5, 8000);
                    break;
                case EVENT_INTRO_LK_5:
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGUID))
                        lichking->AI()->Talk(SAY_LK_INTRO_2);
                    _events.ScheduleEvent(EVENT_INTRO_LK_6, 10000);
                    break;
                case EVENT_INTRO_LK_6:
                    // summon Falric and Marwyn. then go back to the door
                    if (Creature* falric = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_FALRIC_EVENT)))
                    {
                        falric->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                        falric->CastSpell(falric, SPELL_BOSS_SPAWN_AURA, true);
                        falric->SetVisible(true);
                    }
                    if (Creature* marwyn = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_MARWYN_EVENT)))
                    {
                        marwyn->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                        marwyn->CastSpell(marwyn, SPELL_BOSS_SPAWN_AURA, true);
                        marwyn->SetVisible(true);
                    }
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGUID))
                    {
                        lichking->AI()->Talk(SAY_LK_INTRO_3);
                        lichking->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                        lichking->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_7, 10000);
                    _events.ScheduleEvent(EVENT_OPEN_FROSTWORN_DOOR, 5000);
                    break;
                case EVENT_INTRO_LK_7:
                    if (Creature* marwyn = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_MARWYN_EVENT)))
                    {
                        marwyn->AI()->Talk(SAY_MARWYN_INTRO_1);
                        marwyn->SetWalk(true);
                        marwyn->GetMotionMaster()->MovePoint(0, MarwynStartPos);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_8, 1000);
                    break;
                case EVENT_INTRO_LK_8:
                    if (Creature* falric = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_FALRIC_EVENT)))
                    {
                        falric->AI()->Talk(SAY_FALRIC_INTRO_1);
                        falric->SetWalk(true);
                        falric->GetMotionMaster()->MovePoint(0, FalricStartPos);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_9, 5000);
                    break;
                case EVENT_INTRO_LK_9:
                    if (Creature* falric = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_FALRIC_EVENT)))
                    {
                        falric->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                        falric->AI()->Talk(SAY_FALRIC_INTRO_2);
                    }
                    if (Creature* marwyn = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_MARWYN_EVENT)))
                        marwyn->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                    _instance->ProcessEvent(0, EVENT_SPAWN_WAVES); 
                    _events.ScheduleEvent(EVENT_INTRO_LK_10, 4000);
                    break;
                case EVENT_INTRO_LK_10:
                    if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                        Talk(SAY_JAINA_INTRO_END);
                    else
                        Talk(SAY_SYLVANAS_INTRO_END);
                    me->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
                    if (Creature* guard = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_LORALEN_OR_KORELN)))
                        guard->GetMotionMaster()->MovePoint(0, LichKingMoveAwayPos);
                    _events.ScheduleEvent(EVENT_INTRO_LK_11, 5000);
                    break;
                case EVENT_INTRO_LK_11:
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGUID))
                    {
                        if (_instance->GetData(DATA_TEAM_IN_INSTANCE) == ALLIANCE)
                            lichking->AI()->Talk(SAY_LK_JAINA_INTRO_END);
                        else
                            lichking->AI()->Talk(SAY_LK_SYLVANAS_INTRO_END);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_END, 5000);
                    break;
                case EVENT_INTRO_END:
                    if (_instance)
                        _instance->SetData(DATA_INTRO_EVENT, DONE);
                    // Loralen or Koreln disappearAndDie()
                    if (Creature* lichking = me->GetCreature(*me, _lichkingGUID))
                    {
                        lichking->DespawnOrUnsummon(5000);
                        _lichkingGUID = 0;
                    }
                    me->DespawnOrUnsummon(10000);
                    _events.ScheduleEvent(EVENT_CLOSE_FROSTWORN_DOOR, 7000);
                    _events.ScheduleEvent(EVENT_GUARD_FINISH, 9000);
                    break;
                case EVENT_GUARD_FINISH:
                    if (Creature* guard = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_LORALEN_OR_KORELN)))
                    {
                        guard->setDeathState(JUST_DIED);
                        guard->SetCorpseDelay(604800);
                        guard->GetMotionMaster()->MovePoint(0, GuardDiePos);
                    }
                    break;                    
                case EVENT_SKIP_INTRO:
                    me->GetMotionMaster()->MovePoint(0, MoveThronePos);
                    if (Creature* guard = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_LORALEN_OR_KORELN)))
                        guard->GetMotionMaster()->MovePoint(0, MoveGuardPos);
                    if (Creature* lichking = me->SummonCreature(NPC_LICH_KING_PART1, LichKingSpawnPos, TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        lichking->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                        lichking->GetMotionMaster()->MovePoint(0, LichKingMoveThronePos);
                        lichking->SetReactState(REACT_PASSIVE);
                        _lichkingGUID = lichking->GetGUID();
                        _events.ScheduleEvent(EVENT_OPEN_FROSTWORN_DOOR, 0);
                        _events.ScheduleEvent(EVENT_CLOSE_FROSTWORN_DOOR, 4000);
                    }
                    _events.ScheduleEvent(EVENT_INTRO_LK_4, 15000);
                    break;
                case EVENT_OPEN_FROSTWORN_DOOR:
                    if (GameObject* gate = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_FROSTWORN_DOOR)))
                        _instance->HandleGameObject(0 ,true, gate);
                    break;
                case EVENT_CLOSE_FROSTWORN_DOOR:
                    if (GameObject* gate = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_FROSTWORN_DOOR)))
                        _instance->HandleGameObject(0 ,false, gate);
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_jaina_or_sylvanas_horAI(creature);
    }
};

enum TrashSpells
{
    // Ghostly Priest
    SPELL_SHADOW_WORD_PAIN                        = 72318,
    SPELL_CIRCLE_OF_DESTRUCTION                   = 72320,
    SPELL_COWER_IN_FEAR                           = 72321,
    SPELL_DARK_MENDING                            = 72322,

    // Phantom Mage
    SPELL_FIREBALL                                = 72163,
    SPELL_FLAMESTRIKE                             = 72169,
    SPELL_FROSTBOLT                               = 72166,
    SPELL_CHAINS_OF_ICE                           = 72121,
    SPELL_HALLUCINATION                           = 72342,

    // Phantom Hallucination (same as phantom mage + HALLUCINATION_2 when dies)
    SPELL_HALLUCINATION_2                         = 72344,

    // Shadowy Mercenary
    SPELL_SHADOW_STEP                             = 72326,
    SPELL_DEADLY_POISON                           = 72329,
    SPELL_ENVENOMED_DAGGER_THROW                  = 72333,
    SPELL_KIDNEY_SHOT                             = 72335,

    // Spectral Footman
    SPELL_SPECTRAL_STRIKE                         = 72198,
    SPELL_SHIELD_BASH                             = 72194,
    SPELL_TORTURED_ENRAGE                         = 72203,

    // Tortured Rifleman
    SPELL_SHOOT                                   = 72208,
    SPELL_CURSED_ARROW                            = 72222,
    SPELL_FROST_TRAP                              = 72215,
    SPELL_ICE_SHOT                                = 72268,
};

enum TrashEvents
{
    EVENT_TRASH_NONE,

    // Ghostly Priest
    EVENT_SHADOW_WORD_PAIN,
    EVENT_CIRCLE_OF_DESTRUCTION,
    EVENT_COWER_IN_FEAR,
    EVENT_DARK_MENDING,

    // Phantom Mage
    EVENT_FIREBALL,
    EVENT_FLAMESTRIKE,
    EVENT_FROSTBOLT,
    EVENT_CHAINS_OF_ICE,
    EVENT_HALLUCINATION,

    // Shadowy Mercenary
    EVENT_SHADOW_STEP,
    EVENT_DEADLY_POISON,
    EVENT_ENVENOMED_DAGGER_THROW,
    EVENT_KIDNEY_SHOT,

    // Spectral Footman
    EVENT_SPECTRAL_STRIKE,
    EVENT_SHIELD_BASH,
    EVENT_TORTURED_ENRAGE,

    // Tortured Rifleman
    EVENT_SHOOT,
    EVENT_CURSED_ARROW,
    EVENT_FROST_TRAP,
    EVENT_ICE_SHOT,
};

struct npc_gauntlet_trash : public ScriptedAI
{
    npc_gauntlet_trash(Creature* creature) : ScriptedAI(creature),
        _instance(creature->GetInstanceScript())
    {
    }

    void Reset()
    {
        me->CastSpell(me, SPELL_WELL_OF_SOULS, true);
        _events.Reset();
    }

    void EnterEvadeMode()
    {
        if (_instance->GetData(DATA_WAVE_COUNT) != NOT_STARTED)
            _instance->SetData(DATA_WAVE_COUNT, NOT_STARTED);
    }

    void SetData(uint32 type, uint32 value)
    {
        if (type)
            return;

        InternalWaveId = value;
    }

    uint32 GetData(uint32 type)
    {
        if (type)
            return 0;

        return InternalWaveId;
    }

protected:
    EventMap _events;
    InstanceScript* _instance;
    uint32 InternalWaveId;
};

class npc_ghostly_priest : public CreatureScript
{
    public:
        npc_ghostly_priest() : CreatureScript("npc_ghostly_priest") { }

        struct npc_ghostly_priestAI : public npc_gauntlet_trash
        {
            npc_ghostly_priestAI(Creature* creature) : npc_gauntlet_trash(creature)
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000); /// @todo adjust timers
                _events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
                _events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
                _events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_WORD_PAIN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_SHADOW_WORD_PAIN);
                            _events.ScheduleEvent(EVENT_SHADOW_WORD_PAIN, 8000);
                            break;
                        case EVENT_CIRCLE_OF_DESTRUCTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_CIRCLE_OF_DESTRUCTION);
                            _events.ScheduleEvent(EVENT_CIRCLE_OF_DESTRUCTION, 12000);
                            break;
                        case EVENT_COWER_IN_FEAR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_COWER_IN_FEAR);
                            _events.ScheduleEvent(EVENT_COWER_IN_FEAR, 10000);
                            break;
                        case EVENT_DARK_MENDING:
                            // find an ally with missing HP
                            if (Unit* target = DoSelectLowestHpFriendly(40, DUNGEON_MODE(30000, 50000)))
                            {
                                DoCast(target, SPELL_DARK_MENDING);
                                _events.ScheduleEvent(EVENT_DARK_MENDING, 20000);
                            }
                            else
                            {
                                // no friendly unit with missing hp. re-check in just 5 sec.
                                _events.ScheduleEvent(EVENT_DARK_MENDING, 5000);
                            }
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ghostly_priestAI(creature);
        }
};

class npc_phantom_mage : public CreatureScript
{
    public:
        npc_phantom_mage() : CreatureScript("npc_phantom_mage") { }

        struct npc_phantom_mageAI : public npc_gauntlet_trash
        {
            npc_phantom_mageAI(Creature* creature) : npc_gauntlet_trash(creature)
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_FIREBALL, 3000); /// @todo adjust timers
                _events.ScheduleEvent(EVENT_FLAMESTRIKE, 6000);
                _events.ScheduleEvent(EVENT_FROSTBOLT, 9000);
                _events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 12000);
                _events.ScheduleEvent(EVENT_HALLUCINATION, 40000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_FIREBALL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_FIREBALL);
                            _events.ScheduleEvent(EVENT_FIREBALL, 15000);
                            break;
                        case EVENT_FLAMESTRIKE:
                            DoCast(SPELL_FLAMESTRIKE);
                            _events.ScheduleEvent(EVENT_FLAMESTRIKE, 15000);
                            break;
                        case EVENT_FROSTBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_FROSTBOLT);
                            _events.ScheduleEvent(EVENT_FROSTBOLT, 15000);
                            break;
                        case EVENT_CHAINS_OF_ICE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_CHAINS_OF_ICE);
                            _events.ScheduleEvent(EVENT_CHAINS_OF_ICE, 15000);
                            break;
                        case EVENT_HALLUCINATION:
                            //DoCast(SPELL_HALLUCINATION);
                            Position pos;
                            me->GetRandomNearPosition(pos, 10.0f);
                            if (Creature* summon = me->SummonCreature(NPC_PHANTOM_HALLUCINATION, pos, TEMPSUMMON_CORPSE_TIMED_DESPAWN, 10000))
                                summon->SetInCombatWithZone();
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_phantom_mageAI(creature);
        }
};

class npc_phantom_hallucination : public CreatureScript
{
    public:
        npc_phantom_hallucination() : CreatureScript("npc_phantom_hallucination") { }

        struct npc_phantom_hallucinationAI : public npc_phantom_mage::npc_phantom_mageAI
        {
            npc_phantom_hallucinationAI(Creature* creature) : npc_phantom_mage::npc_phantom_mageAI(creature) {}

            void JustDied(Unit* /*killer*/)
            {
                DoCast(SPELL_HALLUCINATION_2);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_phantom_hallucinationAI(creature);
        }
};

class npc_shadowy_mercenary : public CreatureScript
{
    public:
        npc_shadowy_mercenary() : CreatureScript("npc_shadowy_mercenary") { }

        struct npc_shadowy_mercenaryAI : public npc_gauntlet_trash
        {
            npc_shadowy_mercenaryAI(Creature* creature) : npc_gauntlet_trash(creature)
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_SHADOW_STEP, 8000); /// @todo adjust timers
                _events.ScheduleEvent(EVENT_DEADLY_POISON, 5000);
                _events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
                _events.ScheduleEvent(EVENT_KIDNEY_SHOT, 12000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOW_STEP:
                            DoCast(SPELL_SHADOW_STEP);
                            _events.ScheduleEvent(EVENT_SHADOW_STEP, 8000);
                            break;
                        case EVENT_DEADLY_POISON:
                            DoCastVictim(SPELL_DEADLY_POISON);
                            _events.ScheduleEvent(EVENT_DEADLY_POISON, 10000);
                            break;
                        case EVENT_ENVENOMED_DAGGER_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_ENVENOMED_DAGGER_THROW);
                            _events.ScheduleEvent(EVENT_ENVENOMED_DAGGER_THROW, 10000);
                            break;
                        case EVENT_KIDNEY_SHOT:
                            DoCastVictim(SPELL_KIDNEY_SHOT);
                            _events.ScheduleEvent(EVENT_KIDNEY_SHOT, 10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_shadowy_mercenaryAI(creature);
        }
};

class npc_spectral_footman : public CreatureScript
{
    public:
        npc_spectral_footman() : CreatureScript("npc_spectral_footman") { }

        struct npc_spectral_footmanAI : public npc_gauntlet_trash
        {
            npc_spectral_footmanAI(Creature* creature) : npc_gauntlet_trash(creature)
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000); /// @todo adjust timers
                _events.ScheduleEvent(EVENT_SHIELD_BASH, 10000);
                _events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SPECTRAL_STRIKE:
                            DoCastVictim(SPELL_SPECTRAL_STRIKE);
                            _events.ScheduleEvent(EVENT_SPECTRAL_STRIKE, 5000);
                            break;
                        case EVENT_SHIELD_BASH:
                            DoCastVictim(SPELL_SHIELD_BASH);
                            _events.ScheduleEvent(EVENT_SHIELD_BASH, 5000);
                            break;
                        case EVENT_TORTURED_ENRAGE:
                            DoCast(SPELL_TORTURED_ENRAGE);
                            _events.ScheduleEvent(EVENT_TORTURED_ENRAGE, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_spectral_footmanAI(creature);
        }
};

class npc_tortured_rifleman : public CreatureScript
{
    public:
        npc_tortured_rifleman() : CreatureScript("npc_tortured_rifleman") { }

        struct npc_tortured_riflemanAI : public npc_gauntlet_trash
        {
            npc_tortured_riflemanAI(Creature* creature) : npc_gauntlet_trash(creature)
            {
            }

            void EnterCombat(Unit* /*who*/)
            {
                _events.ScheduleEvent(EVENT_SHOOT, 1); /// @todo adjust timers
                _events.ScheduleEvent(EVENT_CURSED_ARROW, 7000);
                _events.ScheduleEvent(EVENT_FROST_TRAP, 10000);
                _events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHOOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_SHOOT);
                            _events.ScheduleEvent(EVENT_SHOOT, 2000);
                            break;
                        case EVENT_CURSED_ARROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_CURSED_ARROW);
                            _events.ScheduleEvent(EVENT_CURSED_ARROW, 10000);
                            break;
                        case EVENT_FROST_TRAP:
                            DoCast(SPELL_FROST_TRAP);
                            _events.ScheduleEvent(EVENT_FROST_TRAP, 30000);
                            break;
                        case EVENT_ICE_SHOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                DoCast(target, SPELL_ICE_SHOT);
                            _events.ScheduleEvent(EVENT_ICE_SHOT, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tortured_riflemanAI(creature);
        }
};

enum QueldelarEnums
{
    //General
    EVENT_BLADESTORM            = 1,
    EVENT_HEROIC_STRIKE         = 2,
    EVENT_MORTAL_STRIKE         = 3,
    EVENT_WHIRLWIND             = 4,

    SPELL_BLADESTORM            = 67541,
    SPELL_HEROIC_STRIKE         = 29426,
    SPELL_MORTAL_STRIKE         = 16856,
    SPELL_WHIRLWIND             = 67716
};

class npc_queldelar : public CreatureScript
{
    public:
        npc_queldelar() : CreatureScript("npc_queldelar") { }

        struct npc_queldelarAI  : public ScriptedAI
        { 
            npc_queldelarAI(Creature* creature) : ScriptedAI(creature)
            {
                me->SetDisplayId(me->GetCreatureTemplate()->Modelid2);
                SetEquipmentSlots(false, 50047, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_BLADESTORM, 10000);
                _events.ScheduleEvent(EVENT_HEROIC_STRIKE, 5000);
                _events.ScheduleEvent(EVENT_MORTAL_STRIKE, 7000);
                _events.ScheduleEvent(EVENT_WHIRLWIND, 13000);
            }
            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLADESTORM:
                            DoCastVictim(SPELL_BLADESTORM);
                            _events.ScheduleEvent(EVENT_BLADESTORM, 10000);
                            break;
                        case EVENT_HEROIC_STRIKE:
                            DoCastVictim(SPELL_HEROIC_STRIKE);
                            _events.ScheduleEvent(EVENT_HEROIC_STRIKE, 5000);
                            break;
                        case EVENT_MORTAL_STRIKE:
                            DoCastVictim(SPELL_MORTAL_STRIKE);
                            _events.ScheduleEvent(EVENT_MORTAL_STRIKE, 7000);
                            break;
                        case EVENT_WHIRLWIND:
                            DoCastVictim(SPELL_WHIRLWIND);
                            _events.ScheduleEvent(EVENT_WHIRLWIND, 13000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        private:
            EventMap _events;
            bool isActive;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_queldelarAI(creature);
        }
};

enum GeneralEvents
{
    //General
    EVENT_SHIELD                 = 1,
    EVENT_SPIKE                  = 2,
    EVENT_CLONE                  = 3,

    SAY_AGGRO                    = 0,
    SAY_DEATH                    = 1,

    SPELL_SHIELD_THROWN          = 69222, // 73076 on hc
    SPELL_SPIKE                  = 69184, // 70399 on hc
    SPELL_CLONE_NAME             = 57507,
    SPELL_CLONE_MODEL            = 45204,

    // Reflection
    EVENT_BALEFUL_STRIKE         = 1,

    SPELL_BALEFUL_STRIKE         = 69933, // 70400 on hc
    SPELL_SPIRIT_BURST           = 69900, // 73046 on hc
};

class npc_frostworn_general : public CreatureScript
{
    public:
        npc_frostworn_general() : CreatureScript("npc_frostworn_general") { }

        struct npc_frostworn_generalAI : public ScriptedAI
        {
            npc_frostworn_generalAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = me->GetInstanceScript();
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_SHIELD, 5000);
                _events.ScheduleEvent(EVENT_SPIKE, 14000);
                _events.ScheduleEvent(EVENT_CLONE, 12000);

                if (_instance)
                    _instance->SetData(DATA_FROSWORN_EVENT, NOT_STARTED);
            }

            void JustDied(Unit* /*killer*/)
            {
                Talk(SAY_DEATH);

                if (_instance)
                    _instance->SetData(DATA_FROSWORN_EVENT, DONE);
            }

            void EnterCombat(Unit* /*victim*/)
            {
                Talk(SAY_AGGRO);

                if (_instance)
                    _instance->SetData(DATA_FROSWORN_EVENT, IN_PROGRESS);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHIELD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST, 0, 45.0f))
                                DoCast(target, SPELL_SHIELD_THROWN);
                            _events.ScheduleEvent(EVENT_SHIELD, urand(8000, 12000));
                            break;
                        case EVENT_SPIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, SPELL_SPIKE);
                            _events.ScheduleEvent(EVENT_SPIKE, urand(15000, 20000));
                            break;
                        case EVENT_CLONE:
                            SummonClones();
                            _events.ScheduleEvent(EVENT_CLONE, 60000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

            void SummonClones()
            {
                std::list<Unit *> playerList;
                SelectTargetList(playerList, 5, SELECT_TARGET_TOPAGGRO, 0, true);
                for (std::list<Unit*>::const_iterator itr = playerList.begin(); itr != playerList.end(); ++itr)
                {
                    Unit* temp = (*itr);
                    Creature* reflection = me->SummonCreature(NPC_REFLECTION, temp->GetPositionX(), temp->GetPositionY(), temp->GetPositionZ(), temp->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 3000);
                    reflection->SetName(temp->GetName());
                    temp->CastSpell(reflection, SPELL_CLONE_NAME, true);
                    temp->CastSpell(reflection, SPELL_CLONE_MODEL, true);
                    reflection->setFaction(me->getFaction());
                    reflection->AI()->AttackStart(temp);
                }
            }

        private:
            EventMap _events;
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_frostworn_generalAI(creature);
        }
};

class npc_spiritual_reflection : public CreatureScript
{
    public:
        npc_spiritual_reflection() : CreatureScript("npc_spiritual_reflection") { }

        struct npc_spiritual_reflectionAI : public ScriptedAI
        {
            npc_spiritual_reflectionAI(Creature *creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                _events.Reset();
                _events.ScheduleEvent(EVENT_BALEFUL_STRIKE, 3000);
            }

            void JustDied(Unit* killer)
            {
                DoCast(killer, SPELL_SPIRIT_BURST);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BALEFUL_STRIKE:
                            DoCastVictim(SPELL_BALEFUL_STRIKE);
                            _events.ScheduleEvent(EVENT_BALEFUL_STRIKE, urand(3000, 8000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            EventMap _events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_spiritual_reflectionAI(creature);
        }
};

enum YellsPhase2
{
    /*INTRO - Pre Escape*/
    SAY_LICH_KING_AGGRO_A              = 0,
    SAY_LICH_KING_AGGRO_H              = 1,

    SAY_JAINA_AGGRO                    = 0,
    SAY_JAINA_WALL_01                  = 1,
    SAY_JAINA_WALL_02                  = 2,
    SAY_JAINA_WALL_03                  = 3,
    SAY_JAINA_WALL_04                  = 4,
    SAY_JAINA_ESCAPE_01                = 5,
    SAY_JAINA_ESCAPE_02                = 6,
    SAY_JAINA_TRAP                     = 7,
    SAY_JAINA_FINAL_1                  = 8,
    SAY_JAINA_FINAL_2                  = 9,
    
    SAY_SYLVANA_AGGRO                  = 0,
    SAY_SYLVANA_WALL_01                = 1,
    SAY_SYLVANA_WALL_02                = 2,
    SAY_SYLVANA_WALL_03                = 3,
    SAY_SYLVANA_WALL_04                = 4,
    SAY_SYLVANA_ESCAPE_01              = 5,
    SAY_SYLVANA_ESCAPE_02              = 6,
    SAY_SYLVANA_TRAP                   = 7,
    SAY_SYLVANA_FINAL_1                = 8,
    
    SAY_BARTLETT_FIRE                  = 0,

    SAY_KORM_FIRE                      = 0,
};

const Position CannonSpawns[4] =
{
    {5230.00f, 1658.75f, 802.22f, 0.00f},
    {5245.74f, 1644.44f, 802.35f, 0.00f},
    {5260.64f, 1636.07f, 802.16f, 0.00f},
    {5275.90f, 1633.22f, 802.25f, 0.00f},
};

class npc_jaina_and_sylvana_hor_part2 : public CreatureScript
{
    public:
        npc_jaina_and_sylvana_hor_part2() : CreatureScript("npc_jaina_and_sylvana_hor_part2") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            InstanceScript* _instance = creature->GetInstanceScript();
            if (!_instance)
                return false;

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                {
                    player->CLOSE_GOSSIP_MENU();
                    ((npc_jaina_and_sylvana_hor_part2AI*)creature->AI())->Start(false, true);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER|UNIT_NPC_FLAG_GOSSIP);
                    creature->SetUInt64Value(UNIT_FIELD_TARGET, 0);
                    creature->setActive(true);

                    _instance->SetData64(DATA_ESCAPE_LIDER, creature->GetGUID());
                    _instance->SetData(DATA_LICHKING_EVENT, IN_PROGRESS);
                    return true;
                }
                default:
                    return false;
            }
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            InstanceScript* _instance = creature->GetInstanceScript();
            if (!_instance)
                return false;

            if (_instance->GetData(DATA_LICHKING_EVENT) == DONE)
                return false;

            if (creature->isQuestGiver())
                player->PrepareQuestMenu(creature->GetGUID());

            player->ADD_GOSSIP_ITEM_DB(10909, 0, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_jaina_and_sylvana_hor_part2AI(creature);
        }

        struct npc_jaina_and_sylvana_hor_part2AI : public npc_escortAI
        {
            npc_jaina_and_sylvana_hor_part2AI(Creature *creature) : npc_escortAI(creature)
            {
                _instance = creature->GetInstanceScript();
                Reset();
            }

            InstanceScript* _instance;

            uint32 CastTimer;
            uint32 StepTimer;
            uint32 Step;
            int32 HoldTimer;
            uint32 Count;
            bool Fight;
            bool Event;
            bool PreFight;
            bool WallCast;
            uint64 m_uiLiderGUID;
            uint64 _iceWallGUID;
            uint64 wallTargetGUID;

            void Reset()
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS)
                    return;

                Step = 0;
                StepTimer = 500;
                Fight = true;
                me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER|UNIT_NPC_FLAG_GOSSIP);
                wallTargetGUID = 0;

                if (me->GetEntry() == NPC_JAINA_PART2)
                {
                    me->CastSpell(me, SPELL_JAINA_ICEBARRIER, false);
                    me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
                }
                else
                    me->CastSpell(me, SPELL_SYLVANAS_CLOAKOFDARKNESS, false);

                if (_instance->GetData(DATA_LICHKING_EVENT) == DONE)
                    me->SetVisible(false);
            }

            void AttackStart(Unit* who)
            {
                if (!who)
                    return;

                if (me->GetEntry() != NPC_SYLVANAS_PART2)
                    return;

                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS || Fight != true)
                    return;

                npc_escortAI::AttackStart(who);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (!_instance)
                    return;

                _instance->SetData(DATA_LICHKING_EVENT, FAIL);
            }

            void WaypointReached(uint32 i)
            {
                if (!_instance)
                    return;

                switch(i)
                {
                    case 3:
                        _instance->SetData(DATA_ICE_WALL_STATE_1, IN_PROGRESS);
                        if (GameObject* gate = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_ICE_WALL_1)))
                        {
                            gate->SetGoState(GO_STATE_READY);
                            _iceWallGUID = gate->GetGUID();
                        }
                        break;
                    case 4:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_WALL_01 : SAY_SYLVANA_WALL_01);
                        CastTimer = 1000;
                        HoldTimer = 30000;
                        SetEscortPaused(true);
                        if (Creature* wallTarget = me->SummonCreature(NPC_ICE_WALL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 720000))
                        {
                            wallTargetGUID = wallTarget->GetGUID();
                            if (me->GetEntry() == NPC_JAINA_PART2)
                                me->CastSpell(me, SPELL_JAINA_DESTROY_ICE_WALL, false);
                        }
                        WallCast = true;
                        break;
                    case 6:
                        _instance->SetData(DATA_ICE_WALL_STATE_2, IN_PROGRESS);

                        if (!wallTargetGUID)
                            break;

                        if (Creature* wallTarget = _instance->instance->GetCreature(wallTargetGUID))
                        {
                            if(wallTarget->isAlive())
                            {
                                wallTarget->DespawnOrUnsummon();
                                wallTargetGUID = 0;
                            }
                        }
                        break;
                    case 8:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_WALL_02 : SAY_SYLVANA_WALL_02);
                        CastTimer = 1000;
                        HoldTimer = 30000;
                        SetEscortPaused(true);
                        if (Creature* wallTarget = me->SummonCreature(NPC_ICE_WALL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 720000))
                        {
                            wallTargetGUID = wallTarget->GetGUID();
                            if (me->GetEntry() == NPC_JAINA_PART2)
                                me->CastSpell(me, SPELL_JAINA_DESTROY_ICE_WALL, false);
                        }
                        WallCast = true;
                        break;
                    case 9:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_ESCAPE_01 : SAY_SYLVANA_ESCAPE_01);
                        break;
                    case 11:
                        _instance->SetData(DATA_ICE_WALL_STATE_3, IN_PROGRESS);

                        if (!wallTargetGUID)
                            break;

                        if (Creature* wallTarget = _instance->instance->GetCreature(wallTargetGUID))
                        {
                            if (wallTarget->isAlive())
                            {
                                wallTarget->DespawnOrUnsummon();
                                wallTargetGUID = 0;
                            }
                        }
                        break;
                    case 12:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_WALL_03 : SAY_SYLVANA_WALL_03);
                        CastTimer = 1000;
                        HoldTimer = 30000;
                        SetEscortPaused(true);
                        if (Creature* wallTarget = me->SummonCreature(NPC_ICE_WALL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 720000))
                        {
                            wallTargetGUID = wallTarget->GetGUID();
                            if (me->GetEntry() == NPC_JAINA_PART2)
                                me->CastSpell(me, SPELL_JAINA_DESTROY_ICE_WALL, false);
                        }
                        WallCast = true;
                        break;
                    case 13:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_ESCAPE_02 : SAY_SYLVANA_ESCAPE_02);
                        break;
                    case 15:
                        _instance->SetData(DATA_ICE_WALL_STATE_4, IN_PROGRESS);

                        if (!wallTargetGUID)
                            break;

                        if (Creature* wallTarget = _instance->instance->GetCreature(wallTargetGUID))
                        {
                            if (wallTarget->isAlive())
                            {
                                wallTarget->DespawnOrUnsummon();
                                wallTargetGUID = 0;
                            }
                        }
                        break;
                    case 16:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_WALL_04 : SAY_SYLVANA_WALL_04);
                        CastTimer = 1000;
                        HoldTimer = 30000;
                        SetEscortPaused(true);
                        if (Creature* wallTarget = me->SummonCreature(NPC_ICE_WALL, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 720000))
                        {
                            wallTargetGUID = wallTarget->GetGUID();
                            if (me->GetEntry() == NPC_JAINA_PART2)
                                me->CastSpell(me, SPELL_JAINA_DESTROY_ICE_WALL, false);
                        }
                        WallCast = true;
                        break;
                    case 19:
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_TRAP : SAY_SYLVANA_TRAP);
                        break;
                    case 20:
                        if (!wallTargetGUID)
                            break;

                        if (Creature* wallTarget = _instance->instance->GetCreature(wallTargetGUID))
                        {
                            if (wallTarget->isAlive())
                            {
                                wallTarget->DespawnOrUnsummon();
                                wallTargetGUID = 0;
                            }
                        }
                        SetEscortPaused(true);
                        if (me->GetEntry() == NPC_JAINA_PART2)
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY2HL);
                        else
                            me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_READY1H);
                        break;
                }
            }

            void MoveInLineOfSight(Unit* who)
            {
                if(!who || !_instance)
                    return;

                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Player* player = who->ToPlayer();

                if (player->GetTeam() == ALLIANCE && me->GetEntry() == NPC_SYLVANAS_PART2)
                    return;

                if (player->GetTeam() == HORDE && me->GetEntry() == NPC_JAINA_PART2)
                    return;

                if (me->IsWithinDistInMap(who, 50.0f) && _instance->GetData(DATA_FROSWORN_EVENT) == DONE && _instance->GetData(DATA_PHASE) == 3)
                {
                    Event = true;
                    me->setFaction(2076);
                    _instance->SetData(DATA_PHASE, 4);
                }
            }

            void DamageTaken(Unit* /*pDoneBy*/, uint32 &uiDamage)
            {
                if (!_instance)
                    return;

                if (_instance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
                {
                    uiDamage = 0;
                    return;
                }

                if (_instance->GetData(DATA_LICHKING_EVENT) == IN_PROGRESS && WallCast == true)
                {
                    HoldTimer = HoldTimer + 100;
                    return;
                }
            }

            void JumpNextStep(uint32 Time)
            {
                StepTimer = Time;
                Step++;
            }

            void Intro()
            {
                switch (Step)
                {
                    case 0:
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);
                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                            me->SetUInt64Value(UNIT_FIELD_TARGET, lichking->GetGUID());
                        JumpNextStep(100);
                        break;
                    case 1:
                        HoRQuestComplete(me->GetEntry());
                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                        {
                            lichking->AI()->Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_LICH_KING_AGGRO_A : SAY_LICH_KING_AGGRO_H);
                            lichking->AI()->AttackStart(me);
                            me->AI()->AttackStart(lichking);
                        }
                        JumpNextStep(3000);
                        break;
                    case 2:
                        if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        {
                            Fight = false;
                            me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
                            JumpNextStep(3000);
                        }
                        else
                            JumpNextStep(100);
                        break;
                    case 3:
                        if (me->GetEntry() == NPC_SYLVANAS_PART2)
                            Fight = true;
                        JumpNextStep(100);
                        break;
                    case 4:
                        if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        {
                            if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                                me->CastSpell(lichking, SPELL_SYLVANAS_SHADOWSTEP, false);
                            JumpNextStep(3000);
                        }
                        else
                            JumpNextStep(100);
                        break;
                    case 5:
                        if (me->GetEntry() == NPC_SYLVANAS_PART2)
                        {
                            Fight = false;
                            me->GetMotionMaster()->MovePoint(0, (me->GetPositionX()-5)+rand()%10, (me->GetPositionY()-5)+rand()%10, me->GetPositionZ());
                            JumpNextStep(3000);
                        }
                        else
                            JumpNextStep(12000);
                        break;
                    case 6:
                        Fight = true;

                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                        {
                            if (me->GetEntry() == NPC_SYLVANAS_PART2)
                                DoCast(SPELL_SYLVANAS_JUMP);
                            DoCast(lichking, me->GetEntry() == NPC_JAINA_PART2 ? SPELL_JAINA_ICEPRISON : SPELL_SYLVANAS_DARKBINDING, false);
                            lichking->AttackStop();
                        }
                        JumpNextStep(1480);
                        break;
                    case 7:
                        me->RemoveAllAuras();

                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                            if (!lichking->HasAura(SPELL_JAINA_ICEPRISON) && !lichking->HasAura(SPELL_SYLVANAS_DARKBINDING))
                                lichking->AddAura(me->GetEntry() == NPC_JAINA_PART2 ? SPELL_JAINA_ICEPRISON : SPELL_SYLVANAS_DARKBINDING, lichking);

                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                        me->AttackStop();

                        if (me->GetEntry() == NPC_JAINA_PART2)
                        {
                            me->RemoveAurasDueToSpell(SPELL_JAINA_ICEBARRIER);
                            Talk(SAY_JAINA_AGGRO);
                        }
                        else
                        {
                            me->RemoveAurasDueToSpell(SPELL_SYLVANAS_CLOAKOFDARKNESS);
                            Talk(SAY_SYLVANA_AGGRO);
                        }
                        JumpNextStep(3000);
                        break;
                    case 8:
                        me->GetMotionMaster()->MovePoint(0, 5577.187f, 2236.003f, 733.012f);
                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                        {
                            if (!lichking->HasAura(SPELL_JAINA_ICEPRISON) && !lichking->HasAura(SPELL_SYLVANAS_DARKBINDING))
                            {
                                lichking->AddAura(me->GetEntry() == NPC_JAINA_PART2 ? SPELL_JAINA_ICEPRISON : SPELL_SYLVANAS_DARKBINDING, lichking);
                                me->SetUInt64Value(UNIT_FIELD_TARGET, lichking->GetGUID());
                            }
                        }
                        JumpNextStep(10000);
                        break;
                    case 9:
                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                        {
                            if (!lichking->HasAura(SPELL_JAINA_ICEPRISON) && !lichking->HasAura(SPELL_SYLVANAS_DARKBINDING))
                            {
                                lichking->AddAura(me->GetEntry() == NPC_JAINA_PART2 ? SPELL_JAINA_ICEPRISON : SPELL_SYLVANAS_DARKBINDING, lichking);
                                me->SetUInt64Value(UNIT_FIELD_TARGET, lichking->GetGUID());
                            }
                        }
                        me->RemoveAllAuras();
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        Count = 1;
                        JumpNextStep(100);
                        break;
                }
            }

            void Outro()
            {
                switch(Step)
                {
                    case 10:
                        JumpNextStep(10000);
                        break;
                    case 11:
                        if (GameObject* gunship = ObjectAccessor::GetGameObject(*me, _instance->GetData64(me->GetEntry() == NPC_JAINA_PART2 ? DATA_SKYBREAKER : DATA_ORGRIM_HAMMER)))
                            gunship->SetPhaseMask(1, true);

                        if (Creature* captain = me->SummonCreature(me->GetEntry() == NPC_JAINA_PART2 ? NPC_BARTLETT : NPC_KORM, 5251.17f, 1610.2f, 795.812f, 2.07997f))
                            captain->AI()->Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_BARTLETT_FIRE : SAY_KORM_FIRE);

                        if (GameObject* cave = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_CAVE)))
                        {
                            if (Creature* caveTarget = me->SummonCreature(NPC_ICE_WALL, cave->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 10.0f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 720000))
                            {
                                caveTarget->SetFloatValue(OBJECT_FIELD_SCALE_X, 4);

                                for (uint8 i = 0; i < 4; ++i)
                                {
                                    if (Creature* cannoner = me->SummonCreature(NPC_ICE_WALL, CannonSpawns[i], TEMPSUMMON_MANUAL_DESPAWN, 720000))
                                    {
                                        cannoner->SetDisplayId(cannoner->GetCreatureTemplate()->Modelid2);
                                        cannoner->SetVisible(true);
                                        cannoner->setFaction(me->getFaction());
                                        cannoner->CastSpell(caveTarget, SPELL_FIRE_CANNON, true);
                                    }
                                }
                            }
                        }
                        JumpNextStep(6000);
                        break;
                    case 12:
                        if (GameObject* cave = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_CAVE)))
                            cave->SetGoState(GO_STATE_READY);

                        me->RemoveAllAuras();

                        if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                            lichking->DespawnOrUnsummon();

                        HoRQuestComplete(38211);

                        if (IsHeroic())
                        {
                            if (me->GetEntry() == NPC_SYLVANAS_PART2)
                                me->SummonGameObject(GO_CAPTAIN_CHEST_3, ChestPos.GetPositionX(), ChestPos.GetPositionY(), ChestPos.GetPositionZ(), ChestPos.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY);
                            else
                                me->SummonGameObject(GO_CAPTAIN_CHEST_4, ChestPos.GetPositionX(), ChestPos.GetPositionY(), ChestPos.GetPositionZ(), ChestPos.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY);
                        }
                        else
                        {
                            if (me->GetEntry() == NPC_SYLVANAS_PART2)
                                me->SummonGameObject(GO_CAPTAIN_CHEST_1, ChestPos.GetPositionX(), ChestPos.GetPositionY(), ChestPos.GetPositionZ(), ChestPos.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY);
                            else
                                me->SummonGameObject(GO_CAPTAIN_CHEST_2, ChestPos.GetPositionX(), ChestPos.GetPositionY(), ChestPos.GetPositionZ(), ChestPos.GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, DAY);
                        }
                        JumpNextStep(10000);
                        break;
                    case 13:
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                        _instance->SetData(DATA_LICHKING_EVENT, DONE);
                        Talk(me->GetEntry() == NPC_JAINA_PART2 ? SAY_JAINA_FINAL_1 : SAY_SYLVANA_FINAL_1);
                        //me->SummonGameObject(me->GetEntry() == NPC_JAINA_PART2 ? GO_STAIRS_SKYBREAKER : GO_STAIRS_ORGRIM_HAMMER, 5247.45f, 1627.72f, 784.302f, 5.88208f, 0, 0, 0.199211f, -0.979957f, 1*DAY);
                        JumpNextStep(10000);
                        break;
                    case 14:
                        JumpNextStep(20000);
                        break;
                    case 15:
                        JumpNextStep(5000);
                        break;
                    case 16:
                        me->SetOrientation(0.68f);
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                        JumpNextStep(5000);
                        break;
                }
            }

            void HoRQuestComplete(uint32 killCredit)
            {
                if (_instance)
                {
                    Map::PlayerList const &PlayerList = _instance->instance->GetPlayers();
                    for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                        i->getSource()->KilledMonsterCredit(killCredit, 0);
                }
            }

            void UpdateEscortAI(uint32 const diff)
            {
                if (!_instance || !Event)
                    return;

                DoMeleeAttackIfReady();

                if (_instance->GetData(DATA_PHASE) == 4 && _instance->GetData(DATA_LICHKING_EVENT) != IN_PROGRESS)
                {
                    if (StepTimer < diff)
                        Intro();
                    else
                        StepTimer -= diff;
                }

                if (_instance->GetData(DATA_LICHKING_EVENT) == SPECIAL && _instance->GetData(DATA_PHASE) != 6)       //End Cinematic
                {
                    _instance->SetData(DATA_PHASE, 6);
                    Step = 10;
                }

                if (_instance->GetData(DATA_PHASE) == 6)
                {
                    if (StepTimer < diff)
                        Outro();
                    else
                        StepTimer -= diff;
                    return;
                }

                if (WallCast == true && CastTimer < diff)
                {
                    if (me->GetEntry() == NPC_SYLVANAS_PART2)
                    {
                        if (Creature* wallTarget = _instance->instance->GetCreature(wallTargetGUID))
                            me->CastSpell(wallTarget, SPELL_SYLVANAS_DESTROY_ICE_WALL, false);
                        CastTimer = 1000;
                    }
                }
                else
                    CastTimer -= diff;

                if (WallCast == true && HoldTimer < 10000 && (_instance->GetData(DATA_SUMMONS) == 0 || !me->isInCombat()))
                {
                    WallCast = false;
                    me->InterruptNonMeleeSpells(false);
                    SetEscortPaused(false);
                    if (GameObject* gate = _instance->instance->GetGameObject(_iceWallGUID))
                        gate->SetGoState(GO_STATE_ACTIVE);
                    ++Count;
                    switch (Count)
                    {
                        case 2:
                            if (GameObject* gate = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_ICE_WALL_2)))
                            {
                                gate->SetGoState(GO_STATE_READY);
                                _iceWallGUID = gate->GetGUID();
                            }
                            break;
                        case 3:
                            if (GameObject* gate = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_ICE_WALL_3)))
                            {
                                gate->SetGoState(GO_STATE_READY);
                                _iceWallGUID = gate->GetGUID();
                            }
                            break;
                        case 4:
                            if (GameObject* gate = ObjectAccessor::GetGameObject(*me, _instance->GetData64(DATA_ICE_WALL_4)))
                            {
                                gate->SetGoState(GO_STATE_READY);
                                _iceWallGUID = gate->GetGUID();
                            }
                            break;
                        case 5:
                            if (Creature* lichking = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_LICH_LING_PART2) : 0))
                            {
                                if (lichking->isAlive())
                                {
                                    lichking->RemoveAurasDueToSpell(SPELL_REMORSELESS_WINTER);
                                    lichking->SetSpeed(MOVE_WALK, 2.5f, true);
                                    Step = 0;
                                }
                            }
                            break;
                    }
                }
                else
                {
                    HoldTimer -= diff;
                    if (HoldTimer <= 0)
                        HoldTimer = 0;
                }

                return;
            }
        };
};

class at_hor_intro_start : public AreaTriggerScript
{
    public:
        at_hor_intro_start() : AreaTriggerScript("at_hor_intro_start") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            InstanceScript* _instance = player->GetInstanceScript();

            if (player->isGameMaster())
                return true;

            if (_instance->GetData(DATA_INTRO_EVENT) == NOT_STARTED)
                _instance->SetData(DATA_INTRO_EVENT, IN_PROGRESS);

            return true;
        }
};

class at_hor_waves_restarter : public AreaTriggerScript
{
    public:
        at_hor_waves_restarter() : AreaTriggerScript("at_hor_waves_restarter") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            InstanceScript* _instance = player->GetInstanceScript();

            if (player->isGameMaster() || _instance->GetData(DATA_WAVE_COUNT))
                return true;

            if (_instance->GetData(DATA_INTRO_EVENT) == DONE && _instance->GetBossState(DATA_MARWYN_EVENT) != DONE)
            {
                _instance->ProcessEvent(0, EVENT_SPAWN_WAVES);

                if (Creature* falric = player->GetCreature(*player, _instance->GetData64(DATA_FALRIC_EVENT)))
                {
                    falric->CastSpell(falric, SPELL_BOSS_SPAWN_AURA, true);
                    falric->SetVisible(true);
                }

                if (Creature* marwyn = player->GetCreature(*player, _instance->GetData64(DATA_MARWYN_EVENT)))
                {
                    marwyn->CastSpell(marwyn, SPELL_BOSS_SPAWN_AURA, true);
                    marwyn->SetVisible(true);
                }
            }
            return true;
        }
};

class at_hor_door_teleport : public AreaTriggerScript
{
    public:
        at_hor_door_teleport() : AreaTriggerScript("at_hor_door_teleport") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            InstanceScript* _instance = player->GetInstanceScript();
            if (!_instance || player->isGameMaster())
                return true;

            if (_instance->GetBossState(DATA_MARWYN_EVENT) == DONE && _instance->GetBossState(DATA_FALRIC_EVENT) == DONE)
                return true;
                
            player->NearTeleportTo(5349.94f, 2049.32f, 707.69f, 0.8541f);
            return true;
        }
};

void AddSC_halls_of_reflection()
{
    new npc_jaina_or_sylvanas_hor();
    new npc_ghostly_priest();
    new npc_phantom_mage();
    new npc_phantom_hallucination();
    new npc_shadowy_mercenary();
    new npc_spectral_footman();
    new npc_tortured_rifleman();
    new at_hor_intro_start();
    new at_hor_waves_restarter();
    new at_hor_door_teleport();
    new npc_frostworn_general();
    new npc_queldelar();
    new npc_spiritual_reflection();
    new npc_jaina_and_sylvana_hor_part2();
}
