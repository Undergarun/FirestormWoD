////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

Position const g_TrashsSpawnPos = { 3427.1f, 7530.21f, 55.3383f, 0.965533f };
Position const g_VulgorMovePos = { 3449.81f, 7557.01f, 55.304f, 0.8995f };

Position const g_SorcererPos[2] =
{
    { 3452.08f, 7550.25f, 55.304f, 0.8995f },
    { 3441.68f, 7558.45f, 55.304f, 0.8995f }
};

Position const g_SorcererSecondPos[2] =
{
    { 3445.23f, 7575.57f, 55.30f, 0.235625f },
    { 3468.69f, 7557.72f, 55.30f, 1.708251f }
};

Position const g_HighmaulSweeperMoves[eHighmaulDatas::HighmaulSweeperCount][eHighmaulDatas::HighmaulSweeperMovesCount] =
{
    /// Right of Mar'gok
    {
        { 3448.05f, 7517.22f, 65.45f, 6.157836f },
        { 3465.95f, 7520.59f, 65.15f, 0.196663f },
        { 3487.64f, 7532.83f, 65.44f, 0.554020f },
        { 3503.33f, 7548.39f, 65.44f, 0.777640f },
        { 3513.44f, 7564.58f, 65.44f, 1.017186f },
        { 3521.71f, 7590.66f, 65.09f, 1.241025f },
        { 3521.19f, 7607.86f, 65.44f, 1.598777f },
        { 3514.75f, 7623.96f, 65.44f, 1.932571f },
        { 3507.99f, 7629.20f, 65.43f, 2.466646f }
    },
    /// Left of Mar'gok
    {
        { 3411.12f, 7546.78f, 65.44f, 1.558138f },
        { 3410.64f, 7564.97f, 65.01f, 1.559507f },
        { 3418.62f, 7590.24f, 65.45f, 1.241420f },
        { 3429.80f, 7608.05f, 65.44f, 1.025436f },
        { 3444.61f, 7621.95f, 65.42f, 0.774108f },
        { 3464.96f, 7634.39f, 65.44f, 0.538489f },
        { 3483.57f, 7637.60f, 65.44f, 0.157571f },
        { 3500.82f, 7634.95f, 65.44f, 6.122672f },
        { 3507.86f, 7629.08f, 65.41f, 5.608234f }
    }
};

/// Kargath Bladefist <Warlord of the Shattered Hand> - 78714
class boss_kargath_bladefist : public CreatureScript
{
    public:
        boss_kargath_bladefist() : CreatureScript("boss_kargath_bladefist") { }

        enum eTalks
        {
            Intro1,
            Intro2,
            Aggro,
            BerserkerRush,
            ChainHurl,
            Impale,
            Berserk,
            FlamePillar,
            Slay,
            Death
        };

        enum eActions
        {
            VulgorDied = 2,
            KargathLastTalk,
            InterruptByPillar
        };

        enum eMoves
        {
            MoveFrontGate = 1,
            MoveChargeOnPlayer
        };

        enum eCosmeticEvents
        {
            OrientationForFight = 1,
            ChainHurlVehicleEnter
        };

        enum eDatas
        {
            MorphWithWeapon = 54674,
            MorphAmputation = 55201
        };

        enum eSpells
        {
            /// Cosmetic
            BladeFistAmputation     = 167593,
            PlayChogallScene        = 178333,
            KargathDiesCrowdSound   = 166861,
            KargathChantingSound    = 168278,

            /// Fight
            /// Impale
            SpellImpale             = 159113,
            SpellImpaleMorph        = 160728,
            /// Blade Dance
            SpellBladeDance         = 159250,
            SpellBladeDanceHit      = 159212,
            SpellBladeDanceCharge   = 159265,
            /// Fire Pillars
            FirePillarTargetSelect  = 159705,
            FirePillarSelector      = 159712,
            TriggerCosmeticAura     = 160519,   ///< Fire aura when breaked by Fire Pillar
            /// Berserker Rush
            BerserkerRushSearcher   = 163180,
            SpellBerserkerRush      = 158986,
            BerserkerRushIncreasing = 159028,   ///< Increase speed and damage done every 2s
            BerserkerRushDamage     = 159001,   ///< Triggers damaging spell 159002 every 2s
            /// Chain Hurl
            ChainHurlJumpAndKnock   = 160061,
            MonstersBrawlTimer      = 159213,

            SpellBerserker          = 26662,

            /// In Mythic difficulty, the raid must perform well to gain favor that is tracked by Roar of the Crowd.
            /// If the raid gains enough favor from Roar of the Crowd, the raid is granted increased damage dealing.
            SpellRoarOfTheCrowd     = 163302,   ///< Enable Alt Power
            CrowdFavorite25         = 163366,
            CrowdFavorite50         = 163368,
            CrowdFavorite75         = 163369,
            CrowdFavorite100        = 163370
        };

        enum eEvents
        {
            EventImpale = 1,
            EventBladeDance,
            EventOpenGrates,
            EventBerserkerRush,
            EventChainHurl,
            EventBerserker
        };

        enum eCreatures
        {
            KargathBladefist    = 78846,    ///< Used for Blade Dance
            ChainHurlVehicle    = 79134
        };

