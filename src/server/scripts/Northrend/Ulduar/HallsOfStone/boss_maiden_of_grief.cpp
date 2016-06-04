////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* Script Data Start
SDName: Boss maiden_of_grief
SDAuthor: LordVanMartin
SD%Complete:
SDComment:
SDCategory:
Script Data End */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "halls_of_stone.h"

enum Spells
{
    SPELL_PARTING_SORROW                          = 59723,
    SPELL_STORM_OF_GRIEF_N                        = 50752,
    SPELL_STORM_OF_GRIEF_H                        = 59772,
    SPELL_SHOCK_OF_SORROW_N                       = 50760,
    SPELL_SHOCK_OF_SORROW_H                       = 59726,
    SPELL_PILLAR_OF_WOE_N                         = 50761,
    SPELL_PILLAR_OF_WOE_H                         = 59727
};

enum Yells
{
    SAY_AGGRO                                     = -1599000,
    SAY_SLAY_1                                    = -1599001,
    SAY_SLAY_2                                    = -1599002,
    SAY_SLAY_3                                    = -1599003,
    SAY_SLAY_4                                    = -1599004,
    SAY_DEATH                                     = -1599005,
    SAY_STUN                                      = -1599006
};

enum Achievements
{
    ACHIEV_GOOD_GRIEF_START_EVENT                 = 20383
};

class boss_maiden_of_grief : public CreatureScript
{
public:
    boss_maiden_of_grief() : CreatureScript("boss_maiden_of_grief") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_maiden_of_griefAI (creature);
    }

    struct boss_maiden_of_griefAI : public ScriptedAI
    {
        boss_maiden_of_griefAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = me->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 PartingSorrowTimer;
        uint32 StormOfGriefTimer;
        uint32 ShockOfSorrowTimer;
        uint32 PillarOfWoeTimer;

        void Reset()
        {
            PartingSorrowTimer = urand(25000, 30000);
            StormOfGriefTimer = 10000;
            ShockOfSorrowTimer = 20000+rand()%5000;
            PillarOfWoeTimer = urand(5000, 15000);

            if (instance)
            {
                instance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, NOT_STARTED);
                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_GOOD_GRIEF_START_EVENT);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(SAY_AGGRO, me);

            if (instance)
            {
                if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_MAIDEN_DOOR)))
                    if (pDoor->GetGoState() == GO_STATE_READY)
                    {
                        EnterEvadeMode();
                        return;
                    }

                instance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, IN_PROGRESS);
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_GOOD_GRIEF_START_EVENT);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (IsHeroic())
            {
                if (PartingSorrowTimer <= diff)
                {
                    Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0);

                    if (target)
                        DoCast(target, SPELL_PARTING_SORROW);

                    PartingSorrowTimer = urand(30000, 40000);
                } else PartingSorrowTimer -= diff;
            }

            if (StormOfGriefTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_STORM_OF_GRIEF_N, true);
                StormOfGriefTimer = urand(15000, 20000);
            } else StormOfGriefTimer -= diff;

            if (ShockOfSorrowTimer <= diff)
            {
                DoResetThreat();
                DoScriptText(SAY_STUN, me);
                DoCast(me, SPELL_SHOCK_OF_SORROW_N);
                ShockOfSorrowTimer = urand(20000, 30000);
            } else ShockOfSorrowTimer -= diff;

            if (PillarOfWoeTimer <= diff)
            {
                Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 1);

                if (target)
                    DoCast(target, SPELL_PILLAR_OF_WOE_N);
                else
                    DoCast(me->getVictim(), SPELL_PILLAR_OF_WOE_N);

                PillarOfWoeTimer = urand(5000, 25000);
            } else PillarOfWoeTimer -= diff;

            DoMeleeAttackIfReady();
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEATH, me);

            if (instance)
                instance->SetData(DATA_MAIDEN_OF_GRIEF_EVENT, DONE);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim == me)
                return;

            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2, SAY_SLAY_3, SAY_SLAY_4), me);
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_maiden_of_grief()
{
    new boss_maiden_of_grief();
}
#endif
