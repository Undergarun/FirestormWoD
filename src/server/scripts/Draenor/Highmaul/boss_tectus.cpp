////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "highmaul.hpp"

uint8 GetEnergyGainFromHealth(float p_HealthPct)
{
    if (p_HealthPct >= 50.0f)
        return 1;
    else if (p_HealthPct >= 20.0f)
        return 2;
    else if (p_HealthPct >= 10.0f)
        return 3;
    else if (p_HealthPct >= 5.0f)
        return 4;
    else
        return 5;
}

void RespawnGuardians(Creature* p_Source, InstanceScript* p_Instance)
{
    if (p_Source == nullptr || p_Instance == nullptr)
        return;

    if (Creature* l_Rokka = Creature::GetCreature(*p_Source, p_Instance->GetData64(eHighmaulCreatures::Rokka)))
    {
        l_Rokka->Respawn();
        l_Rokka->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Lokk = Creature::GetCreature(*p_Source, p_Instance->GetData64(eHighmaulCreatures::Lokk)))
    {
        l_Lokk->Respawn();
        l_Lokk->GetMotionMaster()->MoveTargetedHome();
    }

    if (Creature* l_Oro = Creature::GetCreature(*p_Source, p_Instance->GetData64(eHighmaulCreatures::Oro)))
    {
        l_Oro->Respawn();
        l_Oro->GetMotionMaster()->MoveTargetedHome();
    }
}

void StartGuardians(Creature* p_Source, Unit* p_Target)
{
    if (p_Source == nullptr || p_Target == nullptr)
        return;

    if (Creature* l_Rokka = p_Source->FindNearestCreature(eHighmaulCreatures::Rokka, 100.0f))
        l_Rokka->AI()->AttackStart(p_Target);

    if (Creature* l_Lokk = p_Source->FindNearestCreature(eHighmaulCreatures::Lokk, 100.0f))
        l_Lokk->AI()->AttackStart(p_Target);

    if (Creature* l_Oro = p_Source->FindNearestCreature(eHighmaulCreatures::Oro, 100.0f))
        l_Oro->AI()->AttackStart(p_Target);
}

/// Tectus <The Living Mountain> - 78948
/// Shard of Tectus <Shard of the Mountain> - 80551
/// Mote of Tectus <Mote of the Mountain> - 80557
class boss_tectus : public CreatureScript
{
    public:
        boss_tectus() : CreatureScript("boss_tectus") { }

        enum eSpells
        {
            /// Misc
            BreakPlayerTargetting       = 140562,
            SuicideNoBloodNoLogging     = 117624,
            SpawnTectusShards           = 169931,
            EncounterEvent              = 181089,   ///< Don't know why, maybe useful
            MidsizeTectusDamageReduct   = 178193,   ///< -30% damage done
            MoteOfTectusDamageReduct    = 178194,   ///< -60% damage done
            EarthFurySpawnDustCloud     = 169949,
            Grow                        = 166306,
            /// Energy Management
            ZeroPowerZeroRegen          = 118357,
            /// Periodic dummy of 1s
            TheLivingMountain           = 162287,
            ShardOfTheMountain          = 162658,
            MoteOfTheMountain           = 162674,
            /// Arrow visual on player
            SpellCrystallineBarrage     = 162346,
            CrystallineBarrageSummon    = 162371,
            CrystallineBarrageDoT       = 162370,
            /// +5% damage done
            Accretion                   = 162288,
            Petrification               = 163809,
            /// Fracture
            FractureSearcher            = 163214,   ///< Must trigger 163208
            FractureMissile             = 163208,   ///< Missile: 163209
            /// Tectonic Upheaval
            SpellTectonicUpheaval       = 162475,    ///< 2s cast time, triggers 162510 each 1.5s
            /// Loot
            TectusBonus                 = 177523
        };

        enum eEvents
        {
            EventEnrage = 1,
            EventSpawnBersererk,
            EventSpawnEarthwarper,
            EventCrystallineBarrage,
            EventSpawnCrystalline,
            /// Used at 25 energy
            EventEarthenPillar,
            EventFracture,
            EventAccretion,
            EventTectonicUpheaval
        };

        enum eActions
        {
            GuardianDead,
            ScheduleEarthenPillar,
            ScheduleTectonicUpheaval,
            MoteKilled
        };

        enum eAnimKits
        {
            AnimRise    = 6961,
            AnimRise2   = 6958,
            AnimFall    = 6918,
            AnimWounded = 2156
        };

        enum eCreatures
        {
            EarthenPillarStalker    = 80476,
            ShardOfTectus           = 80551,
            MoteOfTectus            = 80557,
            NightTwistedBerserker   = 80822,
            NightTwistedEarthwarper = 80599
        };

        enum eTalks
        {
            Aggro,
            Shattered,
            TectonicUpheaval,
            TectonicUpheavalCompleted,
            EarthenPillar,
            Slay,
            Death,
            CrystallineBarrage
        };

        enum eMiscs
        {
            ShardSpawnCount = 2,
            MotesSpawnCount = 4,
            InvisDisplay    = 11686
        };

        struct boss_tectusAI : public BossAI
        {
            boss_tectusAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossTectus)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            uint64 m_CrystallineBarrageTarget;

            Position m_FirstCrystalline;
            Position m_SecondCrystalline;

            bool m_TectonicScheduled;

            uint8 m_MoteKilled;

            void Reset() override
            {
                m_Events.Reset();

                if (me->GetEntry() == eHighmaulCreatures::Tectus)
                    _Reset();

                me->CastSpell(me, eSpells::ZeroPowerZeroRegen, true);

                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_PREPARATION | eUnitFlags::UNIT_FLAG_DISARMED);

