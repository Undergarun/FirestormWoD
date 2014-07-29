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
#include "Vehicle.h"

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
    SPELL_SOUL_FED_CRUSH_ARMOR          = 33661,
    SPELL_SPIRITFIRE_BEAM               = 139895,

    // Stormbringer Draz'Kil
    SPELL_CHAIN_LIGHTNING               = 139903,
    SPELL_STORMCLOUD                    = 139900,
    SPELL_STORMBRINGER_VISUAL           = 139871,

    // General Purpose Bunny JMF (Ground)
    SPELL_DARK_WINDS_SMALL              = 139499,
    SPELL_DARK_WINDS_LARGE              = 140781,
    SPELL_DARK_WINDS_LONG               = 139535,

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
    SPELL_JUDGEMENT_OF_THE_LOA          = 139223,

    // Waterspout
    SPELL_WATERSPOUT                    = 140809,
    SPELL_WATERSPOUT_TRIGGERED          = 140814,

    // Greater Cave Bat
    SPELL_SLASHING_TALONS               = 136753,
    SPELL_SONIC_SCREECH                 = 136751,

    // Mysterious Mushroom
    SPELL_SIZE_95_120                   = 123978,
    SPELL_FUNGAL_EXPLOSION              = 140596,
    SPELL_FUNGAL_EXPLOSION_DAMAGE       = 140598,

    // Shale Stalker
    SPELL_SHALE_SHARDS                  = 140616,

    // Fungal Growth
    SPELL_GROW                          = 140626,
    SPELL_FUNGI_SPORES                  = 140620,

    // Mist Lurker
    SPELL_CHOCKING_MISTS                = 140682,
    SPELL_CORROSIVE_BREATH              = 140684,

    // Cavern Burrower
    SPELL_SONIC_CALL                    = 140600,
    SPELL_CRYSTAL_BARBS                 = 140619,
    SPELL_CAVERN_CRUSH_ARMOR            = 140618,
    SPELL_COSMETIC_ALPHA_STATE          = 80808,
    SPELL_SUBMERGE                      = 140586,

    // Eternal Guardian
    SPELL_ETERNAL_PRISON                = 140629,
    SPELL_LIGHTNING_NOVA                = 140628,
    SPELL_SIPHON_LIFE                   = 140630,
    SPELL_ETERNAL_GUARDIAN_SPAWN        = 140577,
    SPELL_ACTIVATE_BELL                 = 140627,
    SPELL_BELL_SHAKE                    = 139179,

    // Bore Worm
    SPELL_WORM_SUBMERGE                 = 134447,
    SPELL_SHIFTING_DOOM                 = 134452,

    // Bow Fly Swarm
    SPELL_LUCIFERASE                    = 134470,

    // Gastropod
    SPELL_ABRASIVE_RADULA               = 134414,
    SPELL_SNAIL_SHELL                   = 134434,
    SPELL_FIXATED                       = 140306,
    SPELL_DEVOURED                      = 134415,
    SPELL_SLIME_TRAIL_AREATRIGGER       = 134397,
    SPELL_SLIME_TRAIL_AURA              = 134398,

    // Web and Corpse Spider
    SPELL_CORPSE_SPIDER_WEB_SPAWN       = 134483,
    SPELL_CORPSE_SPIDER_SPAWN           = 134481,
    SPELL_WEB_SPRAY                     = 139498
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
    EVENT_JUDGEMENT_OF_THE_LOA,

    // Waterspout
    EVENT_WATERSPOUT,

    // Greater Cave Bat
    EVENT_SLASHING_TALONS,
    EVENT_SONIC_SCREECH,

    // Mysterious Mushroom
    EVENT_CHECK_PLAYER,

    // Shale Stalker
    EVENT_SHALE_SHARDS,

    // Fungal Growth
    EVENT_GROW,
    EVENT_FUNGI_SPORES,

    // Mist Lurker
    EVENT_CHOCKING_MISTS,
    EVENT_CORRISIVE_BREATH,

    // Cavern Burrower
    EVENT_SONIC_CALL,
    EVENT_CRYSTAL_BARBS,

    // Eternal Guardian
    EVENT_ETERNAL_PRISON,
    EVENT_LIGHTNING_NOVA,
    EVENT_SIPHON_LIFE,

    // Bore Worm
    EVENT_SHIFTING_DOOM,

    // Bow Fly Swarm
    EVENT_LUCIFERASE,

    // Gastropod
    EVENT_FIXATED,
    EVENT_SLIME_TRAIL,

    // Web and Corpse Spider
    EVENT_WEB_SPRAY
};

