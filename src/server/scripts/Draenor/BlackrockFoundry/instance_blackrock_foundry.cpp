////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

DoorData const g_DoorData[] =
{
    { eFoundryGameObjects::GruulSpikeDoor,              eFoundryDatas::DataGruul,               DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BKFoundrySpikeTrapGate,      eFoundryDatas::DataOregorger,           DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::FurnacePortcullis,           eFoundryDatas::DataOregorger,           DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::BlastFurnaceEncounterDoor,   eFoundryDatas::DataBlastFurnace,        DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::HansgarAndFranzokEntrance,   eFoundryDatas::DataHansgarAndFranzok,   DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE },
    { eFoundryGameObjects::HansgarAndFranzokExit,       eFoundryDatas::DataHansgarAndFranzok,   DoorType::DOOR_TYPE_PASSAGE,    BoundaryType::BOUNDARY_NONE },
    { 0,                                                0,                                      DoorType::DOOR_TYPE_ROOM,       BoundaryType::BOUNDARY_NONE } ///< End
};

class instance_blackrock_foundry : public InstanceMapScript
{
    public:
        instance_blackrock_foundry() : InstanceMapScript("instance_blackrock_foundry", 1205) { }

        struct instance_blackrock_foundryMapScript : public InstanceScript
        {
            instance_blackrock_foundryMapScript(Map* p_Map) : InstanceScript(p_Map)
            {
                m_GruulGuid                 = 0;
                m_PristineTrueIronOres      = 0;

                m_OregorgerGuid             = 0;
                m_VolatileOreGuid           = 0;
                m_VolatileOreGrinded        = false;

                m_HeartOfTheMountain        = 0;
                m_ForemanFeldspar           = 0;
                m_CosmeticBlackhand         = 0;
                m_FurnaceGate               = 0;
                m_PrimalElementalistTime    = 0;
                m_YaWeveGotTimeAchiev       = false;

                m_HansgarGuid               = 0;
                m_FranzokGuid               = 0;
            }

            /// Slagworks
            uint64 m_GruulGuid;
            uint8 m_PristineTrueIronOres;

            uint64 m_OregorgerGuid;
            uint64 m_VolatileOreGuid;
            bool m_VolatileOreGrinded;

            /// Blast Furnace
            uint64 m_HeartOfTheMountain;
            uint64 m_ForemanFeldspar;
            uint64 m_CosmeticBlackhand;
            uint64 m_FurnaceGate;
            uint32 m_PrimalElementalistTime;
            bool m_YaWeveGotTimeAchiev;

            /// The Black Forge
            /// Slagmill Press
            uint64 m_HansgarGuid;
            uint64 m_FranzokGuid;