                switch (me->GetEntry())
                {
                    case eHighmaulCreatures::Tectus:
                    {
                        me->CastSpell(me, eSpells::TheLivingMountain, true);

                        if (m_Instance)
                            m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellCrystallineBarrage);

                        if (!AllGardiansDead())
                        {
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        }
                        else
                        {
                            me->SetAIAnimKitId(0);
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        }

                        break;
                    }
                    case eCreatures::ShardOfTectus:
                    {
                        me->CastSpell(me, eSpells::ShardOfTheMountain, true);
                        me->CastSpell(me, eSpells::Grow, true);
                        me->SetDisplayId(eMiscs::InvisDisplay);
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
                        break;
                    }
                    case eCreatures::MoteOfTectus:
                    {
                        me->CastSpell(me, eSpells::MoteOfTheMountain, true);
                        me->CastSpell(me, eSpells::Grow, true);
                        me->SetDisplayId(eMiscs::InvisDisplay);
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
                        break;
                    }
                    default:
                        break;
                }

                me->RemoveAura(eHighmaulSpells::Berserker);

                me->RemoveAllAreasTrigger();

                me->SetPower(Powers::POWER_ENERGY, 0);
                me->SetMaxPower(Powers::POWER_ENERGY, 100);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER | eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                m_CrystallineBarrageTarget = 0;

                m_FirstCrystalline = Position();
                m_SecondCrystalline = Position();

                m_TectonicScheduled = false;

                m_MoteKilled = 0;
            }

            void JustReachedHome() override
            {
                if (!AllGardiansDead())
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                {
                    me->SetAIAnimKitId(0);
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            bool CanRespawn() override
            {
                return false;
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::GuardianDead:
                    {
                        if (!AllGardiansDead())
                            return;

                        me->SetAIAnimKitId(0);
                        me->PlayOneShotAnimKit(eAnimKits::AnimRise);

                        AddTimedDelayedOperation(4500, [this]() -> void
                        {
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        });

                        break;
                    }
                    case eActions::ScheduleEarthenPillar:
                    {
                        if (m_Events.HasEvent(eEvents::EventEarthenPillar))
                            break;
                        m_Events.ScheduleEvent(eEvents::EventEarthenPillar, 100);
                        break;
                    }
                    case eActions::ScheduleTectonicUpheaval:
                    {
                        if (m_TectonicScheduled || me->HasAura(eSpells::SpellTectonicUpheaval))
                            break;
                        m_TectonicScheduled = true;
                        m_Events.ScheduleEvent(eEvents::EventTectonicUpheaval, 100);
                        break;
                    }
                    case eActions::MoteKilled:
                    {
                        if (m_Instance != nullptr)
                            m_Instance->SetData(eHighmaulDatas::TectusAchievement, time(nullptr));

                        ++m_MoteKilled;

                        if (m_MoteKilled >= (eMiscs::MotesSpawnCount * 2))
                        {
                            me->SetAIAnimKitId(0);
                            me->PlayOneShotAnimKit(eAnimKits::AnimRise);

                            AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                me->SetAIAnimKitId(eAnimKits::AnimWounded);
                                Talk(eTalks::Death);
                            });

                            AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                                if (Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource())
                                    l_Player->CastSpell(me, eSpells::SuicideNoBloodNoLogging, true);
                            });
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                if (!AllGardiansDead())
                {
                    EnterEvadeMode();
                    return;
                }

                if (me->GetEntry() == eHighmaulCreatures::Tectus)
                    _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventCrystallineBarrage, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFracture, 8 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventAccretion, 5 * TimeConstants::IN_MILLISECONDS);

                switch (me->GetEntry())
                {
                    case eCreatures::ShardOfTectus:
                        me->CastSpell(me, eSpells::MidsizeTectusDamageReduct, true);
                        if (m_Instance)
                            m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);
                        break;
                    case eCreatures::MoteOfTectus:
                        me->CastSpell(me, eSpells::MoteOfTectusDamageReduct, true);
                        if (m_Instance)
                            m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);
                        break;
                    case eHighmaulCreatures::Tectus:
                        if (m_Instance)
                            m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me);
                        Talk(eTalks::Aggro);
                        m_Events.ScheduleEvent(eEvents::EventEnrage, IsMythic() ? 480 * TimeConstants::IN_MILLISECONDS : 600 * TimeConstants::IN_MILLISECONDS);
                        m_Events.ScheduleEvent(eEvents::EventSpawnBersererk, 18 * TimeConstants::IN_MILLISECONDS);
                        m_Events.ScheduleEvent(eEvents::EventSpawnEarthwarper, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }
            }

            void KilledUnit(Unit* p_Killed) override
            {
                if (p_Killed->GetTypeId() == TypeID::TYPEID_PLAYER && me->GetEntry() == eHighmaulCreatures::Tectus)
                    Talk(eTalks::Slay);
            }

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAllAreasTrigger();

                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellCrystallineBarrage);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::CrystallineBarrageDoT);

                    if (me->GetEntry() == eHighmaulCreatures::Tectus)
                    {
                        _JustDied();

                        if (sObjectMgr->IsDisabledEncounter(m_Instance->GetEncounterIDForBoss(me), GetDifficulty()))
                            me->SetLootRecipient(nullptr);
                        else
                            CastSpellToPlayers(me->GetMap(), me, eSpells::TectusBonus, true);
                    }

                    Map::PlayerList const& l_PlayerList = m_Instance->instance->GetPlayers();
                    if (l_PlayerList.isEmpty())
                        return;

                    for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = l_Itr->getSource())
                        {
                            /// Hacky but don't know why combat doesn't stop
                            if (!l_Player->isAttackingPlayer())
                                l_Player->CombatStop();
                        }
                    }
                }

                std::list<Creature*> l_Creatures;
                me->GetCreatureListWithEntryInGrid(l_Creatures, eCreatures::MoteOfTectus, 150.0f);

                for (Creature* l_Creature : l_Creatures)
                {
                    me->Kill(l_Creature);
                    l_Creature->DespawnOrUnsummon();
                }

                l_Creatures.clear();

                me->GetCreatureListWithEntryInGrid(l_Creatures, eCreatures::EarthenPillarStalker, 150.0f);

                for (Creature* l_Creature : l_Creatures)
                {
                    me->Kill(l_Creature);
                    l_Creature->DespawnOrUnsummon();
                }
            }

            void EnterEvadeMode() override
            {
                me->ClearUnitState(UnitState::UNIT_STATE_STUNNED);

                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(eHighmaulDatas::BossTectus, EncounterState::FAIL);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SpellCrystallineBarrage);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::CrystallineBarrageDoT);
                }

                std::list<Creature*> l_Motes;
                me->GetCreatureListWithEntryInGrid(l_Motes, eCreatures::MoteOfTectus, 150.0f);

                for (Creature* l_Creature : l_Motes)
                {
                    if (l_Creature->IsAIEnabled)
                        l_Creature->AI()->Reset();

                    l_Creature->DespawnOrUnsummon();
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::FractureSearcher:
                        me->CastSpell(p_Target, eSpells::FractureMissile, true);
                        break;
                    case eSpells::Petrification:
                        Talk(eTalks::TectonicUpheavalCompleted);
                        if (me->GetEntry() == eHighmaulCreatures::Tectus)
                            me->CastSpell(me, eSpells::TheLivingMountain, true);
                        else if (me->GetEntry() == eCreatures::ShardOfTectus)
                            me->CastSpell(me, eSpells::ShardOfTheMountain, true);
                        break;
                    case eSpells::SpellTectonicUpheaval:
                        m_TectonicScheduled = false;
                        break;
                    case eSpells::SpawnTectusShards:
                    {
                        p_Target->PlayOneShotAnimKit(eAnimKits::AnimRise2);
                        p_Target->RestoreDisplayId();

                        uint64 l_Guid = p_Target->GetGUID();
                        AddTimedDelayedOperation(4 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                        {
                            if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                l_Target->CastSpell(l_Target, eSpells::EarthFurySpawnDustCloud, true);
                        });

                        AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this, l_Guid]() -> void
                        {
                            if (Unit* l_Target = Unit::GetUnit(*me, l_Guid))
                                l_Target->ClearUnitState(UnitState::UNIT_STATE_STUNNED);
                        });

                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                /// Prevent bug when boss instakills himself
                if (p_Attacker == me)
                    return;

                /// Prevent boss from dying too soon
                if (me->HasFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE))
                {
                    p_Damage = 0;
                    return;
                }

                /// This buff cause Tectus to be unkillable, although he can still be damaged during this time.
                if (p_Damage > me->GetHealth())
                {
                    if (me->HasAura(eSpells::TheLivingMountain) || me->HasAura(eSpells::ShardOfTheMountain))
                    {
                        me->SetHealth(1);
                        p_Damage = 0;
                        return;
                    }
                    else
                    {
                        m_Events.Reset();

                        me->RemoveAllAuras();
                        me->InterruptNonMeleeSpells(true);
                        me->RemoveAllAreasTrigger();
                        me->SetHealth(1);
                        p_Damage = 0;

                        me->CastSpell(me, eSpells::EncounterEvent, true);
                        me->CastSpell(me, eSpells::BreakPlayerTargetting, true);
                        me->CastSpell(me, eSpells::ZeroPowerZeroRegen, true);

                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);

                        me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
                        me->SetAIAnimKitId(eAnimKits::AnimFall);

                        if (m_Instance)
                            m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                        if (me->GetEntry() == eHighmaulCreatures::Tectus)
                        {
                            Talk(eTalks::Shattered);
                            SpawnShards();
                        }
                        else if (me->GetEntry() == eCreatures::ShardOfTectus)
                            SpawnMotes();
                        else
                        {
                            if (Creature* l_Tectus = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Tectus)))
                                l_Tectus->AI()->DoAction(eActions::MoteKilled);
                        }
                    }
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value) override
            {
                /// Tectus only regens by script
                p_Value = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                if (!UpdateVictim())
                    return;

                if (me->GetDistance(me->GetHomePosition()) >= 70.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventEnrage:
                    {
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);

                        std::list<Creature*> l_Creature;
                        me->GetCreatureListWithEntryInGrid(l_Creature, eCreatures::ShardOfTectus, 150.0f);

                        for (Creature* l_Shard : l_Creature)
                            l_Shard->CastSpell(l_Shard, eHighmaulSpells::Berserker, true);

                        l_Creature.clear();
                        me->GetCreatureListWithEntryInGrid(l_Creature, eCreatures::MoteOfTectus, 150.0f);

                        for (Creature* l_Mote : l_Creature)
                            l_Mote->CastSpell(l_Mote, eHighmaulSpells::Berserker, true);

                        break;
                    }
                    case eEvents::EventSpawnBersererk:
                    {
                        SpawnAdd(eCreatures::NightTwistedBerserker);
                        m_Events.ScheduleEvent(eEvents::EventSpawnBersererk, 40 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSpawnEarthwarper:
                    {
                        SpawnAdd(eCreatures::NightTwistedEarthwarper);
                        m_Events.ScheduleEvent(eEvents::EventSpawnEarthwarper, 40 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventCrystallineBarrage:
                    {
                        /// Crystalline Barrage should not select Main Tank or Off Tank
                        Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -20.0f, true);
                        if (!l_Target)
                            l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -10.0f, true);
                        if (!l_Target)
                            l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 0.0f, true);

                        if (l_Target != nullptr)
                        {
                            Talk(eTalks::CrystallineBarrage, l_Target->GetGUID());

                            me->CastSpell(l_Target, eSpells::SpellCrystallineBarrage, true);
                            m_CrystallineBarrageTarget = l_Target->GetGUID();

                            AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                            {
                                float l_Range = 7.0f;
                                float l_Orientation = me->GetOrientation();
                                float l_OrigX = me->GetPositionX();
                                float l_OrigY = me->GetPositionY();
                                float l_Z = me->GetPositionZ();

                                float l_X = l_OrigX + (l_Range * cos(l_Orientation - M_PI / 2));
                                float l_Y = l_OrigY + (l_Range * sin(l_Orientation - M_PI / 2));
                                me->CastSpell(l_X, l_Y, l_Z, eSpells::CrystallineBarrageSummon, true);

                                m_FirstCrystalline.m_positionX = l_X;
                                m_FirstCrystalline.m_positionY = l_Y;
                                m_FirstCrystalline.m_positionZ = l_Z;

                                l_X = l_OrigX + (l_Range * cos(l_Orientation + M_PI / 2));
                                l_Y = l_OrigY + (l_Range * sin(l_Orientation + M_PI / 2));
                                me->CastSpell(l_X, l_Y, l_Z, eSpells::CrystallineBarrageSummon, true);

                                m_SecondCrystalline.m_positionX = l_X;
                                m_SecondCrystalline.m_positionY = l_Y;
                                m_SecondCrystalline.m_positionZ = l_Z;

                                m_Events.ScheduleEvent(eEvents::EventSpawnCrystalline, 500);
                            });
                        }

                        m_Events.ScheduleEvent(eEvents::EventCrystallineBarrage, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSpawnCrystalline:
                    {
                        if (Unit* l_Target = Unit::GetUnit(*me, m_CrystallineBarrageTarget))
                        {
                            if (!l_Target->HasAura(eSpells::SpellCrystallineBarrage))
                            {
                                m_CrystallineBarrageTarget = 0;
                                m_FirstCrystalline = Position();
                                m_SecondCrystalline = Position();
                                break;
                            }

                            float l_Range = 2.0f;
                            float l_Z = me->GetPositionZ();

                            float l_Orientation = m_FirstCrystalline.GetAngle(l_Target);
                            float l_X = m_FirstCrystalline.m_positionX + (l_Range * cos(l_Orientation));
                            float l_Y = m_FirstCrystalline.m_positionY + (l_Range * sin(l_Orientation));
                            me->CastSpell(l_X, l_Y, l_Z, eSpells::CrystallineBarrageSummon, true);

                            m_FirstCrystalline.m_positionX = l_X;
                            m_FirstCrystalline.m_positionY = l_Y;
                            m_FirstCrystalline.m_positionZ = l_Z;

                            l_Orientation = m_SecondCrystalline.GetAngle(l_Target);
                            l_X = m_SecondCrystalline.m_positionX + (l_Range * cos(l_Orientation));
                            l_Y = m_SecondCrystalline.m_positionY + (l_Range * sin(l_Orientation));
                            me->CastSpell(l_X, l_Y, l_Z, eSpells::CrystallineBarrageSummon, true);

                            m_SecondCrystalline.m_positionX = l_X;
                            m_SecondCrystalline.m_positionY = l_Y;
                            m_SecondCrystalline.m_positionZ = l_Z;
                        }

                        m_Events.ScheduleEvent(eEvents::EventSpawnCrystalline, 500);
                        break;
                    }
                    case eEvents::EventFracture:
                    {
                        me->CastSpell(me, eSpells::FractureSearcher, true);
                        m_Events.ScheduleEvent(eEvents::EventFracture, 6 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventEarthenPillar:
                    {
                        Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -20.0f, true);
                        if (!l_Target)
                            l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -10.0f, true);
                        if (!l_Target)
                            l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 0.0f, true);

                        if (l_Target != nullptr)
                        {
                            float l_X = l_Target->GetPositionX();
                            float l_Y = l_Target->GetPositionY();
                            me->SummonCreature(eCreatures::EarthenPillarStalker, l_X, l_Y, me->GetPositionZ());
                        }

                        Talk(eTalks::EarthenPillar);
                        break;
                    }
                    case eEvents::EventAccretion:
                    {
                        me->CastSpell(me, eSpells::Accretion, true);
                        m_Events.ScheduleEvent(eEvents::EventAccretion, 5 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventTectonicUpheaval:
                    {
                        /// If Tectus's health is depleted during Tectonic Upheaval, he will Shatter.
                        Talk(eTalks::TectonicUpheaval);
                        me->RemoveAura(eSpells::TheLivingMountain);
                        me->RemoveAura(eSpells::ShardOfTheMountain);
                        me->CastSpell(me, eSpells::SpellTectonicUpheaval, false);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            bool AllGardiansDead() const
            {
                if (m_Instance == nullptr)
                    return false;

                if (Creature* l_Rokka = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Rokka)))
                {
                    if (l_Rokka->isAlive())
                        return false;
                }

                if (Creature* l_Lokk = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Lokk)))
                {
                    if (l_Lokk->isAlive())
                        return false;
                }

                if (Creature* l_Oro = Creature::GetCreature(*me, m_Instance->GetData64(eHighmaulCreatures::Oro)))
                {
                    if (l_Oro->isAlive())
                        return false;
                }

                return true;
            }

            void SpawnShards()
            {
                float l_OrigX = me->GetPositionX();
                float l_OrigY = me->GetPositionY();
                float l_Z = me->GetPositionZ();
                float l_Range = 7.0f;

                for (uint8 l_I = 0; l_I < eMiscs::ShardSpawnCount; ++l_I)
                {
                    float l_Orientation = frand(0, 2 * M_PI);
                    float l_X = l_OrigX + (l_Range * cos(l_Orientation));
                    float l_Y = l_OrigY + (l_Range * sin(l_Orientation));

                    if (Creature* l_Shard = me->SummonCreature(eCreatures::ShardOfTectus, l_X, l_Y, l_Z))
                        me->CastSpell(l_Shard, eSpells::SpawnTectusShards, true);
                }
            }

            void SpawnMotes()
            {
                float l_OrigX = me->GetPositionX();
                float l_OrigY = me->GetPositionY();
                float l_Z = me->GetPositionZ();
                float l_Range = 7.0f;

                for (uint8 l_I = 0; l_I < eMiscs::MotesSpawnCount; ++l_I)
                {
                    float l_Orientation = frand(0, 2 * M_PI);
                    float l_X = l_OrigX + (l_Range * cos(l_Orientation));
                    float l_Y = l_OrigY + (l_Range * sin(l_Orientation));

                    if (Creature* l_Mote = me->SummonCreature(eCreatures::MoteOfTectus, l_X, l_Y, l_Z))
                        me->CastSpell(l_Mote, eSpells::SpawnTectusShards, true);
                }
            }

            void SpawnAdd(uint32 p_Entry)
            {
                Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -20.0f, true);
                if (!l_Target)
                    l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, -10.0f, true);
                if (!l_Target)
                    l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 2, 0.0f, true);

                if (l_Target != nullptr)
                {
                    float l_O = frand(0, 2 * M_PI);
                    float l_Range = 5.0f;
                    float l_X = l_Target->GetPositionX() + (l_Range * cos(l_O));
                    float l_Y = l_Target->GetPositionY() + (l_Range * sin(l_O));

                    if (Creature* l_Add = me->SummonCreature(p_Entry, l_X, l_Y, me->GetPositionZ()))
                    {
                        if (l_Add->IsAIEnabled)
                            l_Add->AI()->AttackStart(l_Target);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_tectusAI(p_Creature);
        }
};

