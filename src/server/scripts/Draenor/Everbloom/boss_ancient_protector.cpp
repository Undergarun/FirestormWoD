////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "the_everbloom.hpp"

enum eAncientProtectorsSpells
{
    /// Life Warden Gola
    SpellRevitalizingWater          = 168082, 
    SpellRapidTides                 = 168105,  
    SpellWaterBolt                  = 168092, 

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

    // MISC
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
    ActionUseSlash = 3333
};

enum eAncientProtectorsCreatrues
{
    CreatureBramblePatch = 84008
};

enum eAncientProtectorsTalks
{
    ///> Life Warden Gola
    XELA_INTRO      = 10, ///> The forest closes in around you.(46135)
    XELA_AGGRO      = 11, ///> You are not welcome here!(46133)
    XELA_SPELL02    = 12, ///> Grow! (46139)
    XELA_SPELL03    = 13, ///> Life Renewed.(46140)
    XELA_SPELL01    = 14, ///> Water brings life!(46138)
    XELA_KILL01     = 15, ///> You are warned...(46136)
    XELA_DEATH      = 35, ///> I return... to the soil...(46134)

    ///> Earthshaper Telu
    LUENA_INTRO     = 17, ///> You will never escape...(46209)
    LUENA_SPELL01   = 18, ///> You will be perged!(46212)
    LUENA_SPELL02   = 19, ///> The forst shield us.(46213)
    LUENA_SPELL03   = 20, ///> We protect.(46214)
    LUENA_DEATH     = 21, ///> I will be renewed.(46208)
    LUENA_AGGRO     = 22, ///> You befoul this place!(46207)
    LUENA_KILL01    = 23, ///> The cycle continue... (46210)
    LUENA_KILL02    = 24 ///> Life ends.(46211)

    // DBM //
    /*
    Life Warden Gola:
    SAY_SPELL1 = Revitalizing Waters
    SAY_SPELL2 = [Special Warining] Revitalizing Waters - interrupt Life Warden
    SAY_SPELL3 = Rapid Tides on Earthshaper Telu
    SAY_SPELL4 = [Special Warining] Rapid Tides  on Earthshaper Telu - dispel now
    Earthshaper Telu:
    SAY_SPELL5 = Briarskin on Life Warden Gola
    */
};

#define HostileFaction 16
#define FriendlyFaction 35

static void StartAncientProtectors(InstanceScript* p_Instance, Creature* me, Unit* /*p_Target*/)
{
    Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataLifeWardenGola));
    if (l_Gola)
        l_Gola->SetInCombatWithZone();

    Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataDulhu));
    if (l_Dulhu)
        l_Dulhu->SetInCombatWithZone();

    Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataEarthshaperTelu));
    if (l_Telu)
        l_Telu->SetInCombatWithZone();
}

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
{
    std::list<Creature*> l_CreatureList;
    GetCreatureListWithEntryInGrid(l_CreatureList, p_Object, p_Entry, 300.0f);
    if (l_CreatureList.empty())
        return;

    for (std::list<Creature*>::iterator iter = l_CreatureList.begin(); iter != l_CreatureList.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

static void WipingCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);

    DespawnCreaturesInArea(eAncientProtectorsCreatrues::CreatureBramblePatch, p_Me);

    if (Creature* l_Gola = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataLifeWardenGola)))
    {
        if (Creature* l_Dulhu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataDulhu)))
        {
            if (Creature* l_Telu = p_Instance->instance->GetCreature(p_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
            {
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
    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);

    DespawnCreaturesInArea(eAncientProtectorsCreatrues::CreatureBramblePatch, p_Me);

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
                }
            }
        }
    }
}


/// Life Warden Gola - 83892
class boss_life_warden : public CreatureScript
{
public:
    boss_life_warden() : CreatureScript("boss_life_warden") { }

    struct boss_life_wardenAI : public BossAI
    {
        boss_life_wardenAI(Creature* pCreature) : BossAI(pCreature, eEverbloomData::DataLifeWardenGola)
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

            me->CastSpell(me, eAncientProtectorsSpells::SpellWaterVisualChanneling); ///> water channel

            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();     

            if (instance)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                m_Instance->SetBossState(eEverbloomData::DataLifeWardenGola, EncounterState::IN_PROGRESS);
                StartAncientProtectors(instance, me, p_Who);

                DoZoneInCombat();
            }
      
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

