/*
* Copyright (C) 2014-20xx AshranCore <http://www.ashran.com/>
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
#include "NPCHandler.h"

/*struct EscortSpawns
{
    uint32 summoner;
    uint32 summoned;
    float  pos_x;
    float  pos_y;
};*/

/*EscortSpawns const g_EscortSpawns[10]
{
    // Summoner Summoned      pos_x      pos_y
    { 78430,     79659,     3936.32f, -2503.62f }, ///< Cordana Felsong
    { 79316,     81776,     3962.48f, -2536.50f }, ///< Qiana Moonshadow
    { 78553,     79185,     3956.76f, -2523.68f }, ///< Thrall
    { 78568,     81885,     3940.90f, -2510.88f }, ///< Thaelin Darkanvil
    { 78556,     78965,     3936.05f, -2520.35f }, ///< Ariok
    { 78554,     82973,     3958.91f, -2520.15f }, ///< Vindicator Maraad
    { 78569,     79770,     3931.80f, -2510.70f }, ///< Hansel
    { 79315,     88354,     3943.55f, -2505.18f }, ///< Olin
    { 79675,     76793,     3951.24f, -2501.23f }, ///< Lady Liadrin
    { 78558,     82125,     3936.72f, -2505.22f }  ///< Archmage Khadgar
};

Position g_Waypoints[7] =
{
    { 4070.7846f, -2424.4675f, 94.7915f, 4.6861f },
    { 4042.2768f, -2469.6655f, 94.6138f, 4.5807f }, // upstairs
    { 4040.7263f, -2494.6997f, 77.2829f, 4.6593f }, // downstairs
    { 4032.9309f, -2532.2856f, 71.9626f, 2.9196f },
    { 3993.7102f, -2536.6916f, 66.9982f, 3.2534f }, // Talk + attack
    { 3969.3171f, -2530.2429f, 66.1909f, 2.7586f }, // downstairs house
    { 3954.4253f, -2519.3913f, 69.7395f, 2.4735f } // in the house, cast spell + dispersion
};

Position g_BleedingHollowSpawn[5] =
{
    { 3964.6762f, -2536.0610f, 66.6200f, 5.8923f },
    { 3971.2495f, -2526.6828f, 66.0740f, 5.4917f },
    { 3933.9599f, -2521.4599f, 69.7307f, 0.7224f },
    { 3932.1899f, -2511.2600f, 69.8392f, 4.9061f },
    { 3952.9599f, -2499.9799f, 69.7307f, 3.9810f }
};

Position g_FirstStepPos[10]
{
    { 3936.0500f, -2520.3500f, 69.7307f, 3.9808f }, // Ariok done
    { 3931.8000f, -2510.6999f, 69.7307f, 4.6681f }, // Hansel done
    { 3936.7199f, -2505.2199f, 69.7307f, 5.6082f }, // Khadgar done
    { 3936.3200f, -2503.6201f, 69.7307f, 5.6268f }, // Cordana done
    { 3940.8999f, -2510.8798f, 69.3792f, 5.6180f }, // Thaelin done
    { 3943.5500f, -2505.1799f, 69.3792f, 5.8881f }, // Olin done
    { 3951.2399f, -2501.2299f, 69.7307f, 0.7631f }, // Lady Liadrin done
    { 3956.7600f, -2523.6799f, 69.7497f, 5.8588f }, // Thrall done
    { 3958.9099f, -2520.1499f, 69.8712f, 5.8444f }, // Vindicator done
    { 3962.4799f, -2536.5000f, 61.5501f, 3.7332f }  // Qiana done
};*/


Position g_ShatteredHandSpawn[4] =
{
    { 4419.8500f, -2783.7099f, 15.9326f, 4.84f },
    { 4408.6499f, -2786.5200f, 14.3980f, 4.82f },
    { 4395.9501f, -2786.4499f, 14.4047f, 4.82f },
    { 4384.6699f, -2781.9299f, 15.9378f, 4.58f }
};

bool CheckPosition(Unit* p_SourceUnit, float p_XEstimation, float p_YEstimation)
{
    Position l_Pos;
    p_SourceUnit->GetPosition(&l_Pos);

    return ((l_Pos.m_positionX <= p_XEstimation + 3.0f && l_Pos.m_positionX >= p_XEstimation - 3.0f) &&
            (l_Pos.m_positionY <= p_YEstimation + 3.0f && l_Pos.m_positionY >= p_YEstimation - 3.0f));
}

/// Kill Your Hundred Passive Scene Object
class playerScript_kill_your_hundred : public PlayerScript
{
    public:
        playerScript_kill_your_hundred() : PlayerScript("playerScript_kill_your_hundred") { }

        void OnLogin(Player* p_Player) override
        {
            if (p_Player->HasQuest(TanaanQuests::QuestKillYourHundred))
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneEnterKarGathArena, 16, l_Pos);
            }
        }

        void OnQuestAccept(Player * p_Player, const Quest * p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);

                Position l_Pos;
                p_Player->GetPosition(&l_Pos);
                p_Player->PlayScene(951, p_Player);
            }
        }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred &&
                m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred &&
                m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);

            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred &&
                m_PlayerSceneSecondInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneSecondInstanceId.end())
                p_Player->CancelStandaloneScene(m_PlayerSceneSecondInstanceId[p_Player->GetGUID()]);
        }

        void OnSceneTriggerEvent(Player * p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (p_Event == "Phase")
                {
                    if (!p_Player->GetQuestObjectiveCounter(273758))
                        p_Player->QuestObjectiveSatisfy(82139, 1);

                    m_PlayerSceneSecondInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(839, 16, l_Pos);
                }
                else if (p_Event == "credit")
                {
                    p_Player->MonsterSay("BABABAB 1 !", 1, p_Player->GetGUID());
                }
            }

            bool l_HasSecondScript = std::count_if(m_PlayerSceneSecondInstanceId.begin(), m_PlayerSceneSecondInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasSecondScript)
            {
                if (p_Event == "Credit")
                {
                    p_Player->MonsterSay("BABABAB 2 !", 1, p_Player->GetGUID());
                }
            }
        }

    public:
        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;
        std::map<uint64, uint32> m_PlayerSceneSecondInstanceId;
};
playerScript_kill_your_hundred* g_KillYourHundredPlayerScript = nullptr;

/// Archmage Khadgar - 78558/78559 (Main quest giver/taker)
class npc_archmage_khadgar : public CreatureScript
{
    public:
        npc_archmage_khadgar() : CreatureScript("npc_archmage_khadgar")
        {
        }

