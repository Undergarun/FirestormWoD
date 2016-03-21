/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    // Shared
    SPELL_BOND_OF_THE_GOLDEN_LOTUS  = 143497,

    // Adds
    SPELL_SHARED_TORMENT            = 145655, // This is for Heroic Mode
    SPELL_DEFILED_GROUND            = 143959,
    SPELL_INFERNO_STRIKE            = 143962,
    SPELL_RESIDUAL_BURN             = 144007, // This is for Heroic Mode
    SPELL_CORRUPTION_SHOCK          = 144018,
    SPELL_CORRUPTION_CHAIN          = 145631, // This is for Heroic Mode

    // Sun Thenderheart
    SPELL_CALAMITY_AOE              = 143491,
    SPELL_CALAMITY                  = 143544,
    SPELL_DARK_MEDITATION           = 143546,
    SPELL_MEDITATION_SPIKE          = 143602, // This is for Heroic Mode
    SPELL_SHA_SEAR                  = 143423,
    SPELL_SHADOW_WORD_BANE          = 143434,

    // He Softfoot
    //SPELL_FIXATE                    = 143292, // Fixates on a random player, attacking them until Taunted.
    SPELL_GARROTE                   = 143198,
    SPELL_GOUGE                     = 143301,
    SPELL_INSTANT_POISON            = 143210,
    SPELL_MARK_OF_ANGUISH           = 143812,
    SPELL_DEBILITATION              = 147383,
    SPELL_NOXIOUS_POISON            = 143225,
    SPELL_SHADOWSTEP                = 143267,

    // Rook Stonetoe
    SPELL_CLASH                     = 143027, // Like monk spell
    SPELL_CORRUPTED_BREW            = 143019,
    SPELL_CORRUPTION_KICK           = 143007,
    SPELL_MISERY_SORROW_AND_GLOOM   = 143955,
    SPELL_VENGEFUL_STRIKES          = 144396
};

enum eEvents
{
    // Sun Thunderheart
    EVENT_SUN_DARK_MEDITATION,
    EVENT_MEDITATION_SPIKE,
    EVENT_SHA_SEAR,
    EVENT_SHADOW_WORD_BANE,
    EVENT_CALAMITY,

    // Rook Stonetoe
    EVENT_VENGEFUL_STRIKES,
    EVENT_CORRUPTED_BREW,
    EVENT_CLASH,
    EVENT_ROOK_MEDITATION,

    // He Softfoot
    EVENT_HE_MEDITATION,
    EVENT_MARK_OF_ANGUISH,
    EVENT_MASTER_POISONER,
    EVENT_SHADOWSTEP_GARROTE,
    EVENT_GOUGE,

    // Adds
    EVENT_DEFILED_GROUND,
    EVENT_INFERNO_STRIKE,
    EVENT_CORRUPTION_SHOCK
};

enum eRookSays
{
    TALK_ROOK_AGGRO,
    TALK_ROOK_VENGEFUL_STRIKES,
    TALK_ROOK_DESPERATE_MEASURE,
    TALK_ROOK_SLAY,
    TALK_ROOK_BOND_OF_THE_GOLDEN_LOTUS,
    TALK_ROOK_DEATH
};

enum eHeSays
{
    TALK_HE_AGGRO,
    TALK_HE_DESPERATE_MEASURE,
    TALK_HE_BOND_OF_THE_GOLDEN_LOTUS
};

enum eSunSays
{
    TALK_SUN_AGGRO,
    TALK_SUN_DARK_MEDITATION,
    TALK_SUN_DESPERATE_MEASURE,
    TALK_SUN_SLAY,
    TALK_SUN_WIPE,
    TALK_SUN_BOND_OF_THE_GOLDEN_LOTUS,
    TALK_SUN_DEATH
};

enum ePhases
{
    PHASE_COMBAT,
    PHASE_MEDITATION
};

class boss_sun_tenderheart : public CreatureScript
{
    public:
        boss_sun_tenderheart() : CreatureScript("boss_sun_tenderheart") { }

