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
#include "Vehicle.h"

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
    { 4419.8500f, -2783.7099f, 15.9326f, 4.84f }, ///< Pos 0, extreme right
    { 4408.6499f, -2786.5200f, 14.3980f, 4.82f }, ///< Pos 1, right
    { 4395.9501f, -2786.4499f, 14.4047f, 4.82f }, ///< Pos 2, left
    { 4384.6699f, -2781.9299f, 15.9378f, 4.58f }  ///< Pos 3, extreme left
};

bool CheckPosition(Unit* p_SourceUnit, float p_XEstimation, float p_YEstimation)
{
    Position l_Pos;
    p_SourceUnit->GetPosition(&l_Pos);

    return ((l_Pos.m_positionX <= p_XEstimation + 1.0f && l_Pos.m_positionX >= p_XEstimation - 1.0f) &&
            (l_Pos.m_positionY <= p_YEstimation + 1.0f && l_Pos.m_positionY >= p_YEstimation - 1.0f));
}

/// Kill Your Hundred Passive Scene Object
class playerScript_kill_your_hundred : public PlayerScript
{
    public:
        playerScript_kill_your_hundred() : PlayerScript("playerScript_kill_your_hundred") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;
        std::map<uint64, uint32> m_PlayerSceneSecondInstanceId;
        std::map<uint64, uint32> m_PlayerSceneThirdInstanceId;

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
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);

                if (m_PlayerSceneSecondInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneSecondInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneSecondInstanceId[p_Player->GetGUID()]);

                if (m_PlayerSceneThirdInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneThirdInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneThirdInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);

                if (m_PlayerSceneSecondInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneSecondInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneSecondInstanceId[p_Player->GetGUID()]);

                if (m_PlayerSceneThirdInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneThirdInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneThirdInstanceId[p_Player->GetGUID()]);
            }
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
                else if (p_Event == "Credit")
                    p_Player->CancelStandaloneScene(p_SceneInstanceID);
            }

            bool l_HasSecondScript = std::count_if(m_PlayerSceneSecondInstanceId.begin(), m_PlayerSceneSecondInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasSecondScript)
            {
                if (p_Event == "credit")
                    p_Player->CancelStandaloneScene(p_SceneInstanceID);
            }

            bool l_HasThirdScript = std::count_if(m_PlayerSceneThirdInstanceId.begin(), m_PlayerSceneThirdInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasThirdScript)
            {
                if (p_Event == "Hundred")
                {
                    p_Player->MonsterSay("EVENT HUNDRED - third script ! (validates obj 100/100)", LANG_UNIVERSAL, p_Player->GetGUID());
                    p_Player->QuestObjectiveSatisfy(82066, 1);
                }
                else if (p_Event == "Update")
                {
                    p_Player->MonsterSay("NOTHING DEFINED THERE", LANG_UNIVERSAL, p_Player->GetGUID());
                }
                else if (p_Event == "Credit")
                {
                    p_Player->MonsterSay("EVENT CREDIT - third script ! (validates obj escape + play rocks scene)", LANG_UNIVERSAL, p_Player->GetGUID());
                    p_Player->QuestObjectiveSatisfy(82140, 1);
                    p_Player->PlayScene(956, p_Player);
                }
            }
        }
};
playerScript_kill_your_hundred* g_KillYourHundredPlayerScript = nullptr;

/// Kill Your Hundred Passive Scene Object
class playerScript_cavern_teleport : public PlayerScript
{
    public:
        playerScript_cavern_teleport() : PlayerScript("playerScript_cavern_teleport") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34741 || p_Player && p_Quest && p_Quest->GetQuestId() == 34436)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_sceneInstanceId)
        {
            if (p_sceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
                p_Player->TeleportTo(1265, 4537.817f, -2291.243f, 32.451f, 0.728175f);
                p_Player->PlayScene(922, p_Player);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34741 || p_Player && p_Quest && p_Quest->GetQuestId() == 34436)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
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

                if (p_Event == "Teleport")
                {
                    p_Player->TeleportTo(1265, 4537.817f, -2291.243f, 32.451f, 0.728175f);
                    p_Player->PlayScene(922, p_Player);
                }
            }
        }
};
playerScript_cavern_teleport* g_CavernTeleportPlayerScript = nullptr;

