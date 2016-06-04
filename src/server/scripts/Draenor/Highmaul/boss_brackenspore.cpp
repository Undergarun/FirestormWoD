////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# include "highmaul.hpp"

G3D::Vector3 g_CreepingMossPos[eHighmaulDatas::MaxCreepingMoss] =
{
    { 4096.283f, 7719.135f, 0.2535536f },
    { 4107.993f, 7719.417f, 0.2534866f },
    { 4100.202f, 7735.637f, 0.2534722f },
    { 4095.731f, 7745.500f, 0.2534722f },
    { 4105.304f, 7757.403f, 0.2534722f },
    { 4124.274f, 7726.856f, 0.2535520f },
    { 4114.686f, 7764.564f, 0.2534722f },
    { 4128.087f, 7739.741f, 0.2534592f },
    { 4138.525f, 7750.965f, 0.2534567f },
    { 4148.499f, 7756.370f, 0.2527755f },
    { 4111.810f, 7739.060f, 0.2535081f },
    { 4133.647f, 7758.851f, 0.2534722f },
    { 4126.690f, 7767.401f, 0.2534722f },
    { 4139.254f, 7735.781f, 0.2535115f },
    { 4118.909f, 7753.566f, 0.2534722f },
    { 4151.049f, 7743.521f, 0.6013964f }
};

Position const g_FleshEaterSpawns[eHighmaulDatas::MaxFleshEaterPos] =
{
    { 4178.046f, 7791.621f, -0.3537667f, 3.052145f },
    { 4141.948f, 7720.839f, -1.1697650f, 2.001688f }
};

G3D::Vector3 g_BeachCenter = { 4104.36f, 7769.18f, 0.254f };

void ResetPlayersPower(Creature* p_Source)
{
    if (p_Source == nullptr)
        return;

    Map::PlayerList const& l_Players = p_Source->GetMap()->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
            l_Player->SetPower(Powers::POWER_ALTERNATE_POWER, 0);
    }
}

/// Brackenspore <Walker of the Deep> - 78491
class boss_brackenspore : public CreatureScript
{
    public:
        boss_brackenspore() : CreatureScript("boss_brackenspore") { }

        enum eSpells
        {
            /// Misc
            RotDot                  = 163241,
            CreepingMossPeriodic    = 163347,
            CreepingMossAreaTrigger = 173229,
            FlamethrowerDespawnAT   = 173281,
            BFC9000                 = 164175,
            FlamethrowerAura        = 163663,
            BurningInfusion         = 165223,
            EnergyRegen             = 164248,
            /// Necrotic Breath
            SpellNecroticBreath     = 159219,   ///< Triggers 159220 every second
            /// Infesting Spores
            SpellInfestingSpores    = 159996,   ///< Triggers 163242 every second
            /// Mind Fungus
            SummonMindFungus        = 163141,
            /// Fungal Flesh-Eater
            SummonFungalFleshEater  = 163142,
            /// Spore Shooter
            SporeShooterDummy       = 163594,
            SummonSporeShooter      = 160446,
            /// Living Mushroom
            SummonLivingMushroom    = 160022,
            /// Rejuvenating Mushroom
            RejuvenatingMushDummy   = 177820,
            SummonRejuvenatingMush  = 160021,
            /// Loot
            BrackensporeBonus       = 177524,

            /// Mythic mode only
            SpellCallOfTheTides     = 160425,
            CallOfTheTidesSummonAT  = 160413
        };

        enum eEvents
        {
            EventNecroticBreath = 1,
            EventBerserker,
            EventInfestingSpores,
            EventCheckForIntro,
            EventMindFungus,
            EventLivingMushroom,
            EventSporeShooter,
            EventFungalFleshEater,
            EventRejuvenatingMushroom,
            EventSpecialAbility,
            EventScheduleEnergy,
            EventRot
        };

        enum eActions
        {
            DoIntro,
            CreepingMoss,
            InfestingSpores
        };

        enum eCreatures
        {
            /// Cosmetic part
            IronWarmaster       = 86609,
            IronFlameTechnician = 86607,
            MindFungus          = 86611,
            SporeShooter        = 86612,
            WorldTrigger        = 59481,
            /// Fight
            SporeShooterFight   = 79183,
            MindFungusFight     = 79082,
            FungalFleshEater    = 79092,
            LivingMushroom      = 78884,
            RejuvenatingMush    = 78868,
            InvisibleMan        = 64693
        };

        enum eTalk
        {
            WarnInfestingSpores
        };

        struct boss_brackensporeAI : public BossAI
        {
            boss_brackensporeAI(Creature* p_Creature) : BossAI(p_Creature, eHighmaulDatas::BossBrackenspore)
            {
                m_Instance  = p_Creature->GetInstanceScript();
                m_IntroDone = false;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            EventMap m_CosmeticEvent;
            std::list<uint64> m_Creatures;

            std::set<uint64> m_CreepingMoss;

            bool m_IntroDone;

            std::vector<uint64> m_Flamethrowers;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eHighmaulSpells::Berserker);
                me->RemoveAura(eSpells::CreepingMossPeriodic);

                me->SetPower(Powers::POWER_RAGE, 0);
                me->SetMaxPower(Powers::POWER_RAGE, 500);

                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_REGENERATE_POWER);

                me->RemoveAura(eSpells::EnergyRegen);

