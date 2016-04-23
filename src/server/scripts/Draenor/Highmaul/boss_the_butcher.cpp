////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "highmaul.hpp"

Position const g_MaggotSpawnPos[eHighmaulDatas::MaxMaggotToKill] =
{
    { 3827.051f, 7690.205f, 23.67708f, 0.000f },
    { 3751.507f, 7722.538f, 23.65485f, 4.332f },
    { 3704.671f, 7700.851f, 23.60431f, 3.872f },
    { 3687.367f, 7626.960f, 23.64627f, 1.488f },
    { 3743.118f, 7607.410f, 23.86531f, 5.192f },
    { 3799.805f, 7675.845f, 23.04378f, 3.110f }
};

G3D::Vector3 ComputeLocationSelection(Creature* p_Source, float p_SearchRange, float p_MinRadius, float p_Radius)
{
    using Cluster = std::set<Player*>;
    using GuidCluster = std::set<uint32>;

    std::list<Player*> l_Targets;

    p_Source->GetPlayerListInGrid(l_Targets, p_SearchRange);

    if (!l_Targets.empty())
    {
        l_Targets.remove_if([p_Source, p_MinRadius](Player* p_Player) -> bool
        {
            if (p_Player == nullptr)
                return true;

            if (p_Player->GetDistance(p_Source) < p_MinRadius)
                return true;

            return false;
        });
    }

    std::map<uint32, Cluster> l_ClusterMap;

    for (Player* l_Player : l_Targets)
    {
        Cluster l_Neighboor;
        for (Player* l_PlayerSecond : l_Targets)
        {
            float l_Dist = l_Player->GetDistance(l_PlayerSecond);
            if (l_Dist <= p_Radius)
                l_Neighboor.insert(l_PlayerSecond);
        }

        l_ClusterMap[l_Player->GetGUIDLow()] = Cluster();
        l_ClusterMap[l_Player->GetGUIDLow()].insert(l_Player);

        for (Player* l_PlayerSecond : l_Neighboor)
            l_ClusterMap[l_Player->GetGUIDLow()].insert(l_PlayerSecond);
    }

    size_t l_Size = 0;
    for (auto l_Cluster : l_ClusterMap)
    {
        if (l_Cluster.second.size() > l_Size)
            l_Size = l_Cluster.second.size();
    }

    G3D::Vector3 l_Pos;
    for (auto l_Cluster : l_ClusterMap)
    {
        if (l_Size && l_Cluster.second.size() == l_Size)
        {
            float l_X = 0.0f;
            float l_Y = 0.0f;

            for (Player* l_Player : l_Cluster.second)
            {
                l_X += l_Player->GetPositionX();
                l_Y += l_Player->GetPositionY();
            }

            l_X /= (uint32)l_Size;
            l_Y /= (uint32)l_Size;

            l_Pos.x = l_X;
            l_Pos.y = l_Y;
            l_Pos.z = p_Source->GetPositionZ();
            break;
        }
    }

    return l_Pos;
}

G3D::Vector3 GetCleaveLocation(Creature* p_Source)
{
    return ComputeLocationSelection(p_Source, 5.0f, 0.0f, 5.0f);
}

G3D::Vector3 GetBoundingCleaveLocation(Creature* p_Source)
{
    return ComputeLocationSelection(p_Source, 500.0f, 5.1f, 10.0f);
}

/// The Butcher - 77404
class boss_the_butcher : public CreatureScript
{
    public:
        boss_the_butcher() : CreatureScript("boss_the_butcher") { }

        enum eSpells
        {
            /// Heavy Handed
            HeavyHandedAura     = 156135,
            HeavyHandedProc     = 156138,
            /// The Cleaver
            TheCleaverDmg       = 156143,
            TheCleaverDot       = 156147,
            /// The Tenderizer
            TheTenderizer       = 156151,
            /// Meat Hook
            MeatHook            = 156125,
            MeatHookJump        = 156127,
            /// Bounding Cleave
            BoundingCleaveKnock = 156160,
            BoundingCleaveDummy = 156197,
            BoundingCleaveDmg   = 156172,
            BoundingCleaveCharg = 156171,
            /// Cleave
            SpellCleave         = 156157,
            SpellGushingWounds  = 156152,
            /// Energy management
            Angry5PerTick       = 156720,
            Angry10PerTick      = 160248,
            /// Frenzy (30%)
            SpellFrenzy         = 156598,
            /// Loot
            ButcherBonusLoot    = 177522
        };

        enum eEvents
        {
            EventTenderizer = 1,
            EventCleave,
            EventCleaver,
            EventBerserk,
            EventBoundingCleave,
            EventMeatHook,
            /// Mythic mode only
            EventCadaver
        };

