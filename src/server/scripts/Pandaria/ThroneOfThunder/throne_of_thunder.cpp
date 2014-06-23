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
#include "GridNotifiers.h"
#include "ObjectAccessor.h"
#include "GridNotifiersImpl.h"
#include "CellImpl.h"

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
    SPELL_DARK_WINDS_FORCE_WEATHER      = 139485,

    // Drakkari Frost Warden
    SPELL_FROST_BULWARK                 = 138668,
    SPELL_GLACIAL_FREEZE_TOTEM          = 138690,
    SPELL_GLACIAL_FREEZE_PERIODIC       = 138678,

    // Gurubashi Berserker
    SPELL_BERSERKER_FRENZY              = 138427,
    SPELL_BLOODLETTING                  = 138693,

    // Amani'Shi Flame Caster
    SPELL_ERUPTION                      = 137652,
    SPELL_ERUPTION_DAMAGE               = 138658,
    SPELL_CALL_FLAMES                   = 138607,
    SPELL_FIERY_CORE                    = 138610,
    SPELL_MOLTEN_BARRAGE                = 138651,

    // Farraki Sand Conjurer
    SPELL_CHOCKING_SANDS                = 138742,
    SPELL_CONJURE_ELEMENTALS_RIGHT      = 140634,
    SPELL_CONJURE_ELEMENTALS_LEFT       = 140635,
    SPELL_SAND_BOLT                     = 138739,

    // Zandalari High Priest
    SPELL_LIGHT_OF_THE_LOA              = 139228,

    // Zandalari Prophet
    SPELL_MARK_OF_THE_PROPHET           = 140400,
    SPELL_VISIONS_OF_DEMISE             = 140115,
    SPELL_VISIONS_OF_GRANDEUR           = 139205,

    // Zandalari Warlord
    SPELL_CLEAVE                        = 140414,
    SPELL_STRENGTH_OF_THE_LOA           = 140422,
    SPELL_ZANDALARI_WARCRY              = 134856,

    // Zandalari Prelate
    SPELL_SEAL_OF_THE_LOA               = 139212,
    SPELL_MARK_OF_THE_LOA               = 139213,
    SPELL_JUDGEMENT_OF_THE_LOA          = 139223
};

enum eEvents
{
    // Zandalari Water-Binder
    EVENT_BIND_WATER            = 1,
    EVENT_DELUGE,
    EVENT_FROSTBOLT,

    // Zandalari Blade Initiate
    EVENT_WOUNDING_STRIKE,

    // Zandalari Spear-Shaper
    EVENT_BERSERKING,
    EVENT_RETRIEVE_SPEAR,
    EVENT_SPEAR_SPIN,
    EVENT_THROW_SPEAR,

    // Zandalari Storm-Caller
    EVENT_WATER_BOLT,
    EVENT_STORM_WEAPON,
    EVENT_FOCUSED_LIGHTNING,
    EVENT_FOCUSED_LIGHTNING_AOE,

    // Ancient Python
    EVENT_ANCIENT_VENOM,

    // Spirit Flayer
    EVENT_SPIRIT_LIGHT,

    // Tormented Spirit
    EVENT_TORMENT,

    // Soul-Fed Construct
    EVENT_CRUSH_ARMOR,
    EVENT_SPIRITFIRE_BEAM,

    // Stormbringer Draz'Kil
    EVENT_CHAIN_LIGHTNING,
    EVENT_STORMCLOUD,

    // Drakkari Frost Warden
    EVENT_FROST_BULWARK,
    EVENT_GLACIAL_FREEZE_TOTEM,

    // Gurubashi Berserker
    EVENT_BERSERKER_FRENZY,
    EVENT_BLOODLETTING,

    // Amani'Shi Flame Chanter
    EVENT_ERUPTION,
    EVENT_CALL_FLAMES,

    // Farraki Sand Conjurer
    EVENT_CHOCKING_SAND,
    EVENT_CONJURE_ELEMENTALS,
    EVENT_SAND_BOLT,

