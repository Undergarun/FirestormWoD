////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"
#include "Vehicle.h"

enum eSpells
{
    /// Misc
    SpellSelfStun                       = 94563,
    SpellEmoteWork                      = 43831,
    SpellCarryCrate                     = 173166,
    SpellCarrySack                      = 167539,
    SpellApplauseCheer                  = 84062
};

enum eMovementInformed
{
    MovementInformedClefthoofTargetArrivDest = 1,
    MovementInformedIronStarWallContact
};

enum eEmotes
{
    EmoteWork01                         = 133,
    EmoteFight                          = 36, 
};

enum eAction
{
    ActionQuietDeath = 744
};

/// Iron star kill event.
class basicevent_kill : public BasicEvent
{
public:

    explicit basicevent_kill(InstanceScript* p_Instance, uint64 p_KillerGuid, uint64 p_VictimGuid) : m_Instance(p_Instance), m_KillerGuid(p_KillerGuid), m_VictimGuid(p_VictimGuid) { }

    bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
    {
        if (m_Instance != nullptr)
        {
            if (Creature* l_Killer = m_Instance->instance->GetCreature(m_KillerGuid))
            {
                if (Creature* l_Victim = m_Instance->instance->GetCreature(m_VictimGuid))
                    l_Killer->Kill(l_Victim);
            }
        }

        return true;
    }

private:
    InstanceScript* m_Instance;
    uint64 m_KillerGuid;
    uint64 m_VictimGuid;
    int m_Modifier;
};

/// Grom'kar Battle Master - 83025
class iron_docks_mob_gromkar_battlemaster : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_battlemaster() : CreatureScript("iron_docks_mob_gromkar_battlemaster") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eGromkarEvents
            {
                /// Gromkar Battlemaster
                EventBladestorm = 1,
                EventChainDrag,
                EventChargingSlash
            };

            enum eGromkarSpells
            {
                /// Battlemaster
                SpellBladestorm        = 167232,
                SpellChainDrag         = 176287,
                SpellChargingSlash     = 172889,
                SpellChargingSlashJump = 172885
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->RemoveAllAuras();
                events.ScheduleEvent(eGromkarEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eGromkarEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eGromkarEvents::EventChargingSlash, 10 * TimeConstants::IN_MILLISECONDS);
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
                    case eGromkarEvents::EventBladestorm:
                            me->CastSpell(me, eGromkarSpells::SpellBladestorm);             
                            break;
                    case eGromkarEvents::EventChainDrag:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(l_Target, eGromkarSpells::SpellChainDrag);
                            events.ScheduleEvent(eGromkarEvents::EventChainDrag, 30 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eGromkarEvents::EventBladestorm, 2 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eGromkarEvents::EventChargingSlash:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                me->CastSpell(l_Target, eGromkarSpells::SpellChargingSlashJump);
                            events.ScheduleEvent(eGromkarEvents::EventChargingSlash, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                        default:
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Dead Eye - 83028
/// Grom'kar Dead Eye - 83764
class iron_docks_mob_gromkar_deadeye : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_deadeye() : CreatureScript("iron_docks_mob_gromkar_deadeye") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            enum eDeadeyeEvents
            {
                /// Deadeye
                EventIronShot = 1,
                EventLegShot
            };

            enum eDeadeyeSpells
            {
                /// Deadeye
                SpellIronShot = 167239,
                SpellLegShot  = 167240
            };

            uint32 m_VisualTimer;

            void Reset() override
            {
                events.Reset();
                m_VisualTimer = 5 * TimeConstants::IN_MILLISECONDS;
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eDeadeyeEvents::EventIronShot, urand(5 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eDeadeyeEvents::EventLegShot, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::CreatureIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (Creature* l_ArcheryTarget = me->FindNearestCreature(eIronDocksCreatures::CreatureArcheryTarget, 30.0f))
                        {
                            me->CastSpell(l_ArcheryTarget, eDeadeyeSpells::SpellIronShot, true);
                            m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                        }
                    }
                    else
                        m_VisualTimer -= p_Diff;
                }

                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eDeadeyeEvents::EventLegShot:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->CastSpell(l_Target, eDeadeyeSpells::SpellLegShot);
                            events.ScheduleEvent(eDeadeyeEvents::EventLegShot, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                            break;
                    case eDeadeyeEvents::EventIronShot:
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eDeadeyeSpells::SpellIronShot);
                            events.ScheduleEvent(eDeadeyeEvents::EventIronShot, urand(5 * TimeConstants::IN_MILLISECONDS, 8 * TimeConstants::IN_MILLISECONDS));
                            break;
                        default:
                            break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Foot Soldier - 83765
/// Grom'kar Footsoldier - 85997
/// Grom'kar Footsoldier - 859977
class iron_docks_mob_gromkar_footsoldier : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_footsoldier() : CreatureScript("iron_docks_mob_gromkar_footsoldier") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eFootSoldierEvents
            {
                /// Footsoldier
                EventTacticalKick = 1,
                EventDemoralizingShout,
                EventChargingSlash
            };

            enum eFootSoldierSpells
            {
                /// Footsoldier
                SpellTacticalKick      = 169413,
                SpellDemoralizingShout = 169341,
                SpellChargingSlash     = 172889,
                SpellChargingSlashJump = 172885,
                SpellKneelDown         = 173091
            };

            void Reset() override
            {
                events.Reset();
                me->setFaction(HostileFaction);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);    
            }

            void EnterCombat(Unit* p_Who) override
            {
                me->RemoveAllAuras();
                events.ScheduleEvent(eFootSoldierEvents::EventTacticalKick, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eFootSoldierEvents::EventDemoralizingShout, 8 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eFootSoldierEvents::EventChargingSlash, 25 * TimeConstants::IN_MILLISECONDS);               
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::CreatureIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
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
                    case eFootSoldierEvents::EventDemoralizingShout:
                                me->CastSpell(me, eFootSoldierSpells::SpellDemoralizingShout);
                                events.ScheduleEvent(eFootSoldierEvents::EventDemoralizingShout, 25 * TimeConstants::IN_MILLISECONDS);
                                break;
                    case eFootSoldierEvents::EventTacticalKick:
                             if (Unit* l_Target = me->getVictim())
                                 me->CastSpell(l_Target, eFootSoldierSpells::SpellTacticalKick);
                                events.ScheduleEvent(eFootSoldierEvents::EventTacticalKick, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                                break;
                    case eFootSoldierEvents::EventChargingSlash:
                                if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                    me->CastSpell(l_Target, eFootSoldierSpells::SpellChargingSlashJump);
                                events.ScheduleEvent(eFootSoldierEvents::EventChargingSlash, 25 * TimeConstants::IN_MILLISECONDS);
                                break;
                            default:
                                break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Incinerator - 86809
class iron_docks_mob_gromkar_incinerator : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_incinerator() : CreatureScript("iron_docks_mob_gromkar_incinerator") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eIncineratorEvents
            {
                /// Incinerator
                EventIncendinarySlug = 907,
                EventSharpnelBlast   = 908
            };

            enum eIncienratorSpells
            {
                /// Incinerator
                SpellIncenarySlugs = 176902, ///< Actual full spell, blizzards are just a bunch of r tards.
                SpellSharpnelBlast = 167516,
                SpellIronShot      = 167239
            };

            uint32 m_VisualTimer;

            void Reset() override
            {
                events.Reset();
                m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                me->AddUnitMovementFlag(MovementFlags::MOVEMENTFLAG_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE);
            }

            void EnterCombat(Unit* p_Who) override
            {
                events.ScheduleEvent(eIncineratorEvents::EventIncendinarySlug, urand(15 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (Creature* l_ArcheryTarget = me->FindNearestCreature(eIronDocksCreatures::CreatureArcheryTarget, 30.0f))
                        {
                            me->CastSpell(l_ArcheryTarget, eIncienratorSpells::SpellIronShot, true);
                            m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                        }
                    }
                    else
                        m_VisualTimer -= p_Diff;
                }

                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eIncineratorEvents::EventIncendinarySlug:
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eIncienratorSpells::SpellIncenarySlugs);
                            events.ScheduleEvent(eIncineratorEvents::EventIncendinarySlug, urand(15 * TimeConstants::IN_MILLISECONDS, 18 * TimeConstants::IN_MILLISECONDS));
                            break;
                        default:
                            break;
                }

                DoSpellAttackIfReady(eIncienratorSpells::SpellSharpnelBlast);
            }
        };

        CreatureAI* GetAI(Creature* creature) const override
        {
            return new mob_iron_docksAI(creature);
        }
};

