////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "townlong_steppes.h"

// Kah'tir - 50355
class mob_kah_tir : public CreatureScript
{
    public:
        mob_kah_tir() : CreatureScript("mob_kah_tir")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kah_tirAI(creature);
        }

        struct mob_kah_tirAI : public ScriptedAI
        {
            mob_kah_tirAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEVASTATING_ARC, 40000);
                events.ScheduleEvent(EVENT_SUMMON_QUILEN, 12000);
                events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 20000);
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
                            events.ScheduleEvent(EVENT_DEVASTATING_ARC, 60000);
                            break;
                        case EVENT_SUMMON_QUILEN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SUMMON_QUILEN, false);
                            events.ScheduleEvent(EVENT_SUMMON_QUILEN, 50000);
                            break;
                        case EVENT_TITANIC_STRENGTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TITANIC_STRENGTH, false);
                            events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Lith'ik the Stalker - 50734
class mob_lith_ik : public CreatureScript
{
    public:
        mob_lith_ik() : CreatureScript("mob_lith_ik")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lith_ikAI(creature);
        }

        struct mob_lith_ikAI : public ScriptedAI
        {
            mob_lith_ikAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_TORNADO, 5000);
                events.ScheduleEvent(EVENT_BLADE_FURY, 25000);
                events.ScheduleEvent(EVENT_WINDSONG, 30000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == 64267)
                {
                    summon->DespawnOrUnsummon(15000);
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
                        case EVENT_TORNADO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORNADO, false);
                            events.ScheduleEvent(EVENT_TORNADO, 70000);
                            break;
                        case EVENT_BLADE_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLADE_FURY, false);
                            events.ScheduleEvent(EVENT_BLADE_FURY, 30000);
                            break;
                        case EVENT_WINDSONG:
                            me->CastSpell(me, SPELL_WINDSONG, false);
                            events.ScheduleEvent(EVENT_WINDSONG, 25000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Darkwoods Faerie - 62522
class mob_darkwoods_faerie : public CreatureScript
{
    public:
        mob_darkwoods_faerie() : CreatureScript("mob_darkwoods_faerie")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_darkwoods_faerieAI(creature);
        }

        struct mob_darkwoods_faerieAI : public ScriptedAI
        {
            mob_darkwoods_faerieAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DISGUISE, 5000);
                events.ScheduleEvent(EVENT_FAE_SPIRIT, 15000);
                events.ScheduleEvent(EVENT_NIGHT_SKY, 22000);
                events.ScheduleEvent(EVENT_STARSURGE, 30000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == 64267)
                {
                    summon->DespawnOrUnsummon(15000);
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
                        case EVENT_DISGUISE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(me, SPELL_DISGUISE, false);
                            events.ScheduleEvent(EVENT_DISGUISE, 70000);
                            break;
                        case EVENT_FAE_SPIRIT:
                            me->CastSpell(me, SPELL_FAE_SPIRIT, false);
                            events.ScheduleEvent(EVENT_FAE_SPIRIT, 15000);
                            break;
                        case EVENT_NIGHT_SKY:
                            me->CastSpell(me, SPELL_NIGHT_SKY, false);
                            events.ScheduleEvent(EVENT_NIGHT_SKY, 22000);
                            break;
                        case EVENT_STARSURGE:
                            me->CastSpell(me, SPELL_STARSURGE, false);
                            events.ScheduleEvent(EVENT_STARSURGE, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Hei Feng - 63920
class mob_hei_feng : public CreatureScript
{
    public:
        mob_hei_feng() : CreatureScript("mob_hei_feng")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_hei_fengAI(creature);
        }

        struct mob_hei_fengAI : public ScriptedAI
        {
            mob_hei_fengAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEEP_BREATH, 5000);
                events.ScheduleEvent(EVENT_SERPENT_SWEEP, 15000);
                events.ScheduleEvent(EVENT_SHADOW_DETONATION, 22000);
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
                        case EVENT_DEEP_BREATH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEEP_BREATH, false);
                            events.ScheduleEvent(EVENT_DEEP_BREATH, 30000);
                            break;
                        case EVENT_SERPENT_SWEEP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SERPENT_SWEEP, false);
                            events.ScheduleEvent(EVENT_SERPENT_SWEEP, 15000);
                            break;
                        case EVENT_SHADOW_DETONATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOW_DETONATION, false);
                            events.ScheduleEvent(EVENT_SHADOW_DETONATION, 22000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Eshelon - 50772
class mob_eshelon : public CreatureScript
{
    public:
        mob_eshelon() : CreatureScript("mob_eshelon")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_eshelonAI(creature);
        }

        struct mob_eshelonAI : public ScriptedAI
        {
            mob_eshelonAI(Creature* creature) : ScriptedAI(creature)
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

// Restless Leng - 65586
class mob_restless_leng : public CreatureScript
{
    public:
        mob_restless_leng() : CreatureScript("mob_restless_leng")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_restless_lengAI(creature);
        }

        struct mob_restless_lengAI : public ScriptedAI
        {
            mob_restless_lengAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                std::list<Player*> playerList;
                playerList.clear();
                GetPlayerListInGrid(playerList, me, 20.0f);

                for (auto player: playerList)
                    if (player->GetQuestStatus(QUEST_SEARCH_FOR_RESTLESS_LENG) == QUEST_STATUS_INCOMPLETE)
                        player->KilledMonsterCredit(MOB_RESTLESS_LENG);
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_townlong_steppes()
{
    //Rare mobs
    new mob_kah_tir();
    new mob_lith_ik();
    new mob_eshelon();
    //Elite mobs
    new mob_darkwoods_faerie();
    new mob_hei_feng();
    // Standard Mobs
    new mob_restless_leng();
}
#endif