        struct boss_sun_tenderheartAI : public BossAI
        {
            boss_sun_tenderheartAI(Creature* creature) : BossAI(creature, DATA_FALLEN_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;

            uint8 nextPhaseHealthPct;
            uint8 phase;
            
            void Reset()
            {
                Reset();
                
                events.Reset();

                events.ScheduleEvent(EVENT_SHA_SEAR,         5000);
                events.ScheduleEvent(EVENT_SHADOW_WORD_BANE, 10000);
                events.ScheduleEvent(EVENT_CALAMITY,         8000);

                phase = PHASE_COMBAT;
                nextPhaseHealthPct = 66.66f;
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, FAIL);

                Talk(TALK_SUN_WIPE);
            }
            
            void EnterCombat(Unit* attacker)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, IN_PROGRESS);
                }

                Talk(TALK_SUN_AGGRO);
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
            
            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const* p_SpellInfo)
            {
                if (phase == PHASE_COMBAT && me->HealthBelowPctDamaged(nextPhaseHealthPct, damage) && nextPhaseHealthPct > 33.33f)
                {
                    phase = PHASE_MEDITATION;

                    if (nextPhaseHealthPct == 66.66f)
                        nextPhaseHealthPct = 33.33f;

                    events.ScheduleEvent(EVENT_SUN_DARK_MEDITATION, 1000);
                }
                else if (nextPhaseHealthPct <= 33.33f && phase == PHASE_COMBAT && damage > me->GetHealth())
                {
                    me->CastSpell(me, SPELL_BOND_OF_THE_GOLDEN_LOTUS, false);

                    // Check if other protectors are at 1 health
                    bool isReadyToDie = false;

                    // If not, continue combat
                    if (!fallenProtectorsAreReadyToDie())
                    {
                        damage = 0;
                        me->SetHealth(1);
                        return;
                    }
                    // If, kill all protectors
                    else
                    {
                        if (Creature* rook = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_ROOK_STONETOE))))
                        {
                            if (Creature* he = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_HE_SOFTFOOT))))
                            {
                                attacker->Kill(rook, false, NULL);
                                attacker->Kill(he, false, NULL);
                            }
                        }
                    }
                }
                else if (phase == PHASE_MEDITATION)
                {
                    damage = 0;
                    return;
                }
            }
            
            void JustDied(Unit* killer)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, DONE);
                }

                Talk(TALK_SUN_DEATH);
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
                    case EVENT_SUN_DARK_MEDITATION:
                        me->CastSpell(me, SPELL_DARK_MEDITATION, false);
                        break;
                    case EVENT_MEDITATION_SPIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_MEDITATION_SPIKE, false);

                        events.ScheduleEvent(EVENT_MEDITATION_SPIKE, 8000);
                        break;
                    case EVENT_SHA_SEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SHA_SEAR, false);

                        events.ScheduleEvent(EVENT_SHA_SEAR, 5000);
                        break;
                    case EVENT_SHADOW_WORD_BANE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_SHADOW_WORD_BANE, false);

                        events.ScheduleEvent(EVENT_SHADOW_WORD_BANE, 10000);
                        break;
                    case EVENT_CALAMITY:
                        me->CastSpell(me, SPELL_CALAMITY_AOE, false);
                        events.ScheduleEvent(EVENT_CALAMITY, 8000);
                        break;
                    default:
                        break;
                }

                if (phase == PHASE_COMBAT)
                    DoMeleeAttackIfReady();
            }

            bool fallenProtectorsAreReadyToDie()
            {
                if (pInstance)
                    return false;

                if (Creature* rook = me->GetMap()->GetCreature(pInstance->GetData64(NPC_ROOK_STONETOE)))
                    if (Creature* he = me->GetMap()->GetCreature(pInstance->GetData64(NPC_HE_SOFTFOOT)))
                        if (rook->GetHealth() == 1 && he->GetHealth() == 1)
                            return true;

                return false;
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_sun_tenderheartAI(pCreature);
        }
};

class boss_he_softfoot : public CreatureScript
{
    public:
        boss_he_softfoot() : CreatureScript("boss_he_softfoot") { }

