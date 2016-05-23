////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "grimrail_depot.hpp"

enum eSpells
{

};

enum eMovementInformed
{
    MovementInformedDashSecondCut = 1,
    MovementInformedDashFinish
};

enum eEmotes
{
};

enum eAction
{
    ActionActivateDashEffect = 1
};

/// Assault Cannon - 79548
class grimrail_depot_mob_assault_cannon : public CreatureScript
{
public:

    grimrail_depot_mob_assault_cannon() : CreatureScript("grimrail_depot_mob_assault_cannon") { }

    struct grimrail_depot_mob_assault_cannonAI : public ScriptedAI
    {
        grimrail_depot_mob_assault_cannonAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eAssaultCannonEvents
        {
            EventOverHeatedBlast = 1,
            EventReloading,
            EventSuppressiveFire
        };

        enum eAssaultCannonSpells
        {
            SpellOverHeatedBlast = 167589,
            SpellReloading       = 160680,
            SpellSuppressiveFire = 160681
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_assault_cannonAI(p_Creature);
    }
};

/// Grimrail Bombadier - 81407
class grimrail_depot_mob_grimrail_bombadier : public CreatureScript
{
public:

    grimrail_depot_mob_grimrail_bombadier() : CreatureScript("grimrail_depot_mob_grimrail_bombadier") { }

    struct grimrail_depot_mob_grimrail_bombadierAI : public ScriptedAI
    {
        grimrail_depot_mob_grimrail_bombadierAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGrimrailBombadierEvents
        {
            EventBlackrockBombs    = 1,
            EventDoubleSlash       = 2,
            EventHermoragingWounds = 3
        };

