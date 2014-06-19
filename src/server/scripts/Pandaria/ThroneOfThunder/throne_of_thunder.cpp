/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
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

#include "ScriptedCreature.h"
#include "throne_of_thunder.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureAI.h"
#include "MoveSplineInit.h"

// Only for large/long winds
Position const darkWindPos[2] =
{
    { 5559.361f, 6263.460f, 112.1402f, 1.57f }, // Large
    { 5431.700f, 6131.940f, 112.1400f, 1.57f }  // Long
};

Position const darkWindSourcePos = { 5520.0f, 7700.0f, 0.0f, 0.0f };

enum eSpells
{
    // Zandalari Water-Binder
    SPELL_BIND_WATER                    = 137099,
    SPELL_DELUGE                        = 136881,
    SPELL_FROSTBOLT                     = 136952,

    // Zandalari Blade Initiate
    SPELL_WOUNDING_STRIKE               = 140049,

    // Zandalari Spear-Shaper
    SPELL_BERSERKING                    = 137096,
    SPELL_RETRIEVE_SPEAR_JUMP           = 137070,
    SPELL_SPEAR_SPIN                    = 137077,
    SPELL_THROW_SPEAR                   = 136986,
    SPELL_THROW_SPEAR_DISARM            = 137066,
    SPELL_THROW_SPEAR_PERIODIC          = 137058,

    // Zandalari Storm-Caller
    SPELL_WATER_BOLT                    = 139231,
    SPELL_WATER_BOLT_DAMAGE             = 139251,
    SPELL_STORM_WEAPON                  = 139218,
    SPELL_STORM_WEAPON_DAMAGE           = 139220,
    SPELL_FOCUSED_LIGHTNING_SUMMON      = 139206,
    SPELL_FOCUSED_LIGHTNING_VISUAL      = 139233,
    SPELL_FOCUSED_LIGHTNING_AOE         = 139209,
    SPELL_FOCUSED_LIGHTNING_DAMAGE      = 139210,
    SPELL_FOCUSED_LIGHTNING_DETONATION  = 139211,

    // Ancient Python
    SPELL_STEALTH                       = 139885,
    SPELL_ANCIENT_VENOM                 = 139888,

    // Spirit Flayer
    SPELL_SPIRIT_LANTERN                = 139364,
    SPELL_SPIRIT_LIGHT                  = 139461,

    // Tormented Spirit
    SPELL_TORMENT                       = 139550,

    // Soul-Fed Construct
    SPELL_CRUSH_ARMOR                   = 33661,
    SPELL_SPIRITFIRE_BEAM               = 139895,

    // Stormbringer Draz'Kil
    SPELL_CHAIN_LIGHTNING               = 139903,
    SPELL_STORMCLOUD                    = 139900,
    SPELL_STORMBRINGER_VISUAL           = 139871,

    // General Purpose Bunny JMF (Ground)
    SPELL_DARK_WINDS_SMALL              = 139499,
    SPELL_DARK_WINDS_LARGE              = 140781,
    SPELL_DARK_WINDS_LONG               = 139535,
    SPELL_DARK_WINDS_FORCE_WEATHER      = 139485
};

enum eEvents
{
    // Zandalari Water-Binder
    EVENT_BIND_WATER            = 1,
    EVENT_DELUGE                = 2,
    EVENT_FROSTBOLT             = 3,

    // Zandalari Blade Initiate
    EVENT_WOUNDING_STRIKE       = 4,

    // Zandalari Spear-Shaper
    EVENT_BERSERKING            = 5,
    EVENT_RETRIEVE_SPEAR        = 6,
    EVENT_SPEAR_SPIN            = 7,
    EVENT_THROW_SPEAR           = 8,

    // Zandalari Storm-Caller
    EVENT_WATER_BOLT            = 9,
    EVENT_STORM_WEAPON          = 10,
    EVENT_FOCUSED_LIGHTNING     = 11,
    EVENT_FOCUSED_LIGHTNING_AOE = 12,

    // Ancient Python
    EVENT_ANCIENT_VENOM         = 13,