            void Initialize() override
            {
                SetBossNumber(eFoundryDatas::MaxBossData);

                LoadDoorData(g_DoorData);

                instance->SetObjectVisibility(300.0f);
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eFoundryCreatures::BossGruul:
                        m_GruulGuid = p_Creature->GetGUID();
                        break;
                    case eFoundryCreatures::BossOregorger:
                        m_OregorgerGuid = p_Creature->GetGUID();
                        break;
                    case eFoundryCreatures::HeartOfTheMountain:
                        m_HeartOfTheMountain = p_Creature->GetGUID();
                        break;
                    case eFoundryCreatures::ForemanFeldspar:
                        m_ForemanFeldspar = p_Creature->GetGUID();
                        break;
                    case eFoundryCreatures::BlackhandCosmetic:
                        m_CosmeticBlackhand = p_Creature->GetGUID();
                        break;
                    case eFoundryCreatures::BossHansgar:
                        m_HansgarGuid = p_Creature->GetGUID();
                        break;
                    case eFoundryCreatures::BossFranzok:
                        m_FranzokGuid = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eFoundryGameObjects::GruulSpikeDoor:
                    case eFoundryGameObjects::BKFoundrySpikeTrapGate:
                    case eFoundryGameObjects::FurnacePortcullis:
                    case eFoundryGameObjects::BlastFurnaceEncounterDoor:
                    case eFoundryGameObjects::HansgarAndFranzokEntrance:
                    case eFoundryGameObjects::HansgarAndFranzokExit:
                        AddDoor(p_GameObject, true);
                        break;
                    case eFoundryGameObjects::VolatileBlackrockOre:
                        m_VolatileOreGuid = p_GameObject->GetGUID();
                        break;
                    case eFoundryGameObjects::CrucibleLeft:
                    case eFoundryGameObjects::CrucibleRight:
                        p_GameObject->SetAIAnimKitId(eFoundryVisuals::CrucibleVisuals);
                        break;
                    case eFoundryGameObjects::FurnaceGate:
                        m_FurnaceGate = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectRemove(GameObject* p_GameObject) override
            {
                switch (p_GameObject->GetEntry())
                {
                    case eFoundryGameObjects::GruulSpikeDoor:
                    case eFoundryGameObjects::BKFoundrySpikeTrapGate:
                    case eFoundryGameObjects::FurnacePortcullis:
                    case eFoundryGameObjects::BlastFurnaceEncounterDoor:
                    case eFoundryGameObjects::HansgarAndFranzokEntrance:
                    case eFoundryGameObjects::HansgarAndFranzokExit:
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
                    case eFoundryDatas::DataGruul:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_PristineTrueIronOres >= eFoundryDatas::MaxPristineTrueIronOres)
                                    DoCompleteAchievement(eFoundryAchievements::TheIronPrince);

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_PristineTrueIronOres = 0;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataOregorger:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_VolatileOreGrinded)
                                    DoCompleteAchievement(eFoundryAchievements::HeShootsHeOres);

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_VolatileOreGrinded = false;
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case eFoundryDatas::DataBlastFurnace:
                    {
                        switch (p_State)
                        {
                            case EncounterState::DONE:
                            {
                                if (m_YaWeveGotTimeAchiev)
                                    DoCompleteAchievement(eFoundryAchievements::YaWeveGotTime);

                                break;
                            }
                            case EncounterState::NOT_STARTED:
                            {
                                m_PrimalElementalistTime = 0;
                                m_YaWeveGotTimeAchiev = false;
                                break;
                            }
                        }

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
                    case eFoundryDatas::PristineTrueIronOres:
                    {
                        if (instance->IsLFR())
                            break;

                        ++m_PristineTrueIronOres;
                        break;
                    }
                    case eFoundryDatas::VolatileOreGrinded:
                    {
                        if (instance->IsLFR())
                            break;

                        m_VolatileOreGrinded = p_Data != 0;
                        break;
                    }
                    case eFoundryDatas::PrimalElementalistTime:
                    {
                        if (instance->IsLFR())
                            break;

                        if (!m_PrimalElementalistTime)
                            m_PrimalElementalistTime = p_Data;
                        else
                        {
                            /// Defeating all four Primal Elementalists within 10 seconds of each other
                            if (p_Data > (m_PrimalElementalistTime + 10))
                                m_YaWeveGotTimeAchiev = false;
                            else
                                m_YaWeveGotTimeAchiev = true;
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
                    case eFoundryCreatures::BossGruul:
                        return m_GruulGuid;
                    case eFoundryCreatures::BossOregorger:
                        return m_OregorgerGuid;
                    case eFoundryCreatures::HeartOfTheMountain:
                        return m_HeartOfTheMountain;
                    case eFoundryCreatures::ForemanFeldspar:
                        return m_ForemanFeldspar;
                    case eFoundryCreatures::BlackhandCosmetic:
                        return m_CosmeticBlackhand;
                    case eFoundryGameObjects::VolatileBlackrockOre:
                        return m_VolatileOreGuid;
                    case eFoundryGameObjects::FurnaceGate:
                        return m_FurnaceGate;
                    case eFoundryCreatures::BossHansgar:
                        return m_HansgarGuid;
                    case eFoundryCreatures::BossFranzok:
                        return m_FranzokGuid;
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 p_BossID, Player const* p_Player = nullptr) const override
            {
                if (!InstanceScript::CheckRequiredBosses(p_BossID, p_Player))
                    return false;

                return true;
            }

            void OnPlayerEnter(Player* p_Player) override
            {
                InstanceScript::OnPlayerEnter(p_Player);
            }

            void OnPlayerExit(Player* p_Player) override
            {
                InstanceScript::OnPlayerExit(p_Player);
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
                        l_Player->PlayStandaloneScene(p_ScenePackageID, 16, p_Pos);
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
            return new instance_blackrock_foundryMapScript(p_Map);
        }
};

void AddSC_instance_blackrock_foundry()
{
    new instance_blackrock_foundry();
}