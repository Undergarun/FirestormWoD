
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eWitherbarkEvents
{
    /// Witherbark
    EventBrittleBark = 1,
    EventParchedGrasp,
    EventUncheckedGrowth,
    EventAgitatedWater,
    EventAqueousGlobules,
    EventShootWater
};

enum eWitherbarkSpells
{
    /// Witherbark
    SpellBrittleBarkAura       = 164275, ///< STUN
    SpellBrittleBarkRemoveAura = 164718, ///< remove the stun
    SpellParchedGasp           = 164357,
    SpellAqueousBurst          = 164538,
    SpellGlobueCosmetic        = 164391,
    SpellShootWater            = 177731,
    SpellPertifiedBark         = 164713
};

enum eWitherbarkActions
{
    /// Witherbark
    ActionBrittleEffect         = 10,
    ActionBrittleDeffect,
    ActionEnergyPower,

    /// Magic Waters
    ActionActivateWaterGlobules,
    ActionDeactiveWaterGlobules
};

enum eWitherbarkCreatures
{
    CreatureWaterGlobule            = 81821,
    CreatureEnchantedWatersDelete   = 88862
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

Position g_PositionEnchantedMobs[10] =
{
    { 474.81f, 1638.33f, 87.567f, 0.000000f }, ///< enchanted water 1
    { 458.76f, 1648.68f, 87.567f, 0.000000f }, ///< enchanted water 2
    { 443.61f, 1649.07f, 87.567f, 0.000000f }, ///< enchanted water 3
    { 434.33f, 1661.58f, 87.567f, 0.000000f }, ///< enchanted water 4
    { 422.24f, 1652.27f, 87.567f, 0.000000f }, ///< enchanted water 5
    { 404.96f, 1654.05f, 87.567f, 0.000000f }, ///< enchanted water 6
    { 387.49f, 1650.11f, 87.567f, 0.000000f }, ///< enchanted water 7
    { 376.22f, 1636.72f, 87.567f, 0.000000f }, ///< enchanted water 8
    { 369.48f, 1628.33f, 87.567f, 0.000000f }, ///< enchanted water 9
    { 349.98f, 1594.22f, 87.567f, 0.000000f }  ///< enchanted water 10
};

Position g_PositionCosmeticGlobules[3] =
{
    { 408.211f, 1589.37f, 87.908f, 0.988586f },
    { 440.903f, 1609.97f, 88.661f, 2.791076f },
    { 404.258f, 1628.98f, 88.034f, 5.571381f }
};

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
{
    std::list<Creature*> l_CreatureList;
    GetCreatureListWithEntryInGrid(l_CreatureList, p_Object, p_Entry, 300.0f);

    for (Creature* l_Iter : l_CreatureList)
        l_Iter->DespawnOrUnsummon();
}

#define INVISIBILE_DISPLAY 11686
#define FriendlyFaction 35
#define HostileFaction 16

/// Witherbark - 81522
class boss_witherbark : public CreatureScript
{
    public:
        boss_witherbark() : CreatureScript("boss_witherbark") { }

        struct boss_witherbarkAI : public BossAI
        {
            boss_witherbarkAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomDatas::DataWitherbark)
            {
                m_Instance = me->GetInstanceScript();

                me->AddAura(eWitherbarkSpells::SpellBrittleBarkAura, me);
                me->setFaction(FriendlyFaction);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                SolarVisual();

                m_Counting = 0;
                m_Intro = false;
            }

            uint32 m_Counting;
            uint32 m_CountPerRegeneration;
            uint32 m_Power;
            bool   m_Intro;
            bool   m_Acheivement;

            InstanceScript* m_Instance;

            void Reset() override
            {
                _Reset();
                events.Reset();

                me->RemoveAllAuras();
                me->SetHealth(me->GetMaxHealth());

                m_Acheivement = true;
                m_Power = 100;
                me->SetPower(Powers::POWER_MANA, 100);
                me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, 100);
                me->SetMaxPower(Powers::POWER_MANA, 100);
                me->SetInt32Value(EUnitFields::UNIT_FIELD_MAX_POWER, 100);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                me->AddAura(eWitherbarkSpells::SpellPertifiedBark, me);
            }