        struct boss_kargath_bladefistAI : public BossAI
        {
            boss_kargath_bladefistAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossKargathBladefist), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = p_Creature->GetInstanceScript();
                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);

                m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::ChainHurlVehicleEnter, 2000);
            }

            EventMap m_Events;
            EventMap m_CosmeticEvents;
            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            uint64 m_BerserkerRushTarget;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eSpells::BladeFistAmputation);
                me->RemoveAura(eSpells::PlayChogallScene);
                me->RemoveAura(eSpells::FirePillarTargetSelect);

                me->SetDisplayId(eDatas::MorphWithWeapon);

                m_BerserkerRushTarget = 0;
            }

            void KilledUnit(Unit* p_Who) override
            {
                if (p_Who->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                Talk(eTalks::Aggro);

                ///m_Events.ScheduleEvent(eEvents::EventImpale, 35000);
                ///m_Events.ScheduleEvent(eEvents::EventBladeDance, 3000);
                ///m_Events.ScheduleEvent(eEvents::EventOpenGrates, 4000);
                ///m_Events.ScheduleEvent(eEvents::EventBerserkerRush, 48000);
                m_Events.ScheduleEvent(eEvents::EventChainHurl, 91000);
                ///m_Events.ScheduleEvent(eEvents::EventBerserker, 600000);

                ///me->CastSpell(me, eSpells::FirePillarTargetSelect, true);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                Talk(eTalks::Death);

                me->CastSpell(me, eSpells::BladeFistAmputation, true);
                me->CastSpell(me, eSpells::KargathDiesCrowdSound, true);

                me->SetDisplayId(eDatas::MorphAmputation);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::VulgorDied:
                    {
                        if (m_Instance == nullptr)
                            break;

                        if (Creature* l_Trigger = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::CrowdAreatrigger)))
                        {
                            float l_X = l_Trigger->GetPositionX();
                            float l_Y = l_Trigger->GetPositionY();
                            float l_Z = l_Trigger->GetPositionZ();

                            me->GetMotionMaster()->MoveJump(l_X, l_Y, l_Z, 25.0f, 10.0f);
                            Talk(eTalks::Intro1);
                            m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::OrientationForFight, 500);
                        }

                        break;
                    }
                    case eActions::KargathLastTalk:
                        Talk(eTalks::Intro2);
                        break;
                    case eActions::InterruptByPillar:
                        Talk(eTalks::FlamePillar);
                        me->CastSpell(me, eSpells::TriggerCosmeticAura, true);
                        me->RemoveAura(eSpells::BerserkerRushDamage);
                        me->InterruptNonMeleeSpells(true);
                        /// Break Pillar
                        break;
                    default:
                        break;
                }
            }

            void JustReachedHome() override
            {
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::ChainHurlVehicleEnter, 2000);

                Reset();
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                if (p_ID == eMoves::MoveFrontGate)
                {
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    me->SetWalk(false);
                    m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::OrientationForFight, 500);
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellImpale:
                    {
                        if (m_Vehicle == nullptr)
                            break;

                        p_Target->EnterVehicle(me, 0, true);
                        break;
                    }
                    case eSpells::SpellBladeDanceHit:
                    {
                        Position l_Pos;
                        p_Target->GetPosition(&l_Pos);

                        if (Creature* l_Trigger = me->SummonCreature(eCreatures::KargathBladefist, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
                            me->CastSpell(l_Trigger, eSpells::SpellBladeDanceCharge, true);

                        break;
                    }
                    case eSpells::BerserkerRushSearcher:
                    {
                        me->CastSpell(p_Target, eSpells::SpellBerserkerRush, false);
                        break;
                    }
                    case eSpells::SpellBerserkerRush:
                    {
                        m_BerserkerRushTarget = p_Target->GetGUID();

                        Position l_Pos;
                        p_Target->GetPosition(&l_Pos);
                        me->GetMotionMaster()->MovePoint(0, l_Pos);

                        me->CastSpell(me, eSpells::BerserkerRushIncreasing, true);
                        me->CastSpell(me, eSpells::BerserkerRushDamage, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    case eCosmeticEvents::OrientationForFight:
                    {
                        me->SetOrientation(0.90f);

                        Position l_Pos;
                        me->GetPosition(&l_Pos);
                        me->SetHomePosition(l_Pos);

                        me->CastSpell(me, eSpells::KargathChantingSound, true);
                        break;
                    }
                    case eCosmeticEvents::ChainHurlVehicleEnter:
                    {
                        Position l_Pos;
                        me->GetPosition(&l_Pos);

                        if (Creature* l_ChainHurl = me->SummonCreature(eCreatures::ChainHurlVehicle, l_Pos))
                            l_ChainHurl->EnterVehicle(me, 1, true);

                        break;
                    }
                    default:
                        break;
                }

                /// Update Berserker Rush move
                if (me->HasAura(eSpells::BerserkerRushIncreasing) && m_BerserkerRushTarget)
                {
                    if (Player* l_Target = Player::GetPlayer(*me, m_BerserkerRushTarget))
                    {
                        Position l_Pos;
                        l_Target->GetPosition(&l_Pos);
                        me->GetMotionMaster()->MovePoint(0, l_Pos);
                    }
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                /// Update moves here, avoid some movements problems after Berserker Rush
                if (me->getVictim() && !me->IsWithinMeleeRange(me->getVictim()))
                {
                    Position l_Pos;
                    me->getVictim()->GetPosition(&l_Pos);
                    me->GetMotionMaster()->MovePoint(0, l_Pos);
                }

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventImpale:
                    {
                        Talk(eTalks::Impale);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellImpale, false);

                        me->CastSpell(me, eSpells::SpellImpaleMorph, true);
                        m_Events.ScheduleEvent(eEvents::EventImpale, 43500);
                        break;
                    }
                    case eEvents::EventBladeDance:
                    {
                        me->CastSpell(me, eSpells::SpellBladeDance, true);
                        m_Events.ScheduleEvent(eEvents::EventBladeDance, 23000);
                        break;
                    }
                    case eEvents::EventOpenGrates:
                    {
                        if (m_Instance == nullptr)
                            break;

                        for (uint8 l_I = eHighmaulDatas::RaidGrate001; l_I < eHighmaulDatas::MaxRaidGrates; ++l_I)
                        {
                            if (GameObject* l_RaidGrate = GameObject::GetGameObject(*me, m_Instance->GetData64(eHighmaulGameobjects::RaidGrate1 + l_I)))
                                l_RaidGrate->SetGoState(GOState::GO_STATE_ACTIVE);
                        }

                        break;
                    }
                    case eEvents::EventBerserkerRush:
                    {
                        Talk(eTalks::BerserkerRush);
                        me->CastSpell(me, eSpells::BerserkerRushSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventBerserkerRush, 45000);
                        break;
                    }
                    case eEvents::EventChainHurl:
                    {
                        Talk(eTalks::ChainHurl);
                        me->CastSpell(me, eSpells::ChainHurlJumpAndKnock, true);
                        m_Events.ScheduleEvent(eEvents::EventChainHurl, 106000);
                        break;
                    }
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eSpells::SpellBerserker, true);
                        Talk(eTalks::Berserk);
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
            return new boss_kargath_bladefistAI(p_Creature);
        }
};

