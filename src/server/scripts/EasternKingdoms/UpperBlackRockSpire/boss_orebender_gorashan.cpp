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
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "upper_blackrock_spire.h"

enum eSpells
{
};

enum eEvents
{
};

enum eSays
{
    TALK_AGGRO,     // 43920 - Who dares disturb my research! - Qui ose interrompre mes recherches ?
    TALK_DEATH,     // 43919 - But the power... it protects me... - Mais le pouvoir ... il me protège ...
    TALK_SLAY,      // 43921/43922 - You disappoint me, I thought you'd be tougher./The weak should know better than to test me.
    TALK_SPELL_1,   // 43923 - So much power! I am invincible! - Tant de pouvoir ! Je suis invincible !
    TALK_SPELL_2,   // 43924 - You can't handle this! - Vous n'êtes pas à la hauteur !
    TALK_SPELL_3,   // 43925 - I can feel this in my soul. - Je le sens dans mon âme.
    TALK_SPELL_4    // 43926 - I will destroy you. - Je vais vous détruire.
};

enum eActions
{
};

// Orebender Gor'Ashan - 76413
class boss_orebender_gorashan : public CreatureScript
{
    public:
        boss_orebender_gorashan() : CreatureScript("boss_orebender_gorashan") { }

        struct boss_orebender_gorashanAI : public BossAI
        {
            boss_orebender_gorashanAI(Creature* p_Creature) : BossAI(p_Creature, DATA_OREBENDER_GORASHAN) { }

            void Reset()
            {
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(TALK_AGGRO);
            }

            void JustDied(Unit* p_Killer)
            {
                Talk(TALK_DEATH);
            }

            void DoAction(const int32 p_Action)
            {
            }

            void UpdateAI(const uint32 p_Diff)
            {
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_orebender_gorashanAI(p_Creature);
        }
};

void AddSC_boss_orebender_gorashan()
{
    new boss_orebender_gorashan();
}
