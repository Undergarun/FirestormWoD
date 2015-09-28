////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

Position const g_OreCrateSpawnPos[eFoundryDatas::MaxOreCrateSpawns] =
{
    { 16.221f, 3537.0501f, 280.97699f, 0.0f },
    { 31.512f, 3537.0601f, 280.97699f, 0.0f },
    { 1.4497f, 3548.2399f, 280.97699f, 0.0f },
    { -12.05f, 3567.3999f, 280.97699f, 0.0f },
    { -11.19f, 3585.7900f, 280.97699f, 0.0f },
    { -11.48f, 3599.4599f, 281.01300f, 0.0f },
    { 1.4514f, 3619.1299f, 281.01300f, 0.0f },
    { 17.313f, 3628.5400f, 281.01300f, 0.0f },
    { 37.201f, 3629.0701f, 281.01300f, 0.0f },
    { 52.556f, 3627.7299f, 281.01300f, 0.0f },
    { 69.613f, 3616.8799f, 280.97699f, 0.0f },
    { 80.066f, 3601.2900f, 280.97699f, 0.0f },
    { 80.977f, 3579.9099f, 280.97699f, 0.0f },
    { 79.835f, 3564.7900f, 280.97699f, 0.0f },
    { 69.385f, 3547.6599f, 280.97699f, 0.0f },
    { 53.776f, 3536.9299f, 280.97699f, 0.0f },
    { 16.865f, 3587.8701f, 281.01300f, 0.0f },
    { 16.045f, 3576.7500f, 280.97699f, 0.0f },
    { 52.951f, 3588.3701f, 280.97699f, 0.0f },
    { 51.832f, 3579.7500f, 280.97699f, 0.0f },
    { 42.146f, 3565.1799f, 280.97699f, 0.0f },
    { 32.075f, 3565.8799f, 280.97699f, 0.0f },
    { 38.521f, 3600.9699f, 280.97699f, 0.0f },
    { 29.146f, 3602.8601f, 280.97699f, 0.0f }
};

Position const g_MovePos[eFoundryDatas::MaxOregorgerMovePos] =
{
    { 29.788f, 3685.025f, 289.8583f, 0.0f },
    { 36.313f, 3677.135f, 290.9291f, 0.0f },
    { 40.641f, 3670.757f, 290.8677f, 0.0f },
    { 43.917f, 3664.390f, 290.9170f, 0.0f },
    { 45.461f, 3657.372f, 290.7451f, 0.0f },
    { 46.257f, 3652.046f, 290.9441f, 0.0f },
    { 46.144f, 3647.808f, 290.4916f, 0.0f },
    { 45.582f, 3642.901f, 290.1242f, 0.0f }
};

Position const g_JumpPos = { 52.8958f, 3617.03f, 280.894f, 4.673866f };

/// Oregorger <The Devourer> - 77182
class boss_oregorger : public CreatureScript
{
    public:
        boss_oregorger() : CreatureScript("boss_oregorger") { }

        enum eSpells
        {
            /// Misc
            OregorgerBonusLoot      = 177530,
            RollingFuryVisual       = 174183,
            /// Acid Maw
            AcidMawDoT              = 173471,
            /// Retched Blackrock
            RetchedBlackrockMissile = 156179,
            RetchedBlackrockDoT     = 156203,
            /// Explosive Shard
            ExplosiveShardMissile   = 156390,
            /// Acid Torrent
            AcidTorrentSearcher     = 156240,
            AcidTorrentTriggered    = 160465,
            AcidTorrentDmgAndDebuff = 156297,   ///< Main target and 20s debuff
            AcidTorrentAura         = 156300,   ///< 2s aura
            AcidTorrentMissile      = 156309,   ///< Triggers 156324, needs aura
            /// Blackrock Barrage
            BlackrockSpines         = 156834,
            BlackrockBarrageAoE     = 173461,
            BlackrockBarrageTrigger = 156878
        };

        enum eEvents
        {
            EventAcidMaw = 1,
            EventRetchedBlackrock,
            EventExplosiveShard,
            EventAcidTorrent,
            EventBlackrockSpines,
            EventBlackrockBarrage
        };

        enum eCosmeticEvents
        {
            EventCheckTrashs = 1
        };

        enum eActions
        {
        };

        enum eCreatures
        {
            PathIdentifier          = 77848,
            BlackrockOre            = 77261,
            OreCrate                = 77252,
            DarkshardCrystalback    = 78233,
            DarkshardGnasher        = 78978
        };

        struct boss_oregorgerAI : public BossAI
        {
            boss_oregorgerAI(Creature* p_Creature) : BossAI(p_Creature, eFoundryDatas::DataOregorger), m_Init(false), m_MoveIndex(0)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            EventMap m_CosmeticEvents;

            InstanceScript* m_Instance;

            bool m_Init;
            uint8 m_MoveIndex;

