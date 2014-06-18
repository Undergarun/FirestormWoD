/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
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

#include "ScriptedCreature.h"
#include "throne_of_thunder.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureAI.h"
#include "MoveSplineInit.h"

enum eSpells
{
    // Zandalari Water-Binder
    SPELL_BIND_WATER                    = 137099,
    SPELL_DELUGE                        = 136881,
    SPELL_FROSTBOLT                     = 136952,

    // Zandalari Blade Initiate
    SPELL_WOUNDING_STRIKE               = 140049,

    // Zandalari Spear-Shaper
    SPELL_BERSERKING                    = 137096,
    SPELL_RETRIEVE_SPEAR_JUMP           = 137070,
    SPELL_SPEAR_SPIN                    = 137077,
    SPELL_THROW_SPEAR                   = 136986,
    SPELL_THROW_SPEAR_DISARM            = 137066,
    SPELL_THROW_SPEAR_PERIODIC          = 137058,

    // Zandalari Storm-Caller
    SPELL_WATER_BOLT                    = 139231,
    SPELL_WATER_BOLT_DAMAGE             = 139251,
    SPELL_STORM_WEAPON                  = 139218,
    SPELL_STORM_WEAPON_DAMAGE           = 139220,
    SPELL_FOCUSED_LIGHTNING_SUMMON      = 139206,
    SPELL_FOCUSED_LIGHTNING_VISUAL      = 139233,
    SPELL_FOCUSED_LIGHTNING_AOE         = 139209,
    SPELL_FOCUSED_LIGHTNING_DAMAGE      = 139210,
    SPELL_FOCUSED_LIGHTNING_DETONATION  = 139211
};

enum eEvents
{
    // Zandalari Water-Binder
    EVENT_BIND_WATER            = 1,
    EVENT_DELUGE                = 2,
    EVENT_FROSTBOLT             = 3,

    // Zandalari Blade Initiate
    EVENT_WOUNDING_STRIKE       = 4,

    // Zandalari Spear-Shaper
    EVENT_BERSERKING            = 5,
    EVENT_RETRIEVE_SPEAR        = 6,
    EVENT_SPEAR_SPIN            = 7,
    EVENT_THROW_SPEAR           = 8,

    // Zandalari Storm-Caller
    EVENT_WATER_BOLT            = 9,
    EVENT_STORM_WEAPON          = 10,
    EVENT_FOCUSED_LIGHTNING     = 11,
    EVENT_FOCUSED_LIGHTNING_AOE = 12
};

enum eEquipIds
{
    // Zandalari Water-Binder
    WATER_BINDER_AXE    = 89698,

    // Zandalari Blade Initiate
    BLADE_INITIATE_AXE  = 13632,

    // Zandalari Spear-Shaper
    SPEAR_SHAPER_SPEAR  = 89685,

    // Zandalari Storm-Caller
    STORM_CALLER_EQ_1   = 94703,
    STORM_CALLER_EQ_2   = 89295
};

// Zandalari Water-Binder - 69455
class mob_zandalari_water_binder : public CreatureScript
{
    public:
        mob_zandalari_water_binder() : CreatureScript("mob_zandalari_water_binder") { }

        struct mob_zandalari_water_binderAI : public ScriptedAI
        {
            mob_zandalari_water_binderAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, WATER_BINDER_AXE, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_BIND_WATER, 5000);
                events.ScheduleEvent(EVENT_DELUGE, 10000);
                events.ScheduleEvent(EVENT_FROSTBOLT, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BIND_WATER:
                        me->CastSpell(me, SPELL_BIND_WATER, false);
                        events.ScheduleEvent(EVENT_BIND_WATER, 30000);
                        break;
                    case EVENT_DELUGE:
                        me->CastSpell(me, SPELL_DELUGE, false);
                        events.ScheduleEvent(EVENT_DELUGE, 25000);
                        break;
                    case EVENT_FROSTBOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_FROSTBOLT, false);
                        events.ScheduleEvent(EVENT_FROSTBOLT, 5000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_water_binderAI(creature);
        }
};

