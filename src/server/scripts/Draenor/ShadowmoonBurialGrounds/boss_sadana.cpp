////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "shadowmoon_burial_grounds.hpp"
#include "MoveSplineInit.h"

enum eSadanaSpells
{
    SpellDaggerFallDummy                    = 153240,
    SpellDaggerFallSummon                   = 153200,
    SpellDaggerAlttitudeVisual              = 153225,
    SpellDaggerAura                         = 153236,
    SpellDaggerAuraFunctioning              = 153216,
    SpellDaggerExplosion                    = 153373,
    SpellDaggerFallTriggerMissile           = 153370,
    SpellDaggerFallGroundMarker             = 173073,
    SpellDaggerFallDamage                   = 153224,
    SpellDarkCommunion                      = 153153,
    SpellDarkCommunionBuff                  = 153164,
    SpellDeathSpikeProcTriggerSpell         = 153079,
    SpellDeathSpikeDamage                   = 153089,
    SpellDeathSpikeFull                     = 162696,
    SpellWhispersOfTheDarkStarDamage        = 153093,
    SpellWhisperOfTheDarkStarTriggerSpell   = 153094,
    SpellLunarPurityAreaTrigger             = 164974,
    SpellLunarPurityDamageModification      = 162652,
    SpellShadowRitual                       = 152158,
    SpellLunarRitual                        = 164693,
    SpellShadowRuneVisual                   = 152684,
    SpellShadowRuneVisual01                 = 152691,
    SpellShadowRuneVisual02                 = 152695,
    SpellLunarRuneVisual                    = 164693,
    SpellLunarRuneVisual01                  = 164695,
    SpellLunarRuneVisual02                  = 164696,
    SpellDarkEclipsePeriodicDummy           = 164685,
    SpellDarkEclipseDamage                  = 164686,
    SpellDarkEclipseAreatrirger             = 164704,
    SpellDarkEclipseDummy                   = 164705,
    SpellDarkEclipseDummy02                 = 164706,
    SpellDarkEclipsePeriodicTrigger         = 164710,
    SpellDarkElcipseUnknownPeriodicDummy    = 164974,
    SpellLunaryPurtiyBuff                   = 162652,
    //SpellLunarPurityAreaTrigger           = 162620,
    SpellTenebreuxViolet                    = 152311,
    SpellCosmeticStrangulateState           = 78037
};

enum eSadanaEvents
{
    EventDaggerFall = 1,
    EventDaggerFallMovement,
    EventDaggerFallActivation,
    EventDarkCommunion,
    EventDeathSpike,
    EventWhisperOfTheDarkStar,
    EventDarkEclipse,
    EventDarkEclipseFinish,
    EventLunarPurity,
    EventDefiledSpirtMovement
};

enum eSadanaTalks
{
    TalkAggro = 1, ///< Anscestors of the Shadowmoon.. shine in the darkness! Lend me your strength! [43539]
    TalkDeath,     ///< The dead shall rise again.. [43540]
    TalkIntro,     ///< Rivers of the damned.. rise! RISE in the name of Darkness... RISE in the name of NERZ'UL!.. the Shadowmoon beckons [43541]
    TalkKill01,    ///< Another spirit for the army! [43542]
    TalkKill02,    ///< Embrace the eternal darkness! [43543]
    TalkSpell01,   ///< For Nerz'ul! [43544]
    TalkSpell02,   ///< Mmm.. mmm.. YES! the perfect Sacrifice! [43545]
    TalkSpell03,   ///< Your souls will belong to me! [43546]
    TalkSpell04,   ///< Restless spirits.. heed my command! [43547]
};

enum eSadanaActions
{
    ActionActivateDefiledSpirit = 1, 
    ActionMoveDefiledSpirits,
    ActionActivateLunarTriggersActivate, 
    ActionActivateLunarTriggersDeactivate,
	ActionSadanaReset
};

enum eSadanaCreatures
{
    CreatureDaggerFall                   = 75981,
    CreatureDefiledSpiritSadanaEncounter = 75966,
    CreatureShadowRune                   = 75778
};
   
enum eSadanaMovements
{
    MovementDaggerFallReachedAlttitude = 1
};

#define DaggerFallAltitude 268.028f
#define DaggerGroundAltitude 261.308f
#define HostileFaction 16
#define FriendlyFaction 35

Position const g_PositionCircles[8] = 
{
    {1805.502f, -16.298f, 261.308f, 4.011894f},
    {1809.695f, -26.531f, 261.308f, 3.086692f},
    {1805.346f, -37.156f, 261.308f, 2.155996f},
    {1795.560f, -40.853f, 261.308f, 1.508042f},
    {1785.818f, -37.061f, 261.308f, 0.640176f},
    {1781.712f, -26.524f, 261.308f, 6.263623f},
    {1786.218f, -16.460f, 261.308f, 5.482167f},
    {1795.574f, -12.252f, 261.308f, 4.767466f}
};

Position const g_PositionDefiledSpiritsMovement[8] =
{
    {1817.100f, -26.563f, 276.406f, 4.329479f},
    {1817.556f, -49.306f, 300.783f, 2.829369f},
    {1794.627f, -57.390f, 289.405f, 2.762329f},
    {1773.798f, -52.632f, 278.439f, 1.855194f},
    {1787.415f, -34.515f, 289.940f, 0.099829f},
    {1819.107f, -6.276f, 273.476f,  4.141892f},
    {1789.199f, -9.896f, 270.591f,  3.336035f},
    {1797.886f, -20.040f, 281.501f, 5.166008f}
};

