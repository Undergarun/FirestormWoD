/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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