/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
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

#include "GameObjectAI.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "heart_of_fear.h"


enum eShekzeedAdds
{
    NPC_SETTHIK_WINDBLADE           = 64453,
    NPC_KORTHIK_REAVER              = 63591,
};

// Zorlok - 62980
class boss_shekzeer : public CreatureScript
{
    public:
        boss_shekzeer() : CreatureScript("boss_shekzeer") { }

        struct boss_shekzeerAI : public BossAI
        {
            boss_shekzeerAI(Creature* creature) : BossAI(creature, DATA_SHEKZEER)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_shekzeerAI(creature);
        }
};

void AddSC_boss_shekzeer()
{
    new boss_shekzeer();
}