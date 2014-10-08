/*
 * Copyright (C) 2008-20XX JadeCore <http://www.pandashan.com>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
    { GOB_COUNCIL_ENTRANCE_DOOR_LEFT,       DATA_CONCIL_OF_ELDERS,      DOOR_TYPE_ROOM,     BOUNDARY_W      },
    { GOB_COUNCIL_ENTRANCE_DOOR_RIGHT,      DATA_CONCIL_OF_ELDERS,      DOOR_TYPE_ROOM,     BOUNDARY_W      },
    { GOB_COUNCIL_EXIT_DOOR,                DATA_CONCIL_OF_ELDERS,      DOOR_TYPE_PASSAGE,  BOUNDARY_E      },
    { GOB_TORTOS_EXIT_DOOR,                 DATA_TORTOS,                DOOR_TYPE_PASSAGE,  BOUNDARY_E      },
    { 0,                                    0,                          DOOR_TYPE_ROOM,     BOUNDARY_NONE   }  // END
};

Position statuesPos[4] =
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

            uint64 jinRokhTheBreakerGuid;
            uint64 moguFountainsGuids[4];
            uint64 moguStatuesGuids[4];

            uint64 kazraJinGuid;
            uint64 sudTheSandCrawlerGuid;
            uint64 forostKingMalakkGuid;
            uint64 highPriestressMarLiGuid;
            uint64 garaJalGuid;

            uint64 horridonGuid;
            uint64 horridonGateGuid;
            uint64 tribalDoorsGuid[4];
            uint64 warGodJalakGuid;

            uint64 jiKunGuid;
            uint64 jiKunExitDoorGuid;

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

            uint8 ancientMoguBellActivate;
            uint64 megaeraGuid;
            uint64 durumuGuid;

            uint64 m_DarkAnimusGuid;
            uint64 m_AnimaOrbGuid;
            uint8 m_AnimaGolemToDisable;
            uint8 m_LargeAnimaGolemToDisable;

            void Initialize()
            {
                SetBossNumber(DATA_MAX_BOSS_DATA);
                LoadDoorData(doorData);

                jinRokhTheBreakerGuid   = 0;

                kazraJinGuid            = 0;
                sudTheSandCrawlerGuid   = 0;
                forostKingMalakkGuid    = 0;
                highPriestressMarLiGuid = 0;
                garaJalGuid             = 0;

                horridonGuid            = 0;
                horridonGateGuid        = 0;
                warGodJalakGuid         = 0;

                ancientMoguBellActivate = 0;
                megaeraGuid             = 0;

                jiKunGuid               = 0;
                jiKunExitDoorGuid       = 0;

                firstMoguBloodVatGuid   = 0;
                secondMoguBloodVatGuid  = 0;
                thirdMoguBloodVatGuid   = 0;
                fourthMoguBloodVatGuid  = 0;
                fifthMoguBloodVatGuid   = 0;
                sixthMoguBloodVatGuid   = 0;
                seventhMoguBloodVatGuid = 0;
                eighthMoguBloodVatGuid  = 0;
                ninthMoguBloodVatGuid   = 0;
                tenthMoguBloodVatGuid   = 0;
                durumuGuid              = 0;

                for (uint8 i = 0; i < 4; ++i)
                {
                    moguFountainsGuids[i]   = 0;
                    moguStatuesGuids[i]     = 0;
                    tribalDoorsGuid[i]      = 0;
                }

                m_DarkAnimusGuid = 0;
                m_AnimaOrbGuid = 0;

                if (instance->Is25ManRaid())
                {
                    m_AnimaGolemToDisable = 0;
                    m_LargeAnimaGolemToDisable = 0;
                }
                else
                {
                    m_AnimaGolemToDisable = 13;
                    m_LargeAnimaGolemToDisable = 3;
                }
            }

            void OnCreatureCreate(Creature* creature)
            {
                switch (creature->GetEntry())
                {
                    case NPC_JIN_ROKH_THE_BREAKER:
                        jinRokhTheBreakerGuid = creature->GetGUID();
                        break;
                    case NPC_GARA_JAL_SOUL:
                        garaJalGuid = creature->GetGUID();
                        break;
                    case NPC_HORRIDON:
                        horridonGuid = creature->GetGUID();
                        break;
                    case NPC_WAR_GOD_JALAK:
                        warGodJalakGuid = creature->GetGUID();
                        break;
                    case NPC_STATUE:
                    {
                        Position pos;
                        creature->GetPosition(&pos);

                        if (pos.GetPositionX() == statuesPos[0].GetPositionX() &&
                            pos.GetPositionY() == statuesPos[0].GetPositionY())
                            moguStatuesGuids[0] = creature->GetGUID();
                        else if (pos.GetPositionX() == statuesPos[1].GetPositionX() &&
                                    pos.GetPositionY() == statuesPos[1].GetPositionY())
                            moguStatuesGuids[1] = creature->GetGUID();
                        else if (pos.GetPositionX() == statuesPos[2].GetPositionX() &&
                                    pos.GetPositionY() == statuesPos[2].GetPositionY())
                            moguStatuesGuids[2] = creature->GetGUID();
                        else if (pos.GetPositionX() == statuesPos[3].GetPositionX() &&
                                    pos.GetPositionY() == statuesPos[3].GetPositionY())
                            moguStatuesGuids[3] = creature->GetGUID();
                        break;
                    }
                    case NPC_KAZRA_JIN:
                        kazraJinGuid = creature->GetGUID();
                        break;
                    case NPC_SUL_THE_SANDCRAWLER:
                        sudTheSandCrawlerGuid = creature->GetGUID();
                        break;
                    case NPC_FROST_KING_MALAKK:
                        forostKingMalakkGuid = creature->GetGUID();
                        break;
                    case NPC_HIGH_PRIESTRESS_MAR_LI:
                        highPriestressMarLiGuid = creature->GetGUID();
                        break;
                    case NPC_JI_KUN:
                        jiKunGuid = creature->GetGUID();
                        break;
                    case NPC_MEGAERA:
                        megaeraGuid = creature->GetGUID();
                        break;
                    case NPC_DURUMU_THE_FORGOTTEN:
                        durumuGuid = creature->GetGUID();
                        break;
                    case NPC_DARK_ANIMUS:
                        m_DarkAnimusGuid = creature->GetGUID();
                        break;
                    case NPC_ANIMA_ORB:
                        m_AnimaOrbGuid = creature->GetGUID();
                        break;
                    case NPC_ANIMA_GOLEM:
                    {
                        // ACTION_DESACTIVATE_GOLEM
                        if (urand(0, 1) && m_AnimaGolemToDisable)
                        {
                            // SPELL_CRITICALLY_DAMAGED_1
                            creature->CastSpell(creature, 138400, true);
                            --m_AnimaGolemToDisable;
                        }
                        break;
                    }
                    case NPC_LARGE_ANIMA_GOLEM:
                    {
                        // ACTION_DESACTIVATE_GOLEM
                        if (urand(0, 1) && m_LargeAnimaGolemToDisable)
                        {
                            // SPELL_CRITICALLY_DAMAGED_1
                            creature->CastSpell(creature, 138400, true);
                            --m_LargeAnimaGolemToDisable;
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnGameObjectCreate(GameObject* go)
            {
                switch (go->GetEntry())
                {
                    case GOB_MOGU_FOUNTAIN_NE:
                        moguFountainsGuids[0] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_MOGU_FOUNTAIN_NW:
                        moguFountainsGuids[1] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_MOGU_FOUNTAIN_SW:
                        moguFountainsGuids[2] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_MOGU_FOUNTAIN_SE:
                        moguFountainsGuids[3] = go->GetGUID();
                        go->SetGoState(GO_STATE_READY);
                        break;
                    case GOB_HORRIDON_GATE:
                        horridonGateGuid = go->GetGUID();
                        break;
                    case GOB_FARRAKI_TRIBAL_DOOR:
                        tribalDoorsGuid[0] = go->GetGUID();
                        break;
                    case GOB_GURUBASHI_TRIBAL_DOOR:
                        tribalDoorsGuid[1] = go->GetGUID();
                        break;
                    case GOB_DRAKKARI_TRIBAL_DOOR:
                        tribalDoorsGuid[2] = go->GetGUID();
                        break;
                    case GOB_AMANI_TRIBAL_DOOR:
                        tribalDoorsGuid[3] = go->GetGUID();
                        break;
                    case GOB_FIRST_MOGU_BLOOD_VAT:
                        firstMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_SECOND_MOGU_BLOOD_VAT:
                        secondMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_THIRD_MOGU_BLOOD_VAT:
                        thirdMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_FOURTH_MOGU_BLOOD_VAT:
                        fourthMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_FIFTH_MOGU_BLOOD_VAT:
                        fifthMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_SIXTH_MOGU_BLOOD_VAT:
                        sixthMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_SEVENTH_MOGU_BLOOD_VAT:
                        seventhMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_EIGHTH_MOGU_BLOOD_VAT:
                        eighthMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_NINTH_MOGU_BLOOD_VAT:
                        ninthMoguBloodVatGuid = go->GetGUID();
                        break;
                    case GOB_TENTH_MOGU_BLOOD_VAT:
                        tenthMoguBloodVatGuid = go->GetGUID();
                        break;
                    case  GOB_JI_KUN_EXIT_DOOR:
                        jiKunExitDoorGuid = go->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            bool SetBossState(uint32 id, EncounterState state)
            {
                if (!InstanceScript::SetBossState(id, state))
                    return false;

                return true;
            }

            void SetData(uint32 type, uint32 data)
            {
                switch (type)
                {
                    case DATA_ANCIENT_MOGU_BELL:
                    {
                        ancientMoguBellActivate += data;

                        if (ancientMoguBellActivate >= 3)
                        {
                            if (Creature* megaera = instance->GetCreature(megaeraGuid))
                                megaera->AI()->DoAction(ACTION_MEGAERA_SPAWN);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 type)
            {
                switch (type)
                {
                    case DATA_ANCIENT_MOGU_BELL:
                        return ancientMoguBellActivate;
                    default:
                        return 0;
                }
            }

            uint64 GetData64(uint32 type)
            {
                switch (type)
                {
                    case NPC_JIN_ROKH_THE_BREAKER:
                        return jinRokhTheBreakerGuid;
                    case NPC_GARA_JAL_SOUL:
                        return garaJalGuid;
                    case NPC_HORRIDON:
                        return horridonGuid;
                    case NPC_WAR_GOD_JALAK:
                        return warGodJalakGuid;
                    case GOB_MOGU_FOUNTAIN_NE:
                        return moguFountainsGuids[0];
                    case GOB_MOGU_FOUNTAIN_NW:
                        return moguFountainsGuids[1];
                    case GOB_MOGU_FOUNTAIN_SE:
                        return moguFountainsGuids[2];
                    case GOB_MOGU_FOUNTAIN_SW:
                        return moguFountainsGuids[3];
                    case GOB_HORRIDON_GATE:
                        return horridonGateGuid;
                    case GOB_FARRAKI_TRIBAL_DOOR:
                        return tribalDoorsGuid[0];
                    case GOB_GURUBASHI_TRIBAL_DOOR:
                        return tribalDoorsGuid[1];
                    case GOB_DRAKKARI_TRIBAL_DOOR:
                        return tribalDoorsGuid[2];
                    case GOB_AMANI_TRIBAL_DOOR:
                        return tribalDoorsGuid[3];
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
                    case GOB_JI_KUN_EXIT_DOOR:
                        return jiKunExitDoorGuid;
                    case DATA_STATUE_0:
                        return moguStatuesGuids[0];
                    case DATA_STATUE_1:
                        return moguStatuesGuids[1];
                    case DATA_STATUE_2:
                        return moguStatuesGuids[2];
                    case DATA_STATUE_3:
                        return moguStatuesGuids[3];
                    case NPC_KAZRA_JIN:
                        return kazraJinGuid;
                    case NPC_SUL_THE_SANDCRAWLER:
                        return sudTheSandCrawlerGuid;
                    case NPC_FROST_KING_MALAKK:
                        return forostKingMalakkGuid;
                    case NPC_HIGH_PRIESTRESS_MAR_LI:
                        return highPriestressMarLiGuid;
                    case NPC_JI_KUN:
                        return jiKunGuid;
                    case NPC_MEGAERA:
                        return megaeraGuid;
                    case NPC_DURUMU_THE_FORGOTTEN:
                        return durumuGuid;
                    case NPC_DARK_ANIMUS:
                        return m_DarkAnimusGuid;
                    case NPC_ANIMA_ORB:
                        return m_AnimaOrbGuid;
                    default:
                        break;
                }

                return 0;
            }

            bool CheckRequiredBosses(uint32 bossId, Player const* player = NULL) const
            {
                if (!InstanceScript::CheckRequiredBosses(bossId, player))
                    return false;

                switch (bossId)
                {
                    case DATA_HORRIDON:
                    case DATA_CONCIL_OF_ELDERS:
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
                        if (GetBossState(bossId - 1) != DONE)
                            return false;
                    default:
                        break;
                }

                return true;
            }
        };
};

void AddSC_instance_throne_of_thunder()
{
    new instance_throne_of_thunder();
}
