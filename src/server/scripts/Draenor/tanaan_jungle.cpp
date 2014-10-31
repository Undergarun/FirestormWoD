/*
* Copyright (C) 2014-20xx AhsranCore <http://www.ahrasn.com/>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "tanaan_jungle.h"

// 82188 - Mumper (Horde defenser)
class npc_mumper : public CreatureScript
{
    public:
        npc_mumper() : CreatureScript("npc_mumper") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_mumperAI(creature);
        }

        struct npc_mumperAI : public ScriptedAI
        {
            npc_mumperAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                me->SummonCreature(NPC_IRON_GRUNT, 4042.2026f, -2319.1357f, 64.6786f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void DamageDealt(Unit* p_Target, uint32& p_Damage, DamageEffectType /*damageType*/)
            {
                if (p_Target->ToCreature())
                {
                    if (p_Target->GetHealth() <= p_Damage || p_Target->GetHealthPct() <= 30.0f)
                    {
                        me->SummonCreature(NPC_IRON_GRUNT, 4042.2026f, -2319.1357f, 64.6786f);
                        me->SetFullHealth();
                    }
                }
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 5.0f)
                    {
                        if (Creature* creature = p_Who->ToCreature())
                        {
                            if (creature->GetEntry() == NPC_IRON_GRUNT)
                                AttackStart(p_Who);
                        }
                    }
                }
            }
        };
};

// 81990 - Moriccalas (Horde defenser)
class npc_moriccalas : public CreatureScript
{
    public:
        npc_moriccalas() : CreatureScript("npc_moriccalas") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_moriccalasAI(creature);
        }

        struct npc_moriccalasAI : public ScriptedAI
        {
            npc_moriccalasAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                me->SummonCreature(NPC_IRON_GRUNT, 4090.0317f, -2324.4582f, 64.57f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void DamageDealt(Unit* p_Target, uint32& p_Damage, DamageEffectType /*damageType*/)
            {
                if (p_Target->ToCreature())
                {
                    if (p_Target->GetHealth() <= p_Damage || p_Target->GetHealthPct() <= 30.0f)
                    {
                        me->SummonCreature(NPC_IRON_GRUNT, 4090.0317f, -2324.4582f, 64.57f);
                        me->SetFullHealth();
                    }
                }
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 5.0f)
                    {
                        if (Creature* creature = p_Who->ToCreature())
                        {
                            if (creature->GetEntry() == NPC_IRON_GRUNT)
                                AttackStart(p_Who);
                        }
                    }
                }
            }
        };
};

// 82007 - Tore (Horde defenser)
class npc_tore : public CreatureScript
{
    public:
        npc_tore() : CreatureScript("npc_tore") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_toreAI(creature);
        }

        struct npc_toreAI : public ScriptedAI
        {
            npc_toreAI(Creature* creature) : ScriptedAI(creature) {}

            EventMap m_Events;

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                m_Events.Reset();

                me->SummonCreature(NPC_IRON_GRUNT, 4049.7866f, -2293.332764f, 64.92f);
                me->SummonCreature(NPC_IRON_GRUNT, 4049.0117f, -2296.74585f, 64.92f);

                m_Events.ScheduleEvent(EVENT_AGGRO, 2000);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void DamageDealt(Unit* p_Target, uint32& p_Damage, DamageEffectType /*damageType*/)
            {
                if (p_Target->ToCreature())
                {
                    if (p_Target->GetHealth() <= p_Damage || p_Target->GetHealthPct() <= 30.0f)
                    {
                        if (urand(0, 1))
                            me->SummonCreature(NPC_IRON_GRUNT, 4049.7866f, -2293.332764f, 64.92f);
                        else
                            me->SummonCreature(NPC_IRON_GRUNT, 4049.0117f, -2296.74585f, 64.92f);

                        m_Events.ScheduleEvent(EVENT_AGGRO, 2000);
                    }
                }
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 5.0f)
                    {
                        if (Creature* creature = p_Who->ToCreature())
                        {
                            if (creature->GetEntry() == NPC_IRON_GRUNT)
                                AttackStart(p_Who);
                        }
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update();

                if (m_Events.ExecuteEvent() == EVENT_AGGRO)
                {
                    if (!UpdateVictim())
                    {
                        if (Creature* l_IronGrunt = GetClosestCreatureWithEntry(me, NPC_IRON_GRUNT, 4.0f))
                            AttackStart(l_IronGrunt);
                    }
                }
            }
        };
};

// 83538 - Warsong Commander (ennemy)
class npc_warsong_commander : public CreatureScript
{
    public:
        npc_warsong_commander() : CreatureScript("npc_warsong_commander") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_warsong_commanderAI(creature);
        }

        struct npc_warsong_commanderAI : public ScriptedAI
        {
            npc_warsong_commanderAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
            }

            void DamageDealt(Unit* p_Target, uint32& p_Damage, DamageEffectType /*damageType*/)
            {
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
            }
        };
};

// 78883 - Iron Grunt (ennemy)
class npc_iron_grunt : public CreatureScript
{
    public:
        npc_iron_grunt() : CreatureScript("npc_iron_grunt") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_gruntAI(creature);
        }

        struct npc_iron_gruntAI : public ScriptedAI
        {
            npc_iron_gruntAI(Creature* creature) : ScriptedAI(creature) {}

            Creature* m_Summoner;

            void Reset()
            {
                m_Summoner = nullptr;
            }

            void DamageTaken(Unit* p_Attacker, uint32 &p_Damage)
            {
                if (me->GetHealthPct() >= 30.0f)
                    return;

                switch (m_Summoner->GetEntry())
                {
                    case NPC_MUMPER:
                        break;
                    case NPC_MORICCALAS:
                        break;
                    default:
                        break;
                }
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (!p_Summoner->ToCreature())
                    return;

                m_Summoner = p_Summoner->ToCreature();
                me->SetReactState(REACT_PASSIVE);
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                switch (p_Summoner->GetEntry())
                {
                    case NPC_MUMPER:
                        me->GetMotionMaster()->MovePoint(1, 4055.1755f, -2318.4304f, 64.6965f);
                        break;
                    case NPC_MORICCALAS:
                        me->GetMotionMaster()->MovePoint(1, 4089.0051f, -2318.11f, 64.6918f);
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                switch (m_Summoner->GetEntry())
                {
                    case NPC_MUMPER:
                    {
                        if (p_Id == 1)
                            me->GetMotionMaster()->MovePoint(2, 4056.94f, -2353.3774f, 82.4690f);
                        else if (p_Id == 2)
                        {
                            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                            me->SetReactState(REACT_AGGRESSIVE);
                        }
                        break;
                    }
                    case NPC_MORICCALAS:
                    {
                        if (p_Id == 1)
                            me->GetMotionMaster()->MovePoint(2, 4075.49f, -2318.3967f, 64.6890f);
                        else if (p_Id == 2)
                        {
                            me->GetMotionMaster()->MovePoint(3, 4077.9233f, -2339.1956f, 73.117699f);
                        }
                        else if (p_Id == 3)
                        {
                            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                            me->SetReactState(REACT_AGGRESSIVE);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        };
};


void AddSC_tanaan_jungle()
{
    new npc_mumper();
    new npc_moriccalas();
    new npc_tore();
    new npc_warsong_commander();
    new npc_iron_grunt();
}