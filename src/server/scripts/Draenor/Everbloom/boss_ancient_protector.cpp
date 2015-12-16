////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "the_everbloom.hpp"

enum eAncientProtectorsSpells
{
    /// Life Warden Gola
    SpellRevitalizingWater          = 168082, 
    SpellRapidTides                 = 168105,  
    SpellWaterBolt                  = 168092, 
    SpellUnknownReqCasRapidTides    = 168183,
    SpellUnknownReqTarRapidTides    = 173298,

    /// Earthshaper Telu
    SpellNaturesWrath               = 168040, 
    SpellBriaskinAura               = 168041,
    SpellBriarskin                  = 168491,
    SpellBramblePatchAura           = 167967,
    SpellBramblePatchDamage         = 167977,

    /// Dulhu
    SpellGraspingVine               = 168375,
    SpellGraspingVineJump           = 168378,
    SpellGraspingVineAuraDummy      = 168376,
    SpellNoxiousEruption            = 177145,
    SpellRendingCharge              = 164641,
    SpellRendingChargeDot           = 164643,
    SpellSlash                      = 168383,

    /// Misc
    SpellWaterVisualChanneling      = 83810,
    SpellDustyCloud                 = 174764,
    SpellNatureVisualChanneling     = 13236
};

enum eAncientProtectorsEvents
{
    EventRevitalizingWaters = 202,
    EventRapidTides         = 203,
    EventWaterBolt          = 204,
    EventNaturesWrath       = 205,
    EventBramblePatch       = 206,
    EventGraspingVine       = 207,
    EventNoxiusEruption     = 208,
    EventRendingCharge      = 209,
    EventSlash              = 210,
    EventBriarskin          = 211,
    EventHealing            = 212
};

enum eAncientProtectorsActions
{
    ActionUseSlash = 1,
    ActionRapidTidesDisable,
    ActionRapidTidesEnable
};

enum eAncientProtectorsCreature
{
    CreatureBramblePatch = 84008
};

enum eAncientProtectorsTalks
{
    /// Life Warden Gola
    XELA_AGGRO      = 11, ///< You are not welcome here!(46133)
    XELA_KILL01     = 15, ///< You are warned...(46136)

    /// Earthshaper Telu
    LUENA_SPELL01   = 18, ///< You will be purged!(46212)
    LUENA_SPELL02   = 19, ///< The forst shield us.(46213)
    LUENA_DEATH     = 21, ///< I will be renewed.(46208)
    LUENA_AGGRO     = 22, ///< You befoul this place!(46207)
    LUENA_KILL01    = 23, ///< The cycle continue... (46210)
    LUENA_KILL02    = 24  ///< Life ends.(46211)
};

#define HostileFaction 16
#define FriendlyFaction 35

static void StartAncientProtectors(InstanceScript* p_Instance)
{
    if (p_Instance == nullptr)
        return;

    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossLifeWardenGola)))
        l_Gola->SetInCombatWithZone();

    if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossDulhu)))
        l_Dulhu->SetInCombatWithZone();

    if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossEarthshaperTelu)))
        l_Telu->SetInCombatWithZone();
}

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
{
    std::list<Creature*> l_CreatureList;
    GetCreatureListWithEntryInGrid(l_CreatureList, p_Object, p_Entry, 300.0f);

    for (Creature* l_Iter : l_CreatureList)
        l_Iter->DespawnOrUnsummon();
}

static void WipingCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return;

    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);

    DespawnCreaturesInArea(eAncientProtectorsCreature::CreatureBramblePatch, p_Me);

    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossLifeWardenGola)))
    {
        l_Gola->Respawn();
        l_Gola->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossDulhu)))
    {
        l_Dulhu->Respawn();
        l_Dulhu->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossEarthshaperTelu)))
    {
        l_Telu->Respawn();
        l_Telu->GetMotionMaster()->MoveTargetedHome();
    }
}

