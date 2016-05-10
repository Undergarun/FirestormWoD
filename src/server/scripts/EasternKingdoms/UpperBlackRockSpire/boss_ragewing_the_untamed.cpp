////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# include "upper_blackrock_spire.hpp"

enum eSpells
{
    ///< Engulfing Fire
    SpellEngulfingFireSearcher  = 154996,
    SpellEngulfingFire          = 155025,
    SpellEngulfingFireAT        = 155029,
    SpellEngulfingFireDamage    = 155031,
    SpellEngulfingFireConfuse   = 155056,

    ///< Magma Spit
    SpellMagmaSpitSearcher      = 155050,
    SpellMagmaSpitMissile       = 155053,
    SpellMagmaSpitDamageAndAT   = 155051,

    ///< Ripping Claw
    SpellRippingClaw            = 155065,

    ///< Fire Storm
    SpellFireStormSearcher      = 155070,
    SpellFireStormMissile       = 155073,
    SpellFireStormDamage        = 155081,

    ///< Burning Rage
    SpellBurningRage            = 155620,

    ///< Swirling Winds - Interfere Targetting
    SpellSwirlingWinds          = 167203
};

enum eEvents
{
    EventEngulfingFire  = 1,
    EventMagmaSpit,
    EventRippingClaw,
    EventFireStorm,
    EventBurningRage
};

enum eActions
{
    ActionMoveToBridge
};

enum eBossDatas
{
    MaxWaypoints    = 5,
    MaxWhelps       = 10,
    PhaseOne        = 0,
    PhaseTwo        = 1,
    PhaseThree      = 2
};

static Position const g_MoveToBridgePos[eBossDatas::MaxWaypoints] =
{
    { 15.342f, -391.415f, 052.303f, 3.129f },
    { 03.117f, -401.457f, 061.997f, 4.311f },
    { 10.265f, -405.912f, 079.739f, 6.219f },
    { 17.261f, -406.222f, 097.761f, 0.038f },
    { 23.536f, -407.374f, 114.878f, 0.001f }
};

static Position const g_MoveToWhelpPhasePos[eBossDatas::MaxWaypoints] =
{
    { 22.075f, -402.925f, 112.662f, 3.130f },
    { 17.049f, -402.868f, 117.647f, 3.130f },
    { 12.935f, -402.821f, 122.239f, 3.130f },
    { 03.936f, -402.760f, 126.291f, 3.138f },
    { -5.142f, -406.041f, 129.459f, 6.276f }
};

static Position const g_FireStalkerMovePos = { 38.162f, -406.093f, 110.723f, M_PI };
static Position const g_ThirdPhasePos = { 33.923f, -405.560f, 110.721f, 1.583f };

static Position const g_RagewingWhelpSpawnPos[eBossDatas::MaxWhelps] =
{
    { -52.239f, -419.966f, 137.595f, 0.139f },
    { -16.603f, -409.372f, 124.173f, 1.313f },
    { -57.336f, -423.489f, 137.599f, 0.211f },
    { -42.015f, -419.237f, 137.760f, 0.509f },
    { -57.155f, -423.623f, 137.572f, 0.138f },
    { -9.5611f, -414.897f, 128.347f, 6.025f },
    { -54.981f, -420.820f, 137.425f, 0.697f },
    { -9.0763f, -412.107f, 130.861f, 0.317f },
    { -48.719f, -419.111f, 137.576f, 0.076f },
    { -49.062f, -415.890f, 137.723f, 5.538f }
};

///< Ragewing the Untamed - 76585
class boss_ragewing_the_untamed : public CreatureScript
{
    public:
        boss_ragewing_the_untamed() : CreatureScript("boss_ragewing_the_untamed") { }

        struct boss_ragewing_the_untamedAI : public BossAI
        {
            boss_ragewing_the_untamedAI(Creature* p_Creature) : BossAI(p_Creature, DATA_RAGEWING_THE_UNTAMED)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            bool m_MovedToBridge;
            bool m_EngulfingFire;
            uint8 m_Waypoint;

            float m_FlyPhaseHealthPct;
            uint8 m_Phase;

            uint32 m_FireStormCount;
            uint64 m_EngulfingTargetGuid;

            void Reset()
            {
                _Reset();

                m_Events.Reset();

                m_MovedToBridge = false;
                m_EngulfingFire = false;
                m_Waypoint = 0;
                m_FlyPhaseHealthPct = 70.0f;
                m_Phase = eBossDatas::PhaseOne;
                m_FireStormCount = 6;
                m_EngulfingTargetGuid = 0;

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetCanFly(true);
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_GRAVITY);
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_MASK_MOVING_FLY);

