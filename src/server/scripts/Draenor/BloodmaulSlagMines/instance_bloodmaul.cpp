////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "instance_bloodmaul.hpp"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {
            enum MaxEncounter
            {
                Number = 4
            };

            static const DoorData k_DoorData[] =
            {
                { GameObjects::RoltallBridge,       BossIds::BossForgemasterGogduh, DoorType::DOOR_TYPE_PASSAGE, BoundaryType::BOUNDARY_NONE },
                { GameObjects::RoltallEntranceWall, BossIds::BossForgemasterGogduh, DoorType::DOOR_TYPE_PASSAGE, BoundaryType::BOUNDARY_NONE },
                { GameObjects::RoltallExitWall,     BossIds::BossRoltall,           DoorType::DOOR_TYPE_PASSAGE, BoundaryType::BOUNDARY_NONE },
                { 0,                                0,                              DoorType::DOOR_TYPE_ROOM,    0                           }  // EOF
            };

            static const BossScenarios g_BossScenarios[] =
            {
                { BossIds::BossSlaveWatcherCrushto, eScenarioDatas::BloodmaulCrushto    },
                { BossIds::BossForgemasterGogduh,   eScenarioDatas::BloodmaulMagmolatus },
                { BossIds::BossRoltall,             eScenarioDatas::BloodmaulRoltall    },
                { BossIds::BossGugrokk,             eScenarioDatas::BloodmaulGugrokk    },
                { 0,                                0                                   }
            };

            class instance_Bloodmaul : public InstanceMapScript
            {
                public:
                    instance_Bloodmaul() : InstanceMapScript("instance_Bloodmaul", 1175) { }

                    struct instance_BloodmaulInstanceMapScript : public InstanceScript
                    {
                        /// Slave Watcher Crushto.
                        std::vector<uint64> m_NeutralMinerSpawnGuids;
                        std::list<uint64> m_CapturedMinerGuids;
                        uint32 m_OgreMageDeads;
                        std::list<uint64> m_NearestWarderGuids;
                        uint64 m_slaveWatcherCrushtoGuid;

                        /// Slagna
                        bool m_SlagnaSpawned;

                        /// Gug'rokk
                        uint64 m_GugrokkGuid;
                        uint32 m_UnstableSlagKilled;

                        uint32 m_CheckZPosTimer;

                        uint64 m_CromanGuid;
                        uint64 m_CromanRealGuid;
                        uint64 m_SteelSwordGuid;

                        instance_BloodmaulInstanceMapScript(Map* p_Map)
                            : InstanceScript(p_Map),
                            m_NeutralMinerSpawnGuids(),
                            m_CapturedMinerGuids(),
                            m_OgreMageDeads(0),
                            m_NearestWarderGuids(),
                            m_slaveWatcherCrushtoGuid(0),
                            m_SlagnaSpawned(false),
                            m_GugrokkGuid(0),
                            m_UnstableSlagKilled(0),
                            m_CheckZPosTimer(1000),
                            m_CromanGuid(0),
                            m_CromanRealGuid(0),
                            m_SteelSwordGuid(0)
                        {
                            SetBossNumber(MaxEncounter::Number);
                            LoadDoorData(k_DoorData);
                            LoadScenariosInfos(g_BossScenarios, p_Map->IsChallengeMode() ? eScenarioDatas::BloodmaulChallengeID : eScenarioDatas::BloodmaulScenarioID);
                        }

                        void OnCreatureCreate(Creature* p_Creature)
                        {
                            switch (p_Creature->GetEntry())
                            {
                                case uint32(MobEntries::SlaveWatcherCrushto):
                                    m_slaveWatcherCrushtoGuid = p_Creature->GetGUID();
                                    break;
                                case uint32(MobEntries::MinesBat):
                                    p_Creature->SetDisableGravity(true);
                                    p_Creature->SetCanFly(true);
                                    p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                                    break;
                                case uint32(MobEntries::NeutralMinerSpawn):
                                    p_Creature->setFaction(2102);
                                    p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                                    m_NeutralMinerSpawnGuids.emplace_back(p_Creature->GetGUID());
                                    break;
                                case uint32(MobEntries::EarthCrushStalker):
                                    p_Creature->setFaction(2102);
                                    p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                                    break;
                                case uint32(MobEntries::BloodmaulWarder):
                                {
                                    static const Position k_CrushtoPosition = { 2038.51f, -361.126f, 223.f };

                                    if (k_CrushtoPosition.GetExactDist2d(p_Creature) < 50.0f)
                                        m_NearestWarderGuids.emplace_back(p_Creature->GetGUID());
                                    break;
                                }
                                case uint32(MobEntries::MoltenEarthElemental):
                                    if (Unit* l_Unit = ScriptUtils::SelectNearestCreatureWithEntry(p_Creature, uint32(MobEntries::BloodmaulWarder), 50.0f))
                                        p_Creature->Attack(l_Unit, false);
                                    break;
                                case uint32(MobEntries::Gugrokk):
                                    m_GugrokkGuid = p_Creature->GetGUID();
                                    break;
                                case uint32(MobEntries::CromanTheBarbarian):
                                    m_CromanGuid = p_Creature->GetGUID();
                                    break;
                                case uint32(MobEntries::CromanTheBarbarianReal):
                                    m_CromanRealGuid = p_Creature->GetGUID();
                                    break;
                                default:
                                    break;
                            }
                        }

                        void OnGameObjectCreate(GameObject* p_GameObject)
                        {
                            switch (p_GameObject->GetEntry())
                            {
                                case GameObjects::RoltallBridge:
                                case GameObjects::RoltallEntranceWall:
                                case GameObjects::RoltallExitWall:
                                    AddDoor(p_GameObject, true);
                                    break;
                                case GameObjects::ChallengeDoor:
                                    m_ChallengeDoorGuid = p_GameObject->GetGUID();
                                    break;
                                case CHALLENGE_MOD_ORB:
                                    m_ChallengeOrbGuid = p_GameObject->GetGUID();
                                    break;
                                case GameObjects::SteelSword:
                                    m_SteelSwordGuid = p_GameObject->GetGUID();
                                    break;
                                default:
                                    break;
                            }
                        }

                        void OnGameObjectRemove(GameObject* p_GameObject) override
                        {
                            switch (p_GameObject->GetEntry())
                            {
                                case GameObjects::RoltallBridge:
                                case GameObjects::RoltallEntranceWall:
                                case GameObjects::RoltallExitWall:
                                    AddDoor(p_GameObject, false);
                                    break;
                                default:
                                    break;
                            }
                        }

                        void OnCreatureKilled(Creature* p_Creature, Player* p_Player)
                        {
                            switch (p_Creature->GetEntry())
                            {
                                case uint32(MobEntries::CapturedMiner1):
                                    m_CapturedMinerGuids.remove(p_Creature->GetGUID());
                                    p_Creature->DespawnOrUnsummon();
                                    break;
                                case uint32(MobEntries::BloodmaulOgreMage):
                                {
                                    if (!m_NearestWarderGuids.empty())
                                    {
                                        auto l_Itr = std::begin(m_NearestWarderGuids);
                                        std::advance(l_Itr, m_OgreMageDeads);
                                        if (Creature* l_Warder = sObjectAccessor->FindCreature(*l_Itr))
                                        {
                                            l_Warder->GetMotionMaster()->MoveChase(p_Player);
                                            l_Warder->Attack(p_Player, true);

                                            if (l_Warder->AI())
                                                l_Warder->AI()->Talk(uint32(Talks::WarderAttack));
                                        }

                                        ++m_OgreMageDeads;

                                        if (m_OgreMageDeads == 2)
                                        {
                                            m_OgreMageDeads = 0;

                                            if (Creature* l_Boss = sObjectAccessor->FindCreature(m_slaveWatcherCrushtoGuid))
                                            {
                                                if (l_Boss->AI())
                                                    l_Boss->AI()->Talk(uint32(SlaverWatcherCrushto::Texts::TalkIntro));
                                            }
                                        }
                                    }
                                    break;
                                }
                                case MobEntries::UnstableSlag:
                                    ++m_UnstableSlagKilled;
                                    break;
                                default:
                                    break;
                            }

                            if (!instance->IsChallengeMode() || !IsChallengeModeStarted() || m_CreatureKilled >= eScenarioDatas::BloodmaulKillCount)
                                return;

                            if (p_Creature == nullptr)
                                return;

                            if (!p_Creature->isElite() || p_Creature->IsDungeonBoss())
                                return;

                            ++m_CreatureKilled;
                            SendScenarioProgressUpdate(CriteriaProgressData(eScenarioDatas::BloodmaulEnnemies, m_CreatureKilled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));

                            if (m_CreatureKilled >= eScenarioDatas::BloodmaulKillCount)
                                m_ConditionCompleted = true;
                        }

                        bool SetBossState(uint32 p_ID, EncounterState p_State)
                        {
                            if (!InstanceScript::SetBossState(p_ID, p_State))
                                return false;

                            switch (p_ID)
                            {
                                case BossIds::BossSlaveWatcherCrushto:
                                {
                                    if (p_State == EncounterState::NOT_STARTED)
                                    {
                                        for (uint64 l_Guid : m_CapturedMinerGuids)
                                        {
                                            if (Creature* l_CapturedMiner = sObjectAccessor->FindCreature(l_Guid))
                                                l_CapturedMiner->DespawnOrUnsummon();
                                        }

                                        m_CapturedMinerGuids.clear();
                                        m_OgreMageDeads = 0;
                                    }
                                    else if (p_State == EncounterState::DONE)
                                    {
                                        for (uint64 l_Guid : m_CapturedMinerGuids)
                                        {
                                            if (Creature* l_CapturedMiner = sObjectAccessor->FindCreature(l_Guid))
                                            {
                                                if (l_CapturedMiner->AI())
                                                    l_CapturedMiner->AI()->Talk(uint32(Talks::CapturedMinerReleased));
                                                l_CapturedMiner->CombatStop();
                                                l_CapturedMiner->SetReactState(ReactStates::REACT_PASSIVE);
                                            }
                                        }

                                        if (Creature* l_Croman = sObjectAccessor->FindCreature(m_CromanGuid))
                                        {
                                            if (!l_Croman->IsAIEnabled)
                                                break;

                                            l_Croman->AI()->DoAction(0);    ///< ActionEnableClick
                                        }
                                    }
                                    break;
                                }
                                case BossIds::BossForgemasterGogduh:
                                {
                                    if (p_State != EncounterState::DONE)
                                        break;

                                    if (Creature* l_Croman = sObjectAccessor->FindCreature(m_CromanRealGuid))
                                    {
                                        if (!l_Croman->IsAIEnabled)
                                            break;

                                        l_Croman->AI()->DoAction(0);    ///< ActionPickUpSword
                                    }

                                    break;
                                }
                                case BossIds::BossGugrokk:
                                {
                                    if (p_State == EncounterState::NOT_STARTED)
                                        m_UnstableSlagKilled = 0;
                                    else if (p_State == EncounterState::DONE && m_UnstableSlagKilled == 0 && instance->IsHeroic())
                                        DoCompleteAchievement(eAchievements::IsDraenorOnFire);

                                    if (p_State != EncounterState::DONE)
                                        break;

                                    if (Creature* l_Croman = sObjectAccessor->FindCreature(m_CromanRealGuid))
                                    {
                                        if (!l_Croman->IsAIEnabled)
                                            break;

                                        l_Croman->AI()->DoAction(1);    ///< ActionFreed
                                    }

                                    break;
                                }
                                default:
                                    break;
                            }

                            return true;
                        }

                        void SetData(uint32 p_Type, uint32 p_Data)
                        {
                            switch (p_Type)
                            {
                                case uint32(Data::RaiseTheMiners):
                                    for (auto l_Guid : m_NeutralMinerSpawnGuids)
                                    {
                                        if (Creature* l_Spawn = sObjectAccessor->FindCreature(l_Guid))
                                        {
                                            Position l_Pos;
                                            l_Spawn->GetPosition(&l_Pos);
                                            TempSummon* l_Summon = l_Spawn->SummonCreature(uint32(MobEntries::CapturedMiner1), l_Pos);
                                            l_Summon->SetHealth(l_Summon->GetMaxHealth() / 2.0f);
                                            if (Player* l_Plr = ScriptUtils::SelectRandomPlayerIncludedTank(l_Summon, 80.0f, false))
                                                l_Summon->GetMotionMaster()->MoveChase(l_Plr);
                                            m_CapturedMinerGuids.emplace_back(l_Summon->GetGUID());
                                        }
                                    }
                                    break;
                                case uint32(Data::SpawnSlagna):
                                {
                                    if (m_SlagnaSpawned)
                                        break;

                                    static const Position k_SpawnSlagna = { 2191.21f, -191.67f, 213.72f };
                                    if (Creature* l_Slagna = instance->SummonCreature(uint32(MobEntries::Slagna), k_SpawnSlagna))
                                    {
                                        m_SlagnaSpawned = true;
                                        if (l_Slagna->GetAI())
                                            l_Slagna->GetAI()->SetData(uint32(Data::SpawnSlagna), 0);
                                    }

                                    break;
                                }
                            }
                        }

                        void SetData64(uint32 p_Type, uint64 p_Data)
                        {
                            switch (p_Type)
                            {
                                case uint32(Data::RaiseTheMinersChangeTarget):
                                    if (Player* l_Plr = sObjectAccessor->FindPlayer(p_Data))
                                    {
                                        for (auto l_Guid : m_CapturedMinerGuids)
                                        {
                                            if (Creature* l_Summon = sObjectAccessor->FindCreature(l_Guid))
                                                l_Summon->GetMotionMaster()->MoveChase(l_Plr);
                                        }
                                    }
                                    break;
                            }
                        }

                        uint64 GetData64(uint32 p_Type)
                        {
                            switch (p_Type)
                            {
                                case uint32(MobEntries::Gugrokk):
                                    return m_GugrokkGuid;
                                case uint32(MobEntries::CromanTheBarbarian):
                                    return m_CromanGuid;
                                case uint32(MobEntries::CromanTheBarbarianReal):
                                    return m_CromanRealGuid;
                                case GameObjects::SteelSword:
                                    return m_SteelSwordGuid;
                                default:
                                    break;
                            }

                            return 0;
                        }

                        void Update(uint32 p_Diff)
                        {
                            UpdateOperations(p_Diff);

                            CheckPositionZForPlayers(p_Diff);
                            ScheduleBeginningTimeUpdate(p_Diff);
                            ScheduleChallengeStartup(p_Diff);
                            ScheduleChallengeTimeUpdate(p_Diff);
                        }

                        void CheckPositionZForPlayers(uint32 p_Diff)
                        {
                            if (!m_CheckZPosTimer)
                                return;

                            if (m_CheckZPosTimer <= p_Diff)
                            {
                                Map::PlayerList const& l_PlayerList = instance->GetPlayers();
                                for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                                {
                                    if (Player* l_Player = l_Iter->getSource())
                                    {
                                        if (l_Player->GetPositionZ() <= 150.0f)
                                            l_Player->Kill(l_Player);
                                    }
                                }

                                m_CheckZPosTimer = 1000;
                            }
                            else
                                m_CheckZPosTimer -= p_Diff;
                        }
                    };

                    InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
                    {
                        return new instance_BloodmaulInstanceMapScript(p_Map);
                    }
            };

            class AreaTrigger_at_SpawnSlagna : public AreaTriggerScript
            {
                public:

                    AreaTrigger_at_SpawnSlagna() : AreaTriggerScript("at_SpawnSlagna") { }

                    bool OnTrigger(Player* p_Player, AreaTriggerEntry const* /*trigger*/)
                    {
                        if (p_Player->GetInstanceScript())
                        {
                            p_Player->GetInstanceScript()->SetData(uint32(Data::SpawnSlagna), 0);
                            return true;
                        }

                        return false;
                    }
            };
        }
    }
}

void AddSC_instance_Bloodmaul()
{
    new MS::Instances::Bloodmaul::instance_Bloodmaul();
    new MS::Instances::Bloodmaul::AreaTrigger_at_SpawnSlagna();
}