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
                m_KargathBladefistGuid      = 0;
                m_JhornTheMadGuid           = 0;
                m_ThoktarIronskullGuid      = 0;
                m_ArenaElevatorGuid         = 0;
                m_ArenaElevatorActivated    = false;
                m_CollisionWallGuid         = 0;
                m_VulgorGuid                = 0;
                m_GateArenaInnerGuid        = 0;
                m_TheButcherGuid            = 0;
                m_BrackensporeGuid          = 0;
            }

            /// The Coliseum
            uint64 m_KargathBladefistGuid;
            uint64 m_JhornTheMadGuid;
            uint64 m_ThoktarIronskullGuid;
            uint64 m_ArenaElevatorGuid;
            bool   m_ArenaElevatorActivated;
            uint64 m_CollisionWallGuid;
            uint64 m_VulgorGuid;
            uint64 m_GateArenaInnerGuid;

            /// The Underbelly
            uint64 m_TheButcherGuid;

            /// Gorian Strand
            uint64 m_BrackensporeGuid;

            void Initialize() override
            {
                SetBossNumber(eHighmaulDatas::MaxHighmaulBosses);
                LoadDoorData(g_DoorData);
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
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
                    case eHighmaulCreatures::TheButcher:
                        m_TheButcherGuid = p_Creature->GetGUID();
                        break;
                    case eHighmaulCreatures::Brackenspore:
                        m_BrackensporeGuid = p_Creature->GetGUID();
                        break;
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
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 p_BossID, EncounterState p_State) override
            {
                if (!InstanceScript::SetBossState(p_BossID, p_State))
                    return false;

                return true;
            }

            void SetData(uint32 p_Type, uint32 p_Data) override
            {
                switch (p_Type)
                {
                    case eHighmaulDatas::ElevatorActivated:
                        m_ArenaElevatorActivated = p_Data;
                        break;
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
                    default:
                        return 0;
                }
            }

            uint64 GetData64(uint32 p_Type) override
            {
                switch (p_Type)
                {
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
                    case eHighmaulCreatures::TheButcher:
                        return m_TheButcherGuid;
                    case eHighmaulCreatures::Brackenspore:
                        return m_BrackensporeGuid;
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 p_BossID, Player const* p_Player = nullptr) const override
            {
                if (!InstanceScript::CheckRequiredBosses(p_BossID, p_Player))
                    return false;

                /*switch (p_BossID)
                {
                    default:
                        break;
                }*/

                return true;
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