        enum eTalks
        {
            Intro1,
            Intro2,
            Intro3, ///< Didn't see it on video
            Aggro,
            BoundingCleave,
            Frenzy,
            Cleave,
            Berserk,
            Slay,
            Wipe
        };

        enum eCreatures
        {
            NightTwistedCadaver = 82505,
            Maggot              = 80728
        };

        enum eAction
        {
            MaggotKilled
        };

        struct boss_the_butcherAI : public BossAI
        {
            boss_the_butcherAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTheButcher)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            bool m_IntroDone;

            /// Timers for prevent spamming for few abilities
            uint32 m_CleaveCooldown;

            /// Mythic mode only
            uint8 m_AddCount;

            std::set<uint8> m_MaggotSpawned;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eSpells::SpellFrenzy);
                me->RemoveAura(eHighmaulSpells::Berserker);
                me->RemoveAura(eSpells::Angry5PerTick);
                me->RemoveAura(eSpells::Angry10PerTick);

                me->SetSpeed(UnitMoveType::MOVE_SWIM, me->GetSpeed(UnitMoveType::MOVE_RUN) * 0.5f);

                m_CleaveCooldown = 0;

                m_AddCount = 0;

                me->SetCanDualWield(true);

                /// Wrong flags remove
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISARMED | eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_PREPARATION);

                me->SetPower(Powers::POWER_ENERGY, 0);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::TheCleaverDot);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::TheTenderizer);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellGushingWounds);
                }

                m_MaggotSpawned.clear();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                me->CastSpell(me, eSpells::Angry5PerTick, true);

                Talk(eTalks::Aggro);

                m_Events.ScheduleEvent(eEvents::EventTenderizer, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventCleave, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventCleaver, 12 * TimeConstants::IN_MILLISECONDS);

                if (!IsLFR())
                    m_Events.ScheduleEvent(eEvents::EventBerserk, IsMythic() ? 240 * TimeConstants::IN_MILLISECONDS : 300 * TimeConstants::IN_MILLISECONDS);

                /// Meat Hook is an ability that The Butcher uses to pull his tank to him.
                /// We assume that this ability exists to prevent The Butcher from being kited,
                /// but it is not otherwise in use during the fight.
                m_Events.ScheduleEvent(eEvents::EventMeatHook, 5 * TimeConstants::IN_MILLISECONDS);

                /// Mythic mode only
                if (IsMythic())
                {
                    m_AddCount = 0;
                    m_Events.ScheduleEvent(eEvents::EventCadaver, 18 * TimeConstants::IN_MILLISECONDS);
                }

                if (!IsLFR())
                {
                    uint8 l_I = urand(0, (eHighmaulDatas::MaxMaggotToKill - 1));

                    me->SummonCreature(eCreatures::Maggot, g_MaggotSpawnPos[l_I]);
                    m_MaggotSpawned.insert(l_I);
                }
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (me->HasAura(eSpells::SpellFrenzy))
                    return;

                if (me->HealthBelowPctDamaged(30, p_Damage))
                {
                    me->CastSpell(me, eSpells::SpellFrenzy, true);

                    me->RemoveAura(eSpells::Angry5PerTick);
                    me->CastSpell(me, eSpells::Angry10PerTick, true);

                    Talk(eTalks::Frenzy);
                }
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                Talk(eTalks::Wipe);

                if (m_Instance != nullptr)
                    m_Instance->SetBossState(eHighmaulDatas::BossTheButcher, EncounterState::FAIL);
            }

            void RegeneratePower(Powers p_Power, int32& p_Value) override
            {
                /// The Butcher only regens by script
                p_Value = 0;
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::TheCleaverDot);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::TheTenderizer);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellGushingWounds);

                    if (sObjectMgr->IsDisabledEncounter(m_Instance->GetEncounterIDForBoss(me), GetDifficulty()))
                        me->SetLootRecipient(nullptr);
                    else
                        CastSpellToPlayers(me->GetMap(), me, eSpells::ButcherBonusLoot, true);
                }
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER)
                    Talk(eTalks::Slay);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::EFFECT_MOTION_TYPE)
                    return;

                if (p_ID == eSpells::BoundingCleaveCharg)
                {
                    me->CastSpell(me, eSpells::BoundingCleaveDmg, true);

                    me->RemoveAura(eSpells::BoundingCleaveDummy);

                    AddTimedDelayedOperation(50, [this]() -> void
                    {
                        if (Unit* l_Target = me->getVictim())
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveChase(l_Target);
                        }
                    });
                }
            }

            void SpellMissTarget(Unit* p_Target, SpellInfo const* p_SpellInfo, SpellMissInfo p_MissInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::TheCleaverDmg:
                    {
                        /// This attack will not apply its bleed component if it is avoided.
                        if (p_MissInfo == SpellMissInfo::SPELL_MISS_DODGE)
                            break;
                        me->AddAura(eSpells::TheCleaverDot, p_Target);
                        break;
                    }
                    default:
                        break;
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::MeatHook:
                        p_Target->CastSpell(me, eSpells::MeatHookJump, true);
                        break;
                    case eSpells::SpellCleave:
                    case eSpells::BoundingCleaveDmg:
                        me->CastSpell(p_Target, eSpells::SpellGushingWounds, true);
                        break;
                    default:
                        break;
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                if (p_SpellInfo->Id == eSpells::BoundingCleaveCharg)
                {
                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            AttackStart(l_Target);

                        if (Creature* l_Maggot = me->FindNearestCreature(eCreatures::Maggot, 10.0f))
                            me->Kill(l_Maggot);
                    });
                }
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eAction::MaggotKilled)
                {
                    std::vector<uint8> l_Indexes = { 0, 1, 2, 3, 4, 5 };
                    std::random_shuffle(l_Indexes.begin(), l_Indexes.end());

                    for (uint8 l_I : l_Indexes)
                    {
                        if (m_MaggotSpawned.find(l_I) != m_MaggotSpawned.end())
                            continue;

                        me->SummonCreature(eCreatures::Maggot, g_MaggotSpawnPos[l_I]);
                        m_MaggotSpawned.insert(l_I);
                        break;
                    }

                    if (m_Instance != nullptr)
                        m_Instance->SetData(eHighmaulDatas::ButcherAchievement, 1);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!m_IntroDone)
                {
                    if (Player* l_Player = me->FindNearestPlayer(130.0f))
                    {
                        m_IntroDone = true;
                        AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro1); });
                        AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Talk(eTalks::Intro2); });
                    }
                }

                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                ScheduleEnergy(p_Diff);

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventTenderizer:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::TheTenderizer, true);
                        m_Events.ScheduleEvent(eEvents::EventTenderizer, 16 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCleave:
                    {
                        if (m_CleaveCooldown <= p_Diff)
                        {
                            m_CleaveCooldown = 20 * TimeConstants::IN_MILLISECONDS;
                            Talk(eTalks::Cleave);
                        }
                        else
                            m_CleaveCooldown -= p_Diff;

                        G3D::Vector3 l_Source = GetCleaveLocation(me);
                        me->CastSpell(l_Source, eSpells::SpellCleave, false);

                        AddTimedDelayedOperation(500, [this, l_Source]() -> void
                        {
                            G3D::Vector3 l_Target = l_Source;
                            l_Target.z += 0.1f;

                            /// Sniffed values
                            G3D::Vector3 l_Orientation(0.0f, 4.035325f, 0.0f);

                            me->PlayOrphanSpellVisual(l_Source, l_Orientation, l_Target, 37116);

                            if (Creature* l_Maggot = me->FindNearestCreature(eCreatures::Maggot, 5.0f))
                            {
                                Position l_Pos = { l_Target.x, l_Target.y, l_Target.z };
                                if (l_Maggot->IsNearPosition(&l_Pos, 5.0f))
                                    me->Kill(l_Maggot);
                            }
                        });

                        m_Events.ScheduleEvent(eEvents::EventCleave, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCleaver:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::TheCleaverDmg, true);
                        m_Events.ScheduleEvent(eEvents::EventCleaver, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventBerserk:
                    {
                        Talk(eTalks::Berserk);
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventBoundingCleave:
                    {
                        Talk(eTalks::BoundingCleave);
                        me->CastSpell(me, eSpells::BoundingCleaveKnock, true);
                        /// Charge on players after 8s
                        me->CastSpell(me, eSpells::BoundingCleaveDummy, false);

                        m_Events.DelayEvents(12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventMeatHook:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            if (!l_Target->IsWithinMeleeRange(me, 5.0f))
                                me->CastSpell(l_Target, eSpells::MeatHook, true);
                        }
                        m_Events.ScheduleEvent(eEvents::EventMeatHook, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCadaver:
                    {
                        /// Every four waves of adds, The Butcher spawns one add more
                        ++m_AddCount;
                        uint8 l_Count = floor(float(m_AddCount) / 4.0f) + 1;

                        float l_Radius = 50.0f;
                        float l_PosX = me->GetPositionX();
                        float l_PosY = me->GetPositionY();

                        for (uint8 l_I = 0; l_I < l_Count; ++l_I)
                        {
                            float l_Orientation = frand(0, 2 * M_PI);
                            float l_X = l_PosX + (l_Radius * cos(l_Orientation));
                            float l_Y = l_PosY + (l_Radius * sin(l_Orientation));
                            me->SummonCreature(eCreatures::NightTwistedCadaver, l_X, l_Y, me->GetPositionZ());
                        }

                        m_Events.ScheduleEvent(eEvents::EventCadaver, 14 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                EnterEvadeIfOutOfCombatArea(p_Diff);
                DoMeleeAttackIfReady();
            }

            void ScheduleEnergy(uint32 const p_Diff)
            {
                /// Bounding Cleave Icon Bounding Cleave is an ability that The Butcher uses when he reaches 100 Energy
                /// (this happens exactly every 60 seconds before The Butcher reaches 30% health, and every 30 seconds after that).
                if (me->GetPower(Powers::POWER_ENERGY) >= 100)
                    m_Events.ScheduleEvent(eEvents::EventBoundingCleave, 50);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_the_butcherAI(p_Creature);
        }
};

/// Night-Twisted Cadaver - 82505
class npc_highmaul_night_twisted_cadaver : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_cadaver() : CreatureScript("npc_highmaul_night_twisted_cadaver") { }

        enum eSpells
        {
            Paleobomb   = 163047,
            PaleVitriol = 163042
        };

        struct npc_highmaul_night_twisted_cadaverAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_night_twisted_cadaverAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            bool m_Explode;

            void Reset() override
            {
                m_Explode = false;

                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (m_Instance == nullptr)
                        return;

                    me->SetWalk(true);

                    if (Creature* l_Butcher = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::TheButcher)))
                        me->GetMotionMaster()->MoveChase(l_Butcher);
                });
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (m_Instance == nullptr || m_Explode)
                    return;

                if (Creature* l_Butcher = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::TheButcher)))
                {
                    if (me->GetDistance(l_Butcher) <= 1.0f)
                        ExplodeAndSpawnVitriol();
                }

                if (Player* l_Player = me->FindNearestPlayer(1.0f))
                    ExplodeAndSpawnVitriol();
            }

            void ExplodeAndSpawnVitriol()
            {
                me->GetMotionMaster()->Clear();

                m_Explode = true;

                me->CastSpell(me, eSpells::Paleobomb, true);
                me->CastSpell(me, eSpells::PaleVitriol, true);

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->Kill(me); });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_cadaverAI(p_Creature);
        }
};

