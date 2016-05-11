////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

# include "upper_blackrock_spire.hpp"

enum eSpells
{
    ///< Black Iron Grunt
    SPELL_DEVASTATE                 = 153832,
    SPELL_RALLYING_BANNER           = 153792,
    ///< Black Iron Leadbelcher
    SPELL_INCENDIARY_SHELL          = 153981,
    SPELL_RIFLE_SHOT                = 153974,
    ///< Sentry Cannon
    SPELL_CANNON_SHOT               = 154178,
    ///< Ragemaw Worg
    SPELL_BLACKROCK_RABIES          = 154017,
    SPELL_FRANTIC_MAULING           = 154039,
    ///< Black Iron Warcaster
    SPELL_BOLT_OF_STEEL             = 153642,
    SPELL_SHRAPNEL_STORM            = 153942,
    SPELL_SHRAPNEL_STORM_MISSILE    = 153941,
    ///< Black Iron Alchemist
    SPELL_DEBILITATING_RAY          = 155505,
    SPELL_REJUVENATING_SERUM        = 155498,
    ///< Drakonid Monstrosity
    SPELL_ERUPTION                  = 155037,
    SPELL_MONSTRUOUS_CLAWS          = 155032,
    ///< Black Iron Veteran
    SPELL_SUNDER_ARMOR              = 155581,
    SPELL_SHIELD_SMASH              = 155575,
    ///< Black Iron Dreadweaver
    SPELL_SHADOW_BOLT               = 155587,
    SPELL_SHADOW_BOLT_VOLLEY        = 155588,
    SPELL_VEIL_OF_SHADOW            = 155586,
    ///< Black Iron Summoner
    SPELL_FIREBALL                  = 155590,
    SPELL_FROST_NOVA                = 155589,
    SPELL_SUMMON_VETERAN            = 169151,
    SPELL_SUMMON_DREADWEAVER        = 169088,
    ///< Black Iron Elite
    SPELL_BERSERKER_CHARGE          = 155584,
    SPELL_BESTIAL_ROAR              = 139385,
    SPELL_INTIMIDATING_SHOUT        = 167259,
    ///< Black Iron Siegebreaker
    SPELL_SMASH                     = 155572,
    SPELL_FRENZY                    = 81173
};

enum eEvents
{
    ///< Black Iron Grunt
    EVENT_DEVASTATE = 1,
    EVENT_RALLYING_BANNER,
    ///< Black Iron Leadbelcher
    EVENT_INCENDIARY_SHELL,
    EVENT_RIFLE_SHOT,
    EVENT_ACTIVATE_CANON,
    ///< Sentry Cannon
    EVENT_CANNON_SHOT,
    EVENT_SAFETY_PROTOCOLS,
    ///< Ragemaw Worg
    EVENT_FRANTIC_MAULING,
    ///< Black Iron Warcaster
    EVENT_BOLT_OF_STEEL,
    EVENT_SHRAPNEL_STORM,
    ///< Rune Glow
    EVENT_CHECK_ADDS,
    ///< Black Iron Alchemist
    EVENT_DEBILITATING_RAY,
    EVENT_REJUVENATING_SERUM,
    ///< Drakonid Monstrosity
    EVENT_ERUPTION,
    ///< Black Iron Veteran
    EVENT_SUNDER_ARMOR,
    EVENT_SHIELD_SMASH,
    ///< Black Iron Dreadweaver
    EVENT_SHADOW_BOLT,
    EVENT_SHADOW_BOLT_VOLLEY,
    EVENT_VEIL_OF_SHADOW,
    ///< Black Iron Summoner
    EVENT_FIREBALL,
    EVENT_FROST_NOVA,
    EVENT_SUMMON_VETERAN,
    EVENT_SUMMON_DREADWEAVER,
    ///< Black Iron Elite
    EVENT_BERSERKER_CHARGE,
    EVENT_BESTIAL_ROAR,
    EVENT_INTIMIDATING_SHOUT,
    ///< Black Iron Siegebreaker
    EVENT_SMASH,
    EVENT_FRENZY
};

enum eActions
{
    ActionActivateLeeroyRes
};

enum eTalks
{
    TALK_IRON_GRUNT_AGGRO,      ///< Iron Horde, we have unwanted visitors! -- Help! We have intruders!
    TALK_IRON_GRUNT_NEAR_DEATH  ///< We need backup!
};

///< Black Iron Grunt - 76179
class mob_black_iron_grunt : public CreatureScript
{
    public:
        mob_black_iron_grunt() : CreatureScript("mob_black_iron_grunt") { }

        struct mob_black_iron_gruntAI : public ScriptedAI
        {
            mob_black_iron_gruntAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            bool m_DeathEventDone;