        struct boss_he_softfootAI : public BossAI
        {
            boss_he_softfootAI(Creature* creature) : BossAI(creature, DATA_FALLEN_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 nextPhaseHealthPct;
            uint8 phase;
            
            void Reset()
            {
                Reset();
                
                events.Reset();

                events.ScheduleEvent(EVENT_MASTER_POISONER,    10000);
                events.ScheduleEvent(EVENT_SHADOWSTEP_GARROTE, 8000);
                events.ScheduleEvent(EVENT_GOUGE,              9000);

                phase = PHASE_COMBAT;
                nextPhaseHealthPct = 66.66f;
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, FAIL);
            }
            
            void EnterCombat(Unit* attacker)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, IN_PROGRESS);
                }
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
            
            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const* p_SpellInfo)
            {
                if (phase == PHASE_COMBAT && me->HealthBelowPctDamaged(nextPhaseHealthPct, damage) && nextPhaseHealthPct > 33.33f)
                {
                    phase = PHASE_MEDITATION;

                    if (nextPhaseHealthPct == 66.66f)
                        nextPhaseHealthPct = 33.33f;
                }
                else if (nextPhaseHealthPct <= 33.33f && phase == PHASE_COMBAT && damage > me->GetHealth())
                {
                    me->CastSpell(me, SPELL_BOND_OF_THE_GOLDEN_LOTUS, false);

                    // Check if other protectors are at 1 health
                    bool isReadyToDie = false;

                    // If not, continue combat
                    if (!fallenProtectorsAreReadyToDie())
                    {
                        damage = 0;
                        me->SetHealth(1);
                        events.ScheduleEvent(EVENT_HE_MEDITATION, 1000);
                        return;
                    }
                    // If, kill all protectors
                    else
                    {
                        if (Creature* rook = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_ROOK_STONETOE))))
                        {
                            if (Creature* sun = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_SUN_THUNDERHEART))))
                            {
                                attacker->Kill(rook, false, NULL);
                                attacker->Kill(sun, false, NULL);
                            }
                        }
                    }
                }
                else if (phase == PHASE_MEDITATION)
                {
                    damage = 0;
                    return;
                }
            }
            
            void JustDied(Unit* killer)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, DONE);
                }
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
                    case EVENT_HE_MEDITATION:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(target, SPELL_MARK_OF_ANGUISH, false);
                            me->CastSpell(target, SPELL_DEBILITATION, true);
                        }

                        break;
                    case EVENT_MASTER_POISONER:
                        if (roll_chance_i(50))
                            me->CastSpell(me, SPELL_NOXIOUS_POISON, false);
                        else
                            me->CastSpell(me, SPELL_INSTANT_POISON, false);

                        events.ScheduleEvent(EVENT_MASTER_POISONER, 10000);
                        break;
                    case EVENT_SHADOWSTEP_GARROTE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(target, SPELL_SHADOWSTEP, false);
                            me->CastSpell(target, SPELL_GARROTE, false);
                        }

                        events.ScheduleEvent(EVENT_SHADOWSTEP_GARROTE, 8000);
                        break;
                    case EVENT_GOUGE:
                        if (Unit* target = me->getVictim())
                            me->CastSpell(target, SPELL_GOUGE, false);

                        events.ScheduleEvent(EVENT_GOUGE, 9000);
                        break;
                    default:
                        break;
                }

                if (phase == PHASE_COMBAT)
                    DoMeleeAttackIfReady();
            }

            bool fallenProtectorsAreReadyToDie()
            {
                if (pInstance)
                    return false;

                if (Creature* rook = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_ROOK_STONETOE))))
                    if (Creature* sun = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_SUN_THUNDERHEART))))
                        if (rook->GetHealth() == 1 && sun->GetHealth() == 1)
                            return true;

                return false;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_he_softfootAI(creature);
        }
};

class boss_rook_stonetoe : public CreatureScript
{
    public:
        boss_rook_stonetoe() : CreatureScript("boss_rook_stonetoe") { }

        struct boss_rook_stonetoeAI : public BossAI
        {
            boss_rook_stonetoeAI(Creature* creature) : BossAI(creature, DATA_FALLEN_PROTECTORS)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            uint8 nextPhaseHealthPct;
            uint8 phase;
            
