////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "auchindon.hpp"
#include "Unit.h"
#include "GridNotifiers.h"

/// Event teleports player to Kaathar hall.
class EventTeleportPlayer : public BasicEvent
{
    public:
        explicit EventTeleportPlayer(Unit* p_Unit, int p_Value) : BasicEvent(), m_Obj(p_Unit), m_Modifier(p_Value)
        {
        }

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (m_Obj && m_Obj->IsPlayer())
            {
                    switch (m_Modifier)
                    {
                        case 101:
                        {
                            m_Obj->ToPlayer()->TeleportTo(1182, 1904.29f, 3185.111f, 30.799f, 3.34086f);
                            break;
                        }
                    }
            }

            return true;
        }

    private:
        Unit* m_Obj;
        int m_Modifier;
};

class instance_auchindon : public InstanceMapScript
{
    public:
        instance_auchindon()
            : InstanceMapScript("instance_auchindon", 1182)
        {
        }

        struct instance_auchindon_InstanceMapScript : public InstanceScript
        {
            instance_auchindon_InstanceMapScript(Map* map) : InstanceScript(map)
            {
                m_KaatharDied = false;
                m_TuulaniSummoned = true;
            }

            InstanceScript* m_Instance = this;

            uint32 m_auiEncounter[4];

            /// Creatures
            uint64 m_NyamiGuid;
            uint64 m_Tuulani02;
            uint64 m_UniqueGuardGuid;
            uint64 m_TuulaniGuid;
            uint64 m_WardenGuid;
            uint64 m_GromtashGuid;
            uint64 m_DuragGuid;
            uint64 m_GulkoshGuid;
            uint64 m_ElumGuid;
            uint64 m_IruunGuid;
            uint64 m_JoraGuid;

            /// Bosses
            uint64 m_KaatharGuid;
            uint64 m_NyamibossGuid;
            uint64 m_AzzakelGuid;
            uint64 m_TeronogorGuid;

            /// Objects
            uint64 m_HolyBarrierKathaarObjectGuid;
            uint64 m_CrystalKaatharGuid;
            uint64 m_WindowGuid;
            uint64 m_FelBarrierAzzakelObjectGuid;
            uint64 m_FelPortalGuid;
            uint64 m_SoulTransportStartGuid;
            uint64 m_SoulTransport01Guid;
            uint64 m_SoulTransport02Guid;
            uint64 m_SoulTransport03Guid;

            /// Triggers
            uint64 m_TriggerBubbleMiddleNyamiGuid;
            uint64 m_TriggerAzzakelFelPortalGuid;

            bool m_KaatharDied;
            bool m_TuulaniSummoned;

            /// Dispensor
            std::list<uint64> m_Dispensor;

            void Initialize() override
            {
                /// Creatures
                m_NyamiGuid                    = 0;
                m_Tuulani02                    = 0;
                m_UniqueGuardGuid              = 0;
                m_TuulaniGuid                  = 0;
                m_WardenGuid                   = 0;
                m_GromtashGuid                 = 0;
                m_DuragGuid                    = 0;
                m_GulkoshGuid                  = 0;
                m_ElumGuid                     = 0;
                m_IruunGuid                    = 0;
                m_JoraGuid                     = 0;

                /// Bosses
                m_KaatharGuid                  = 0;
                m_NyamibossGuid                = 0;
                m_AzzakelGuid                  = 0;
                m_TeronogorGuid                = 0;

                /// Objects
                m_HolyBarrierKathaarObjectGuid = 0;
                m_CrystalKaatharGuid           = 0;
                m_WindowGuid                   = 0;
                m_SoulTransportStartGuid       = 0;
                m_SoulTransport01Guid          = 0;
                m_SoulTransport02Guid          = 0;
                m_SoulTransport03Guid          = 0;

                /// Triggers
                m_TriggerBubbleMiddleNyamiGuid = 0;
            }