/// Vul'gor <The Shadow of Highmaul> - 80048
class npc_highmaul_vulgor : public CreatureScript
{
    public:
        npc_highmaul_vulgor() : CreatureScript("npc_highmaul_vulgor") { }

        enum eActions
        {
            StartIntro,
            ContinueIntro,
            VulgorDied
        };

        enum eMove
        {
            MoveInArena
        };

        enum eTalks
        {
            Aggro,
            Slay,
            Spell
        };

        enum eSpells
        {
            SpellCleave         = 161712,
            SpellEarthBreaker   = 162271,
            EarthBreakerSearch  = 163933,
            SpellEyeOfTheTiger  = 161716,

            VulgorDieCrowdSound = 166860
        };

        enum eEvents
        {
            EventCleave = 1,
            EventEarthBreaker,
            EventEyeOfTheTiger
        };

        struct npc_highmaul_vulgorAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_vulgorAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature), m_Summons(p_Creature)
            {
                m_Instance          = p_Creature->GetInstanceScript();
                m_HealthPct         = 30;
                m_IntroContinued    = false;
                m_SorcererGuids[0]  = 0;
                m_SorcererGuids[1]  = 0;

                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }

            InstanceScript* m_Instance;
            int32 m_HealthPct;
            bool m_IntroContinued;

            uint64 m_SorcererGuids[2];

            EventMap m_Events;
            SummonList m_Summons;

            void Reset() override
            {
                if (Creature* l_FirstSorcerer = Creature::GetCreature(*me, m_SorcererGuids[0]))
                {
                    l_FirstSorcerer->Respawn();
                    l_FirstSorcerer->GetMotionMaster()->Clear();
                    l_FirstSorcerer->GetMotionMaster()->MoveTargetedHome();
                }

                if (Creature* l_SecondSorcerer = Creature::GetCreature(*me, m_SorcererGuids[1]))
                {
                    l_SecondSorcerer->Respawn();
                    l_SecondSorcerer->GetMotionMaster()->Clear();
                    l_SecondSorcerer->GetMotionMaster()->MoveTargetedHome();
                }

                m_Events.Reset();

                m_Summons.DespawnAll();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalks::Aggro);

                m_Events.ScheduleEvent(eEvents::EventCleave, 5000);
                m_Events.ScheduleEvent(eEvents::EventEarthBreaker, 8000);
                m_Events.ScheduleEvent(eEvents::EventEyeOfTheTiger, 18000);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustSummoned(Creature* p_Summon) override
            {
                m_Summons.Summon(p_Summon);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::EarthBreakerSearch)
                {
                    me->SetFacingTo(me->GetAngle(p_Target));
                    me->CastSpell(p_Target, eSpells::SpellEarthBreaker, false);
                }
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::StartIntro)
                {
                    std::list<Creature*> l_BladespireSorcerers;
                    me->GetCreatureListWithEntryInGrid(l_BladespireSorcerers, eHighmaulCreatures::BladespireSorcerer, 15.0f);

                    if (l_BladespireSorcerers.size() == 2)
                    {
                        uint8 l_Count = 0;
                        for (Creature* l_Sorcerer : l_BladespireSorcerers)
                        {
                            l_Sorcerer->SetWalk(true);
                            l_Sorcerer->GetMotionMaster()->MovePoint(eMove::MoveInArena, g_SorcererPos[l_Count]);
                            m_SorcererGuids[l_Count] = l_Sorcerer->GetGUID();

                            ++l_Count;
                        }
                    }

                    me->SetWalk(true);
                    me->GetMotionMaster()->MovePoint(eMove::MoveInArena, g_VulgorMovePos);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (m_Instance == nullptr || p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMove::MoveInArena:
                    {
                        me->SetWalk(false);
                        me->SetHomePosition(g_VulgorMovePos);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_ONESHOT_ROAR);
                        AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_STATE_READY1H); });
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_IntroContinued || m_Instance == nullptr)
                    return;

                if (me->HealthBelowPctDamaged(m_HealthPct, p_Damage))
                {
                    m_IntroContinued = true;

                    if (Creature* l_Jhorn = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::JhornTheMad)))
                        l_Jhorn->AI()->DoAction(eActions::ContinueIntro);

                    if (Creature* l_Thoktar = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ThoktarIronskull)))
                        l_Thoktar->AI()->DoAction(eActions::ContinueIntro);

                    /// Spawn new trash mobs
                    for (uint8 l_I = 0; l_I < 2; ++l_I)
                    {
                        if (Creature* l_Sorcerer = me->SummonCreature(eHighmaulCreatures::BladespireSorcerer, g_TrashsSpawnPos))
                            l_Sorcerer->GetMotionMaster()->MovePoint(eMove::MoveInArena, g_SorcererSecondPos[l_I]);
                    }
                }
            }

            void JustReachedHome() override
            {
                me->GetMotionMaster()->MovePoint(0, g_VulgorMovePos);

                m_IntroContinued = false;
            }

            void JustDied(Unit* p_Killer) override
            {
                m_Summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Jhorn = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::JhornTheMad)))
                        l_Jhorn->AI()->DoAction(eActions::VulgorDied);

                    if (Creature* l_Thoktar = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ThoktarIronskull)))
                        l_Thoktar->AI()->DoAction(eActions::VulgorDied);

                    if (Creature* l_MargokCosmetic = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::MargokCosmetic)))
                        l_MargokCosmetic->AI()->DoAction(eActions::VulgorDied);
                }

                me->CastSpell(me, eSpells::VulgorDieCrowdSound, true);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellCleave, true);
                        m_Events.ScheduleEvent(eEvents::EventCleave, 19000);
                        break;
                    case eEvents::EventEarthBreaker:
                        me->CastSpell(me, eSpells::EarthBreakerSearch, true);
                        m_Events.ScheduleEvent(eEvents::EventEarthBreaker, 15000);
                        break;
                    case eEvents::EventEyeOfTheTiger:
                        me->CastSpell(me, eSpells::SpellEyeOfTheTiger, true);
                        m_Events.ScheduleEvent(eEvents::EventEyeOfTheTiger, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_vulgorAI(p_Creature);
        }
};

