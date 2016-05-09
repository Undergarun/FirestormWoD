////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Garr
SD%Complete: 50
SDComment: Adds NYI
SDCategory: Molten Core
EndScriptData */

#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "molten_core.h"

enum Spells
{
    // Garr
    SPELL_ANTIMAGIC_PULSE   = 19492,
    SPELL_MAGMA_SHACKLES    = 19496,
    SPELL_ENRAGE            = 19516,

    // Adds
    SPELL_ERUPTION          = 19497,
    SPELL_IMMOLATE          = 20294
};

enum Events
{
    EVENT_ANTIMAGIC_PULSE    = 1,
    EVENT_MAGMA_SHACKLES     = 2
};

class boss_garr : public CreatureScript
{
    public:
        boss_garr() : CreatureScript("boss_garr") { }

        struct boss_garrAI : public BossAI
        {
            boss_garrAI(Creature* creature) : BossAI(creature, BOSS_GARR)
            {
            }

            void EnterCombat(Unit* victim)
            {
                BossAI::EnterCombat(victim);
                events.ScheduleEvent(EVENT_ANTIMAGIC_PULSE, 25000);
                events.ScheduleEvent(EVENT_MAGMA_SHACKLES, 15000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ANTIMAGIC_PULSE:
                            DoCast(me, SPELL_ANTIMAGIC_PULSE);
                            events.ScheduleEvent(EVENT_ANTIMAGIC_PULSE, urand(10000, 15000));
                            break;
                        case EVENT_MAGMA_SHACKLES:
                            DoCast(me, SPELL_MAGMA_SHACKLES);
                            events.ScheduleEvent(EVENT_MAGMA_SHACKLES, urand(8000, 12000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_garrAI(creature);
        }
};

class mob_firesworn : public CreatureScript
{
    public:
        mob_firesworn() : CreatureScript("mob_firesworn") { }

        struct mob_fireswornAI : public ScriptedAI
        {
            mob_fireswornAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 immolateTimer;
            bool m_AlreadyInDamageTaken;

            void Reset()
            {
                m_AlreadyInDamageTaken = false;
                immolateTimer = 4000;                              //These times are probably wrong
            }

            void DamageTaken(Unit* /*attacker*/, uint32& damage, SpellInfo const* /*p_SpellInfo*/)
            {
                uint32 const health10pct = me->CountPctFromMaxHealth(10);
                uint32 health = me->GetHealth();
                if (int32(health) - int32(damage) < int32(health10pct))
                {
                    damage = 0;

                    if (!m_AlreadyInDamageTaken)
                    {
                        m_AlreadyInDamageTaken = true;

                        DoCastVictim(SPELL_ERUPTION);
                        me->DespawnOrUnsummon();

                        m_AlreadyInDamageTaken = false;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (immolateTimer <= diff)
                {
                     if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_IMMOLATE);
                    immolateTimer = urand(5000, 10000);
                }
                else
                    immolateTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_fireswornAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_garr()
{
    new boss_garr();
    new mob_firesworn();
}
#endif