/// Maggot - 80728
class npc_highmaul_maggot : public CreatureScript
{
    public:
        npc_highmaul_maggot() : CreatureScript("npc_highmaul_maggot") { }

        enum eAction
        {
            MaggotKilled
        };

        struct npc_highmaul_maggotAI : public ScriptedAI
        {
            npc_highmaul_maggotAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Butcher = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::TheButcher)))
                    {
                        if (l_Butcher->IsAIEnabled)
                            l_Butcher->AI()->DoAction(eAction::MaggotKilled);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_maggotAI(p_Creature);
        }
};

/// Heavy Handed - 156135
class spell_highmaul_heavy_handed : public SpellScriptLoader
{
    public:
        spell_highmaul_heavy_handed() : SpellScriptLoader("spell_highmaul_heavy_handed") { }

        enum eSpell
        {
            HeavyHandedProc = 156138
        };

        class spell_highmaul_heavy_handed_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_heavy_handed_AuraScript);

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (p_EventInfo.GetDamageInfo()->GetSpellInfo() != nullptr &&
                    p_EventInfo.GetDamageInfo()->GetSpellInfo()->Id == eSpell::HeavyHandedProc)
                    return;

                if (Unit* l_Butcher = GetTarget())
                {
                    if (Unit* l_Target = p_EventInfo.GetActionTarget())
                        l_Butcher->CastSpell(l_Target, eSpell::HeavyHandedProc, true);
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_highmaul_heavy_handed_AuraScript::OnProc, EFFECT_0, SPELL_AURA_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_heavy_handed_AuraScript();
        }
};