enum eActions
{
    ACTION_SPIDER_ENGAGED,
    ACTION_SWITCH_FIXATED
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
                            me->CastSpell(target, SPELL_SOUL_FED_CRUSH_ARMOR, true);
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
                        plr->CastSpell(plr, SPELL_DARK_WINDS_FORCE_WEATHER, true);
                    }
                    else
                        plr->SendApplyMovementForce(false, darkWindSourcePos, force);
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

// Waterspout - 70147
class mob_waterspout : public CreatureScript
{
    public:
        mob_waterspout() : CreatureScript("mob_waterspout") { }

        struct mob_waterspoutAI : public ScriptedAI
        {
            mob_waterspoutAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->CastSpell(me, SPELL_WATERSPOUT, true);

                events.ScheduleEvent(EVENT_WATERSPOUT, 1000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_WATERSPOUT:
                        me->CastSpell(me, SPELL_WATERSPOUT_TRIGGERED, true);
                        events.ScheduleEvent(EVENT_WATERSPOUT, 1000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_waterspoutAI(creature);
        }
};

// Vampiric Cave Bat - 69352
class mob_vampiric_cave_bat : public CreatureScript
{
    public:
        mob_vampiric_cave_bat() : CreatureScript("mob_vampiric_cave_bat") { }

        struct mob_vampiric_cave_batAI : public ScriptedAI
        {
            mob_vampiric_cave_batAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->CastSpell(me, SPELL_DRAIN_THE_WEAK, true);
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
            return new mob_vampiric_cave_batAI(creature);
        }
};

// Greater Cave Bat - 69351
class mob_greater_cave_bat : public CreatureScript
{
    public:
        mob_greater_cave_bat() : CreatureScript("mob_greater_cave_bat") { }

        struct mob_greater_cave_batAI : public ScriptedAI
        {
            mob_greater_cave_batAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                me->ReenableEvadeMode();

                events.Reset();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_SLASHING_TALONS, 8000);
                events.ScheduleEvent(EVENT_SONIC_SCREECH, 3000);
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
                    case EVENT_SLASHING_TALONS:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_SLASHING_TALONS, true);
                        events.ScheduleEvent(EVENT_SLASHING_TALONS, 8000);
                        break;
                    case EVENT_SONIC_SCREECH:
                        me->CastSpell(me, SPELL_SONIC_SCREECH, false);
                        events.ScheduleEvent(EVENT_SONIC_SCREECH, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_greater_cave_batAI(creature);
        }
};

// Mysterious Mushroom - 70545
class mob_mysterious_mushroom : public CreatureScript
{
    public:
        mob_mysterious_mushroom() : CreatureScript("mob_mysterious_mushroom") { }

        struct mob_mysterious_mushroomAI : public ScriptedAI
        {
            mob_mysterious_mushroomAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISABLE_TURN);
                me->CastSpell(me, SPELL_SIZE_95_120, true);
                me->CastSpell(me, SPELL_FUNGAL_EXPLOSION, true);

                events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_CHECK_PLAYER:
                        if (Unit* target = me->SelectNearestPlayerNotGM(4.f))
                        {
                            me->CastSpell(me, SPELL_FUNGAL_EXPLOSION_DAMAGE, true);
                            me->DespawnOrUnsummon();
                            return;
                        }
                        events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mysterious_mushroomAI(creature);
        }
};

