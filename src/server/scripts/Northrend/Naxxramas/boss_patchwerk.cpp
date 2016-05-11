////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "naxxramas.h"

enum Spells
{
    SPELL_HATEFUL_STRIKE                        = 41926,
    H_SPELL_HATEFUL_STRIKE                      = 59192,
    SPELL_FRENZY                                = 28131,
    SPELL_BERSERK                               = 26662,
    SPELL_SLIME_BOLT                            = 32309
};

enum Yells
{
    SAY_AGGRO_1                                 = -1533017,
    SAY_AGGRO_2                                 = -1533018,
    SAY_SLAY                                    = -1533019,
    SAY_DEATH                                   = -1533020,
    EMOTE_BERSERK                               = -1533021,
    EMOTE_ENRAGE                                = -1533022
};

enum Events
{
    EVENT_NONE,
    EVENT_BERSERK,
    EVENT_HATEFUL,
    EVENT_SLIME
};

enum
{
    ACHIEV_MAKE_QUICK_WERK_OF_HIM_STARTING_EVENT  = 10286
};

class boss_patchwerk : public CreatureScript
{
public:
    boss_patchwerk() : CreatureScript("boss_patchwerk") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_patchwerkAI (creature);
    }

    struct boss_patchwerkAI : public BossAI
    {
        boss_patchwerkAI(Creature* creature) : BossAI(creature, BOSS_PATCHWERK) {}

        bool Enraged;

        void Reset()
        {
            _Reset();

            if (instance)
                instance->DoStopTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_MAKE_QUICK_WERK_OF_HIM_STARTING_EVENT);
        }

        void KilledUnit(Unit* /*Victim*/)
        {
            if (!(rand()%5))
                DoScriptText(SAY_SLAY, me);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();
            DoScriptText(SAY_DEATH, me);
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
            Enraged = false;
            DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2), me);
            events.ScheduleEvent(EVENT_HATEFUL, 1000);
            events.ScheduleEvent(EVENT_BERSERK, 360000);

            if (instance)
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_MAKE_QUICK_WERK_OF_HIM_STARTING_EVENT);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_HATEFUL:
                    {
                        //Cast Hateful strike on the player with the highest
                        //amount of HP within melee distance
                        uint32 MostHP = 0;
                        Unit* pMostHPTarget = NULL;
                        std::list<HostileReference*>::const_iterator i = me->getThreatManager().getThreatList().begin();
                        for (; i != me->getThreatManager().getThreatList().end(); ++i)
                        {
                            Unit* target = (*i)->getTarget();
                            if (target->isAlive() && target != me->getVictim() && target->GetHealth() > MostHP && me->IsWithinMeleeRange(target))
                            {
                                MostHP = target->GetHealth();
                                pMostHPTarget = target;
                            }
                        }

                        if (!pMostHPTarget)
                            pMostHPTarget = me->getVictim();

                        DoCast(pMostHPTarget, RAID_MODE(SPELL_HATEFUL_STRIKE, H_SPELL_HATEFUL_STRIKE), true);

                        events.ScheduleEvent(EVENT_HATEFUL, 1000);
                        break;
                    }
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        DoScriptText(EMOTE_BERSERK, me);
                        events.ScheduleEvent(EVENT_SLIME, 2000);
                        break;
                    case EVENT_SLIME:
                        DoCast(me->getVictim(), SPELL_SLIME_BOLT, true);
                        events.ScheduleEvent(EVENT_SLIME, 2000);
                        break;
                }
            }

            if (!Enraged && HealthBelowPct(5))
            {
                DoCast(me, SPELL_FRENZY, true);
                DoScriptText(EMOTE_ENRAGE, me);
                Enraged = true;
            }

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_patchwerk()
{
    new boss_patchwerk();
}
#endif