    // Spirit Flayer
    EVENT_SPIRIT_LIGHT          = 14,

    // Tormented Spirit
    EVENT_TORMENT               = 15,

    // Soul-Fed Construct
    EVENT_CRUSH_ARMOR           = 16,
    EVENT_SPIRITFIRE_BEAM       = 17,

    // Stormbringer Draz'Kil
    EVENT_CHAIN_LIGHTNING       = 18,
    EVENT_STORMCLOUD            = 19
};

enum eEquipIds
{
    // Zandalari Water-Binder
    WATER_BINDER_AXE    = 89698,

    // Zandalari Blade Initiate
    BLADE_INITIATE_AXE  = 13632,

    // Zandalari Spear-Shaper
    SPEAR_SHAPER_SPEAR  = 89685,

    // Zandalari Storm-Caller
    STORM_CALLER_EQ_1   = 94703,
    STORM_CALLER_EQ_2   = 89295,

    // Spirit Flayer
    SPIRIT_FLAYER_LAMP  = 93755,

    // Stormbringer Draz'Kil
    DRAZ_KIL_SPEAR      = 94247
};

// Zandalari Water-Binder - 69455
class mob_zandalari_water_binder : public CreatureScript
{
    public:
        mob_zandalari_water_binder() : CreatureScript("mob_zandalari_water_binder") { }

        struct mob_zandalari_water_binderAI : public ScriptedAI
        {
            mob_zandalari_water_binderAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                SetEquipmentSlots(false, WATER_BINDER_AXE, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_BIND_WATER, 5000);
                events.ScheduleEvent(EVENT_DELUGE, 10000);
                events.ScheduleEvent(EVENT_FROSTBOLT, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BIND_WATER:
                        me->CastSpell(me, SPELL_BIND_WATER, false);
                        events.ScheduleEvent(EVENT_BIND_WATER, 30000);
                        break;
                    case EVENT_DELUGE:
                        me->CastSpell(me, SPELL_DELUGE, false);
                        events.ScheduleEvent(EVENT_DELUGE, 25000);
                        break;
                    case EVENT_FROSTBOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_FROSTBOLT, false);
                        events.ScheduleEvent(EVENT_FROSTBOLT, 5000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_water_binderAI(creature);
        }
};

// Zandalari Blade Initiate - 70230
class mob_zandalari_blade_initiate : public CreatureScript
{
    public:
        mob_zandalari_blade_initiate() : CreatureScript("mob_zandalari_blade_initiate") { }

        struct mob_zandalari_blade_initiateAI : public ScriptedAI
        {
            mob_zandalari_blade_initiateAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                SetEquipmentSlots(false, BLADE_INITIATE_AXE, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_WOUNDING_STRIKE, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WOUNDING_STRIKE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_WOUNDING_STRIKE, false);
                        events.ScheduleEvent(EVENT_WOUNDING_STRIKE, 8000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_blade_initiateAI(creature);
        }
};

// Zandalari Spear-Shaper - 69388
class mob_zandalari_spear_shaper : public CreatureScript
{
    public:
        mob_zandalari_spear_shaper() : CreatureScript("mob_zandalari_spear_shaper") { }

        struct mob_zandalari_spear_shaperAI : public ScriptedAI
        {
            mob_zandalari_spear_shaperAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                SetEquipmentSlots(false, SPEAR_SHAPER_SPEAR, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_BERSERKING, 5000);
                events.ScheduleEvent(EVENT_RETRIEVE_SPEAR, 10000);
                events.ScheduleEvent(EVENT_SPEAR_SPIN, 8000);
                events.ScheduleEvent(EVENT_THROW_SPEAR, 7000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_BERSERKING:
                        me->CastSpell(me, SPELL_BERSERKING, false);
                        events.ScheduleEvent(EVENT_BERSERKING, 20000);
                        break;
                    case EVENT_RETRIEVE_SPEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_RETRIEVE_SPEAR_JUMP, false);
                        events.ScheduleEvent(EVENT_RETRIEVE_SPEAR, 10000);
                        break;
                    case EVENT_SPEAR_SPIN:
                        me->CastSpell(me, SPELL_SPEAR_SPIN, false);
                        events.ScheduleEvent(EVENT_SPEAR_SPIN, 8000);
                        break;
                    case EVENT_THROW_SPEAR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_THROW_SPEAR, false);
                        me->CastSpell(me, SPELL_THROW_SPEAR_DISARM, true);
                        events.ScheduleEvent(EVENT_THROW_SPEAR, 7000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_spear_shaperAI(creature);
        }
};

// Thrown Spear - 69438
class mob_thrown_spear : public CreatureScript
{
    public:
        mob_thrown_spear() : CreatureScript("mob_thrown_spear") { }