// Shale Stalker - 70587
class mob_shale_stalker : public CreatureScript
{
    public:
        mob_shale_stalker() : CreatureScript("mob_shale_stalker") { }

        struct mob_shale_stalkerAI : public ScriptedAI
        {
            mob_shale_stalkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_SHALE_SHARDS, 5000);
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
                    case EVENT_SHALE_SHARDS:
                        me->CastSpell(me, SPELL_SHALE_SHARDS, true);
                        events.ScheduleEvent(EVENT_SHALE_SHARDS, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_shale_stalkerAI(creature);
        }
};

// Fungal Growth - 70153
class mob_fungal_growth : public CreatureScript
{
    public:
        mob_fungal_growth() : CreatureScript("mob_fungal_growth") { }

        struct mob_fungal_growthAI : public ScriptedAI
        {
            mob_fungal_growthAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_GROW, 1000);
                events.ScheduleEvent(EVENT_FUNGI_SPORES, 5000);
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
                    case EVENT_GROW:
                        me->CastSpell(me, SPELL_GROW, false);
                        events.ScheduleEvent(EVENT_GROW, 6000);
                        break;
                    case EVENT_FUNGI_SPORES:
                        me->CastSpell(me, SPELL_FUNGI_SPORES, false);
                        events.ScheduleEvent(EVENT_FUNGI_SPORES, 15000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_fungal_growthAI(creature);
        }
};

// Mist Lurker - 70594
class mob_mist_lurker : public CreatureScript
{
    public:
        mob_mist_lurker() : CreatureScript("mob_mist_lurker") { }

        struct mob_mist_lurkerAI : public ScriptedAI
        {
            mob_mist_lurkerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_CHOCKING_MISTS, 4000);
                events.ScheduleEvent(EVENT_CORRISIVE_BREATH, 8000);
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
                    case EVENT_CHOCKING_MISTS:
                        me->CastSpell(me, SPELL_CHOCKING_MISTS, false);
                        events.ScheduleEvent(EVENT_CHOCKING_MISTS, 10000);
                        break;
                    case EVENT_CORRISIVE_BREATH:
                        me->CastSpell(me, SPELL_CORROSIVE_BREATH, false);
                        events.ScheduleEvent(EVENT_CORRISIVE_BREATH, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mist_lurkerAI(creature);
        }
};

// Cavern Burrower - 70589
class mob_cavern_burrower : public CreatureScript
{
    public:
        mob_cavern_burrower() : CreatureScript("mob_cavern_burrower") { }

        struct mob_cavern_burrowerAI : public ScriptedAI
        {
            mob_cavern_burrowerAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();

                me->CastSpell(me, SPELL_COSMETIC_ALPHA_STATE, true);
                me->CastSpell(me, SPELL_SUBMERGE, true);
            }

            void EnterCombat(Unit* attacker)
            {
                me->RemoveAura(SPELL_COSMETIC_ALPHA_STATE);
                me->RemoveAura(SPELL_SUBMERGE);

                events.ScheduleEvent(EVENT_CRYSTAL_BARBS, 2000);
                events.ScheduleEvent(EVENT_SONIC_CALL, 4000);
                events.ScheduleEvent(EVENT_CRUSH_ARMOR, 5000);
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
                    case EVENT_CRYSTAL_BARBS:
                        me->CastSpell(me, SPELL_CRYSTAL_BARBS, false);
                        events.ScheduleEvent(EVENT_CRYSTAL_BARBS, 15000);
                        break;
                    case EVENT_SONIC_CALL:
                        me->CastSpell(me, SPELL_SONIC_CALL, false);
                        events.ScheduleEvent(EVENT_SONIC_CALL, 20000);
                        break;
                    case EVENT_CRUSH_ARMOR:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_CAVERN_CRUSH_ARMOR, false);
                        events.ScheduleEvent(EVENT_CRUSH_ARMOR, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_cavern_burrowerAI(creature);
        }
};

