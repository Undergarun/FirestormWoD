/*
 * Copyright (C) 2008 - 2010 TrinityCore <http://www.trinitycore.org>
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ScriptPCH.h"
#include "blackrock_spire.h"

enum Creatures
{
    NPC_ROOKERY_WHELP                             = 10161
};

class npc_rookey_whelp : public CreatureScript
{
    public:
        npc_rookey_whelp() : CreatureScript("npc_rookey_whelp") { }

        struct npc_rookey_whelpAI : public ScriptedAI
        {
            npc_rookey_whelpAI(Creature* creature) : ScriptedAI(creature)
            {
                _instance = creature->GetInstanceScript();
            }

            void JustDied(Unit* /*who*/)
            {
                if (_instance)
                    _instance->SetData(WHELP_DEATH_COUNT, _instance->GetData(WHELP_DEATH_COUNT) + 1);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

        private:
            InstanceScript* _instance;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
               return new npc_rookey_whelpAI(creature);
        }
};

void AddSC_blackrock_spire()
{
    new npc_rookey_whelp();
}
