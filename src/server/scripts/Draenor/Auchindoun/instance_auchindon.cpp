////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "auchindon.hpp"

class clapping : public BasicEvent
{
public:
    explicit clapping(Unit* unit, int value) : m_Obj(unit), m_Modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                switch (m_Modifier)
                {
                case 0:
                    if (Player* nearest = m_Obj->FindNearestPlayer(50.0f, true))
                    {
                        m_Obj->SetFacingToObject(nearest);
                        m_Obj->CastSpell(m_Obj, eAuchindonSpells::SpellApplaud);

                        m_Obj->m_Events.AddEvent(new clapping(m_Obj, 1), m_Obj->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
                case 1:
                {
                    m_Obj->RemoveAllAuras();
                    break;
                }
                }
            }
        }
        return true;
    }

private:
    Unit* m_Obj;
    int m_Modifier;
    int m_Event;
};

class ArcaneBombEvent : public BasicEvent
{
public:
    explicit ArcaneBombEvent(Unit* unit, int value) : m_Obj(unit), m_Modifier(value)
    {
    }

    bool Execute(uint64 /*currTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* m_Instance = m_Obj->GetInstanceScript())
            {
                switch (m_Modifier)
                {
                case 0:
                    if (Creature* l_Nearest = m_Obj->FindNearestCreature(eAuchindonCreatures::CreatureArcaneBomb, 50.0f, true))
                    {
                        m_Obj->CastSpell(l_Nearest, eAuchindonSpells::SpellArcaneBombAreaTrigger);
                        m_Obj->m_Events.AddEvent(new ArcaneBombEvent(m_Obj, 0), m_Obj->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                    }
                    break;
                }
            }
        }
        return true;
    }

private:
    Unit* m_Obj;
    int m_Modifier;
    int Event;
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
		instance_auchindon_InstanceMapScript(Map* map) : InstanceScript(map) {}

        InstanceScript* m_Instance = this;

		uint32 m_auiEncounter[4];
        // Creatures
        uint64 m_Nyami;
        uint64 m_Tuulani;
        uint64 m_Warden;
        uint64 m_Gromtash;
        uint64 m_Durag;
        uint64 m_Gulkosh;
        // Bosses
        uint64 m_Kaathar;
        uint64 m_Nyamiboss;
        uint64 m_Azzakel;
        uint64 m_Teronogor;
        // Objects
        uint64 m_HolyBarrierKathaarObject;
        uint64 m_CrystalKaathar;
        uint64 m_Window;
        uint64 m_FelBarrierAzzakelObject;
        uint64 m_FelPortal;
        uint64 m_SoulTransportStart;
        uint64 m_SoulTransport01;
        uint64 m_SoulTransport02;
        uint64 m_SoulTransport03;
        // Triggers
        uint64 m_TriggerBubbleMiddleNyami;
        uint64 m_TriggerAzzakelFelPortal;

        // Dispensor
        std::list<uint64> m_Dispensor;

		void Initialize() override
		{           
            // Creatures
            m_Nyami = 0;
            m_Tuulani = 0;
            m_Warden = 0;
            m_Gromtash = 0;
            m_Durag = 0;
            m_Gulkosh = 0;
            // Bosses
            m_Kaathar = 0;
            m_Nyamiboss = 0;
            m_Azzakel = 0;
            m_Teronogor = 0;
            // Objects
            m_HolyBarrierKathaarObject = 0;
            m_CrystalKaathar = 0;
            m_Window = 0;
            m_SoulTransportStart = 0;
            m_SoulTransport01 = 0;
            m_SoulTransport02 = 0;
            m_SoulTransport03 = 0;
            // Triggers
            m_TriggerBubbleMiddleNyami = 0;

            LaunchSpawning();
		}

        void LaunchSpawning()
        {
            if (Creature* l_Teronogor = m_Instance->instance->GetCreature(m_Instance->GetData64(eDataAuchindonDatas::DataBossTeronogor)))
            {
                for (int32 i = 0; i <= 5; i++)
                {                  
                    if (Creature* l_Guard = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionGuards[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Guard->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 505);
                    }
                }
                // Auchindon Summon Clappers
                for (int32 i = 0; i < 2; i++)
                {
                    if(Creature* l_Clapper = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionGuards2nd[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                        l_Clapper->m_Events.AddEvent(new clapping(l_Clapper, 0), l_Clapper->m_Events.CalculateTime(6 * TimeConstants::IN_MILLISECONDS));
                }

                // Cicrular Mobs - Magus
                for (int32 i = 0; i < 3; i++)
                {                
                    if (Creature* l_Magus = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionCircularMages[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Magus->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, );
                    }
                }
                // Cicrular Mobs - Priest
                for (int32 i = 0; i < 2; i++)
                {
                    if (Creature* l_Priest = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, g_PositionCircularPriests[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Priest->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, SpellKneel);
                    }
                }
                // Cicrular Mobs - Cleric
                for (int32 i = 0; i < 2; i++)
                {
                    if (Creature* l_Cleric = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniCleric, g_PositionCircularHolies[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Cleric->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, SpellKneel);
                    }
                }

                // Hovering Magus near preacher
                for (int32 i = 0; i < 2; i++)
                {             
                    if (Creature* l_Magus = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionAuchenaiMagus2nd[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Magus->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, SpellKneel);
                    }
                }
                // Preacher near 2 magus             
                if (Creature* l_Preacher = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniArbiter, g_PositionAuchenaiReader1st, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Preacher->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, SpellKneel);
                }

                for (int32 i = 0; i < 4; i++)
                {
                    if (Creature* l_Vigilant = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniVigiliant, g_PositionAuchenaiVigilant[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Vigilant->CastSpell(l_Vigilant, eAuchindonSpells::SpellGuard);
                        l_Vigilant->SetCurrentEquipmentId(1);

                        l_Vigilant->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                        l_Vigilant->SetFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                        l_Vigilant->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        l_Vigilant->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                    }
                }
                for (int32 i = 0; i < 2; i++)
                {
                    // Two Guarding Hopilite
                    if (Creature* l_Hopilite = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniHoplite, g_PositionHopilliteGuardState[i], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Hopilite->CastSpell(l_Hopilite, eAuchindonSpells::SpellGuard);
                    }
                }
                // Defender that reads near two guarding hopilite
                if (Creature* l_Defender = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniDefender, g_PositionDefenderWhoReadsNearTwoHopilite, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Defender->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteRead);
                }
                // Priest who meditates
                if (Creature* l_Priest = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureSargeriSoulPriest, g_PositionSoulPriestWhoMeditates, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Priest->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteHover);
                }
                // Magus who cast arcane bomb near hovering priest.
                if (Creature* l_Mage = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionMagusWhoCastArcane, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Mage->CastSpell(l_Mage, SpellArcaneChanneling);
                    l_Mage->m_Events.AddEvent(new ArcaneBombEvent(l_Mage, 0), l_Mage->m_Events.CalculateTime(20 * TimeConstants::IN_MILLISECONDS));
                }
                // Magus who talk to defender
                if (Creature* l_Magus = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniMagus, g_PositionMagusWhoTalksToDefender, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Magus->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteTalk);
                }
                // Soul Priest who talks to defenders
                if (Creature* l_SoulPriest = l_Teronogor->SummonCreature(eAuchindonCreatures::CreatureAucheniSoulPriest, g_PositionSoulPriestTalksToTwoDefender, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_SoulPriest->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, eAuchindonSpells::SpellEmoteTalk);
                }
            }
        }

        void OnGameObjectCreate(GameObject* go) override
		{
			switch (go->GetEntry())
			{
            case eAuchindonObjects::GameobjectHolyBarrier:
                    m_HolyBarrierKathaarObject = go->GetGUID();
                    break;          
            case eAuchindonObjects::GameobjectAuchindonWindow:
                    m_Window = go->GetGUID();
                    break;
            case eAuchindonObjects::GameobjectFelBarrier:
                    m_FelBarrierAzzakelObject = go->GetGUID();
                    break;
            case eAuchindonObjects::GameobjectSoulTransportStart:
                    m_SoulTransportStart = go->GetGUID();
                    break;
            case eAuchindonObjects::GameobjectSoulTransport1:
                    m_SoulTransport01 = go->GetGUID();
                    break;
            case eAuchindonObjects::GameobjectSoulTransport2:
                    m_SoulTransport02 = go->GetGUID();
                    break;
            case eAuchindonObjects::GameobjectSoulTransport3:
                    m_SoulTransport03 = go->GetGUID();
                    break;
			}
		}

        void OnCreatureCreate(Creature* p_Creature) override
		{
            switch (p_Creature->GetEntry())
			{
                case eAuchindonBosses::BossKaathar:
                    m_Kaathar = p_Creature->GetGUID();
                        break;
                case eAuchindonBosses::BossNyami:
                    m_Nyamiboss = p_Creature->GetGUID();
                        break;
                case eAuchindonBosses::BossAzaakel:
                    m_Azzakel = p_Creature->GetGUID();
                        break;
                case eAuchindonBosses::BossTeronogor:
                    m_Teronogor = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureSoulBinderTuulani:
                    m_Tuulani = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureSoulBinderNyami:
                    m_Nyami = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureShieldSpot:
                    m_TriggerBubbleMiddleNyami = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureWardenAzzakael:
                    m_Warden = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureDemonsSummoner:
                    m_TriggerAzzakelFelPortal = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureGulkosh:
                    m_Gulkosh = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureGromtashTheDestructor:
                    m_Gromtash = p_Creature->GetGUID();
                        break;
                case eAuchindonCreatures::CreatureDuragTheDominator:
                    m_Durag = p_Creature->GetGUID();
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

                        // Curtain flames achievement, No Tags Backs! (9552)
                        UnitList l_Targets;
                        JadeCore::AnyUnitHavingBuffInObjectRangeCheck u_check(p_Creature, p_Creature, 100, 153392, true);
                        JadeCore::UnitListSearcher<JadeCore::AnyUnitHavingBuffInObjectRangeCheck> searcher(p_Creature, l_Targets, u_check);
                        p_Creature->VisitNearbyObject(100, searcher);

                        if (l_Targets.empty())
                            return;
                        else
                        {
                            DoCompleteAchievement(9552);
                        }

                        break;
                    }
            }
        }

        uint64 GetData64(uint32 p_Data) override
		{
            switch (p_Data)
			{
            case eDataAuchindonDatas::DataSoulTransportStart:
                    return m_SoulTransportStart;
                    break;
            case eDataAuchindonDatas::DataSoulTransport1:
                    return m_SoulTransport01;
                    break;
            case eDataAuchindonDatas::DataSoulTransport2:
                    return m_SoulTransport02;
                    break;
            case eDataAuchindonDatas::DataSoulTransport3:
                    return m_SoulTransport03;
                    break;
            case eDataAuchindonDatas::DataHolyBarrier:
                    return m_HolyBarrierKathaarObject;
                    break;
            case eDataAuchindonDatas::DataAuchindonWindow:
                    return m_Window;
                    break;
            case eDataAuchindonDatas::DataFelBarrier:
                    return m_FelBarrierAzzakelObject;
                    break;
            case eDataAuchindonDatas::DataFelPortal:
                    return m_FelPortal;
                    break;
            case eDataAuchindonDatas::DataBossKathaar:
                    return m_Kaathar;
                    break;
            case eDataAuchindonDatas::DataBossAzzakael:
                    return m_Azzakel;
                    break;
            case eDataAuchindonDatas::DataBossNyami:
                    return m_Nyamiboss;
                    break;
            case eDataAuchindonDatas::DataBossTeronogor:
                    return m_Teronogor;
                    break;
            case eDataAuchindonDatas::DataNyami:
                    return m_Nyami;
                    break;
            case eDataAuchindonDatas::DataTuulani:
                    return m_Tuulani;
                    break;  
            case eDataAuchindonDatas::DataWarden:
                    return m_Warden;
                    break;
            case eDataAuchindonDatas::DataGulkosh:
                    return m_Gulkosh;
                    break;
            case eDataAuchindonDatas::DataGromtash:
                    return m_Gromtash;
                    break;
            case eDataAuchindonDatas::DataDurag:
                    return m_Durag;
                    break;
            case eDataAuchindonDatas::DataTriggerMiddleNyamiFightBubble:
                    return m_TriggerBubbleMiddleNyami;
                    break;
            case eDataAuchindonDatas::DataTriggerAzzakelController:
                    return m_TriggerAzzakelFelPortal;
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

void AddSC_instance_auchindon()
{
	new instance_auchindon;
}