        enum eGrimrailBombadierSpells
        {
            SpellBlackrockBombDummy          = 164183,
            SpellBlackrockBombTriggerMissile = 164187,
            SpellBlackrockBombDamage         = 164188,
            SpellBlackrockSummon             = 168112,
            SpellDoubleSlash                 = 164218,
            SpellHermoragingWounds           = 164241
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGrimrailBombadierEvents::EventBlackrockBombs,    0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGrimrailBombadierEvents::EventDoubleSlash,       0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGrimrailBombadierEvents::EventHermoragingWounds, 0 * TimeConstants::IN_MILLISECONDS);
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
                case eGrimrailBombadierEvents::EventBlackrockBombs:
                    events.ScheduleEvent(eGrimrailBombadierEvents::EventBlackrockBombs, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                case eGrimrailBombadierEvents::EventDoubleSlash:
                    events.ScheduleEvent(eGrimrailBombadierEvents::EventDoubleSlash, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                case eGrimrailBombadierEvents::EventHermoragingWounds:
                    events.ScheduleEvent(eGrimrailBombadierEvents::EventHermoragingWounds,0 *TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_grimrail_bombadierAI(p_Creature);
    }
};

/// Grimrail Laborer - 81235
class grimrail_depot_mob_grimrail_laborer : public CreatureScript
{
public:

    grimrail_depot_mob_grimrail_laborer() : CreatureScript("grimrail_depot_mob_grimrail_laborer") { }

    struct grimrail_depot_mob_grimrail_laborerAI : public ScriptedAI
    {
        grimrail_depot_mob_grimrail_laborerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGrimrailLaborerEvents
        {
            EventHaymaker = 1,
        };

        enum eGrimrailLaborerSpells
        {
            SpellHaymaker = 170099
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGrimrailLaborerEvents::EventHaymaker, 6 * TimeConstants::IN_MILLISECONDS);
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
                case eGrimrailLaborerEvents::EventHaymaker:
                    if (Unit* l_Target = me->getVictim())
                        events.ScheduleEvent(eGrimrailLaborerEvents::EventHaymaker, 8 * TimeConstants::IN_MILLISECONDS);
                        break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_grimrail_laborerAI(p_Creature);
    }
};

/// Grimrail Overseer - 81212
class grimrail_depot_mob_grimrail_overseer : public CreatureScript
{
public:

    grimrail_depot_mob_grimrail_overseer() : CreatureScript("grimrail_depot_mob_grimrail_overseer") { }

    struct grimrail_depot_mob_grimrail_overseerAI : public ScriptedAI
    {
        grimrail_depot_mob_grimrail_overseerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGrimrailOverseerEvents
        {
            EventDash        = 1,
            EventHewingSwipe
        };

        enum eGrimrailOverseerSpells
        {
            SpellDashDummy   = 164168,
            SpellDashCharge  = 164170,
            SpellDashDamage  = 164171,
            SpellHewingSwipe = 164163
        };

        bool m_DashEffect;

        void Reset() override
        {
            events.Reset();
            m_DashEffect = false;
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGrimrailOverseerEvents::EventDash, 15 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGrimrailOverseerEvents::EventHewingSwipe, 6 * TimeConstants::IN_MILLISECONDS);
        }

        void MovementInform(uint32 /*p_Type*/, uint32 p_Id) override
        {
            switch (p_Id)
            {
                case eMovementInformed::MovementInformedDashSecondCut:
                    {
                        Position l_Position;
                        me->GetFirstCollisionPosition(l_Position, 2.0f, 0.0f);
                        me->GetMotionMaster()->MoveCharge(&l_Position, 24.0f, eMovementInformed::MovementInformedDashFinish);
                        break;
                    }
                case eMovementInformed::MovementInformedDashFinish:
                    m_DashEffect = false;
                    break;
                default:
                    break;
            }
        }

        void DoAction(int32 const p_Action) override
        {
            switch (p_Action)
            {
                case eAction::ActionActivateDashEffect:
                    m_DashEffect = true;
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (m_DashEffect) /// Handles dash damage.
            {
                std::list<Player*> l_ListPlayers;
                me->GetPlayerListInGrid(l_ListPlayers, 1.2f);
                if (!l_ListPlayers.empty())
                {
                    for (Player* l_Itr : l_ListPlayers)
                    {
                        l_Itr->CastSpell(l_Itr, eGrimrailOverseerSpells::SpellDashDamage, true);
                    }
                }
            }

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING) || m_DashEffect)
                return;

            switch (events.ExecuteEvent())
            {
                case eGrimrailOverseerEvents::EventDash:
                    if (Unit * l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO, 0, 100.0f, true))
                        me->CastSpell(l_Random, eGrimrailOverseerSpells::SpellDashDummy);
                    events.ScheduleEvent(eGrimrailOverseerEvents::EventDash, 15 * TimeConstants::IN_MILLISECONDS);
                    break;
                case eGrimrailOverseerEvents::EventHewingSwipe:
                    if (Unit* l_Target = me->getVictim())
                        me->CastSpell(l_Target, eGrimrailOverseerSpells::SpellHewingSwipe);
                    events.ScheduleEvent(eGrimrailOverseerEvents::EventHewingSwipe, 6 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_grimrail_overseerAI(p_Creature);
    }
};

/// Grimrail Scout <Blackfuse Company> - 82590
class grimrail_depot_mob_grimrail_scout : public CreatureScript
{
public:

    grimrail_depot_mob_grimrail_scout() : CreatureScript("grimrail_depot_mob_grimrail_scout") { }

    struct grimrail_depot_mob_grimrail_scoutAI : public ScriptedAI
    {
        grimrail_depot_mob_grimrail_scoutAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGrimrailScoutEvents
        {
            EventArcaneBlitz = 1,
            EventScoutingAHead = 2
        };

        enum eGrimrailScoutSpells
        {
            SpellArcaneBlitzAura            = 166397,
            SpellArcaneBlitzTriggerMissileA = 166398,
            SpellArcaneBlitzTriggerMissileB = 166399,
            SpellArcaneBlitzDamage          = 166404,
            SpellScoutingAHead              = 166350,
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGrimrailScoutEvents::EventArcaneBlitz,0 * TimeConstants::IN_MILLISECONDS);
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
                case eGrimrailScoutEvents::EventArcaneBlitz:
                    me->CastSpell(me, eGrimrailScoutSpells::SpellArcaneBlitzAura);
                    events.ScheduleEvent(eGrimrailScoutEvents::EventArcaneBlitz,0 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_grimrail_scoutAI(p_Creature);
    }
};

/// Grimrail Technician <Blackfuse Company> - 81236
class grimrail_depot_mob_grimrail_technician : public CreatureScript
{
public:

    grimrail_depot_mob_grimrail_technician() : CreatureScript("grimrail_depot_mob_grimrail_technician") { }

    struct grimrail_depot_mob_grimrail_technicianAI : public ScriptedAI
    {
        grimrail_depot_mob_grimrail_technicianAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGrimrailTechnicianEvents
        {
            Event50kVolts   = 1,
            EventActivating = 2
        };

        enum eGrimrailTechnicianSpells
        {
            Spell50kVolts   = 164192,
            SpellActivating = 163966,
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGrimrailTechnicianEvents::Event50kVolts,0 * TimeConstants::IN_MILLISECONDS);
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
                case eGrimrailTechnicianEvents::Event50kVolts:
                    if (Unit * l_Random = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM, 0, 100.0f, true))
                        me->CastSpell(l_Random, eGrimrailTechnicianSpells::Spell50kVolts);
                    events.ScheduleEvent(eGrimrailTechnicianEvents::Event50kVolts,0 *  TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_grimrail_technicianAI(p_Creature);
    }
};

/// Grom'kar Boomer - 79720
class grimrail_depot_mob_gromkar_boomer : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_boomer() : CreatureScript("grimrail_depot_mob_gromkar_boomer") { }

    struct grimrail_depot_mob_gromkar_boomerAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_boomerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarboomerEvents
        {
            EventBlackrockMortar = 1,
        };

        enum eGromkarboomerSpells
        {
            SpellBlackrockMortarTriggerMissile = 161258,
            SpellBlackrockMortarDamage         = 160963
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarboomerEvents::EventBlackrockMortar, 0 * TimeConstants::IN_MILLISECONDS);
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
                case eGromkarboomerEvents::EventBlackrockMortar:
                    events.ScheduleEvent(eGromkarboomerEvents::EventBlackrockMortar, 0 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_boomerAI(p_Creature);
    }
};

/// Grom'kar Capitan - 82597
class grimrail_depot_mob_gromkar_capitan : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_capitan() : CreatureScript("grimrail_depot_mob_gromkar_capitan") { }

    struct grimrail_depot_mob_gromkar_capitanAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_capitanAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarCapitanEvents
        {
            EventRecklessSlash = 1
        };

        enum eGromkarCapitanSpells
        {
            SpellRecklessSlash = 166380
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarCapitanEvents::EventRecklessSlash, 0 * TimeConstants::IN_MILLISECONDS);
        }