        struct mob_thrown_spearAI : public ScriptedAI
        {
            mob_thrown_spearAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->CastSpell(me, SPELL_THROW_SPEAR_PERIODIC, true);
            }

            void UpdateAI(const uint32 diff) { }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_thrown_spearAI(creature);
        }
};

// Zandalari Storm-Caller - 69390
class mob_zandalari_storm_caller : public CreatureScript
{
    public:
        mob_zandalari_storm_caller() : CreatureScript("mob_zandalari_storm_caller") { }

        struct mob_zandalari_storm_callerAI : public ScriptedAI
        {
            mob_zandalari_storm_callerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint32 focusedLightningTarget;

            void Reset()
            {
                me->ReenableEvadeMode();
                SetEquipmentSlots(false, STORM_CALLER_EQ_1, STORM_CALLER_EQ_2, EQUIP_NO_CHANGE);

                focusedLightningTarget = 0;

                events.Reset();

                std::list<Creature*> focusedLightnings;
                me->GetCreatureListWithEntryInGrid(focusedLightnings, NPC_TRASH_FOCUSED_LIGHTNING, 100.0f);

                for (auto itr : focusedLightnings)
                    itr->DespawnOrUnsummon();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_WATER_BOLT, 8000);
                events.ScheduleEvent(EVENT_STORM_WEAPON, 5000);
                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WATER_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_WATER_BOLT, false);
                        events.ScheduleEvent(EVENT_WATER_BOLT, 8000);
                        break;
                    case EVENT_STORM_WEAPON:
                        me->CastSpell(me, SPELL_STORM_WEAPON, false);
                        events.ScheduleEvent(EVENT_STORM_WEAPON, 15000);
                        break;
                    case EVENT_FOCUSED_LIGHTNING:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 40.0f, true))
                        {
                            me->CastSpell(target, SPELL_FOCUSED_LIGHTNING_SUMMON, false);
                            focusedLightningTarget = target->GetGUIDLow();
                        }
                        events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            uint32 GetData(uint32 type)
            {
                return focusedLightningTarget;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_storm_callerAI(creature);
        }
};

// Focused Lightning - 70174
class mob_trash_focused_lightning : public CreatureScript
{
    public:
        mob_trash_focused_lightning() : CreatureScript("mob_trash_focused_lightning") { }

        struct mob_trash_focused_lightningAI : public ScriptedAI
        {
            mob_trash_focused_lightningAI(Creature* creature) : ScriptedAI(creature)
            {
                focusedGuid = 0;
                exploded = false;
            }

            uint64 focusedGuid;
            bool exploded;
            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_VISUAL, true);
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (!summoner->ToCreature())
                    return;

                Creature* stormCaller = summoner->ToCreature();
                focusedGuid = MAKE_NEW_GUID(stormCaller->AI()->GetData(0), 0, HIGHGUID_PLAYER);

                if (Player* player = Player::GetPlayer(*me, focusedGuid))
                {
                    me->ClearUnitState(UNIT_STATE_CASTING | UNIT_STATE_STUNNED);
                    if (player && me->Attack(player, true))
                        me->GetMotionMaster()->MoveChase(player, 0.5f);
                }