                me->RemoveAllAreasTrigger();
                m_CreepingMoss.clear();

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RotDot);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::FlamethrowerAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::BurningInfusion);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                ResetPlayersPower(me);

                if (!m_IntroDone)
                {
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 438);

                    AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                    {
                        for (uint8 l_I = 0; l_I < eHighmaulDatas::MaxCreepingMoss; ++l_I)
                            me->CastSpell(g_CreepingMossPos[l_I], eSpells::CreepingMossAreaTrigger, true);
                    });
                }
                else
                {
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

                    if (m_Flamethrowers.empty())
                    {
                        std::list<Creature*> l_BFCs;
                        me->GetCreatureListWithEntryInGrid(l_BFCs, eHighmaulCreatures::BFC9000, 100.0f);

                        for (Creature* l_Creature : l_BFCs)
                        {
                            l_Creature->Respawn(true);
                            l_Creature->CastSpell(l_Creature, eSpells::BFC9000, true);

                            if (l_Creature->AI())
                                l_Creature->AI()->Reset();

                            m_Flamethrowers.push_back(l_Creature->GetGUID());
                        }
                    }
                    else
                    {
                        for (uint64 l_Guid : m_Flamethrowers)
                        {
                            if (Creature* l_Creature = Creature::GetCreature(*me, l_Guid))
                            {
                                l_Creature->Respawn(true);
                                l_Creature->CastSpell(l_Creature, eSpells::BFC9000, true);

                                if (l_Creature->AI())
                                    l_Creature->AI()->Reset();
                            }
                        }
                    }
                }
            }

            bool CanRespawn() override
            {
                return false;
            }

            void AreaTriggerCreated(AreaTrigger* p_AreaTrigger) override
            {
                if (p_AreaTrigger == nullptr)
                    return;

                if (m_CreepingMoss.find(p_AreaTrigger->GetGUID()) != m_CreepingMoss.end())
                    return;

                m_CreepingMoss.insert(p_AreaTrigger->GetGUID());
            }

            void AreaTriggerDespawned(AreaTrigger* p_AreaTrigger) override
            {
                if (p_AreaTrigger == nullptr)
                    return;

                if (m_CreepingMoss.find(p_AreaTrigger->GetGUID()) == m_CreepingMoss.end())
                    return;

                m_CreepingMoss.erase(p_AreaTrigger->GetGUID());
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eActions::DoIntro:
                    {
                        m_IntroDone = true;

                        std::list<Creature*> l_Warmasters;
                        me->GetCreatureListWithEntryInGrid(l_Warmasters, eCreatures::IronWarmaster, 50.0f);

                        if (!l_Warmasters.empty())
                            l_Warmasters.sort(JadeCore::ObjectDistanceOrderPred(me, false));

                        if (Creature* l_IronWar = (*l_Warmasters.begin()))
                        {
                            if (l_IronWar->GetAI())
                                l_IronWar->AI()->DoAction(eActions::DoIntro);

                            l_Warmasters.remove(l_IronWar);
                        }

                        AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                        {
                            std::list<AreaTrigger*> l_AreaTriggers;
                            me->GetAreaTriggerList(l_AreaTriggers, eSpells::CreepingMossAreaTrigger);

                            for (AreaTrigger* l_AT : l_AreaTriggers)
                            {
                                me->CastSpell(*l_AT, eSpells::FlamethrowerDespawnAT, true);
                                l_AT->SetDuration(10);
                            }

                            for (uint64 l_Guid : m_Flamethrowers)
                            {
                                if (Creature* l_Creature = Creature::GetCreature(*me, l_Guid))
                                    l_Creature->CastSpell(l_Creature, eSpells::BFC9000, true);
                            }

                            std::list<Creature*> l_MindFungus;
                            me->GetCreatureListWithEntryInGrid(l_MindFungus, eCreatures::MindFungus, 100.0f);

                            for (Creature* l_Creature : l_MindFungus)
                            {
                                l_Creature->RemoveAllAreasTrigger();
                                l_Creature->DespawnOrUnsummon();
                            }

                            if (Creature* l_Shooter = me->FindNearestCreature(eCreatures::SporeShooter, 100.0f))
                                l_Shooter->DespawnOrUnsummon();

                            std::list<Creature*> l_IronFlames;
                            me->GetCreatureListWithEntryInGrid(l_IronFlames, eCreatures::IronFlameTechnician, 50.0f);

                            for (Creature* l_Creature : l_IronFlames)
                            {
                                if (l_Creature->IsAIEnabled)
                                    l_Creature->AI()->DoAction(eActions::DoIntro);

                                if (Creature* l_Trigger = l_Creature->FindNearestCreature(eCreatures::WorldTrigger, 100.0f))
                                {
                                    l_Trigger->SetReactState(ReactStates::REACT_PASSIVE);
                                    l_Trigger->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);

                                    l_Creature->InterruptNonMeleeSpells(true);
                                    l_Creature->GetMotionMaster()->MovePoint(0, *l_Trigger);
                                }
                            }
                        });

                        std::list<uint64> l_WarMasterGuids;
                        for (Creature* l_Iter : l_Warmasters)
                            l_WarMasterGuids.push_back(l_Iter->GetGUID());

                        AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this, l_WarMasterGuids]() -> void
                        {
                            if (l_WarMasterGuids.empty())
                                return;

                            if (Creature* l_IronWar = Creature::GetCreature(*me, (*l_WarMasterGuids.begin())))
                                me->Kill(l_IronWar);

                            Reset();
                        });

                        break;
                    }
                    case eActions::CreepingMoss:
                    {
                        SummonCreepingMoss();
                        break;
                    }
                    case eActions::InfestingSpores:
                    {
                        m_Events.ScheduleEvent(eEvents::EventInfestingSpores, 1);
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                _EnterCombat();

                m_Events.ScheduleEvent(eEvents::EventNecroticBreath, 30 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventBerserker, 600 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventMindFungus, 10 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventLivingMushroom, 17 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventSporeShooter, 20 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventFungalFleshEater, 32 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRejuvenatingMushroom, 80 * TimeConstants::IN_MILLISECONDS);
                m_Events.ScheduleEvent(eEvents::EventRot, 10 * TimeConstants::IN_MILLISECONDS);

                /// Mythic Specials. Shared cd, which special he uses is random.
                if (IsMythic())
                    m_Events.ScheduleEvent(eEvents::EventSpecialAbility, 20 * TimeConstants::IN_MILLISECONDS);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                /// Spawn timer for Creeping Moss AreaTrigger
                /// 5s for LFR, 2s for Normal mode, 1.85s for Heroic mode
                /// 1.4s for Mythic mode and 1.75s for others
                me->CastSpell(me, eSpells::CreepingMossPeriodic, true);
                me->CastSpell(me, eSpells::EnergyRegen, true);

                std::list<Creature*> l_CosmeticMobs;
                me->GetCreatureListWithEntryInGrid(l_CosmeticMobs, eHighmaulCreatures::BlackrockGrunt, 800.0f);

                for (Creature* l_Grunt : l_CosmeticMobs)
                    l_Grunt->DespawnOrUnsummon();

                l_CosmeticMobs.clear();
                me->GetCreatureListWithEntryInGrid(l_CosmeticMobs, eCreatures::IronFlameTechnician, 80.0f);

                for (Creature* l_Technician : l_CosmeticMobs)
                    l_Technician->DespawnOrUnsummon();

                l_CosmeticMobs.clear();
                me->GetCreatureListWithEntryInGrid(l_CosmeticMobs, eCreatures::IronWarmaster, 80.0f);

                for (Creature* l_Warmaster : l_CosmeticMobs)
                    l_Warmaster->DespawnOrUnsummon();
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RotDot);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::FlamethrowerAura);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::BurningInfusion);

                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                    if (sObjectMgr->IsDisabledEncounter(m_Instance->GetEncounterIDForBoss(me), GetDifficulty()))
                        me->SetLootRecipient(nullptr);
                    else
                        CastSpellToPlayers(me->GetMap(), me, eSpells::BrackensporeBonus, true);
                }

                ResetPlayersPower(me);

                for (uint64 l_Guid : m_Flamethrowers)
                {
                    if (Creature* l_Creature = Creature::GetCreature(*me, l_Guid))
                        l_Creature->RemoveAura(eSpells::BFC9000);
                }

                me->RemoveAllAreasTrigger();
            }

            void EnterEvadeMode() override
            {
                CreatureAI::EnterEvadeMode();

                if (m_Instance != nullptr)
                    m_Instance->SetBossState(eHighmaulDatas::BossBrackenspore, EncounterState::FAIL);
            }

            void SetGUID(uint64 p_Guid, int32 /*p_ID*/) override
            {
                m_Creatures.push_back(p_Guid);
                m_CosmeticEvent.RescheduleEvent(eEvents::EventCheckForIntro, 1000);
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo) override
            {
                if (p_Target == nullptr)
                    return;

                switch (p_SpellInfo->Id)
                {
                    case eSpells::SummonMindFungus:
                        me->SummonCreature(eCreatures::MindFungusFight, *p_Target, TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eSpells::SummonFungalFleshEater:
                        me->SummonCreature(eCreatures::FungalFleshEater, g_FleshEaterSpawns[urand(1, eHighmaulDatas::MaxFleshEaterPos) - 1],
                            TempSummonType::TEMPSUMMON_CORPSE_TIMED_DESPAWN, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eSpells::SporeShooterDummy:
                    {
                        uint8 l_Count = IsMythic() ? 4 : 2;
                        for (uint8 l_I = 0; l_I < l_Count; ++l_I)
                            me->CastSpell(me, eSpells::SummonSporeShooter, true);
                        break;
                    }
                    case eSpells::RejuvenatingMushDummy:
                        me->CastSpell(p_Target, eSpells::SummonRejuvenatingMush, true);
                        break;
                    case eSpells::SpellCallOfTheTides:
                        p_Target->CastSpell(p_Target, eSpells::CallOfTheTidesSummonAT, true, nullptr, nullptr, me->GetGUID());
                        break;
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers /*p_Power*/, int32& p_Value) override
            {
                /// Brackenspore only regens by script
                p_Value = 0;
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                UpdateOperations(p_Diff);

                m_CosmeticEvent.Update(p_Diff);

                if (m_CosmeticEvent.ExecuteEvent())
                {
                    bool l_CanSchedule = true;
                    for (uint64 l_Guid : m_Creatures)
                    {
                        if (Creature* l_Add = Creature::GetCreature(*me, l_Guid))
                        {
                            if (l_Add->isAlive())
                            {
                                l_CanSchedule = false;
                                break;
                            }
                        }
                    }

                    if (l_CanSchedule)
                        DoAction(eActions::DoIntro);
                    else
                        m_CosmeticEvent.ScheduleEvent(eEvents::EventCheckForIntro, 1000);
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                /// Update moves here, avoid some movements problems during Infesting Spores
                if (me->getVictim() && !me->IsWithinMeleeRange(me->getVictim()) && me->HasAura(eSpells::SpellInfestingSpores))
                {
                    Position l_Pos;
                    me->getVictim()->GetPosition(&l_Pos);

                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MovePoint(0, l_Pos);
                }

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventNecroticBreath:
                    {
                        me->CastSpell(me, eSpells::SpellNecroticBreath, false);
                        m_Events.ScheduleEvent(eEvents::EventNecroticBreath, 32 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventBerserker:
                    {
                        me->CastSpell(me, eHighmaulSpells::Berserker, true);
                        break;
                    }
                    case eEvents::EventInfestingSpores:
                    {
                        Talk(eTalk::WarnInfestingSpores);
                        me->RemoveAura(eSpells::EnergyRegen);
                        me->CastSpell(me, eSpells::SpellInfestingSpores, false);
                        m_Events.ScheduleEvent(eEvents::EventScheduleEnergy, 12 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventMindFungus:
                    {
                        me->CastSpell(me, eSpells::SummonMindFungus, true);
                        m_Events.ScheduleEvent(eEvents::EventMindFungus, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventLivingMushroom:
                    {
                        me->CastSpell(me, eSpells::SummonLivingMushroom, true);
                        m_Events.ScheduleEvent(eEvents::EventLivingMushroom, 55 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSporeShooter:
                    {
                        me->CastSpell(me, eSpells::SporeShooterDummy, true);
                        m_Events.ScheduleEvent(eEvents::EventSporeShooter, 57 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventFungalFleshEater:
                    {
                        me->CastSpell(me, eSpells::SummonFungalFleshEater, true);
                        m_Events.ScheduleEvent(eEvents::EventFungalFleshEater, 120 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventRejuvenatingMushroom:
                    {
                        me->CastSpell(me, eSpells::RejuvenatingMushDummy, true);
                        m_Events.ScheduleEvent(eEvents::EventRejuvenatingMushroom, 130 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventSpecialAbility:
                    {
                        DoSpecialAbility();
                        m_Events.ScheduleEvent(eEvents::EventSpecialAbility, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eEvents::EventScheduleEnergy:
                    {
                        me->GetMotionMaster()->Clear();

                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->GetMotionMaster()->MoveChase(l_Target);

                        me->CastSpell(me, eSpells::EnergyRegen, true);
                        break;
                    }
                    case eEvents::EventRot:
                    {
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, eSpells::RotDot, true);
                        m_Events.ScheduleEvent(eEvents::EventRot, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    default:
                        break;
                }

                /// If boss is outside the beach, it should resets
                if (me->GetDistance(g_BeachCenter.x, g_BeachCenter.y, g_BeachCenter.z) >= 100.0f)
                {
                    EnterEvadeMode();
                    return;
                }

                DoMeleeAttackIfReady();
            }

            void SummonCreepingMoss()
            {
                Map* l_Map = me->GetMap();
                if (l_Map == nullptr)
                    return;

                float l_Orientation = frand(0.0f, 2 * M_PI);

                /// Use different spawn radius depending on orientation
                float l_Radius = GetSpawnRangeByOrientation(l_Orientation);

                float l_OStep = 2 * M_PI / 30.0f;
                float l_X = g_BeachCenter.x + (l_Radius * cos(l_Orientation));
                float l_Y = g_BeachCenter.y + (l_Radius * sin(l_Orientation));
                float l_Z = l_Map->GetHeight(l_X, l_Y, MAX_HEIGHT);

                /// First of all, verify if we can spawn an AreaTrigger all around the beach center
                /// With a radius of 60-80 yards, it must cover the beach progressively
                {
                    if (me->IsWithinLOS(l_X, l_Y, l_Z) && CheckCreepingMossPosition(l_X, l_Y))
                    {
                        me->CastSpell(l_X, l_Y, l_Z, eSpells::CreepingMossAreaTrigger, true);
                        return;
                    }

                    for (uint8 l_J = 0; l_J < 30; ++l_J)
                    {
                        l_Orientation = Position::NormalizeOrientation(l_Orientation + l_OStep);
                        l_Radius = GetSpawnRangeByOrientation(l_Orientation);

                        l_X = g_BeachCenter.x + (l_Radius * cos(l_Orientation));
                        l_Y = g_BeachCenter.y + (l_Radius * sin(l_Orientation));
                        l_Z = l_Map->GetHeight(l_X, l_Y, MAX_HEIGHT);

                        if (me->IsWithinLOS(l_X, l_Y, l_Z) && CheckCreepingMossPosition(l_X, l_Y))
                        {
                            me->CastSpell(l_X, l_Y, l_Z, eSpells::CreepingMossAreaTrigger, true);
                            return;
                        }
                    }
                }

                float l_MaxRadius = 80.0f;
                float l_Step = 2.0f;

                /// Secondly, check for each less radius (2 yard step) ...
                for (uint8 l_I = 0; l_I < 45; ++l_I)
                {
                    l_MaxRadius -= l_Step;
                    l_Radius = GetSpawnRangeByOrientation(l_Orientation, l_MaxRadius);

                    l_X = g_BeachCenter.x + (l_Radius * cos(l_Orientation));
                    l_Y = g_BeachCenter.y + (l_Radius * sin(l_Orientation));
                    l_Z = l_Map->GetHeight(l_X, l_Y, MAX_HEIGHT);

                    if (me->IsWithinLOS(l_X, l_Y, l_Z) && CheckCreepingMossPosition(l_X, l_Y))
                    {
                        me->CastSpell(l_X, l_Y, l_Z, eSpells::CreepingMossAreaTrigger, true);
                        return;
                    }

                    /// ... and for each orientation (3.33% step)
                    for (uint8 l_J = 0; l_J < 30; ++l_J)
                    {
                        l_Orientation = Position::NormalizeOrientation(l_Orientation + l_OStep);
                        l_Radius = GetSpawnRangeByOrientation(l_Orientation, l_MaxRadius);

                        l_X = g_BeachCenter.x + (l_Radius * cos(l_Orientation));
                        l_Y = g_BeachCenter.y + (l_Radius * sin(l_Orientation));
                        l_Z = l_Map->GetHeight(l_X, l_Y, MAX_HEIGHT);

                        if (me->IsWithinLOS(l_X, l_Y, l_Z) && CheckCreepingMossPosition(l_X, l_Y))
                        {
                            me->CastSpell(l_X, l_Y, l_Z, eSpells::CreepingMossAreaTrigger, true);
                            return;
                        }
                    }
                }
            }

            bool CheckCreepingMossPosition(float p_X, float p_Y) const
            {
                /// No more than one Creeping Moss every 10 yards
                float l_CheckRange = 10.0f;

                for (uint64 l_Iter : m_CreepingMoss)
                {
                    if (AreaTrigger* l_AT = AreaTrigger::GetAreaTrigger(*me, l_Iter))
                    {
                        if (l_AT->GetDistance2d(p_X, p_Y) < l_CheckRange)
                            return false;
                    }
                }

                return true;
            }

            float GetSpawnRangeByOrientation(float p_Orientation, float p_MaxRadius = 80.0f) const
            {
                if (p_Orientation <= (M_PI / 4.0f))
                    return p_MaxRadius;
                else if (p_Orientation <= (3 * M_PI / 4.0f))
                    return p_MaxRadius - 20.0f;
                else if (p_Orientation <= (5 * M_PI / 4.0f))
                    return p_MaxRadius;
                else if (p_Orientation <= (2 * M_PI - M_PI / 4))
                    return p_MaxRadius - 20.0f;
                else
                    return p_MaxRadius;
            }

            void DoSpecialAbility()
            {
                /// Call of the Tides
                if (urand(0, 1))
                    me->CastSpell(me, eSpells::SpellCallOfTheTides, true);
                /// Exploding Fungus
                else
                {
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new boss_brackensporeAI(p_Creature);
        }
};

/// Mind Fungus (Cosmetic) - 86611
/// Mind Fungus (For fight) - 79082
class npc_highmaul_mind_fungus : public CreatureScript
{
    public:
        npc_highmaul_mind_fungus() : CreatureScript("npc_highmaul_mind_fungus") { }

        enum eSpells
        {
            MindFungusVisual    = 171862,
            MindFungusAura      = 163138,
            MindFungusAT        = 159489
        };

        struct npc_highmaul_mind_fungusAI : public ScriptedAI
        {
            npc_highmaul_mind_fungusAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->CastSpell(me, eSpells::MindFungusVisual, true);
                me->CastSpell(me, eSpells::MindFungusAura, true);
                me->CastSpell(me, eSpells::MindFungusAT, true);

                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->RemoveAura(eSpells::MindFungusVisual);
                me->RemoveAura(eSpells::MindFungusAura);
                me->RemoveAura(eSpells::MindFungusAT);
                me->RemoveAllAreasTrigger();
                me->DespawnOrUnsummon(500);
            }

            void UpdateAI(uint32 const /*p_Diff*/) override { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_mind_fungusAI(p_Creature);
        }
};

/// Spore Shooter (Cosmetic) - 86612
/// Spore Shooter (For fight) - 79183
class npc_highmaul_spore_shooter : public CreatureScript
{
    public:
        npc_highmaul_spore_shooter() : CreatureScript("npc_highmaul_spore_shooter") { }

        enum eSpell
        {
            SporeShot = 173241
        };

        enum eEvent
        {
            EventSporeShot = 1
        };

        struct npc_highmaul_spore_shooterAI : public ScriptedAI
        {
            npc_highmaul_spore_shooterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/) override
            {
                m_Events.ScheduleEvent(eEvent::EventSporeShot, urand(100, 1500));
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                me->DespawnOrUnsummon(20 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                if (m_Events.ExecuteEvent() == eEvent::EventSporeShot)
                {
                    me->CastSpell(me, eSpell::SporeShot, false);
                    m_Events.ScheduleEvent(eEvent::EventSporeShot, 4500);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_spore_shooterAI(p_Creature);
        }
};

/// Fungal Flesh-Eater - 79092
class npc_highmaul_fungal_flesh_eater : public CreatureScript
{
    public:
        npc_highmaul_fungal_flesh_eater() : CreatureScript("npc_highmaul_fungal_flesh_eater") { }

        enum eTalk
        {
            Warn
        };

        enum eSpells
        {
            InfestedWaters  = 164644,
            FleshEater      = 159972,
            Decay           = 160013
        };

        enum eEvents
        {
            EventDecay = 1,
            EventFleshEater
        };

        enum eData
        {
            AnimKit = 1718
        };

        struct npc_highmaul_fungal_flesh_eaterAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_fungal_flesh_eaterAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            bool m_Scheduled;

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                Talk(eTalk::Warn);

                AddTimedDelayedOperation(1 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->SetFloatValue(EObjectFields::OBJECT_FIELD_SCALE, 1.0f);
                    me->SetFloatValue(EUnitFields::UNIT_FIELD_BOUNDING_RADIUS, 0.31f);
                    me->SetFloatValue(EUnitFields::UNIT_FIELD_COMBAT_REACH, 7.0f);
                });

                AddTimedDelayedOperation(5 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->PlayOneShotAnimKit(eData::AnimKit); });
                AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->ClearUnitState(UnitState::UNIT_STATE_ROOT); });

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 2);

                m_Scheduled = false;
            }

            void DamageDealt(Unit* /*p_Victim*/, uint32& /*p_Damage*/, DamageEffectType /*p_DamageType*/) override
            {
                if (!m_Scheduled)
                {
                    m_Scheduled = true;
                    m_Events.ScheduleEvent(eEvents::EventDecay, 5 * TimeConstants::IN_MILLISECONDS);
                    m_Events.ScheduleEvent(eEvents::EventFleshEater, 1 * TimeConstants::IN_MILLISECONDS);
                }
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                MS::AI::CosmeticAI::UpdateAI(p_Diff);

                if (me->GetPositionZ() <= 0.0f && !me->HasAura(eSpells::InfestedWaters))
                    me->CastSpell(me, eSpells::InfestedWaters, true);
                else if (me->GetPositionZ() > 0.0f && me->HasAura(eSpells::InfestedWaters))
                    me->RemoveAura(eSpells::InfestedWaters);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventDecay:
                        me->CastSpell(me, eSpells::Decay, false);
                        m_Events.ScheduleEvent(eEvents::EventDecay, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventFleshEater:
                        me->CastSpell(me, eSpells::FleshEater, true);
                        m_Events.ScheduleEvent(eEvents::EventFleshEater, 10 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_fungal_flesh_eaterAI(p_Creature);
        }
};

/// Living Mushroom - 78884
class npc_highmaul_living_mushroom : public CreatureScript
{
    public:
        npc_highmaul_living_mushroom() : CreatureScript("npc_highmaul_living_mushroom") { }

        enum eSpells
        {
            LivingMushroomVisual    = 159280,
            LivingMushroomL1Visual  = 164245,
            Withering3Percent       = 160399,
            Withering5Percent       = 163113,
            LivingSpores            = 159291
        };

        struct npc_highmaul_living_mushroomAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_living_mushroomAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->DisableHealthRegen();
                me->SetHealth(me->GetMaxHealth() / 2);
                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);

                me->CastSpell(me, eSpells::LivingMushroomVisual, true);
                me->CastSpell(me, eSpells::LivingMushroomL1Visual, true);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 3);

                AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::Withering3Percent, true); });

                AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->RemoveAura(eSpells::Withering3Percent);
                    me->CastSpell(me, eSpells::Withering5Percent, true);
                });

                AddTimedDelayedOperation(16 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::Withering3Percent, true);
                    me->CastSpell(me, eSpells::Withering5Percent, true);
                });

                AddTimedDelayedOperation(30 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->DespawnOrUnsummon(); });
            }

            void HealReceived(Unit* /*p_Healer*/, uint32& p_Heal) override
            {
                if ((me->GetHealth() + p_Heal) >= me->GetMaxHealth() && !me->HasAura(eSpells::LivingSpores))
                    me->CastSpell(me, eSpells::LivingSpores, true);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
            {
                if (p_Attacker != me)
                    p_Damage = 0;
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                me->DespawnOrUnsummon(20 * TimeConstants::IN_MILLISECONDS);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_living_mushroomAI(p_Creature);
        }
};

/// Rejuvenating Mushroom - 78868
class npc_highmaul_rejuvenating_mushroom : public CreatureScript
{
    public:
        npc_highmaul_rejuvenating_mushroom() : CreatureScript("npc_highmaul_rejuvenating_mushroom") { }

        enum eSpells
        {
            RejuvenatingMushroomVisual      = 159253,
            RejuvenatingMushroomL1Visual    = 164246,
            Withering3Percent               = 163122,
            Withering5Percent               = 163124,
            RejuvenatingSpores              = 159292
        };

        struct npc_highmaul_rejuvenating_mushroomAI : public MS::AI::CosmeticAI
        {
            npc_highmaul_rejuvenating_mushroomAI(Creature* p_Creature) : MS::AI::CosmeticAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset() override
            {
                me->DisableHealthRegen();
                me->SetHealth(me->GetMaxHealth() / 2);
                me->AddUnitState(UnitState::UNIT_STATE_STUNNED);

                me->CastSpell(me, eSpells::RejuvenatingMushroomVisual, true);
                me->CastSpell(me, eSpells::RejuvenatingMushroomL1Visual, true);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 4);

                AddTimedDelayedOperation(6 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->CastSpell(me, eSpells::Withering3Percent, true); });

                AddTimedDelayedOperation(11 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->RemoveAura(eSpells::Withering3Percent);
                    me->CastSpell(me, eSpells::Withering5Percent, true);
                });

                AddTimedDelayedOperation(16 * TimeConstants::IN_MILLISECONDS, [this]() -> void
                {
                    me->CastSpell(me, eSpells::Withering3Percent, true);
                    me->CastSpell(me, eSpells::Withering5Percent, true);
                });

                AddTimedDelayedOperation(30 * TimeConstants::IN_MILLISECONDS, [this]() -> void { me->DespawnOrUnsummon(); });
            }

            void HealReceived(Unit* /*p_Healer*/, uint32& p_Heal) override
            {
                if ((me->GetHealth() + p_Heal) >= me->GetMaxHealth() && !me->HasAura(eSpells::RejuvenatingSpores))
                    me->CastSpell(me, eSpells::RejuvenatingSpores, true);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* /*p_SpellInfo*/) override
            {
                if (p_Attacker != me)
                    p_Damage = 0;
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);

                me->DespawnOrUnsummon(20 * TimeConstants::IN_MILLISECONDS);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rejuvenating_mushroomAI(p_Creature);
        }
};

/// BFC9000 - 81403
class npc_highmaul_bfc9000 : public CreatureScript
{
    public:
        npc_highmaul_bfc9000() : CreatureScript("npc_highmaul_bfc9000") { }

        enum eSpells
        {
            Flamethrower    = 163663,
            BFC9000         = 164175
        };

        struct npc_highmaul_bfc9000AI : public ScriptedAI
        {
            npc_highmaul_bfc9000AI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset() override
            {
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                me->SetFlag(EUnitFields::UNIT_FIELD_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_SPELLCLICK);

                me->SetReactState(ReactStates::REACT_PASSIVE);
            }

            void OnSpellClick(Unit* p_Clicker) override
            {
                if (!me->HasAura(eSpells::BFC9000) || p_Clicker->HasAura(eSpells::Flamethrower))
                    return;

                p_Clicker->CastSpell(p_Clicker, eSpells::Flamethrower, true, nullptr, nullptr, me->GetGUID());
                me->RemoveAura(eSpells::BFC9000);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_bfc9000AI(p_Creature);
        }
};

/// Necrotic Breath - 159220
class spell_highmaul_necrotic_breath : public SpellScriptLoader
{
    public:
        spell_highmaul_necrotic_breath() : SpellScriptLoader("spell_highmaul_necrotic_breath") { }

        class spell_highmaul_necrotic_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_necrotic_breath_SpellScript)

            enum eSpell
            {
                TargetRestrict = 20036
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
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_necrotic_breath_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_24);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_necrotic_breath_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_24);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_necrotic_breath_SpellScript();
        }
};

/// Flamethrower (aura) - 163322
class spell_highmaul_flamethrower_aura : public SpellScriptLoader
{
    public:
        spell_highmaul_flamethrower_aura() : SpellScriptLoader("spell_highmaul_flamethrower_aura") { }

        enum eSpells
        {
            FlamethrowerRegen   = 163667,
            PulsingHeat         = 163666
        };

        class spell_highmaul_flamethrower_aura_AuraScript: public AuraScript
        {
            PrepareAuraScript(spell_highmaul_flamethrower_aura_AuraScript)

            void OnTick(AuraEffect const* p_AurEff)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->GetPower(Powers::POWER_ALTERNATE_POWER) >= 100)
                    {
                        l_Target->CastSpell(l_Target, eSpells::PulsingHeat, true);
                        p_AurEff->GetBase()->Remove(AuraRemoveMode::AURA_REMOVE_BY_CANCEL);
                    }
                }
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode == AuraRemoveMode::AURA_REMOVE_BY_CANCEL)
                    return;

                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpells::FlamethrowerRegen, true);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_flamethrower_aura_AuraScript::OnTick, EFFECT_2, SPELL_AURA_OBS_MOD_POWER);
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_flamethrower_aura_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_PACIFY_SILENCE, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_flamethrower_aura_AuraScript();
        }

        class spell_highmaul_flamethrower_aura_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_flamethrower_aura_SpellScript)

            SpellCastResult CheckPulsingHeat()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(eSpells::PulsingHeat))
                    {
                        SetCustomCastResultMessage(SpellCustomErrors::SPELL_CUSTOM_ERROR_WEAPON_OVERHEATED);
                        return SpellCastResult::SPELL_FAILED_CUSTOM_ERROR;
                    }
                    else if (l_Caster->HasAura(GetSpellInfo()->Id))
                    {
                        l_Caster->RemoveAura(GetSpellInfo()->Id);
                        return SpellCastResult::SPELL_FAILED_DONT_REPORT;
                    }
                }

                return SpellCastResult::SPELL_CAST_OK;
            }

            void Register() override
            {
                OnCheckCast += SpellCheckCastFn(spell_highmaul_flamethrower_aura_SpellScript::CheckPulsingHeat);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_highmaul_flamethrower_aura_SpellScript();
        }
};

