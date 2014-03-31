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

// TODO: - more add spawn points
//       - nonelite spells
//       - spawn 2 more crusher groups
//       - web doors

#include "ScriptPCH.h"
#include "azjol_nerub.h"

enum Spells
{
    SPELL_ACID_CLOUD          = 53400,
    H_SPELL_ACID_CLOUD        = 59419,
    SPELL_LEECH_POISON        = 53030,
    SPELL_LEECH_POISON_PCT    = 53800,
    H_SPELL_LEECH_POISON      = 59417,
    SPELL_PIERCE_ARMOR        = 53418,
    SPELL_WEB_GRAB            = 57731,
    H_SPELL_WEB_GRAB          = 59421,
    SPELL_WEB_FRONT_DOORS     = 53177,
    SPELL_WEB_SIDE_DOORS      = 53185,

    // anubar crusher
    SPELL_FRENZY              = 53801,
    SPELL_SMASH               = 53318,
    H_SPELL_SMASH             = 59346
};

enum Creatures
{
    NPC_HADRONOX              = 28921,
    NPC_ANUBAR_CHAMPION       = 29117,
    NPC_ANUBAR_CRYPT_FIEND    = 29118,
    NPC_ADD_CHAMPION          = 29062,
    NPC_ADD_CRYPT_FIEND       = 29063,
    NPC_ADD_NECROMANCER       = 29064
};

enum Events
{
    EVENT_CLOUD = 1,
    EVENT_LEECH,
    EVENT_PIECRE,
    EVENT_GRAB,
    EVENT_SPAWN,
    EVENT_FORCEMOVE,
    EVENT_UNSTUCK // temporary
};

Position const HadronoxWaypoints[14] =
{
    {565.681458f, 513.247803f, 698.7f, 0.0f},
    {578.135559f, 512.468811f, 698.5f, 0.0f},
    {596.820007f, 510.811249f, 694.8f, 0.0f},
    {608.183777f, 513.395508f, 695.5f, 0.0f},
    {618.232422f, 525.414185f, 697.0f, 0.0f},
    {620.192932f, 539.329041f, 706.3f, 0.0f},
    {615.690979f, 552.474915f, 713.2f, 0.0f},
    {607.791870f, 566.636841f, 720.1f, 0.0f},
    {599.256104f, 580.134399f, 724.7f, 0.0f},
    {591.600220f, 589.028748f, 730.7f, 0.0f},
    {587.181580f, 596.026489f, 739.5f, 0.0f},
    {577.790588f, 583.640930f, 727.9f, 0.0f},
    {559.274597f, 563.085999f, 728.7f, 0.0f},
    {528.960815f, 565.453613f, 733.2f, 0.0f}
};

Position const AddWaypoints[6] =
{
    {582.961304f, 606.298645f, 739.3f, 0.0f},
    {590.240662f, 597.044556f, 739.2f, 0.0f},
    {600.471436f, 585.080200f, 726.2f, 0.0f},
    {611.900940f, 562.884094f, 718.9f, 0.0f},
    {615.533936f, 529.052002f, 703.3f, 0.0f},
    {606.844604f, 515.054199f, 696.2f, 0.0f}
};

class boss_hadronox : public CreatureScript
{
    public:
        boss_hadronox() : CreatureScript("boss_hadronox") { }

        struct boss_hadronoxAI : public ScriptedAI
        {
            boss_hadronoxAI(Creature* c) : ScriptedAI(c), _summons(me)
            {
                _instance = c->GetInstanceScript();
                _home = c->GetHomePosition();
            }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFloatValue(UNIT_FIELD_BOUNDINGRADIUS, 9.0f);
                me->SetFloatValue(UNIT_FIELD_COMBATREACH, 9.0f);
				me->GetMotionMaster()->Clear();
				me->GetMotionMaster()->MovePoint(50,me->GetHomePosition());

				_WaveCount = 0;
                _wpCount = 0;
                _wpReached = false;
                _movementStarted = false;
                _engaged = false;
                _denied = true;

                _events.Reset();
                _summons.DespawnAll();

                if (_instance)
                    _instance->SetData(DATA_HADRONOX_EVENT, NOT_STARTED);
            }

            void JustSummoned(Creature* summon)
            {
                _summons.push_back(summon->GetGUID()); //.Summon(summon);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || id != _wpCount)
                    return;

                if (id == 10)
                {
                    // TODO: web doors
                    _denied = false;
                    _events.CancelEvent(EVENT_SPAWN);
                }

                if (id < 13)
                {
                    ++_wpCount;
                    _wpReached = true;
                }
                else
                {
                    _events.CancelEvent(EVENT_FORCEMOVE);
                    me->SetReactState(REACT_AGGRESSIVE);
                }
            }