/// Night-Twisted Supplicant - 86185
class npc_highmaul_night_twisted_supplicant : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_supplicant() : CreatureScript("npc_highmaul_night_twisted_supplicant") { }

        enum eSpell
        {
            NightTwistedCovenant = 172138
        };

        enum eDisplay
        {
            InvisDisplay = 11686
        };

        enum eAction
        {
            Rise
        };

        struct npc_highmaul_night_twisted_supplicantAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_night_twisted_supplicantAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature) { }

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                SetCanSeeEvenInPassiveMode(true);
            }

            bool CanRespawn() override
            {
                /// This mob is only cosmetic for Rokka, Lokk and Oro spawn
                return false;
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_Target == nullptr || p_SpellInfo->Id != eSpell::NightTwistedCovenant)
                    return;

                p_Target->SetDisplayId(eDisplay::InvisDisplay);
            }

            void MoveInLineOfSight(Unit* p_Mover) override
            {
                if (me->GetDisplayId() == eDisplay::InvisDisplay)
                    return;

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (p_Mover->GetTypeId() != TypeID::TYPEID_PLAYER)
                    return;

                std::list<Creature*> l_OtherMe;
                me->GetCreatureListWithEntryInGrid(l_OtherMe, me->GetEntry(), 100.0f);

                for (Creature* l_Covenant : l_OtherMe)
                    l_Covenant->CastSpell(l_Covenant, eSpell::NightTwistedCovenant, false);

                AddTimedDelayedOperation(5000, [this]() -> void
                {
                    if (Creature* l_Rokka = me->FindNearestCreature(eHighmaulCreatures::Rokka, 100.0f))
                        l_Rokka->AI()->DoAction(eAction::Rise);

                    if (Creature* l_Lokk = me->FindNearestCreature(eHighmaulCreatures::Lokk, 100.0f))
                        l_Lokk->AI()->DoAction(eAction::Rise);

                    if (Creature* l_Oro = me->FindNearestCreature(eHighmaulCreatures::Oro, 100.0f))
                        l_Oro->AI()->DoAction(eAction::Rise);
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_supplicantAI(p_Creature);
        }
};

/// Rokkaa <Heart of Tectus> - 86071
/// Lokk <Hands of Tectus> - 86073
class npc_highmaul_rokka_and_lokk : public CreatureScript
{
    public:
        npc_highmaul_rokka_and_lokk() : CreatureScript("npc_highmaul_rokka_and_lokk") { }

        enum eAnimKit
        {
            AnimRise = 6961
        };

        enum eActions
        {
            Rise         = 0,
            GuardianDead = 0
        };

        enum eSpells
        {
            /// Grow scale, regens 10% of health every second
            Reconstitution      = 172116,
            ReconstitutionScale = 172126,
            /// AoE on caster
            EarthenThrust       = 172115,
            /// Trigger 172073 missile
            /// If no player hit, casts 172078
            MeteoricEarthspire  = 172110
        };

        enum eEvents
        {
            EventEarthenThrust = 1,
            EventMeteoricEarthspire,
            EventReconstitution
        };

        struct npc_highmaul_rokka_and_lokkAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_rokka_and_lokkAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Risen = false;
            }

            bool m_Risen;

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                if (!m_Risen)
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                    me->SetAIAnimKitId(0);

                m_Events.Reset();

                RespawnGuardians(me, m_Instance);
            }

            void JustReachedHome() override
            {
                if (m_Risen)
                    me->SetAIAnimKitId(0);
            }

            bool CanRespawn() override
            {
                return false;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartGuardians(me, p_Attacker);

                m_Events.ScheduleEvent(eEvents::EventEarthenThrust, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMeteoricEarthspire, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_Events.HasEvent(eEvents::EventReconstitution) || me->HasAura(eSpells::ReconstitutionScale))
                    return;

                if (me->HealthBelowPctDamaged(21, p_Damage))
                    m_Events.ScheduleEvent(eEvents::EventReconstitution, 100);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::Rise && !m_Risen)
                {
                    m_Risen = true;
                    me->SetAIAnimKitId(0);
                    me->PlayOneShotAnimKit(eAnimKit::AnimRise);

                    AddTimedDelayedOperation(4500, [this]() -> void
                    {
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    });
                }
            }

            void JustDied(Unit* p_Killer) override
            {
                if (Creature* l_Tectus = me->FindNearestCreature(eHighmaulCreatures::Tectus, 100.0f))
                    l_Tectus->AI()->DoAction(eActions::GuardianDead);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventEarthenThrust:
                        me->CastSpell(me, eSpells::EarthenThrust, false);
                        m_Events.ScheduleEvent(eEvents::EventEarthenThrust, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventMeteoricEarthspire:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::MeteoricEarthspire, false);
                        m_Events.ScheduleEvent(eEvents::EventMeteoricEarthspire, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventReconstitution:
                        me->CastSpell(me, eSpells::ReconstitutionScale, true);
                        me->CastSpell(me, eSpells::Reconstitution, false);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rokka_and_lokkAI(p_Creature);
        }
};

