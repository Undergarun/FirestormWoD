////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "VMapFactory.h"
#include "throne_of_thunder.h"

DoorData const doorData[] =
{
    { GOB_JIN_ROKH_ENTRANCE_DOOR,           DATA_JIN_ROKH_THE_BREAKER,  DOOR_TYPE_ROOM,     BOUNDARY_W      },
    { GOB_JIN_ROKH_EXIT_DOOR,               DATA_JIN_ROKH_THE_BREAKER,  DOOR_TYPE_PASSAGE,  BOUNDARY_S      },
    { GOB_HORRIDON_ENTRANCE_DOOR,           DATA_HORRIDON,              DOOR_TYPE_ROOM,     BOUNDARY_W      },
    { GOB_HORRIDON_EXIT_DOOR,               DATA_HORRIDON,              DOOR_TYPE_PASSAGE,  BOUNDARY_N      },
    { GOB_COUNCIL_ENTRANCE_DOOR_LEFT,       DATA_COUNCIL_OF_ELDERS,     DOOR_TYPE_ROOM,     BOUNDARY_W      },
    { GOB_COUNCIL_ENTRANCE_DOOR_RIGHT,      DATA_COUNCIL_OF_ELDERS,     DOOR_TYPE_ROOM,     BOUNDARY_W      },
    { GOB_COUNCIL_EXIT_DOOR,                DATA_COUNCIL_OF_ELDERS,     DOOR_TYPE_PASSAGE,  BOUNDARY_E      },
    { GOB_TORTOS_EXIT_DOOR,                 DATA_TORTOS,                DOOR_TYPE_PASSAGE,  BOUNDARY_E      },
    { GOB_MEGAERA_EXIT_DOOR,                DATA_MEGAERA,               DOOR_TYPE_PASSAGE,  BOUNDARY_S      },
    { GOB_JI_KUN_EXIT_DOOR,                 DATA_JI_KUN,                DOOR_TYPE_PASSAGE,  BOUNDARY_S      },
    { GOB_DURUMU_EXIT_DOOR,                 DATA_DURUMU_THE_FORGOTTEN,  DOOR_TYPE_PASSAGE,  BOUNDARY_S      },
    { GOB_PRIMORDIUS_ENTRANCE,              DATA_PRIMORDIUS,            DOOR_TYPE_ROOM,     BOUNDARY_E      },
    { GOB_PRIMORDIUS_EXIT,                  DATA_PRIMORDIUS,            DOOR_TYPE_PASSAGE,  BOUNDARY_W      },
    { GOB_DARK_ANIMUS_ENTRANCE,             DATA_DARK_ANIMUS,           DOOR_TYPE_ROOM,     BOUNDARY_S      },
    { GOB_DARK_ANIMUS_EXIT,                 DATA_DARK_ANIMUS,           DOOR_TYPE_PASSAGE,  BOUNDARY_N      },
    { GOB_IRON_QON_SEWER_DOOR,              DATA_IRON_QON,              DOOR_TYPE_PASSAGE,  BOUNDARY_N      },
    { GOB_IRON_QON_EXIT_DOOR,               DATA_IRON_QON,              DOOR_TYPE_PASSAGE,  BOUNDARY_E      },
    { GOB_TWIN_ENTRANCE,                    DATA_TWIN_CONSORTS,         DOOR_TYPE_ROOM,     BOUNDARY_SW     },
    { GOB_TWIN_EXIT,                        DATA_TWIN_CONSORTS,         DOOR_TYPE_PASSAGE,  BOUNDARY_SE     },
    { 0,                                    0,                          DOOR_TYPE_ROOM,     BOUNDARY_NONE   }  // END
};

Position const g_StatuesPos[4] =
{
    { 5946.0f, 6317.5f, 158.083f, 3.923960f }, // NW
    { 5840.0f, 6316.0f, 158.083f, 5.486100f }, // SW
    { 5943.5f, 6211.0f, 158.083f, 2.355830f }, // NE
    { 5839.0f, 6211.5f, 158.083f, 0.781267f }  // SE
};

