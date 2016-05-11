////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_instructormalicia
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scholomance.h"

#define SPELL_CALLOFGRAVES         17831
#define SPELL_CORRUPTION           11672
#define SPELL_FLASHHEAL            10917
#define SPELL_RENEW                10929
#define SPELL_HEALINGTOUCH         9889

class boss_instructor_malicia : public CreatureScript
{
public:
    boss_instructor_malicia() : CreatureScript("boss_instructor_malicia") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_instructormaliciaAI (creature);
    }

    struct boss_instructormaliciaAI : public ScriptedAI
    {
        boss_instructormaliciaAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 CallOfGraves_Timer;
        uint32 Corruption_Timer;
        uint32 FlashHeal_Timer;
        uint32 Renew_Timer;
        uint32 HealingTouch_Timer;
        uint32 FlashCounter;
        uint32 TouchCounter;

        void Reset()
        {
            CallOfGraves_Timer = 4000;
            Corruption_Timer = 8000;
            FlashHeal_Timer = 38000;
            Renew_Timer = 32000;
            HealingTouch_Timer = 45000;
            FlashCounter = 0;
            TouchCounter = 0;
        }

        void JustDied(Unit* /*killer*/)
        {
            InstanceScript* instance = me->GetInstanceScript();
            if (instance)
            {
                instance->SetData(DATA_INSTRUCTORMALICIA_DEATH, 0);

                if (instance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                    me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //CallOfGraves_Timer
            if (CallOfGraves_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CALLOFGRAVES);
                CallOfGraves_Timer = 65000;
            } else CallOfGraves_Timer -= diff;

            //Corruption_Timer
            if (Corruption_Timer <= diff)
            {
                Unit* target = NULL;
                target = SelectTarget(SELECT_TARGET_RANDOM, 0);
                if (target) DoCast(target, SPELL_CORRUPTION);

                Corruption_Timer = 24000;
            } else Corruption_Timer -= diff;

            //Renew_Timer
            if (Renew_Timer <= diff)
            {
                DoCast(me, SPELL_RENEW);
                Renew_Timer = 10000;
            } else Renew_Timer -= diff;

            //FlashHeal_Timer
            if (FlashHeal_Timer <= diff)
            {
                DoCast(me, SPELL_FLASHHEAL);

                //5 Flashheals will be casted
                if (FlashCounter < 2)
                {
                    FlashHeal_Timer = 5000;
                    ++FlashCounter;
                }
                else
                {
                    FlashCounter=0;
                    FlashHeal_Timer = 30000;
                }
            } else FlashHeal_Timer -= diff;

            //HealingTouch_Timer
            if (HealingTouch_Timer <= diff)
            {
                DoCast(me, SPELL_HEALINGTOUCH);

                //3 Healingtouchs will be casted
                if (HealingTouch_Timer < 2)
                {
                    HealingTouch_Timer = 5500;
                    ++TouchCounter;
                }
                else
                {
                    TouchCounter=0;
                    HealingTouch_Timer = 30000;
                }
            } else HealingTouch_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_instructormalicia()
{
    new boss_instructor_malicia();
}
#endif
