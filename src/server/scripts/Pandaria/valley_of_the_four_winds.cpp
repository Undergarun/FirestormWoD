#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "valley_of_the_four_winds.h"

class mob_bonobos : public CreatureScript
{
    public:
        mob_bonobos() : CreatureScript("mob_bonobos")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_bonobosAI(creature);
        }

        struct mob_bonobosAI : public ScriptedAI
        {
            mob_bonobosAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GOING_BANANAS, 12000);
                events.ScheduleEvent(EVENT_BANANARANG, 8000);
                events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
            }

            void JustSummoned(Creature* summon)
            {
                summon->DespawnOrUnsummon(12000);
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
                        case EVENT_GOING_BANANAS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GOING_BANANAS, false);
                            events.ScheduleEvent(EVENT_GOING_BANANAS, 10000);
                            break;
                        case EVENT_BANANARANG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BANANARANG, false);
                            events.ScheduleEvent(EVENT_BANANARANG, 20000);
                            break;
                        case EVENT_TOSS_FILTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TOSS_FILTH, false);
                            events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_sele_na : public CreatureScript
{
    public:
        mob_sele_na() : CreatureScript("mob_sele_na")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sele_naAI(creature);
        }

        struct mob_sele_naAI : public ScriptedAI
        {
            mob_sele_naAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_RAIN_DANCE, 5000);
                events.ScheduleEvent(EVENT_TORRENT, 15000);
                events.ScheduleEvent(EVENT_WATER_BOLT, 25000);
            }

            void JustSummoned(Creature* summon)
            {
                summon->DespawnOrUnsummon(12000);
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
                        case EVENT_RAIN_DANCE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RAIN_DANCE, false);
                            events.ScheduleEvent(EVENT_RAIN_DANCE, 5000);
                            break;
                        case EVENT_TORRENT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORRENT, false);
                            events.ScheduleEvent(EVENT_TORRENT, 15000);
                            break;
                        case EVENT_WATER_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WATER_BOLT, false);
                            events.ScheduleEvent(EVENT_WATER_BOLT, 25000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_blackhoof : public CreatureScript
{
    public:
        mob_blackhoof() : CreatureScript("mob_blackhoof")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_blackhoofAI(creature);
        }

        struct mob_blackhoofAI : public ScriptedAI
        {
            mob_blackhoofAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_RUSHING_RAGE, 5000);
                events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 15000);
                events.ScheduleEvent(EVENT_BELLOWING_RAGE, 25000);
            }

            void JustSummoned(Creature* summon)
            {
                summon->DespawnOrUnsummon(12000);
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
                            events.ScheduleEvent(EVENT_BELLOWING_RAGE, 25000);
                            break;
                        case EVENT_RUSHING_RAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RUSHING_RAGE, false);
                            events.ScheduleEvent(EVENT_RUSHING_RAGE, 5000);
                            break;
                        case EVENT_YAUNGOL_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_YAUNGOL_STOMP, false);
                            events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_ik_thik_warrior : public CreatureScript
{
    public:
        mob_ik_thik_warrior() : CreatureScript("mob_ik_thik_warrior")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ik_thik_warriorAI(creature);
        }

        struct mob_ik_thik_warriorAI : public ScriptedAI
        {
            mob_ik_thik_warriorAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_PIERCE_ARMOR, 5000);
                events.ScheduleEvent(EVENT_SHOCK_AND_AWE, 15000);
            }

            void JustSummoned(Creature* summon)
            {
                summon->DespawnOrUnsummon(12000);
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
                        case EVENT_PIERCE_ARMOR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_PIERCE_ARMOR, false);
                            events.ScheduleEvent(EVENT_PIERCE_ARMOR, 25000);
                            break;
                        case EVENT_SHOCK_AND_AWE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHOCK_AND_AWE, false);
                            events.ScheduleEvent(EVENT_SHOCK_AND_AWE, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_sheepie : public CreatureScript
{
public:
    mob_sheepie() : CreatureScript("mob_sheepie")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_OH_SHEEPIE) == QUEST_STATUS_INCOMPLETE)
            player->KilledMonsterCredit(NPC_SHEEPIE);

        return true;
    }
};

