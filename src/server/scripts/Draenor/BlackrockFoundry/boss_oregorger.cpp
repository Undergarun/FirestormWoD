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
            RollingBox              = 160665,
            /// Phase 1
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
            BlackrockBarrageTrigger = 156878,
            /// Phase 2
            RollingFuryAura         = 155898,
            HungerDrivePeriodic     = 165127
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
            OreCrateCosmetic        = 79504,
            OreCrate                = 77252,
            DarkshardCrystalback    = 78233,
            DarkshardGnasher        = 78978
        };

        enum eData
        {
            DataMitigationPct
        };

        enum eTalks
        {
            BlackrockBarrage,
            Phase2,
            ReturnToPhase1
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

            float m_MitigationPct;

            std::set<uint8> m_CratesToActivate;
            std::set<uint8> m_ActivatedCrates;
            uint8 m_ActivatedCratesCount;

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

                m_MitigationPct = 0.0f;

                m_CratesToActivate.clear();
                m_ActivatedCrates.clear();
                m_ActivatedCratesCount = 0;
            }

            bool CanRespawn() override
            {
                return false;
            }

            void SetFData(uint32 p_ID, float p_Value) override
            {
                switch (p_ID)
                {
                    case eData::DataMitigationPct:
                    {
                        m_MitigationPct = p_Value;
                        break;
                    }
                    default:
                        break;
                }
            }

            float GetFData(uint32 p_ID /*= 0*/) const override
            {
                switch (p_ID)
                {
                    case eData::DataMitigationPct:
                        return m_MitigationPct;
                    default:
                        break;
                }

                return 0.0f;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                HandleFirstPhase();
            }

            void SetPower(Powers p_Power, int32 p_Value) override
            {
                if (p_Power != Powers::POWER_MANA)
                    return;

                if (!me->isInCombat())
                    return;

                /// Phase One lasts until Oregorger runs out of Mana, at which point he enters Phase Two.
                if (p_Value == 0)
                {
                    m_Events.Reset();

                    Talk(eTalks::Phase2);

                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->GetMotionMaster()->Clear();
                        me->GetMotionMaster()->MoveTargetedHome();
                    });

                    m_CratesToActivate.clear();

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        std::list<Creature*> l_CrateList;
                        me->GetCreatureListWithEntryInGrid(l_CrateList, eCreatures::OreCrateCosmetic, 150.0f);

                        if (l_CrateList.empty())
                            return;

                        std::vector<uint8> l_OreCrateSpawns;

                        for (uint8 l_I = 0; l_I < eFoundryDatas::MaxOreCrateSpawns; ++l_I)
                            l_OreCrateSpawns.push_back(l_I);

                        std::random_shuffle(l_OreCrateSpawns.begin(), l_OreCrateSpawns.end());

                        /// In LFR difficulty, 16 Ore Crates will spawn, instead of 12, in the first  Rolling Fury phase. The uneaten Ore will remain for future phases.
                        uint8 l_Count = IsLFR() ? 16 : 12;

                        if ((l_Count + m_ActivatedCratesCount) > (int)eFoundryDatas::MaxOreCrateSpawns)
                            l_Count = (int)eFoundryDatas::MaxOreCrateSpawns - m_ActivatedCratesCount;

                        for (uint8 l_I : l_OreCrateSpawns)
                        {
                            if (m_ActivatedCrates.find(l_I) != m_ActivatedCrates.end())
                                continue;

                            if (l_Count <= 0)
                                break;

                            l_CrateList.sort(JadeCore::PositionDistanceOrderPred(g_OreCrateSpawnPos[l_I]));

                            if (Creature* l_Crate = (*l_CrateList.begin()))
                                l_Crate->CastSpell(g_OreCrateSpawnPos[l_I], eSpells::RollingBox, true);

                            --l_Count;
                            m_ActivatedCrates.insert(l_I);
                            m_CratesToActivate.insert(l_I);
                            ++m_ActivatedCratesCount;
                        }
                    });

                    AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        for (uint8 l_I : m_CratesToActivate)
                        {
                            if (Creature* l_Crate = me->SummonCreature(eCreatures::OreCrate, g_OreCrateSpawnPos[l_I]))
                            {
                                l_Crate->SetReactState(ReactStates::REACT_PASSIVE);
                                l_Crate->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                                l_Crate->AddUnitState(UnitState::UNIT_STATE_STUNNED);
                                l_Crate->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                                l_Crate->ApplySpellImmune(0, SpellImmunity::IMMUNITY_EFFECT, SpellEffects::SPELL_EFFECT_KNOCK_BACK, true);
                            }
                        }
                    });

                    AddTimedDelayedOperation(7 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->SetFacingTo(g_NorthOrientation);
                    });

                    AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->CastSpell(me, eSpells::RollingFuryAura, true);
                        me->CastSpell(me, eSpells::HungerDrivePeriodic, true);
                    });
                }
                /// When Oregorger reaches full Mana, Phase One restarts.
                else if (p_Value == 100)
                {
                    HandleFirstPhase();

                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                    {
                        me->GetMotionMaster()->Clear();

                        AttackStart(l_Target);
                    }
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                /// Oregorger only regens by script
                p_Value = 0;
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
                    case eSpells::ExplosiveShardMissile:
                    case eSpells::BlackrockBarrageAoE:
                    {
                        me->EnergizeBySpell(me, eSpells::RetchedBlackrockMissile, -5, Powers::POWER_MANA);
                        break;
                    }
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

                        l_PlayerList.sort(JadeCore::ObjectDistanceOrderPred(me));

                        m_FrontPlayers.clear();

                        for (Player* l_Player : l_PlayerList)
                            m_FrontPlayers.push_back(l_Player->GetGUID());

                        if (Unit* l_Target = Unit::GetUnit(*me, m_AcidTorrentTarget))
                            me->CastSpell(l_Target, eSpells::AcidTorrentTriggered, true);

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

                                /// Sprays a cone of acid at a random ranged foe, inflicting 475000 to 525000 Physical damage to the closest player
                                /// and increasing that player's damage taken from Acid Torrent by 475000 to 525000 for 20 sec.
                                if (Player* l_MainTarget = Player::GetPlayer(*me, l_Guid))
                                    me->CastSpell(l_MainTarget, eSpells::AcidTorrentDmgAndDebuff, true);

                                continue;
                            }

                            if (Player* l_Target = Player::GetPlayer(*me, l_Guid))
                                me->CastSpell(l_Target, eSpells::AcidTorrentAura, true);
                        }

                        /// All other targets take up to 300000 Nature damage, reduced by the closest player's total damage mitigation.
                        me->CastSpell(me, eSpells::AcidTorrentAura, true);
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
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -10.0f))
                            me->CastSpell(l_Target, eSpells::RetchedBlackrockMissile, false);
                        else if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::RetchedBlackrockMissile, false);

                        m_Events.ScheduleEvent(eEvents::EventRetchedBlackrock, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventExplosiveShard:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 5.0f))
                            me->CastSpell(l_Target, eSpells::ExplosiveShardMissile, true);
                        else if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::ExplosiveShardMissile, true);

                        m_Events.ScheduleEvent(eEvents::EventExplosiveShard, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventAcidTorrent:
                    {
                        Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, -10.0f);
                        if (l_Target == nullptr)
                            l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM);

                        if (l_Target != nullptr)
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
                            l_Aura->SetStackAmount(IsMythic() ? 5 : 3);

                        Talk(eTalks::BlackrockBarrage);
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

            void HandleFirstPhase()
            {
                m_Events.ScheduleEvent(eEvents::EventAcidMaw, 3 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRetchedBlackrock, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventExplosiveShard, 9 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventAcidTorrent, 12 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBlackrockSpines, 14 * TimeConstants::IN_MILLISECONDS);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_oregorgerAI(p_Creature);
        }
};