class instance_throne_of_thunder : public InstanceMapScript
{
public:
    instance_throne_of_thunder() : InstanceMapScript("instance_throne_of_thunder", 1098) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_throne_of_thunder_InstanceMapScript(map);
    }

    struct instance_throne_of_thunder_InstanceMapScript : public InstanceScript
    {
        instance_throne_of_thunder_InstanceMapScript(Map* map) : InstanceScript(map) { }

        /// Jin'rokh - Boss #1
        uint64 jinRokhTheBreakerGuid;
        uint64 moguFountainsGuids[4];
        uint64 moguStatuesGuids[4];

        /// Horridon - Boss #2
        uint64 horridonGuid;
        uint64 horridonGateGuid;
        uint64 tribalDoorsGuid[4];
        uint64 warGodJalakGuid;

        /// Council of Elders - Boss #3
        uint64 kazraJinGuid;
        uint64 sudTheSandCrawlerGuid;
        uint64 forostKingMalakkGuid;
        uint64 highPriestressMarLiGuid;
        uint64 garaJalGuid;
        uint64 garaJalSoulGuid;

        /// Megaera - Boss #5
        uint8 ancientMoguBellActivate;
        uint64 megaeraGuid;
        uint64 flamingMegaeraHead;
        uint64 frozenMegaeraHead;
        uint64 venomousMegaeraHead;
        uint64 arcarneMegaeraHead;

        /// Ji-Kun - Boss #6
        uint64 jiKunGuid;

        /// Durumu the forgotten - Boss #7
        uint64 durumuGuid;
        uint64 durumuEntranceGateGuid;
        uint64 durumuExitGateGuid;

        /// Primordius - Boss #8
        uint64 firstMoguBloodVatGuid;
        uint64 secondMoguBloodVatGuid;
        uint64 thirdMoguBloodVatGuid;
        uint64 fourthMoguBloodVatGuid;
        uint64 fifthMoguBloodVatGuid;
        uint64 sixthMoguBloodVatGuid;
        uint64 seventhMoguBloodVatGuid;
        uint64 eighthMoguBloodVatGuid;
        uint64 ninthMoguBloodVatGuid;
        uint64 tenthMoguBloodVatGuid;

        /// Dark Animus - Boss #9
        uint64 m_DarkAnimusGuid;
        uint64 m_AnimaOrbGuid;

        /// Iron Qon - Boss #10
        uint64 m_IronQonGuid;
        uint64 m_RoShakGuid;
        uint64 m_QuetZalGuid;
        uint64 m_DamRenGuid;

        /// TwinConsorts - Boss #11
        uint64 m_LulinGuid;
        uint64 m_SuenGuid;

        bool m_ThunderTrapActivated;

        bool m_ThirdPartInit;

        /// Lei Shen - Boss #12
        uint64 m_LeiShenGuid;
        uint64 m_PillarControllerGuids[4];
        uint64 m_FloorGuids[4];
        uint64 m_ChargingStationGuid;
        uint64 m_LeiShenWindows[4];
        uint64 m_LeiShenWindFX[4];
        uint64 m_LeiShenPillars[4];

        /// Ra-den - Boss #13 - HM only
        uint64 m_RadenIrisGuid;

        void Initialize() override
        {
            SetBossNumber(DATA_MAX_BOSS_DATA);
            LoadDoorData(doorData);

            jinRokhTheBreakerGuid = 0;

            kazraJinGuid = 0;
            sudTheSandCrawlerGuid = 0;
            forostKingMalakkGuid = 0;
            highPriestressMarLiGuid = 0;
            garaJalSoulGuid = 0;
            garaJalGuid = 0;

            horridonGuid = 0;
            horridonGateGuid = 0;
            warGodJalakGuid = 0;

            ancientMoguBellActivate = 0;
            megaeraGuid = 0;
            flamingMegaeraHead = 0;
            frozenMegaeraHead = 0;
            venomousMegaeraHead = 0;
            arcarneMegaeraHead = 0;

            durumuGuid = 0;
            durumuEntranceGateGuid = 0;
            durumuExitGateGuid = 0;

            jiKunGuid = 0;

            firstMoguBloodVatGuid = 0;
            secondMoguBloodVatGuid = 0;
            thirdMoguBloodVatGuid = 0;
            fourthMoguBloodVatGuid = 0;
            fifthMoguBloodVatGuid = 0;
            sixthMoguBloodVatGuid = 0;
            seventhMoguBloodVatGuid = 0;
            eighthMoguBloodVatGuid = 0;
            ninthMoguBloodVatGuid = 0;
            tenthMoguBloodVatGuid = 0;

            for (uint8 i = 0; i < 4; ++i)
            {
                moguFountainsGuids[i] = 0;
                moguStatuesGuids[i] = 0;
                tribalDoorsGuid[i] = 0;
            }

            m_DarkAnimusGuid = 0;
            m_AnimaOrbGuid = 0;

            m_IronQonGuid = 0;
            m_RoShakGuid = 0;
            m_QuetZalGuid = 0;
            m_DamRenGuid = 0;

            m_LulinGuid = 0;
            m_SuenGuid = 0;

            m_ThunderTrapActivated = false;
            m_ThirdPartInit = false;
            m_LeiShenGuid = 0;
            m_ChargingStationGuid = 0;

            memset(m_PillarControllerGuids, 0, sizeof(uint64) * 4);
            memset(m_FloorGuids, 0, sizeof(uint64) * 4);
            memset(m_LeiShenWindows, 0, sizeof(uint64) * 4);
            memset(m_LeiShenWindFX, 0, sizeof(uint64) * 4);
            memset(m_LeiShenPillars, 0, sizeof(uint64) * 4);

            m_RadenIrisGuid = 0;
        }

        void OnCreatureCreate(Creature* p_Creature) override
        {
            switch (p_Creature->GetEntry())
            {
            case NPC_JIN_ROKH_THE_BREAKER:
                jinRokhTheBreakerGuid = p_Creature->GetGUID();
                break;
            case NPC_GARA_JAL:
                garaJalGuid = p_Creature->GetGUID();
                break;
            case NPC_GARA_JAL_SOUL:
                garaJalSoulGuid = p_Creature->GetGUID();
                break;
            case NPC_HORRIDON:
                horridonGuid = p_Creature->GetGUID();
                break;
            case NPC_WAR_GOD_JALAK:
                warGodJalakGuid = p_Creature->GetGUID();
                break;
            case NPC_STATUE:
            {
                Position pos;
                p_Creature->GetPosition(&pos);

                if (pos.GetPositionX() == g_StatuesPos[0].GetPositionX() &&
                    pos.GetPositionY() == g_StatuesPos[0].GetPositionY())
                    moguStatuesGuids[0] = p_Creature->GetGUID();
                else if (pos.GetPositionX() == g_StatuesPos[1].GetPositionX() &&
                    pos.GetPositionY() == g_StatuesPos[1].GetPositionY())
                    moguStatuesGuids[1] = p_Creature->GetGUID();
                else if (pos.GetPositionX() == g_StatuesPos[2].GetPositionX() &&
                    pos.GetPositionY() == g_StatuesPos[2].GetPositionY())
                    moguStatuesGuids[2] = p_Creature->GetGUID();
                else if (pos.GetPositionX() == g_StatuesPos[3].GetPositionX() &&
                    pos.GetPositionY() == g_StatuesPos[3].GetPositionY())
                    moguStatuesGuids[3] = p_Creature->GetGUID();
                break;
            }
            case NPC_KAZRA_JIN:
                kazraJinGuid = p_Creature->GetGUID();
                break;
            case NPC_SUL_THE_SANDCRAWLER:
                sudTheSandCrawlerGuid = p_Creature->GetGUID();
                break;
            case NPC_FROST_KING_MALAKK:
                forostKingMalakkGuid = p_Creature->GetGUID();
                break;
            case NPC_HIGH_PRIESTRESS_MAR_LI:
                highPriestressMarLiGuid = p_Creature->GetGUID();
                break;
            case NPC_JI_KUN:
                jiKunGuid = p_Creature->GetGUID();
                break;
            case NPC_MEGAERA:
                megaeraGuid = p_Creature->GetGUID();
                break;
            case NPC_FLAMING_HEAD:
                flamingMegaeraHead = p_Creature->GetGUID();
                break;
            case NPC_FROZEN_HEAD:
                frozenMegaeraHead = p_Creature->GetGUID();
                break;
            case NPC_VENOMOUS_HEAD:
                venomousMegaeraHead = p_Creature->GetGUID();
                break;
            case NPC_ARCANE_HEAD:
                arcarneMegaeraHead = p_Creature->GetGUID();
                break;
            case NPC_DURUMU_THE_FORGOTTEN:
                durumuGuid = p_Creature->GetGUID();
                break;
            case NPC_DARK_ANIMUS:
                m_DarkAnimusGuid = p_Creature->GetGUID();
                break;
            case NPC_ANIMA_ORB:
                m_AnimaOrbGuid = p_Creature->GetGUID();
                break;
            case NPC_IRON_QON:
                m_IronQonGuid = p_Creature->GetGUID();
                break;
            case NPC_RO_SHAK:
                m_RoShakGuid = p_Creature->GetGUID();
                break;
            case NPC_QUET_ZAL:
                m_QuetZalGuid = p_Creature->GetGUID();
                break;
            case NPC_DAM_REN:
                m_DamRenGuid = p_Creature->GetGUID();
                break;
            case NPC_LU_LIN:
                m_LulinGuid = p_Creature->GetGUID();
                break;
            case NPC_SUEN:
                m_SuenGuid = p_Creature->GetGUID();
                break;
            case NPC_WORLD_TRIGGER:
                p_Creature->SetReactState(REACT_PASSIVE);
                p_Creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                break;
            case NPC_LEI_SHEN:
                m_LeiShenGuid = p_Creature->GetGUID();
                break;
            case NPC_STATIC_SHOCK_CONDUIT:
                m_PillarControllerGuids[0] = p_Creature->GetGUID();
                break;
            case NPC_DIFFUSION_CHAIN_CONDUIT:
                m_PillarControllerGuids[1] = p_Creature->GetGUID();
                break;
            case NPC_OVERCHARGE_CONDUIT:
                m_PillarControllerGuids[2] = p_Creature->GetGUID();
                break;
            case NPC_BOUNCING_BOLT_CONDUIT:
                m_PillarControllerGuids[3] = p_Creature->GetGUID();
                break;
            default:
                break;
            }
        }

        void OnGameObjectCreate(GameObject* p_GameObject) override
        {
            switch (p_GameObject->GetEntry())
            {
                /// Doors
                case GOB_JIN_ROKH_ENTRANCE_DOOR:
                case GOB_JIN_ROKH_EXIT_DOOR:
                case GOB_HORRIDON_EXIT_DOOR:
                case GOB_COUNCIL_ENTRANCE_DOOR_LEFT:
                case GOB_COUNCIL_ENTRANCE_DOOR_RIGHT:
                case GOB_COUNCIL_EXIT_DOOR:
                case GOB_TORTOS_EXIT_DOOR:
                case GOB_MEGAERA_EXIT_DOOR:
                case GOB_JI_KUN_EXIT_DOOR:
                case GOB_DURUMU_EXIT_DOOR:
                case GOB_PRIMORDIUS_ENTRANCE:
                case GOB_PRIMORDIUS_EXIT:
                case GOB_DARK_ANIMUS_ENTRANCE:
                case GOB_DARK_ANIMUS_EXIT:
                case GOB_IRON_QON_SEWER_DOOR:
                case GOB_IRON_QON_EXIT_DOOR:
                case GOB_IRON_QON_EXIT_DOOR_2:
                case GOB_TWIN_ENTRANCE:
                case GOB_TWIN_EXIT:
                case GOB_CONCUBINES_BACK:
                case GOB_CONCUBINES_FRONT:
                    AddDoor(p_GameObject, true);
                    break;
                    /// Jin'Rokh #01
                case GOB_MOGU_FOUNTAIN_NE:
                    moguFountainsGuids[0] = p_GameObject->GetGUID();
                    p_GameObject->SetGoState(GO_STATE_READY);
                    break;
                case GOB_MOGU_FOUNTAIN_NW:
                    moguFountainsGuids[1] = p_GameObject->GetGUID();
                    p_GameObject->SetGoState(GO_STATE_READY);
                    break;
                case GOB_MOGU_FOUNTAIN_SW:
                    moguFountainsGuids[2] = p_GameObject->GetGUID();
                    p_GameObject->SetGoState(GO_STATE_READY);
                    break;
                case GOB_MOGU_FOUNTAIN_SE:
                    moguFountainsGuids[3] = p_GameObject->GetGUID();
                    p_GameObject->SetGoState(GO_STATE_READY);
                    break;
                    /// Horridon #02
                case GOB_HORRIDON_GATE:
                    horridonGateGuid = p_GameObject->GetGUID();
                    break;
                case GOB_FARRAKI_TRIBAL_DOOR:
                    tribalDoorsGuid[0] = p_GameObject->GetGUID();
                    break;
                case GOB_GURUBASHI_TRIBAL_DOOR:
                    tribalDoorsGuid[1] = p_GameObject->GetGUID();
                    break;
                case GOB_DRAKKARI_TRIBAL_DOOR:
                    tribalDoorsGuid[2] = p_GameObject->GetGUID();
                    break;
                case GOB_AMANI_TRIBAL_DOOR:
                    tribalDoorsGuid[3] = p_GameObject->GetGUID();
                    break;
                    /// Ji-Kun #06
                    /// Durumu #07
                case GOB_DURUMU_ENTRANCE_GATE:
                    durumuEntranceGateGuid = p_GameObject->GetGUID();
                    break;
                case GOB_DURUMU_EXIT_GATE:
                    durumuExitGateGuid = p_GameObject->GetGUID();
                    break;
                    /// Primordius #08
                case GOB_FIRST_MOGU_BLOOD_VAT:
                    firstMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_SECOND_MOGU_BLOOD_VAT:
                    secondMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_THIRD_MOGU_BLOOD_VAT:
                    thirdMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_FOURTH_MOGU_BLOOD_VAT:
                    fourthMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_FIFTH_MOGU_BLOOD_VAT:
                    fifthMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_SIXTH_MOGU_BLOOD_VAT:
                    sixthMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_SEVENTH_MOGU_BLOOD_VAT:
                    seventhMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_EIGHTH_MOGU_BLOOD_VAT:
                    eighthMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_NINTH_MOGU_BLOOD_VAT:
                    ninthMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                case GOB_TENTH_MOGU_BLOOD_VAT:
                    tenthMoguBloodVatGuid = p_GameObject->GetGUID();
                    break;
                    /// Dark Animus #09
                    /// Iron Qon #10
                    /// Lei Shen #12
                case GOB_NORTH_PILLAR:
                    m_LeiShenPillars[0] = p_GameObject->GetGUID();
                    break;
                case GOB_EAST_PILLAR:
                    m_LeiShenPillars[1] = p_GameObject->GetGUID();
                    break;
                case GOB_SOUTH_PILLAR:
                    m_LeiShenPillars[2] = p_GameObject->GetGUID();
                    break;
                case GOB_WEST_PILLAR:
                    m_LeiShenPillars[3] = p_GameObject->GetGUID();
                    break;
                case GOB_FLOOR_NORTH:
                    m_FloorGuids[0] = p_GameObject->GetGUID();
                    break;
                case GOB_FLOOR_EAST:
                    m_FloorGuids[1] = p_GameObject->GetGUID();
                    break;
                case GOB_FLOOR_SOUTH:
                    m_FloorGuids[2] = p_GameObject->GetGUID();
                    break;
                case GOB_FLOOR_WEST:
                    m_FloorGuids[3] = p_GameObject->GetGUID();
                    break;
                case GOB_CHARGING_STATION:
                    m_ChargingStationGuid = p_GameObject->GetGUID();
                    break;
                case GOB_NW_WINDOW:
                    m_LeiShenWindows[0] = p_GameObject->GetGUID();
                    break;
                case GOB_NE_WINDOW:
                    m_LeiShenWindows[1] = p_GameObject->GetGUID();
                    break;
                case GOB_SE_WINDOW:
                    m_LeiShenWindows[2] = p_GameObject->GetGUID();
                    break;
                case GOB_SW_WINDOW:
                    m_LeiShenWindows[3] = p_GameObject->GetGUID();
                    break;
                case GOB_WIND_NW_FX:
                    m_LeiShenWindFX[0] = p_GameObject->GetGUID();
                    break;
                case GOB_WIND_NE_FX:
                    m_LeiShenWindFX[1] = p_GameObject->GetGUID();
                    break;
                case GOB_WIND_SE_FX:
                    m_LeiShenWindFX[2] = p_GameObject->GetGUID();
                    break;
                case GOB_WIND_SW_FX:
                    m_LeiShenWindFX[3] = p_GameObject->GetGUID();
                    break;
                    /// Ra-den #13
                case GOB_RADEN_SECRET_ENTRY_IRIS:
                    m_RadenIrisGuid = p_GameObject->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnGameObjectRemove(GameObject* p_GameObject) override
        {
            switch (p_GameObject->GetEntry())
            {
                    case GOB_JIN_ROKH_ENTRANCE_DOOR:
                    case GOB_JIN_ROKH_EXIT_DOOR:
                    case GOB_HORRIDON_EXIT_DOOR:
                    case GOB_COUNCIL_ENTRANCE_DOOR_LEFT:
                    case GOB_COUNCIL_ENTRANCE_DOOR_RIGHT:
                    case GOB_COUNCIL_EXIT_DOOR:
                    case GOB_TORTOS_EXIT_DOOR:
                    case GOB_MEGAERA_EXIT_DOOR:
                    case GOB_JI_KUN_EXIT_DOOR:
                    case GOB_DURUMU_EXIT_DOOR:
                    case GOB_PRIMORDIUS_ENTRANCE:
                    case GOB_PRIMORDIUS_EXIT:
                    case GOB_DARK_ANIMUS_ENTRANCE:
                    case GOB_DARK_ANIMUS_EXIT:
                    case GOB_IRON_QON_SEWER_DOOR:
                    case GOB_IRON_QON_EXIT_DOOR:
                    case GOB_IRON_QON_EXIT_DOOR_2:
                    case GOB_TWIN_ENTRANCE:
                    case GOB_TWIN_EXIT:
                    case GOB_CONCUBINES_BACK:
                    case GOB_CONCUBINES_FRONT:
                        AddDoor(p_GameObject, false);
                        break;
                    default:
                        break;
            }
        }

        bool SetBossState(uint32 p_ID, EncounterState p_State) override
        {
            if (!InstanceScript::SetBossState(p_ID, p_State))
                return false;

            return true;
        }

        void SetData(uint32 p_Type, uint32 p_Data) override
        {
            switch (p_Type)
            {
            case DATA_ANCIENT_MOGU_BELL:
            {
                ancientMoguBellActivate += p_Data;

                if (ancientMoguBellActivate >= 3)
                {
                    if (Creature* megaera = instance->GetCreature(megaeraGuid))
                        megaera->AI()->DoAction(ACTION_MEGAERA_SPAWN);
                }

                break;
            }
            case DATA_THUNDER_CLAP:
                m_ThunderTrapActivated = true;
                break;
            default:
                break;
            }
        }

        uint32 GetData(uint32 p_Type) override
        {
            switch (p_Type)
            {
            case DATA_ANCIENT_MOGU_BELL:
                return ancientMoguBellActivate;
            case DATA_THUNDER_CLAP:
                return m_ThunderTrapActivated;
            default:
                return 0;
            }
        }

        uint64 GetData64(uint32 p_Type) override
        {
            switch (p_Type)
            {
                /// Jin'rokh
            case NPC_JIN_ROKH_THE_BREAKER:
                return jinRokhTheBreakerGuid;
            case GOB_MOGU_FOUNTAIN_NE:
                return moguFountainsGuids[0];
            case GOB_MOGU_FOUNTAIN_NW:
                return moguFountainsGuids[1];
            case GOB_MOGU_FOUNTAIN_SE:
                return moguFountainsGuids[2];
            case GOB_MOGU_FOUNTAIN_SW:
                return moguFountainsGuids[3];
            case DATA_STATUE_0:
                return moguStatuesGuids[0];
            case DATA_STATUE_1:
                return moguStatuesGuids[1];
            case DATA_STATUE_2:
                return moguStatuesGuids[2];
            case DATA_STATUE_3:
                return moguStatuesGuids[3];
                /// Horridon
            case GOB_HORRIDON_GATE:
                return horridonGateGuid;
            case NPC_HORRIDON:
                return horridonGuid;
            case NPC_WAR_GOD_JALAK:
                return warGodJalakGuid;
            case GOB_FARRAKI_TRIBAL_DOOR:
                return tribalDoorsGuid[0];
            case GOB_GURUBASHI_TRIBAL_DOOR:
                return tribalDoorsGuid[1];
            case GOB_DRAKKARI_TRIBAL_DOOR:
                return tribalDoorsGuid[2];
            case GOB_AMANI_TRIBAL_DOOR:
                return tribalDoorsGuid[3];
                /// Council of Elders
            case NPC_GARA_JAL:
                return garaJalGuid;
            case NPC_GARA_JAL_SOUL:
                return garaJalSoulGuid;
            case NPC_KAZRA_JIN:
                return kazraJinGuid;
            case NPC_SUL_THE_SANDCRAWLER:
                return sudTheSandCrawlerGuid;
            case NPC_FROST_KING_MALAKK:
                return forostKingMalakkGuid;
            case NPC_HIGH_PRIESTRESS_MAR_LI:
                return highPriestressMarLiGuid;
                /// Tortos
                /// Megaera
            case NPC_MEGAERA:
                return megaeraGuid;
            case NPC_FLAMING_HEAD:
                return flamingMegaeraHead;
            case NPC_FROZEN_HEAD:
                return frozenMegaeraHead;
            case NPC_VENOMOUS_HEAD:
                return venomousMegaeraHead;
            case NPC_ARCANE_HEAD:
                return arcarneMegaeraHead;
                /// Ji-Kun
            case NPC_JI_KUN:
                return jiKunGuid;
                /// Durumu
            case GOB_DURUMU_ENTRANCE_GATE:
                return durumuEntranceGateGuid;
            case GOB_DURUMU_EXIT_GATE:
                return durumuExitGateGuid;
            case NPC_DURUMU_THE_FORGOTTEN:
                return durumuGuid;
                /// Primordius
            case GOB_FIRST_MOGU_BLOOD_VAT:
                return firstMoguBloodVatGuid;
            case GOB_SECOND_MOGU_BLOOD_VAT:
                return secondMoguBloodVatGuid;
            case GOB_THIRD_MOGU_BLOOD_VAT:
                return thirdMoguBloodVatGuid;
            case GOB_FOURTH_MOGU_BLOOD_VAT:
                return fourthMoguBloodVatGuid;
            case GOB_FIFTH_MOGU_BLOOD_VAT:
                return fifthMoguBloodVatGuid;
            case GOB_SIXTH_MOGU_BLOOD_VAT:
                return sixthMoguBloodVatGuid;
            case GOB_SEVENTH_MOGU_BLOOD_VAT:
                return seventhMoguBloodVatGuid;
            case GOB_EIGHTH_MOGU_BLOOD_VAT:
                return eighthMoguBloodVatGuid;
            case GOB_NINTH_MOGU_BLOOD_VAT:
                return ninthMoguBloodVatGuid;
            case GOB_TENTH_MOGU_BLOOD_VAT:
                return tenthMoguBloodVatGuid;
                /// Dark Animus
            case NPC_DARK_ANIMUS:
                return m_DarkAnimusGuid;
            case NPC_ANIMA_ORB:
                return m_AnimaOrbGuid;
                /// Iron Qon
            case NPC_IRON_QON:
                return m_IronQonGuid;
            case NPC_RO_SHAK:
                return m_RoShakGuid;
            case NPC_QUET_ZAL:
                return m_QuetZalGuid;
            case NPC_DAM_REN:
                return m_DamRenGuid;
                /// Twin Consorts
            case NPC_LU_LIN:
                return m_LulinGuid;
            case NPC_SUEN:
                return m_SuenGuid;
                /// Lei Shen
            case NPC_LEI_SHEN:
                return m_LeiShenGuid;
            case NPC_STATIC_SHOCK_CONDUIT:
                return m_PillarControllerGuids[0];
            case NPC_DIFFUSION_CHAIN_CONDUIT:
                return m_PillarControllerGuids[1];
            case NPC_OVERCHARGE_CONDUIT:
                return m_PillarControllerGuids[2];
            case NPC_BOUNCING_BOLT_CONDUIT:
                return m_PillarControllerGuids[3];
            case GOB_NORTH_PILLAR:
                return m_LeiShenPillars[0];
            case GOB_EAST_PILLAR:
                return m_LeiShenPillars[1];
            case GOB_SOUTH_PILLAR:
                return m_LeiShenPillars[2];
            case GOB_WEST_PILLAR:
                return m_LeiShenPillars[3];
            case GOB_FLOOR_NORTH:
                return m_FloorGuids[0];
            case GOB_FLOOR_EAST:
                return m_FloorGuids[1];
            case GOB_FLOOR_SOUTH:
                return m_FloorGuids[2];
            case GOB_FLOOR_WEST:
                return m_FloorGuids[3];
            case GOB_CHARGING_STATION:
                return m_ChargingStationGuid;
            case GOB_NW_WINDOW:
                return m_LeiShenWindows[0];
            case GOB_NE_WINDOW:
                return m_LeiShenWindows[1];
            case GOB_SE_WINDOW:
                return m_LeiShenWindows[2];
            case GOB_SW_WINDOW:
                return m_LeiShenWindows[3];
            case GOB_WIND_NW_FX:
                return m_LeiShenWindFX[0];
            case GOB_WIND_NE_FX:
                return m_LeiShenWindFX[1];
            case GOB_WIND_SE_FX:
                return m_LeiShenWindFX[2];
            case GOB_WIND_SW_FX:
                return m_LeiShenWindFX[3];
                /// Ra-den
            case GOB_RADEN_SECRET_ENTRY_IRIS:
                return m_RadenIrisGuid;
            default:
                break;
            }

            return 0;
        }

        bool CheckRequiredBosses(uint32 p_BossID, Player const* p_Player = NULL) const override
        {
            if (!InstanceScript::CheckRequiredBosses(p_BossID, p_Player))
                return false;

            switch (p_BossID)
            {
            case DATA_HORRIDON:
            case DATA_COUNCIL_OF_ELDERS:
            case DATA_TORTOS:
            case DATA_MEGAERA:
            case DATA_JI_KUN:
            case DATA_DURUMU_THE_FORGOTTEN:
            case DATA_PRIMORDIUS:
            case DATA_DARK_ANIMUS:
            case DATA_IRON_QON:
            case DATA_TWIN_CONSORTS:
            case DATA_LEI_SHEN:
            case DATA_RA_DEN:
                if (GetBossState(p_BossID - 1) != DONE)
                    return false;
            default:
                break;
            }

            return true;
        }

        void OnPlayerLeave(Player* p_Player)
        {
            std::list<uint32> l_AurasToRemove;
            l_AurasToRemove.push_back(SPELL_ARCING_LIGHTNING);
            l_AurasToRemove.push_back(SPELL_ARCING_PERIODIC_CHECK);
            l_AurasToRemove.push_back(SPELL_ARCING_PLAYER_CHECK);

            for (uint32 l_Aura : l_AurasToRemove)
            {
                if (p_Player->HasAura(l_Aura))
                    p_Player->RemoveAura(l_Aura);
            }
        }

        void OnPlayerEnter(Player* p_Player) override
        {
            if (!m_ThirdPartInit && instance->IsLFR())
            {
                uint32 l_DungeonID = p_Player->GetGroup() ? sLFGMgr->GetDungeon(p_Player->GetGroup()->GetGUID()) : 0;
                if (l_DungeonID == 612) ///< Third part
                    m_ThirdPartInit = true;
            }
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_instance_throne_of_thunder()
{
    new instance_throne_of_thunder();
}
#endif
