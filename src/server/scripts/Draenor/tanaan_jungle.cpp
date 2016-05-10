////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "tanaan_jungle.h"
#include "GameObjectAI.h"
#include "NPCHandler.h"
#include "Vehicle.h"
#include "PhaseMgr.h"
#include "Common.h"

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

/// Passive Scene Object
class playerScript_quests_custom_gestion : public PlayerScript
{
    public:
        playerScript_quests_custom_gestion() : PlayerScript("playerScript_quests_custom_gestion") { }

        void OnItemLooted(Player* p_Player, Item* p_Item)
        {
            if (p_Item && p_Item->GetVisibleEntry() == 112323)
                p_Player->AddAura(162676, p_Player);
        }

        void OnQuestComplete(Player* p_Player, Quest const* p_Quest)
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestShadowOfTheWorldbreaker)
                p_Player->RewardQuest(p_Quest, 0, nullptr);
        }
};

/// Passive Scene Object
class playerScript_cave_in : public PlayerScript
{
    public:
        playerScript_cave_in() : PlayerScript("playerScript_cave_in") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_SceneInstanceId) override
        {
            if (m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] == p_SceneInstanceId)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseAfterArena;
                l_PhaseMask |= TanaanPhases::PhaseRocks;
                l_PhaseMask |= TanaanPhases::PhaseOlinLiadrin;
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (p_Event == "EarlyPhase")
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask &= ~TanaanPhases::PhaseAfterArena;
                    l_PhaseMask |= TanaanPhases::PhaseRocks;
                    l_PhaseMask |= TanaanPhases::PhaseOlinLiadrin;
                    p_Player->SetPhaseMask(l_PhaseMask, true);
                }
            }
        }
};
playerScript_cave_in* g_CaveInScript = nullptr;

/// Passive Scene Object
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

        void OnQuestAccept(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestKillYourHundred)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                p_Player->SetPhaseMask(l_PhaseMask, true);

                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);

                Position l_Pos;
                p_Player->GetPosition(&l_Pos);
                p_Player->PlayScene(TanaanSceneObjects::SceneKargathYells, p_Player);
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

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
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

        void OnSceneCancel(Player* p_Player, uint32 p_SceneInstanceId) override
        {
            if (m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] == p_SceneInstanceId)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (!p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjEnterTheArena))
                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditEnterTheArena, 1);

                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseExarchMalaadar;
                l_PhaseMask |= TanaanPhases::PhaseArenaOrcs;

                if (p_Player->GetTeamId() == TEAM_ALLIANCE)
                    l_PhaseMask |= TanaanPhases::PhaseArenaPrisonnersAlly;
                else if (p_Player->GetTeamId() == TEAM_HORDE)
                    l_PhaseMask |= TanaanPhases::PhaseArenaPrisonnersHorde;

                p_Player->SetPhaseMask(l_PhaseMask, true);

                m_PlayerSceneSecondInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneKargathFight, 16, l_Pos);
            }
            else if (m_PlayerSceneThirdInstanceId[p_Player->GetGUID()] == p_SceneInstanceId)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();

                l_PhaseMask &= ~TanaanPhases::PhaseArenaOrcs;
                l_PhaseMask &= ~TanaanPhases::PhaseArenaPrisonnersHorde;
                l_PhaseMask &= ~TanaanPhases::PhaseArenaPrisonnersAlly;
                l_PhaseMask |= TanaanPhases::PhaseAfterArena;

                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
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
                    if (!p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjEnterTheArena))
                        p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditEnterTheArena, 1);

                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask &= ~TanaanPhases::PhaseExarchMalaadar;
                    l_PhaseMask |= TanaanPhases::PhaseArenaOrcs;

                    if (p_Player->GetTeamId() == TEAM_ALLIANCE)
                        l_PhaseMask |= TanaanPhases::PhaseArenaPrisonnersAlly;
                    else if (p_Player->GetTeamId() == TEAM_HORDE)
                        l_PhaseMask |= TanaanPhases::PhaseArenaPrisonnersHorde;

                    p_Player->SetPhaseMask(l_PhaseMask, true);

                    m_PlayerSceneSecondInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneKargathFight, 16, l_Pos);
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
                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditCombattantSlainInArena, 1);
                else if (p_Event == "Update")
                {

                    uint32 l_PhaseMask = p_Player->GetPhaseMask();

                    l_PhaseMask &= ~TanaanPhases::PhaseArenaOrcs;
                    l_PhaseMask &= ~TanaanPhases::PhaseArenaPrisonnersHorde;
                    l_PhaseMask &= ~TanaanPhases::PhaseArenaPrisonnersAlly;
                    l_PhaseMask |= TanaanPhases::PhaseAfterArena;

                    p_Player->SetPhaseMask(l_PhaseMask, true);
                }
                else if (p_Event == "Credit")
                {
                    Position l_Pos;
                    p_Player->GetPosition(&l_Pos);
                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditEscapeKargathArena, 1);
                    p_Player->CancelStandaloneScene(p_SceneInstanceID);

                    if (g_CaveInScript)
                        g_CaveInScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneCaveIn, 16, l_Pos);
                }
            }
        }
};
playerScript_kill_your_hundred* g_KillYourHundredPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_cavern_teleport : public PlayerScript
{
    public:
        playerScript_cavern_teleport() : PlayerScript("playerScript_cavern_teleport") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerHorde || (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerAlly))
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_sceneInstanceId) override
        {
            if (p_sceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask |= TanaanPhases::PhaseCaveLastRock;
                p_Player->SetPhaseMask(l_PhaseMask, true);
                p_Player->TeleportTo(TanaanZones::MapTanaan, 4537.817f, -2291.243f, 32.451f, 0.728175f);
                p_Player->PlayScene(TanaanSceneObjects::SceneFromCaveToRidge, p_Player);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if ((p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerHorde) || (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerAlly))
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
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
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask |= TanaanPhases::PhaseCaveLastRock;

                    p_Player->SetPhaseMask(l_PhaseMask, true);
                    p_Player->TeleportTo(TanaanZones::MapTanaan, 4537.817f, -2291.243f, 32.451f, 0.728175f);
                    p_Player->PlayScene(TanaanSceneObjects::SceneFromCaveToRidge, p_Player);
                }
            }
        }
};
playerScript_cavern_teleport* g_CavernTeleportPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_map_shift : public PlayerScript
{
    public:
        playerScript_map_shift() : PlayerScript("playerScript_map_shift") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if ((p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerHorde) || (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerAlly))
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_sceneInstanceId) override
        {
            if (p_sceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
                std::set<uint32> l_PhaseId, l_Terrainswap, l_InactiveTerrainSwap;
                l_Terrainswap.insert((uint32)TanaanZones::TerrainSwapID);
                l_InactiveTerrainSwap.insert((uint32)TanaanZones::TerrainSwapID);
                p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap, l_InactiveTerrainSwap);

                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhasePoolMobs;
                l_PhaseMask &= ~TanaanPhases::PhaseGroupPool;
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if ((p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerHorde) || (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerAlly))
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
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
                    std::set<uint32> l_PhaseId, l_Terrainswap, l_InactiveTerrainSwap;
                    l_Terrainswap.insert((uint32)TanaanZones::TerrainSwapID);
                    l_InactiveTerrainSwap.insert((uint32)TanaanZones::TerrainSwapID);
                    p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap, l_InactiveTerrainSwap);
                }
            }
        }
};
playerScript_map_shift* g_MapShiftPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_gunpowder_plot : public PlayerScript
{
    public:
        playerScript_gunpowder_plot() : PlayerScript("playerScript_gunpowder_plot") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestTheGunpowderPlot)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestTheGunpowderPlot)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string /*p_Event*/) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                Creature* l_Speaker = p_Player->FindNearestCreature(TanaanCreatures::NpcHanselHeavyHands, 5.0f);

                if (!l_Speaker || !l_Speaker->AI())
                    return;

                /*if (p_Event == "TalkA")
                    /// TALK
                else if (p_Event == "TalkB")
                    /// TALK
                else if (p_Event == "TalkC")
                    /// TALK ///< p_Player->PlayScene(893, p_Player);
                else if (p_Event == "TalkD")
                    /// TALK*/
            }
        }
};
playerScript_gunpowder_plot* g_GunpowderPlotPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_taste_of_iron : public PlayerScript
{
    public:
        playerScript_taste_of_iron() : PlayerScript("playerScript_taste_of_iron") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestATasteOfIron)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                {
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
                    p_Player->RemoveAura(TanaanSpells::SpellTasteOfIronGameAura);
                }
            }
        }

        void OnSceneCancel(Player* p_Player, uint32 p_SceneInstanceId) override
        {
            if (p_SceneInstanceId == m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask |= TanaanPhases::PhaseCannonTurret;
                p_Player->SetPhaseMask(l_PhaseMask, true);

                p_Player->CancelStandaloneScene(p_SceneInstanceId);
                p_Player->ExitVehicle();
                p_Player->RemoveAura(TanaanSpells::SpellTasteOfIronGameAura);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestATasteOfIron)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                {
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
                    p_Player->RemoveAura(TanaanSpells::SpellTasteOfIronGameAura);
                }
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                if (p_Event == "Credit")
                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditIronHordeSlain, 1);
                else if (p_Event == "CancelGame")
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask |= TanaanPhases::PhaseCannonTurret;
                    p_Player->SetPhaseMask(l_PhaseMask, true);

                    p_Player->CancelStandaloneScene(p_SceneInstanceID);
                    p_Player->ExitVehicle();
                    p_Player->RemoveAura(TanaanSpells::SpellTasteOfIronGameAura);
                }
            }
        }
};
playerScript_taste_of_iron* g_TasteOfIronPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_the_home_stretch : public PlayerScript
{
    public:
        playerScript_the_home_stretch() : PlayerScript("playerScript_the_home_stretch") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if ((p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestTheHomeStretchAlly) || p_Quest->GetQuestId() == TanaanQuests::QuestTheHomeStretchHorde)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if ((p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestTheHomeStretchAlly) || p_Quest->GetQuestId() == TanaanQuests::QuestTheHomeStretchHorde)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                if (p_Event == "Damage")
                    p_Player->EnvironmentalDamage(DAMAGE_FIRE, urand(11230, 14320));
            }
        }
};
playerScript_the_home_stretch* g_TheHomeStretchPlayerScript = nullptr;

/// Passive Scene Object
class playerscript_bridge_destruction : public PlayerScript
{
    public:
        playerscript_bridge_destruction() : PlayerScript("playerscript_bridge_destruction") { }