            if (m_Instance != nullptr)
            WipingCondition(instance, me);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();

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

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAncientProtectorsEvents::EventRevitalizingWaters:
                {
                    if (Creature* l_Telu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
                    {
                        me->CastSpell(l_Telu, eEverbloomSpells::SpellHealingWaters);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventRevitalizingWaters, urand(12 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                }
                case eAncientProtectorsEvents::EventRapidTides:
                {
                    if (Creature* l_Telu = m_Instance->instance->GetCreature(m_Instance->GetData64(eEverbloomData::DataEarthshaperTelu)))
                    {
                        me->CastSpell(l_Telu, eAncientProtectorsSpells::SpellRapidTides);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventRevitalizingWaters, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
                case eAncientProtectorsEvents::EventWaterBolt:
                {
                    if (Unit* l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->CastSpell(l_Random, eAncientProtectorsSpells::SpellWaterBolt);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventWaterBolt, urand(6 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                        break;
                    }
                }
            }
        }

    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new boss_life_wardenAI(pCreature);
    }
};

/// Earthshaper Telu - 83893
class boss_earthshaper_telu : public CreatureScript
{
public:
    boss_earthshaper_telu() : CreatureScript("boss_earthshaper_telu") { }

    struct boss_earthshaper_teluAI : public BossAI
    {
        boss_earthshaper_teluAI(Creature* pCreature) : BossAI(pCreature, eEverbloomData::DataEarthshaperTelu)
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
         
            if (m_Instance)
            {
                instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                instance->SetBossState(eEverbloomData::DataEarthshaperTelu, IN_PROGRESS);

                StartAncientProtectors(instance, me, p_Who);

                DoZoneInCombat();
            }
        }

        void KilledUnit(Unit* p_Who) override
        {
            if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                Talk(eAncientProtectorsTalks::LUENA_KILL01);
            else
                Talk(eAncientProtectorsTalks::LUENA_KILL02);
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();
            Talk(eAncientProtectorsTalks::LUENA_DEATH);

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
            _JustReachedHome();

            if (m_Instance)
            {
                WipingCondition(instance, me);
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAncientProtectorsEvents::EventBriarskin:
                {
                    if (Creature* l_Gola = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomData::DataLifeWardenGola)))
                    {
                        if (Creature* l_Dulhu = m_Instance->instance->GetCreature(instance->GetData64(eEverbloomData::DataDulhu)))
                        {
                            if (roll_chance_i(50))
                            {
                                me->AddAura(eAncientProtectorsSpells::SpellBriaskinAura, l_Dulhu);
                            }
                            else
                            {
                                me->AddAura(eAncientProtectorsSpells::SpellBriaskinAura, l_Gola);
                            }

                            events.ScheduleEvent(eAncientProtectorsEvents::EventBriarskin, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    }
                }
                case eAncientProtectorsEvents::EventNaturesWrath:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->CastSpell(l_Target, eAncientProtectorsSpells::SpellNaturesWrath);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventNaturesWrath, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
                case eAncientProtectorsEvents::EventBramblePatch:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->SummonCreature(eAncientProtectorsCreatrues::CreatureBramblePatch, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), l_Target->GetOrientation(), TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventBramblePatch, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                }
            }
         }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new boss_earthshaper_teluAI(pCreature);
    }
};

/// Dulhu - 83894
class boss_dulhu : public CreatureScript
{
public:
    boss_dulhu() : CreatureScript("boss_dulhu") { }

    struct boss_dulhuAI : public BossAI
    {
        boss_dulhuAI(Creature* pCreature) : BossAI(pCreature, eEverbloomData::DataDulhu)
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
        }

        void EnterCombat(Unit* p_Who) override
        {
            _EnterCombat();

            if (m_Instance)
            {
                instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                instance->SetBossState(eEverbloomData::DataEarthshaperTelu, IN_PROGRESS);

                StartAncientProtectors(instance, me, p_Who);

                DoZoneInCombat();
            }

            events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eAncientProtectorsEvents::EventRendingCharge, 10 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eAncientProtectorsEvents::EventGraspingVine, 16 * TimeConstants::IN_MILLISECONDS);

