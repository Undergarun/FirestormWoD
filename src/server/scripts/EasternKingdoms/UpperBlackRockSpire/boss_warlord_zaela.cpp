////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "upper_blackrock_spire.hpp"

enum eSpells
{
    ///< Misc
    BurningBridge               = 166721,
    BurningBridgeDoT            = 166730,
    BurningBreath               = 166040,

    ///< Destructive Smite
    DestructiveSmiteNormal      = 155673,
    DestructiveSmiteHeroic      = 169158,

    ///< Rebounding Blade
    ReboundingBladeSearcher     = 155705,
    ReboundingBladeDamages      = 155711,

    ///< Black Iron Cyclone
    BlackIronCycloneSearcher    = 155720,
    BlackIronCycloneSpell       = 155721,
    BlackIronCycloneAura        = 166149
};

enum eEvents
{
    DestructiveSmite = 1,
    ReboundingBlade,
    BlackIronCyclone,
    EndBlackIronCyclone,
    BackOnTheGround,
    SummonRiders,
    BurningBreathTime
};

enum eSays
{
    TalkWave1_01,
    TalkWave1_02,
    TalkWave2,
    TalkWave3_01,
    TalkWave3_02,
    TalkIntro,
    TalkAggro,
    TalkSpell1,
    TalkSpell2,
    TalkSpell3,
    TalkSpell4,
    TalkDeath,
    TalkSlay
};

enum eActions
{
    ActionTharbekTalk1,
    ActionZaelaTalk1,
    ActionZaelaTalk2,
    ActionTharbekTalk2,
    ActionZaelaTalk3,
    ActionTharbekTalk3,
    ActionZaelaTalk4,
    ActionTharbekTalk4,
    ActionIronflightCirclePath,
    ActionFightRiders,
    ActionBurningBreath,
    ActionInitRiders
};

enum eMisc
{
    PhaseGround,
    PhaseAir,
    MaxBurningBridge = 11,
    MaxRidersToSpawn = 3,
    MaxBurningBreath = 8
};

static Position const g_BurningBridgePos[eMisc::MaxBurningBridge] =
{
    { 24.26327f, -141.7272f, 97.55093f, 4.784885f },
    { 23.47960f, -147.0169f, 97.50233f, 4.764563f },
    { 22.55750f, -154.0300f, 97.51096f, 4.744549f },
    { 20.71427f, -159.5637f, 97.81489f, 4.715597f },
    { 18.25090f, -164.5560f, 98.32717f, 4.682451f },
    { 15.71870f, -169.4781f, 98.78542f, 4.652592f },
    { 13.29070f, -174.3310f, 99.15034f, 4.627447f },
    { 11.23930f, -179.7610f, 99.72741f, 4.609943f },
    { 10.00140f, -184.1760f, 100.2681f, 4.601732f },
    { 8.860130f, -189.4292f, 100.8892f, 4.596208f },
    { 7.769579f, -194.1159f, 101.4526f, 4.591072f }
};

Position const g_IntroPos = { 147.5021f, -444.0213f, 121.9753f, 1.6032f };
static Position const g_SpawnPos = { 22.1094f, -103.059f, 97.7569f, 4.711267f };
static Position const g_IronflightSpawnPos = { 20.5842f, -46.9536f, 107.4032f, 4.7185f };
static Position const g_IronflightSecondPos = { 21.342f, -173.01f, 117.342f, 1.551f };
static Position const g_CenterPos = { 20.647974f, -122.817032f, 97.818550f, 1.531097f };

static Position const g_BurningBreathPos[eMisc::MaxBurningBreath] =
{
    { -5.649f, -110.458f, 97.000f, 0.053f },
    { 10.748f, -145.371f, 97.000f, 1.627f },
    { 45.843f, -130.213f, 97.000f, 3.184f },
    { 33.629f, -97.0580f, 97.000f, 4.660f },
    { -3.202f, -132.664f, 97.000f, 0.128f },
    { 33.935f, -143.047f, 97.000f, 1.573f },
    { 47.438f, -108.742f, 97.000f, 3.211f },
    { 6.1734f, -96.8873f, 97.000f, 4.817f }
};

///< Warlord Zaela - 77120
class boss_warlord_zaela : public CreatureScript
{
    public:
        boss_warlord_zaela() : CreatureScript("boss_warlord_zaela") { }