                events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING_AOE, 1000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (exploded)
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_FOCUSED_LIGHTNING_AOE:
                        me->CastSpell(me, SPELL_FOCUSED_LIGHTNING_AOE, true);
                        events.ScheduleEvent(EVENT_FOCUSED_LIGHTNING_AOE, 1000);
                        break;
                    default:
                        break;
                }

                if (Player* player = Player::GetPlayer(*me, focusedGuid))
                {
                    if (player->GetDistance(me) <= 1.0f)
                    {
                        me->CastSpell(player, SPELL_FOCUSED_LIGHTNING_DETONATION, true);
                        me->DespawnOrUnsummon(200);
                        exploded = true;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_trash_focused_lightningAI(creature);
        }
};

// Ancient Python - 70448
class mob_ancient_python : public CreatureScript
{
    public:
        mob_ancient_python() : CreatureScript("mob_ancient_python") { }

        struct mob_ancient_pythonAI : public ScriptedAI
        {
            mob_ancient_pythonAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;
            uint32 stealthTimer;

            void Reset()
            {
                me->ReenableEvadeMode();
                events.Reset();
                stealthTimer = 20000;
                me->CastSpell(me, SPELL_STEALTH, true);
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_ANCIENT_VENOM, 1000);
                stealthTimer = 0;
            }

            void UpdateAI(const uint32 diff)
            {
                if (stealthTimer)
                {
                    if (stealthTimer <= diff)
                    {
                        me->CastSpell(me, SPELL_STEALTH, true);
                        stealthTimer = 20000;
                    }
                    else
                        stealthTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_ANCIENT_VENOM:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_ANCIENT_VENOM, false);
                        events.ScheduleEvent(EVENT_ANCIENT_VENOM, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ancient_pythonAI(creature);
        }
};

// Spirit Flayer - 70246
class mob_spirit_flayer : public CreatureScript
{
    public:
        mob_spirit_flayer() : CreatureScript("mob_spirit_flayer") { }

        struct mob_spirit_flayerAI : public ScriptedAI
        {
            mob_spirit_flayerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                events.Reset();

                events.ScheduleEvent(EVENT_SPIRIT_LIGHT, 1000);

                SetEquipmentSlots(false, EQUIP_NO_CHANGE, SPIRIT_FLAYER_LAMP, EQUIP_NO_CHANGE);

                me->SetReactState(REACT_PASSIVE);
                me->CastSpell(me, SPELL_SPIRIT_LANTERN, true);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_SPIRIT_LIGHT:
                        if (Unit* target = me->SelectNearbyTarget(NULL, 10.0f))
                            me->CastSpell(target, SPELL_SPIRIT_LIGHT, true);
                        events.ScheduleEvent(EVENT_SPIRIT_LIGHT, 1000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_spirit_flayerAI(creature);
        }
};

// Tormented Spirit - 70341
class mob_tourmented_spirit : public CreatureScript
{
    public:
        mob_tourmented_spirit() : CreatureScript("mob_tourmented_spirit") { }

        struct mob_tourmented_spiritAI : public ScriptedAI
        {
            mob_tourmented_spiritAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_TORMENT, 500);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (me->isInCombat())
                    return;

                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (me->GetDistance(who) <= 50.0f)
                {
                    AttackStart(who);
                    me->CombatStart(who, true);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_TORMENT:
                        me->CastSpell(me, SPELL_TORMENT, true);
                        events.ScheduleEvent(EVENT_TORMENT, 4000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_tourmented_spiritAI(creature);
        }
};

// Soul-Fed Construct - 70308
class mob_soul_fed_construct : public CreatureScript
{
    public:
        mob_soul_fed_construct() : CreatureScript("mob_soul_fed_construct") { }

        struct mob_soul_fed_constructAI : public ScriptedAI
        {
            mob_soul_fed_constructAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_CRUSH_ARMOR, 2000);
                events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, 5000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CRUSH_ARMOR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_CRUSH_ARMOR, true);
                        events.ScheduleEvent(EVENT_CRUSH_ARMOR, 10000);
                        break;
                    case EVENT_SPIRITFIRE_BEAM:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_SPIRITFIRE_BEAM, true);
                        events.ScheduleEvent(EVENT_SPIRITFIRE_BEAM, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_soul_fed_constructAI(creature);
        }
};

// Stormbringer Draz'Kil - 70445
class mob_stormbringer_draz_kil : public CreatureScript
{
    public:
        mob_stormbringer_draz_kil() : CreatureScript("mob_stormbringer_draz_kil") { }

