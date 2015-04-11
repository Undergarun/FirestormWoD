////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

/// Brackenspore <Walker of the Deep> - 78491
class boss_brackenspore : public CreatureScript
{
    public:
        boss_brackenspore() : CreatureScript("boss_brackenspore") { }

        enum eSpells
        {
            /// Misc
            Berserker               = 26662,
            Rot                     = 163240,
            RotDot                  = 163241,
            /// Necrotic Breath
            SpellNecroticBreath     = 159219,   ///< Triggers 159220 every second
            /// Infesting Spores
            SpellInfestingSpores    = 159996    ///< Triggers 163242 every second
        };

        enum eEvents
        {
            EventNecroticBreath = 1,
            EventBerserker,
            EventInfestingSpores
        };

        struct boss_brackensporeAI : public BossAI
        {
            boss_brackensporeAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossBrackenspore)
            {
                m_Instance  = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eSpells::Berserker);

                me->CastSpell(me, eSpells::Rot, true);

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RotDot);
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventNecroticBreath, 30 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBerserker, 600 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventInfestingSpores, 45 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RotDot);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventNecroticBreath:
                        me->CastSpell(me, eSpells::SpellNecroticBreath, false);
                        m_Events.ScheduleEvent(eEvents::EventNecroticBreath, 32 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventBerserker:
                        me->CastSpell(me, eSpells::Berserker, true);
                        break;
                    case eEvents::EventInfestingSpores:
                        me->CastSpell(me, eSpells::SpellInfestingSpores, false);
                        m_Events.ScheduleEvent(eEvents::EventInfestingSpores, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_brackensporeAI(p_Creature);
        }
};

/// Necrotic Breath - 159220
class spell_highmaul_necrotic_breath : public SpellScriptLoader
{
    public:
        spell_highmaul_necrotic_breath() : SpellScriptLoader("spell_highmaul_necrotic_breath") { }

        class spell_highmaul_necrotic_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_necrotic_breath_SpellScript);

            enum eSpell
            {
                TargetRestrict = 20036
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_necrotic_breath_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_necrotic_breath_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_necrotic_breath_SpellScript();
        }
};

void AddSC_boss_brackenspore()
{
    /// Boss
    new boss_brackenspore();

    /// NPCs

    /// Spells
    new spell_highmaul_necrotic_breath();

    /// AreaTriggers
}