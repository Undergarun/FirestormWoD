////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

static BossScenarios const g_BossScenarios[] =
{
    { eIronDocksDatas::DataNokgar,      eIronDocksScenario::IronDocksNokgar     },
    { eIronDocksDatas::DataGrimrail,    eIronDocksScenario::IronDocksGrimrail   },
    { eIronDocksDatas::DataOshir,       eIronDocksScenario::IronDocksOshir      },
    { eIronDocksDatas::DataSkulloc,     eIronDocksScenario::IronDocksSkulloc    },
    { 0,                                0                                       }
};

class instance_iron_docks : public InstanceMapScript
{
    public:
        instance_iron_docks() : InstanceMapScript("instance_iron_docks", 1195) { }

        struct instance_iron_docks_InstanceMapScript : public InstanceScript
        {
            instance_iron_docks_InstanceMapScript(InstanceMap* p_Map) : InstanceScript(p_Map)  { }

            uint64 m_MakoggGuid;
            uint64 m_DuguruGuid;
            uint64 m_NoxxGuid;
            uint64 m_NokgarGuid;
            uint64 m_WolfGuid;
            uint64 m_OshirGuid;
            uint64 m_SkullocGuid;
            uint64 m_ZoggoshGuid;
            uint64 m_KoramarGuid;
            uint64 m_DarunaGuid;
            uint64 m_GwarnokGuid;
            uint64 m_OlugarGuid;
            uint64 m_TurretGuid;
            /// Iron stars
            bool m_SecondEvent;
            bool m_ThirdEvent;

            /// Encounter Gates
            uint64 m_EncounterGateOshirGuid;

