////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"

enum eSpells
{
    // Tortos
    SPELL_SNAPPING_BITE         = 135251,
    SPELL_FURIOUS_STONE_BREATH  = 133939,
    SPELL_GROWING_FURY          = 136010,
    SPELL_QUAKE_STOMP           = 134920,
    SPELL_ROCK_FALL             = 134849,
    SPELL_ROCK_FALL_GROUND_AREA = 134475,
    SPELL_ROCK_FALL_SUMMON      = 134365,
    SPELL_ROCK_FALL_LARGE_DMG   = 134476,
    SPELL_CALL_OF_TORTOS        = 136294,

    // Whirl Turtle
    SPELL_SPINNING_SHELL_DMG    = 134011,
    SPELL_SPINNING_SHELL_AURA   = 133974,
    SPELL_SHELL_BLOCK_AURA      = 133971,
    SPELL_KICK_SHELL_OVERRIDER  = 134030,
    SPELL_KICK_SHELL_ROOT       = 134073,
    SPELL_SHELL_CONCUSSION      = 134092,

    // Humming Crystal
    SPELL_CRYSTAL_SHELL_AURA    = 137552,
    SPELL_CRYSTAL_SHELL_DMG     = 137633,
    SPELL_CRYSTAL_SHELL_HEAL    = 137648
};

enum eEvents
{
    EVENT_BERSERK               = 1,
    EVENT_SNAPPING_BITE,
    EVENT_FURIOUS_STONE_BREATH,
    EVENT_CHECK_MELEE,
    EVENT_QUAKE_STOMP,
    EVENT_VAMPIRIC_BAT,
    EVENT_ROCKFALL,
    EVENT_CALL_OF_TORTOS,

    // Whirl Turtle
    EVENT_CHECK_NEAR_PLAYERS,
    EVENT_SWITCH_TARGET
};

enum eActions
{
    ACTION_SPAWN_WHIRL_TURTLE
};

enum eMoves
{
    MOVE_KICK_SHELL = 1,
    MOVE_TURTLE_POS
};

Position const vampiricBatSummonPos[2] =
{
    { 6015.67f, 4975.62f, 61.51f, 5.96f },
    { 6064.82f, 4959.10f, 61.48f, 2.81f }
};

Position const whirlTurtleSpawnPos = { 6033.39f, 4879.94f, -61.19f, 1.45f };

Position const whirlTurtleMovePos = { 6035.80f, 4951.44f, -61.22f, 1.42f };

class RockFallLargeTargetSelector
{
    public:
        RockFallLargeTargetSelector(Unit* caster) : _caster(caster) { };
        bool operator()(Unit* unit) const
        {
            return unit->GetDistance(_caster) <= 5.0f;
        }

        bool operator()(WorldObject* object) const
        {
            return object->GetDistance(_caster) <= 5.0f;
        }

    private:
        Unit* _caster;
};

class ShellConcussionTargetSelector
{
    public:

        bool operator()(WorldObject* object) const
        {
            return object->IsPlayer() || (object->ToUnit() && object->ToUnit()->IsControlledByPlayer());
        }
};

// Tortos - 67977
class boss_tortos : public CreatureScript
{
    public:
        boss_tortos() : CreatureScript("boss_tortos") { }

        struct boss_tortosAI : public BossAI
        {
            boss_tortosAI(Creature* creature) : BossAI(creature, DATA_TORTOS)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            bool breathScheduled;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();

                summons.DespawnAll();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetPower(POWER_ENERGY, 0);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->RemoveAura(SPELL_ROCK_FALL);

                breathScheduled = false;

