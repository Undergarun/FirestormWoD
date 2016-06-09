
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL 
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eWitherbarkActions
{
    ///< Witherbark
    ActionBrittleEffect         = 1,
    ActionBrittleDeffect,
    ActionEnergyPower,
    ///< Magic Waters
    ActionActivateWaterGlobules,
    ActionDeactiveWaterGlobules
};

enum eWitherbarkCreatures
{
    CreatureWaterGlobule          = 81821,
    CreatureEnchantedWatersDelete = 88862,
    CreatureUncheckedGrowth       = 81737
};

enum eWitherbarkTalks
{
    WitherbarkAggro   = 4, ///< So... Thirsty(45108)
    WitherbarkKill01  = 5, ///< Return to the earth...(45110)
    WitherbarkKill02  = 6, ///< Feed my roots.(45111) 
    WitherbarkSpell01 = 7, ///< Refreshed.(45112)
    WitherbarkSpell02 = 8, ///< I Cannot Be Stopped.(45113)
    WitherbarkDeath   = 9  ///< Arghh...(45109)
};

///< Positions
Position const g_PositionAqueoursMobs[4] =
{
    { 410.45f, 1588.56f, 89.078f, 5.205070f }, 
    { 474.81f, 1638.33f, 87.651f, 0.382940f },
    { 434.33f, 1661.58f, 87.561f, 1.227150f },
    { 387.49f, 1650.11f, 87.651f, 2.288160f } 
};

Position const g_PositionEnchantedMobs[10] =
{
    { 474.81f, 1638.33f, 87.567f, 0.000000f }, 
    { 458.76f, 1648.68f, 87.567f, 0.000000f }, 
    { 443.61f, 1649.07f, 87.567f, 0.000000f }, 
    { 434.33f, 1661.58f, 87.567f, 0.000000f }, 
    { 422.24f, 1652.27f, 87.567f, 0.000000f }, 
    { 404.96f, 1654.05f, 87.567f, 0.000000f }, 
    { 387.49f, 1650.11f, 87.567f, 0.000000f }, 
    { 376.22f, 1636.72f, 87.567f, 0.000000f }, 
    { 369.48f, 1628.33f, 87.567f, 0.000000f }, 
    { 349.98f, 1594.22f, 87.567f, 0.000000f }  
};

Position const g_PositionWitherbark         = { 417.79f, 1615.38f, 89.377f, 4.825330f }; ///< Witherbark

Position const g_PositionUnchekedGrowth     = { 418.44f, 1616.12f, 89.292f, 4.434609f }; ///< UNCHECKED_GROWTH

Position const g_PositionNaturalistFollower = { 440.251f, 1574.739f, 88.130f, 3.615971f };

Position const g_PositionCosmeticGlobules[3] =
{
    { 408.211f, 1589.37f, 87.908f, 0.988586f },
    { 440.903f, 1609.97f, 88.661f, 2.791076f },
    { 404.258f, 1628.98f, 88.034f, 5.571381f }
};

Position const g_PositionNaturalistFollowerMov[2] =
{
    { 403.718f, 1563.935f, 88.180f, 2.955451f },
    { 457.685f, 1591.681f, 86.146f, 1.012375f }
};

/// Witherbark - 81522
class boss_witherbark : public CreatureScript
{
public:

    boss_witherbark() : CreatureScript("boss_witherbark") { }

    struct boss_witherbarkAI : public BossAI
    {
        boss_witherbarkAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataWitherbark)
        {
            m_Instance = me->GetInstanceScript();
            m_Counting = 0;
            m_Intro = false;
            m_First = true;
        }

        enum eWitherbarkEvents
        {
            ///< Witherbark
            EventBrittleBark = 1,
            EventParchedGrasp,
            EventUncheckedGrowth,
            EventAgitatedWater,
            EventAqueousGlobules,
            EventBarrageOfLeaves
        };