        std::map<uint64, uint32> m_PlayerSceneFirstInstanceId;

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKargatharProvingGrounds)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            if (p_Player && p_Quest && p_Quest->GetQuestId() == TanaanQuests::QuestKargatharProvingGrounds)
            {
                if (m_PlayerSceneFirstInstanceId.find(p_Player->GetGUID()) != m_PlayerSceneFirstInstanceId.end())
                    p_Player->CancelStandaloneScene(m_PlayerSceneFirstInstanceId[p_Player->GetGUID()]);
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasFirstScript = std::count_if(m_PlayerSceneFirstInstanceId.begin(), m_PlayerSceneFirstInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (l_HasFirstScript)
            {
                if (p_Event == "Bridge")
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask &= ~TanaanPhases::PhaseBridge;
                    p_Player->SetPhaseMask(l_PhaseMask, true);
                }

                return;
            }
        }
};
playerscript_bridge_destruction* g_BridgeDestructionPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_blaze_of_glory : public PlayerScript
{
    public:
        playerScript_blaze_of_glory() : PlayerScript("playerScript_blaze_of_glory")
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

        void OnQuestAccept(Player* p_Player, const Quest* p_Quest) override
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

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
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

/// Passive Scene Object
class playerScript_a_potential_ally : public PlayerScript
{
    public:
        playerScript_a_potential_ally() : PlayerScript("playerScript_a_potential_ally") { }

        std::map<uint64, uint32> m_PlayerSceneInstanceId;

        void OnLogin(Player* p_Player) override
        {
            if (p_Player->HasQuest(TanaanQuests::QuestAPotentialAlly) || p_Player->HasQuest(TanaanQuests::QuestAPotentialAllyHorde))
            {
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

        void OnQuestAbandon(Player* p_Player, const Quest* p_Quest) override
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestAPotentialAlly:
                case TanaanQuests::QuestAPotentialAllyHorde:
                    p_Player->CancelStandaloneScene(m_PlayerSceneInstanceId[p_Player->GetGUID()]);
                default:
                    break;
            }
        }

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasScript = std::count_if(m_PlayerSceneInstanceId.begin(), m_PlayerSceneInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (!l_HasScript)
                return;

            if (p_Event == "Fire Gone" || p_Event == "Credit")
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask |= TanaanPhases::PhaseExarchMalaadar;
                p_Player->SetPhaseMask(l_PhaseMask, true);

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
};
playerScript_a_potential_ally* g_APotentialAllyPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_scene_alliance_boat : public PlayerScript
{
    public:
        playerScript_scene_alliance_boat() : PlayerScript("playerScript_scene_alliance_boat") { }

        std::map<uint64, uint32> m_PlayerSceneInstanceId;

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasScript = std::count_if(m_PlayerSceneInstanceId.begin(), m_PlayerSceneInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (!l_HasScript)
                return;

            if (p_Event == "Teleport")
                p_Player->TeleportTo(TanaanZones::MapDraenor, 2308.9621f, 454.9409f, 6.0f, p_Player->GetOrientation());
        }
};
playerScript_scene_alliance_boat* g_SceneAllianceBoatPlayerScript = nullptr;

/// Passive Scene Object
class playerScript_scene_horde_boat : public PlayerScript
{
    public:
        playerScript_scene_horde_boat() : PlayerScript("playerScript_scene_horde_boat") { }

        std::map<uint64, uint32> m_PlayerSceneInstanceId;

        void OnSceneTriggerEvent(Player* p_Player, uint32 p_SceneInstanceID, std::string p_Event) override
        {
            bool l_HasScript = std::count_if(m_PlayerSceneInstanceId.begin(), m_PlayerSceneInstanceId.end(), [p_SceneInstanceID](const std::pair<uint64, uint32> &p_Pair) -> bool
            {
                return p_Pair.second == p_SceneInstanceID;
            });

            if (!l_HasScript)
                return;

            if (p_Event == "Teleport")
                p_Player->TeleportTo(TanaanZones::MapDraenor, 5538.213379f, 5015.2690f, 13.0f, p_Player->GetOrientation());
        }
};
playerScript_scene_horde_boat* g_SceneHordeBoatPlayerScript = nullptr;

/// Tanaan entering
class playerScript_enter_tanaan : public PlayerScript
{
    public:
        playerScript_enter_tanaan() : PlayerScript("playerScript_enter_tanaan") { }

        void UpdatePhaseMask(Player* p_Player)
        {
            /// UPDATE PHASEMASK DEPENDING OF QUESTS
            uint32 l_PhaseMask = p_Player->GetPhaseMask();

            if (p_Player->GetQuestStatus(TanaanQuests::QuestOnslaughtEnd) == QUEST_STATUS_INCOMPLETE)
            {
                l_PhaseMask |= TanaanPhases::PhaseTeronGor;
                l_PhaseMask |= TanaanPhases::PhaseChoGall;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_INCOMPLETE)
            {
                l_PhaseMask |= TanaanPhases::PhaseGulDan;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_COMPLETE)
            {
                l_PhaseMask &= ~TanaanPhases::PhaseEscortGroup;
                l_PhaseMask |= TanaanPhases::PhaseGroupHouse;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_NONE || p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) == QUEST_STATUS_FAILED)
            {
                l_PhaseMask |= TanaanPhases::PhasePortal;
                l_PhaseMask |= TanaanPhases::PhaseBridge;
                l_PhaseMask |= TanaanPhases::PhaseEscortGroup;
            }
            else
                l_PhaseMask &= ~TanaanPhases::PhaseEscortGroup;

            if (p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_REWARDED || p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_COMPLETE)
            {
                l_PhaseMask &= ~TanaanPhases::PhasePortal;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestBledDryAlly) == QUEST_STATUS_INCOMPLETE)
            {
                l_PhaseMask |= TanaanPhases::PhaseEasternCageAlly;
                l_PhaseMask |= TanaanPhases::PhaseSouthernCageAlly;
            }
            else if (p_Player->GetQuestStatus(TanaanQuests::QuestBledDryHorde) == QUEST_STATUS_INCOMPLETE)
            {
                l_PhaseMask |= TanaanPhases::PhaseEasternCageHorde;
                l_PhaseMask |= TanaanPhases::PhaseSouthernCageHorde;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) != QUEST_STATUS_NONE && p_Player->GetQuestStatus(TanaanQuests::QuestCostOfWar) != QUEST_STATUS_FAILED)
            {
                l_PhaseMask &= ~TanaanPhases::PhaseGroupHouse;
                l_PhaseMask |= TanaanPhases::PhaseAriokAltar;
                l_PhaseMask |= TanaanPhases::PhaseAltarGroup;
            }

            /// ALTAR ALTERCATION QUEST
            if (!p_Player->GetQuestObjectiveCounter(273075))
            {
                l_PhaseMask |= TanaanPhases::PhaseAriokAltar;
            }
            else if (p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_COMPLETE || (p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_REWARDED))
            {
                l_PhaseMask &= ~TanaanPhases::PhaseAriokAltar;
                l_PhaseMask &= ~TanaanPhases::PhaseAltarGroup;

                l_PhaseMask |= TanaanPhases::PhaseKhadgarBridge;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestKargatharProvingGrounds) == QUEST_STATUS_REWARDED)
            {
                l_PhaseMask &= ~TanaanPhases::PhaseKhadgarBridge;
                l_PhaseMask |= TanaanPhases::PhaseGroupKillYourHundred;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestAPotentialAlly) == QUEST_STATUS_COMPLETE || p_Player->GetQuestStatus(TanaanQuests::QuestAPotentialAlly) == QUEST_STATUS_REWARDED)
                l_PhaseMask |= TanaanPhases::PhaseExarchMalaadar;

            if (p_Player->GetQuestStatus(TanaanQuests::QuestKillYourHundred) == QUEST_STATUS_INCOMPLETE)
            {
                l_PhaseMask &= ~TanaanPhases::PhaseExarchMalaadar;
                l_PhaseMask &= ~TanaanPhases::PhaseGroupKillYourHundred;
            }
            else if (p_Player->GetQuestStatus(TanaanQuests::QuestKillYourHundred) == QUEST_STATUS_COMPLETE)
            {
                p_Player->NearTeleportTo(4511.1216f, -2637.9119f, 2.4f, p_Player->GetOrientation());
                l_PhaseMask |= TanaanPhases::PhaseCaveGroup;
                l_PhaseMask |= TanaanPhases::PhaseOlinLiadrin;
            }
            else if (p_Player->GetQuestStatus(TanaanQuests::QuestKillYourHundred) == QUEST_STATUS_REWARDED)
            {
                l_PhaseMask |= TanaanPhases::PhaseCaveGroup;
                l_PhaseMask |= TanaanPhases::PhaseYrelQuestGiver;
                l_PhaseMask &= ~TanaanPhases::PhaseOlinLiadrin;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestYrelTanaan) == QUEST_STATUS_COMPLETE || p_Player->GetQuestStatus(TanaanQuests::QuestYrelHorde) == QUEST_STATUS_COMPLETE)
            {
                l_PhaseMask &= ~TanaanPhases::PhaseYrelQuestGiver;
                l_PhaseMask |= TanaanPhases::PhaseYrelQuestTaker;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestKeliDanTheBreakerAlly) == QUEST_STATUS_COMPLETE ||
                p_Player->GetQuestStatus(TanaanQuests::QuestKeliDanTheBreakerAlly) == QUEST_STATUS_REWARDED ||
                p_Player->GetQuestStatus(TanaanQuests::QuestKeliDanTheBreakerHorde) == QUEST_STATUS_COMPLETE ||
                p_Player->GetQuestStatus(TanaanQuests::QuestKeliDanTheBreakerHorde) == QUEST_STATUS_REWARDED)
            {
                l_PhaseMask |= TanaanPhases::PhaseCaveLastRock;
                l_PhaseMask |= TanaanPhases::PhasePoolMobs;
                l_PhaseMask |= TanaanPhases::PhaseGroupPool;
                l_PhaseMask |= TanaanPhases::PhaseLiadrinCordana;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_REWARDED)
            {
                l_PhaseMask &= ~TanaanPhases::PhasePoolMobs;
                l_PhaseMask &= ~TanaanPhases::PhaseGroupPool;
                l_PhaseMask &= ~TanaanPhases::PhaseLiadrinCordana;

                std::set<uint32> l_PhaseId, l_Terrainswap, l_InactiveTerrainSwap;
                l_Terrainswap.insert((uint32)TanaanZones::TerrainSwapID);
                l_InactiveTerrainSwap.insert((uint32)TanaanZones::TerrainSwapID);

                p_Player->GetSession()->SendSetPhaseShift(l_PhaseId, l_Terrainswap, l_InactiveTerrainSwap);
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestGaNarOfTheFrostwolf) != QUEST_STATUS_NONE)
            {
                l_PhaseMask |= TanaanPhases::PhaseThaelinQuestGiver;
                l_PhaseMask |= TanaanPhases::PhaseCannonTurret;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestTakingATripToTheTopOfTheTank) != QUEST_STATUS_NONE)
            {
                l_PhaseMask &= ~TanaanPhases::PhaseThaelinQuestGiver;
                l_PhaseMask |= TanaanPhases::PhaseThaelinTurret;
                l_PhaseMask |= TanaanPhases::PhasePortalDestroyed;
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestTheHomeStretchAlly) == QUEST_STATUS_COMPLETE || p_Player->GetQuestStatus(TanaanQuests::QuestTheHomeStretchHorde) == QUEST_STATUS_COMPLETE)
                l_PhaseMask |= TanaanPhases::PhaseFinalePhase;

            p_Player->SetPhaseMask(l_PhaseMask, true);
        }

        void OnUpdateZone(Player* p_Player, uint32 p_NewZoneId, uint32 p_OldZoneID, uint32 p_NewAreaId) override
        {
            if (p_NewZoneId != TanaanZones::ZoneTanaanJungle)
                return;

            /// To Tanaan from another Zone
            if (p_OldZoneID != TanaanZones::ZoneTanaanJungle)
            {
                /// ADD BEGINNING QUESTS
                if (p_Player->GetQuestStatus(TanaanQuests::QuestStartDraenor) == QUEST_STATUS_INCOMPLETE)
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestStartDraenor))
                    {
                        p_Player->CompleteQuest(TanaanQuests::QuestStartDraenor);
                        p_Player->RewardQuest(l_Quest, 0, nullptr, false);
                    }
                }

                if (p_Player->GetQuestStatus(TanaanQuests::QuestAzerothLastStand) == QUEST_STATUS_NONE)
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestAzerothLastStand))
                        p_Player->AddQuest(l_Quest, nullptr);
                }

                UpdatePhaseMask(p_Player);
            }
            else
            {
                /// INTERACT WITH BLACKROCK TRIGGER
                switch (p_NewAreaId)
                {
                    case TanaanZones::AreaTheDarkPortal:
                    {
                        p_Player->PlayScene(TanaanSceneObjects::SceneSoulTrain, p_Player);
                        break;
                    }
                    case TanaanZones::AreaBlackRockQuarry:
                    {
                        switch (p_Player->GetTeamId())
                        {
                            case TEAM_ALLIANCE:
                            {
                                if (p_Player->GetQuestStatus(TanaanQuests::QuestKeliDanTheBreakerAlly) == QUEST_STATUS_COMPLETE)
                                {
                                    if (Creature* l_Creature = GetClosestCreatureWithEntry(p_Player, TanaanCreatures::NpcBlackRockTrigger, 100.0f))
                                    {
                                        if (l_Creature->GetAI())
                                            l_Creature->AI()->SetGUID(p_Player->GetGUID());
                                    }
                                }
                                break;
                            }
                            case TEAM_HORDE:
                            {
                                if (p_Player->GetQuestStatus(TanaanQuests::QuestKeliDanTheBreakerHorde) == QUEST_STATUS_COMPLETE)
                                {
                                    if (Creature* l_Creature = GetClosestCreatureWithEntry(p_Player, TanaanCreatures::NpcBlackRockTrigger, 100.0f))
                                    {
                                        if (l_Creature->GetAI())
                                            l_Creature->AI()->SetGUID(p_Player->GetGUID());
                                    }
                                }
                                break;
                            }
                            /// This can be happens
                            case TEAM_NEUTRAL:
                                break;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        void OnLogin(Player* p_Player) override
        {
            if (p_Player->GetZoneId() == TanaanZones::ZoneTanaanJungle)
                UpdatePhaseMask(p_Player);
        }

        void OnQuestAbandon(Player* p_Player, const Quest* /*p_Quest*/) override
        {
            if (p_Player->GetZoneId() == TanaanZones::ZoneTanaanJungle)
                UpdatePhaseMask(p_Player);
        }

        void OnQuestReward(Player* p_Player, const Quest* p_Quest) override
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestAzerothLastStand:
                {
                    if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestOnslaughtEnd))
                    {
                        uint32 l_PhaseMask = p_Player->GetPhaseMask();

                        l_PhaseMask |= TanaanPhases::PhaseTeronGor;
                        l_PhaseMask |= TanaanPhases::PhaseChoGall;

                        p_Player->SetPhaseMask(l_PhaseMask, true);
                        p_Player->AddQuest(l_Quest, nullptr);
                    }
                    break;
                }
                default:
                    break;
            }
        }

        void OnObjectiveValidate(Player* p_Player, uint32 p_QuestId, uint32 p_ObjectiveId) override
        {
            if (p_QuestId == TanaanQuests::QuestThePortalPower && p_ObjectiveId == TanaanQuestObjectives::ObjEnterGulDanPrison)
                p_Player->PlayScene(TanaanSceneObjects::SceneGulDanReavel, p_Player);
        }
};