    // Zandalari High Priest
    EVENT_LIGHT_OF_THE_LOA,

    // Zandalari Prophet
    EVENT_MARK_OF_THE_PROPHET,
    EVENT_VISIONS_OF_DEMISE,
    EVENT_VISIONS_OF_GRANDEUR,

    // Zandalari Warlord
    EVENT_CLEAVE,
    EVENT_STRENGTH_OF_THE_LOA,
    EVENT_ZANDALARI_WARCRY,

    // Zandalari Prelate
    EVENT_JUDGEMENT_OF_THE_LOA
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
    DRAZ_KIL_SPEAR      = 94247,

    // Drakkari Frost Warden
    FROST_WARDEN_EQ_1   = 94118,
    FROST_WARDEN_EQ_2   = 94194,

    // Gurubashi Berserker
    GURUBASHI_ZERK_EQ_1 = 90001,
    GURUBASHI_ZERK_EQ_2 = 90007,

    // Amani'Shi Flame Chanter
    FLAME_CHANTER_STAFF = 93241,

    // Farraki Sand Conjurer
    SAND_CONJURER_AXE   = 93238,

    // Zandalari High Priest
    HIGH_PRIEST_STAFF   = 94134,

    // Zandalari Prophet
    PROPHET_WEAPON      = 94248,

    // Zandalari Warlord
    WARLORD_WEAPON      = 93283,

    // Zandalari Prelate
    PRELATE_MAIN_HAND   = 94122,
    PRELATE_OFF_HAND    = 94193
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
            mob_stormbringer_draz_kilAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

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

            void JustDied(Unit* killer)
            {
                if (pInstance)
                    pInstance->DoRemoveAurasDueToSpellOnPlayers(SPELL_STORMCLOUD);
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

// Drakkari Frost Warden - 69910
class mob_drakkari_frost_warden : public CreatureScript
{
    public:
        mob_drakkari_frost_warden() : CreatureScript("mob_drakkari_frost_warden") { }

        struct mob_drakkari_frost_wardenAI : public ScriptedAI
        {
            mob_drakkari_frost_wardenAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, FROST_WARDEN_EQ_1, FROST_WARDEN_EQ_2, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_FROST_BULWARK, 2000);
                events.ScheduleEvent(EVENT_GLACIAL_FREEZE_TOTEM, 5000);
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
                    case EVENT_FROST_BULWARK:
                        me->CastSpell(me, SPELL_FROST_BULWARK, false);
                        events.ScheduleEvent(EVENT_FROST_BULWARK, 10000);
                        break;
                    case EVENT_GLACIAL_FREEZE_TOTEM:
                        me->CastSpell(me, SPELL_GLACIAL_FREEZE_TOTEM, true);
                        events.ScheduleEvent(EVENT_GLACIAL_FREEZE_TOTEM, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_drakkari_frost_wardenAI(creature);
        }
};

// Glacial Freeze Totem - 70047
class mob_gacial_freeze_totem : public CreatureScript
{
    public:
        mob_gacial_freeze_totem() : CreatureScript("mob_gacial_freeze_totem") { }

        struct mob_gacial_freeze_totemAI : public ScriptedAI
        {
            mob_gacial_freeze_totemAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                me->CastSpell(me, SPELL_GLACIAL_FREEZE_PERIODIC, true);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gacial_freeze_totemAI(creature);
        }
};

// Gurubashi Berserker - 69905
// Gurubashi Berserker - 69916
class mob_gurubashi_berserker : public CreatureScript
{
    public:
        mob_gurubashi_berserker() : CreatureScript("mob_gurubashi_berserker") { }

        struct mob_gurubashi_berserkerAI : public ScriptedAI
        {
            mob_gurubashi_berserkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, GURUBASHI_ZERK_EQ_1, GURUBASHI_ZERK_EQ_2, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_BERSERKER_FRENZY, 1000);
                events.ScheduleEvent(EVENT_BLOODLETTING, 5000);
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
                    case EVENT_BERSERKER_FRENZY:
                        me->CastSpell(me, SPELL_BERSERKER_FRENZY, false);
                        break;
                    case EVENT_BLOODLETTING:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM, 0, 5.0f))
                            me->CastSpell(target, SPELL_BLOODLETTING, true);
                        events.ScheduleEvent(EVENT_BLOODLETTING, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gurubashi_berserkerAI(creature);
        }
};

