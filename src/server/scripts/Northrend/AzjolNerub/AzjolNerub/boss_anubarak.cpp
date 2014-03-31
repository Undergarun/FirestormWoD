/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
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

#include "ScriptPCH.h"
#include "azjol_nerub.h"

enum Spells
{
    SPELL_CARRION_BEETLES            = 53520,
    SPELL_SUMMON_CARRION_BEETLES     = 53521,
    SPELL_LEECHING_SWARM             = 53467,
    SPELL_LEECHING_SWARM_H           = 59430,
    SPELL_POUND                      = 53472,
    SPELL_POUND_H                    = 59433,
    SPELL_POUND_DMG                  = 53509,
    SPELL_POUND_DMG_H                = 59432,
    SPELL_SUBMERGE                   = 53421,
    SPELL_IMPALE_DMG                 = 53454,
    SPELL_IMPALE_DMG_H               = 59446,
    SPELL_IMPALE_SHAKEGROUND         = 53455,
  //SPELL_IMPALE_TARGET              = 53458
};

enum Creatures
{
    NPC_DARTER                       = 29213,
    NPC_GUARDIAN                     = 29216,
    NPC_VENOMANCER                   = 29217,
    NPC_CARRION_BEETLE               = 29209,
    NPC_IMPALE_TARGET                = 29184
};

// Not in DB
enum Yells
{
    SAY_INTRO                        = -1601010,
    SAY_AGGRO                        = -1601000,
    SAY_SLAY_1                       = -1601001,
    SAY_SLAY_2                       = -1601002,
    SAY_SLAY_3                       = -1601003,
    SAY_LOCUST_1                     = -1601005,
    SAY_LOCUST_2                     = -1601006,
    SAY_LOCUST_3                     = -1601007,
    SAY_SUBMERGE_1                   = -1601008,
    SAY_SUBMERGE_2                   = -1601009,
    SAY_DEATH                        = -1601004
};

enum Achievement
{
    ACHIEV_TIMED_START_EVENT         = 20381
};

enum Events
{
    EVENT_LEECHING_SWARM = 1,
    EVENT_BEETLES,
    EVENT_PREPARE_POUND,
    EVENT_POUND,
    EVENT_EMERGE,
    EVENT_SUMMON_GUARDIAN,
    EVENT_SUMMON_VENOMANCER,
    EVENT_SUMMON_DARTER,
    EVENT_IMPALE,
    EVENT_IMPALE_DMG
};

Position const SpawnPoint[2] =
{
    { 550.7f, 282.8f, 224.3f, 0.0f },
    { 551.1f, 229.4f, 224.3f, 0.0f }
};

Position const SpawnPointGuardian[2] =
{
    { 550.348633f, 316.006805f, 234.2947f, 0.0f },
    { 550.188660f, 324.264557f, 237.7412f, 0.0f }
};

class boss_anub_arak : public CreatureScript
{
    public:
        boss_anub_arak() : CreatureScript("boss_anub_arak") { }

        struct boss_anub_arakAI : public ScriptedAI
        {
            boss_anub_arakAI(Creature* c) : ScriptedAI(c), _summons(me)
            {
                _instance = c->GetInstanceScript();
            }

            void Reset()
            {
                _addsAlive = 0;
                _targetGUID = 0;
                _submergeCount = 0;
                _summons.DespawnAll();
                _events.Reset();

                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 5.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 5.0f);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveAura(SPELL_SUBMERGE);

                if (_instance)
                {
                    _instance->SetData(DATA_ANUBARAK_EVENT, NOT_STARTED);
                    _instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
                }
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);

                _events.ScheduleEvent(EVENT_LEECHING_SWARM, 20*IN_MILLISECONDS);
                _events.ScheduleEvent(EVENT_PREPARE_POUND, 14*IN_MILLISECONDS);

