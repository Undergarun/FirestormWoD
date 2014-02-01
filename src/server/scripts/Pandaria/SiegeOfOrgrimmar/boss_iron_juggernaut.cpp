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
    SPELL_BERSERK             = 26662,  // Increases the caster's attack and movement speeds by 150% and all damage it deals by 500%.  Also grants immunity to Taunt effects.
    SPELL_EXPLOSIVE_TAR       = 144492,
    SPELL_FLAME_VENTS         = 144464, // Inflicts 292500 to 307500 Fire damage in a frontal cone, searing the target's flesh for 29250 to 30750 damage every 1 second.
    SPELL_IGNITE_ARMOR        = 144467, // Melts an enemy's armor, increasing Fire damage taken by 10% for 30 sec. This sears the target's flesh for 29250 to 30750 damage every 1 second.
    SPELL_SEISMIC_ACTIVITY    = 144483, // A mighty drill bores into the earth, causing an earthquake that deals 39000 to 41000 Nature damage every 1 second.
    SPELL_SHOCK_PULSE         = 144485, // A mighty seismic eruption knocks enemies back and inflicts 73125 to 76875 Nature damage.
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

/*
                         // Spawn Mine
                        switch (rand() % 4) {
                        case 0:
                            me->SummonCreature(NPC_CRAWLER_MINE , LR_X, LR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 1:
                            me->SummonCreature(NPC_CRAWLER_MINE  , LL_X, LL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 2:
                            me->SummonCreature(NPC_CRAWLER_MINE  , UR_X, UR_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        case 3:
                            me->SummonCreature(NPC_CRAWLER_MINE  , UL_X, UL_Y,
                                    SPAWN_Z, 0,
                                    TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,
                                    60000);
                            break;
                        }
*/

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

void AddSC_iron_juggernaut()
{
    new boss_iron_juggernaut();
    new mob_crawler_mine();
}
