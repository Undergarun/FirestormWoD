/*
 * Copyright (C) 2012-2014 JadeCore <http:///www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http:///www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http:///getmangos.com/>
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
 * with this program. If not, see <http:///www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"
#include "Vehicle.h"
#include "ThreatManager.h"
#include "GameObjectAI.h"

enum eSpells
{
    SPELL_PRIMORDIAL_STRIKE             = 136037,
    SPELL_MALFORMED_BLOOD               = 136050,
    SPELL_MUTATED_ABOMINATION           = 140544,
    SPELL_EVOLUTION                     = 139144,

    /// Auras added when Primordius Evolution gains 1 stack
    SPELL_VENTRAL_SACS_AURA             = 136210,
    SPELL_VENTRAL_SACS_DMG              = 136211,
    SPELL_CAUSTIC_GAZ                   = 136215,
    SPELL_CAUSTIC_GAZ_DMG               = 136216,
    SPELL_ACIDIC_SPINES_AURA            = 136218, /// Triggers Acidic Explosion,
    SPELL_PATHOGEN_GLANDS               = 136225,
    SPELL_VOLATILE_PATHOGEN             = 136228,
    SPELL_METABOLIC_BOOST               = 136245,
    SPELL_ERUPTING_PUSTULES             = 136246,
    /// Auras generated when adds die
    SPELL_MUTAGENIC_POOL                = 136049,
    SPELL_VOLATILE_POOL                 = 140506,
    /// Benefic/Nephasts auras added when adds die
    SPELL_THICK_BONES                   = 136184,
    SPELL_FRAGULES_BONES                = 136185,
    SPELL_CLEAR_MIND                    = 136186,
    SPELL_CLOUDED_MIND                  = 136187,
    SPELL_IMPROVED_SYNAPSES             = 136182,
    SPELL_DULLED_SYNAPSES               = 136183,
    SPELL_KEEN_EYESIGHT                 = 136180,
    SPELL_IMPAIRED_EYESIGHT             = 136181,
    SPELL_VOLATILE_MUTATE_PRIMORDIUS    = 140509,
    /// Saurok transformation
    SPELL_FULLY_MUTATED                 = 140546,
    SPELL_SECOND_FULLY_MUTATED          = 140706,
    /// Heroic Mode
    SPELL_DEADLY_MUTAGEN                = 136995,
    SPELL_BLACK_BLOOD                   = 137000
};

enum eEvents
{
    EVENT_PRIMORDIAL_STRIKE         = 1,
    EVENT_MALFORMED_BLOOD           = 2,
    EVENT_SUMMON_LIVING_FLUID       = 3,
    EVENT_MUTAGENIC_CHECK           = 4,
    EVENT_VOLATIL_CHECK             = 5,
    EVENT_VISCOUS_HORROR            = 6,
    EVENT_BLACK_BLOOD               = 7
};

enum eTalks
{
    TALK_INTRO_01,
    TALK_INTRO_02,
    TALK_INTRO_03,
    TALK_INTRO_04,
    TALK_AGGRO,
    TALK_EVOLUTION,
    TALK_SLAY,
    TALK_DEATH
};

#define INVIBLE_DISPLAY 11686

uint32 evolutionAuras[7] =
{
    SPELL_VENTRAL_SACS_AURA,
    SPELL_CAUSTIC_GAZ,
    SPELL_ACIDIC_SPINES_AURA,
    SPELL_PATHOGEN_GLANDS,
    SPELL_VOLATILE_PATHOGEN,
    SPELL_METABOLIC_BOOST,
    SPELL_ERUPTING_PUSTULES
};

uint32 beneficAuras[4] =
{
    SPELL_THICK_BONES,
    SPELL_CLEAR_MIND,
    SPELL_IMPROVED_SYNAPSES,
    SPELL_KEEN_EYESIGHT
};

uint32 nephastAuras[4] =
{
    SPELL_FRAGULES_BONES,
    SPELL_CLOUDED_MIND,
    SPELL_DULLED_SYNAPSES,
    SPELL_IMPAIRED_EYESIGHT
};

uint32 gobEntries[10] =
{
    GOB_FIRST_MOGU_BLOOD_VAT,
    GOB_SECOND_MOGU_BLOOD_VAT,
    GOB_THIRD_MOGU_BLOOD_VAT,
    GOB_FOURTH_MOGU_BLOOD_VAT,
    GOB_FIFTH_MOGU_BLOOD_VAT,
    GOB_SIXTH_MOGU_BLOOD_VAT,
    GOB_SEVENTH_MOGU_BLOOD_VAT,
    GOB_EIGHTH_MOGU_BLOOD_VAT,
    GOB_NINTH_MOGU_BLOOD_VAT,
    GOB_TENTH_MOGU_BLOOD_VAT
};

Position gSpawnPositions[10] =
{
    { 5623.743f, 4599.775f, 55.366f, 2.011344f }, /// GOB_FIRST_MOGU_BLOOD_VAT
    { 5646.376f, 4624.545f, 55.365f, 2.612180f }, /// GOB_SECOND_MOGU_BLOOD_VAT
    { 5655.994f, 4656.011f, 55.366f, 3.118759f }, /// GOB_THIRD_MOGU_BLOOD_VAT
    { 5646.648f, 4687.429f, 55.366f, 3.703888f }, /// GOB_FOURTH_MOGU_BLOOD_VAT
    { 5624.318f, 4712.191f, 55.367f, 4.105229f }, /// GOB_FIFTH_MOGU_BLOOD_VAT
    { 5561.621f, 4710.213f, 55.365f, 5.206335f }, /// GOB_SIXTH_MOGU_BLOOD_VAT
    { 5537.327f, 4688.097f, 55.366f, 5.811091f }, /// GOB_SEVENTH_MOGU_BLOOD_VAT
    { 5529.932f, 4656.286f, 55.369f, 6.223429f }, /// GOB_EIGHTH_MOGU_BLOOD_VAT
    { 5536.340f, 4624.192f, 55.367f, 0.484527f }, /// GOB_NINTH_MOGU_BLOOD_VAT
    { 5560.005f, 4600.493f, 55.366f, 1.068864f }  /// GOB_TENTH_MOGU_BLOOD_VAT
};

/// Primordius - 69017
class boss_primordius : public CreatureScript
{
    public:
        boss_primordius() : CreatureScript("boss_primordius") { }

        struct boss_primordiusAI : public BossAI
        {
            boss_primordiusAI(Creature* p_Creature) : BossAI(p_Creature, DATA_PRIMORDIUS)
            {
                m_Instance = p_Creature->GetInstanceScript();

                m_IntroDone = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            bool m_HasEvolued;

            bool m_IntroDone;
            uint8 m_IntroStep;
            uint32 m_IntroTimer;

            void Reset()
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(FAIL))
                        m_Instance->SetBossState(DATA_PRIMORDIUS, NOT_STARTED);
                }

                _Reset();

                m_HasEvolued = false;
                m_IntroTimer = 0;

                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();

                for (uint8 l_Index = 0; l_Index < 10; l_Index++)
                {
                    if (GameObject* l_Vat = m_Instance->instance->GetGameObject(m_Instance->GetData64(gobEntries[l_Index])))
                        l_Vat->SetGoState(GO_STATE_READY);
                }

                std::list<Creature*> l_LivingFluidList;

                GetCreatureListWithEntryInGrid(l_LivingFluidList, me, NPC_LIVING_FLUID, 200.0f);

                for (Creature* l_LivingFluid: l_LivingFluidList)
                    l_LivingFluid->DespawnOrUnsummon();
            }

            void EnterEvadeMode()
            {
                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_PRIMORDIUS, FAIL);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                m_Events.CancelEvent(EVENT_SUMMON_LIVING_FLUID);
                m_Events.CancelEvent(EVENT_PRIMORDIAL_STRIKE);
                m_Events.CancelEvent(EVENT_MALFORMED_BLOOD);

                me->SetFullHealth();
                me->RemoveAllAuras();
                me->GetMotionMaster()->MoveTargetedHome();

                std::list<Creature*> l_LivingFluidList;
                std::list<Creature*> l_ViscousHorrorList;

                GetCreatureListWithEntryInGrid(l_LivingFluidList, me, NPC_LIVING_FLUID, 200.0f);
                GetCreatureListWithEntryInGrid(l_ViscousHorrorList, me, NPC_VISCOUS_HORROR, 200.0f);

                for (Creature* l_LivingFluid: l_LivingFluidList)
                    l_LivingFluid->DespawnOrUnsummon();

                for (Creature* l_ViscousHorror : l_ViscousHorrorList)
                    l_ViscousHorror->DespawnOrUnsummon();

                _EnterEvadeMode();
            }

            void JustSummoned(Creature* p_Summon)
            {
                summons.Summon(p_Summon);
            }

            void SummonedCreatureDespawn(Creature* p_Summon)
            {
                summons.Despawn(p_Summon);
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (m_IntroDone)
                    return;

                if (p_Who->IsPlayer() && p_Who->GetDistance(me) <= 60.f)
                {
                    Talk(TALK_INTRO_01);
                    m_IntroDone = true;
                    m_IntroTimer = 7500;
                    m_IntroStep = TALK_INTRO_01;
                }
            }

            void EnterCombat(Unit* /*p_Who*/)
            {
                Talk(TALK_AGGRO);

                m_Events.Reset();

                m_Events.ScheduleEvent(EVENT_PRIMORDIAL_STRIKE, 6000);
                m_Events.ScheduleEvent(EVENT_MALFORMED_BLOOD, 7000);
                m_Events.ScheduleEvent(EVENT_SUMMON_LIVING_FLUID, 5000);

                if (IsHeroic())
                    m_Events.ScheduleEvent(EVENT_VISCOUS_HORROR, 30000);

                me->setPowerType(POWER_ENERGY);
                me->SetPower(POWER_ENERGY, 0, true);
                me->SetMaxPower(POWER_ENERGY, 60);
                me->AddAura(SPELL_MUTATED_ABOMINATION, me);
            }

            void KilledUnit(Unit* p_Victim)
            {
                if (p_Victim->IsPlayer())
                    Talk(TALK_SLAY);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                summons.DespawnAll();

                me->RemoveAura(SPELL_MUTATED_ABOMINATION);

                Talk(TALK_DEATH);

                if (m_Instance)
                {
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    m_Instance->SetBossState(DATA_PRIMORDIUS, DONE);
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootDistribution(me, l_Player->GetGroup());
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) >= 56)
                    m_HasEvolued = false;

                /// Generates 4 energy points in 2 seconds
                p_Value = 4;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (m_IntroTimer && m_IntroStep < TALK_INTRO_04)
                {
                    if (m_IntroTimer <= p_Diff)
                    {
                        Talk(++m_IntroStep);
                        m_IntroTimer = m_IntroStep == TALK_INTRO_03 ? 13000 : 8500;
                    }
                    else
                        m_IntroTimer -= p_Diff;
                }

                if (m_Instance)
                {
                    if (m_Instance->IsWipe())
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                if (!UpdateVictim())
                {
                    if (me->isInCombat())
                        me->CombatStop();

                    EnterEvadeMode();

                    return;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (!m_HasEvolued)
                {
                    if (me->GetPowerPct(POWER_ENERGY) >= 100.f)
                    {
                        me->AddAura(SPELL_EVOLUTION, me);
                        me->SetPower(POWER_ENERGY, 0, true);

                        m_HasEvolued = true;

                        Talk(TALK_EVOLUTION);
                    }
                }

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_PRIMORDIAL_STRIKE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_PRIMORDIAL_STRIKE, false);

                        m_Events.ScheduleEvent(EVENT_PRIMORDIAL_STRIKE, 20000);
                        break;
                    case EVENT_MALFORMED_BLOOD:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            l_Target->AddAura(SPELL_MALFORMED_BLOOD, l_Target);

                        m_Events.ScheduleEvent(EVENT_MALFORMED_BLOOD, 20000);
                        break;
                    case EVENT_SUMMON_LIVING_FLUID:
                        for (uint8 l_Index = 0; l_Index < 10; l_Index++)
                            me->SummonCreature(NPC_LIVING_FLUID, gSpawnPositions[l_Index]);

                        m_Events.ScheduleEvent(EVENT_SUMMON_LIVING_FLUID, 12000);
                        break;
                    case EVENT_VISCOUS_HORROR:
                        me->SummonCreature(NPC_VISCOUS_HORROR, gSpawnPositions[urand(0, 9)]);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_primordiusAI(p_Creature);
        }
};