            void Initialize() override
            {
                SetBossNumber(4);
                LoadScenariosInfos(g_BossScenarios, instance->IsChallengeMode() ? eIronDocksScenario::IronDocksChallengeID : eIronDocksScenario::IronDocksScenarioID);

                /// Bosses
                m_MakoggGuid        = 0;
                m_DuguruGuid        = 0;
                m_NoxxGuid          = 0;
                m_NokgarGuid        = 0;
                m_WolfGuid          = 0;
                m_OshirGuid         = 0;
                m_SkullocGuid       = 0;
                /// Mini bosses
                m_DarunaGuid        = 0;
                m_GwarnokGuid       = 0;
                m_OlugarGuid        = 0;
                // Triggers
                // Gobs
                // Creatures
                m_TurretGuid        = 0;
                m_ZoggoshGuid       = 0;
                m_KoramarGuid       = 0;
                m_SecondEvent       = false;
                m_ThirdEvent        = false;

                /// Encounter Gates
                m_EncounterGateOshirGuid = 0;
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                case eIronDocksCreatures::CreatureFleshrenderNokgar:
                        m_NokgarGuid = p_Creature->GetGUID();
                        break;
                case eIronDocksCreatures::CreatureOshir:
                        m_OshirGuid = p_Creature->GetGUID();
                        break;
                case eIronDocksCreatures::CreatureSkulloc:
                        m_SkullocGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureNox:
                        m_NoxxGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureMakogg:
                        m_MakoggGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureDuguru:
                        m_DuguruGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureDreadfang:
                        m_WolfGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureZoggosh:
                        m_ZoggoshGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureKoramar:
                        m_KoramarGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureChampionDruna:
                        m_DarunaGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreaturePitwardenGwarnok:
                        m_GwarnokGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureSiegemasterOlugar:
                        m_OlugarGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::CreatureTurret:
                        m_TurretGuid = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eIronDocksGameObject::GameObjectChallengeModeDoor:
                        AddChallengeModeDoor(p_GameObject);
                        break;
                    case CHALLENGE_MOD_ORB:
                        m_ChallengeOrbGuid = p_GameObject->GetGUID();
                        break;
                    case eIronDocksGameObject::GameObjectEncounterGateOshir:
                        m_EncounterGateOshirGuid = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnUnitDeath(Unit* p_Unit) override
            {
                Creature* l_Creature = p_Unit->ToCreature();
                if (!l_Creature)
                    return;

                switch (l_Creature->GetEntry())
                {
                    case eIronDocksCreatures::CreatureFleshrenderNokgar:
                    {
                        if (Creature* l_Daruna = instance->GetCreature(GetData64(eIronDocksDatas::DataDaruna)))
                        {
                            if (Creature* l_Olugar = instance->GetCreature(GetData64(eIronDocksDatas::DataOlugar)))
                            {
                                if (Creature* l_Gwarnok = instance->GetCreature(GetData64(eIronDocksDatas::DataGwarnok)))
                                {
                                    if (l_Gwarnok->isAlive() && l_Olugar->isAlive() && l_Daruna->isAlive())
                                        DoCompleteAchievement(eIronDocksAchievements::AchievementMilitaristicExpansionist);
                                }
                            }
                        }

                        break;
                    }
                    case eIronDocksCreatures::CreatureSkulloc:
                    {
                        if (l_Creature->GetMap()->IsHeroic())
                            DoCompleteAchievement(eIronDocksAchievements::AchievementHeroicIronDocks);
                        else
                            DoCompleteAchievement(eIronDocksAchievements::AchievementNormalIronDocks);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnCreatureKilled(Creature* p_Creature, Player* p_Player) override
            {
                if (!instance->IsChallengeMode() || !IsChallengeModeStarted() || m_CreatureKilled >= eIronDocksScenario::IronDocksKillCount)
                    return;

                if (p_Creature == nullptr)
                    return;

                if (!p_Creature->isElite() || p_Creature->IsDungeonBoss())
                    return;

                ++m_CreatureKilled;
                SendScenarioProgressUpdate(CriteriaProgressData(eIronDocksScenario::IronDocksEnnemies, m_CreatureKilled, m_InstanceGuid, time(nullptr), m_BeginningTime, 0));

                if (m_CreatureKilled >= eIronDocksScenario::IronDocksKillCount)
                    m_ConditionCompleted = true;
            }

            uint64 GetData64(uint32 p_Type) override
            {
                switch (p_Type)
                {
                    /// Bosses
                    case eIronDocksDatas::DataNokgar:
                        return m_NokgarGuid;
                    case eIronDocksDatas::DataOshir:
                        return m_OshirGuid;
                    case eIronDocksDatas::DataSkulloc:
                        return m_SkullocGuid;
                    case eIronDocksDatas::DataGrimrailDuguru:
                        return m_DuguruGuid;
                    case eIronDocksDatas::DataGrimrailMakogg:
                        return m_MakoggGuid;
                    case eIronDocksDatas::DataGrimrailNoxx:
                        return m_NoxxGuid;
                    case eIronDocksDatas::DataMountWolf:
                        return m_WolfGuid;
                    case eIronDocksDatas::DataTurret:
                        return m_TurretGuid;
                    // Mini Bosses
                    case eIronDocksDatas::DataDaruna:
                        return m_DarunaGuid;
                    case eIronDocksDatas::DataGwarnok:
                        return m_GwarnokGuid;
                    case eIronDocksDatas::DataOlugar:
                        return m_OlugarGuid;
                    /// RP
                    case eIronDocksDatas::DataZuggosh:
                        return m_ZoggoshGuid;
                    case eIronDocksDatas::DataKoramar:
                        return m_KoramarGuid;
                    /// Encounter Gates
                    case eIronDocksDatas::DataEncounterGateOshir:
                        return m_EncounterGateOshirGuid;
                        break;
                    default:
                        break;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

                switch (type)
                {
                    case eIronDocksDatas::DataSkulloc:
                    {
                        if (state == EncounterState::DONE)
                            DoKilledMonsterKredit(eIronDocksScenario::DailyChallengeQuestID, eIronDocksScenario::DailyChallengeKillCredit);

                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void Update(uint32 p_Diff) override
            {
                UpdateOperations(p_Diff);
                ScheduleBeginningTimeUpdate(p_Diff);
                ScheduleChallengeStartup(p_Diff);
                ScheduleChallengeTimeUpdate(p_Diff);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const override
        {
            return new instance_iron_docks_InstanceMapScript(p_Map);
        }
};

void AddSC_instance_iron_docks()
{
    new instance_iron_docks();
}