            void SolarVisual()
            {
                for (uint8 l_I = 0; l_I < 3; l_I++)
                {
                    if (Creature* l_Cosmetic = me->SummonCreature(eWitherbarkCreatures::CreatureWaterGlobule, g_PositionCosmeticGlobules[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN))
                    {
                        l_Cosmetic->AddUnitState(UnitState::UNIT_STATE_CANNOT_AUTOATTACK);
                        l_Cosmetic->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        l_Cosmetic->CastSpell(l_Cosmetic, eEverbloomSpells::SpellCosmeticRoot); ///< COSMETIC ROOTS
                    }
                }

                std::list<Creature*> l_TenderGuidsList;
                me->GetCreatureListWithEntryInGrid(l_TenderGuidsList, eEverbloomCreatures::CreatureEverbloomTender, 200.0f);

                for (Creature* l_Iter : l_TenderGuidsList)
                    l_Iter->CastSpell(l_Iter, eEverbloomSpells::SpellSolarChannel);
            }

            void JustReachedHome() override
            {
                _JustReachedHome();

                DespawnCreaturesInArea(eEverbloomCreatures::CreatureEnchanctedWater, me);
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

                        if (m_Counting == 6 && !m_Intro)
                        {
                            m_Intro = true;
                            me->SetReactState(ReactStates::REACT_DEFENSIVE);

                            me->setFaction(HostileFaction);
                            me->RemoveAllAuras();
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
                        me->GetCreatureListWithEntryInGrid(l_EnchantedWatersList, eEverbloomCreatures::CreatureEnchanctedWater, 200.0f);

                        for (Creature* l_Iter : l_EnchantedWatersList)
                        {
                            if (l_Iter->IsAIEnabled)
                                l_Iter->GetAI()->DoAction(eWitherbarkActions::ActionActivateWaterGlobules);
                        }

                        break;
                    }
                    case eWitherbarkActions::ActionBrittleDeffect:
                    {
                        if (!me->isInCombat())
                            return;

                        me->MonsterTextEmote("Witherbark has absorbed enough water and become active again!", me->GetGUID(), true);

                        events.Reset();
                        events.ScheduleEvent(eWitherbarkEvents::EventParchedGrasp, 8 * TimeConstants::IN_MILLISECONDS);
                        events.ScheduleEvent(eWitherbarkEvents::EventUncheckedGrowth, 10 * TimeConstants::IN_MILLISECONDS);

                        Talk(eWitherbarkTalks::WitherbarkSpell01);

                        std::list<Creature*> WateryCreaturesList;
                        me->GetCreatureListWithEntryInGrid(WateryCreaturesList, eEverbloomCreatures::CreatureEnchanctedWater, 300.0f);

                        for (Creature* l_Iter : WateryCreaturesList)
                        {
                            if (l_Iter->IsAIEnabled)
                                l_Iter->GetAI()->DoAction(eWitherbarkActions::ActionDeactiveWaterGlobules);
                        }

                        break;
                    }
                }
            }

            void EnterCombat(Unit* p_Who) override
            {
                _EnterCombat();

                Talk(eWitherbarkTalks::WitherbarkAggro);

                me->RemoveAura(eWitherbarkSpells::SpellBrittleBarkAura);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                events.ScheduleEvent(eWitherbarkEvents::EventParchedGrasp,       12 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eWitherbarkEvents::EventUncheckedGrowth,    20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eEverbloomEvents::EventBarrageOfLeaves,     10 * TimeConstants::IN_MILLISECONDS);

                m_CountPerRegeneration = 4 * TimeConstants::IN_MILLISECONDS;

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                
                    DoZoneInCombat();
                }

                for (uint8 l_I = 0; l_I < 10; l_I++)
                    me->SummonCreature(eEverbloomCreatures::CreatureEnchanctedWater, g_PositionEnchantedMobs[l_I], TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
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

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Acheivement)
                {
                    if (m_Instance != nullptr && IsHeroic())
                        m_Instance->DoCompleteAchievement(eEverbloomAchievements::AchivementWaterManagement);
                }

                Talk(eWitherbarkTalks::WitherbarkDeath);

                DespawnCreaturesInArea(eWitherbarkCreatures::CreatureWaterGlobule, me);
                DespawnCreaturesInArea(eEverbloomCreatures::CreatureEnchanctedWater, me);
            }

            void RegeneratePower(Powers p_Power, int32& p_Value) override
            {
                me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, me->GetPower(Powers::POWER_MANA));
            }

            void BrittleRegeneration(uint32 const p_Diff)
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
                            int32 l_Value = 0;
                            l_Value = m_Power - 10;
                            m_Power -= 10;

                            me->SetPower(Powers::POWER_MANA, l_Value);
                            me->SetInt32Value(EUnitFields::UNIT_FIELD_POWER, l_Value);

                            m_CountPerRegeneration = 3 * TimeConstants::IN_MILLISECONDS;
                        }
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
                    case eEverbloomEvents::EventBarrageOfLeaves:
                    {
                        me->CastSpell(me, eEverbloomSpells::SpellLivingLeavesDummy);
                        events.ScheduleEvent(eEverbloomEvents::EventBarrageOfLeaves, 24 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_witherbarkAI(p_Creature);
        }
};