/// Tanaan entering
class playerScript_add_tanaan_for_max_level : public PlayerScript
{
    public:
        playerScript_add_tanaan_for_max_level() : PlayerScript("playerScript_add_tanaan_for_max_level") { }

        void OnLogin(Player* p_Player) override
        {
            if (p_Player->GetMapId() != TanaanZones::MapTanaan && p_Player->getLevel() == 100 && p_Player->GetQuestStatus(TanaanQuests::QuestStartDraenor) == QUEST_STATUS_NONE)
            {
                if (const Quest* l_StartDraenor = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestStartDraenor))
                    p_Player->AddQuest(l_StartDraenor, nullptr);
            }
        }
};

/// 1265 - Tanaan Map
class map_dark_portal_entrance : public WorldMapScript
{
    public:
        map_dark_portal_entrance() : WorldMapScript("map_dark_portal_entrance", TanaanZones::MapTanaan) { }

        void OnPlayerEnter(Map* p_Map, Player* /*p_Player*/) override { p_Map->SetObjectVisibility(350.0f); }
};

/// Archmage Khadgar - 78558/78559 (Main quest giver/taker)
class npc_archmage_khadgar : public CreatureScript
{
    public:
        npc_archmage_khadgar() : CreatureScript("npc_archmage_khadgar")
        {
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/) override
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestCostOfWar:
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestBlazeOfGlory))
                        p_Player->AddQuest(l_Quest, p_Creature);

                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask |= TanaanPhases::PhaseAltarGroup;
                    l_PhaseMask |= TanaanPhases::PhaseAriokAltar;
                    l_PhaseMask &= ~TanaanPhases::PhaseGroupHouse;
                    p_Player->SetPhaseMask(l_PhaseMask, true);

                    break;
                }
                case TanaanQuests::QuestBledDryAlly:
                case TanaanQuests::QuestBledDryHorde:
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask &= ~TanaanPhases::PhaseEasternCageHorde;
                    l_PhaseMask &= ~TanaanPhases::PhaseSouthernCageHorde;
                    l_PhaseMask &= ~TanaanPhases::PhaseEasternCageAlly;
                    l_PhaseMask &= ~TanaanPhases::PhaseSouthernCageAlly;
                    p_Player->SetPhaseMask(l_PhaseMask, true);
                    break;
                }
                case TanaanQuests::QuestBlazeOfGlory:
                {
                    if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestAltarAltercation))
                        p_Player->AddQuest(l_Quest, nullptr);
                }

                default:
                    break;
            }

            return true;
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, Quest const* p_Quest) override
        {
            uint64 l_PhaseMask = p_Player->GetPhaseMask();

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
                    l_PhaseMask &= ~TanaanPhases::PhaseEscortGroup;

                    if (p_Player->GetTeamId() == TEAM_ALLIANCE)
                    {
                        l_PhaseMask |= TanaanPhases::PhaseEasternCageAlly;
                        l_PhaseMask |= TanaanPhases::PhaseSouthernCageAlly;
                        l_PhaseMask |= TanaanPhases::PhaseQuestGiversAlly;
                    }
                    else if (p_Player->GetTeamId() == TEAM_HORDE)
                    {
                        l_PhaseMask |= TanaanPhases::PhaseEasternCageHorde;
                        l_PhaseMask |= TanaanPhases::PhaseSouthernCageHorde;
                        l_PhaseMask |= TanaanPhases::PhaseQuestGiversHorde;
                    }

                    p_Player->SetPhaseMask(l_PhaseMask, true);
                    p_Player->PlayScene(TanaanSceneObjects::SceneCostOfWarEscort, p_Player);

                    if (p_Creature->GetAI())
                        p_Creature->AI()->SetGUID(p_Player->GetGUID());

                    p_Player->PlayerTalkClass->SendCloseGossip();
                    break;
                }
                case TanaanQuests::QuestThePortalPower:
                {
                    l_PhaseMask |= TanaanPhases::PhaseGulDan;
                    p_Player->SetPhaseMask(l_PhaseMask, true);
                }
                default:
                    break;
            }
            return true;
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestAzerothLastStand) == QUEST_STATUS_INCOMPLETE)
                p_Player->CompleteQuest(TanaanQuests::QuestAzerothLastStand);
            else if (p_Player->GetQuestStatus(TanaanQuests::QuestAzerothLastStand) == QUEST_STATUS_COMPLETE)
            {
                const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestAzerothLastStand);

                if (l_Quest)
                    p_Player->RewardQuest(l_Quest, 0, p_Creature);
            }

            if (p_Player->GetQuestStatus(TanaanQuests::QuestStartDraenor) == QUEST_STATUS_COMPLETE)
            {
                const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestStartDraenor);
                p_Player->RewardQuest(l_Quest, 0, p_Creature);
            }


            if (p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_NONE &&
                p_Player->GetQuestStatus(TanaanQuests::QuestBlazeOfGlory) == QUEST_STATUS_REWARDED)
            {
                if (Quest const* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestAltarAltercation))
                    p_Player->AddQuest(l_Quest, nullptr);
            }

            p_Player->PrepareQuestMenu(p_Creature->GetGUID());
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_archmage_khadgarAI(creature);
        }

        struct npc_archmage_khadgarAI : public ScriptedAI
        {
            npc_archmage_khadgarAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eEvent
            {
                EventCheckPlayers = 1
            };

            std::map<uint64, uint32> m_PlayerTimers;
            std::set<uint64> m_PlayerGuids;

            void Reset() override
            {
                m_Events.Reset();
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Id*/) override
            {
                if (std::find(m_PlayerGuids.begin(), m_PlayerGuids.end(), p_Guid) == m_PlayerGuids.end())
                    m_PlayerTimers.insert(std::make_pair(p_Guid, 32000));
                else
                    return;

                m_PlayerGuids.insert(p_Guid);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_PlayerTimers.empty())
                {
                    for (auto l_Itr = m_PlayerTimers.begin(); l_Itr != m_PlayerTimers.end(); l_Itr++)
                    {
                        Player* l_Player = me->GetPlayer(*me, l_Itr->first);

                        if (!l_Player)
                            continue;

                        if (!l_Itr->second)
                        {
                            l_Itr = m_PlayerTimers.erase(l_Itr);

                            if (l_Itr == m_PlayerTimers.end())
                                break;

                            continue;
                        }
                        else
                        {
                            if (l_Itr->second <= p_Diff)
                            {
                                l_Itr->second = 0;

                                uint32 l_PhaseMask = l_Player->GetPhaseMask();
                                l_PhaseMask |= TanaanPhases::PhaseGroupHouse;
                                l_Player->SetPhaseMask(l_PhaseMask, true);
                            }
                            else
                                l_Itr->second -= p_Diff;
                        }
                    }
                }
            }
        };
};

