/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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
#include "ObjectMgr.h"
#include "ScriptMgr.h"

#define BOSS_NIUZAO 0

enum eSpells
{
};

enum eEvents
{
};

enum eCreatures
{
};

enum eSays
{
};

enum eActions
{
};

class boss_niuzao : public CreatureScript
{
    public:
        boss_niuzao() : CreatureScript("boss_niuzao") { }

        struct boss_niuzaoAI : public BossAI
        {
            boss_niuzaoAI(Creature* creature) : BossAI(creature, BOSS_NIUZAO) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                _Reset();
            }

            void KilledUnit(Unit* who)
            {
                if (who->IsPlayer())
                    //Talk(TALK_SLAY);
                        return;
            }

            void EnterCombat(Unit* attacker)
            {
            }

            void JustDied(Unit* killer)
            {
            }

            void DoAction(const int32 action)
            {
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                /*switch (events.ExecuteEvent())
                {
                    default:
                        break;
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_niuzaoAI(creature);
        }
};

void AddSC_boss_niuzao()
{
    new boss_niuzao();
}
