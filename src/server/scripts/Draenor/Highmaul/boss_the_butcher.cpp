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
            HeavyHandedAura = 156135,
            HeavyHandedProc = 156138,

            TheCleaverDmg   = 156143,
            TheCleaverDoT   = 156147,

            TheTenderizer   = 156151,

            MeatHook        = 156125,
            MeetHookJump    = 156127
        };

        struct boss_the_butcherAI : public BossAI
        {
            boss_the_butcherAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTheButcher) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();
            }

            void DoAction(int32 const p_Action) override
            {
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
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

        CreatureAI* GetAI(Creature* p_Creature) const override
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