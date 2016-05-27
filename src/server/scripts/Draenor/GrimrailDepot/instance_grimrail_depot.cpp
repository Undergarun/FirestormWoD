////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "grimrail_depot.hpp"

/*
static BossScenarios const g_BossScenarios[] =
{
    { GrimrailDepotData::DataRocketsparkEncounter,   eGrimrailDepotScenarios::ScenarioBossRocketsparkAndBorka },
    { GrimrailDepotData::DataNitroggThundertower,    eGrimrailDepotScenarios::ScenarioBossNitroggThundertower },
    { GrimrailDepotData::DataSkyLordTovra,           eGrimrailDepotScenarios:: },
    { 0, 0 }
};
*/

class instance_grimrail_depot : public InstanceMapScript
{
public:                                                      

    instance_grimrail_depot() : InstanceMapScript("instance_grimrail_depot", 1208) { }

    struct instance_grimrail_depotAI : public InstanceScript
    {
        instance_grimrail_depotAI(Map* map) : InstanceScript(map) {}

        InstanceScript* m_Instance = this;

        uint64 m_RocketsparkGUID;
        uint64 m_BorkaGUID;
        uint64 m_SkylordTovraGUID;
        uint64 m_SkylordTovraDragonGUID;
        uint64 m_RocketsparkAndBurkaEntrance;
        uint64 m_RocketsparkAndBurkaExit;
        uint64 m_NitrogThunderPowerGUID;
        uint64 m_NitroggTurretGUID;
        uint64 m_NitroggAssaultFlapsGUID;
        uint64 m_NitroggInvisibleDoorGUID;
        uint64 m_SkylordTovraDoorGUID;

        void Initialize() override
        {
            SetBossNumber(3);
            //LoadScenariosInfos(g_BossScenarios, instance->IsChallengeMode() ? eGrimrailDepotScenarios::IronDocksChallengeID : eIronDocksScenario::IronDocksScenarioID);

            m_RocketsparkGUID             = 0;
            m_BorkaGUID                   = 0;
            m_NitrogThunderPowerGUID      = 0;
            m_SkylordTovraGUID            = 0;
            m_NitroggTurretGUID           = 0;
            m_NitroggAssaultFlapsGUID     = 0;
            m_NitroggInvisibleDoorGUID    = 0;
            m_SkylordTovraDoorGUID        = 0;
            m_RocketsparkAndBurkaEntrance = 0;
            m_RocketsparkAndBurkaExit     = 0;
        }

        void OnCreatureCreate(Creature* l_Creature) override
        {
            if (l_Creature)
            {
                switch (l_Creature->GetEntry())
                {
                    case GrimrailDepotBosses::BossRocketSpark:
                        m_RocketsparkGUID = l_Creature->GetGUID();
                        break;
                    case GrimrailDepotBosses::BossBorkatheBrute:
                        m_BorkaGUID = l_Creature->GetGUID();
                        break;
                    case GrimrailDepotBosses::BossNitroggThundertower:
                        m_NitrogThunderPowerGUID = l_Creature->GetGUID();
                        break;
                    case GrimrailDepotBosses::BossSkylordTovra:
                        m_SkylordTovraGUID = l_Creature->GetGUID();
                        break;
                    case GrimrailDepotCreatures::CreatureIronTurretNitrogg:
                        m_NitroggTurretGUID = l_Creature->GetGUID();
                        break;
                    case GrimrailDepotCreatures::CreatureSkylordTovraDragon:
                        m_SkylordTovraDragonGUID = l_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }
        }

        void OnUnitDeath(Unit* p_Unit) override
        {
            Creature* l_Creature = p_Unit->ToCreature();
            if (!l_Creature)
                return;

            switch (l_Creature->GetEntry())
            {
            case GrimrailDepotBosses::BossSkylordTovra:
            {
                if (l_Creature->GetMap()->IsHeroic())
                    DoCompleteAchievement(GrimrailDepotAchivement::AchievementGrimrailDepotHeroic);
                else
                    DoCompleteAchievement(GrimrailDepotAchivement::AchievementGrimrailDepotNormal);
                break;
            }
            default:
                break;
            }
        }

        void OnGameObjectCreate(GameObject* p_Go) override
        {
            switch (p_Go->GetEntry())
            {
            case GrimrailDepotGobjects::GameObjectIronWroughtGate:
                m_RocketsparkAndBurkaExit = p_Go->GetGUID();
                break;
            case GrimrailDepotGobjects::GameObjectSpikedGate:
                m_RocketsparkAndBurkaEntrance = p_Go->GetGUID();
                break;
            case GrimrailDepotGobjects::GameObjectAssaultFlaps:
                m_NitroggAssaultFlapsGUID = p_Go->GetGUID();
                break;
            case GrimrailDepotGobjects::GameObjectInvisibleDoor:
                m_NitroggInvisibleDoorGUID = p_Go->GetGUID();
                break;
            case GrimrailDepotGobjects::GameObjectSpikedGateSkylordTovra:
                m_SkylordTovraDoorGUID = p_Go->GetGUID();
                break;
            default:
                break;
            }
        }
   
        uint64 GetData64(uint32 p_Identifier) override
        {
            switch (p_Identifier)
            {
                case GrimrailDepotData::DataRocketspark:
                    return m_RocketsparkGUID;
                    break;
                case GrimrailDepotData::DataBorka:
                    return m_BorkaGUID;
                    break;
                case GrimrailDepotData::DataNitroggThundertower:
                    return m_NitrogThunderPowerGUID;
                    break;
                case GrimrailDepotData::DataSkyLordTovra:
                    return m_SkylordTovraGUID;
                    break;
                case GrimrailDepotData::DataNitroggTurret:
                    return m_NitroggTurretGUID;
                    break;
                case GrimrailDepotData::DataAssaultFlaps:
                    return m_NitroggAssaultFlapsGUID;
                    break;
                case GrimrailDepotData::DataInvisibleDoor:
                    return m_NitroggInvisibleDoorGUID;
                    break;
                case GrimrailDepotData::DataSkyLordTovraDragon:
                    return m_SkylordTovraDragonGUID;
                    break;
                case GrimrailDepotData::DataSpikedGateSkylordTovraDoor:
                    return m_SkylordTovraDoorGUID;
                    break;
                case GrimrailDepotData::DataSpikedGateBoss1:
                    return m_RocketsparkAndBurkaEntrance;
                    break;
                case GrimrailDepotData::DataIronWroughtGate:
                    return m_RocketsparkAndBurkaExit;
                    break;
                default:
                    break;
            }
            return 0;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_grimrail_depotAI(map);
    }

};

void AddSC_instance_grimrail_depot()
{
    new instance_grimrail_depot();
}