/// Oro <Wrath of Tectus> - 86072
class npc_highmaul_oro : public CreatureScript
{
    public:
        npc_highmaul_oro() : CreatureScript("npc_highmaul_oro") { }

        enum eAnimKit
        {
            AnimRise = 6961
        };

        enum eActions
        {
            Rise         = 0,
            GuardianDead = 0
        };

        enum eSpells
        {
            /// Grow scale, regens 10% of health every second
            Reconstitution      = 172116,
            ReconstitutionScale = 172126,
            /// Launches Stonebolt at targets further than 5 yards
            StoneboltVolley     = 172058,
            /// Poisons a player, inflicting damage to his allies
            RadiatingPoison     = 172066
        };

        enum eEvents
        {
            EventStoneboltVolley = 1,
            EventRadiatingPoison,
            EventReconstitution
        };

        struct npc_highmaul_oroAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_oroAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Risen = false;
            }

            bool m_Risen;

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset() override
            {
                if (!m_Risen)
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
                else
                    me->SetAIAnimKitId(0);

                m_Events.Reset();

                RespawnGuardians(me, m_Instance);
            }

            void JustReachedHome() override
            {
                if (m_Risen)
                    me->SetAIAnimKitId(0);
            }

            bool CanRespawn() override
            {
                return false;
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                StartGuardians(me, p_Attacker);

                m_Events.ScheduleEvent(eEvents::EventStoneboltVolley, 6 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRadiatingPoison, 10 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action == eActions::Rise && !m_Risen)
                {
                    m_Risen = true;
                    me->SetAIAnimKitId(0);
                    me->PlayOneShotAnimKit(eAnimKit::AnimRise);

                    AddTimedDelayedOperation(4500, [this]() -> void
                    {
                        me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                    });
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo) override
            {
                if (m_Events.HasEvent(eEvents::EventReconstitution) || me->HasAura(eSpells::ReconstitutionScale))
                    return;

                if (me->HealthBelowPctDamaged(21, p_Damage))
                    m_Events.ScheduleEvent(eEvents::EventReconstitution, 100);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (Creature* l_Tectus = me->FindNearestCreature(eHighmaulCreatures::Tectus, 100.0f))
                    l_Tectus->AI()->DoAction(eActions::GuardianDead);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventStoneboltVolley:
                        me->CastSpell(me, eSpells::StoneboltVolley, false);
                        m_Events.ScheduleEvent(eEvents::EventStoneboltVolley, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRadiatingPoison:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::RadiatingPoison, false);
                        m_Events.ScheduleEvent(eEvents::EventRadiatingPoison, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventReconstitution:
                        me->CastSpell(me, eSpells::ReconstitutionScale, true);
                        me->CastSpell(me, eSpells::Reconstitution, false);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_oroAI(p_Creature);
        }
};

/// Earthen Pillar Stalker - 80476
class npc_highmaul_earthen_pillar_stalker : public CreatureScript
{
    public:
        npc_highmaul_earthen_pillar_stalker() : CreatureScript("npc_highmaul_earthen_pillar_stalker") { }

        struct npc_highmaul_earthen_pillar_stalkerAI: public MS::AI::CosmeticAI
        {
            npc_highmaul_earthen_pillar_stalkerAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_PillarGuid = 0;
            }

            uint64 m_PillarGuid;

            enum eSpells
            {
                EarthenPillarTimer  = 166024,
                EarthenPillarKill   = 162522
            };

            enum eGameObject
            {
                GoBEarthenPillar = 229649
            };

            void Reset() override
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->CastSpell(me, eSpells::EarthenPillarTimer, true);

                if (GameObject* l_Pillar = me->SummonGameObject(eGameObject::GoBEarthenPillar, *me, 0.0f, 0.0f, 0.0f, 0.0f, 0, 0, nullptr, 100, 0, false, true))
                    m_PillarGuid = l_Pillar->GetGUID();
            }

            void DoAction(int32 const p_Action) override
            {
                if (p_Action)
                    return;

                me->CastSpell(me, eSpells::EarthenPillarKill, true);

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (GameObject* l_Pillar = GameObject::GetGameObject(*me, m_PillarGuid))
                        l_Pillar->Delete();

                    if (GameObject* l_Pillar = me->SummonGameObject(eGameObject::GoBEarthenPillar, *me, 0.0f, 0.0f, 0.0f, 0.0f, 0))
                        m_PillarGuid = l_Pillar->GetGUID();
                });

                AddTimedDelayedOperation(60 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    if (GameObject* l_Pillar = GameObject::GetGameObject(*me, m_PillarGuid))
                        l_Pillar->Delete();

                    me->DespawnOrUnsummon();
                });
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_earthen_pillar_stalkerAI(p_Creature);
        }
};