/// Heavy Handed - 156138
class spell_highmaul_heavy_handed_proc : public SpellScriptLoader
{
    public:
        spell_highmaul_heavy_handed_proc() : SpellScriptLoader("spell_highmaul_heavy_handed_proc") { }

        class spell_highmaul_heavy_handed_proc_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_heavy_handed_proc_SpellScript);

            uint64 m_Target;

            bool Load() override
            {
                m_Target = 0;
                return true;
            }

            void HandleBeforeCast()
            {
                if (Unit* l_Target = GetExplTargetUnit())
                    m_Target = l_Target->GetGUID();
            }

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                /// All attacks made by the caster strike the next closest target
                if (p_Targets.size() > 1)
                {
                    p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                    {
                        if (p_Object == nullptr || p_Object->GetGUID() == m_Target)
                            return true;

                        return false;
                    });

                    if (p_Targets.size() > 1)
                    {
                        p_Targets.sort(JadeCore::ObjectDistanceOrderPred(l_Caster));
                        WorldObject* l_Object = p_Targets.front();
                        p_Targets.clear();
                        p_Targets.push_back(l_Object);
                    }
                }
                /// If no target is found, the primary target is struck again.
                else
                {
                    p_Targets.clear();

                    if (Unit* l_Target = Unit::GetUnit(*l_Caster, m_Target))
                        p_Targets.push_back(l_Target);
                }
            }

            void Register() override
            {
                BeforeCast += SpellCastFn(spell_highmaul_heavy_handed_proc_SpellScript::HandleBeforeCast);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_heavy_handed_proc_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_heavy_handed_proc_SpellScript();
        }
};

