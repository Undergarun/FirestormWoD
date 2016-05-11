////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Watchkeeper_Gargolmar
SD%Complete: 80
SDComment: Missing adds to heal him. Surge should be used on target furthest away, not random.
SDCategory: Hellfire Citadel, Hellfire Ramparts
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSays
{
    SAY_TAUNT              = -1543000,
    SAY_HEAL               = -1543001,
    SAY_SURGE              = -1543002,
    SAY_AGGRO_1            = -1543003,
    SAY_AGGRO_2            = -1543004,
    SAY_AGGRO_3            = -1543005,
    SAY_KILL_1             = -1543006,
    SAY_KILL_2             = -1543007,
    SAY_DIE                = -1543008
};

enum eSpells
{
    SPELL_MORTAL_WOUND     = 30641,
    H_SPELL_MORTAL_WOUND   = 36814,
    SPELL_SURGE            = 34645,
    SPELL_RETALIATION      = 22857
};

class boss_watchkeeper_gargolmar : public CreatureScript
{
    public:

        boss_watchkeeper_gargolmar()
            : CreatureScript("boss_watchkeeper_gargolmar")
        {
        }

        struct boss_watchkeeper_gargolmarAI : public ScriptedAI
        {
            boss_watchkeeper_gargolmarAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            uint32 Surge_Timer;
            uint32 MortalWound_Timer;
            uint32 Retaliation_Timer;

            bool HasTaunted;
            bool YelledForHeal;

            void Reset()
            {
                Surge_Timer = 5000;
                MortalWound_Timer = 4000;
                Retaliation_Timer = 0;

                HasTaunted = false;
                YelledForHeal = false;
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3), me);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!me->getVictim() && me->canCreatureAttack(who))
                {
                    if (!me->CanFly() && me->GetDistanceZ(who) > CREATURE_Z_ATTACK_RANGE)
                        return;

                    float attackRadius = me->GetAttackDistance(who);
                    if (me->IsWithinDistInMap(who, attackRadius) && me->IsWithinLOSInMap(who))
                    {
                        //who->RemoveSpellsCausingAura(SPELL_AURA_MOD_STEALTH);
                        AttackStart(who);
                    }
                    else if (!HasTaunted && me->IsWithinDistInMap(who, 60.0f))
                    {
                        DoScriptText(SAY_TAUNT, me);
                        HasTaunted = true;
                    }
                }
            }

            void KilledUnit(Unit* /*victim*/)
            {
                DoScriptText(RAND(SAY_KILL_1, SAY_KILL_2), me);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoScriptText(SAY_DIE, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (MortalWound_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_MORTAL_WOUND);
                    MortalWound_Timer = 5000+rand()%8000;
                }
                else
                    MortalWound_Timer -= diff;

                if (Surge_Timer <= diff)
                {
                    DoScriptText(SAY_SURGE, me);

                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_SURGE);

                    Surge_Timer = 5000+rand()%8000;
                }
                else
                    Surge_Timer -= diff;

                if (HealthBelowPct(20))
                {
                    if (Retaliation_Timer <= diff)
                    {
                        DoCast(me, SPELL_RETALIATION);
                        Retaliation_Timer = 30000;
                    }
                    else
                        Retaliation_Timer -= diff;
                }

                if (!YelledForHeal)
                {
                    if (HealthBelowPct(40))
                    {
                        DoScriptText(SAY_HEAL, me);
                        YelledForHeal = true;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_watchkeeper_gargolmarAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_watchkeeper_gargolmar()
{
    new boss_watchkeeper_gargolmar();
}
#endif
