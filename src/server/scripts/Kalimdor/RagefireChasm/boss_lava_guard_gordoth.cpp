
#include "ScriptPCH.h"
#include "ragefire_chasm.h"

enum Spells
{
    SPELL_ENRAGE            = 50420,
    SPELL_GROUND_RUPTURE    = 119999,
    SPELL_SEISMIC_SLAM      = 120024,
};

enum Events
{
    EVENT_GROUND_RUPTURE    = 1,
    EVENT_SEISMIC_SLAM,
};

class boss_lava_guard_gordoth : public CreatureScript
{
public:
    boss_lava_guard_gordoth() : CreatureScript("boss_lava_guard_gordoth") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_lava_guard_gordothAI (creature);
    }

    struct boss_lava_guard_gordothAI : public BossAI
    {
        boss_lava_guard_gordothAI(Creature* creature) : BossAI(creature, DATA_LAVA_GUARD_GORDOTH)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK_DEST, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);

            me->setActive(true);
        }

        void Reset()
        {
            _Reset();

            isEnrage = false;
        }

        void EnterCombat(Unit* who)
        {
            events.ScheduleEvent(EVENT_GROUND_RUPTURE, urand(5000, 7000));
            events.ScheduleEvent(EVENT_SEISMIC_SLAM, urand(7000, 10000));

            DoZoneInCombat();
            instance->SetBossState(DATA_LAVA_GUARD_GORDOTH, IN_PROGRESS);
        }

        void JustDied(Unit* /*killer*/)
        {
            _JustDied();

            instance->DoUpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE, 11520, 1, 0, me);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(diff);

            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;

            if (me->GetHealthPct() <= 30.0f && !isEnrage)
            {
                isEnrage = true;
                DoCast(me, SPELL_ENRAGE);
                return;
            }

            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_GROUND_RUPTURE:
                        DoCastVictim(SPELL_GROUND_RUPTURE);
                        events.ScheduleEvent(EVENT_GROUND_RUPTURE, urand(7000, 10000));
                        break;
                    case EVENT_SEISMIC_SLAM:
                        DoCast(me, SPELL_SEISMIC_SLAM);
                        events.ScheduleEvent(EVENT_SEISMIC_SLAM, urand(10000, 15000));
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }

    private:
        bool isEnrage;
    };
};

void AddSC_boss_lava_guard_gordoth()
{
    new boss_lava_guard_gordoth();
}