/// Bounding Cleave (Periodic Dummy) - 156197
class spell_highmaul_bounding_cleave_dummy : public SpellScriptLoader
{
    public:
        spell_highmaul_bounding_cleave_dummy() : SpellScriptLoader("spell_highmaul_bounding_cleave_dummy") { }

        enum eSpell
        {
            BoundingCleaveCharge = 156171
        };

        class spell_highmaul_bounding_cleave_dummy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_bounding_cleave_dummy_AuraScript);

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (p_AurEff->GetTickNumber() != 8)
                    return;

                if (GetTarget() == nullptr)
                    return;

                Creature* l_Butcher = GetTarget()->ToCreature();
                if (l_Butcher == nullptr)
                    return;

                G3D::Vector3 l_Pos = GetBoundingCleaveLocation(l_Butcher);
                if (l_Pos.x == 0.0f || l_Pos.y == 0.0f || l_Pos.z == 0.0f)
                    l_Pos = ComputeLocationSelection(l_Butcher, 500.0f, 0.0f, 10.0f);

                l_Butcher->CastSpell(l_Pos, eSpell::BoundingCleaveCharge, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_bounding_cleave_dummy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_bounding_cleave_dummy_AuraScript();
        }
};

/// Gushing Wounds - 156152
class spell_highmaul_gushing_wounds : public SpellScriptLoader
{
    public:
        spell_highmaul_gushing_wounds() : SpellScriptLoader("spell_highmaul_gushing_wounds") { }

        class spell_highmaul_gushing_wounds_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_gushing_wounds_AuraScript);

            enum eSpell
            {
                GushingWoundsKill = 156153
            };

            void AfterApply(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Target = GetTarget())
                {
                    Map* l_Map = l_Target->GetMap();
                    Aura* l_Aura = p_AurEff->GetBase();

                    if (l_Map->IsLFR())
                    {
                        l_Aura->SetDuration(10 * TimeConstants::IN_MILLISECONDS);
                        l_Aura->SetMaxDuration(10 * TimeConstants::IN_MILLISECONDS);
                    }
                    else
                    {
                        l_Aura->SetDuration(15 * TimeConstants::IN_MILLISECONDS);
                        l_Aura->SetMaxDuration(15 * TimeConstants::IN_MILLISECONDS);
                    }

                    if (l_Aura->GetStackAmount() >= 5)
                        l_Target->CastSpell(l_Target, eSpell::GushingWoundsKill, true);
                }
            }

            void Register() override
            {
                AfterEffectApply += AuraEffectApplyFn(spell_highmaul_gushing_wounds_AuraScript::AfterApply, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_gushing_wounds_AuraScript();
        }
};

/// Pale Vitriol - 163042
class areatrigger_highmaul_pale_vitriol : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_pale_vitriol() : AreaTriggerEntityScript("areatrigger_highmaul_pale_vitriol") { }

        enum eSpell
        {
            PaleVitriol = 163046
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 4.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                    l_Caster->CastSpell(l_Unit, eSpell::PaleVitriol, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_pale_vitriol();
        }
};

void AddSC_boss_the_butcher()
{
    /// Boss
    new boss_the_butcher();

    /// NPCs
    new npc_highmaul_night_twisted_cadaver();
    new npc_highmaul_maggot();

    /// Spells
    new spell_highmaul_heavy_handed();
    new spell_highmaul_heavy_handed_proc();
    new spell_highmaul_bounding_cleave_dummy();
    new spell_highmaul_gushing_wounds();

    /// AreaTrigger
    new areatrigger_highmaul_pale_vitriol();
}