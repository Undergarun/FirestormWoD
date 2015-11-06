////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "iron_docks.hpp"

enum eSpells
{
    /// Misc
    SpellSelfStun                       = 94563,
    SpellEmoteWork                      = 43831,
    SpellCarryCrate                     = 173166,
    SpellCarrySack                      = 167539,
    SpellApplauseCheer                  = 84062
};

enum eEmotes
{
    EmoteWork01                         = 133,
    EmoteFight                           = 36, 
};

enum eAction
{
    ActionQuietDeath = 744
};

class basicevent_explosion_iron_star : public BasicEvent
{
    public:
        explicit basicevent_explosion_iron_star(Unit* p_Me, uint64 p_Unit, uint64 p_OtherUnit, int p_Value) : m_Me(p_Me), m_ObjGUID(p_Unit), m_OtherObjGUID(p_OtherUnit), m_Modifier(p_Value) { }

        enum eExplosionIronStarEventSpells
        {
            /// Iron Star - Object
            SpellQuietSuicide = 163832
        };

        bool Execute(uint64 /*p_CurrTime*/, uint32 /*p_Diff*/)
        {
            if (!m_Me)
                return false;

            if (!m_Me->IsInWorld())
                return false;

            switch (m_Modifier)
            {
                case 0:
                {
                    if (m_ObjGUID != 0)
                    {
                        if (Creature* l_Creature = Creature::GetCreature(*m_Me, m_ObjGUID))
                        {
                            l_Creature->DespawnOrUnsummon();
                            l_Creature->CastSpell(l_Creature, eExplosionIronStarEventSpells::SpellQuietSuicide);
                        }
                    }

                    break;
                }
                case 1:
                {
                    if (m_ObjGUID != 0 && m_OtherObjGUID != 0)
                    {
                        if (Unit* l_Unit = Unit::GetUnit(*m_Me, m_OtherObjGUID))
                        {
                            if (Creature* l_Creature = Creature::GetCreature(*m_Me, m_ObjGUID))
                            {
                                l_Unit->EnterVehicle(l_Creature, 0, true);
                            }
                        }
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }

    private:
        Unit* m_Me;
        uint64 m_ObjGUID;
        uint64 m_OtherObjGUID;
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
                SpellChainDrag         = 172860,
                SpellChargingSlash     = 172889,
                SpellChargingSlashJump = 172885
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
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
                            events.ScheduleEvent(eGromkarEvents::EventBladestorm,  35 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eGromkarEvents::EventChainDrag:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
                                me->CastSpell(l_Target, eGromkarSpells::SpellChainDrag);
                            events.ScheduleEvent(eGromkarEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
                            break;
                    case eGromkarEvents::EventChargingSlash:
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0f, true))
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
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (Creature* l_ArcheryTarget = me->FindNearestCreature(eIronDocksCreatures::TriggerArcheryTarget, 30.0f))
                        {
                            l_ArcheryTarget->setFaction(1);
                            me->CastSpell(l_ArcheryTarget, eDeadeyeSpells::SpellIronShot);
                        }

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
                SpellChargingSlashJump = 172885
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Who) override
            {
                events.ScheduleEvent(eFootSoldierEvents::EventTacticalKick, urand(10 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eFootSoldierEvents::EventDemoralizingShout, 8 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eFootSoldierEvents::EventChargingSlash, 25 * TimeConstants::IN_MILLISECONDS);

                me->RemoveAura(eSpells::SpellSelfStun);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
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
                        if (Creature* l_ArcheryTarget = me->FindNearestCreature(eIronDocksCreatures::TriggerArcheryTarget, 30.0f))
                        {
                            l_ArcheryTarget->setFaction(1);
                            me->CastSpell(l_ArcheryTarget, eIncienratorSpells::SpellIronShot);

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

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_VisualTimer;

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

            void Reset() override
            {
                events.Reset();
                m_VisualTimer = 6 * TimeConstants::IN_MILLISECONDS;
            }

            void EnterCombat(Unit* p_Who) override
            {
                me->RemoveAura(eSpells::SpellEmoteWork);
                me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);
                events.ScheduleEvent(eTechnicianEvents::EventGreaseVial, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eTechnicianEvents::EventFlyingHammer, urand(6 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eTechnicianEvents::EventExplosiveGrenade, 20 * TimeConstants::IN_MILLISECONDS);
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
                    p_Who->Kill(me);
            }

            void UpdateAI(uint32 const p_Diff) override
            {
                if (!UpdateVictim())
                {
                    if (m_VisualTimer <= p_Diff)
                    {
                        if (me->HasAura(SpellEmoteWork))
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
                m_VisualTimer = 16 * TimeConstants::IN_MILLISECONDS;
                me->SetReactState(ReactStates::REACT_DEFENSIVE);
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
                SpellBarbedArrowDoT          = 164648,
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
                /// I don't think Darona actually shot barber arrows, wowhead is stupid. Icy veins says it aswell.
                events.ScheduleEvent(eDorunaEvents::EventBurningArrow, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eDorunaEvents::EventBarbedArrow, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
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
                        me->CastSpell(me, eDorunaSpells::SpellBurningArrowDummy);
                        events.ScheduleEvent(eDorunaEvents::EventBurningArrow, 30 * TimeConstants::IN_MILLISECONDS);
                        break;
                    }
                    case eDorunaEvents::EventBarbedArrow:
                    {
                        me->CastSpell(me, eDorunaSpells::SpellBarbedArrowBarrageDummy);
                        me->AddAura(eDorunaSpells::SpellBarbedArrowAura, me);
                        events.ScheduleEvent(eDorunaEvents::EventBarbedArrow, 15 * TimeConstants::IN_MILLISECONDS);
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
                SpellChainDrag          = 172860,
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
                events.ScheduleEvent(eGwarnokEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eGwarnokEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eGwarnokEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));

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
                            events.ScheduleEvent(eGwarnokEvents::EventBladestorm, urand(20 * TimeConstants::IN_MILLISECONDS, 25 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eGwarnokEvents::EventChargingSlash:
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                                me->CastSpell(l_Target, eGwarnokSpells::SpellChargingSlashJump);
                            events.ScheduleEvent(eGwarnokEvents::EventChargingSlash, urand(5 * TimeConstants::IN_MILLISECONDS, 15 * TimeConstants::IN_MILLISECONDS));
                            break;
                        }
                    case eGwarnokEvents::EventChainDrag:
                        {
                            if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 50.0F, true))
                            {
                                me->CastSpell(l_Target, eGwarnokSpells::SpellChainDrag);
                                l_Target->GetMotionMaster()->MoveJump(*me, 8.0f, 5.0f, 10.0f);
                            }
                            events.ScheduleEvent(eGwarnokEvents::EventChainDrag, urand(20 * TimeConstants::IN_MILLISECONDS, 30 * TimeConstants::IN_MILLISECONDS));
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
class iron_docks_mob_iron_star : public CreatureScript
{
    public:

        iron_docks_mob_iron_star() : CreatureScript("iron_docks_mob_iron_star") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) {}

            void Reset() override
            {
                events.Reset();
                me->setFaction(FriendlyFaction);
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC | eUnitFlags::UNIT_FLAG_IMMUNE_TO_NPC);
                me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
            }

            void DoAction(int32 const p_Action) override
            {
                switch (p_Action)
                {
                    case eAction::ActionQuietDeath:
                        me->SetSpeed(UnitMoveType::MOVE_RUN, 12.0f, true);
                        me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FORCE_MOVEMENT);
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

/// Ogron Laborer - 83578
/// Ogron Laborer - 83761
/// Ogron Laborer - 583578
class iron_docks_mob_ogron : public CreatureScript
{
    public:

        iron_docks_mob_ogron() : CreatureScript("iron_docks_mob_ogron") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                SpellThunderingStomp = 173135
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eOrgonEvents::EventThunderingStomp, 20 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eOrgonEvents::EventFlurry, urand(8 * TimeConstants::IN_MILLISECONDS, 13 * TimeConstants::IN_MILLISECONDS));

                if (!roll_chance_i(30))
                    me->MonsterYell("CRUSH THEM!!", Language::LANG_UNIVERSAL, me->GetGUID());
            }

            void MoveInLineOfSight(Unit* p_Who) override
            {
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
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
            return new mob_iron_docksAI(p_Creature);
        }
};

/// Grom'kar Deck-Hand - 83697
/// Grom'kar Deck-Hand - 83762
class iron_docks_mob_gromkar_deck_hand : public CreatureScript
{
    public:

        iron_docks_mob_gromkar_deck_hand() : CreatureScript("iron_docks_mob_gromkar_deck_hand") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

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
                if (p_Who && p_Who->IsInWorld() && p_Who->GetTypeId() != TypeID::TYPEID_PLAYER && p_Who->GetEntry() == eIronDocksCreatures::NpcIronStar && me->IsWithinDistInMap(p_Who, 1.2f))
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
                            events.ScheduleEvent(eDeckHandEvents::EventHatchetToss, urand(8 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
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

/// Thunderlord Wrangler - 83390
class iron_docks_mob_thundering_wandler : public CreatureScript
{
    public:

        iron_docks_mob_thundering_wandler() : CreatureScript("iron_docks_mob_thundering_wandler") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eThunderingWandlerEvents
            {
                /// Thunderlord Wrangler
                SpellCultTraps       = 173336,
                SpellCultTrapsDamage = 173324,
                SpellRendingCleave   = 167815,
                SpellSpearThrow      = 167095
            };

            enum eThunderWandlerEvents
            {
                /// Thunderlord Wrangler
                EventCultTraps = 1,
                EventSpearThrow,
                EventRendingCleave         
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eThunderWandlerEvents::EventCultTraps, 18 * TimeConstants::IN_MILLISECONDS);
                events.ScheduleEvent(eThunderWandlerEvents::EventSpearThrow, urand(6 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                events.ScheduleEvent(eThunderWandlerEvents::EventRendingCleave, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
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
                    case eThunderWandlerEvents::EventCultTraps:
                            if (Unit* l_Target = me->getVictim())
                            {
                                me->CastSpell(l_Target, eThunderingWandlerEvents::SpellCultTraps, true);
                                me->GetMotionMaster()->MoveKnockbackFrom(me->GetPositionX(), me->GetPositionY(), 10.0f, 8.0f);
                            }
                            events.ScheduleEvent(eThunderWandlerEvents::EventCultTraps, 18 * TimeConstants::IN_MILLISECONDS);
                            break;
                    case eThunderWandlerEvents::EventSpearThrow:
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eThunderingWandlerEvents::SpellSpearThrow);
                            events.ScheduleEvent(eThunderWandlerEvents::EventSpearThrow, urand(6 * TimeConstants::IN_MILLISECONDS, 10 * TimeConstants::IN_MILLISECONDS));
                            break;
                    case eThunderWandlerEvents::EventRendingCleave:
                            if (Unit* l_Target = me->getVictim())
                                me->CastSpell(l_Target, eThunderingWandlerEvents::SpellRendingCleave);
                            events.ScheduleEvent(eThunderWandlerEvents::EventRendingCleave, urand(8 * TimeConstants::IN_MILLISECONDS, 12 * TimeConstants::IN_MILLISECONDS));
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
                EventClefthoofStampede = 1,
                EventClefthoofStampede2
            };

            enum eClefthoofSpells
            {
                /// Rushing
                SpellClefthoofStampedeVisualMovement = 173350,
                SpellClefthoofStampedeVisualHorning  = 173351,
                SpellClefthoofStampedeVisualJump     = 173352,
                SpellClefthoofStampedeDummyCast      = 173384,
                SpellClefthoofStampedeDamage         = 173351,
                SpellClefthoofSpinyHorns             = 158349
            };

            uint64 m_StampedeGuid;

            void Reset() override
            {
                events.Reset();
                m_StampedeGuid = 0;
                me->AddAura(eClefthoofSpells::SpellClefthoofSpinyHorns, me);
            }

            void EnterCombat(Unit* p_Attacker) override
            {
                events.ScheduleEvent(eClefthoofEvents::EventClefthoofStampede, urand(5 * TimeConstants::IN_MILLISECONDS, 9 * TimeConstants::IN_MILLISECONDS));
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
                    case eClefthoofEvents::EventClefthoofStampede:
                        {
                            ///
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
class iron_docks_mob_drake : public CreatureScript
{
    public:

        iron_docks_mob_drake() : CreatureScript("iron_docks_mob_drake") { }

        struct mob_iron_docksAI : public ScriptedAI
        {
            mob_iron_docksAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eIronwingFlamespitterEvents
            {
                /// Ironwing Flamespitter
                EventLavaBarrage = 1
            };

            enum eIronwingFlamespitterSpells
            {
                /// Ironwing Flamespitter
                SpellLavaBarrageDummy       = 176356,
                SpellLavaBarrageAreaTrigger = 173482,
                SpellLavaBarrageDot         = 173489
            };

            void Reset() override
            {
                events.Reset();
            }

            void EnterCombat(Unit* p_Attacker) override
            {
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

            void HandlePeriodic(constAuraEffectPtr p_AurEff)
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

            void HandleCharge()
            {
                // will recode from scratch
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

            void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
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

/// Burning Arrows - 172810
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
                OnEffectLaunch += SpellEffectFn(iron_docks_spell_barbed_arrow_dummy_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new iron_docks_spell_barbed_arrow_dummy_SpellScript();
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

            void HandlePeriodic(constAuraEffectPtr /*p_AurEff*/)
            {
                PreventDefaultAction();

                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->IsAIEnabled)
                    {
                        if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 300.0f, true))
                        {
                            l_Caster->CastSpell(l_Target, eBurningArrowSpells::SpellBurningArrowAreaTrigger);
                        }
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

/// Lava Barrage Dummy - 
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
                    if (!l_Caster->IsAIEnabled)
                    {
                        if (Unit* l_Target = l_Caster->GetAI()->SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                            l_Caster->CastSpell(l_Target, eLavaBarrageSpells::SpellLavaBarrageAreaTrigger, true);
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
        uint32 m_Timer = 1 * TimeConstants::IN_MILLISECONDS;

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
            SpellBarbedArrowDoT         = 164648
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
                if (l_PlayerList.empty())
                    return;

                for (auto l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eBarbedArrowSpells::SpellBarbedArrowDoT))
                    {
                        l_Itr->CastSpell(l_Itr, eBarbedArrowSpells::SpellBarbedArrowDoT);
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
                if (l_Target && l_Target->HasAura(eBarbedArrowSpells::SpellBarbedArrowDoT))
                    l_Target->RemoveAura(eBarbedArrowSpells::SpellBarbedArrowDoT);
            }
        }

        iron_docks_area_trigger_barbed_arrow* GetAI() const override
        {
            return new iron_docks_area_trigger_barbed_arrow();
        }
};

/// Grease Vial Areatrigger - 
class iron_docks_area_trigger_oil_effect : public AreaTriggerEntityScript
{
    public:

        iron_docks_area_trigger_oil_effect() : AreaTriggerEntityScript("iron_docks_area_trigger_oil_effect") { }
      
        enum eGreaseVialSpells
        {
            SpellGreaseVial       = 172649,
            SpellGreaseVialEffect = 172636
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
                if (l_PlayerList.empty())
                    return;

                for (auto l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eGreaseVialSpells::SpellGreaseVialEffect))
                    {
                        l_Itr->CastSpell(l_Itr, eGreaseVialSpells::SpellGreaseVialEffect);
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
                if (l_PlayerList.empty())
                    return;

                for (auto l_Itr : l_PlayerList)
                {
                    if (!l_Itr->HasAura(eCaltTrapsSpells::SpellCultTrapsDamage))
                    {
                        l_Itr->CastSpell(l_Itr, eCaltTrapsSpells::SpellCultTrapsDamage);
                        m_Targets.push_back(l_Itr->GetGUID());

                        if (l_Itr->HasAura(eCaltTrapsSpells::SpellCultTrapsDamage))
                        {
                            if (AuraPtr l_Aura = l_Itr->GetAura(eCaltTrapsSpells::SpellCultTrapsDamage))
                                l_Aura->SetDuration(1);
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

void AddSC_iron_docks()
{
    /// NPCs
    new iron_docks_mob_gromkar_battlemaster();
    new iron_docks_mob_gromkar_deadeye();
    new iron_docks_mob_gromkar_footsoldier();
    new iron_docks_mob_gromkar_incinerator();
    new iron_docks_mob_gromkar_technician();
    new iron_docks_mob_siege_master_olugar();
    new iron_docks_mob_champion_darona();
    new iron_docks_mob_gwarnok();
    new iron_docks_mob_iron_star();
    new iron_docks_mob_ogron();
    new iron_docks_mob_gromkar_deck_hand();
    new iron_docks_mob_gromkar_technician_deckhand_leader();
    new iron_docks_mob_thundering_wandler();
    new iron_docks_mob_rampaging_clefthoof();
    new iron_docks_mob_drake();
    /// Spells
    new iron_docks_spell_flurry_periodic();
    new iron_docks_spell_charge_forward();
    new iron_docks_spell_barbed_arrow_aura();
    new iron_docks_spell_barbed_arrow_dummy();
    new iron_docks_spell_burning_arrow_aura();
    new iron_docks_spell_charging_slash_effect();
    new iron_docks_spell_lava_barrage_dummy();
    /// AreaTriggers
    new iron_docks_area_trigger_burning_arrow();
    new iron_docks_area_trigger_lava_barrage_effect();
    new iron_docks_area_trigger_barbed_arrow();
    new iron_docks_area_trigger_oil_effect();
    new iron_docks_area_trigger_jagged_caltrops();
}