static bool WiningCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return false;

    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);

    DespawnCreaturesInArea(eAncientProtectorsCreature::CreatureBramblePatch, p_Me);

    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossLifeWardenGola)))
    {
        if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossDulhu)))
        {
            if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomBosses::BossEarthshaperTelu)))
            {
                if (l_Gola->isDead() && l_Dulhu->isDead() && l_Telu->isDead())
                {
                    p_Instance->SetBossState(eEverbloomDatas::DataAncientProtectors, EncounterState::DONE);
                    return true;
                }
            }
        }
    }

    return false;
}

class MostCreatureHPMissingInRange
{
    public:
        MostCreatureHPMissingInRange(Unit const* p_Obj, float p_Range, uint32 p_HP) : m_Obj(p_Obj), m_Range(p_Range), m_HP(p_HP) { }
        bool operator()(Unit* p_Unit)
        {
            if (p_Unit->isAlive() && p_Unit->isInCombat() && !m_Obj->IsHostileTo(p_Unit) && m_Obj->IsWithinDistInMap(p_Unit, m_Range) && p_Unit->GetMaxHealth() - p_Unit->GetHealth() > m_HP && p_Unit->GetTypeId() != TYPEID_PLAYER)
            {
                if (p_Unit->GetEntry() == eEverbloomBosses::BossDulhu || p_Unit->GetEntry() == eEverbloomBosses::BossEarthshaperTelu || p_Unit->GetEntry() == eEverbloomBosses::BossLifeWardenGola)
                    m_HP = p_Unit->GetMaxHealth() - p_Unit->GetHealth();

                return true;
            }

            return false;
        }
    private:
        Unit const* m_Obj;
        float m_Range;
        uint32 m_HP;
};

/// Life Warden Gola - 83892
class boss_life_warden : public CreatureScript
{
    public:
        boss_life_warden() : CreatureScript("boss_life_warden") { }

        struct boss_life_wardenAI : public BossAI
        {
            boss_life_wardenAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomDatas::DataLifeWardenGola)
            {
                m_Instance = me->GetInstanceScript();
                m_Intro = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            bool m_Intro;

            void Reset() override
            {
                _Reset();

                events.Reset();

                me->CastSpell(me, eAncientProtectorsSpells::SpellWaterVisualChanneling); ///< water channel

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Who) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                    StartAncientProtectors(m_Instance);
                }
      
                events.ScheduleEvent(eAncientProtectorsEvents::EventRapidTides, 30 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eAncientProtectorsEvents::EventHealing, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eAncientProtectorsEvents::EventRevitalizingWaters, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eAncientProtectorsEvents::EventWaterBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));

                Talk(eAncientProtectorsTalks::XELA_AGGRO);
                me->RemoveAura(eAncientProtectorsSpells::SpellWaterVisualChanneling);
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eAncientProtectorsTalks::XELA_KILL01);
            }

            void JustReachedHome() override
            {
                _JustReachedHome();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                    WipingCondition(m_Instance, me);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    if (!WiningCondition(m_Instance, me))
                        me->SetLootRecipient(nullptr);
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
                    case eAncientProtectorsEvents::EventRevitalizingWaters:
                    {
                        Unit* l_Target = nullptr;
                        MostCreatureHPMissingInRange l_Check(me, 30.0f, 1);
                        JadeCore::UnitLastSearcher<MostCreatureHPMissingInRange> l_Searcher(me, l_Target, l_Check);
                        me->VisitNearbyObject(30.0f, l_Searcher);

                        if (l_Target && l_Target->IsInWorld())
                            me->CastSpell(l_Target, eEverbloomSpells::SpellHealingWaters);

                        Talk(eAncientProtectorsTalks::LUENA_SPELL01);

                        events.ScheduleEvent(eAncientProtectorsEvents::EventRevitalizingWaters, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                    case eAncientProtectorsEvents::EventRapidTides:
                    {
                        if (m_Instance == nullptr)
                            return;

                        me->AddAura(eAncientProtectorsSpells::SpellUnknownReqCasRapidTides, me);
                        Talk(eAncientProtectorsTalks::LUENA_SPELL02);

                        if (Creature* l_Telu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossEarthshaperTelu)))
                        {
                            if (Creature* l_Dulhu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomBosses::BossDulhu)))
                            {
                                if (roll_chance_i(50))
                                {
                                    l_Telu->AddAura(eAncientProtectorsSpells::SpellUnknownReqTarRapidTides, l_Telu);

                                    me->AddAura(eAncientProtectorsSpells::SpellRapidTides, l_Telu);
                                }
                                else
                                {
                                    l_Dulhu->AddAura(eAncientProtectorsSpells::SpellUnknownReqTarRapidTides, l_Dulhu);

                                    me->AddAura(eAncientProtectorsSpells::SpellRapidTides, l_Dulhu);
                                }
                            }
                        }

                        events.ScheduleEvent(eAncientProtectorsEvents::EventRapidTides, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eAncientProtectorsEvents::EventWaterBolt:
                    {
                        if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Random, eAncientProtectorsSpells::SpellWaterBolt);

                        events.ScheduleEvent(eAncientProtectorsEvents::EventWaterBolt, 6 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_life_wardenAI(p_Creature);
        }
};