        enum eWitherbarkSpells
        {
            ///< Witherbark
            SpellBrittleBarkAura       = 164275, ///< STUN
            SpellBrittleBarkRemoveAura = 164718, ///< remove the stun
            SpellParchedGasp           = 164357,
            SpellPertifiedBark         = 164713,
            SpellVisualSubmerge        = 177694,
            SpellLivingLeavesDummy     = 169494,
            SpellCosmeticRoot          = 164715,
            SpellSolarChannel          = 170594
        };

        InstanceScript* m_Instance;
        uint32 m_Counting;
        uint32 m_CountPerRegeneration;
        uint32 m_Power;
        bool   m_Intro;
        bool   m_Acheivement;
        bool   m_First;
  
        void Reset() override
        {
            _Reset();
            events.Reset();
            m_Power = 100;
            me->RemoveAllAuras();
            m_Acheivement = true;
            me->SetHealth(me->GetMaxHealth());    
            me->SetPower(Powers::POWER_MANA, 100);
            me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, 100);
            me->SetMaxPower(Powers::POWER_MANA, 100);
            me->SetInt32Value(EUnitFields::UNIT_FIELD_MAX_POWER, 100);
            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);
            me->AddAura(eWitherbarkSpells::SpellPertifiedBark, me);
            if (m_First)
            {
                SolarVisual();
                m_First = false;
                me->AddAura(eWitherbarkSpells::SpellBrittleBarkAura, me);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }
        }

        void SolarVisual()
        {
            for (uint8 l_I = 0; l_I < 3; l_I++)
            {
                if (Creature* l_Cosmetic = me->SummonCreature(eWitherbarkCreatures::CreatureWaterGlobule, g_PositionCosmeticGlobules[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                {
                    l_Cosmetic->AddUnitState(UnitState::UNIT_STATE_CANNOT_AUTOATTACK);
                    l_Cosmetic->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                    l_Cosmetic->CastSpell(l_Cosmetic, eWitherbarkSpells::SpellCosmeticRoot); ///< COSMETIC ROOTS
                }
            }

            std::list<Creature*> l_TenderGuidsList;
            me->GetCreatureListWithEntryInGrid(l_TenderGuidsList, eEverbloomCreature::CreatureEverbloomTender, 200.0f);
            if (l_TenderGuidsList.empty())
                return;

            for (Creature* l_Itr : l_TenderGuidsList)
                l_Itr->CastSpell(l_Itr, eWitherbarkSpells::SpellSolarChannel);
        }

        void JustReachedHome() override
        {
            _JustReachedHome();
            DespawnCreaturesInArea(eEverbloomCreature::CreatureEnchanctedWater, me);
            DespawnCreaturesInArea(eWitherbarkCreatures::CreatureWaterGlobule, me);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eWitherbarkActions::ActionEnergyPower:
                {
                    if (!me->isInCombat())
                        return;

                    m_Acheivement = false;
                    if (m_Power < 100)
                        m_Power += 10;
                    break;
                }
                case eEverbloomActions::ActionCountPre1StBossKill:
                {
                    m_Counting++;
                    if (m_Counting >= 6 && !m_Intro)
                    {
                        m_Intro = true;
                        me->RemoveAllAuras();
                        me->setFaction(HostileFaction);
                        me->SetReactState(ReactStates::REACT_DEFENSIVE);                            
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_UNK_15 | eUnitFlags::UNIT_FLAG_UNK_6);
                    }
                    break;
                }
                case eWitherbarkActions::ActionBrittleEffect:
                {
                    if (!me->isInCombat())
                        return;

                    events.Reset();
                    Talk(eWitherbarkTalks::WitherbarkSpell02);
                    me->MonsterTextEmote("|TInterface\\Icons\\inv_misc_embers.blp:20|t %s is completely dehydrated and has |cFFFF0404|Hspell:164275|h[Brittle Bark]|h|r!", me->GetGUID(), true);
                    std::list<Creature*> l_EnchantedWatersList;
                    me->GetCreatureListWithEntryInGrid(l_EnchantedWatersList, eEverbloomCreature::CreatureEnchanctedWater, 200.0f);
                    if (l_EnchantedWatersList.empty())
                        return;

                    for (Creature* l_Itr : l_EnchantedWatersList)
                    {
                        if (l_Itr->IsAIEnabled)
                            l_Itr->GetAI()->DoAction(eWitherbarkActions::ActionActivateWaterGlobules);
                    }
                    break;
                }
                case eWitherbarkActions::ActionBrittleDeffect:
                {
                    if (!me->isInCombat())
                        return;
                  
                    events.Reset();
                    Talk(eWitherbarkTalks::WitherbarkSpell01);
                    events.ScheduleEvent(eWitherbarkEvents::EventParchedGrasp, 8000);
                    events.ScheduleEvent(eWitherbarkEvents::EventUncheckedGrowth, 10000);
                    me->MonsterTextEmote("Witherbark has absorbed enough water and become active again!", me->GetGUID(), true);
                    std::list<Creature*> l_WateryCreaturesList;
                    me->GetCreatureListWithEntryInGrid(l_WateryCreaturesList, eEverbloomCreature::CreatureEnchanctedWater, 300.0f);
                    if (!l_WateryCreaturesList.empty())
                    {
                        for (Creature* l_Itr : l_WateryCreaturesList)
                        {
                            if (l_Itr->IsAIEnabled)
                                l_Itr->GetAI()->DoAction(eWitherbarkActions::ActionDeactiveWaterGlobules);
                        }
                    }
                    break;
                }
                default:
                    break;
            }
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();
            Talk(eWitherbarkTalks::WitherbarkAggro);
            me->RemoveAura(eWitherbarkSpells::SpellBrittleBarkAura);
            me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);
            m_CountPerRegeneration = 4 * TimeConstants::IN_MILLISECONDS;
            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);
            events.ScheduleEvent(eWitherbarkEvents::EventParchedGrasp,       8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eWitherbarkEvents::EventUncheckedGrowth,    12 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eWitherbarkEvents::EventBarrageOfLeaves,    10 * TimeConstants::IN_MILLISECONDS);
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);         
                DoZoneInCombat();
            }
            for (uint8 l_I = 0; l_I < 8; l_I++)
                me->SummonCreature(eEverbloomCreature::CreatureEnchanctedWater, g_PositionEnchantedMobs[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
            {
                if (roll_chance_i(50))
                    Talk(eWitherbarkTalks::WitherbarkKill01);
                else
                    Talk(eWitherbarkTalks::WitherbarkKill02);
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

            if (m_Instance != nullptr)
            {
                if (m_Acheivement)
                {
                    if (m_Instance->instance && !m_Instance->instance->IsHeroic())
                        m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchivementWaterManagement);
                }
            }

            Talk(eWitherbarkTalks::WitherbarkDeath);
            DespawnCreaturesInArea(eWitherbarkCreatures::CreatureWaterGlobule, me);
            DespawnCreaturesInArea(eEverbloomCreature::CreatureEnchanctedWater, me);
        }

        void RegeneratePower(Powers p_Power, int32& p_Value) override
        {
            if (!me->GetPower(Powers::POWER_MANA))
                return;

            int32 l_Val = me->GetPower(Powers::POWER_MANA);
            me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, l_Val);
        }

        void BrittleRegeneration(uint32 const p_Diff)  /// Forces the the boss to show the somewhat faked energy, no idea why but the regular missionary function of handling energy is shit and doesn't respond the way its needed
        {
            me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, m_Power);
            if (m_Power <= 0 && !me->HasAura(eWitherbarkSpells::SpellBrittleBarkAura))
                me->AddAura(eWitherbarkSpells::SpellBrittleBarkAura, me);
            if (me->HasAura(eWitherbarkSpells::SpellBrittleBarkAura))
            {
                if (m_Power >= 100)
                 me->RemoveAura(eWitherbarkSpells::SpellBrittleBarkAura);
            }
            else
            {
                if (m_CountPerRegeneration <= p_Diff)
                {
                    if (m_Power >= 10)
                    {
                        int32 l_Val = 0;
                        l_Val = m_Power - 10;
                        m_Power -= 10;
                        me->SetPower(Powers::POWER_MANA, l_Val);
                        me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, l_Val);   
                    }

                    m_CountPerRegeneration = 3 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_CountPerRegeneration -= p_Diff;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);  

            BrittleRegeneration(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eWitherbarkEvents::EventParchedGrasp:
                {
                    me->CastSpell(me, eWitherbarkSpells::SpellParchedGasp);
                    events.ScheduleEvent(eWitherbarkEvents::EventParchedGrasp, 10 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eWitherbarkEvents::EventBarrageOfLeaves:
                {
                    me->CastSpell(me, eWitherbarkSpells::SpellLivingLeavesDummy);
                    events.ScheduleEvent(eWitherbarkEvents::EventBarrageOfLeaves, 24 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eWitherbarkEvents::EventUncheckedGrowth:
                {
                    if (m_Instance != nullptr)
                    {
                        if (m_Instance->instance && !m_Instance->instance->IsHeroic() && me->HasAura(eWitherbarkSpells::SpellBrittleBarkAura))
                            return;

                        Position l_Position;
                        me->GetRandomNearPosition(l_Position, 15.0f);
                        me->SummonCreature(eWitherbarkCreatures::CreatureUncheckedGrowth, l_Position, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(eWitherbarkEvents::EventUncheckedGrowth, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_witherbarkAI(p_Creature);
    }
};

/// Everbloom Naturalist - 81819
class the_everbloom_witherbark_mob_naturalist : public CreatureScript
{
	public:

    the_everbloom_witherbark_mob_naturalist() : CreatureScript("the_everbloom_witherbark_mob_naturalist") { }

    struct the_everbloom_witherbark_mob_naturalistAI : public ScriptedAI
    {
        the_everbloom_witherbark_mob_naturalistAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eEverbloomNaturalistEvents
        {
            EventBarrageOfLeaves = 1
        };

        enum eEverbloomNaturalistSpells
        {
            SpellBarrageOfLeaves = 164834,
            SpellSolarChannel    = 170594
        };

        InstanceScript* m_Instance;

        void Reset() override
        {
            events.Reset();
            me->CastSpell(me, eEverbloomNaturalistSpells::SpellSolarChannel);
        }

        void RandomSayings()
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataWitherbark)))
                {
                    if (l_Witherbark->IsWithinDistInMap(me, 30.0f))
                    {
                        switch (urand(0, 2))
                        {
                            case 0:
                                me->MonsterYell("We have not yet purified the water!", Language::LANG_UNIVERSAL, me->GetGUID());
                                break;
                            case 1:
                                me->MonsterYell("We are all... doomed", Language::LANG_UNIVERSAL, me->GetGUID());
                                break;
                            case 2:
                                me->MonsterYell("We have not yet purified the water!", Language::LANG_UNIVERSAL, me->GetGUID());
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            /// Cosmetic Aquous burst.
            if (Creature* l_NearestAquousGlobule = me->FindNearestCreature(eWitherbarkCreatures::CreatureWaterGlobule, 3.0f, true))
                me->Kill(l_NearestAquousGlobule);

            RandomSayings();
            me->RemoveAura(eEverbloomNaturalistSpells::SpellSolarChannel);
            DespawnCreaturesInArea(eEverbloomCreature::CreatureGlobuleWater, me);
            events.ScheduleEvent(eEverbloomNaturalistEvents::EventBarrageOfLeaves, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataWitherbark)))
                    if (l_Witherbark->IsWithinDistInMap(me, 30.0f) && l_Witherbark->IsAIEnabled)
                        l_Witherbark->AI()->DoAction(eEverbloomActions::ActionCountPre1StBossKill);
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);
        
            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eEverbloomNaturalistEvents::EventBarrageOfLeaves:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 45.0f, true))
                            me->CastSpell(l_Target, eEverbloomNaturalistSpells::SpellBarrageOfLeaves);
                        events.ScheduleEvent(eEverbloomNaturalistEvents::EventBarrageOfLeaves, 7 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_witherbark_mob_naturalistAI(p_Creature);
    }
};

/// Agitated Waters - 88862
class the_everbloom_witherbark_mob_agitated_water : public CreatureScript
{
	public:

    the_everbloom_witherbark_mob_agitated_water() : CreatureScript("the_everbloom_witherbark_mob_agitated_water") {}

    struct the_everbloom_witherbark_mob_agitated_waterAI : public Scripted_NoMovementAI
    {
        the_everbloom_witherbark_mob_agitated_waterAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eAgitatedWatersEvents
        {
            EventAqueousGlobules = 1,
            EventShootWater
        };

        enum eAgitatedWatersSpells
        {
            SpellShootWater = 177731
        };

        InstanceScript* m_Instance;
        uint32 m_Interval;
        bool m_StartEvent;

        void Reset() override
        {
            events.Reset();
            me->SetInCombatWithZone();
            me->setFaction(HostileFaction);
			me->SetDisplayId(InvisibleDisplay);
            m_Interval = 20 * TimeConstants::IN_MILLISECONDS;
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);      
            events.ScheduleEvent(eAgitatedWatersEvents::EventShootWater, urand(4 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eWitherbarkActions::ActionActivateWaterGlobules:
                    m_Interval = 20 * TimeConstants::IN_MILLISECONDS;
                    events.CancelEvent(eAgitatedWatersEvents::EventAqueousGlobules);
                    events.ScheduleEvent(eAgitatedWatersEvents::EventAqueousGlobules, urand(3 * TimeConstants::IN_MILLISECONDS, 7 * TimeConstants::IN_MILLISECONDS));
                    break;
                case eWitherbarkActions::ActionDeactiveWaterGlobules:
                    events.CancelEvent(eAgitatedWatersEvents::EventAqueousGlobules);
                    DespawnCreaturesInArea(eWitherbarkCreatures::CreatureWaterGlobule, me);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAgitatedWatersEvents::EventAqueousGlobules:
                        if (roll_chance_i(40))
                            me->SummonCreature(eWitherbarkCreatures::CreatureWaterGlobule, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        m_Interval -= 3 * TimeConstants::IN_MILLISECONDS;
                        events.ScheduleEvent(eAgitatedWatersEvents::EventAqueousGlobules, irand(m_Interval, m_Interval + (2 * TimeConstants::IN_MILLISECONDS)));
                        break;
                case eAgitatedWatersEvents::EventShootWater:
                        if (!UpdateVictim())
                            return;

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eAgitatedWatersSpells::SpellShootWater);
                        events.ScheduleEvent(eAgitatedWatersEvents::EventShootWater, urand(14 * TimeConstants::IN_MILLISECONDS, 70 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_witherbark_mob_agitated_waterAI(p_Creature);
    }
};

/// Aqueous Globue  - 81821
class the_everbloom_witherbark_mob_aqueous_globule : public CreatureScript
{
	public:

    the_everbloom_witherbark_mob_aqueous_globule() : CreatureScript("the_everbloom_witherbark_mob_aqueous_globule") {}

    struct the_everbloom_witherbark_mob_aqueous_globuleAI : public ScriptedAI
    {
        the_everbloom_witherbark_mob_aqueous_globuleAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eAqueousGlobuleSpells
        {
            SpellAqueousBurst    = 164538,
            SpellGlobueCosmetic  = 164391,
            SpellShootWater      = 177731,
            SpellBrittleBarkAura = 164275
        };

        InstanceScript* m_Instance;
        uint32 m_DiffCheckup;
        bool m_Caught;

        void Reset() override
        {
            events.Reset();
            m_Caught = false;
            me->setFaction(HostileFaction);
            me->SetDisplayId(InvisibleDisplay);
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.4f);
            me->SetReactState(ReactStates::REACT_PASSIVE);
            m_DiffCheckup = 1 * TimeConstants::IN_MILLISECONDS;
            me->AddUnitState(UnitState::UNIT_STATE_CANNOT_AUTOATTACK);
            me->CastSpell(me, eAqueousGlobuleSpells::SpellGlobueCosmetic);

            if (m_Instance != nullptr)
            {
                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataWitherbark)))
                {
                    me->SetFacingToObject(l_Witherbark);

                    float l_X = me->m_positionX + 18 * cos(me->m_orientation);
                    float l_Y = me->m_positionY + 18 * sin(me->m_orientation);
                    me->GetMotionMaster()->MoveJump(0, l_X, l_Y, 86.871f);
                }
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->CastSpell(me, eAqueousGlobuleSpells::SpellAqueousBurst);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (m_Instance != nullptr)
            { 
                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataWitherbark)))
                {
                    if (l_Witherbark->isInCombat() && l_Witherbark->HasAura(eAqueousGlobuleSpells::SpellBrittleBarkAura))
                    {
                        if (!me->isMoving()) /// Automatically forces the aqueous globule to get to Witherbark location.
                            me->GetMotionMaster()->MovePoint(0, *l_Witherbark);

                        if (m_DiffCheckup <= p_Diff && !m_Caught) // Checks radius between Witherbark and current globule and increases water power incase of contact.
                        {
                            if (l_Witherbark->IsWithinDistInMap(me, 3.0f))
                            {
                                if (l_Witherbark->IsAIEnabled)
                                {
                                    int32 l_Val = l_Witherbark->GetPower(Powers::POWER_MANA);
                                    l_Val += 25;
                                    m_Caught = true;
                                    l_Witherbark->AI()->DoAction(eWitherbarkActions::ActionEnergyPower);
                                    l_Witherbark->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, l_Val);
                                    me->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);
                                }
                            }

                            m_DiffCheckup = 1 * TimeConstants::IN_MILLISECONDS;
                        }
                        else
                            m_DiffCheckup -= p_Diff;
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_witherbark_mob_aqueous_globuleAI(p_Creature);
    }
};

/// Unchecked Growth - 81737
class the_everbloom_witherbark_mob_unchecked_growth : public CreatureScript
{
	public:

    the_everbloom_witherbark_mob_unchecked_growth() : CreatureScript("the_everbloom_witherbark_mob_unchecked_growth") {}

    struct the_everbloom_witherbark_mob_unchecked_growthAI : public ScriptedAI
    {
        the_everbloom_witherbark_mob_unchecked_growthAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = p_Creature->GetInstanceScript();
        }

        enum eUncheckedGrowthSpells
        {
            SpellDustyCloud      = 174764,
            SpellVisualSubmerge  = 177694,
            SpellUncheckedGrowth = 164294
        };
        
        enum eUncheckedGrowthEvents
        {
            EventMerge = 1
        };

        InstanceScript* m_Instance;
        uint32 m_DiffCheckup;
        bool m_Active;

        void Reset() override
        {
            events.Reset();                   
            m_Active = false;
            me->SetReactState(ReactStates::REACT_PASSIVE);
            m_DiffCheckup = 1 * TimeConstants::IN_MILLISECONDS;    
            me->AddAura(eUncheckedGrowthSpells::SpellDustyCloud, me);
            me->AddAura(eUncheckedGrowthSpells::SpellVisualSubmerge, me);
            events.ScheduleEvent(eUncheckedGrowthEvents::EventMerge, 10 * TimeConstants::IN_MILLISECONDS); /// Submerge duration.
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);  
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            me->DespawnOrUnsummon();
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!m_Active)
            {
                if (m_DiffCheckup <= p_Diff) /// Unchecked Growth dot when npc is doorment.
                {
                    std::list<Player*> l_ListPlayers;
                    me->GetPlayerListInGrid(l_ListPlayers, 1.20f);
                    if (!l_ListPlayers.empty())
                    {
                        for (Player* l_Itr : l_ListPlayers)
                        {
                            if (!l_Itr)
                                continue;

                            l_Itr->CastSpell(l_Itr, eUncheckedGrowthSpells::SpellUncheckedGrowth);
                        }
                    }

                    m_DiffCheckup = 1 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_DiffCheckup -= p_Diff;
            }

            events.Update(p_Diff);

            switch (events.ExecuteEvent())
            {
                case eUncheckedGrowthEvents::EventMerge:
                    m_Active = true;
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveAura(eUncheckedGrowthSpells::SpellDustyCloud);
                    me->RemoveAura(eUncheckedGrowthSpells::SpellVisualSubmerge);   
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    break;
                default:
                    break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_witherbark_mob_unchecked_growthAI(p_Creature);
    }
};

/// Living Leaves - 324266
class the_everbloom_witherbark_mob_living_leaves : public CreatureScript
{
	public:

    the_everbloom_witherbark_mob_living_leaves() : CreatureScript("the_everbloom_witherbark_mob_living_leaves") { }

    struct the_everbloom_witherbark_mob_living_leavesAI : public ScriptedAI
    {
        the_everbloom_witherbark_mob_living_leavesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        enum eLivingLeavesSpells
        {
            SpellLivingLeavesEffect      = 169495,
            SpellLivingLeavesAreaTrigger = 169497
        };

        InstanceScript* m_Instance;
        uint32 m_Time;

        void Reset() override
        {
            events.Reset();
            m_Time = 2 * TimeConstants::IN_MILLISECONDS;
            me->CastSpell(me, eLivingLeavesSpells::SpellLivingLeavesAreaTrigger);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            events.Update(p_Diff);

            if (m_Time <= p_Diff)
            {
                std::list<Player*> l_PlayerList;
                me->GetPlayerListInGrid(l_PlayerList, 2.0f);
                if (!l_PlayerList.empty())
                {
                    for (Player* l_Itr : l_PlayerList)
                    {
                        if (!l_Itr->HasAura(eLivingLeavesSpells::SpellLivingLeavesEffect))
                            l_Itr->CastSpell(l_Itr, eLivingLeavesSpells::SpellLivingLeavesEffect, true);
                    }
                }

                m_Time = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Time -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_witherbark_mob_living_leavesAI(p_Creature);
    }
};

/// Brittle Bark - 164275
class the_everbloom_witherbark_aura_brittle_bark : public SpellScriptLoader
{
public:

    the_everbloom_witherbark_aura_brittle_bark() : SpellScriptLoader("the_everbloom_witherbark_aura_brittle_bark") { }

    class the_everbloom_witherbark_aura_brittle_bark_AuraScript : public AuraScript
    {
        PrepareAuraScript(the_everbloom_witherbark_aura_brittle_bark_AuraScript);

        void OnApply(AuraEffect const*/*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/) 
        { 
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsInWorld() && l_Caster->isInCombat() && l_Caster->IsAIEnabled)
                    GetCaster()->GetAI()->DoAction(eWitherbarkActions::ActionBrittleEffect);
            }
        }

        void OnRemove(AuraEffect const*/*p_aurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (l_Caster->IsInWorld() && l_Caster->isInCombat() && l_Caster->IsAIEnabled)
                    GetCaster()->GetAI()->DoAction(eWitherbarkActions::ActionBrittleDeffect);
            }
        }

        void Register()
        {
            AfterEffectApply += AuraEffectApplyFn(the_everbloom_witherbark_aura_brittle_bark_AuraScript::OnApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_STUN, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            AfterEffectRemove += AuraEffectRemoveFn(the_everbloom_witherbark_aura_brittle_bark_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_STUN, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new the_everbloom_witherbark_aura_brittle_bark_AuraScript();
    }
};

/// Agitated Waters - 177731 
class the_everbloom_witherbark_spell_agitated_waters : public SpellScriptLoader
{
public:

    the_everbloom_witherbark_spell_agitated_waters() : SpellScriptLoader("the_everbloom_witherbark_spell_agitated_waters") {}

    class the_everbloom_witherbark_spell_agitated_waters_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_witherbark_spell_agitated_waters_SpellScript);

        SpellCastResult CheckCast()
        {
            if (GetCaster())
            {       
                if (InstanceScript* l_Instance = GetCaster()->GetInstanceScript())
                {
                    if (Creature* l_Witherbark = l_Instance->instance->GetCreature(l_Instance->GetData64(eEverbloomData::DataWitherbark)))
                    {
                        if (!l_Witherbark->isInCombat())
                            return SpellCastResult::SPELL_FAILED_DONT_REPORT;

                        if (!l_Witherbark->isAlive())
                            return SpellCastResult::SPELL_FAILED_DONT_REPORT;
                    }
                }
            }

            return SpellCastResult::SPELL_CAST_OK;
        }

        void Register() override
        {
            OnCheckCast += SpellCheckCastFn(the_everbloom_witherbark_spell_agitated_waters_SpellScript::CheckCast);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_witherbark_spell_agitated_waters_SpellScript;
    }
};

/// Living Leaves - 169494
class the_everbloom_witherbark_spell_living_leaves_dummy : public SpellScriptLoader
{
public:

    the_everbloom_witherbark_spell_living_leaves_dummy() : SpellScriptLoader("the_everbloom_witherbark_spell_living_leaves_dummy") { }

    class the_everbloom_spell_living_leaves_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_spell_living_leaves_dummy_SpellScript);

        void HandleDummy(SpellEffIndex p_EffIndex)
        {
            if (!GetCaster())
                return;

            for (uint8 l_I = 0; l_I < 5; l_I++)
            {
                Position l_Pos;
                GetCaster()->GetRandomNearPosition(l_Pos, 15.0f);
                GetCaster()->SummonCreature(eEverbloomCreature::CreatureLivingLeaves, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 15 * TimeConstants::IN_MILLISECONDS);
            }
        }

        void Register()
        {
            OnEffectLaunch += SpellEffectFn(the_everbloom_spell_living_leaves_dummy_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_spell_living_leaves_dummy_SpellScript();
    }
};

/// Living Leaves - 169497
class the_everbloom_witherbark_areatrigger_living_leaves : public AreaTriggerEntityScript
{
public:

    the_everbloom_witherbark_areatrigger_living_leaves() : AreaTriggerEntityScript("the_everbloom_witherbark_areatrigger_living_leaves")
    {
    }

    enum eLivingLeavesSpells
    {
        SpellLivingLeavesEffect = 169495,
        SpellLivingLeavesAreaTrigger = 169497
    };

    uint32 p_Diff = 1 * TimeConstants::IN_MILLISECONDS;
    std::list<uint64> m_Targets;

    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (p_Diff <= p_Time)
        {
            std::list<Player*> l_PlayerList;
            JadeCore::AnyPlayerInObjectRangeCheck check(p_AreaTrigger, 1.2f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> searcher(p_AreaTrigger, l_PlayerList, check);
            p_AreaTrigger->VisitNearbyObject(1.2f, searcher);
            if (!l_PlayerList.empty())
            {
                for (std::list<Player*>::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (!(*l_Itr)->HasAura(eLivingLeavesSpells::SpellLivingLeavesEffect))
                    {
                        ((*l_Itr)->AddAura(eLivingLeavesSpells::SpellLivingLeavesEffect, (*l_Itr)));
                        m_Targets.push_back((*l_Itr)->GetGUID());
                    }
                }
            }

            p_Diff = 2 * TimeConstants::IN_MILLISECONDS;
        }
        else
            p_Diff -= p_Time;
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time)
    {
        if (m_Targets.empty())
            return;

        for (uint64 l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(eLivingLeavesSpells::SpellLivingLeavesEffect))
                l_Target->RemoveAura(eLivingLeavesSpells::SpellLivingLeavesEffect);
        }
    }

    the_everbloom_witherbark_areatrigger_living_leaves* GetAI() const override
    {
        return new the_everbloom_witherbark_areatrigger_living_leaves();
    }
};

void AddSC_boss_witherbark()
{
    new boss_witherbark();                                    ///< 81522
    new the_everbloom_witherbark_mob_agitated_water();        ///< 88862
    new the_everbloom_witherbark_mob_aqueous_globule();       ///< 81821
    new the_everbloom_witherbark_mob_naturalist();            ///< 81819
    new the_everbloom_witherbark_mob_unchecked_growth();      ///< 81737
    new the_everbloom_witherbark_mob_living_leaves();         ///< 324266
    new the_everbloom_witherbark_aura_brittle_bark();         ///< 164275
    new the_everbloom_witherbark_spell_agitated_waters();     ///< 177731
    new the_everbloom_witherbark_spell_living_leaves_dummy(); ///< 169494
    new the_everbloom_witherbark_areatrigger_living_leaves(); ///< 169497
}