class mob_krungko_fingerlicker : public CreatureScript
{
    public:
        mob_krungko_fingerlicker() : CreatureScript("mob_krungko_fingerlicker")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_krungko_fingerlickerAI(creature);
        }

        struct mob_krungko_fingerlickerAI : public ScriptedAI
        {
            mob_krungko_fingerlickerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HOZEN_RAGE, 5000);
                events.ScheduleEvent(EVENT_HOZEN_STRIKE, 15000);
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
                        case EVENT_HOZEN_RAGE:
                                me->CastSpell(me, SPELL_HOZEN_RAGE, false);
                            events.ScheduleEvent(EVENT_HOZEN_RAGE, 25000);
                            break;
                        case EVENT_HOZEN_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HOZEN_STRIKE, false);
                            events.ScheduleEvent(EVENT_HOZEN_STRIKE, 25000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_jim_jim : public CreatureScript
{
    public:
        mob_jim_jim() : CreatureScript("mob_jim_jim")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_jim_jimAI(creature);
        }

        struct mob_jim_jimAI : public ScriptedAI
        {
            mob_jim_jimAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HOZEN_RAGE, 5000);
                events.ScheduleEvent(EVENT_CLUB, 15000);
                events.ScheduleEvent(EVENT_FORCEFUL_THROW, 22000);
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
                        case EVENT_HOZEN_RAGE:
                                me->CastSpell(me, SPELL_HOZEN_RAGE, false);
                            events.ScheduleEvent(EVENT_HOZEN_RAGE, 28000);
                            break;
                        case EVENT_CLUB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLUB, false);
                            events.ScheduleEvent(EVENT_CLUB, 28000);
                            break;
                        case EVENT_FORCEFUL_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCEFUL_THROW, false);
                            events.ScheduleEvent(EVENT_FORCEFUL_THROW, 28000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_rit_rit : public CreatureScript
{
    public:
        mob_rit_rit() : CreatureScript("mob_rit_rit")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_rit_ritAI(creature);
        }

        struct mob_rit_ritAI : public ScriptedAI
        {
            mob_rit_ritAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_FIREWALL, 5000);
                events.ScheduleEvent(EVENT_FROSTBOLT, 11000);
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
                        case EVENT_FIREWALL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FIREWALL, false);
                            events.ScheduleEvent(EVENT_FIREWALL, 15000);
                            break;
                        case EVENT_FROSTBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FROSTBOLT, false);
                            events.ScheduleEvent(EVENT_FROSTBOLT, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_kunzen_soupmaster : public CreatureScript
{
    public:
        mob_kunzen_soupmaster() : CreatureScript("mob_kunzen_soupmaster")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kunzen_soupmasterAI(creature);
        }

        struct mob_kunzen_soupmasterAI : public ScriptedAI
        {
            mob_kunzen_soupmasterAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_OOKED_IN_THE_DOOK, 5000);
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
                    if (eventId == EVENT_OOKED_IN_THE_DOOK)
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_OOKED_IN_THE_DOOK, false);
                        events.ScheduleEvent(EVENT_OOKED_IN_THE_DOOK, 15000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_maaka : public CreatureScript
{
    public:
        mob_maaka() : CreatureScript("mob_maaka")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_maakaAI(creature);
        }

        struct mob_maakaAI : public ScriptedAI
        {
            mob_maakaAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HOZEN_RAGE, 5000);
                events.ScheduleEvent(EVENT_CLUB, 15000);
                events.ScheduleEvent(EVENT_FORCEFUL_THROW, 22000);
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
                        case EVENT_HOZEN_RAGE:
                                me->CastSpell(me, SPELL_HOZEN_RAGE, false);
                            events.ScheduleEvent(EVENT_HOZEN_RAGE, 28000);
                            break;
                        case EVENT_CLUB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLUB, false);
                            events.ScheduleEvent(EVENT_CLUB, 28000);
                            break;
                        case EVENT_FORCEFUL_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCEFUL_THROW, false);
                            events.ScheduleEvent(EVENT_FORCEFUL_THROW, 28000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_teeku : public CreatureScript
{
    public:
        mob_teeku() : CreatureScript("mob_teeku")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_teekuAI(creature);
        }

        struct mob_teekuAI : public ScriptedAI
        {
            mob_teekuAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HOZEN_RAGE, 5000);
                events.ScheduleEvent(EVENT_CLUB, 15000);
                events.ScheduleEvent(EVENT_FORCEFUL_THROW, 22000);
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
                        case EVENT_HOZEN_RAGE:
                                me->CastSpell(me, SPELL_HOZEN_RAGE, false);
                            events.ScheduleEvent(EVENT_HOZEN_RAGE, 28000);
                            break;
                        case EVENT_CLUB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLUB, false);
                            events.ScheduleEvent(EVENT_CLUB, 28000);
                            break;
                        case EVENT_FORCEFUL_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCEFUL_THROW, false);
                            events.ScheduleEvent(EVENT_FORCEFUL_THROW, 28000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_kon_kon : public CreatureScript
{
    public:
        mob_kon_kon() : CreatureScript("mob_kon_kon")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kon_konAI(creature);
        }

        struct mob_kon_konAI : public ScriptedAI
        {
            mob_kon_konAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_HOZEN_RAGE, 5000);
                events.ScheduleEvent(EVENT_CLUB, 15000);
                events.ScheduleEvent(EVENT_FORCEFUL_THROW, 22000);
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
                        case EVENT_HOZEN_RAGE:
                                me->CastSpell(me, SPELL_HOZEN_RAGE, false);
                            events.ScheduleEvent(EVENT_HOZEN_RAGE, 28000);
                            break;
                        case EVENT_CLUB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLUB, false);
                            events.ScheduleEvent(EVENT_CLUB, 28000);
                            break;
                        case EVENT_FORCEFUL_THROW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCEFUL_THROW, false);
                            events.ScheduleEvent(EVENT_FORCEFUL_THROW, 28000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_second_sheepie : public CreatureScript
{
public:
    mob_second_sheepie() : CreatureScript("mob_second_sheepie")
    {
    }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_LOST_SHEEPIE) == QUEST_STATUS_INCOMPLETE)
            player->AddItem(ITEM_SHEEPIE, 1);

        if (player->GetQuestStatus(QUEST_LOST_SHEEPIE_AGAIN) == QUEST_STATUS_INCOMPLETE)
            player->AddItem(ITEM_SHEEPIE, 1);

        return true;
    }
};

void AddSC_valley_of_the_four_winds()
{
    // Rare Mobs
    new mob_bonobos();
    new mob_sele_na();
    new mob_blackhoof();
    // Standard Mobs
    new mob_ik_thik_warrior();
    new mob_sheepie();
    new mob_krungko_fingerlicker();
    new mob_jim_jim();
    new mob_rit_rit();
    new mob_kunzen_soupmaster();
    new mob_maaka();
    new mob_teeku();
    new mob_kon_kon();
    new mob_second_sheepie();
}