// Zandalari Blade Initiate - 70230
class mob_zandalari_blade_initiate : public CreatureScript
{
    public:
        mob_zandalari_blade_initiate() : CreatureScript("mob_zandalari_blade_initiate") { }

        struct mob_zandalari_blade_initiateAI : public ScriptedAI
        {
            mob_zandalari_blade_initiateAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, BLADE_INITIATE_AXE, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_WOUNDING_STRIKE, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WOUNDING_STRIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_WOUNDING_STRIKE, false);
                        events.ScheduleEvent(EVENT_WOUNDING_STRIKE, 8000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_blade_initiateAI(creature);
        }
};

// Zandalari Spear-Shaper - 69388
class mob_zandalari_spear_shaper : public CreatureScript
{
    public:
        mob_zandalari_spear_shaper() : CreatureScript("mob_zandalari_spear_shaper") { }

        struct mob_zandalari_spear_shaperAI : public ScriptedAI
        {
            mob_zandalari_spear_shaperAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                SetEquipmentSlots(false, SPEAR_SHAPER_SPEAR, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_BERSERKING, 5000);
                events.ScheduleEvent(EVENT_RETRIEVE_SPEAR, 10000);
                events.ScheduleEvent(EVENT_SPEAR_SPIN, 8000);
                events.ScheduleEvent(EVENT_THROW_SPEAR, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BERSERKING:
                        me->CastSpell(me, SPELL_BERSERKING, false);
                        events.ScheduleEvent(EVENT_BERSERKING, 20000);
                        break;
                    case EVENT_RETRIEVE_SPEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_RETRIEVE_SPEAR_JUMP, false);
                        events.ScheduleEvent(EVENT_RETRIEVE_SPEAR, 10000);
                        break;
                    case EVENT_SPEAR_SPIN:
                        me->CastSpell(me, SPELL_SPEAR_SPIN, false);
                        events.ScheduleEvent(EVENT_SPEAR_SPIN, 8000);
                        break;
                    case EVENT_THROW_SPEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_THROW_SPEAR, false);
                        me->CastSpell(me, SPELL_THROW_SPEAR_DISARM, true);
                        events.ScheduleEvent(EVENT_THROW_SPEAR, 7000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_spear_shaperAI(creature);
        }
};

// Thrown Spear - 69438
class mob_thrown_spear : public CreatureScript
{
    public:
        mob_thrown_spear() : CreatureScript("mob_thrown_spear") { }

        struct mob_thrown_spearAI : public ScriptedAI
        {
            mob_thrown_spearAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->CastSpell(me, SPELL_THROW_SPEAR_PERIODIC, true);
            }

            void UpdateAI(const uint32 diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_thrown_spearAI(creature);
        }
};

// Zandalari Storm-Caller - 69390
class mob_zandalari_storm_caller : public CreatureScript
{
    public:
        mob_zandalari_storm_caller() : CreatureScript("mob_zandalari_storm_caller") { }

        struct mob_zandalari_storm_callerAI : public ScriptedAI
        {
            mob_zandalari_storm_callerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint32 focusedLightningTarget;

            void Reset()
            {
                SetEquipmentSlots(false, STORM_CALLER_EQ_1, STORM_CALLER_EQ_2, EQUIP_NO_CHANGE);

                focusedLightningTarget = 0;

                events.Reset();

                std::list<Creature*> focusedLightnings;
                me->GetCreatureListWithEntryInGrid(focusedLightnings, NPC_TRASH_FOCUSED_LIGHTNING, 100.0f);

                for (auto itr : focusedLightnings)
                    itr->DespawnOrUnsummon();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_WATER_BOLT, 8000);
                events.ScheduleEvent(EVENT_STORM_WEAPON, 5000);
                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WATER_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_WATER_BOLT, false);
                        events.ScheduleEvent(EVENT_WATER_BOLT, 8000);
                        break;
                    case EVENT_STORM_WEAPON:
                        me->CastSpell(me, SPELL_STORM_WEAPON, false);
                        events.ScheduleEvent(EVENT_STORM_WEAPON, 15000);
                        break;
                    case EVENT_FOCUSED_LIGHTNING:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                        {
                            me->CastSpell(target, SPELL_FOCUSED_LIGHTNING_SUMMON, false);
                            focusedLightningTarget = target->GetGUIDLow();
                        }
                        events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            uint32 GetData(uint32 type)
            {
                return focusedLightningTarget;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_storm_callerAI(creature);
        }
};