            void Reset()
            {
                summons.DespawnAll();

                m_Events.Reset();

                m_DeathEventDone = false;
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                Talk(TALK_IRON_GRUNT_AGGRO);

                m_Events.ScheduleEvent(EVENT_DEVASTATE, 3000);
                m_Events.ScheduleEvent(EVENT_RALLYING_BANNER, 10000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (me->HealthBelowPctDamaged(20, p_Damage) && !m_DeathEventDone)
                {
                    m_DeathEventDone = true;
                    Talk(TALK_IRON_GRUNT_NEAR_DEATH);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_DEVASTATE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_DEVASTATE, false);
                        m_Events.ScheduleEvent(EVENT_DEVASTATE, 8000);
                        break;
                    case EVENT_RALLYING_BANNER:
                        me->MonsterTextEmote(LANG_RALLYING_BANNER_SUMMONED, me->GetGUID(), true);
                        me->CastSpell(me, SPELL_RALLYING_BANNER, false);
                        m_Events.ScheduleEvent(EVENT_RALLYING_BANNER, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_gruntAI(p_Creature);
        }
};

///< Rallying Banner - 76222
class mob_rallying_banner : public CreatureScript
{
    public:
        mob_rallying_banner() : CreatureScript("mob_rallying_banner") { }

        struct mob_rallying_bannerAI : public ScriptedAI
        {
            mob_rallying_bannerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum Spells
            {
                SpellRallyingAura = 153799
            };

            void Reset()
            {
                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->SetReactState(ReactStates::REACT_PASSIVE);

                me->RemoveAllAreasTrigger();

                me->CastSpell(me, Spells::SpellRallyingAura, true);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                me->RemoveAllAreasTrigger();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_rallying_bannerAI(p_Creature);
        }
};

///< Black Iron Leadbelcher - 76157
class mob_black_iron_leadbelcher : public CreatureScript
{
    public:
        mob_black_iron_leadbelcher() : CreatureScript("mob_black_iron_leadbelcher") { }

        struct mob_black_iron_leadbelcherAI : public ScriptedAI
        {
            mob_black_iron_leadbelcherAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            bool m_Canon;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_Events.Reset();

                m_Canon = false;

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_INCENDIARY_SHELL, 4000);
                m_Events.ScheduleEvent(EVENT_RIFLE_SHOT, 8000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (me->HealthBelowPctDamaged(50, p_Damage) && !m_Canon)
                {
                    m_Events.ScheduleEvent(eEvents::EVENT_ACTIVATE_CANON, 1000);
                    m_Canon = true;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_INCENDIARY_SHELL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_INCENDIARY_SHELL, false);
                        m_Events.ScheduleEvent(EVENT_INCENDIARY_SHELL, 10000);
                        break;
                    case EVENT_RIFLE_SHOT:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_RIFLE_SHOT, false);
                        m_Events.ScheduleEvent(EVENT_RIFLE_SHOT, 10000);
                        break;
                    case eEvents::EVENT_ACTIVATE_CANON:
                    {
                        if (Creature* l_Canon = me->FindNearestCreature(NPC_SENTRY_CANNON, 30.0f))
                        {
                            Position l_Pos;
                            l_Canon->GetPosition(&l_Pos);
                            l_Pos.m_positionX -= 2.0f;
                            l_Pos.m_positionY -= 2.0f;

                            me->GetMotionMaster()->MovePoint(1, l_Pos);
                        }
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID)
            {
                if (p_ID == 1)
                {
                    if (Creature* l_Canon = me->FindNearestCreature(NPC_SENTRY_CANNON, 2.0f))
                    {
                        l_Canon->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);

                        if (me->getVictim() != nullptr)
                            l_Canon->AI()->AttackStart(me->getVictim());
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_leadbelcherAI(p_Creature);
        }
};

///< Sentry Cannon - 76314
class mob_sentry_cannon : public CreatureScript
{
    public:
        mob_sentry_cannon() : CreatureScript("mob_sentry_cannon") { }

        struct mob_sentry_cannonAI : public ScriptedAI
        {
            mob_sentry_cannonAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                me->AddUnitState(UnitState::UNIT_STATE_ROOT);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_CANNON_SHOT, 2000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CANNON_SHOT:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_CANNON_SHOT, false);
                        m_Events.ScheduleEvent(EVENT_CANNON_SHOT, 5000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_sentry_cannonAI(p_Creature);
        }
};

///< Ragemaw Worg - 76181
class mob_ragemaw_worg : public CreatureScript
{
    public:
        mob_ragemaw_worg() : CreatureScript("mob_ragemaw_worg") { }

        struct mob_ragemaw_worgAI : public ScriptedAI
        {
            mob_ragemaw_worgAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_FRANTIC_MAULING, 10000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (me->HealthBelowPctDamaged(50, p_Damage) && !me->HasAura(SPELL_BLACKROCK_RABIES))
                    me->CastSpell(me, SPELL_BLACKROCK_RABIES, true);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_FRANTIC_MAULING:
                        me->CastSpell(me, SPELL_FRANTIC_MAULING, false);
                        m_Events.ScheduleEvent(EVENT_FRANTIC_MAULING, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_ragemaw_worgAI(p_Creature);
        }
};

///< Black Iron Warcaster - 76151
class mob_black_iron_warcaster : public CreatureScript
{
    public:
        mob_black_iron_warcaster() : CreatureScript("mob_black_iron_warcaster") { }

        struct mob_black_iron_warcasterAI : public ScriptedAI
        {
            mob_black_iron_warcasterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_BOLT_OF_STEEL, 8000);
                m_Events.ScheduleEvent(EVENT_SHRAPNEL_STORM, 15000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_BOLT_OF_STEEL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_BOLT_OF_STEEL, false);
                        m_Events.ScheduleEvent(EVENT_BOLT_OF_STEEL, 10000);
                        break;
                    case EVENT_SHRAPNEL_STORM:
                        m_Events.ScheduleEvent(EVENT_SHRAPNEL_STORM, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_warcasterAI(p_Creature);
        }
};

///< Rune Glow - 76396
class mob_rune_glow : public CreatureScript
{
    public:
        mob_rune_glow() : CreatureScript("mob_rune_glow") { }

        struct mob_rune_glowAI : public ScriptedAI
        {
            mob_rune_glowAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            void Reset()
            {
                me->ReenableEvadeMode();

                me->SetReactState(REACT_PASSIVE);
                me->SetFullHealth();
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);

                m_Events.ScheduleEvent(EVENT_CHECK_ADDS, 1000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_ADDS:
                    {
                        if (me->SelectNearbyTarget(me, 10.0f))
                        {
                            m_Events.ScheduleEvent(EVENT_CHECK_ADDS, 1000);
                            break;
                        }

                        me->DespawnOrUnsummon();
                        if (m_Instance)
                            m_Instance->SetData(DATA_RUNES_DISABLED, 1);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_rune_glowAI(p_Creature);
        }
};

///< Black Iron Alchemist - 76100
class mob_black_iron_alchemist : public CreatureScript
{
    public:
        mob_black_iron_alchemist() : CreatureScript("mob_black_iron_alchemist") { }

        struct mob_black_iron_alchemistAI : public ScriptedAI
        {
            mob_black_iron_alchemistAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_DEBILITATING_RAY, 5000);
                m_Events.ScheduleEvent(EVENT_REJUVENATING_SERUM, 20000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_DEBILITATING_RAY:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_DEBILITATING_RAY, false);
                        m_Events.ScheduleEvent(EVENT_DEBILITATING_RAY, 30000);
                        break;
                    case EVENT_REJUVENATING_SERUM:
                        if (Unit* l_Target = me->SelectNearbyAlly(me, NOMINAL_MELEE_RANGE, true))
                            me->CastSpell(l_Target, SPELL_REJUVENATING_SERUM, false);
                        m_Events.ScheduleEvent(EVENT_REJUVENATING_SERUM, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_alchemistAI(p_Creature);
        }
};

///< Black Iron Engineer - 76101
class mob_black_iron_engineer : public CreatureScript
{
    public:
        mob_black_iron_engineer() : CreatureScript("mob_black_iron_engineer") { }

        struct mob_black_iron_engineerAI : public ScriptedAI
        {
            mob_black_iron_engineerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_DEBILITATING_RAY, 5000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_DEBILITATING_RAY:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_DEBILITATING_RAY, false);
                        m_Events.ScheduleEvent(EVENT_DEBILITATING_RAY, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_engineerAI(p_Creature);
        }
};

///< Drakonid Monstrosity - 76018
///< Drakonid Monstrosity - 82556
class mob_drakonid_monstrosity : public CreatureScript
{
    public:
        mob_drakonid_monstrosity() : CreatureScript("mob_drakonid_monstrosity") { }

        struct mob_drakonid_monstrosityAI : public ScriptedAI
        {
            mob_drakonid_monstrosityAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();

                me->CastSpell(me, eSpells::SPELL_MONSTRUOUS_CLAWS, true);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_ERUPTION, 7000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_ERUPTION:
                        me->CastSpell(me, SPELL_ERUPTION, false);
                        m_Events.ScheduleEvent(EVENT_ERUPTION, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_drakonid_monstrosityAI(p_Creature);
        }
};

///< Black Iron Veteran - 77034
///< Black Iron Veteran - 84462
class mob_black_iron_veteran : public CreatureScript
{
    public:
        mob_black_iron_veteran() : CreatureScript("mob_black_iron_veteran") { }

        struct mob_black_iron_veteranAI : public ScriptedAI
        {
            mob_black_iron_veteranAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_SUNDER_ARMOR, 5000);
                m_Events.ScheduleEvent(EVENT_SHIELD_SMASH, 15000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_SUNDER_ARMOR:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_SUNDER_ARMOR, false);
                        m_Events.ScheduleEvent(EVENT_SUNDER_ARMOR, 15000);
                        break;
                    case EVENT_SHIELD_SMASH:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_SHIELD_SMASH, false);
                        m_Events.ScheduleEvent(EVENT_SHIELD_SMASH, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_veteranAI(p_Creature);
        }
};

///< Black Iron Dreadweaver - 77035
///< Black Iron Dreadweaver - 84475
class mob_black_iron_dreadweaver : public CreatureScript
{
    public:
        mob_black_iron_dreadweaver() : CreatureScript("mob_black_iron_dreadweaver") { }

        struct mob_black_iron_dreadweaverAI : public ScriptedAI
        {
            mob_black_iron_dreadweaverAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_SHADOW_BOLT, 2000);
                m_Events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, 20000);
                m_Events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 55000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_SHADOW_BOLT:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_SHADOW_BOLT, false);
                        m_Events.ScheduleEvent(EVENT_SHADOW_BOLT, 10000);
                        break;
                    case EVENT_SHADOW_BOLT_VOLLEY:
                        me->CastSpell(me, SPELL_SHADOW_BOLT_VOLLEY, false);
                        m_Events.ScheduleEvent(EVENT_SHADOW_BOLT_VOLLEY, 20000);
                        break;
                    case EVENT_VEIL_OF_SHADOW:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_VEIL_OF_SHADOW, false);
                        m_Events.ScheduleEvent(EVENT_VEIL_OF_SHADOW, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_dreadweaverAI(p_Creature);
        }
};

///< Black Iron Summoner - 77036
class mob_black_iron_summoner : public CreatureScript
{
    public:
        mob_black_iron_summoner() : CreatureScript("mob_black_iron_summoner") { }

        struct mob_black_iron_summonerAI : public ScriptedAI
        {
            mob_black_iron_summonerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_FIREBALL, 2000);
                m_Events.ScheduleEvent(EVENT_FROST_NOVA, 8000);
                m_Events.ScheduleEvent(EVENT_SUMMON_VETERAN, 15000);
                m_Events.ScheduleEvent(EVENT_SUMMON_DREADWEAVER, 30000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_FIREBALL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_FIREBALL, false);
                        m_Events.ScheduleEvent(EVENT_FIREBALL, 10000);
                        break;
                    case EVENT_FROST_NOVA:
                        me->CastSpell(me, SPELL_FROST_NOVA, false);
                        m_Events.ScheduleEvent(EVENT_FROST_NOVA, 20000);
                        break;
                    case EVENT_SUMMON_VETERAN:
                        me->MonsterTextEmote(LANG_SUMMON_BLACK_IRON_VETERAN, 0, true);
                        me->CastSpell(me, SPELL_SUMMON_VETERAN, false);
                        m_Events.ScheduleEvent(EVENT_SUMMON_VETERAN, 30000);
                        break;
                    case EVENT_SUMMON_DREADWEAVER:
                        me->MonsterTextEmote(LANG_SUMMON_BLACK_IRON_DREADWEAVER, 0, true);
                        me->CastSpell(me, SPELL_SUMMON_DREADWEAVER, false);
                        m_Events.ScheduleEvent(EVENT_SUMMON_DREADWEAVER, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_summonerAI(p_Creature);
        }
};

///< Black Iron Elite - 77037
class mob_black_iron_elite : public CreatureScript
{
    public:
        mob_black_iron_elite() : CreatureScript("mob_black_iron_elite") { }

        struct mob_black_iron_eliteAI : public ScriptedAI
        {
            mob_black_iron_eliteAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_BERSERKER_CHARGE, 8000);
                m_Events.ScheduleEvent(EVENT_BESTIAL_ROAR, 10000);
                m_Events.ScheduleEvent(EVENT_INTIMIDATING_SHOUT, 30000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_BERSERKER_CHARGE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_BERSERKER_CHARGE, false);
                        m_Events.ScheduleEvent(EVENT_BERSERKER_CHARGE, 10000);
                        break;
                    case EVENT_BESTIAL_ROAR:
                        me->CastSpell(me, SPELL_BESTIAL_ROAR, false);
                        m_Events.ScheduleEvent(EVENT_BESTIAL_ROAR, 30000);
                        break;
                    case EVENT_INTIMIDATING_SHOUT:
                        me->CastSpell(me, SPELL_INTIMIDATING_SHOUT, false);
                        m_Events.ScheduleEvent(EVENT_INTIMIDATING_SHOUT, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_eliteAI(p_Creature);
        }
};

///< Black Iron Siegebreaker - 77033
class mob_black_iron_siegebreaker : public CreatureScript
{
    public:
        mob_black_iron_siegebreaker() : CreatureScript("mob_black_iron_siegebreaker") { }

        struct mob_black_iron_siegebreakerAI : public ScriptedAI
        {
            mob_black_iron_siegebreakerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(EVENT_SMASH, 5000);
                m_Events.ScheduleEvent(EVENT_FRENZY, 10000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_SMASH:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_SMASH, false);
                        m_Events.ScheduleEvent(EVENT_SMASH, 10000);
                        break;
                    case EVENT_FRENZY:
                        me->CastSpell(me, SPELL_BESTIAL_ROAR, false);
                        m_Events.ScheduleEvent(EVENT_FRENZY, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_siegebreakerAI(p_Creature);
        }
};

///< Black Iron Groundshaker - 76599
class mob_black_iron_groundshaker : public CreatureScript
{
    public:
        mob_black_iron_groundshaker() : CreatureScript("mob_black_iron_groundshaker") { }

        struct mob_black_iron_groundshakerAI : public ScriptedAI
        {
            mob_black_iron_groundshakerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eEvents
            {
                EventIntimidatingRoar = 1,
                EventEarthpounder
            };

            enum eSpells
            {
                SpellIntimidatingRoar   = 154827,
                Earthpounder            = 154749
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(eEvents::EventIntimidatingRoar, 5000);
                m_Events.ScheduleEvent(eEvents::EventEarthpounder, 12000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventIntimidatingRoar:
                        me->CastSpell(me, eSpells::SpellIntimidatingRoar, true);
                        m_Events.ScheduleEvent(eEvents::EventIntimidatingRoar, 20000);
                        break;
                    case eEvents::EventEarthpounder:
                        me->CastSpell(me, eSpells::Earthpounder, false);
                        m_Events.ScheduleEvent(eEvents::EventEarthpounder, 25000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_groundshakerAI(p_Creature);
        }
};

///< Black Iron Drake-Keeper - 76935
class mob_black_iron_drakekeeper : public CreatureScript
{
    public:
        mob_black_iron_drakekeeper() : CreatureScript("mob_black_iron_drakekeeper") { }

        struct mob_black_iron_drakekeeperAI : public ScriptedAI
        {
            mob_black_iron_drakekeeperAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eEvents
            {
                EventFlameChain = 1
            };

            enum eSpells
            {
                SpellFlameChain = 155356
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(eEvents::EventFlameChain, urand(5000, 10000));
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case eEvents::EventFlameChain:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, eSpells::SpellFlameChain, false);
                        m_Events.ScheduleEvent(eEvents::EventFlameChain, urand(15000, 20000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_drakekeeperAI(p_Creature);
        }
};

///< Emberscale Adolescent - 76696
class mob_emberscale_adolescent : public CreatureScript
{
    public:
        mob_emberscale_adolescent() : CreatureScript("mob_emberscale_adolescent") { }

        struct mob_emberscale_adolescentAI : public ScriptedAI
        {
            mob_emberscale_adolescentAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum eSpells
            {
                SpellCaptivityChains = 155136
            };

            void Reset()
            {
                if (me->AddAura(eSpells::SpellCaptivityChains, me))
                {
                    me->SetReactState(ReactStates::REACT_PASSIVE);
                    me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_emberscale_adolescentAI(p_Creature);
        }
};

///< Leeroy Jenkins - 77075
class mob_leeroy_jenkins : public CreatureScript
{
    public:
        mob_leeroy_jenkins() : CreatureScript("mob_leeroy_jenkins") { }

        struct mob_leeroy_jenkinsAI : public ScriptedAI
        {
            mob_leeroy_jenkinsAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_ChickenTime = 0;
                m_BigRun = false;
            }

            enum Spells
            {
                ClassSpecificRes    = 157175,
                PermanentFeignDeath = 29266,
                ChickenTimer        = 172584,
                DevoutShoulders     = 166563,
                CosmeticHearthstone = 101188,
                FollowerLeeroy      = 174984
            };

            enum Talks
            {
                TalkWake1,
                TalkWake2,
                TalkWake3,
                TalkWake4,
                TalkWake5,
                TalkLEEEEEEEEEEEEEROOOOY,
                TalkShoulders,
                TalkGarrison
            };

            enum Events
            {
                EventTalk1 = 1,
                EventTalk2,
                EventTalk3,
                EventTalk4,
                EventTalk5,
                EventUpdateChicken,
                EventChicken,
                EventMove,
                EventBack,
                EventShoulders,
                EventTransform,
                EventFinal
            };

            EventMap m_TalkEvents;

            uint32 m_ChickenTime;
            bool m_BigRun;

            InstanceScript* m_Instance;

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                me->CastSpell(me, Spells::PermanentFeignDeath, true);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case eActions::ActionActivateLeeroyRes:
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_INTERACT_SPELL_ID, Spells::ClassSpecificRes);
                        me->SetFlag(EUnitFields::UNIT_FIELD_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_SPELLCLICK);
                        break;
                    default:
                        break;
                }
            }

            void SpellHit(Unit* /*p_Caster*/, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == Spells::CosmeticHearthstone)
                {
                    if (m_Instance != nullptr)
                    {
                        m_Instance->DoCompleteAchievement(eAchievements::AchievementLeeeeeeeeeeeeeeeeroy);
                        m_Instance->DoCastSpellOnPlayers(Spells::FollowerLeeroy);
                    }

                    me->DespawnOrUnsummon();
                }
                else
                {
                    if (!p_SpellInfo->HasEffect(SpellEffects::SPELL_EFFECT_RESURRECT) || !me->HasAura(Spells::PermanentFeignDeath))
                        return;

                    me->SetFullHealth();
                    me->RemoveAura(Spells::PermanentFeignDeath);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS_2, eUnitFlags2::UNIT_FLAG2_FEIGN_DEATH | eUnitFlags::UNIT_FLAG_NON_ATTACKABLE);
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_INTERACT_SPELL_ID, 0);
                    me->RemoveFlag(EUnitFields::UNIT_FIELD_NPC_FLAGS, NPCFlags::UNIT_NPC_FLAG_SPELLCLICK);
                    me->RemoveFlag(EObjectFields::OBJECT_FIELD_DYNAMIC_FLAGS, UnitDynFlags::UNIT_DYNFLAG_DEAD);
                    me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, Anim::ANIM_KNEEL_LOOP);
                    me->SetOrientation(1.4885f);
                    me->SetFacingTo(1.4885f);
                    me->SetWalk(true);

                    m_TalkEvents.ScheduleEvent(Events::EventTalk1, 1000);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_TalkEvents.Update(p_Diff);

                switch (m_TalkEvents.ExecuteEvent())
                {
                    case Events::EventTalk1:
                        Talk(Talks::TalkWake1);
                        m_TalkEvents.ScheduleEvent(Events::EventTalk2, 8000);
                        break;
                    case Events::EventTalk2:
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, 0);
                        Talk(Talks::TalkWake2);
                        me->GetMotionMaster()->MovePoint(0, 74.720f, -319.148f, 91.446f);
                        m_TalkEvents.ScheduleEvent(Events::EventTalk3, 13000);
                        break;
                    case Events::EventTalk3:
                        Talk(Talks::TalkWake3);
                        m_TalkEvents.ScheduleEvent(Events::EventTalk4, 9000);
                        break;
                    case Events::EventTalk4:
                        Talk(Talks::TalkWake4);
                        m_TalkEvents.ScheduleEvent(Events::EventTalk5, 10000);
                        break;
                    case Events::EventTalk5:
                    {
                        Talk(Talks::TalkWake5);
                        m_TalkEvents.ScheduleEvent(Events::EventMove, 6500);
                        break;
                    }
                    case Events::EventUpdateChicken:
                    {
                        if (m_ChickenTime)
                        {
                            --m_ChickenTime;
                            m_TalkEvents.ScheduleEvent(Events::EventUpdateChicken, 1000);
                        }

                        if (m_Instance)
                            m_Instance->DoUpdateWorldState(eWorldStates::WorldStateChickenTimer, m_ChickenTime);

                        break;
                    }
                    case Events::EventChicken:
                    {
                        if (m_Instance)
                        {
                            m_Instance->DoUpdateWorldState(eWorldStates::WorldStateEnableChicken, 0);
                            m_Instance->DoUpdateWorldState(eWorldStates::WorldStateChickenTimer, 0);
                        }

                        me->RemoveAura(Spells::ChickenTimer);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, 0);

                        Talk(Talks::TalkLEEEEEEEEEEEEEROOOOY);
                        me->SetWalk(false);

                        Movement::MoveSplineInit l_Init(me);
                        l_Init.Path().push_back(G3D::Vector3(80.363f, -278.893f, 91.463f));
                        l_Init.Path().push_back(G3D::Vector3(54.556f, -263.705f, 94.041f));
                        l_Init.Path().push_back(G3D::Vector3(67.622f, -240.374f, 98.381f));
                        l_Init.Path().push_back(G3D::Vector3(103.112f, -243.005f, 106.436f));
                        l_Init.Path().push_back(G3D::Vector3(106.794f, -259.459f, 106.436f));
                        l_Init.Path().push_back(G3D::Vector3(150.768f, -262.547f, 110.908f));
                        l_Init.Path().push_back(G3D::Vector3(136.134f, -364.976f, 116.833f));
                        l_Init.Path().push_back(G3D::Vector3(95.402f, -366.344f, 116.838f));
                        l_Init.Path().push_back(G3D::Vector3(93.032f, -473.292f, 116.842f));
                        l_Init.Path().push_back(G3D::Vector3(35.159f, -475.309f, 110.950f));
                        l_Init.Path().push_back(G3D::Vector3(23.331f, -525.586f, 110.942f));
                        l_Init.Path().push_back(G3D::Vector3(57.042f, -542.330f, 110.933f));
                        l_Init.SetWalk(false);
                        l_Init.Launch();
                        m_BigRun = true;
                        break;
                    }
                    case Events::EventMove:
                        me->GetMotionMaster()->MovePoint(1, 69.690f, -309.859f, 91.427f);
                        break;
                    case Events::EventBack:
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, 0);
                        me->GetMotionMaster()->MovePoint(3, 69.690f, -309.859f, 91.427f);
                        break;
                    case Events::EventShoulders:
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, Anim::ANIM_EMOTE_CHEER);
                        Talk(Talks::TalkShoulders);
                        m_TalkEvents.ScheduleEvent(Events::EventTransform, 3000);
                        break;
                    case Events::EventTransform:
                        me->CastSpell(me, Spells::DevoutShoulders, true);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, Anim::ANIM_EMOTE_FLEX);
                        m_TalkEvents.ScheduleEvent(Events::EventFinal, 3000);
                        break;
                    case Events::EventFinal:
                        Talk(Talks::TalkGarrison);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, 0);
                        me->CastSpell(me, Spells::CosmeticHearthstone, false);
                        break;
                    default:
                        break;
                }

                ///< Check movespline end
                if (m_BigRun && me->IsSplineFinished() && m_Instance != nullptr)
                {
                    if (Creature* l_SonOfBeast = Creature::GetCreature(*me, m_Instance->GetData64(eCreatures::NPC_SON_OF_THE_BEAST)))
                    {
                        if (!l_SonOfBeast->isAlive())
                        {
                            m_BigRun = false;

                            Position l_Pos;
                            l_SonOfBeast->GetPosition(&l_Pos);
                            l_Pos.m_positionX -= 3.0f;
                            l_Pos.m_positionY -= 3.0f;

                            me->GetMotionMaster()->MovePoint(5, l_Pos);
                        }
                    }
                }

                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }

            void MovementInform(uint32 /*p_Type*/, uint32 p_ID)
            {
                switch (p_ID)
                {
                    case 1: ///< Events::EventMove
                        me->GetMotionMaster()->MovePoint(2, 49.700f, -308.808f, 91.545f);
                        break;
                    case 2:
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, Anim::ANIM_KNEEL_LOOP);
                        m_TalkEvents.ScheduleEvent(Events::EventBack, 1500);
                        break;
                    case 3:
                        me->GetMotionMaster()->MovePoint(4, 78.228f, -333.180f, 91.488f);
                        break;
                    case 4: ///< Chicken timer
                    {
                        me->SetOrientation(4.667f);
                        me->SetFacingTo(4.667f);
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_EMOTE_STATE, Emote::EMOTE_STATE_USE_STANDING);
                        me->CastSpell(me, Spells::ChickenTimer, true);
                        m_ChickenTime = 900;
                        m_TalkEvents.ScheduleEvent(Events::EventUpdateChicken, 1000);
                        m_TalkEvents.ScheduleEvent(Events::EventChicken, 900000);

                        if (m_Instance)
                        {
                            m_Instance->DoUpdateWorldState(eWorldStates::WorldStateEnableChicken, 1);
                            m_Instance->DoUpdateWorldState(eWorldStates::WorldStateChickenTimer, m_ChickenTime);
                        }

                        break;
                    }
                    case 5: ///< Just found Son of the Beast
                        me->SetUInt32Value(EUnitFields::UNIT_FIELD_STATE_ANIM_ID, Anim::ANIM_KNEEL_LOOP);
                        m_TalkEvents.ScheduleEvent(Events::EventShoulders, 2000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_leeroy_jenkinsAI(p_Creature);
        }
};

///< Son of the Beast - 77927
class mob_son_of_the_beast : public CreatureScript
{
    public:
        mob_son_of_the_beast() : CreatureScript("mob_son_of_the_beast") { }

