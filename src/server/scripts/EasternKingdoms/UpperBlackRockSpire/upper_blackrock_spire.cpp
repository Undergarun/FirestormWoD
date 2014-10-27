/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptPCH.h"
#include "upper_blackrock_spire.h"
#include "Language.h"

enum eSpells
{
    // Black Iron Grunt
    SPELL_DEVASTATE                 = 153832,
    SPELL_RALLYING_BANNER           = 153792,
    // Black Iron Leadbelcher
    SPELL_INCENDIARY_SHELL          = 153981,
    SPELL_RIFLE_SHOT                = 153974,
    // Sentry Cannon
    SPELL_CANNON_SHOT               = 154178,
    SPELL_SAFETY_PROTOCOLS          = 154894,
    // Ragemaw Worg
    SPELL_BLACKROCK_RABIES          = 154017,
    SPELL_FRANTIC_MAULING           = 154039,
    // Black Iron Warcaster
    SPELL_BOLT_OF_STEEL             = 153642,
    SPELL_SHRAPNEL_STORM            = 153942,
    SPELL_SHRAPNEL_STORM_MISSILE    = 153941,
    // Black Iron Alchemist
    SPELL_DEBILITATING_RAY          = 155505,
    SPELL_REJUVENATING_SERUM        = 155498,
    // Drakonid Monstrosity
    SPELL_ERUPTION                  = 155037,
    // Black Iron Veteran
    SPELL_SUNDER_ARMOR              = 155581,
    SPELL_SHIELD_SMASH              = 155575,
    // Black Iron Dreadweaver
    SPELL_SHADOW_BOLT               = 155587,
    SPELL_SHADOW_BOLT_VOLLEY        = 155588,
    SPELL_VEIL_OF_SHADOW            = 155586,
    // Black Iron Summoner
    SPELL_FIREBALL                  = 155590,
    SPELL_FROST_NOVA                = 155589,
    SPELL_SUMMON_VETERAN            = 169151,
    SPELL_SUMMON_DREADWEAVER        = 169088,
    // Black Iron Elite
    SPELL_BERSERKER_CHARGE          = 155584,
    SPELL_BESTIAL_ROAR              = 139385,
    SPELL_INTIMIDATING_SHOUT        = 167259,
    // Black Iron Siegebreaker
    SPELL_SMASH                     = 155572,
    SPELL_FRENZY                    = 81173
};

enum eEvents
{
    // Black Iron Grunt
    EVENT_DEVASTATE = 1,
    EVENT_RALLYING_BANNER,
    // Black Iron Leadbelcher
    EVENT_INCENDIARY_SHELL,
    EVENT_RIFLE_SHOT,
    // Sentry Cannon
    EVENT_CANNON_SHOT,
    EVENT_SAFETY_PROTOCOLS,
    // Ragemaw Worg
    EVENT_FRANTIC_MAULING,
    // Black Iron Warcaster
    EVENT_BOLT_OF_STEEL,
    EVENT_SHRAPNEL_STORM,
    // Rune Glow
    EVENT_CHECK_ADDS,
    // Black Iron Alchemist
    EVENT_DEBILITATING_RAY,
    EVENT_REJUVENATING_SERUM,
    // Drakonid Monstrosity
    EVENT_ERUPTION,
    // Black Iron Veteran
    EVENT_SUNDER_ARMOR,
    EVENT_SHIELD_SMASH,
    // Black Iron Dreadweaver
    EVENT_SHADOW_BOLT,
    EVENT_SHADOW_BOLT_VOLLEY,
    EVENT_VEIL_OF_SHADOW,
    // Black Iron Summoner
    EVENT_FIREBALL,
    EVENT_FROST_NOVA,
    EVENT_SUMMON_VETERAN,
    EVENT_SUMMON_DREADWEAVER,
    // Black Iron Elite
    EVENT_BERSERKER_CHARGE,
    EVENT_BESTIAL_ROAR,
    EVENT_INTIMIDATING_SHOUT,
    // Black Iron Siegebreaker
    EVENT_SMASH,
    EVENT_FRENZY
};

enum eActions
{
};

enum eTalks
{
    TALK_IRON_GRUNT_AGGRO,          // Iron Horde, we have unwanted visitors! -- Help! We have intruders!
    TALK_IRON_GRUNT_NEAR_DEATH      // We need backup!
};