uint32 g_LunarRuneEntries[3] = { eSadanaSpells::SpellLunarRuneVisual, eSadanaSpells::SpellLunarRuneVisual01, eSadanaSpells::SpellLunarRuneVisual02 };
uint32 g_ShadowRuneEntries[3] = { eSadanaSpells::SpellShadowRuneVisual, eSadanaSpells::SpellShadowRuneVisual01, eSadanaSpells::SpellShadowRuneVisual02 };

Position const g_PositionSpiritHomePoint = {1795.783f, -27.240f, 277.135f, 0.040068f };

/// Basic Event - [The message Sadana says few minutes after players entered the instance for the first time]
class EventSadanaIntro : public BasicEvent
{
	public:

    explicit EventSadanaIntro(Unit* unit) : m_Obj(unit)
    {
    }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Sadana = l_Instance->instance->GetCreature(l_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossSadana)))
                {
                    if (l_Sadana->IsAIEnabled)
                        l_Sadana->AI()->Talk(eSadanaTalks::TalkIntro);
                }
            }
        }
        return true;
    }

private:
    Unit* m_Obj;
};

/// Sadana Bloodyfury - 75509 [Boss]
class boss_sadana_bloodfury : public CreatureScript
{
	public:

    boss_sadana_bloodfury() : CreatureScript("boss_sadana_bloodfury") { }

    struct boss_sadana_bloodfuryAI : public BossAI
    {
        boss_sadana_bloodfuryAI(Creature* p_Creature) : BossAI(p_Creature, eShadowmoonBurialGroundsDatas::DataBossSadana)
        {
            m_Instance = me->GetInstanceScript();
            m_First = false;
        }

        enum eSadanaSpells
        {
            SpellDaggerFallDummy                  = 153240,
            SpellDaggerFallSummon                 = 153200,
            SpellDaggerAlttitudeVisual            = 153225,
            SpellDaggerAura                       = 153236,
            SpellDaggerAuraFunctioning            = 153216,
            SpellDaggerExplosion                  = 153373,
            SpellDaggerFallTriggerMissile         = 153370,
            SpellDaggerFallGroundMarker           = 173073,
            SpellDaggerFallDamage                 = 153224,
            SpellDarkCommunion                    = 153153,
            SpellDarkCommunionBuff                = 153164,
            SpellDeathSpikeProcTriggerSpell       = 153079,
            SpellDeathSpikeDamage                 = 153089,
            SpellDeathSpikeFull                   = 162696,
            SpellWhispersOfTheDarkStarDamage      = 153093,
            SpellWhisperOfTheDarkStarTriggerSpell = 153094,
            SpellDarkEclipsePeriodicDummy         = 164685,
            SpellDarkEclipseDamage                = 164686,
            SpellDarkEclipseAreatrirger           = 164704,
            SpellDarkEclipseDummy                 = 164705,
            SpellDarkEclipseDummy02               = 164706,
            SpellDarkEclipsePeriodicTrigger       = 164710,
            SpellDarkElcipseUnknownPeriodicDummy  = 164974,
            SpellCosmeticStrangulateState         = 78037,
            SpellShadowRitual                     = 152158
        };

        enum eSadanaEvents
        {
            EventDaggerFall = 1,
            EventDaggerFallMovement,
            EventDaggerFallActivation,
            EventDarkCommunion,
            EventDeathSpike,
            EventWhisperOfTheDarkStar,
            EventDarkEclipse,
            EventDarkEclipseFinish
        };

        InstanceScript* m_Instance;
        uint64 m_CommunionGuid;
        bool m_CommunionInRange;
        bool m_CommunionInAction;
        bool m_First;

        void Reset() override
        {
            _Reset();
            events.Reset();
            m_CommunionGuid = 0;
            m_CommunionInRange = false;
            m_CommunionInAction = false;
            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            me->CastSpell(me, eSadanaSpells::SpellShadowRitual);
            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC
                | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            if (!m_First) // new way to handle constructor (new for me)
            {
                m_First = true;
                me->m_Events.AddEvent(new EventSadanaIntro(me), me->m_Events.CalculateTime(40 * TimeConstants::IN_MILLISECONDS));
            }
            /// Activate shadow runes;
            std::list<Creature*> l_ListShadowrunes;
            me->GetCreatureListWithEntryInGrid(l_ListShadowrunes, eSadanaCreatures::CreatureShadowRune, 200.0f);
            if (!l_ListShadowrunes.empty())
            {
                for (Creature* l_Itr : l_ListShadowrunes)
                {
                    if (!l_Itr)
                        continue;

                    if (l_Itr->IsAIEnabled)
                        l_Itr->GetAI()->Reset();
                }
            }
            /// Delete all ground markers nearby.
            std::list<AreaTrigger*> l_listAreaTriggers;
            me->GetAreatriggerListInRange(l_listAreaTriggers, 300.0f);
            if (!l_listAreaTriggers.empty())
            {
                for (AreaTrigger* l_Itr : l_listAreaTriggers)
                {
                    if (!l_Itr)
                        continue;

                    if (l_Itr->GetSpellId() != eSadanaSpells::SpellDaggerFallGroundMarker)
                        continue;

                    l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
                }
            }

			/// Attempt to forcely reset the defiled spirits.
			std::list<Creature*> l_ListDefiledSpirit;
			me->GetCreatureListWithEntryInGrid(l_ListDefiledSpirit, eSadanaCreatures::CreatureDefiledSpiritSadanaEncounter, 300.0f);
			if (!l_ListDefiledSpirit.empty())
			{
				for (Creature* l_Itr : l_ListDefiledSpirit)
				{
					l_Itr->DespawnOrUnsummon();
				}
			}
			SummonPathsDefiledSpirits();
            if (me->GetMap())
                me->GetMap()->SetObjectVisibility(1000.0f);
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            if (m_First)
            {
                HandleDoorCombatActivation();
                DespawnCreaturesInArea(eSadanaCreatures::CreatureDaggerFall, me);
                /// A removal method which doesn't crash the momma of the server. Yes'ir
                std::list<Creature*> l_SadanaSpirits;
                me->GetCreatureListInGrid(l_SadanaSpirits, 300.0f);
                if (!l_SadanaSpirits.empty())
                {
                    for (Creature* l_itr : l_SadanaSpirits)
                    {
                        l_itr->Respawn();
                    }
                }
                /// Delete all ground markers nearby.
                std::list<AreaTrigger*> l_listAreaTriggers;
                me->GetAreatriggerListInRange(l_listAreaTriggers, 300.0f);
                if (!l_listAreaTriggers.empty())
                {
                    for (AreaTrigger* l_Itr : l_listAreaTriggers)
                    {
                        if (!l_Itr)
                            continue;

                        if (l_Itr->GetSpellId() != eSadanaSpells::SpellDaggerFallGroundMarker)
                            continue;

                        l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
                    }
                }
            }
        }