/// Grom'kar Technician - 81432
class iron_docks_mob_gromkar_technician : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_technician() : CreatureScript("iron_docks_mob_gromkar_technician") { }

        struct iron_docks_mob_gromkar_technicianAI : public ScriptedAI
        {
            iron_docks_mob_gromkar_technicianAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eTechnicianSpells
            {
                /// Gromkar Technician
                SpellFlyingHammer         = 172703,
                SpellHighExplosiveGrenade = 178298,
                SpellGreaseVial           = 172649,
                SpellGreaseVialEffect     = 172636,
                SpellArmedWithExplosives  = 178296
            };

            enum eTechnicianEvents
            {
                /// Gromkar Technician
                EventGreaseVial = 1,
                EventFlyingHammer,
                EventExplosiveGrenade
            };
   
            uint32 m_VisualTimer;

            void Reset() override
            {
                events.Reset();
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                me->CastSpell(me, eTechnicianSpells::SpellArmedWithExplosives);       
            }

            void EnterCombat(Unit* p_Who) override
            {
                me->RemoveAura(eSpells::SpellEmoteWork);
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                events.ScheduleEvent(eTechnicianEvents::EventGreaseVial, urand(10 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eTechnicianEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eTechnicianEvents::EventExplosiveGrenade, urand(10 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::CreatureIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (me->HasAura(eSpells::SpellEmoteWork))
                        me->RemoveAura(eSpells::SpellEmoteWork);            

                        switch (urand(0, 1))
                        {
                            case 0: ///< Work
                                me->CastSpell(me, eSpells::SpellEmoteWork);
                                break;
                            case 1: ///< Craft
                                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 133);
                                break;
                            default:
                                break;
                        }

                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                        m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_VisualTimer -= p_Diff;
                }

                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eTechnicianEvents::EventGreaseVial:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->CastSpell(l_Target, eTechnicianSpells::SpellGreaseVial);
                            events.ScheduleEvent(eTechnicianEvents::EventGreaseVial, urand(40 * TimeConstants::IN_MILLISECONDS, 70 * TimeConstants::IN_MILLISECONDS));
                            break;
                        case eTechnicianEvents::EventFlyingHammer:
                            if (Unit* l_Victim = me->getVictim())
                                me->CastSpell(l_Victim, eTechnicianSpells::SpellFlyingHammer);
                            events.ScheduleEvent(eTechnicianEvents::EventFlyingHammer, urand(8 * TimeConstants::IN_MILLISECONDS, 14 * TimeConstants::IN_MILLISECONDS));
                            break;
                        case eTechnicianEvents::EventExplosiveGrenade:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->CastSpell(l_Target, eTechnicianSpells::SpellHighExplosiveGrenade);                       
                            me->RemoveAura(eTechnicianSpells::SpellArmedWithExplosives);
                            break;
                        default:
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_mob_gromkar_technicianAI(p_Creature);
        }
};

/// Siegemaster Olugar - 83026
class iron_docks_mob_siege_master_olugar : public CreatureScript
{
    public:

        iron_docks_mob_siege_master_olugar() : CreatureScript("iron_docks_mob_siege_master_olugar") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eOlugarEvents
            {
                /// Siege Master Olugar
                EventGatecrasher = 1,
                EventShatteringStrike
            };

            enum eOlugarSpells
            {
                /// Siege Master Olugar
                SpellGatecrasherDamage   = 172963,
                SpellThrowGatecrasher    = 172952,
                SpellPitFighter          = 173455, ///< Upon disarm
                SpellShatteringStrike    = 172982
            };

            uint32 m_VisualTimer;

            void Reset() override
            {
                events.Reset();
                me->setFaction(HostileFaction);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                m_VisualTimer = 16 * TimeConstants::IN_MILLISECONDS;
            }

            void EnterCombat(Unit* p_Who) override
            {
                /// I don't think Darona actually shot barber arrows, wowhead is stupid. Icy veins says it aswell.
                events.ScheduleEvent(eOlugarEvents::EventShatteringStrike, urand(6 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eOlugarEvents::EventGatecrasher, 20 * TimeConstants::IN_MILLISECONDS);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        me->MonsterSay("Stop showin' off.", Language::LANG_UNIVERSAL, me->GetGUID());
                        m_VisualTimer = 16 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_VisualTimer -= p_Diff;
                }

                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                /// Pit fighter
                if (me->HasAuraType(AuraType::SPELL_AURA_MOD_DISARM))
                {
                    if (!me->HasAura(eOlugarSpells::SpellPitFighter))
                        me->AddAura(eOlugarSpells::SpellPitFighter, me);
                }
                else
                {
                    if (me->HasAura(eOlugarSpells::SpellPitFighter))
                        me->RemoveAura(eOlugarSpells::SpellPitFighter);
                }

                switch (events.ExecuteEvent())
                {
                    case eOlugarEvents::EventShatteringStrike:
                        {
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eOlugarSpells::SpellShatteringStrike);
                            events.ScheduleEvent(eOlugarEvents::EventShatteringStrike, urand(6 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eOlugarEvents::EventGatecrasher:
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                                me->CastSpell(l_Target, eOlugarSpells::SpellThrowGatecrasher);
                            events.ScheduleEvent(eOlugarEvents::EventGatecrasher, 20 * TimeConstants::IN_MILLISECONDS);
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Hammer Stalker - 86534
class iron_docks_mob_hammer_stalker : public CreatureScript /// Being used for the hammer stalker spell from Throw Gatecrasher of Olugar.
{
    public:

    iron_docks_mob_hammer_stalker() : CreatureScript("iron_docks_mob_hammer_stalker") { }

    struct iron_docks_mob_hammer_stalkerAI : public ScriptedAI
    {
        iron_docks_mob_hammer_stalkerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eOlugarSpells
        {
            /// Siege Master Olugar
            SpellGatecrasherDamage  = 172963
        };

        uint32 m_DamageDiff;

        void Reset() override
        {
            me->setFaction(HostileFaction);
            m_DamageDiff = 1 * TimeConstants::IN_MILLISECONDS;
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_DISABLE_TURN);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_DISABLE_MOVE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (m_DamageDiff <= p_Diff)
            {
                std::list<Player*> l_ListPlayers;
                me->GetPlayerListInGrid(l_ListPlayers, 2.0f);
                if (!l_ListPlayers.empty())
                {
                    for (auto l_Itr : l_ListPlayers)
                    {
                        l_Itr->CastSpell(l_Itr, eOlugarSpells::SpellGatecrasherDamage);
                    }
                }

                m_DamageDiff = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_DamageDiff -= p_Diff;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_mob_hammer_stalkerAI(p_Creature);
    }
};

/// Champion Druna - 81603
class iron_docks_mob_champion_darona : public CreatureScript
{
    public:

        iron_docks_mob_champion_darona() : CreatureScript("iron_docks_mob_champion_darona") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eDorunaEvents
            {    
                /// Champion Daruna
                EventBurningArrow = 1,
                EventBarbedArrow,
                EventChampionPresence
            };

            enum eDorunaSpells
            {
                /// Champion Darona
                SpellBarbedArrowBarrageDummy = 166923,
                SpellBarbedArrowAreaTrigger  = 164278,
                SpellBarbedArrowAura         = 164370,
                SpellBarbedArrowDot          = 164648,
                SpellBurningArrowDummy       = 172810,
                SpellBurningArrowAreaTrigger = 164234,
                SpellBurningArrowDoT         = 164632,
                SpellChampionsPresence       = 173091
            };

            void Reset() override
            {
                events.Reset();
                me->CastSpell(me, eDorunaSpells::SpellChampionsPresence);
            }

            void EnterCombat(Unit* p_Who) override
            {
                events.ScheduleEvent(eDorunaEvents::EventBurningArrow, 8 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eDorunaEvents::EventBarbedArrow, 11 * TimeConstants::IN_MILLISECONDS);
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
                    case eDorunaEvents::EventBurningArrow:
                    {
                        me->MonsterYell("Light them up!", Language::LANG_UNIVERSAL, me->GetGUID());
                        ///< Commands the Flameslinger to shoot         
                        std::list<Creature*> l_ListFlameSlingers;
                        me->GetCreatureListWithEntryInGrid(l_ListFlameSlingers, eIronDocksCreatures::CreatureGromkarFlameslinger, 200.0f);
                        if (!l_ListFlameSlingers.empty())
                        {
                            for (auto l_Itr : l_ListFlameSlingers)
                            {
                                if (l_Itr->IsAIEnabled)
                                    l_Itr->GetAI()->DoAction(eIronDocksActions::ActionBurningArrowSingle);
                            }
                        }

                        events.ScheduleEvent(eDorunaEvents::EventBurningArrow, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eDorunaEvents::EventBarbedArrow:
                    {
                        me->CastSpell(me, eDorunaSpells::SpellBarbedArrowBarrageDummy);
                        me->AddAura(eDorunaSpells::SpellBarbedArrowAura, me);
                        events.ScheduleEvent(eDorunaEvents::EventBarbedArrow, 50 * TimeConstants::IN_MILLISECONDS);
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Pitwarden Gwarnok - 84520
class iron_docks_mob_gwarnok : public CreatureScript
{
    public:

        iron_docks_mob_gwarnok() : CreatureScript("iron_docks_mob_gwarnok") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eGwarnokEvents
            {
                /// Gromkar Battlemaster
                EventBladestorm = 1,
                EventChainDrag,
                EventChargingSlash
            };

            enum eGwarnokSpells
            {
                /// Pitwarden Gwarnok
                SpellBladestorm         = 167232,
                SpellChainDrag          = 176287,
                SpellChargingSlash      = 172889,
                SpellChargingSlashJump  = 172885,
                SpellBrutalInspiration  = 172943
            };

            void Reset() override
            {
                events.Reset();
                me->SetReactState(ReactStates::REACT_DEFENSIVE);
            }

            void EnterCombat(Unit* p_Who) override
            {
                events.ScheduleEvent(eGwarnokEvents::EventChargingSlash, 8 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eGwarnokEvents::EventChainDrag, 15 * TimeConstants::IN_MILLISECONDS);
                me->CastSpell(me, eGwarnokSpells::SpellBrutalInspiration);
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
                    case eGwarnokEvents::EventBladestorm:
                        {
                            me->CastSpell(me, eGwarnokSpells::SpellBladestorm);
                            break;
                        }
                    case eGwarnokEvents::EventChargingSlash:
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                                me->CastSpell(l_Target, eGwarnokSpells::SpellChargingSlashJump);
                            events.ScheduleEvent(eGwarnokEvents::EventChargingSlash, 15 * TimeConstants::IN_MILLISECONDS);
                            break;
                        }
                    case eGwarnokEvents::EventChainDrag:
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                            {
                                me->CastSpell(l_Target, eGwarnokSpells::SpellChainDrag);
                                l_Target->GetMotionMaster()->MoveJump(*me, 8.0f, 5.0f, 10.0f);
                            }
                            events.ScheduleEvent(eGwarnokEvents::EventBladestorm, 2 * TimeConstants::IN_MILLISECONDS);
                            events.ScheduleEvent(eGwarnokEvents::EventChainDrag, 25 * TimeConstants::IN_MILLISECONDS);
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Iron Star - 81247
class iron_docks_mob_iron_star_vehicle : public CreatureScript
{
    public:

    iron_docks_mob_iron_star_vehicle() : CreatureScript("iron_docks_mob_iron_star_vehicle") { }

    struct iron_docks_mob_iron_star_vehicleAI : public ScriptedAI
    {
        iron_docks_mob_iron_star_vehicleAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
        }

        InstanceScript* m_Instance;

        void Reset() override
        {
        }

        void UpdateAI(uint32 const p_Diff) override
        {
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_mob_iron_star_vehicleAI(p_Creature);
    }
};


/// Iron Star - 812477
class iron_docks_mob_iron_star : public CreatureScript
{
public:

    iron_docks_mob_iron_star() : CreatureScript("iron_docks_mob_iron_star") { }

    struct iron_docks_mob_iron_starAI : public ScriptedAI
    {
        iron_docks_mob_iron_starAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
            m_Instance = me->GetInstanceScript();
            m_Activated = false;
            m_Event = false;
        }

        enum eIronStarSpells
        {
            /// Iron Star - Object
            SpellQuietSuicide = 163832,
            SpellAlert = 149814,
            SpellCrushed = 167847
        };

        enum eIronStarSpellVisualKit
        {
            SpellVisualKitAlert = 47409
        };

        InstanceScript* m_Instance;
        bool m_Activated;
        bool m_Event;

        void Reset() override
        {
            events.Reset();
            m_Activated = true;
            me->setFaction(FriendlyFaction);
            me->SetReactState(ReactStates::REACT_PASSIVE);

            /// Takes iron star orientation
            if (TempSummon* l_TempSummon = me->ToTempSummon())
            {
                if (Unit* l_Summoner = l_TempSummon->GetSummoner())
                {
                    float l_X = l_Summoner->m_positionX + 220 * cos(l_Summoner->GetOrientation());
                    float l_Y = l_Summoner->m_positionY + 220 * sin(l_Summoner->GetOrientation());

                    me->GetMotionMaster()->MoveCharge(l_X, l_Y, me->GetPositionZ(), 42.0f, eMovementInformed::MovementInformedIronStarWallContact);
                }
            }


            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
        }
        /*
        void DoAction(int32 const p_Action) override
        {
        switch (p_Action)
        {
        case eIronDocksActions::ActionAcitvateIronStar:
        {
        m_Activated = true;

        float l_X = me->m_positionX + 100 * cos(me->m_orientation);
        float l_Y = me->m_positionY + 100 * sin(me->m_orientation);

        me->GetMotionMaster()->MoveCharge(l_X, l_Y, me->GetPositionZ(), 42.0f, eMovementInformed::MovementInformedIronStarWallContact);
        break;
        }
        default:
        break;
        }
        }
        */

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            if (p_Id == eMovementInformed::MovementInformedIronStarWallContact)
            {
                m_Activated = false;
                me->CastSpell(me, eIronStarSpells::SpellQuietSuicide);
                me->DespawnOrUnsummon(2 * TimeConstants::IN_MILLISECONDS);
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (m_Instance != nullptr)
            {
                if (m_Activated)
                {
                    std::list<Unit*> l_UnfriendlyList;
                    JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(me, me, 4.0f);
                    JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(me, l_UnfriendlyList, l_Check);
                    me->VisitNearbyObject(3.4f, l_Searcher);

                    for (Unit* l_Unit : l_UnfriendlyList)
                    {
                        if (l_Unit->GetTypeId() == TypeID::TYPEID_PLAYER)
                            continue;

                        if (l_Unit->ToCreature()->GetEntry() == eIronDocksCreatures::CreatureIronStar)
                            continue;

                        l_Unit->SetFacingToObject(me);
                        l_Unit->SendPlaySpellVisualKit(eIronStarSpellVisualKit::SpellVisualKitAlert, 0);
                        l_Unit->CastSpell(l_Unit, eIronStarSpells::SpellCrushed);
                        l_Unit->Kill(l_Unit);
                        /// me->m_Events.AddEvent(new basicevent_kill(m_Instance, me->GetGUID(), l_Unit->GetGUID()), me->m_Events.CalculateTime(3 * TimeConstants::IN_MILLISECONDS));
                    }
                }

                if (!m_Event)
                {
                    if (Player* l_Player = me->FindNearestPlayer(2.0f, true))
                    {
                        if (Creature* l_Skulloc = m_Instance->instance->GetCreature(m_Instance->GetData64(eIronDocksDatas::DataSkulloc)))
                        {
                            if (l_Skulloc->IsAIEnabled)
                            {
                                m_Event = true;
                                l_Skulloc->GetAI()->DoAction(eIronDocksActions::ActionActivateBridgeBombardmement);
                            }
                        }
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_mob_iron_starAI(p_Creature);
    }
};


/// Ogron Laborer - 83578
class iron_docks_mob_laborer_ogron : public CreatureScript
{
    public:

        iron_docks_mob_laborer_ogron() : CreatureScript("iron_docks_mob_laborer_ogron") { }

        struct iron_docks_mob_laborer_ogronAI : public ScriptedAI
        {
            iron_docks_mob_laborer_ogronAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eOrgonEvents
            {
                /// Ogron
                EventFlurry = 1,
                EventThunderingStomp
            };

            enum eOrgonSpells
            {
                /// Unruly Orgon
                SpellFlurry          = 178412,
                SpellFlurryDamage    = 178414,
                SpellThunderingStomp = 173135,
                SpellCosmeticRoar    = 105533
            };

            enum eOrgonCreatures
            {
                CreatureTriggerCannon = 99657
            };

            bool m_HasShouted;

            void Reset() override
            {
                events.Reset();
                m_HasShouted = false;
                me->setFaction(HostileFaction);
                me->SetReactState(ReactStates::REACT_AGGRESSIVE);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                me->SetStandState(0);
                me->SetAIAnimKitId(0);
                events.ScheduleEvent(eOrgonEvents::EventThunderingStomp, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eOrgonEvents::EventFlurry, urand(8 * TimeConstants::IN_MILLISECONDS, 13 * TimeConstants::IN_MILLISECONDS));
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::CreatureIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);

                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 10.0f) && !m_HasShouted && me->FindNearestCreature(eOrgonCreatures::CreatureTriggerCannon, 5.0f))
                {
                    m_HasShouted = true;
                    me->MonsterYell("CRUSH THEM!!", Language::LANG_UNIVERSAL, me->GetGUID());
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
                    case eOrgonEvents::EventThunderingStomp:
                        me->CastSpell(me, eOrgonSpells::SpellThunderingStomp);
                        events.ScheduleEvent(eOrgonEvents::EventThunderingStomp, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    case eOrgonEvents::EventFlurry:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, eOrgonSpells::SpellFlurry);
                        events.ScheduleEvent(eOrgonEvents::EventFlurry, urand(8 * TimeConstants::IN_MILLISECONDS, 13 * TimeConstants::IN_MILLISECONDS));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_mob_laborer_ogronAI(p_Creature);
        }
};

/// Grom'kar Deck-Hand - 83697
class iron_docks_mob_gromkar_deck_hand : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_deck_hand() : CreatureScript("iron_docks_mob_gromkar_deck_hand") { }

        struct iron_docks_mob_gromkar_deck_handAI : public ScriptedAI
        {
            iron_docks_mob_gromkar_deck_handAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eDeckHandEvents
            {
                /// Deck-hand
                EventHatchetToss = 921
            };

            enum eDeckHandSpells
            {
                /// Deckhand
                SpellHatchetToss = 173112,  
            };

            void Reset() override
            {
                events.Reset();

                switch (urand(0, 1)) ///< Handles visual for Deck hands.
                {
                    case 0:
                        me->CastSpell(me, eSpells::SpellCarrySack);
                        break;
                    case 1:
                        me->CastSpell(me, eSpells::SpellCarryCrate);
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Who) override
            {
                // Removes to non following units.
                me->RemoveAura(eSpells::SpellEmoteWork);
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                events.ScheduleEvent(eDeckHandEvents::EventHatchetToss, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::CreatureIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
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
                    case eDeckHandEvents::EventHatchetToss:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->CastSpell(l_Target, eDeckHandSpells::SpellHatchetToss);
                            events.ScheduleEvent(eDeckHandEvents::EventHatchetToss, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                            break;
                        default:
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_mob_gromkar_deck_handAI(p_Creature);
        }
};

/// Grom'kar Chainmaster - 86526
class iron_docks_mob_gromkar_chain_master : public CreatureScript
{
    public:

    iron_docks_mob_gromkar_chain_master() : CreatureScript("iron_docks_mob_gromkar_chain_master") { }

    struct iron_docks_mob_gromkar_chain_masterAI : public ScriptedAI
    {
        iron_docks_mob_gromkar_chain_masterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eChainMasterEvents
        {
            EventIronWarCry = 1
        };

        enum eChainMasterSpells
        {
            SpellIronWarCry = 173042
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* p_Who) override
        {
            events.ScheduleEvent(eChainMasterEvents::EventIronWarCry, 10 * TimeConstants::IN_MILLISECONDS);
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
                case eChainMasterEvents::EventIronWarCry:
                    DoCast(eChainMasterSpells::SpellIronWarCry);
                        events.ScheduleEvent(eChainMasterEvents::EventIronWarCry, 25 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_mob_gromkar_chain_masterAI(p_Creature);
    }
};

/// Grom'kar Technician - 83763
class iron_docks_mob_gromkar_technician_deckhand_leader : public CreatureScript
{
    public:

    iron_docks_mob_gromkar_technician_deckhand_leader() : CreatureScript("iron_docks_mob_gromkar_technician_deckhand_leader") { }

    struct mob_iron_docksAI : public ScriptedAI
    {
        mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature)
        {
        }

        enum eTechnicianEvents
        {
            /// Gromkar Technician
            EventGreaseVial = 1,
            EventFlyingHammer,
            EventExplosiveGrenade
        };

        enum eTechnicianSpells
        {
            /// Gromkar Technician
            SpellFlyingHammer         = 172703,
            SpellHighExplosiveGrenade = 178298,
            SpellGreaseVial           = 172649,
            SpellGreaseVialEffect     = 172636
        };

        std::list<uint64> m_Workers;

        void Reset() override
        {
            events.Reset();
            me->SetSpeed(UnitMoveType::MOVE_RUN, 0.5, true);
        }

        void EnterCombat(Unit* p_Attacker) override
        {
            me->RemoveAura(eSpells::SpellEmoteWork);
            events.ScheduleEvent(eTechnicianEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTechnicianEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
            events.ScheduleEvent(eTechnicianEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
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
                case eTechnicianEvents::EventGreaseVial:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eTechnicianSpells::SpellGreaseVial);
                        events.ScheduleEvent(eTechnicianEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eTechnicianEvents::EventFlyingHammer:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eTechnicianSpells::SpellFlyingHammer);
                        events.ScheduleEvent(eTechnicianEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                        break;
                case eTechnicianEvents::EventExplosiveGrenade:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                            me->CastSpell(l_Target, eTechnicianSpells::SpellHighExplosiveGrenade);
                        events.ScheduleEvent(eTechnicianEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
                        break;
                    default:
                        break;
            }

            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new mob_iron_docksAI(p_Creature);
    }
};

/// Trigger_Cannon_Event_Start - 99658
class iron_docks_mob_stand_third_event : public CreatureScript
{
    public:
        iron_docks_mob_stand_third_event() : CreatureScript("iron_docks_mob_stand_third_event") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_CanEvent;

            void Reset() override
            {
                me->setFaction(FriendlyFaction);
                m_CanEvent = false;
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() == TypeID::TYPEID_PLAYER && me->IsWithinDistInMap(p_Who, 15.0f) && !m_CanEvent)
                {
                    m_CanEvent = true;

                    if (InstanceScript* l_InstanceScript = me->GetInstanceScript())
                        l_InstanceScript->SetData(eIronDocksDatas::DataThirdEvent, uint32(true));
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Trigger_Cannon_Crash_Fixed_Target - 99657
/// Trigger_Cannon_Crash_Fixed_Target - 100981
/// Trigger_Cannon_Crash_Fixed_Target - 100982
class iron_docks_trigger_cannon : public CreatureScript
{
    public:
        iron_docks_trigger_cannon() : CreatureScript("iron_docks_trigger_cannon") { }

        struct mob_iron_docksAI : public Scripted_NoMovementAI
        {
            mob_iron_docksAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

            void Reset() override
            {
                me->setFaction(HostileFaction);
                me->SetDisplayId(InvisibleDisplay);
                me->GetMap()->SetObjectVisibility(1000.0f);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(ReactStates::REACT_PASSIVE);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new mob_iron_docksAI(p_Creature);
        }
};


/// Archery Target - 79423
class iron_docks_mob_archery_target : public CreatureScript
{
    public:

    iron_docks_mob_archery_target() : CreatureScript("iron_docks_mob_archery_target") { }

    struct iron_docks_mob_archery_targetAI : public Scripted_NoMovementAI
    {
        iron_docks_mob_archery_targetAI(Creature* p_Creature) : Scripted_NoMovementAI(p_Creature) { }

        void Reset()
        {
            me->SetReactState(ReactStates::REACT_PASSIVE);
            me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
        }

        void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
        {
            p_Damage = 0;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new iron_docks_mob_archery_targetAI(p_Creature);
    }
};

/// Rampaging Clefthoof - 83392
class iron_docks_mob_rampaging_clefthoof : public CreatureScript
{
    public:

        iron_docks_mob_rampaging_clefthoof() : CreatureScript("iron_docks_mob_rampaging_clefthoof") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eClefthoofEvents
            {
                /// Rushing
                EventClefthoofStampede = 1
            };

            enum eClefthoofSpells
            {
                /// Rushing
                SpellClefthoofStampedeVisualMovement = 173350,
                SpellClefthoofStampedeVisualHorning  = 173351,
                SpellClefthoofStampedeVisualJump     = 173352,
                SpellClefthoofStampedeDummyCast      = 173384,
                SpellClefthoofStampedeDamage         = 173351,
                SpellClefthoofSpinyHorns             = 158349,
                SpellClefthoofTrampled               = 173349,
                SpellClefthoofFrenzy                 = 158337
            };

            uint64 m_StampedeGuid;
            uint32 m_StampedeDiff;
            bool m_Frenzied;
            bool m_Stampede;

            void Reset() override
            {
                events.Reset();
                m_Frenzied = false;
                m_Stampede = false;
                m_StampedeGuid = 0;
                m_StampedeDiff = 1 * TimeConstants::IN_MILLISECONDS;
                me->AddAura(eClefthoofSpells::SpellClefthoofSpinyHorns, me);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eClefthoofEvents::EventClefthoofStampede, 12 * TimeConstants::IN_MILLISECONDS);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (!m_Frenzied && me->GetHealthPct() <= 30)
                {
                    m_Frenzied = true;
                    me->CastSpell(me, eClefthoofSpells::SpellClefthoofFrenzy);
                }
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
            {
                if (p_Id == eMovementInformed::MovementInformedClefthoofTargetArrivDest)
                {
                    m_Stampede = false;          
                    me->SetReactState(ReactStates::REACT_AGGRESSIVE);
                    me->CastSpell(me, eClefthoofSpells::SpellClefthoofStampedeDamage);
                    me->RemoveAura(eClefthoofSpells::SpellClefthoofStampedeVisualMovement);                          
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                    if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 20.0f, true))
                        me->Attack(l_Target, true);
                }
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                    return;

                events.Update(p_Diff);

                if (m_Stampede)
                {
                    if (m_StampedeDiff <= p_Diff)
                    {
                        std::list<Player*> l_PlayerList;
                        JadeCore::AnyPlayerInObjectRangeCheck l_Check(me, 2.0f);
                        JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(me, l_PlayerList, l_Check);
                        me->VisitNearbyObject(2.0f, l_Searcher);
                        if (l_PlayerList.empty())
                            return;

                        for (auto l_Itr : l_PlayerList)
                        {
                            if (me->isInFront(l_Itr, M_PI))
                                l_Itr->AddAura(eClefthoofSpells::SpellClefthoofTrampled, l_Itr);
                        }

                        m_StampedeDiff = 1 * TimeConstants::IN_MILLISECONDS;
                    }
                    else
                        m_StampedeDiff -= p_Diff;
                }


                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (events.ExecuteEvent())
                {
                    case eClefthoofEvents::EventClefthoofStampede:
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_FARTHEST, 0, 45.0f, true))
                            {
                                m_Stampede = true;
                                me->SetReactState(ReactStates::REACT_PASSIVE);
                                m_StampedeDiff = 1 * TimeConstants::IN_MILLISECONDS;
                                me->CastSpell(l_Target, eClefthoofSpells::SpellClefthoofStampedeDummyCast);                                                           
                            }

                            events.ScheduleEvent(eClefthoofEvents::EventClefthoofStampede, 25 * TimeConstants::IN_MILLISECONDS);
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Ironwing Flamespitter - 83389
class iron_docks_mob_ironwing_flamespitter : public CreatureScript
{
    public:

        iron_docks_mob_ironwing_flamespitter() : CreatureScript("iron_docks_mob_ironwing_flamespitter") { }

        struct iron_docks_mob_ironwing_flamespitterAI : public ScriptedAI
        {
            iron_docks_mob_ironwing_flamespitterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eIronwingFlamespitterEvents
            {
                /// Ironwing Flamespitter
                EventLavaBarrage = 1,
                EventLavaBlast
            };

            enum eIronwingFlamespitterSpells
            {
                /// Ironwing Flamespitter
                SpellLavaBarrageDummy       = 176356,
                SpellLavaBarrageAreaTrigger = 173482,
                SpellLavaBarrageDot         = 173489,
                SpellLavaBlastDummyAura     = 173514,
                SpellLavaBlastMissile       = 173516,
                SpellLavaBlastDot           = 173517,
                SpellLavaBlastAreatrigger   = 173518
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eIronwingFlamespitterEvents::EventLavaBlast,  10 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eIronwingFlamespitterEvents::EventLavaBarrage, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
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
                    case eIronwingFlamespitterEvents::EventLavaBarrage:
                            me->CastSpell(me, eIronwingFlamespitterSpells::SpellLavaBarrageDummy); // dummy
                            events.ScheduleEvent(eIronwingFlamespitterEvents::EventLavaBarrage, urand(15 * TimeConstants::IN_MILLISECONDS, 20 * TimeConstants::IN_MILLISECONDS));
                            break;
                    case eIronwingFlamespitterEvents::EventLavaBlast:
                            me->CastSpell(me, eIronwingFlamespitterSpells::SpellLavaBlastDummyAura); // dummy
                            events.ScheduleEvent(eIronwingFlamespitterEvents::EventLavaBlast, 35 * TimeConstants::IN_MILLISECONDS);
                            break;
                        default:
                            break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const override
        {
            return new iron_docks_mob_ironwing_flamespitterAI(p_Creature);
        }
};

/// Flurry - 178421
class iron_docks_spell_flurry_periodic : public SpellScriptLoader
{
    public:

        iron_docks_spell_flurry_periodic() : SpellScriptLoader("iron_docks_spell_flurry_periodic") { }

        class iron_docks_auras : public AuraScript
        {
            PrepareAuraScript(iron_docks_auras);

            enum eOrgonSpells
            {
                SpellFlurryDamage = 178414
            };

            void HandlePeriodic(AuraEffect const* p_AurEff)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 5.0f);
                    if (l_PlayerList.empty())
                        return;

                    for (auto l_Itr : l_PlayerList)
                    {
                        if (l_Caster->isInFront(l_Itr, M_PI * 0.5f))
                            l_Caster->CastSpell(l_Itr, eOrgonSpells::SpellFlurryDamage);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_auras::HandlePeriodic, SpellEffIndex::EFFECT_1, AuraType::SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_auras();
        }
};

/// Launch Forward - 167299
class iron_docks_spell_charge_forward : public SpellScriptLoader
{
    public:

        iron_docks_spell_charge_forward() : SpellScriptLoader("iron_docks_spell_charge_forward") { }

        class iron_docks_spell_charge_forward_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_spell_charge_forward_SpellScript);

            enum eIronStarSpellChargeForwardNpc
            {
                CreatureIronStarNpc = 812477
            };

            void HandleCharge()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetVehicleKit())
                    {
                        l_Caster->ToCreature()->DespawnOrUnsummon();
                        l_Caster->GetVehicleKit()->RemoveAllPassengers(true);
                        l_Caster->SummonCreature(eIronStarSpellChargeForwardNpc::CreatureIronStarNpc, *l_Caster, TempSummonType::TEMPSUMMON_MANUAL_DESPAWN);
                    }
                }
            }

            void Register() override
            {           
                AfterCast += SpellCastFn(iron_docks_spell_charge_forward_SpellScript::HandleCharge);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spell_charge_forward_SpellScript();
        }
};

/// Lava Blast - 173514 
class iron_docks_spell_lava_blast : public SpellScriptLoader
{
    public:

        iron_docks_spell_lava_blast() : SpellScriptLoader("iron_docks_spell_lava_blast") { }

        class iron_docks_spell_lava_blast_AuraScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_spell_lava_blast_AuraScript);

            enum eLavaBlastSpells
            {
                SpellLavaBlastTriggerMissile = 173516
            };

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {     
                    if (l_Caster->IsAIEnabled)
                    {
                        if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 300.0f, true))
                            l_Caster->CastSpell(l_Target, eLavaBlastSpells::SpellLavaBlastTriggerMissile, true);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_spell_lava_blast_AuraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_spell_lava_blast_AuraScript();
        }
};

/// Barbed Arrow Barrage - 164370
class iron_docks_spell_barbed_arrow_aura : public SpellScriptLoader
{
    public:

        iron_docks_spell_barbed_arrow_aura() : SpellScriptLoader("iron_docks_spell_barbed_arrow_aura") { }

        class iron_docks_auraScript : public AuraScript
        {
            PrepareAuraScript(iron_docks_auraScript);

            enum eBarbedarrowSpells
            {
                SpellBarbedArrowAreaTrigger = 164278
            };

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {     
                    if (l_Caster->IsAIEnabled)
                    {
                        if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 300.0f, true))
                            l_Caster->CastSpell(l_Target, eBarbedarrowSpells::SpellBarbedArrowAreaTrigger);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_auraScript::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_auraScript();
        }
};

/// Barbed Arrows - 164371 
class iron_docks_spell_barbed_arrow_dummy : public SpellScriptLoader
{
    public:

        iron_docks_spell_barbed_arrow_dummy() : SpellScriptLoader("iron_docks_spell_barbed_arrow_dummy") { }

        class iron_docks_spell_barbed_arrow_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(iron_docks_spell_barbed_arrow_dummy_SpellScript);

            enum eBarbedArrowSpells
            {
                SpellBarbedArrowAura = 164370
            };
            
            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, eBarbedArrowSpells::SpellBarbedArrowAura);
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(iron_docks_spell_barbed_arrow_dummy_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_APPLY_AURA);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spell_barbed_arrow_dummy_SpellScript();
        }
};

/// Trampling Stampede - 173384 
class iron_docks_spell_trampling_stampede : public SpellScriptLoader
{
    public:

    iron_docks_spell_trampling_stampede() : SpellScriptLoader("iron_docks_spell_trampling_stampede") { }

    class iron_docks_spell_trampling_stampede_SpellScript : public SpellScript
    {
        PrepareSpellScript(iron_docks_spell_trampling_stampede_SpellScript);

        enum eTramplingStampedeSpells
        {
            SpellClefthoofStampedeVisualMovement = 173350,
            SpellClefthoofStampedeVisualHorning = 173351,
            SpellClefthoofStampedeVisualJump = 173352,
            SpellClefthoofStampedeDummyCast = 173384,
            SpellClefthoofStampedeDamage = 173351,
            SpellClefthoofTrampled = 173349
        };

        void HandleBeforeCast()
        {
            if (GetCaster())
                GetCaster()->AddAura(eTramplingStampedeSpells::SpellClefthoofStampedeVisualMovement, GetCaster());
        }

        void HandleDummy(SpellEffIndex p_EffIndex)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                {
                    Position l_Position;
                    l_Target->GetPosition(&l_Position);
                    l_Caster->GetMotionMaster()->MoveCharge(l_Position.GetPositionX(), l_Position.GetPositionY(), l_Position.GetPositionZ(), 42.0f, eMovementInformed::MovementInformedClefthoofTargetArrivDest);
                }
            }
        }

        void Register() override
        {
            BeforeCast += SpellCastFn(iron_docks_spell_trampling_stampede_SpellScript::HandleBeforeCast);
            OnEffectLaunch += SpellEffectFn(iron_docks_spell_trampling_stampede_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new iron_docks_spell_trampling_stampede_SpellScript();
    }
};

/// Burning Arrows - 172810
class iron_docks_spell_burning_arrow_aura : public SpellScriptLoader
{
    public:

        iron_docks_spell_burning_arrow_aura() : SpellScriptLoader("iron_docks_spell_burning_arrow_aura") { }

        class iron_docks_Spells : public AuraScript
        {
            PrepareAuraScript(iron_docks_Spells);

            enum eBurningArrowSpells
            {
                SpellBurningArrowDummy       = 172810,
                SpellBurningArrowAreaTrigger = 164234,
                SpellBurningArrowDoT         = 164632
            };

            void HandlePeriodic(AuraEffect const* /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->IsAIEnabled)
                    {
                        if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 300.0f, true))
                            l_Caster->CastSpell(l_Target, eBurningArrowSpells::SpellBurningArrowAreaTrigger);
                    }
                }
            }

            void Register() override
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(iron_docks_Spells::HandlePeriodic, SpellEffIndex::EFFECT_0, AuraType::SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const override
        {
            return new iron_docks_Spells();
        }
};

/// Charging Slash - 172885
class iron_docks_spell_charging_slash_effect : public SpellScriptLoader
{
    public:

        iron_docks_spell_charging_slash_effect() : SpellScriptLoader("iron_docks_spell_charging_slash_effect") { }

        class spells_iron_docks_SpellScript : public SpellScript
        {
            PrepareSpellScript(spells_iron_docks_SpellScript);

            enum eChargingSlashSpells
            {
                SpellChargingSlash     = 172889,
                SpellChargingSlashJump = 172885
            };

            void OnHitTarget()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, eChargingSlashSpells::SpellChargingSlash);
                }
            }

            void Register() override
            {
                OnHit += SpellHitFn(spells_iron_docks_SpellScript::OnHitTarget);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spells_iron_docks_SpellScript();
        }
};

/// Lava Barrage Dummy - 176356
class iron_docks_spell_lava_barrage_dummy : public SpellScriptLoader
{
    public:

        iron_docks_spell_lava_barrage_dummy() : SpellScriptLoader("iron_docks_spell_lava_barrage_dummy") { }

        class iron_docks_spells : public SpellScript
        {
            PrepareSpellScript(iron_docks_spells);

            enum eLavaBarrageSpells
            {
                SpellLavaBarrageDummy       = 176356,
                SpellLavaBarrageAreaTrigger = 173482,
                SpellLavaBarrageDoT         = 173489
            };

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->IsAIEnabled)
                    {
                        for (uint8 l_I = 0; l_I < 2; l_I++)
                        {
                            if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                                l_Caster->CastSpell(l_Target, eLavaBarrageSpells::SpellLavaBarrageAreaTrigger, true);
                        }
                    }
                }
            }

            void Register() override
            {
                OnEffectLaunch += SpellEffectFn(iron_docks_spells::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spells();
        }
};

/// Burning Arrows - 164628
class iron_docks_area_trigger_burning_arrow : public AreaTriggerEntityScript
{
    public:

        iron_docks_area_trigger_burning_arrow() : AreaTriggerEntityScript("iron_docks_area_trigger_burning_arrow") { }

        std::list<uint64> m_Targets;
        uint32 m_Timer = 1 * TimeConstants::IN_MILLISECONDS;

        enum eBurningArrowSpells
        {
            SpellBurningArrowDummy       = 172810,
            SpellBurningArrowAreaTrigger = 164234,
            SpellBurningArrowDoT         = 164632
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);
                if (l_PlayerList.empty())
                    return;

                for (auto l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eBurningArrowSpells::SpellBurningArrowDoT))
                    {
                        l_Itr->CastSpell(l_Itr, eBurningArrowSpells::SpellBurningArrowDoT, true);
                        m_Targets.push_back(l_Itr->GetGUID());
                    }
                }

                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Targets.empty())
                return;

            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eBurningArrowSpells::SpellBurningArrowDoT))
                    l_Target->RemoveAura(eBurningArrowSpells::SpellBurningArrowDoT);
            }
        }

        iron_docks_area_trigger_burning_arrow* GetAI() const override
        {
            return new iron_docks_area_trigger_burning_arrow();
        }
};

/// Lava Barrage - 173484
class iron_docks_area_trigger_lava_barrage_effect : public AreaTriggerEntityScript
{
    public:

        iron_docks_area_trigger_lava_barrage_effect() : AreaTriggerEntityScript("iron_docks_area_trigger_lava_barrage_effect") { }

        enum eBarrageLavaSpells
        {
            /// Ironwing Flamespitter
            SpellLavaBarrageDummy       = 176356,
            SpellLavaBarrageAreaTrigger = 173482,
            SpellLavaBarrageDot         = 173489
        };

        std::list<uint64> m_Targets;
        uint32 m_Timer = 500;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);
                if (l_PlayerList.empty())
                    return;

                for (auto l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eBarrageLavaSpells::SpellLavaBarrageDot))
                    {
                        l_Itr->CastSpell(l_Itr, eBarrageLavaSpells::SpellLavaBarrageDot);
                        m_Targets.push_back(l_Itr->GetGUID());
                    }
                }

                m_Timer = 500;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Targets.empty())
                return;

            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eBarrageLavaSpells::SpellLavaBarrageDot))
                    l_Target->RemoveAura(eBarrageLavaSpells::SpellLavaBarrageDot);
            }
        }

        iron_docks_area_trigger_lava_barrage_effect* GetAI() const override
        {
            return new iron_docks_area_trigger_lava_barrage_effect();
        }
};