                _Reset();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_TORTOS, NOT_STARTED);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CRYSTAL_SHELL_DMG);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CRYSTAL_SHELL_HEAL);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_KICK_SHELL_OVERRIDER);
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                _EnterCombat();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_TORTOS, IN_PROGRESS);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }

                events.ScheduleEvent(EVENT_BERSERK, 780 * IN_MILLISECONDS);
                events.ScheduleEvent(EVENT_SNAPPING_BITE, 8000);
                events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                events.ScheduleEvent(EVENT_QUAKE_STOMP, 29000);
                events.ScheduleEvent(EVENT_VAMPIRIC_BAT, 30000);
                events.ScheduleEvent(EVENT_ROCKFALL, 5000);
                events.ScheduleEvent(EVENT_CALL_OF_TORTOS, 14500);
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);

                if (summon->GetEntry() == NPC_ROCKFALL)
                    summon->CastSpell(summon, SPELL_ROCK_FALL_GROUND_AREA, false);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_TORTOS, FAIL);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SetBossState(DATA_TORTOS, DONE);
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CRYSTAL_SHELL_DMG);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_CRYSTAL_SHELL_HEAL);
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_KICK_SHELL_OVERRIDER);
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootDistribution(me, l_Player->GetGroup());
                }

                me->SummonGameObject(GOB_TORTOS_DEATH_COLLISION, 6038.69f, 4923.87f, -61.1953f, 1.513821f, 0, 0, 0, 1.0f, 0);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SPAWN_WHIRL_TURTLE:
                    {
                        me->MonsterTextEmote("Tortos lets out a booming call, attracting nearby turtles.", 0, true);
                        for (uint8 i = 0; i < 3; ++i)
                        {
                            if (Creature* turtle = me->SummonCreature(NPC_WHIRL_TURTLE, whirlTurtleSpawnPos.m_positionX, whirlTurtleSpawnPos.m_positionY, whirlTurtleSpawnPos.m_positionZ))
                            {
                                Position pos = whirlTurtleMovePos;
                                turtle->GetMotionMaster()->MovePoint(MOVE_TURTLE_POS, pos.m_positionX, pos.m_positionY, pos.m_positionZ);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers power, int32& value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (me->HasAura(SPELL_FURIOUS_STONE_BREATH) || !me->isInCombat())
                {
                    value = 0;
                    return;
                }

                // Tortos regenerates 2 energy per second
                // Creatures regenerate every 2s
                value = 4;

                if (me->GetPower(POWER_ENERGY) + value >= 100 && !breathScheduled)
                {
                    breathScheduled = true;
                    events.ScheduleEvent(EVENT_FURIOUS_STONE_BREATH, 100);
                }
            }

            void SetPower(Powers power, int32 value)
            {
                if (power != POWER_ENERGY)
                    return;

                if (value == 0)
                    breathScheduled = false;
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
                    case EVENT_BERSERK:
                        me->CastSpell(me, SPELL_ENRAGE, true);
                        break;
                    case EVENT_SNAPPING_BITE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SNAPPING_BITE, false);
                        events.ScheduleEvent(EVENT_SNAPPING_BITE, 8000);
                        break;
                    case EVENT_FURIOUS_STONE_BREATH:
                        me->MonsterTextEmote("Tortos prepares to unleash a |cFFFF0000|Hspell:133939|h[Souffle de pierre furieux]|h|r !", 0, true);
                        me->CastSpell(me, SPELL_FURIOUS_STONE_BREATH, false);
                        break;
                    case EVENT_CHECK_MELEE:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        {
                            if (!me->IsWithinMeleeRange(target))
                                me->CastSpell(me, SPELL_GROWING_FURY, true);
                        }
                        events.ScheduleEvent(EVENT_CHECK_MELEE, 1000);
                        break;
                    }
                    case EVENT_QUAKE_STOMP:
                        me->CastSpell(me, SPELL_QUAKE_STOMP, false);
                        events.ScheduleEvent(EVENT_QUAKE_STOMP, 49000);
                        break;
                    case EVENT_VAMPIRIC_BAT:
                    {
                        for (uint8 i = 0; i < 8; ++i)
                        {
                            Position pos = vampiricBatSummonPos[urand(0, 1)];
                            if (Creature* vampiricBat = me->SummonCreature(NPC_VAMPIRIC_CAVE_BAT_SUMMON, pos.m_positionX, pos.m_positionY, pos.m_positionZ))
                            {
                                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                    vampiricBat->AI()->AttackStart(target);
                            }
                        }

                        events.ScheduleEvent(EVENT_VAMPIRIC_BAT, 30000);
                        break;
                    }
                    case EVENT_ROCKFALL:
                        me->CastSpell(me, SPELL_ROCK_FALL, true);
                        break;
                    case EVENT_CALL_OF_TORTOS:
                        me->CastSpell(me, SPELL_CALL_OF_TORTOS, false);
                        events.ScheduleEvent(EVENT_CALL_OF_TORTOS, 60000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_tortosAI(creature);
        }
};

