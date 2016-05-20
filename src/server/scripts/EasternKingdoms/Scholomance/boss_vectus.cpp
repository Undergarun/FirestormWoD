////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Vectus
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"

enum eEnums
{
    SPELL_FLAMESTRIKE            = 18399,
    SPELL_BLAST_WAVE             = 16046,
    SPELL_FIRESHIELD             = 19626,
    SPELL_FRENZY                 = 8269 //28371
};

class boss_vectus : public CreatureScript
{
public:
    boss_vectus() : CreatureScript("boss_vectus") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_vectusAI (creature);
    }

    struct boss_vectusAI : public ScriptedAI
    {
        boss_vectusAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 m_uiFireShield_Timer;
        uint32 m_uiBlastWave_Timer;
        uint32 m_uiFrenzy_Timer;

        void Reset()
        {
            m_uiFireShield_Timer = 2000;
            m_uiBlastWave_Timer = 14000;
            m_uiFrenzy_Timer = 0;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            //FireShield_Timer
            if (m_uiFireShield_Timer <= uiDiff)
            {
                DoCast(me, SPELL_FIRESHIELD);
                m_uiFireShield_Timer = 90000;
            }
            else
                m_uiFireShield_Timer -= uiDiff;

            //BlastWave_Timer
            if (m_uiBlastWave_Timer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_BLAST_WAVE);
                m_uiBlastWave_Timer = 12000;
            }
            else
                m_uiBlastWave_Timer -= uiDiff;

            //Frenzy_Timer
            if (HealthBelowPct(25))
            {
                if (m_uiFrenzy_Timer <= uiDiff)
                {
                    DoCast(me, SPELL_FRENZY);
                    DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                    m_uiFrenzy_Timer = 24000;
                }
                else
                    m_uiFrenzy_Timer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_vectus()
{
    new boss_vectus();
}
#endif