        void UpdateAI(const uint32 p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;

            switch (events.ExecuteEvent())
            {
                case eGromkarCapitanEvents::EventRecklessSlash:
                    events.ScheduleEvent(eGromkarCapitanEvents::EventRecklessSlash, 0 * TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }

        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_capitanAI(p_Creature);
    }
};

/// Grom'kar Cinderseer - 88163
class grimrail_depot_mob_gromkar_cinderseer : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_cinderseer() : CreatureScript("grimrail_depot_mob_gromkar_cinderseer") { }

    struct grimrail_depot_mob_gromkar_cinderseerAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_cinderseerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarCinderseerEvents
        {
            EventFlametongue = 1,
            EventLavaWreath  = 2
        };

        enum eGromkarCinderseerSpells
        {
            SpellFlametongueDummy          = 176031,
            SpellFlametongueTriggerMissile = 176032,
            SpellFlametongueAura           = 176033,
            SpellFlametongue               = 176039,
            SpellLavaWreath                = 176027,
            SpellLavaWreathDummy           = 176025
        };

        void Reset() override
       {
           events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarCinderseerEvents::EventFlametongue,0 * TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGromkarCinderseerEvents::EventLavaWreath, 0 * TimeConstants::IN_MILLISECONDS);
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
            case eGromkarCinderseerEvents::EventFlametongue:
                    events.ScheduleEvent(eGromkarCinderseerEvents::EventFlametongue, 0* TimeConstants::IN_MILLISECONDS);
                    break;
            case eGromkarCinderseerEvents::EventLavaWreath:
                    events.ScheduleEvent(eGromkarCinderseerEvents::EventLavaWreath, 0* TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_cinderseerAI(p_Creature);
    }
};

/// Grom'kar Far Seer - 82579
class grimrail_depot_mob_gromkar_farseer : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_farseer() : CreatureScript("grimrail_depot_mob_gromkar_farseer") { }

