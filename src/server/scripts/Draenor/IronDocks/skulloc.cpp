/*
* Copyright (C) 2008-2015 TrinityCore <http://www.trinitycore.org/>
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

/* ScriptData
SDName: boss_skulloc
SD%Complete: 60
SDComment: Ground Slam need further development (knock back effect must be added to the core)
SDCategory: Gruul's Lair
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "iron_docks.h"
#include "Vehicle.h"

enum Yells
{
    KORAMAR_KILL01 = 94, // So.. weak. (43913) // Bow before the might of the iron horde! (43914)
    KORAMAR_DEATH = 95, // Inconcivable (43898)
};
enum Spells
{
    // Skulloc
    // Cannon Barrage
    SPELL_CANNON_BARRAGE_DUMMY_START_PERIODIC = 176131,
    SPELL_CANNON_BARRAGE_DUMMY_ON_PERIODIC = 168822,
    SPELL_CANNON_BARRAGE_EXPLOSION_ON_CONTACT = 168820,
    SPELL_CANNON_BARRAGE_EXPLOSION = 168540,
    SPELL_CANNON_BARRAGE_AURA = 168537,
    SPELL_CANNON_BARRAGE_MISSILE_ORIGINAL_ONE = 168539,

    SPELL_CANNON_BARRAGE_EXPLOSION_FULL = 168541,

    SPELL_GRONN_SMASH = 168227,
    SPELL_GRONN_SMASH_JUMP = 168167,

    SPELL_BACKDRAFT = 169132,

    // Koramar
    SPELL_BERSERKER_LEAP_LIGHTNING_VISUAL = 168399,
    SPELL_BERSERKER_LEAP_JUMP = 168964,
    SPELL_BERSERKER_LEAP_DUMMY = 168965,
    SPELL_BLADE_STORM = 168997,
    SPELL_SHATTERING_BLADES = 168939,

    // ZUGGOSH
    SPELL_RAPID_FIRE = 156628,
};
enum Events
{
    // Skulloc
    EVENT_BACKDRAFT = 955,
    EVENT_CANNON_BARRAGE = 51,
    EVENT_CANNON_BARRAGE_2 = 52,
    EVENT_CANNON_BARRAGE_PRE = 53,
    EVENT_CANNON_BARRAGE_CANCEL = 912,

    // Karomar
    EVENT_BERSERKER_LEAP = 53,
    EVENT_BLADE_STORM = 54,
    EVENT_SHATTERING_BLADES = 55,
    EVENT_BLADE_STORM_MOVE_JITTER_CANCEL = 57,
    EVENT_BLADE_STORM_MOVE_JITTER = 58,

    // Zuggosh
    EVENT_RAPID_FIRE = 56,
    EVENT_RAPID_FIRE_CHANGE_TARGET = 85,
};
enum Actions
{
    ACTION_LEAVE_TURRET = 70,
};
enum Creatures
{
    TRIGGER_BACKDRAFT = 432551,
};
enum Talks
{
    TALK_KORAMAR_12 = 50, // How dare you marching into my docks, and so bravery set foot onto my ship.. nothing will stop the Iron Horde least of all you.. enjoy your death weaklings. (46911)
    TALK_KORAMAR_13 = 51, // Zoggosh.. ready the ready the rocks! This.. Ends... Now! (46912)
    TALK_ZOGGOSH_03 = 11, // Yes sir.. (44049)
    TALK_KORAMAR_VENGENCE = 60,
};

#define cannonbarrageinterval 30000
#define backdraftinterval 30000
#define shatteringbladesinterval 40000
#define bladestormwinterval 14000
#define berserkerleapinterval 24000
#define rapidfireinterval 8000 
Position backdraftnpc = { 6859.93f, -989.91f, 23.054f, 3.000260f };

class pre_last_boss_event : public BasicEvent
{
public:
    explicit pre_last_boss_event(Unit* unit, int value) : obj(unit), modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*diff*/)
    {
        if (InstanceScript* instance = obj->GetInstanceScript())
        {
            if (Creature* Zoggosh = instance->instance->GetCreature(instance->GetData64(DATA_ZUGGOSH)))
            {
                if (Creature* Koramar = instance->instance->GetCreature(instance->GetData64(DATA_KORAMAR)))
                {
                    switch (modifier)
                    {
                    case 0:
                        Koramar->AI()->Talk(TALK_KORAMAR_12);
                        Koramar->m_Events.AddEvent(new pre_last_boss_event(Koramar, 1), Koramar->m_Events.CalculateTime(8000));
                        break;
                    case 1:
                        Koramar->AI()->Talk(TALK_KORAMAR_13);
                        Koramar->m_Events.AddEvent(new pre_last_boss_event(Koramar, 2), Koramar->m_Events.CalculateTime(8000));
                        break;
                    case 2:
                        Koramar->AI()->Talk(TALK_ZOGGOSH_03);
                        break;
                    }
                }
            }
        }
        return true;
    }