/// Barbed Arrow Barrage - 164278
class iron_docks_area_trigger_barbed_arrow : public AreaTriggerEntityScript
{
    public:

        iron_docks_area_trigger_barbed_arrow() : AreaTriggerEntityScript("iron_docks_area_trigger_barbed_arrow") { }
 
        enum eBarbedArrowSpells
        {
            SpellBarbedArrowAreaTrigger = 164278,
            SpellBarbedArrowAura        = 164370,
            SpellBarbedArrowDot         = 164648
        };

        uint32 m_Timer = 500;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);
                if (!l_PlayerList.empty())
                {
                    for (Player* l_Itr : l_PlayerList)
                    {
                       l_Itr->CastSpell(l_Itr, eBarbedArrowSpells::SpellBarbedArrowDot);    
                    }
                }

                m_Timer = 500;
            }
            else
                m_Timer -= p_Time;
        }

        iron_docks_area_trigger_barbed_arrow* GetAI() const override
        {
            return new iron_docks_area_trigger_barbed_arrow();
        }
};

/// Grease Vial - 172629 
class iron_docks_area_trigger_oil_effect : public AreaTriggerEntityScript
{
    public:

        iron_docks_area_trigger_oil_effect() : AreaTriggerEntityScript("iron_docks_area_trigger_oil_effect") { }
      
