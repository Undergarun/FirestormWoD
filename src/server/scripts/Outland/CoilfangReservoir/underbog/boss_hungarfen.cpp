////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Hungarfen
SD%Complete: 95
SDComment: Need confirmation if spell data are same in both modes. Summons should have faster rate in heroic
SDCategory: Coilfang Resevoir, Underbog
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

#define SPELL_FOUL_SPORES   31673
#define SPELL_ACID_GEYSER   38739

// 17770
class boss_hungarfen : public CreatureScript
{
public:
    boss_hungarfen() : CreatureScript("boss_hungarfen") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_hungarfenAI (creature);
    }

    struct boss_hungarfenAI : public ScriptedAI
    {
        boss_hungarfenAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        bool Root;
        uint32 Mushroom_Timer;
        uint32 AcidGeyser_Timer;

        void Reset()
        {
            Root = false;
            Mushroom_Timer = 5000;                              // 1 mushroom after 5s, then one per 10s. This should be different in heroic mode
            AcidGeyser_Timer = 10000;
        }

        void EnterCombat(Unit* /*who*/)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (!HealthAbovePct(20))
            {
                if (!Root)
                {
                    DoCast(me, SPELL_FOUL_SPORES);
                    Root = true;
                }
            }

            if (Mushroom_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    me->SummonCreature(17990, target->GetPositionX()+(rand()%8), target->GetPositionY()+(rand()%8), target->GetPositionZ(), float(rand()%5), TEMPSUMMON_TIMED_DESPAWN, 22000);
                else
                    me->SummonCreature(17990, me->GetPositionX()+(rand()%8), me->GetPositionY()+(rand()%8), me->GetPositionZ(), float(rand()%5), TEMPSUMMON_TIMED_DESPAWN, 22000);

                Mushroom_Timer = 10000;
            } else Mushroom_Timer -= diff;

            if (AcidGeyser_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_ACID_GEYSER);
                AcidGeyser_Timer = 10000+rand()%7500;
            } else AcidGeyser_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#define SPELL_SPORE_CLOUD       34168
#define SPELL_PUTRID_MUSHROOM   31690
#define SPELL_GROW              31698

class mob_underbog_mushroom : public CreatureScript
{
public:
    mob_underbog_mushroom() : CreatureScript("mob_underbog_mushroom") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_underbog_mushroomAI (creature);
    }

    struct mob_underbog_mushroomAI : public ScriptedAI
    {
        mob_underbog_mushroomAI(Creature* creature) : ScriptedAI(creature) {}

        bool Stop;
        uint32 Grow_Timer;
        uint32 Shrink_Timer;

        void Reset()
        {
            Stop = false;
            Grow_Timer = 0;
            Shrink_Timer = 20000;

            DoCast(me, SPELL_PUTRID_MUSHROOM, true);
            DoCast(me, SPELL_SPORE_CLOUD, true);
        }

        void MoveInLineOfSight(Unit* /*who*/) {}

        void AttackStart(Unit* /*who*/) {}

        void EnterCombat(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            if (Stop)
                return;

            if (Grow_Timer <= diff)
            {
                DoCast(me, SPELL_GROW);
                Grow_Timer = 3000;
            } else Grow_Timer -= diff;

            if (Shrink_Timer <= diff)
            {
                me->RemoveAurasDueToSpell(SPELL_GROW);
                Stop = true;
            } else Shrink_Timer -= diff;
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_hungarfen()
{
    new boss_hungarfen();
    new mob_underbog_mushroom();
}
#endif