                me->RemoveAllAreasTrigger();
                summons.DespawnAll();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventEngulfingFire, 13000);
                m_Events.ScheduleEvent(eEvents::EventMagmaSpit, 8000);
                m_Events.ScheduleEvent(eEvents::EventRippingClaw, 6000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (m_Phase == eBossDatas::PhaseTwo)
                {
                    p_Damage = 0;
                    return;
                }
                else if (m_Phase == eBossDatas::PhaseThree)
                    return;

                if (me->HealthBelowPctDamaged(m_FlyPhaseHealthPct, p_Damage))
                {
                    if (m_Phase == eBossDatas::PhaseOne && (m_FlyPhaseHealthPct == 70.0f || m_FlyPhaseHealthPct == 40.0f))
                    {
                        m_Phase = eBossDatas::PhaseTwo;

                        LaunchWhelpPhaseMoves();

                        if (m_FlyPhaseHealthPct == 70.0f)
                            m_FlyPhaseHealthPct = 40.0f;
                        else
                            m_FlyPhaseHealthPct = 0.0f;
                    }
                }
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                me->RemoveAllAreasTrigger();
                summons.DespawnAll();
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case eActions::ActionMoveToBridge:
                        if (m_MovedToBridge)
                            return;
                        m_MovedToBridge = true;
                        me->GetMotionMaster()->MovePoint(m_Waypoint, g_MoveToBridgePos[m_Waypoint]);
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID)
            {
                if (m_Phase == eBossDatas::PhaseThree)
                {
                    me->SetCanFly(false);
                    me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_GRAVITY);
                    me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_MASK_MOVING_FLY);
                    return;
                }

                if (p_ID > eBossDatas::MaxWaypoints)
                    return;

                ++m_Waypoint;

                if (m_Phase == eBossDatas::PhaseOne)
                {
                    if (m_Waypoint == eBossDatas::MaxWaypoints)
                    {
                        m_Waypoint = 0;
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        return;
                    }

                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->GetMotionMaster()->MovePoint(m_Waypoint, g_MoveToBridgePos[m_Waypoint]);
                }
                else
                {
                    if (m_Waypoint == eBossDatas::MaxWaypoints)
                    {
                        m_Waypoint = 0;
                        m_FireStormCount = 6;
                        me->CastSpell(me, eSpells::SpellFireStormSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventFireStorm, 1000);
                        --m_FireStormCount;
                        return;
                    }

                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->GetMotionMaster()->MovePoint(m_Waypoint, g_MoveToWhelpPhasePos[m_Waypoint]);
                }
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo)
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellMagmaSpitSearcher:
                    {
                        if (p_Victim == nullptr)
                            break;
                        me->CastSpell(p_Victim, eSpells::SpellMagmaSpitMissile, true);
                        break;
                    }
                    case eSpells::SpellEngulfingFireSearcher:
                    {
                        if (p_Victim == nullptr)
                            break;
                        me->MonsterTextEmote(LANG_ENGULFING_FIRE_RAGEWING, 0, true);
                        me->CastSpell(p_Victim, eSpells::SpellEngulfingFire, false);
                        break;
                    }
                    case eSpells::SpellEngulfingFire:
                        if (p_Victim != me)
                            break;
                        m_EngulfingFire = true;
                        break;
                    case eSpells::SpellFireStormSearcher:
                    {
                        if (p_Victim == nullptr)
                            break;
                        me->CastSpell(p_Victim, eSpells::SpellFireStormMissile, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (m_EngulfingFire)
                    UpdateOrientationIfNeeded();

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventEngulfingFire:
                    {
                        if (m_Instance == nullptr || m_Phase == eBossDatas::PhaseThree)
                            break;

                        if (m_Phase == eBossDatas::PhaseTwo)
                        {
                            m_Events.ScheduleEvent(eEvents::EventEngulfingFire, 2000);
                            break;
                        }

                        bool l_Left = urand(0, 1);
                        if (Creature* l_Tracker = Creature::GetCreature(*me, m_Instance->GetData64(l_Left ? eCreatures::NPC_ENGULFING_FIRE_L_TO_R : eCreatures::NPC_ENGULFING_FIRE_R_TO_L)))
                        {
                            m_EngulfingTargetGuid = l_Tracker->GetGUID();
                            me->CastSpell(l_Tracker, eSpells::SpellEngulfingFireSearcher, true);
                        }

                        m_Events.ScheduleEvent(eEvents::EventEngulfingFire, 22000);
                        break;
                    }
                    case eEvents::EventMagmaSpit:
                    {
                        if (m_Phase == eBossDatas::PhaseTwo)
                        {
                            m_Events.ScheduleEvent(eEvents::EventMagmaSpit, 2000);
                            break;
                        }

                        me->CastSpell(me, eSpells::SpellMagmaSpitSearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventMagmaSpit, 17000);
                        break;
                    }
                    case eEvents::EventRippingClaw:
                    {
                        if (m_Phase == eBossDatas::PhaseTwo)
                        {
                            m_Events.ScheduleEvent(eEvents::EventRippingClaw, 2000);
                            break;
                        }

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellRippingClaw, true);
                        m_Events.ScheduleEvent(eEvents::EventRippingClaw, 15000);
                        break;
                    }
                    case eEvents::EventFireStorm:
                        --m_FireStormCount;
                        me->CastSpell(me, eSpells::SpellFireStormSearcher, true);
                        if (m_FireStormCount)
                            m_Events.ScheduleEvent(eEvents::EventFireStorm, 2000);
                        else
                            LaunchNormalPhaseMoves();
                        break;
                    case eEvents::EventBurningRage:
                        me->CastSpell(me, eSpells::SpellBurningRage, true);
                        m_Events.ScheduleEvent(eEvents::EventBurningRage, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void UpdateOrientationIfNeeded()
            {
                if (!me->HasAura(eSpells::SpellEngulfingFireAT))
                    m_EngulfingFire = false;
                else
                    CheckEngulfingFireTargets();
            }

            void CheckEngulfingFireTargets()
            {
                std::list<Player*> l_PlayerList;
                me->GetPlayerListInGrid(l_PlayerList, 50.0f);

                if (Creature* l_Tracker = Creature::GetCreature(*me, m_EngulfingTargetGuid))
                    me->SetOrientation(me->GetAngle(l_Tracker));

                l_PlayerList.remove_if([this](Player* p_Player) -> bool
                {
                    if (p_Player == nullptr)
                        return true;

                    if (Aura* l_Aura = p_Player->GetAura(eSpells::SpellEngulfingFireConfuse, me->GetGUID()))
                    {
                        if (l_Aura->GetDuration() > 500)
                            return true;
                    }

                    bool l_InFront = me->isInFront(p_Player, M_PI / 6.0f);
                    if (!l_InFront)
                        return true;

                    return false;
                });

                for (Player* l_Player : l_PlayerList)
                {
                    me->CastSpell(l_Player, eSpells::SpellEngulfingFireDamage, true);
                    me->CastSpell(l_Player, eSpells::SpellEngulfingFireConfuse, true);
                }
            }

            void LaunchWhelpPhaseMoves()
            {
                me->CastSpell(me, eSpells::SpellSwirlingWinds, true);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->GetMotionMaster()->MovePoint(m_Waypoint, g_MoveToWhelpPhasePos[m_Waypoint]);

                for (uint8 l_I = 0; l_I < eBossDatas::MaxWhelps; ++l_I)
                    me->SummonCreature(eCreatures::NPC_RAGEWING_WHELP, g_RagewingWhelpSpawnPos[l_I]);
            }

            void LaunchNormalPhaseMoves()
            {
                if (m_FlyPhaseHealthPct == 0.0f)
                {
                    m_Phase = eBossDatas::PhaseThree;
                    m_Events.CancelEvent(eEvents::EventEngulfingFire);
                    m_Events.ScheduleEvent(eEvents::EventBurningRage, 5000);

                    me->RemoveAura(eSpells::SpellSwirlingWinds);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                    me->GetMotionMaster()->MovePoint(eBossDatas::MaxWaypoints + 1, g_ThirdPhasePos);
                    return;
                }
                else
                    m_Phase = eBossDatas::PhaseOne;

                me->RemoveAura(eSpells::SpellSwirlingWinds);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->GetMotionMaster()->MovePoint(eBossDatas::MaxWaypoints + 1, g_MoveToBridgePos[eBossDatas::MaxWaypoints - 1]);
            }

            bool CanBeTargetedOutOfLOS()
            {
                return true;
            }

            bool CanTargetOutOfLOS()
            {
                return true;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_ragewing_the_untamedAI(p_Creature);
        }
};

///< Fire Storm Stalker - 76885
class mob_fire_storm_stalker : public CreatureScript
{
    public:
        mob_fire_storm_stalker() : CreatureScript("mob_fire_storm_stalker") { }

        struct mob_fire_storm_stalkerAI : public ScriptedAI
        {
            mob_fire_storm_stalkerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                p_Damage = 0;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_fire_storm_stalkerAI(p_Creature);
        }
};

///< Engulfing Fire Invisible Stalker R to L - 76813
class mob_engulfing_fire_stalker_r_to_l : public CreatureScript
{
    public:
        mob_engulfing_fire_stalker_r_to_l() : CreatureScript("mob_engulfing_fire_stalker_r_to_l") { }

        struct mob_engulfing_fire_stalker_r_to_lAI : public ScriptedAI
        {
            mob_engulfing_fire_stalker_r_to_lAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            enum Events
            {
                EventCheckPlayer = 1,
                EventMoveCenter
            };

            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
                m_Events.ScheduleEvent(Events::EventCheckPlayer, 1000);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }

            void SpellHit(Unit* /*p_Caster*/, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::SpellEngulfingFireSearcher)
                    m_Events.ScheduleEvent(Events::EventMoveCenter, 1800);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID)
            {
                if (p_ID == 1)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveTargetedHome();
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                p_Damage = 0;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventCheckPlayer:
                    {
                        if (me->FindNearestPlayer(10.0f) && m_Instance != nullptr)
                        {
                            if (Creature* l_Ragewing = Creature::GetCreature(*me, m_Instance->GetData64(NPC_RAGEWING_THE_UNTAMED)))
                                l_Ragewing->AI()->DoAction(eActions::ActionMoveToBridge);
                        }

                        m_Events.ScheduleEvent(Events::EventCheckPlayer, 1000);
                        break;
                    }
                    case Events::EventMoveCenter:
                        me->GetMotionMaster()->MovePoint(1, g_FireStalkerMovePos);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_engulfing_fire_stalker_r_to_lAI(p_Creature);
        }
};

