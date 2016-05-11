////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eAncientProtectorsSpells
{
    SpellBramblePatchAura = 167967,
    SpellBramblePatchDamage = 167977,
    SpellDustyCloud = 174764,
    SpellNatureVisualChanneling = 13236
};

enum eAncientProtectorsActions
{
    ActionUseSlash = 1,
    ActionRapidTidesDisable,
    ActionRapidTidesEnable
};

enum eAncientProtectorsCreatrues
{
    CreatureBramblePatch = 84008
};

enum eAncientProtectorsTalks
{
    /// Life Warden Gola
    TalkXelaIntro = 10, ///> The forest closes in around you.(46135)
    TalkXelaAggro = 11, ///> You are not welcome here!(46133)
    TalkXelaSpell02 = 12, ///> Grow! (46139)
    TalkXelaSpell03 = 13, ///> Life Renewed.(46140)
    TalkXelaSpell01 = 14, ///> Water brings life!(46138)
    TalkXelaKill01 = 15, ///> You are warned...(46136)
    TalkXelaDeath = 35, ///> I return... to the soil...(46134)

    /// Earthshaper Telu
    TalkLuenaAggro = 17, ///> You will never escape...(46209)
    TalkLuenaSpell01 = 18, ///> You will be purged!(46212)
    TalkLuenaSpell02 = 19, ///> The forst shield us.(46213)
    TalkLuenaSpell03 = 20, ///> We protect.(46214)
    TalkLuenaDeath = 21, ///> I will be renewed.(46208)
    TalkLuenaKill01 = 22, ///> The cycle continue... (46210)
    TalkLuenaKill02 = 24  ///> Life ends.(46211)
};

static void StartAncientProtectors(InstanceScript* p_Instance, Creature* /*p_Me*/, Unit* /*p_Target*/)
{
    if (p_Instance == nullptr)
        return;

    p_Instance->SetBossState(eEverbloomData::DataLifeWardenGola, EncounterState::IN_PROGRESS);
    p_Instance->SetBossState(eEverbloomData::DataDulhu, EncounterState::IN_PROGRESS);
    p_Instance->SetBossState(eEverbloomData::DataEarthshaperTelu, EncounterState::IN_PROGRESS);

    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataLifeWardenGola)))
        l_Gola->SetInCombatWithZone();
    if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataDulhu)))
        l_Dulhu->SetInCombatWithZone();
    if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
        l_Telu->SetInCombatWithZone();
}

static void WipingCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return;

    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataLifeWardenGola)))
    {
        if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataDulhu)))
        {
            if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
            {
                p_Instance->SetBossState(eEverbloomData::DataLifeWardenGola, EncounterState::FAIL);
                p_Instance->SetBossState(eEverbloomData::DataDulhu, EncounterState::FAIL);
                p_Instance->SetBossState(eEverbloomData::DataEarthshaperTelu, EncounterState::FAIL);
                p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);
                DespawnCreaturesInArea(eAncientProtectorsCreatrues::CreatureBramblePatch, p_Me);
                l_Gola->Respawn();
                l_Gola->GetMotionMaster()->MovePoint(0, l_Gola->GetHomePosition().GetPositionX(), l_Gola->GetHomePosition().GetPositionY(), l_Gola->GetHomePosition().GetPositionZ());
                l_Telu->Respawn();
                l_Telu->GetMotionMaster()->MovePoint(0, l_Telu->GetHomePosition().GetPositionX(), l_Telu->GetHomePosition().GetPositionY(), l_Telu->GetHomePosition().GetPositionZ());
                l_Dulhu->Respawn();
                l_Dulhu->GetMotionMaster()->MovePoint(0, l_Dulhu->GetHomePosition().GetPositionX(), l_Dulhu->GetHomePosition().GetPositionY(), l_Dulhu->GetHomePosition().GetPositionZ());
            }
        }
    }
}

