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
    EVENT_TALK_1    = 1
};

enum eSays
{
    TALK_WAVE1_01,
    TALK_WAVE1_02,
    TALK_WAVE2,
    TALK_WAVE3_01,
    TALK_WAVE3_02
};

enum eActions
{
    ACTION_THARBEK_TALK_1,
    ACTION_ZAELA_TALK_1,
    ACTION_ZAELA_TALK_2,
    ACTION_THARBEK_TALK_2,
    ACTION_ZAELA_TALK_3,
    ACTION_THARBEK_TALK_3,
    ACTION_ZAELA_TALK_4,
    ACTION_THARBEK_TALK_4
};

// Warlord Zaela - 77120
class boss_warlord_zaela : public CreatureScript
{
    public:
        boss_warlord_zaela() : CreatureScript("boss_warlord_zaela") { }

        struct boss_warlord_zaelaAI : public BossAI
        {
            boss_warlord_zaelaAI(Creature* p_Creature) : BossAI(p_Creature, DATA_WARLORD_ZAELA)
            {
                m_Instance          = p_Creature->GetInstanceScript();
                m_IntroDone         = false;
                m_FirstTalkTimer    = 0;
                m_SecondTalkTimer   = 0;
            }

            InstanceScript* m_Instance;
            bool m_IntroDone;
            EventMap m_Events;

            uint32 m_FirstTalkTimer;
            uint32 m_SecondTalkTimer;

            void Reset()
            {
                me->ReenableEvadeMode();

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);

                m_Events.Reset();
            }

            void KilledUnit(Unit* p_Who)
            {
            }

            void EnterCombat(Unit* p_Attacker)
            {
            }

            void JustDied(Unit* p_Killer)
            {
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER ||
                    p_Who->GetDistance(me) > 35.f)
                    return;

                if (m_IntroDone)
                    return;

                m_IntroDone = true;
                Talk(TALK_WAVE1_01);

                if (m_Instance)
                {
                    if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(NPC_COMMANDER_THARBEK)))
                        l_Tharbek->AI()->DoAction(ACTION_THARBEK_TALK_1);
                }
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ZAELA_TALK_1:
                        m_FirstTalkTimer = 8500;
                        break;
                    case ACTION_ZAELA_TALK_2:
                    {
                        Talk(TALK_WAVE2);

                        if (m_Instance)
                        {
                            if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(NPC_COMMANDER_THARBEK)))
                                l_Tharbek->AI()->DoAction(ACTION_THARBEK_TALK_2);
                        }

                        break;
                    }
                    case ACTION_ZAELA_TALK_3:
                    {
                        Talk(TALK_WAVE3_01);

                        if (m_Instance)
                        {
                            if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(NPC_COMMANDER_THARBEK)))
                                l_Tharbek->AI()->DoAction(ACTION_THARBEK_TALK_3);
                        }

                        break;
                    }
                    case ACTION_ZAELA_TALK_4:
                        m_SecondTalkTimer = 10000;
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                ScheduleFirstTalk(p_Diff);
                ScheduleSecondTalk(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                /*switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }*/
            }

            void ScheduleFirstTalk(const uint32 p_Diff)
            {
                if (!m_FirstTalkTimer)
                    return;

                if (m_FirstTalkTimer <= p_Diff)
                {
                    m_FirstTalkTimer = 0;
                    Talk(TALK_WAVE1_02);
                }
                else
                    m_FirstTalkTimer -= p_Diff;
            }

            void ScheduleSecondTalk(const uint32 p_Diff)
            {
                if (!m_SecondTalkTimer)
                    return;

                if (m_SecondTalkTimer <= p_Diff)
                {
                    m_SecondTalkTimer = 0;
                    Talk(TALK_WAVE3_02);

                    if (m_Instance)
                    {
                        if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(NPC_COMMANDER_THARBEK)))
                            l_Tharbek->AI()->DoAction(ACTION_THARBEK_TALK_4);
                    }
                }
                else
                    m_SecondTalkTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_warlord_zaelaAI(p_Creature);
        }
};

void AddSC_boss_warlord_zaela()
{
    new boss_warlord_zaela();
}