/// Kill Your Hundred Passive Scene Object
class playerScript_map_shift : public PlayerScript
{
    public:
        playerScript_map_shift() : PlayerScript("playerScript_map_shift") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34741 || p_Player && p_Quest && p_Quest->GetQuestId() == 34436)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_sceneInstanceId)
        {
            if (p_sceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
                p_Player->TeleportTo(1265, 4537.817f, -2291.243f, 32.451f, 0.728175f);
                p_Player->PlayScene(922, p_Player);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34741 || p_Player && p_Quest && p_Quest->GetQuestId() == 34436)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
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

                if (p_Event == "Force Phase")
                {

                    uint64 l_Guid = p_Player->GetGUID();
                    p_Player->AddCriticalOperation([l_Guid]() -> void
                    {
                        Player * l_Player = HashMapHolder<Player>::Find(l_Guid);

                        if (!l_Player)
                            return;

                       // l_Player->SwitchToPhasedMap(1307);
//                        l_Player->SetPhaseMask(0x800000, true);
                    });
                }
            }
        }
};
playerScript_map_shift* g_MapShiftPlayerScript = nullptr;

/// Kill Your Hundred Passive Scene Object
class playerScript_gunpowder_plot : public PlayerScript
{
    public:
        playerScript_gunpowder_plot() : PlayerScript("playerScript_gunpowder_plot") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34987)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_sceneInstanceId)
        {
            if (p_sceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
                p_Player->TeleportTo(1265, 4537.817f, -2291.243f, 32.451f, 0.728175f);
                p_Player->PlayScene(922, p_Player);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34987)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
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

                Creature* l_Speaker = p_Player->FindNearestCreature(78569, 5.0f);

                if (!l_Speaker || !l_Speaker->AI())
                    return;

                if (p_Event == "TalkA")
                    l_Speaker->AI()->Talk(0);
                else if (p_Event == "TalkB")
                    l_Speaker->AI()->Talk(1);
                else if (p_Event == "TalkC")
                    l_Speaker->AI()->Talk(2); ///< p_Player->PlayScene(893, p_Player);
                else if (p_Event == "TalkD")
                    l_Speaker->AI()->Talk(3);
            }
        }
};
playerScript_gunpowder_plot* g_GunpowderPlotPlayerScript = nullptr;

/// Kill Your Hundred Passive Scene Object
class playerScript_taste_of_iron : public PlayerScript
{
    public:
        playerScript_taste_of_iron() : PlayerScript("playerScript_taste_of_iron") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34445)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_sceneInstanceId)
        {
            if (p_sceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest)
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == 34445)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneTriggerEvent(Player * p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                if (p_Event == "Credit")
                    p_Player->QuestObjectiveSatisfy(80016, 1);
                else if (p_Event == "CancelGame")
                {
                    p_Player->CancelStandaloneScene(p_SceneInstanceID);
                    p_Player->ExitVehicle();
                    p_Player->RemoveAura(164042);
                    p_Player->SummonCreature(80521, 4063.77f, -2020.12f, 75.4733f, p_Player->GetOrientation(), TEMPSUMMON_TIMED_DESPAWN, 60, p_Player->GetGUID());
                }
            }
        }
};
playerScript_taste_of_iron* g_TaseOfIronPlayerScript = nullptr;

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

                if (this->CheckPosition(eCreatureIds::Moriccalas))
                {
                    if (me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE))
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                    me->GetMotionMaster()->MoveCharge(4094.678467f, -2318.3186f, 64.6841f, 10.0f, 1);
                    me->SetReactState(REACT_PASSIVE);
                    me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                }
                else if (this->CheckPosition(eCreatureIds::Mumper))
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

                if (this->CheckPosition(eCreatureIds::Moriccalas))
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
                else if (this->CheckPosition(eCreatureIds::Mumper))
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

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == TanaanActions::ActionSummonHandBrawler)
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, me, 80.0f);
                    for (Player* l_Player : l_PlayerList)
                    {
                        if (l_Player->GetQuestObjectiveCounter(273713) < 99)
                            me->SummonCreature(TanaanCreatures::NpcShatteredHandBrawler, g_ShatteredHandSpawn[urand(0, 3)]);

                        break;
                    }
                }
            }
        };
};