        struct boss_warlord_zaelaAI : public BossAI
        {
            boss_warlord_zaelaAI(Creature* p_Creature) : BossAI(p_Creature, eDatas::DATA_WARLORD_ZAELA)
            {
                m_Instance               = p_Creature->GetInstanceScript();
                m_TharbekIntroDone       = false;
                m_FirstTalkTimer         = 0;
                m_SecondTalkTimer        = 0;
                m_IntroDone              = false;
                m_BlackIronCycloneTarget = 0;
                m_EmberscaleIronflight   = 0;
                m_NextHealthPct          = 60.0f;
                m_Phase = eMisc::PhaseGround;
            }

            InstanceScript* m_Instance;
            bool m_TharbekIntroDone;
            EventMap m_Events;

            uint32 m_FirstTalkTimer;
            uint32 m_SecondTalkTimer;

            uint64 m_BlackIronCycloneTarget;
            uint64 m_EmberscaleIronflight;
            Position m_JumpPos;
            uint32 m_BurningBreathCount;

            float m_NextHealthPct;
            eMisc m_Phase;

            bool m_IntroDone;

            void Reset()
            {
                me->RemoveAllAreasTrigger();

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED);

                m_Events.Reset();

                _Reset();

                summons.DespawnAll();

                if (m_TharbekIntroDone)
                {
                    m_JumpPos = g_SpawnPos;
                    me->NearTeleportTo(m_JumpPos.m_positionX, m_JumpPos.m_positionY, m_JumpPos.m_positionZ, m_JumpPos.m_orientation);
                }
                else
                {
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    me->NearTeleportTo(g_IntroPos.m_positionX, g_IntroPos.m_positionY, g_IntroPos.m_positionZ, g_IntroPos.m_orientation);
                }

                m_Phase = eMisc::PhaseGround;
                m_BurningBreathCount = 0;
                m_NextHealthPct = 60.0f;
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                Talk(eSays::TalkSlay);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                _EnterCombat();

                Talk(eSays::TalkAggro);

                m_Events.ScheduleEvent(eEvents::DestructiveSmite, 10000);
                m_Events.ScheduleEvent(eEvents::ReboundingBlade, 1000);
                m_Events.ScheduleEvent(eEvents::BlackIronCyclone, 5000);

                SpawnBurningBridge();
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                Talk(eSays::TalkDeath);

                me->RemoveAllAreasTrigger();
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TypeID::TYPEID_PLAYER || p_Who->GetDistance(me) > 35.f)
                    return;

