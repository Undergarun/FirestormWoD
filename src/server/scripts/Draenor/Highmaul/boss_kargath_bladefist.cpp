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
            KargathLastTalk
        };

        enum eMoves
        {
            MoveFrontGate = 1,
            MoveChargeOnPlayer
        };

        enum eCosmeticEvents
        {
            OrientationForFight = 1
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

            /// Fight
            /// Impale
            SpellImpale             = 159113,
            /// Blade Dance
            SpellBladeDance         = 159250,
            SpellBladeDanceHit      = 159212,
            SpellBladeDanceMorph    = 159214,
            SpellBladeDanceFadeOut  = 159209,
            SpellBladeDanceCharge   = 159265
        };

        enum eEvents
        {
            EventImpale = 1,
            EventBladeDance
        };

        enum eCreatures
        {
            KargathBladefist    = 78846 ///< Used for Blade Dance
        };

        struct boss_kargath_bladefistAI : public BossAI
        {
            boss_kargath_bladefistAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossKargathBladefist), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = p_Creature->GetInstanceScript();
                p_Creature->SetReactState(ReactStates::REACT_PASSIVE);
            }

            EventMap m_Events;
            EventMap m_CosmeticEvents;
            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eSpells::BladeFistAmputation);
                me->RemoveAura(eSpells::PlayChogallScene);

                me->SetDisplayId(eDatas::MorphWithWeapon);
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

                m_Events.ScheduleEvent(eEvents::EventImpale, 35000);
                m_Events.ScheduleEvent(eEvents::EventBladeDance, 3000);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                Talk(eTalks::Death);

                me->CastSpell(me, eSpells::BladeFistAmputation, true);
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
                    default:
                        break;
                }
            }

            void JustReachedHome() override
            {
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

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
                        p_Target->EnterVehicle(me);
                        break;
                    case eSpells::SpellBladeDanceHit:
                    {
                        Position l_Pos;
                        p_Target->GetPosition(&l_Pos);

                        if (Creature* l_Trigger = me->SummonCreature(eCreatures::KargathBladefist, l_Pos, TempSummonType::TEMPSUMMON_TIMED_DESPAWN, 5000))
                        {
                            me->CastSpell(l_Trigger, eSpells::SpellBladeDanceCharge, true);
                            l_Trigger->CastSpell(l_Trigger, eSpells::SpellBladeDanceMorph, true);
                            l_Trigger->CastSpell(l_Trigger, eSpells::SpellBladeDanceFadeOut, true);
                        }

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
                        break;
                    }
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventImpale:
                    {
                        Talk(eTalks::Impale);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::SpellImpale, false);

                        m_Events.ScheduleEvent(eEvents::EventImpale, 44000);
                        break;
                    }
                    case eEvents::EventBladeDance:
                    {
                        me->CastSpell(me, eSpells::SpellBladeDance, true);
                        m_Events.ScheduleEvent(eEvents::EventBladeDance, 23000);
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
            EarthBreakerSearch  = 163933
        };

        enum eEvents
        {
            EventCleave = 1,
            EventEarthBreaker
        };

        struct npc_highmaul_vulgorAI : public ScriptedAI
        {
            npc_highmaul_vulgorAI(Creature* p_Creature) : ScriptedAI(p_Creature)
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

                m_Events.ScheduleEvent(eEvents::EventCleave, 5000);
                m_Events.ScheduleEvent(eEvents::EventEarthBreaker, 8000);

                summons.DespawnAll();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalks::Aggro);
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void JustSummoned(Creature* p_Summon) override
            {
                summons.Summon(p_Summon);
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
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_STATE_READY1H);
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
            }

            void JustDied(Unit* p_Killer) override
            {
                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    if (Creature* l_Jhorn = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::JhornTheMad)))
                        l_Jhorn->AI()->DoAction(eActions::VulgorDied);

                    if (Creature* l_Thoktar = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::ThoktarIronskull)))
                        l_Thoktar->AI()->DoAction(eActions::VulgorDied);

                    if (Creature* l_MargokCosmetic = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::MargokCosmetic)))
                        l_MargokCosmetic->AI()->DoAction(eActions::VulgorDied);
                }
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

        struct npc_highmaul_fire_pillarAI : public ScriptedAI
        {
            npc_highmaul_fire_pillarAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
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

        struct npc_highmaul_ravenous_bloodmawAI : public ScriptedAI
        {
            npc_highmaul_ravenous_bloodmawAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
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
            BladeDanceDmg       = 159217,
            BladeDanceCharge    = 159265
        };

        struct npc_highmaul_kargath_bladefist_triggerAI : public ScriptedAI
        {
            npc_highmaul_kargath_bladefist_triggerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void SpellHit(Unit* p_Caster, SpellInfo const* p_SpellInfo) override
            {
                if (p_Caster == nullptr)
                    return;

                if (p_SpellInfo->Id == eSpells::BladeDanceCharge)
                {
                    p_Caster->CastSpell(me, eSpells::BladeDanceDmg, true);
                    me->CastSpell(p_Caster, eSpells::BladeDanceCharge, true);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_kargath_bladefist_triggerAI(p_Creature);
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

        class spell_highmaul_impale_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_impale_AuraScript);

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
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

    /// Spells
    new spell_highmaul_earth_breaker();
    new spell_highmaul_impale();

    /// AreaTriggers
    new areatrigger_highmaul_molten_bomb();
}