/// Bladespire Sorcerer - 80071
class npc_highmaul_bladespire_sorcerer : public CreatureScript
{
    public:
        npc_highmaul_bladespire_sorcerer() : CreatureScript("npc_highmaul_bladespire_sorcerer") { }

        enum eMove
        {
            MoveInArena
        };

        enum eSpells
        {
            SpellMoltenBombSearcher = 161630,
            SpellMoltenBomb         = 161631,
            SpellFlameBoltSearcher  = 162369,
            SpellFlameBolt          = 162351,
            SpellSearingArmor       = 162231,
            SpellSearingArmorAura   = 177705
        };

        enum eEvents
        {
            EventMoltenBomb = 1,
            EventFlameBolt,
            EventSearingArmor
        };

        struct npc_highmaul_bladespire_sorcererAI : public ScriptedAI
        {
            npc_highmaul_bladespire_sorcererAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
                p_Creature->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }

            EventMap m_Events;
            Position m_Position;

            void Reset() override
            {
                m_Events.Reset();

                me->RemoveAllAreasTrigger();
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case eMove::MoveInArena:
                    {
                        me->SetWalk(false);
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_STATE_READY2H);
                        me->GetPosition(&m_Position);
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvents::EventMoltenBomb, 5000);
                m_Events.ScheduleEvent(eEvents::EventFlameBolt, 2000);
                m_Events.ScheduleEvent(eEvents::EventSearingArmor, 8000);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::SpellMoltenBombSearcher:
                        me->CastSpell(p_Target, eSpells::SpellMoltenBomb, true);
                        break;
                    case eSpells::SpellFlameBoltSearcher:
                        me->CastSpell(p_Target, eSpells::SpellFlameBolt, false);
                        break;
                    case eSpells::SpellSearingArmor:
                        me->ClearUnitState(UnitState::UNIT_STATE_CASTING);
                        me->CastSpell(p_Target, eSpells::SpellSearingArmorAura, true);
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();
            }

            void JustReachedHome() override
            {
                me->GetMotionMaster()->MovePoint(0, m_Position);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventMoltenBomb:
                        me->CastSpell(me, eSpells::SpellMoltenBombSearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventMoltenBomb, 15000);
                        break;
                    case eEvents::EventFlameBolt:
                        me->CastSpell(me, eSpells::SpellFlameBoltSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventFlameBolt, 10000);
                        break;
                    case eEvents::EventSearingArmor:
                        me->CastSpell(me, eSpells::SpellSearingArmor, true);
                        me->ClearUnitState(UnitState::UNIT_STATE_CASTING);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_bladespire_sorcererAI(p_Creature);
        }
};

/// Smoldering Stoneguard - 80051
class npc_highmaul_somldering_stoneguard : public CreatureScript
{
    public:
        npc_highmaul_somldering_stoneguard() : CreatureScript("npc_highmaul_somldering_stoneguard") { }

