////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    SPELL_PULSING_FLAMES = 147042
};

enum eEvents
{
};

enum eActions
{
};

class boss_galakras : public CreatureScript
{
    public:
        boss_galakras() : CreatureScript("boss_galakras") { }

        struct boss_galakrasAI : public BossAI
        {
            boss_galakrasAI(Creature* creature) : BossAI(creature, DATA_GALAKRAS) 
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_galakrasAI(creature);
        }
};

class mob_master_cannoneer_dagryn : public CreatureScript
{
    public:
        mob_master_cannoneer_dagryn() : CreatureScript("mob_master_cannoneer_dagryn") { }

        struct mob_master_cannoneer_dagrynAI : public ScriptedAI
        {
            mob_master_cannoneer_dagrynAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_master_cannoneer_dagrynAI(creature);
        }
};

class mob_high_enforcer_thranok : public CreatureScript
{
    public:
        mob_high_enforcer_thranok() : CreatureScript("mob_high_enforcer_thranok") { }

        struct mob_high_enforcer_thranokAI : public ScriptedAI
        {
            mob_high_enforcer_thranokAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_high_enforcer_thranokAI(creature);
        }
};

class mob_lieutenant_krugruk : public CreatureScript
{
    public:
        mob_lieutenant_krugruk() : CreatureScript("mob_lieutenant_krugruk") { }

        struct mob_lieutenant_krugrukAI : public ScriptedAI
        {
            mob_lieutenant_krugrukAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_lieutenant_krugrukAI(creature);
        }
};

class mob_korgra_the_snake : public CreatureScript
{
    public:
        mob_korgra_the_snake() : CreatureScript("mob_korgra_the_snake") { }

        struct mob_korgra_the_snakeAI : public ScriptedAI
        {
            mob_korgra_the_snakeAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_korgra_the_snakeAI(creature);
        }
};

class mob_dragonmaw_bonecrusher : public CreatureScript
{
    public:
        mob_dragonmaw_bonecrusher() : CreatureScript("mob_dragonmaw_bonecrusher") { }

        struct mob_dragonmaw_bonecrusherAI : public ScriptedAI
        {
            mob_dragonmaw_bonecrusherAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dragonmaw_bonecrusherAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_galakras()
{
    new boss_galakras();
    new mob_master_cannoneer_dagryn();
    new mob_high_enforcer_thranok();
    new mob_lieutenant_krugruk();
    new mob_korgra_the_snake();
    new mob_dragonmaw_bonecrusher();
}
#endif
