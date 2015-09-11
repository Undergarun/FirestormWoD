////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

/// Oregorger <The Devourer> - 77182
class boss_oregorger : public CreatureScript
{
    public:
        boss_oregorger() : CreatureScript("boss_oregorger") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        enum eCosmeticEvents
        {
        };

        enum eActions
        {
        };

        enum eCreatures
        {
            PathIdentifier  = 77848,
            BlackrockOre    = 77261,
            OreCrate        = 77252
        };

        struct boss_oregorgerAI : public BossAI
        {
            boss_oregorgerAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataOregorger)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            InstanceScript* m_Instance;

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                me->RemoveAura(eFoundrySpells::Berserker);

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

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    ///CastSpellToPlayers(me->GetMap(), me, eSpells::GruulBonus, true);

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
                me->RemoveAllAuras();

                me->InterruptNonMeleeSpells(true);

                /// Just in case, to prevent the fail Return to Home
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                me->ClearUnitState(UnitState::UNIT_STATE_DISTRACTED);
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eFoundryDatas::DataGruul, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
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
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    default:
                        break;
                }

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
            return new boss_oregorgerAI(p_Creature);
        }
};

void AddSC_boss_oregorger()
{
    /// Boss
    new boss_oregorger();

    /// NPCs

    /// Spells

    /// AreaTriggers
}