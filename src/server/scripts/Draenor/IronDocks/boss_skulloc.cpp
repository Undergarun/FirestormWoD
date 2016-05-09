////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eCreatures
{
    CreatureBackdraft = 432551,
    CreatureCannon    = 84030
};

enum eTalks
{
    TalkKoramar12       = 50,         ///< How dare you marching into my docks, and so bravery set foot onto my ship.. nothing will stop the Iron Horde least of all you.. enjoy your death weaklings. (46911)
    TalkKoramar13       = 51,         ///< Zoggosh.. ready the ready the rocks! This.. Ends... Now! (46912)
    TalkZoggosh03       = 11,         ///< Yes sir.. (44049)
    TalkKoramarVengence = 60,

    /// Bombardmenet Bridge Event
    TalkZoggoshBombardmenet01 = 12,   ///< Sir.. they're messing with the Iron Stars (44050)
    TalkKoramarBombardmenet01 = 25,   ///< Hah! will you look on that, maybe I understimated them.. Zoggosh, prepare Skuloc let them have a taste of what he has to offer. (46901)
    TalkZoggoshBombardmenet02 = 13,   ///< Igniting the cannon sir! (44051)

    /// Past Bombardmement Bridge Event (Upon Grimrail Encounter)
    TalkKoramarPastBombardment01 = 27, ///< What is wrong with you?! keep firing you all! (46904)
    TalkZoggoshPastBombardment01 = 14, ///< Sir.. he's out of ammunition. I'm reloading.. I'm reloading!! (44052)
    TalkKoramarPastBombardment02 = 28  ///< Arghh.. forget it, they'll never make it pass the tracks.. (unknown message) (46905)
};

enum eMovementInformed
{
    MovementInformedKoramarJump = 1,
    MovementInformedKoramarBladestorm
};

Position const g_BackdraftNpcPos = { 6859.93f, -989.91f, 23.054f, 3.000260f };

Position const g_HomeSkullcHomePos = { 6729.220f, -977.335f, 23.046f, 6.230830f};

Position const g_BladestormPositions[8] =
{
    {6730.51f, -986.184f, 23.046f, 0.0f },
    {6770.27f, -999.860f, 23.047f, 0.0f },
    {6740.20f, -1002.999f, 23.046f, 0.0f },
    {6783.170f, -967.849f, 23.180f, 0.0f },
    {6807.46f, -1001.870f, 23.048f, 0.0f },
    {6785.44f, -1001.138f, 23.046f, 0.0f },
    {6839.651f, -968.411f, 23.046f, 0.0f },
    {6757.62f, -986.855f,  22.815f, 0.0f }
};

class basicevent_pre_boss : public BasicEvent
{
public:

