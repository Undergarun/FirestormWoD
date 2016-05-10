////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    // Phase 1
    SPELL_SHA_BOLT              = 143295,
    SPELL_CORROSIVE_BLAST       = 143436,
    SPELL_SWIRL                 = 143309,
    SPELL_SEEPING_SHA           = 143281,

    // Phase 1 to 2

    // Phase 2

    // Phase 2 to 1

    // Others
    SPELL_SHA_CORRUPTION        = 143579,
    SPELL_SHA_POOL              = 143460,
    SPELL_SWELLING_CORRUTION    = 143574
};

enum eEvents
{
    EVENT_SHA_BOLT,
    EVENT_CORROSIVE_BLAST,
    EVENT_SWIRL,
    EVENT_SEEPING_SHA
};

enum eActions
{
};

enum ePhases
{
    PHASE_IMMERSEUS_BOSS,
    PHASE_IMMERSEUS_ADDS
};

enum eTalk
{
};

class boss_immerseus : public CreatureScript
{
    public:
        boss_immerseus() : CreatureScript("boss_immerseus") { }

        struct boss_immerseusAI : public BossAI
        {
            boss_immerseusAI(Creature* creature) : BossAI(creature, DATA_IMMERSEUS)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            uint8 phase;
            uint32 corruptionPct;

            void Reset()
            {
                _Reset();
                events.Reset();

                // Corruption
                corruptionPct = 100;
                me->setPowerType(POWER_ENERGY);
                me->SetPower(POWER_ENERGY, 100);
                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);

                events.ScheduleEvent(EVENT_SHA_BOLT,        5000);
                events.ScheduleEvent(EVENT_CORROSIVE_BLAST, 7000);
                events.ScheduleEvent(EVENT_SWIRL,           40000);
                events.ScheduleEvent(EVENT_SEEPING_SHA,     2000);

                phase = PHASE_IMMERSEUS_BOSS;

                summons.DespawnAll();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_IMMERSEUS, NOT_STARTED);
                }
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_IMMERSEUS, FAIL);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_IMMERSEUS, IN_PROGRESS);
                }
            }

            void RegeneratePower(Powers /*power*/, int32& value)
            {
                value = 0;
                return;
            }

            void DoAction(const int32 /*p_Action*/)
            {
            }

            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }

            void KilledUnit(Unit* /*p_Who*/)
            {
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (phase == PHASE_IMMERSEUS_BOSS && damage > me->GetHealth())
                {
                    damage = 0;
                    me->SetHealth(1);
                    phase = PHASE_IMMERSEUS_ADDS;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SHA_BOLT:
                    {
                        if (phase == PHASE_IMMERSEUS_BOSS)
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_BOLT, false);

                        events.ScheduleEvent(EVENT_SHA_BOLT, 10000);
                        break;
                    }
                    case EVENT_CORROSIVE_BLAST:
                    {
                        if (phase == PHASE_IMMERSEUS_BOSS)
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CORROSIVE_BLAST, false);

                        events.ScheduleEvent(EVENT_CORROSIVE_BLAST, 7000);
                        break;
                    }
                    case EVENT_SWIRL:
                    {
                        if (phase == PHASE_IMMERSEUS_BOSS)
                            me->CastSpell(me, SPELL_SWIRL, false);

                        events.ScheduleEvent(EVENT_SWIRL, 15000);
                        break;
                    }
                    case EVENT_SEEPING_SHA:
                    { 
                        if (phase == PHASE_IMMERSEUS_BOSS && pInstance)
                        {
                            const Map::PlayerList &PlayerList = pInstance->instance->GetPlayers();
                            if (!PlayerList.isEmpty())
                                for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                                    if (Player* player = i->getSource())
                                        me->CastSpell(player, SPELL_SEEPING_SHA, false);
                        }

                        break;
                    }
                    default:
                        break;
                }

                if (phase == PHASE_IMMERSEUS_BOSS)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_immerseusAI(creature);
        }
};

class mob_contaminated_puddle : public CreatureScript
{
    public:
        mob_contaminated_puddle() : CreatureScript("mob_contaminated_puddle") { }

        struct mob_contaminated_puddleAI : public ScriptedAI
        {
            mob_contaminated_puddleAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_contaminated_puddleAI(creature);
        }
};

class mob_sha_puddle : public CreatureScript
{
    public:
        mob_sha_puddle() : CreatureScript("mob_sha_puddle") { }

        struct mob_sha_puddleAI : public ScriptedAI
        {
            mob_sha_puddleAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_puddleAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_immerseus()
{
    new boss_immerseus();
    new mob_contaminated_puddle();
    new mob_sha_puddle();
}
#endif

/*
Intro:
Lorewalker Cho yells: Ah, we adventure together again, except this time I am afraid that the circumstances are much more dire.
Lorewalker Cho yells: What has become of the Vale?
Lorewalker Cho yells: The land is scarred! The ever blooming trees and plants wither and die, as the water from the pools drains away.
Lorewalker Cho yells: Come, let us see the other horrors Garrosh has unleashed upon our land.
Lorewalker Cho yells: Once, the Titans used these life-giving waters to create and shape all life in Pandaria.
Lorewalker Cho yells: It is these waters that kept the Vale in bloom. Their runoff into the Valley of the Four Winds created the most fertile farmland in the world!
Lorewalker Cho yells: And now, the malignance of the Old God has manifested itself within the waters.
Lorewalker Cho yells: Such a foul, foul thing - destroy it, before it seeps deep into the soil and corrupts all of Pandaria!

Outro:
Lorewalker Cho yells: Ah, you have done it. The waters are pure once more!
Lorewalker Cho yells: Can you feel their life-giving energies flow through you?
Lorewalker Cho yells: It will take much time for the Vale to heal, but you have given us hope!
*/