/// Everbloom Naturalist - 81819
class the_everbloom_naturalist : public CreatureScript
{
    public:
        the_everbloom_naturalist() : CreatureScript("the_everbloom_naturalist") { }

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                m_Instance = me->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();

                if (Creature* l_Globule = me->FindNearestCreature(eEverbloomCreatures::CreatureGlobuleWater, 20.0f))
                    me->CastSpell(l_Globule, eEverbloomSpells::SpellSolarChannel);
                else if (Creature* l_Globule = me->FindNearestCreature(eEverbloomCreatures::TriggerTenderBeamTrigger, 50.0f))
                    me->CastSpell(l_Globule, eEverbloomSpells::SpellNaturesChanneling);
            }

            void RandomSayings()
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossWitherbark)))
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

            void EnterCombat(Unit* pAttacker) override
            {
                RandomSayings();

                DespawnCreaturesInArea(eEverbloomCreatures::CreatureGlobuleWater, me);

                events.ScheduleEvent(eEverbloomEvents::EventBarrageOfLeaves, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                me->RemoveAura(eEverbloomSpells::SpellSolarChannel);
            }

            void JustDied(Unit* pKiller) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossWitherbark)))
                {
                    if (l_Witherbark->IsWithinDistInMap(me, 30.0f) && l_Witherbark->IsAIEnabled)
                        l_Witherbark->AI()->DoAction(eEverbloomActions::ActionCountPre1StBossKill);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eEverbloomEvents::EventBarrageOfLeaves:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 45.0f, true))
                            me->CastSpell(l_Target, eEverbloomSpells::SepllBarrageOfLeaves);

                        events.ScheduleEvent(eEverbloomEvents::EventBarrageOfLeaves, 7 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const override
        {
            return new the_everbloom_creaturesAI(pCreature);
        }
};

/// Agitated Waters - 88862
class the_everbloom_agitated_water : public CreatureScript
{
    public:
        the_everbloom_agitated_water() : CreatureScript("the_everbloom_agitated_water") {}

        struct the_everbloom_creaturesAI : public Scripted_NoMovementAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            int32 m_Interval;
            bool m_StartEvent;