    explicit basicevent_pre_boss(Unit* p_Unit, int p_Value) : m_Obj(p_Unit), m_Modifier(p_Value) { }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (m_Obj)
        {
            if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
            {
                if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                {
                    if (l_Koramar->IsAIEnabled)
                    {
                        switch (m_Modifier)
                        {
                        case 0:
                            l_Koramar->AI()->Talk(eTalks::TalkKoramar12);
                            l_Koramar->m_Events.AddEvent(new basicevent_pre_boss(l_Koramar, 1), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        case 1:
                            l_Koramar->AI()->Talk(eTalks::TalkKoramar13);
                            l_Koramar->m_Events.AddEvent(new basicevent_pre_boss(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        case 2:
                            l_Koramar->AI()->Talk(eTalks::TalkZoggosh03);
                            break;
                        default:
                            break;
                        }
                    }
                }
            }
        }

        return true;
    }

private:

    Unit* m_Obj;
    int m_Modifier;
};

class basicevent_bombardment_bridge : public BasicEvent
{
public:
    explicit basicevent_bombardment_bridge(Unit* unit, int value) : m_Obj(unit), m_Modifier(value) { }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
   
        if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
        {
            if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
            {
                if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                {
                    if (Creature* l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                    {
                        if (l_Koramar->IsAIEnabled && l_Skulloc->IsAIEnabled && l_Zoggosh->IsAIEnabled)
                        {
                            switch (m_Modifier)
                            {
                                case 0:
                                {
                                    l_Zoggosh->AI()->Talk(eTalks::TalkZoggoshBombardmenet01);
                                    l_Zoggosh->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Zoggosh, 1), l_Zoggosh->m_Events.CalculateTime(7 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 1:
                                {
                                    l_Koramar->AI()->Talk(eTalks::TalkKoramarBombardmenet01);
                                    l_Koramar->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 2:
                                {
                                    l_Zoggosh->AI()->Talk(eTalks::TalkZoggoshBombardmenet02);
                                    l_Skulloc->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Skulloc, 3), l_Skulloc->m_Events.CalculateTime(10 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 3:
                                {
                                    l_Skulloc->AI()->DoAction(eIronDocksActions::ActionBombardBridge);
                                    l_Skulloc->m_Events.AddEvent(new basicevent_bombardment_bridge(l_Skulloc, 3), l_Skulloc->m_Events.CalculateTime(12 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    Unit* m_Obj;
    int m_Modifier;
};

class basicevent_past_bombardment_bridge : public BasicEvent
{
public:
    explicit basicevent_past_bombardment_bridge(Unit* unit, int value) : m_Obj(unit), m_Modifier(value) { }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (InstanceScript* l_Instance = m_Obj->GetInstanceScript())
        {
            if (Creature* l_Zoggosh = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
            {
                if (Creature* l_Koramar = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataKoramar)))
                {
                    if (Creature* l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                    {
                        if (l_Koramar->IsAIEnabled && l_Skulloc->IsAIEnabled && l_Zoggosh->IsAIEnabled)
                        {
                            switch (m_Modifier)
                            {
                                case 0:
                                {
                                    l_Koramar->AI()->Talk(eTalks::TalkKoramarBombardmenet01);
                                    l_Zoggosh->m_Events.AddEvent(new basicevent_past_bombardment_bridge(l_Zoggosh, 1), l_Zoggosh->m_Events.CalculateTime(9 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 1:
                                {
                                    l_Zoggosh->AI()->Talk(eTalks::TalkZoggoshPastBombardment01);
                                    l_Koramar->m_Events.AddEvent(new basicevent_past_bombardment_bridge(l_Koramar, 2), l_Koramar->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                                    break;
                                }
                                case 2:
                                {
                                    l_Koramar->AI()->Talk(eTalks::TalkKoramarPastBombardment02);
                                    break;
                                }          
                                default:
                                    break;
                            }
                        }
                    }
                }
            }
        }

        return true;
    }

private:
    Unit* m_Obj;
    int m_Modifier;
};

static void SkullocEnforcersStart(InstanceScript* p_Instance, Creature* /*p_Me*/)
{
    if (p_Instance == nullptr)
        return;

    if (Creature* l_Skulloc = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
        l_Skulloc->SetInCombatWithZone();
    if (Creature* l_Koramar = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataKoramar)))
        l_Koramar->SetInCombatWithZone();
    if (Creature* l_Zoggosh = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
        l_Zoggosh->SetInCombatWithZone();
}

static void SkullocEnforcersWipingCondition(InstanceScript* p_Instance, Creature* p_Me)
{
    if (p_Instance == nullptr)
        return;

    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);


    if (Creature* l_Skulloc = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
    {
        if (Creature* l_Koramar = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataKoramar)))
        {
            if (Creature* l_Zoggosh = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
            {
                l_Zoggosh->Respawn();
                l_Koramar->Respawn();
                l_Skulloc->Respawn();
                l_Zoggosh->GetMotionMaster()->MoveTargetedHome();
                l_Koramar->GetMotionMaster()->MoveTargetedHome();
                l_Skulloc->GetMotionMaster()->MoveTargetedHome();
            }
        }
    }
}

static void SkullocEnforcersWiningCondition(InstanceScript* p_Instance, Creature* p_Me, uint64 p_KillerGuid)
{
    if (p_Instance == nullptr)
        return;

    if (p_KillerGuid == 0)
        return;

    p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, p_Me);

    if (Creature* l_Skulloc = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
    {
        if (Creature* l_Koramar = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataKoramar)))
        {
            if (Creature* l_Zoggosh = p_Instance->instance->GetCreature(p_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
            {
                if (l_Skulloc->isDead() && l_Koramar->isDead() && l_Zoggosh->isDead())
                {
                    if (Player* l_Player = Player::GetPlayer(*p_Me, p_KillerGuid))
                    {
                        l_Skulloc->SetLootRecipient(l_Player);
                        p_Instance->SetBossState(eIronDocksDatas::DataSkulloc, EncounterState::DONE);
                    }
                }
            }
        }
    }
}

/// Skulloc <Son of Gruul> - 83612
class boss_skulloc : public CreatureScript
{
    public:

        boss_skulloc() : CreatureScript("boss_skulloc") { }

        struct boss_skullocAI : public BossAI
        {
            boss_skullocAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataSkulloc), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = me->GetInstanceScript();
                m_BombardmentPosition = 0;
                m_First = false;
                m_GrimrailEvent = false;
                m_BridgeBombardmenet = false;
            }

            enum eSkullocSpells
            {
                SpellCannonBarrageAura            = 168537,
                SpellCannonBarrageMissileOriginal = 168539,
                SpellGronnSmash                   = 168227,
                SpellBackdraft                    = 169132,
                SpellRideVehicle                  = 164314
            };

            enum eSkullocEvents
            {
                EventBackdraft = 1,
                EventCannonBarrage,
                EventCannonBarragePre,
                EventCannonBarrageCancel
            };

            enum eSkullocMovementInformed
            {
                MovementInformedBeginBarrage = 1
            };

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;
            uint8 m_BombardmentPosition;
            bool m_First;
            bool m_GrimrailEvent;
            bool m_BombardmentReady;
            bool m_BridgeBombardmenet;

            void Reset() override
            {
                _Reset();
                events.Reset();       
                if (me->GetMap())
                {
                    me->GetMap()->SetObjectVisibility(5000.0f);
                }
                m_BombardmentReady = false;
                m_Vehicle->InstallAllAccessories(false);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 20.0f) && !m_BombardmentReady)
                {
                    m_BombardmentReady = true;
                    me->m_Events.AddEvent(new basicevent_pre_boss(me, 0), me->m_Events.CalculateTime(8 * TimeConstants::IN_MILLISECONDS));
                }
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {            
                events.ScheduleEvent(eSkullocEvents::EventCannonBarrage, 50 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eSkullocEvents::EventBackdraft, 20 * TimeConstants::IN_MILLISECONDS);
                if (m_Instance != nullptr)
                {
                    SkullocEnforcersStart(m_Instance, me);
                    if (Creature* l_Zoggosh = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                    {
                        if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                        {
                            if (l_Turret->IsAIEnabled && l_Zoggosh->IsAIEnabled)
                            {
                                l_Zoggosh->CastSpell(l_Turret, eSkullocSpells::SpellRideVehicle);

                                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                    l_Turret->AI()->AttackStart(l_Target);
                            }
                        }
                    }
                }
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eIronDocksActions::ActionActivateGrimrailEvent:
                        if (!m_GrimrailEvent)
                        {
                            m_GrimrailEvent = true;
                            me->m_Events.KillAllEvents(true); ///< Stops bombardment event (Bridge)
                            me->m_Events.AddEvent(new basicevent_past_bombardment_bridge(me, 0), me->m_Events.CalculateTime(1 * TimeConstants::IN_MILLISECONDS));
                            me->SummonCreature(eIronDocksCreatures::CreatureTrain, g_TrainSpawnPos, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                        }
                        break;
                    case eIronDocksActions::ActionActivateBridgeBombardmement:
                        if (!m_BridgeBombardmenet)
                        {
                            m_BridgeBombardmenet = true;
                            me->m_Events.AddEvent(new basicevent_bombardment_bridge(me, 0), me->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                        }
                        break;
                    case eIronDocksActions::ActionBombardBridge:
                        if (Unit* l_Unit = m_Vehicle->GetPassenger(0))
                        {
                            l_Unit->CastSpell(l_Unit, eSkullocSpells::SpellCannonBarrageAura);
                        }
                        else if (m_Vehicle->GetPassenger(0) == nullptr)
                        {
                            m_Vehicle->InstallAccessory(eCreatures::CreatureCannon, 0, 1, 6, 30000);
                        }
                        break;
                    default:
                        break;
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                if (p_Id == eSkullocMovementInformed::MovementInformedBeginBarrage)
                {
                    me->CastSpell(me, eSkullocSpells::SpellGronnSmash);
                    events.ScheduleEvent(eSkullocEvents::EventCannonBarrageCancel, 8 * TimeConstants::IN_MILLISECONDS);
                }
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                    SkullocEnforcersWipingCondition(m_Instance, me);
                }

                summons.DespawnAll();
            }

            void JustDied(Unit* p_Killer) override
            {
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                    SkullocEnforcersWiningCondition(m_Instance, me, p_Killer->GetGUID());
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eSkullocEvents::EventCannonBarrage:
                    {
                        m_BombardmentReady = true;
                        me->SetReactState(ReactStates::REACT_PASSIVE);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        me->MonsterTextEmote("Skulloc prepares a |cFFFF0404|Hspell:164275|h[Cannon Barrage]!|h|r! Get behind cover to survive!", me->GetGUID(), true);
                        me->GetMotionMaster()->MovePoint(eSkullocMovementInformed::MovementInformedBeginBarrage, g_HomeSkullcHomePos.GetPositionX(), g_HomeSkullcHomePos.GetPositionY(), g_HomeSkullcHomePos.GetPositionZ());
                        events.ScheduleEvent(eSkullocEvents::EventCannonBarrage, 50 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eSkullocEvents::EventCannonBarrageCancel:
                        {
                            me->CastSpell(me, eSkullocSpells::SpellCannonBarrageAura);
                            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            break;
                        }
                    case eSkullocEvents::EventBackdraft:
                        {
                               if (Creature* l_BackdraftCreature = me->FindNearestCreature(eCreatures::CreatureBackdraft, 300.0f, true))
                                l_BackdraftCreature->CastSpell(l_BackdraftCreature, eSkullocSpells::SpellBackdraft);
                            events.ScheduleEvent(eSkullocEvents::EventBackdraft, 30 * TimeConstants::IN_MILLISECONDS);
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
            return new boss_skullocAI(p_Creature);
        }
};

/// Koramar <Captain of Blackhand's Might> - 83613
class iron_docks_skulloc_mob_koramar : public CreatureScript
{
    public:

        iron_docks_skulloc_mob_koramar() : CreatureScript("iron_docks_skulloc_mob_koramar") { }

        struct iron_docks_skulloc_mob_koramarAI : public ScriptedAI
        {
            iron_docks_skulloc_mob_koramarAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = me->GetInstanceScript();
            }

            enum eKoramarSpells
            {
                /// Koramar
                SpellBerserkerLeapLightVisual = 168399,
                SpellBerserkerLeapJump        = 168964,
                SpellBerserkerLeapDummy       = 168965,
                SpellBladestorm               = 168402,
                SpellShatteringBlades         = 168939
            };

            enum eKoramarEvents
            {
                /// Karomar
                EventBerserkerLeap = 1,
                EventBladestorm,
                EventShatteringBlades,
            };

            InstanceScript* m_Instance;
            uint8 m_BladestormMoveCount;

            void Reset() override
            {
                events.Reset();
                me->setFaction(HostileFaction);
                if (me->GetMap())
                {
                    me->GetMap()->SetObjectVisibility(5000.0f);
                }            
            }

            void EnterCombat(Unit* p_Attacker) override
            {             
                me->GetMotionMaster()->MoveChase(p_Attacker); ///< He get bugs out upon combat, this seems to fix it. I guess...
                events.ScheduleEvent(eKoramarEvents::EventBladestorm, 40 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eKoramarEvents::EventShatteringBlades, 15 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eKoramarEvents::EventBerserkerLeap, 20 * TimeConstants::IN_MILLISECONDS);
            }

            void JustDied(Unit* /*p_Killer*/) override
            {
                if (m_Instance != nullptr)
                {
                    if (Creature* l_Turret = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataTurret)))
                    {
                        if (l_Turret->IsAIEnabled)
                            l_Turret->GetAI()->DoAction(eIronDocksActions::ActionLeaveTurret);
                    }
                }
            }

            void JustReachedHome() override
            {
                if (m_Instance != nullptr)
                {
                    m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                    SkullocEnforcersWipingCondition(m_Instance, me);
                }
                summons.DespawnAll();
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {             
                switch (p_Id)
                {
                    case eMovementInformed::MovementInformedKoramarBladestorm:
                    {
                        me->GetMotionMaster()->MovePoint(eMovementInformed::MovementInformedKoramarBladestorm, g_BladestormPositions[m_BladestormMoveCount].GetPositionX(), g_BladestormPositions[m_BladestormMoveCount].GetPositionZ(), g_BladestormPositions[m_BladestormMoveCount].GetPositionY());
                        m_BladestormMoveCount++;

                        if (m_BladestormMoveCount >= 7)
                        {
                            me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            m_BladestormMoveCount = 0;
                        }
                        break;
                    }
                    case eMovementInformed::MovementInformedKoramarJump:
                        {
                            me->CastSpell(me, eKoramarSpells::SpellBerserkerLeapLightVisual);
                            break;
                        }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eKoramarEvents::EventBladestorm:
                            me->CastSpell(me, eKoramarSpells::SpellBladestorm);
                            me->SetReactState(ReactStates::REACT_PASSIVE);
                            me->GetMotionMaster()->MovePoint(eMovementInformed::MovementInformedKoramarBladestorm, g_BladestormPositions[m_BladestormMoveCount].GetPositionX(), g_BladestormPositions[m_BladestormMoveCount].GetPositionY(), g_BladestormPositions[m_BladestormMoveCount].GetPositionZ());
                            m_BladestormMoveCount++;
                            events.ScheduleEvent(eKoramarEvents::EventBladestorm, 40 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eKoramarEvents::EventShatteringBlades:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_FARTHEST, 0, 100.0f, true))
                                me->CastSpell(l_Target, eKoramarSpells::SpellShatteringBlades);
                            events.ScheduleEvent(eKoramarEvents::EventShatteringBlades, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eKoramarEvents::EventBerserkerLeap:
                            me->CastSpell(me, eKoramarSpells::SpellBerserkerLeapDummy);
                            events.ScheduleEvent(eKoramarEvents::EventBerserkerLeap, 20 * TimeConstants::IN_MILLISECONDS);
                            break;
                        default:
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_skulloc_mob_koramarAI(p_Creature);
        }
};

/// Blackhand's Might Turret - 84215
class iron_docks_skulloc_mob_turret : public CreatureScript
{
    public:

        iron_docks_skulloc_mob_turret() : CreatureScript("iron_docks_skulloc_mob_turret") { }

        struct iron_docks_skulloc_mob_turretAI : public ScriptedAI
        {
            iron_docks_skulloc_mob_turretAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance = me->GetInstanceScript();
            }

            enum eTurretSpells
            {
                SpellRapidFire = 168348
            };

            enum eTurretEvents
            {
                EventRapidFire = 1,
                EventRapidFireChangeTarget,
                EventInstallAccessories
            };

            Vehicle* m_Vehicle;
            uint64 m_TargetGuid;
            InstanceScript* m_Instance;

            void Reset() override
            {
                events.Reset();
                if (me->GetMap())
                {
                    me->GetMap()->SetObjectVisibility(5000.0f);
                }
                m_TargetGuid = 0;
                ASSERT(m_Vehicle);
                me->setFaction(HostileFaction);       
                me->SetUnitMovementFlags(MovementFlags::MOVEMENTFLAG_ROOT);
                DespawnCreaturesInArea(eIronDocksCreatures::CreatureZoggosh, me);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE); 
                events.ScheduleEvent(eTurretEvents::EventInstallAccessories, 5 * TimeConstants::IN_MILLISECONDS);
            }

            void EnterCombat(Unit* /*p_Who*/) override
            {
                if (m_Instance != nullptr)
                    SkullocEnforcersStart(m_Instance, me);

                events.ScheduleEvent(eTurretEvents::EventRapidFireChangeTarget, 2 * TimeConstants::IN_MILLISECONDS);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eIronDocksActions::ActionLeaveTurret:
                        {                                   
                            if (m_Instance != nullptr)
                            {
                                if (Creature* l_Zoggosh = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataZuggosh)))
                                {
                                    if (l_Zoggosh->IsAIEnabled)
                                        l_Zoggosh->AI()->Talk(eTalks::TalkKoramarVengence);

                                    l_Zoggosh->SetReactState(ReactStates::REACT_AGGRESSIVE);
                                    l_Zoggosh->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                                    if (Player* l_Target = me->FindNearestPlayer(100.0f, true))
                                        l_Zoggosh->Attack(l_Target, true);
                                }
                            }
                  
                            me->CombatStop();
                            me->setFaction(FriendlyFaction);
                            me->DespawnOrUnsummon();
                            me->getHostileRefManager().clearReferences();
                            events.CancelEvent(eTurretEvents::EventRapidFire);
                            if (m_Vehicle != nullptr)
                            m_Vehicle->RemoveAllPassengers();
                            break;
                        }
                        default:
                            break;
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {       
                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eTurretEvents::EventRapidFireChangeTarget:
                        {
                            if (!UpdateVictim())
                                return;

                            events.CancelEvent(eTurretEvents::EventRapidFire);

                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 60.0f, true))
                                m_TargetGuid = l_Target->GetGUID();
                            events.ScheduleEvent(eTurretEvents::EventRapidFireChangeTarget, 10 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eTurretEvents::EventRapidFire, 2 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eTurretEvents::EventRapidFire:
                        {
                            if (!UpdateVictim())
                                return;

                            if (Unit* l_Target = Unit::GetUnit(*me, m_TargetGuid))
                            {
                                if (l_Target->IsInWorld() && l_Target->IsWithinDistInMap(me, 200.0f, true))
                                    me->CastSpell(l_Target, eTurretSpells::SpellRapidFire);
                            }
                            events.ScheduleEvent(eTurretEvents::EventRapidFire, 2 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eTurretEvents::EventInstallAccessories:
                        {
                            m_Vehicle->InstallAllAccessories(false);
                            break;
                        }
                        default:
                            break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_skulloc_mob_turretAI(p_Creature);
        }
};

/// Zoggosh - 83616
class iron_docks_skulloc_mob_zuggosh : public CreatureScript
{
public:

    iron_docks_skulloc_mob_zuggosh() : CreatureScript("iron_docks_skulloc_mob_zuggosh") { }

    struct iron_docks_skulloc_mob_zuggoshAI : public BossAI
    {
        iron_docks_skulloc_mob_zuggoshAI(Creature* p_Creature) : BossAI(p_Creature, eIronDocksDatas::DataZuggosh)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
            if (me->GetMap())
            {
                me->GetMap()->SetObjectVisibility(5000.0f);
            }
        }

        void JustReachedHome() override
        {
            if (m_Instance != nullptr)
            {
                m_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, me);
                SkullocEnforcersWipingCondition(m_Instance, me);
            }
        }

        void JustDied(Unit* p_Killer) override
        {
            if (m_Instance != nullptr && p_Killer)
            {
                SkullocEnforcersWiningCondition(m_Instance, me, p_Killer->GetGUID());
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_skulloc_mob_zuggoshAI(p_Creature);
    }
};

/// Backdraft - 169132
class iron_docks_skulloc_spell_back_draft : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_back_draft() : SpellScriptLoader("iron_docks_skulloc_spell_back_draft") { }

        class iron_docks_skulloc_spell_back_draft_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_skulloc_spell_back_draft_SpellScript);

            void HandleDamage(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (l_Target->IsWithinLOSInMap(l_Caster))
                            SetHitDamage(0);
                    }
                }
            }

            void Register() override
            {
                OnEffectHitTarget += SpellEffectFn(iron_docks_skulloc_spell_back_draft_SpellScript::HandleDamage, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_skulloc_spell_back_draft_SpellScript();
        }
};

/// Gronn Smash - 168227
class iron_docks_skulloc_spell_gronn_smash : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_gronn_smash() : SpellScriptLoader("iron_docks_skulloc_spell_gronn_smash") { }

        class iron_docks_skulloc_spell_gronn_smash_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_skulloc_spell_gronn_smash_SpellScript);

            void HandleScriptEffect(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 100.0f);
                    if (l_PlayerList.empty())
                        return;

                    for (auto l_Iter : l_PlayerList)
                    {
                        l_Iter->GetMotionMaster()->MoveJump(g_BackdraftNpcPos.GetPositionX(), g_BackdraftNpcPos.GetPositionY(), g_BackdraftNpcPos.GetPositionZ(), 40.0f, 20.0f, 10.0f);
                    }
                }
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(iron_docks_skulloc_spell_gronn_smash_SpellScript::HandleScriptEffect, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_SCRIPT_EFFECT);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_skulloc_spell_gronn_smash_SpellScript();
        }
};

/// Cannon Barrage - 168537
class iron_docks_skulloc_spell_cannon_barrage : public SpellScriptLoader
{
    public:
        iron_docks_skulloc_spell_cannon_barrage() : SpellScriptLoader("iron_docks_skulloc_spell_cannon_barrage") { }

        class iron_docks_skulloc_spell_cannon_barrage_SpellScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_skulloc_spell_cannon_barrage_SpellScript);

            enum eCannonBarrageSpells
            {
                SpellCannonBarrageMissileOriginal = 168539
            };

            std::list<uint64> m_TargetGuids;
            bool Load() override
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Creature*> l_CreatureList;
                    m_TargetGuids.clear();

                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                    {
                        /// Position is changing incrementally, instead of randomally
                        if (Creature * l_Skulloc = l_Instance->instance->GetCreature(l_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                        {
                            if (l_Skulloc->IsAIEnabled)
                            {
                                if (boss_skulloc::boss_skullocAI* l_LinkAI = CAST_AI(boss_skulloc::boss_skullocAI, l_Skulloc->GetAI()))
                                {
                                    if (l_Skulloc->isInCombat())
                                        l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::CreatureCannonBarrageInBossFight, 200.0f);
                                    else
                                    {
                                        switch (l_LinkAI->m_BombardmentPosition)
                                        {
                                            case 0:
                                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::CreatureCannonBarrageTarget, 1500.0f);
                                                break;
                                            case 1:
                                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::CreatureCannonBarrageTarget2, 1500.0f);
                                                break;
                                            case 2:
                                                l_Caster->GetCreatureListWithEntryInGrid(l_CreatureList, eIronDocksCreatures::CreatureCannonBarrageTarget3, 1500.0f);
                                                break;
                                            case 3:
                                                l_LinkAI->m_BombardmentPosition = 0;
                                                break;
                                            default:
                                                break;
                                        }

                                        l_LinkAI->m_BombardmentPosition++;
                                    }
                                }
                            }
                        }
                    }

                    for (auto l_Iter : l_CreatureList)
                        m_TargetGuids.push_back(l_Iter->GetGUID());
                }
                return true;
            }

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Target = GetTarget())
                {
                    if (l_Target->GetTypeId() != TypeID::TYPEID_PLAYER)
                    {
                        if (Creature* l_TargetCreature = l_Target->ToCreature())
                        {
                            l_TargetCreature->SetReactState(ReactStates::REACT_AGGRESSIVE);
                            l_TargetCreature->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
                            l_TargetCreature->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
                        }
                    }
                }
            }

            void PeriodicTick(AuraEffect const* /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (m_TargetGuids.empty())
                        return;

                    for (auto l_Iter : m_TargetGuids)
                    {
                        if (Creature* l_Target = Creature::GetCreature(*l_Caster, l_Iter))
                        {
                            l_Caster->CastSpell(l_Target, eCannonBarrageSpells::SpellCannonBarrageMissileOriginal);
                            l_Caster->SetFacingToObject(l_Target);
                        }
                    }
                }
            }

            void Register() override
            {
                AfterEffectRemove += AuraEffectRemoveFn(iron_docks_skulloc_spell_cannon_barrage_SpellScript::OnRemove, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_skulloc_spell_cannon_barrage_SpellScript::PeriodicTick, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_skulloc_spell_cannon_barrage_SpellScript();
        }
};

/// Berserker Leap - 168965
class iron_docks_skulloc_spell_berserker_jump : public SpellScriptLoader
{
    public:

        iron_docks_skulloc_spell_berserker_jump() : SpellScriptLoader("iron_docks_skulloc_spell_berserker_jump") { }

        class iron_docks_skulloc_spell_berserker_jump_AuraScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_skulloc_spell_berserker_jump_AuraScript);

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->IsAIEnabled)
                    {
                        if (Unit* l_Random = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            l_Caster->GetMotionMaster()->CustomJump(l_Random->GetPositionX(), l_Random->GetPositionY(), l_Random->GetPositionZ(), 15.0f, 8.0f, eMovementInformed::MovementInformedKoramarJump);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_skulloc_spell_berserker_jump_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_skulloc_spell_berserker_jump_AuraScript();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_skulloc()
{
    /// Bosses
    new boss_skulloc(); /// 83612
    new iron_docks_skulloc_mob_koramar(); /// 83613
    new iron_docks_skulloc_mob_turret();  /// 84215
    new iron_docks_skulloc_mob_zuggosh(); /// 83616
    /// Spells
    new iron_docks_skulloc_spell_back_draft();  /// 169132
    new iron_docks_skulloc_spell_gronn_smash(); /// 168227
    new iron_docks_skulloc_spell_cannon_barrage(); /// 168537
    new iron_docks_skulloc_spell_berserker_jump(); /// 168965
}
#endif