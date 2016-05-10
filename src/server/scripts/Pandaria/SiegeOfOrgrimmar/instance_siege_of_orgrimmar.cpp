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
#include "siege_of_orgrimmar.h"

DoorData const doorData[] =
{
    /*{GOB_STONE_GUARD_DOOR_ENTRANCE,          DATA_STONE_GUARD,          DOOR_TYPE_ROOM,       BOUNDARY_E   },
    {GOB_STONE_GUARD_DOOR_EXIT,              DATA_STONE_GUARD,          DOOR_TYPE_PASSAGE,    BOUNDARY_W   },
    {GOB_FENG_DOOR_FENCE,                    DATA_FENG,                 DOOR_TYPE_ROOM,       BOUNDARY_NONE},
    {GOB_FENG_DOOR_EXIT,                     DATA_FENG,                 DOOR_TYPE_PASSAGE,    BOUNDARY_N   },
    {GOB_GARAJAL_FENCE,                      DATA_GARAJAL,              DOOR_TYPE_ROOM,       BOUNDARY_NONE},
    {GOB_GARAJAL_EXIT,                       DATA_GARAJAL,              DOOR_TYPE_PASSAGE,    BOUNDARY_W   },
    {GOB_SPIRIT_KINGS_WIND_WALL,             DATA_SPIRIT_KINGS,         DOOR_TYPE_ROOM,       BOUNDARY_NONE},
    {GOB_SPIRIT_KINGS_EXIT,                  DATA_SPIRIT_KINGS,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {GOB_CELESTIAL_DOOR,                     DATA_ELEGON,               DOOR_TYPE_ROOM,       BOUNDARY_E   },
    {GOB_ELEGON_DOOR_ENTRANCE,               DATA_SPIRIT_KINGS,         DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},
    {GOB_ELEGON_CELESTIAL_DOOR,              DATA_ELEGON,               DOOR_TYPE_ROOM,       BOUNDARY_E   },
    {GOB_WILL_OF_EMPEROR_ENTRANCE,           DATA_ELEGON,               DOOR_TYPE_PASSAGE,    BOUNDARY_NONE},*/
    {0,                                      0,                         DOOR_TYPE_ROOM,       BOUNDARY_NONE},// END
};

class instance_siege_of_orgrimmar : public InstanceMapScript
{
    public:
        instance_siege_of_orgrimmar() : InstanceMapScript("instance_siege_of_orgrimmar", 1136) { }

        InstanceScript* GetInstanceScript(InstanceMap* map) const
        {
            return new instance_siege_of_orgrimmar_InstanceMapScript(map);
        }

        struct instance_siege_of_orgrimmar_InstanceMapScript : public InstanceScript
        {
            instance_siege_of_orgrimmar_InstanceMapScript(Map* map) : InstanceScript(map) {}

            // First part
            uint64 immerseusGuid;
            uint64 fallenProtectorsGuid[MAX_FALLEN_PROTECTORS];
            uint64 norushenGuid;
            uint64 amalgamOfCorruptionGuid;
            uint64 shaOfPrideGuid;

            // Second part
            uint64 galakrasGuid;
            uint64 ironJuggernautGuid;
            uint64 korkronDarkShamansGuid[MAX_KORKRON_DARK_SHAMANS];
            uint64 generalNazgrimGuid;

            // Third part
            uint64 malkorokGuid;
            uint64 spoilsOfPandariaGuid;
            uint64 thokTheBloodthirstyGuid;

            // Last part
            uint64 siegecrafterBlackfuseGuid;
            uint64 paragonsOfTheKlaxxiGuid[MAX_PARAGONS_OF_THE_KLAXXI];
            uint64 garroshHellscreamGuid;