/// Flamethrower "Regen" - 163667
class spell_highmaul_flamethrower_regen : public SpellScriptLoader
{
    public:
        spell_highmaul_flamethrower_regen() : SpellScriptLoader("spell_highmaul_flamethrower_regen") { }

        class spell_highmaul_flamethrower_regen_AuraScript: public AuraScript
        {
            PrepareAuraScript(spell_highmaul_flamethrower_regen_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->ModifyPower(Powers::POWER_ALTERNATE_POWER, -2);
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_flamethrower_regen_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_flamethrower_regen_AuraScript();
        }
};

/// Pulsing Heat - 163666
class spell_highmaul_pulsing_heat : public SpellScriptLoader
{
    public:
        spell_highmaul_pulsing_heat() : SpellScriptLoader("spell_highmaul_pulsing_heat") { }

        class spell_highmaul_pulsing_heat_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_pulsing_heat_AuraScript)

            enum eSpell
            {
                FlamethrowerRegen = 163667
            };

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                    l_Target->CastSpell(l_Target, eSpell::FlamethrowerRegen, true);
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_pulsing_heat_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_pulsing_heat_AuraScript();
        }
};

/// Creeping Moss 1 - 163347
class spell_highmaul_creeping_moss : public SpellScriptLoader
{
    public:
        spell_highmaul_creeping_moss() : SpellScriptLoader("spell_highmaul_creeping_moss") { }