            void Reset()
            {
                Reset();
                
                events.Reset();

                events.ScheduleEvent(EVENT_VENGEFUL_STRIKES, 10000);
                events.ScheduleEvent(EVENT_CORRUPTED_BREW,   7000);
                events.ScheduleEvent(EVENT_CLASH,            8000);

                phase = PHASE_COMBAT;
                nextPhaseHealthPct = 66.66f;
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, FAIL);
            }
            
            void EnterCombat(Unit* attacker)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, IN_PROGRESS);
                }
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
            
            void DamageTaken(Unit* attacker, uint32& damage, SpellInfo const* p_SpellInfo)
            {
                if (phase == PHASE_COMBAT && me->HealthBelowPctDamaged(nextPhaseHealthPct, damage) && nextPhaseHealthPct > 33.33f)
                {
                    phase = PHASE_MEDITATION;

                    if (nextPhaseHealthPct == 66.66f)
                        nextPhaseHealthPct = 33.33f;
                }
                else if (nextPhaseHealthPct <= 33.33f && phase == PHASE_COMBAT && damage > me->GetHealth())
                {
                    me->CastSpell(me, SPELL_BOND_OF_THE_GOLDEN_LOTUS, false);

                    // Check if other protectors are at 1 health
                    bool isReadyToDie = false;

                    // If not, continue combat
                    if (!fallenProtectorsAreReadyToDie())
                    {
                        damage = 0;
                        me->SetHealth(1);
                        events.ScheduleEvent(EVENT_ROOK_MEDITATION, 1000);
                        return;
                    }
                    // If, kill all protectors
                    else
                    {
                        if (Creature* he = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_HE_SOFTFOOT))))
                        {
                            if (Creature* sun = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_SUN_THUNDERHEART))))
                            {
                                attacker->Kill(he, false, NULL);
                                attacker->Kill(sun, false, NULL);
                            }
                        }
                    }
                }
                else if (phase == PHASE_MEDITATION)
                {
                    damage = 0;
                    return;
                }
            }
            
            void JustDied(Unit* killer)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_FALLEN_PROTECTORS, DONE);
                }
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
                    case EVENT_VENGEFUL_STRIKES:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_VENGEFUL_STRIKES, false);

                        events.ScheduleEvent(EVENT_VENGEFUL_STRIKES, 10000);
                        break;
                    case EVENT_CORRUPTED_BREW:
                        // Make this for distant targets
                        // In Heroic difficulty, every two casts, Corrupted Brew's travel time is reduced by half a second.
                        // This effect resets when Rook Stonetoe begins his Desperate Measures.
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_CORRUPTED_BREW, false);

                        events.ScheduleEvent(EVENT_CORRUPTED_BREW, 7000);
                        break;
                    case EVENT_CLASH:
                        // In Non-Heroic Difficulty, Rook Stonetoe's Clash will not be cast during any Desperate Measures phase.
                        // Launch Corruption Kick just after Clash
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_CLASH, false);

                        events.ScheduleEvent(EVENT_CLASH, 8000);
                        break;
                    case EVENT_ROOK_MEDITATION:
                        // Visual ?
                        me->CastSpell(me, SPELL_MISERY_SORROW_AND_GLOOM, false);
                        break;
                    default:
                        break;
                }

                if (phase == PHASE_COMBAT)
                    DoMeleeAttackIfReady();
            }

            bool fallenProtectorsAreReadyToDie()
            {
                if (pInstance)
                    return false;

                if (Creature* sun = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_SUN_THUNDERHEART))))
                    if (Creature* he = me->GetMap()->GetCreature(pInstance->GetData64(pInstance->GetData64(NPC_HE_SOFTFOOT))))
                        if (sun->GetHealth() == 1 && he->GetHealth() == 1)
                            return true;

                return false;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_rook_stonetoeAI(creature);
        }
};

class mob_embodied_misery : public CreatureScript
{
    public:
        mob_embodied_misery() : CreatureScript("mob_embodied_misery") { }

        struct mob_embodied_miseryAI : public ScriptedAI
        {
            mob_embodied_miseryAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                Reset();

                events.Reset();