        struct mob_son_of_the_beastAI : public ScriptedAI
        {
            mob_son_of_the_beastAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            enum Events
            {
                EventFieryCharge = 1,
                EventStopCharge,
                EventFlameEruption,
                EventTerrifyingRoar
            };

            enum Spells
            {
                FieryCharge     = 157347,
                FieryTrailAT    = 157364,
                FlameEruption   = 157467,
                TerrifyingRoar  = 157428
            };

            EventMap m_Events;

            InstanceScript* m_Instance;

            bool m_Charging;

            void Reset()
            {
                m_Events.Reset();

                m_Charging = false;

                me->RemoveAllAreasTrigger();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(Events::EventFieryCharge, 8000);
                m_Events.ScheduleEvent(Events::EventFlameEruption, 15000);
                m_Events.ScheduleEvent(Events::EventTerrifyingRoar, 20000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                me->RemoveAllAreasTrigger();
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (m_Charging)
                    me->CastSpell(me, Spells::FieryTrailAT, true);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventFieryCharge:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_FARTHEST))
                            me->CastSpell(l_Target, Spells::FieryCharge, true);
                        m_Events.ScheduleEvent(Events::EventFieryCharge, 8000);
                        m_Events.ScheduleEvent(Events::EventStopCharge, 600);
                        m_Charging = true;
                        break;
                    case Events::EventStopCharge:
                        m_Charging = false;
                        break;
                    case Events::EventFlameEruption:
                        me->CastSpell(me, Spells::FlameEruption, true);
                        m_Events.ScheduleEvent(Events::EventFlameEruption, 15000);
                        break;
                    case Events::EventTerrifyingRoar:
                        me->CastSpell(me, Spells::TerrifyingRoar, true);
                        m_Events.ScheduleEvent(Events::EventTerrifyingRoar, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_son_of_the_beastAI(p_Creature);
        }
};

///< Black Iron Wyrmcaller - 80649
class mob_black_iron_wyrmcaller : public CreatureScript
{
    public:
        mob_black_iron_wyrmcaller() : CreatureScript("mob_black_iron_wyrmcaller") { }