        /*void SummonEscorter(uint32 p_SummonerEntry, uint32 p_EscorterEntry, Creature* p_Khadgar, Player* p_Escorted)
        {
            if (Creature* l_Summoner = GetClosestCreatureWithEntry(p_Khadgar, p_SummonerEntry, 20.0f))
            {
                Position l_Pos;
                l_Summoner->GetPosition(&l_Pos);

                if (Creature* l_Summoned = l_Summoner->SummonCreature(p_EscorterEntry, l_Pos, TEMPSUMMON_MANUAL_DESPAWN, 36000, 0, p_Escorted->GetGUID()))
                {
                    l_Summoned->SetPhaseMask(p_Escorted->GetPhaseMask(), true);

                    if (l_Summoned->AI() && p_EscorterEntry == 82125)
                        l_Summoned->AI()->SetGUID(p_Escorted->GetGUID());
                }
            }
        }*/

        bool OnQuestReward(Player * p_Player, Creature * p_Creature, const Quest * p_Quest, uint32 p_Option)
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestCostOfWar:
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask &= ~TanaanPhases::PhaseCostOfWarEnded;
                    l_PhaseMask |= TanaanPhases::PhaseBlazeOfGlory;
                    p_Player->SetPhaseMask(l_PhaseMask, true);

                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestBlazeOfGlory))
                        p_Player->AddQuest(l_Quest, p_Creature);

                    break;
                }
                default:
                    break;
            }

            return true;
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest)
        {
            uint64 l_PlayerPhase = p_Player->GetPhaseMask();

            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestAzerothsLastStand:
                {
                    Position l_Pos;
                    p_Player->GetPosition(&l_Pos);

                    if (Creature* l_Creature = p_Creature->SummonCreature(TanaanCreatures::NpcIronGrunt, l_Pos))
                    {
                        l_Creature->SetReactState(REACT_AGGRESSIVE);
                        l_Creature->GetAI()->AttackStart(p_Player);
                    }
                    break;
                }
                case TanaanQuests::QuestCostOfWar:
                {
//                    l_PlayerPhase |= TANAAN_ESCORT_SECOND_ZONE;
                    l_PlayerPhase |= TanaanPhases::PhaseCostOfWarEnded;
                    l_PlayerPhase &= ~TanaanPhases::PhaseGulDan;
                    l_PlayerPhase &= ~TanaanPhases::PhaseDarkPortalBase;
                    p_Player->SetPhaseMask(l_PlayerPhase, true);
                    p_Player->PlayScene(TanaanSceneObjects::SceneCostOfWarEscort, p_Player);

//                    for (int8 l_Itr = 0; l_Itr < 10; l_Itr++)
//                        SummonEscorter(g_EscortSpawns[l_Itr].summoner, g_EscortSpawns[l_Itr].summoned, p_Creature, p_Player);

                    p_Player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                default:
                    break;
            }
            return true;
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestDarkPortal) == QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(TanaanQuests::QuestDarkPortal);
            else if (p_Player->GetQuestStatus(TanaanQuests::QuestDarkPortal) == QUEST_STATUS_COMPLETE)
            {
                const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestDarkPortal);
                p_Player->RewardQuest(l_Quest, 0, p_Creature);
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestStartDraenor) == QUEST_STATUS_COMPLETE)
            {
                const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestStartDraenor);
                p_Player->RewardQuest(l_Quest, 0, p_Creature);
            }

            p_Player->PrepareQuestMenu(p_Creature->GetGUID());
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        /*CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_archmage_khadgarAI(creature);
        }

        struct npc_archmage_khadgarAI : public ScriptedAI
        {
            npc_archmage_khadgarAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                std::list<Creature*> l_CreatureList;
                me->GetCreatureListWithEntryInGrid(l_CreatureList, 78507, 20.0f);

                for (Creature* l_Grunt : l_CreatureList)
                    me->AddAura(29266, l_Grunt);

                if (CheckPosition(me, g_EscortSpawns[9].pos_x, g_EscortSpawns[9].pos_y))
                {
                    for (uint8 l_Itr = 0; l_Itr < 10; l_Itr++)
                    {
                        if (Creature* l_Summoner = GetClosestCreatureWithEntry(me, g_EscortSpawns[l_Itr].summoner, 30.0f))
                        {
                            if (CheckPosition(l_Summoner, g_EscortSpawns[l_Itr].pos_x, g_EscortSpawns[l_Itr].pos_y))
                                l_Summoner->SetPhaseMask(TANAAN_COW_ENDED, true);
                        }
                    }

                    me->SetPhaseMask(TANAAN_COW_ENDED, true);
                }
            }
        };*/
};


/// Vindicator Maraad - 78554 (Main quest giver/taker)
class npc_vindicator_maraad_tanaan : public CreatureScript
{
    public:
        npc_vindicator_maraad_tanaan() : CreatureScript("npc_vindicator_maraad_tanaan")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Creature->GetAreaId() != TanaanZones::AreaBlackRockQuarry)
                return false;

            p_Player->PrepareQuestMenu(p_Creature->GetGUID());
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_vindicator_maraad_tanaanAI(creature);
        }

        struct npc_vindicator_maraad_tanaanAI : public ScriptedAI
        {
            npc_vindicator_maraad_tanaanAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (me->GetAreaId() != TanaanZones::AreaBlackRockQuarry && me->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER))
                    me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }
        };
};

/// 82188 - 81990 - 82007 - 82010 - 81994 - 82011 - 81997 - 82082 - 82191 - 82012 - 82014 - 82002 - 81996 - 81998 - 79062 - 81993 - 81995 - 82000
/// 82001 - 82003 - 82004 - 82005 - 82008 - 82025 - 82016 - 82260 - 82264 - 82014 - 82009 - 82075 - 82017 - 82189 - 82263
class npc_generic_tanaan_guardian : public CreatureScript
{
    public:
        npc_generic_tanaan_guardian() : CreatureScript("npc_generic_tanaan_guardian") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_generic_tanaan_guardianAI(creature);
        }

        struct npc_generic_tanaan_guardianAI : public ScriptedAI
        {
            npc_generic_tanaan_guardianAI(Creature* creature) : ScriptedAI(creature) {}

            bool m_EnableCheck;

            void Reset()
            {
                m_EnableCheck = true;
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_EnableCheck = false;

                if (Creature* l_Grunt = p_Attacker->ToCreature())
                {
                    if (l_Grunt->GetEntry() == TanaanCreatures::NpcIronGrunt)
                        me->SetAttackTime(WeaponAttackType::BaseAttack, 1000);
                }

                me->SetAttackTime(WeaponAttackType::BaseAttack, 2000);
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
                if (p_Victim->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_Victim->ToCreature()->GetEntry() == TanaanCreatures::NpcIronGrunt)
                    m_EnableCheck = true;
            }

            void EnterEvadeMode()
            {
                m_EnableCheck = true;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_EnableCheck)
                {
                    std::list<Creature*> l_EnemyList;
                    GetCreatureListWithEntryInGrid(l_EnemyList, me, TanaanCreatures::NpcIronGrunt, 3.0f);

                    for (Creature* l_Grunt : l_EnemyList)
                    {
                        if (!l_Grunt->isMoving())
                        {
                            if (me->GetDistance(l_Grunt) < 3.0f)
                            {
                                AttackStart(l_Grunt);
                                break;
                            }
                        }
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING) || !UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };
};