/// 82188 - 81990 - 82007 - 82010 - 81994 - 82011 - 81997 - 82082 - 82191 - 82012 - 82014 - 82002 - 81996 - 81998 - 79062 - 81993 - 81995
/// 82001 - 82003 - 82004 - 82005 - 82008 - 82025 - 82016 - 82260 - 82264 - 82014 - 82009 - 82075 - 82017 - 82189 - 82263 - 82000
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
            enum Constants
            {
                SearchVictimInterval = 600
            };

            npc_generic_tanaan_guardianAI(Creature* creature) : ScriptedAI(creature) {}

            clock_t m_LastVictimSearch;

            void Reset() override
            {
                m_LastVictimSearch = clock() + Constants::SearchVictimInterval + urand(100, 1654);  ///< Dont make all attacker sync
                me->SetUInt32Value(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);

                if (me->GetEntry() == TanaanCreatures::NpcMoriccalas)
                    me->AddAura(TanaanSpells::SpellHornOfWinter, me);

                me->setFaction(2580);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                if (Creature* l_Grunt = p_Attacker->ToCreature())
                {
                    if (l_Grunt->GetEntry() == TanaanCreatures::NpcIronGrunt)
                        me->SetAttackTime(WeaponAttackType::BaseAttack, 800 + urand(0, 400));
                }

                me->SetAttackTime(WeaponAttackType::BaseAttack, 1800 + urand(0, 400));
            }

            void DamageTaken(Unit* p_DoneBy, uint32& p_Damage, const SpellInfo* /*l_Spellinfo*/) override
            {
                if (p_DoneBy->ToCreature())
                {
                    if (me->GetHealth() <= p_Damage || me->GetHealthPct() <= 80.0f)
                        me->SetFullHealth();
                }
            }

            void UpdateAI(uint32 const /*p_Diff*/) override
            {
                bool l_HasVictim = me->isInCombat();

                if ((clock() - m_LastVictimSearch) > 0)
                {
                    std::list<Creature*> l_AllEnemyList;
                    std::vector<Creature*> l_EnemyList;

                    GetCreatureListWithEntryInGrid(l_AllEnemyList, me, TanaanCreatures::NpcIronGrunt, 2.8f);

                    std::for_each(l_AllEnemyList.begin(), l_AllEnemyList.end(), [this, &l_EnemyList](const Creature* p_A)
                    {
                        if (!p_A->isAlive() || !p_A->IsWithinMeleeRange(me))
                            return;

                        l_EnemyList.push_back(const_cast<Creature*>(p_A));
                    });

                    auto l_Seed = std::chrono::system_clock::now().time_since_epoch().count();
                    std::shuffle(l_EnemyList.begin(), l_EnemyList.end(), std::default_random_engine(l_Seed));

                    if (!l_EnemyList.empty())
                    {
                        Creature* l_Grunt = *l_EnemyList.begin();

                        me->SetFacingToObject(l_Grunt);
                        me->GetAI()->AttackStart(l_Grunt);
                        me->SetInCombatWith(l_Grunt);

                        l_HasVictim = true;
                        m_LastVictimSearch = clock() + Constants::SearchVictimInterval + urand(100, 1654);  ///< Doesn't make all attackers synchronized
                    }
                }

                if (me->HasUnitState(UNIT_STATE_CASTING) || !me->isInCombat())
                    return;

                if (me->HasReactState(REACT_PASSIVE) && me->getThreatManager().isThreatListEmpty())
                {
                    EnterEvadeMode();
                    false;
                }

                DoMeleeAttackIfReady();
            }
        };
};

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

            void Reset() override
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
                        if ((m_Pos.m_positionX <= 4091.0f && m_Pos.m_positionX >= 4090.0f) &&
                            (m_Pos.m_positionY >= -2325.0f && m_Pos.m_positionY <= -2324.0f))
                            return true;
                    }
                    case eCreatureIds::Mumper:
                    {
                        if ((m_Pos.m_positionX <= 4042.0f && m_Pos.m_positionX >= 4041.0f) &&
                            (m_Pos.m_positionY >= -2325.0f && m_Pos.m_positionY <= -2324.0f))
                            return true;
                    }
                    default:
                        break;
                }

                return false;
            }

            void EnterEvadeMode() override
            {
                m_HasReset = true;
                CreatureAI::EnterEvadeMode();
            }

            void MovementInform(uint32 p_Type, uint32 p_Id) override
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

            std::vector<uint64> l_GuidChecker;

            void MoveInLineOfSight(Unit* p_Unit) override
            {
                if (p_Unit->GetTypeId() != TYPEID_PLAYER)
                    return;

                Player* l_Player = p_Unit->ToPlayer();

                if (!l_Player || l_Player->GetPositionZ() > me->GetPositionZ() || !l_Player->isInFront(me))
                    return;

                if (l_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_INCOMPLETE && l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjEnterGulDanPrison) < 1)
                    l_Player->KilledMonsterCredit(TanaanKillCredits::CreditEnterGuldanPrison);
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

            void Reset() override
            {
                me->setFaction(eFactions::FactionAggressive);
                me->AddAura(TanaanSpells::SpellAuraTormentedSoul, me);
            }

            void MoveInLineOfSight(Unit* p_Target) override
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

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_INCOMPLETE ||
                p_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_FAILED)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseAriokAltar;
                p_Player->SetPhaseMask(l_PhaseMask, true);

                if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjAriokGossip))
                    return true;

                if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjAriokGossip) < 1)
                    p_Player->KilledMonsterCredit(TanaanKillCredits::CreditAriokGossip);

                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);

                if (Creature* l_Ariok = p_Player->SummonCreature(p_Creature->GetEntry(), l_Pos, TEMPSUMMON_MANUAL_DESPAWN))
                {

                    if (l_Ariok->GetAI())
                    {
                        l_Ariok->AI()->SetGUID(p_Player->GetGUID());
                        l_Ariok->AI()->DoAction(eDatas::ActionStartEscort);
                    }
                }
            }

            return true;
        }

        struct npc_tanaan_ariokAI : public ScriptedAI
        {
            npc_tanaan_ariokAI(Creature* creature) : ScriptedAI(creature) { }

            bool m_IsSummoned;
            uint64 m_PlayerGuid;

            void Reset() override
            {
                m_IsSummoned = false;
                m_PlayerGuid = 0;
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Index*/) override
            {
                m_PlayerGuid = p_Guid;
            }

            void DoAction(int32 const p_Action) override
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

            void IsSummonedBy(Unit* /*p_Summoner*/) override
            {
                m_IsSummoned = true;
            }

            void UpdateAI(uint32 const /*p_Diff*/) override
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                if (m_IsSummoned)
                {
                    if (Creature* l_Bleeding = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcBleedingHollowBloodchosen, 55.0f))
                    {
                        me->GetMotionMaster()->Clear();

                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                if (!l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed))
                                    l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditAriokEscort, 1);

                                me->DespawnOrUnsummon();
                            }
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
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

            void SpellHit(Unit* p_Caster, const SpellInfo* p_SpellInfo) override
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

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestKargatharProvingGrounds:
                {
                    Position const l_Pos = *p_Player;
                    p_Creature->CastSpell(p_Creature, TanaanSpells::SpellMeteorShower, false);

                    if (p_Creature->AI())
                    {
                        p_Creature->AI()->SetGUID(p_Player->GetGUID(), 1);
                        p_Creature->AI()->DoAction(TanaanActions::ActionTimerSummon);
                    }

                    if (g_BridgeDestructionPlayerScript)
                        g_BridgeDestructionPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneBridgeDestruction, 16, l_Pos);

                    break;
                }
                case TanaanQuests::QuestKillYourHundred:
                {
                    if (p_Creature->GetAI())
                    {
                        uint32 l_PhaseMask = p_Player->GetPhaseMask();
                        l_PhaseMask &= ~TanaanPhases::PhaseGroupKillYourHundred;
                        p_Player->SetPhaseMask(l_PhaseMask, true);

                        p_Creature->AI()->SetGUID(p_Player->GetGUID(), 2);
                        p_Creature->AI()->DoAction(TanaanActions::ActionEventSceneArena);
                    }
                    break;
                }
//                 case TanaanQuests::QuestAPotentialAlly:
//                 case TanaanQuests::QuestAPotentialAllyHorde:
//                 {
//                     Position l_Pos;
//                     p_Player->GetPosition(&l_Pos);
//
//                     if (g_APotentialAllyPlayerScript)
//                         g_APotentialAllyPlayerScript->m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneRingOfFire, 16, l_Pos);
//                     break;
//                 }
                default:
                    break;
            }

            return true;
        }

        bool OnQuestReward(Player* p_Player, Creature* /*p_Creature*/, const Quest* p_Quest, uint32 /*p_Option*/) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestKargatharProvingGrounds)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (g_APotentialAllyPlayerScript)
                    g_APotentialAllyPlayerScript->m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneRingOfFire, 16, l_Pos);

                p_Player->SetPhaseMask(p_Player->GetPhaseMask(), true);

            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const override
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

            void Reset() override
            {
                m_DestroyTimer       = 0;
                m_PlayerAltarGuid    = 0;
                m_SceneTimer         = 0;
                m_PlayerForSceneGuid = 0;

                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE || !me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE)))
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                }

                if (me->HasAura(TanaanSpells::SpellKnockthrough))
                    me->RemoveAura(TanaanSpells::SpellKnockthrough);
            }

            void DoAction(const int32 p_Action) override
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

            void SetGUID(uint64 p_Guid, int32 p_Id) override
            {
                if (p_Id == 1)
                    m_PlayerAltarGuid = p_Guid;
                else if (p_Id == 2)
                    m_PlayerForSceneGuid = p_Guid;
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (Player* l_Player = p_Who->ToPlayer())
                {
                    if (l_Player->GetQuestStatus(TanaanQuests::QuestAltarAltercation) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBloodRitualOrbDestroyed) >= 3)
                            l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditFindKhadgarAtBridge, 1);
                    }
                }
            }

            void UpdateAI(uint32 const diff) override
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
                        {
                            l_Player->SummonCreature(TanaanCreatures::NpcArchmageKhadgarSum, l_Pos);
                            uint64 l_PhaseMask = l_Player->GetPhaseMask();
                            l_PhaseMask &= ~TanaanPhases::PhaseKhadgarBridge;
                            l_Player->SetPhaseMask(l_PhaseMask, true);
                        }
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
                                g_KillYourHundredPlayerScript->m_PlayerSceneFirstInstanceId[l_Player->GetGUID()] = l_Player->PlayStandaloneScene(TanaanSceneObjects::SceneEnterKarGathArena, 63, l_Pos);
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
            npc_tanaan_khadgar_bridgeAI(Creature* creature) : ScriptedAI(creature)
            {
                m_SpectatorGuid = 0;
                m_MovementTimer = 0;
            }

            uint64 m_SpectatorGuid;
            uint32 m_MovementTimer;

            void IsSummonedBy(Unit* p_Summoner) override
            {
                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                m_SpectatorGuid = p_Summoner->GetGUID();

                me->GetMotionMaster()->MovePoint(1, 4213.2266f, -2786.2f, 23.398f);
            }

            void MovementInform(uint32 p_Type, uint32 p_Id) override
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                if (p_Id == 1)
                    m_MovementTimer = 200;
                else
                {
                    Player* l_Player = sObjectAccessor->GetPlayer(*me, m_SpectatorGuid);

                    if (l_Player && !l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjFollowKhadgar))
                        l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditFollowKhadgar, 1);

                    me->DespawnOrUnsummon();
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_MovementTimer)
                {
                    if (m_MovementTimer <= p_Diff)
                    {
                        m_MovementTimer = 0;
                        me->GetMotionMaster()->MovePoint(2, 4229.7402f, -2812.96f, 17.2016f);
                    }
                    else
                        m_MovementTimer -= p_Diff;
                }
            }

        };
};

/// 83670 - Blood Ritual Orb
class npc_blood_ritual_orb : public CreatureScript
{
    public:
        npc_blood_ritual_orb() : CreatureScript("npc_blood_ritual_orb") { }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
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

                        uint32 l_PhaseMask = p_Player->GetPhaseMask();
                        l_PhaseMask &= ~TanaanPhases::PhaseAltarGroup;
                        l_PhaseMask |= TanaanPhases::PhaseGroupKillYourHundred;
                        l_PhaseMask |= TanaanPhases::PhaseKhadgarBridge;
                        p_Player->SetPhaseMask(l_PhaseMask, true);

