////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "the_everbloom.hpp"

class instance_everbloom : public InstanceMapScript
{
public:

    instance_everbloom() : InstanceMapScript("instance_everbloom", 1279) { }

    InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
    {
        return new instance_everbloom_instancemapscript(p_Map);
    }

    struct instance_everbloom_instancemapscript : public InstanceScript
    {
        instance_everbloom_instancemapscript(Map* p_Map) : InstanceScript(p_Map) {}

        InstanceScript* m_Instance = this;
        uint32 m_CountGuid;
        uint64 m_TaluGuid;
        uint64 m_GolaGuid;
        uint64 m_DulhuGuid;
        uint64 m_WitherbarkGuid;
        uint64 m_ArchmagesolGuid;
        uint64 m_XeritacGuid;
        uint64 m_PreYalnuGuid;
        uint64 m_YalnuGuid;
        uint64 m_PostTeleportTriggerGuid;
        uint64 m_MageTriggerGuid;
        uint64 m_YalnuDoorGuid;
        uint64 m_WebDoorGuid;

        void Initialize() override
        {
            if (instance && instance->IsHeroic())
            SetBossNumber(5); /// Including optimal xeritac
            else
            SetBossNumber(4); /// Excluding optimal xeritac
            /// Creatures
            m_TaluGuid = 0;
            m_GolaGuid = 0;
            m_DulhuGuid = 0;
            m_WitherbarkGuid = 0;
            m_ArchmagesolGuid = 0;
            m_XeritacGuid = 0;
            m_PreYalnuGuid = 0;
            m_YalnuGuid = 0;
            m_PostTeleportTriggerGuid = 0;
            m_MageTriggerGuid = 0;
            m_MageTriggerGuid = 0;
            m_CountGuid = 0;
            /// Objects
            m_YalnuDoorGuid = 0;
            m_WebDoorGuid = 0;
            instance->SetObjectVisibility(1000.0f);
        }

        void OnCreatureCreate(Creature* p_Creature) override
        {
            switch (p_Creature->GetEntry())
            {
                case eEverbloomBosses::BossWitherbark:
                    m_WitherbarkGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossEarthshaperTelu:
                    m_TaluGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossLifeWardenGola:
                    m_GolaGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossDulhu:
                    m_DulhuGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossArchmageSol:
                    m_ArchmagesolGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossXeritac:
                    m_XeritacGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossYalnu:
                    m_YalnuGuid = p_Creature->GetGUID();
                    break;
                case eEverbloomCreature::CreatureUndermageKeasel:
                    m_MageTriggerGuid = p_Creature->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnUnitDeath(Unit* p_Unit) override
        {
            if (!p_Unit )
                return;

            if (m_Instance == nullptr)
                return;

            if (Creature* l_Creature = p_Unit->ToCreature())
            {
                switch (l_Creature->GetEntry())
                {
                    case eEverbloomBosses::BossWitherbark:
                    case eEverbloomBosses::BossArchmageSol:
                    case eEverbloomBosses::BossXeritac:
                        m_CountGuid++;
                        if (m_CountGuid >= 2 ? !m_Instance->instance->IsHeroic() : 3)
                        {
                            if (Creature* l_Trigger = instance->GetCreature(GetData64(eEverbloomData::DataRpMage)))
                            {
                                if (l_Trigger->IsAIEnabled)
                                    l_Trigger->AI()->DoAction(eEverbloomActions::ActionYalnuEvent);
                            }
                        }
                        break;
                    case eEverbloomBosses::BossYalnu:
                        if (m_Instance->instance->IsHeroic())
                            m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchivementTheEverbloomHeroic);
                        else
                            m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchievementTheEverbloomNormal);
                        break;
                    default:
                        break;
                }
            }
        }

        void OnGameObjectCreate(GameObject* p_Go) override
        { 
            switch (p_Go->GetEntry())
            {
                case eEverbloomGameObjects::ObjectWebDoor:
                        m_WebDoorGuid = p_Go->GetGUID();
                        break;
                    default:
                        break;
            }
        }

        uint64 GetData64(uint32 p_Identifier) override
        {
            switch (p_Identifier)
            {
                case eEverbloomData::DataEarthshaperTelu:
                    return m_TaluGuid;
                    break;
                case eEverbloomData::DataLifeWardenGola:
                    return m_GolaGuid;
                    break;
                case eEverbloomData::DataDulhu:
                    return m_DulhuGuid;
                    break;
                case eEverbloomData::DataWitherbark:
                    return m_WitherbarkGuid;
                    break;
                case eEverbloomData::DataArchmageSol:
                    return m_ArchmagesolGuid;
                    break;
                case eEverbloomData::DataXeritac:
                    return m_XeritacGuid;
                    break;
                case eEverbloomData::DataYalnu:
                    return m_YalnuGuid;
                    break;
                case eEverbloomData::DataRpMage:
                    return m_MageTriggerGuid;
                    break;       
                case eEverbloomData::DataObjectWebDoor:
                    return m_WebDoorGuid;
                    break;
                default:
                    break;
            }
            return 0;
        }
    };
};

void AddSC_instance_everbloom()
{
    new instance_everbloom();
}
