////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "auchindoun.hpp"

static BossScenarios const g_BossScenarios[] =
{
    { eAuchindounDatas::DataBossKathaar,    eAuchindounChallengeDatas::KaatharCriteriaID },
    { eAuchindounDatas::DataBossNyami,      eAuchindounChallengeDatas::NyamiCriteriaID },
    { eAuchindounDatas::DataBossAzzakael,   eAuchindounChallengeDatas::AzaakelCriteriaID },
    { eAuchindounDatas::DataBossTeronogor,  eAuchindounChallengeDatas::TerongorCriteriaID },
    { 0,                                    0 }
};

class instance_auchindoun : public InstanceMapScript
{
public:

    instance_auchindoun()
        : InstanceMapScript("instance_auchindoun", 1182)
    {
    }

    struct instance_auchindoun_InstanceMapScript : public InstanceScript
    {
        instance_auchindoun_InstanceMapScript(Map* p_Map) : InstanceScript(p_Map)
        {
            m_KaatharDied = false;
            m_TuulaniSummoned = true;
        }

        InstanceScript* m_Instance = this;
        uint32 m_auiEncounter[4];
        /// Creatures
        uint64 m_NyamiGuid;
        uint64 m_Tuulani02Guid;
        uint64 m_UniqueGuardGuid;
        uint64 m_TuulaniGuid;
        uint64 m_WardenGuid;
        uint64 m_GromtashGuid;
        uint64 m_DuragGuid;
        uint64 m_GulkoshGuid;
        uint64 m_ElumGuid;
        uint64 m_IruunGuid;
        uint64 m_JoraGuid;
        uint64 m_AssainatingGuardGuid;
        uint64 m_AssainatedGuardGuid;
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
		uint64 m_AuchindounCrystal;
        /// Triggers
        uint64 m_TriggerBubbleMiddleNyamiGuid;
        uint64 m_TriggerAzzakelFelPortalGuid;
        bool m_KaatharDied;
        bool m_TuulaniSummoned;

        void Initialize() override
        {           
            /// Creatures
            m_NyamiGuid                       = 0;
            m_Tuulani02Guid                   = 0;
            m_UniqueGuardGuid                 = 0;
            m_TuulaniGuid                     = 0;
            m_WardenGuid                      = 0;
            m_GromtashGuid                    = 0;
            m_DuragGuid                       = 0;
            m_GulkoshGuid                     = 0;
            m_ElumGuid                        = 0;
            m_IruunGuid                       = 0;
            m_JoraGuid                        = 0;
            m_AssainatingGuardGuid            = 0;
            m_AssainatedGuardGuid             = 0;
            /// Bosses
            m_KaatharGuid                     = 0;
            m_NyamibossGuid                   = 0;
            m_AzzakelGuid                     = 0;
            m_TeronogorGuid                   = 0;
            /// Objects
            m_HolyBarrierKathaarObjectGuid    = 0;
            m_CrystalKaatharGuid              = 0;
            m_WindowGuid                      = 0;
            m_SoulTransportStartGuid          = 0;
            m_SoulTransport01Guid             = 0;
            m_SoulTransport02Guid             = 0;
            m_SoulTransport03Guid             = 0;
			m_AuchindounCrystal				  = 0;
            /// Triggers
            m_TriggerBubbleMiddleNyamiGuid    = 0;
			m_KaatharDied					  = false;

            instance->SetObjectVisibility(150.0f);

            SetBossNumber(eAuchindounDatas::DataMaxBosses);

            LoadScenariosInfos(g_BossScenarios, instance->IsChallengeMode() ? eAuchindounChallengeDatas::ChallengeScenarioID : eAuchindounChallengeDatas::NormalScenarioID);
        }
        /*
        void OnPlayerEnter(Player* p_Player) override
        {
            InstanceScript::OnPlayerEnter(p_Player);

            if (m_KaatharDied)
            {
                p_Player->m_Events.AddEvent(new EventTeleport(p_Player, 1), p_Player->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
            }
        }
        */