                        if (Creature* l_Ariok = GetClosestCreatureWithEntry(p_Player, TanaanCreatures::NpcAriok, 50.0f))
                            l_Ariok->DespawnOrUnsummon();
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

            void Reset() override
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == TanaanActions::ActionSummonHandBrawler)
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, me, 80.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainInArena) < 99)
                        {
                            if (Creature* l_Creature = me->SummonCreature(TanaanCreatures::NpcShatteredHandBrawler, g_ShatteredHandSpawn[urand(0, 3)]))
                                l_Creature->SetPhaseMask(TanaanPhases::PhaseArenaOrcs, true);
                        }

                        break;
                    }
                }
            }
        };
};

/// 82057 - Shattered Hand Brawler
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

            void JustDied(Unit* p_Killer) override
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

                if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainAdd) <= 98 && l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainInArena) <= 98)
                {
                    if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainInArena))
                        l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditCombattantSlainAdd, 1);

                    l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditCombattantSlainInArena, 1);
                }

                if (l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainAdd) == 98 && l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainInArena) == 99)
                {
                    if (g_KillYourHundredPlayerScript)
                        g_KillYourHundredPlayerScript->m_PlayerSceneThirdInstanceId[l_Player->GetGUID()] = l_Player->PlayStandaloneScene(TanaanSceneObjects::SceneEscapingTheArena, 63, l_Pos);

                    l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditCombattantSlainAdd, 1);
                }
            }

            void IsSummonedBy(Unit* p_Summoner) override
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

            void MovementInform(uint32 p_Type, uint32 /*p_Id*/) override
            {
                if (p_Type != EFFECT_MOTION_TYPE)
                    return;

                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 80.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (me->isInFront(l_Player) && l_Player->HasQuest(TanaanQuests::QuestKillYourHundred) && l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjCombattantSlainAdd) < 99)
                    {
                        if (!l_Player->isInCombat())
                        {
                            uint64 l_Guid = l_Player->GetGUID();
                            AddTimedDelayedOperation(50, [this, l_Guid]() -> void
                            {
                                if (Player* l_Player = Player::GetPlayer(*me, l_Guid))
                                    AttackStart(l_Player);
                            });
                        }

                        break;
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);
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

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/) override
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestKillYourHundred:
                {

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

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_archmage_khadgar_shadowmoonAI(creature);
        }

        struct npc_archmage_khadgar_shadowmoonAI : public ScriptedAI
        {
            npc_archmage_khadgar_shadowmoonAI(Creature* creature) : ScriptedAI(creature) { }

            uint16 m_SceneTimer;
            uint64 p_PlayerGuid;

            void Reset() override
            {
                p_PlayerGuid = 0;
                m_SceneTimer = 0;
            }

            void SetData(uint32 /*p_Id*/, uint32 p_Value) override
            {
                m_SceneTimer = (uint16)p_Value;
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Id*/) override
            {
                p_PlayerGuid = p_Guid;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_SceneTimer)
                {
                    if (m_SceneTimer <= m_SceneTimer)
                    {
                        if (p_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, p_PlayerGuid))
                            {
                                uint32 l_PhaseMask = l_Player->GetPhaseMask();
                                l_PhaseMask &= ~TanaanPhases::PhaseOlinLiadrin;
                                l_PhaseMask |= TanaanPhases::PhaseYrelQuestGiver;
                                l_Player->SetPhaseMask(l_PhaseMask, true);

                                l_Player->PlayScene(TanaanSceneObjects::SceneLiadrinAndOlinEnter, l_Player);
                            }
                        }
                        m_SceneTimer = 0;
                    }
                    else
                        m_SceneTimer -= p_Diff;
                }
            }
        };
};

/// 79583 - Ungra
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

            void JustDied(Unit* /*p_Killer*/) override
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 20.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (!l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjUngraMastersOfShadowAlly) && !l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjUngraMastersOfShadowHorde))
                        l_Player->KilledMonsterCredit(me->GetEntry());
                }
            }
        };
};

/// 79585 - Taskmaster Gurran
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

            void JustDied(Unit* /*p_Killer*/) override
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 20.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (!l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjGurranMastersOfShadowAlly) && !l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjGurranMastersOfShadowHorde))
                        l_Player->KilledMonsterCredit(me->GetEntry());
                }
            }
        };
};

/// 79593 - Ankova the Fallen
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

            void JustDied(Unit* /*p_Killer*/) override
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 20.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (!l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjAnkovaMastersOfShadowsAlly) && !l_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjAnkovaMastersOfShadowsHorde))
                        l_Player->KilledMonsterCredit(me->GetEntry());
                }
            }
        };
};

/// 79794 - Yrel
class npc_tanaan_yrel : public CreatureScript
{
    public:
        npc_tanaan_yrel() : CreatureScript("npc_tanaan_yrel")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestYrelTanaan || p_Quest->GetQuestId() == TanaanQuests::QuestYrelHorde)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseYrelQuestGiver;
                p_Player->SetPhaseMask(l_PhaseMask, true);


                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);

                p_Player->SummonCreature(TanaanCreatures::NpcYrel, l_Pos);
            }

            return false;
        }
};

/// 78994 - Yrel
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

            void Reset() override
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                m_IsSummoned = true;

                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                m_PlayerGuid = p_Summoner->GetGUID();

                me->GetMotionMaster()->MoveFollow(p_Summoner, 1.0f, 1.0f);

                m_Events.ScheduleEvent(eEvents::EventCheckStopFollow, 6000);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
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
                                uint32 l_PhaseMask = l_Player->GetPhaseMask();
                                l_PhaseMask |= TanaanPhases::PhaseYrelQuestTaker;
                                l_Player->SetPhaseMask(l_PhaseMask, true);

                                if (l_Player->HasQuest(TanaanQuests::QuestYrelTanaan) || l_Player->HasQuest(TanaanQuests::QuestYrelHorde))
                                    l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditEscortYrel, 1);

                                me->DespawnOrUnsummon();
                            }
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_IsSummoned)
                {
                    m_Events.Update(p_Diff);

                    switch (m_Events.ExecuteEvent())
                    {
                        case eEvents::EventCheckStopFollow:
                        {
                            if (Creature* l_BaseCheck = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcExarchMaladaar, 50.0f))
                            {
                                me->GetMotionMaster()->Clear();
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);

                                if (me->HasUnitState(UNIT_STATE_FOLLOW))
                                    me->ClearUnitState(UNIT_STATE_FOLLOW);

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

/// 79537 - Exarch Maladaar
class npc_exarch_maladaar_tanaan_cave : public CreatureScript
{
    public:
        npc_exarch_maladaar_tanaan_cave() : CreatureScript("npc_exarch_maladaar_tanaan_cave")
        {
        }

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_exarch_maladaar_tanaan_caveAI(p_Creature);
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerAlly)
            {
                if (p_Creature->GetAI())
                {
                    p_Creature->AI()->SetGUID(p_Player->GetGUID(), 0);
//                    p_Creature->AI()->DoAction(1);
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

            void SetGUID(uint64 p_Guid, int32 /*p_Id*/) override
            {
                m_PlayerGuid = p_Guid;
            }

            void DoAction(int32 const /*p_Id*/) override
            {
                Talk(0);

                if (m_PlayerGuid)
                {
                    if (GameObject* l_Gob = GetClosestGameObjectWithEntry(me, TanaanGameObjects::GobIronCageDoor, 80.0f))
                    {
                        if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            l_Gob->UseDoorOrButton(120000, false, l_Player);
                    }
                }
            }
        };
};

/// 79702 - Keli'Dan the Breaker
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

            void EnterCombat(Unit* /*p_Victim*/) override
            {
                if (GameObject* l_Gob = GetClosestGameObjectWithEntry(me, TanaanGameObjects::GobIronCageDoor, 40.0f))
                {
                    l_Gob->RemoveFlag(GAMEOBJECT_FIELD_FLAGS, GO_FLAG_IN_USE);
                    l_Gob->SetGoState(GO_STATE_READY);
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 36.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->HasQuest(TanaanQuests::QuestKeliDanTheBreakerAlly) || l_Player->HasQuest(TanaanQuests::QuestKeliDanTheBreakerHorde))
                    {
                        Position l_Pos;

                        l_Player->GetPosition(&l_Pos);
                        l_Player->KilledMonsterCredit(me->GetEntry());
                        g_CavernTeleportPlayerScript->m_PlayerSceneFirstInstanceId[l_Player->GetGUID()] = l_Player->PlayStandaloneScene(TanaanSceneObjects::SceneNerZhulReveal, 16, l_Pos);
                    }
                }
            }
        };
};

/// 78430 - Cordana Felsong
class npc_cordana_felsong_blackrock : public CreatureScript
{
    public:
        npc_cordana_felsong_blackrock() : CreatureScript("npc_cordana_felsong_blackrock")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* /*p_Creature*/, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestTheBattleOfTheForge)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseLiadrinCordana;
                p_Player->SetPhaseMask(l_PhaseMask, true);

                p_Player->PlayScene(TanaanSceneObjects::SceneBlackHandReveal, p_Player);
                p_Player->SummonCreature(TanaanCreatures::NpcLadyLiadrin, 4609.370f, -2243.42f, 14.849f);
                p_Player->SummonCreature(TanaanCreatures::NpcCordanaFelsong, 4613.3f, -2246.3f, 15.19f);
            }

            return false;
        }

        CreatureAI* GetAI(Creature* p_Creature) const override
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

            void Reset() override
            {
                me->AddAura(TanaanSpells::SpellCoverOfElune, me);
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                m_Summoned = true;

                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                m_PlayerGuid = p_Summoner->GetGUID();

                if (Player* l_Player = p_Summoner->ToPlayer())
                    me->GetMotionMaster()->MoveFollow(l_Player, 0.2f, 0.3f);
            }

            void UpdateAI(const uint32 /*diff*/) override
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

                    if ((l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_COMPLETE ||
                         l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_REWARDED ||
                         l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_NONE))
                        me->DespawnOrUnsummon();
                }
            }
        };
};

/// 78430 - Lady Liadrin
class npc_lady_liadrin_blackrock : public CreatureScript
{
    public:
        npc_lady_liadrin_blackrock() : CreatureScript("npc_lady_liadrin_blackrock")
        {
        }

        bool OnQuestAccept(Player* /*p_Player*/, Creature* p_Creature, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestKeliDanTheBreakerHorde)
            {
                if (p_Creature->GetAI())
                {
///                    p_Creature->AI()->DoAction(1);
                }
            }

            return false;
        }

        CreatureAI* GetAI(Creature* p_Creature) const override
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
            void DoAction(int32 const /*p_Id*/) override
            {
                /// TALK
                Talk(0);

                if (GameObject* l_Gob = GetClosestGameObjectWithEntry(me, TanaanGameObjects::GobIronCageDoor, 80.0f))
                    l_Gob->UseDoorOrButton(120000, false);
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                m_Summoned = true;

                if (p_Summoner->GetTypeId() != TYPEID_PLAYER)
                    return;

                m_PlayerGuid = p_Summoner->GetGUID();

                if (Player* l_Player = p_Summoner->ToPlayer())
                    me->GetMotionMaster()->MoveFollow(l_Player, 0.2f, 5.7f);
            }

            void UpdateAI(const uint32 /*diff*/) override
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

                    if ((l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_COMPLETE ||
                        l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_REWARDED ||
                        l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_NONE))
                        me->DespawnOrUnsummon();
                }
            }
        };
};

