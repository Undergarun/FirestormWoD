////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

/// Ko'ragh <Breaker of Magic> - 79015
class boss_koragh : public CreatureScript
{
    public:
        boss_koragh() : CreatureScript("boss_koragh") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        enum eActions
        {
        };

        enum eCreatures
        {
        };

        enum eTalks
        {
            Intro,
            Aggro,
            BarrierShattered,
            ExpelMagic,
            SuppressionField,
            DominatingPower,
            Berserk,
            Slay,
            Death,
            CausticEnergyWarn,
            ExpelMagicFelWarn,
            ExpelMagicArcaneWarn
        };

        struct boss_koraghAI : public BossAI
        {
            boss_koraghAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossKoragh)
            {
                m_Instance  = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                /*switch (p_Action)
                {
                    default:
                        break;
                }*/
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::Aggro);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    if (IsLFR())
                    {
                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }

                Talk(eTalks::Death);
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                    m_Instance->SetBossState(eHighmaulDatas::BossTectus, EncounterState::FAIL);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                /*switch (p_SpellInfo->Id)
                {
                    default:
                        break;
                }*/
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                /*switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_koraghAI(p_Creature);
        }
};

/// Breaker Ritualist <Breaker of Fel> - 86330
class npc_highmaul_breaker_of_fel : public CreatureScript
{
    public:
        npc_highmaul_breaker_of_fel() : CreatureScript("npc_highmaul_breaker_of_fel") { }

        enum eSpells
        {
            /// Cosmetic
            FelBreakerFelChannel    = 172664
        };

        enum eEvents
        {
        };

        struct npc_highmaul_breaker_of_felAI : public ScriptedAI
        {
            npc_highmaul_breaker_of_felAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::FelBreakerFelChannel, false);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_breaker_of_felAI(p_Creature);
        }
};

/// Breaker Ritualist <Breaker of Fire> - 86329
class npc_highmaul_breaker_of_fire : public CreatureScript
{
    public:
        npc_highmaul_breaker_of_fire() : CreatureScript("npc_highmaul_breaker_of_fire") { }

        enum eSpells
        {
            /// Cosmetic
            FelBreakerFireChannel   = 172455
        };

        enum eEvents
        {
        };

        struct npc_highmaul_breaker_of_fireAI : public ScriptedAI
        {
            npc_highmaul_breaker_of_fireAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::FelBreakerFireChannel, false);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_breaker_of_fireAI(p_Creature);
        }
};

/// Breaker Ritualist <Breaker of Frost> - 86326
class npc_highmaul_breaker_of_frost : public CreatureScript
{
    public:
        npc_highmaul_breaker_of_frost() : CreatureScript("npc_highmaul_breaker_of_frost") { }

        enum eSpells
        {
            /// Cosmetic
            FelBreakerFrostChannel  = 172448
        };

        enum eEvents
        {
        };

        struct npc_highmaul_breaker_of_frostAI : public ScriptedAI
        {
            npc_highmaul_breaker_of_frostAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                me->CastSpell(me, eSpells::FelBreakerFrostChannel, false);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_breaker_of_frostAI(p_Creature);
        }
};

void AddSC_boss_koragh()
{
    /// Boss
    new boss_koragh();

    /// NPCs
    new npc_highmaul_breaker_of_fel();
    new npc_highmaul_breaker_of_fire();
    new npc_highmaul_breaker_of_frost();

    /// Spells

    /// AreaTriggers
}