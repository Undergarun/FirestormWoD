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

#define SAY_GREET           RAND(-1533000, -1533004, -1533005, -1533006, -1533007)
#define SAY_AGGRO           RAND(-1533001, -1533002, -1533003)
#define SAY_SLAY            -1533008

#define MOB_CRYPT_GUARD     16573

const Position GuardSummonPos = {3333.72f, -3476.30f, 287.1f, 6.2801f};

enum Events
{
    EVENT_NONE,
    EVENT_IMPALE,
    EVENT_LOCUST,
    EVENT_SPAWN_GUARDIAN_NORMAL,
    EVENT_BERSERK
};

enum Spells
{
    SPELL_IMPALE_10                 = 28783,
    SPELL_IMPALE_25                 = 56090,
    SPELL_LOCUST_SWARM_10           = 28785,
    SPELL_LOCUST_SWARM_25           = 54021,
    SPELL_SUMMON_CORPSE_SCARABS_PLR = 29105,    // This spawns 5 corpse scarabs on top of player
    SPELL_SUMMON_CORPSE_SCARABS_MOB = 28864,   // This spawns 10 corpse scarabs on top of dead guards
    SPELL_BERSERK                   = 27680
};

enum
{
    ACHIEV_TIMED_START_EVENT                      = 9891
};

class boss_anubrekhan : public CreatureScript
{
public:
    boss_anubrekhan() : CreatureScript("boss_anubrekhan") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_anubrekhanAI (creature);
    }

    struct boss_anubrekhanAI : public BossAI
    {
        boss_anubrekhanAI(Creature* creature) : BossAI(creature, BOSS_ANUBREKHAN) {}

        bool hasTaunted;

        void Reset()
        {
            _Reset();

            hasTaunted = false;

            if (GetDifficulty() == Difficulty::Difficulty25N)
            {
                Position pos;

                // respawn guard using home position,
                // otherwise, after a wipe, they respawn where boss was at wipe moment.
                pos = me->GetHomePosition();
                pos.m_positionY -= 10.0f;
                me->SummonCreature(MOB_CRYPT_GUARD, pos, TEMPSUMMON_CORPSE_DESPAWN);

                pos = me->GetHomePosition();
                pos.m_positionY += 10.0f;
                me->SummonCreature(MOB_CRYPT_GUARD, pos, TEMPSUMMON_CORPSE_DESPAWN);
            }
        }

        void KilledUnit(Unit* victim)
        {
            //Force the player to spawn corpse scarabs via spell, TODO: Check percent chance for scarabs, 20% at the moment
            if (!(rand()%5))
                if (victim->IsPlayer())
                    victim->CastSpell(victim, SPELL_SUMMON_CORPSE_SCARABS_PLR, true, NULL, NULL, me->GetGUID());

            DoScriptText(SAY_SLAY, me);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();

            // start achievement timer (kill Maexna within 20 min)
            if (instance)
                instance->DoStartTimedAchievement(ACHIEVEMENT_TIMED_TYPE_EVENT, ACHIEV_TIMED_START_EVENT);
        }
        void EnterCombat(Unit* /*who*/)
        {
            _EnterCombat();
            DoScriptText(SAY_AGGRO, me);
            events.ScheduleEvent(EVENT_IMPALE, urand(10000, 20000));
            events.ScheduleEvent(EVENT_LOCUST, 90000);
            events.ScheduleEvent(EVENT_BERSERK, 600000);

            if (GetDifficulty() == Difficulty::Difficulty10N)
                events.ScheduleEvent(EVENT_SPAWN_GUARDIAN_NORMAL, urand(15000, 20000));
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (!hasTaunted && me->IsWithinDistInMap(who, 60.0f) && who->IsPlayer())
            {
                DoScriptText(SAY_GREET, me);
                hasTaunted = true;
            }
            ScriptedAI::MoveInLineOfSight(who);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            BossAI::SummonedCreatureDespawn(summon);

            // check if it is an actual killed guard
            if (!me->isAlive() || summon->isAlive() || summon->GetEntry() != MOB_CRYPT_GUARD)
                return;

            summon->CastSpell(summon, SPELL_SUMMON_CORPSE_SCARABS_MOB, true, NULL, NULL, me->GetGUID());
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim() || !CheckInRoom())
                return;

            events.Update(diff);

            while (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_IMPALE:
                        //Cast Impale on a random target
                        //Do NOT cast it when we are afflicted by locust swarm
                        if (!me->HasAura(RAID_MODE(SPELL_LOCUST_SWARM_10, SPELL_LOCUST_SWARM_25)))
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                                DoCast(target, RAID_MODE(SPELL_IMPALE_10, SPELL_IMPALE_25));
                        events.ScheduleEvent(EVENT_IMPALE, urand(10000, 20000));
                        break;
                    case EVENT_LOCUST:
                        // TODO : Add Text
                        DoCast(me, RAID_MODE(SPELL_LOCUST_SWARM_10, SPELL_LOCUST_SWARM_25));
                        DoSummon(MOB_CRYPT_GUARD, GuardSummonPos, 0, TEMPSUMMON_CORPSE_DESPAWN);
                        events.ScheduleEvent(EVENT_LOCUST, 90000);
                        break;
                    case EVENT_SPAWN_GUARDIAN_NORMAL:
                        // TODO : Add Text
                        DoSummon(MOB_CRYPT_GUARD, GuardSummonPos, 0, TEMPSUMMON_CORPSE_DESPAWN);
                        break;
                    case EVENT_BERSERK:
                        DoCast(me, SPELL_BERSERK, true);
                        events.ScheduleEvent(EVENT_BERSERK, 600000);
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_anubrekhan()
{
    new boss_anubrekhan();
}
#endif