    struct grimrail_depot_mob_gromkar_farseerAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_farseerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarFarSeerEvents
        {
            EventFarSight    = 1,
            EventHealingRain = 2,
            EventStormShiled = 3,
            EventThunderZone = 4
        };

        enum eGromkarFarSeerSpells
        {
            SpellFarSight           = 166364,
            SpellHealingRain        = 166387,
            SpellHealingRainDummy   = 166388,
            SpellStormShieldAura    = 166335,
            SpellStormShieldDamage  = 166336,
            SpellThunderZoneAura    = 166340,
            SpellThunderZoneDummy   = 166346
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarFarSeerEvents::EventStormShiled, 0*TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGromkarFarSeerEvents::EventThunderZone, 0*TimeConstants::IN_MILLISECONDS);
            events.ScheduleEvent(eGromkarFarSeerEvents::EventHealingRain, 0*TimeConstants::IN_MILLISECONDS);
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
                case eGromkarFarSeerEvents::EventStormShiled:
                    events.ScheduleEvent(eGromkarFarSeerEvents::EventStormShiled, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                case eGromkarFarSeerEvents::EventHealingRain:
                {
                    if (Unit* l_FriendlyUnit = DoSelectLowestHpFriendly(85)) // heal
                        me->CastSpell(l_FriendlyUnit, eGromkarFarSeerSpells::SpellHealingRain);
                    events.ScheduleEvent(eGromkarFarSeerEvents::EventHealingRain, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                }
                case eGromkarFarSeerEvents::EventThunderZone:
                    events.ScheduleEvent(eGromkarFarSeerEvents::EventThunderZone, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_farseerAI(p_Creature);
    }
};

/// Grom'kar Grenadier - 79739
class grimrail_depot_mob_gromkar_grenadier : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_grenadier() : CreatureScript("grimrail_depot_mob_gromkar_grenadier") { }

    struct grimrail_depot_mob_gromkar_grenadierAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_grenadierAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarGrenadierEvents
        {
            EventBlackrockGrenade = 1
        };

        enum eGromkarGrenadierSpells
        {
            SpellBlackRockGrenade = 161150
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarGrenadierEvents::EventBlackrockGrenade,0 *TimeConstants::IN_MILLISECONDS);
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
                case eGromkarGrenadierEvents::EventBlackrockGrenade:
                    events.ScheduleEvent(eGromkarGrenadierEvents::EventBlackrockGrenade, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_grenadierAI(p_Creature);
    }
};

/// Grom'kar Gunner - 77483
class grimrail_depot_mob_gromkar_gunner : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_gunner() : CreatureScript("grimrail_depot_mob_gromkar_gunner") { }

    struct grimrail_depot_mob_gromkar_gunnerAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_gunnerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarGunnerEvents
        {
            EventShrapnelBlast = 1
        };

        enum eGromkarGunnerSpells
        {
            SpellShrapnelBlast = 160943
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarGunnerEvents::EventShrapnelBlast, 0*TimeConstants::IN_MILLISECONDS);
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
                case eGromkarGunnerEvents::EventShrapnelBlast:
                    events.ScheduleEvent(eGromkarGunnerEvents::EventShrapnelBlast, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_gunnerAI(p_Creature);
    }
};

/// Grom'kar Hulk - 80938
class grimrail_depot_mob_gromkar_hulk : public CreatureScript
{
public:

    grimrail_depot_mob_gromkar_hulk() : CreatureScript("grimrail_depot_mob_gromkar_hulk") { }