///< Engulfing Fire Invisible Stalker L to R - 76837
class mob_engulfing_fire_stalker_l_to_r : public CreatureScript
{
    public:
        mob_engulfing_fire_stalker_l_to_r() : CreatureScript("mob_engulfing_fire_stalker_l_to_r") { }

        struct mob_engulfing_fire_stalker_l_to_rAI : public ScriptedAI
        {
            mob_engulfing_fire_stalker_l_to_rAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum Events
            {
                EventMoveCenter = 1
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
            }

            void SpellHit(Unit* /*p_Caster*/, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == eSpells::SpellEngulfingFireSearcher)
                    m_Events.ScheduleEvent(Events::EventMoveCenter, 1800);
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID)
            {
                if (p_ID == 1)
                {
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveTargetedHome();
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                p_Damage = 0;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventMoveCenter:
                        me->GetMotionMaster()->MovePoint(1, g_FireStalkerMovePos);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_engulfing_fire_stalker_l_to_rAI(p_Creature);
        }
};

///< Ragewing Whelp - 83745
class mob_ragewing_whelp : public CreatureScript
{
    public:
        mob_ragewing_whelp() : CreatureScript("mob_ragewing_whelp") { }

        struct mob_ragewing_whelpAI : public ScriptedAI
        {
            mob_ragewing_whelpAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            bool m_Ground;