/// Night-Twisted Berserker - 80822
class npc_highmaul_night_twisted_berserker : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_berserker() : CreatureScript("npc_highmaul_night_twisted_berserker") { }

        struct npc_highmaul_night_twisted_berserkerAI : public ScriptedAI
        {
            npc_highmaul_night_twisted_berserkerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                EncounterSpawn  = 181113,
                RavingAssault   = 163312
            };

            enum eEvent
            {
                EventRavingAssault = 1
            };

            enum eTalk
            {
                Aggro
            };

            EventMap m_Events;

            void Reset() override
            {
                me->CastSpell(me, eSpells::EncounterSpawn, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalk::Aggro);

                m_Events.ScheduleEvent(eEvent::EventRavingAssault, 8 * TimeConstants::IN_MILLISECONDS);
            }

            void SpellHitTarget(Unit* p_Victim, SpellInfo const* p_SpellInfo) override
            {
                if (p_Victim == nullptr || p_Victim == me->ToUnit() || p_SpellInfo->Id != eSpells::RavingAssault)
                    return;

                Position l_Pos  = *p_Victim;
                float l_Angle   = me->GetRelativeAngle(l_Pos.GetPositionX(), l_Pos.GetPositionY());
                float l_Dist    = me->GetDistance(l_Pos);

                me->GetFirstCollisionPosition(l_Pos, l_Dist, l_Angle);
                me->GetMotionMaster()->MoveCharge(&l_Pos, SPEED_CHARGE, eSpells::RavingAssault);
            }

            void MovementInform(uint32 p_Type, uint32 p_ID) override
            {
                if (p_ID != eSpells::RavingAssault)
                    return;

                me->RemoveAura(eSpells::RavingAssault);
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
                    case eEvent::EventRavingAssault:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::RavingAssault, false);
                        m_Events.ScheduleEvent(eEvent::EventRavingAssault, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new npc_highmaul_night_twisted_berserkerAI(p_Creature);
        }
};