private:
    Creature* storm;
    Unit* obj;
    int modifier;
    int Event;
};

class boss_skulloc : public CreatureScript
{
public:
    boss_skulloc() : CreatureScript("boss_skulloc") { }

    struct boss_skullocAI : public BossAI
    {
        boss_skullocAI(Creature* creature) : BossAI(creature, DATA_SKULLOC), vehicle(creature->GetVehicleKit())
        {
            instance = me->GetInstanceScript();
            me->SetUnitMovementFlags(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->Kill(me);
            me->Respawn();

            boolpreevent = false;
        }
        InstanceScript* instance;
        Vehicle* vehicle;
        bool boolpreevent;
        void Reset() override
        {
            _Reset();
            DespawnCreaturesInArea(TRIGGER_BACKDRAFT, me);

            ASSERT(vehicle);

            me->SummonCreature(TRIGGER_BACKDRAFT, backdraftnpc, TEMPSUMMON_MANUAL_DESPAWN);
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 300.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void MoveInLineOfSight(Unit* who)
        {
            if (who && who->IsInWorld() && who->GetTypeId() == TYPEID_PLAYER && me->IsWithinDistInMap(who, 20.0f) && !boolpreevent)
            {
                boolpreevent = true;
                me->m_Events.AddEvent(new pre_last_boss_event(me, 0), me->m_Events.CalculateTime(8000));
            }
        }
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();

            if (Creature* turret = instance->instance->GetCreature(instance->GetData64(DATA_TURRET)))
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                    turret->AI()->EnterCombat(target);
            }

            events.ScheduleEvent(EVENT_CANNON_BARRAGE_PRE, cannonbarrageinterval + 10000);
            events.ScheduleEvent(EVENT_BACKDRAFT, 15000);
        }
        void JustReachedHome()
        {
            me->Kill(me);
            me->Respawn();
            if (Creature* zuggosh = instance->instance->GetCreature(instance->GetData64(DATA_ZUGGOSH)))
            {
                zuggosh->DespawnOrUnsummon(500);
            }
            if (Creature* turret = instance->instance->GetCreature(instance->GetData64(DATA_TURRET)))
            {
                turret->Kill(turret);
                turret->Respawn();
            }  
            if (Creature* koramar = instance->instance->GetCreature(instance->GetData64(DATA_KORAMAR)))
            {
                koramar->AI()->Reset();
                koramar->Respawn();
            }
        }
        void KilledUnit(Unit* who) override
        {
        }
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
        }
        void DoAction(int32 const action)
        {
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_CANNON_BARRAGE_PRE:
                    me->CastSpell(me, SPELL_GRONN_SMASH);
                    events.ScheduleEvent(EVENT_CANNON_BARRAGE, 6000);
                    events.ScheduleEvent(EVENT_CANNON_BARRAGE_CANCEL, 30000);
                    events.ScheduleEvent(EVENT_CANNON_BARRAGE_PRE, 50000);
                    break;
                case EVENT_CANNON_BARRAGE:        
                    me->AddAura(SPELL_CANNON_BARRAGE_AURA, me);
                    events.ScheduleEvent(EVENT_CANNON_BARRAGE, 12000);
                    break;
                case EVENT_CANNON_BARRAGE_CANCEL:
                    events.CancelEvent(EVENT_CANNON_BARRAGE);
                    break;
                case EVENT_BACKDRAFT:
                    if (Creature* backdrafttrigger = me->FindNearestCreature(TRIGGER_BACKDRAFT, 300.0f, true))
                    {
                        backdrafttrigger->SetUnitMovementFlags(MOVEMENTFLAG_ROOT);
                        backdrafttrigger->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        backdrafttrigger->SetLevel(101);
                        backdrafttrigger->setFaction(14);

                        backdrafttrigger->CastSpell(backdrafttrigger, SPELL_BACKDRAFT);
                        events.ScheduleEvent(EVENT_BACKDRAFT, backdraftinterval);
                    }
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_skullocAI(creature);
    }
};
class boss_turret : public CreatureScript
{
public:
    boss_turret() : CreatureScript("boss_turret") { }

