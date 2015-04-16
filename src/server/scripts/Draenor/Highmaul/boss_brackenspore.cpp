////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "highmaul.hpp"

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

G3D::Vector3 g_BeachCenter = { 4090.034f, 7793.5166f, 2.974596f };

struct CreepingMossData
{
    CreepingMossData(AreaTrigger* p_AreaTrigger)
    {
        Guid = p_AreaTrigger->GetGUID();
        p_AreaTrigger->GetPosition(&Position);
    }

    uint64 Guid;
    Position Position;
};

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
            Berserker               = 26662,
            Rot                     = 163240,
            RotDot                  = 163241,
            CreepingMossPeriodic    = 163347,
            CreepingMossAreaTrigger = 173229,
            FlamethrowerDespawnAT   = 173281,
            BFC9000                 = 164175,
            FlamethrowerAura        = 163663,
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
            SummonRejuvenatingMush  = 160021
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
            EventSpecialAbility
        };

        enum eActions
        {
            DoIntro,
            CreepingMoss
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
            RejuvenatingMush    = 78868
        };

        enum eTalk
        {
            InfestingSpores
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

            std::map<uint32, CreepingMossData> m_CreepingMoss;

            bool m_IntroDone;

            void Reset() override
            {
                m_Events.Reset();

                _Reset();

                me->RemoveAura(eSpells::Berserker);
                me->RemoveAura(eSpells::CreepingMossPeriodic);

                me->CastSpell(me, eSpells::Rot, true);

                me->RemoveAllAreasTrigger();
                m_CreepingMoss.clear();

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RotDot);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::FlamethrowerAura);
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
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
            }

            void AreaTriggerCreated(AreaTrigger* p_AreaTrigger) override
            {
                if (p_AreaTrigger == nullptr)
                    return;

                if (m_CreepingMoss.find(p_AreaTrigger->GetGUIDLow()) != m_CreepingMoss.end())
                    return;

                m_CreepingMoss.insert(std::make_pair(p_AreaTrigger->GetGUIDLow(), CreepingMossData(p_AreaTrigger)));
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

                        if (!l_Warmasters.empty())
                        {
                            if (Creature* l_IronWar = (*l_Warmasters.begin()))
                                me->Kill(l_IronWar);
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

                            std::list<Creature*> l_BFCs;
                            me->GetCreatureListWithEntryInGrid(l_BFCs, eHighmaulCreatures::BFC9000, 100.0f);

                            for (Creature* l_Creature : l_BFCs)
                                l_Creature->CastSpell(l_Creature, eSpells::BFC9000, true);

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
                                if (Creature* l_Trigger = l_Creature->FindNearestCreature(eCreatures::WorldTrigger, 100.0f))
                                {
                                    l_Creature->InterruptNonMeleeSpells(true);
                                    l_Creature->GetMotionMaster()->MovePoint(0, *l_Trigger);
                                }
                            }
                        });

                        AddTimedDelayedOperation(8 * TimeConstants::IN_MILLISECONDS, [this]() -> void { Reset(); });
                        break;
                    }
                    case eActions::CreepingMoss:
                    {
                        Map* l_Map = me->GetMap();
                        if (l_Map == nullptr)
                            break;

                        float l_MaxRadius = 90.0f;
                        float l_Step = 0.5f;
                        float l_Orientation = frand(0.0f, 2 * M_PI);

                        float l_X = g_CreepingMossPos->x + (l_MaxRadius * cos(l_Orientation));
                        float l_Y = g_CreepingMossPos->y + (l_MaxRadius * sin(l_Orientation));
                        float l_Z = l_Map->GetHeight(l_X, l_Y, MAX_HEIGHT);

                        if (me->IsWithinLOS(l_X, l_Y, l_Z) && CheckCreepingMossPosition(l_X, l_Y))
                        {
                            me->CastSpell(l_X, l_Y, l_Z, eSpells::CreepingMossAreaTrigger, true);
                            break;
                        }

                        uint8 l_MaxCount = 180;
                        for (uint8 l_I = 0; l_I < l_MaxCount; ++l_I)
                        {
                            l_MaxRadius -= l_Step;
                            l_X = g_CreepingMossPos->x + (l_MaxRadius * cos(l_Orientation));
                            l_Y = g_CreepingMossPos->y + (l_MaxRadius * sin(l_Orientation));
                            l_Z = l_Map->GetHeight(l_X, l_Y, MAX_HEIGHT);

                            if (me->IsWithinLOS(l_X, l_Y, l_Z) && CheckCreepingMossPosition(l_X, l_Y))
                            {
                                me->CastSpell(l_X, l_Y, l_Z, eSpells::CreepingMossAreaTrigger, true);
                                break;
                            }
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                _EnterCombat();

                ///m_Events.ScheduleEvent(eEvents::EventNecroticBreath, 30 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventBerserker, 600 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventInfestingSpores, 45 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventMindFungus, 10 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventLivingMushroom, 17 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventSporeShooter, 20 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventFungalFleshEater, 32 * TimeConstants::IN_MILLISECONDS);
                ///m_Events.ScheduleEvent(eEvents::EventRejuvenatingMushroom, 80 * TimeConstants::IN_MILLISECONDS);

                /// Mythic Specials. Shared cd, which special he uses is random.
                ///if (IsMythic())
                    ///m_Events.ScheduleEvent(eEvents::EventSpecialAbility, 20 * TimeConstants::IN_MILLISECONDS);

                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, me, 1);

                /// Spawn timer for Creeping Moss AreaTrigger
                /// 5s for LFR, 2s for Normal mode, 1.85s for Heroic mode
                /// 1.4s for Mythic mode and 1.75s for others
                me->CastSpell(me, eSpells::CreepingMossPeriodic, true);
            }

            void JustDied(Unit* p_Killer) override
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::RotDot);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::FlamethrowerAura);
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                }

                ResetPlayersPower(me);
            }

            void SetGUID(uint64 p_Guid, int32 p_ID) override
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
                        me->SummonCreature(eCreatures::MindFungusFight, *p_Target);
                        break;
                    case eSpells::SummonFungalFleshEater:
                        me->SummonCreature(eCreatures::FungalFleshEater, g_FleshEaterSpawns[urand(1, eHighmaulDatas::MaxFleshEaterPos) - 1]);
                        break;
                    case eSpells::SporeShooterDummy:
                        me->CastSpell(me, eSpells::SummonSporeShooter, true);
                        break;
                    case eSpells::RejuvenatingMushDummy:
                        me->CastSpell(p_Target, eSpells::SummonRejuvenatingMush, true);
                        break;
                    default:
                        break;
                }
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

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventNecroticBreath:
                        me->CastSpell(me, eSpells::SpellNecroticBreath, false);
                        m_Events.ScheduleEvent(eEvents::EventNecroticBreath, 32 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventBerserker:
                        me->CastSpell(me, eSpells::Berserker, true);
                        break;
                    case eEvents::EventInfestingSpores:
                        Talk(eTalk::InfestingSpores);
                        me->CastSpell(me, eSpells::SpellInfestingSpores, false);
                        m_Events.ScheduleEvent(eEvents::EventInfestingSpores, 45 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventMindFungus:
                        me->CastSpell(me, eSpells::SummonMindFungus, true);
                        m_Events.ScheduleEvent(eEvents::EventMindFungus, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventLivingMushroom:
                        me->CastSpell(me, eSpells::SummonLivingMushroom, true);
                        m_Events.ScheduleEvent(eEvents::EventLivingMushroom, 55 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventSporeShooter:
                        me->CastSpell(me, eSpells::SporeShooterDummy, true);
                        m_Events.ScheduleEvent(eEvents::EventSporeShooter, 57 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventFungalFleshEater:
                        me->CastSpell(me, eSpells::SummonFungalFleshEater, true);
                        m_Events.ScheduleEvent(eEvents::EventFungalFleshEater, 120 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventRejuvenatingMushroom:
                        me->CastSpell(me, eSpells::RejuvenatingMushDummy, true);
                        m_Events.ScheduleEvent(eEvents::EventRejuvenatingMushroom, 130 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eEvents::EventSpecialAbility:
                        m_Events.ScheduleEvent(eEvents::EventSpecialAbility, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            bool CheckCreepingMossPosition(float p_X, float p_Y) const
            {
                /// No more than one Creeping Moss every 12 yards
                float l_CheckRange = 12.0f;

                for (auto l_Iter : m_CreepingMoss)
                {
                    if (AreaTrigger* l_AT = AreaTrigger::GetAreaTrigger(*me, l_Iter.second.Guid))
                    {
                        if (l_AT->GetDistance2d(p_X, p_Y) > l_CheckRange)
                            return false;
                    }
                }

                return true;
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

            void JustDied(Unit* p_Killer) override
            {
                me->RemoveAura(eSpells::MindFungusVisual);
                me->RemoveAura(eSpells::MindFungusAura);
                me->RemoveAura(eSpells::MindFungusAT);
                me->DespawnOrUnsummon(500);
            }

            void UpdateAI(uint32 const p_Diff) override { }
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

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventSporeShot, 100);
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
                    m_Events.ScheduleEvent(eEvent::EventSporeShot, 1500);
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
            FleshEater      = 159973,
            Decay           = 160013
        };

        enum eEvent
        {
            EventDecay = 1
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

            void Reset() override
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);

                me->CastSpell(me, eSpells::FleshEater, true);

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
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                m_Events.ScheduleEvent(eEvent::EventDecay, 2 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* p_Killer) override
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

                if (m_Events.ExecuteEvent() == eEvent::EventDecay)
                {
                    me->CastSpell(me, eSpells::Decay, false);
                    m_Events.ScheduleEvent(eEvent::EventDecay, 5 * TimeConstants::IN_MILLISECONDS);
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
            }

            void HealReceived(Unit* p_Healer, uint32& p_Heal) override
            {
                if ((me->GetHealth() + p_Heal) >= me->GetMaxHealth() && !me->HasAura(eSpells::LivingSpores))
                    me->CastSpell(me, eSpells::LivingSpores, true);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
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
            }

            void HealReceived(Unit* p_Healer, uint32& p_Heal) override
            {
                if ((me->GetHealth() + p_Heal) >= me->GetMaxHealth() && !me->HasAura(eSpells::RejuvenatingSpores))
                    me->CastSpell(me, eSpells::RejuvenatingSpores, true);
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_Instance != nullptr)
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new npc_highmaul_rejuvenating_mushroomAI(p_Creature);
        }
};

/// Necrotic Breath - 159220
class spell_highmaul_necrotic_breath : public SpellScriptLoader
{
    public:
        spell_highmaul_necrotic_breath() : SpellScriptLoader("spell_highmaul_necrotic_breath") { }

        class spell_highmaul_necrotic_breath_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_highmaul_necrotic_breath_SpellScript);

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
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_necrotic_breath_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_highmaul_necrotic_breath_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
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
            PrepareAuraScript(spell_highmaul_flamethrower_aura_AuraScript);

            void OnTick(constAuraEffectPtr p_AurEff)
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

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
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
            PrepareSpellScript(spell_highmaul_flamethrower_aura_SpellScript);

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

        SpellScript* GetSpellScript() const
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
            PrepareAuraScript(spell_highmaul_flamethrower_regen_AuraScript);

            void OnTick(constAuraEffectPtr /*p_AurEff*/)
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
            PrepareAuraScript(spell_highmaul_pulsing_heat_AuraScript);

            enum eSpell
            {
                FlamethrowerRegen = 163667
            };

            void OnRemove(constAuraEffectPtr /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
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
            PrepareAuraScript(spell_highmaul_creeping_moss_AuraScript);

            enum eAction
            {
                CreepingMoss = 1
            };

            void OnTick(constAuraEffectPtr /*p_AurEff*/)
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

/// Mind Fungus - 159489
class areatrigger_highmaul_mind_fungus : public AreaTriggerEntityScript
{
    public:
        areatrigger_highmaul_mind_fungus() : AreaTriggerEntityScript("areatrigger_highmaul_mind_fungus") { }

        enum eSpell
        {
            MindFungus = 163140
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                std::list<Unit*> l_TargetList;
                float l_Radius = 13.0f;

                JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
                JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
                p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

                for (Unit* l_Unit : l_TargetList)
                    l_Caster->CastSpell(l_Unit, eSpell::MindFungus, true);
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

        void OnSetCreatePosition(AreaTrigger* p_AreaTrigger, Unit* p_Caster, Position& p_SourcePosition, Position& p_DestinationPosition, std::list<Position>& p_PathToLinearDestination) override
        {
            if (p_Caster == nullptr)
                return;

            if (Unit* l_Target = MS::ScriptUtils::SelectRandomPlayerIncludedTank(p_Caster, 40.0f))
                p_DestinationPosition = *l_Target;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
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

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
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

/// Gorian Strands - 10094
class areatrigger_at_highmaul_infested_waters : public AreaTriggerScript
{
    public:
        areatrigger_at_highmaul_infested_waters() : AreaTriggerScript("areatrigger_at_highmaul_infested_waters") { }

        enum eSpell
        {
            InfestedWaters = 164642
        };

        void OnEnter(Player* p_Player, AreaTriggerEntry const* p_AreaTrigger) override
        {
            p_Player->RemoveAura(eSpell::InfestedWaters);
        }

        void OnExit(Player* p_Player, AreaTriggerEntry const* p_AreaTrigger) override
        {
            if (p_Player->GetPositionZ() < 0.0f)
                p_Player->CastSpell(p_Player, eSpell::InfestedWaters, true);
        }
};

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

    /// Spells
    new spell_highmaul_necrotic_breath();
    new spell_highmaul_flamethrower_aura();
    new spell_highmaul_flamethrower_regen();
    new spell_highmaul_pulsing_heat();
    new spell_highmaul_creeping_moss();

    /// AreaTriggers (Spells)
    new areatrigger_highmaul_mind_fungus();
    new areatrigger_highmaul_spore_shot();

    /// AreaTriggers (Area)
    new areatrigger_at_highmaul_infested_waters();
}