// Rockfall - 68219
class mob_rockfall : public CreatureScript
{
    public:
        mob_rockfall() : CreatureScript("mob_rockfall") { }

        struct mob_rockfallAI : public ScriptedAI
        {
            mob_rockfallAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_rockfallAI(creature);
        }
};

// Vampiric Cave Bat - 68497
class mob_vampiric_cave_bat_summon : public CreatureScript
{
    public:
        mob_vampiric_cave_bat_summon() : CreatureScript("mob_vampiric_cave_bat_summon") { }

        struct mob_vampiric_cave_bat_summonAI : public ScriptedAI
        {
            mob_vampiric_cave_bat_summonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->CastSpell(me, SPELL_DRAIN_THE_WEAK, true);
                me->ReenableEvadeMode();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_vampiric_cave_bat_summonAI(creature);
        }
};

// Whirl Turtle - 67966
class mob_whirl_turtle : public CreatureScript
{
    public:
        mob_whirl_turtle() : CreatureScript("mob_whirl_turtle") { }

        struct mob_whirl_turtleAI : public ScriptedAI
        {
            mob_whirl_turtleAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            bool shellBlocked;

            void Reset()
            {
                shellBlocked = false;

                me->CastSpell(me, SPELL_SPINNING_SHELL_AURA, true);
                me->ReenableEvadeMode();

                me->SetReactState(REACT_PASSIVE);

                me->SetSpeed(MOVE_WALK, 2.0f);
                me->SetSpeed(MOVE_RUN, 2.0f);

                events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                events.ScheduleEvent(EVENT_CHECK_NEAR_PLAYERS, 1500);
                events.ScheduleEvent(EVENT_SWITCH_TARGET, 3000);
            }

