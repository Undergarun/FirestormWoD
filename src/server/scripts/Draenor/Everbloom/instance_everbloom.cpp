////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "the_everbloom.hpp"

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

        uint64 Talu;
        uint64 Gola;
        uint64 Dulhu;
        uint64 Witherbark;
        uint64 Archmagesol;
        uint64 Xeritac;
        uint64 PreYalnu;
        uint64 Yalnu;
        uint64 PostTeleportTrigger;
        uint64 MageTrigger;
        uint32 count;

        // objects
        uint64 YalnuDoor;
        uint64 WebDoor;

        void Initialize() override
        {
            SetBossNumber(4); // excluding optimal xeritac

            // Creatures
            Talu = 0;
            Gola = 0;
            Witherbark = 0;
            Dulhu = 0;
            Archmagesol = 0;
            Xeritac = 0;
            PreYalnu = 0;
            Yalnu = 0;
            PostTeleportTrigger = 0;
            MageTrigger = 0;
            count = 0;

            // Objects
            YalnuDoor = 0;
            WebDoor = 0;

            instance->SetObjectVisibility(1000.0f);
        }

        void OnCreatureCreate(Creature* l_Creature) override
        {
            switch (l_Creature->GetEntry())
            {
                case eEverbloomBosses::BossWitherbark:
                    Witherbark = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossEarthshaperTelu:
                    Talu = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossLifeWardenGola:
                    Gola = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossDulhu:
                    Dulhu = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossArchmageSol:
                    Archmagesol = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossXeritac:
                    Xeritac = l_Creature->GetGUID();
                    break;
                case eEverbloomBosses::BossYalnu:
                    Yalnu = l_Creature->GetGUID();
                    break;
                case eEverbloomCreature::CreatureRpYalnu:
                    PreYalnu = l_Creature->GetGUID();
                    break;
                case eEverbloomCreature::CreatureUndermageKeasel:
                    MageTrigger = l_Creature->GetGUID();
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
                    count++;
                    
                    if (count >= 2 ? !l_Creature->GetMap()->IsHeroic() : 3)
                        if (Creature* RPmageTrigger = instance->GetCreature(GetData64(eEverbloomData::DataRpMage)))
                        {
                            if (RPmageTrigger->GetAI())
                            RPmageTrigger->AI()->DoAction(eEverbloomActions::ActionYalnuEvent);
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
                WebDoor = p_Go->GetGUID();
                break;
            }
        }

        uint64 GetData64(uint32 p_Identifier) override
        {
            switch (p_Identifier)
            {
                case eEverbloomData::DataEarthshaperTelu:
                    return Talu;
                    break;
                case eEverbloomData::DataLifeWardenGola:
                    return Gola;
                    break;
                case eEverbloomData::DataDulhu:
                    return Dulhu;
                    break;
                case eEverbloomData::DataWitherbark:
                    return Witherbark;
                    break;
                case eEverbloomData::DataArchmageSol:
                    return Archmagesol;
                    break;
                case eEverbloomData::DataXeritac:
                    return Xeritac;
                    break;
                case eEverbloomData::DataYalnu:
                    return Yalnu;
                    break;
                case eEverbloomData::DataRpMage:
                    return MageTrigger;
                    break;
                case eEverbloomData::DataRpYalnu:
                    return PreYalnu;
                    break;
                
                    // Objects
                case eEverbloomData::DataObjectWebDoor:
                    return WebDoor;
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