/// Tanaan escorters - 79659 - 81776 - 79185 - 81885 - 78965 - 82973 - 79770 - 88354 - 76793 - 82125
/*class npc_generic_tanaan_escorter : public CreatureScript
{
    public:
        npc_generic_tanaan_escorter() : CreatureScript("npc_generic_tanaan_escorter") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_generic_tanaan_escorterAI(creature);
        }

        enum eEvents
        {
            EVENT_TALK = 1,
            EVENT_GO,
        };

        struct npc_generic_tanaan_escorterAI : public ScriptedAI
        {
            npc_generic_tanaan_escorterAI(Creature* creature) : ScriptedAI(creature) { }

            bool m_EnableCheck;
            EventMap m_Events;
            uint64 m_EscortedPlayerGuid;

            void Reset()
            {
                m_EscortedPlayerGuid = 0;
                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_TALK, 1000);
                me->GetMotionMaster()->Clear();
            }

            void SetGUID(uint64 p_Guid, int32 id)
            {
                m_EscortedPlayerGuid = p_Guid;
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                switch (p_Id)
                {
                    case 1:
                        me->GetMotionMaster()->MovePoint(2, g_Waypoints[1]);
                        break;
                    case 2:
                        me->GetMotionMaster()->MovePoint(3, g_Waypoints[2]);
                        break;
                    case 3:
                        me->GetMotionMaster()->MovePoint(4, g_Waypoints[3]);
                        break;
                    case 4:
                        me->GetMotionMaster()->MovePoint(5, g_Waypoints[4]);
                        break;
                    case 5:
                    {
                        if (me->GetEntry() == 82125)
                        {
                            std::list<Creature*> l_CreatureList;
                            Talk(2);
                            me->GetCreatureListWithEntryInGrid(l_CreatureList, 78507, 30.0f);

                            l_CreatureList.remove_if([this](Creature* l_Creature) -> bool
                            {
                                if (l_Creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                                    return false;

                                return true;
                            });

                            for (Creature* l_Creature : l_CreatureList)
                                me->CastSpell(l_Creature, 133123, true);
                        }
                        else if (me->GetEntry() == 81776) ///< Qiana
                        {
                            me->GetMotionMaster()->MovePoint(6, g_FirstStepPos[9]);
                            break;
                        }

                        me->GetMotionMaster()->MovePoint(6, g_Waypoints[5]);
                        break;
                    }
                    case 6:
                    {
                        if (me->GetEntry() == 82973) ///< Vindicator Maraad
                        {
                            me->GetMotionMaster()->MovePoint(7, g_FirstStepPos[8]);
                            break;
                        }
                        else if (me->GetEntry() == 79185) ///< Thrall
                        {
                            me->GetMotionMaster()->MovePoint(7, g_FirstStepPos[7]);
                            break;
                        }
                        else if (me->GetEntry() == 81776)
                        {
                            me->DespawnOrUnsummon();
                            break;
                        }

                        me->GetMotionMaster()->MovePoint(7, g_Waypoints[6]);
                        break;
                    }
                    case 7:
                    {
                        switch (me->GetEntry())
                        {
                            case 82125: ///< Khadgar
                            {
                                std::list<Creature*> l_CreatureList;
                                me->GetCreatureListWithEntryInGrid(l_CreatureList, 78507, 20.0f);

                                l_CreatureList.remove_if([this](Creature* l_Creature) -> bool
                                {
                                    if (l_Creature->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                                        return false;

                                    return true;
                                });

                                for (Creature* l_Creature : l_CreatureList)
                                    me->CastSpell(l_Creature, 133123, true);

                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[2]);

                                break;
                            }
                            case 79770: ///< Hansel
                            {
                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[1]);
                                break;
                            }
                            case 78965: ///< Ariok
                            {
                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[0]);
                                break;
                            }
                            case 79659: ///< Cordana
                            {
                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[3]);
                                break;
                            }
                            case 81885: ///< Thaelin
                            {
                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[4]);
                                break;
                            }
                            case 88354: ///< Olin
                            {
                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[5]);
                                break;
                            }
                            case 76793: ///< Lady Liadrin
                            {
                                me->GetMotionMaster()->MovePoint(8, g_FirstStepPos[6]);
                                break;
                            }
                            case 82973:
                            case 79185:
                            {
                                me->DespawnOrUnsummon();
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    case 8:
                    {
                        switch (me->GetEntry())
                        {
                            case 82125: // Khadgar
                            {
                                if (Player* l_EscortedPlr = me->GetPlayer(*me, m_EscortedPlayerGuid))
                                {
                                    uint32 l_PlrPhase = l_EscortedPlr->GetPhaseMask();
                                    l_PlrPhase |= TANAAN_COW_ENDED;
                                    l_PlrPhase &= ~TANAAN_ESCORT_SECOND_ZONE;
                                    l_EscortedPlr->SetPhaseMask(l_PlrPhase, true);
                                }
                                break;
                            }
                            default:
                                me->DespawnOrUnsummon();
                                break;
                        }
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_TALK:
                    {
                        switch (me->GetEntry())
                        {
                            case 82125: // Khadgar
                                Talk(1);
                                m_Events.ScheduleEvent(EVENT_GO, 2000);
                                break;
                            case 76793: // Lady
                                m_Events.ScheduleEvent(EVENT_GO, 2500);
                                break;
                            case 88354: // Olin
                                m_Events.ScheduleEvent(EVENT_GO, 2800);
                                break;
                            case 79770: // Hansel
                                m_Events.ScheduleEvent(EVENT_GO, 3100);
                                break;
                            case 82973: // Vindicator
                                m_Events.ScheduleEvent(EVENT_GO, 3400);
                                break;
                            case 78965: // Ariok
                                m_Events.ScheduleEvent(EVENT_GO, 3700);
                                break;
                            case 81885: // Thaelin
                                m_Events.ScheduleEvent(EVENT_GO, 4000);
                                break;
                            case 79185: // Thrall
                                m_Events.ScheduleEvent(EVENT_GO, 4300);
                                break;
                            case 81776: // Qiana
                                m_Events.ScheduleEvent(EVENT_GO, 4600);
                                break;
                            case 79659: // Cordana
                                m_Events.ScheduleEvent(EVENT_GO, 4900);
                                break;
                        }
                        break;
                    }
                    case EVENT_GO:
                    {
                        me->GetMotionMaster()->MovePoint(1, g_Waypoints[0]);
                        if (me->GetEntry() == 82125)
                        {
                            for (int8 l_Itr = 0; l_Itr < 5; l_Itr++)
                            {
                                if (Creature* l_Creature = me->SummonCreature(78507, g_BleedingHollowSpawn[l_Itr]))
                                    l_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };
};*/