        class spell_highmaul_creeping_moss_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_creeping_moss_AuraScript)

            enum eAction
            {
                CreepingMoss = 1
            };

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (GetTarget() == nullptr)
                    return;

                if (Creature* l_Target = GetTarget()->ToCreature())
                {
                    if (l_Target->GetAI())
                        l_Target->AI()->DoAction(eAction::CreepingMoss);
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_creeping_moss_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_creeping_moss_AuraScript();
        }
};

/// Flamethrower - 163310
class spell_highmaul_flamethrower : public SpellScriptLoader
{
    public:
        spell_highmaul_flamethrower() : SpellScriptLoader("spell_highmaul_flamethrower") { }

        enum eSpells
        {
            CreepingMoss    = 173229,
            BurningInfusion = 165223,
            Flamethrower    = 173281
        };

        class spell_highmaul_flamethrower_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_flamethrower_SpellScript)

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<AreaTrigger*> l_CreepingMoss;

                    l_Caster->GetAreaTriggerListWithSpellIDInRange(l_CreepingMoss, eSpells::CreepingMoss, 15.0f);

                    if (l_CreepingMoss.empty())
                        return;

                    l_CreepingMoss.remove_if([this, l_Caster](AreaTrigger* p_AreaTrigger) -> bool
                    {
                        if (p_AreaTrigger == nullptr)
                            return true;

                        Position l_Pos = *p_AreaTrigger;
                        if (l_Pos.GetExactDist(l_Caster) > 12.0f)
                            return true;

                        if (!l_Caster->isInFront(p_AreaTrigger))
                            return true;

                        return false;
                    });

                    for (AreaTrigger* l_AT : l_CreepingMoss)
                    {
                        l_AT->SetDuration(1);
                        l_Caster->CastSpell(l_Caster, eSpells::BurningInfusion, true);
                        l_Caster->CastSpell(*l_AT, eSpells::Flamethrower, true);

                        if (Aura* l_Infusion = l_Caster->GetAura(eSpells::BurningInfusion))
                        {
                            if (l_Infusion->GetStackAmount() >= eHighmaulDatas::BurningInfusionNeeded)
                            {
                                if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                                    l_Instance->SetData(eHighmaulDatas::BrackensporeAchievement, 1);
                            }
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectHit += SpellEffectFn(spell_highmaul_flamethrower_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_flamethrower_SpellScript();
        }
};

/// Burning Infusion - 165223
class spell_highmaul_burning_infusion : public SpellScriptLoader
{
    public:
        spell_highmaul_burning_infusion() : SpellScriptLoader("spell_highmaul_burning_infusion") { }

        class spell_highmaul_burning_infusion_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_burning_infusion_AuraScript)

            void OnTick(AuraEffect const* p_AurEff)
            {
                p_AurEff->GetBase()->DropStack();
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_burning_infusion_AuraScript::OnTick, EFFECT_3, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_burning_infusion_AuraScript();
        }
};