        enum eGreaseVialSpells
        {
            SpellGreaseVial       = 172649,
            SpellGreaseVialEffect = 172636,
            SpellKnockedDown      = 172631
        };

        std::list<uint64> m_Targets;
        uint32 m_Timer = 2 * TimeConstants::IN_MILLISECONDS;

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);
                if (l_PlayerList.empty())
                    return;

                for (auto l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eGreaseVialSpells::SpellGreaseVialEffect)) // Visual only
                    {
                        l_Itr->CastSpell(l_Itr, eGreaseVialSpells::SpellGreaseVialEffect);
                        m_Targets.push_back(l_Itr->GetGUID());               
                    }

                    if (l_Itr->HasUnitState(UnitState::UNIT_STATE_CASTING)) // Aura effect hardcoded
                        l_Itr->CastSpell(l_Itr, eGreaseVialSpells::SpellKnockedDown);
                }

                m_Timer = 2 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Targets.empty())
                return;

            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eGreaseVialSpells::SpellGreaseVialEffect))
                    l_Target->RemoveAura(eGreaseVialSpells::SpellGreaseVialEffect);
            }
        }

        iron_docks_area_trigger_oil_effect* GetAI() const override
        {
            return new iron_docks_area_trigger_oil_effect();
        }
};

/// Jagget Caltrops - 173325
class iron_docks_area_trigger_jagged_caltrops : public AreaTriggerEntityScript
{
    public:

