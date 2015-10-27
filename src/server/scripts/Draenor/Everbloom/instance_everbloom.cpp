////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "the_everbloom.hpp"

class instance_everbloom : public InstanceMapScript
{
    public:
        instance_everbloom() : InstanceMapScript("instance_everbloom", 1279) { }

        struct instance_everbloom_instancemapscript : public InstanceScript
        {
            instance_everbloom_instancemapscript(Map* p_Map) : InstanceScript(p_Map) { }

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

            /// Objects
            uint64 m_YalnuDoorGUID;
            uint64 m_WebDoorGUID;

            void Initialize() override
            {
                SetBossNumber(eEverbloomDatas::DataMaxBosses); ///< Excluding optional xeritac

                /// Creatures
                m_TaluGUID                  = 0;
                m_GolaGUID                  = 0;
                m_DulhuGUID                 = 0;
                m_WitherbarkGUID            = 0;
                m_ArchmagesolGUID           = 0;
                m_XeritacGUID               = 0;
                m_PreYalnuGUID              = 0;
                m_YalnuGUID                 = 0;
                m_PostTeleportTriggerGUID   = 0;
                m_MageTriggerGUID           = 0;
                m_MageTriggerGUID           = 0;

                m_CountGUID                 = 0;

                /// Objects
                m_YalnuDoorGUID             = 0;
                m_WebDoorGUID               = 0;

                instance->SetObjectVisibility(1000.0f);
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eEverbloomBosses::BossWitherbark:
                        m_WitherbarkGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomBosses::BossEarthshaperTelu:
                        m_TaluGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomBosses::BossLifeWardenGola:
                        m_GolaGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomBosses::BossDulhu:
                        m_DulhuGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomBosses::BossArchmageSol:
                        m_ArchmagesolGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomBosses::BossXeritac:
                        m_XeritacGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomBosses::BossYalnu:
                        m_YalnuGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomCreatures::CreatureRpYalnu:
                        m_PreYalnuGUID = p_Creature->GetGUID();
                        break;
                    case eEverbloomCreatures::CreatureUndermageKeasel:
                        m_MageTriggerGUID = p_Creature->GetGUID();
                        break;
                    default:
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
                    {
                        m_CountGUID++;

                        if (m_CountGUID >= 2 ? !l_Creature->GetMap()->IsHeroic() : 3)
                        {
                            if (Creature* l_Trigger = instance->GetCreature(m_MageTriggerGUID))
                            {
                                if (l_Trigger->IsAIEnabled)
                                    l_Trigger->AI()->DoAction(eEverbloomActions::ActionYalnuEvent);
                            }
                        }

                        break;
                    }
                    case eEverbloomBosses::BossYalnu:
                    {
                        if (l_Creature->GetMap()->IsHeroic())
                            DoCompleteAchievement(eEverbloomAchievements::AchivementTheEverbloomHeroic);
                        else
                            DoCompleteAchievement(eEverbloomAchievements::AchievementTheEverbloomNormal);

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
                    case eEverbloomGameObjects::ObjectWebDoor:
                        m_WebDoorGUID = p_GameObject->GetGUID();
                        break;
                    default:
                        break;
                }
            }

            uint64 GetData64(uint32 p_Identifier) override
            {
                switch (p_Identifier)
                {
                    case eEverbloomBosses::BossEarthshaperTelu:
                        return m_TaluGUID;
                    case eEverbloomBosses::BossLifeWardenGola:
                        return m_GolaGUID;
                    case eEverbloomBosses::BossDulhu:
                        return m_DulhuGUID;
                    case eEverbloomBosses::BossWitherbark:
                        return m_WitherbarkGUID;
                    case eEverbloomBosses::BossArchmageSol:
                        return m_ArchmagesolGUID;
                    case eEverbloomBosses::BossXeritac:
                        return m_XeritacGUID;
                    case eEverbloomBosses::BossYalnu:
                        return m_YalnuGUID;
                    case eEverbloomCreatures::CreatureUndermageKeasel:
                        return m_MageTriggerGUID;
                    case eEverbloomCreatures::CreatureRpYalnu:
                        return m_PreYalnuGUID;
                    case eEverbloomGameObjects::ObjectWebDoor:
                        return m_WebDoorGUID;
                    default:
                        break;
                }

                return 0;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* p_Map) const override
        {
            return new instance_everbloom_instancemapscript(p_Map);
        }
};

void AddSC_instance_everbloom()
{
    new instance_everbloom();
}