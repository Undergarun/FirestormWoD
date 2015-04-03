////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

/// The Butcher - 77404
class boss_the_butcher : public CreatureScript
{
    public:
        boss_the_butcher() : CreatureScript("boss_the_butcher") { }

        enum eSpells
        {
            /// Heavy Handed
            HeavyHandedAura = 156135,
            HeavyHandedProc = 156138,
            /// The Cleaver
            TheCleaverDmg   = 156143,
            TheCleaverDot   = 156147,
            /// The Tenderizer
            TheTenderizer   = 156151,
            /// Meat Hook
            MeatHook        = 156125,
            MeatHookJump    = 156127,
            /// Berserker
            SpellBerserk    = 26662,
            /// Frenzy (30%)
            SpellFrenzy     = 156598
        };

        enum eEvents
        {
            EventTenderizer = 1,
            EventCleave,
            EventCleaver,
            EventBerserk,
            EventBoundingCleave,
            EventMeatHook
        };

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3, ///< Didn't see it on video
            Aggro,
            BoundingCleave,
            Frenzy,
            Cleave,
            Berserk,
            Slay,
            Wipe
        };

        struct boss_the_butcherAI : public BossAI
        {
            boss_the_butcherAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTheButcher)
            {
                m_IntroDone = false;
            }

            EventMap m_Events;

            uint32 m_RegenTimer;

            bool m_IntroDone;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                m_RegenTimer = 0;

                me->RemoveAura(eSpells::SpellFrenzy);

                /// Wrong flags remove
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_PREPARATION);

                me->SetPower(Powers::POWER_ENERGY, 0);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                m_RegenTimer = 3000;

                Talk(eTalks::Aggro);

                m_Events.ScheduleEvent(eEvents::EventTenderizer, 6000);
                m_Events.ScheduleEvent(eEvents::EventCleave, 10000);
                m_Events.ScheduleEvent(eEvents::EventCleaver, 12000);
                m_Events.ScheduleEvent(eEvents::EventBerserk, 300000);

                /// Meat Hook is an ability that The Butcher uses to pull his tank to him.
                /// We assume that this ability exists to prevent The Butcher from being kited,
                /// but it is not otherwise in use during the fight.
                m_Events.ScheduleEvent(eEvents::EventMeatHook, 4000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HasAura(eSpells::SpellFrenzy))
                    return;

                if (me->HealthBelowPctDamaged(30, p_Damage))
                {
                    me->CastSpell(me, eSpells::SpellFrenzy, true);
                    Talk(eTalks::Frenzy);
                }
            }

            void EnterEvadeMode() override
            {
                _EnterEvadeMode();

                Talk(eTalks::Wipe);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void SpellMissTarget(Unit* p_Target, SpellInfo const* p_SpellInfo, SpellMissInfo p_MissInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::TheCleaverDmg:
                    {
                        /// This attack will not apply its bleed component if it is avoided.
                        if (p_MissInfo == SpellMissInfo::SPELL_MISS_DODGE)
                            break;
                        me->AddAura(eSpells::TheCleaverDot, p_Target);
                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::MeatHook:
                        p_Target->CastSpell(me, eSpells::MeatHookJump, true);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_IntroDone)
                {
                    if (Player* l_Player = me->FindNearestPlayer(130.0f))
                    {
                        m_IntroDone = true;
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });
                        AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                    }
                }

                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                ScheduleEnergyRegen(p_Diff);

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventTenderizer:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::TheTenderizer, true);
                        m_Events.ScheduleEvent(eEvents::EventTenderizer, 16500);
                        break;
                    case eEvents::EventCleave:
                        Talk(eTalks::Cleave);
                        m_Events.ScheduleEvent(eEvents::EventCleave, 6000);
                        break;
                    case eEvents::EventCleaver:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::TheCleaverDmg, true);
                        m_Events.ScheduleEvent(eEvents::EventCleaver, 7500);
                        break;
                    case eEvents::EventBerserk:
                        Talk(eTalks::Berserk);
                        me->CastSpell(me, eSpells::SpellBerserk, true);
                        break;
                    case eEvents::EventBoundingCleave:
                        Talk(eTalks::BoundingCleave);
                        me->SetPower(Powers::POWER_ENERGY, 0);
                        break;
                    case eEvents::EventMeatHook:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            if (!l_Target->IsWithinMeleeRange(me))
                                me->CastSpell(l_Target, eSpells::MeatHook, true);
                        }
                        m_Events.ScheduleEvent(eEvents::EventMeatHook, 4000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleEnergyRegen(uint32 const p_Diff)
            {
                if (!m_RegenTimer)
                    return;

                /// The Butcher gains 5 energy every 3 seconds
                if (m_RegenTimer <= p_Diff)
                {
                    int32 l_Value = me->HasAura(eSpells::SpellFrenzy) ? 10 : 5;
                    me->ModifyPower(Powers::POWER_ENERGY, l_Value);
                    m_RegenTimer = 3000;

                    /// Bounding Cleave Icon Bounding Cleave is an ability that The Butcher uses when he reaches 100 Energy
                    /// (this happens exactly every 60 seconds before The Butcher reaches 30% health, and every 30 seconds after that).
                    if (me->GetPower(Powers::POWER_ENERGY) >= 100)
                        m_Events.ScheduleEvent(eEvents::EventBoundingCleave, 50);
                }
                else
                    m_RegenTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_the_butcherAI(p_Creature);
        }
};

