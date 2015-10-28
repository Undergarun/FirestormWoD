////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

DoorData const g_DoorData[] =
{
    { eHighmaulGameobjects::GateArenaExit,      eHighmaulDatas::BossKargathBladefist,   DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::EarthenPillar,      eHighmaulDatas::BossTheButcher,         DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::FungalGiantDoor,    eHighmaulDatas::BossTheButcher,         DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::WindDoor,           eHighmaulDatas::BossTheButcher,         DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::WindDoor,           eHighmaulDatas::BossBrackenspore,       DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::Earthwall1,         eHighmaulDatas::BossTectus,             DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::Earthwall2,         eHighmaulDatas::BossTectus,             DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::Earthwall3,         eHighmaulDatas::BossTectus,             DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::Earthwall4,         eHighmaulDatas::BossTectus,             DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::TwinOgronEntrance,  eHighmaulDatas::BossTwinOgron,          DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::TwinOgronExit,      eHighmaulDatas::BossTwinOgron,          DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::FelBreakerEntrance, eHighmaulDatas::BossKoragh,             DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::FelBreakerExitDoor, eHighmaulDatas::BossKoragh,             DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::ThroneRoomDoor,     eHighmaulDatas::BossImperatorMargok,    DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::StairBlockingDoor,  eHighmaulDatas::BossImperatorMargok,    DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::StairBlockingDoor,  eHighmaulDatas::BossImperatorMargok,    DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eHighmaulGameobjects::StairBlockingDoor,  eHighmaulDatas::BossImperatorMargok,    DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { 0,                                        0,                                      DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE } ///< End
};

class instance_highmaul : public InstanceMapScript
{
    public:
        instance_highmaul() : InstanceMapScript("instance_highmaul", 1228) { }

        struct instance_highmaul_InstanceMapScript : public InstanceScript
        {
            instance_highmaul_InstanceMapScript(Map* p_Map) : InstanceScript(p_Map)
            {
                m_Initialized               = false;
                m_ForTests                  = false;

                m_ArenaMasterGuid           = 0;

                m_KargathBladefistGuid      = 0;
                m_JhornTheMadGuid           = 0;
                m_ThoktarIronskullGuid      = 0;
                m_ArenaElevatorGuid         = 0;
                m_ArenaElevatorActivated    = false;
                m_CollisionWallGuid         = 0;
                m_VulgorGuid                = 0;
                m_GateArenaInnerGuid        = 0;
                m_CrowdAreatriggerGuid      = 0;
                m_MargokCosmeticGuid        = 0;

                for (uint8 l_I = eHighmaulDatas::RaidGrate001; l_I < eHighmaulDatas::MaxRaidGrates; ++l_I)
                    m_RaidGrateGuids[l_I]   = 0;

                m_IronBombersCount          = 0;
                m_DrunkenBileslingerCount   = 0;

                m_TheButcherGuid            = 0;

                m_BrackensporeGuid          = 0;

                m_TectusGuid                = 0;
                m_GuardiansGuids.resize(eHighmaulDatas::MaxTectusGuardians);

                m_PhemosGuid                = 0;
                m_PolGuid                   = 0;

                m_KoraghGuid                = 0;

                m_PlayerPhases.clear();
            }

            bool m_Initialized;
            bool m_ForTests;

            uint64 m_ArenaMasterGuid;

            /// The Coliseum
            uint64 m_KargathBladefistGuid;
            uint64 m_JhornTheMadGuid;
            uint64 m_ThoktarIronskullGuid;
            uint64 m_ArenaElevatorGuid;
            bool   m_ArenaElevatorActivated;
            uint64 m_CollisionWallGuid;
            uint64 m_VulgorGuid;
            uint64 m_GateArenaInnerGuid;
            uint64 m_CrowdAreatriggerGuid;
            uint64 m_MargokCosmeticGuid;
            uint64 m_RaidGrateGuids[4];
            uint8  m_IronBombersCount;
            uint8  m_DrunkenBileslingerCount;

            /// The Underbelly
            uint64 m_TheButcherGuid;

            /// Gorian Strand
            uint64 m_BrackensporeGuid;

            /// The Market
            uint64 m_TectusGuid;
            std::vector<uint64> m_GuardiansGuids;

            /// The Gorthenon
            uint64 m_PhemosGuid;
            uint64 m_PolGuid;

            /// Chamber of Nullification
            uint64 m_KoraghGuid;

            /// Throne of the Imperator
            uint64 m_ImperatorMargokGuid;
            uint64 m_HighCouncilorMalgris;

            /// Phasing
            std::map<uint32, uint32> m_PlayerPhases;

            void Initialize() override
            {
                SetBossNumber(eHighmaulDatas::MaxHighmaulBosses);
                LoadDoorData(g_DoorData);

                instance->SetObjectVisibility(500.0f);
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eHighmaulCreatures::GhargArenaMaster:
                        m_ArenaMasterGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::KargathBladefist:
                        m_KargathBladefistGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::JhornTheMad:
                        m_JhornTheMadGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::ThoktarIronskull:
                        m_ThoktarIronskullGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Vulgor:
                        m_VulgorGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::CrowdAreatrigger:
                        m_CrowdAreatriggerGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::MargokCosmetic:
                        m_MargokCosmeticGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::IronBomberSpawner:
                        p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                        p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                        break;
                    case eHighmaulCreatures::IronBomber:
                        if (!m_IronBombersCount)
                            SendUpdateWorldState(eHighmaulWorldStates::IronBomberEnable, 1);
                        ++m_IronBombersCount;
                        SendUpdateWorldState(eHighmaulWorldStates::IronBomberRemaining, m_IronBombersCount);
                        break;
                    case eHighmaulCreatures::DrunkenBileslinger:
                        if (!m_DrunkenBileslingerCount)
                            SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerEnable, 1);
                        ++m_DrunkenBileslingerCount;
                        SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerRemaining, m_DrunkenBileslingerCount);
                        break;
                    case eHighmaulCreatures::TheButcher:
                        m_TheButcherGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Brackenspore:
                        m_BrackensporeGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Tectus:
                        m_TectusGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Rokka:
                    case eHighmaulCreatures::Oro:
                    case eHighmaulCreatures::Lokk:
                        m_GuardiansGuids[p_Creature->GetEntry() - eHighmaulCreatures::Rokka] = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Phemos:
                        m_PhemosGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Pol:
                        m_PolGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Koragh:
                        m_KoraghGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::IronGrunt:
                    case eHighmaulCreatures::BlackrockGrunt:
                    case eHighmaulCreatures::LowBatchDeadPale:
                    case eHighmaulCreatures::NightTwistedPaleVis:
                    case eHighmaulCreatures::CosmeticGorianWarr:
                    case eHighmaulCreatures::GorianCivilian:
                    case eHighmaulCreatures::RuneOfNullification:
                        p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                        p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                        p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    case eHighmaulCreatures::ImperatorMargok:
                        m_ImperatorMargokGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::HighCouncilorMalgris:
                        m_HighCouncilorMalgris = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnCreatureRemove(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eHighmaulCreatures::IronBomber:
                    {
                        if (!m_IronBombersCount)
                            break;

                        --m_IronBombersCount;
                        SendUpdateWorldState(eHighmaulWorldStates::IronBomberRemaining, m_IronBombersCount);

                        if (!m_IronBombersCount)
                        {
                            SendUpdateWorldState(eHighmaulWorldStates::IronBomberEnable, 0);
                            break;
                        }

                        break;
                    }
                    case eHighmaulCreatures::DrunkenBileslinger:
                    {
                        if (!m_DrunkenBileslingerCount)
                            break;

                        --m_DrunkenBileslingerCount;
                        SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerRemaining, m_DrunkenBileslingerCount);

                        if (!m_DrunkenBileslingerCount)
                        {
                            SendUpdateWorldState(eHighmaulWorldStates::DrunkenBileslingerEnable, 0);
                            break;
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eHighmaulGameobjects::GateArenaExit:
                    case eHighmaulGameobjects::FungalGiantDoor:
                    case eHighmaulGameobjects::EarthenPillar:
                    case eHighmaulGameobjects::WindDoor:
                    case eHighmaulGameobjects::Earthwall1:
                    case eHighmaulGameobjects::Earthwall2:
                    case eHighmaulGameobjects::Earthwall3:
                    case eHighmaulGameobjects::Earthwall4:
                    case eHighmaulGameobjects::TwinOgronEntrance:
                    case eHighmaulGameobjects::TwinOgronExit:
                    case eHighmaulGameobjects::FelBreakerEntrance:
                    case eHighmaulGameobjects::FelBreakerExitDoor:
                    case eHighmaulGameobjects::ThroneRoomDoor:
                    case eHighmaulGameobjects::StairBlockingDoor:
                        AddDoor(p_GameObject, true);
                        break;
                    case eHighmaulGameobjects::ArenaElevator:
                        m_ArenaElevatorGuid = p_GameObject->GetGUID();
                        p_GameObject->SetTransportState(GOState::GO_STATE_TRANSPORT_ACTIVE);
                        break;
                    case eHighmaulGameobjects::CollisionWall:
                        m_CollisionWallGuid = p_GameObject->GetGUID();
                        p_GameObject->SetGoState(GOState::GO_STATE_ACTIVE);
                        break;
                    case eHighmaulGameobjects::GateArenaInner:
                        m_GateArenaInnerGuid = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate1:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate001] = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate2:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate002] = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate3:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate003] = p_GameObject->GetGUID();
                        break;
                    case eHighmaulGameobjects::RaidGrate4:
                        m_RaidGrateGuids[eHighmaulDatas::RaidGrate004] = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eHighmaulGameobjects::GateArenaExit:
                    case eHighmaulGameobjects::FungalGiantDoor:
                    case eHighmaulGameobjects::EarthenPillar:
                    case eHighmaulGameobjects::WindDoor:
                    case eHighmaulGameobjects::Earthwall1:
                    case eHighmaulGameobjects::Earthwall2:
                    case eHighmaulGameobjects::Earthwall3:
                    case eHighmaulGameobjects::Earthwall4:
                    case eHighmaulGameobjects::TwinOgronEntrance:
                    case eHighmaulGameobjects::TwinOgronExit:
                    case eHighmaulGameobjects::FelBreakerEntrance:
                    case eHighmaulGameobjects::FelBreakerExitDoor:
                    case eHighmaulGameobjects::ThroneRoomDoor:
                    case eHighmaulGameobjects::StairBlockingDoor:
                        AddDoor(p_GameObject, false);
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 p_BossID, EncounterState p_State) override
            {
                if (!InstanceScript::SetBossState(p_BossID, p_State))
                    return false;

                switch (p_BossID)
                {
                    case eHighmaulDatas::BossKargathBladefist:
                    {
                        if (p_State != EncounterState::DONE)
                            break;

                        SendUpdateWorldState(eHighmaulWorldStates::DisableCrowdSound, 1);
                        SendUpdateWorldState(eHighmaulWorldStates::UnknownHighmaulWorldState, 0);
                        SendUpdateWorldState(eHighmaulWorldStates::UnknownHighmaulWorldState2, 0);
                        PlaySceneForPlayers(g_PlayScenePos, 1338);
                        CastSpellToPlayers(instance, nullptr, eHighmaulSpells::ChogallNight, true);
                        break;
                    }
                    default:
                        break;
                }

                return true;
            }

            void SetData(uint32 p_Type, uint32 p_Data) override
            {
                switch (p_Type)
                {
                    case eHighmaulDatas::ElevatorActivated:
                    {
                        m_ArenaElevatorActivated = p_Data;
                        break;
                    }
                    case eHighmaulDatas::TestsActivated:
                    {
                        m_ForTests = p_Data != 0;

                        /// Open all doors for tests
                        if (m_ForTests)
                        {
                            for (uint8 l_I = 0; l_I < eHighmaulDatas::MaxHighmaulBosses; ++l_I)
                            {
                                BossInfo* l_BossInfos = GetBossInfo(l_I);
                                if (l_BossInfos != nullptr)
                                {
                                    for (uint32 l_Type = 0; l_Type < DoorType::MAX_DOOR_TYPES; ++l_Type)
                                    {
                                        for (DoorSet::iterator l_Iter = l_BossInfos->door[l_Type].begin(); l_Iter != l_BossInfos->door[l_Type].end(); ++l_Iter)
                                            (*l_Iter)->SetGoState(GOState::GO_STATE_ACTIVE);
                                    }
                                }
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type) override
            {
                switch (p_Type)
                {
                    case eHighmaulDatas::ElevatorActivated:
                        return m_ArenaElevatorActivated;
                    case eHighmaulDatas::TestsActivated:
                        return m_ForTests;
                    default:
                        return 0;
                }
            }

            uint64 GetData64(uint32 p_Type) override
            {
                switch (p_Type)
                {
                    case eHighmaulCreatures::GhargArenaMaster:
                        return m_ArenaMasterGuid;
                    case eHighmaulCreatures::KargathBladefist:
                        return m_KargathBladefistGuid;
                    case eHighmaulCreatures::JhornTheMad:
                        return m_JhornTheMadGuid;
                    case eHighmaulCreatures::ThoktarIronskull:
                        return m_ThoktarIronskullGuid;
                    case eHighmaulCreatures::Vulgor:
                        return m_VulgorGuid;
                    case eHighmaulGameobjects::ArenaElevator:
                        return m_ArenaElevatorGuid;
                    case eHighmaulGameobjects::CollisionWall:
                        return m_CollisionWallGuid;
                    case eHighmaulGameobjects::GateArenaInner:
                        return m_GateArenaInnerGuid;
                    case eHighmaulCreatures::CrowdAreatrigger:
                        return m_CrowdAreatriggerGuid;
                    case eHighmaulCreatures::MargokCosmetic:
                        return m_MargokCosmeticGuid;
                    case eHighmaulCreatures::TheButcher:
                        return m_TheButcherGuid;
                    case eHighmaulCreatures::Brackenspore:
                        return m_BrackensporeGuid;
                    case eHighmaulGameobjects::RaidGrate1:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate001];
                    case eHighmaulGameobjects::RaidGrate2:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate002];
                    case eHighmaulGameobjects::RaidGrate3:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate003];
                    case eHighmaulGameobjects::RaidGrate4:
                        return m_RaidGrateGuids[eHighmaulDatas::RaidGrate004];
                    case eHighmaulCreatures::Tectus:
                        return m_TectusGuid;
                    case eHighmaulCreatures::Rokka:
                    case eHighmaulCreatures::Oro:
                    case eHighmaulCreatures::Lokk:
                        return m_GuardiansGuids[p_Type - eHighmaulCreatures::Rokka];
                    case eHighmaulCreatures::Phemos:
                        return m_PhemosGuid;
                    case eHighmaulCreatures::Pol:
                        return m_PolGuid;
                    case eHighmaulCreatures::Koragh:
                        return m_KoraghGuid;
                    case eHighmaulCreatures::ImperatorMargok:
                        return m_ImperatorMargokGuid;
                    case eHighmaulCreatures::HighCouncilorMalgris:
                        return m_HighCouncilorMalgris;
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 p_BossID, Player const* p_Player = nullptr) const override
            {
                /// Bypass required bosses for PTR tests
                if (m_ForTests)
                    return true;

                if (!InstanceScript::CheckRequiredBosses(p_BossID, p_Player))
                    return false;

                /// Highmaul has 4 main encounters (Kargath Bladefist, Ko'ragh, Twin Ogron, Imperator Mar'gok).
                /// There are also three optional encounters - The Butcher, Brackenspore and Tectus.
                switch (p_BossID)
                {
                    case eHighmaulDatas::BossTwinOgron:
                        if (GetBossState(eHighmaulDatas::BossKargathBladefist) != EncounterState::DONE)
                            return false;
                        break;
                    case eHighmaulDatas::BossKoragh:
                    case eHighmaulDatas::BossImperatorMargok:
                        if (GetBossState(p_BossID - 1) != EncounterState::DONE)
                            return false;
                        break;
                    default:
                        break;
                }

                return true;
            }

            void FillInitialWorldStates(ByteBuffer& p_Buffer) override
            {
                p_Buffer << uint32(eHighmaulWorldStates::IronBomberEnable) << uint32(0);
                p_Buffer << uint32(eHighmaulWorldStates::IronBomberRemaining) << uint32(0);
                p_Buffer << uint32(eHighmaulWorldStates::DrunkenBileslingerEnable) << uint32(0);
                p_Buffer << uint32(eHighmaulWorldStates::DrunkenBileslingerRemaining) << uint32(0);
            }

            void OnPlayerEnter(Player* p_Player) override
            {
                InstanceScript::OnPlayerEnter(p_Player);

                /// We must save the player phases to prevent some bugs
                if (m_PlayerPhases.find(p_Player->GetGUIDLow()) == m_PlayerPhases.end())
                    m_PlayerPhases.insert(std::make_pair(p_Player->GetGUIDLow(), p_Player->GetPhaseMask()));

                if (GetBossState(eHighmaulDatas::BossKargathBladefist) == EncounterState::DONE)
                {
                    p_Player->SetPhaseMask(eHighmaulDatas::PhaseKargathDefeated, true);
                    p_Player->CastSpell(p_Player, eHighmaulSpells::ChogallNight, true);

                    /// We don't need to update the enter pos if player is summoned by his allies
                    if (!p_Player->IsSummoned())
                    {
                        uint64 l_Guid = p_Player->GetGUID();
                        AddTimedDelayedOperation(200, [this, l_Guid]() -> void
                        {
                            if (Player* l_Player = sObjectAccessor->FindPlayer(l_Guid))
                            {
                                if (GetBossState(eHighmaulDatas::BossKoragh) == EncounterState::DONE)
                                    l_Player->NearTeleportTo(eHighmaulLocs::FelBreakerRoom);
                                else if (GetBossState(eHighmaulDatas::BossTectus) == EncounterState::DONE)
                                    l_Player->NearTeleportTo(eHighmaulLocs::PalaceFrontGate);
                                else if (GetBossState(eHighmaulDatas::BossTheButcher) == EncounterState::DONE)
                                    l_Player->NearTeleportTo(eHighmaulLocs::BeachEntrance);
                                else
                                    l_Player->NearTeleportTo(eHighmaulLocs::KargathDefeated);
                            }
                        });
                    }
                }
                else
                {
                    p_Player->SetPhaseMask(eHighmaulDatas::PhaseNone, true);
                    p_Player->RemoveAura(eHighmaulSpells::PlayChogallScene);
                    p_Player->RemoveAura(eHighmaulSpells::ChogallNight);
                }

                /// Disable non available bosses for LFR
                if (!m_Initialized)
                {
                    m_Initialized = true;

                    uint32 l_DungeonID = p_Player->GetGroup() ? sLFGMgr->GetDungeon(p_Player->GetGroup()->GetGUID()) : 0;

                    if (!instance->IsLFR())
                        l_DungeonID = 0;

                    switch (l_DungeonID)
                    {
                        case eHighmaulDungeons::WalledCity:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eHighmaulDatas::BossTectus);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossTwinOgron);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossKoragh);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossImperatorMargok);

                            SetDisabledBosses(l_DisabledMask);
                            break;
                        }
                        case eHighmaulDungeons::ArcaneSanctum:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eHighmaulDatas::BossKargathBladefist);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossTheButcher);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossBrackenspore);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossImperatorMargok);

                            SetDisabledBosses(l_DisabledMask);
                            break;
                        }
                        case eHighmaulDungeons::ImperatorsFall:
                        {
                            uint32 l_DisabledMask = 0;

                            l_DisabledMask |= (1 << eHighmaulDatas::BossKargathBladefist);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossTheButcher);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossBrackenspore);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossTectus);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossTwinOgron);
                            l_DisabledMask |= (1 << eHighmaulDatas::BossKoragh);

                            SetDisabledBosses(l_DisabledMask);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void OnPlayerExit(Player* p_Player) override
            {
                InstanceScript::OnPlayerExit(p_Player);

                p_Player->RemoveAura(eHighmaulSpells::PlayChogallScene);
                p_Player->RemoveAura(eHighmaulSpells::ChogallNight);

                /// We must restore original phasing for each players
                if (m_PlayerPhases.find(p_Player->GetGUIDLow()) != m_PlayerPhases.end())
                {
                    p_Player->SetPhaseMask(m_PlayerPhases[p_Player->GetGUIDLow()], true);
                    m_PlayerPhases.erase(p_Player->GetGUIDLow());
                }
                else
                    p_Player->SetPhaseMask(eHighmaulDatas::PhaseNone, true);
            }

            void SendUpdateWorldState(uint32 p_Field, uint32 p_Value)
            {
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                        l_Player->SendUpdateWorldState(p_Field, p_Value);
                }
            }

            void PlaySceneForPlayers(Position const p_Pos, uint32 p_ScenePackageID)
            {
                Map::PlayerList const& l_Players = instance->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                    {
                        l_Player->PlayStandaloneScene(p_ScenePackageID, 16, p_Pos);
                        l_Player->SetPhaseMask(eHighmaulDatas::PhaseKargathDefeated, true);
                    }
                }
            }

            void Update(uint32 p_Diff) override
            {
                UpdateOperations(p_Diff);
                UpdateCombatResurrection(p_Diff);
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const override
        {
            return new instance_highmaul_InstanceMapScript(p_Map);
        }
};

void AddSC_instance_highmaul()
{
    new instance_highmaul();
}