// Amani'shi Flame Chanter - 69909
class mob_amani_shi_flame_chanter : public CreatureScript
{
    public:
        mob_amani_shi_flame_chanter() : CreatureScript("mob_amani_shi_flame_chanter") { }

        struct mob_amani_shi_flame_chanterAI : public ScriptedAI
        {
            mob_amani_shi_flame_chanterAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, FLAME_CHANTER_STAFF, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_ERUPTION, 2000);
                events.ScheduleEvent(EVENT_CALL_FLAMES, 5000);
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
                    case EVENT_ERUPTION:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_ERUPTION, false);
                        events.ScheduleEvent(EVENT_ERUPTION, 10000);
                        break;
                    case EVENT_CALL_FLAMES:
                    {
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(target, SPELL_CALL_FLAMES, false);
                            me->CastSpell(me, SPELL_FIERY_CORE, true);
                        }

                        events.ScheduleEvent(EVENT_CALL_FLAMES, 7000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_amani_shi_flame_chanterAI(creature);
        }
};

// Farraki Sand Conjurer - 69899
class mob_farraki_sand_conjurer : public CreatureScript
{
    public:
        mob_farraki_sand_conjurer() : CreatureScript("mob_farraki_sand_conjurer") { }

        struct mob_farraki_sand_conjurerAI : public ScriptedAI
        {
            mob_farraki_sand_conjurerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, SAND_CONJURER_AXE, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_CHOCKING_SAND, 2000);
                events.ScheduleEvent(EVENT_SAND_BOLT, 5000);
                events.ScheduleEvent(EVENT_CONJURE_ELEMENTALS, 8000);
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
                    case EVENT_CHOCKING_SAND:
                        me->CastSpell(me, SPELL_CHOCKING_SANDS, false);
                        events.ScheduleEvent(EVENT_CHOCKING_SAND, 15000);
                        break;
                    case EVENT_SAND_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_SAND_BOLT, false);
                        events.ScheduleEvent(EVENT_SAND_BOLT, 10000);
                        break;
                    case EVENT_CONJURE_ELEMENTALS:
                        me->CastSpell(me, SPELL_CONJURE_ELEMENTALS_RIGHT, true);
                        me->CastSpell(me, SPELL_CONJURE_ELEMENTALS_LEFT, true);
                        events.ScheduleEvent(EVENT_CONJURE_ELEMENTALS, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_farraki_sand_conjurerAI(creature);
        }
};

// Sand Elemental - 69944
class mob_sand_elemental : public CreatureScript
{
    public:
        mob_sand_elemental() : CreatureScript("mob_sand_elemental") { }

        struct mob_sand_elementalAI : public ScriptedAI
        {
            mob_sand_elementalAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                me->ReenableEvadeMode();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sand_elementalAI(creature);
        }
};

// Zandalari High Priest - 69906
class mob_zandalari_high_priest : public CreatureScript
{
    public:
        mob_zandalari_high_priest() : CreatureScript("mob_zandalari_high_priest") { }