            uint64 m_AcidTorrentTarget;
            std::list<uint64> m_FrontPlayers;

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                me->RemoveAura(eFoundrySpells::Berserker);
                me->RemoveAura(eSpells::BlackrockSpines);

                me->RemoveAllAreasTrigger();

                _Reset();

                if (!m_Init)
                    m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCheckTrashs, 1 * TimeConstants::IN_MILLISECONDS);
                else
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                m_MoveIndex = 0;

                m_AcidTorrentTarget = 0;
                m_FrontPlayers.clear();
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                ///m_Events.ScheduleEvent(eEvents::EventAcidMaw, 3 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventRetchedBlackrock, 6 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventExplosiveShard, 9 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventAcidTorrent, 12 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventBlackrockSpines, 14 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                me->RemoveAllAreasTrigger();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RetchedBlackrockDoT);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::AcidTorrentDmgAndDebuff);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::AcidMawDoT);

                    CastSpellToPlayers(me->GetMap(), me, eSpells::OregorgerBonusLoot, true);

                    if (IsLFR())
                    {
                        Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                        if (l_Player && l_Player->GetGroup())
                            sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                    }
                }
            }

            void EnterEvadeMode() override
            {
                me->RemoveAllAuras();

                me->RemoveAllAreasTrigger();

                me->InterruptNonMeleeSpells(true);

                /// Just in case, to prevent the fail Return to Home
                me->ClearUnitState(UnitState::UNIT_STATE_ROOT);
                me->ClearUnitState(UnitState::UNIT_STATE_DISTRACTED);
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                summons.DespawnAll();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eFoundryDatas::DataOregorger, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RetchedBlackrockDoT);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::AcidTorrentDmgAndDebuff);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::AcidMawDoT);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE && p_Type != MovementGeneratorType::EFFECT_MOTION_TYPE)
                    return;

                switch (p_Type)
                {
                    case MovementGeneratorType::POINT_MOTION_TYPE:
                    {
                        if (m_MoveIndex >= eFoundryDatas::MaxOregorgerMovePos)
                        {
                            me->RemoveAura(eSpells::RollingFuryVisual);

                            AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                me->GetMotionMaster()->MoveJump(g_JumpPos, 20.0f, 20.0f);
                            });
                        }
                        else
                        {
                            AddTimedDelayedOperation(100, [this]() -> void
                            {
                                me->GetMotionMaster()->MovePoint(m_MoveIndex, g_MovePos[m_MoveIndex]);
                                ++m_MoveIndex;
                            });
                        }

                        break;
                    }
                    case MovementGeneratorType::EFFECT_MOTION_TYPE:
                    {
                        me->SetWalk(false);
                        me->SetSpeed(UnitMoveType::MOVE_WALK, 2.85714f);
                        me->SetHomePosition(*me);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    }
                    default:
                        break;
                }
            }

            void OnSpellCasted(SpellInfo const* p_SpellInfo) override
            {
                switch (p_SpellInfo->Id)
                {
                    case eSpells::AcidTorrentSearcher:
                    {
                        if (!m_AcidTorrentTarget)
                            break;

                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 100.0f);

                        if (l_PlayerList.empty())
                            break;

                        l_PlayerList.remove_if([this](Player* p_Player) -> bool
                        {
                            if (p_Player == nullptr || !p_Player->isInFront(me))
                                return true;

                            return false;
                        });

                        if (l_PlayerList.empty())
                            break;

                        if (Unit* l_Target = Unit::GetUnit(*me, m_AcidTorrentTarget))
                            me->CastSpell(l_Target, eSpells::AcidTorrentTriggered, true);

                        l_PlayerList.sort(JadeCore::ObjectDistanceOrderPred(me));

                        m_FrontPlayers.clear();

                        for (Player* l_Player : l_PlayerList)
                            m_FrontPlayers.push_back(l_Player->GetGUID());

                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            AttackStart(l_Target);

                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MoveChase(l_Target);
                        }

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
                    case eSpells::AcidTorrentTriggered:
                    {
                        bool l_First = true;
                        for (uint64 l_Guid : m_FrontPlayers)
                        {
                            if (l_First)
                            {
                                l_First = false;

                                if (Player* l_MainTarget = Player::GetPlayer(*me, l_Guid))
                                    me->CastSpell(l_MainTarget, eSpells::AcidTorrentDmgAndDebuff, true);

                                continue;
                            }

                            if (Player* l_Target = Player::GetPlayer(*me, l_Guid))
                                me->CastSpell(l_Target, eSpells::AcidTorrentAura, true);
                        }

                        me->CastSpell(me, eSpells::AcidTorrentMissile, true);
                        break;
                    }
                    case eSpells::BlackrockBarrageAoE:
                    {
                        if (p_Target == me)
                            break;

                        me->CastSpell(p_Target, eSpells::BlackrockBarrageTrigger, true);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvents.Update(p_Diff);

                switch (m_CosmeticEvents.ExecuteEvent())
                {
                    case eCosmeticEvents::EventCheckTrashs:
                    {
                        std::list<Creature*> l_CreatureList;
                        me->GetCreatureListWithEntryInGrid(l_CreatureList, eCreatures::DarkshardCrystalback, 150.0f);
                        me->GetCreatureListWithEntryInGrid(l_CreatureList, eCreatures::DarkshardGnasher, 150.0f);

                        if (l_CreatureList.empty())
                            break;

                        l_CreatureList.remove_if([this](Creature* p_Creature) -> bool
                        {
                            if (p_Creature == nullptr || !p_Creature->isAlive())
                                return true;

                            return false;
                        });

                        /// All trashes are dead, launch move
                        if (l_CreatureList.empty())
                        {
                            m_Init = true;

                            me->CastSpell(me, eSpells::RollingFuryVisual, true);

                            me->SetWalk(true);
                            me->SetSpeed(UnitMoveType::MOVE_WALK, 10.0f);

                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(m_MoveIndex, g_MovePos[m_MoveIndex]);

                            ++m_MoveIndex;
                        }

                        if (!m_Init)
                            m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCheckTrashs, 1 * TimeConstants::IN_MILLISECONDS);

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
                    case eEvents::EventAcidMaw:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::AcidMawDoT, true);

                        m_Events.ScheduleEvent(eEvents::EventAcidMaw, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventRetchedBlackrock:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, -10.0f))
                            me->CastSpell(l_Target, eSpells::RetchedBlackrockMissile, false);

                        m_Events.ScheduleEvent(eEvents::EventRetchedBlackrock, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventExplosiveShard:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2))
                            me->CastSpell(l_Target, eSpells::ExplosiveShardMissile, true);

                        m_Events.ScheduleEvent(eEvents::EventExplosiveShard, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventAcidTorrent:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, -10.0f))
                        {
                            float l_O = me->GetAngle(l_Target);

                            me->SetFacingTo(l_O);
                            me->SetReactState(ReactStates::REACT_PASSIVE);

                            m_AcidTorrentTarget = l_Target->GetGUID();
                        }

                        me->CastSpell(me, eSpells::AcidTorrentSearcher, false);
                        m_Events.ScheduleEvent(eEvents::EventAcidTorrent, 13 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventBlackrockSpines:
                    {
                        if (AuraPtr l_Aura = me->AddAura(eSpells::BlackrockSpines, me))
                            l_Aura->SetStackAmount(3);

                        m_Events.ScheduleEvent(eEvents::EventBlackrockSpines, 19 * TimeConstants::IN_MILLISECONDS);
                        m_Events.ScheduleEvent(eEvents::EventBlackrockBarrage, 100);
                        break;
                    }
                    case eEvents::EventBlackrockBarrage:
                    {
                        if (AuraPtr l_Aura = me->GetAura(eSpells::BlackrockSpines))
                            l_Aura->DropStack();

                        if (me->HasAura(eSpells::BlackrockSpines))
                            m_Events.ScheduleEvent(eEvents::EventBlackrockBarrage, 500);

                        me->CastSpell(me, eSpells::BlackrockBarrageAoE, false);
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
            return new boss_oregorgerAI(p_Creature);
        }
};

