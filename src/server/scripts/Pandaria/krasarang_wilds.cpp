#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "krasarang_wilds.h"

class mob_arness_the_scale : public CreatureScript
{
    public:
        mob_arness_the_scale() : CreatureScript("mob_arness_the_scale")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_arness_the_scaleAI(creature);
        }

        struct mob_arness_the_scaleAI : public ScriptedAI
        {
            mob_arness_the_scaleAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GRAPPLING_HOOK,  40000);
                events.ScheduleEvent(EVENT_VANISH, 20000);
                events.ScheduleEvent(EVENT_VICIOUS_REND, 15000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_GRAPPLING_HOOK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GRAPPLING_HOOK, false);
                            events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 40000);
                            break;
                        case EVENT_VANISH:
                            me->CastSpell(me, SPELL_VANISH, false);
                            events.ScheduleEvent(EVENT_VANISH, 40000);
                            break;
                        case EVENT_VICIOUS_REND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VICIOUS_REND, false);
                            events.ScheduleEvent(EVENT_VICIOUS_REND, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_gaarn_the_toxic : public CreatureScript
{
    public:
        mob_gaarn_the_toxic() : CreatureScript("mob_gaarn_the_toxic")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gaarn_the_toxicAI(creature);
        }

        struct mob_gaarn_the_toxicAI : public ScriptedAI
        {
            mob_gaarn_the_toxicAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOWBOLT, 15000);
                events.ScheduleEvent(EVENT_VOIDCLOUD, 70000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SHADOWBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOWBOLT, false);
                            events.ScheduleEvent(EVENT_SHADOWBOLT, 15000);
                            break;
                        case EVENT_VOIDCLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VOIDCLOUD, false);
                            events.ScheduleEvent(EVENT_VOIDCLOUD, 70000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_qu_nas : public CreatureScript
{
    public:
        mob_qu_nas() : CreatureScript("mob_qu_nas")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_qu_nasAI(creature);
        }

        struct mob_qu_nasAI : public ScriptedAI
        {
            mob_qu_nasAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEVASTATING_ARC, 25000);
                events.ScheduleEvent(EVENT_SUMMON_QUILEN, 15000);
                events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 70000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_DEVASTATING_ARC:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEVASTATING_ARC, false);
                            events.ScheduleEvent(EVENT_DEVASTATING_ARC, 30000);
                            break;
                        case EVENT_SUMMON_QUILEN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SUMMON_QUILEN, false);
                            events.ScheduleEvent(EVENT_SUMMON_QUILEN, 80000);
                            break;
                        case EVENT_TITANIC_STRENGTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TITANIC_STRENGTH, false);
                            events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 55000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_torik_ethis : public CreatureScript
{
    public:
        mob_torik_ethis() : CreatureScript("mob_torik_ethis")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_torik_ethisAI(creature);
        }

        struct mob_torik_ethisAI : public ScriptedAI
        {
            mob_torik_ethisAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLADE_FURY,       8000);
                events.ScheduleEvent(EVENT_TORNADO,         40000);
                events.ScheduleEvent(EVENT_WINDSONG,        32000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == MOB_TORNADO)
                {
                    summon->AddAura(SPELL_TORNADO_DMG, summon);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->GetMotionMaster()->MoveRandom(20.0f);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BLADE_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLADE_FURY, false);
                            events.ScheduleEvent(EVENT_BLADE_FURY, 8000);
                            break;
                        case EVENT_TORNADO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORNADO, false);
                            events.ScheduleEvent(EVENT_TORNADO, 40000);
                            break;
                        case EVENT_WINDSONG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WINDSONG, false);
                            events.ScheduleEvent(EVENT_WINDSONG, 32000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

#define GO_KAN_SAY_1        "Feel the strength of the Yaungol!"

class mob_go_kan : public CreatureScript
{
    public:
        mob_go_kan() : CreatureScript("mob_go_kan")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_go_kanAI(creature);
        }

        struct mob_go_kanAI : public ScriptedAI
        {
            mob_go_kanAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_SIT);
                events.Reset();
                events.ScheduleEvent(EVENT_BELLOWING_RAGE,       8000);
                events.ScheduleEvent(EVENT_RUSHING_CHARGE,      17000);
                events.ScheduleEvent(EVENT_YAUNGOL_STOMP,       25000);
            }

            void EnterCombat(Unit* who)
            {
                me->RemoveFlag(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_SIT);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_BELLOWING_RAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BELLOWING_RAGE, false);
                            events.ScheduleEvent(EVENT_BELLOWING_RAGE, 35000);
                            break;
                        case EVENT_RUSHING_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RUSHING_CHARGE, false);
                            events.ScheduleEvent(EVENT_RUSHING_CHARGE, 35000);
                            break;
                        case EVENT_YAUNGOL_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_YAUNGOL_STOMP, false);
                            events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 35000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_spirit_of_the_crane : public CreatureScript
{
    public:
        mob_spirit_of_the_crane() : CreatureScript("mob_spirit_of_the_crane")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(30716) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(60487);
                creature->DespawnOrUnsummon();
            }

            return true;
        }
};

void AddSC_krasarang_wilds()
{
    new mob_gaarn_the_toxic();
    new mob_arness_the_scale();
    new mob_qu_nas();
    new mob_torik_ethis();
    new mob_go_kan();
    new mob_spirit_of_the_crane();
}