/// 78883 - Iron Grunt (ennemy)
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
            npc_iron_gruntAI(Creature* creature) : ScriptedAI(creature) { }

            Position m_Pos;
            bool m_HasReset;

            enum eCreatureIds
            {
                Moriccalas = 1,
                Mumper     = 2
            };

            void Reset()
            {
                m_HasReset = false;

                if (m_HasReset)
                    return;

                me->SetReactState(REACT_AGGRESSIVE);

                m_Pos = me->GetHomePosition();

                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (CheckPosition(eCreatureIds::Moriccalas))
                {
                    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                    me->GetMotionMaster()->MoveCharge(4094.678467f, -2318.3186f, 64.6841f, 10.0f, 1);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                }
                else if (CheckPosition(eCreatureIds::Mumper))
                {
                    if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                    me->GetMotionMaster()->MoveCharge(4054.158203f, -2319.444092f, 64.671272f, 10.0f, 1);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                }

                m_HasReset = true;
            }

            bool CheckPosition(uint8 p_Id)
            {
                switch (p_Id)
                {
                    case eCreatureIds::Moriccalas:
                    {
                        if ((m_Pos.m_positionX <= 4094.0f && m_Pos.m_positionX >= 4093.0f) &&
                            (m_Pos.m_positionY >= -2324.0f && m_Pos.m_positionY <= -2323.0f))
                            return true;
                        else
                            break;
                    }
                    case eCreatureIds::Mumper:
                    {
                        if ((m_Pos.m_positionX <= 4043.0f && m_Pos.m_positionX >= 4042.0f) &&
                            (m_Pos.m_positionY >= -2320.0f && m_Pos.m_positionY <= -2319.0f))
                            return true;
                        else
                            break;
                    }
                    default:
                        break;
                }

                return false;
            }

            void EnterEvadeMode()
            {
                m_HasReset = true;
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                if (CheckPosition(eCreatureIds::Moriccalas))
                {
                    switch (p_Id)
                    {
                        case 1:
                            me->GetMotionMaster()->MoveCharge(4078.83f, -2318.550293f, 64.69f, 10.0f, 2);
                            break;
                        case 2:
                            me->GetMotionMaster()->MoveCharge(4078.955078f, -2345.177734f, 77.115952f, 10.0f, 3);
                            break;
                        case 3:
                        {
                            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

//                             me->getThreatManager().resetAllAggro();
//                             me->getThreatManager().clearReferences();
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->Clear();

                            if (Creature* l_Moriccalas = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcMoriccalas, 3.0f))
                                AttackStart(l_Moriccalas);

                            break;
                        }
                        default:
                            break;
                    }
                }
                else if (CheckPosition(eCreatureIds::Mumper))
                {
                    switch (p_Id)
                    {
                        case 1:
                            me->GetMotionMaster()->MoveCharge(4053.91845f, -2358.014404f, 85.542175f, 10.0f, 2);
                            break;
                        case 2:
                        {
                            if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC))
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

//                             me->getThreatManager().resetAllAggro();
//                             me->getThreatManager().clearReferences();
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->GetMotionMaster()->Clear();

                            if (Creature* l_Mumper = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcMumper, 3.0f))
                                AttackStart(l_Mumper);

                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        };
};

/// 82484 - Iron Gronnling (ennemy)
class npc_iron_gronnling : public CreatureScript
{
    public:
        npc_iron_gronnling() : CreatureScript("npc_iron_gronnling") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_iron_gronnlingAI(creature);
        }

        struct npc_iron_gronnlingAI : public ScriptedAI
        {
            npc_iron_gronnlingAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE ||
                    !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }

                if (me->HasAura(TanaanSpells::SpellKnockthrough))
                    me->RemoveAura(TanaanSpells::SpellKnockthrough);
            }
        };
};

/// 81711 - ShadowMoon Ritualist (ennemy)
class npc_shadowmoon_ritualist : public CreatureScript
{
    public:
        npc_shadowmoon_ritualist() : CreatureScript("npc_shadowmoon_ritualist") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_shadowmoon_ritualistAI(creature);
        }

        struct npc_shadowmoon_ritualistAI : public ScriptedAI
        {
            npc_shadowmoon_ritualistAI(Creature* creature) : ScriptedAI(creature) {}

            void Reset()
            {
                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE ||
                    !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }
            }
        };
};

/// 300003 - Gul'Dan Trigger (Friendly)
class npc_gul_dan_trigger : public CreatureScript
{
    public:
        npc_gul_dan_trigger() : CreatureScript("npc_gul_dan_trigger") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_gul_dan_triggerAI(creature);
        }

        struct npc_gul_dan_triggerAI : public ScriptedAI
        {
            npc_gul_dan_triggerAI(Creature* creature) : ScriptedAI(creature) { }

            void UpdateAI(uint32 const diff)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 5.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->isInFront(me))
                    {
                        if (l_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_INCOMPLETE && l_Player->GetQuestObjectiveCounter(273930) < 1)
                            l_Player->KilledMonsterCredit(TanaanKillCredits::CreditEnterGuldanPrison);
                    }
                }
            }
        };
};

/// 82647 - Tormented Soul (ennemy)
class npc_tormented_soul : public CreatureScript
{
    public:
        npc_tormented_soul() : CreatureScript("npc_tormented_soul") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tormented_soulAI(creature);
        }

        struct npc_tormented_soulAI : public ScriptedAI
        {
            npc_tormented_soulAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                if (Creature* l_GulDan = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcIronGrunt, 30.0f))
                    me->setFaction(eFactions::FactionAggressive);
            }

            void MoveInLineOfSight(Unit* p_Target)
            {
                if (p_Target->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (p_Target->ToPlayer()->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_INCOMPLETE && me->GetDistance(p_Target) <= 3.0f)
                    AttackStart(p_Target);
            }
        };
};

/// 78556 - Ariok
class npc_tanaan_ariok : public CreatureScript
{
    public:
        npc_tanaan_ariok() : CreatureScript("npc_tanaan_ariok") { }