    struct boss_zoggoshAI : public BossAI
    {
        boss_zoggoshAI(Creature* creature) : BossAI(creature, DATA_TURRET), vehicle(creature->GetVehicleKit())
        {
            instance = me->GetInstanceScript();
            me->SetUnitMovementFlags(MOVEMENTFLAG_ROOT);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            me->setFaction(16);

            me->Kill(me);
            me->Respawn();
        }
        Vehicle* vehicle;
        Unit* Rtarget = NULL;
        InstanceScript* instance;
        void Reset() override
        {
            _Reset();
            Rtarget = NULL;

            ASSERT(vehicle);
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 300.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_RAPID_FIRE_CHANGE_TARGET, 2000);
        }
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();
        }
        void SpellHitTarget(Unit* target, const SpellInfo* pSpell) override
        {
        }
        void DoAction(int32 const action)
        {
            switch (action)
            {
            case ACTION_LEAVE_TURRET:
                vehicle->RemoveAllPassengers();

                if (Creature* zuggosh = instance->instance->GetCreature(instance->GetData64(DATA_ZUGGOSH)))
                    zuggosh->AI()->Talk(TALK_KORAMAR_VENGENCE);

                me->DespawnOrUnsummon(500);
                me->getHostileRefManager().clearReferences();
                me->CombatStop();
                me->setFaction(35);
                events.CancelEvent(EVENT_RAPID_FIRE);
                break;
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_RAPID_FIRE_CHANGE_TARGET:
                    events.CancelEvent(EVENT_RAPID_FIRE);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 60.0f, true))
                    {
                        Rtarget = target;
                    }

                    events.ScheduleEvent(EVENT_RAPID_FIRE_CHANGE_TARGET, 10000);
                    events.ScheduleEvent(EVENT_RAPID_FIRE, 1500);
                    break;
                case EVENT_RAPID_FIRE:
                    if (Rtarget != NULL && Rtarget && Rtarget->IsInWorld() && Rtarget->IsWithinDistInMap(me, 200.0f, true))
                    {
                        me->CastSpell(Rtarget, SPELL_RAPID_FIRE);
                    }
                    events.ScheduleEvent(EVENT_RAPID_FIRE, 1500);
                    break;
                }
            }
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_zoggoshAI(creature);
    }
};
class boss_koramar : public CreatureScript
{
public:
    boss_koramar() : CreatureScript("boss_koramar") { }