/// Energy Regen - 164248
class spell_highmaul_energy_regen : public SpellScriptLoader
{
    public:
        spell_highmaul_energy_regen() : SpellScriptLoader("spell_highmaul_energy_regen") { }

        enum eAction
        {
            InfestingSpores = 2
        };

        class spell_highmaul_energy_regen_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_highmaul_energy_regen_AuraScript)

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    l_Target->EnergizeBySpell(l_Target, GetSpellInfo()->Id, 10, Powers::POWER_RAGE);

                    if (Creature* l_Boss = l_Target->ToCreature())
                    {
                        if (l_Boss->IsAIEnabled && l_Boss->GetPowerPct(Powers::POWER_RAGE) >= 100.0f)
                            l_Boss->AI()->DoAction(eAction::InfestingSpores);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_highmaul_energy_regen_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_energy_regen_AuraScript();
        }
};

/// Spore Shot - 173244
class spell_highmaul_spore_shot : public SpellScriptLoader
{
    public:
        spell_highmaul_spore_shot() : SpellScriptLoader("spell_highmaul_spore_shot") { }

        class spell_highmaul_spore_shot_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_spore_shot_SpellScript)

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (p_Object == nullptr)
                        return true;

                    /// Should always hit a player if possible
                    if (p_Object->GetTypeId() == TypeID::TYPEID_PLAYER)
                        return false;
                    else if (Creature* l_Creature = p_Object->ToCreature())
                    {
                        if (l_Creature->GetOwner() == nullptr)
                            return true;
                    }

                    return false;
                });
            }

            void Register() override
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_spore_shot_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_highmaul_spore_shot_SpellScript();
        }
};