            void ElementalDamageTaken(Unit* attacker, uint32 /*damage*/, SpellSchoolMask /*damageSchoolMask*/)
            {
                if (!_engaged && attacker->GetCharmerOrOwnerPlayerOrPlayerItself())
                {
                    _engaged = true;
                    me->GetMotionMaster()->Clear();
                    me->SetReactState(REACT_AGGRESSIVE);
                    DoResetThreat();
                    DoZoneInCombat();
                    me->AddThreat(attacker, 9999.9f);
                    me->GetMotionMaster()->MoveChase(attacker);

                    _events.CancelEvent(EVENT_FORCEMOVE);
                    _events.ScheduleEvent(EVENT_PIECRE, urand(10, 15) *IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_GRAB, urand(15, 20) *IN_MILLISECONDS);
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_ENGAGED:
                        return _engaged ? 1 : 0;
                    case DATA_DENIED:
                        return _denied ? 1 : 0;
                }

                return 0;
            }

            void JustDied(Unit* /*killer*/)
            {
                if (_instance)
                    _instance->SetData(DATA_HADRONOX_EVENT, DONE);
            }

            void EnterEvadeMode()
            {
                if (!_engaged)
                {
                    if (_wpCount >= 13)
                        me->SetReactState(REACT_AGGRESSIVE);
                    else
                        me->SetReactState(REACT_PASSIVE);

                    return;
                }

                //_EnterEvadeMode();
                //me->SetHomePosition(_home);
                //me->GetMotionMaster()->MoveTargetedHome();
                Reset();
            }

            void UpdateAI(uint32 const diff)
            {
                if (me->IsVisible() && !_movementStarted && me->isInCombat())
                {
                    _movementStarted = true;
                    _wpReached = true;
                    _events.ScheduleEvent(EVENT_CLOUD, urand(5, 10) *IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_LEECH, urand(7, 12) *IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_UNSTUCK, 3*IN_MILLISECONDS);
                    _events.ScheduleEvent(EVENT_SPAWN, 1.5*IN_MILLISECONDS);
                }

                if (_wpReached)
                {
                    _wpReached = false;

                    if (me->isInCombat())
                    {
                        me->SetReactState(REACT_AGGRESSIVE);
                        _events.ScheduleEvent(EVENT_FORCEMOVE, urand(7, 10) *IN_MILLISECONDS);
                    }
                    else
                        me->GetMotionMaster()->MovePoint(_wpCount, HadronoxWaypoints[_wpCount]);
                }

                if (!UpdateVictim() && _engaged)
                    return;

                _events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = _events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_CLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60, false))
                                DoCast(target, DUNGEON_MODE(SPELL_ACID_CLOUD, H_SPELL_ACID_CLOUD));
                            _events.ScheduleEvent(EVENT_CLOUD, urand(25, 35) *IN_MILLISECONDS);
                            break;
                        case EVENT_LEECH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 20, false))
                                DoCast(target, DUNGEON_MODE(SPELL_LEECH_POISON, H_SPELL_LEECH_POISON));
                            _events.ScheduleEvent(EVENT_LEECH, urand(10, 20) *IN_MILLISECONDS);
                            break;
                        case EVENT_PIECRE:
                            DoCastVictim(SPELL_PIERCE_ARMOR);
                            _events.ScheduleEvent(EVENT_PIECRE, urand(15, 20) *IN_MILLISECONDS);
                            break;
                        case EVENT_GRAB:
                            DoCast(me, DUNGEON_MODE(SPELL_WEB_GRAB, H_SPELL_WEB_GRAB));
                            DoCast(me, DUNGEON_MODE(SPELL_ACID_CLOUD, H_SPELL_ACID_CLOUD));
                            _events.ScheduleEvent(EVENT_GRAB, urand(17, 23) *IN_MILLISECONDS);
                            break;
                        case EVENT_SPAWN:
                            me->SummonCreature(RAND(NPC_ADD_CHAMPION, NPC_ADD_CRYPT_FIEND, NPC_ADD_NECROMANCER), AddWaypoints[0]);
							_WaveCount++;
							if (_WaveCount <= 50 )  //prevent too much spawn & freeze serv
								_events.ScheduleEvent(EVENT_SPAWN, urand(1500, 3000));
                            break;
                        case EVENT_FORCEMOVE:
                            me->SetReactState(REACT_PASSIVE);
                            me->GetMotionMaster()->MovePoint(_wpCount, HadronoxWaypoints[_wpCount]);
                            break;
                        case EVENT_UNSTUCK:
                            if (me->getVictim())
                                if (me->IsWithinCombatRange(me->getVictim(), 10.0f) && !me->IsWithinLOSInMap(me->getVictim()))
                                    me->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 10.0f, 1.0f, 1.0f);
                            if (_engaged)
                            {
                                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 150.0f, true);
                                if (!target)
                                    EnterEvadeMode();
                            }
                            _events.ScheduleEvent(EVENT_UNSTUCK, 3*IN_MILLISECONDS);
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
            SummonList _summons;
            EventMap _events;
            Position _home;
            uint8 _wpCount;
			uint8 _WaveCount;
            bool _wpReached;
            bool _movementStarted;
            bool _engaged;
            bool _denied;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_hadronoxAI(creature);
        }
};

class npc_anubar_crusher : public CreatureScript
{
    public:
        npc_anubar_crusher() : CreatureScript("npc_anubar_crusher") { }