            void Reset()
            {
                m_Ground = false;

                me->SetCanFly(true);
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_GRAVITY);
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_MASK_MOVING_FLY);

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Ragewing = Creature::GetCreature(*me, m_Instance->GetData64(NPC_RAGEWING_THE_UNTAMED)))
                    {
                        if (l_Ragewing->getVictim() != nullptr)
                            AttackStart(l_Ragewing->getVictim());
                    }
                }
            }

            void DamageDealt(Unit* /*p_Attacker*/, uint32& /*p_Damage*/, DamageEffectType /*p_DamageType*/)
            {
                if (!m_Ground)
                {
                    m_Ground = true;

                    me->SetCanFly(false);
                    me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_DISABLE_GRAVITY);
                    me->RemoveUnitMovementFlag(MovementFlags::MOVEMENTFLAG_MASK_MOVING_FLY);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_ragewing_whelpAI(p_Creature);
        }
};

///< Magma Spit - 155051
class areatrigger_magma_spit : public AreaTriggerEntityScript
{
    public:
        areatrigger_magma_spit() : AreaTriggerEntityScript("areatrigger_magma_spit") { }

        enum eSpells
        {
            MagmaPool = 155057
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 3.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, eSpells::MagmaPool, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_magma_spit();
        }
};

///< Fire Storm (Missile) - 155073
class spell_fire_storm_missile: public SpellScriptLoader
{
    public:
        spell_fire_storm_missile() : SpellScriptLoader("spell_fire_storm_missile") { }

        class spell_fire_storm_missile_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_fire_storm_missile_SpellScript);

            void HandleOnHit()
            {
                ///< This spell needs a script cause of SpellInfo::NeedsToBeTriggeredByCaster
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Tracker = GetHitUnit())
                        l_Caster->CastSpell(l_Tracker, eSpells::SpellFireStormDamage, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_fire_storm_missile_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_fire_storm_missile_SpellScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_ragewing_the_untamed()
{
    new boss_ragewing_the_untamed();
    new mob_fire_storm_stalker();
    new mob_engulfing_fire_stalker_r_to_l();
    new mob_engulfing_fire_stalker_l_to_r();
    new mob_ragewing_whelp();
    new areatrigger_magma_spit();
    new spell_fire_storm_missile();
}
#endif