/// 79097 - Kargath Bladefist
class npc_shattered_hand_brawler : public CreatureScript
{
    public:
        npc_shattered_hand_brawler() : CreatureScript("npc_shattered_hand_brawler") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_shattered_hand_brawlerAI(creature);
        }

        struct npc_shattered_hand_brawlerAI : public ScriptedAI
        {
            npc_shattered_hand_brawlerAI(Creature* creature) : ScriptedAI(creature) { }

            enum PosDefine
            {
                PosExtremeRight = 0,
                PosRight        = 1,
                PosLeft         = 2,
                PosExtremeLeft  = 3
            };

            void JustDied(Unit* p_Killer)
            {
                if (p_Killer->GetTypeId() != TYPEID_PLAYER)
                    return;

                Player* l_Player = p_Killer->ToPlayer();
                Position l_Pos;
                l_Player->GetPosition(&l_Pos);

                if (Creature* l_Kargath = me->FindNearestCreature(TanaanCreatures::NpcKargath, 80.0f))
                {
                    if (l_Kargath->GetAI())
                        l_Kargath->AI()->DoAction(TanaanActions::ActionSummonHandBrawler);
                }

                if (l_Player->GetQuestObjectiveCounter(273763) <= 98 && l_Player->GetQuestObjectiveCounter(273713) <= 98)
                {
                    if (l_Player->GetQuestObjectiveCounter(273713))
                        l_Player->QuestObjectiveSatisfy(82142, 1);

                    l_Player->QuestObjectiveSatisfy(82066, 1);
                }

                if (l_Player->GetQuestObjectiveCounter(273763) == 98 && l_Player->GetQuestObjectiveCounter(273713) == 99)
                {
                    if (g_KillYourHundredPlayerScript)
                        g_KillYourHundredPlayerScript->m_PlayerSceneThirdInstanceId[l_Player->GetGUID()] = l_Player->PlayStandaloneScene(946, 16, l_Pos);

                    l_Player->QuestObjectiveSatisfy(82142, 1);
                }
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (p_Summoner->ToCreature() && p_Summoner->GetEntry() == TanaanCreatures::NpcKargath)
                {
                    for (uint8 l_Itr = 0; l_Itr < 4; l_Itr++)
                    {
                        if (CheckPosition(me, g_ShatteredHandSpawn[l_Itr].m_positionX, g_ShatteredHandSpawn[l_Itr].m_positionY))
                        {
                            switch (l_Itr)
                            {
                                case PosDefine::PosExtremeRight:
                                    me->GetMotionMaster()->MoveJump(4419.021f, -2806.122f, 4.721f, 10.0f, 20.0f, 4.611885f, 1);
                                    break;
                                case PosDefine::PosRight:
                                    me->GetMotionMaster()->MoveJump(4407.139f, -2804.924f, 4.721f, 10.0f, 20.0f, 4.611885f, 1);
                                    break;
                                case PosDefine::PosLeft:
                                    me->GetMotionMaster()->MoveJump(4393.716f, -2804.005f, 4.612f, 10.0f, 20.0f, 4.611f, 1);
                                    break;
                                case PosDefine::PosExtremeLeft:
                                    me->GetMotionMaster()->MoveJump(4384.470f, -2799.359f, 4.281f, 10.0f, 20.0f, 4.611f, 1);
                                    break;
                                default:
                                    break;
                            }

                            break;
                        }
                    }
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != EFFECT_MOTION_TYPE)
                    return;

                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 80.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (me->isInFront(l_Player) && l_Player->HasQuest(TanaanQuests::QuestKillYourHundred) && l_Player->GetQuestObjectiveCounter(273763) < 99)
                    {
                        AttackStart(l_Player);
                        break;
                    }
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_archmage_khadgar_shadowmoon : public CreatureScript
{
    public:
        npc_archmage_khadgar_shadowmoon() : CreatureScript("npc_archmage_khadgar_shadowmoon")
        {
        }

        bool OnQuestReward(Player * p_Player, Creature * p_Creature, const Quest * p_Quest, uint32 p_Option)
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestKillYourHundred:
                {
                    p_Creature->MonsterSay("EVENT HUNDRED - third script ! (validates obj 100/100)", LANG_UNIVERSAL, p_Player->GetGUID());

                    if (p_Creature->GetAI())
                    {
                        p_Creature->AI()->SetData(0, 1500);
                        p_Creature->AI()->SetGUID(p_Player->GetGUID(), 0);
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
            return new npc_archmage_khadgar_shadowmoonAI(creature);
        }

        struct npc_archmage_khadgar_shadowmoonAI : public ScriptedAI
        {
            npc_archmage_khadgar_shadowmoonAI(Creature* creature) : ScriptedAI(creature) { }

            uint16 m_SceneTimer;
            uint64 p_PlayerGuid;

            void Reset()
            {
                p_PlayerGuid = 0;
                m_SceneTimer = 0;
            }

            void SetData(uint32 p_Id, uint32 p_Value)
            {
                m_SceneTimer = (uint16)p_Value;
            }

            void SetGUID(uint64 p_Guid, int32 p_Id)
            {
                p_PlayerGuid = p_Guid;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_SceneTimer)
                {
                    if (m_SceneTimer <= m_SceneTimer)
                    {
                        if (p_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, p_PlayerGuid))
                                l_Player->PlayScene(948, l_Player);
                        }
                        m_SceneTimer = 0;
                    }
                    else
                        m_SceneTimer -= p_Diff;
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_tanaan_ungra : public CreatureScript
{
    public:
        npc_tanaan_ungra() : CreatureScript("npc_tanaan_ungra")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_ungraAI(creature);
        }

        struct npc_tanaan_ungraAI : public ScriptedAI
        {
            npc_tanaan_ungraAI(Creature* creature) : ScriptedAI(creature) { }

            void JustDied(Unit* p_Killer)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 20.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (!l_Player->GetQuestObjectiveCounter(272926))
                        l_Player->KilledMonsterCredit(me->GetEntry());
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_taskmaster_gurran : public CreatureScript
{
    public:
        npc_taskmaster_gurran() : CreatureScript("npc_taskmaster_gurran")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_taskmaster_gurranAI(creature);
        }

        struct npc_taskmaster_gurranAI : public ScriptedAI
        {
            npc_taskmaster_gurranAI(Creature* creature) : ScriptedAI(creature) { }

            void JustDied(Unit* p_Killer)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 20.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (!l_Player->GetQuestObjectiveCounter(272927))
                        l_Player->KilledMonsterCredit(me->GetEntry());
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_ankova_the_fallen : public CreatureScript
{
    public:
        npc_ankova_the_fallen() : CreatureScript("npc_ankova_the_fallen")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_ankova_the_fallenAI(creature);
        }

        struct npc_ankova_the_fallenAI : public ScriptedAI
        {
            npc_ankova_the_fallenAI(Creature* creature) : ScriptedAI(creature) { }

            void JustDied(Unit* p_Killer)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 20.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (!l_Player->GetQuestObjectiveCounter(272929))
                        l_Player->KilledMonsterCredit(me->GetEntry());
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_tanaan_yrel : public CreatureScript
{
    public:
        npc_tanaan_yrel() : CreatureScript("npc_tanaan_yrel")
        {
        }

        bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
        {
            if (p_Quest->GetQuestId() == 34434 || p_Quest->GetQuestId() == 34740)
            {
                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);

                p_Player->SummonCreature(78994, l_Pos);
            }

            return false;
        }
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_tanaan_yrel_summon : public CreatureScript
{
    public:
        npc_tanaan_yrel_summon() : CreatureScript("npc_tanaan_yrel_summon")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_yrel_summonAI(creature);
        }

        struct npc_tanaan_yrel_summonAI : public ScriptedAI
        {
            npc_tanaan_yrel_summonAI(Creature* creature) : ScriptedAI(creature)
            {
                m_IsSummoned = false;
                m_PlayerGuid = 0;
            }

            enum eEvents
            {
                EventCheckStopFollow = 1,
                EventContinueRun     = 2
            };

            bool m_IsSummoned;
            EventMap m_Events;
            uint64 m_PlayerGuid;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_IsSummoned = true;

                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                m_PlayerGuid = p_Summoner->GetGUID();

                me->GetMotionMaster()->MoveFollow(p_Summoner, 1.0f, 1.0f);

                m_Events.ScheduleEvent(eEvents::EventCheckStopFollow, 6000);
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                switch (p_Id)
                {
                    case 1:
                        me->GetMotionMaster()->MovePoint(2, 4540.006f, -2501.200f, 20.08f);
                        break;
                    case 2:
                        me->GetMotionMaster()->MovePoint(3, 4541.180f, -2474.347f, 25.218f);
                        break;
                    case 3:
                        me->GetMotionMaster()->MovePoint(4, 4513.743f, -2473.987f, 25.298f);
                        break;
                    case 4:
                        me->GetMotionMaster()->MovePoint(5, 4516.58f, -2495.62f, 25.872f);
                        break;
                    case 5:
                    {
                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                if (l_Player->HasQuest(34434) || l_Player->HasQuest(34740))
                                    l_Player->QuestObjectiveSatisfy(79794, 1);

                                me->DespawnOrUnsummon();
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_IsSummoned)
                {
                    m_Events.Update(p_Diff);

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventCheckStopFollow:
                        {
                            if (Creature* l_BaseCheck = GetClosestCreatureWithEntry(me, 79537, 50.0f))
                            {
                                me->GetMotionMaster()->Clear();
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                                if (me->HasUnitState(UNIT_STATE_FOLLOW))
                                    me->ClearUnitState(UNIT_STATE_FOLLOW);

                                if (m_PlayerGuid)
                                    me->MonsterSay("TEST POSITION", LANG_UNIVERSAL, m_PlayerGuid);

                                m_Events.CancelEvent(eEvents::EventCheckStopFollow);
                                m_Events.ScheduleEvent(eEvents::EventContinueRun, 1000);
                            }
                            else
                                m_Events.ScheduleEvent(eEvents::EventCheckStopFollow, 800);

                            break;
                        }
                        case eEvents::EventContinueRun:
                        {
                            me->GetMotionMaster()->MovePoint(1, 4539.223f, -2521.753f, 14.361f);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_exarch_maladaar_tanaan_cave : public CreatureScript
{
    public:
        npc_exarch_maladaar_tanaan_cave() : CreatureScript("npc_exarch_maladaar_tanaan_cave")
        {
        }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_exarch_maladaar_tanaan_caveAI(p_Creature);
        }

        bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
        {
            if (p_Quest->GetQuestId() == 34436 || p_Quest->GetQuestId() == 34741)
            {
                if (p_Creature->GetAI())
                {
                    p_Creature->AI()->SetGUID(p_Player->GetGUID(), 0);
                    p_Creature->AI()->DoAction(1);
                }
            }

            return false;
        }

        struct npc_exarch_maladaar_tanaan_caveAI : public ScriptedAI
        {
            npc_exarch_maladaar_tanaan_caveAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_PlayerGuid = 0;
            }

            uint64 m_PlayerGuid;

            void SetGUID(uint64 p_Guid, int32 p_Id)
            {
                m_PlayerGuid = p_Guid;
            }

            void DoAction(int32 const p_Id)
            {
                Talk(0);

                if (m_PlayerGuid)
                {
                    if (GameObject* l_Gob = GetClosestGameObjectWithEntry(me, 233197, 40.0f))
                    {
                        if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            l_Gob->UseDoorOrButton(120000, false, l_Player);
                    }
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_keli_dan_the_breaker : public CreatureScript
{
    public:
        npc_keli_dan_the_breaker() : CreatureScript("npc_keli_dan_the_breaker")
        {
        }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_keli_dan_the_breakerAI(p_Creature);
        }

        struct npc_keli_dan_the_breakerAI : public ScriptedAI
        {
            npc_keli_dan_the_breakerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void EnterCombat(Unit* p_Victim)
            {
                if (GameObject* l_Gob = GetClosestGameObjectWithEntry(me, 233197, 40.0f))
                {
                    l_Gob->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
                    l_Gob->SetGoState(GO_STATE_READY);
                }
            }

            void JustDied(Unit* p_Killer)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 36.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->HasQuest(34436) || l_Player->HasQuest(34741))
                    {
                        Position l_Pos;

                        l_Player->GetPosition(&l_Pos);
                        l_Player->KilledMonsterCredit(me->GetEntry());
                        g_CavernTeleportPlayerScript->m_PlayerSceneFirstInstanceId[l_Player->GetGUID()] = l_Player->PlayStandaloneScene(952, 16, l_Pos);
                    }
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_cordana_felsong_blackrock : public CreatureScript
{
    public:
        npc_cordana_felsong_blackrock() : CreatureScript("npc_cordana_felsong_blackrock")
        {
        }

        bool OnQuestAccept(Player * p_Player, Creature * p_Creature, const Quest * p_Quest)
        {
            if (p_Quest->GetQuestId() == 34439)
            {
                p_Player->PlayScene(896, p_Player);
                p_Player->SummonCreature(79675, 4609.370f, -2243.42f, 14.849f);
                p_Player->SummonCreature(78430, 4613.3f, -2246.3f, 15.19f);
            }

            return false;
        }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_cordana_felsong_blackrockAI(p_Creature);
        }

        struct npc_cordana_felsong_blackrockAI : public ScriptedAI
        {
            npc_cordana_felsong_blackrockAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_PlayerGuid = 0;
                m_Summoned = false;
            }

            uint64 m_PlayerGuid;
            bool m_Summoned;

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Summoned = true;

                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                m_PlayerGuid = p_Summoner->GetGUID();

                if (Player* l_Player = p_Summoner->ToPlayer())
                    me->GetMotionMaster()->MoveFollow(l_Player, 0.2f, 0.3f);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!m_Summoned)
                    return;

                if (Player* l_EscortedPlayer = sObjectAccessor->FindPlayer(m_PlayerGuid))
                {
                    if (Unit* l_Target = l_EscortedPlayer->getVictim())
                    {
                        if (!me->IsWithinMeleeRange(l_Target))
                        {
                            Position l_Pos;
                            l_EscortedPlayer->GetPosition(&l_Pos);

                            me->GetMotionMaster()->MoveCharge(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, me->GetSpeed(MOVE_RUN));
                            return;
                        }

                        me->Attack(l_Target, true);
                        DoMeleeAttackIfReady();
                        return;
                    }

                    if ((l_EscortedPlayer->GetQuestStatus(34439) == QUEST_STATUS_COMPLETE || l_EscortedPlayer->GetQuestStatus(34439) == QUEST_STATUS_REWARDED || l_EscortedPlayer->GetQuestStatus(34439) == QUEST_STATUS_NONE))
                        me->DespawnOrUnsummon();
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_lady_liadrin_blackrock : public CreatureScript
{
    public:
        npc_lady_liadrin_blackrock() : CreatureScript("npc_lady_liadrin_blackrock")
        {
        }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_lady_liadrin_blackrockAI(p_Creature);
        }

        struct npc_lady_liadrin_blackrockAI : public ScriptedAI
        {
            npc_lady_liadrin_blackrockAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_PlayerGuid = 0;
                m_Summoned = false;
            }

            uint64 m_PlayerGuid;
            bool m_Summoned;

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Summoned = true;

                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                m_PlayerGuid = p_Summoner->GetGUID();

                if (Player* l_Player = p_Summoner->ToPlayer())
                    me->GetMotionMaster()->MoveFollow(l_Player, 0.2f, 5.7f);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!m_Summoned)
                    return;

                if (Player* l_EscortedPlayer = sObjectAccessor->FindPlayer(m_PlayerGuid))
                {
                    if (Unit* l_Target = l_EscortedPlayer->getVictim())
                    {
                        if (!me->IsWithinMeleeRange(l_Target))
                        {
                            Position l_Pos;
                            l_EscortedPlayer->GetPosition(&l_Pos);

                            me->GetMotionMaster()->MoveCharge(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, me->GetSpeed(MOVE_RUN));
                            return;
                        }

                        me->Attack(l_Target, true);
                        DoMeleeAttackIfReady();
                        return;
                    }

                    if ((l_EscortedPlayer->GetQuestStatus(34439) == QUEST_STATUS_COMPLETE || l_EscortedPlayer->GetQuestStatus(34439) == QUEST_STATUS_REWARDED || l_EscortedPlayer->GetQuestStatus(34439) == QUEST_STATUS_NONE))
                        me->DespawnOrUnsummon();
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_blackrock_grunt : public CreatureScript
{
    public:
        npc_blackrock_grunt() : CreatureScript("npc_blackrock_grunt")
        {
        }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_lady_liadrin_blackrockAI(p_Creature);
        }

        struct npc_lady_liadrin_blackrockAI : public ScriptedAI
        {
            npc_lady_liadrin_blackrockAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void JustDied(Unit* p_Killer)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 3.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->GetQuestStatus(34439) == QUEST_STATUS_INCOMPLETE)
                        l_Player->KilledMonsterCredit(80786);
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_ogron_warcrusher : public CreatureScript
{
    public:
        npc_ogron_warcrusher() : CreatureScript("npc_ogron_warcrusher")
        {
        }

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new npc_ogron_warcrusherAI(p_Creature);
        }

        struct npc_ogron_warcrusherAI : public ScriptedAI
        {
            npc_ogron_warcrusherAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void JustDied(Unit* p_Killer)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 3.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->GetQuestStatus(34439) == QUEST_STATUS_INCOMPLETE)
                        l_Player->KilledMonsterCredit(80775);
                }
            }
        };
};

/// Archmage Khadgar - 78561 (Main quest giver/taker)
class npc_farseer_drek_thar : public CreatureScript
{
    public:
        npc_farseer_drek_thar() : CreatureScript("npc_farseer_drek_thar")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest)
        {
            uint64 l_PlayerPhase = p_Player->GetPhaseMask();

            switch (p_Quest->GetQuestId())
            {
                case 34442:
                {
                    p_Player->PlayScene(928, p_Player);
                    break;
                }
                default:
                    break;
            }
            return true;
        }

        bool OnQuestReward(Player * p_Player, Creature * p_Creature, const Quest * p_Quest, uint32 p_Option)
        {
            Position l_Pos;
            p_Player->GetPosition(&l_Pos);

            if (p_Quest->GetQuestId() == 34439 && g_MapShiftPlayerScript)
                g_MapShiftPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(910, 16, l_Pos);

            return false;
        }
};

// Ken-Ken - 60979
class npc_thaelin_darkanvil_tanaan : public CreatureScript
{
    public:
        npc_thaelin_darkanvil_tanaan() : CreatureScript("npc_thaelin_darkanvil_tanaan")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature)
        {
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes, I need you to help me operate that enormous tank.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "I would like to buy from you.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*sender*/, uint32 p_Action)
        {
            p_Player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);
                p_Player->SummonCreature(p_Creature->GetEntry(), l_Pos);
                p_Player->QuestObjectiveSatisfy(80880, 1);
                p_Player->CLOSE_GOSSIP_MENU();
            }
            else if (p_Action == GOSSIP_ACTION_TRADE)
                p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_thaelin_darkanvil_tanaanAI(creature);
        }

        struct npc_thaelin_darkanvil_tanaanAI : public ScriptedAI
        {
            npc_thaelin_darkanvil_tanaanAI(Creature* creature) : ScriptedAI(creature)
            {
                m_PlayerGuid = 0;
            }

            enum eEvents
            {
                EventCheckTalk = 1
            };

            EventMap m_Events;
            uint64 m_PlayerGuid;
            bool m_Summoned;

            void Reset()
            {
                m_Summoned = false;
                m_Events.Reset();
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_Summoned = true;
                m_PlayerGuid = p_Summoner->GetGUID();
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);

                Talk(0);

                if (Player* l_Player = p_Summoner->ToPlayer())
                    me->GetMotionMaster()->MoveFollow(l_Player, 1.0f, 1.0f);

                m_Events.ScheduleEvent(eEvents::EventCheckTalk, 5000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage)
            {
                if (p_Damage >= me->GetHealth())
                    me->SetFullHealth();
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type == FOLLOW_MOTION_TYPE)
                {
                    if (Creature* l_Thaelin = GetClosestCreatureWithEntry(me, 80521, 4.0f))
                    {
                        Talk(3);
                        Position l_Pos;
                        l_Thaelin->GetPosition(&l_Pos);

                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveCharge(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, me->GetSpeed(MOVE_RUN));

                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                                l_Player->QuestObjectiveSatisfy(80887, 1);
                        }
                    }
                }
                else if (p_Type == DISTRACT_MOTION_TYPE)
                {
                    me->DespawnOrUnsummon();
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (!m_Summoned)
                    return;

                if (Player* l_EscortedPlayer = sObjectAccessor->FindPlayer(m_PlayerGuid))
                {
                    if (Unit* l_Target = l_EscortedPlayer->getVictim())
                    {
                        if (!me->IsWithinMeleeRange(l_Target))
                        {
                            Position l_Pos;
                            l_EscortedPlayer->GetPosition(&l_Pos);

                            me->GetMotionMaster()->MoveCharge(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, me->GetSpeed(MOVE_RUN));
                            return;
                        }

                        me->Attack(l_Target, true);
                        DoMeleeAttackIfReady();
                        return;
                    }

                    if ((l_EscortedPlayer->GetQuestStatus(35747) == QUEST_STATUS_COMPLETE || l_EscortedPlayer->GetQuestStatus(35747) == QUEST_STATUS_REWARDED || l_EscortedPlayer->GetQuestStatus(35747) == QUEST_STATUS_NONE))
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                }

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCheckTalk:
                        if (GetClosestCreatureWithEntry(me, 86039, 85.0f))
                            Talk(1);
                        else
                            m_Events.ScheduleEvent(eEvents::EventCheckTalk, 800);
                        break;
                    default:
                        break;
                }
            }
        };
};

// Ken-Ken - 60979
class npc_tanaan_gogluk : public CreatureScript
{
    public:
        npc_tanaan_gogluk() : CreatureScript("npc_tanaan_gogluk")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_goglukAI(creature);
        }

        struct npc_tanaan_goglukAI : public ScriptedAI
        {
            npc_tanaan_goglukAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                Position l_Pos;
                me->GetPosition(&l_Pos);

 //               me->SummonCreature(86691, l_Pos);
 //               me->SummonCreature(86690, l_Pos);
            }

            void SetPassengersFightingOrDespawn(uint8 p_Seat, bool p_Despawn, Vehicle* p_Vehicle, Unit* p_Target = nullptr)
            {
                if (Unit* l_Passenger = p_Vehicle->GetPassenger(p_Seat))
                {
                    if (Creature* l_Creature = l_Passenger->ToCreature())
                    {
                        if (l_Creature->GetAI())
                        {
                            if (!p_Despawn)
                            {
                                l_Creature->SetInCombatWith(p_Target);
                                l_Creature->AI()->DoAction(1);
                            }
                            else
                                l_Creature->DespawnOrUnsummon();
                        }
                    }
                }
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (me->IsVehicle())
                {
                    if (Vehicle* l_Vehicle = me->GetVehicleKit())
                    {
                        SetPassengersFightingOrDespawn(0, false, l_Vehicle, p_Attacker);
                        SetPassengersFightingOrDespawn(1, false, l_Vehicle, p_Attacker);
                    }
                }
            }

            void JustDied(Unit* p_Killer)
            {
                if (me->IsVehicle())
                {
                    if (Vehicle* l_Vehicle = me->GetVehicleKit())
                    {
                        SetPassengersFightingOrDespawn(0, true, l_Vehicle);
                        SetPassengersFightingOrDespawn(1, true, l_Vehicle);
                    }
                }
            }

            void EnterEvadeMode()
            {
                if (me->IsVehicle())
                {
                    if (Vehicle* l_Vehicle = me->GetVehicle())
                    {
                        if (Unit* l_Passenger = l_Vehicle->GetPassenger(0))
                            l_Passenger->CombatStop(true);

                        if (Unit* l_Passenger = l_Vehicle->GetPassenger(1))
                            l_Passenger->CombatStop(true);
                    }
                }
            }
        };
};

// Ken-Ken - 60979
class npc_tanaan_gogluk_adds : public CreatureScript
{
    public:
        npc_tanaan_gogluk_adds() : CreatureScript("npc_tanaan_gogluk_adds")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_gogluk_addsAI(creature);
        }

        struct npc_tanaan_gogluk_addsAI : public ScriptedAI
        {
            npc_tanaan_gogluk_addsAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eEvents
            {
                EventCannonBarrage = 1,
                EventMachineGun    = 2
            };

            void DoAction(int32 const p_Action)
            {
                switch (me->GetEntry())
                {
                    case 86690:
                        m_Events.ScheduleEvent(eEvents::EventCannonBarrage, 1000);
                        break;
                    case 86691:
                        m_Events.ScheduleEvent(eEvents::EventMachineGun, 3000);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCannonBarrage:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 173479, false);
                        m_Events.ScheduleEvent(eEvents::EventCannonBarrage, 80000);
                        break;
                    case eEvents::EventMachineGun:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, 173502, true);
                        m_Events.ScheduleEvent(eEvents::EventMachineGun, 8000);
                        break;
                    default:
                        break;
                }
            }
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
                this->OnDamaged(p_Gameobject, p_Player);

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

/// 233056/233057 - Mark of the Shadowmoon/Bleeding Hollow
class gob_makeshift_plunger : public GameObjectScript
{
    public:
        gob_makeshift_plunger() : GameObjectScript("gob_makeshift_plunger") { }

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject)
        {
            if (p_Player->GetQuestStatus(34987) == QUEST_STATUS_INCOMPLETE)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                g_GunpowderPlotPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(902, 16, l_Pos);
            }
            return false;
        }
};