        void OnGameObjectCreate(GameObject* p_Go) override
        {
            if (p_Go)
            {
                switch (p_Go->GetEntry())
                {
					case eAuchindounObjects::GameObjectAuchindounCrystal:
						m_AuchindounCrystal = p_Go->GetGUID();
						break;
                    case eAuchindounObjects::GameobjectHolyBarrier:
                        m_HolyBarrierKathaarObjectGuid = p_Go->GetGUID();
                        p_Go->SetLootState(LootState::GO_READY);
                        p_Go->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false);
                        break;
                    case eAuchindounObjects::GameobjectAuchindounWindow:
                        m_WindowGuid = p_Go->GetGUID();
                        break;
                    case eAuchindounObjects::GameobjectFelBarrier:
                        m_FelBarrierAzzakelObjectGuid = p_Go->GetGUID();
                        break;
                    case eAuchindounObjects::GameobjectSoulTransportStart:
                        m_SoulTransportStartGuid = p_Go->GetGUID();
                        break;
                    case eAuchindounObjects::GameobjectSoulTransport1:
                        if (instance != nullptr)
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
                            {
                                m_SoulTransport01Guid = p_Go->GetGUID();
                            }
                        }
                        break;
                    case eAuchindounObjects::GameobjectSoulTransport2:
                        if (instance != nullptr)
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
                            {
                                m_SoulTransport02Guid = p_Go->GetGUID();
                            }
                        }
                        break;
                    case eAuchindounObjects::GameobjectSoulTransport3:
                        if (instance != nullptr)
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
							{
                                m_SoulTransport03Guid = p_Go->GetGUID();
                            }
                        }
                        break;
                    case CHALLENGE_MOD_ORB:
                        m_ChallengeOrbGuid = p_Go->GetGUID();
                        break;
                    case eAuchindounChallengeDatas::ChallengeModeDoor:
                    case eAuchindounChallengeDatas::ChallengeModeDoorSecond:
                        AddChallengeModeDoor(p_Go);
                        break;
                    default:
                        break;
                }
            }
        }

        void OnCreatureCreate(Creature* p_Creature) override
        {
            if (p_Creature)
            { 
                switch (p_Creature->GetEntry())
                {
                    case eAuchindounCreatures::CreatureSoulBinderTuulani01:
                        m_Tuulani02Guid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureIruun:
                        m_IruunGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureJoraa:
                        m_JoraGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureDurem:
                        m_ElumGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureAuchenaiDefenderUnique:
                        m_UniqueGuardGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounBosses::BossKaathar:
                        m_KaatharGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounBosses::BossNyami:
                        m_NyamibossGuid = p_Creature->GetGUID();
                        p_Creature->SummonCreature(eAuchindounCreatures::CreatureWardenAzzakael, 1661.218f, 2917.974f, 49.063f, 1.604011f,TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        break;
                    case eAuchindounBosses::BossAzaakel:
                        m_AzzakelGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounBosses::BossTeronogor:
                        m_TeronogorGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureSoulBinderTuulani:
                        m_TuulaniGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureSoulBinderNyami:
                        m_NyamiGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureShieldSpot:
                        m_TriggerBubbleMiddleNyamiGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureWardenAzzakael:
                        m_WardenGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureDemonsSummoner:
                        m_TriggerAzzakelFelPortalGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureGulkosh:
                        m_GulkoshGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureGromtashTheDestructor:
                        m_GromtashGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureDuragTheDominator:
                        m_DuragGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureSargereiAssasinating:
                        m_AssainatingGuardGuid = p_Creature->GetGUID();
                        break;
                    case eAuchindounCreatures::CreatureAuchenaiAssainated:
                        m_AssainatedGuardGuid = p_Creature->GetGUID();
                        break;
                    default:
                        break;
                }
            }
        }

        void OnUnitDeath(Unit* p_Unit) override
        {
            if (m_Instance == nullptr)
                return;

            Creature* p_Creature = p_Unit->ToCreature();
            if (!p_Creature)
                return;

            switch (p_Creature->GetEntry())
            {
                case eAuchindounBosses::BossKaathar:
                    {
                        if (GameObject* l_Holybarrier = instance->GetGameObject(GetData64(eAuchindounDatas::DataHolyBarrier)))
                        {
                            l_Holybarrier->Delete();
                            m_KaatharDied = true;
                        }                
                        break;
                    }
                case eAuchindounBosses::BossAzaakel:
                    {
                        if (GameObject* l_Felbarrier = instance->GetGameObject(GetData64(eAuchindounDatas::DataFelBarrier)))
                            l_Felbarrier->Delete();

                        if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(eAuchindounDatas::DataSoulTransportStart)))
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
                            {
                                if (l_Teronogor->IsAIEnabled)
                                l_Teronogor->GetAI()->DoAction(eAuchindounActions::ActionSoulMove1);
                            }
                        }
                        break;
                    }              
                    /// Soul Transport
                 case eAuchindounCreatures::CreatureGromtashTheDestructor:
                     if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(eAuchindounDatas::DataSoulTransport3)))
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
                            {
                                if (l_Teronogor->IsAIEnabled)
                                    l_Teronogor->GetAI()->DoAction(eAuchindounActions::ActionSoulMove4);
                            }
                        }
                        break;
                 case eAuchindounCreatures::CreatureGulkosh:
                     if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(eAuchindounDatas::DataSoulTransport2)))
                        {
                            if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
                            {
                                if (l_Teronogor->IsAIEnabled)
                                l_Teronogor->GetAI()->DoAction(eAuchindounActions::ActionSoulMove3);
                            }
                        }
                        break;
                 case eAuchindounCreatures::CreatureDuragTheDominator:
                    {
                        if (instance != nullptr)
                        {
                            if (GameObject* l_SoulTransport = instance->GetGameObject(GetData64(eAuchindounDatas::DataSoulTransport1)))
                            {
                                if (Creature* l_Teronogor = instance->GetCreature(GetData64(eAuchindounDatas::DataBossTeronogor)))
                                {
                                    if (l_Teronogor->IsAIEnabled)
                                        l_Teronogor->GetAI()->DoAction(eAuchindounActions::ActionSoulMove2);
                                }
                            }
                        }
                        break;
                    }
                 case eAuchindounBosses::BossTeronogor:
                    {
                        DoKilledMonsterKredit(eAuchindounChallengeDatas::DailyChallengeQuestID, eAuchindounChallengeDatas::DailyChallengeKillCredit);

                        if (p_Creature->GetMap() && p_Creature->GetMap()->IsHeroic())
                            DoCompleteAchievement(eAuchindounAchievements::AchievementAuchindounHeroic);
                        else
                            DoCompleteAchievement(eAuchindounAchievements::AchievementAuchindounNormal);

                        /// Curtain flames achievement, No Tags Backs! (9552)
                        UnitList l_Targets;
                        JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(p_Creature, p_Creature, 100, 153392, true);
                        JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(p_Creature, l_Targets, u_check);
                        p_Creature->VisitNearbyObject(100, searcher);

                        if (!l_Targets.empty())
                            DoCompleteAchievement(eAuchindounAchievements::AchievementNoTagBacks);
                        break;
                    }
                 default:
                     break;
            }
        }

        uint64 GetData64(uint32 p_Data) override
        {
            switch (p_Data)
            {
				case eAuchindounDatas::DataCrystal:
					return m_AuchindounCrystal;
					break;
                case eAuchindounDatas::DataTuulani02:
                    return m_Tuulani02Guid;
                    break;
                case eAuchindounDatas::DataGuard:
                    return m_UniqueGuardGuid;
                    break;
                case eAuchindounDatas::DataElum:
                    return m_ElumGuid;
                    break;
                case eAuchindounDatas::DataIruun:
                    return m_IruunGuid;
                    break;
                case eAuchindounDatas::DataJorra:
                    return m_JoraGuid;
                    break;
                case eAuchindounDatas::DataSoulTransportStart:
                    return m_SoulTransportStartGuid;
                    break;
                case eAuchindounDatas::DataSoulTransport1:
                    return m_SoulTransport01Guid;
                    break;
                case eAuchindounDatas::DataSoulTransport2:
                    return m_SoulTransport02Guid;
                    break;
                case eAuchindounDatas::DataSoulTransport3:
                    return m_SoulTransport03Guid;
                    break;
                case eAuchindounDatas::DataHolyBarrier:
                    return m_HolyBarrierKathaarObjectGuid;
                    break;
                case eAuchindounDatas::DataAuchindounWindow:
                    return m_WindowGuid;
                    break;
                case eAuchindounDatas::DataFelBarrier:
                    return m_FelBarrierAzzakelObjectGuid;
                    break;
                case eAuchindounDatas::DataFelPortal:
                    return m_FelPortalGuid;
                    break;
                case eAuchindounDatas::DataBossKathaar:
                    return m_KaatharGuid;
                    break;
                case eAuchindounDatas::DataBossAzzakael:
                    return m_AzzakelGuid;
                    break;
                case eAuchindounDatas::DataBossNyami:
                    return m_NyamibossGuid;
                    break;
                case eAuchindounDatas::DataBossTeronogor:
                    return m_TeronogorGuid;
                    break;
                case eAuchindounDatas::DataNyami:
                    return m_NyamiGuid;
                    break;
                case eAuchindounDatas::DataTuulani:
                    return m_TuulaniGuid;
                    break;
                case eAuchindounDatas::DataWarden:
                    return m_WardenGuid;
                    break;
                case eAuchindounDatas::DataGulkosh:
                    return m_GulkoshGuid;
                    break;
                case eAuchindounDatas::DataGromtash:
                    return m_GromtashGuid;
                    break;
                case eAuchindounDatas::DataDurag:
                    return m_DuragGuid;
                    break;
                case eAuchindounDatas::DataTriggerMiddleNyamiFightBubble:
                    return m_TriggerBubbleMiddleNyamiGuid;
                    break;
                case eAuchindounDatas::DataTriggerAzzakelController:
                    return m_TriggerAzzakelFelPortalGuid;
                    break;
                case eAuchindounDatas::DataAssinatingGuard:
                    return m_AssainatingGuardGuid;
                    break;
                case eAuchindounDatas::DataAssinatedGuard:
                    return m_AssainatedGuardGuid;
                    break;
                default:
                    break;
            }

            return 0;
        }
    };

    InstanceScript* GetInstanceScript(InstanceMap* p_Map) const override
    {
        return new instance_auchindoun_InstanceMapScript(p_Map);
    }
};

void AddSC_instance_auchindoun()
{
    new instance_auchindoun;
}