/// Night-Twisted Earthwarper - 80599
class npc_highmaul_night_twisted_earthwarper : public CreatureScript
{
    public:
        npc_highmaul_night_twisted_earthwarper() : CreatureScript("npc_highmaul_night_twisted_earthwarper") { }

        struct npc_highmaul_night_twisted_earthwarperAI : public ScriptedAI
        {
            npc_highmaul_night_twisted_earthwarperAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                EncounterSpawn      = 181113,
                GiftOfEarth         = 162894,
                EarthenFlechettes   = 162968
            };

            enum eEvents
            {
                EventEarthenFlechettes = 1,
                EventGiftOfEarth
            };

            enum eTalk
            {
                Aggro
            };

            EventMap m_Events;

            void Reset() override
            {
                me->CastSpell(me, eSpells::EncounterSpawn, true);

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                Talk(eTalk::Aggro);

                m_Events.ScheduleEvent(eEvents::EventEarthenFlechettes, 5 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventGiftOfEarth, 12 * TimeConstants::IN_MILLISECONDS);
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
                    case eEvents::EventEarthenFlechettes:
                        me->CastSpell(me, eSpells::EarthenFlechettes, false);
                        m_Events.ScheduleEvent(eEvents::EventEarthenFlechettes, 15 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventGiftOfEarth:
                        me->CastSpell(me, eSpells::GiftOfEarth, false);
                        m_Events.ScheduleEvent(eEvents::EventGiftOfEarth, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_night_twisted_earthwarperAI(p_Creature);
        }
};

/// Meteoric Earthspire - 172073
class spell_highmaul_meteoric_earthspire : public SpellScriptLoader
{
    public:
        spell_highmaul_meteoric_earthspire() : SpellScriptLoader("spell_highmaul_meteoric_earthspire") { }

        class spell_highmaul_meteoric_earthspire_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_meteoric_earthspire_SpellScript);

            enum eSpell
            {
                RupturedEarth = 172078
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (!p_Targets.empty())
                    return;

                if (Unit* l_Caster = GetCaster())
                {
                    if (WorldLocation const* l_Location = GetExplTargetDest())
                        l_Caster->CastSpell(l_Location, eSpell::RupturedEarth, true);
                }
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_meteoric_earthspire_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_meteoric_earthspire_SpellScript();
        }
};

/// Stonebolt Volley - 172058
class spell_highmaul_stonebolt_volley : public SpellScriptLoader
{
    public:
        spell_highmaul_stonebolt_volley() : SpellScriptLoader("spell_highmaul_stonebolt_volley") { }