        struct mob_black_iron_wyrmcallerAI : public ScriptedAI
        {
            mob_black_iron_wyrmcallerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum Events
            {
                EventChainPull = 1,
                EventDragonRoar,
                EventWindfury
            };

            enum Spells
            {
                ChainPull       = 163007,
                DragonRoar      = 163011,
                WindfuryTotem   = 163061,
                WindfurySpawn   = 163064,
                Windfury        = 163065
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(Events::EventChainPull, 1000);
                m_Events.ScheduleEvent(Events::EventDragonRoar, 8000);
                m_Events.ScheduleEvent(Events::EventWindfury, 6000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventChainPull:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, Spells::ChainPull, false);
                        m_Events.ScheduleEvent(Events::EventChainPull, 10000);
                        break;
                    case Events::EventDragonRoar:
                        me->CastSpell(me, Spells::DragonRoar, true);
                        m_Events.ScheduleEvent(Events::EventDragonRoar, 15000);
                        break;
                    case Events::EventWindfury:
                    {
                        me->CastSpell(me, Spells::WindfuryTotem, true);
                        me->CastSpell(me, Spells::Windfury, true);

                        if (Creature* l_Totem = me->FindNearestCreature(NPC_WINDFURY_TOTEM, 10.0f))
                            l_Totem->CastSpell(l_Totem, Spells::WindfurySpawn, true);

                        m_Events.ScheduleEvent(Events::EventWindfury, 12000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_wyrmcallerAI(p_Creature);
        }
};

///< Emberscale Ironflight - 77180
class mob_emberscale_ironflight : public CreatureScript
{
    public:
        mob_emberscale_ironflight() : CreatureScript("mob_emberscale_ironflight") { }