        struct npc_anubar_crusherAI : public ScriptedAI
        {
            npc_anubar_crusherAI(Creature* c) : ScriptedAI(c)
            {
                _instance = c->GetInstanceScript();
            }

            void Reset()
            {
                _smashTimer = urand(7, 10) *IN_MILLISECONDS;

                if (_instance)
                    _instance->SetData(DATA_HADRONOX_EVENT, NOT_STARTED);

				if (Creature* Hadronox = ObjectAccessor::GetCreature(*me, _instance->GetData64(DATA_HADRONOX)))
					Hadronox->AI()->Reset();

                if (Creature* champion = GetClosestCreatureWithEntry(me, NPC_ANUBAR_CHAMPION, 200.0f, false))
                    champion->Respawn();
                if (Creature* cryptFiend = GetClosestCreatureWithEntry(me, NPC_ANUBAR_CRYPT_FIEND, 200.0f, false))
                    cryptFiend->Respawn();
            }

            void EnterCombat(Unit* /*who*/)
            {
                if (_instance)
                    _instance->SetData(DATA_HADRONOX_EVENT, IN_PROGRESS);
            }

            void UpdateAI(uint32 const diff)
            {
                if (!UpdateVictim())
                    return;

                if (_smashTimer <= diff)
                {
                    DoCastVictim(DUNGEON_MODE(SPELL_SMASH, H_SPELL_SMASH));
                    _smashTimer = urand(10, 15) *IN_MILLISECONDS;
                }
                else
                    _smashTimer -= diff;

                if (HealthBelowPct(30) && !me->HasAura(SPELL_FRENZY))
                {
                    me->InterruptNonMeleeSpells(true);
                    DoCast(me, SPELL_FRENZY, true);
                }

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
            uint32 _smashTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_anubar_crusherAI(creature);
        }
};

class npc_hadronox_nerubian : public CreatureScript
{
    public:
        npc_hadronox_nerubian() : CreatureScript("npc_hadronox_nerubian") { }

        struct npc_hadronox_nerubianAI : public ScriptedAI
        {
            npc_hadronox_nerubianAI(Creature* c) : ScriptedAI(c)
            {
                _instance = c->GetInstanceScript();
            }

            void Reset()
            {
                _wpCount = 1;
                _wpReached = true;
                me->SetReactState(REACT_DEFENSIVE);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE || id != _wpCount)
                    return;

                if (id == 2)
                    if (Creature* hadronox = ObjectAccessor::GetCreature(*me, _instance ? _instance->GetData64(DATA_HADRONOX) : 0))
                        if (hadronox->AI()->GetData(DATA_ENGAGED))
                        {
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->SetInCombatWithZone();
                            me->AddThreat(hadronox, 1000.0f);
                            AttackStart(hadronox);
                            return;
                        }

                if (id < 5)
                {
                    ++_wpCount;
                    _wpReached = true;
                }
                else // somehow missed hadronox
                {
                    me->SetReactState(REACT_AGGRESSIVE);
                    me->SetInCombatWithZone();
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (_wpReached && !me->isInCombat())
                {
                    _wpReached = false;
                    me->GetMotionMaster()->MovePoint(_wpCount, AddWaypoints[_wpCount]);
                }

                if (me->HasReactState(REACT_DEFENSIVE))
                    if (Creature* hadronox = me->FindNearestCreature(NPC_HADRONOX, 20.0f))
                    {
                        me->GetMotionMaster()->Clear();
                        me->SetReactState(REACT_AGGRESSIVE);
                        me->AddThreat(hadronox, 1000.0f);
                        AttackStart(hadronox);
                    }

                if (!UpdateVictim())
                    return;

                /*
                TODO: spells
                */

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
            uint8 _wpCount;
            bool _wpReached;
            bool _movementStarted;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_hadronox_nerubianAI(creature);
        }
};

class spell_hadronox_leech_poison : public SpellScriptLoader
{
    public:
        spell_hadronox_leech_poison() : SpellScriptLoader("spell_hadronox_leech_poison") { }

        class spell_hadronox_leech_poison_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_hadronox_leech_poison_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* caster = GetCaster();
                if (caster && caster->isAlive() && GetTargetApplication()->GetRemoveMode() == AURA_REMOVE_BY_DEATH)
                    caster->CastSpell(caster, SPELL_LEECH_POISON_PCT, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_hadronox_leech_poison_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_LEECH, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_hadronox_leech_poison_AuraScript();
        }
};

class achievement_hadronox_denied : public AchievementCriteriaScript
{
    public:
        achievement_hadronox_denied() : AchievementCriteriaScript("achievement_hadronox_denied")
        {
        }

        bool OnCheck(Player* /*player*/, Unit* target)
        {
            if (!target)
                return false;

            if (Creature* hadronox = target->ToCreature())
                if (hadronox->AI()->GetData(DATA_DENIED))
                    return true;

            return false;
        }
};

void AddSC_boss_hadronox()
{
    new boss_hadronox();
    new npc_anubar_crusher();
    new npc_hadronox_nerubian();
    new spell_hadronox_leech_poison();
    new achievement_hadronox_denied();
}
