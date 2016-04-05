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
    SPELL_BERSERK             = 26662,
    SPELL_EXPLOSIVE_TAR       = 144492,
    SPELL_FLAME_VENTS         = 144464,
    SPELL_IGNITE_ARMOR        = 144467,
    SPELL_SEISMIC_ACTIVITY    = 144483,
    SPELL_SHOCK_PULSE         = 144485
};

enum eEvents
{
};

class boss_iron_juggernaut : public CreatureScript
{
    public:
        boss_iron_juggernaut() : CreatureScript("boss_iron_juggernaut") { }

        struct boss_iron_juggernautAI : public BossAI
        {
            boss_iron_juggernautAI(Creature* creature) : BossAI(creature, DATA_IRON_JUGGERNAUT)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_iron_juggernautAI(creature);
        }
};

class mob_crawler_mine : public CreatureScript
{
    public:
        mob_crawler_mine() : CreatureScript("mob_crawler_mine") { }

        struct mob_crawler_mineAI : public ScriptedAI
        {
            mob_crawler_mineAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_crawler_mineAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_iron_juggernaut()
{
    new boss_iron_juggernaut();
    new mob_crawler_mine();
}
#endif