        struct mob_emberscale_ironflightAI : public ScriptedAI
        {
            mob_emberscale_ironflightAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit()) { }

            enum Events
            {
                EventFlameSpit = 1
            };

            enum Spells
            {
                FlameSpit = 162905
            };

            EventMap m_Events;
            Vehicle* m_Vehicle;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(Events::EventFlameSpit, 5000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventFlameSpit:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, Spells::FlameSpit, false);
                        m_Events.ScheduleEvent(Events::EventFlameSpit, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_emberscale_ironflightAI(p_Creature);
        }
};

///< Black Iron Flame Reaver - 80678
class mob_black_iron_flame_reaver : public CreatureScript
{
    public:
        mob_black_iron_flame_reaver() : CreatureScript("mob_black_iron_flame_reaver") { }

        struct mob_black_iron_flame_reaverAI : public ScriptedAI
        {
            mob_black_iron_flame_reaverAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum Events
            {
                EventFlameShock = 1
            };

            enum Spells
            {
                FlameShock = 163057
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(Events::EventFlameShock, 5000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventFlameShock:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, Spells::FlameShock, false);
                        m_Events.ScheduleEvent(Events::EventFlameShock, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_flame_reaverAI(p_Creature);
        }
};

///< Black Iron Rageguard - 82418
class mob_black_iron_rageguard : public CreatureScript
{
    public:
        mob_black_iron_rageguard() : CreatureScript("mob_black_iron_rageguard") { }