/// Crate Ore - 77252
class npc_foundry_crate_ore : public CreatureScript
{
    public:
        npc_foundry_crate_ore() : CreatureScript("npc_foundry_crate_ore") { }

        struct npc_foundry_crate_oreAI : public ScriptedAI
        {
            npc_foundry_crate_oreAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eCreature
            {
                BlackrockOre = 77261
            };

            enum eVisual
            {
                BlackrockOreVisual = 37247
            };

            void JustDied(Unit* p_Killer)
            {
                me->DespawnOrUnsummon();

                if (InstanceScript* l_Instance = me->GetInstanceScript())
                {
                    if (Creature* l_Oregorger = Creature::GetCreature(*me, l_Instance->GetData64(eFoundryCreatures::BossOregorger)))
                    {
                        if (Creature* l_Ore = l_Oregorger->SummonCreature(eCreature::BlackrockOre, *me))
                        {
                            l_Ore->SetReactState(ReactStates::REACT_PASSIVE);
                            l_Ore->AddUnitState(UnitState::UNIT_STATE_STUNNED);
                            l_Ore->SetFlag(EUnitFields::UNIT_FIELD_FLAGS2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                            l_Ore->SendPlaySpellVisual(eVisual::BlackrockOreVisual, l_Ore, 0.0f, 0.0f, Position());
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_foundry_crate_oreAI(p_Creature);
        }
};

/// Acid Torrent - 156297
class spell_foundry_acid_torrent : public SpellScriptLoader
{
    public:
        spell_foundry_acid_torrent() : SpellScriptLoader("spell_foundry_acid_torrent") { }

        class spell_foundry_acid_torrent_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_acid_torrent_SpellScript);

            enum eData
            {
                DamageMitigationPct
            };

            uint32 m_Damage;

            bool Load() override
            {
                m_Damage = 0;
                return true;
            }

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                m_Damage = GetHitDamage();
            }

            void HandleAfterHit()
            {
                uint32 l_Damage = GetHitDamage();

                if (m_Damage == l_Damage)
                    return;
                else if (m_Damage > l_Damage && m_Damage > 0)
                {
                    float l_Pct = (float)l_Damage / (float)m_Damage;

                    if (Creature* l_Oregorger = GetCaster()->ToCreature())
                    {
                        if (!l_Oregorger->IsAIEnabled)
                            return;

                        l_Oregorger->AI()->SetFData(eData::DamageMitigationPct, l_Pct);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_foundry_acid_torrent_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
                AfterHit += SpellHitFn(spell_foundry_acid_torrent_SpellScript::HandleAfterHit);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_acid_torrent_SpellScript();
        }
};

/// Acid Torrent (AoE) - 156324
class spell_foundry_acid_torrent_aoe : public SpellScriptLoader
{
    public:
        spell_foundry_acid_torrent_aoe() : SpellScriptLoader("spell_foundry_acid_torrent_aoe") { }

        class spell_foundry_acid_torrent_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_acid_torrent_aoe_SpellScript);

            enum eData
            {
                DamageMitigationPct
            };

            void HandleDamage(SpellEffIndex p_EffIndex)
            {
                if (Creature* l_Oregorger = GetCaster()->ToCreature())
                {
                    if (!l_Oregorger->IsAIEnabled)
                        return;

                    float l_Pct = l_Oregorger->AI()->GetFData(eData::DamageMitigationPct);
                    int32 l_Dmg = GetHitDamage();

                    l_Dmg *= l_Pct;
                    SetHitDamage(l_Dmg);
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_foundry_acid_torrent_aoe_SpellScript::HandleDamage, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_acid_torrent_aoe_SpellScript();
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
                    if (l_Unit->GetDistance(p_AreaTrigger) <= 6.5f)
                    {
                        if (!l_Unit->HasAura(eSpell::RetchedBlackrockDoT))
                            l_Unit->CastSpell(l_Unit, eSpell::RetchedBlackrockDoT, true);
                    }
                    else if (!l_Unit->FindNearestAreaTrigger(p_AreaTrigger->GetSpellId(), 6.5f))
                    {
                        if (l_Unit->HasAura(eSpell::RetchedBlackrockDoT))
                            l_Unit->RemoveAura(eSpell::RetchedBlackrockDoT);
                    }
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 6.5;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                {
                    if (!l_Unit->FindNearestAreaTrigger(p_AreaTrigger->GetSpellId(), l_Radius))
                    {
                        if (l_Unit->HasAura(eSpell::RetchedBlackrockDoT))
                            l_Unit->RemoveAura(eSpell::RetchedBlackrockDoT);
                    }
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
    new npc_foundry_crate_ore();

    /// Spells
    new spell_foundry_acid_torrent();
    new spell_foundry_acid_torrent_aoe();

    /// AreaTriggers
    new areatrigger_foundry_retched_blackrock();
    new areatrigger_foundry_explosive_shard();
}