        class spell_highmaul_stonebolt_volley_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_stonebolt_volley_SpellScript);

            enum eSpell
            {
                RupturedEarth = 172078
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (p_Object->GetDistance(l_Caster) <= 5.0f)
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_stonebolt_volley_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_stonebolt_volley_SpellScript();
        }
};

/// The Living Mountain - 162287
/// Shard of the Mountain - 162658
/// Mote of the Mountain - 162674
class spell_highmaul_tectus_energy_gain : public SpellScriptLoader
{
    public:
        spell_highmaul_tectus_energy_gain() : SpellScriptLoader("spell_highmaul_tectus_energy_gain") { }

        class spell_highmaul_tectus_energy_gain_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_tectus_energy_gain_AuraScript);

            enum eActions
            {
                ScheduleEarthenPillar = 1,
                ScheduleTectonicUpheaval
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (!l_Target->isInCombat())
                        return;

                    uint32 l_OldPower = l_Target->GetPower(Powers::POWER_ENERGY);
                    int32 l_PowerGain = GetEnergyGainFromHealth(l_Target->GetHealthPct());
                    l_Target->EnergizeBySpell(l_Target, GetSpellInfo()->Id, l_PowerGain, Powers::POWER_ENERGY);
                    uint32 l_NewPower = l_OldPower + l_PowerGain;

                    if (l_Target->IsAIEnabled)
                    {
                        if (l_NewPower >= 100)
                            l_Target->AI()->DoAction(eActions::ScheduleTectonicUpheaval);
                        else
                        {
                            /// On Mythic difficulty, Tectus also uses this ability at 50 Energy.
                            if (l_Target->GetMap()->IsMythic())
                            {
                                if ((l_OldPower < 25 && l_NewPower >= 25) ||
                                    (l_OldPower < 50 && l_NewPower >= 50))
                                    l_Target->AI()->DoAction(eActions::ScheduleEarthenPillar);
                            }
                            else
                            {
                                if (l_OldPower < 25 && l_NewPower >= 25)
                                    l_Target->AI()->DoAction(eActions::ScheduleEarthenPillar);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_tectus_energy_gain_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_tectus_energy_gain_AuraScript();
        }
};

/// Earthen Pillar (timer) - 166024
class spell_highmaul_earthen_pillar_timer : public SpellScriptLoader
{
    public:
        spell_highmaul_earthen_pillar_timer() : SpellScriptLoader("spell_highmaul_earthen_pillar_timer") { }

        class spell_highmaul_earthen_pillar_timer_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_earthen_pillar_timer_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (l_Target->IsAIEnabled)
                        l_Target->AI()->DoAction(0);
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_earthen_pillar_timer_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_earthen_pillar_timer_AuraScript();
        }
};

/// Accretion - 162288
class spell_highmaul_accretion : public SpellScriptLoader
{
    public:
        spell_highmaul_accretion() : SpellScriptLoader("spell_highmaul_accretion") { }

        class spell_highmaul_accretion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_accretion_AuraScript);

            uint32 m_DamageTaken;

            bool Load()
            {
                m_DamageTaken = 0;
                return true;
            }

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Target = GetTarget())
                {
                    m_DamageTaken += p_EventInfo.GetDamageInfo()->GetDamage();

                    if (m_DamageTaken >= l_Target->CountPctFromMaxHealth(2))
                    {
                        p_AurEff->GetBase()->DropStack();
                        m_DamageTaken = 0;
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_highmaul_accretion_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_DAMAGE_PERCENT_DONE);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_accretion_AuraScript();
        }
};

/// Tectonic Upheaval - 162475
class spell_highmaul_tectonic_upheaval : public SpellScriptLoader
{
    public:
        spell_highmaul_tectonic_upheaval() : SpellScriptLoader("spell_highmaul_tectonic_upheaval") { }

        class spell_highmaul_tectonic_upheaval_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_tectonic_upheaval_AuraScript);

            enum eSpell
            {
                Petrification = 163809
            };

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->EnergizeBySpell(l_Target, GetSpellInfo()->Id, -10, Powers::POWER_ENERGY);
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (Unit* l_Target = GetTarget())
                {
                    AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                    if (l_RemoveMode != AuraRemoveMode::AURA_REMOVE_BY_CANCEL)
                        l_Target->CastSpell(l_Target, eSpell::Petrification, true);

                    l_Target->SetPower(Powers::POWER_ENERGY, 0);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_tectonic_upheaval_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_tectonic_upheaval_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_tectonic_upheaval_AuraScript();
        }
};

/// Highmaul Raid - Earth Fury - Spawn Dust Cloud - 169949
class spell_highmaul_spawn_dust_cloud : public SpellScriptLoader
{
    public:
        spell_highmaul_spawn_dust_cloud() : SpellScriptLoader("spell_highmaul_spawn_dust_cloud") { }

        class spell_highmaul_spawn_dust_cloud_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_spawn_dust_cloud_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
            {
                if (GetTarget() == nullptr)
                    return;

                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    l_Target->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    l_Target->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                }
            }

            void Register() override
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_highmaul_spawn_dust_cloud_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_spawn_dust_cloud_AuraScript();
        }
};

/// Earthen Flechettes - 162968
class spell_highmaul_earthen_flechettes : public SpellScriptLoader
{
    public:
        spell_highmaul_earthen_flechettes() : SpellScriptLoader("spell_highmaul_earthen_flechettes") { }