        struct mob_stormbringer_draz_kilAI : public ScriptedAI
        {
            mob_stormbringer_draz_kilAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                SetEquipmentSlots(false, DRAZ_KIL_SPEAR, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                me->CastSpell(me, SPELL_STORMBRINGER_VISUAL, false);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 5000);
                events.ScheduleEvent(EVENT_STORMCLOUD, 10000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHAIN_LIGHTNING:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_CHAIN_LIGHTNING, false);
                        events.ScheduleEvent(EVENT_CHAIN_LIGHTNING, 10000);
                        break;
                    case EVENT_STORMCLOUD:
                        me->CastSpell(me, SPELL_STORMCLOUD, false);
                        events.ScheduleEvent(EVENT_STORMCLOUD, 60000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_stormbringer_draz_kilAI(creature);
        }
};

// General Purpose Bunny JMF (Ground) - 59394
class mob_dark_winds : public CreatureScript
{
    public:
        mob_dark_winds() : CreatureScript("mob_dark_winds") { }

        struct mob_dark_windsAI : public ScriptedAI
        {
            mob_dark_windsAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                if (me->GetPositionX() <= darkWindPos[0].GetPositionX() + 1.0f &&
                    me->GetPositionY() <= darkWindPos[0].GetPositionY() + 1.0f &&
                    me->GetPositionX() >= darkWindPos[0].GetPositionX() - 1.0f &&
                    me->GetPositionY() >= darkWindPos[0].GetPositionY() - 1.0f)
                    me->CastSpell(me, SPELL_DARK_WINDS_LARGE, true);
                else if (me->GetPositionX() >= darkWindPos[1].GetPositionX() - 1.0f &&
                         me->GetPositionX() <= darkWindPos[1].GetPositionY() + 1.0f &&
                         me->GetPositionY() < darkWindPos[0].GetPositionY() - 1.0f)
                    me->CastSpell(me, SPELL_DARK_WINDS_LONG, true);
                else
                    me->CastSpell(me, SPELL_DARK_WINDS_SMALL, true);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
            }

            void UpdateAI(const uint32 diff)
            {
                float force = 3.0f;
                std::list<Unit*> playerList;
                std::list<Unit*> unitToRemove;
                float radius = 30.0f;

                JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(me, me, radius);
                JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(me, playerList, u_check);
                me->VisitNearbyObject(radius, searcher);

                for (auto itr : playerList)
                {
                    if (itr->GetTypeId() == TYPEID_PLAYER)
                        continue;

                    unitToRemove.push_back(itr);
                }

                for (auto itr : unitToRemove)
                    playerList.remove(itr);

                for (auto player : playerList)
                {
                    bool inSquare = false;
                    Player* plr = player->ToPlayer();
                    std::list<Creature*> darkWinds;
                    plr->GetCreatureListWithEntryInGrid(darkWinds, NPC_DARK_WINDS_BUNNY, 30.0f);

                    for (auto itr : darkWinds)
                    {
                        if (IsInSquare(plr, itr))
                        {
                            inSquare = true;
                            break;
                        }
                    }

                    if (inSquare)
                    {
                        plr->SendApplyMovementForce(true, darkWindSourcePos, force);

                        if (!plr->HasAura(SPELL_DARK_WINDS_FORCE_WEATHER))
                            plr->CastSpell(plr, SPELL_DARK_WINDS_FORCE_WEATHER, true);
                    }
                    else
                    {
                        plr->SendApplyMovementForce(false, darkWindSourcePos, force);

                        if (plr->HasAura(SPELL_DARK_WINDS_FORCE_WEATHER))
                            plr->RemoveAura(SPELL_DARK_WINDS_FORCE_WEATHER);
                    }
                }
            }