        enum eDatas
        {
            ActionStartEscort = 1
        };

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_INCOMPLETE ||
                p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_FAILED)
            {
                uint64 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseBlazeOfGlory;
                l_PhaseMask |= TanaanPhases::PhaseAltarAltercation;

                if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjAriokGossip) < 1)
                    p_Player->KilledMonsterCredit(TanaanKillCredits::CreditAriokGossip);

                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);

                if (Creature* l_Ariok = p_Creature->SummonCreature(p_Creature->GetEntry(), l_Pos, TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Ariok->SetPhaseMask(l_PhaseMask, true);

                    if (l_Ariok->GetAI())
                    {
                        l_Ariok->AI()->SetGUID(p_Player->GetGUID());
                        l_Ariok->AI()->DoAction(eDatas::ActionStartEscort);
                    }
                }
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }

            return true;
        }

        struct npc_tanaan_ariokAI : public ScriptedAI
        {
            npc_tanaan_ariokAI(Creature* creature) : ScriptedAI(creature) { }

            bool m_IsSummoned;
            bool m_EscortPaused;
            uint64 m_PlayerGuid;

            void Reset()
            {
                m_IsSummoned = false;
                m_EscortPaused = false;
                m_PlayerGuid = 0;
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Index*/)
            {
                m_PlayerGuid = p_Guid;
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == eDatas::ActionStartEscort && m_IsSummoned)
                {
                    if (me->HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP))
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    if (m_PlayerGuid)
                    {
                        if (Player* p_Followed = me->GetPlayer(*me, m_PlayerGuid))
                            me->GetMotionMaster()->MoveFollow(p_Followed, 0.2f, 5.23f);
                    }
                }
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_IsSummoned = true;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (m_IsSummoned && !m_EscortPaused)
                {
                    if (Creature* l_Bleeding = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcBleedingHollowBloodchosen, 54.02f))
                    {
                        me->GetMotionMaster()->Clear();
                        m_EscortPaused = true;

                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                if (!l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed))
                                    l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditAriokEscort, 1);
                            }
                        }
                    }

                    if (m_IsSummoned && m_EscortPaused)
                    {
                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed) >= 3)
                                    me->GetMotionMaster()->MoveFollow(l_Player, 0.2f, 5.23f);
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_ariokAI(creature);
        }
};

/// 78507 - Bleeding Hollow Savage
class npc_bleeding_hollow_sauvage : public CreatureScript
{
    public:
        npc_bleeding_hollow_sauvage() : CreatureScript("npc_bleeding_hollow_sauvage") { }

        struct npc_bleeding_hollow_sauvageAI : public ScriptedAI
        {
            npc_bleeding_hollow_sauvageAI(Creature* creature) : ScriptedAI(creature) { }

            void SpellHit(Unit* p_Caster, const SpellInfo* p_SpellInfo)
            {
                if (p_SpellInfo->Id == TanaanSpells::SpellArcaneBarrage)
                {
                    p_Caster->DealDamage(me, me->GetHealth());
                    me->DespawnOrUnsummon();
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_bleeding_hollow_sauvageAI(creature);
        }
};

/// 78560 - Archmage Khadgar
class npc_archmage_khadgar_bridge : public CreatureScript
{
    public:
        npc_archmage_khadgar_bridge() : CreatureScript("npc_archmage_khadgar_bridge") { }

        bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestKargatharProvingGrounds:
                {
                    uint64 l_PhaseMask = p_Player->GetPhaseMask();
                    // PHASES MANIPULATION
                    p_Player->SetPhaseMask(l_PhaseMask, true);
                    p_Creature->CastSpell(p_Creature, TanaanSpells::SpellMeteorShower, false);
                    p_Creature->MonsterSay("BABABAB !", 1, p_Player->GetGUID());

                    if (p_Creature->AI())
                    {
                        p_Creature->AI()->SetGUID(p_Player->GetGUID(), 1);
                        p_Creature->AI()->DoAction(TanaanActions::ActionTimerSummon);
                    }

                    break;
                }
                case TanaanQuests::QuestKillYourHundred:
                {
                    if (p_Creature->GetAI())
                    {
                        p_Creature->AI()->SetGUID(p_Player->GetGUID(), 2);
                        p_Creature->AI()->DoAction(TanaanActions::ActionEventSceneArena);
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_archmage_khadgar_bridgeAI(creature);
        }

        struct npc_archmage_khadgar_bridgeAI : public ScriptedAI
        {
            npc_archmage_khadgar_bridgeAI(Creature* creature) : ScriptedAI(creature) {}

            uint32 m_DestroyTimer;
            uint64 m_PlayerAltarGuid;
            uint64 m_PlayerForSceneGuid;
            uint32 m_SceneTimer;

            void Reset()
            {
                m_DestroyTimer = 0;
                m_PlayerAltarGuid = 0;
                m_SceneTimer   = 0;
                m_PlayerForSceneGuid = 0;

                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }

                if (me->HasAura(TanaanSpells::SpellKnockthrough))
                    me->RemoveAura(TanaanSpells::SpellKnockthrough);
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case TanaanActions::ActionTimerSummon:
                        m_DestroyTimer = 8000;
                        break;
                    case TanaanActions::ActionEventSceneArena:
                        m_SceneTimer = 13500;
                        break;
                    default:
                        break;
                }
            }

            void SetGUID(uint64 p_Guid, int32 p_Id)
            {
                if (p_Id == 1)
                    m_PlayerAltarGuid = p_Guid;
                else if (p_Id == 2)
                    m_PlayerForSceneGuid = p_Guid;
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (Player* l_Player = p_Who->ToPlayer())
                {
                    if (l_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed) >= 3)
                        {
                            // Change Phase
                            l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditFindKhadgarAtBridge, 1);
                        }
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (m_DestroyTimer)
                {
                    if (m_DestroyTimer <= diff)
                    {
                        if (!m_PlayerAltarGuid)
                            return;

                        m_DestroyTimer = 0;
                        Position l_Pos;
                        me->GetPosition(&l_Pos);

                        if (Player* l_Player = me->GetPlayer(*me, m_PlayerAltarGuid))
                            l_Player->SummonCreature(TanaanCreatures::NpcArchmageKhadgarSum, l_Pos);

                        me->MonsterSay("BABAB BABAB !!!", 1, m_PlayerAltarGuid);
                    }
                    else
                        m_DestroyTimer -= diff;
                }