/// Retched Blackrock - 156186
class areatrigger_foundry_retched_blackrock : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_retched_blackrock() : AreaTriggerEntityScript("areatrigger_foundry_retched_blackrock") { }

        enum eSpell
        {
            RetchedBlackrockDoT = 156203
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 15.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Unit->GetDistance(p_AreaTrigger) <= 7.0f)
                    {
                        if (!l_Unit->HasAura(eSpell::RetchedBlackrockDoT, l_Caster->GetGUID()))
                            l_Unit->CastSpell(l_Unit, eSpell::RetchedBlackrockDoT, true, nullptr, NULLAURA_EFFECT, l_Caster->GetGUID());
                    }
                    else if (l_Unit->HasAura(eSpell::RetchedBlackrockDoT, l_Caster->GetGUID()))
                        l_Unit->RemoveAura(eSpell::RetchedBlackrockDoT, l_Caster->GetGUID());
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_retched_blackrock();
        }
};

/// Explosive Shard - 156388
class areatrigger_foundry_explosive_shard : public AreaTriggerEntityScript
{
    public:
        areatrigger_foundry_explosive_shard() : AreaTriggerEntityScript("areatrigger_foundry_explosive_shard") { }

        enum eSpell
        {
            ExplosiveShardAoE = 156374
        };

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
                l_Caster->CastSpell(*p_AreaTrigger, eSpell::ExplosiveShardAoE, true);
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_foundry_explosive_shard();
        }
};

void AddSC_boss_oregorger()
{
    /// Boss
    new boss_oregorger();

    /// NPCs

    /// Spells

    /// AreaTriggers
    new areatrigger_foundry_retched_blackrock();
    new areatrigger_foundry_explosive_shard();
}