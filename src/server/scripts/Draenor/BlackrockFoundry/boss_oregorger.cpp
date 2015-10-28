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

struct PointData
{
    uint8 I;
    uint8 J;
};

PointData const g_CollisionPoints[eFoundryDatas::MaxOregorgerCollisions] =
{
    { 4, 5 },   ///< 1: Linked to 3 and 7
    { 4, 4 },   ///< 2: Linked to 3 and 7
    { 4, 2 },   ///< 3: Linked to 6 and 1
    { 5, 2 },   ///< 4: Linked to 6 and 1
    { 2, 4 },   ///< 5: Linked to 2 and 8
    { 2, 2 },   ///< 6: Linked to 5 and 4
    { 1, 4 },   ///< 7: Linked to 2 and 8
    { 2, 1 }    ///< 8: Linked to 5 and 4
};

std::pair<uint8, uint8> const g_LinkedCollisionPoints[eFoundryDatas::MaxOregorgerCollisions] =
{
    { 3, 7 },
    { 3, 7 },
    { 6, 1 },
    { 6, 1 },
    { 2, 8 },
    { 5, 4 },
    { 2, 8 },
    { 5, 4 }
};

/// Some positions in Oregorger's room aren't accessible by players or creatures, just bypass them
bool const g_BypassPoints[eFoundryDatas::MaxOregorgerPatterns][eFoundryDatas::MaxOregorgerPatterns] =
{
    { true,  false, false, false, false, true,  true  },
    { false, false, true,  true,  false, false, false },
    { false, false, false, false, false, true,  false },
    { false, true,  false, true,  false, true,  false },
    { false, true,  false, false, false, false, false },
    { false, false, false, true,  true,  false, true  },
    { true,  false, false, false, false, false, true  }
};

float const g_OregorgerPatternsX[eFoundryDatas::MaxOregorgerPatterns] =
{
    -6.71f,
    10.28f,
    22.23f,
    44.96f,
    60.04f,
    76.24f,
    86.94f
};

float const g_OregorgerPatternsY[eFoundryDatas::MaxOregorgerPatterns] =
{
    3542.52f,
    3558.64f,
    3573.06f,
    3596.53f,
    3609.33f,
    3624.89f,
    3634.95f
};

Position const g_VolatileOreSpawn = { 34.08f, 3636.04f, 281.19f, 4.83f };

/// Oregorger <The Devourer> - 77182
class boss_oregorger : public CreatureScript
{
    public:
        boss_oregorger() : CreatureScript("boss_oregorger") { }

        enum eSpells
        {
            /// Misc
            OregorgerBonusLoot          = 177530,
            RollingFuryVisual           = 174183,
            RollingBox                  = 160665,
            WallshakingRoar             = 160662,
            /// Phase 1
            /// Acid Maw
            AcidMawDoT                  = 173471,
            /// Retched Blackrock
            RetchedBlackrockMissile     = 156179,
            RetchedBlackrockDoT         = 156203,
            /// Explosive Shard
            ExplosiveShardMissile       = 156390,
            ExplosiveShardAoE           = 156374,
            /// Acid Torrent
            AcidTorrentSearcher         = 156240,
            AcidTorrentTriggered        = 160465,
            AcidTorrentDmgAndDebuff     = 156297,   ///< Main target and 20s debuff
            AcidTorrentAura             = 156300,   ///< 2s aura
            AcidTorrentMissile          = 156309,   ///< Triggers 156324, needs aura
            /// Blackrock Barrage
            BlackrockSpines             = 156834,
            BlackrockBarrageAoE         = 173461,
            BlackrockBarrageTrigger     = 156878,
            /// Phase 2
            RollingFuryAura             = 155898,
            HungerDrivePeriodic         = 165127,
            EarthshakingCollision       = 155897,
            ConsumeBlackrockOre         = 155862,
            /// Spells for Mythic mode
            ExpelUnstableSlagSummon     = 155908,
            UnstableSlagExplosionAT     = 155910,
            ExpelUnstableSlagVisual     = 155916,
            UnstableSlagExplosionDmg    = 155923
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
            EventCheckTrashs = 1,
            EventCollectOre
        };

