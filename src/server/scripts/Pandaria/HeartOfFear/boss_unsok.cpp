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

// 62511 - Amber-Shaper Un'sok
class boss_unsok : public CreatureScript
{
    public:
        boss_unsok() : CreatureScript("boss_unsok") { }

        struct boss_unsokAI : public BossAI
        {
            boss_unsokAI(Creature* creature) : BossAI(creature, DATA_UNSOK)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            uint8 phase;

            void Reset()
            {
                events.Reset();
                summons.DespawnAll();
                phase = 0;

                me->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, EQUIP_UNSOK);
            }

            void JustDied(Unit* /*killer*/)
            {

            }

            void EnterCombat(Unit* attacker)
            {

            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {

            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    /*
                    switch (eventId)
                    {
                        default:
                            break;
                    }
                    */
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_unsokAI(creature);
        }
};

void AddSC_boss_unsok()
{
    new boss_unsok();   // 62511
}