                if (m_SceneTimer)
                {
                    if (m_SceneTimer <= diff)
                    {
                        m_SceneTimer = 0;

                        if (Player* l_Player = me->GetPlayer(*me, m_PlayerForSceneGuid))
                        {
                            Position l_Pos;
                            l_Player->GetPosition(&l_Pos);
                            if (g_KillYourHundredPlayerScript)
                                g_KillYourHundredPlayerScript->m_PlayerSceneFirstInstanceId[l_Player->GetGUID()] = l_Player->PlayStandaloneScene(TanaanSceneObjects::SceneEnterKarGathArena, 16, l_Pos);
                        }
                    }
                    else
                        m_SceneTimer -= diff;
                }
            }
        };
};

/// 80244 - Archmage Khadgar (summoned)
class npc_tanaan_khadgar_bridge : public CreatureScript
{
    public:
        npc_tanaan_khadgar_bridge() : CreatureScript("npc_tanaan_khadgar_bridge") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_khadgar_bridgeAI(creature);
        }

        struct npc_tanaan_khadgar_bridgeAI : public ScriptedAI
        {
            npc_tanaan_khadgar_bridgeAI(Creature* creature) : ScriptedAI(creature) { m_Spectator = nullptr; }

            Player* m_Spectator;

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                me->GetMotionMaster()->MovePoint(1, 4213.2266f, -2786.2f, 23.398f);
                m_Spectator = p_Summoner->ToPlayer();
                p_Summoner->ToPlayer()->PlayScene(TanaanSceneObjects::SceneBridgeDestruction, p_Summoner);
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                if (p_Id == 1)
                    me->GetMotionMaster()->MovePoint(2, 4229.7402f, -2812.96f, 17.2016f);
                else
                {
                    // PHASE CHANGE (gob 231137 disappears, 231136 appears)

                    if (m_Spectator && !m_Spectator->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjFollowKhadgar))
                        m_Spectator->QuestObjectiveSatisfy(TanaanKillCredits::CreditFollowKhadgar, 1);


                    me->DespawnOrUnsummon();
                }

            }

        };
};

/// 83670 - Blood Ritual Orb
class npc_blood_ritual_orb : public CreatureScript
{
    public:
        npc_blood_ritual_orb() : CreatureScript("npc_blood_ritual_orb") { }

        bool OnGossipHello(Player * p_Player, Creature * p_Creature)
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_INCOMPLETE)
            {
                if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed) <= 2)
                {
                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditBloodOrb, 1);
                    p_Creature->DespawnOrUnsummon();

                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed) == 3)
                    {
                        p_Player->PlayScene(TanaanSceneObjects::SceneKilRoggRevealed, p_Player);

                        if (Creature* l_Ariok = GetClosestCreatureWithEntry(p_Player, 78556, 50.0f))
                        {
                            l_Ariok->DespawnOrUnsummon();
                        }
                    }
                }
            }

            return true;
        }
};

/// 79097 - Kargath Bladefist
class npc_kargath_bladefist : public CreatureScript
{
    public:
        npc_kargath_bladefist() : CreatureScript("npc_kargath_bladefist") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kargath_bladefistAI(creature);
        }

        struct npc_kargath_bladefistAI : public ScriptedAI
        {
            npc_kargath_bladefistAI(Creature* creature) : ScriptedAI(creature) { }

            bool m_FightBegin;
            std::map<uint64, bool> m_Map;

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                me->GetMotionMaster()->MovePoint(1, 4213.2266f, -2786.2f, 23.398f);
                p_Summoner->ToPlayer()->PlayScene(TanaanSceneObjects::SceneBridgeDestruction, p_Summoner);
            }

            void MoveInLineOfSight(Unit* p_Unit)
            {
                if (p_Unit->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (m_Map.find(p_Unit->ToPlayer()->GetGUID()) == m_Map.end())
                    m_Map.insert(std::make_pair(p_Unit->ToPlayer()->GetGUID(), false));
            }

            void UpdateAI(uint32 const diff)
            {
            }

            /// @TODO : Remove map, define DoAction there, declare script for Shattered Hand Brawler (82057) and call the DoAction in its JustDied

        };
};

/// 237670/237667 - Dark Portal
class go_platform_tanaan : public GameObjectScript
{
    public:
        go_platform_tanaan() : GameObjectScript("go_platform_tanaan") { }

        struct go_platform_tanaanAI : public GameObjectAI
        {
            go_platform_tanaanAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            void Reset()
            {
                go->SetGoState(GO_STATE_ACTIVE);
                go->SetLootState(GO_ACTIVATED);
            }

        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new go_platform_tanaanAI(p_Go);
        }
};

/// 233104 - Statis Rune
class gob_static_rune : public GameObjectScript
{
    public:
        gob_static_rune() : GameObjectScript("gob_static_rune") { }

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject)
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_INCOMPLETE && p_Player->GetQuestObjectiveCounter(273936) < 1)
            {
                if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBurningBladeDestroyed) == 0 ||
                    p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjShatteredHandDestroyed) == 0 ||
                    p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBlackrockMarkDestroyed) == 0)
                    return false;

                uint64 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseDarkPortalBase;
                l_PhaseMask |= TanaanPhases::PhaseGulDan;

                p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditStatisRuneDestroyed, 1, QUEST_OBJECTIVE_TYPE_NPC, p_Gameobject->GetGUID());
                p_Player->PlayScene(TanaanSceneObjects::SceneGulDanFreedom, p_Player);
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
            return true;
        }
};

/// 229325/229350 - Bleeding Hollow Cages
class go_bleeding_hollow_cage : public GameObjectScript
{
    public:
        go_bleeding_hollow_cage() : GameObjectScript("go_bleeding_hollow_cage") { }

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject)
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestBledDry) == QUEST_STATUS_INCOMPLETE)
            {
                /// Eastern Cage
                if (p_Gameobject->GetEntry() == TanaanGameObjects::GobEasternCage)
                {
                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjEasternCageOpened) >= 1)
                        return true;

                    p_Player->KilledMonsterCredit(TanaanKillCredits::CreditEasternCageOpened);
                    p_Player->PlayScene(TanaanSceneObjects::SceneEasterCage, p_Player);
                }
                /// Southest Cage
                else if (p_Gameobject->GetEntry() == TanaanGameObjects::GobSouthernCage)
                {
                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjSouthernCageOpened) >= 1)
                        return true;

                    p_Player->KilledMonsterCredit(TanaanKillCredits::CreditSouthernCageOpened);
                    p_Player->PlayScene(TanaanSceneObjects::SceneSouthernCage, p_Player);
                }
            }
            return true;
        }
};