    struct boss_koramarAI : public BossAI
    {
        boss_koramarAI(Creature* creature) : BossAI(creature, DATA_KORAMAR)
        {
            instance = me->GetInstanceScript();
        }
        InstanceScript* instance;
        void Reset() override
        {
            _Reset();
            me->setFaction(16);
        }
        void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
        {
            std::list<Creature*> creatures;
            GetCreatureListWithEntryInGrid(creatures, object, entry, 300.0f);
            if (creatures.empty())
                return;

            for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
                (*iter)->DespawnOrUnsummon();
        }
        void EnterCombat(Unit* /*who*/) override
        {
            _EnterCombat();

            events.ScheduleEvent(EVENT_BLADE_STORM, bladestormwinterval);
            events.ScheduleEvent(EVENT_SHATTERING_BLADES, shatteringbladesinterval);
            events.ScheduleEvent(EVENT_BERSERKER_LEAP, berserkerleapinterval);
        }
        void JustDied(Unit* /*killer*/) override
        {
            _JustDied();

            if (Creature* turret = instance->instance->GetCreature(instance->GetData64(DATA_TURRET)))
            {
                turret->GetAI()->DoAction(ACTION_LEAVE_TURRET);
            }
        }
        void UpdateAI(uint32 const diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                case EVENT_BLADE_STORM:
                    me->CastSpell(me, SPELL_BLADE_STORM);
                    events.ScheduleEvent(EVENT_BLADE_STORM, bladestormwinterval);
                    events.ScheduleEvent(EVENT_BLADE_STORM_MOVE_JITTER, 1000);
                    events.ScheduleEvent(EVENT_BLADE_STORM_MOVE_JITTER_CANCEL, 6000);
                    break;
                case EVENT_BLADE_STORM_MOVE_JITTER:
                    me->GetMotionMaster()->MoveRandom(10.0f);
                    events.ScheduleEvent(EVENT_BLADE_STORM_MOVE_JITTER, 1000);
                    break;
                case EVENT_BLADE_STORM_MOVE_JITTER_CANCEL:
                    events.CancelEvent(EVENT_BLADE_STORM_MOVE_JITTER);
                    break;
                case EVENT_SHATTERING_BLADES:
                    me->CastSpell(me->getVictim(), SPELL_SHATTERING_BLADES);
                    events.ScheduleEvent(EVENT_SHATTERING_BLADES, shatteringbladesinterval);
                    break;
                case EVENT_BERSERKER_LEAP:
                    me->CastSpell(me, SPELL_BERSERKER_LEAP_DUMMY);
                    events.ScheduleEvent(EVENT_BERSERKER_LEAP, berserkerleapinterval);
                    break;
                }
            }
            DoMeleeAttackIfReady();
        }
    private:

    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_koramarAI(creature);
    }
};
class koramar_berserker_jump : public SpellScriptLoader
{
public:
    koramar_berserker_jump() : SpellScriptLoader("koramar_berserker_jump") { }

    class iron_docks_auras : public AuraScript
    {
        PrepareAuraScript(iron_docks_auras);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].Amplitude = 500;
            //spell->Effects[0].TargetA = TARGET_DEST_DEST;
            return true;
        }
        void HandlePeriodic(constAuraEffectPtr /*aurEff*/)
        {
            PreventDefaultAction();
            
                if (GetCaster())
                {
                    if (Player* player = GetCaster()->FindNearestPlayer(50.0f, true))
                    {
                        GetCaster()->CastSpell(player, SPELL_BERSERKER_LEAP_JUMP);
                        //GetCaster()->GetMotionMaster()->MoveJump(player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 20.0f, 10.0f, 10.0f);
                        GetCaster()->CastSpell(player, SPELL_BERSERKER_LEAP_LIGHTNING_VISUAL);
                    }
               }
            
        }
        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_auras::HandlePeriodic, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new iron_docks_auras();
    }
};
class spell_gronn_smash : public SpellScriptLoader
{
public:
    spell_gronn_smash() : SpellScriptLoader("spell_gronn_smash") { }

    class spell_gronn_smash_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_gronn_smash_SpellScript);

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();

            if (!caster)
                return;

            std::list<Player*> pl_list;

            caster->GetPlayerListInGrid(pl_list, 80.0f);

            if (pl_list.empty())
                return;

            for (auto itr : pl_list)
            {
                itr->GetMotionMaster()->MoveJump(backdraftnpc.GetPositionX(), backdraftnpc.GetPositionY(), backdraftnpc.GetPositionZ(), 40.0f, 20.0f, 10.0f);
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(spell_gronn_smash_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_gronn_smash_SpellScript();
    }
};
class spell_barrage_targets : public SpellScriptLoader
{
public:
    spell_barrage_targets() : SpellScriptLoader("spell_barrage_targets") { }

    class iron_docks_spells : public AuraScript
    {
        PrepareAuraScript(iron_docks_spells);

        bool Load()
        {
            list_targets.clear();

            if (GetCaster()->GetEntry() == 83612)
            {
                GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_IN_BOSS_FIGHT, 200.0f);
            }
            else
            {
                switch (urand(0, 2))
                {
                case 0:
                    GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_TARGET, 1500.0F);
                    break;
                case 1:
                    GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_TARGET_2, 1500.0F);
                    break;
                case 2:
                    GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_TARGET_3, 1500.0F);
                    break;
                }
            }
            return true;
        }
        void PeriodicTick(constAuraEffectPtr /*aurEff*/)
        {
            PreventDefaultAction();
            if (Unit* caster = GetCaster())
            {
                for (auto itr : list_targets)
                {
                    caster->CastSpell(itr, SPELL_CANNON_BARRAGE_MISSILE_ORIGINAL_ONE);
                }
            }
        }

    private:
    std::list<Creature*> list_targets;

        void Register()
        {
            OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_spells::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
        }
    };

    AuraScript* GetAuraScript() const
    {
        return new iron_docks_spells();
    }
};
class spell_rapid_fire_damage : public SpellScriptLoader
{
public:
    spell_rapid_fire_damage() : SpellScriptLoader("spell_rapid_fire_damage") { }