/// 80786 - Blackrock Grunt
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

            void JustDied(Unit* /*p_Killer*/) override
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 3.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_INCOMPLETE)
                        l_Player->KilledMonsterCredit(TanaanKillCredits::CreditBlackrockGrunt);
                }
            }
        };
};

/// 80775/81294 - Ogron Warcrusher
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

            EventMap m_Events;

            enum eEvents
            {
                EventAddaura = 1
            };

            void Reset() override
            {
                m_Events.Reset();
                m_Events.ScheduleEvent(EventAddaura, 3000);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 3.0f);

                for (Player* l_Player : l_PlayerList)
                {
                    if (l_Player->GetQuestStatus(TanaanQuests::QuestTheBattleOfTheForge) == QUEST_STATUS_INCOMPLETE)
                        l_Player->KilledMonsterCredit(TanaanKillCredits::CreditOgronWarcrusher);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventAddaura:
                        me->AddAura(TanaanSpells::SpellCrushingStomp, me);
                        m_Events.ScheduleEvent(eEvents::EventAddaura, 12000);
                        break;
                }
            }
        };
};

/// 78996 - Farseer Drek'Thar
class npc_farseer_drek_thar : public CreatureScript
{
    public:
        npc_farseer_drek_thar() : CreatureScript("npc_farseer_drek_thar")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* /*p_Creature*/, Quest const* p_Quest) override
        {
            uint64 l_PlayerPhase = p_Player->GetPhaseMask();

            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestGaNarOfTheFrostwolf:
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask |= TanaanPhases::PhaseThaelinQuestGiver;
                    l_PhaseMask |= TanaanPhases::PhaseCannonTurret;
                    p_Player->SetPhaseMask(l_PhaseMask, true);

                    p_Player->PlayScene(TanaanSceneObjects::SceneWaterPortal, p_Player);
                }
                    break;
                default:
                    break;
            }
            return true;
        }

        bool OnQuestReward(Player* p_Player, Creature* /*p_Creature*/, const Quest* p_Quest, uint32 /*p_Option*/) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestTheBattleOfTheForge && g_MapShiftPlayerScript)
            {
                Position l_Pos;
                uint32 l_PhaseMask = p_Player->GetPhaseMask();

                l_PhaseMask &= ~TanaanPhases::PhasePoolMobs;
                l_PhaseMask &= ~TanaanPhases::PhaseGroupPool;

                p_Player->SetPhaseMask(l_PhaseMask, true);
                p_Player->GetPosition(&l_Pos);

                g_MapShiftPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneDamExplosion, 16, l_Pos);

            }

            return false;
        }
};

/// 78568 - Thaelin Darkanvil
class npc_thaelin_darkanvil_tanaan : public CreatureScript
{
    public:
        npc_thaelin_darkanvil_tanaan() : CreatureScript("npc_thaelin_darkanvil_tanaan")
        {
        }

        bool OnGossipHello(Player* p_Player, Creature* p_Creature) override
        {
            if (const Quest* l_Quest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestTakingATripToTheTopOfTheTank))
                p_Player->PlayerTalkClass->SendQuestQueryResponse(l_Quest);

            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Yes, I need you to help me operate that enormous tank.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, "I would like to buy from you.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
            p_Player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "I would like to check your quests, please.", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
            p_Player->SEND_GOSSIP_MENU(1, p_Creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* p_Player, Creature* p_Creature, uint32 /*sender*/, uint32 p_Action) override
        {
            p_Player->PlayerTalkClass->ClearMenus();

            if (p_Action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (p_Player->GetQuestStatus(TanaanQuests::QuestTakingATripToTheTopOfTheTank) != QUEST_STATUS_INCOMPLETE/* && p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjTopOfTheTank) >= 1*/)
                {
                    p_Player->CLOSE_GOSSIP_MENU();
                    return true;
                }

                Position l_Pos;
                p_Creature->GetPosition(&l_Pos);
                p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditSpeakWithThaelin, 1);
                p_Player->CLOSE_GOSSIP_MENU();

                if (Creature* l_Thaelin = p_Player->SummonCreature(p_Creature->GetEntry(), l_Pos))
                {
                    uint32 l_PhaseMask = p_Player->GetPhaseMask();
                    l_PhaseMask &= ~TanaanPhases::PhaseThaelinQuestGiver;
                    l_Thaelin->SetPhaseMask(4294967295, true);
                    p_Player->SetPhaseMask(l_PhaseMask, true);
                }
            }
            else if (p_Action == GOSSIP_ACTION_TRADE)
                p_Player->GetSession()->SendListInventory(p_Creature->GetGUID());
            else if (p_Action == GOSSIP_ACTION_INFO_DEF + 2)
            {
                const Quest* l_TasteQuest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestATasteOfIron);
                const Quest* l_ShadowQuest = sObjectMgr->GetQuestTemplate(TanaanQuests::QuestShadowOfTheWorldbreaker);

                if (l_TasteQuest && l_ShadowQuest)
                {
                    if (p_Player->GetQuestStatus(TanaanQuests::QuestShadowOfTheWorldbreaker) != QUEST_STATUS_REWARDED)
                        p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_ShadowQuest, p_Creature->GetGUID());

                    if (p_Player->GetQuestStatus(TanaanQuests::QuestTakingATripToTheTopOfTheTank) == QUEST_STATUS_REWARDED)
                        p_Player->PlayerTalkClass->SendQuestGiverQuestDetails(l_TasteQuest, p_Creature->GetGUID());
                }
            }


            return true;
        }

        CreatureAI* GetAI(Creature* creature) const override
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

            void Reset() override
            {
                m_Summoned = false;
                m_Events.Reset();

                if (GameObject* l_MakeshiftPlunger = GetClosestGameObjectWithEntry(me, 231066, 10.0f))
                    me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                m_Summoned = true;
                m_PlayerGuid = p_Summoner->GetGUID();
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR);
                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);

                /// TALK
                Talk(0);

                if (Player* l_Player = p_Summoner->ToPlayer())
                    me->GetMotionMaster()->MoveFollow(l_Player, 0, 1.0f);

                m_Events.ScheduleEvent(eEvents::EventCheckTalk, 5000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, const SpellInfo* /*l_Spellinfo*/) override
            {
                if (p_Damage >= me->GetHealth())
                    me->SetFullHealth();
            }

            void MovementInform(uint32 p_Type, uint32 /*p_Id*/) override
            {
                if (p_Type == FOLLOW_MOTION_TYPE)
                {
                    if (Creature* l_Thaelin = GetClosestCreatureWithEntry(me, TanaanCreatures::NpcThaelinDarkanvil, 4.0f))
                    {
                        /// TALK
                        Talk(3);
                        Position l_Pos;
                        l_Thaelin->GetPosition(&l_Pos);

                        me->GetMotionMaster()->Clear();


                        if (m_PlayerGuid)
                        {
                            if (Player* l_Player = me->GetPlayer(*me, m_PlayerGuid))
                            {
                                l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditEscortThaelin, 1);

                                uint32 l_PhaseMask = l_Player->GetPhaseMask();
                                l_PhaseMask |= TanaanPhases::PhaseThaelinTurret;
                                l_Player->SetPhaseMask(l_PhaseMask, true);
                                me->DespawnOrUnsummon();
                            }
                        }
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (!m_Summoned)
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCheckTalk:
                        if (GetClosestCreatureWithEntry(me, TanaanCreatures::NpcGogluk, 85.0f))
                            Talk(1); ///< TALK
                        else
                            m_Events.ScheduleEvent(eEvents::EventCheckTalk, 800);
                        break;
                    default:
                        break;
                }

                if (!m_PlayerGuid)
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

                    if ((l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTakingATripToTheTopOfTheTank) == QUEST_STATUS_COMPLETE ||
                         l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTakingATripToTheTopOfTheTank) == QUEST_STATUS_REWARDED ||
                         l_EscortedPlayer->GetQuestStatus(TanaanQuests::QuestTakingATripToTheTopOfTheTank) == QUEST_STATUS_NONE))
                    {
                        me->DespawnOrUnsummon();
                        return;
                    }
                }
            }
        };
};

/// 86039 - Gogluk
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

            void Reset() override
            {
                Position l_Pos;
                me->GetPosition(&l_Pos);
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

            void EnterCombat(Unit* p_Attacker) override
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

            void JustDied(Unit* /*p_Killer*/) override
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

            void EnterEvadeMode() override
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

/// 86690/86691 - Gogluk Adds
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

            void DoAction(int32 const /*p_Action*/) override
            {
                switch (me->GetEntry())
                {
                    case TanaanCreatures::NpcCannonTurret:
                        m_Events.ScheduleEvent(eEvents::EventCannonBarrage, 1000);
                        break;
                    case TanaanCreatures::NpcGunTurret:
                        m_Events.ScheduleEvent(eEvents::EventMachineGun, 3000);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCannonBarrage:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, TanaanSpells::SpellCannonBarrage, false);
                        m_Events.ScheduleEvent(eEvents::EventCannonBarrage, 80000);
                        break;
                    case eEvents::EventMachineGun:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, TanaanSpells::SpellMachineGun, true);
                        m_Events.ScheduleEvent(eEvents::EventMachineGun, 8000);
                        break;
                    default:
                        break;
                }
            }
        };
};