        enum eSpell
        {
            SpellCleave = 161703
        };

        enum eEvent
        {
            EventCleave = 1
        };

        struct npc_highmaul_somldering_stoneguardAI : public ScriptedAI
        {
            npc_highmaul_somldering_stoneguardAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventCleave, 2000);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvent::EventCleave:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpell::SpellCleave, false);
                        m_Events.ScheduleEvent(eEvent::EventCleave, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_somldering_stoneguardAI(p_Creature);
        }
};

/// Fire Pillar - 78757
class npc_highmaul_fire_pillar : public CreatureScript
{
    public:
        npc_highmaul_fire_pillar() : CreatureScript("npc_highmaul_fire_pillar") { }

        enum eSpells
        {
            FirePillarSelector      = 159712,
            FirePillarActivated     = 159706,
            FirePillarKnockback     = 158994,
            FirePillarSteamTimer    = 163967,

            FlameJet                = 159202,   ///< AreaTrigger
            KargathTrigger          = 159070,
            KargathTrigger2         = 159073
        };

        struct npc_highmaul_fire_pillarAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_fire_pillarAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_LOOTING | eUnitFlags::UNIT_FLAG_TAXI_FLIGHT);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IN_COMBAT | eUnitFlags::UNIT_FLAG_PLAYER_CONTROLLED);
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo) override
            {
                if (p_Caster == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::FirePillarSelector:
                        me->CastSpell(me, eSpells::FirePillarSteamTimer, true);
                        me->CastSpell(me, eSpells::FirePillarActivated, true);
                        me->CastSpell(me, eSpells::FirePillarKnockback, true, nullptr, NULLAURA_EFFECT, p_Caster->GetGUID());

                        AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::FlameJet, true); });

                        me->GetMotionMaster()->MoveCharge(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 4.0f);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IN_COMBAT);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_fire_pillarAI(p_Creature);
        }
};

/// Ravenous Bloodmaw - 79296
class npc_highmaul_ravenous_bloodmaw : public CreatureScript
{
    public:
        npc_highmaul_ravenous_bloodmaw() : CreatureScript("npc_highmaul_ravenous_bloodmaw") { }

        enum eSpell
        {
            Generic1HMeleeInstakill = 162445
        };

        struct npc_highmaul_ravenous_bloodmawAI : public ScriptedAI
        {
            npc_highmaul_ravenous_bloodmawAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_CheckPlayer;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                m_CheckPlayer = 1000;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_CheckPlayer)
                    return;

                if (m_CheckPlayer <= p_Diff)
                {
                    std::list<Player*> l_PlrList;
                    me->GetPlayerListInGrid(l_PlrList, 2.0f);

                    for (Player* l_Player : l_PlrList)
                    {
                        if (!l_Player->isAlive())
                            continue;

                        me->CastSpell(l_Player, eSpell::Generic1HMeleeInstakill, true);
                    }

                    m_CheckPlayer = 1000;
                }
                else
                    m_CheckPlayer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_ravenous_bloodmawAI(p_Creature);
        }
};

/// Kargath Bladefist (trigger) - 78846
class npc_highmaul_kargath_bladefist_trigger : public CreatureScript
{
    public:
        npc_highmaul_kargath_bladefist_trigger() : CreatureScript("npc_highmaul_kargath_bladefist_trigger") { }

        enum eSpells
        {
            BladeDanceDmg           = 159217,
            BladeDanceCharge        = 159265,
            SpellBladeDanceFadeOut  = 159209
        };

        enum eData
        {
            MorphWithWeapon = 54674
        };

        struct npc_highmaul_kargath_bladefist_triggerAI : public ScriptedAI
        {
            npc_highmaul_kargath_bladefist_triggerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_ONESHOT_ATTACK1H);
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo) override
            {
                if (p_Caster == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::BladeDanceCharge)
                {
                    me->CastSpell(me, eSpells::BladeDanceDmg, true, nullptr, NULLAURA_EFFECT, p_Caster->GetGUID());
                    me->SetDisplayId(eData::MorphWithWeapon);
                    me->CastSpell(p_Caster, eSpells::BladeDanceCharge, true);
                    me->CastSpell(me, eSpells::SpellBladeDanceFadeOut, true);
                    me->DespawnOrUnsummon(1500);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_kargath_bladefist_triggerAI(p_Creature);
        }
};

/// Drunken Bileslinger - 78954
class npc_highmaul_drunken_bileslinger : public CreatureScript
{
    public:
        npc_highmaul_drunken_bileslinger() : CreatureScript("npc_highmaul_drunken_bileslinger") { }

        struct npc_highmaul_drunken_bileslingerAI : public ScriptedAI
        {
            npc_highmaul_drunken_bileslingerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_drunken_bileslingerAI(p_Creature);
        }
};

/// Iron Bomber - 78926
class npc_highmaul_iron_bomber : public CreatureScript
{
    public:
        npc_highmaul_iron_bomber() : CreatureScript("npc_highmaul_iron_bomber") { }

        struct npc_highmaul_iron_bomberAI : public ScriptedAI
        {
            npc_highmaul_iron_bomberAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_iron_bomberAI(p_Creature);
        }
};

