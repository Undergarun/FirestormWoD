////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Magistrate_Barthilas
SD%Complete: 70
SDComment:
SDCategory: Stratholme
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "stratholme.h"

#define SPELL_DRAININGBLOW    16793
#define SPELL_CROWDPUMMEL    10887
#define SPELL_MIGHTYBLOW    14099
#define SPELL_FURIOUS_ANGER     16791

#define MODEL_NORMAL            10433
#define MODEL_HUMAN             3637

class boss_magistrate_barthilas : public CreatureScript
{
public:
    boss_magistrate_barthilas() : CreatureScript("boss_magistrate_barthilas") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_magistrate_barthilasAI (creature);
    }

    struct boss_magistrate_barthilasAI : public ScriptedAI
    {
        boss_magistrate_barthilasAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 DrainingBlow_Timer;
        uint32 CrowdPummel_Timer;
        uint32 MightyBlow_Timer;
        uint32 FuriousAnger_Timer;
        uint32 AngerCount;

        void Reset()
        {
            DrainingBlow_Timer = 20000;
            CrowdPummel_Timer = 15000;
            MightyBlow_Timer = 10000;
            FuriousAnger_Timer = 5000;
            AngerCount = 0;

            if (me->isAlive())
                me->SetDisplayId(MODEL_NORMAL);
            else
                me->SetDisplayId(MODEL_HUMAN);
        }

        void MoveInLineOfSight(Unit* who)
        {
            //nothing to see here yet

            ScriptedAI::MoveInLineOfSight(who);
        }

        void JustDied(Unit* /*killer*/)
        {
            me->SetDisplayId(MODEL_HUMAN);
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (FuriousAnger_Timer <= diff)
            {
                FuriousAnger_Timer = 4000;
                if (AngerCount > 25)
                    return;

                ++AngerCount;
                DoCast(me, SPELL_FURIOUS_ANGER, false);
            } else FuriousAnger_Timer -= diff;

            //DrainingBlow
            if (DrainingBlow_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_DRAININGBLOW);
                DrainingBlow_Timer = 15000;
            } else DrainingBlow_Timer -= diff;

            //CrowdPummel
            if (CrowdPummel_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CROWDPUMMEL);
                CrowdPummel_Timer = 15000;
            } else CrowdPummel_Timer -= diff;

            //MightyBlow
            if (MightyBlow_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_MIGHTYBLOW);
                MightyBlow_Timer = 20000;
            } else MightyBlow_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_magistrate_barthilas()
{
    new boss_magistrate_barthilas();
}
#endif