            StartAncientProtectors(instance, me, p_Who);
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAncientProtectorsActions::ActionUseSlash:
                    events.ScheduleEvent(eAncientProtectorsEvents::EventSlash, 500);
                    break;
            }
        }

        void JustReachedHome() override
        {
            _JustReachedHome();

            if (instance)
            {
                WipingCondition(instance, me);
            }        
        }

        void JustDied(Unit* /*p_Killer*/) override
        {
            _JustDied();       

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
            events.Update(p_Diff);

            if (!UpdateVictim())
                return;

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eAncientProtectorsEvents::EventRendingCharge:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        me->CastSpell(l_Target, eAncientProtectorsSpells::SpellRendingCharge);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventRendingCharge, 20 * TimeConstants::IN_MILLISECONDS);
                    }
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
                    events.ScheduleEvent(eAncientProtectorsEvents::EventNoxiusEruption, urand(25 * TimeConstants::IN_MILLISECONDS, 32 * TimeConstants::IN_MILLISECONDS));
                    break;
                }
                case eAncientProtectorsEvents::EventGraspingVine:
                {
                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                    {
                        l_Target->CastSpell(me, eAncientProtectorsSpells::SpellGraspingVine);
                        events.ScheduleEvent(eAncientProtectorsEvents::EventGraspingVine, 45 * TimeConstants::IN_MILLISECONDS);
                    }
                    break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new boss_dulhuAI(pCreature);
    }
};

/// Grasping Vine - 
class the_everbloom_grasping_vine : public SpellScriptLoader
{
public:
    the_everbloom_grasping_vine() : SpellScriptLoader("the_everbloom_grasping_vine") { }

    class everbloom_spells : public SpellScript
    {
        PrepareSpellScript(everbloom_spells);

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (!GetCaster())
                return;
            
            std::list<Player*> l_PlayerList;
            GetCaster()->GetPlayerListInGrid(l_PlayerList, 100.0f);

            if (l_PlayerList.empty())
                return;

            std::list<Player*>::const_iterator it = l_PlayerList.begin();
            std::advance(it, urand(0, l_PlayerList.size() - 1));

            if ((*it) && (*it)->IsInWorld() && (*it)->isAlive())
            {
                (*it)->CastSpell(GetCaster(), eAncientProtectorsSpells::SpellGraspingVineJump); ///> JUMP
            }
            
            if (GetCaster() && GetCaster()->GetAI())
                GetCaster()->GetAI()->DoAction(eAncientProtectorsActions::ActionUseSlash);
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(everbloom_spells::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new everbloom_spells();
    }
};

/// Bramble Patch Trigger - 
class the_everbloom_bramble_patch : public CreatureScript
{
public:
    the_everbloom_bramble_patch() : CreatureScript("the_everbloom_bramble_patch") { }

    struct the_everbloom_spells : public Scripted_NoMovementAI
    {
        the_everbloom_spells(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;
        uint32 m_Time;
         
        void Reset() override
        {
            me->SetDisplayId(11686);
            me->setFaction(HostileFaction);
            m_Time = 1500;

            me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
         
            me->CastSpell(me, eAncientProtectorsSpells::SpellBramblePatchAura);
            me->CastSpell(me, eAncientProtectorsSpells::SpellDustyCloud); ///> dusty cloud
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
                    for (auto itr : l_PlayerList)
                    {
                        if (!itr->HasAura(eAncientProtectorsSpells::SpellBramblePatchDamage))
                        {
                            itr->CastSpell(itr, eAncientProtectorsSpells::SpellBramblePatchDamage, true);

                            AuraPtr aura = itr->GetAura(eAncientProtectorsSpells::SpellBramblePatchDamage);
                        }                        
                    }
                }

                m_Time = 500;
            }
            else
            {
                m_Time -= p_Diff;
            }
        }
    };

    CreatureAI* GetAI(Creature* pCreature) const override
    {
        return new the_everbloom_spells(pCreature);
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

        void OnHitApply()
        {
            if (!GetCaster() && !GetHitUnit())
                return;

            GetCaster()->AddAura(eAncientProtectorsSpells::SpellRendingChargeDot, GetHitUnit());
        }

        void Register()
        {
            OnHit += SpellHitFn(the_everbloom_spells::OnHitApply);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new the_everbloom_spells();
    }
};

void AddSC_boss_ancient_protectors()
{
    // bosses
    new boss_life_warden();
    new boss_dulhu();
    new boss_earthshaper_telu();

    // spells
    new the_everbloom_grasping_vine();
    new the_everbloom_rending_charge();
  
    // creatures
    new the_everbloom_bramble_patch();
}