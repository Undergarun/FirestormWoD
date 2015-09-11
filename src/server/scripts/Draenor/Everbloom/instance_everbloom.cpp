////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "the_everbloom.hpp"

#define EverbloomMaxBosses 5

class instance_everbloom : public InstanceMapScript
{
public:
    instance_everbloom() : InstanceMapScript("instance_everbloom", 1279) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_everbloom_instancemapscript(map);
    }

    struct instance_everbloom_instancemapscript : public InstanceScript
    {
        instance_everbloom_instancemapscript(Map* map) : InstanceScript(map) {}

        InstanceScript* m_Instance = this;

        uint64 m_TaluGUID;
        uint64 m_GolaGUID;
        uint64 m_DulhuGUID;
        uint64 m_WitherbarkGUID;
        uint64 m_ArchmagesolGUID;
        uint64 m_XeritacGUID;
        uint64 m_PreYalnuGUID;
        uint64 m_YalnuGUID;
        uint64 m_PostTeleportTriggerGUID;
        uint64 m_MageTriggerGUID;
        uint32 m_CountGUID;

        // objects
        uint64 m_YalnuDoorGUID;
        uint64 m_WebDoorGUID;

        void Initialize() override
        {
            SetBossNumber(EverbloomMaxBosses); // excluding optimal xeritac

            // Creatures
            m_TaluGUID = 0;
            m_GolaGUID = 0;
            m_DulhuGUID = 0;
            m_WitherbarkGUID = 0;
            m_ArchmagesolGUID = 0;
            m_XeritacGUID = 0;
            m_PreYalnuGUID = 0;
            m_YalnuGUID = 0;
            m_PostTeleportTriggerGUID = 0;
            m_MageTriggerGUID = 0;
            m_MageTriggerGUID = 0;

            // Objects
            m_YalnuDoorGUID = 0;
            m_WebDoorGUID = 0;

            instance->SetObjectVisibility(1000.0f);
        }

        void OnCreatureCreate(Creature* l_Creature) override
        {
            switch (l_Creature->GetEntry())
            {
                case eEverbloomBosses::BossWitherbark:
                    m_WitherbarkGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossEarthshaperTelu:
                    m_TaluGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossLifeWardenGola:
                    m_GolaGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossDulhu:
                    m_DulhuGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossArchmageSol:
                    m_ArchmagesolGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossXeritac:
                    m_XeritacGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossYalnu:
                    m_YalnuGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomCreature::CreatureRpYalnu:
                    m_PreYalnuGUID = l_Creature->GetGUID();
                    break;
                case eEverbloomCreature::CreatureUndermageKeasel:
                    m_MageTriggerGUID = l_Creature->GetGUID();
                    break;
            }
        }
        void OnUnitDeath(Unit* p_Unit) override
        {
            Creature* l_Creature = p_Unit->ToCreature();
            if (!l_Creature)
                return;

            switch (l_Creature->GetEntry())
            {
                case eEverbloomBosses::BossWitherbark:
                case eEverbloomBosses::BossArchmageSol:
                case eEverbloomBosses::BossXeritac:
                    m_CountGUID++;
                    
                    if (m_CountGUID >= 2 ? !l_Creature->GetMap()->IsHeroic() : 3)
                    {
                        if (Creature* l_Trigger = instance->GetCreature(GetData64(eEverbloomData::DataRpMage)))
                        {
                            if (l_Trigger->GetAI())
                                l_Trigger->AI()->DoAction(eEverbloomActions::ActionYalnuEvent);
                        }
                    }
                    break;
                case eEverbloomBosses::BossYalnu:
                    if (l_Creature->GetMap()->IsHeroic())
                    {
                        m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchivementTheEverbloomHeroic);
                    }
                    else
                    {
                        m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchievementTheEverbloomNormal);
                    }
                    break;
            }
        }

        void OnGameObjectCreate(GameObject* p_Go) override
        { 
            switch (p_Go->GetEntry())
            {
            case eEverbloomData::DataObjectWebDoor:
                m_WebDoorGUID = p_Go->GetGUID();
                break;
            }
        }

        uint64 GetData64(uint32 p_Identifier) override
        {
            switch (p_Identifier)
            {
                case eEverbloomData::DataEarthshaperTelu:
                    return m_TaluGUID;
                    break;
                case eEverbloomData::DataLifeWardenGola:
                    return m_GolaGUID;
                    break;
                case eEverbloomData::DataDulhu:
                    return m_DulhuGUID;
                    break;
                case eEverbloomData::DataWitherbark:
                    return m_WitherbarkGUID;
                    break;
                case eEverbloomData::DataArchmageSol:
                    return m_ArchmagesolGUID;
                    break;
                case eEverbloomData::DataXeritac:
                    return m_XeritacGUID;
                    break;
                case eEverbloomData::DataYalnu:
                    return m_YalnuGUID;
                    break;
                case eEverbloomData::DataRpMage:
                    return m_MageTriggerGUID;
                    break;
                case eEverbloomData::DataRpYalnu:
                    return m_PreYalnuGUID;
                    break;
                
                    // Objects
                case eEverbloomData::DataObjectWebDoor:
                    return m_WebDoorGUID;
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
