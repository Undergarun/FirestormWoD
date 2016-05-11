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

#define SPELL_MORTAL_WOUND      25646
#define SPELL_ENRAGE            RAID_MODE(28371, 54427)
#define SPELL_DECIMATE          RAID_MODE(28374, 54426)
#define SPELL_BERSERK           26662
#define SPELL_INFECTED_WOUND    29306

#define MOB_ZOMBIE  16360

const Position PosSummon[3] =
{
    {3267.9f, -3172.1f, 297.42f, 0.94f},
    {3253.2f, -3132.3f, 297.42f, 0},
    {3308.3f, -3185.8f, 297.42f, 1.58f}
};

enum Events
{
    EVENT_NONE,
    EVENT_WOUND,
    EVENT_ENRAGE,
    EVENT_DECIMATE,
    EVENT_BERSERK,
    EVENT_SUMMON
};

#define EMOTE_NEARBY    " spots a nearby zombie to devour!"

class boss_gluth : public CreatureScript
{
public:
    boss_gluth() : CreatureScript("boss_gluth") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_gluthAI (creature);
    }

    struct boss_gluthAI : public BossAI
    {
        boss_gluthAI(Creature* creature) : BossAI(creature, BOSS_GLUTH)
        {
            // Do not let Gluth be affected by zombies' debuff
            me->ApplySpellImmune(0, IMMUNITY_ID, SPELL_INFECTED_WOUND, true);
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetEntry() == MOB_ZOMBIE && me->IsWithinDistInMap(who, 7))
            {
                SetGazeOn(who);
                // TODO: use a script text
                me->MonsterTextEmote(EMOTE_NEARBY, 0, true);
            }
            else
                BossAI::MoveInLineOfSight(who);
        }

        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
            events.ScheduleEvent(EVENT_WOUND, 10000);
            events.ScheduleEvent(EVENT_ENRAGE, 15000);
            events.ScheduleEvent(EVENT_DECIMATE, 105000);
            events.ScheduleEvent(EVENT_BERSERK, 8*60000);
            events.ScheduleEvent(EVENT_SUMMON, 15000);
        }

        void JustSummoned(Creature* summon)
        {
            if (summon->GetEntry() == MOB_ZOMBIE)
                summon->AI()->AttackStart(me);
            summons.Summon(summon);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictimWithGaze() || !CheckInRoom())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_WOUND:
                        DoCast(me->getVictim(), SPELL_MORTAL_WOUND);
                        events.ScheduleEvent(EVENT_WOUND, 10000);
                        break;
                    case EVENT_ENRAGE:
                        // TODO : Add missing text
                        DoCast(me, SPELL_ENRAGE);
                        events.ScheduleEvent(EVENT_ENRAGE, 15000);
                        break;
                    case EVENT_DECIMATE:
                        // TODO : Add missing text
                        DoCastAOE(SPELL_DECIMATE);
                        events.ScheduleEvent(EVENT_DECIMATE, 105000);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK);
                        events.ScheduleEvent(EVENT_BERSERK, 5*60000);
                        break;
                    case EVENT_SUMMON:
                        for (int32 i = 0; i < RAID_MODE(1, 2); ++i)
                            DoSummon(MOB_ZOMBIE, PosSummon[rand() % RAID_MODE(1, 3)]);
                        events.ScheduleEvent(EVENT_SUMMON, 10000);
                        break;
                }
            }

            if (me->getVictim() && me->getVictim()->GetEntry() == MOB_ZOMBIE)
            {
                if (me->IsWithinMeleeRange(me->getVictim()))
                {
                    me->Kill(me->getVictim());
                    me->ModifyHealth(int32(me->CountPctFromMaxHealth(5)));
                }
            }
            else
                DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_gluth()
{
    new boss_gluth();
}
#endif