        void JustSummoned(Creature* p_Summon) override
        {
            if (p_Summon)
            {
                switch (p_Summon->GetEntry())
                {
                    case eSadanaCreatures::CreatureShadowRune:
                        p_Summon->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        p_Summon->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        break;
                    case eShadowmoonBurialGroundsCreatures::CreatureEclipseTrigger:
                        p_Summon->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        p_Summon->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        break;         
                    default:
                        break;
                }
            }
        }

        void HandleDoorCombatActivation()
        {
            if (m_Instance != nullptr)
            {
                if (GameObject* l_SadanaDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataSadanaFightDoor)))
                {
                    /// Activate
                    l_SadanaDoor->SetLootState(LootState::GO_READY);
                    l_SadanaDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
        }

        void HandleDoorEncounterFinishActivation()
        {
            if (m_Instance != nullptr)
            {
                if (GameObject* l_SadanaFightDoor = m_Instance->instance->GetGameObject(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataStoneDoorSadana)))
                {
                    /// Activate
                    l_SadanaFightDoor->SetLootState(LootState::GO_READY);
                    l_SadanaFightDoor->UseDoorOrButton(10 * TimeConstants::IN_MILLISECONDS, false, me);
                }
            }
        }

        void SummonPathsDefiledSpirits()
        {
            for (uint16 l_I = 0; l_I <= 30; l_I++)
            {
                Position l_Position;
                me->GetRandomNearPosition(l_Position, 10.0f);
                l_Position.m_positionZ = frand(279.636f, 284.058f);
                me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureDefliedSpirit01, l_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
            }
        }