        enum eCreatures
        {
            PathIdentifier          = 77848,
            OreCrateCosmetic        = 79504,
            OreCrate                = 77252,
            DarkshardCrystalback    = 78233,
            DarkshardGnasher        = 78978,
            BlackrockOre            = 77261,
            UnstableSlag            = 77299
        };

        enum eGameObject
        {
            VolatileBlackrockOre = 237308
        };

        enum eDatas
        {
            DataMitigationPct,
            MovementFinished = 666
        };

        enum eTalks
        {
            BlackrockBarrage,
            Phase2,
            ReturnToPhase1
        };

        enum ePhases
        {
            PhaseFight,
            PhaseRolling
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

            bool m_VolatileOre;
            uint64 m_VolatileOreGuid;

            uint8 m_Phase;

            uint64 m_AcidTorrentTarget;
            std::list<uint64> m_FrontPlayers;

            float m_MitigationPct;

            std::set<uint64> m_Crates;
            std::set<uint8> m_CratesToActivate;
            std::set<uint8> m_ActivatedCrates;
            uint8 m_ActivatedCratesCount;

            uint8 m_PointID;
            Position m_Destination;
            Position m_NextPos;

            /// Counter for Path Identifier reached
            uint8 m_PathCount;

            std::set<uint64> m_BlackrockOres;

            void Reset() override
            {
                ClearDelayedOperations();

                m_Events.Reset();
                m_CosmeticEvents.Reset();

                summons.DespawnAll();

                me->RemoveAura(eFoundrySpells::Berserker);
                me->RemoveAura(eSpells::BlackrockSpines);

                me->SetPower(Powers::POWER_MANA, 100);

                me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                me->RemoveAllAreasTrigger();

                _Reset();

                if (!m_Init)
                    m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCheckTrashs, 1 * TimeConstants::IN_MILLISECONDS);
                else
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);

                std::list<Creature*> l_CrateList;
                me->GetCreatureListWithEntryInGrid(l_CrateList, eCreatures::OreCrateCosmetic, 150.0f);

                for (Creature* l_Crate : l_CrateList)
                {
                    m_Crates.insert(l_Crate->GetGUID());
                    l_Crate->Respawn();
                }

                l_CrateList.clear();

                me->GetCreatureListWithEntryInGrid(l_CrateList, eCreatures::OreCrate, 150.0f);

                for (Creature* l_Crate : l_CrateList)
                    l_Crate->DespawnOrUnsummon();

                m_MoveIndex = 0;

                if (GameObject* l_Ore = GameObject::GetGameObject(*me, m_VolatileOreGuid))
                    l_Ore->Delete();

                m_VolatileOre = false;
                m_VolatileOreGuid = 0;

                m_Phase = ePhases::PhaseFight;

                m_AcidTorrentTarget = 0;
                m_FrontPlayers.clear();

                m_MitigationPct = 0.0f;

                m_CratesToActivate.clear();
                m_ActivatedCrates.clear();
                m_ActivatedCratesCount = 0;

                m_PointID = 255;
                m_Destination = Position();
                m_NextPos = Position();

                m_PathCount = 0;