                if (!m_TharbekIntroDone && m_Instance != nullptr && m_Instance->GetBossState(eDatas::DATA_COMMANDER_THARBEK) != EncounterState::DONE)
                {
                    m_TharbekIntroDone = true;
                    Talk(eSays::TalkWave1_01);

                    if (m_Instance)
                    {
                        if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(eCreatures::NPC_COMMANDER_THARBEK)))
                            l_Tharbek->AI()->DoAction(eActions::ActionTharbekTalk1);
                    }
                }
                else if (m_TharbekIntroDone && !m_IntroDone && m_Instance != nullptr && m_Instance->GetBossState(eDatas::DATA_COMMANDER_THARBEK) == EncounterState::DONE)
                {
                    Talk(eSays::TalkIntro);
                    m_IntroDone = true;

                    std::list<Creature*> l_Emberscales;
                    me->GetCreatureListWithEntryInGrid(l_Emberscales, eCreatures::NPC_EMBERSCALE_IRONFLIGHT_2, 250.0f);

                    for (Creature* l_Ironflight : l_Emberscales)
                    {
                        if (l_Ironflight->GetAI())
                            l_Ironflight->AI()->DoAction(eActions::ActionInitRiders);
                    }

                    return;
                }
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case eActions::ActionZaelaTalk1:
                        m_FirstTalkTimer = 8500;
                        break;
                    case eActions::ActionZaelaTalk2:
                    {
                        Talk(eSays::TalkWave2);

                        if (m_Instance)
                        {
                            if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(eCreatures::NPC_COMMANDER_THARBEK)))
                                l_Tharbek->AI()->DoAction(eActions::ActionTharbekTalk2);
                        }

                        break;
                    }
                    case eActions::ActionZaelaTalk3:
                    {
                        Talk(eSays::TalkWave3_01);

                        if (m_Instance)
                        {
                            if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(eCreatures::NPC_COMMANDER_THARBEK)))
                                l_Tharbek->AI()->DoAction(eActions::ActionTharbekTalk3);
                        }

                        break;
                    }
                    case eActions::ActionZaelaTalk4:
                        m_SecondTalkTimer = 10000;
                        break;
                    default:
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo)
            {
                if (p_Victim == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::ReboundingBladeSearcher:
                        me->CastSpell(p_Victim, eSpells::ReboundingBladeDamages, true);
                        break;
                    case eSpells::BlackIronCycloneSearcher:
                        m_BlackIronCycloneTarget = p_Victim->GetGUID();
                        me->CastSpell(me, eSpells::BlackIronCycloneSpell, false);
                        break;
                    case eSpells::BlackIronCycloneAura:
                    {
                        if (Player* l_Target = Player::GetPlayer(*me, m_BlackIronCycloneTarget))
                        {
                            Position l_Pos;
                            l_Target->GetPosition(&l_Pos);
                            me->GetMotionMaster()->MovePoint(0, l_Pos);
                        }
                        m_Events.ScheduleEvent(eEvents::EndBlackIronCyclone, 4500);
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (m_Phase == eMisc::PhaseAir)
                {
                    p_Damage = 0;
                    return;
                }

                if (m_NextHealthPct > 0.0f && me->HealthBelowPctDamaged(m_NextHealthPct, p_Damage))
                {
                    m_NextHealthPct = 0.0f;
                    m_Phase = eMisc::PhaseAir;
                    m_Events.ScheduleEvent(eEvents::BackOnTheGround, 40000);
                    m_Events.ScheduleEvent(eEvents::SummonRiders, 1000);

                    if (urand(0, 1))
                        Talk(eSays::TalkSpell2);
                    else
                        Talk(eSays::TalkSpell1);

                    if (Creature* l_Ironflight = me->SummonCreature(eCreatures::NPC_EMBERSCALE_IRONFLIGHT_2, g_IronflightSpawnPos))
                    {
                        m_EmberscaleIronflight = l_Ironflight->GetGUID();
                        l_Ironflight->SetSpeed(UnitMoveType::MOVE_FLIGHT, 2.0f);

                        me->GetPosition(&m_JumpPos);
                        l_Ironflight->GetMotionMaster()->MovePoint(0, m_JumpPos.m_positionX, m_JumpPos.m_positionY, m_JumpPos.m_positionZ + 5.0f);
                        me->EnterVehicle(l_Ironflight);
                        l_Ironflight->AI()->DoAction(eActions::ActionIronflightCirclePath);
                    }

                    m_Events.ScheduleEvent(eEvents::BurningBreathTime, 100);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                ScheduleFirstTalk(p_Diff);
                ScheduleSecondTalk(p_Diff);

                if (!UpdateVictim())
                {
                    if (m_Phase == eMisc::PhaseAir)
                        Reset();
                    return;
                }

                m_Events.Update(p_Diff);

                ///< Update Cyclone's target here
                if (me->HasAura(eSpells::BlackIronCycloneAura))
                {
                    if (Player* l_Target = Player::GetPlayer(*me, m_BlackIronCycloneTarget))
                    {
                        Position l_Pos;
                        l_Target->GetPosition(&l_Pos);
                        me->GetMotionMaster()->MovePoint(0, l_Pos);
                        return;
                    }
                }

                ///< Update target movements here to avoid some movements problems
                if (me->getVictim() && !me->IsWithinMeleeRange(me->getVictim()))
                {
                    Position l_Pos;
                    me->getVictim()->GetPosition(&l_Pos);
                    me->GetMotionMaster()->MovePoint(0, l_Pos);
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::DestructiveSmite:
                    {
                        if (m_Phase == eMisc::PhaseAir)
                        {
                            m_Events.ScheduleEvent(eEvents::DestructiveSmite, 1000);
                            break;
                        }

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, IsHeroic() ? eSpells::DestructiveSmiteHeroic : eSpells::DestructiveSmiteNormal, true);
                        m_Events.ScheduleEvent(eEvents::DestructiveSmite, 15500);
                        break;
                    }
                    case eEvents::ReboundingBlade:
                    {
                        if (m_Phase == eMisc::PhaseAir)
                        {
                            m_Events.ScheduleEvent(eEvents::ReboundingBlade, 1000);
                            break;
                        }

                        me->CastSpell(me, eSpells::ReboundingBladeSearcher, true);
                        m_Events.ScheduleEvent(eEvents::ReboundingBlade, 10500);
                        break;
                    }
                    case eEvents::BlackIronCyclone:
                    {
                        if (m_Phase == eMisc::PhaseAir)
                        {
                            m_Events.ScheduleEvent(eEvents::BlackIronCyclone, 1000);
                            break;
                        }

                        Talk(eSays::TalkSpell3);
                        me->CastSpell(me, eSpells::BlackIronCycloneSearcher, true);
                        m_Events.ScheduleEvent(eEvents::BlackIronCyclone, 19500);
                        break;
                    }
                    case eEvents::EndBlackIronCyclone:
                        if (Unit* l_Target = me->getVictim())
                            AttackStart(l_Target);
                        break;
                    case eEvents::BackOnTheGround:
                    {
                        m_Events.CancelEvent(eEvents::BurningBreathTime);

                        if (Creature* l_Ironflight = Creature::GetCreature(*me, m_EmberscaleIronflight))
                        {
                            l_Ironflight->GetMotionMaster()->Clear();
                            l_Ironflight->GetMotionMaster()->MovePoint(0, m_JumpPos.m_positionX, m_JumpPos.m_positionY, m_JumpPos.m_positionZ + 10.0f);
                            me->ExitVehicle(&m_JumpPos);
                            me->NearTeleportTo(m_JumpPos.m_positionX, m_JumpPos.m_positionY, m_JumpPos.m_positionZ, m_JumpPos.m_orientation);
                            l_Ironflight->DespawnOrUnsummon(50);
                            m_Phase = eMisc::PhaseGround;
                            Talk(eSays::TalkSpell4);
                        }
                        break;
                    }
                    case eEvents::SummonRiders:
                    {
                        for (uint8 l_I = 0; l_I < eMisc::MaxRidersToSpawn; ++l_I)
                        {
                            Position l_Pos = g_CenterPos;
                            float l_Orientation = frand(0, 2 * M_PI);

                            l_Pos.m_positionX = g_CenterPos.m_positionX + 15.0f * cosf(l_Orientation);
                            l_Pos.m_positionY = g_CenterPos.m_positionY + 15.0f * sinf(l_Orientation);

                            if (Creature* l_Rider = me->SummonCreature(eCreatures::NPC_BLACK_IRON_WYRM_RIDER, l_Pos))
                            {
                                l_Rider->SetReactState(ReactStates::REACT_AGGRESSIVE);

                                if (me->getVictim())
                                    l_Rider->AI()->AttackStart(me->getVictim());
                                l_Rider->AI()->DoAction(eActions::ActionFightRiders);
                            }
                        }

                        break;
                    }
                    case eEvents::BurningBreathTime:
                    {
                        std::list<Creature*> l_EmberscaleList;
                        me->GetCreatureListWithEntryInGrid(l_EmberscaleList, eCreatures::NPC_EMBERSCALE_IRONFLIGHT_2, 200.0f);

                        l_EmberscaleList.remove_if([this](Creature* p_Creature) -> bool
                        {
                            if (p_Creature == nullptr)
                                return true;

                            if (p_Creature->GetGUID() == m_EmberscaleIronflight)
                                return true;

                            if (p_Creature->HasAura(eSpells::BurningBreath))
                                return true;

                            return false;
                        });

                        if (l_EmberscaleList.empty())
                            break;

                        if (Creature* l_Emberscale = JadeCore::Containers::SelectRandomContainerElement(l_EmberscaleList))
                        {
                            l_Emberscale->AI()->SetData(0, m_BurningBreathCount);
                            l_Emberscale->AI()->DoAction(eActions::ActionBurningBreath);
                        }

                        ++m_BurningBreathCount;

                        if (m_BurningBreathCount >= eMisc::MaxBurningBreath)
                            m_BurningBreathCount = 0;

                        m_Events.ScheduleEvent(eEvents::BurningBreathTime, IsHeroic() ? 6000 : 10000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleFirstTalk(const uint32 p_Diff)
            {
                if (!m_FirstTalkTimer)
                    return;

                if (m_FirstTalkTimer <= p_Diff)
                {
                    m_FirstTalkTimer = 0;
                    Talk(eSays::TalkWave1_02);
                }
                else
                    m_FirstTalkTimer -= p_Diff;
            }

            void ScheduleSecondTalk(const uint32 p_Diff)
            {
                if (!m_SecondTalkTimer)
                    return;

                if (m_SecondTalkTimer <= p_Diff)
                {
                    m_SecondTalkTimer = 0;
                    Talk(eSays::TalkWave3_02);

                    if (m_Instance)
                    {
                        if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(eCreatures::NPC_COMMANDER_THARBEK)))
                            l_Tharbek->AI()->DoAction(eActions::ActionTharbekTalk4);
                    }

                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    me->NearTeleportTo(g_SpawnPos.m_positionX, g_SpawnPos.m_positionY, g_SpawnPos.m_positionZ, g_SpawnPos.m_orientation);
                }
                else
                    m_SecondTalkTimer -= p_Diff;
            }

            void SpawnBurningBridge()
            {
                for (uint8 l_I = 0; l_I < eMisc::MaxBurningBridge; ++l_I)
                {
                    Position const l_Pos = g_BurningBridgePos[l_I];
                    me->CastSpell(l_Pos.m_positionX, l_Pos.m_positionY, l_Pos.m_positionZ, eSpells::BurningBridge, true);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_warlord_zaelaAI(p_Creature);
        }
};

///< Emberscale Ironflight - 82428
class mob_zaela_emberscale_ironfight : public CreatureScript
{
    public:
        mob_zaela_emberscale_ironfight() : CreatureScript("mob_zaela_emberscale_ironfight") { }

        struct mob_zaela_emberscale_ironfightAI : public ScriptedAI
        {
            mob_zaela_emberscale_ironfightAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_BossFlight = false;
                m_BurningBreathCount = 0;
                m_RiderGuid = 0;
            }

            enum Events
            {
                CheckNearRider = 1,
                MoveSecond,
                CirclePath
            };

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            EventMap m_Events;
            bool m_BossFlight;

            uint32 m_BurningBreathCount;
            uint64 m_RiderGuid;

            void Reset()
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(Events::CheckNearRider, 2000);
                m_Events.ScheduleEvent(Events::CirclePath, 5000);

                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                if (Creature* l_Rider = me->FindNearestCreature(eCreatures::NPC_BLACK_IRON_WYRM_RIDER, 10.0f))
                    m_RiderGuid = l_Rider->GetGUID();
            }

            void EnterEvadeMode()
            {
                if (m_BossFlight)
                    me->DespawnOrUnsummon();
            }

            void FillCirclePath(Position const& p_Center, float p_Radius, float p_Z, Movement::PointsArray& p_Path, bool p_Clockwise)
            {
                float l_Step = p_Clockwise ? -M_PI / 8.0f : M_PI / 8.0f;
                float l_Angle = p_Center.GetAngle(me->GetPositionX(), me->GetPositionY());

                for (uint8 l_Iter = 0; l_Iter < 16; l_Angle += l_Step, ++l_Iter)
                {
                    G3D::Vector3 l_Point;
                    l_Point.x = p_Center.GetPositionX() + p_Radius * cosf(l_Angle);
                    l_Point.y = p_Center.GetPositionY() + p_Radius * sinf(l_Angle);
                    l_Point.z = p_Z;
                    p_Path.push_back(l_Point);
                }
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eActions::ActionIronflightCirclePath:
                        m_Events.ScheduleEvent(Events::MoveSecond, 500);
                        m_Events.ScheduleEvent(Events::CirclePath, 5000);
                        break;
                    case eActions::ActionBurningBreath:
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MovePoint(999, g_BurningBreathPos[m_BurningBreathCount]);
                        break;
                    case eActions::ActionInitRiders:
                    {
                        if (Creature* l_Rider = me->FindNearestCreature(eCreatures::NPC_BLACK_IRON_WYRM_RIDER, 10.0f))
                        {
                            l_Rider->EnterVehicle(me);
                            l_Rider->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        }
                        else if (Creature* l_Rider = Creature::GetCreature(*me, m_RiderGuid))
                        {
                            l_Rider->EnterVehicle(me);
                            l_Rider->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID)
            {
                if (p_ID == 999)
                {
                    float l_Orientation = g_BurningBreathPos[m_BurningBreathCount].m_orientation;

                    me->SetOrientation(l_Orientation);
                    me->SetFacingTo(l_Orientation);

                    me->CastSpell(me, eSpells::BurningBreath, false);
                    m_Events.ScheduleEvent(Events::CirclePath, 10000);
                }
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (p_Summoner->GetEntry() == eCreatures::NPC_WARLORD_ZAELA)
                {
                    m_BossFlight = true;
                    m_Events.CancelEvent(Events::CirclePath);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case Events::CheckNearRider:
                    {
                        if (m_BossFlight)
                            break;

                        if (Creature* l_Rider = me->FindNearestCreature(eCreatures::NPC_BLACK_IRON_WYRM_RIDER, 10.0f))
                        {
                            l_Rider->EnterVehicle(me);
                            l_Rider->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        }
                        else if (Creature* l_Rider = Creature::GetCreature(*me, m_RiderGuid))
                        {
                            l_Rider->EnterVehicle(me);
                            l_Rider->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        }

                        break;
                    }
                    case Events::MoveSecond:
                        me->GetMotionMaster()->MovePoint(0, g_IronflightSecondPos);
                        break;
                    case Events::CirclePath:
                    {
                        Movement::MoveSplineInit l_Init(me);

                        ///< Creating the circle path from the center
                        FillCirclePath(g_CenterPos, 45.0f, me->GetPositionZ() + 10.0f, l_Init.Path(), true);
                        l_Init.SetWalk(true);
                        l_Init.SetCyclic();
                        l_Init.Launch();
                        break;
                    }
                    default:
                        break;
                }
            }

            void SetData(uint32 p_ID, uint32 p_Value)
            {
                m_BurningBreathCount = p_Value;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_zaela_emberscale_ironfightAI(p_Creature);
        }
};

///< Black Iron Wyrm Rider - 82429
class mob_zaela_black_iron_wyrm_rider : public CreatureScript
{
    public:
        mob_zaela_black_iron_wyrm_rider() : CreatureScript("mob_zaela_black_iron_wyrm_rider") { }

        struct mob_zaela_black_iron_wyrm_riderAI : public ScriptedAI
        {
            mob_zaela_black_iron_wyrm_riderAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            enum Spells
            {
                ChainPullSpell = 167241,
                DragonRoar     = 167242
            };

            enum Events
            {
                ChainPull = 1,
                TriggerDragonRoar
            };

            InstanceScript* m_Instance;

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == eActions::ActionFightRiders)
                {
                    m_Events.ScheduleEvent(Events::ChainPull, 5000);

                    if (IsHeroic())
                        m_Events.ScheduleEvent(Events::TriggerDragonRoar, 6000);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case Events::ChainPull:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, Spells::ChainPullSpell, false);
                        m_Events.ScheduleEvent(Events::ChainPull, 15000);
                        break;
                    case Events::TriggerDragonRoar:
                        me->CastSpell(me, Spells::DragonRoar, true);
                        m_Events.ScheduleEvent(Events::TriggerDragonRoar, 6000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_zaela_black_iron_wyrm_riderAI(p_Creature);
        }
};

///< Burning Bridge - 166721
class areatrigger_burning_bridge : public AreaTriggerEntityScript
{
    public:
        areatrigger_burning_bridge() : AreaTriggerEntityScript("areatrigger_burning_bridge") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 3.5f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, eSpells::BurningBridgeDoT, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_burning_bridge();
        }
};

///< Burning Breath (triggered) - 166041
class spell_burning_breath: public SpellScriptLoader
{
    public:
        spell_burning_breath() : SpellScriptLoader("spell_burning_breath") { }

        class spell_burning_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_burning_breath_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!l_Caster->isInFront(p_Object, M_PI / 6.0f))
                        return true;

                    return false;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_burning_breath_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_129);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_burning_breath_SpellScript();
        }
};

void AddSC_boss_warlord_zaela()
{
    new boss_warlord_zaela();
    new mob_zaela_emberscale_ironfight();
    new mob_zaela_black_iron_wyrm_rider();
    new areatrigger_burning_bridge();
    new spell_burning_breath();
}
