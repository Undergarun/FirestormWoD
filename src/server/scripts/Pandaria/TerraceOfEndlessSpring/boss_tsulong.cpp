/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "terrace_of_endless_spring.h"

enum eTsulongEvents
{
    EVENT_NONE,
    EVENT_FLY,
    EVENT_WAYPOINT_FIRST,
    EVENT_WAYPOINT_SECOND,
    EVENT_SWITCH_TO_NIGHT_PHASE,
    EVENT_SPAWN_SUNBEAM,
    EVENT_SHADOW_BREATH,
    EVENT_NIGHTMARES,
    EVENT_DARK_OF_NIGHT,
};

enum eTsulongSpells
{
    // Tsulong
    SPELL_DREAD_SHADOWS        = 122767,
    SPELL_DREAD_SHADOWS_DEBUFF = 122768,
    SPELL_SUNBEAM_DUMMY        = 122782,
    SPELL_SUNBEAM_PROTECTION   = 122789,
    SPELL_NIGHT_PHASE_EFFECT   = 122841,
    SPELL_SHADOW_BREATH        = 122752,
    SPELL_NIGHTMARES           = 122770,
    SPELL_SPAWN_DARK_OF_NIGHT  = 123739,

    // the dark of the night
    SPELL_BUMP_DARK_OF_NIGHT   = 130013,
    SPELL_VISUAL_DARK_OF_NIGHT = 123740,

};

enum eTsulongTimers
{
    TIMER_FIRST_WAYPOINT = 5000, // 5 secs for test, live : 120 000
    TIMER_SHADOW_BREATH  = 25000,
    TIMER_NIGHTMARES     = 11600,
    TIMER_DARK_OF_NIGHT  = 30000,
};

enum eTsulongPhase
{
    PHASE_NONE,
    PHASE_FLY,
    PHASE_DAY,
    PHASE_NIGHT
};

enum eTsulongWaypoints
{
    WAYPOINT_FIRST = 10001,
    WAYPOINT_SECOND = 10002
};

enum eTsulongDisplay
{
    DISPLAY_TSULON_NIGHT = 42532,
    DISPLAY_TSULON_DAY   = 42533
};

enum eTsulongActions
{
    ACTION_SPAWN_SUNBEAM = 3,
};

enum eTsulongCreatures
{
    SUNBEAM_DUMMY_ENTRY    = 62849,
};

class boss_tsulong : public CreatureScript
{
    public:
        boss_tsulong() : CreatureScript("boss_tsulong") { }

        struct boss_tsulongAI : public BossAI
        {
            boss_tsulongAI(Creature* creature) : BossAI(creature, DATA_TSULONG)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 phase;
            bool firstSpecialEnabled;
            bool secondSpecialEnabled;
            bool inFly;

            void Reset()
            {
                _Reset();
                events.Reset();
                summons.DespawnAll();

                inFly = false;

                me->SetDisableGravity(true);
                me->SetCanFly(true);
                me->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS);