                m_BlackrockOres.clear();
            }

            void JustSummoned(Creature* p_Summon) override
            {
                if (p_Summon->GetEntry() == eCreatures::BlackrockOre)
                    m_BlackrockOres.insert(p_Summon->GetGUID());
                else if (p_Summon->GetEntry() == eCreatures::UnstableSlag)
                    p_Summon->CastSpell(p_Summon, eSpells::ExpelUnstableSlagVisual, true);

                BossAI::JustSummoned(p_Summon);
            }

            void SummonedCreatureDespawn(Creature* p_Summon) override
            {
                if (p_Summon->GetEntry() == eCreatures::BlackrockOre)
                    m_BlackrockOres.erase(p_Summon->GetGUID());

                BossAI::SummonedCreatureDespawn(p_Summon);
            }

            bool CanRespawn() override
            {
                return false;
            }

            void SetFData(uint32 p_ID, float p_Value) override
            {
                switch (p_ID)
                {
                    case eDatas::DataMitigationPct:
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
                    case eDatas::DataMitigationPct:
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
                if (p_Value == 0 && m_Phase == ePhases::PhaseFight)
                {
                    m_Events.Reset();

                    Talk(eTalks::Phase2);

                    m_Phase = ePhases::PhaseRolling;

                    if (!m_VolatileOre)
                    {
                        m_VolatileOre = true;

                        if (GameObject* l_Ore = me->SummonGameObject(eGameObject::VolatileBlackrockOre, g_VolatileOreSpawn, 0.0f, 0.0f, 0.0f, 0.0f, 0))
                            m_VolatileOreGuid = l_Ore->GetGUID();
                    }

                    m_PathCount = 0;

                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->AttackStop();

                        std::pair<G3D::Vector2, uint8> l_PointData = GetNearestIntersectionPoint(me, 0.0f);
                        if (l_PointData.first.x != 0.0f && l_PointData.first.y != 0.0f)
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, G3D::Vector3(l_PointData.first, me->GetPositionZ()));

                            m_PointID = l_PointData.second;
                        }
                    });

                    m_CratesToActivate.clear();

                    AddTimedDelayedOperation(3 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        if (m_Crates.empty())
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

                            Creature* l_NearestCrate = nullptr;
                            float l_Dist = 100000.0f;
                            for (uint64 l_Guid : m_Crates)
                            {
                                if (Creature* l_Crate = Creature::GetCreature(*me, l_Guid))
                                {
                                    if (l_Crate->GetDistance(g_OreCrateSpawnPos[l_I]) < l_Dist)
                                    {
                                        l_NearestCrate = l_Crate;
                                        l_Dist = me->GetDistance(l_Crate);
                                    }
                                }
                            }

                            m_Crates.erase(l_NearestCrate->GetGUID());

                            l_NearestCrate->CastSpell(g_OreCrateSpawnPos[l_I], eSpells::RollingBox, true);
                            l_NearestCrate->DespawnOrUnsummon(1 * TimeConstants::IN_MILLISECONDS);

                            --l_Count;
                            m_ActivatedCrates.insert(l_I);
                            m_CratesToActivate.insert(l_I);
                            ++m_ActivatedCratesCount;
                        }

                        me->CastSpell(me, eSpells::WallshakingRoar, true);
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
                        SelectPath();
                    });

                    AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        me->CastSpell(me, eSpells::RollingFuryAura, true);
                        me->CastSpell(me, eSpells::HungerDrivePeriodic, true);

                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCollectOre, 200);

                        if (m_NextPos.m_positionX == 0.0f)
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, m_Destination);
                        }
                        else
                        {
                            me->GetMotionMaster()->Clear();
                            me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, m_NextPos);
                        }
                    });
                }
                /// When Oregorger reaches full Mana, Phase One restarts.
                else if (p_Value == 100)
                {
                    Talk(eTalks::ReturnToPhase1);

                    m_Phase = ePhases::PhaseFight;

                    me->RemoveAura(eSpells::HungerDrivePeriodic);
                    me->RemoveAura(eSpells::RollingFuryAura);

                    m_CosmeticEvents.CancelEvent(eCosmeticEvents::EventCollectOre);

                    AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        HandleFirstPhase();

                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                        {
                            me->GetMotionMaster()->Clear();

                            AttackStart(l_Target);
                        }
                    });
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
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ExplosiveShardAoE);

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
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::ExplosiveShardAoE);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_Type != MovementGeneratorType::POINT_MOTION_TYPE && p_Type != MovementGeneratorType::EFFECT_MOTION_TYPE)
                    return;

                /// Handle phase 2 movements in a specific way
                if (m_Phase == ePhases::PhaseRolling)
                {
                    if (p_ID == eDatas::MovementFinished && me->HasAura(eSpells::RollingFuryAura))
                    {
                        /// Movement not finished, handle it in a different way
                        if (me->GetDistance(m_Destination) > 0.5f)
                        {
                            if (!IsCenterPos(me->m_positionX, me->m_positionY))
                            {
                                switch (m_PathCount)
                                {
                                    case 1:
                                    case 4:
                                    {
                                        me->SetFacingTo(me->GetOrientation() + M_PI / 2.0f);
                                        break;
                                    }
                                    case 5:
                                    {
                                        AddTimedDelayedOperation(50, [this]() -> void
                                        {
                                            m_PathCount = 0;

                                            me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, m_Destination);
                                            return;
                                        });

                                        break;
                                    }
                                    case 2:
                                    case 3:
                                    default:
                                        break;
                                }
                            }

                            AddTimedDelayedOperation(125, [this]() -> void
                            {
                                SelectNextPathIdentifier();
                            });

                            return;
                        }

                        m_PathCount = 0;

                        me->RemoveAura(eSpells::RollingFuryAura);

                        me->CastSpell(me, eSpells::EarthshakingCollision, true);

                        /// In Mythic difficulty, these collisions also leave Unstable Slag residue at the location of the collision.
                        /// Unstable Slag explodes if another collision occurs at the same location.
                        if (IsMythic())
                        {
                            if (Creature* l_Slag = me->FindNearestCreature(eCreatures::UnstableSlag, 2.0f))
                            {
                                l_Slag->CastSpell(l_Slag, eSpells::UnstableSlagExplosionAT, true);

                                //////////////////////////////////////////////////////////////////////////
                                /// MISSING AREATRIGGER TEMPLATE DATAS FOR MYTHIC
                                //////////////////////////////////////////////////////////////////////////
                            }
                            else
                                me->CastSpell(me, eSpells::ExpelUnstableSlagSummon, true);
                        }

                        AddTimedDelayedOperation(2 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            SelectPath();
                        });

                        AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            me->CastSpell(me, eSpells::RollingFuryAura, true);

                            if (m_NextPos.m_positionX != 0.0f)
                                me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, m_NextPos);
                            else
                                me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, m_Destination);
                        });
                    }

                    return;
                }

                if (!me->isInCombat())
                {
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

                            std::list<Creature*> l_CrateList;
                            me->GetCreatureListWithEntryInGrid(l_CrateList, eCreatures::OreCrateCosmetic, 150.0f);

                            m_Crates.clear();

                            for (Creature* l_Crate : l_CrateList)
                                m_Crates.insert(l_Crate->GetGUID());

                            break;
                        }
                        default:
                            break;
                    }
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
                    case eCosmeticEvents::EventCollectOre:
                    {
                        if (Creature* l_Ore = me->FindNearestCreature(eCreatures::BlackrockOre, 2.0f))
                        {
                            l_Ore->DespawnOrUnsummon();
                            me->CastSpell(me, eSpells::ConsumeBlackrockOre, true);
                        }

                        if (!me->HasAura(eSpells::HungerDrivePeriodic))
                            break;

                        m_CosmeticEvents.ScheduleEvent(eCosmeticEvents::EventCollectOre, 200);
                        break;
                    }
                    default:
                        break;
                }

                if (!UpdateVictim() || me->HasAura(eSpells::HungerDrivePeriodic))
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
                        AddTimedDelayedOperation(2050, [this]() -> void
                        {
                            if (AuraPtr l_Aura = me->GetAura(eSpells::BlackrockSpines))
                                l_Aura->DropStack();

                            if (me->HasAura(eSpells::BlackrockSpines))
                                m_Events.ScheduleEvent(eEvents::EventBlackrockBarrage, 50);
                        });

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

                if (me->HasAura(eSpells::BlackrockSpines))
                    m_Events.ScheduleEvent(eEvents::EventBlackrockBarrage, 100);
            }

            uint8 GetCollisionOrIntersectionPoint(uint8 p_I, uint8 p_J) const
            {
                for (uint8 l_I = 0; l_I < eFoundryDatas::MaxOregorgerCollisions; ++l_I)
                {
                    if (g_CollisionPoints[l_I].I == p_I && g_CollisionPoints[l_I].J == p_J)
                        return l_I;
                }

                return 255;
            }

            std::pair<G3D::Vector2, uint8> GetNearestIntersectionPoint(Creature* p_Source, float p_MinDist = 1.0f) const
            {
                float l_Dist = 100000.0f;

                G3D::Vector2 l_Point = G3D::Vector2(0.0f, 0.0f);
                uint8 l_PointID = 0;

                Position l_Pos;
                l_Pos.m_positionZ = p_Source->GetPositionZ();

                /// Check position X for each part of X axis
                for (uint8 l_I = 0; l_I < eFoundryDatas::MaxOregorgerPatterns; ++l_I)
                {
                    float l_X = g_OregorgerPatternsX[l_I];

                    /// When X part is found, search for the Y part
                    for (uint8 l_J = 0; l_J < eFoundryDatas::MaxOregorgerPatterns; ++l_J)
                    {
                        uint8 l_CollisionID = GetCollisionOrIntersectionPoint(l_I, l_J);

                        /// Some points must be bypassed, and some others aren't collision or intersection (it's the same here) points
                        if (g_BypassPoints[l_I][l_J] || l_CollisionID == 255)
                            continue;

                        float l_Y = g_OregorgerPatternsY[l_J];

                        /// Y part is found too, then we can calculate the middle position of the square
                        float l_CenterX, l_CenterY;

                        l_CenterX = (l_X + (l_I > 0 ? g_OregorgerPatternsX[l_I - 1] : 0.0f)) / 2.0f;
                        l_CenterY = (l_Y + (l_J > 0 ? g_OregorgerPatternsY[l_J - 1] : 0.0f)) / 2.0f;

                        l_Pos.m_positionX = l_CenterX;
                        l_Pos.m_positionY = l_CenterY;

                        if (p_Source->GetDistance(l_Pos) >= p_MinDist && p_Source->GetDistance(l_Pos) < l_Dist)
                        {
                            l_Dist = p_Source->GetDistance(l_Pos);
                            l_Point = G3D::Vector2(l_CenterX, l_CenterY);
                            l_PointID = l_CollisionID;
                        }
                    }
                }

                return std::make_pair(l_Point, l_PointID);
            }

            std::pair<G3D::Vector2, uint8> GetRandomCollisionPoint() const
            {
                if (m_PointID == 255)
                    return std::make_pair(G3D::Vector2(), m_PointID);

                std::pair<uint8, uint8> l_Choices = g_LinkedCollisionPoints[m_PointID];
                uint8 l_ID = 0;

                /////////////////////////////////////////////////////////////////////
                /// Try to choose a point with Blackrock ores to eat
                if (!m_BlackrockOres.empty())
                {
                    float l_MinDistA = 100000.0f;
                    float l_MinDistB = 100000.0f;

                    Position l_PosA, l_PosB;
                    PointData l_PointA = g_CollisionPoints[l_Choices.first - 1];
                    PointData l_PointB = g_CollisionPoints[l_Choices.second - 1];

                    uint8 l_CenterCount = 0;
                    uint8 l_OutterCount = 0;

                    l_PosA.m_positionX = (g_OregorgerPatternsX[l_PointA.I] + g_OregorgerPatternsX[l_PointA.I - 1]) / 2.0f;
                    l_PosA.m_positionY = (g_OregorgerPatternsY[l_PointA.J] + g_OregorgerPatternsY[l_PointA.J - 1]) / 2.0f;
                    l_PosA.m_positionZ = me->GetPositionZ();

                    l_PosB.m_positionX = (g_OregorgerPatternsX[l_PointB.I] + g_OregorgerPatternsX[l_PointB.I - 1]) / 2.0f;
                    l_PosB.m_positionY = (g_OregorgerPatternsY[l_PointB.J] + g_OregorgerPatternsY[l_PointB.J - 1]) / 2.0f;
                    l_PosB.m_positionZ = me->GetPositionZ();

                    for (uint64 l_Guid : m_BlackrockOres)
                    {
                        if (Creature* l_Ore = Creature::GetCreature(*me, l_Guid))
                        {
                            /// Count the ores
                            if (IsCenterPos(l_Ore->m_positionX, l_Ore->m_positionY))
                                l_CenterCount++;
                            else
                                l_OutterCount++;

                            if (l_Ore->GetDistance(l_PosA) < l_MinDistA)
                                l_MinDistA = l_Ore->GetDistance(l_PosA);

                            if (l_Ore->GetDistance(l_PosB) < l_MinDistB)
                                l_MinDistB = l_Ore->GetDistance(l_PosB);
                        }
                    }

                    /// If more ores at center than at outter of the room, move to center
                    if (l_CenterCount > l_OutterCount)
                    {
                        if (IsCenterPos(l_PosA.m_positionX, l_PosA.m_positionY))
                            l_ID = l_Choices.first;
                        else
                            l_ID = l_Choices.second;
                    }
                    /// If more ores at outter than at center of the room, move to outter
                    else if (l_OutterCount > l_CenterCount)
                    {
                        if (!IsCenterPos(l_PosA.m_positionX, l_PosA.m_positionY))
                            l_ID = l_Choices.first;
                        else
                            l_ID = l_Choices.second;
                    }
                    /// If it's equal, move to the nearest ore
                    else
                    {
                        if (l_MinDistA < l_MinDistB)
                            l_ID = l_Choices.first;
                        else
                            l_ID = l_Choices.second;
                    }
                }
                /// If no ore to eat, choose randomly
                else
                {
                    if (urand(0, 1))
                        l_ID = l_Choices.first;
                    else
                        l_ID = l_Choices.second;
                }
                /////////////////////////////////////////////////////////////////////

                --l_ID;

                PointData l_Point = g_CollisionPoints[l_ID];

                float l_CenterX, l_CenterY;

                l_CenterX = (g_OregorgerPatternsX[l_Point.I] + g_OregorgerPatternsX[l_Point.I - 1]) / 2.0f;
                l_CenterY = (g_OregorgerPatternsY[l_Point.J] + g_OregorgerPatternsY[l_Point.J - 1]) / 2.0f;

                return std::make_pair(G3D::Vector2(l_CenterX, l_CenterY), l_ID);
            }

            bool IsCenterPos(float p_X, float p_Y) const
            {
                /// We need to know if a specific point is in the center of the room, or not
                /// The rolling direction will change depending on that
                /// If it's the room's center, Oregorger will rolls clockwise
                for (uint8 l_I = 0; l_I < eFoundryDatas::MaxOregorgerPatterns; ++l_I)
                {
                    if (p_X > g_OregorgerPatternsX[l_I])
                        continue;

                    for (uint8 l_J = 0; l_J < eFoundryDatas::MaxOregorgerPatterns; ++l_J)
                    {
                        if (p_Y > g_OregorgerPatternsY[l_J])
                            continue;

                        if (l_I >= 2 && l_I <= 4 && l_J >= 2 && l_J <= 4)
                            return true;
                        else
                            return false;
                    }
                }

                return false;
            }

            void SelectPath()
            {
                /// At this point, Oregorger is at an intersection point
                /// Choose in priority paths with one or more Blackrock Ore to eat
                std::pair<G3D::Vector2, uint8> l_PointDatas = GetRandomCollisionPoint();

                G3D::Vector2 l_Point = l_PointDatas.first;

                m_PointID = l_PointDatas.second;

                /// Calculate path
                if (l_Point.x != 0.0f && l_Point.y != 0.0f)
                {
                    /// At center, moves are always on a front lines, directly setup destination
                    if (IsCenterPos(l_Point.x, l_Point.y))
                    {
                        m_Destination.m_positionX = l_Point.x;
                        m_Destination.m_positionY = l_Point.y;
                        m_Destination.m_positionZ = me->GetPositionZ();

                        m_NextPos = Position();

                        me->SetFacingTo(me->GetAngle(&m_Destination));
                    }
                    else
                    {
                        m_Destination.m_positionX = l_Point.x;
                        m_Destination.m_positionY = l_Point.y;
                        m_Destination.m_positionZ = me->GetPositionZ();

                        float l_O = 0.0f;

                        /// In this case, the first step is the nearest collision point
                        if (IsCenterPos(me->m_positionX, me->m_positionY))
                        {
                            l_Point = GetNearestIntersectionPoint(me).first;

                            m_NextPos.m_positionX = l_Point.x;
                            m_NextPos.m_positionY = l_Point.y;
                            m_NextPos.m_positionZ = me->GetPositionZ();

                            l_O += me->GetAngle(&m_NextPos);

                            me->SetFacingTo(l_O);
                        }
                        else
                        {
                            l_O = me->GetOrientation();
                            l_O -= M_PI / 2.0f;

                            me->SetFacingTo(l_O);

                            m_NextPos = SelectNextPathIdentifier(false);
                        }
                    }
                }
            }

            Position SelectNextPathIdentifier(bool p_LaunchMove = true)
            {
                std::list<Creature*> l_PathList;
                me->GetCreatureListWithEntryInGrid(l_PathList, eCreatures::PathIdentifier, 5.0f);

                /// If no path identifier in line of sight, it should be the last move
                if (l_PathList.empty())
                {
                    if (p_LaunchMove)
                    {
                        AddTimedDelayedOperation(50, [this]() -> void
                        {
                            m_PathCount = 0;

                            me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, m_Destination);
                        });
                    }

                    return Position();
                }

                /// Only one should remain in this list
                l_PathList.remove_if([this](Creature* p_Creature) -> bool
                {
                    if (p_Creature == nullptr || !me->isInFront(p_Creature, M_PI / 2.0f))
                        return true;

                    if (p_Creature->GetDistance(me) < 0.5f)
                        return true;

                    return false;
                });

                /// Shouldn't happens
                if (l_PathList.empty())
                    return Position();

                Position l_Pos;

                if (Creature* l_PathIdentifier = (*l_PathList.begin()))
                {
                    l_Pos = *l_PathIdentifier;

                    ++m_PathCount;

                    if (p_LaunchMove)
                        me->GetMotionMaster()->MovePoint(eDatas::MovementFinished, *l_PathIdentifier);
                }

                return l_Pos;
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

            enum eSpell
            {
                CrateGlow = 160691
            };

            void Reset() override
            {
                me->CastSpell(me, eSpell::CrateGlow, true);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAuras();

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

/// Rolling Fury - 155898
class spell_foundry_rolling_fury_aura : public SpellScriptLoader
{
    public:
        spell_foundry_rolling_fury_aura() : SpellScriptLoader("spell_foundry_rolling_fury_aura") { }

        class spell_foundry_rolling_fury_aura_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_foundry_rolling_fury_aura_AuraScript);

            enum eSpell
            {
                RollingFuryDamage = 155900
            };

            uint32 m_DamageTimer;

            bool Load()
            {
                m_DamageTimer = 500;
                return true;
            }

            void OnUpdate(uint32 p_Diff)
            {
                if (m_DamageTimer)
                {
                    if (m_DamageTimer <= p_Diff)
                    {
                        if (Unit* l_Caster = GetCaster())
                        {
                            std::list<Unit*> l_TargetList;
                            float l_Radius = 1.0f;

                            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(l_Caster, l_Caster, l_Radius);
                            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(l_Caster, l_TargetList, l_Check);
                            l_Caster->VisitNearbyObject(l_Radius, l_Searcher);

                            for (Unit* l_Iter : l_TargetList)
                                l_Iter->CastSpell(l_Iter, eSpell::RollingFuryDamage, true, nullptr, NULLAURA_EFFECT, l_Caster->GetGUID());
                        }

                        m_DamageTimer = 500;
                    }
                    else
                        m_DamageTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_foundry_rolling_fury_aura_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_foundry_rolling_fury_aura_AuraScript();
        }
};

/// Harvest Volatile Blackrock - 163453
class spell_foundry_harvest_volatile_blackrock : public SpellScriptLoader
{
    public:
        spell_foundry_harvest_volatile_blackrock() : SpellScriptLoader("spell_foundry_harvest_volatile_blackrock") { }

        class spell_foundry_harvest_volatile_blackrock_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_harvest_volatile_blackrock_SpellScript);

            enum eSpell
            {
                CarryingVolatileBlackrock = 163454
            };

            void HandleScript(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eSpell::CarryingVolatileBlackrock, true);
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(spell_foundry_harvest_volatile_blackrock_SpellScript::HandleScript, EFFECT_2, SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_harvest_volatile_blackrock_SpellScript();
        }
};

