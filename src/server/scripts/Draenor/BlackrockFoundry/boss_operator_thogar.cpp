////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "boss_operator_thogar.hpp"

/// Operator Thogar <Grimrail Overseer> - 76906
class boss_operator_thogar : public CreatureScript
{
    public:
        boss_operator_thogar() : CreatureScript("boss_operator_thogar") { }

        enum eEvents
        {
            EventBerserker = 1
        };

        enum eTimers
        {
            TimerBerserker = 8 * TimeConstants::MINUTE * TimeConstants::IN_MILLISECONDS
        };

        struct boss_operator_thogarAI : public BossAI
        {
            boss_operator_thogarAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataOperatorThogar) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eFoundrySpells::Berserker);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->IsPlayer())
                    Talk(eThogarTalks::TalkSlay);
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                Talk(eThogarTalks::TalkAggro);

                m_Events.ScheduleEvent(eEvents::EventBerserker, eTimers::TimerBerserker);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                Talk(eThogarTalks::TalkDeath);
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                Talk(eThogarTalks::TalkWipe);
            }

            void DoAction(int32 const /*p_Action*/) override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eFoundrySpells::Berserker, true);
                        Talk(eThogarTalks::TalkBerserk);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_operator_thogarAI(p_Creature);
        }
};

/// Train - 76907
class npc_foundry_train : public CreatureScript
{
    public:
        npc_foundry_train() : CreatureScript("npc_foundry_train") { }

        enum eSpells
        {
        };

        enum eEvents
        {
        };

        struct npc_foundry_trainAI : public ScriptedAI
        {
            npc_foundry_trainAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                AddTimedDelayedOperation(100, [this]() -> void
                {
                    if (Creature* l_Train = me->SummonCreature(81226, *me))
                    {
                        if (Creature* l_SiegeEngine = l_Train->SummonCreature(81316, *l_Train))
                            l_SiegeEngine->EnterVehicle(l_Train);

                        l_Train->EnterVehicle(me);

                        l_Train->CastSpell(l_Train, 156553, true);
                        l_Train->CastSpell(l_Train, 164263, true);
                    }
                });

                /*AddTimedDelayedOperation(1100, [this]() -> void
                {
                    me->CastSpell(me, 164265, true);
                });*/
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_trainAI(p_Creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_operator_thogar()
{
    /// Boss
    new boss_operator_thogar();

    /// Creatures
    new npc_foundry_train();
}
#endif