            void Initialize()
            {
                SetBossNumber(DATA_MAX_BOSS_DATA);
                LoadDoorData(doorData);

                immerseusGuid                   = 0;

                for (uint8 i = 0; i < MAX_FALLEN_PROTECTORS; ++i)
                    fallenProtectorsGuid[i]    = 0;
                
                norushenGuid                    = 0;
                amalgamOfCorruptionGuid         = 0;
                shaOfPrideGuid                  = 0;
                galakrasGuid                    = 0;
                ironJuggernautGuid              = 0;

                for (uint8 i = 0; i < MAX_KORKRON_DARK_SHAMANS; ++i)
                    korkronDarkShamansGuid[i]   = 0;

                malkorokGuid                    = 0;
                spoilsOfPandariaGuid            = 0;
                thokTheBloodthirstyGuid         = 0;
                siegecrafterBlackfuseGuid       = 0;

                for (uint8 i = 0; i < MAX_PARAGONS_OF_THE_KLAXXI; ++i)
                    paragonsOfTheKlaxxiGuid[i]  = 0;

                garroshHellscreamGuid           = 0;
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_IMMERSEUS:
                        immerseusGuid = creature->GetGUID();
                        break;
                    case NPC_ROOK_STONETOE:
                        fallenProtectorsGuid[ROOK_STONETOE] = creature->GetGUID();
                        break;
                    case NPC_HE_SOFTFOOT:
                        fallenProtectorsGuid[HE_SOFTFOOT] = creature->GetGUID();
                        break;
                    case NPC_SUN_THUNDERHEART:
                        fallenProtectorsGuid[SUN_THUNDERHEART] = creature->GetGUID();
                        break;
                    case NPC_NORUSHEN:
                        norushenGuid = creature->GetGUID();
                        break;
                    case NPC_AMALGAM_OF_CORRUPTION:
                        amalgamOfCorruptionGuid = creature->GetGUID();
                        break;
                    case NPC_SHA_OF_PRIDE:
                        shaOfPrideGuid = creature->GetGUID();
                        break;
                    case NPC_GALAKRAS:
                        galakrasGuid = creature->GetGUID();
                        break;
                    case NPC_IRON_JUGGERNAUT:
                        ironJuggernautGuid = creature->GetGUID();
                        break;
                    case NPC_EARTHBREAKER_HAROMM:
                        korkronDarkShamansGuid[EARTHBREAKER_HAROMM] = creature->GetGUID();
                        break;
                    case NPC_WAVEBINDER_KARDIS:
                        korkronDarkShamansGuid[WAVEBINDER_KARDIS] = creature->GetGUID();
                        break;
                    case NPC_GENERAL_NAZGRIM:
                        generalNazgrimGuid = creature->GetGUID();
                        break;
                    case NPC_MALKOROK:
                        malkorokGuid = creature->GetGUID();
                        break;
                    case NPC_SPOILS_OF_PANDARIA:
                        spoilsOfPandariaGuid = creature->GetGUID();
                        break;
                    case NPC_THOK_THE_BLOODTHIRSTY:
                        thokTheBloodthirstyGuid = creature->GetGUID();
                        break;
                    case NPC_SIEGECRAFTER_BLACKFUSE:
                        siegecrafterBlackfuseGuid = creature->GetGUID();
                        break;
                    case NPC_KILRUK_THE_WIND_REAVER:
                        paragonsOfTheKlaxxiGuid[KILRUK_THE_WIND_REAVER] = creature->GetGUID();
                        break;
                    case NPC_XARIL_THE_POISONED_MIND:
                        paragonsOfTheKlaxxiGuid[XARIL_THE_POISONED_MIND] = creature->GetGUID();
                        break;
                    case NPC_KAZTIK_THE_MANIPULATOR:
                        paragonsOfTheKlaxxiGuid[KAZTIK_THE_MANIPULATOR] = creature->GetGUID();
                        break;
                    case NPC_KORVEN_THE_PRIME:
                        paragonsOfTheKlaxxiGuid[KORVEN_THE_PRIME] = creature->GetGUID();
                        break;
                    case NPC_IYYOKUK_THE_LUCID:
                        paragonsOfTheKlaxxiGuid[IYYOKUK_THE_LUCID] = creature->GetGUID();
                        break;
                    case NPC_KAROZ_THE_LOCUST:
                        paragonsOfTheKlaxxiGuid[KAROZ_THE_LOCUST] = creature->GetGUID();
                        break;
                    case NPC_SKEER_THE_BLOODSEEKER:
                        paragonsOfTheKlaxxiGuid[SKEER_THE_BLOODSEEKER] = creature->GetGUID();
                        break;
                    case NPC_RIKKAL_THE_DISSECTOR:
                        paragonsOfTheKlaxxiGuid[RIKKAL_THE_DISSECTOR] = creature->GetGUID();
                        break;
                    case NPC_HISEK_THE_SWARMKEEPER:
                        paragonsOfTheKlaxxiGuid[HISEK_THE_SWARMKEEPER] = creature->GetGUID();
                        break;
                    case NPC_GARROSH_HELLSCREAM:
                        garroshHellscreamGuid = creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* /*go*/)
            {
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                return true;
            }

            void SetData(uint32 /*type*/, uint32 /*p_Data*/)
            {
            }