    class iron_docks_spells : public SpellScript
    {
        PrepareSpellScript(iron_docks_spells);

        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetCaster()->GetEntry() != BOSS_TURRET)
                return;

                SetHitDamage(urand(2500, 3000));
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_spells();
    }
};
/*
class spell_barrage_targets : public SpellScriptLoader
{
public:
    spell_barrage_targets() : SpellScriptLoader("spell_barrage_targets") { }

    class spell_barrage_targets_spell_script : public SpellScript
    {
        PrepareSpellScript(spell_barrage_targets_spell_script);

        
         bool Load()
        {            
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            //spell->Effects[0].TargetA = TARGET_DEST_DEST;
            spell->Effects[0].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
//spell->Effects[0].RadiusEntry = sSpellRadiusStore.LookupEntry(13);

           /// spell->Effects[1].TargetA = TARGET_DEST_DEST;
            spell->Effects[1].TargetB = TARGET_UNIT_DEST_AREA_ENEMY;
           // spell->Effects[1].RadiusEntry = sSpellRadiusStore.LookupEntry(13);
            
            return true;
        }
    
        void CorrectTargets(std::list<WorldObject*>& targets)
        {
             targets.clear();

            std::list<Creature*> list_targets;
            list_targets.clear();

            if (GetCaster()->GetEntry() == ACCESSORIES_SKULLOC_TURRET)
            {
                GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_IN_BOSS_FIGHT, 200.0f);

                for (auto itr : list_targets)
                {
                    targets.push_back(itr->ToUnit());
                }
            }
            else
            {
                switch (urand(0, 2))
                {
                case 0:
                    GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_TARGET, 1500.0F);
                    break;
                case 1:
                    GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_TARGET_2, 1500.0F);
                    break;
                case 2:
                    GetCaster()->GetCreatureListWithEntryInGrid(list_targets, TRIGGER_CANNON_BARRAGE_TARGET_3, 1500.0F);
                    break;
                }
                for (auto itr : list_targets)
                {
                    targets.push_back(itr->ToUnit());
                }
            }
        }
    /*
    void UponHit()
    {
    if (!GetCaster() || !GetHitUnit())
    return;

    GetHitUnit()->CastSpell(GetHitUnit(), SPELL_CANNON_BARRAGE_EXPLOSION);
    }
    
    void Register()
    {
        //OnEffectHitTarget += SpellEffectFn(spell_barrage_targets_spell_script::HandleTriggerMissile, EFFECT_0, TARGET_DEST_DEST);
        //OnHit += SpellHitFn(spell_barrage_targets_spell_script::UponHit);
        OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_barrage_targets_spell_script::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
    }
};
SpellScript* GetSpellScript() const
{
    return new spell_barrage_targets_spell_script();
}
};
*/
class spell_back_draft : public SpellScriptLoader
{
public:
    spell_back_draft() : SpellScriptLoader("spell_back_draft") { }

    class iron_docks_spells : public SpellScript
    {
        PrepareSpellScript(iron_docks_spells);

        bool Load()
        {
            SpellInfo* spell = const_cast<SpellInfo*>(GetSpellInfo());
            spell->Effects[0].TargetB = 87;

            return true;
        }
        void HandleDamage(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetHitUnit()->IsWithinLOSInMap(GetCaster()))
                SetHitDamage(0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_1, SPELL_EFFECT_SCHOOL_DAMAGE);
            OnEffectHitTarget += SpellEffectFn(iron_docks_spells::HandleDamage, EFFECT_2, SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new iron_docks_spells();
    }
};
void AddSC_boss_skulloc()
{
    new boss_skulloc();
    new boss_koramar();
    new boss_turret();
    new spell_back_draft();
    new spell_gronn_smash();
    new spell_barrage_targets();
    new koramar_berserker_jump();
    new spell_rapid_fire_damage();
}
