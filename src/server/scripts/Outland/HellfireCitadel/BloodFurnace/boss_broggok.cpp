////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Broggok
SD%Complete: 70
SDComment: pre-event not made
SDCategory: Hellfire Citadel, Blood Furnace
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "blood_furnace.h"

enum eEnums
{
    SAY_AGGRO               = -1542008,

    SPELL_SLIME_SPRAY       = 30913,
    SPELL_POISON_CLOUD      = 30916,
    SPELL_POISON_BOLT       = 30917,

    SPELL_POISON            = 30914
};

// 17380
class boss_broggok : public CreatureScript
{
    public:

        boss_broggok()
            : CreatureScript("boss_broggok")
        {
        }

        struct boss_broggokAI : public BossAI
        {
            boss_broggokAI(Creature* creature) : BossAI(creature, DATA_BROGGOK)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;

            uint32 AcidSpray_Timer;
            uint32 PoisonSpawn_Timer;
            uint32 PoisonBolt_Timer;
            bool canAttack;


            void Reset()
            {
                _Reset();
                AcidSpray_Timer = 10000;
                PoisonSpawn_Timer = 5000;
                PoisonBolt_Timer = 7000;
                DoAction(ACTION_RESET_BROGGOK);
                instance->SetData(TYPE_BROGGOK_EVENT, NOT_STARTED);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);
            }

            void JustSummoned(Creature* summoned)
            {
                summoned->setFaction(16);
                summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                summoned->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                summoned->CastSpell(summoned, SPELL_POISON, false, 0, nullptr, me->GetGUID());
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                if (!canAttack)
                    return;

                if (AcidSpray_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_SLIME_SPRAY);
                    AcidSpray_Timer = 4000+rand()%8000;
                }
                else
                    AcidSpray_Timer -=diff;

                if (PoisonBolt_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_POISON_BOLT);
                    PoisonBolt_Timer = 4000+rand()%8000;
                }
                else
                    PoisonBolt_Timer -=diff;

                if (PoisonSpawn_Timer <= diff)
                {
                    DoCast(me, SPELL_POISON_CLOUD);
                    PoisonSpawn_Timer = 20000;
                }
                else
                    PoisonSpawn_Timer -=diff;

                DoMeleeAttackIfReady();
            }

            void JustDied(Unit* /*killer*/)
            {
                if (instance)
                {
                    instance->HandleGameObject(instance->GetData64(DATA_DOOR4), true);
                    instance->HandleGameObject(instance->GetData64(DATA_DOOR5), true);
                    instance->SetData(TYPE_BROGGOK_EVENT, DONE);
                }
            }

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_broggokAI(creature);
        }
};

class go_broggok_lever : public GameObjectScript
{
public:
    go_broggok_lever() : GameObjectScript("go_broggok_lever") {}

    bool OnGossipHello(Player* /*player*/, GameObject* go)
    {
        if (InstanceScript* instance = go->GetInstanceScript())
            if (instance->GetData(TYPE_BROGGOK_EVENT) != DONE && instance->GetData(TYPE_BROGGOK_EVENT) != IN_PROGRESS)
            {
                instance->SetData(TYPE_BROGGOK_EVENT, IN_PROGRESS);
                if (Creature* broggok = Creature::GetCreature(*go, instance->GetData64(DATA_BROGGOK)))
                    broggok->AI()->DoAction(ACTION_PREPARE_BROGGOK);
            }
            go->UseDoorOrButton();
            return false;
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_broggok()
{
    new boss_broggok();
    new go_broggok_lever();
}
#endif