/// Iron Grunt - 84946
class npc_highmaul_iron_grunt : public CreatureScript
{
    public:
        npc_highmaul_iron_grunt() : CreatureScript("npc_highmaul_iron_grunt") { }

        struct npc_highmaul_iron_gruntAI : public ScriptedAI
        {
            npc_highmaul_iron_gruntAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_iron_gruntAI(p_Creature);
        }
};

/// Highmaul Sweeper - 88874
class npc_highmaul_highmaul_sweeper : public CreatureScript
{
    public:
        npc_highmaul_highmaul_sweeper() : CreatureScript("npc_highmaul_highmaul_sweeper") { }

        enum eSpells
        {
            ArenaSweeper        = 177776,
            MonstersBrawl       = 177815,
            ArenaSweeperImpact  = 177802
        };

        enum eAction
        {
            KickOutPlayers
        };

        enum eSweeperType
        {
            TypeRight,
            TypeLeft
        };

        struct npc_highmaul_highmaul_sweeperAI : public ScriptedAI
        {
            npc_highmaul_highmaul_sweeperAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint8 m_MoveID;
            uint8 m_SweeperType;
            bool m_ComeBack;

            void Reset() override
            {
                m_MoveID        = 0;
                m_SweeperType   = me->GetOrientation() >= 6.0f ? eSweeperType::TypeRight : eSweeperType::TypeLeft;
                m_ComeBack      = false;

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::KickOutPlayers)
                {
                    me->CastSpell(me, eSpells::ArenaSweeper, true);
                    me->GetMotionMaster()->MovePoint(m_MoveID, g_HighmaulSweeperMoves[m_SweeperType][m_MoveID]);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE)
                    return;

                if (m_MoveID < (eHighmaulDatas::HighmaulSweeperMovesCount - 1) && !m_ComeBack)
                {
                    ++m_MoveID;
                    me->GetMotionMaster()->MovePoint(m_MoveID, g_HighmaulSweeperMoves[m_SweeperType][m_MoveID]);
                }
                else if (m_MoveID == (eHighmaulDatas::HighmaulSweeperMovesCount - 1))
                {
                    m_ComeBack = true;
                    --m_MoveID;
                    me->GetMotionMaster()->MovePoint(m_MoveID, g_HighmaulSweeperMoves[m_SweeperType][m_MoveID]);
                }
                else if (m_MoveID == 0 && m_ComeBack)
                    m_ComeBack = false;
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::MonstersBrawl)
                    me->CastSpell(p_Target, eSpells::ArenaSweeperImpact, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_highmaul_sweeperAI(p_Creature);
        }
};

/// Earth Breaker - 162271
class spell_highmaul_earth_breaker : public SpellScriptLoader
{
    public:
        spell_highmaul_earth_breaker() : SpellScriptLoader("spell_highmaul_earth_breaker") { }

        class spell_highmaul_earth_breaker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_earth_breaker_SpellScript);

            enum eDatas
            {
                TargetRestrict  = 20675,
                EarthMissile    = 162472,
                SmolderingStone = 80051
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eDatas::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Radius = GetSpellInfo()->Effects[1].CalcRadius(l_Caster);
                p_Targets.remove_if([l_Radius, l_Caster, l_Restriction](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->IsInAxe(l_Caster, l_Restriction->Width, l_Radius))
                        return true;

                    return false;
                });
            }

            void HandleBeforeCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_Radius = 30.0f;
                    float l_O = l_Caster->GetOrientation();
                    float l_X = l_Caster->GetPositionX() + (l_Radius * cos(l_O));
                    float l_Y = l_Caster->GetPositionY() + (l_Radius * sin(l_O));
                    float l_Z = l_Caster->GetPositionZ();
                    l_Caster->CastSpell(l_X, l_Y, l_Z, eDatas::EarthMissile, true);
                    l_Caster->SummonCreature(eDatas::SmolderingStone, l_X, l_Y, l_Z, l_O, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 45000);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_earth_breaker_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_129);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_earth_breaker_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_CONE_ENEMY_129);
                BeforeCast += SpellCastFn(spell_highmaul_earth_breaker_SpellScript::HandleBeforeCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_earth_breaker_SpellScript();
        }
};

/// Impale - 159113
class spell_highmaul_impale : public SpellScriptLoader
{
    public:
        spell_highmaul_impale() : SpellScriptLoader("spell_highmaul_impale") { }

        enum eSpell
        {
            OpenWounds = 159178
        };

        class spell_highmaul_impale_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_impale_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                Unit* l_Caster = GetCaster();
                if (l_Target == nullptr || l_Caster == nullptr)
                    return;

                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AuraRemoveMode::AURA_REMOVE_BY_DEATH)
                    l_Caster->CastSpell(l_Target, eSpell::OpenWounds, true);

                l_Target->ExitVehicle();
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_impale_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_PERIODIC_DAMAGE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_impale_AuraScript();
        }
};

/// Fire Pillar Steam Timer - 163967
class spell_highmaul_fire_pillar_steam_timer : public SpellScriptLoader
{
    public:
        spell_highmaul_fire_pillar_steam_timer() : SpellScriptLoader("spell_highmaul_fire_pillar_steam_timer") { }

        enum eSpell
        {
            FirePillarSteamCosmetic = 163970
        };

        class spell_highmaul_fire_pillar_steam_timer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_fire_pillar_steam_timer_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                l_Target->CastSpell(l_Target, eSpell::FirePillarSteamCosmetic, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_fire_pillar_steam_timer_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_fire_pillar_steam_timer_AuraScript();
        }
};

