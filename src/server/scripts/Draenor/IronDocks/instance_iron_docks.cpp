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

class basicevent_iron_stars : public BasicEvent
{
    public:
        explicit basicevent_iron_stars(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value), BasicEvent() { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (!m_Modifier)
                            l_Zoggosh->AI()->Talk(eIronDocksTalks::TalkZoggosh03_2);
                    }
                }
            }

            return true;
        }

    private:
        Unit* m_Obj;
        int m_Modifier;
};

class basicevent_bombardment_bridge : public BasicEvent
{
    public:
        explicit basicevent_bombardment_bridge(Unit* unit, int value) : m_Obj(unit), m_Modifier(value), BasicEvent() { }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                {
                    if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                    {
                        if (Creature* l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                        {
                            if (l_Koramar->IsAIEnabled && l_Skulloc->IsAIEnabled && l_Zoggosh->IsAIEnabled)
                            {
                                switch (m_Modifier)
                                {
                                    case 0:
                                    {
                                        l_Koramar->AI()->Talk(eIronDocksTalks::TalkKoramar010);
                                        l_Koramar->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 1:
                                    {
                                        l_Zoggosh->AI()->Talk(eIronDocksTalks::TalkZoggosh04);
                                        l_Zoggosh->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Zoggosh, 9), l_Zoggosh->m_Events.CalculateTime(2 * TimeConstants::IN_MILLISECONDS));
                                        break;
                                    }
                                    case 9:
                                    {
                                        if (l_Skulloc->GetVehicleKit() && l_Skulloc->GetVehicleKit()->GetPassenger(0))
                                        {
                                            if (Creature* l_Turret = l_Skulloc->GetVehicleKit()->GetPassenger(0)->ToCreature())
                                            {
                                                l_Turret->CastSpell(l_Turret, eIronDocksSpell::SpellCannonBarrageAura);
                                                l_Turret->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Turret, 9), l_Turret->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                            }
                                        }

                                        break;
                                    }
                                    default:
                                        break;
                                }
                            }
                        }
                    }
                }
            }

            return true;
        }

    private:
        Unit* m_Obj;
        int m_Modifier;
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
            /// Scenario handling
            uint32 m_CreatureKilled;

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
                m_CreatureKilled    = 0;
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eIronDocksCreatures::BossFleshrenderNokgar:
                        m_NokgarGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::BossOshir:
                        m_OshirGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::BossSkulloc:
                        m_SkullocGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcNox:
                        m_NoxxGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcMakogg:
                        m_MakoggGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcDuguru:
                        m_DuguruGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::BossDreadfang:
                        m_WolfGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcZoggosh:
                        m_ZoggoshGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcKoramar:
                        m_KoramarGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcChampionDruna:
                        m_DarunaGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcPitwardenGwarnok:
                        m_GwarnokGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::NpcSiegemasterOlugar:
                        m_OlugarGuid = p_Creature->GetGUID();
                        break;
                    case eIronDocksCreatures::BossTurret:
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
                    case eIronDocksGameObject::ChallengeModeDoor:
                        m_ChallengeDoorGuid = p_GameObject->GetGUID();
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
                    case eIronDocksCreatures::BossFleshrenderNokgar:
                    {
                        if (Creature* l_Daruna = instance->GetCreature(GetData64(eIronDocksDatas::DataDaruna)))
                        {
                            if (Creature* l_Olugar = instance->GetCreature(GetData64(eIronDocksDatas::DataOlugar)))
                            {
                                if (Creature* l_Gwarnok = instance->GetCreature(GetData64(eIronDocksDatas::DataGwarnok)))
                                {
                                    if (l_Gwarnok->isAlive() && l_Olugar->isAlive() && l_Daruna->isAlive())
                                        DoCompleteAchievement(eIronDocksAchievements::MilitaristicExpansionist);
                                }
                            }
                        }

                        break;
                    }
                    case eIronDocksCreatures::BossSkulloc:
                    {
                        if (l_Creature->GetMap()->IsHeroic())
                            DoCompleteAchievement(eIronDocksAchievements::HeroicIronDocks);
                        else
                            DoCompleteAchievement(eIronDocksAchievements::NormalIronDocks);
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

            void SetData(uint32 p_Type, uint32 p_Data) override
            {
                switch (p_Type)
                {
                    case eIronDocksDatas::DataSecondEvent:
                    {
                        if (!m_SecondEvent)
                        {
                            m_SecondEvent = true;

                            if (Creature* l_Skulloc = instance->GetCreature(GetData64(eIronDocksDatas::DataSkulloc)))
                                l_Skulloc->m_Events.AddEvent(new basicevent_iron_stars(l_Skulloc, 0), l_Skulloc->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                        }

                        break;
                    }
                    case eIronDocksDatas::DataThirdEvent:
                    {
                        if (!m_ThirdEvent)
                        {
                            m_ThirdEvent = true;

                            if (Creature* l_Skulloc = instance->GetCreature(GetData64(eIronDocksDatas::DataSkulloc)))
                                l_Skulloc->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Skulloc, 0), l_Skulloc->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                        }

                        break;
                    }
                    default:
                        break;
                }
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
                    default:
                        break;
                }

                return 0;
            }

            bool SetBossState(uint32 type, EncounterState state) override
            {
                if (!InstanceScript::SetBossState(type, state))
                    return false;

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