// Eternal Guardian - 70586
class mob_eternal_guardian : public CreatureScript
{
    public:
        mob_eternal_guardian() : CreatureScript("mob_eternal_guardian") { }

        struct mob_eternal_guardianAI : public ScriptedAI
        {
            mob_eternal_guardianAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();

                me->CastSpell(me, SPELL_ETERNAL_GUARDIAN_SPAWN, true);
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_ETERNAL_PRISON, 4000);
                events.ScheduleEvent(EVENT_LIGHTNING_NOVA, 6000);
                events.ScheduleEvent(EVENT_SIPHON_LIFE, 8000);
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
                    case EVENT_ETERNAL_PRISON:
                        me->CastSpell(me, SPELL_ETERNAL_PRISON, false);
                        events.ScheduleEvent(EVENT_ETERNAL_PRISON, 10000);
                        break;
                    case EVENT_LIGHTNING_NOVA:
                        me->CastSpell(me, SPELL_LIGHTNING_NOVA, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_NOVA, 15000);
                        break;
                    case EVENT_SIPHON_LIFE:
                        me->CastSpell(me, SPELL_SIPHON_LIFE, false);
                        events.ScheduleEvent(EVENT_SIPHON_LIFE, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_eternal_guardianAI(creature);
        }
};

// Bore Worm - 68221
class mob_bore_worm : public CreatureScript
{
    public:
        mob_bore_worm() : CreatureScript("mob_bore_worm") { }

        struct mob_bore_wormAI : public ScriptedAI
        {
            mob_bore_wormAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();

                me->CastSpell(me, SPELL_WORM_SUBMERGE, true);
            }

            void EnterCombat(Unit* attacker)
            {
                me->RemoveAura(SPELL_WORM_SUBMERGE);
                me->CastSpell(me, SPELL_SHIFTING_DOOM, true);
                events.ScheduleEvent(EVENT_SHIFTING_DOOM, 1000);
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
                    case EVENT_SHIFTING_DOOM:
                        me->CastSpell(me, SPELL_SHIFTING_DOOM, true);
                        events.ScheduleEvent(EVENT_SHIFTING_DOOM, 1000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_bore_wormAI(creature);
        }
};

// Bow Fly Swarm - 68222
class mob_bow_fly_swarm : public CreatureScript
{
    public:
        mob_bow_fly_swarm() : CreatureScript("mob_bow_fly_swarm") { }

        struct mob_bow_fly_swarmAI : public ScriptedAI
        {
            mob_bow_fly_swarmAI(Creature* creature) : ScriptedAI(creature)
            {
                spawnTimer = 2000;
            }

            EventMap events;
            uint64 bowFlyGuids[8];

            uint32 spawnTimer;

            void Reset()
            {
                events.Reset();

                me->ReenableEvadeMode();

                Position pos;
                me->GetPosition(&pos);

                for (int8 i = 0; i < 8; ++i)
                {
                    bowFlyGuids[i] = 0;

                    if (Creature* bowFly = me->SummonCreature(NPC_BLOW_FLY, pos))
                        bowFlyGuids[i] = bowFly->GetGUID();
                }
            }

            void EnterCombat(Unit* attacker)
            {
                events.ScheduleEvent(EVENT_LUCIFERASE, 3000);
            }