                events.ScheduleEvent(EVENT_DEFILED_GROUND, 5000);
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
                    case EVENT_DEFILED_GROUND:
                        if (Unit* target = me->getVictim())
                            me->CastSpell(target, SPELL_DEFILED_GROUND,false);

                        events.ScheduleEvent(EVENT_DEFILED_GROUND, 5000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_embodied_miseryAI(creature);
        }
};

class mob_embodied_sorrow : public CreatureScript
{
    public:
        mob_embodied_sorrow() : CreatureScript("mob_embodied_sorrow") { }

        struct mob_embodied_sorrowAI : public ScriptedAI
        {
            mob_embodied_sorrowAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                Reset();

                events.Reset();

                events.ScheduleEvent(EVENT_INFERNO_STRIKE, 5000);
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
                    case EVENT_INFERNO_STRIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_INFERNO_STRIKE, false);

                        events.ScheduleEvent(EVENT_INFERNO_STRIKE, 5000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_embodied_sorrowAI(creature);
        }
};

class mob_embodied_gloom : public CreatureScript
{
    public:
        mob_embodied_gloom() : CreatureScript("mob_embodied_gloom") { }

        struct mob_embodied_gloomAI : public ScriptedAI
        {
            mob_embodied_gloomAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                Reset();

                events.Reset();

                events.ScheduleEvent(EVENT_CORRUPTION_SHOCK, 5000);
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
                    case EVENT_CORRUPTION_SHOCK:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_CORRUPTION_SHOCK, false);

                        events.ScheduleEvent(EVENT_CORRUPTION_SHOCK, 5000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_embodied_gloomAI(creature);
        }
};

class mob_embodied_despair : public CreatureScript
{
    public:
        mob_embodied_despair() : CreatureScript("mob_embodied_despair") { }

        struct mob_embodied_despairAI : public ScriptedAI
        {
            mob_embodied_despairAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                Reset();

                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_embodied_despairAI(creature);
        }
};

class mob_embodied_desperation : public CreatureScript
{
    public:
        mob_embodied_desperation() : CreatureScript("mob_embodied_desperation") { }

        struct mob_embodied_desperationAI : public ScriptedAI
        {
            mob_embodied_desperationAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                Reset();

                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_embodied_desperationAI(creature);
        }
};

// Calamity - 143491
class spell_calamity_aoe: public SpellScriptLoader
{
    public:
        spell_calamity_aoe() : SpellScriptLoader("spell_calamity_aoe") { }

        class spell_calamity_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_calamity_aoe_SpellScript);