        struct mob_black_iron_rageguardAI : public ScriptedAI
        {
            mob_black_iron_rageguardAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            enum Events
            {
                EventShatteringSmash = 1,
                EventShockwave
            };

            enum Spells
            {
                ShatteringSmash = 165944,
                Shockwave       = 165954
            };

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Events.ScheduleEvent(Events::EventShatteringSmash, 5000);
                m_Events.ScheduleEvent(Events::EventShockwave, 9000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UnitState::UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case Events::EventShatteringSmash:
                        if (Unit* l_Target = SelectTarget(SelectAggroTarget::SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, Spells::ShatteringSmash, false);
                        m_Events.ScheduleEvent(Events::EventShatteringSmash, 15000);
                        break;
                    case Events::EventShockwave:
                        me->CastSpell(me, Spells::Shockwave, false);
                        m_Events.ScheduleEvent(Events::EventShockwave, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_rageguardAI(p_Creature);
        }
};

///< Emberscale Whelping - 76694
class mob_emberscale_whelping : public CreatureScript
{
    public:
        mob_emberscale_whelping() : CreatureScript("mob_emberscale_whelping") { }

        struct mob_emberscale_whelpingAI : public ScriptedAI
        {
            mob_emberscale_whelpingAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetReactState(ReactStates::REACT_PASSIVE);
                me->SetFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_emberscale_whelpingAI(p_Creature);
        }
};

///< Rallying Banner - 153799
class areatrigger_rallying_banner : public AreaTriggerEntityScript
{
    public:
        areatrigger_rallying_banner() : AreaTriggerEntityScript("areatrigger_rallying_banner") { }

        uint32 m_GrowTime;

        void OnCreate(AreaTrigger* /*p_AreaTrigger*/)
        {
            m_GrowTime = 1000;
        }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (l_Caster == nullptr)
                return;

            std::list<Unit*> l_TargetList;
            float l_Scale = p_AreaTrigger->GetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE);

            JadeCore::AnyFriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Scale);
            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Scale, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->isInCombat())
                    continue;

                if (l_Caster->getVictim() == nullptr)
                    continue;

                if (l_Unit->ToCreature()->GetAI() && l_Unit->ToCreature()->GetAI())
                    l_Unit->ToCreature()->AI()->AttackStart(l_Caster->getVictim());
            }

            if (m_GrowTime <= p_Time)
            {
                m_GrowTime = 1000;
                l_Scale += 2.0f;
                p_AreaTrigger->SetFloatValue(AREATRIGGER_FIELD_EXPLICIT_SCALE, l_Scale);
            }
            else
                m_GrowTime -= p_Time;
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_rallying_banner();
        }
};

///< Fiery Trail - 157364
class areatrigger_fiery_trail : public AreaTriggerEntityScript
{
    public:
        areatrigger_fiery_trail() : AreaTriggerEntityScript("areatrigger_fiery_trail") { }