                if (pInstance)
                {
                    if (pInstance->GetBossState(DATA_PROTECTORS) == DONE)
                    {
                        phase = PHASE_NIGHT;
                        me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                        me->setFaction(14);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                        me->SetHomePosition(-1017.841f, -3049.621f, 12.823f, 4.72f);
                        me->GetMotionMaster()->MoveTargetedHome();
                    }
                    else
                    {
                        phase = PHASE_DAY;
                        me->SetDisplayId(DISPLAY_TSULON_DAY);
                        me->setFaction(35);
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 35);
                    }
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_TSULONG, FAIL);
            }

            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    DoZoneInCombat();
                }

                phase = PHASE_NIGHT;
                events.SetPhase(PHASE_NIGHT);
                events.ScheduleEvent(EVENT_SWITCH_TO_NIGHT_PHASE, 0, 0, PHASE_NIGHT);
                events.ScheduleEvent(EVENT_SPAWN_SUNBEAM, 2000, 0, PHASE_NIGHT);
                events.ScheduleEvent(EVENT_SHADOW_BREATH, TIMER_SHADOW_BREATH, 0, PHASE_NIGHT);
                events.ScheduleEvent(EVENT_NIGHTMARES, TIMER_NIGHTMARES, 0, PHASE_NIGHT);
                events.ScheduleEvent(EVENT_DARK_OF_NIGHT, TIMER_DARK_OF_NIGHT, 0, PHASE_NIGHT);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void KilledUnit(Unit* who)
            {
            }

            void JustDied(Unit* killer)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_TSULONG, DONE);
                }

                _JustDied();
            }

            void DoAction(const int32 action)
            {
                if (action == ACTION_START_TSULONG_WAYPOINT)
                {
                    phase = PHASE_FLY;
                    events.SetPhase(phase);
                    events.ScheduleEvent(EVENT_FLY, 5000, 0, phase);
                }

                if (action == ACTION_SPAWN_SUNBEAM)
                    events.ScheduleEvent(EVENT_SPAWN_SUNBEAM, 0, 0, PHASE_NIGHT);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                switch (id)
                {
                    case WAYPOINT_FIRST:
                        events.ScheduleEvent(EVENT_WAYPOINT_FIRST, 0, 0, PHASE_FLY);
                        break;
                    case WAYPOINT_SECOND:
                        events.ScheduleEvent(EVENT_WAYPOINT_SECOND, 0, 0, PHASE_FLY);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING) || inFly)
                {
                    if (phase == PHASE_FLY)
                    {
                        switch (events.ExecuteEvent())
                        {
                            case EVENT_FLY:
                                me->setFaction(14);
                                me->SetReactState(REACT_PASSIVE);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, 0);
                                me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                                me->GetMotionMaster()->MovePoint(WAYPOINT_FIRST, -1018.10f, -2947.431f, 50.12f);
                                inFly = true;
                                break;
                            case EVENT_WAYPOINT_FIRST:
                                me->GetMotionMaster()->Clear();
                                me->GetMotionMaster()->MovePoint(WAYPOINT_SECOND, -1017.841f, -3049.621f, 12.823f);
                                break;
                            case EVENT_WAYPOINT_SECOND:
                                me->SetHomePosition(-1017.841f, -3049.621f, 12.823f, 4.72f);
                                me->SetReactState(REACT_AGGRESSIVE);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                                inFly = false;
                                events.SetPhase(PHASE_NONE);
                                phase = PHASE_NONE;
                                break;
                            default:
                                break;
                        }
                    }
                    return;
                }

                if (phase == PHASE_NIGHT)
                {
                    switch (events.ExecuteEvent())
                    {
                        case EVENT_SWITCH_TO_NIGHT_PHASE:
                            me->SetDisplayId(DISPLAY_TSULON_NIGHT);
                            me->setFaction(14);
                            me->CastSpell(me, SPELL_DREAD_SHADOWS, true);
                            break;
                        case EVENT_SPAWN_SUNBEAM:
                            Position pos;
                            me->GetRandomNearPosition(pos, 30.0f);
                            me->SummonCreature(SUNBEAM_DUMMY_ENTRY, pos);
                            break;
                        case EVENT_SHADOW_BREATH:
                            me->CastSpell(SelectTarget(SELECT_TARGET_TOPAGGRO), SPELL_SHADOW_BREATH, false);
                            events.ScheduleEvent(EVENT_SHADOW_BREATH, TIMER_SHADOW_BREATH, 0, PHASE_NIGHT);
                            break;
                        case EVENT_NIGHTMARES:
                            me->CastSpell(SelectTarget(SELECT_TARGET_RANDOM), SPELL_NIGHTMARES, false);
                            events.ScheduleEvent(EVENT_NIGHTMARES, TIMER_NIGHTMARES, 0, PHASE_NIGHT);
                            break;
                        case EVENT_DARK_OF_NIGHT:
                            me->CastSpell(me, SPELL_SPAWN_DARK_OF_NIGHT, false);
                            events.ScheduleEvent(EVENT_DARK_OF_NIGHT, TIMER_DARK_OF_NIGHT, 0, PHASE_NIGHT);
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
            return new boss_tsulongAI(creature);
        }
};

class npc_sunbeam : public CreatureScript
{
    public:
        npc_sunbeam() : CreatureScript("npc_sunbeam") { }

        struct npc_sunbeamAI : public CreatureAI
        {
            InstanceScript* pInstance;

            npc_sunbeamAI(Creature* creature) : CreatureAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                creature->SetObjectScale(5.0f);
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE|UNIT_FLAG_NOT_SELECTABLE|UNIT_FLAG_NON_ATTACKABLE);
                creature->CastSpell(creature, SPELL_SUNBEAM_DUMMY, true);
            }

            void Despawn()
            {
                if (pInstance)
                {
                    if (Creature* tsulong = pInstance->instance->GetCreature(pInstance->GetData64(NPC_TSULONG)))
                        tsulong->AI()->DoAction(ACTION_SPAWN_SUNBEAM);
                }

                me->DespawnOrUnsummon(1000);
            }

            void JustDied(Unit* killer)
            {
                Despawn();
            }

            void UpdateAI(uint32 const diff)
            {
                float scale = me->GetFloatValue(OBJECT_FIELD_SCALE_X);
                if (scale <= 1.0f)
                    Despawn();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sunbeamAI(creature);
        }
};

class npc_dark_of_night : public CreatureScript
{
    public:
        npc_dark_of_night() : CreatureScript("npc_dark_of_night") { }

        struct npc_dark_of_nightAI : public CreatureAI
        {
            InstanceScript* pInstance;
            uint64 sunbeamTargetGUID;
            uint32 visualCastTimer;
            bool explode;

            npc_dark_of_nightAI(Creature* creature) : CreatureAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                creature->SetReactState(REACT_PASSIVE);
                creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                creature->CastSpell(me, SPELL_VISUAL_DARK_OF_NIGHT, false);
                sunbeamTargetGUID = 0;
                visualCastTimer = 1000;
                explode = false;

            }

