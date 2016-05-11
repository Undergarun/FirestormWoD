////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Duskwood
SD%Complete: 100
SDComment: Quest Support:8735
SDCategory: Duskwood
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum Yells
{
    YELL_TWILIGHTCORRUPTOR_RESPAWN                                  = 0,
    YELL_TWILIGHTCORRUPTOR_AGGRO                                    = 1,
    YELL_TWILIGHTCORRUPTOR_KILL                                     = 2
};


/*######
# at_twilight_grove
######*/

class at_twilight_grove : public AreaTriggerScript
{
public:
    at_twilight_grove() : AreaTriggerScript("at_twilight_grove") { }

    bool OnTrigger(Player* player, const AreaTriggerEntry* /*at*/)
    {
        if (player->HasQuestForItem(21149))
        {
            if (Unit* TCorrupter = player->SummonCreature(15625, -10328.16f, -489.57f, 49.95f, 0, TEMPSUMMON_MANUAL_DESPAWN, 60000))
            {
                TCorrupter->setFaction(14);
                TCorrupter->SetMaxHealth(832750);
            }
            if (Creature* CorrupterSpeaker = player->SummonCreature(1, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ()-1, 0, TEMPSUMMON_TIMED_DESPAWN, 15000))
            {
                CorrupterSpeaker->SetName("Twilight Corrupter");
                CorrupterSpeaker->SetVisible(true);
                CorrupterSpeaker->AI()->Talk(YELL_TWILIGHTCORRUPTOR_RESPAWN, player->GetGUID());
            }
        }
        return false;
    };
};

/*######
# boss_twilight_corrupter
######*/

#define SPELL_SOUL_CORRUPTION 25805
#define SPELL_CREATURE_OF_NIGHTMARE 25806
#define SPELL_LEVEL_UP 24312

class boss_twilight_corrupter : public CreatureScript
{
public:
    boss_twilight_corrupter() : CreatureScript("boss_twilight_corrupter") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_twilight_corrupterAI (creature);
    }

    struct boss_twilight_corrupterAI : public ScriptedAI
    {
        boss_twilight_corrupterAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 SoulCorruption_Timer;
        uint32 CreatureOfNightmare_Timer;
        uint8 KillCount;

        void Reset()
        {
            SoulCorruption_Timer = 15000;
            CreatureOfNightmare_Timer = 30000;
            KillCount = 0;
        }
        void EnterCombat(Unit* /*who*/)
        {
            Talk(YELL_TWILIGHTCORRUPTOR_AGGRO);
        }

        void KilledUnit(Unit* victim)
        {
            if (victim->IsPlayer())
            {
                ++KillCount;
                Talk(YELL_TWILIGHTCORRUPTOR_KILL, victim->GetGUID());

                if (KillCount == 3)
                {
                    DoCast(me, SPELL_LEVEL_UP, true);
                    KillCount = 0;
                }
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            if (SoulCorruption_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_SOUL_CORRUPTION);
                SoulCorruption_Timer = rand()%4000+15000; //gotta confirm Timers
            } else SoulCorruption_Timer-=diff;

            if (CreatureOfNightmare_Timer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CREATURE_OF_NIGHTMARE);
                CreatureOfNightmare_Timer = 45000; //gotta confirm Timers
            } else CreatureOfNightmare_Timer-=diff;
            DoMeleeAttackIfReady();
        };
    };
};

#ifndef __clang_analyzer__
void AddSC_duskwood()
{
    new boss_twilight_corrupter();
    new at_twilight_grove();
}
#endif