            void Reset() override
            {
                me->SetInCombatWithZone();
                me->setFaction(HostileFaction);
                m_Interval = 20 * TimeConstants::IN_MILLISECONDS;

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                events.ScheduleEvent(eWitherbarkEvents::EventShootWater, urand(4 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eWitherbarkActions::ActionActivateWaterGlobules:
                        m_Interval = 20 * TimeConstants::IN_MILLISECONDS;

                        events.CancelEvent(eWitherbarkEvents::EventAqueousGlobules);
                        events.ScheduleEvent(eWitherbarkEvents::EventAqueousGlobules, urand(3 * TimeConstants::IN_MILLISECONDS, 7 * TimeConstants::IN_MILLISECONDS));
                        break;
                    case eWitherbarkActions::ActionDeactiveWaterGlobules:
                        events.CancelEvent(eWitherbarkEvents::EventAqueousGlobules);
                        DespawnCreaturesInArea(eWitherbarkCreatures::CreatureWaterGlobule, me);
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
                    case eWitherbarkEvents::EventAqueousGlobules:
                        m_Interval -= 3 * TimeConstants::IN_MILLISECONDS;

                        if (roll_chance_i(40))
                            me->SummonCreature(eWitherbarkCreatures::CreatureWaterGlobule, *me, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);

                        events.ScheduleEvent(eWitherbarkEvents::EventAqueousGlobules, urand(m_Interval, m_Interval +  (2 * TimeConstants::IN_MILLISECONDS)));
                        break;
                    case eWitherbarkEvents::EventShootWater:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, SpellShootWater);

                        events.ScheduleEvent(eWitherbarkEvents::EventShootWater, urand(14 * TimeConstants::IN_MILLISECONDS, 70 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Aqueous Globue  - 81821
class the_everbloom_globule : public CreatureScript
{
    public:
        the_everbloom_globule() : CreatureScript("the_everbloom_globule") {}

        struct the_everbloom_creaturesAI : public ScriptedAI
        {
            the_everbloom_creaturesAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            uint32 m_DiffCheckup;
            bool m_Caught;

            void Reset() override
            {
                me->CastSpell(me, eWitherbarkSpells::SpellGlobueCosmetic);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->setFaction(16);
                me->SetDisplayId(11686);
                me->SetSpeed(UnitMoveType::MOVE_RUN, 0.4f);
 
                m_Caught = false;
                m_DiffCheckup = 1 * TimeConstants::IN_MILLISECONDS;
            }

            void JustDied(Unit* p_Killer) override
            {
                me->CastSpell(me, eWitherbarkSpells::SpellAqueousBurst);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (m_Instance == nullptr)
                    return;

                if (Creature* l_Witherbark = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossWitherbark)))
                {
                    if (l_Witherbark->isInCombat() && l_Witherbark->HasAura(eWitherbarkSpells::SpellBrittleBarkAura))
                    {
                        if (!me->isMoving())
                            me->GetMotionMaster()->MoveFollow(l_Witherbark, 0, 0, MovementSlot::MOTION_SLOT_ACTIVE);

                        if (m_DiffCheckup <= p_Diff && !m_Caught)
                        {
                            if (l_Witherbark->IsWithinDistInMap(me, 3.0f))
                            {
                                if (l_Witherbark->IsAIEnabled)
                                {
                                    m_Caught = true;

                                    int32 l_Value = l_Witherbark->GetPower(Powers::POWER_MANA);
                                    l_Value += 10;

                                    l_Witherbark->AI()->DoAction(eWitherbarkActions::ActionEnergyPower);
                                    l_Witherbark->SetInt32Value(UNIT_FIELD_POWER, l_Value);

                                    me->DespawnOrUnsummon(500);

                                    m_DiffCheckup = 1 * TimeConstants::IN_MILLISECONDS;
                                }
                            }
                        }
                        else
                            m_DiffCheckup -= p_Diff;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_creaturesAI(p_Creature);
        }
};

/// Brittle Bark - 164275
class the_everbloom_brittle_bark : public SpellScriptLoader
{
    public:
        the_everbloom_brittle_bark() : SpellScriptLoader("the_everbloom_brittle_bark") { }

        class the_everbloom_spells : public AuraScript
        {
            PrepareAuraScript(the_everbloom_spells);

            void OnApply(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Creature* l_Caster = GetCaster()->ToCreature())
                {
                    if (l_Caster->IsInWorld() && l_Caster->isInCombat() && l_Caster->IsAIEnabled)
                        l_Caster->AI()->DoAction(eWitherbarkActions::ActionBrittleEffect);
                }
            }

            void OnRemove(constAuraEffectPtr /*p_aurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (GetCaster() == nullptr)
                    return;

                if (Creature* l_Caster = GetCaster()->ToCreature())
                {
                    if (l_Caster->IsInWorld() && l_Caster->isInCombat() && l_Caster->IsAIEnabled)
                        l_Caster->AI()->DoAction(eWitherbarkActions::ActionBrittleDeffect);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(the_everbloom_spells::OnApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_STUN, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
                AfterEffectRemove += AuraEffectRemoveFn(the_everbloom_spells::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_MOD_STUN, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new the_everbloom_spells();
        }
};

/// Agitated Waters - 177731 
class the_everbloom_agitated_waters : public SpellScriptLoader
{
    public:
        the_everbloom_agitated_waters() : SpellScriptLoader("the_everbloom_agitated_waters") {}

        class the_everbloom_agitated_waters_SpellScript : public SpellScript
        {
            PrepareSpellScript(the_everbloom_agitated_waters_SpellScript);

            SpellCastResult CheckCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                    {
                        if (Creature* l_Witherbark = l_Instance->instance->GetCreature(l_Instance->GetData64(eEverbloomBosses::BossWitherbark)))
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
                OnCheckCast += SpellCheckCastFn(the_everbloom_agitated_waters_SpellScript::CheckCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new the_everbloom_agitated_waters_SpellScript;
        }
};

void AddSC_boss_witherbark()
{
    /// Boss
    new boss_witherbark();

    /// Npcs
    new the_everbloom_agitated_water();
    new the_everbloom_globule();
    new the_everbloom_naturalist();

    /// Spells
    new the_everbloom_brittle_bark();
    new the_everbloom_agitated_waters();
}