////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "violet_hold.h"

//Spells
enum Spells
{
    SPELL_CORROSIVE_SALIVA                     = 54527,
    SPELL_OPTIC_LINK                           = 54396
};

class boss_moragg : public CreatureScript
{
public:
    boss_moragg() : CreatureScript("boss_moragg") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new boss_moraggAI (creature);
    }

    struct boss_moraggAI : public ScriptedAI
    {
        boss_moraggAI(Creature* creature) : ScriptedAI(creature)
        {
            instance = creature->GetInstanceScript();
        }

        uint32 uiOpticLinkTimer;
        uint32 uiCorrosiveSalivaTimer;

        InstanceScript* instance;

        void Reset()
        {
            uiOpticLinkTimer = 10000;
            uiCorrosiveSalivaTimer = 5000;

            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, NOT_STARTED);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, NOT_STARTED);
            }
        }

        void EnterCombat(Unit* /*who*/)
        {
            if (instance)
            {
                if (GameObject* pDoor = instance->instance->GetGameObject(instance->GetData64(DATA_MORAGG_CELL)))
                    if (pDoor->GetGoState() == GO_STATE_READY)
                   {
                        EnterEvadeMode();
                        return;
                    }
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                    instance->SetData(DATA_1ST_BOSS_EVENT, IN_PROGRESS);
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                    instance->SetData(DATA_2ND_BOSS_EVENT, IN_PROGRESS);
            }
        }

        void AttackStart(Unit* who)
        {
            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC) || me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE))
                return;

            if (me->Attack(who, true))
            {
                me->AddThreat(who, 0.0f);
                me->SetInCombatWith(who);
                who->SetInCombatWith(me);
                DoStartMovement(who);
            }
        }

        void MoveInLineOfSight(Unit* /*who*/) {}

        void UpdateAI(const uint32 diff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if (uiOpticLinkTimer <= diff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 100, true))
                    DoCast(target, SPELL_OPTIC_LINK);
                uiOpticLinkTimer = 15000;
            } else uiOpticLinkTimer -= diff;

            if (uiCorrosiveSalivaTimer <= diff)
            {
                DoCast(me->getVictim(), SPELL_CORROSIVE_SALIVA);
                uiCorrosiveSalivaTimer = 10000;
            } else uiCorrosiveSalivaTimer -= diff;

            DoMeleeAttackIfReady();
        }
        void JustDied(Unit* /*killer*/)
        {
            if (instance)
            {
                if (instance->GetData(DATA_WAVE_COUNT) == 6)
                {
                    instance->SetData(DATA_1ST_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 7);
                }
                else if (instance->GetData(DATA_WAVE_COUNT) == 12)
                {
                    instance->SetData(DATA_2ND_BOSS_EVENT, DONE);
                    instance->SetData(DATA_WAVE_COUNT, 13);
                }
            }
        }
    };

};

#ifndef __clang_analyzer__
void AddSC_boss_moragg()
{
    new boss_moragg();
}
#endif