        struct mob_zandalari_high_priestAI : public ScriptedAI
        {
            mob_zandalari_high_priestAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, HIGH_PRIEST_STAFF, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, 5000);
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
                    case EVENT_LIGHT_OF_THE_LOA:
                    {
                        std::list<Unit*> assistList;
                        {
                            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(me, me, 10.0f);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(me, assistList, u_check);
                            me->VisitNearbyObject(10.0f, searcher);
                        }

                        Unit* target = NULL;
                        if (!assistList.empty())
                        {
                            assistList.sort(JadeCore::HealthPctOrderPred());
                            target = assistList.front();
                        }

                        if (target)
                            me->CastSpell(target, SPELL_LIGHT_OF_THE_LOA, false);

                        events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, 10000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_high_priestAI(creature);
        }
};

// Zandalari Prophet - 70557
class mob_zandalari_prophet : public CreatureScript
{
    public:
        mob_zandalari_prophet() : CreatureScript("mob_zandalari_prophet") { }

        struct mob_zandalari_prophetAI : public ScriptedAI
        {
            mob_zandalari_prophetAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, PROPHET_WEAPON, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_MARK_OF_THE_PROPHET, 5000);
                events.ScheduleEvent(EVENT_VISIONS_OF_DEMISE, 10000);
                events.ScheduleEvent(EVENT_VISIONS_OF_GRANDEUR, 10000);
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
                    case EVENT_MARK_OF_THE_PROPHET:
                        if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(target, SPELL_MARK_OF_THE_PROPHET, false);
                        events.ScheduleEvent(EVENT_MARK_OF_THE_PROPHET, 10000);
                        break;
                    case EVENT_VISIONS_OF_DEMISE:
                        me->CastSpell(me, SPELL_VISIONS_OF_DEMISE, false);
                        events.ScheduleEvent(EVENT_VISIONS_OF_DEMISE, 15000);
                        break;
                    case EVENT_VISIONS_OF_GRANDEUR:
                        me->CastSpell(me, SPELL_VISIONS_OF_GRANDEUR, false);
                        events.ScheduleEvent(EVENT_VISIONS_OF_GRANDEUR, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_prophetAI(creature);
        }
};

// Zandalari Warlord - 69911
class mob_zandalari_warlord : public CreatureScript
{
    public:
        mob_zandalari_warlord() : CreatureScript("mob_zandalari_warlord") { }

        struct mob_zandalari_warlordAI : public ScriptedAI
        {
            mob_zandalari_warlordAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                SetEquipmentSlots(false, WARLORD_WEAPON, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_CLEAVE, 5000);
                events.ScheduleEvent(EVENT_STRENGTH_OF_THE_LOA, 8000);
                events.ScheduleEvent(EVENT_ZANDALARI_WARCRY, 10000);
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
                    case EVENT_CLEAVE:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_CLEAVE, true);
                        events.ScheduleEvent(EVENT_CLEAVE, 8000);
                        break;
                    case EVENT_STRENGTH_OF_THE_LOA:
                        me->CastSpell(me, SPELL_STRENGTH_OF_THE_LOA, false);
                        events.ScheduleEvent(EVENT_STRENGTH_OF_THE_LOA, 10000);
                        break;
                    case EVENT_ZANDALARI_WARCRY:
                        me->CastSpell(me, SPELL_ZANDALARI_WARCRY, true);
                        events.ScheduleEvent(EVENT_ZANDALARI_WARCRY, 12000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_warlordAI(creature);
        }
};

// Zandalari Prelate - 69927
class mob_zandalari_prelate : public CreatureScript
{
    public:
        mob_zandalari_prelate() : CreatureScript("mob_zandalari_prelate") { }

        struct mob_zandalari_prelateAI : public ScriptedAI
        {
            mob_zandalari_prelateAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();
                me->CastSpell(me, SPELL_SEAL_OF_THE_LOA, true);

