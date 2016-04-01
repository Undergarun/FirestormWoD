////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "grimrail_depot.hpp"

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
        uint64 m_NitrogThunderPowerGUID;
        uint64 m_SkylordTovraGUID;

        void Initialize() override
        {
            SetBossNumber(3);
            m_RocketsparkGUID        = 0;
            m_BorkaGUID              = 0;
            m_NitrogThunderPowerGUID = 0;
            m_SkylordTovraGUID       = 0;
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
                    default:
                        break;
                }
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