        iron_docks_area_trigger_jagged_caltrops() : AreaTriggerEntityScript("iron_docks_area_trigger_jagged_caltrops") { }

        enum eCaltTrapsSpells
        {
            SpellCultTraps       = 173336,
            SpellCultTrapsDamage = 173324
        };

        std::list<uint64> m_Targets;
        uint32 m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Timer <= p_Time)
            {
                std::list<Player*> l_PlayerList;
                JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
                JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
                p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);
                if (!l_PlayerList.empty())
                {

                    for (Player* l_Itr : l_PlayerList)
                    {
                        if (!l_Itr->HasAura(eCaltTrapsSpells::SpellCultTrapsDamage))
                        {
                            l_Itr->CastSpell(l_Itr, eCaltTrapsSpells::SpellCultTrapsDamage);
                            m_Targets.push_back(l_Itr->GetGUID());
                        }
                    }
                }

                m_Timer = 1 * TimeConstants::IN_MILLISECONDS;
            }
            else
                m_Timer -= p_Time;
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
        {
            if (m_Targets.empty())
                return;

            for (uint64 l_Guid : m_Targets)
            {
                Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
                if (l_Target && l_Target->HasAura(eCaltTrapsSpells::SpellCultTrapsDamage))
                    l_Target->RemoveAura(eCaltTrapsSpells::SpellCultTrapsDamage);
            }
        }

        iron_docks_area_trigger_jagged_caltrops* GetAI() const override
        {
            return new iron_docks_area_trigger_jagged_caltrops();
        }
};