/// 80521 - Thaelin Darkanvil
class npc_thaelin_tanaan_questgiver : public CreatureScript
{
    public:
        npc_thaelin_tanaan_questgiver() : CreatureScript("npc_thaelin_tanaan_questgiver")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == TanaanQuests::QuestTheHomeStretchAlly || p_Quest->GetQuestId() == TanaanQuests::QuestTheHomeStretchHorde)
            {
                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (g_TheHomeStretchPlayerScript)
                    g_TheHomeStretchPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneFinaleIronBastion, 16, l_Pos);

                if (p_Creature->GetAI())
                    p_Creature->GetAI()->SetGUID(p_Player->GetGUID());
            }

            return false;
        }

        bool OnQuestReward(Player* /*p_Player*/, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/) override
        {
            if (p_Quest->GetQuestId() != TanaanQuests::QuestTakingATripToTheTopOfTheTank)
                return true;

            std::list<Creature*> l_ThaelinList;
            p_Creature->GetCreatureListWithEntryInGrid(l_ThaelinList, TanaanCreatures::NpcThaelinDarkanvilSecond, 15.0f);

            for (Creature* l_Thaelin : l_ThaelinList)
                l_Thaelin->DespawnOrUnsummon();

            return false;
        }

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new npc_thaelin_tanaan_questgiverAI(creature);
        }

        struct npc_thaelin_tanaan_questgiverAI : public ScriptedAI
        {
            npc_thaelin_tanaan_questgiverAI(Creature* creature) : ScriptedAI(creature)
            {
                m_TimerValues =
                {
                    16000,
                    9000,
                    9000,
                    9000,
                    9000,
                    9000,
                    9000,
                };

                m_Auras =
                {
                    TanaanSpells::SpellIronBastionProgressA,
                    TanaanSpells::SpellIronBastionProgressB,
                    TanaanSpells::SpellIronBastionProgressC,
                    TanaanSpells::SpellIronBastionProgressD,
                    TanaanSpells::SpellIronBastionProgressE,
                    TanaanSpells::SpellIronBastionProgressF,
                    TanaanSpells::SpellIronBastionProgressG
                };

                l_UpdateCounter = 0;
                m_Scene.m_Done = false;
            }

            struct PlayerScene
            {
                uint32 m_Timers[7];
                bool m_Done;
            };

            typedef std::map<uint64, PlayerScene> SceneMap;

            PlayerScene m_Scene;
            SceneMap m_SceneMap;
            std::list<uint64> m_GuidList;
            std::vector<uint32> m_TimerValues;
            std::vector<uint32> m_Auras;
            uint8 l_UpdateCounter;

            void SetGUID(uint64 p_Guid, int32 /*p_Id*/) override
            {
                if (std::find(m_GuidList.begin(), m_GuidList.end(), p_Guid) != m_GuidList.end())
                    return;

                if (m_SceneMap.empty())
                {
                    for (uint8 l_I = 1; l_I < 7; l_I++)
                        m_Scene.m_Timers[l_I] = 0;

                    m_Scene.m_Timers[0] = 16000;
                    m_SceneMap.insert(std::make_pair(p_Guid, m_Scene));

                    return;
                }

                for (SceneMap::iterator l_Itr = m_SceneMap.begin(); l_Itr != m_SceneMap.end(); ++l_Itr)
                {
                    if (l_Itr->first != p_Guid)
                    {
                        l_Itr->second.m_Done = false;

                        for (uint8 l_SubItr = 0; l_SubItr < 7; l_SubItr++)
                            l_Itr->second.m_Timers[l_SubItr] = l_SubItr ? 0 : 16000;

                        m_SceneMap.insert(std::make_pair(p_Guid, m_Scene));
                        break;
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_SceneMap.empty())
                    return;

                bool l_RestartLoop = false;

                for (SceneMap::iterator l_Itr = m_SceneMap.begin(); l_Itr != m_SceneMap.end(); ++l_Itr)
                {
                    if (l_Itr->second.m_Done)
                        continue;

                    Player* l_Player = me->GetPlayer(*me, l_Itr->first);

                    if (!l_Player)
                        continue;

                    for (uint8 l_SubItr = 0; l_SubItr < 7; l_SubItr++)
                    {
                        if (l_Itr->second.m_Timers[l_SubItr])
                        {
                            if (l_Itr->second.m_Timers[l_SubItr] <= p_Diff)
                            {
                                l_Itr->second.m_Timers[l_SubItr] = 0;

                                if (l_SubItr != 6)
                                    l_Itr->second.m_Timers[l_SubItr + 1] = m_TimerValues[l_SubItr + 1];
                                else
                                {
                                    m_GuidList.push_back(l_Itr->first);
                                    l_Itr = m_SceneMap.erase(l_Itr);
                                    l_RestartLoop = true;
                                    break;
                                }

                                l_Player->AddAura(m_Auras[l_SubItr], l_Player);
                            }
                            else
                                l_Itr->second.m_Timers[l_SubItr] -= p_Diff;
                        }
                    }

                    if (l_RestartLoop)
                        break;
                }
            }
        };
};

/// 300005 - Blackrock Trigger
class npc_black_rock_trigger : public CreatureScript
{
    public:
        npc_black_rock_trigger() : CreatureScript("npc_black_rock_trigger")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_black_rock_triggerAI(creature);
        }

        struct npc_black_rock_triggerAI : public ScriptedAI
        {
            npc_black_rock_triggerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eEvent
            {
                EventCheckPlayers = 1
            };

            std::map<uint64, uint32> m_PlayerTimers;
            std::set<uint64> m_PlayerGuids;

            void Reset() override
            {
                m_Events.Reset();
            }

            void SetGUID(uint64 p_Guid, int32 /*p_Id*/) override
            {
                if (std::find(m_PlayerGuids.begin(), m_PlayerGuids.end(), p_Guid) == m_PlayerGuids.end())
                    m_PlayerTimers.insert(std::make_pair(p_Guid, 22000));

                m_PlayerGuids.insert(p_Guid);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_PlayerTimers.empty())
                {
                    for (auto l_Itr = m_PlayerTimers.begin(); l_Itr != m_PlayerTimers.end(); l_Itr++)
                    {
                        Player* l_Player = me->GetPlayer(*me, l_Itr->first);

                        if (!l_Player)
                            continue;

                        if (!l_Itr->second)
                        {
                            l_Itr = m_PlayerTimers.erase(l_Itr);

                            if (l_Itr == m_PlayerTimers.end())
                                break;

                            continue;
                        }
                        else
                        {
                            if (l_Itr->second <= p_Diff)
                            {
                                l_Itr->second = 0;

                                uint32 l_PhaseMask = l_Player->GetPhaseMask();
                                l_PhaseMask |= TanaanPhases::PhaseGroupPool;
                                l_PhaseMask |= TanaanPhases::PhasePoolMobs;
                                l_PhaseMask |= TanaanPhases::PhaseLiadrinCordana;
                                l_Player->SetPhaseMask(l_PhaseMask, true);
                            }
                            else
                                l_Itr->second -= p_Diff;
                        }
                    }
                }
            }
        };
};

/// 300006 - Final Tanaan Trigger
class npc_final_tanaan_trigger : public CreatureScript
{
    public:
        npc_final_tanaan_trigger() : CreatureScript("npc_final_tanaan_trigger")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_final_tanaan_triggerAI(creature);
        }

        struct npc_final_tanaan_triggerAI : public ScriptedAI
        {
            npc_final_tanaan_triggerAI(Creature* creature) : ScriptedAI(creature) { }

            void MoveInLineOfSight(Unit* p_Unit) override
            {
                if (Player* l_Player = p_Unit->ToPlayer())
                {
                    if (l_Player->GetDistance2d(me) >= 10.0f)
                        return;

                    if (l_Player->GetQuestStatus(TanaanQuests::QuestTheHomeStretchAlly) == QUEST_STATUS_INCOMPLETE || l_Player->GetQuestStatus(TanaanQuests::QuestTheHomeStretchHorde) == QUEST_STATUS_INCOMPLETE)
                    {
                        l_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditBoatsReached, 1);

                        uint32 l_PhaseMask = l_Player->GetPhaseMask();
                        l_PhaseMask |= TanaanPhases::PhaseFinalePhase;
                        l_Player->SetPhaseMask(l_PhaseMask, true);
                    }
                }
            }
        };
};

/// Archmage Khadgar - 78563
class npc_tanaan_khadgar_final : public CreatureScript
{
    public:
        npc_tanaan_khadgar_final() : CreatureScript("npc_tanaan_khadgar_final")
        {
        }

        bool OnQuestReward(Player* p_Player, Creature* p_Creature, const Quest* p_Quest, uint32 /*p_Option*/) override
        {
            switch (p_Quest->GetQuestId())
            {
                case TanaanQuests::QuestTheHomeStretchAlly:
                {
                    if (p_Creature->GetAI())
                        p_Creature->AI()->DoAction(1);

                    Position l_Pos;
                    p_Player->GetPosition(&l_Pos);

                    if (g_SceneAllianceBoatPlayerScript)
                        g_SceneAllianceBoatPlayerScript->m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneAllianceBoat, 63, l_Pos);
                    break;
                }
                case TanaanQuests::QuestTheHomeStretchHorde:
                {
                    Position l_Pos;
                    p_Player->GetPosition(&l_Pos);

                    if (g_SceneHordeBoatPlayerScript)
                        g_SceneHordeBoatPlayerScript->m_PlayerSceneInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneHordeBoat, 63, l_Pos);
                    break;
                }
                default:
                    break;
            }
            return true;
        }
};

/// 300006 - Final Tanaan Trigger
class npc_tanaan_napestone_riverbeast : public CreatureScript
{
    public:
        npc_tanaan_napestone_riverbeast() : CreatureScript("npc_tanaan_napestone_riverbeast")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_napestone_riverbeastAI(creature);
        }

        struct npc_tanaan_napestone_riverbeastAI : public ScriptedAI
        {
            npc_tanaan_napestone_riverbeastAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eDatas
            {
                EventWhipSplash = 1,
                SpellWhipSplash = 167439
            };

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Target*/) override
            {
                m_Events.ScheduleEvent(eDatas::EventWhipSplash, 3000);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eDatas::EventWhipSplash)
                {
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(l_Target, eDatas::SpellWhipSplash, false);
                    m_Events.ScheduleEvent(eDatas::EventWhipSplash, 12000);
                }
            }
        };
};

/// 300006 - Final Tanaan Trigger
class npc_tanaan_mandragora : public CreatureScript
{
    public:
        npc_tanaan_mandragora() : CreatureScript("npc_tanaan_mandragora")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_mandragoraAI(creature);
        }

        struct npc_tanaan_mandragoraAI : public ScriptedAI
        {
            npc_tanaan_mandragoraAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eDatas
            {
                EventChomp       = 1,
                SpellChomp       = 161533,
                EventImpactSplit = 2,
                SpellImpactSplit = 161299
            };

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Target*/) override
            {
                m_Events.ScheduleEvent(eDatas::EventChomp, 3000);
                m_Events.ScheduleEvent(eDatas::EventImpactSplit, 10000);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eDatas::EventChomp)
                {
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->CastSpell(l_Target, eDatas::SpellChomp, false);
                    m_Events.ScheduleEvent(eDatas::EventChomp, 20000);
                }
                else if (m_Events.ExecuteEvent() == eDatas::EventImpactSplit)
                {
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                        me->CastSpell(l_Target, eDatas::SpellImpactSplit, false);
                    m_Events.ScheduleEvent(eDatas::EventImpactSplit, 20000);
                }
            }
        };
};

/// 300006 - Final Tanaan Trigger
class npc_tanaan_overseer_gotrigg : public CreatureScript
{
    public:
        npc_tanaan_overseer_gotrigg() : CreatureScript("npc_tanaan_overseer_gotrigg")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_overseer_gotriggAI(creature);
        }

        struct npc_tanaan_overseer_gotriggAI : public ScriptedAI
        {
            npc_tanaan_overseer_gotriggAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eDatas
            {
                EventBurningBody = 1,
                SpellBurningBody = 166401,
                EventBurningRadiance = 2,
                SpellBurningRadiance = 166403
            };

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Target*/) override
            {
                /// TALK

                m_Events.ScheduleEvent(eDatas::EventBurningBody, 3000);
                m_Events.ScheduleEvent(eDatas::EventBurningBody, 10000);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eDatas::EventBurningBody)
                {
                    if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                        me->AddAura(eDatas::SpellBurningBody, l_Target);
                    m_Events.ScheduleEvent(eDatas::EventBurningBody, 20000);
                }
                else if (m_Events.ExecuteEvent() == eDatas::EventBurningRadiance)
                {
                    me->AddAura(eDatas::SpellBurningRadiance, me);
                    m_Events.ScheduleEvent(eDatas::EventBurningRadiance, 20000);
                }
            }
        };
};

