////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

/// Pol - 78238
class boss_twin_ogron_pol : public CreatureScript
{
    public:
        boss_twin_ogron_pol() : CreatureScript("boss_twin_ogron_pol") { }

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
        };

        struct boss_twin_ogron_polAI : public BossAI
        {
            boss_twin_ogron_polAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTwinOgron)
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
                switch (p_Action)
                {
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();
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

                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_twin_ogron_polAI(p_Creature);
        }
};

/// Phemos - 78237
class boss_twin_ogron_phemos : public CreatureScript
{
    public:
        boss_twin_ogron_phemos() : CreatureScript("boss_twin_ogron_phemos") { }

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
        };

        struct boss_twin_ogron_phemosAI : public BossAI
        {
            boss_twin_ogron_phemosAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTwinOgron)
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
                switch (p_Action)
                {
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();
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

                switch (p_SpellInfo->Id)
                {
                    default:
                        break;
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_twin_ogron_phemosAI(p_Creature);
        }
};

void AddSC_boss_twin_ogron()
{
    new boss_twin_ogron_pol();
    new boss_twin_ogron_phemos();
}