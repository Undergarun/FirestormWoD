////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "shadowmoon_burial_grounds.hpp"

#define ShadowlandNpcDisplayId 58948

class instance_shadowmoon_burial_grounds : public InstanceMapScript
{
public:

    instance_shadowmoon_burial_grounds() : InstanceMapScript("instance_shadowmoon_burial_grounds", 1176) { }

    InstanceScript* GetInstanceScript(InstanceMap* p_Map) const
    {
        return new instance_everbloom_instancemapscript(p_Map);
    }

    struct instance_everbloom_instancemapscript : public InstanceScript
    {
        instance_everbloom_instancemapscript(Map* p_Map) : InstanceScript(p_Map) {}

        InstanceScript* m_Instance = this;

        uint64 m_SadanaGUID;
        uint64 m_SadanaEclipseTriggerGUID;
        uint64 m_NhallishGUID;
        uint64 m_BonemawGUID;
        uint64 m_BonemawMouthGUID;
        uint64 m_NerzhulGUID;
        uint64 m_NerzulPropGUID;
		uint64 m_PortalToTheShadowLandReturnGUID;
        uint64 m_WandererFirstTalkGUID;
        uint64 m_WandererSecondTalkGUID;
        uint64 m_WandererThirdTalkGUID; 
        uint64 m_DoorGobjectSadanaGUID;
        uint64 m_DoorGobjectNerzulGUID;
        uint64 m_DoorGobjectSadanaFightDoorGUID;
        uint64 m_DoorGobjectNhalishEntranceGUID;
        uint64 m_DoorGobjectBonemawEntranceGUID;    

        void Initialize() override
        {
            m_SadanaGUID                      = 0;
            m_NhallishGUID                    = 0;
            m_BonemawGUID                     = 0;
            m_NerzhulGUID                     = 0;
            m_BonemawMouthGUID                = 0;
            m_NerzulPropGUID                  = 0;
            m_WandererFirstTalkGUID           = 0;
            m_WandererSecondTalkGUID          = 0;
            m_WandererThirdTalkGUID           = 0;
            m_DoorGobjectSadanaGUID           = 0;
            m_DoorGobjectSadanaFightDoorGUID  = 0;
            m_DoorGobjectNerzulGUID           = 0;
            m_DoorGobjectBonemawEntranceGUID  = 0;
            m_DoorGobjectNhalishEntranceGUID  = 0;
            m_SadanaEclipseTriggerGUID        = 0;
			m_PortalToTheShadowLandReturnGUID = 0;
        }

        void OnCreatureCreate(Creature* l_Creature) override
        {
            if (l_Creature)
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
					case eShadowmoonBurialGroundsCreatures::CreatureBonemawMouth:
						m_BonemawMouthGUID = l_Creature->GetGUID();
						break;
					case eShadowmoonBurialGroundsBosses::BossNerzul:
						m_NerzhulGUID = l_Creature->GetGUID();
						break;
					case eShadowmoonBurialGroundsCreatures::CreatureNerzulVisual:
						m_NerzulPropGUID = l_Creature->GetGUID();
						break;
					case eShadowmoonBurialGroundsCreatures::CreatureNewDarkEclipse:
						m_SadanaEclipseTriggerGUID = l_Creature->GetGUID();
						break;
					case eShadowmoonBurialGroundsDatas::DataPortalToTheShadowlandReturn:
						m_PortalToTheShadowLandReturnGUID = l_Creature->GetGUID();
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
                case eShadowmoonBurialGroundsBosses::BossNerzul:
                {
					if (m_Instance != nullptr)
					{
						if (Creature* l_ShadowlandPortalReturn = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataPortalToTheShadowlandReturn)))
						{
							l_ShadowlandPortalReturn->SetDisplayId(ShadowlandNpcDisplayId);
							l_ShadowlandPortalReturn->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);					
						}
					}

                    /// Dungeon Achievement
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

        void OnGameObjectCreate(GameObject* p_Go) override
        {
            if (p_Go)
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
					case eShadowmoonBurialGroundsGameObjects::GameObjectNhalishDoorEntrance:
						m_DoorGobjectNhalishEntranceGUID = p_Go->GetGUID();
						break;
					case eShadowmoonBurialGroundsGameObjects::GameObjectBonemawDoorEntrance:
						m_DoorGobjectBonemawEntranceGUID = p_Go->GetGUID();
						break;
					default:
						break;
                }
            }
        }
      
        void SetData(uint32 p_Type, uint32 p_Data) override
        {
            switch (p_Type)
            {
                case eShadowmoonBurialGroundsDatas::DataWandererFirstTalk:
                    m_WandererFirstTalkGUID = p_Data;
                    break;
                case eShadowmoonBurialGroundsDatas::DataWandererSecondTalk:
                    m_WandererSecondTalkGUID = p_Data;
                    break;
                case eShadowmoonBurialGroundsDatas::DataWandererThirdTalk:
                    m_WandererThirdTalkGUID = p_Data;
                    break;
                default:
                    break;
            }
        }
        
        uint64 GetData64(uint32 p_Identifier) override
        {
            switch (p_Identifier)
            {
                case eShadowmoonBurialGroundsDatas::DataBossSadana:
                    return m_SadanaGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataSadanaEclipseTrigger:
                    return m_SadanaEclipseTriggerGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBossNhallish:
                    return m_NhallishGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBossBonemaw:
                    return m_BonemawGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBonemawMouth:
                    return m_BonemawMouthGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBossNerzul:
                    return m_NerzhulGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataWandererFirstTalk:
                    return m_WandererFirstTalkGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataWandererSecondTalk:
                    return m_WandererSecondTalkGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataWandererThirdTalk:
                    return m_WandererThirdTalkGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataStoneDoorSadana:
                    return m_DoorGobjectSadanaGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataNerzulDoor:
                    return m_DoorGobjectNerzulGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataSadanaFightDoor:
                    return m_DoorGobjectSadanaFightDoorGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataNhalishDoorEntrance:
                    return m_DoorGobjectNhalishEntranceGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataBonemawDoorEntrance:
                    return m_DoorGobjectBonemawEntranceGUID;
                    break;
                case eShadowmoonBurialGroundsDatas::DataNerzulProp:
                    return m_NerzulPropGUID;
                    break;
				case eShadowmoonBurialGroundsDatas::DataPortalToTheShadowlandReturn:
					return m_PortalToTheShadowLandReturnGUID;
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
