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
   SPELL_AURA_OF_PRIDE            = 146817,
   SPELL_BANISHMENT               = 146823,
   SPELL_CORRUPTED_PRISON         = 144574,
   SPELL_IMPRISON                 = 144563,
   SPELL_MARK_OF_ARROGANCE        = 144351,
   SPELL_NATURES_BARRIER          = 145379,
   SPELL_PROJECTION               = 146822,
   SPELL_REACHING_ATTACK          = 144774,
   SPELL_SELF_REFLECTION          = 144800,
   SPELL_SWELLING_PRIDE           = 144400,
   SPELL_UNLEASHED                = 144832,
   SPELL_WOUNDED_PRIDE            = 144358
};

class boss_sha_of_pride : public CreatureScript
{
    public:
        boss_sha_of_pride() : CreatureScript("boss_sha_of_pride") { }

        struct boss_sha_of_prideAI : public BossAI
        {
            boss_sha_of_prideAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_PRIDE)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_prideAI(creature);
        }
};

class mob_reflection : public CreatureScript
{
    public:
        mob_reflection() : CreatureScript("mob_reflection") { }

        struct mob_reflectionAI : public ScriptedAI
        {
            mob_reflectionAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_reflectionAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_sha_of_pride()
{
    new boss_sha_of_pride();
    new mob_reflection();
}
#endif