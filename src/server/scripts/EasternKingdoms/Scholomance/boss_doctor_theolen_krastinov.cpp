////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Doctor_Theolen_Krastinov
SD%Complete: 100
SDComment:
SDCategory: Scholomance
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "scholomance.h"

enum eEnums
{
    SPELL_REND                  = 16509,
    SPELL_BACKHAND              = 18103,
    SPELL_FRENZY                = 8269
};

class boss_doctor_theolen_krastinov : public CreatureScript
{
public:
    boss_doctor_theolen_krastinov() : CreatureScript("boss_doctor_theolen_krastinov") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_theolenkrastinovAI (creature);
    }

    struct boss_theolenkrastinovAI : public ScriptedAI
    {
        boss_theolenkrastinovAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 m_uiRend_Timer;
        uint32 m_uiBackhand_Timer;
        uint32 m_uiFrenzy_Timer;

        void Reset()
        {
            m_uiRend_Timer = 8000;
            m_uiBackhand_Timer = 9000;
            m_uiFrenzy_Timer = 1000;
        }

        void JustDied(Unit* /*killer*/)
        {
            InstanceScript* instance = me->GetInstanceScript();
            if (instance)
            {
                instance->SetData(DATA_DOCTORTHEOLENKRASTINOV_DEATH, 0);

                if (instance->GetData(TYPE_GANDLING) == IN_PROGRESS)
                    me->SummonCreature(1853, 180.73f, -9.43856f, 75.507f, 1.61399f, TEMPSUMMON_DEAD_DESPAWN, 0);
            }
        }

        void UpdateAI(const uint32 uiDiff)
        {
            if (!UpdateVictim())
                return;

            //Rend_Timer
            if (m_uiRend_Timer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_REND);
                m_uiRend_Timer = 10000;
            }
            else
                m_uiRend_Timer -= uiDiff;

            //Backhand_Timer
            if (m_uiBackhand_Timer <= uiDiff)
            {
                DoCast(me->getVictim(), SPELL_BACKHAND);
                m_uiBackhand_Timer = 10000;
            }
            else
                m_uiBackhand_Timer -= uiDiff;

            //Frenzy_Timer
            if (HealthBelowPct(26))
            {
                if (m_uiFrenzy_Timer <= uiDiff)
                {
                    DoCast(me, SPELL_FRENZY);
                    DoScriptText(EMOTE_GENERIC_FRENZY_KILL, me);

                    m_uiFrenzy_Timer = 120000;
                }
                else
                    m_uiFrenzy_Timer -= uiDiff;
            }

            DoMeleeAttackIfReady();
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_theolenkrastinov()
{
    new boss_doctor_theolen_krastinov();
}
#endif