/// Throw Volatile Ore - 163455
class spell_foundry_throw_volatile_ore : public SpellScriptLoader
{
    public:
        spell_foundry_throw_volatile_ore() : SpellScriptLoader("spell_foundry_throw_volatile_ore") { }

        class spell_foundry_throw_volatile_ore_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_foundry_throw_volatile_ore_SpellScript);

            enum eSpell
            {
                CarryingVolatileBlackrock = 163454
            };

            void HandleAfterCast()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->RemoveAura(eSpell::CarryingVolatileBlackrock);
            }

            void Register() override
            {
                AfterCast += SpellCastFn(spell_foundry_throw_volatile_ore_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_foundry_throw_volatile_ore_SpellScript();
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

/// Volatile Blackrock Ore - 237308
class go_foundry_volatile_blackrock_ore : public GameObjectScript
{
    public:
        go_foundry_volatile_blackrock_ore() : GameObjectScript("go_foundry_volatile_blackrock_ore") { }

        struct go_foundry_volatile_blackrock_oreAI : public GameObjectAI
        {
            go_foundry_volatile_blackrock_oreAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject), m_Deleted(false) { }

            bool m_Deleted;

            void OnStateChanged(uint32 p_State) override
            {
                if (p_State == GOState::GO_STATE_READY && !m_Deleted)
                {
                    m_Deleted = true;
                    go->Delete();
                }
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_foundry_volatile_blackrock_oreAI(p_GameObject);
        }
};

/// Ore Grinder - 231313
class go_founrdy_ore_grinder : public GameObjectScript
{
    public:
        go_founrdy_ore_grinder() : GameObjectScript("go_founrdy_ore_grinder") { }

        struct go_founrdy_ore_grinderAI : public GameObjectAI
        {
            go_founrdy_ore_grinderAI(GameObject* p_GameObject) : GameObjectAI(p_GameObject) { }

            enum eSpells
            {
                CompleteTheAchievement      = 163569,
                CarryingVolatileBlackrock   = 163454
            };

            bool GossipHello(Player* p_Player) override
            {
                if (p_Player->HasAura(eSpells::CarryingVolatileBlackrock))
                {
                    p_Player->CastSpell(p_Player, eSpells::CompleteTheAchievement, true);

                    if (InstanceScript* l_Instance = p_Player->GetInstanceScript())
                        l_Instance->SetData(eFoundryDatas::VolatileOreGrinded, 1);
                }

                return false;
            }
        };

        GameObjectAI* GetAI(GameObject* p_GameObject) const override
        {
            return new go_founrdy_ore_grinderAI(p_GameObject);
        }
};

void AddSC_boss_oregorger()
{
    /// Boss
    new boss_oregorger();

    /// NPC
    new npc_foundry_crate_ore();

    /// Spells
    new spell_foundry_acid_torrent();
    new spell_foundry_acid_torrent_aoe();
    new spell_foundry_rolling_fury_aura();
    new spell_foundry_harvest_volatile_blackrock();
    new spell_foundry_throw_volatile_ore();

    /// AreaTriggers
    new areatrigger_foundry_retched_blackrock();
    new areatrigger_foundry_explosive_shard();

    /// GameObjects
    new go_foundry_volatile_blackrock_ore();
    new go_founrdy_ore_grinder();
}