            void MovementInform(uint32 /*type*/, uint32 id)
            {
                switch (id)
                {
                    case MOVE_KICK_SHELL:
                        me->DespawnOrUnsummon();
                        break;
                    case MOVE_TURTLE_POS:
                    {
                        std::list<Player*> plrList;
                        me->GetPlayerListInGrid(plrList, 60.0f);
                        if (plrList.empty())
                            break;

                        me->SetSpeed(MOVE_WALK, 1.0f);
                        me->SetSpeed(MOVE_RUN, 1.0f);
                        JadeCore::RandomResizeList(plrList, 1);
                        me->GetMotionMaster()->MoveChase(plrList.front());
                        me->CombatStart(plrList.front());
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (shellBlocked)
                    return;

                if (me->HealthBelowPctDamaged(5, damage))
                {
                    shellBlocked = true;
                    me->RemoveAura(SPELL_SPINNING_SHELL_AURA);
                    me->CastSpell(me, SPELL_SHELL_BLOCK_AURA, true);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_NEAR_PLAYERS:
                    {
                        if (me->HasAura(SPELL_SHELL_BLOCK_AURA))
                            break;

                        std::list<Player*> plrList;
                        me->GetPlayerListInGrid(plrList, 5.0f);

                        for (auto itr : plrList)
                            itr->CastSpell(itr, SPELL_SPINNING_SHELL_DMG, true);

                        events.ScheduleEvent(EVENT_CHECK_NEAR_PLAYERS, 1500);
                        break;
                    }
                    case EVENT_SWITCH_TARGET:
                    {
                        if (me->HasAura(SPELL_SHELL_BLOCK_AURA))
                            break;

                        std::list<Player*> plrList;
                        me->GetPlayerListInGrid(plrList, 60.0f);
                        if (plrList.empty())
                        {
                            events.ScheduleEvent(EVENT_SWITCH_TARGET, 3000);
                            break;
                        }

                        JadeCore::RandomResizeList(plrList, 1);
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveChase(plrList.front());
                        events.ScheduleEvent(EVENT_SWITCH_TARGET, 3000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_whirl_turtleAI(creature);
        }
};

// Humming Crystal - 69639
class mob_humming_crystal : public CreatureScript
{
    public:
        mob_humming_crystal() : CreatureScript("mob_humming_crystal") { }

        struct mob_humming_crystalAI : public ScriptedAI
        {
            mob_humming_crystalAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_CRYSTAL_SHELL_AURA, true);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_humming_crystalAI(creature);
        }
};

// Quake Stomp - 134920
class spell_quake_stomp: public SpellScriptLoader
{
    public:
        spell_quake_stomp() : SpellScriptLoader("spell_quake_stomp") { }

        class spell_quake_stomp_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_quake_stomp_SpellScript);

            void HandleDamage(SpellEffIndex /*effIndex*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        SetHitDamage(target->CountPctFromMaxHealth(65));
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_quake_stomp_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_quake_stomp_SpellScript();
        }
};

// Rockfall - 134364 / 140431
class spell_rockfall: public SpellScriptLoader
{
    public:
        spell_rockfall() : SpellScriptLoader("spell_rockfall") { }

        class spell_rockfall_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rockfall_SpellScript);

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
                        caster->CastSpell(target, SPELL_ROCK_FALL_SUMMON, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rockfall_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnHit += SpellHitFn(spell_rockfall_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rockfall_SpellScript();
        }
};

// Rockfall (damage) - 134539
class spell_rockfall_damage: public SpellScriptLoader
{
    public:
        spell_rockfall_damage() : SpellScriptLoader("spell_rockfall_damage") { }

        class spell_rockfall_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rockfall_damage_SpellScript);

            void HandleAfterCast()
            {
                if (Unit* caster = GetCaster())
                    caster->CastSpell(caster, SPELL_ROCK_FALL_LARGE_DMG, true);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_rockfall_damage_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rockfall_damage_SpellScript();
        }
};

// Rockfall (large damage) - 134476
class spell_rockfall_large_damage: public SpellScriptLoader
{
    public:
        spell_rockfall_large_damage() : SpellScriptLoader("spell_rockfall_large_damage") { }

        class spell_rockfall_large_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_rockfall_large_damage_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if(RockFallLargeTargetSelector(GetCaster()));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_rockfall_large_damage_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_rockfall_large_damage_SpellScript();
        }
};

// Call of Tortos - 136294
class spell_call_of_tortos: public SpellScriptLoader
{
    public:
        spell_call_of_tortos() : SpellScriptLoader("spell_call_of_tortos") { }

        class spell_call_of_tortos_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_call_of_tortos_SpellScript);

            void HandleAfterCast()
            {
                if (Creature* tortos = GetCaster()->ToCreature())
                    tortos->AI()->DoAction(ACTION_SPAWN_WHIRL_TURTLE);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_call_of_tortos_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_call_of_tortos_SpellScript();
        }
};

// Shell Block (triggered) - 140054
class spell_shell_block: public SpellScriptLoader
{
    public:
        spell_shell_block() : SpellScriptLoader("spell_shell_block") { }

        class spell_shell_block_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_shell_block_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_KICK_SHELL_OVERRIDER, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->RemoveAura(SPELL_KICK_SHELL_OVERRIDER);
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_shell_block_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_shell_block_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_shell_block_AuraScript();
        }
};

// Kick Shell - 134031
class spell_kick_shell: public SpellScriptLoader
{
    public:
        spell_kick_shell() : SpellScriptLoader("spell_kick_shell") { }

        class spell_kick_shell_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_kick_shell_AuraScript);

            void OnApply(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->CastSpell(target, SPELL_KICK_SHELL_ROOT, true);
            }

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        target->RemoveAura(SPELL_KICK_SHELL_ROOT);
                        target->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED | UNIT_STATE_ROOT);
                        target->SetSpeed(MOVE_WALK, 5.0f);
                        target->SetSpeed(MOVE_RUN, 5.0f);
                        target->AddAura(SPELL_SHELL_CONCUSSION, target);
                        float orientation = caster->GetOrientation();
                        float x = target->GetPositionX() + ((150.0f) * cos(orientation));
                        float y = target->GetPositionY() + ((150.0f) * sin(orientation));
                        target->GetMotionMaster()->Clear();
                        target->GetMotionMaster()->MovePoint(MOVE_KICK_SHELL, x, y, target->GetPositionZ());
                    }
                }
            }

            void Register()
            {
                AfterEffectApply += AuraEffectApplyFn(spell_kick_shell_AuraScript::OnApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(spell_kick_shell_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_kick_shell_AuraScript();
        }
};

// Shell Concussion - 134091 / 136431
class spell_shell_concussion: public SpellScriptLoader
{
    public:
        spell_shell_concussion() : SpellScriptLoader("spell_shell_concussion") { }

        class spell_shell_concussion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shell_concussion_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if(ShellConcussionTargetSelector());
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_shell_concussion_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_shell_concussion_SpellScript();
        }
};

// Crystal Shell (damage absorb) - 137633
class spell_crystal_shell_damage_absorption: public SpellScriptLoader
{
    public:
        spell_crystal_shell_damage_absorption() : SpellScriptLoader("spell_crystal_shell_damage_absorption") { }

        class spell_crystal_shell_damage_absorption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_crystal_shell_damage_absorption_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                if (Unit* target = GetUnitOwner())
                    amount = target->CountPctFromMaxHealth(15);
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* target = GetTarget())
                    target->RemoveAura(SPELL_CRYSTAL_SHELL_HEAL);
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_crystal_shell_damage_absorption_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB);
                AfterEffectRemove += AuraEffectRemoveFn(spell_crystal_shell_damage_absorption_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_SCHOOL_ABSORB, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_crystal_shell_damage_absorption_AuraScript();
        }
};

// Crystal Shell (heal absorb) - 137648
class spell_crystal_shell_heal_absorption: public SpellScriptLoader
{
    public:
        spell_crystal_shell_heal_absorption() : SpellScriptLoader("spell_crystal_shell_heal_absorption") { }

        class spell_crystal_shell_heal_absorption_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_crystal_shell_heal_absorption_AuraScript);

            void CalculateAmount(AuraEffect const* /*aurEff*/, int32 & amount, bool & /*canBeRecalculated*/)
            {
                amount = -1;
            }

            void OnAbsorb(AuraEffect* /*p_AurEff*/, DamageInfo& dmgInfo, uint32& absorbAmount)
            {
                if (Unit* target = GetTarget())
                {
                    absorbAmount = dmgInfo.GetDamage();

                    if (AuraEffect* damageAbsorb = target->GetAuraEffect(SPELL_CRYSTAL_SHELL_DMG, EFFECT_0))
                    {
                        uint32 amount = damageAbsorb->GetAmount();
                        uint32 maxAmount = target->CountPctFromMaxHealth(75);
                        uint32 newAmount = std::min((amount + absorbAmount), maxAmount);
                        damageAbsorb->ChangeAmount(newAmount);
                    }
                }
            }

            void Register()
            {
                DoEffectCalcAmount += AuraEffectCalcAmountFn(spell_crystal_shell_heal_absorption_AuraScript::CalculateAmount, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB);
                OnEffectAbsorb += AuraEffectAbsorbFn(spell_crystal_shell_heal_absorption_AuraScript::OnAbsorb, EFFECT_0, SPELL_AURA_SCHOOL_HEAL_ABSORB);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_crystal_shell_heal_absorption_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_tortos()
{
    new boss_tortos();
    new mob_rockfall();
    new mob_vampiric_cave_bat_summon();
    new mob_whirl_turtle();
    new mob_humming_crystal();
    new spell_quake_stomp();
    new spell_rockfall();
    new spell_rockfall_damage();
    new spell_rockfall_large_damage();
    new spell_call_of_tortos();
    new spell_shell_block();
    new spell_kick_shell();
    new spell_shell_concussion();
    new spell_crystal_shell_damage_absorption();
    new spell_crystal_shell_heal_absorption();
}
#endif