static void WiningCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return;


    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataLifeWardenGola)))
    {
        if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataDulhu)))
        {
            if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
            {
                if (l_Gola->isDead() && l_Dulhu->isDead() && l_Telu->isDead())
                {
                    p_Instance->SetBossState(eEverbloomData::DataLifeWardenGola, EncounterState::DONE);
                    p_Instance->SetBossState(eEverbloomData::DataEarthshaperTelu, EncounterState::DONE);
                    p_Instance->SetBossState(eEverbloomData::DataDulhu, EncounterState::DONE);
                    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);
                    DespawnCreaturesInArea(eAncientProtectorsCreatrues::CreatureBramblePatch, p_Me);
                }
            }
        }
    }
}

class MostCreatureHPMissingInRange
{
public:

    MostCreatureHPMissingInRange(Unit const* p_Obj, float p_Range, uint32 p_Hp) : m_Obj(p_Obj), m_Range(p_Range), m_Hp(p_Hp) {}
    bool operator()(Unit* p_U)
    {
        if (p_U->isAlive() && p_U->isInCombat() && !m_Obj->IsHostileTo(p_U) && m_Obj->IsWithinDistInMap(p_U, m_Range) && p_U->GetMaxHealth() - p_U->GetHealth() > m_Hp && p_U->GetTypeId() != TypeID::TYPEID_PLAYER)
        {
            if (p_U->GetEntry() == eEverbloomBosses::BossDulhu || p_U->GetEntry() == eEverbloomBosses::BossEarthshaperTelu || p_U->GetEntry() == eEverbloomBosses::BossLifeWardenGola)
                m_Hp = p_U->GetMaxHealth() - p_U->GetHealth();
            return true;
        }
        return false;
    }

private:
    Unit const* m_Obj;
    float m_Range;
    uint32 m_Hp;
};

/// Life Warden Gola - 83892
class boss_life_warden_gola : public CreatureScript
{
public:

    boss_life_warden_gola() : CreatureScript("boss_life_warden_gola") { }

