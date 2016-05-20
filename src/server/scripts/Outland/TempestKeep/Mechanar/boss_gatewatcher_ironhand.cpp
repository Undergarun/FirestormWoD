////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Gatewatcher_Ironhand
SD%Complete: 75
SDComment:
SDCategory: Tempest Keep, The Mechanar
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eSays
{
    SAY_AGGRO_1                    = -1554006,
    SAY_HAMMER_1                   = -1554007,
    SAY_HAMMER_2                   = -1554008,
    SAY_SLAY_1                     = -1554009,
    SAY_SLAY_2                     = -1554010,
    SAY_DEATH_1                    = -1554011,
    EMOTE_HAMMER                   = -1554012
};

enum eSpells
{
    // Spells to be casted
    SPELL_SHADOW_POWER             = 35322,
    H_SPELL_SHADOW_POWER           = 39193,
    SPELL_HAMMER_PUNCH             = 35326,
    SPELL_JACKHAMMER               = 35327,
    H_SPELL_JACKHAMMER             = 39194,
    SPELL_STREAM_OF_MACHINE_FLUID  = 35311
};

class boss_gatewatcher_iron_hand : public CreatureScript
{
    public:

        boss_gatewatcher_iron_hand()
            : CreatureScript("boss_gatewatcher_iron_hand")
        {
        }
            // Gatewatcher Iron-Hand AI
            struct boss_gatewatcher_iron_handAI : public ScriptedAI
            {
                boss_gatewatcher_iron_handAI(Creature* creature) : ScriptedAI(creature)
                {
                }

                uint32 Shadow_Power_Timer;
                uint32 Jackhammer_Timer;
                uint32 Stream_of_Machine_Fluid_Timer;

                void Reset()
                {
                    Shadow_Power_Timer = 25000;
                    Jackhammer_Timer = 45000;
                    Stream_of_Machine_Fluid_Timer = 55000;

                }
                void EnterCombat(Unit* /*who*/)
                {
                    DoScriptText(SAY_AGGRO_1, me);
                }

                void KilledUnit(Unit* /*victim*/)
                {
                    if (rand()%2)
                        return;

                    DoScriptText(RAND(SAY_SLAY_1, SAY_SLAY_2), me);
                }

                void JustDied(Unit* /*killer*/)
                {
                    DoScriptText(SAY_DEATH_1, me);
                    //TODO: Add door check/open code
                }

                void UpdateAI(const uint32 diff)
                {
                    //Return since we have no target
                    if (!UpdateVictim())
                        return;

                    //Shadow Power
                    if (Shadow_Power_Timer <= diff)
                    {
                        DoCast(me, SPELL_SHADOW_POWER);
                        Shadow_Power_Timer = urand(20000, 28000);
                    }
                    else
                        Shadow_Power_Timer -= diff;

                    //Jack Hammer
                    if (Jackhammer_Timer <= diff)
                    {
                        //TODO: expect cast this about 5 times in a row (?), announce it by emote only once
                        DoScriptText(EMOTE_HAMMER, me);
                        DoCast(me->getVictim(), SPELL_JACKHAMMER);

                        //chance to yell, but not same time as emote (after spell in fact casted)
                        if (rand()%2)
                                            return;

                        DoScriptText(RAND(SAY_HAMMER_1, SAY_HAMMER_2), me);
                        Jackhammer_Timer = 30000;
                    }
                    else
                        Jackhammer_Timer -= diff;

                    //Stream of Machine Fluid
                    if (Stream_of_Machine_Fluid_Timer <= diff)
                    {
                        DoCast(me->getVictim(), SPELL_STREAM_OF_MACHINE_FLUID);
                        Stream_of_Machine_Fluid_Timer = urand(35000, 50000);
                    }
                    else
                        Stream_of_Machine_Fluid_Timer -= diff;

                    DoMeleeAttackIfReady();
                }
            };

            CreatureAI* GetAI(Creature* creature) const
            {
                return new boss_gatewatcher_iron_handAI(creature);
            }
};

#ifndef __clang_analyzer__
void AddSC_boss_gatewatcher_iron_hand()
{
    new boss_gatewatcher_iron_hand();
}
#endif