/// Lava Blast - 173518 
class iron_docks_area_trigger_lava_blast : public AreaTriggerEntityScript
{
public:

    iron_docks_area_trigger_lava_blast() : AreaTriggerEntityScript("iron_docks_area_trigger_lava_blast") { }

    enum eCaltTrapsSpells
    {
        SpellLavaBlastDot = 173517
    };

    std::list<uint64> m_Targets;
    uint32 m_Timer = 500;
    void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Timer <= p_Time)
        {
            std::list<Player*> l_PlayerList;
            JadeCore::AnyPlayerInObjectRangeCheck l_Check(p_AreaTrigger, 2.0f);
            JadeCore::PlayerListSearcher<JadeCore::AnyPlayerInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_PlayerList, l_Check);
            p_AreaTrigger->VisitNearbyObject(2.0f, l_Searcher);
            if (!l_PlayerList.empty())
            {
                for (Player* l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eCaltTrapsSpells::SpellLavaBlastDot))
                    {
                        l_Itr->CastSpell(l_Itr, eCaltTrapsSpells::SpellLavaBlastDot);
                        m_Targets.push_back(l_Itr->GetGUID());
                    }
                }
            }

            m_Timer = 500;
        }
        else
            m_Timer -= p_Time;
    }

    void OnRemove(AreaTrigger* p_AreaTrigger, uint32 p_Time) override
    {
        if (m_Targets.empty())
            return;

        for (uint64 l_Guid : m_Targets)
        {
            Unit* l_Target = Unit::GetUnit(*p_AreaTrigger, l_Guid);
            if (l_Target && l_Target->HasAura(eCaltTrapsSpells::SpellLavaBlastDot))
                l_Target->RemoveAura(eCaltTrapsSpells::SpellLavaBlastDot);
        }
    }

    iron_docks_area_trigger_lava_blast* GetAI() const override
    {
        return new iron_docks_area_trigger_lava_blast();
    }
};

