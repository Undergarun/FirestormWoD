#include "instance_bloodmaul.h"

namespace MS
{
    namespace Instances
    {
        namespace Bloodmaul
        {
            enum MaxEncounter
            {
                Number = 4,
            };

            static const DoorData k_DoorData[] =
            {
                { 0, 0, DOOR_TYPE_ROOM, 0 }  // EOF
            };

            class instance_Bloodmaul : public InstanceMapScript
            {
            public:
                instance_Bloodmaul() : InstanceMapScript("instance_Bloodmaul", 1175) { }

                struct instance_BloodmaulInstanceMapScript : public InstanceScript
                {
                    uint32 m_BeginningTime;
                    bool m_CanUpdate;
                    ObjectGuid m_InstanceGuid;

                    // Scenario handling.
                    uint32 m_CreatureKilled;

                    // Slave Watcher Crushto.
                    std::vector<uint64> m_NeutralMinerSpawnGuids;
                    std::list<uint64> m_CapturedMinerGuids;
                    uint32 m_OgreMageDeads;
                    std::list<uint64> m_NearestWarderGuids;

                    instance_BloodmaulInstanceMapScript(Map* p_Map)
                        : InstanceScript(p_Map),
                        m_BeginningTime(0),
                        m_CanUpdate(false),
                        m_CreatureKilled(0),
                        m_NeutralMinerSpawnGuids(),
                        m_CapturedMinerGuids(),
                        m_OgreMageDeads(0),
                        m_NearestWarderGuids()
                    {
                        SetBossNumber(MaxEncounter::Number);
                        LoadDoorData(k_DoorData);
                        //LoadScenariosInfos(k_ScenarioData, p_Map->IsChallengeMode() ? ScenarioDatas::ChallengeScenarioId : ScenarioDatas::ScenarioId);
                    }

                    void OnCreatureCreate(Creature* p_Creature)
                    {
                        switch (p_Creature->GetEntry())
                        {
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
                        } break;
                        default:
                            break;
                        }
                    }

                    void OnGameObjectCreate(GameObject* p_Gameobject)
                    {
                        switch (p_Gameobject->GetEntry())
                        {
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
                            if (m_OgreMageDeads++ == 1)
                            {
                                for (auto l_Guid : m_NearestWarderGuids)
                                {
                                    if (Creature* l_Warder = sObjectAccessor->FindCreature(l_Guid))
                                        l_Warder->Attack(p_Player, true);
                                }
                            }
                            break;
                        }

                        /*if (!instance->IsChallengeMode() || !IsChallengeModeStarted() || m_CreatureKilled >= ScenarioDatas::MaxEnnemiesToKill)
                            return;

                        if (p_Creature == nullptr)
                            return;

                        if (!p_Creature->isElite() || p_Creature->IsDungeonBoss())
                            return;

                        ++m_CreatureKilled;
                        SendScenarioProgressUpdate(CriteriaProgressData(ScenarioDatas::EnnemiesCriteriaId, m_CreatureKilled, m_InstanceGuid, time(NULL), m_BeginningTime, 0));*/
                    }

                    bool SetBossState(uint32 p_ID, EncounterState p_State)
                    {
                        if (!InstanceScript::SetBossState(p_ID, p_State))
                            return false;

                        switch (p_ID)
                        {
                        case uint32(BossIds::SlaveWatcherCrushto):
                            if (p_State == EncounterState::NOT_STARTED)
                            {
                                for (auto l_Guid : m_CapturedMinerGuids)
                                {
                                    if (Creature* l_CapturedMiner = sObjectAccessor->FindCreature(l_Guid))
                                        l_CapturedMiner->DespawnOrUnsummon();
                                }
                                m_CapturedMinerGuids.clear();
                            }
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
                                    {
                                        Position l_TargetPos;
                                        l_Plr->GetPosition(&l_TargetPos);
                                        l_Summon->DisableEvadeMode();
                                        l_Summon->getThreatManager().addThreat(l_Plr, 5000.0f);
                                        l_Summon->GetMotionMaster()->MovePoint(0, l_TargetPos);
                                        l_Summon->Attack(l_Plr, true);
                                    }
                                    m_CapturedMinerGuids.emplace_back(l_Summon->GetGUID());
                                }
                            }
                            break;
                        }
                    }

                    void SetData64(uint32 p_Type, uint64 p_Data)
                    {
                        switch (p_Type)
                        {
                        case uint32(Data::RaiseTheMinersChangeTarget):
                            if (Player* l_Plr = sObjectAccessor->FindPlayer(p_Data))
                            {
                                Position l_TargetPos;
                                l_Plr->GetPosition(&l_TargetPos);
                                for (auto l_Guid : m_CapturedMinerGuids)
                                {
                                    if (Creature* l_Summon = sObjectAccessor->FindCreature(l_Guid))
                                    {
                                        l_Summon->getThreatManager().resetAllAggro();
                                        l_Summon->getThreatManager().addThreat(l_Plr, 5000.0f);
                                        l_Summon->GetMotionMaster()->MovePoint(0, l_TargetPos);
                                        l_Summon->Attack(l_Plr, true);
                                    }
                                }
                            }
                            break;
                        }
                    }

                    void OnPlayerEnter(Player* p_Player)
                    {
                        if (!p_Player->IsInWorld())
                            return;

                        InstanceScript::OnPlayerEnter(p_Player);
                        m_CanUpdate = true;
                    }

                    void Update(uint32 p_Diff)
                    {
                        ScheduleBeginningTimeUpdate(p_Diff);
                        ScheduleChallengeStartup(p_Diff);
                        ScheduleChallengeTimeUpdate(p_Diff);

                        if (!m_CanUpdate)
                            return;

                        m_BeginningTime += p_Diff;
                    }
                };

                InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
                {
                    return new instance_BloodmaulInstanceMapScript(p_Map);
                }
            };
        }
    }
}

void AddSC_instance_Bloodmaul()
{
    new MS::Instances::Bloodmaul::instance_Bloodmaul();
}