// Focused Lightning - 70174
class mob_trash_focused_lightning : public CreatureScript
{
    public:
        mob_trash_focused_lightning() : CreatureScript("mob_trash_focused_lightning") { }

        struct mob_trash_focused_lightningAI : public ScriptedAI
        {
            mob_trash_focused_lightningAI(Creature* creature) : ScriptedAI(creature)
            {
                focusedGuid = 0;
                exploded = false;
            }

            uint64 focusedGuid;
            bool exploded;
            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_VISUAL, true);
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (!summoner->ToCreature())
                    return;

                Creature* stormCaller = summoner->ToCreature();
                focusedGuid = MAKE_NEW_GUID(stormCaller->AI()->GetData(0), 0, HIGHGUID_PLAYER);

                if (Player* player = Player::GetPlayer(*me, focusedGuid))
                {
                    me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                    if (player && me->Attack(player, true))
                        me->GetMotionMaster()->MoveChase(player, 0.5f);
                }

                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING_AOE, 1000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (exploded)
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FOCUSED_LIGHTNING_AOE:
                        me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_AOE, true);
                        events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING_AOE, 1000);
                        break;
                    default:
                        break;
                }

                if (Player* player = Player::GetPlayer(*me, focusedGuid))
                {
                    if (player->GetDistance(me) <= 1.0f)
                    {
                        me->CastSpell(player, SPELL_FOCUSED_LIGHTNING_DETONATION, true);
                        me->DespawnOrUnsummon(200);
                        exploded = true;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_trash_focused_lightningAI(creature);
        }
};

// Water Bolt - 139231
class spell_water_bolt : public SpellScriptLoader
{
    public:
        spell_water_bolt() : SpellScriptLoader("spell_water_bolt") { }

        class spell_water_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_water_bolt_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                JadeCore::RandomResizeList(targets, 1);
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_WATER_BOLT_DAMAGE, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_water_bolt_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnHit += SpellHitFn(spell_water_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_water_bolt_SpellScript();
        }
};

// Storm Weapon - 139218
class spell_storm_weapon : public SpellScriptLoader
{
    public:
        spell_storm_weapon() : SpellScriptLoader("spell_storm_weapon") { }

        class spell_storm_weapon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_storm_weapon_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* caster = GetTarget();

                if (!caster || !eventInfo.GetActor() || !caster->ToCreature())
                    return;

                if (caster != eventInfo.GetActor())
                    return;

                if (caster->ToCreature()->HasSpellCooldown(SPELL_STORM_WEAPON_DAMAGE))
                    return;

                if (Unit* target = caster->getVictim())
                {
                    caster->CastSpell(target, SPELL_STORM_WEAPON_DAMAGE, true);
                    caster->ToCreature()->_AddCreatureSpellCooldown(SPELL_STORM_WEAPON_DAMAGE, time(NULL) + 4);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_storm_weapon_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_storm_weapon_AuraScript();
        }
};

// Focused Lightning (AoE) - 139209
class spell_focused_lightning_aoe : public SpellScriptLoader
{
    public:
        spell_focused_lightning_aoe() : SpellScriptLoader("spell_focused_lightning_aoe") { }

        class spell_focused_lightning_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_focused_lightning_aoe_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_FOCUSED_LIGHTNING_DAMAGE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_focused_lightning_aoe_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_focused_lightning_aoe_SpellScript();
        }
};

void AddSC_throne_of_thunder()
{
    new mob_zandalari_water_binder();
    new mob_zandalari_blade_initiate();
    new mob_zandalari_spear_shaper();
    new mob_thrown_spear();
    new mob_zandalari_storm_caller();
    new mob_trash_focused_lightning();
    new spell_storm_weapon();
    new spell_water_bolt();
    new spell_focused_lightning_aoe();
}