/// Fire Pillar Activated - 159706
class spell_highmaul_fire_pillar_activated : public SpellScriptLoader
{
    public:
        spell_highmaul_fire_pillar_activated() : SpellScriptLoader("spell_highmaul_fire_pillar_activated") { }

        class spell_highmaul_fire_pillar_activated_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_fire_pillar_activated_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                /// Deactivate Pillar
                l_Target->RemoveAllAreasTrigger();
                l_Target->GetMotionMaster()->MoveCharge(l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ() - 4.0f);
                l_Target->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IN_COMBAT);
                l_Target->RemoveAllAuras();
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_fire_pillar_activated_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_fire_pillar_activated_AuraScript();
        }
};

/// Berserker Rush - 159028
class spell_highmaul_berserker_rush : public SpellScriptLoader
{
    public:
        spell_highmaul_berserker_rush() : SpellScriptLoader("spell_highmaul_berserker_rush") { }

        enum eAction
        {
            InterruptByPillar = 4
        };

        enum eSpell
        {
            BerserkerRushIncrease = 159029
        };

        class spell_highmaul_berserker_rush_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_berserker_rush_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();

                if (Creature* l_Kargath = l_Target->ToCreature())
                {
                    if (l_Kargath->IsAIEnabled && l_RemoveMode != AuraRemoveMode::AURA_REMOVE_BY_EXPIRE)
                        l_Kargath->AI()->DoAction(eAction::InterruptByPillar);
                    else if (l_Kargath->IsAIEnabled)
                    {
                        if (Unit* l_Victim = l_Kargath->AI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            l_Kargath->GetMotionMaster()->Clear();
                            l_Kargath->AI()->AttackStart(l_Victim);
                        }
                    }

                    l_Kargath->RemoveAura(eSpell::BerserkerRushIncrease);
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_berserker_rush_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_berserker_rush_AuraScript();
        }
};

/// Chain Hurl - 159947
class spell_highmaul_chain_hurl : public SpellScriptLoader
{
    public:
        spell_highmaul_chain_hurl() : SpellScriptLoader("spell_highmaul_chain_hurl") { }

        enum eDatas
        {
            MaxAffectedTargets  = 5,
            SpellObscured       = 160131,
            OpenWounds          = 159178,
            MaxLFRHealer        = 1,
            MaxLFRDamagers      = 3,
            MaxLFRTank          = 1
        };

        class spell_highmaul_chain_hurl_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_chain_hurl_SpellScript);

            uint64 m_Targets[eDatas::MaxAffectedTargets];

            bool Load() override
            {
                ////////////////////////////////////////////////////////////////////////
                /// TEMP
                ////////////////////////////////////////////////////////////////////////
                return false;

                for (uint8 l_I = 0; l_I < eDatas::MaxAffectedTargets; ++l_I)
                    m_Targets[l_I] = 0;

                return true;
            }

            bool TargetsAlreadySelected() const
            {
                for (uint8 l_I = 0; l_I < eDatas::MaxAffectedTargets; ++l_I)
                {
                    if (!m_Targets[l_I])
                        return false;
                }

                return true;
            }

            void ProcessLFRTargetting(std::list<WorldObject*>& p_Targets, std::list<Player*> p_PlayerList)
            {
                std::list<Player*> l_TanksList      = p_PlayerList;
                std::list<Player*> l_HealersList    = p_PlayerList;
                std::list<Player*> l_DamagersList   = p_PlayerList;

                uint8 l_OpenWoundsStacks = 0;
                l_TanksList.remove_if([this, &l_OpenWoundsStacks](Player* p_Player) -> bool
                {
                    if (p_Player->GetRoleForGroup() != Roles::ROLE_TANK)
                        return true;

                    if (AuraPtr l_OpenWounds = p_Player->GetAura(eDatas::OpenWounds))
                    {
                        if (l_OpenWounds->GetStackAmount() > l_OpenWoundsStacks)
                            l_OpenWoundsStacks = l_OpenWounds->GetStackAmount();
                    }

                    return false;
                });

                if (l_TanksList.empty())
                    return;

                l_TanksList.remove_if([this, l_OpenWoundsStacks](Player* p_Player) -> bool
                {
                    if (p_Player->GetRoleForGroup() != Roles::ROLE_TANK)
                        return true;

                    if (AuraPtr l_OpenWounds = p_Player->GetAura(eDatas::OpenWounds))
                    {
                        if (l_OpenWounds->GetStackAmount() != l_OpenWoundsStacks)
                            return true;
                    }

                    return false;
                });

                if (l_TanksList.empty())
                    return;

                /// Just in case of all tanks have the same amount of Open Wounds
                JadeCore::RandomResizeList(l_TanksList, eDatas::MaxLFRTank);

                l_HealersList.remove_if([this](Player* p_Player) -> bool
                {
                    if (p_Player->GetRoleForGroup() != Roles::ROLE_HEALER)
                        return true;

                    return false;
                });

                if (l_HealersList.empty())
                    return;

                JadeCore::RandomResizeList(l_HealersList, eDatas::MaxLFRHealer);

                l_DamagersList.remove_if([this](Player* p_Player) -> bool
                {
                    if (p_Player->GetRoleForGroup() != Roles::ROLE_DAMAGE)
                        return true;

                    return false;
                });

                if (l_DamagersList.empty() || l_DamagersList.size() < eDatas::MaxLFRDamagers)
                    return;

                JadeCore::RandomResizeList(l_DamagersList, eDatas::MaxLFRDamagers);

                uint8 l_Count = 0;

                for (Player* l_Player : l_TanksList)
                {
                    p_Targets.push_back(l_Player);
                    m_Targets[l_Count] = l_Player->GetGUID();
                    ++l_Count;
                }

                for (Player* l_Player : l_HealersList)
                {
                    p_Targets.push_back(l_Player);
                    m_Targets[l_Count] = l_Player->GetGUID();
                    ++l_Count;
                }

                for (Player* l_Player : l_DamagersList)
                {
                    p_Targets.push_back(l_Player);
                    m_Targets[l_Count] = l_Player->GetGUID();
                    ++l_Count;
                }
            }

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                p_Targets.clear();

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                if (TargetsAlreadySelected())
                {
                    for (uint8 l_I = 0; l_I < eDatas::MaxAffectedTargets; ++l_I)
                    {
                        /// Spell has SPELL_ATTR3_ONLY_TARGET_PLAYERS
                        if (WorldObject* l_Object = Player::GetPlayer(*l_Caster, m_Targets[l_I]))
                            p_Targets.push_back(l_Object);
                    }

                    return;
                }

                std::list<Player*> l_PlayerList;
                l_Caster->GetPlayerListInGrid(l_PlayerList, 300.0f);

                if (l_PlayerList.empty())
                    return;

                l_PlayerList.remove_if(JadeCore::UnitAuraCheck(true, eDatas::SpellObscured));

                if (l_PlayerList.empty())
                    return;

                /// In Looking for Raid difficulty, Kargath automatically tosses the tank with the highest amount of Open Wounds stacks,
                /// One random healer, and three random DPS into the crowd.
                if (l_Caster->GetMap() && l_Caster->GetMap()->IsLFR())
                    ProcessLFRTargetting(p_Targets, l_PlayerList);
                /// Kargath uses his chain to lash the 5 closest enemies and toss them into the arena's stands.
                else
                {
                    l_PlayerList.sort(JadeCore::ObjectDistanceOrderPred(l_Caster));

                    uint8 l_Count = 0;
                    for (Player* l_Player : l_PlayerList)
                    {
                        if (l_Count >= eDatas::MaxAffectedTargets)
                            break;

                        m_Targets[l_Count] = l_Player->GetGUID();
                        p_Targets.push_back(l_Player);
                        ++l_Count;
                    }
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_chain_hurl_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_chain_hurl_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENEMY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_chain_hurl_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_chain_hurl_SpellScript();
        }
};