            void JustDied(Unit* killer)
            {
                for (uint8 i = 0; i < 8; ++i)
                {
                    if (Creature* bowFly = Creature::GetCreature(*me, bowFlyGuids[i]))
                        bowFly->DespawnOrUnsummon();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (spawnTimer)
                {
                    if (spawnTimer <= diff)
                    {
                        for (uint8 i = 0; i < 8; ++i)
                        {
                            if (Creature* bowFly = Creature::GetCreature(*me, bowFlyGuids[i]))
                            {
                                bowFly->EnterVehicle(me, i);
                                bowFly->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_PC);
                                bowFly->SetReactState(REACT_PASSIVE);
                            }
                        }

                        spawnTimer = 0;
                    }
                    else
                        spawnTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                switch (events.ExecuteEvent())
                {
                    case EVENT_LUCIFERASE:
                        me->CastSpell(me, SPELL_LUCIFERASE, false);
                        events.ScheduleEvent(EVENT_LUCIFERASE, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_bow_fly_swarmAI(creature);
        }
};

// Gastropod - 68220
class mob_gastropod : public CreatureScript
{
    public:
        mob_gastropod() : CreatureScript("mob_gastropod") { }

        struct mob_gastropodAI : public ScriptedAI
        {
            mob_gastropodAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap m_Events;
            uint64 m_FixatedTarget;

            void Reset()
            {
                m_Events.Reset();

                me->ReenableEvadeMode();

                m_FixatedTarget = 0;

                me->SetSpeed(MOVE_WALK, 0.5f);
                me->SetSpeed(MOVE_RUN, 0.5f);

                me->CastSpell(me, SPELL_SNAIL_SHELL, true);
                me->CastSpell(me, SPELL_ABRASIVE_RADULA, true);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                me->RemoveAura(SPELL_SNAIL_SHELL);
                me->CastSpell(p_Attacker, SPELL_FIXATED, true);

                Position l_Pos;
                p_Attacker->GetPosition(&l_Pos);
                me->GetMotionMaster()->MovePoint(0, l_Pos);
                m_FixatedTarget = p_Attacker->GetGUID();

                m_Events.ScheduleEvent(EVENT_FIXATED, 1000);
                m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                m_Events.ScheduleEvent(EVENT_SLIME_TRAIL, 2000);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_SWITCH_FIXATED:
                    {
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                        {
                            me->CastSpell(l_Target, SPELL_FIXATED, true);
                            me->GetMotionMaster()->Clear();
                            m_FixatedTarget = l_Target->GetGUID();

                            Position l_Pos;
                            l_Target->GetPosition(&l_Pos);
                            me->GetMotionMaster()->MovePoint(0, l_Pos);
                        }

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_FIXATED:
                    {
                        if (Unit* l_Target = Unit::GetUnit(*me, m_FixatedTarget))
                        {
                            me->GetMotionMaster()->Clear();

                            Position l_Pos;
                            l_Target->GetPosition(&l_Pos);
                            me->GetMotionMaster()->MovePoint(0, l_Pos);
                        }
                        m_Events.ScheduleEvent(EVENT_FIXATED, 1000);
                        break;
                    }
                    case EVENT_CHECK_PLAYER:
                    {
                        std::list<Player*> l_Targets;
                        Position l_Pos;

                        me->GetPosition(&l_Pos);
                        me->GetPlayerListInGrid(l_Targets, 0.f);

                        for (Player* l_Player : l_Targets)
                        {
                            if (!l_Player->isInFront(me))
                                continue;

                            if (l_Player->HasAura(SPELL_DEVOURED))
                                continue;

                            Position l_PlayerPos;
                            l_Player->GetPosition(&l_PlayerPos);

                            if (l_Pos.m_positionX >= l_PlayerPos.m_positionX - 5.f &&
                                l_Pos.m_positionX <= l_PlayerPos.m_positionX + 5.f &&
                                l_Pos.m_positionY >= l_PlayerPos.m_positionY - 5.f &&
                                l_Pos.m_positionY <= l_PlayerPos.m_positionY + 5.f)
                            {
                                me->CastSpell(l_Player, SPELL_DEVOURED, true);
                                break;
                            }
                        }

                        m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                        break;
                    }
                    case EVENT_SLIME_TRAIL:
                    {
                        me->CastSpell(me, SPELL_SLIME_TRAIL_AREATRIGGER, true);

                        std::list<AreaTrigger*> l_SlimeTrails;
                        me->GetAreaTriggerList(l_SlimeTrails, SPELL_SLIME_TRAIL_AREATRIGGER);
                        for (AreaTrigger* l_AreaTrigger : l_SlimeTrails)
                        {
                            std::list<Player*> l_Targets;
                            l_AreaTrigger->GetPlayerListInGrid(l_Targets, 2.5f);

                            for (Player* l_Plr : l_Targets)
                                me->CastSpell(l_Plr, SPELL_SLIME_TRAIL_AURA, true);
                        }

                        m_Events.ScheduleEvent(EVENT_SLIME_TRAIL, 2000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gastropodAI(creature);
        }
};

// Web - 68249
class mob_web : public CreatureScript
{
    public:
        mob_web() : CreatureScript("mob_web") { }

        struct mob_webAI : public ScriptedAI
        {
            mob_webAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            uint32 m_VehicleTimer;
            uint64 m_CorpseSpiderGuid;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_IMMUNE_TO_PC);
                me->SetReactState(REACT_PASSIVE);

                me->CastSpell(me, SPELL_CORPSE_SPIDER_WEB_SPAWN, true);

                m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);

                m_VehicleTimer = 2000;
                m_CorpseSpiderGuid = 0;

                Position l_Pos;
                me->GetPosition(&l_Pos);

                if (Creature* l_Spider = me->SummonCreature(NPC_CORPSE_SPIDER, l_Pos))
                    m_CorpseSpiderGuid = l_Spider->GetGUID();
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_VehicleTimer)
                {
                    if (m_VehicleTimer <= diff)
                    {
                        m_VehicleTimer = 0;

                        if (Creature* l_Spider = Creature::GetCreature(*me, m_CorpseSpiderGuid))
                            l_Spider->EnterVehicle(me, 0);
                    }
                    else
                        m_VehicleTimer -= diff;
                }

                m_Events.Update(diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_PLAYER:
                    {
                        std::list<Player*> l_Players;
                        me->GetPlayerListInGrid(l_Players, 4.0f);

                        if (!l_Players.empty() && me->GetVehicleKit())
                        {
                            Unit* l_Target = l_Players.front();

                            if (Unit* l_Spider = me->GetVehicleKit()->GetPassenger(0))
                            {
                                me->RemoveAura(SPELL_CORPSE_SPIDER_WEB_SPAWN);
                                l_Spider->EnterVehicle(me, 1);
                                l_Spider->ToCreature()->AI()->DoAction(ACTION_SPIDER_ENGAGED);
                                break;
                            }
                        }

                        m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 1000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_webAI(creature);
        }
};

// Corpse Spider - 68248
class mob_corpse_spider : public CreatureScript
{
    public:
        mob_corpse_spider() : CreatureScript("mob_corpse_spider") { }

        struct mob_corpse_spiderAI : public ScriptedAI
        {
            mob_corpse_spiderAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_Events.Reset();

                me->CastSpell(me, SPELL_CORPSE_SPIDER_SPAWN, true);
            }

            void EnterCombat(Unit* attacker)
            {
                m_Events.ScheduleEvent(EVENT_WEB_SPRAY, 5000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_SPIDER_ENGAGED && me->HasAura(SPELL_CORPSE_SPIDER_SPAWN))
                    me->RemoveAura(SPELL_CORPSE_SPIDER_SPAWN);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_WEB_SPRAY:
                        me->CastSpell(me, SPELL_WEB_SPRAY, false);
                        m_Events.ScheduleEvent(EVENT_WEB_SPRAY, 10000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_corpse_spiderAI(creature);
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

// Waterspout (triggered) - 140814
class spell_waterspout : public SpellScriptLoader
{
    public:
        spell_waterspout() : SpellScriptLoader("spell_waterspout") { }

        class spell_waterspout_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_waterspout_SpellScript);

            SpellCastResult CheckEnnemies()
            {
                if (Unit* caster = GetCaster())
                {
                    Unit* target = caster->SelectNearbyTarget(NULL, 2.0f);
                    if (!target || target->GetTypeId() != TYPEID_PLAYER)
                        return SPELL_FAILED_NO_VALID_TARGETS;
                }

                return SPELL_CAST_OK;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_waterspout_SpellScript::CheckEnnemies);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_waterspout_SpellScript();
        }
};

// Drain the Weak - 135103
class spell_drain_the_weak : public SpellScriptLoader
{
    public:
        spell_drain_the_weak() : SpellScriptLoader("spell_drain_the_weak") { }

        class spell_drain_the_weak_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_drain_the_weak_AuraScript);

            void OnProc(constAuraEffectPtr aurEff, ProcEventInfo& eventInfo)
            {
                PreventDefaultAction();
                Unit* caster = GetTarget();

                if (!caster || !eventInfo.GetActor() || !caster->ToCreature())
                    return;

                if (caster != eventInfo.GetActor())
                    return;

                if (caster->ToCreature()->HasSpellCooldown(SPELL_DRAIN_THE_WEAK_TRIGGERED))
                    return;

                if (Unit* target = caster->getVictim())
                {
                    if (target->GetHealth() >= 350000)
                        return;

                    caster->CastSpell(target, SPELL_DRAIN_THE_WEAK_TRIGGERED, true);
                    caster->ToCreature()->_AddCreatureSpellCooldown(SPELL_DRAIN_THE_WEAK_TRIGGERED, time(NULL) + 2);
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_drain_the_weak_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_drain_the_weak_AuraScript();
        }
};

// Drain the Weak (damage) - 135101
class spell_drain_the_weak_damage : public SpellScriptLoader
{
    public:
        spell_drain_the_weak_damage() : SpellScriptLoader("spell_drain_the_weak_damage") { }

        class spell_drain_the_weak_damage_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_drain_the_weak_damage_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    int32 bp = GetHitDamage() * 50;
                    caster->CastCustomSpell(caster, SPELL_DRAIN_THE_WEAK_HEAL, &bp, NULL, NULL, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_drain_the_weak_damage_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_drain_the_weak_damage_SpellScript();
        }
};

class SonicCallTargetSelector
{
    public:
        bool operator()(Unit* unit) const
        {
            return unit->GetEntry() != NPC_CAVERN_BURROWER;
        }

        bool operator()(WorldObject* object) const
        {
            return !object->ToUnit() || object->ToUnit()->GetEntry() != NPC_CAVERN_BURROWER;
        }
};

// Sonic Call - 140600
class spell_sonic_call : public SpellScriptLoader
{
    public:
        spell_sonic_call() : SpellScriptLoader("spell_sonic_call") { }

        class spell_sonic_call_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_sonic_call_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if(SonicCallTargetSelector());
            }

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (!GetHitUnit() || !caster->getVictim())
                        return;

                    if (Creature* borrower = GetHitUnit()->ToCreature())
                        borrower->AI()->AttackStart(caster->getVictim());
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_sonic_call_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnHit += SpellHitFn(spell_sonic_call_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_sonic_call_SpellScript();
        }
};

class SiphonLifeTargetSelector
{
    public:
        SiphonLifeTargetSelector(Unit* caster) : _caster(caster) { }

        bool operator()(Unit* unit) const
        {
            return unit->GetDistance(_caster) <= 15.0f;
        }

        bool operator()(WorldObject* object) const
        {
            return object->GetDistance(_caster) <= 15.0f;
        }

    private:
        Unit* _caster;
};

// Siphon Life - 140630
class spell_siphon_life : public SpellScriptLoader
{
    public:
        spell_siphon_life() : SpellScriptLoader("spell_siphon_life") { }

        class spell_siphon_life_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_siphon_life_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& targets)
            {
                if (targets.empty())
                    return;

                targets.remove_if(SiphonLifeTargetSelector(GetCaster()));
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_siphon_life_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_siphon_life_SpellScript();
        }
};

// Luciferase - 134470
class spell_luciferase : public SpellScriptLoader
{
    public:
        spell_luciferase() : SpellScriptLoader("spell_luciferase") { }

        class spell_luciferase_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_luciferase_SpellScript);

            void HandleOnHit()
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetHitUnit())
                        caster->CastSpell(target, GetSpellInfo()->Effects[0].TriggerSpell, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_luciferase_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_luciferase_SpellScript();
        }
};

// Fixated - 140306
class spell_fixated : public SpellScriptLoader
{
    public:
        spell_fixated() : SpellScriptLoader("spell_fixated") { }

        class spell_fixated_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fixated_AuraScript);

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes mode)
            {
                if (!GetCaster())
                    return;

                if (Creature* l_Gastropod = GetCaster()->ToCreature())
                    l_Gastropod->AI()->DoAction(ACTION_SWITCH_FIXATED);
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_fixated_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_fixated_AuraScript();
        }
};

// Devoured - 134415
class spell_devoured : public SpellScriptLoader
{
    public:
        spell_devoured() : SpellScriptLoader("spell_devoured") { }