/// Flamethrower (overrider) - 163663
class spell_highmaul_flamethrower_overrider : public SpellScriptLoader
{
    public:
        spell_highmaul_flamethrower_overrider() : SpellScriptLoader("spell_highmaul_flamethrower_overrider") { }

        enum eSpell
        {
            BFC9000 = 164175
        };

        class spell_highmaul_flamethrower_overrider_AuraScript: public AuraScript
        {
            PrepareAuraScript(spell_highmaul_flamethrower_overrider_AuraScript)

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AuraRemoveMode::AURA_REMOVE_BY_DEATH || GetCaster() == nullptr)
                    return;

                if (Creature* l_BFC9000 = GetCaster()->ToCreature())
                {
                    l_BFC9000->Respawn(true);
                    l_BFC9000->CastSpell(l_BFC9000, eSpell::BFC9000, true);

                    if (l_BFC9000->IsAIEnabled)
                        l_BFC9000->AI()->Reset();
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_highmaul_flamethrower_overrider_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_ENABLE_ALT_POWER, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new spell_highmaul_flamethrower_overrider_AuraScript();
        }
};

/// Mind Fungus - 159489
class areatrigger_highmaul_mind_fungus : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_mind_fungus() : AreaTriggerEntityScript("areatrigger_highmaul_mind_fungus") { }

        enum eSpell
        {
            MindFungus = 163140
        };

        std::set<uint64> m_AffectedPlayers;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 13.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                std::set<uint64> l_Targets;

                for (Unit* l_Iter : l_TargetList)
                {
                    l_Targets.insert(l_Iter->GetGUID());

                    if (!l_Iter->HasAura(eSpell::MindFungus, l_Caster->GetGUID()))
                    {
                        m_AffectedPlayers.insert(l_Iter->GetGUID());
                        l_Iter->CastSpell(l_Iter, eSpell::MindFungus, true);
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
                        l_Unit->RemoveAura(eSpell::MindFungus, l_Caster->GetGUID());

                        continue;
                    }

                    ++l_Iter;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                for (uint64 l_Guid : m_AffectedPlayers)
                {
                    if (Unit* l_Unit = Unit::GetUnit(*l_Caster, l_Guid))
                        l_Unit->RemoveAura(eSpell::MindFungus, l_Caster->GetGUID());
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_mind_fungus();
        }
};