            void HandleOnHit()
            {
                if (Unit* sun = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        sun->CastSpell(target, SPELL_CALAMITY, true);

                        if (target->HasAura(SPELL_SHADOW_WORD_BANE))
                            target->RemoveAura(SPELL_SHADOW_WORD_BANE);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_calamity_aoe_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_calamity_aoe_SpellScript();
        }
};

void AddSC_fallen_protectors()
{
    new boss_sun_tenderheart();
    new boss_he_softfoot();
    new boss_rook_stonetoe();
    new mob_embodied_misery();
    new mob_embodied_sorrow();
    new mob_embodied_gloom();
    new mob_embodied_despair();
    new mob_embodied_desperation();
    new spell_calamity_aoe();
}

/*
Intro:
Lorewalker Cho yells: Can it be...? Oh no, no no no no no.
Lorewalker Cho yells: The Golden Lotus... they pledged their lives to defend this place.
Rook Stonetoe yells: You... Rook knows you...
Lorewalker Cho yells: Yes! Rook Stonetoe! You remember me! What has happened to you?
Rook Stonetoe yells: Rook... does not know... Head... is cloudy...
Sun Tenderheart yells: We know only despair now. A fitting punishment for our failure...
Lorewalker Cho yells: I see. This sha energy has trapped their spirits here, to endlessly relive their failure. It feeds on their despair.
Rook Stonetoe yells: Please... help...
Lorewalker Cho yells: This is a fate far worse than death - please, heroes! Set their souls free!

Aggro:
Rook Stonetoe yells: Sun... He... aid me.
Sun Tenderheart yells: Will you succeed where we have failed?
He Softfoot nods to his foes.

Vengeful Strikes:
Rook Stonetoe yells: Brawl... with Rook.
Rook Stonetoe yells: Rook fight!

Dark Meditation:
Sun Tenderheart yells: We must redeem the Vale!
Sun Tenderheart yells: You will suffer from our failure.

Desperate Measures:
Rook Stonetoe yells: Rook... needs help!
Sun Tenderheart yells: Protectors, come to my aid!
He Softfoot signals for help.

Killed a Player:
Rook Stonetoe yells: You... are not my friend.
Sun Tenderheart yells: You too have failed.

Wipe:
Sun Tenderheart yells: We will never find solace.

Bond of the Golden Lotus:
Rook Stonetoe yells: Rook... not safe...
Sun Tenderheart yells: We have not fulfilled our oath!
He Softfoot enters a meditative pose to heal his wounds.

Death:
Rook Stonetoe yells: Feeling... better now... Thank you... little friend.
Sun Tenderheart yells: We return to when we lost our way. You... are very kind.

Outro:
Lorewalker Cho yells: Be at peace, dear friends.
Lorewalker Cho yells: May your souls become one with the land you gave your life to protect.
*/

/*
-- Rook Stonetoe
INSERT INTO creature_text VALUE
(71475, 0, 0, "Sun... He... aid me.", 14, 0, 100, 0, 0, soundId, 'ROOK_AGGRO'),
(71475, 1, 0, "Brawl... with Rook.", 14, 0, 100, 0, 0, soundId, 'ROOK_VENGEFUL_STRIKES_1'),
(71475, 1, 1, "Rook fight!", 14, 0, 100, 0, 0, soundId, 'ROOK_VENGEFUL_STRIKES_2'),
(71475, 2, 0, "Rook... needs help!", 14, 0, 100, 0, 0, soundId, 'ROOK_DESPERATE_MEASURE'),
(71475, 3, 0, "You... are not my friend.", 14, 0, 100, 0, 0, soundId, 'ROOK_SLAY'),
(71475, 4, 0, "Rook... not safe...", 14, 0, 100, 0, 0, soundId, 'ROOK_BOND_OF_THE_GOLDEN_LOTUS'),
(71475, 5, 0, "Feeling... better now... Thank you... little friend.", 14, 0, 100, 0, 0, soundId, 'ROOK_DEATH');

-- He Softfoot
INSERT INTO creature_text VALUE
(71479, 0, 0, "He Softfoot nods to his foes.", 16, 0, 100, 0, 0, soundId, 'HE_AGGRO'),
(71479, 1, 0, "He Softfoot signals for help.", 16, 0, 100, 0, 0, soundId, 'HE_DESPERATE_MEASURE'),
(71479, 2, 0, "He Softfoot enters a meditative pose to heal his wounds.", 16, 0, 100, 0, 0, soundId, 'HE_BOUND_OF_THE_GOLDEN_LOTUS');

--Sun Tenderheart
INSERT INTO creature_text VALUE
(71480, 0, 0, "Will you succeed where we have failed?", 14, 0, 100, 0, 0, soundId, 'SUN_AGGRO'),
(71480, 1, 0, "We must redeem the Vale!", 14, 0, 100, 0, 0, soundId, 'SUN_DARK_MEDITATION_1'),
(71480, 1, 1, "You will suffer from our failure.", 14, 0, 100, 0, 0, soundId, 'SUN_DARK_MEDITATION_2'),
(71480, 2, 0, "Protectors, come to my aid!", 14, 0, 100, 0, 0, soundId, 'SUN_DESPERATE_MEASURE'),
(71480, 3, 0, "You too have failed.", 14, 0, 100, 0, 0, soundId, 'SUN_SLAY'),
(71480, 4, 0, "We will never find solace.", 14, 0, 100, 0, 0, soundId, 'SUN_WIPE'),
(71480, 5, 0, "We have not fulfilled our oath!", 14, 0, 100, 0, 0, soundId, 'SUN_BOND_OF_THE_GOLDEN_LOTUS'),
(71480, 6, 0, "We return to when we lost our way. You... are very kind.", 14, 0, 100, 0, 0, soundId, 'SUN_DEATH');
*/