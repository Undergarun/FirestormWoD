#include "instance_skyreach.h"
#include "ObjectAccessor.h"

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
            { DOOR_RANJIT_ENTRANCE,             Data::Ranjit,           DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_RANJIT_EXIT,                 Data::Ranjit,           DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_ARAKNATH_ENTRANCE_1,         Data::Araknath,         DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_ARAKNATH_ENTRANCE_2,         Data::Araknath,         DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_ARAKNATH_EXIT_1,             Data::Araknath,         DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_ARAKNATH_EXIT_2,             Data::Araknath,         DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_RUKHRAN_ENTRANCE,            Data::Rukhran,          DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { DOOR_RUKHRAN_EXIT,                Data::Rukhran,          DOOR_TYPE_PASSAGE,  BOUNDARY_NONE },
            { DOOR_HIGH_SAVE_VIRYX_ENTRANCE,    Data::HighSageViryx,    DOOR_TYPE_ROOM,     BOUNDARY_NONE },
            { 0,                                0,                      DOOR_TYPE_ROOM,     0 }  // EOF
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
                uint64 m_SelectedSolarConstructorGuid;
                uint64 m_InteriorFocusGuid;
                uint64 m_SolarConstructorEnergizerGuid;

                // Rukhran part.
                uint64 m_RukhranGuid;
                uint64 m_SkyreachRavenWhispererGuid;
                std::set<uint64> m_PileOfAshesGuid;
                std::set<uint64> m_SolarFlaresGuid;
                uint64 m_CacheOfArakoanTreasuresGuid;

                // Wind maze zone.
                std::map<uint64, uint32> m_PlayerGuidToBlockId;
                std::vector<uint64> m_WindMazeBlockGuids;

                // High Sage Viryx.
                std::vector<uint64> m_MagnifyingGlassFocusGuids;

                instance_SkyreachInstanceMapScript(Map* p_Map) 
                    : InstanceScript(p_Map),
                    m_BeginningTime(0),
                    m_CanUpdate(false),
                    m_InstanceGuid(MAKE_NEW_GUID(instance->GetId(), 0, HIGHGUID_INSTANCE_SAVE)),
                    m_AraknathGuid(0),
                    m_SkyreachArcanologistGuid(0),
                    m_SolarConstructorsGuid(),
                    m_SelectedSolarConstructorGuid(0),
                    m_InteriorFocusGuid(0),
                    m_RukhranGuid(0),
                    m_SkyreachRavenWhispererGuid(0),
                    m_PileOfAshesGuid(),
                    m_SolarFlaresGuid(),
                    m_CacheOfArakoanTreasuresGuid(0),
                    m_SolarConstructorEnergizerGuid(0),
                    m_PlayerGuidToBlockId(),
                    m_WindMazeBlockGuids(),
                    m_MagnifyingGlassFocusGuids()
                {
                    SetBossNumber(MaxEncounter::Number);
                    LoadDoorData(k_DoorData);

                    for (uint32 i = Blocks::FirstStair; i <= Blocks::SecondStair; i++)
                        m_WindMazeBlockGuids.push_back(MAKE_NEW_GUID(sObjectMgr->GenerateLowGuid(HIGHGUID_AREATRIGGER), 6452, HIGHGUID_AREATRIGGER));
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
                        p_Creature->DisableEvadeMode();
                        p_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_IMMUNE_TO_PC);
                        p_Creature->CastSpell(p_Creature, uint32(RandomSpells::ENERGIZE_VISUAL_1));
                        break;
                    case MobEntries::SKYREACH_RAVEN_WHISPERER:
                        m_SkyreachRavenWhispererGuid = p_Creature->GetGUID();
                        break;
                    case MobEntries::YoungKaliri:
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        p_Creature->setFaction(16);
                        p_Creature->DisableEvadeMode();
                        p_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                        break;
                    case MobEntries::Kaliri:
                    case MobEntries::Kaliri2:
                        // Setting fly.
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        break;
                    case MobEntries::PILE_OF_ASHES:
                        m_PileOfAshesGuid.insert(p_Creature->GetGUID());
                        break;
                    case MobEntries::SOLAR_FLARE:
                        m_SolarFlaresGuid.insert(p_Creature->GetGUID());
                        break;
                    case MobEntries::GrandDefenseConstruct:
                    case MobEntries::RadiantSupernova:
                    case MobEntries::AirFamiliar:
                        p_Creature->setFaction(16);
                        break;
                    case MobEntries::ArakkoaPincerBirdsController:
                        p_Creature->setFaction(16);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        break;
                    case MobEntries::SolarZealot:
                        p_Creature->setFaction(16);
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        p_Creature->DisableEvadeMode();
                        break;
                    case MobEntries::DreadRavenHatchling:
                        p_Creature->setFaction(16);
                        break;
                    case MobEntries::SunConstructEnergizer:
                        m_SolarConstructorEnergizerGuid = p_Creature->GetGUID();
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        break;
                    case MobEntries::InteriorFocus:
                        m_InteriorFocusGuid = p_Creature->GetGUID();
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        break;
                    case MobEntries::AraokkoaMagnifyingConstructA:
                        m_MagnifyingGlassFocusGuids.push_back(p_Creature->GetGUID());
                        p_Creature->setFaction(16);
                        p_Creature->SetDisableGravity(true);
                        p_Creature->SetCanFly(true);
                        p_Creature->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_ALWAYS_STAND | UNIT_BYTE1_FLAG_HOVER);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        p_Creature->DisableEvadeMode();
                        break;
                    case MobEntries::ArakkoaMagnifyingGlassFocus:
                        p_Creature->SetDisplayId(17519);
                        p_Creature->SetReactState(REACT_PASSIVE);
                        p_Creature->SetCanFly(false);
                        p_Creature->DisableEvadeMode();
                        p_Creature->SetDisableGravity(false);
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
                    case GameObjectEntries::DOOR_HIGH_SAVE_VIRYX_ENTRANCE:
                        AddDoor(p_Gameobject, true);
                        break;
                    case GameObjectEntries::CACHE_OF_ARAKKOAN_TREASURES:
                        p_Gameobject->SetPhaseMask(0, true);
                        m_CacheOfArakoanTreasuresGuid = p_Gameobject->GetGUID();
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
                        case DONE:
                            if (GameObject* l_Gob = sObjectAccessor->FindGameObject(m_CacheOfArakoanTreasuresGuid))
                                l_Gob->SetPhaseMask(1, true);
                            break;
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

                        if (Creature* l_InteriorFocus = sObjectAccessor->FindCreature(m_InteriorFocusGuid))
                            l_InteriorFocus->CastStop();
                        if (Creature* l_SolarConstructorEnergizer = sObjectAccessor->FindCreature(m_SolarConstructorEnergizerGuid))
                            l_SolarConstructorEnergizer->CastStop();
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

                        if (Creature* l_InteriorFocus = sObjectAccessor->FindCreature(m_InteriorFocusGuid))
                            l_InteriorFocus->CastSpell(l_InteriorFocus, uint32(RandomSpells::ENERGIZE_VISUAL_3));

                        if (Creature* l_SolarConstructorEnergizer = sObjectAccessor->FindCreature(m_SolarConstructorEnergizerGuid))
                            l_SolarConstructorEnergizer->CastSpell(l_SolarConstructorEnergizer, uint32(RandomSpells::ENERGIZE_VISUAL_3));
                        break;
                    case Data::AraknathSolarConstructorActivation:
                        if (p_Data)
                        {
                            if (m_SolarConstructorsGuid.empty())
                                break;

                            auto l_RandUnit = m_SolarConstructorsGuid.begin();
                            std::advance(l_RandUnit, urand(0, m_SolarConstructorsGuid.size() - 1));
                            m_SelectedSolarConstructorGuid = *l_RandUnit;

                            if (Creature* l_Constructor = sObjectAccessor->FindCreature(m_SelectedSolarConstructorGuid))
                            {
                                l_Constructor->RemoveAura(RandomSpells::SUBMERGED);
                                l_Constructor->CastSpell(l_Constructor, uint32(RandomSpells::ENERGIZE_HEAL));
                            }

                            // Visual part.
                            if (Creature* l_InteriorFocus = sObjectAccessor->FindCreature(m_InteriorFocusGuid))
                                l_InteriorFocus->CastSpell(l_InteriorFocus, uint32(RandomSpells::ENERGIZE_VISUAL_3));

                            if (Creature* l_SolarConstructorEnergizer = sObjectAccessor->FindCreature(m_SolarConstructorEnergizerGuid))
                                l_SolarConstructorEnergizer->CastSpell(l_SolarConstructorEnergizer, uint32(RandomSpells::ENERGIZE_VISUAL_2));
                        }
                        else
                        {
                            if (Creature* l_Constructor = sObjectAccessor->FindCreature(m_SelectedSolarConstructorGuid))
                            {
                                l_Constructor->AddAura(RandomSpells::SUBMERGED, l_Constructor);
                                l_Constructor->CastStop();
                            }

                            // Visual part.
                            if (Creature* l_InteriorFocus = sObjectAccessor->FindCreature(m_InteriorFocusGuid))
                                l_InteriorFocus->CastStop();

                            if (Creature* l_SolarConstructorEnergizer = sObjectAccessor->FindCreature(m_SolarConstructorEnergizerGuid))
                                l_SolarConstructorEnergizer->CastStop();

                            m_SelectedSolarConstructorGuid = 0;
                        }
                    case Data::StartingLensFlare:
                    {
                        if (m_MagnifyingGlassFocusGuids.empty())
                            break;

                        auto l_Itr = m_MagnifyingGlassFocusGuids.begin();
                        std::advance(l_Itr, m_MagnifyingGlassFocusGuids.size() - 1);

                        if (Creature* l_Creature = sObjectAccessor->FindCreature(*l_Itr))
                            l_Creature->CastSpell(l_Creature, uint32(RandomSpells::LensFlare), true);

                    } break;
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

                void SetData64(uint32 p_Type, uint64 p_Data)
                {
                    enum class Spells : uint32
                    {
                        SOLAR_FLARE = 160964,
                        DORMANT = 160641,
                        SUMMON_SOLAR_FLARE = 153810,
                    };

                    switch (p_Type)
                    {
                    case Data::SolarFlareDying:
                        if (m_SolarFlaresGuid.find(p_Data) == m_SolarFlaresGuid.end())
                            break;
                        m_SolarFlaresGuid.erase(p_Data);

                        if (Unit* l_SolarFlareDying = sObjectAccessor->FindCreature(p_Data))
                        {
                            auto l_Piles = InstanceSkyreach::SelectNearestCreatureListWithEntry(l_SolarFlareDying, MobEntries::PILE_OF_ASHES, 5.0f);
                            for (auto l_Pile : l_Piles)
                            {
                                if (m_PileOfAshesGuid.find(l_Pile->GetGUID()) == m_PileOfAshesGuid.end())
                                    continue;
                                m_PileOfAshesGuid.erase(l_Pile->GetGUID());

                                Position l_Pos;
                                l_Pile->GetPosition(&l_Pos);
                                if (l_Pile->ToCreature())
                                {
                                    l_Pile->CastSpell(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, uint32(Spells::SUMMON_SOLAR_FLARE), true);
                                    TempSummon* l_Summon = l_Pile->SummonCreature(MobEntries::SOLAR_FLARE, l_Pos);
                                    m_SolarFlaresGuid.insert(l_Summon->GetGUID());
                                    l_Pile->ToCreature()->DespawnOrUnsummon(500);
                                }
                            }

                            // We summon a new pile of ashes.
                            l_SolarFlareDying->CastSpell(l_SolarFlareDying, uint32(Spells::DORMANT), true);

                            Position l_Pos;
                            l_SolarFlareDying->GetPosition(&l_Pos);
                            TempSummon* l_Sum = l_SolarFlareDying->SummonCreature(MobEntries::PILE_OF_ASHES, l_Pos);
                            l_Sum->setFaction(16);
                            l_Sum->SetReactState(REACT_PASSIVE);
                            l_Sum->CastSpell(l_Sum, uint32(Spells::DORMANT), true);
                            l_SolarFlareDying->Kill(l_SolarFlareDying);

                            if (l_SolarFlareDying->ToCreature())
                                l_SolarFlareDying->ToCreature()->DespawnOrUnsummon(500);
                        }
                        break;
                    }
                }

                void OnPlayerEnter(Player* p_Player)
                {
                    if (!p_Player->IsInWorld())
                        return;

                    m_PlayerGuidToBlockId[p_Player->GetGUID()] = 0;
                    m_CanUpdate = true;
                }

                void Update(uint32 p_Diff)
                {
                    if (!m_CanUpdate)
                        return;

                    m_BeginningTime += p_Diff;

                    // We check here if a player is in the WindMaze zone.
                    Map::PlayerList const &l_PlayerList = instance->GetPlayers();
                    if (!l_PlayerList.isEmpty())
                    {
                        for (Map::PlayerList::const_iterator i = l_PlayerList.begin(); i != l_PlayerList.end(); ++i)
                        {
                            Player* l_Plr = i->getSource();
                            if (!l_Plr)
                                continue;

                            // Is he in WindMaze zone ?
                            if (IsPointInBlock(Blocks::ConvexHull, *l_Plr))
                            {
                                bool l_IsInBlock = false;
                                uint32 i = Blocks::FirstStair;
                                for (; i <= Blocks::SecondStair; i++)
                                {
                                    if (IsPointInBlock(i, *l_Plr))
                                    {
                                        l_IsInBlock = true;
                                        break;
                                    }
                                }

                                // If the player is in one of the blocks and if it doesn't have the Wind aura, add it.
                                if (l_IsInBlock)
                                {
                                    float l_Magnitude = 1;
                                    Position l_ForceDir = CalculateForceVectorFromBlockId(i, l_Magnitude);
                                    if (!l_Plr->HasAura(RandomSpells::Wind))
                                    {
                                        l_Plr->AddAura(RandomSpells::Wind, l_Plr);
                                        // Apply force.
                                        l_Plr->SendApplyMovementForce(m_WindMazeBlockGuids[i], true, l_ForceDir, l_Magnitude);
                                    }
                                    else if (i != m_PlayerGuidToBlockId[l_Plr->GetGUID()])
                                    {
                                        // Remove old force.
                                        // Add new force.
                                        if (l_Plr->HasMovementForce(m_WindMazeBlockGuids[m_PlayerGuidToBlockId[l_Plr->GetGUID()]]))
                                            l_Plr->SendApplyMovementForce(m_WindMazeBlockGuids[m_PlayerGuidToBlockId[l_Plr->GetGUID()]], false, l_ForceDir, l_Magnitude);
                                        l_Plr->SendApplyMovementForce(m_WindMazeBlockGuids[i], true, l_ForceDir, l_Magnitude);
                                    }

                                    m_PlayerGuidToBlockId[l_Plr->GetGUID()] = i;
                                }
                                // Otherwise remove it if it has the Wind aura.
                                else if (l_Plr->HasAura(RandomSpells::Wind))
                                {
                                    if (l_Plr->HasMovementForce(m_WindMazeBlockGuids[m_PlayerGuidToBlockId[l_Plr->GetGUID()]]))
                                        l_Plr->SendApplyMovementForce(m_WindMazeBlockGuids[m_PlayerGuidToBlockId[l_Plr->GetGUID()]], false, Position(), 1.0f);
                                    l_Plr->RemoveAura(RandomSpells::Wind);
                                }
                            }
                            // If player is out of the WindMaze zone and has the aura, remove it.
                            else if (l_Plr->HasAura(RandomSpells::Wind))
                            {
                                if (l_Plr->HasMovementForce(m_WindMazeBlockGuids[m_PlayerGuidToBlockId[l_Plr->GetGUID()]]))
                                    l_Plr->SendApplyMovementForce(m_WindMazeBlockGuids[m_PlayerGuidToBlockId[l_Plr->GetGUID()]], false, Position(), 1.0f);
                                l_Plr->RemoveAura(RandomSpells::Wind);
                            }
                        }
                    }

                    if (m_SelectedSolarConstructorGuid)
                    {
                        if (instance)
                        {
                            Map::PlayerList const &PlayerList = instance->GetPlayers();
                            if (PlayerList.isEmpty())
                                return;

                            Unit* l_Araknath = instance->GetCreature(m_AraknathGuid);
                            Unit* l_SelectedSolarConstructor = instance->GetCreature(m_SelectedSolarConstructorGuid);

                            if (!l_Araknath || !l_SelectedSolarConstructor)
                                return;

                            Player* l_ClosestPlayerInBeam = nullptr;
                            float l_ClosestDistance = std::numeric_limits<float>::max();
                            for (Map::PlayerList::const_iterator i = PlayerList.begin(); i != PlayerList.end(); ++i)
                            {
                                if (DistanceFromLine(*l_Araknath, *l_SelectedSolarConstructor, *i->getSource()) < 0.5f // If is in beam.
                                    && dotProductXY(*l_Araknath - *l_SelectedSolarConstructor, *l_Araknath - *i->getSource()) > 0 // If player is between solar constructor and araknath.
                                    && l_ClosestDistance > l_SelectedSolarConstructor->GetDistance(i->getSource()))
                                {
                                    l_ClosestDistance = l_SelectedSolarConstructor->GetDistance(i->getSource());
                                    l_ClosestPlayerInBeam = i->getSource();
                                }
                            }

                            auto l_CurrentSpell = l_SelectedSolarConstructor->GetCurrentSpell(CURRENT_CHANNELED_SPELL);
                            if (l_ClosestPlayerInBeam
                                && l_CurrentSpell
                                && l_CurrentSpell->GetUnitTarget()
                                && l_CurrentSpell->GetUnitTarget()->ToPlayer() != l_ClosestPlayerInBeam)
                                l_SelectedSolarConstructor->CastSpell(l_ClosestPlayerInBeam, uint32(RandomSpells::ENERGIZE_DMG), false, nullptr, NULLAURA_EFFECT, m_AraknathGuid);
                            else if (!l_ClosestPlayerInBeam
                                && l_CurrentSpell
                                && l_CurrentSpell->GetUnitTarget()
                                && l_CurrentSpell->GetUnitTarget()->ToPlayer())
                            l_SelectedSolarConstructor->CastSpell(l_SelectedSolarConstructor, uint32(RandomSpells::ENERGIZE_HEAL));
                        }
                    }
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