        enum eSpells
        {
            FieryTrailDmg = 157420
        };

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            std::list<Unit*> l_TargetList;
            float l_Radius = 3.0f;

            JadeCore::AnyUnfriendlyUnitInObjectRangeCheck l_Check(p_AreaTrigger, p_AreaTrigger->GetCaster(), l_Radius);
            JadeCore::UnitListSearcher<JadeCore::AnyUnfriendlyUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius)
                    l_Unit->CastSpell(l_Unit, eSpells::FieryTrailDmg, true);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_fiery_trail();
        }
};

///< Shrapnel Storm - 153942
class spell_shrapnel_storm: public SpellScriptLoader
{
    public:
        spell_shrapnel_storm() : SpellScriptLoader("spell_shrapnel_storm") { }

        class spell_shrapnel_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shrapnel_storm_SpellScript)

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Position const* l_Pos = GetExplTargetDest())
                {
                    if (Unit* l_Caster = GetCaster())
                        l_Caster->CastSpell(l_Pos->GetPositionX(), l_Pos->GetPositionY(), l_Pos->GetPositionZ(), SPELL_SHRAPNEL_STORM_MISSILE, true);
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_shrapnel_storm_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_shrapnel_storm_SpellScript();
        }
};

///< Eruption - 155037
class spell_eruption: public SpellScriptLoader
{
    public:
        spell_eruption() : SpellScriptLoader("spell_eruption") { }

        class spell_eruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_eruption_SpellScript)

            enum eSpell
            {
                TargetRestrict = 20473
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
                switch (m_scriptSpellId)
                {
                case 138652:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                    break;
                default:
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
                    OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_CONE_ENEMY_104);
                    break;
                }
            }
        };

        SpellScript* GetSpellScript() const override
        {
            return new spell_eruption_SpellScript();
        }
};

///< Class Specific Res - 157175
class spell_class_specific_res: public SpellScriptLoader
{
    public:
        spell_class_specific_res() : SpellScriptLoader("spell_class_specific_res") { }

        class spell_class_specific_res_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_class_specific_res_SpellScript)

            void HandleDummy(SpellEffIndex /*p_EffIndex*/)
            {
                if (Player* l_Player = GetCaster()->ToPlayer())
                {
                    if (Unit* l_Leeroy = GetHitUnit())
                    {
                        ///< Only classes which can resurrect, can use this
                        switch (l_Player->getClass())
                        {
                            case CLASS_WARRIOR:
                            case CLASS_WARLOCK:
                            case CLASS_DEATH_KNIGHT:
                            case CLASS_ROGUE:
                            case CLASS_HUNTER:
                                return;
                            default:
                                break;
                        }

                        PlayerSpellMap l_Spells = l_Player->GetSpellMap();
                        for (auto l_Spell : l_Spells)
                        {
                            if (SpellInfo const* l_SpellInfo = sSpellMgr->GetSpellInfo(l_Spell.first))
                            {
                                if (!l_SpellInfo->HasEffect(SPELL_EFFECT_RESURRECT))
                                    continue;

                                l_Player->CastSpell(l_Leeroy, l_SpellInfo->Id, false);
                            }
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_class_specific_res_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_class_specific_res_SpellScript();
        }
};

///< Whelp Cage - 227011
class go_ubrs_whelp_cage : public GameObjectScript
{
    public:
        go_ubrs_whelp_cage() : GameObjectScript("go_ubrs_whelp_cage") { }

        bool GossipHello(Player* p_Player, GameObject* p_Gameobject)
        {
            if (p_Gameobject->GetGoState() != GOState::GO_STATE_READY)
            {
                std::list<Creature*> l_WhelpList;
                p_Player->GetCreatureListWithEntryInGrid(l_WhelpList, eCreatures::NPC_EMBERSCALE_WHELPING, 10.0f);

                for (Creature* l_EmbercaleWhelp : l_WhelpList)
                {
                    if (l_EmbercaleWhelp->GetAI())
                    {
                        l_EmbercaleWhelp->SetReactState(ReactStates::REACT_AGGRESSIVE);
                        l_EmbercaleWhelp->RemoveFlag(EUnitFields::UNIT_FIELD_FLAGS, eUnitFlags::UNIT_FLAG_NON_ATTACKABLE | eUnitFlags::UNIT_FLAG_NOT_SELECTABLE | eUnitFlags::UNIT_FLAG_IMMUNE_TO_PC);
                        l_EmbercaleWhelp->AI()->AttackStart(p_Player);
                    }
                }
            }

            return false;
        }
};

#ifndef __clang_analyzer__
void AddSC_upper_blackrock_spire()
{
    new mob_black_iron_grunt();
    new mob_rallying_banner();
    new mob_black_iron_leadbelcher();
    new mob_sentry_cannon();
    new mob_ragemaw_worg();
    new mob_black_iron_warcaster();
    new mob_rune_glow();
    new mob_black_iron_alchemist();
    new mob_black_iron_engineer();
    new mob_drakonid_monstrosity();
    new mob_black_iron_veteran();
    new mob_black_iron_dreadweaver();
    new mob_black_iron_summoner();
    new mob_black_iron_elite();
    new mob_black_iron_siegebreaker();
    new mob_black_iron_groundshaker();
    new mob_black_iron_drakekeeper();
    new mob_emberscale_adolescent();
    new mob_leeroy_jenkins();
    new mob_son_of_the_beast();
    new mob_black_iron_wyrmcaller();
    new mob_emberscale_ironflight();
    new mob_black_iron_flame_reaver();
    new mob_black_iron_rageguard();
    new mob_emberscale_whelping();
    new areatrigger_rallying_banner();
    new areatrigger_fiery_trail();
    new spell_shrapnel_storm();
    new spell_eruption();
    new spell_class_specific_res();
    new go_ubrs_whelp_cage();
}
#endif