/// Earthshaper Telu - 83893
class boss_earthshaper_telu : public CreatureScript
{
    public:
        boss_earthshaper_telu() : CreatureScript("boss_earthshaper_telu") { }

        struct boss_earthshaper_teluAI : public BossAI
        {
            boss_earthshaper_teluAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomDatas::DataEarthshaperTelu)
            {
                m_Instance = me->GetInstanceScript();
                m_Intro = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            bool m_Intro;
            bool m_Tides;

            void Reset() override
            {
                _Reset();

                events.Reset();

                m_Tides = false;

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->CastSpell(me, eAncientProtectorsSpells::SpellNatureVisualChanneling); ///> water channel
            }

            void EnterCombat(Unit* p_Who) override
            {
                _EnterCombat();

                me->RemoveAura(eAncientProtectorsSpells::SpellNatureVisualChanneling);
                Talk(eAncientProtectorsTalks::LUENA_AGGRO);

                events.ScheduleEvent(eAncientProtectorsEvents::EventBriarskin,    15 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eAncientProtectorsEvents::EventNaturesWrath, 8 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eAncientProtectorsEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);

                if (m_Instance != nullptr)
                {
                    instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                    StartAncientProtectors(m_Instance);
                }
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eAncientProtectorsTalks::LUENA_KILL01);
                else
                    Talk(eAncientProtectorsTalks::LUENA_KILL02);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAncientProtectorsActions::ActionRapidTidesEnable:
                        m_Tides = true;

                        events.ScheduleEvent(eAncientProtectorsEvents::EventBramblePatch, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eAncientProtectorsActions::ActionRapidTidesDisable:
                        m_Tides = false;

                        events.CancelEvent(eAncientProtectorsEvents::EventBramblePatch);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                Talk(eAncientProtectorsTalks::LUENA_DEATH);

                if (m_Instance != nullptr)
                {
                    if (!WiningCondition(m_Instance, me))
                    {
                        me->SetLootRecipient(nullptr);
                        DespawnCreaturesInArea(eAncientProtectorsCreature::CreatureBramblePatch, me);
                    }
                }
            }

            void JustReachedHome() override
            {
                _JustReachedHome();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                    WipingCondition(m_Instance, me);
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
                    case eAncientProtectorsEvents::EventBriarskin:
                    {
                        if (m_Instance == nullptr)
                            return;

                        Talk(eAncientProtectorsTalks::LUENA_SPELL01);

                        if (Creature* l_Gola = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomBosses::BossLifeWardenGola)))
                        {
                            if (Creature* l_Dulhu = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomBosses::BossDulhu)))
                            {
                                if (roll_chance_i(50))
                                    me->AddAura(eAncientProtectorsSpells::SpellBriaskinAura, l_Dulhu);
                                else
                                    me->AddAura(eAncientProtectorsSpells::SpellBriaskinAura, l_Gola);
                            }
                        }

                        events.ScheduleEvent(eAncientProtectorsEvents::EventBriarskin, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eAncientProtectorsEvents::EventNaturesWrath:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eAncientProtectorsSpells::SpellNaturesWrath);

                        events.ScheduleEvent(eAncientProtectorsEvents::EventNaturesWrath, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eAncientProtectorsEvents::EventBramblePatch:
                    {
                        Talk(eAncientProtectorsTalks::LUENA_SPELL02);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->SummonCreature(eAncientProtectorsCreature::CreatureBramblePatch, *l_Target, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 18 * TimeConstants::IN_MILLISECONDS);

                        if (m_Tides)
                            events.ScheduleEvent(eAncientProtectorsEvents::EventBramblePatch, 6 * TimeConstants::IN_MILLISECONDS);
                        else
                            events.ScheduleEvent(eAncientProtectorsEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
             }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_earthshaper_teluAI(p_Creature);
        }
};

