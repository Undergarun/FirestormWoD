////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_the_niuzao_temple.h"

enum eSpells
{
    SPELL_SUMMON_GLOBULE    = 119990,
    SPELL_DETONATE          = 120001,

    SPELL_SAP_PUDDLE        = 119939,
    SPELL_VISUAL_SHIELD     = 131628,
    SPELL_SAP_RESIDUE       = 119941, // DOT
    SPELL_GROW              = 120865
};

enum eEvents
{
    EVENT_GROW              = 1
};

class boss_jinbak : public CreatureScript
{
    public:
        boss_jinbak() : CreatureScript("boss_jinbak") {}

        struct boss_jinbakAI : public BossAI
        {
            boss_jinbakAI(Creature* creature) : BossAI(creature, DATA_JINBAK)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;

            void Reset()
            {
                _Reset();
            }

            void EnterCombat(Unit* /*who*/)
            {
                _EnterCombat();
            }

            void JustReachedHome()
            {
                instance->SetBossState(DATA_JINBAK, FAIL);
                summons.DespawnAll();
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& /*damage*/, SpellInfo const*  /*p_SpellInfo*/)
            {}

            void JustSummoned(Creature* summoned)
            {
                summons.Summon(summoned);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*killer*/)
            {
                _JustDied();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_jinbakAI(creature);
        }
};

class npc_sap_puddle : public CreatureScript
{
    public:
        npc_sap_puddle() : CreatureScript("npc_sap_puddle") {}

        struct npc_sap_puddleAI : public Scripted_NoMovementAI
        {
            npc_sap_puddleAI(Creature* creature) : Scripted_NoMovementAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;
            EventMap _events;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_SAP_PUDDLE, true);

                _events.ScheduleEvent(EVENT_GROW, 1000);
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                switch(_events.ExecuteEvent())
                {
                    case EVENT_GROW:
                        me->AddAura(SPELL_GROW, me);
                        _events.ScheduleEvent(EVENT_GROW, 1000);
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_sap_puddleAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_jinbak()
{
    new boss_jinbak();
    new npc_sap_puddle();
}
#endif