            uint32 GetData(uint32 /*type*/)
            {
                return 0;
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case NPC_IMMERSEUS:
                        return immerseusGuid;
                    case NPC_ROOK_STONETOE:
                        return fallenProtectorsGuid[ROOK_STONETOE];
                    case NPC_HE_SOFTFOOT:
                        return fallenProtectorsGuid[HE_SOFTFOOT];
                    case NPC_SUN_THUNDERHEART:
                        return fallenProtectorsGuid[SUN_THUNDERHEART];
                    case NPC_NORUSHEN:
                        return norushenGuid;
                    case NPC_AMALGAM_OF_CORRUPTION:
                        return amalgamOfCorruptionGuid;
                    case NPC_SHA_OF_PRIDE:
                        return shaOfPrideGuid;
                    case NPC_GALAKRAS:
                        return galakrasGuid;
                    case NPC_IRON_JUGGERNAUT:
                        return ironJuggernautGuid;
                    case NPC_EARTHBREAKER_HAROMM:
                        return korkronDarkShamansGuid[EARTHBREAKER_HAROMM];
                    case NPC_WAVEBINDER_KARDIS:
                        return korkronDarkShamansGuid[WAVEBINDER_KARDIS];
                    case NPC_GENERAL_NAZGRIM:
                        return generalNazgrimGuid;
                    case NPC_MALKOROK:
                        return malkorokGuid;
                    case NPC_SPOILS_OF_PANDARIA:
                        return spoilsOfPandariaGuid;
                    case NPC_THOK_THE_BLOODTHIRSTY:
                        return thokTheBloodthirstyGuid;
                    case NPC_SIEGECRAFTER_BLACKFUSE:
                        return siegecrafterBlackfuseGuid;
                    case NPC_KILRUK_THE_WIND_REAVER:
                        return paragonsOfTheKlaxxiGuid[KILRUK_THE_WIND_REAVER];
                    case NPC_XARIL_THE_POISONED_MIND:
                        return paragonsOfTheKlaxxiGuid[XARIL_THE_POISONED_MIND];
                    case NPC_KAZTIK_THE_MANIPULATOR:
                        return paragonsOfTheKlaxxiGuid[KAZTIK_THE_MANIPULATOR];
                    case NPC_KORVEN_THE_PRIME:
                        return paragonsOfTheKlaxxiGuid[KORVEN_THE_PRIME];
                    case NPC_IYYOKUK_THE_LUCID:
                        return paragonsOfTheKlaxxiGuid[IYYOKUK_THE_LUCID];
                    case NPC_KAROZ_THE_LOCUST:
                        return paragonsOfTheKlaxxiGuid[KAROZ_THE_LOCUST];
                    case NPC_SKEER_THE_BLOODSEEKER:
                        return paragonsOfTheKlaxxiGuid[SKEER_THE_BLOODSEEKER];
                    case NPC_RIKKAL_THE_DISSECTOR:
                        return paragonsOfTheKlaxxiGuid[RIKKAL_THE_DISSECTOR];
                    case NPC_HISEK_THE_SWARMKEEPER:
                        return paragonsOfTheKlaxxiGuid[HISEK_THE_SWARMKEEPER];
                    case NPC_GARROSH_HELLSCREAM:
                        return garroshHellscreamGuid;
                    default:
                        break;
                }

                return 0;
            }

            bool IsWipe()
            {
                Map::PlayerList const& PlayerList = instance->GetPlayers();

                if (PlayerList.isEmpty())
                    return true;

                for (Map::PlayerList::const_iterator Itr = PlayerList.begin(); Itr != PlayerList.end(); ++Itr)
                {
                    Player* player = Itr->getSource();

                    if (!player)
                        continue;

                    if (player->isAlive() && !player->isGameMaster())
                        return false;
                }

                return true;
            }

            bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const
            {
                if (!InstanceScript::CheckRequiredBosses(bossId, player))
                    return false;

                switch (bossId)
                {
                    case DATA_GARROSH_HELLSCREAM:
                    case DATA_PARAGONS_OF_THE_KLAXXI:
                    case DATA_SIEGECRAFTER_BLACKFUSE:
                    case DATA_THOK_THE_BLOODTHIRSTY:
                    case DATA_SPOILS_OF_PANDARIA:
                    case DATA_MALKOROK:
                    case DATA_GENERAL_NAZGRIM:
                    case DATA_KORKRON_DARK_SHAMANS:
                    case DATA_IRON_JUGGERNAUT:
                    case DATA_GALAKRAS:
                    case DATA_SHA_OF_PRIDE:
                    case DATA_NORUSHEN:
                    case DATA_FALLEN_PROTECTORS:
                        if (GetBossState(bossId - 1) != DONE)
                            return false;
                    default:
                        break;
                }

                return true;
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_instance_siege_of_orgrimmar()
{
    new instance_siege_of_orgrimmar();
}
#endif