/// 300006 - Final Tanaan Trigger
class npc_tanaan_arena_helper : public CreatureScript
{
    public:
        npc_tanaan_arena_helper() : CreatureScript("npc_tanaan_arena_helper")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_tanaan_overseer_gotriggAI(creature);
        }

        struct npc_tanaan_overseer_gotriggAI : public ScriptedAI
        {
            npc_tanaan_overseer_gotriggAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;

            enum eDatas
            {
                EventCheckEnnemies = 1
            };

            void Reset() override
            {
                m_Events.Reset();

                if (me->GetAreaId() == 7040 || me->GetAreaId() == 7130)
                    m_Events.ScheduleEvent(eDatas::EventCheckEnnemies, 1000);
            }

            void EnterCombat(Unit* /*p_Target*/) override
            {
                m_Events.CancelEvent(EventCheckEnnemies);
            }

            void EnterEvadeMode() override
            {
                m_Events.ScheduleEvent(eDatas::EventCheckEnnemies, 1000);
            }

            void MovementInform(uint32 p_Type, uint32 /*p_Id*/) override
            {
                if (p_Type != DISTRACT_MOTION_TYPE)
                    return;

                if (Creature* l_ShattrdHandBrwlr = GetClosestCreatureWithEntry(me, 82057, 3.0f))
                {
                    me->SetFacingToObject(l_ShattrdHandBrwlr);
                    AttackStart(l_ShattrdHandBrwlr);
                    me->SetInCombatWith(l_ShattrdHandBrwlr);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == eDatas::EventCheckEnnemies)
                {
                    if (Creature* l_ShatteredHandBrawler = GetClosestCreatureWithEntry(me, 82057, 80.0f))
                    {
                        if (!l_ShatteredHandBrawler->isInCombat())
                            me->GetMotionMaster()->MoveCharge(l_ShatteredHandBrawler->m_positionX, l_ShatteredHandBrawler->m_positionY, l_ShatteredHandBrawler->m_positionZ, 10.0f);
                    }

                    m_Events.ScheduleEvent(eDatas::EventCheckEnnemies, 1000);
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

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject) override
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestThePortalPower) == QUEST_STATUS_INCOMPLETE && p_Player->GetQuestObjectiveCounter(273936) < 1)
            {
                if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBurningBladeDestroyed) == 0 ||
                    p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjShatteredHandDestroyed) == 0 ||
                    p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjBlackrockMarkDestroyed) == 0)
                    return true;

                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseGulDan;
                l_PhaseMask &= ~TanaanPhases::PhasePortal;
                p_Player->SetPhaseMask(l_PhaseMask, true);

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

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject) override
        {
            uint32 l_PhaseMask = p_Player->GetPhaseMask();

            if (p_Player->GetQuestStatus(TanaanQuests::QuestBledDryAlly) == QUEST_STATUS_INCOMPLETE || p_Player->GetQuestStatus(TanaanQuests::QuestBledDryHorde) == QUEST_STATUS_INCOMPLETE)
            {
                this->OnDamaged(p_Gameobject, p_Player);

                /// Eastern Cage
                if (p_Gameobject->GetEntry() == TanaanGameObjects::GobEasternCage)
                {
                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjEasternCageOpened) >= 1)
                        return true;

                    l_PhaseMask &= ~TanaanPhases::PhaseEasternCageHorde;
                    l_PhaseMask &= ~TanaanPhases::PhaseEasternCageAlly;
                    p_Player->KilledMonsterCredit(TanaanKillCredits::CreditEasternCageOpened);
                    p_Player->KilledMonsterCredit(78529);
                    p_Player->PlayScene(TanaanSceneObjects::SceneEasterCage, p_Player);
                }
                /// Southest Cage
                else if (p_Gameobject->GetEntry() == TanaanGameObjects::GobSouthernCage)
                {
                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjSouthernCageOpened) >= 1)
                        return true;

                    l_PhaseMask &= ~TanaanPhases::PhaseSouthernCageHorde;
                    l_PhaseMask &= ~TanaanPhases::PhaseSouthernCageAlly;
                    p_Player->KilledMonsterCredit(TanaanKillCredits::CreditSouthernCageOpened);
                    p_Player->KilledMonsterCredit(85141);
                    p_Player->PlayScene(TanaanSceneObjects::SceneSouthernCage, p_Player);
                }

                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
            return true;
        }
};

/// 233056/233057 - Mark of the Shadowmoon/Bleeding Hollow
class gob_mark_of_tanaan : public GameObjectScript
{
    public:
        gob_mark_of_tanaan() : GameObjectScript("gob_mark_of_tanaan") { }

        bool OnGossipHello(Player* p_Player, GameObject* p_Gameobject) override
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestOnslaughtEnd) == QUEST_STATUS_INCOMPLETE)
            {
                /// Shadowmoon Gob (Cho'Gall)
                if (p_Gameobject->GetEntry() == TanaanGameObjects::GobMarkOfShadowmoon)
                {
                    uint32 l_Phasemask = p_Player->GetPhaseMask();
                    l_Phasemask &= ~TanaanPhases::PhaseChoGall;
                    p_Player->SetPhaseMask(l_Phasemask, true);

                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjNorthernSpireDisabled) >= 1)
                        return true;

                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditNorthernSpireDisabled, 1);
                    p_Player->PlayScene(TanaanSceneObjects::SceneChoGallsFreedom, p_Player);
                }
                /// Bleeding Hollow Gob (Teron'Gor)
                else if (p_Gameobject->GetEntry() == TanaanGameObjects::GobMarkOfBleedingHollow)
                {
                    uint32 l_Phasemask = p_Player->GetPhaseMask();
                    l_Phasemask &= ~TanaanPhases::PhaseTeronGor;
                    p_Player->SetPhaseMask(l_Phasemask, true);

                    if (p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjSouthernSpireDisabled) >= 1)
                        return true;

                    p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditSouthernSpireDisabled, 1);
                    p_Player->PlayScene(TanaanSceneObjects::SceneTeronGorsFreedom, p_Player);
                }
            }
            return true;
        }
};

/// 231066 - Makeshift Plunger
class gob_makeshift_plunger : public GameObjectScript
{
    public:
        gob_makeshift_plunger() : GameObjectScript("gob_makeshift_plunger") { }

        bool OnGossipHello(Player* p_Player, GameObject* /*p_Gameobject*/) override
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestTheGunpowderPlot) == QUEST_STATUS_INCOMPLETE)
            {
                if (!p_Player->GetQuestObjectiveCounter(273294))
                    return true;

                Position l_Pos;
                p_Player->GetPosition(&l_Pos);
                p_Player->RemoveAura(162676);
                p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditMakeShiftPlunger, 1);

                g_GunpowderPlotPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneBuildingExplosion, 16, l_Pos);
            }
            return true;
        }
};

/// 231261 - Worldbreaker Side Turret
class gob_worldbreaker_side_turret : public GameObjectScript
{
    public:
        gob_worldbreaker_side_turret() : GameObjectScript("gob_worldbreaker_side_turret")
        {
        }

        bool OnGossipHello(Player* p_Player, GameObject* /*p_Gameobject*/) override
        {
            if (p_Player->GetQuestStatus(34445) == QUEST_STATUS_INCOMPLETE && !p_Player->GetQuestObjectiveCounter(273388))
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask &= ~TanaanPhases::PhaseCannonTurret;
                p_Player->SetPhaseMask(l_PhaseMask, true);
                p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditEnterWorldbreakerTurret, 1);

                Position l_Pos;
                p_Player->GetPosition(&l_Pos);

                if (!g_TasteOfIronPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
                {
                    g_TasteOfIronPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()] = p_Player->PlayStandaloneScene(TanaanSceneObjects::SceneShootingGallery, 63, l_Pos);
                    p_Player->AddAura(TanaanSpells::SpellTasteOfIronGameAura, p_Player);
                }
            }
            return true;
        }

        struct gob_worldbreaker_side_turretAI : public GameObjectAI
        {
            gob_worldbreaker_side_turretAI(GameObject* p_Go) : GameObjectAI(p_Go) { go->SetCancelAnim(true); }
        };

        GameObjectAI* GetAI(GameObject* p_Go) const override
        {
            return new gob_worldbreaker_side_turretAI(p_Go);
        }
};

/// 232538 - Main Cannon Trigger
class gob_main_cannon_trigger : public GameObjectScript
{
    public:
        gob_main_cannon_trigger() : GameObjectScript("gob_main_cannon_trigger")
        {
        }

        bool OnGossipHello(Player* p_Player, GameObject* /*p_Gameobject*/) override
        {
            if (p_Player->GetQuestStatus(TanaanQuests::QuestATasteOfIron) == QUEST_STATUS_INCOMPLETE && p_Player->GetQuestObjectiveCounter(TanaanQuestObjectives::ObjIronHordeSlain) >= 200)
            {
                if (uint32 l_SceneInstance = g_TasteOfIronPlayerScript->m_PlayerSceneFirstInstanceId[p_Player->GetGUID()])
                    p_Player->CancelStandaloneScene(l_SceneInstance);

                p_Player->SendMovieStart(TanaanMovies::MovieDoorDestruction);
                p_Player->QuestObjectiveSatisfy(TanaanKillCredits::CreditMainCannonTrigger, 1);
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                l_PhaseMask |= TanaanPhases::PhasePortalDestroyed;
                p_Player->SetPhaseMask(l_PhaseMask, true);
            }
            return false;
        }
};

/// 165988 - Inferno
class spell_tanaan_inferno : public SpellScriptLoader
{
    public:
        spell_tanaan_inferno() : SpellScriptLoader("spell_tanaan_inferno") { }

        class spell_tanaan_inferno_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tanaan_inferno_SpellScript);

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
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

#ifndef __clang_analyzer__
void AddSC_tanaan_jungle()
{
    new npc_archmage_khadgar();
    new npc_generic_tanaan_guardian();
    new npc_iron_grunt();
    new npc_iron_gronnling();
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
    new npc_thaelin_tanaan_questgiver();
    new npc_black_rock_trigger();
    new npc_final_tanaan_trigger();
    new npc_tanaan_khadgar_final();
    new npc_tanaan_napestone_riverbeast();
    new npc_tanaan_mandragora();
    new npc_tanaan_overseer_gotrigg();
    new npc_tanaan_arena_helper();
    new gob_static_rune();
    new go_platform_tanaan();
    new go_bleeding_hollow_cage();
    new gob_mark_of_tanaan();
    new gob_makeshift_plunger();
    new gob_worldbreaker_side_turret();
    new gob_main_cannon_trigger();
    new map_dark_portal_entrance();
    new playerScript_enter_tanaan();
    new playerScript_add_tanaan_for_max_level();
    new playerScript_quests_custom_gestion();
    new spell_tanaan_inferno();

    g_BlazeOfGloryPlayerScript      = new playerScript_blaze_of_glory();
    g_APotentialAllyPlayerScript    = new playerScript_a_potential_ally();
    g_KillYourHundredPlayerScript   = new playerScript_kill_your_hundred();
    g_CaveInScript                  = new playerScript_cave_in();
    g_CavernTeleportPlayerScript    = new playerScript_cavern_teleport();
    g_MapShiftPlayerScript          = new playerScript_map_shift();
    g_GunpowderPlotPlayerScript     = new playerScript_gunpowder_plot();
    g_TasteOfIronPlayerScript       = new playerScript_taste_of_iron();
    g_TheHomeStretchPlayerScript    = new playerScript_the_home_stretch();
    g_BridgeDestructionPlayerScript = new playerscript_bridge_destruction();
    g_SceneAllianceBoatPlayerScript = new playerScript_scene_alliance_boat();
    g_SceneHordeBoatPlayerScript    = new playerScript_scene_horde_boat();
}
#endif