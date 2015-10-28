////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "shadowmoon_burial_grounds.hpp"

class instance_shadowmoon_burial_grounds : public InstanceMapScript
{
public:
    instance_shadowmoon_burial_grounds() : InstanceMapScript("instance_shadowmoon_burial_grounds", 1176) { }

    InstanceScript* GetInstanceScript(InstanceMap* map) const
    {
        return new instance_everbloom_instancemapscript(map);
    }

    struct instance_everbloom_instancemapscript : public InstanceScript
    {
        instance_everbloom_instancemapscript(Map* map) : InstanceScript(map) {}

        InstanceScript* m_Instance = this;
        uint64 m_SadanaGUID;
        uint64 m_NhallishGUID;
        uint64 m_BonemawGUID;
        uint64 m_NerzhulGUID;
        uint64 m_WandererFirstTalkGUID;
        uint64 m_DoorGobjectSadanaGUID;
        uint64 m_DoorGobjectNerzulGUID;
        uint64 m_DoorGobjectSadanaFightDoorGUID;
        uint64 m_NerzulPropGUID;

        void Initialize() override
        {
            // Creatures
            m_SadanaGUID = 0;
            m_NhallishGUID = 0;
            m_BonemawGUID = 0;
            m_NerzhulGUID = 0;
            m_NerzulPropGUID = 0;
            m_WandererFirstTalkGUID = 0;

            // Doors
            m_DoorGobjectSadanaGUID = 0;
            m_DoorGobjectNerzulGUID = 0;
            m_DoorGobjectSadanaFightDoorGUID = 0;
        }

        void OnCreatureCreate(Creature* l_Creature) override
        {
            switch (l_Creature->GetEntry())
            {
                case eShadowmoonBurialGroundsBosses::BossSadana:
                    m_SadanaGUID = l_Creature->GetGUID();
                    break;
                case eShadowmoonBurialGroundsBosses::BossNhallish:
                    m_NhallishGUID = l_Creature->GetGUID();
                    break;
                case eShadowmoonBurialGroundsBosses::BossBoneMaw:
                    m_BonemawGUID = l_Creature->GetGUID();
                    break;
                case eShadowmoonBurialGroundsBosses::BossNerzul:
                    m_NerzhulGUID = l_Creature->GetGUID();
                    break;
                case eShadowmoonBurialGroundsCreatures::CreatureNerzulVisual:
                    m_NerzulPropGUID = l_Creature->GetGUID();
                    break;
                default:
                    break;
            }
        }

        void OnUnitDeath(Unit* p_Unit) override
        {
            if (Creature* l_Creature = p_Unit->ToCreature())
            {
                switch (l_Creature->GetEntry())
                {
                    case eShadowmoonBurialGroundsBosses::BossNerzul:
                    {
                        ///< Dungeon Achievement
                        if (p_Unit->GetMap()->IsHeroic())
                            this->DoCompleteAchievement(eShadowmoonBurialGroundsAchivement::AchievementShadowmoonBurialGroundsHeroic);
                        else
                            this->DoCompleteAchievement(eShadowmoonBurialGroundsAchivement::AchievementShadowmonBurialGroundsNormal);
                        break;
                    }
                    default:
                        break;
                }
            }
        }

        void OnGameObjectCreate(GameObject* p_Go) override
        {
            switch (p_Go->GetEntry())
            {
                case eShadowmoonBurialGroundsGameObjects::GameObjectSadanaDoor:
                    m_DoorGobjectSadanaGUID = p_Go->GetGUID();
                    break;
                case eShadowmoonBurialGroundsGameObjects::GameObjectSadanaFightDoor:
                    m_DoorGobjectSadanaFightDoorGUID = p_Go->GetGUID();
                    break;
                case eShadowmoonBurialGroundsGameObjects::GameObjectNerzulDoor:
                    m_DoorGobjectNerzulGUID = p_Go->GetGUID();
                   break;            
                default:
                    break;
            }
        }

        void SetData(uint32 p_Type, uint32 p_Data) override
        {
            switch (p_Type)
            {
                case eShadowmoonBurialGroundsDatas::DataWandererTalk:
                    if (m_WandererFirstTalkGUID <= 4)
                        m_WandererFirstTalkGUID++;
                    break;
                default:
                    break;
            }
        }

        uint64 GetData64(uint32 p_Identifier) override
        {
            switch (p_Identifier)
            {
                 ///< Creatures
                case eShadowmoonBurialGroundsDatas::DataBossSadana:
                    return m_SadanaGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBossNhallish:
                    return m_NhallishGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBossBonemaw:
                    return m_BonemawGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBossNerzul:
                    return m_NerzhulGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataWandererTalk:
                    return m_WandererFirstTalkGUID;
                    break;

                 ///< Doors
                case eShadowmoonBurialGroundsDatas::DataStoneDoorSadana:
                    return m_DoorGobjectSadanaGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataNerzulDoor:
                    return m_DoorGobjectNerzulGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataSadanaFightDoor:
                    return m_DoorGobjectSadanaFightDoorGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataNerzulProp:
                    return m_NerzulPropGUID;
                    break;
                default:
                    break;
            }
            return 0;
        }
    };
};

void AddSC_instance_shadowmoon_burial_grounds()
{
    new instance_shadowmoon_burial_grounds();
}