void AddSC_iron_docks()
{
    new iron_docks_mob_gromkar_battlemaster(); /// 83025
    new iron_docks_mob_gromkar_deadeye(); /// 83028 / 83764
    new iron_docks_mob_gromkar_footsoldier(); /// 83765
    new iron_docks_mob_gromkar_incinerator(); /// 86809
    new iron_docks_mob_gromkar_technician(); /// 81432
    new iron_docks_mob_siege_master_olugar(); /// 83026
    new iron_docks_mob_champion_darona(); /// 81603
    new iron_docks_mob_gwarnok(); /// 84520
    new iron_docks_mob_iron_star(); /// 812477
    new iron_docks_mob_iron_star_vehicle(); /// 81247
    new iron_docks_mob_laborer_ogron(); /// 83578
    new iron_docks_mob_gromkar_deck_hand(); /// 83762
    new iron_docks_mob_gromkar_technician_deckhand_leader(); /// 83763
    new iron_docks_mob_rampaging_clefthoof(); /// 83392
    new iron_docks_mob_ironwing_flamespitter(); /// 83389
    new iron_docks_mob_hammer_stalker(); /// 86534
    new iron_docks_mob_archery_target(); /// 79423
    new iron_docks_spell_flurry_periodic(); /// 178412
    new iron_docks_spell_charge_forward(); /// 167299
    new iron_docks_spell_barbed_arrow_aura(); /// 164370
    new iron_docks_spell_barbed_arrow_dummy(); /// 172810
    new iron_docks_spell_burning_arrow_aura(); /// 172810
    new iron_docks_spell_charging_slash_effect(); /// 172885
    new iron_docks_spell_lava_barrage_dummy(); /// 176356
    new iron_docks_spell_lava_blast(); /// 173514
    new iron_docks_spell_trampling_stampede(); /// 173384
    new iron_docks_area_trigger_burning_arrow();/// 177642 
    new iron_docks_area_trigger_lava_barrage_effect();///176358 
    new iron_docks_area_trigger_barbed_arrow(); ///164278 
    new iron_docks_area_trigger_oil_effect();///172629
    new iron_docks_area_trigger_jagged_caltrops();///173325 
    new iron_docks_area_trigger_lava_blast();///173518
}