        void SummonEclipseTriggers()
        {
            DespawnCreaturesInArea(eShadowmoonBurialGroundsCreatures::CreatureEclipseTrigger, me);

            for (uint8 l_I = 0; l_I <= 7; l_I++)
                me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureEclipseTrigger, g_PositionCircles[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (m_Instance != nullptr)
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

            _EnterCombat();
            SummonEclipseTriggers();
            HandleDoorCombatActivation();
            Talk(eSadanaTalks::TalkAggro);
            me->RemoveAura(eSadanaSpells::SpellShadowRitual);                
            events.ScheduleEvent(eSadanaEvents::EventDeathSpike, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 12 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventWhisperOfTheDarkStar, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventDarkCommunion, 30 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eSadanaEvents::EventDarkEclipse, 60 * TimeConstants::IN_MILLISECONDS);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eSadanaTalks::TalkKill01);
                else
                    Talk(eSadanaTalks::TalkKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eSadanaTalks::TalkDeath);     
            HandleDoorCombatActivation(); 
            HandleDoorEncounterFinishActivation();
            DespawnCreaturesInArea(eSadanaCreatures::CreatureShadowRune, me);
            DespawnCreaturesInArea(eSadanaCreatures::CreatureDaggerFall, me);
            DespawnCreaturesInArea(eShadowmoonBurialGroundsCreatures::CreatureEclipseTrigger, me);       
            /// A removal method which doesn't crash the momma of the server. Yes'ir
            std::list<Creature*> l_SadanaSpirits;
            me->GetCreatureListWithEntryInGrid(l_SadanaSpirits, eShadowmoonBurialGroundsCreatures::CreatureDefliedSpirit01 ,300.0f);
            if (!l_SadanaSpirits.empty())
            {
                for (Creature* l_Itr : l_SadanaSpirits)
                {
                    l_Itr->SetDisplayId(11686);
                    l_Itr->RemoveAllAuras();
                    l_Itr->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                }
            }

            /// Delete all ground markers nearby.
            std::list<AreaTrigger*> l_listAreaTriggers;
            me->GetAreatriggerListInRange(l_listAreaTriggers, 300.0f);
            if (!l_listAreaTriggers.empty())
            {
                for (AreaTrigger* l_Itr : l_listAreaTriggers)
                {
                    if (!l_Itr)
                        continue;

                    if (l_Itr->GetSpellId() != eSadanaSpells::SpellDaggerFallGroundMarker)
                        continue;

                    l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
                }
            }

            if (Creature* l_WorldTrigger = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureWorldTriggerSadanaRp, 200.0f, true))
            {
                if (Creature* l_WeepingSoul = me->SummonCreature(eShadowmoonBurialGroundsCreatures::CreatureWeepingSpirit, l_WorldTrigger->GetPositionX(), l_WorldTrigger->GetPositionY(), l_WorldTrigger->GetPositionZ(), l_WorldTrigger->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS))
                {
                    if (l_WeepingSoul->IsAIEnabled)
                        l_WeepingSoul->AI()->Talk(eShadowmoonBurialGroundsTalks::TalkUnknown01);
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {     
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            /// Dark Communion - (needs to be above then unit_state_casting so it can still work with the cast itself)
            if (m_CommunionGuid != 0 && m_CommunionInRange)
            {
                if (Creature* l_Creature = Creature::GetCreature(*me, m_CommunionGuid))
                {
                    l_Creature->UpdatePosition(l_Creature->GetPositionX(), l_Creature->GetPositionY(), l_Creature->GetPositionZ(), l_Creature->GetOrientation(), true);  
                    if (me->IsWithinDistInMap(l_Creature, 3.5f))
                    {           
                        m_CommunionInRange = false;    
                        l_Creature->DespawnOrUnsummon();
                        me->CastSpell(me, eSadanaSpells::SpellDarkCommunionBuff, false);
                    }
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            if (m_CommunionInAction) /// During communion phase other spells should be disabled.
                return;

            switch (events.ExecuteEvent())
            {
                case eSadanaEvents::EventDaggerFall:
                {             
                    /// Delete all ground markers nearby.
                    std::list<AreaTrigger*> l_listAreaTriggers;
                    me->GetAreatriggerListInRange(l_listAreaTriggers, 300.0f);
                    if (!l_listAreaTriggers.empty())
                    {
                        for (AreaTrigger* l_Itr : l_listAreaTriggers)
                        {
                            if (!l_Itr || l_Itr->GetSpellId() != eSadanaSpells::SpellDaggerFallGroundMarker)
                                continue;

                            l_Itr->Remove(1 * TimeConstants::IN_MILLISECONDS);
                        }
                    }          
                    for (uint8 l_I = 0; l_I < 2; l_I++)
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true, -eSadanaSpells::SpellDaggerFallGroundMarker))
                        {
                            if (Creature* l_Dagger = me->SummonCreature(eSadanaCreatures::CreatureDaggerFall, l_Target->GetPositionX(), l_Target->GetPositionY(), DaggerFallAltitude, l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                                l_Target->CastSpell(l_Target, eSadanaSpells::SpellDaggerFallGroundMarker);     
                        }
                    }
                    events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 20 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eSadanaEvents::EventWhisperOfTheDarkStar:
                    me->CastSpell(me, eSadanaSpells::SpellWhisperOfTheDarkStarTriggerSpell);
                    events.ScheduleEvent(eSadanaEvents::EventWhisperOfTheDarkStar, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eSadanaEvents::EventDarkCommunion:
                    if (Creature* l_NearestDefiledSpirit = me->FindNearestCreature(eShadowmoonBurialGroundsCreatures::CreatureDefliedSpirit01, 100.0f, true))
                    {               
                        l_NearestDefiledSpirit->AddAura(eSadanaSpells::SpellCosmeticStrangulateState, l_NearestDefiledSpirit);
                        me->CastSpell(l_NearestDefiledSpirit, eSadanaSpells::SpellDarkCommunion);
                        m_CommunionGuid = 0;
                        m_CommunionGuid = l_NearestDefiledSpirit->GetGUID();
                        m_CommunionInRange = true;
                        m_CommunionInAction = true;
                    }
                    events.ScheduleEvent(eSadanaEvents::EventDarkCommunion, 30 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eSadanaEvents::EventDeathSpike:
                    me->CastSpell(me, eSadanaSpells::SpellDeathSpikeFull);
                    events.ScheduleEvent(eSadanaEvents::EventDeathSpike, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eSadanaEvents::EventDarkEclipse:
                {
                    events.Reset();
                    /// Activate shadow runes;
                    std::list<Creature*> l_ListShadowrunes;
                    me->GetCreatureListWithEntryInGrid(l_ListShadowrunes, eSadanaCreatures::CreatureShadowRune, 200.0f);
                    if (!l_ListShadowrunes.empty())
                    {
                        std::list<Creature*>::const_iterator l_It = l_ListShadowrunes.begin();
                        std::advance(l_It, urand(0, l_ListShadowrunes.size() - 6));

                        if ((*l_It)->IsAIEnabled)
                            (*l_It)->GetAI()->DoAction(eSadanaActions::ActionActivateLunarTriggersActivate);
                    }
                    /// Eclipse triggers = functional: adds the friendly buff upon standing on the rune itself and getting immuned to the eclipse damage within seconds.
                    std::list<Creature*> l_ListTriggersLunars;
                    me->GetCreatureListWithEntryInGrid(l_ListTriggersLunars, eShadowmoonBurialGroundsCreatures::CreatureEclipseTrigger, 200.0f);
                    if (!l_ListTriggersLunars.empty())
                    {
                        for (Creature* l_Itr : l_ListTriggersLunars)
                        {
                            if (!l_Itr)
                                continue;

                            if (l_Itr->IsAIEnabled)
                                l_Itr->GetAI()->DoAction(eSadanaActions::ActionActivateLunarTriggersActivate);
                        }
                    }
                    me->NearTeleportTo(1795.788f, -26.7637f, 261.3088f, 4.629905f); /// To the Middle.
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->AttackStop();
                    me->SetReactState(ReactStates::REACT_PASSIVE);

                    if (InstanceScript* l_Instance = me->GetInstanceScript())
                    {
                        if (Creature* l_NearestEclipseTrigger = l_Instance->instance->GetCreature(l_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataSadanaEclipseTrigger)))
                            l_NearestEclipseTrigger->AddAura(eSadanaSpells::SpellDarkEclipsePeriodicDummy, l_NearestEclipseTrigger); /// Please work
                    }
                
                    events.ScheduleEvent(eSadanaEvents::EventDarkEclipse, 60 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eSadanaEvents::EventDarkEclipseFinish, 27 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eSadanaEvents::EventDarkEclipseFinish:
                {
                    events.ScheduleEvent(eSadanaEvents::EventDeathSpike, 8 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 12 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eSadanaEvents::EventWhisperOfTheDarkStar, 20 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eSadanaEvents::EventDarkCommunion, 30 * TimeConstants::IN_MILLISECONDS);
                    events.ScheduleEvent(eSadanaEvents::EventDarkEclipse, 60 * TimeConstants::IN_MILLISECONDS);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    /// Activate shadow runes - cosmetic;
                    std::list<Creature*> l_ListTriggersRunes;
                    me->GetCreatureListWithEntryInGrid(l_ListTriggersRunes, eSadanaCreatures::CreatureShadowRune, 100.0f);
                    if (!l_ListTriggersRunes.empty())
                    {
                        for (Creature* l_Itr : l_ListTriggersRunes)
                        {
                            if (l_Itr->IsAIEnabled)
                                l_Itr->GetAI()->DoAction(eSadanaActions::ActionActivateLunarTriggersDeactivate);
                        }
                    }
                    /// Eclipse triggers = functional: adds the friendly buff upon standing on the rune itself and getting immuned to the eclipse damage within seconds.
                    std::list<Creature*> l_ListTriggersLunars;
                    me->GetCreatureListWithEntryInGrid(l_ListTriggersLunars, eShadowmoonBurialGroundsCreatures::CreatureEclipseTrigger, 100.0f);
                    if (!l_ListTriggersLunars.empty())
                    {
                        for (Creature* l_Itr : l_ListTriggersLunars)
                        {
                            if (l_Itr->IsAIEnabled)
                                l_Itr->GetAI()->DoAction(eSadanaActions::ActionActivateLunarTriggersDeactivate);
                        }
                    }
                    break;
                }
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_sadana_bloodfuryAI(p_Creature);
    }
};

/// Defiled Spirit - 75966 [The spirits that flies around in circles, responsible for dark communion spell]
class shadowmoon_burial_grounds_sadana_creature_defiled_spirit : public CreatureScript
{
	public:

    shadowmoon_burial_grounds_sadana_creature_defiled_spirit() : CreatureScript("shadowmoon_burial_grounds_sadana_creature_defiled_spirit") {}

    struct shadowmoon_burial_grounds_sadana_creature_defiled_spiritAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_sadana_creature_defiled_spiritAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_MovementIndentifier;
        bool m_StartEvent;
        bool m_Activation;

        void Reset() override
        {
            events.Reset();
            me->SetCanFly(true);
            m_Activation = false;
            m_MovementIndentifier = 0;
            me->SetDisableGravity(true);
            me->setFaction(HostileFaction);           
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->CastSpell(me, eSadanaSpells::SpellTenebreuxViolet);
            me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 0.9f);              
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC); 
            if (me->GetMap())
                me->GetMap()->SetObjectVisibility(1000.0f);

            Movement::MoveSplineInit init(me);
            FillCirclePath(g_PositionSpiritHomePoint, me->GetDistance2d(g_PositionSpiritHomePoint.GetPositionX(), g_PositionSpiritHomePoint.GetPositionY()), g_PositionSpiritHomePoint.GetPositionZ(), init.Path(), true);
            init.SetWalk(true);
            init.SetCyclic();
            init.Launch();      

			/*
            Position l_Position;
            l_Position = g_PositionDefiledSpiritsMovement[m_MovementIndentifier];
            me->GetMotionMaster()->MoveTakeoff(m_MovementIndentifier, l_Position.GetPositionX() + frand(2.0f, 8.0f), l_Position.GetPositionY() + frand(2.0f, 10.0f), 286.785f + frand(2.0f, 10.0f));
			*/
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eSadanaActions::ActionActivateDefiledSpirit:
                {
                    if (m_Activation)
                        return;

                    events.Reset();
                    m_Activation = true;
                    me->StopMoving();
                    me->SetSpeed(UnitMoveType::MOVE_RUN, 0.5f, true);
					me->SetSpeed(UnitMoveType::MOVE_FLIGHT, 1.2f, true);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);

                    if (m_Instance != nullptr)
                    {
                        ///< Reset handling - encounter.
                        if (Creature * l_Sadana = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossSadana)))
                            me->GetMotionMaster()->MoveFollow(l_Sadana, 0, 0);
                    }
                    break;
                }
				case eSadanaActions::ActionSadanaReset:
				{
					break;
				}
                default:
                    break;
            }
        }

        void FillCirclePath(Position const& p_CenterPos, float p_Radius, float p_Z, Movement::PointsArray& p_Path, bool p_Clockwise)
        {
            float m_Step   =  p_Clockwise ? -M_PI / 8.0f : M_PI / 8.0f;
            float m_Angle  =  p_CenterPos.GetAngle(me->GetPositionX(), me->GetPositionY());
            float m_Radius = p_Radius * 1.4300;

            for (uint8 i = 0; i < 16; m_Angle += m_Step, ++i)
            {
                G3D::Vector3 l_Point;
                l_Point.x = p_CenterPos.GetPositionX() + m_Radius * cosf(m_Angle);
                l_Point.y = p_CenterPos.GetPositionY() + m_Radius * sinf(m_Angle);
                l_Point.z = 283.294f;
                p_Path.push_back(l_Point);
            }
        }

		/*
        void MovementInform(uint32 p_Type, uint32 p_Id) override
        {
            if (me && me->IsInWorld() && me->isAlive())
            {
                if (p_Id == m_MovementIndentifier)
                {
                    m_MovementIndentifier++;

                    /// Movement handler.
                    if (m_MovementIndentifier >= 8)
                        m_MovementIndentifier = 0;

                    Position l_Position;
                    l_Position = g_PositionDefiledSpiritsMovement[m_MovementIndentifier];
					me->GetMotionMaster()->MoveTakeoff(m_MovementIndentifier, l_Position.GetPositionX() + frand(2.0f, 8.0f), l_Position.GetPositionY() + frand(2.0f, 10.0f), 286.785f + frand(2.0f, 10.0f));
                }
            }
        }
		*/

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                /// Reset handling - encounter.
                if (Creature * l_Sadana = m_Instance->instance->GetCreature(m_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossSadana)))
                {
                    if (l_Sadana->IsAIEnabled)
                    {
                        if (boss_sadana_bloodfury::boss_sadana_bloodfuryAI* l_LinkAI = CAST_AI(boss_sadana_bloodfury::boss_sadana_bloodfuryAI, l_Sadana->GetAI()))
                        {
                            l_LinkAI->m_CommunionGuid = 0;
                            l_LinkAI->m_CommunionInRange = false;
                            me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                            l_Sadana->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            l_Sadana->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                            l_Sadana->CastStop();
                        }
                    }
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_sadana_creature_defiled_spiritAI(p_Creature);
    }
};

/// Falling Dagger - 75981 [Falling dagger]
class shadowmoon_burial_grounds_sadana_creature_falling_dagger : public CreatureScript
{
	public:

    shadowmoon_burial_grounds_sadana_creature_falling_dagger() : CreatureScript("shadowmoon_burial_grounds_sadana_creature_falling_dagger") {}

    struct shadowmoon_burial_grounds_sadana_creature_falling_daggerAI : public ScriptedAI
    {
        shadowmoon_burial_grounds_sadana_creature_falling_daggerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
            m_CanDamage = false;
        }

        InstanceScript* m_Instance;
        bool m_StartEvent;
        bool m_CanDamage;

        void Reset() override
        {
            events.Reset();
            me->setFaction(HostileFaction);
            me->SetCanFly(true);
            me->SetDisableGravity(true);
            me->SetReactState(ReactStates::REACT_PASSIVE);     
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            me->SetFlag(EUnitFields::UNIT_FIELD_LEVEL, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->CastSpell(me, eSadanaSpells::SpellDaggerAuraFunctioning);        
            events.ScheduleEvent(eSadanaEvents::EventDaggerFallMovement, 2 * TimeConstants::IN_MILLISECONDS);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id)
        {
            switch (p_Id)
            {
                case eSadanaMovements::MovementDaggerFallReachedAlttitude: /// Damage starts ticking once dagger is in player's alttitude.
                    events.ScheduleEvent(eSadanaEvents::EventDaggerFallActivation, 500);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eSadanaEvents::EventDaggerFallActivation:               
                    m_CanDamage = true;
                    me->CastSpell(me, eSadanaSpells::SpellDaggerAlttitudeVisual); /// The visual when the daggerfall reaches the ground
                    events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 100); /// Activates hardcoded damage intervals
                    break;
                case eSadanaEvents::EventDaggerFallMovement:
                    me->SetSpeed(UnitMoveType::MOVE_RUN, 2.0f);
                    me->GetMotionMaster()->MoveTakeoff(eSadanaMovements::MovementDaggerFallReachedAlttitude, me->GetPositionX(), me->GetPositionY(), DaggerGroundAltitude);
                    break;
                case eSadanaEvents::EventDaggerFall: /// Responsible for damage ticking, disabling the automatic damage from spelldbc
                    DoCastAOE(eSadanaSpells::SpellDaggerFallDamage);
                    events.ScheduleEvent(eSadanaEvents::EventDaggerFall, 1 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_sadana_creature_falling_daggerAI(p_Creature);
    }
};

/// Shadow Rune - 75778 [Responsible for the cosmetic shadow runes changing to lunars when eclipse starts]
class shadowmoon_burial_grounds_sadana_creature_shadow_rune : public CreatureScript
{
	public:

    shadowmoon_burial_grounds_sadana_creature_shadow_rune() : CreatureScript("shadowmoon_burial_grounds_sadana_creature_shadow_rune") {}

    struct shadowmoon_burial_grounds_sadana_creature_shadow_runeAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_sadana_creature_shadow_runeAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
            m_HasBeenActivated = false;
        }

        enum eShadowRuneSpells
        {
            SpellLunarPurityAreaTrigger        = 164974,
            SpellLunarPurityDamageModification = 162652,
            SpellShadowRitual                  = 152158,
            SpellLunarRitual                   = 164693,
            SpellShadowRuneVisual              = 152684,
            SpellShadowRuneVisual01            = 152691,
            SpellShadowRuneVisual02            = 152695,
            SpellLunarRuneVisual               = 164693,
            SpellLunarRuneVisual01             = 164695,
            SpellLunarRuneVisual02             = 164696
        };

        InstanceScript* m_Instance;
        bool m_HasBeenActivated;

        void Reset() override
        {
            events.Reset();
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            switch (urand(0, 2))
            {
                case 0:
                    me->CastSpell(me, eShadowRuneSpells::SpellShadowRuneVisual);
                    break;
                case 1:
                    me->CastSpell(me, eShadowRuneSpells::SpellShadowRuneVisual01);
                    break;
                case 2:
                    me->CastSpell(me, eShadowRuneSpells::SpellShadowRuneVisual02);
                    break;
                default:
                    break;
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eSadanaActions::ActionActivateLunarTriggersActivate:
                {
                    switch (urand(0, 2))
                    {
                        case 0:
                            me->CastSpell(me, eShadowRuneSpells::SpellLunarRuneVisual);
                            break;
                        case 1:
                            me->CastSpell(me, eShadowRuneSpells::SpellLunarRuneVisual01);
                            break;
                        case 2:
                            me->CastSpell(me, eShadowRuneSpells::SpellLunarRuneVisual02);
                            break;
                        default:
                            break;
                    }        
                    m_HasBeenActivated = true;
                    me->CastSpell(me, eShadowRuneSpells::SpellLunarPurityAreaTrigger);
                    for (uint8 l_I = 0; l_I < 3; l_I++)
                        me->RemoveAura(g_ShadowRuneEntries[l_I]);
                    break;
                }
                case eSadanaActions::ActionActivateLunarTriggersDeactivate:
                {
                    if (!m_HasBeenActivated)
                        return;

                    switch (urand(0, 2))
                    {
                        case 0:
                            me->CastSpell(me, eShadowRuneSpells::SpellShadowRuneVisual);
                            break;
                        case 1:
                            me->CastSpell(me, eShadowRuneSpells::SpellShadowRuneVisual01);
                            break;
                        case 2:
                            me->CastSpell(me, eShadowRuneSpells::SpellShadowRuneVisual02);
                            break;
                        default:
                            break;
                    }
                    m_HasBeenActivated = false;
                    me->RemoveAura(eShadowRuneSpells::SpellLunarRitual);                 
                    for (uint8 l_I = 0; l_I < 3; l_I++)
                        me->RemoveAura(g_LunarRuneEntries[l_I]);            
                    break;
                }
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_sadana_creature_shadow_runeAI(p_Creature);
    }
};

/// Eclipse Trigger - 76052 [Responsible for the eclipse trigger, searching targets when activated to grant them a buff
class shadowmoon_burial_grounds_sadana_creature_eclipse_trigger : public CreatureScript
{
	public:

    shadowmoon_burial_grounds_sadana_creature_eclipse_trigger() : CreatureScript("shadowmoon_burial_grounds_sadana_creature_eclipse_trigger") {}

    struct shadowmoon_burial_grounds_sadana_creature_eclipse_triggerAI : public Scripted_NoMovementAI
    {
        shadowmoon_burial_grounds_sadana_creature_eclipse_triggerAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eEclispeSpells
        {
            SpellLunarPurityDamageModification = 162652
        };

        InstanceScript* m_Instance;
        bool m_ReadyForAction;
        uint32 m_ActionDiff;

        void Reset() override
        {
            events.Reset();
            m_ReadyForAction = false;
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            m_ActionDiff = 2 * TimeConstants::IN_MILLISECONDS;
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eSadanaActions::ActionActivateLunarTriggersActivate:
                    m_ReadyForAction = true;
                    break;
                case eSadanaActions::ActionActivateLunarTriggersDeactivate:
                    m_ReadyForAction = false;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            ///< Lunar Purity
            if (m_ReadyForAction)
            {
                if (m_ActionDiff <= p_Diff)
                {
                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 1.2f);
                    if (!l_PlayerList.empty())
                    {
                        for (Player* l_Itr : l_PlayerList)
                            l_Itr->CastSpell(l_Itr, eEclispeSpells::SpellLunarPurityDamageModification);
                    }

                    m_ActionDiff = 2 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_ActionDiff -= p_Diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new shadowmoon_burial_grounds_sadana_creature_eclipse_triggerAI(p_Creature);
    }
};

/// Shadow Burn - 153224 [Ticking damage spell from daggerfall proc]
class shadowmoon_burial_grounds_sadana_spell_shadow_burn : public SpellScriptLoader
{
	public:

    shadowmoon_burial_grounds_sadana_spell_shadow_burn() : SpellScriptLoader("shadowmoon_burial_grounds_sadana_spell_shadow_burn") { }

    class shadowmoon_burial_grounds_sadana_spell_shadow_burn_SpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_sadana_spell_shadow_burn_SpellScript);

        SpellCastResult CheckCaster()
        {
            if (!GetCaster())
                return SpellCastResult::SPELL_FAILED_DONT_REPORT; 

            if (GetCaster()->GetPositionZ() > 262.0f)
                return SpellCastResult::SPELL_FAILED_DONT_REPORT;

            return SpellCastResult::SPELL_CAST_OK;        
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(shadowmoon_burial_grounds_sadana_spell_shadow_burn_SpellScript::CheckCaster);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_sadana_spell_shadow_burn_SpellScript;
    }
};

/// Dark Communion - 153153  [ Responisble for making the ghost stop moving, and start following the boss ]
class shadowmoon_burial_grounds_sadana_spell_dark_communion : public SpellScriptLoader
{
	public:

    shadowmoon_burial_grounds_sadana_spell_dark_communion() : SpellScriptLoader("shadowmoon_burial_grounds_sadana_spell_dark_communion") { }

    class shadowmoon_burial_grounds_sadana_spell_dark_communion_AuraScript : public AuraScript
    {
        PrepareAuraScript(shadowmoon_burial_grounds_sadana_spell_dark_communion_AuraScript);

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetTarget())
                return;

            if (!GetCaster())
                return;

            if (GetTarget()->IsAIEnabled)
                GetTarget()->GetAI()->DoAction(eSadanaActions::ActionActivateDefiledSpirit);
        }

        void OnRemove(AuraEffect const*/*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->GetTypeId() == TypeID::TYPEID_PLAYER)
                        return;

                    l_Target->ToCreature()->DespawnOrUnsummon();
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                    {
                        if (Creature* l_Sadana = l_Instance->instance->GetCreature(l_Instance->GetData64(eShadowmoonBurialGroundsDatas::DataBossSadana)))
                        {
                            if (l_Sadana->IsAIEnabled)
                            {
                                if (boss_sadana_bloodfury::boss_sadana_bloodfuryAI* l_LinkAI = CAST_AI(boss_sadana_bloodfury::boss_sadana_bloodfuryAI, l_Sadana->GetAI()))
                                {
                                    if (l_LinkAI && l_LinkAI->m_CommunionInAction)
                                        l_LinkAI->m_CommunionInAction = false;
                                }
                            }
                        }
                    }
                }
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(shadowmoon_burial_grounds_sadana_spell_dark_communion_AuraScript::OnApply,    SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(shadowmoon_burial_grounds_sadana_spell_dark_communion_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new shadowmoon_burial_grounds_sadana_spell_dark_communion_AuraScript();
    }
};

/// Dark Eclipse - 164685  [Handles the trigger deactivation, and somethign with the damage]
class shadowmoon_burial_grounds_sadana_spell_dark_eclipse : public SpellScriptLoader
{
	public:
    
    shadowmoon_burial_grounds_sadana_spell_dark_eclipse() : SpellScriptLoader("shadowmoon_burial_grounds_sadana_spell_dark_eclipse") { }

    class shadowmoon_burial_grounds_sadana_spell_dark_eclipse_AuraScript : public AuraScript
    {
        PrepareAuraScript(shadowmoon_burial_grounds_sadana_spell_dark_eclipse_AuraScript);

        void OnApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (!GetCaster())
                return;

            GetCaster()->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void HandlePeriodic(AuraEffect const* p_AurEff)
        {
            PreventDefaultAction();
            if (GetCaster())
            {
                if (!GetSpellInfo())
                    return;

                GetCaster()->CastSpell(GetCaster(), eSadanaSpells::SpellDarkEclipseDamage);
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(shadowmoon_burial_grounds_sadana_spell_dark_eclipse_AuraScript::OnApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            OnEffectPeriodic += AuraEffectPeriodicFn(shadowmoon_burial_grounds_sadana_spell_dark_eclipse_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new shadowmoon_burial_grounds_sadana_spell_dark_eclipse_AuraScript();
    }
};

/// Dark Eclipse damage - 164686 [Makes every damage coming from eclipse set to 0 if you have the eclipse immunity from standing in the lunars]
class shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damage : public SpellScriptLoader
{
	public:

    shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damage() : SpellScriptLoader("shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damage") { }

    class shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damageSpellScript : public SpellScript
    {
        PrepareSpellScript(shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damageSpellScript);

        enum eEclipseSpells
        {
            SpellLunaryPurtiyBuff = 162652
        };

        void HandleDamage(SpellEffIndex /*p_EffIndex*/)
        {
            if (!GetCaster() || !GetHitUnit())
                return;

            if (GetHitUnit()->HasAura(eEclipseSpells::SpellLunaryPurtiyBuff))
                SetHitDamage(0);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damageSpellScript::HandleDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damageSpellScript();
    }
};

void AddSC_boss_sadana()
{   
    new boss_sadana_bloodfury();                                      ///< 75509
    new shadowmoon_burial_grounds_sadana_creature_shadow_rune();      ///< 75778
    new shadowmoon_burial_grounds_sadana_creature_defiled_spirit();   ///< 75966
    new shadowmoon_burial_grounds_sadana_creature_falling_dagger();   ///< 75981
    new shadowmoon_burial_grounds_sadana_creature_eclipse_trigger();  ///< 76052
    new shadowmoon_burial_grounds_sadana_spell_shadow_burn();         ///< 153224
    new shadowmoon_burial_grounds_sadana_spell_dark_eclipse();        ///< 164685
    new shadowmoon_burial_grounds_sadana_spell_dark_communion();      ///< 153153
    new shadowmoon_burial_grounds_sadana_spell_dark_eclipse_damage(); ///< 16468
}