/// Spore Shot - 173241
class areatrigger_highmaul_spore_shot : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_spore_shot() : AreaTriggerEntityScript("areatrigger_highmaul_spore_shot")
        {
            m_Casted = false;
        }

        enum eSpell
        {
            SporeShotDamage = 173244
        };

        bool m_Casted;

        void OnSetCreatePosition(AreaTrigger* /*p_AreaTrigger*/, Unit* p_Caster, Position& /*p_SourcePosition*/, Position& p_DestinationPosition, std::list<Position>& /*p_PathToLinearDestination*/) override
        {
            if (p_Caster == nullptr)
                return;

            if (Unit* l_Target = MS::ScriptUtils::SelectRandomPlayerIncludedTank(p_Caster, 40.0f))
                p_DestinationPosition = *l_Target;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                if (Player* l_Target = p_AreaTrigger->FindNearestPlayer(1.0f))
                {
                    l_Caster->CastSpell(*p_AreaTrigger, eSpell::SporeShotDamage, true);
                    p_AreaTrigger->SetDuration(1);
                    m_Casted = true;
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                if (!m_Casted)
                    l_Caster->CastSpell(*p_AreaTrigger, eSpell::SporeShotDamage, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_spore_shot();
        }
};

/// Creeping Moss - 173229
class areatrigger_highmaul_creeping_moss : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_creeping_moss() : AreaTriggerEntityScript("areatrigger_highmaul_creeping_moss") { }

        enum eSpell
        {
            CreepingMossDamage  = 163590,
            CreepingMossHealing = 165494
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                if (!l_Caster->isInCombat())
                    return;

                std::list<Unit*> l_TargetList;
                float l_Radius = 6.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_CheckEnnemy(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_SearcherEnnemy(p_AreaTrigger, l_TargetList, l_CheckEnnemy);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_SearcherEnnemy);

                if (!l_TargetList.empty())
                {
                    l_TargetList.remove_if([this](Unit* p_Unit) -> bool
                    {
                        if (p_Unit == nullptr || (!p_Unit->IsPlayer() && !p_Unit->GetOwner()))
                            return true;

                        return false;
                    });
                }

                for (Unit* l_Unit : l_TargetList)
                    l_Unit->CastSpell(l_Unit, eSpell::CreepingMossDamage, true);

                l_TargetList.clear();

                JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                if (!l_TargetList.empty())
                {
                    l_TargetList.remove_if([this](Unit* p_Unit) -> bool
                    {
                        if (p_Unit == nullptr)
                            return true;

                        if (p_Unit->HasFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC) ||
                            p_Unit->HasFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC))
                            return true;

                        return false;
                    });
                }

                for (Unit* l_Unit : l_TargetList)
                    l_Unit->CastSpell(l_Unit, eSpell::CreepingMossHealing, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_creeping_moss();
        }
};