/// Living Fluid - 69069
class mob_living_fluid : public CreatureScript
{
    public:
        mob_living_fluid() : CreatureScript("mob_living_fluid") { }

        struct mob_living_fluidAI : public ScriptedAI
        {
            mob_living_fluidAI(Creature * p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            bool m_StartMutagenicMethod;
            bool m_StartVolatileMethod;

            InstanceScript* m_Instance;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);

                me->SetSpeed(MOVE_WALK, 0.3f);
                me->SetSpeed(MOVE_RUN, 0.3f);

                m_StartVolatileMethod  = false;
                m_StartMutagenicMethod = false;
            }

            /// Start moving slowly to Primordius position when summoned
            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                if (m_Instance)
                {
                    if (Creature* l_Primordius = GetClosestCreatureWithEntry(me, NPC_PRIMORDIUS, 200.0f, true))
                        me->GetMotionMaster()->MoveFollow(l_Primordius, 0.1f, 0.1f);
                }
            }

            void VolatilePool()
            {
                if (m_StartVolatileMethod)
                {
                    me->SetSpeed(MOVE_WALK, 0.1f);
                    me->SetSpeed(MOVE_RUN, 0.1f);

                    /// If Living Fluid finds a really near player, it applies a random nephast aura to him.
                    if (Player* l_Player = me->FindNearestPlayer(3.0f))
                    {
                        uint8 l_CountAura = 0;

                        for (uint8 l_Index = 0; l_Index < 4; ++l_Index)
                            if (l_Player->HasAura(nephastAuras[l_Index]))
                                ++l_CountAura;

                        if (l_CountAura < 4)
                        {
                            uint32 l_Index = urand(0, 3);

                            while (l_Player->HasAura(nephastAuras[l_Index]))
                                l_Index = urand(0, 3);

                            m_StartVolatileMethod = false;

                            /// Stops checking players.
                            events.CancelEvent(EVENT_VOLATIL_CHECK);

                            me->AddAura(nephastAuras[l_Index], l_Player);
                            me->RemoveAura(SPELL_VOLATILE_POOL);
                            me->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void MutagenicPool()
            {
                if (m_StartMutagenicMethod)
                {
                    /// If players is already mutated, a debuff is applied instead of a buff.
                    if (Player* l_Player = me->FindNearestPlayer(3.0f))
                    {
                        if (l_Player->HasAura(SPELL_FULLY_MUTATED))
                        {
                            m_StartVolatileMethod = true;
                            m_StartMutagenicMethod = false;

                            /// Stops checking players.
                            events.CancelEvent(EVENT_MUTAGENIC_CHECK);

                            VolatilePool();

                            return;
                        }
                    }

                    std::list<AreaTrigger*> l_AreatriggerList;
                    me->GetAreaTriggerList(l_AreatriggerList, SPELL_MUTAGENIC_POOL);

                    if (!l_AreatriggerList.empty())
                    {
                        for (AreaTrigger* l_AreaTrigger : l_AreatriggerList)
                        {
                            std::list<Player*> l_PlayerList;

                            /// Checks every player in a radius <= 3.0f from the AT.
                            l_AreaTrigger->GetPlayerListInGrid(l_PlayerList, 3.0f);

                            if (!l_PlayerList.empty())
                            {
                                for (Player* l_Player : l_PlayerList)
                                {
                                    l_Player->CastSpell(l_Player, beneficAuras[urand(0, 3)], false);

                                    uint8 l_StackCount = 0;

                                    for (uint8 l_Index = 0; l_Index < 4; ++l_Index)
                                    {
                                        /// Checks if player has one of the benefic auras.
                                        if (Aura* l_Aura = l_Player->GetAura(beneficAuras[l_Index]))
                                        {
                                            l_StackCount += l_Aura->GetStackAmount();

                                            /// If the cumulated stacks >= 5, the player gets transformed, then the AT despawns (player keeps benefic auras).
                                            if (l_StackCount >= 5)
                                            {
                                                me->AddAura(SPELL_FULLY_MUTATED, l_Player);
                                                me->AddAura(SPELL_SECOND_FULLY_MUTATED, l_Player);

                                                break;
                                            }
                                        }
                                    }

                                    /// Stops checking players.
                                    events.CancelEvent(EVENT_MUTAGENIC_CHECK);

                                    m_StartMutagenicMethod = false;

                                    l_AreaTrigger->Remove(0);
                                }
                            }
                        }

                        me->DespawnOrUnsummon();
                    }
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (p_Damage >= me->GetHealth())
                {
                    p_Damage = 0;

                    me->SetReactState(REACT_PASSIVE);
                    me->SetDisplayId(INVIBLE_DISPLAY);
                    me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NOT_SELECTABLE);

                    /// 80% chance to apply Volatile Pool.
                    /// 20% chance to apply Mutagenic Pool.
                    me->CastSpell(me, roll_chance_i(20) ? SPELL_VOLATILE_POOL : SPELL_MUTAGENIC_POOL, true);

                    /// Starts checking players near for applying instructions.
                    if (me->HasAura(SPELL_VOLATILE_POOL))
                        events.ScheduleEvent(EVENT_VOLATIL_CHECK, 500);
                    else
                        events.ScheduleEvent(EVENT_MUTAGENIC_CHECK, 700);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                /// When a Living Fluid reaches Primordius, this one gains one evolution stack, his power is reset, and he gets healed. The Living Fluid despawns.
                if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE))
                {
                    if (Creature* l_Primordius = GetClosestCreatureWithEntry(me, NPC_PRIMORDIUS, 0.3f, true))
                    {
                        l_Primordius->AddAura(SPELL_EVOLUTION, l_Primordius);
                        l_Primordius->SetPower(POWER_ENERGY, 0, true);
                        l_Primordius->ModifyHealth(l_Primordius->GetMaxHealth() * 0.10f);
                        me->DespawnOrUnsummon();
                    }
                }

                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    /// Checks every 500 ms if there's a player near of living fluid with volatile pool form. If so, call VolatilePool();
                    case EVENT_VOLATIL_CHECK:
                    {
                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 3.0f);

                        if (!l_PlayerList.empty())
                        {
                            m_StartVolatileMethod = true;

                            VolatilePool();
                        }

                        events.ScheduleEvent(EVENT_VOLATIL_CHECK, 500);
                        break;
                    }
                    /// Checks every 500 ms if there's an areatrigger, then a player near of this AT. If so, call MutagenicPool();
                    case EVENT_MUTAGENIC_CHECK:
                    {
                        std::list<AreaTrigger*> l_AreatriggerList;
                        me->GetAreaTriggerList(l_AreatriggerList, SPELL_MUTAGENIC_POOL);

                        if (!l_AreatriggerList.empty())
                        {
                            for (AreaTrigger* l_AreaTrigger : l_AreatriggerList)
                            {
                                std::list<Player*> l_PlayerList;

                                /// Checks every player in a radius <= 3.0f from the AT.
                                l_AreaTrigger->GetPlayerListInGrid(l_PlayerList, 3.0f);

                                if (!l_PlayerList.empty())
                                {
                                    m_StartMutagenicMethod = true;
                                    MutagenicPool();
                                }
                            }
                        }

                        events.ScheduleEvent(EVENT_MUTAGENIC_CHECK, 500);
                        break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_living_fluidAI(p_Creature);
        }
};

/// Viscous Horror - 69070
class mob_viscous_horror : public CreatureScript
{
    public:
        mob_viscous_horror() : CreatureScript("mob_viscous_horror") { }

        struct mob_viscous_horrorAI : public ScriptedAI
        {
            mob_viscous_horrorAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);

                me->SetSpeed(MOVE_WALK, 0.3f);
                me->SetSpeed(MOVE_RUN, 0.3f);
            }

            void EnterCombat(Unit* /*attacker*/)
            {
                events.ScheduleEvent(EVENT_BLACK_BLOOD, 10000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                /// If he gets <= 10.0f distance from primordius, wipes all the raid.
                if (m_Instance)
                    if (Creature* l_Primordius = GetClosestCreatureWithEntry(me, NPC_PRIMORDIUS, 10.0f, true))
                        me->CastSpell(me, SPELL_DEADLY_MUTAGEN, false);

                events.Update(p_Diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BLACK_BLOOD:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_BLACK_BLOOD, false);

                        events.ScheduleEvent(EVENT_BLACK_BLOOD, 25000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_viscous_horrorAI(p_Creature);
        }
};

/// Congeal Blood - 136051
class spell_congeal_blood: public SpellScriptLoader
{
    public:
        spell_congeal_blood() : SpellScriptLoader("spell_congeal_blood") { }

        class spell_congeal_blood_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_congeal_blood_SpellScript);

            /// If target is not Vicious Horror or Living Fluid, damages are set to 0.
            void HandleBeforeHit()
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    if (Creature* l_Creature = l_Target->ToCreature())
                        if (l_Creature->GetEntry() != NPC_VISCOUS_HORROR || l_Creature->GetEntry() != NPC_LIVING_FLUID)
                            SetHitDamage(0);
                }
            }

            void Register()
            {
                BeforeHit += SpellHitFn(spell_congeal_blood_SpellScript::HandleBeforeHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_congeal_blood_SpellScript();
        }
};

/// Evolution - 139144
class spell_primordius_evolution: public SpellScriptLoader
{
    public:
        spell_primordius_evolution() : SpellScriptLoader("spell_primordius_evolution") { }

        class spell_primordius_evolution_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_primordius_evolution_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster    = GetCaster();
                Unit* l_Target    = GetTarget();

                if (!l_Caster || !l_Target)
                    return;

                std::list<uint32> l_AuraList;

                if (l_Target->GetInstanceScript()->instance->IsHeroic())
                {
                    /// Checks if Primordius already got one of those auras, and if so put it in a list. Process gets out of the loop if amount > 4.
                    for (uint8 l_Index = 0; l_Index <= 6; ++l_Index)
                    {
                        uint32 l_AuraId;

                        l_AuraId = evolutionAuras[l_Index];

                        if (l_Target->HasAura(l_AuraId))
                            l_AuraList.push_back(l_AuraId);

                        if (l_AuraList.size() >= 4)
                            break;
                    }

                    /// In the case Primordius has less than 4 auras, he gets a new random one added.
                    if (l_AuraList.size() < 4)
                        l_Caster->AddAura(evolutionAuras[urand(0, 6)], l_Target);
                    /// If Primordius has 4 auras or more from the table, one of those auras is removed, then a new random one is added.
                    else if (l_AuraList.size() >= 4)
                    {
                        JadeCore::RandomResizeList(l_AuraList, l_AuraList.size());

                        if (l_Target->HasAura(l_AuraList.front()))
                            l_Target->RemoveAura(l_AuraList.front());

                        l_Caster->AddAura(evolutionAuras[urand(0, 6)], l_Target);
                    }

                    return;
                }

                /// Checks if Primordius already got one of those auras, and if so put it in a list. Process gets out of the loop if amount > 3.
                for (uint8 l_Index = 0; l_Index <= 6; ++l_Index)
                {
                    uint32 l_AuraId;

                    l_AuraId = evolutionAuras[l_Index];

                    if (l_Target->HasAura(l_AuraId))
                        l_AuraList.push_back(l_AuraId);

                    if (l_AuraList.size() >= 3)
                        break;
                }

                /// In the case Primordius has less than 3 auras, he gets a new random one added.
                if (l_AuraList.size() < 3)
                {
                    l_Caster->AddAura(evolutionAuras[urand(0, 6)], l_Target);
                }
                /// If Primordius has 3 auras or more from the table, one of those auras is removed, then a new random one is added.
                else if (l_AuraList.size() >= 3)
                {
                    JadeCore::RandomResizeList(l_AuraList, l_AuraList.size());

                    if (l_Target->HasAura(l_AuraList.front()))
                        l_Target->RemoveAura(l_AuraList.front());

                    l_Caster->AddAura(evolutionAuras[urand(0, 6)], l_Target);
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_primordius_evolution_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_primordius_evolution_AuraScript();
        }
};

void AddSC_boss_primordius()
{
    new boss_primordius();
    new mob_living_fluid();
    new mob_viscous_horror();
    new spell_congeal_blood();
    new spell_primordius_evolution();
}