#include "instance_skyreach.h"

namespace MS
{
    namespace InstanceSkyreach
    {
        enum MaxEncounter
        {
            Number = 4,
        };

        static const DoorData k_DoorData[] =
        {
            { DOOR_RANJIT_ENTRANCE,     Data::Ranjit,   DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_RANJIT_EXIT,         Data::Ranjit,   DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_ARAKNATH_ENTRANCE_1, Data::Araknath, DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_ARAKNATH_ENTRANCE_2, Data::Araknath, DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_ARAKNATH_EXIT_1,     Data::Araknath, DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_ARAKNATH_EXIT_2,     Data::Araknath, DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_RUKHRAN_ENTRANCE,    Data::Rukhran,  DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_RUKHRAN_EXIT,        Data::Rukhran,  DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { 0,                        0,              DOOR_TYPE_ROOM,     0 }  // EOF
        };

        class instance_Skyreach : public InstanceMapScript
        {
        public:
            instance_Skyreach() : InstanceMapScript("instance_Skyreach", 1209) { }

            struct instance_SkyreachInstanceMapScript : public InstanceScript
            {
                uint32 m_BeginningTime;
                bool   m_CanUpdate;
                ObjectGuid m_InstanceGuid;

                // Araknath part.
                uint64 m_AraknathGuid;
                uint64 m_SkyreachArcanologistGuid;
                std::list<uint64> m_SolarConstructorsGuid;
                uint64 m_SelectedSolarConstructor;

                // Rukhran part.
                uint64 m_RukhranGuid;
                uint64 m_SkyreachRavenWhispererGuid;

                instance_SkyreachInstanceMapScript(Map* p_Map) 
                    : InstanceScript(p_Map),
                    m_BeginningTime(0),
                    m_CanUpdate(false),
                    m_InstanceGuid(MAKE_NEW_GUID(instance->GetId(), 0, HIGHGUID_INSTANCE_SAVE)),
                    m_AraknathGuid(0),
                    m_SkyreachArcanologistGuid(0),
                    m_SolarConstructorsGuid(),
                    m_SelectedSolarConstructor(0),
                    m_RukhranGuid(0)
                {
                    SetBossNumber(MaxEncounter::Number);
                    LoadDoorData(k_DoorData);
                }

                void OnCreatureCreate(Creature* p_Creature)
                {
                    switch (p_Creature->GetEntry())
                    {
                    case BossEntries::RANJIT:
                        break;
                    case BossEntries::ARAKNATH:
                        m_AraknathGuid = p_Creature->GetGUID();
                        break;
                    case BossEntries::RUKHRAN:
                        m_RukhranGuid = p_Creature->GetGUID();
                        break;
                    case MobEntries::SKYREACH_ARCANALOGIST:
                        m_SkyreachArcanologistGuid = p_Creature->GetGUID();
                        break;
                    case MobEntries::SKYREACH_SOLAR_CONSTRUCTOR:
                        m_SolarConstructorsGuid.emplace_front(p_Creature->GetGUID());
                        p_Creature->RemoveAura(RandomSpells::SUBMERGED);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        p_Creature->CastSpell(p_Creature, uint32(RandomSpells::ENERGIZE_VISUAL_1));
                        break;
                    case MobEntries::SKYREACH_RAVEN_WHISPERER:
                        m_SkyreachRavenWhispererGuid = p_Creature->GetGUID();
                        break;
                    case MobEntries::YOUNG_KALIRI:
                        // Setting fly.
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        break;
                    default:
                        break;
                    }
                }

                void OnGameObjectCreate(GameObject* p_Gameobject)
                {
                    switch (p_Gameobject->GetEntry())
                    {
                    case GameObjectEntries::DOOR_RANJIT_ENTRANCE:
                    case GameObjectEntries::DOOR_RANJIT_EXIT:
                    case GameObjectEntries::DOOR_ARAKNATH_ENTRANCE_1:
                    case GameObjectEntries::DOOR_ARAKNATH_ENTRANCE_2:
                    case GameObjectEntries::DOOR_ARAKNATH_EXIT_1:
                    case GameObjectEntries::DOOR_ARAKNATH_EXIT_2:
                    case GameObjectEntries::DOOR_RUKHRAN_ENTRANCE:
                    case GameObjectEntries::DOOR_RUKHRAN_EXIT:
                        AddDoor(p_Gameobject, true);
                        break;
                    case GameObjectEntries::CACHE_OF_ARAKKOAN_TREASURES:
                        p_Gameobject->RemoveFromWorld();
                        break;
                    default:
                        break;
                    }
                }

                bool SetBossState(uint32 p_ID, EncounterState p_State)
                {
                    if (!InstanceScript::SetBossState(p_ID, p_State))
                        return false;

                    switch (p_ID)
                    {
                    case Data::Ranjit:
                        break;
                    case Data::Araknath:
                        switch (p_State)
                        {
                        case FAIL:
                            if (Creature* l_SkyreachArcanologist = sObjectAccessor->FindCreature(m_SkyreachArcanologistGuid))
                                l_SkyreachArcanologist->Respawn();
                            break;
                        }
                        break;
                    case Data::Rukhran:
                        switch (p_State)
                        {
                        case FAIL:
                            if (Creature* l_SkyreachRavenWhisperer = sObjectAccessor->FindCreature(m_SkyreachRavenWhispererGuid))
                                l_SkyreachRavenWhisperer->Respawn();
                            break;
                        }
                        break;
                    case Data::SkyreachArcanologist:
                        break;
                    default:
                        break;
                    }

                    return true;
                }

                void SetData(uint32 p_Type, uint32 p_Data)
                {
                    switch (p_Type)
                    {
                    case Data::SkyreachRavenWhispererIsDead:
                        if (Creature* l_Rukhran = sObjectAccessor->FindCreature(m_RukhranGuid))
                        {
                            l_Rukhran->GetMotionMaster()->Clear(true);
                            l_Rukhran->GetMotionMaster()->MovePoint(12, 918.92f, 1913.46f, 215.87f);
                        }
                        break;
                    case Data::SkyreachArcanologistIsDead:
                        if (Creature* l_Araknath = sObjectAccessor->FindCreature(m_AraknathGuid))
                        {
                            l_Araknath->RemoveAura(RandomSpells::SUBMERGED);
                            l_Araknath->SetReactState(REACT_AGGRESSIVE);
                        }

                        for (uint64 l_Guid : m_SolarConstructorsGuid)
                        {
                            if (Creature* l_Constructor = sObjectAccessor->FindCreature(l_Guid))
                            {
                                l_Constructor->CastStop();
                                l_Constructor->AddAura(RandomSpells::SUBMERGED, l_Constructor);
                            }
                        }
                        break;
                    case Data::SkyreachArcanologistReset:
                        if (Creature* l_Araknath = sObjectAccessor->FindCreature(m_AraknathGuid))
                        {
                            l_Araknath->getThreatManager().resetAllAggro();

                            if (l_Araknath->GetAI())
                                l_Araknath->GetAI()->Reset();
                        }

                        for (uint64 l_Guid : m_SolarConstructorsGuid)
                        {
                            if (Creature* l_Constructor = sObjectAccessor->FindCreature(l_Guid))
                            {
                                l_Constructor->CastSpell(l_Constructor, uint32(RandomSpells::ENERGIZE_VISUAL_1));
                                l_Constructor->RemoveAura(RandomSpells::SUBMERGED);
                            }
                        }
                        break;
                    case Data::AraknathSolarConstructorActivation:
                        if (p_Data)
                        {
                            auto l_RandUnit = m_SolarConstructorsGuid.begin();
                            std::advance(l_RandUnit, urand(0, m_SolarConstructorsGuid.size() - 1));
                            m_SelectedSolarConstructor = *l_RandUnit;

                            if (Creature* l_Constructor = sObjectAccessor->FindCreature(m_SelectedSolarConstructor))
                            {
                                l_Constructor->RemoveAura(RandomSpells::SUBMERGED);
                                l_Constructor->CastSpell(l_Constructor, uint32(RandomSpells::ENERGIZE_HEAL));
                            }
                        }
                        else
                        {
                            if (Creature* l_Constructor = sObjectAccessor->FindCreature(m_SelectedSolarConstructor))
                            {
                                l_Constructor->AddAura(RandomSpells::SUBMERGED, l_Constructor);
                                l_Constructor->CastStop();
                            }
                        }
                    default:
                        break;
                    }
                }

                uint32 GetData(uint32 p_Type)
                {
                    switch (p_Type)
                    {
                    default:
                        break;
                    }

                    return 0;
                }

                uint64 GetData64(uint32 p_Type)
                {
                    switch (p_Type)
                    {
                    default:
                        return 0;
                    }
                }

                void OnPlayerEnter(Player* p_Player)
                {
                    if (!p_Player->IsInWorld())
                        return;

                    m_CanUpdate = true;
                }

                void Update(uint32 p_Diff)
                {
                    if (!m_CanUpdate)
                        return;

                    m_BeginningTime += p_Diff;
                }
            };

            InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
            {
                return new instance_SkyreachInstanceMapScript(p_Map);
            }
        };
    }
}

void AddSC_instance_Skyreach()
{
    new MS::InstanceSkyreach::instance_Skyreach();
}