                SetEquipmentSlots(false, PRELATE_MAIN_HAND, PRELATE_OFF_HAND, EQUIP_NO_CHANGE);

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_JUDGEMENT_OF_THE_LOA, 8000);
                events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, 10000);
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
                    case EVENT_JUDGEMENT_OF_THE_LOA:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_JUDGEMENT_OF_THE_LOA, true);
                        events.ScheduleEvent(EVENT_JUDGEMENT_OF_THE_LOA, 10000);
                        break;
                    case EVENT_LIGHT_OF_THE_LOA:
                    {
                        std::list<Unit*> assistList;
                        {
                            JadeCore::AnyFriendlyUnitInObjectRangeCheck u_check(me, me, 10.0f);
                            JadeCore::UnitListSearcher<JadeCore::AnyFriendlyUnitInObjectRangeCheck> searcher(me, assistList, u_check);
                            me->VisitNearbyObject(10.0f, searcher);
                        }

                        Unit* target = NULL;
                        if (!assistList.empty())
                        {
                            assistList.sort(JadeCore::HealthPctOrderPred());
                            target = assistList.front();
                        }

                        if (target)
                            me->CastSpell(target, SPELL_LIGHT_OF_THE_LOA, false);

                        events.ScheduleEvent(EVENT_LIGHT_OF_THE_LOA, 15000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_prelateAI(creature);
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

// Glacial Freeze (damage & stun) - 138687
class spell_glacial_freeze : public SpellScriptLoader
{
    public:
        spell_glacial_freeze() : SpellScriptLoader("spell_glacial_freeze") { }

        class spell_glacial_freeze_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_glacial_freeze_SpellScript);

            void HandleAfterCast()
            {
                if (GetCaster()->ToCreature())
                    GetCaster()->ToCreature()->DespawnOrUnsummon();
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_glacial_freeze_SpellScript::HandleAfterCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_glacial_freeze_SpellScript();
        }
};

// Eruption - 138652
class spell_eruption : public SpellScriptLoader
{
    public:
        spell_eruption() : SpellScriptLoader("spell_eruption") { }

        class spell_eruption_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_eruption_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, SPELL_ERUPTION_DAMAGE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_eruption_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_eruption_SpellScript();
        }
};

// Fiery Core - 138610
class spell_fiery_core : public SpellScriptLoader
{
    public:
        spell_fiery_core() : SpellScriptLoader("spell_fiery_core") { }

        class spell_fiery_core_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fiery_core_AuraScript);

            void OnTick(constAuraEffectPtr aurEff)
            {
                if (AuraPtr fieryCore = aurEff->GetBase())
                {
                    if (fieryCore->GetStackAmount() >= 5)
                    {
                        GetTarget()->RemoveAura(SPELL_FIERY_CORE);
                        GetTarget()->CastSpell(GetTarget(), SPELL_MOLTEN_BARRAGE, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_fiery_core_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_fiery_core_AuraScript();
        }
};

// Judgement of the Loa - 139223
class spell_judgement_of_the_loa : public SpellScriptLoader
{
    public:
        spell_judgement_of_the_loa() : SpellScriptLoader("spell_judgement_of_the_loa") { }

        class spell_judgement_of_the_loa_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_judgement_of_the_loa_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                    {
                        int32 stacks = 1;
                        if (AuraPtr loaStacks = target->GetAura(SPELL_MARK_OF_THE_LOA))
                            stacks = loaStacks->GetStackAmount();

                        SetHitDamage(GetHitDamage() * stacks);
                    }
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_judgement_of_the_loa_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_judgement_of_the_loa_SpellScript();
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
    new mob_drakkari_frost_warden();
    new mob_gacial_freeze_totem();
    new mob_gurubashi_berserker();
    new mob_amani_shi_flame_chanter();
    new mob_farraki_sand_conjurer();
    new mob_sand_elemental();
    new mob_zandalari_high_priest();
    new mob_zandalari_prophet();
    new mob_zandalari_warlord();
    new mob_zandalari_prelate();
    new spell_storm_weapon();
    new spell_water_bolt();
    new spell_focused_lightning_aoe();
    new spell_spirit_light();
    new spell_glacial_freeze();
    new spell_eruption();
    new spell_fiery_core();
    new spell_judgement_of_the_loa();
}