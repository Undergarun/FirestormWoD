/*
* Copyright (C) 2014-20xx AhsranCore <http://www.ashran.com/>
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
#include "GameObjectAI.h"

// /!\ IN PROGRESS ! /!\

// Archmage Khadgar - 78558 (Main quest giver/taker)
class npc_archmage_khadgar : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_archmage_khadgar() : MS::Game::Scripting::Interfaces::CreatureScript("npc_archmage_khadgar")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest)
        {
            if (p_Quest->GetQuestId() == 35933)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (Creature* l_creature = p_Creature->SummonCreature(NPC_IRON_GRUNT, l_Pos))
                {
                    l_creature->SetReactState(REACT_AGGRESSIVE);
                    l_creature->GetAI()->AttackStart(p_Player);
                }
            }
            return true;
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, Quest* const p_Quest)
        {
            if (p_Quest->GetQuestId() == 34393)
            {
                if (GameObject* l_Gob = GetClosestGameObjectWithEntry(p_Creature, 237670, 200.0f))
                    l_Gob->DestroyForPlayer(p_Player); // remains there, wtf ? to check soon
            }
            else if (p_Quest->GetQuestId() == 34420)
            {
                if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(30040))
                    p_Player->AddQuest(l_Quest, p_Creature);
            }

            return true;
        }
};

// 82188 - Mumper (Horde defenser)
class npc_mumper : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_mumper() : MS::Game::Scripting::Interfaces::CreatureScript("npc_mumper") { }

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

//                me->SummonCreature(NPC_IRON_GRUNT, 4042.2026f, -2319.1357f, 64.6786f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void KilledUnit(Unit* p_Victim)
            {
                if (p_Victim->ToCreature())
                {
//                    me->SummonCreature(NPC_IRON_GRUNT, 4042.2026f, -2319.1357f, 64.6786f);
                    me->SetFullHealth();
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
class npc_moriccalas : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_moriccalas() : MS::Game::Scripting::Interfaces::CreatureScript("npc_moriccalas") { }

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

//                me->SummonCreature(NPC_IRON_GRUNT, 4090.0317f, -2324.4582f, 64.57f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void KilledUnit(Unit* p_Victim)
            {
                if (p_Victim->ToCreature())
                {
//                    me->SummonCreature(NPC_IRON_GRUNT, 4090.0317f, -2324.4582f, 64.57f);
                    me->SetFullHealth();
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
class npc_tore : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_tore() : MS::Game::Scripting::Interfaces::CreatureScript("npc_tore") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_toreAI(creature);
        }

        struct npc_toreAI : public ScriptedAI
        {
            npc_toreAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                me->SummonCreature(NPC_IRON_GRUNT, 4049.7866f, -2293.332764f, 64.92f);
//                me->SummonCreature(NPC_IRON_GRUNT, 4049.0117f, -2296.74585f, 64.92f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                 {
//                     if (urand(0, 1))
//                         me->SummonCreature(NPC_IRON_GRUNT, 4049.7866f, -2293.332764f, 64.92f);
//                     else
//                         me->SummonCreature(NPC_IRON_GRUNT, 4049.0117f, -2296.74585f, 64.92f);
//
//                     me->SetFullHealth();
//                 }
//             }

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

// 82010 - Bonesaw (Horde defenser)
class npc_bonesaw : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_bonesaw() : MS::Game::Scripting::Interfaces::CreatureScript("npc_bonesaw") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bonesawAI(creature);
        }

        struct npc_bonesawAI : public ScriptedAI
        {
            npc_bonesawAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                 me->SummonCreature(NPC_IRON_GRUNT, 4048.229248f, -2315.651611f, 64.775131f, 0.169521f);
//                 me->SummonCreature(NPC_IRON_GRUNT, 4047.729f, -2311.875977f, 64.49f, 0.742862f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                 {
//                     if (urand(0, 1))
//                         me->SummonCreature(NPC_IRON_GRUNT, 4048.229248f, -2315.651611f, 64.775131f, 0.169521f);
//                     else
//                         me->SummonCreature(NPC_IRON_GRUNT, 4047.729f, -2311.875977f, 64.49f, 0.742862f);
//
//                     me->SetFullHealth();
//                 }
//             }

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

// 81994 - Corselle (Horde defenser)
class npc_corselle : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_corselle() : MS::Game::Scripting::Interfaces::CreatureScript("npc_corselle") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_corselleAI(creature);
        }

        struct npc_corselleAI : public ScriptedAI
        {
            npc_corselleAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                 me->SummonCreature(NPC_IRON_GRUNT, 4046.1f, -2306.198975f, 64.920486f, 0.636633f);
//                 me->SummonCreature(NPC_IRON_GRUNT, 4046.62793f, -2302.73584f, 64.920486f, 5.352353f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                 {
//                     if (urand(0, 1))
//                         me->SummonCreature(NPC_IRON_GRUNT, 4046.1f, -2306.198975f, 64.920486f, 0.636633f);
//                     else
//                         me->SummonCreature(NPC_IRON_GRUNT, 4046.62793f, -2302.73584f, 64.920486f, 5.352353f);
//
//                     me->SetFullHealth();
//                 }
//             }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 4.0f)
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

// 82011 - Northpaul (Horde defenser)
class npc_northpaul : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_northpaul() : MS::Game::Scripting::Interfaces::CreatureScript("npc_northpaul") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_northpaulAI(creature);
        }

        struct npc_northpaulAI : public ScriptedAI
        {
            npc_northpaulAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                me->SummonCreature(NPC_IRON_GRUNT, 4046.1f, -2306.198975f, 64.920486f, 0.636633f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                 {
//                     me->SummonCreature(NPC_IRON_GRUNT, 4112.6787f, -2376.455811f, 79.124863f, 0.636633f);
//                     me->SetFullHealth();
//                 }
//             }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 4.0f)
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

// 81997 - Roague (Horde defenser)
class npc_roague : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_roague() : MS::Game::Scripting::Interfaces::CreatureScript("npc_roague") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_roagueAI(creature);
        }

        struct npc_roagueAI : public ScriptedAI
        {
            npc_roagueAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                me->SummonCreature(NPC_IRON_GRUNT, 4038.351807f, -2276.771973f, 64.920219f, 0.23f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                     me->SummonCreature(NPC_IRON_GRUNT, 4038.351807f, -2276.771973f, 64.920219f, 0.23f);
//             }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 4.0f)
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

// 82082 - Racy (Horde defenser)
class npc_racy : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_racy() : MS::Game::Scripting::Interfaces::CreatureScript("npc_racy") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_racyAI(creature);
        }

        struct npc_racyAI : public ScriptedAI
        {
            npc_racyAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                me->SummonCreature(NPC_IRON_GRUNT, 4034.043945f, -2427.018799f, 93.726318f, 0.140602f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                     me->SummonCreature(NPC_IRON_GRUNT, 4034.043945f, -2427.018799f, 93.726318f, 0.140602f);
//             }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 2.0f)
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

// 82191 - High Warlord Shoju (Horde defenser)
class npc_high_warlord_shoju : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_high_warlord_shoju() : MS::Game::Scripting::Interfaces::CreatureScript("npc_high_warlord_shoju") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_high_warlord_shojuAI(creature);
        }

        struct npc_high_warlord_shojuAI : public ScriptedAI
        {
            npc_high_warlord_shojuAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

//                me->SummonCreature(NPC_IRON_GRUNT, 4034.609131f, -2420.910889f, 93.813171f, 0.077768f);
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage)
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

//             void KilledUnit(Unit* p_Victim)
//             {
//                 if (p_Victim->ToCreature())
//                     me->SummonCreature(NPC_IRON_GRUNT, 4034.609131f, -2420.910889f, 93.813171f, 0.077768f);
//             }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who)
                {
                    if (me->GetDistance(p_Who) < 2.0f)
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

// 83538 - Warsong Commander (ennemy)
class npc_warsong_commander : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_warsong_commander() : MS::Game::Scripting::Interfaces::CreatureScript("npc_warsong_commander") { }

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
class npc_iron_grunt : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_iron_grunt() : MS::Game::Scripting::Interfaces::CreatureScript("npc_iron_grunt") { }

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
//                 if (me->GetHealthPct() >= 30.0f)
//                     return;

//                 switch (m_Summoner->GetEntry())
//                 {
//                     case NPC_MUMPER:
//                         break;
//                     case NPC_MORICCALAS:
//                         break;
//                     default:
//                         break;
//                 }
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

// 82484 - Iron Gronnling (ennemy)
class npc_iron_gronnling : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_iron_gronnling() : MS::Game::Scripting::Interfaces::CreatureScript("npc_iron_gronnling") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_gronnlingAI(creature);
        }

        struct npc_iron_gronnlingAI : public ScriptedAI
        {
            npc_iron_gronnlingAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }

                if (me->HasAura(166131))
                    me->RemoveAura(166131);
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
            }
        };
};

// 81711 - ShadowMoon Ritualist (ennemy)
class npc_shadowmoon_ritualist : public MS::Game::Scripting::Interfaces::CreatureScript
{
    public:
        npc_shadowmoon_ritualist() : MS::Game::Scripting::Interfaces::CreatureScript("npc_shadowmoon_ritualist") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_shadowmoon_ritualistAI(creature);
        }

        struct npc_shadowmoon_ritualistAI : public ScriptedAI
        {
            npc_shadowmoon_ritualistAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
            }
        };
};

// 237670/237667 - Dark Portal
class go_platform_tanaan : public MS::Game::Scripting::Interfaces::GameObjectScript
{
    public:
        go_platform_tanaan() : MS::Game::Scripting::Interfaces::GameObjectScript("go_platform_tanaan") { }

        struct go_platform_tanaanAI : public GameObjectAI
        {
            go_platform_tanaanAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            void OnGetVisibilityRange(float &p_Visibility) // Needs to be implemented for gob scripts
            {
                p_Visibility *= 2.0f;
            }

        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new go_platform_tanaanAI(p_Go);
        }
};

void AddSC_tanaan_jungle()
{
    new npc_archmage_khadgar();
    new npc_mumper();
    new npc_moriccalas();
    new npc_tore();
    new npc_bonesaw();
    new npc_corselle();
    new npc_northpaul();
    new npc_roague();
    new npc_racy();
    new npc_high_warlord_shoju();
    new npc_warsong_commander();
    new npc_iron_grunt();
    new npc_iron_gronnling();
    new npc_shadowmoon_ritualist();
    new go_platform_tanaan();
}