        class spell_devoured_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_devoured_AuraScript);

            void OnRemove(constAuraEffectPtr aurEff, AuraEffectHandleModes mode)
            {
                if (Unit* target = GetTarget())
                {
                    if (Unit* caster = GetCaster())
                        caster->Kill(target, false, GetSpellInfo());
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_devoured_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_STUN, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_devoured_AuraScript();
        }
};

// Ancient Mogu Bell - 218723
class go_ancient_mogu_bell : public GameObjectScript
{
    public:
        go_ancient_mogu_bell() : GameObjectScript("go_ancient_mogu_bell") { }

        bool OnGossipHello(Player* /*player*/, GameObject* go)
        {
            InstanceScript* pInstance = go->GetInstanceScript();
            if (!pInstance)
                return false;

            /*if (pInstance->GetBossState(DATA_TORTOS) != DONE)
                return false;*/

            if (go->GetGoState() == GO_STATE_ACTIVE)
                return false;

            pInstance->SetData(DATA_ANCIENT_MOGU_BELL, 1);
            go->SetGoState(GO_STATE_ACTIVE);

            if (Creature* bunny = GetClosestCreatureWithEntry(go, NPC_SLG_GENERIC_MOP, 10.0f))
            {
                bunny->CastSpell(bunny, SPELL_ACTIVATE_BELL, true);

                switch (pInstance->GetData(DATA_ANCIENT_MOGU_BELL))
                {
                    case 1:
                        bunny->CastSpell(bunny, SPELL_BELL_SHAKE, true);
                        bunny->MonsterTextEmote("The cavern trembles violently!", 0, true);
                        break;
                    case 2:
                        bunny->MonsterTextEmote("An ancient beast stir within the mists!", 0, true);
                        break;
                    case 3:
                        bunny->MonsterTextEmote("|cFFF00000Megaera|r rises from the mists!", 0, true);
                        break;
                    default:
                        break;
                }
            }

            return true;
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
    new mob_waterspout();
    new mob_vampiric_cave_bat();
    new mob_greater_cave_bat();
    new mob_mysterious_mushroom();
    new mob_shale_stalker();
    new mob_fungal_growth();
    new mob_mist_lurker();
    new mob_cavern_burrower();
    new mob_eternal_guardian();
    new mob_bore_worm();
    new mob_bow_fly_swarm();
    new mob_gastropod();
    new mob_web();
    new mob_corpse_spider();
    new spell_storm_weapon();
    new spell_water_bolt();
    new spell_focused_lightning_aoe();
    new spell_spirit_light();
    new spell_glacial_freeze();
    new spell_eruption();
    new spell_fiery_core();
    new spell_judgement_of_the_loa();
    new spell_waterspout();
    new spell_drain_the_weak();
    new spell_drain_the_weak_damage();
    new spell_sonic_call();
    new spell_siphon_life();
    new spell_luciferase();
    new spell_fixated();
    new spell_devoured();
    new go_ancient_mogu_bell();
}