    struct grimrail_depot_mob_gromkar_hulkAI : public ScriptedAI
    {
        grimrail_depot_mob_gromkar_hulkAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eGromkarHulkEvents
        {
            EventGettingAngry = 1
        };

        enum eGromkarHulkSpells
        {
            SpellGettingAngry = 176023
        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
            events.ScheduleEvent(eGromkarHulkEvents::EventGettingAngry, 0*TimeConstants::IN_MILLISECONDS);
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
                case eGromkarHulkEvents::EventGettingAngry:
                    events.ScheduleEvent(eGromkarHulkEvents::EventGettingAngry, 0*TimeConstants::IN_MILLISECONDS);
                    break;
                default:
                    break;
            }
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_gromkar_hulkAI(p_Creature);
    }
};

/// Iron Infantry - 79888
class grimrail_depot_mob_iron_infantry : public CreatureScript
{
public:

    grimrail_depot_mob_iron_infantry() : CreatureScript("grimrail_depot_mob_iron_infantry") { }

    struct grimrail_depot_mob_iron_infantryAI : public ScriptedAI
    {
        grimrail_depot_mob_iron_infantryAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

        enum eIronInfantrySpells
        {

        };

        enum eIronInfantryEvents
        {

        };

        void Reset() override
        {
            events.Reset();
        }

        void EnterCombat(Unit* /*p_Attacker*/) override
        {
        }

        void UpdateAI(uint32 const p_Diff) override
        {
            if (!UpdateVictim())
                return;

            events.Update(p_Diff);

            if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                return;
        }
    };

    CreatureAI* GetAI(Creature* p_Creature) const override
    {
        return new grimrail_depot_mob_iron_infantryAI(p_Creature);
    }
};


/// Dash - 164168
class grimrail_depot_spell_dash_dummy : public SpellScriptLoader
{
public:
    grimrail_depot_spell_dash_dummy() : SpellScriptLoader("grimrail_depot_spell_dash_dummy") { }

    class grimrail_depot_spell_dash_dummy_SpellScript : public SpellScript
    {
        PrepareSpellScript(grimrail_depot_spell_dash_dummy_SpellScript)

        void HandleDummy(SpellEffIndex /*p_EffIndex*/)
        {
            if (Unit* l_Caster = GetCaster())
            {
                if (Unit* l_Target = GetExplTargetUnit())
                    l_Caster->GetMotionMaster()->MoveCharge(l_Target, 24.0f, eMovementInformed::MovementInformedDashSecondCut);

                if (l_Caster->IsAIEnabled)
                    l_Caster->GetAI()->DoAction(eAction::ActionActivateDashEffect);
            }
        }

        void Register()
        {
            OnEffectHitTarget += SpellEffectFn(grimrail_depot_spell_dash_dummy_SpellScript::HandleDummy, SpellEffIndex::EFFECT_0, SpellEffects::SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const override
    {
        return new grimrail_depot_spell_dash_dummy_SpellScript();
    }
};

#ifndef __clang_analyzer__
void AddSC_grimrail_depot()
{
    new grimrail_depot_mob_assault_cannon();          /// 79548
    new grimrail_depot_mob_grimrail_bombadier();      /// 81407
    new grimrail_depot_mob_grimrail_laborer();        /// 81235
    new grimrail_depot_mob_grimrail_overseer();       /// 81212
    new grimrail_depot_mob_grimrail_scout();          /// 82590
    new grimrail_depot_mob_grimrail_technician();     /// 81236
    new grimrail_depot_mob_gromkar_boomer();          /// 79720
    new grimrail_depot_mob_gromkar_capitan();         /// 82597
    new grimrail_depot_mob_gromkar_cinderseer();      /// 88163
    new grimrail_depot_mob_gromkar_farseer();         /// 82579
    new grimrail_depot_mob_gromkar_grenadier();       /// 79739
    new grimrail_depot_mob_gromkar_gunner();          /// 77483
    new grimrail_depot_mob_gromkar_hulk();            /// 80938
    new grimrail_depot_mob_iron_infantry();           /// 79888
}
#endif