            void OnPlayerEnter(Player* p_Player) override
            {
                if (m_KaatharDied)
                {
                    p_Player->m_Events.AddEvent(new EventTeleportPlayer(p_Player, 101), p_Player->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                }

                if (m_TuulaniSummoned)
                {
                    m_TuulaniSummoned = false;
               
                    if (Creature* l_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossTeronogor)))
                    {
                        l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureSoulBinderTuulani, 1507.84f, 2953.38f, 35.238f, 6.254070f, TEMPSUMMON_MANUAL_DESPAWN);
                    }
                }
            }

            void OnGameObjectCreate(GameObject* go) override
            {
                switch (go->GetEntry())
                {
                    case eAuchindonObjects::GameobjectHolyBarrier:
                        m_HolyBarrierKathaarObjectGuid = go->GetGUID();
                            break;          
                    case eAuchindonObjects::GameobjectAuchindonWindow:
                        m_WindowGuid = go->GetGUID();
                            break;
                    case eAuchindonObjects::GameobjectFelBarrier:
                        m_FelBarrierAzzakelObjectGuid = go->GetGUID();
                            break;
                    case eAuchindonObjects::GameobjectSoulTransportStart:
                        m_SoulTransportStartGuid = go->GetGUID();
                            break;
                    case eAuchindonObjects::GameobjectSoulTransport1:
                        m_SoulTransport01Guid = go->GetGUID();
                            break;
                    case eAuchindonObjects::GameobjectSoulTransport2:
                        m_SoulTransport02Guid = go->GetGUID();
                            break;
                    case eAuchindonObjects::GameobjectSoulTransport3:
                        m_SoulTransport03Guid = go->GetGUID();
                            break;
                }
            }

            void OnCreatureCreate(Creature* p_Creature) override
            {
                switch (p_Creature->GetEntry())
                {
                    case eAuchindonCreatures::CreatureSoulBinderTuulani01:
                        m_Tuulani02 = p_Creature->GetGUID();
                        break;
                    case eAuchindonCreatures::CreatureIruun:
                        m_IruunGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindonCreatures::CreatureJoraa:
                        m_JoraGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindonCreatures::CreatureDurem:
                        m_ElumGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindonCreatures::CreatureAuchenaiDefenderUnique:
                        m_UniqueGuardGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindonBosses::BossKaathar:
                        m_KaatharGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonBosses::BossNyami:
                        m_NyamibossGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonBosses::BossAzaakel:
                        m_AzzakelGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonBosses::BossTeronogor:
                        m_TeronogorGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureSoulBinderTuulani:
                        m_TuulaniGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureSoulBinderNyami:
                        m_NyamiGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureShieldSpot:
                        m_TriggerBubbleMiddleNyamiGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureWardenAzzakael:
                        m_WardenGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureDemonsSummoner:
                        m_TriggerAzzakelFelPortalGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureGulkosh:
                        m_GulkoshGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureGromtashTheDestructor:
                        m_GromtashGuid = p_Creature->GetGUID();
                            break;
                    case eAuchindonCreatures::CreatureDuragTheDominator:
                        m_DuragGuid = p_Creature->GetGUID();
                            break;
                }
            }

            void OnUnitDeath(Unit* p_Unit) override
            {
                Creature* p_Creature = p_Unit->ToCreature();
                if (!p_Creature)
                    return;

                switch (p_Creature->GetEntry())
                {
                    case eAuchindonBosses::BossKaathar:
                        {
                            if (GameObject* l_Holybarrier = instance->GetGameObject(GetData64(DataHolyBarrier)))
                            {
                                l_Holybarrier->Delete();

                                m_KaatharDied = true;
                            }
                        }
                    case eAuchindonBosses::BossAzaakel:
                        {
                            if (GameObject* felbarrier = instance->GetGameObject(GetData64(DataFelBarrier)))
                            {
                                felbarrier->Delete();
                            }
                            if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(DataSoulTransportStart)))
                            {
                                if (Creature* l_Teronogor = instance->GetCreature(GetData64(DataBossTeronogor)))
                                {
                                    if (l_Teronogor->GetAI())
                                    l_Teronogor->GetAI()->DoAction(ActionSoulMove1);
                                }
                            }
                        }
                        break;
                        // Soul Transport
                     case eAuchindonCreatures::CreatureGromtashTheDestructor:
                         if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(DataSoulTransport3)))
                            {
                                if (Creature* l_Teronogor = instance->GetCreature(GetData64(DataBossTeronogor)))
                                {
                                    if (l_Teronogor->GetAI())
                                        l_Teronogor->GetAI()->DoAction(ActionSoulMove4);
                                }
                            }
                            break;
                     case eAuchindonCreatures::CreatureGulkosh:
                         if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(DataSoulTransport2)))
                            {
                                if (Creature* l_Teronogor = instance->GetCreature(GetData64(DataBossTeronogor)))
                                {
                                    if (l_Teronogor->GetAI())
                                    l_Teronogor->GetAI()->DoAction(ActionSoulMove3);
                                }
                            }
                            break;
                     case eAuchindonCreatures::CreatureDuragTheDominator:
                        {
                            if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(DataSoulTransport1)))
                            {
                                if (Creature* l_Teronogor = instance->GetCreature(GetData64(DataBossTeronogor)))
                                {
                                    if (l_Teronogor->GetAI())
                                    l_Teronogor->GetAI()->DoAction(ActionSoulMove2);
                                }
                            }
                            break;
                        }
                     case eAuchindonBosses::BossTeronogor:
                        {
                            if (p_Creature->GetMap() && p_Creature->GetMap()->IsHeroic())
                            {
                                DoCompleteAchievement(eAuchindonAchievements::AchievementAuchindonHeroic);
                            }
                            else
                            {
                                DoCompleteAchievement(eAuchindonAchievements::AchievementAuchindonNormal);
                            }

                            /// Curtain flames achievement, No Tags Backs! (9552)
                            UnitList l_Targets;
                            JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(p_Creature, p_Creature, 100, 153392, true);
                            JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(p_Creature, l_Targets, u_check);
                            p_Creature->VisitNearbyObject(100, searcher);

                            if (l_Targets.empty())
                                return;
                            else
                            {
                                DoCompleteAchievement(eAuchindonAchievements::AchievementNoTagBacks);
                            }

                            break;
                        }
                }
            }

            uint64 GetData64(uint32 p_Data) override
            {
                switch (p_Data)
                {
                    case eDataAuchindonDatas::DataTuulani02:
                        return m_Tuulani02;
                        break;
                    case eDataAuchindonDatas::DataGuard:
                        return m_UniqueGuardGuid;
                        break;
                    case eDataAuchindonDatas::DataElum:
                        return m_ElumGuid;
                        break;
                    case eDataAuchindonDatas::DataIruun:
                        return m_IruunGuid;
                        break;
                    case eDataAuchindonDatas::DataJorra:
                        return m_JoraGuid;
                        break;
                    case eDataAuchindonDatas::DataSoulTransportStart:
                        return m_SoulTransportStartGuid;
                        break;
                    case eDataAuchindonDatas::DataSoulTransport1:
                        return m_SoulTransport01Guid;
                        break;
                    case eDataAuchindonDatas::DataSoulTransport2:
                        return m_SoulTransport02Guid;
                        break;
                    case eDataAuchindonDatas::DataSoulTransport3:
                        return m_SoulTransport03Guid;
                        break;
                    case eDataAuchindonDatas::DataHolyBarrier:
                        return m_HolyBarrierKathaarObjectGuid;
                        break;
                    case eDataAuchindonDatas::DataAuchindonWindow:
                        return m_WindowGuid;
                        break;
                    case eDataAuchindonDatas::DataFelBarrier:
                        return m_FelBarrierAzzakelObjectGuid;
                        break;
                    case eDataAuchindonDatas::DataFelPortal:
                        return m_FelPortalGuid;
                        break;
                    case eDataAuchindonDatas::DataBossKathaar:
                        return m_KaatharGuid;
                        break;
                    case eDataAuchindonDatas::DataBossAzzakael:
                        return m_AzzakelGuid;
                        break;
                    case eDataAuchindonDatas::DataBossNyami:
                        return m_NyamibossGuid;
                        break;
                    case eDataAuchindonDatas::DataBossTeronogor:
                        return m_TeronogorGuid;
                        break;
                    case eDataAuchindonDatas::DataNyami:
                        return m_NyamiGuid;
                        break;
                    case eDataAuchindonDatas::DataTuulani:
                        return m_TuulaniGuid;
                        break;
                    case eDataAuchindonDatas::DataWarden:
                        return m_WardenGuid;
                        break;
                    case eDataAuchindonDatas::DataGulkosh:
                        return m_GulkoshGuid;
                        break;
                    case eDataAuchindonDatas::DataGromtash:
                        return m_GromtashGuid;
                        break;
                    case eDataAuchindonDatas::DataDurag:
                        return m_DuragGuid;
                        break;
                    case eDataAuchindonDatas::DataTriggerMiddleNyamiFightBubble:
                        return m_TriggerBubbleMiddleNyamiGuid;
                        break;
                    case eDataAuchindonDatas::DataTriggerAzzakelController:
                        return m_TriggerAzzakelFelPortalGuid;
                        break;
                }
                return 0;
            }
        };

        InstanceScript* GetInstanceScript(InstanceMap* map) const override
        {
            return new instance_auchindon_InstanceMapScript(map);
        }
};

#ifndef __clang_analyzer__
void AddSC_instance_auchindon()
{
    new instance_auchindon;
}
#endif
