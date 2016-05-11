////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Hydromancer_Thespia
SD%Complete: 80
SDComment: Needs additional adjustments (when instance script is adjusted)
SDCategory: Coilfang Resevoir, The Steamvault
EndScriptData */

/* ContentData
boss_hydromancer_thespia
mob_coilfang_waterelemental
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "steam_vault.h"

#define SAY_SUMMON                  -1545000
#define SAY_AGGRO_1                 -1545001
#define SAY_AGGRO_2                 -1545002
#define SAY_AGGRO_3                 -1545003
#define SAY_SLAY_1                  -1545004
#define SAY_SLAY_2                  -1545005
#define SAY_DEAD                    -1545006

#define SPELL_LIGHTNING_CLOUD       25033
#define SPELL_LUNG_BURST            31481
#define SPELL_ENVELOPING_WINDS      31718

#define SPELL_WATER_BOLT_VOLLEY     34449
#define H_SPELL_WATER_BOLT_VOLLEY   37924

// 17797
class boss_hydromancer_thespia : public CreatureScript
{
public:
    boss_hydromancer_thespia() : CreatureScript("boss_hydromancer_thespia") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_thespiaAI (creature);
    }

    struct boss_thespiaAI : public ScriptedAI
    {
        boss_thespiaAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        InstanceScript* instance;

        uint32 LightningCloud_Timer;
        uint32 LungBurst_Timer;
        uint32 EnvelopingWinds_Timer;

        void Reset()
        {
            LightningCloud_Timer = 15000;
            LungBurst_Timer = 7000;
            EnvelopingWinds_Timer = 9000;

            if (instance)
                instance->SetData(TYPE_HYDROMANCER_THESPIA, NOT_STARTED);
        }

        void JustDied(Unit* /*killer*/)
        {
            DoScriptText(SAY_DEAD, me);

            if (instance)
                instance->SetData(TYPE_HYDROMANCER_THESPIA, DONE);
        }

        void KilledUnit(Unit* /*victim*/)
        {
            DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
        }

        void EnterCombat(Unit* /*who*/)
        {
            DoScriptText(RAND(SAY_AGGRO_1, SAY_AGGRO_2, SAY_AGGRO_3), me);

            if (instance)
                instance->SetData(TYPE_HYDROMANCER_THESPIA, IN_PROGRESS);
        }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            //LightningCloud_Timer
            if (LightningCloud_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_LIGHTNING_CLOUD);

                //cast twice in Heroic mode
                if (IsHeroic())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_LIGHTNING_CLOUD);

                LightningCloud_Timer = 15000+rand()%10000;
            } else LightningCloud_Timer -=diff;

            //LungBurst_Timer
            if (LungBurst_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_LUNG_BURST);
                LungBurst_Timer = 7000+rand()%5000;
            } else LungBurst_Timer -=diff;

            //EnvelopingWinds_Timer
            if (EnvelopingWinds_Timer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                    DoCast(target, SPELL_ENVELOPING_WINDS);

                //cast twice in Heroic mode
                if (IsHeroic())
                    if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0))
                        DoCast(target, SPELL_ENVELOPING_WINDS);
                EnvelopingWinds_Timer = 10000+rand()%5000;
            } else EnvelopingWinds_Timer -=diff;

            DoMeleeAttackIfReady();
        }
    };

};

// 17917
class mob_coilfang_waterelemental : public CreatureScript
{
public:
    mob_coilfang_waterelemental() : CreatureScript("mob_coilfang_waterelemental") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_coilfang_waterelementalAI (creature);
    }

    struct mob_coilfang_waterelementalAI : public ScriptedAI
    {
        mob_coilfang_waterelementalAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 WaterBoltVolley_Timer;

        void Reset()
        {
            WaterBoltVolley_Timer = 3000+rand()%3000;
        }

        void EnterCombat(Unit* /*who*/) { }

        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (WaterBoltVolley_Timer <= diff)
            {
                DoCast(me, SPELL_WATER_BOLT_VOLLEY);
                WaterBoltVolley_Timer = 7000+rand()%5000;
            } else WaterBoltVolley_Timer -= diff;

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_hydromancer_thespia()
{
    new boss_hydromancer_thespia();
    new mob_coilfang_waterelemental();
}
#endif