// Black Iron Grunt - 76179
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
                me->ReenableEvadeMode();

                m_Events.Reset();

                m_DeathEventDone = false;
            }

            void EnterCombat(Unit* p_Attacker)
            {
                Talk(TALK_IRON_GRUNT_AGGRO);

                m_Events.ScheduleEvent(EVENT_DEVASTATE, 3000);
                m_Events.ScheduleEvent(EVENT_RALLYING_BANNER, 10000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage)
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
                        me->MonsterTextEmote(LANG_RALLYING_BANNER_SUMMONED, 0, true);
                        me->CastSpell(me, SPELL_RALLYING_BANNER, false);
                        /*
                        @TODO:
                            - Script for Rallying Banner
                            - 153799 : Create Areatrigger, which grows up
                            - And aggro nearby allies
                        */
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

// Black Iron Leadbelcher - 76157
class mob_black_iron_leadbelcher : public CreatureScript
{
    public:
        mob_black_iron_leadbelcher() : CreatureScript("mob_black_iron_leadbelcher") { }

        struct mob_black_iron_leadbelcherAI : public ScriptedAI
        {
            mob_black_iron_leadbelcherAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_Events.Reset();
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(EVENT_INCENDIARY_SHELL, 4000);
                m_Events.ScheduleEvent(EVENT_RIFLE_SHOT, 8000);
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
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_black_iron_leadbelcherAI(p_Creature);
        }
};

// Sentry Cannon - 76314
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
                me->ReenableEvadeMode();

                m_Events.Reset();

                me->AddUnitState(UNIT_STATE_ROOT);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(EVENT_CANNON_SHOT, 5000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage)
            {
                if (me->HealthBelowPctDamaged(50, p_Damage) && !me->HasAura(SPELL_SAFETY_PROTOCOLS))
                    me->CastSpell(me, SPELL_SAFETY_PROTOCOLS, true);
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
                        m_Events.ScheduleEvent(EVENT_CANNON_SHOT, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_sentry_cannonAI(p_Creature);
        }
};

// Ragemaw Worg - 76181
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

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(EVENT_FRANTIC_MAULING, 10000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage)
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

// Black Iron Warcaster - 76151
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

            void EnterCombat(Unit* p_Attacker)
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

// Rune Glow - 76396
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

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                m_Events.ScheduleEvent(EVENT_CHECK_ADDS, 1000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_ADDS:
                    {
                        if (Unit* l_Creature = me->SelectNearbyAlly(me, 10.f))
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

// Black Iron Alchemist - 76100
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

            void EnterCombat(Unit* p_Attacker)
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
                        if (Unit* l_Target = me->SelectNearbyAlly(me))
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

// Black Iron Engineer - 76101
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

            void EnterCombat(Unit* p_Attacker)
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

// Drakonid Monstrosity - 76018
// Drakonid Monstrosity - 82556
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
            }

            void EnterCombat(Unit* p_Attacker)
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
                        m_Events.ScheduleEvent(EVENT_DEBILITATING_RAY, 15000);
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

// Black Iron Veteran - 77034
// Black Iron Veteran - 84462
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

            void EnterCombat(Unit* p_Attacker)
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

// Black Iron Dreadweaver - 77035
// Black Iron Dreadweaver - 84475
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

            void EnterCombat(Unit* p_Attacker)
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

// Black Iron Summoner - 77036
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

            void EnterCombat(Unit* p_Attacker)
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

// Black Iron Elite - 77037
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

            void EnterCombat(Unit* p_Attacker)
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

// Black Iron Siegebreaker - 77033
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

            void EnterCombat(Unit* p_Attacker)
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

// Shrapnel Storm - 153942
class spell_shrapnel_storm : public SpellScriptLoader
{
    public:
        spell_shrapnel_storm() : SpellScriptLoader("spell_shrapnel_storm") { }

        class spell_shrapnel_storm_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_shrapnel_storm_SpellScript);

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

// Eruption - 155037
class spell_eruption : public SpellScriptLoader
{
    public:
        spell_eruption() : SpellScriptLoader("spell_eruption") { }

        class spell_eruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_eruption_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                p_Targets.remove_if([this, l_Caster](WorldObject* p_Object) -> bool
                {
                    if (!p_Object)
                        return true;

                    if (!l_Caster->isInFront(p_Object, M_PI / 12.f))
                        return true;

                    return false;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_CONE_ENEMY_104);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_CONE_ENEMY_104);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_eruption_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_CONE_ENEMY_104);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_eruption_SpellScript();
        }
};

void AddSC_upper_blackrock_spire()
{
    new mob_black_iron_grunt();
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
    new spell_shrapnel_storm();
    new spell_eruption();
}