/// 233056/233057 - Mark of the Shadowmoon/Bleeding Hollow
class gob_mark_of_tanaan : public GameObjectScript
{
    public:
        gob_mark_of_tanaan() : GameObjectScript("gob_mark_of_tanaan") { }

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject)
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestOnslaughtEnd) == QUEST_STATUS_INCOMPLETE)
            {
                /// Shadowmoon Gob
                if (p_Gameobject->GetEntry() == TanaanGameObjects::GobMarkOfShadowmoon)
                {
                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjNorthernSpireDisabled) >= 1)
                        return true;

                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditNorthernSpireDisabled, 1);
                    p_Player->PlayScene(TanaanSceneObjects::SceneChoGallsFreedom, p_Player);
                }
                /// Bleeding Hollow Gob
                else if (p_Gameobject->GetEntry() == TanaanGameObjects::GobMarkOfBleedingHollow)
                {
                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjSouthernSpireDisabled) >= 1)
                        return true;

                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditSouthernSpireDisabled, 1);
                    p_Player->PlayScene(TanaanSceneObjects::SceneTeronGorsFreedom, p_Player);
                }
            }
            return true;
        }
};

/// 1265 - Tanaan Map
class map_dark_portal_entrance : public WorldMapScript
{
    public:
        map_dark_portal_entrance() : WorldMapScript("map_dark_portal_entrance", TanaanZones::MapTanaan) { }

        void OnPlayerEnter(Map* p_Map, Player* /*p_Player*/) { p_Map->SetObjectVisibility(350.0f); }
};

/// Tanaan entering
class playerScript_enter_tanaan : public PlayerScript
{
    public:
        playerScript_enter_tanaan() : PlayerScript("playerScript_enter_tanaan") { }

        void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneID, uint32 p_NewAreaId)
        {
            if (p_NewZoneId == TanaanZones::ZoneTanaanJungle)
            {
                uint64 l_PhaseMask = p_Player->GetPhaseMask();

                if (p_Player->GetQuestStatus(TanaanQuests::QuestStartDraenor) == QUEST_STATUS_INCOMPLETE)
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestStartDraenor))
                    {
                        p_Player->CompleteQuest(TanaanQuests::QuestStartDraenor);
                        p_Player->RewardQuest(l_Quest, 0, nullptr, false);
                    }
                }
                if (p_Player->GetQuestStatus(TanaanQuests::QuestDarkPortal) == QUEST_STATUS_NONE)
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestDarkPortal))
                        p_Player->AddQuest(l_Quest, nullptr);

                    l_PhaseMask |= TanaanPhases::PhaseDarkPortalBase;
                }
                if (p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_REWARDED ||
                    p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_COMPLETE)
                {
                    l_PhaseMask &= ~TanaanPhases::PhaseDarkPortalBase;
                    l_PhaseMask |= TanaanPhases::PhaseGulDan;
                }
                if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_COMPLETE)
                {
                    l_PhaseMask &= ~TanaanPhases::PhaseGulDan;
                    l_PhaseMask &= ~TanaanPhases::PhaseEscortSecondZone;

                    l_PhaseMask |= TanaanPhases::PhaseCostOfWarEnded;
                }
                if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_REWARDED)
                {
                    l_PhaseMask |= TanaanPhases::PhaseEscortSecondZone;
                }
                if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_REWARDED)
                {
                    if ((l_PhaseMask & TanaanPhases::PhaseCostOfWarEnded) != 0)
                        l_PhaseMask &= ~TanaanPhases::PhaseCostOfWarEnded;

                    l_PhaseMask |= TanaanPhases::PhaseBlazeOfGlory;
                }

                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }

        void OnLogin(Player* p_Player)
        {
            if (p_Player->GetZoneId() == TanaanZones::ZoneTanaanJungle)
            {
                uint64 l_PhaseMask = p_Player->GetPhaseMask();

                if (p_Player->GetQuestStatus(TanaanQuests::QuestDarkPortal) == QUEST_STATUS_NONE)
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestDarkPortal))
                        p_Player->AddQuest(l_Quest, nullptr);

                    l_PhaseMask |= TanaanPhases::PhaseDarkPortalBase;
                }

                if (p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_REWARDED ||
                    p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_COMPLETE)
                {
                    l_PhaseMask &= ~TanaanPhases::PhaseDarkPortalBase;
                    l_PhaseMask |= TanaanPhases::PhaseGulDan;
                }

                if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_REWARDED)
                {
                    l_PhaseMask |= TanaanPhases::PhaseEscortSecondZone;
                }

                if (p_Player->GetQuestStatus(TanaanQuests::QuestStartDraenor) == QUEST_STATUS_INCOMPLETE)
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestStartDraenor))
                    {
                        p_Player->CompleteQuest(TanaanQuests::QuestStartDraenor);
                        p_Player->RewardQuest(l_Quest, 0, nullptr, false);
                    }
                }

                if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_COMPLETE)
                {
                    l_PhaseMask &= ~TanaanPhases::PhaseEscortSecondZone;
                    l_PhaseMask |= TanaanPhases::PhaseCostOfWarEnded;
                }

                if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_REWARDED)
                {
                    if ((l_PhaseMask & TanaanPhases::PhaseCostOfWarEnded) != 0)
                        l_PhaseMask &= ~TanaanPhases::PhaseCostOfWarEnded;

                    l_PhaseMask |= TanaanPhases::PhaseBlazeOfGlory;
                }

                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest)
        {
            switch (p_Quest->GetQuestId())
            {
            case TanaanQuests::QuestDarkPortal:
                {
                    if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestOnslaughtEnd))
                        p_Player->AddQuest(l_Quest, nullptr);
                    break;
                }
                default:
                    break;
            }
        }

        void OnObjectiveValidate(Player* p_Player, uint32 p_QuestId, uint32 p_ObjectiveId)
        {
            if (p_QuestId == TanaanQuests::QuestThePortalPower && p_ObjectiveId == TanaanQuestObjectives::ObjEnterGulDanPrison)
                p_Player->PlayScene(TanaanSceneObjects::SceneGulDanReavel, p_Player);
        }
};

/// Blaze of Glory Passive Scene Object
class playerScript_blaze_of_glory : public PlayerScript
{
    public:
        playerScript_blaze_of_glory()
            : PlayerScript("playerScript_blaze_of_glory")
        {
            for (uint32 l_I = 0; l_I < sPathNodeStore.GetNumRows(); ++l_I)
            {
                PathNodeEntry const* l_NodeEntry = sPathNodeStore.LookupEntry(l_I);

                if (!l_NodeEntry || l_NodeEntry->PathID != BlazeOfGloryData::HutsPath)
                    continue;

                LocationEntry const* l_LocationEntry = sLocationStore.LookupEntry(l_NodeEntry->LocationID);

                if (!l_LocationEntry)
                    continue;

                Hut l_Hut
                {
                    l_LocationEntry->X,
                    l_LocationEntry->Y,
                    l_LocationEntry->Z,
                    false
                };

                m_HutsTemplate.push_back(l_Hut);
            }
        }