/// 233056/233057 - Mark of the Shadowmoon/Bleeding Hollow
class gob_worldbreaker_side_turret : public GameObjectScript
{
    public:
        gob_worldbreaker_side_turret() : GameObjectScript("gob_worldbreaker_side_turret")
        {
        }

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject)
        {
            if (p_Player->GetQuestStatus(34445) == QUEST_STATUS_INCOMPLETE)
            {
                ///< Change player phase
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);
                if (!g_TaseOfIronPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
                    g_TaseOfIronPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(871, 16, l_Pos);
            }
            return false;
        }

        struct gob_worldbreaker_side_turretAI : public GameObjectAI
        {
            gob_worldbreaker_side_turretAI(GameObject* p_Go) : GameObjectAI(p_Go) { go->SetCancelAnim(true); }
        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new gob_worldbreaker_side_turretAI(p_Go);
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
//    new npc_vindicator_maraad_tanaan();
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
    new npc_shattered_hand_brawler();
    new npc_archmage_khadgar_shadowmoon();
    new npc_tanaan_ungra();
    new npc_taskmaster_gurran();
    new npc_ankova_the_fallen();
    new npc_tanaan_yrel_summon();
    new npc_tanaan_yrel();
    new npc_exarch_maladaar_tanaan_cave();
    new npc_keli_dan_the_breaker();
    new npc_cordana_felsong_blackrock();
    new npc_lady_liadrin_blackrock();
    new npc_blackrock_grunt();
    new npc_ogron_warcrusher();
    new npc_farseer_drek_thar();
    new npc_thaelin_darkanvil_tanaan();
    new npc_tanaan_gogluk();
    new npc_tanaan_gogluk_adds();
    new gob_static_rune();
    new go_platform_tanaan();
    new go_bleeding_hollow_cage();
    new gob_mark_of_tanaan();
    new gob_makeshift_plunger();
    new gob_worldbreaker_side_turret();
    new map_dark_portal_entrance();
    new playerScript_enter_tanaan();
    new spell_tanaan_inferno();

    g_BlazeOfGloryPlayerScript   = new playerScript_blaze_of_glory();
    g_APotentialAllyPlayerScript = new playerScript_a_potential_ally();
    g_KillYourHundredPlayerScript = new playerScript_kill_your_hundred();
    g_CavernTeleportPlayerScript = new playerScript_cavern_teleport();
    g_MapShiftPlayerScript = new playerScript_map_shift();
    g_GunpowderPlotPlayerScript = new playerScript_gunpowder_plot();
    g_TaseOfIronPlayerScript = new playerScript_taste_of_iron();
}