            bool IsInSquare(Unit* plr, Unit* darkWind)
            {
                float sizeX = 0.0f;
                float sizeY = 0.0f;

                if (AreaTrigger* wind = darkWind->GetAreaTrigger(SPELL_DARK_WINDS_SMALL))
                {
                    sizeX = 12.0f;
                    sizeY = 20.0f;
                }
                else if (AreaTrigger* wind = darkWind->GetAreaTrigger(SPELL_DARK_WINDS_LARGE))
                {
                    sizeX = 45.0f;
                    sizeY = 25.0f;
                }
                else if (AreaTrigger* wind = darkWind->GetAreaTrigger(SPELL_DARK_WINDS_LONG))
                {
                    sizeX = 22.0f;
                    sizeY = 35.0f;
                }

                float minX = darkWind->GetPositionX() - (sizeX / 2.0f);
                float maxX = darkWind->GetPositionX() + (sizeX / 2.0f);
                float minY = darkWind->GetPositionY() - (sizeY / 2.0f);
                float maxY = darkWind->GetPositionY() + (sizeY / 2.0f);

                if (plr->GetPositionX() >= minX && plr->GetPositionX() <= maxX &&
                    plr->GetPositionY() >= minY && plr->GetPositionY() <= maxY)
                    return true;

                return false;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_dark_windsAI(creature);
        }
};

// Water Bolt - 139231
class spell_water_bolt : public SpellScriptLoader
{
    public:
        spell_water_bolt() : SpellScriptLoader("spell_water_bolt") { }

        class spell_water_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_water_bolt_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                JadeCore::RandomResizeList(targets, 1);
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_WATER_BOLT_DAMAGE, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_water_bolt_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
                OnHit += SpellHitFn(spell_water_bolt_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_water_bolt_SpellScript();
        }
};

// Storm Weapon - 139218
class spell_storm_weapon : public SpellScriptLoader
{
    public:
        spell_storm_weapon() : SpellScriptLoader("spell_storm_weapon") { }

        class spell_storm_weapon_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_storm_weapon_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* caster = GetTarget();

                if (!caster || !eventInfo.GetActor() || !caster->ToCreature())
                    return;

                if (caster != eventInfo.GetActor())
                    return;

                if (caster->ToCreature()->HasSpellCooldown(SPELL_STORM_WEAPON_DAMAGE))
                    return;

                if (Unit* target = caster->getVictim())
                {
                    caster->CastSpell(target, SPELL_STORM_WEAPON_DAMAGE, true);
                    caster->ToCreature()->_AddCreatureSpellCooldown(SPELL_STORM_WEAPON_DAMAGE, time(NULL) + 4);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_storm_weapon_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_storm_weapon_AuraScript();
        }
};

// Focused Lightning (AoE) - 139209
class spell_focused_lightning_aoe : public SpellScriptLoader
{
    public:
        spell_focused_lightning_aoe() : SpellScriptLoader("spell_focused_lightning_aoe") { }

        class spell_focused_lightning_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_focused_lightning_aoe_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_FOCUSED_LIGHTNING_DAMAGE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_focused_lightning_aoe_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_focused_lightning_aoe_SpellScript();
        }
};

// Spirit Light - 139461
class spell_spirit_light : public SpellScriptLoader
{
    public:
        spell_spirit_light() : SpellScriptLoader("spell_spirit_light") { }

        class spell_spirit_light_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_spirit_light_SpellScript);

            SpellCastResult CheckEnnemies()
            {
                if (Unit* caster = GetCaster())
                {
                    Unit* target = GetExplTargetUnit();
                    if (!target || target->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_NO_VALID_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_spirit_light_SpellScript::CheckEnnemies);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_spirit_light_SpellScript();
        }
};

void AddSC_throne_of_thunder()
{
    new mob_zandalari_water_binder();
    new mob_zandalari_blade_initiate();
    new mob_zandalari_spear_shaper();
    new mob_thrown_spear();
    new mob_zandalari_storm_caller();
    new mob_trash_focused_lightning();
    new mob_ancient_python();
    new mob_spirit_flayer();
    new mob_tourmented_spirit();
    new mob_soul_fed_construct();
    new mob_stormbringer_draz_kil();
    new mob_dark_winds();
    new spell_storm_weapon();
    new spell_water_bolt();
    new spell_focused_lightning_aoe();
    new spell_spirit_light();
}