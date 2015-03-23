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

        struct boss_the_butcherAI : public BossAI
        {
            boss_the_butcherAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTheButcher) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                _Reset();
            }

            void EnterCombat(Unit* p_Attacker)
            {
                _EnterCombat();

            }

            void JustDied(Unit* p_Killer)
            {
                _JustDied();
            }

            void DoAction(int32 const p_Action)
            {
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                /*switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_the_butcherAI(p_Creature);
        }
};

void AddSC_boss_the_butcher()
{
    /// Boss
    new boss_the_butcher();

    /// NPCs

    /// Spells
}