        void OnLogin(Player* p_Player) override
        {
            if (p_Player->HasQuest(TanaanQuests::QuestBlazeOfGlory))
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(BlazeOfGloryData::SceneId, 16, l_Pos);
                m_PlayerFiredHuts[p_Player->GetGUID()] = 0;
            }
        }

        void OnQuestAccept(Player * p_Player, const Quest * p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestBlazeOfGlory)
            {
                if (m_PlayerSceneInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneInstanceId[p_Player->GetGUID()]);

                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(BlazeOfGloryData::SceneId, 16, l_Pos);
                m_PlayerFiredHuts[p_Player->GetGUID()] = 0;
            }
        }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestBlazeOfGlory &&
                m_PlayerSceneInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneInstanceId.end())
                p_Player->CancelStandaloneScene(m_PlayerSceneInstanceId[p_Player->GetGUID()]);
        }

        void OnSceneTriggerEvent(Player * p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasScript = std::count_if(m_PlayerSceneInstanceId.begin(), m_PlayerSceneInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (!l_HasScript)
                return;

            if (p_Event == "Visual" && p_Player->HasQuest(TanaanQuests::QuestBlazeOfGlory))
                p_Player->AddAura(BlazeOfGloryData::SpellTrailOfFlameVisual, p_Player);
            else if (p_Event == "Clear")
                p_Player->RemoveAura(BlazeOfGloryData::SpellTrailOfFlameVisual);
        }

        void OnCastInferno(Player* p_Player)
        {
            uint32 l_FiredCount = 0;

            for (uint32 l_I = 0; l_I < m_HutsTemplate.size(); l_I++)
            {
                if (!p_Player->IsInDist2d(m_HutsTemplate[l_I].X, m_HutsTemplate[l_I].Y, 20))
                    continue;

                uint32 l_HutFlag = 1 << l_I;

                if ((m_PlayerFiredHuts[p_Player->GetGUID()] & l_HutFlag) != 0)
                    continue;

                m_PlayerFiredHuts[p_Player->GetGUID()] |= l_HutFlag;
                l_FiredCount++;
            }

            if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestBlazeOfGlory))
            {
                if (const QuestObjective* l_Objective = l_Quest->GetQuestObjective(l_Quest->GetQuestObjectiveId(TanaanQuests::QuestBlazeOfGlory, 0)))
                    p_Player->ToPlayer()->QuestObjectiveSatisfy(l_Objective->ObjectID, l_FiredCount);
            }
        }

    private:
        std::map<uint64, uint32> m_PlayerSceneInstanceId;
        std::vector<Hut> m_HutsTemplate;
        std::map<uint64, uint32> m_PlayerFiredHuts;
};
playerScript_blaze_of_glory* g_BlazeOfGloryPlayerScript = nullptr;

/// A potential Ally Passive Scene Object
class playerScript_a_potential_ally : public PlayerScript
{
    public:
        playerScript_a_potential_ally() : PlayerScript("playerScript_a_potential_ally") { }

        void OnLogin(Player* p_Player) override
        {
            if (p_Player->HasQuest(TanaanQuests::QuestAPotentialAlly))
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneRingOfFire, 16, l_Pos);
            }
        }

        void OnQuestAccept(Player * p_Player, const Quest * p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestAPotentialAlly)
            {
                if (m_PlayerSceneInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneInstanceId[p_Player->GetGUID()]);

                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneRingOfFire, 16, l_Pos);
            }
        }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestAPotentialAlly &&
                m_PlayerSceneInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneInstanceId.end())
                p_Player->CancelStandaloneScene(m_PlayerSceneInstanceId[p_Player->GetGUID()]);
        }

        void OnSceneTriggerEvent(Player * p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasScript = std::count_if(m_PlayerSceneInstanceId.begin(), m_PlayerSceneInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (!l_HasScript)
                return;

            if (p_Event == "Fire Gone" || p_Event == "Credit")
            {
                switch (p_Player->GetTeamId())
                {
                    case TEAM_ALLIANCE:
                    {
                        if (!p_Player->GetQuestObjectiveCounter(272833))
                            p_Player->QuestObjectiveSatisfy(79537, 1);
                        break;
                    }
                    case TEAM_HORDE:
                    {
                        if (!p_Player->GetQuestObjectiveCounter(272869))
                            p_Player->QuestObjectiveSatisfy(78996, 1);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

    private:
        std::map<uint64, uint32> m_PlayerSceneInstanceId;
};
playerScript_a_potential_ally* g_APotentialAllyPlayerScript = nullptr;

/// 165988 - Inferno
class spell_tanaan_inferno : public SpellScriptLoader
{
    public:
        spell_tanaan_inferno() : SpellScriptLoader("spell_tanaan_inferno") { }

        class spell_tanaan_inferno_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tanaan_inferno_SpellScript);

            void HandleDummy(SpellEffIndex effIndex)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster || l_Caster->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (g_BlazeOfGloryPlayerScript)
                    g_BlazeOfGloryPlayerScript->OnCastInferno(l_Caster->ToPlayer());
            }

            void Register()
            {
                OnEffectLaunch += SpellEffectFn(spell_tanaan_inferno_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_tanaan_inferno_SpellScript();
        }
};

void AddSC_tanaan_jungle()
{
    new npc_archmage_khadgar();
    new npc_vindicator_maraad_tanaan();
    new npc_generic_tanaan_guardian();
//    new npc_generic_tanaan_escorter();
    new npc_iron_grunt();
    new npc_iron_gronnling();
    new npc_shadowmoon_ritualist();
    new npc_gul_dan_trigger();
    new npc_tormented_soul();
    new npc_tanaan_ariok();
    new npc_bleeding_hollow_sauvage();
    new npc_blood_ritual_orb();
    new npc_archmage_khadgar_bridge();
    new npc_tanaan_khadgar_bridge();
    new npc_kargath_bladefist();
    new gob_static_rune();
    new go_platform_tanaan();
    new go_bleeding_hollow_cage();
    new gob_mark_of_tanaan();
    new map_dark_portal_entrance();
    new playerScript_enter_tanaan();
    new spell_tanaan_inferno();

    g_BlazeOfGloryPlayerScript   = new playerScript_blaze_of_glory();
    g_APotentialAllyPlayerScript = new playerScript_a_potential_ally();
    g_KillYourHundredPlayerScript = new playerScript_kill_your_hundred();
}