/// Chain Hurl (Vehicle Aura) - 159948
class spell_highmaul_chain_hurl_vehicle : public SpellScriptLoader
{
    public:
        spell_highmaul_chain_hurl_vehicle() : SpellScriptLoader("spell_highmaul_chain_hurl_vehicle") { }

        enum eSpell
        {
            ChainHurlJumpDest = 159995
        };

        class spell_highmaul_chain_hurl_vehicle_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_chain_hurl_vehicle_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                Unit* l_Target = GetTarget();
                if (l_Target == nullptr)
                    return;

                l_Target->CastSpell(l_Target, eSpell::ChainHurlJumpDest, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_chain_hurl_vehicle_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_CONTROL_VEHICLE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_chain_hurl_vehicle_AuraScript();
        }
};

/// Molten Bomb - 161634
class areatrigger_highmaul_molten_bomb : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_molten_bomb() : AreaTriggerEntityScript("areatrigger_highmaul_molten_bomb") { }

        enum eSpell
        {
            MoltenBomb = 161635
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 3.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                    l_Caster->CastSpell(l_Unit, eSpell::MoltenBomb, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_molten_bomb();
        }
};

/// Flame Jet - 159202
class areatrigger_highmaul_flame_jet : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_flame_jet() : AreaTriggerEntityScript("areatrigger_highmaul_flame_jet") { }

        enum eSpell
        {
            FlameJet = 159311
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 6.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                    l_Caster->CastSpell(l_Unit, eSpell::FlameJet, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_flame_jet();
        }
};

void AddSC_boss_kargath_bladefist()
{
    /// Boss
    new boss_kargath_bladefist();

    /// NPCs
    new npc_highmaul_vulgor();
    new npc_highmaul_bladespire_sorcerer();
    new npc_highmaul_somldering_stoneguard();
    new npc_highmaul_fire_pillar();
    new npc_highmaul_ravenous_bloodmaw();
    new npc_highmaul_kargath_bladefist_trigger();
    new npc_highmaul_drunken_bileslinger();
    new npc_highmaul_iron_bomber();
    new npc_highmaul_iron_grunt();
    new npc_highmaul_highmaul_sweeper();

    /// Spells
    new spell_highmaul_earth_breaker();
    new spell_highmaul_impale();
    new spell_highmaul_fire_pillar_steam_timer();
    new spell_highmaul_fire_pillar_activated();
    new spell_highmaul_berserker_rush();
    new spell_highmaul_chain_hurl();
    new spell_highmaul_chain_hurl_vehicle();

    /// AreaTriggers
    new areatrigger_highmaul_molten_bomb();
    new areatrigger_highmaul_flame_jet();
}