    struct boss_life_warden_golaAI : public BossAI
    {
        boss_life_warden_golaAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataLifeWardenGola)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
        }

        enum eGolaEvents
        {
            EventRevitalizingWaters = 1,
            EventRapidTides,
            EventWaterBolt,
            EventHealing
        };

        enum eGolaSpells
        {
            /// Life Warden Gola
            SpellRevitalizingWater = 168082,
            SpellRapidTides = 168105,
            SpellWaterBolt = 168092,
            SpellUnknownReqCasRapidTides = 168183,
            SpellUnknownReqTarRapidTides = 173298,
            SpellHealingWaters = 164887,
            SpellWaterVisualChanneling = 83810
        };

        InstanceScript* m_Instance;
        bool m_Intro;

        void Reset()
        {
            events.Reset();
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->CastSpell(me, eGolaSpells::SpellWaterVisualChanneling);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Who)
        {
            if (m_Instance)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                StartAncientProtectors(m_Instance, me, p_Who);
                DoZoneInCombat();
            }
            Talk(eAncientProtectorsTalks::TalkXelaAggro);
            me->RemoveAura(eGolaSpells::SpellWaterVisualChanneling);
            events.ScheduleEvent(eGolaEvents::EventRapidTides, 30 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGolaEvents::EventHealing, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eGolaEvents::EventRevitalizingWaters, 20 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGolaEvents::EventWaterBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
        }

        void KilledUnit(Unit* p_Who)
        {
            if (p_Who && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(eAncientProtectorsTalks::TalkXelaKill01);
        }

        void JustReachedHome()
        {
            summons.DespawnAll();
            if (m_Instance != nullptr)
                WipingCondition(instance, me);
        }

        void JustDied(Unit* /*p_Killer*/)
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Dulhu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataDulhu)))
                {
                    if (Creature* l_Telu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
                    {
                        if (!l_Telu->isDead() && !l_Dulhu->isDead())
                        {
                            me->SetLootRecipient(NULL);
                        }
                        else
                        {
                            WiningCondition(m_Instance, me);
                        }
                    }
                }
            }
        }

        void UpdateAI(uint32 const p_Diff)
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eGolaEvents::EventRevitalizingWaters:
            {
                Unit* l_Target = NULL;
                MostCreatureHPMissingInRange u_check(me, 30.0f, 1);
                JadeCore::UnitLastSearcher<MostCreatureHPMissingInRange> searcher(me, l_Target, u_check);
                me->VisitNearbyObject(30.0f, searcher);
                if (l_Target && l_Target->IsInWorld())
                    me->CastSpell(l_Target, eGolaSpells::SpellHealingWaters);
                Talk(eAncientProtectorsTalks::TalkLuenaSpell01);
                events.ScheduleEvent(eGolaEvents::EventRevitalizingWaters, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                break;
            }
            case eGolaEvents::EventRapidTides:
            {
                me->AddAura(eGolaSpells::SpellUnknownReqCasRapidTides, me);
                Talk(eAncientProtectorsTalks::TalkLuenaSpell02);
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Telu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
                    {
                        if (Creature* l_Dulhu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataDulhu)))
                        {
                            if (roll_chance_i(50))
                            {
                                l_Telu->AddAura(eGolaSpells::SpellUnknownReqTarRapidTides, l_Telu);
                                me->AddAura(eGolaSpells::SpellRapidTides, l_Telu);
                            }
                            else
                            {
                                l_Dulhu->AddAura(eGolaSpells::SpellUnknownReqTarRapidTides, l_Dulhu);
                                me->AddAura(eGolaSpells::SpellRapidTides, l_Dulhu);
                            }
                        }
                    }
                }
                events.ScheduleEvent(eGolaEvents::EventRapidTides, 30 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            case eGolaEvents::EventWaterBolt:
            {
                if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    me->CastSpell(l_Random, eGolaSpells::SpellWaterBolt);
                events.ScheduleEvent(eGolaEvents::EventWaterBolt, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            default:
                break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new boss_life_warden_golaAI(p_Creature);
    }
};

/// Earthshaper Telu - 83893
class boss_earthshaper_telu : public CreatureScript
{
public:

    boss_earthshaper_telu() : CreatureScript("boss_earthshaper_telu") { }

    struct boss_earthshaper_teluAI : public BossAI
    {
        boss_earthshaper_teluAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataEarthshaperTelu)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
        }

        enum eTeluEvents
        {
            EventBriarskin = 1,
            EventNaturesWrath,
            EventBramblePatch
        };

        enum eTeluSpells
        {
            SpellBriaskinAura = 168041,
            SpellNaturesWrath = 168040,
            SpellNatureVisualChanneling = 13236
        };

        InstanceScript* m_Instance;
        bool m_Intro;
        bool m_Tides;

        void Reset() override
        {
            events.Reset();
            m_Tides = false;
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->CastSpell(me, eAncientProtectorsSpells::SpellNatureVisualChanneling); ///> water channel
        }

        void EnterCombat(Unit* p_Who) override
        {
            Talk(eAncientProtectorsTalks::TalkLuenaAggro);
            me->RemoveAura(eTeluSpells::SpellNatureVisualChanneling);
            events.ScheduleEvent(eTeluEvents::EventBriarskin, 15 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTeluEvents::EventNaturesWrath, 8 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eTeluEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eEverbloomData::DataEarthshaperTelu, IN_PROGRESS);
                StartAncientProtectors(m_Instance, me, p_Who);
                DoZoneInCombat();
            }
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(eAncientProtectorsTalks::TalkLuenaKill01);
            else
                Talk(eAncientProtectorsTalks::TalkLuenaKill02);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eAncientProtectorsActions::ActionRapidTidesEnable:
                m_Tides = true;
                events.ScheduleEvent(eTeluEvents::EventBramblePatch, 1 * TimeConstants::IN_MILLISECONDS);
                break;
            case eAncientProtectorsActions::ActionRapidTidesDisable:
                m_Tides = false;
                events.CancelEvent(eTeluEvents::EventBramblePatch);
                events.ScheduleEvent(eTeluEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);
                break;
            }
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            Talk(eAncientProtectorsTalks::TalkLuenaDeath);
            if (m_Instance != nullptr)
            {
                if (Creature* l_Gola = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomData::DataLifeWardenGola)))
                {
                    if (Creature* l_Dulhu = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomData::DataDulhu)))
                    {
                        if (!l_Dulhu->isDead() && !l_Gola->isDead())
                        {
                            me->SetLootRecipient(NULL);
                            DespawnCreaturesInArea(eAncientProtectorsCreatrues::CreatureBramblePatch, me);
                        }
                        else
                        {
                            WiningCondition(m_Instance, me);
                        }
                    }
                }
            }
        }

        void JustReachedHome() override
        {
            summons.DespawnAll();
            if (m_Instance != nullptr)
            {
                WipingCondition(instance, me);
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
            case eTeluEvents::EventBriarskin:
            {
                if (m_Instance == nullptr)
                    return;

                Talk(eAncientProtectorsTalks::TalkLuenaSpell01);
                if (Creature* l_Gola = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomData::DataLifeWardenGola)))
                {
                    if (Creature* l_Dulhu = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomData::DataDulhu)))
                    {
                        if (roll_chance_i(50))
                        {
                            me->AddAura(eTeluSpells::SpellBriaskinAura, l_Dulhu);
                        }
                        else
                        {
                            me->AddAura(eTeluSpells::SpellBriaskinAura, l_Gola);
                        }

                        events.ScheduleEvent(eTeluEvents::EventBriarskin, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
            }
            case eTeluEvents::EventNaturesWrath:
            {
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    me->CastSpell(l_Target, eTeluSpells::SpellNaturesWrath);
                events.ScheduleEvent(eTeluEvents::EventNaturesWrath, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            case eTeluEvents::EventBramblePatch:
            {
                Talk(eAncientProtectorsTalks::TalkLuenaSpell02);

                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                {
                    me->SummonCreature(eAncientProtectorsCreatrues::CreatureBramblePatch, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 18 * TimeConstants::IN_MILLISECONDS);
                    if (m_Tides)
                        events.ScheduleEvent(eTeluEvents::EventBramblePatch, 6 * TimeConstants::IN_MILLISECONDS);
                    else
                        events.ScheduleEvent(eTeluEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);
                    break;
                }
            }
            default:
                break;
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
        boss_dulhuAI(Creature* p_Creature) : BossAI(p_Creature, eEverbloomData::DataDulhu)
        {
            m_Instance = me->GetInstanceScript();
            m_Intro = false;
        }

        enum eDulhuEvents
        {
            EventGraspingVine = 1,
            EventNoxiusEruption,
            EventRendingCharge,
            EventSlash
        };

        enum eDulhuSpells
        {
            SpellGraspingVine = 168375,
            SpellGraspingVineJump = 168378,
            SpellGraspingVineAuraDummy = 168376,
            SpellNoxiousEruption = 177145,
            SpellRendingCharge = 164641,
            SpellRendingChargeDot = 164643,
            SpellSlash = 168383
        };

        InstanceScript* m_Instance;
        uint64 m_GraspingVineTargetGUID;
        uint32 m_GraspingVineDiff;
        bool m_Intro;
        bool m_Tides;

        void Reset() override
        {
            events.Reset();
            m_Tides = false;
            m_GraspingVineTargetGUID = NULL;
            m_GraspingVineDiff = 6 * TimeConstants::IN_MILLISECONDS;
        }

        void EnterCombat(Unit* p_Who) override
        {
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                StartAncientProtectors(m_Instance, me, p_Who);
                DoZoneInCombat();
            }
            events.ScheduleEvent(eDulhuEvents::EventNoxiusEruption, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eDulhuEvents::EventRendingCharge, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eDulhuEvents::EventGraspingVine, 16 * TimeConstants::IN_MILLISECONDS);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
            case eAncientProtectorsActions::ActionRapidTidesEnable:
                m_Tides = true;
                events.ScheduleEvent(eDulhuEvents::EventNoxiusEruption, 1 * TimeConstants::IN_MILLISECONDS);
                break;
            case eAncientProtectorsActions::ActionRapidTidesDisable:
                m_Tides = false;
                events.CancelEvent(eDulhuEvents::EventNoxiusEruption);
                events.ScheduleEvent(eDulhuEvents::EventNoxiusEruption, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                break;
            default:
                break;
            }
        }

        void JustReachedHome() override
        {
            summons.DespawnAll();
            if (m_Instance != nullptr)
                WipingCondition(m_Instance, me);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            if (m_Instance != nullptr)
            {
                if (Creature* l_Gola = instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataLifeWardenGola)))
                {
                    if (Creature* l_Telu = instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
                    {
                        if (!l_Telu->isDead() && !l_Gola->isDead())
                        {
                            me->SetLootRecipient(NULL);
                        }
                        else
                        {
                            WiningCondition(m_Instance, me);
                        }
                    }
                }
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            // Handle grasping Vine.
            if (m_GraspingVineTargetGUID != 0)
            {
                if (Unit* l_GraspingVineTarget = sObjectAccessor->GetUnit(*me, m_GraspingVineTargetGUID))
                {
                    if (me->IsWithinDistInMap(l_GraspingVineTarget, 5.0f))
                    {
                        // Cast Slash
                        me->CastSpell(l_GraspingVineTarget, eDulhuSpells::SpellSlash);
                        m_GraspingVineTargetGUID = NULL;
                    }
                }

                // Incase Slash didn't happen, set target to NULL;
                if (m_GraspingVineDiff <= p_Diff)
                    m_GraspingVineTargetGUID = NULL;
                else
                    m_GraspingVineDiff -= p_Diff;
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
            case eDulhuEvents::EventRendingCharge:
            {
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    me->CastSpell(l_Target, eDulhuSpells::SpellRendingCharge);
                events.ScheduleEvent(eDulhuEvents::EventRendingCharge, 20 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            case eDulhuEvents::EventSlash:
            {
                me->CastSpell(me, eDulhuSpells::SpellSlash);
                break;
            }
            case eDulhuEvents::EventNoxiusEruption:
            {
                DoCastAOE(eDulhuSpells::SpellNoxiousEruption);
                if (m_Tides)
                    events.ScheduleEvent(eDulhuEvents::EventNoxiusEruption, urand(25 * TimeConstants::IN_MILLISECONDS, 32 * TimeConstants::IN_MILLISECONDS));
                else
                    events.ScheduleEvent(eDulhuEvents::EventNoxiusEruption, 6 * TimeConstants::IN_MILLISECONDS);
                break;
            }
            case eDulhuEvents::EventGraspingVine:
            {
                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                {
                    m_GraspingVineTargetGUID = l_Target->GetGUID();
                    m_GraspingVineDiff = 6 * TimeConstants::IN_MILLISECONDS;
                    l_Target->CastSpell(me, eDulhuSpells::SpellGraspingVineAuraDummy);
                    l_Target->GetMotionMaster()->MoveJump(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), 8.0f, 6.0f, 10.0f);
                }
                events.ScheduleEvent(eDulhuEvents::EventGraspingVine, 45 * TimeConstants::IN_MILLISECONDS);
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
        return new boss_dulhuAI(p_Creature);
    }
};

/// Bramble Patch - 84008
class the_everbloom_ancient_protectors_mob_bramble_patch : public CreatureScript
{
public:

    the_everbloom_ancient_protectors_mob_bramble_patch() : CreatureScript("the_everbloom_ancient_protectors_mob_bramble_patch") { }

    struct the_everbloom_ancient_protectors_mob_bramble_patchAI : public Scripted_NoMovementAI
    {
        the_everbloom_ancient_protectors_mob_bramble_patchAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_Time;

        void Reset() override
        {
            me->SetDisplayId(InvisibleDisplay);
            me->setFaction(HostileFaction);
            m_Time = 2 * TimeConstants::IN_MILLISECONDS;
            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->CastSpell(me, eAncientProtectorsSpells::SpellBramblePatchAura);
            me->CastSpell(me, eAncientProtectorsSpells::SpellDustyCloud); ///> dusty cloud
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (m_Time <= p_Diff)
            {
                std::list<Player*> l_PlayerList;
                me->GetPlayerListInGrid(l_PlayerList, 2.0f);
                if (!l_PlayerList.empty())
                {
                    for (Player* l_Itr : l_PlayerList)
                    {
                        if (!l_Itr->HasAura(eAncientProtectorsSpells::SpellBramblePatchDamage))
                            l_Itr->CastSpell(l_Itr, eAncientProtectorsSpells::SpellBramblePatchDamage, true);
                    }
                }

                m_Time = 2 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Time -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new the_everbloom_ancient_protectors_mob_bramble_patchAI(p_Creature);
    }
};

/// Rending Charge - 164641 
class the_everbloom_ancient_protectors_spell_rending_charge : public SpellScriptLoader
{
public:

    the_everbloom_ancient_protectors_spell_rending_charge() : SpellScriptLoader("the_everbloom_ancient_protectors_spell_rending_charge") { }

    class the_everbloom_ancient_protectors_spell_rending_charge_SpellScript : public SpellScript
    {
        PrepareSpellScript(the_everbloom_ancient_protectors_spell_rending_charge_SpellScript)

        enum eSpellRendingCharge
        {
            SpellRendingChargeDot = 164643
        };

        void OnCastApply()
        {
            if (!GetCaster() && !GetExplTargetUnit())
                return;

            GetCaster()->CastSpell(GetExplTargetUnit(), eSpellRendingCharge::SpellRendingChargeDot, true);
        }

        void Register()
        {
            AfterCast += SpellCastFn(the_everbloom_ancient_protectors_spell_rending_charge_SpellScript::OnCastApply);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new the_everbloom_ancient_protectors_spell_rending_charge_SpellScript();
    }
};

/// Rapid Tides - 168105 
class the_everbloom_ancient_protectors_spell_rapid_tides : public SpellScriptLoader
{
public:

    the_everbloom_ancient_protectors_spell_rapid_tides() : SpellScriptLoader("the_everbloom_ancient_protectors_spell_rapid_tides") { }

    class the_everbloom_ancient_protectors_spell_rapid_tides_AuraScript : public AuraScript
    {
        PrepareAuraScript(the_everbloom_ancient_protectors_spell_rapid_tides_AuraScript)

        void AfterApply(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Target = GetTarget())
            {
                if (l_Target->IsAIEnabled && l_Target->ToCreature())
                    l_Target->ToCreature()->AI()->DoAction(eAncientProtectorsActions::ActionRapidTidesEnable);
            }
        }

        void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
        {
            if (Unit* l_Target = GetTarget())
            {
                if (l_Target->IsAIEnabled && l_Target->ToCreature())
                    l_Target->ToCreature()->AI()->DoAction(eAncientProtectorsActions::ActionRapidTidesDisable);
            }
        }

        void Register() override
        {
            AfterEffectApply += AuraEffectApplyFn(the_everbloom_ancient_protectors_spell_rapid_tides_AuraScript::AfterApply, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
            OnEffectRemove += AuraEffectRemoveFn(the_everbloom_ancient_protectors_spell_rapid_tides_AuraScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_DUMMY, AuraEffectHandleModes::AURA_EFFECT_HANDLE_REAL);
        }
    };

    AuraScript* GetAuraScript() const override
    {
        return new the_everbloom_ancient_protectors_spell_rapid_tides_AuraScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_boss_ancient_protectors()
{
    new boss_dulhu();                                              ///< 83894
    new boss_life_warden_gola();                                   ///< 83892
    new boss_earthshaper_telu();                                   ///< 83893
    new the_everbloom_ancient_protectors_mob_bramble_patch();      ///< 84008
    new the_everbloom_ancient_protectors_spell_rending_charge();   ///< 164641
    new the_everbloom_ancient_protectors_spell_rapid_tides();      ///< 168105
}
#endif