        class spell_highmaul_earthen_flechettes_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_earthen_flechettes_SpellScript);

            enum eSpell
            {
                TargetRestrict = 22531
            };

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                SpellTargetRestrictionsEntry const* l_Restriction = sSpellTargetRestrictionsStore.LookupEntry(eSpell::TargetRestrict);
                if (l_Restriction == nullptr)
                    return;

                Unit* l_Caster = GetCaster();
                if (l_Caster == nullptr)
                    return;

                float l_Angle = 2 * M_PI / 360 * l_Restriction->ConeAngle;
                p_Targets.remove_if([l_Caster, l_Angle](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    if (!p_Object->isInFront(l_Caster, l_Angle))
                        return true;

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_earthen_flechettes_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_earthen_flechettes_SpellScript();
        }
};

/// Petrification - 162892
class spell_highmaul_petrification : public SpellScriptLoader
{
    public:
        spell_highmaul_petrification() : SpellScriptLoader("spell_highmaul_petrification") { }

        class spell_highmaul_petrification_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_petrification_AuraScript);

            enum eSpell
            {
                Petrification = 163809
            };

            void OnProc(AuraEffect const* p_AurEff, ProcEventInfo& p_EventInfo)
            {
                PreventDefaultAction();

                if (Unit* l_Target = GetTarget())
                {
                    if (Unit* l_Attacker = p_EventInfo.GetActor())
                    {
                        if (l_Attacker->GetEntry() != eHighmaulCreatures::Tectus)
                            return;

                        l_Attacker->CastSpell(l_Attacker, eSpell::Petrification, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectProc += AuraEffectProcFn(spell_highmaul_petrification_AuraScript::OnProc, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_petrification_AuraScript();
        }
};

/// Raving Assault - 163312
class spell_highmaul_raving_assault : public SpellScriptLoader
{
    public:
        spell_highmaul_raving_assault() : SpellScriptLoader("spell_highmaul_raving_assault") { }

        class spell_highmaul_raving_assault_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_raving_assault_AuraScript);

            enum eSpell
            {
                RavingAssaultDamage = 163318
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
                                l_Caster->CastSpell(l_Iter, eSpell::RavingAssaultDamage, true);
                        }

                        m_DamageTimer = 500;
                    }
                    else
                        m_DamageTimer -= p_Diff;
                }
            }

            void Register() override
            {
                OnAuraUpdate += AuraUpdateFn(spell_highmaul_raving_assault_AuraScript::OnUpdate);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_raving_assault_AuraScript();
        }
};

/// Crystalline Barrage - 162371
class areatrigger_highmaul_crystalline_barrage : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_crystalline_barrage() : AreaTriggerEntityScript("areatrigger_highmaul_crystalline_barrage") { }

        enum eSpell
        {
            CrystallineBarrage = 162370
        };

        std::set<uint64> m_AffectedPlayers;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 2.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                std::set<uint64> l_Targets;

                for (Unit* l_Iter : l_TargetList)
                {
                    l_Targets.insert(l_Iter->GetGUID());

                    if (!l_Iter->HasAura(eSpell::CrystallineBarrage))
                    {
                        m_AffectedPlayers.insert(l_Iter->GetGUID());
                        l_Iter->CastSpell(l_Iter, eSpell::CrystallineBarrage, true);
                    }
                }

                for (std::set<uint64>::iterator l_Iter = m_AffectedPlayers.begin(); l_Iter != m_AffectedPlayers.end();)
                {
                    if (l_Targets.find((*l_Iter)) != l_Targets.end())
                    {
                        ++l_Iter;
                        continue;
                    }

                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, (*l_Iter)))
                    {
                        l_Iter = m_AffectedPlayers.erase(l_Iter);
                        l_Unit->RemoveAura(eSpell::CrystallineBarrage);

                        continue;
                    }

                    ++l_Iter;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                for (uint64 l_Guid : m_AffectedPlayers)
                {
                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, l_Guid))
                        l_Unit->RemoveAura(eSpell::CrystallineBarrage);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_crystalline_barrage();
        }
};

/// Gift of Earth - 162894
class areatrigger_highmaul_gift_of_earth : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_gift_of_earth() : AreaTriggerEntityScript("areatrigger_highmaul_gift_of_earth") { }

        enum eSpells
        {
            Accretion       = 162288,
            Petrification   = 162892
        };

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination) override
        {
            if (p_Caster == nullptr)
                return;

            if (Unit* l_Tectus = p_Caster->FindNearestCreature(eHighmaulCreatures::Tectus, 200.0f))
                p_DestinationPosition = *l_Tectus;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                if (Player* l_Target = p_AreaTrigger->FindNearestPlayer(1.0f))
                {
                    l_Caster->CastSpell(l_Target, eSpells::Petrification, true);
                    p_AreaTrigger->SetDuration(1);
                }
                else if (Creature* l_Tectus = p_AreaTrigger->FindNearestCreature(eHighmaulCreatures::Tectus, 1.0f))
                {
                    p_AreaTrigger->SetDuration(1);

                    if (Aura* l_Accretion = l_Tectus->GetAura(eSpells::Accretion) ? l_Tectus->GetAura(eSpells::Accretion) : l_Tectus->AddAura(eSpells::Accretion, l_Tectus))
                    {
                        l_Accretion->ModStackAmount(10);
                        l_Accretion->RefreshDuration();
                    }
                }
                else if (Creature* l_Tectus = p_AreaTrigger->FindNearestCreature(eHighmaulCreatures::Tectus, 100.0f))
                    p_AreaTrigger->SetDestination(*l_Tectus);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_gift_of_earth();
        }
};

void AddSC_boss_tectus()
{
    /// Boss
    new boss_tectus();

    /// NPCs
    new npc_highmaul_night_twisted_supplicant();
    new npc_highmaul_rokka_and_lokk();
    new npc_highmaul_oro();
    new npc_highmaul_earthen_pillar_stalker();
    new npc_highmaul_night_twisted_berserker();
    new npc_highmaul_night_twisted_earthwarper();

    /// Spells
    new spell_highmaul_meteoric_earthspire();
    new spell_highmaul_stonebolt_volley();
    new spell_highmaul_tectus_energy_gain();
    new spell_highmaul_earthen_pillar_timer();
    new spell_highmaul_accretion();
    new spell_highmaul_tectonic_upheaval();
    new spell_highmaul_spawn_dust_cloud();
    new spell_highmaul_earthen_flechettes();
    new spell_highmaul_petrification();
    new spell_highmaul_raving_assault();

    /// AreaTriggers
    new areatrigger_highmaul_crystalline_barrage();
    new areatrigger_highmaul_gift_of_earth();
}