/// Heavy Handed - 156135
class spell_highmaul_heavy_handed : public SpellScriptLoader
{
    public:
        spell_highmaul_heavy_handed() : SpellScriptLoader("spell_highmaul_heavy_handed") { }

        enum eSpell
        {
            HeavyHandedProc = 156138
        };

        class spell_highmaul_heavy_handed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_heavy_handed_AuraScript);

            void OnProc(constAuraEffectPtr p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() != nullptr &&
                    p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == eSpell::HeavyHandedProc)
                    return;

                if (Unit* l_Butcher = GetTarget())
                {
                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                        l_Butcher->CastSpell(l_Target, eSpell::HeavyHandedProc, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_highmaul_heavy_handed_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_heavy_handed_AuraScript();
        }
};

/// Heavy Handed - 156138
class spell_highmaul_heavy_handed_proc : public SpellScriptLoader
{
    public:
        spell_highmaul_heavy_handed_proc() : SpellScriptLoader("spell_highmaul_heavy_handed_proc") { }

        class spell_highmaul_heavy_handed_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_heavy_handed_proc_SpellScript);

            uint64 m_Target;

            bool Load() override
            {
                m_Target = 0;
                return true;
            }

            void HandleBeforeCast()
            {
                if (Unit* l_Target = GetExplTargetUnit())
                    m_Target = l_Target->GetGUID();
            }

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                /// All attacks made by the caster strike the next closest target
                if (p_Targets.size() > 1)
                {
                    p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                    {
                        if (p_Object == nullptr || p_Object->GetGUID() == m_Target)
                            return true;

                        return false;
                    });

                    if (p_Targets.size() > 1)
                    {
                        p_Targets.sort(JadeCore::ObjectDistanceOrderPred(l_Caster));
                        WorldObject* l_Object = p_Targets.front();
                        p_Targets.clear();
                        p_Targets.push_back(l_Object);
                    }
                }
                /// If no target is found, the primary target is struck again.
                else
                {
                    p_Targets.clear();

                    if (Unit* l_Target = Unit::GetUnit(*l_Caster, m_Target))
                        p_Targets.push_back(l_Target);
                }
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(spell_highmaul_heavy_handed_proc_SpellScript::HandleBeforeCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_heavy_handed_proc_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_heavy_handed_proc_SpellScript();
        }
};

void AddSC_boss_the_butcher()
{
    /// Boss
    new boss_the_butcher();

    /// NPCs

    /// Spells
    new spell_highmaul_heavy_handed();
    new spell_highmaul_heavy_handed_proc();
}