            void UpdateAI(uint32 const diff)
            {
                if (visualCastTimer <= diff)
                {
                    me->CastSpell(me, SPELL_VISUAL_DARK_OF_NIGHT, false);
                    visualCastTimer = 1000;
                }
                else
                    visualCastTimer -= diff;

                // Try to find a sunbeam
                if (!sunbeamTargetGUID)
                {
                    std::list<Creature*> sumbeams;
                    me->GetCreatureListWithEntryInGrid(sumbeams, SUNBEAM_DUMMY_ENTRY, 100.0f);
                    float minDist = 150.0f;
                    Creature* tmp = NULL;

                    for (auto itr : sumbeams)
                    {
                        float dist = itr->GetDistance2d(me);
                        if (dist < minDist)
                        {
                            tmp = itr;
                            sunbeamTargetGUID = itr->GetGUID();
                        }
                    }

                    if (tmp)
                        me->GetMotionMaster()->MovePoint(1, tmp->GetPositionX(), tmp->GetPositionY(), tmp->GetPositionZ());
                }
                // Check if we are close enought to kill the sunbeam !
                else if (!explode)
                {
                    Creature* tmp = me->GetMap()->GetCreature(sunbeamTargetGUID);
                    if (!tmp)
                        return;

                    float dist = tmp->GetDistance(me);
                    if (dist <= tmp->GetFloatValue(OBJECT_FIELD_SCALE_X))
                    {
                        me->CastSpell(me, SPELL_BUMP_DARK_OF_NIGHT, false);
                        me->Kill(tmp);
                        me->DespawnOrUnsummon(1000);
                        explode = true;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_dark_of_nightAI(creature);
        }
};

// 125843, jam spell ?
class spell_dread_shadows_damage : public SpellScriptLoader
{
    public:
        spell_dread_shadows_damage() : SpellScriptLoader("spell_dread_shadows_damage") { }

        class spell_dread_shadows_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dread_shadows_damage_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_damage_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_damage_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dread_shadows_damage_SpellScript();
        }
};

class DreadShadowsTargetCheck
{
    public:
        bool operator()(WorldObject* object) const
        {
            // check Sunbeam protection
            if (object->ToUnit() && object->ToUnit()->HasAura(122789))
                return true;

            return false;
        }
};

// 122768
class spell_dread_shadows_malus : public SpellScriptLoader
{
    public:
        spell_dread_shadows_malus() : SpellScriptLoader("spell_dread_shadows_malus") { }

        class spell_dread_shadows_malus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dread_shadows_malus_SpellScript);

            void RemoveInvalidTargets(std::list<WorldObject*>& targets)
            {
                targets.remove(GetCaster());
                targets.remove_if(DreadShadowsTargetCheck());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_malus_SpellScript::RemoveInvalidTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_dread_shadows_malus_SpellScript::RemoveInvalidTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dread_shadows_malus_SpellScript();
        }
};

// 122789
class spell_sunbeam : public SpellScriptLoader
{
    public:
        spell_sunbeam() : SpellScriptLoader("spell_sunbeam") { }

        class spell_sunbeam_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sunbeam_SpellScript);

            void CheckTargets(std::list<WorldObject*>& targets)
            {
                targets.clear();
                Map::PlayerList const& players = GetCaster()->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator itr = players.begin(); itr != players.end(); ++itr)
                {
                    Player* plr = itr->getSource();
                    if (!plr)
                        continue;

                    float scale = GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE_X);
                    if (plr->GetExactDist2d(GetCaster()) <= scale)
                        targets.push_back(plr);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sunbeam_SpellScript::CheckTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sunbeam_SpellScript();
        }


        class spell_sunbeam_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_sunbeam_aura_AuraScript);

            void OnApply(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                {
                    if (Pet* pet = GetTarget()->ToPlayer()->GetPet())
                        pet->AddAura(SPELL_SUNBEAM_PROTECTION, pet);

                    float scale = GetCaster()->GetFloatValue(OBJECT_FIELD_SCALE_X);
                    if (scale > 0.2f)
                        GetCaster()->SetObjectScale(scale - 0.2f);
                }

                GetTarget()->RemoveAurasDueToSpell(SPELL_DREAD_SHADOWS_DEBUFF);
            }

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (GetTarget()->GetTypeId() == TYPEID_PLAYER)
                {
                    if (Pet* pet = GetTarget()->ToPlayer()->GetPet())
                        pet->RemoveAurasDueToSpell(SPELL_SUNBEAM_PROTECTION);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_sunbeam_aura_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_sunbeam_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_sunbeam_aura_AuraScript();
        }
};

void AddSC_boss_tsulong()
{
    new boss_tsulong();
    new npc_sunbeam();
    new npc_dark_of_night();
    new spell_dread_shadows_damage();
    new spell_dread_shadows_malus();
    new spell_sunbeam();
}