/// Dulhu - 83894
class boss_dulhu : public CreatureScript
{
    public:
        boss_dulhu() : CreatureScript("boss_dulhu") { }

        struct boss_dulhuAI : public BossAI
        {
            boss_dulhuAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomDatas::DataDulhu)
            {
                m_Instance = me->GetInstanceScript();
                m_Intro = false;
            }

            InstanceScript* m_Instance;
            uint64 m_GraspingVineTargetGUID;
            uint32 m_GraspingVineDiff;
            bool m_Intro;
            bool m_Tides;

            void Reset() override
            {
                _Reset();

                events.Reset();

                m_Tides = false;

                m_GraspingVineTargetGUID = 0;
                m_GraspingVineDiff = 6 * TimeConstants::IN_MILLISECONDS;
            }

            void EnterCombat(Unit* p_Who) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                {
                    instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);

                    StartAncientProtectors(m_Instance);
                }

                events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eAncientProtectorsEvents::EventRendingCharge, 10 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eAncientProtectorsEvents::EventGraspingVine, 16 * TimeConstants::IN_MILLISECONDS);
            }
     
            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAncientProtectorsActions::ActionRapidTidesEnable:
                        m_Tides = true;

                        events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, 1 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eAncientProtectorsActions::ActionRapidTidesDisable:
                        m_Tides = false;

                        events.CancelEvent(eAncientProtectorsEvents::EventNoxiusEruption);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }
            }
        
            void JustReachedHome() override
            {
                _JustReachedHome();
                summons.DespawnAll();

                if (m_Instance != nullptr)
                    WipingCondition(m_Instance, me);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    if (!WiningCondition(m_Instance, me))
                        me->SetLootRecipient(nullptr);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                /// Handle grasping Vine.
                if (m_GraspingVineTargetGUID)
                {
                    if (Unit* l_GraspingVineTarget = Unit::GetUnit(*me, m_GraspingVineTargetGUID))
                    {
                        if (me->IsWithinDistInMap(l_GraspingVineTarget, 5.0f))
                        {
                            /// Cast Slash
                            me->CastSpell(l_GraspingVineTarget, eAncientProtectorsSpells::SpellSlash);
                            m_GraspingVineTargetGUID = 0;
                        }
                    }

                    /// In case Slash didn't happens, set target to 0
                    if (m_GraspingVineDiff <= p_Diff)
                        m_GraspingVineTargetGUID = 0;
                    else
                        m_GraspingVineDiff -= p_Diff;
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eAncientProtectorsEvents::EventRendingCharge:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            me->CastSpell(l_Target, eAncientProtectorsSpells::SpellRendingCharge);

                        events.ScheduleEvent(eAncientProtectorsEvents::EventRendingCharge, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eAncientProtectorsEvents::EventSlash:
                    {
                        me->CastSpell(me, eAncientProtectorsSpells::SpellSlash);
                        break;
                    }
                    case eAncientProtectorsEvents::EventNoxiusEruption:
                    {
                        DoCastAOE(eAncientProtectorsSpells::SpellNoxiousEruption);

                        if (m_Tides)
                             events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, urand(25 * TimeConstants::IN_MILLISECONDS, 32 * TimeConstants::IN_MILLISECONDS));
                        else
                            events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, 6 * TimeConstants::IN_MILLISECONDS);

                        break;
                    }
                    case eAncientProtectorsEvents::EventGraspingVine:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        {
                            m_GraspingVineTargetGUID = l_Target->GetGUID();
                            m_GraspingVineDiff = 6 * TimeConstants::IN_MILLISECONDS;

                            l_Target->CastSpell(me, eAncientProtectorsSpells::SpellGraspingVineAuraDummy);
                            l_Target->GetMotionMaster()->MoveJump(*me, 8.0f, 6.0f, 10.0f);
                        }

                        events.ScheduleEvent(eAncientProtectorsEvents::EventGraspingVine, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_dulhuAI(p_Creature);
        }
};