/// Call of the Tides - 160413
class areatrigger_highmaul_call_of_the_tides : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_call_of_the_tides() : AreaTriggerEntityScript("areatrigger_highmaul_call_of_the_tides") { }

        enum eSpell
        {
            CallOfTheTidesDamage = 163798
        };

        void OnSetCreatePosition(AreaTrigger* /*p_AreaTrigger*/, Unit* p_Caster, Position& /*p_SourcePosition*/, Position& p_DestinationPosition, std::list<Position>& /*p_PathToLinearDestination*/) override
        {
            if (p_Caster == nullptr)
                return;

            float l_Orientation = p_Caster->GetOrientation();
            float l_X = p_Caster->GetPositionX() + (220.0f * cos(l_Orientation));
            float l_Y = p_Caster->GetPositionY() + (220.0f * sin(l_Orientation));
            float l_Z = p_Caster->GetMap()->GetHeight(l_X, l_Y, MAX_HEIGHT);

            p_DestinationPosition.m_positionX = l_X;
            p_DestinationPosition.m_positionY = l_Y;
            p_DestinationPosition.m_positionZ = l_Z;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
            }
        }

        AreaTriggerEntityScript* GetAI() const override
        {
            return new areatrigger_highmaul_call_of_the_tides();
        }
};

/// Gorian Strands - 10094
class areatrigger_at_highmaul_infested_waters : public AreaTriggerScript
{
    public:
        areatrigger_at_highmaul_infested_waters() : AreaTriggerScript("areatrigger_at_highmaul_infested_waters") { }

        enum eSpell
        {
            InfestedWaters = 164642
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* /*p_AreaTrigger*/) override
        {
            p_Player->RemoveAura(eSpell::InfestedWaters);
        }

        void OnExit(Player* p_Player, AreaTriggerEntry const* /*p_AreaTrigger*/) override
        {
            if (p_Player->GetPositionZ() < 0.0f)
                p_Player->CastSpell(p_Player, eSpell::InfestedWaters, true);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_brackenspore()
{
    /// Boss
    new boss_brackenspore();

    /// NPCs
    new npc_highmaul_mind_fungus();
    new npc_highmaul_spore_shooter();
    new npc_highmaul_fungal_flesh_eater();
    new npc_highmaul_living_mushroom();
    new npc_highmaul_rejuvenating_mushroom();
    new npc_highmaul_bfc9000();

    /// Spells
    new spell_highmaul_necrotic_breath();
    new spell_highmaul_flamethrower_aura();
    new spell_highmaul_flamethrower_regen();
    new spell_highmaul_pulsing_heat();
    new spell_highmaul_creeping_moss();
    new spell_highmaul_flamethrower();
    new spell_highmaul_burning_infusion();
    new spell_highmaul_energy_regen();
    new spell_highmaul_spore_shot();
    new spell_highmaul_flamethrower_overrider();

    /// AreaTriggers (Spells)
    new areatrigger_highmaul_mind_fungus();
    new areatrigger_highmaul_spore_shot();
    new areatrigger_highmaul_creeping_moss();
    new areatrigger_highmaul_call_of_the_tides();

    /// AreaTriggers (Area)
    new areatrigger_at_highmaul_infested_waters();
}
#endif
