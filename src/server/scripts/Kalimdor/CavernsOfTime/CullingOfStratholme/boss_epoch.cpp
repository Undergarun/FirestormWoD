////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* Script Data Start
SDName: Boss epoch
SDAuthor: Tartalo
SD%Complete: 80
SDComment: TODO: Intro, consecutive attacks to a random target durin time wrap, adjust timers
SDCategory:
Script Data End */

#include "ScriptPCH.h"
#include "culling_of_stratholme.h"

enum Spells
{
    SPELL_CURSE_OF_EXERTION                     = 52772,
    SPELL_TIME_WARP                             = 52766, //Time slows down, reducing attack, casting and movement speed by 70% for 6 sec.
    SPELL_TIME_STOP                             = 58848, //Stops time in a 50 yard sphere for 2 sec.
    SPELL_WOUNDING_STRIKE                       = 52771, //Used only on the tank
    H_SPELL_WOUNDING_STRIKE                     = 58830
};

enum Yells
{
    SAY_INTRO                                   = -1595000, //"Prince Arthas Menethil, on this day, a powerful darkness has taken hold of your soul. The death you are destined to visit upon others will this day be your own."
    SAY_AGGRO                                   = -1595001, //"We'll see about that, young prince."
    SAY_TIME_WARP_1                             = -1595002, //"Tick tock, tick tock..."
    SAY_TIME_WARP_2                             = -1595003, //"Not quick enough!"
    SAY_TIME_WARP_3                             = -1595004, //"Let's get this over with. "
    SAY_SLAY_1                                  = -1595005, //"There is no future for you."
    SAY_SLAY_2                                  = -1595006, //"This is the hour of our greatest triumph!"
    SAY_SLAY_3                                  = -1595007, //"You were destined to fail. "
    SAY_DEATH                                   = -1595008 //"*gurgles*"
};

class boss_epoch : public CreatureScript
{
public:
    boss_epoch() : CreatureScript("boss_epoch") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_epochAI (creature);
    }

    struct boss_epochAI : public ScriptedAI
    {
        boss_epochAI(Creature* c) : ScriptedAI(c)
        {
            instance = c->GetInstanceScript();
        }

        uint8 uiStep;

        uint32 uiStepTimer;
        uint32 uiWoundingStrikeTimer;
        uint32 uiTimeWarpTimer;
        uint32 uiTimeStopTimer;
        uint32 uiCurseOfExertionTimer;

        InstanceScript* instance;

        void Reset()
        {
            uiStep = 1;
            uiStepTimer = 26000;
            uiCurseOfExertionTimer = 9300;
            uiTimeWarpTimer = 25300;
            uiTimeStopTimer = 21300;
            uiWoundingStrikeTimer = 5300;

            if (instance)
                instance->SetData(DATA_EPOCH_EVENT, NOT_STARTED);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if (instance)
                instance->SetData(DATA_EPOCH_EVENT, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiCurseOfExertionTimer < diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_CURSE_OF_EXERTION);
                uiCurseOfExertionTimer = 9300;
            } else uiCurseOfExertionTimer -= diff;

            if (uiWoundingStrikeTimer < diff)
            {
                DoCastVictim(SPELL_WOUNDING_STRIKE);
                uiWoundingStrikeTimer = 5300;
            } else uiWoundingStrikeTimer -= diff;

            if (uiTimeStopTimer < diff)
            {
                DoCastAOE(SPELL_TIME_STOP);
                uiTimeStopTimer = 21300;
            } else uiTimeStopTimer -= diff;

            if (uiTimeWarpTimer < diff)
            {
                DoScriptText(RAND(SAY_TIME_WARP_1, SAY_TIME_WARP_2, SAY_TIME_WARP_3), me);
                DoCastAOE(SPELL_TIME_WARP);
                uiTimeWarpTimer = 25300;
            } else uiTimeWarpTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
                instance->SetData(DATA_EPOCH_EVENT, DONE);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3), me);
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_epoch()
{
    new boss_epoch();
}
#endif