                if (_instance)
                {
                    _instance->SetData(DATA_ANUBARAK_EVENT, IN_PROGRESS);
                    _instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
                }
            }

            void DamageTaken(Unit* /*attacker*/, uint32 &damage)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING) || me->HasAura(SPELL_SUBMERGE))
                    return;

                if (me->HealthBelowPctDamaged(100 - 25 * (_submergeCount + 1), damage))
                {
                    _events.Reset();
                    _events.ScheduleEvent(EVENT_EMERGE, 40*IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_SUMMON_GUARDIAN, 5*IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_SUMMON_DARTER, 8*IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_IMPALE, 4*IN_MILLISECONDS);

                    if (_submergeCount)
                        _events.ScheduleEvent(EVENT_SUMMON_VENOMANCER, 25*IN_MILLISECONDS);

                    ++_submergeCount;
                    me->RemoveAllAuras();
                    DoCast(me, SPELL_SUBMERGE, false);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            void SpellHitTarget(Unit* target, SpellInfo const* spell)
            {
                if (spell->Id == DUNGEON_MODE<uint32>(SPELL_POUND, SPELL_POUND_H))
                    DoCast(target, DUNGEON_MODE<uint32>(SPELL_POUND_DMG, SPELL_POUND_DMG_H), true);
            }

            void JustSummoned(Creature* summon)
            {
                _summons.Summon(summon);

                switch (summon->GetEntry())
                {
                    case NPC_GUARDIAN:
                    case NPC_VENOMANCER:
                        ++_addsAlive;
                        DoZoneInCombat(summon, 100.0f);
                        if (Unit* target = me->getVictim())
                            summon->AI()->AttackStart(target);
                        break;
                    case NPC_DARTER:
                    case NPC_CARRION_BEETLE:
                        DoZoneInCombat(summon, 100.0f);
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            summon->AI()->AttackStart(target);
                        break;
                    case NPC_IMPALE_TARGET:
                        _targetGUID = summon->GetGUID();
                        summon->SetReactState(REACT_PASSIVE);
                        summon->SetDisplayId(summon->GetCreatureTemplate()->Modelid2);
                        summon->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        summon->CastSpell(summon, SPELL_IMPALE_SHAKEGROUND, true);
                        break;
                }
            }

            void SummonedCreatureDies(Creature* summon, Unit* /*killer*/)
            {
                if (summon->GetEntry() == NPC_GUARDIAN || summon->GetEntry() == NPC_VENOMANCER)
                    --_addsAlive;
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasReactState(REACT_PASSIVE) && !me->getVictim())
                    me->SetReactState(REACT_AGGRESSIVE);

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Phase 1
                        case EVENT_LEECHING_SWARM:
                            DoCast(me, DUNGEON_MODE<uint32>(SPELL_LEECHING_SWARM, SPELL_LEECHING_SWARM_H));
                            _events.ScheduleEvent(EVENT_LEECHING_SWARM, urand(30, 35) *IN_MILLISECONDS);
                            _events.ScheduleEvent(EVENT_BEETLES, 4*IN_MILLISECONDS);
                            break;
                        case EVENT_BEETLES:
                            DoCast(SPELL_CARRION_BEETLES);
                            break;
                        case EVENT_PREPARE_POUND:
                        {
                            float x, y, z;
                            z = me->GetPositionZ();
                            me->GetNearPoint2D(x, y, 3.0f, me->GetOrientation());

                            if (Creature* target = me->SummonCreature(22515, x, y, z + 2.0f, 0.0f, TEMPSUMMON_TIMED_DESPAWN, 4500))
                            {
                                _targetGUID = target->GetGUID();
                                target->SetMaxHealth(100000);
                                target->SetHealth(100000);
                                target->SetReactState(REACT_PASSIVE);
                                target->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_IMMUNE_TO_NPC);
                                AttackStart(target);
                                me->SetReactState(REACT_PASSIVE);
                                me->GetMotionMaster()->MoveFollow(target, 0.0f, 0.0f);
                            }
                            _events.DelayEvents(750);
                            _events.ScheduleEvent(EVENT_POUND, 700);
                            return;
                        }
                        case EVENT_POUND:
                        {
                            if (Creature* target = ObjectAccessor::GetCreature(*me, _targetGUID))
                            {
                                DoCast(target, DUNGEON_MODE<uint32>(SPELL_POUND, SPELL_POUND_H));
                                me->SetFacingToObject(target);
                            }
                            _targetGUID = 0;
                            _events.ScheduleEvent(EVENT_PREPARE_POUND, urand(17, 20) *IN_MILLISECONDS);
                            return;
                        }
                        // Phase 2
                        case EVENT_EMERGE:
                            if (!_addsAlive)
                            {
                                me->RemoveAura(SPELL_SUBMERGE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                                _events.Reset();
                                _events.ScheduleEvent(EVENT_LEECHING_SWARM, urand(2, 15) *IN_MILLISECONDS);
                                _events.ScheduleEvent(EVENT_PREPARE_POUND, urand(3, 7) *IN_MILLISECONDS);
                            }
                            else
                                _events.ScheduleEvent(EVENT_EMERGE, 1*IN_MILLISECONDS);
                            return;
                        case EVENT_SUMMON_GUARDIAN:
                            for (uint8 i = 0; i < 2; ++i)
                                me->SummonCreature(NPC_GUARDIAN, SpawnPointGuardian[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3*IN_MILLISECONDS);
                            break;
                        case EVENT_SUMMON_VENOMANCER:
                            for (uint8 i = 0; i < 2; ++i)
                                me->SummonCreature(NPC_VENOMANCER, SpawnPointGuardian[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3*IN_MILLISECONDS);
                            break;
                        case EVENT_SUMMON_DARTER:
                            for (uint8 i = 0; i < 2; ++i)
                                me->SummonCreature(NPC_DARTER, SpawnPoint[i], TEMPSUMMON_CORPSE_TIMED_DESPAWN, 3*IN_MILLISECONDS);
                            _events.ScheduleEvent(EVENT_SUMMON_DARTER, 30*IN_MILLISECONDS / _submergeCount);
                            break;
                        case EVENT_IMPALE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                                me->SummonCreature(NPC_IMPALE_TARGET, *target, TEMPSUMMON_TIMED_DESPAWN, 5*IN_MILLISECONDS);
                            _events.ScheduleEvent(EVENT_IMPALE_DMG, 4*IN_MILLISECONDS);
                            break;
                        case EVENT_IMPALE_DMG:
                            if (Creature* target = ObjectAccessor::GetCreature(*me, _targetGUID))
                            {
                                target->RemoveAurasDueToSpell(SPELL_IMPALE_SHAKEGROUND);
                                me->CastSpell(target, DUNGEON_MODE<uint32>(SPELL_IMPALE_DMG, SPELL_IMPALE_DMG_H), true);
                            }
                            _targetGUID = 0;
                            _events.ScheduleEvent(EVENT_IMPALE, 5*IN_MILLISECONDS);
                            break;
                        default:
                            break;
                    }
                }

                if (!me->HasAura(SPELL_SUBMERGE))
                    DoMeleeAttackIfReady();
            }

            void KilledUnit(Unit* victim)
            {
                if (victim == me)
                    return;

                DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3), me);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoScriptText(SAY_DEATH, me);
                _summons.DespawnAll();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                if (_instance)
                    _instance->SetData(DATA_ANUBARAK_EVENT, DONE);

                // quest stuff
                me->SummonCreature(NPC_RECLAIMER_AZAK, questPos[0]);
                me->SummonCreature(NPC_AZJOL_ANAK_WARRIOR, questPos[1]);
                me->SummonCreature(NPC_AZJOL_ANAK_WARRIOR, questPos[2]);
            }

        private:
            InstanceScript* _instance;
            SummonList _summons;
            EventMap _events;
            uint8 _addsAlive;
            uint8 _submergeCount;
            uint64 _targetGUID;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_anub_arakAI(creature);
        }
};

void AddSC_boss_anub_arak()
{
    new boss_anub_arak();
}