/// Bramble Patch Trigger - 
class the_everbloom_bramble_patch : public CreatureScript
{
    public:
        the_everbloom_bramble_patch() : CreatureScript("the_everbloom_bramble_patch") { }

        struct the_everbloom_spells : public Scripted_NoMovementAI
        {
            the_everbloom_spells(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            uint32 m_Time;
         
            void Reset() override
            {
                me->SetDisplayId(11686);
                me->setFaction(HostileFaction);
                m_Time = 2 * TimeConstants::IN_MILLISECONDS;

                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
         
                me->CastSpell(me, eAncientProtectorsSpells::SpellBramblePatchAura);
                me->CastSpell(me, eAncientProtectorsSpells::SpellDustyCloud); ///< Dusty cloud
            }

            void UpdateAI(const uint32 p_Diff) override
            {
                events.Update(p_Diff);

                if (m_Time <= p_Diff)
                {
                    std::list<Player*> l_PlayerList;
                    me->GetPlayerListInGrid(l_PlayerList, 2.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        if (!l_Player->HasAura(eAncientProtectorsSpells::SpellBramblePatchDamage))
                            l_Player->CastSpell(l_Player, eAncientProtectorsSpells::SpellBramblePatchDamage, true);
                    }

                    m_Time = 2 * TimeConstants::IN_MILLISECONDS;
                }
                else
                    m_Time -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new the_everbloom_spells(p_Creature);
        }
};

/// Rending Charge - 164641 
class the_everbloom_rending_charge : public SpellScriptLoader
{
    public:
        the_everbloom_rending_charge() : SpellScriptLoader("the_everbloom_rending_charge") { }

        class the_everbloom_spells : public SpellScript
        {
            PrepareSpellScript(the_everbloom_spells);

            void OnCastApply()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetExplTargetUnit())
                        l_Caster->CastSpell(l_Target, eAncientProtectorsSpells::SpellRendingChargeDot, true);
                }
            }

            void Register() override
            {
                AfterCast += SpellCastFn(the_everbloom_spells::OnCastApply);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new the_everbloom_spells();
        }
};

/// Rapid Tides - 168105 
class spell_everbloom_rapid_tides : public SpellScriptLoader
{
    public:
        spell_everbloom_rapid_tides() : SpellScriptLoader("spell_everbloom_rapid_tides") { }

        class spell_everbloom_rapid_tides_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_everbloom_rapid_tides_AuraScript);

            void AfterApply(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (l_Target->IsAIEnabled)
                        l_Target->AI()->DoAction(eAncientProtectorsActions::ActionRapidTidesEnable);
                }
            }

            void OnRemove(constAuraEffectPtr p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (l_Target->IsAIEnabled)
                        l_Target->AI()->DoAction(eAncientProtectorsActions::ActionRapidTidesDisable);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_everbloom_rapid_tides_AuraScript::AfterApply, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
                OnEffectRemove += AuraEffectRemoveFn(spell_everbloom_rapid_tides_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_everbloom_rapid_tides_AuraScript();
        }
};

void AddSC_boss_ancient_protectors()
{
    /// Bosses
    new boss_life_warden();
    new boss_dulhu();
    new boss_earthshaper_telu();

    /// Npc
    new the_everbloom_rending_charge();

    /// Spells
    new the_everbloom_bramble_patch();
    new spell_everbloom_rapid_tides();
}