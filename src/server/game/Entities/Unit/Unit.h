////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __UNIT_H
#define __UNIT_H

#include "Common.h"
#include "Object.h"
#include "Opcodes.h"
#include "SpellAuraDefines.h"
#include "UpdateFields.h"
#include "SharedDefines.h"
#include "ThreatManager.h"
#include "HostileRefManager.h"
#include "FollowerReference.h"
#include "FollowerRefManager.h"
#include "EventProcessor.h"
#include "MotionMaster.h"
#include "DBCStructure.h"
#include "SpellInfo.h"
#include "Path.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "../SharedPtrs/SharedPtrs.h"
#include "Timer.h"
#include "../DynamicObject/DynamicObject.h"
#include "../AreaTrigger/AreaTrigger.h"
#include "../Conversation/Conversation.hpp"

#define WORLD_TRIGGER   12999

enum SpellInterruptFlags
{
    SPELL_INTERRUPT_FLAG_MOVEMENT     = 0x01, // why need this for instant?
    SPELL_INTERRUPT_FLAG_PUSH_BACK    = 0x02, // push back
    SPELL_INTERRUPT_FLAG_UNK3         = 0x04, // any info?
    SPELL_INTERRUPT_FLAG_INTERRUPT    = 0x08, // interrupt
    SPELL_INTERRUPT_FLAG_ABORT_ON_DMG = 0x10,               // _complete_ interrupt on direct damage
    //SPELL_INTERRUPT_UNK             = 0x20                // unk, 564 of 727 spells having this spell start with "Glyph"
};

// See SpellAuraInterruptFlags for other values definitions
enum SpellChannelInterruptFlags
{
    CHANNEL_INTERRUPT_FLAG_INTERRUPT    = 0x08,  // interrupt
    CHANNEL_FLAG_DELAY                  = 0x4000
};

enum SpellAuraInterruptFlags
{
    AURA_INTERRUPT_FLAG_HITBYSPELL                  = 0x00000001,   // 0    removed when getting hit by a negative spell?
    AURA_INTERRUPT_FLAG_TAKE_DAMAGE                 = 0x00000002,   // 1    removed by any damage
    AURA_INTERRUPT_FLAG_CAST                        = 0x00000004,   // 2    cast any spells
    AURA_INTERRUPT_FLAG_MOVE                        = 0x00000008,   // 3    removed by any movement
    AURA_INTERRUPT_FLAG_TURNING                     = 0x00000010,   // 4    removed by any turning
    AURA_INTERRUPT_FLAG_JUMP                        = 0x00000020,   // 5    removed by entering combat
    AURA_INTERRUPT_FLAG_NOT_MOUNTED                 = 0x00000040,   // 6    removed by dismounting
    AURA_INTERRUPT_FLAG_NOT_ABOVEWATER              = 0x00000080,   // 7    removed by entering water
    AURA_INTERRUPT_FLAG_NOT_UNDERWATER              = 0x00000100,   // 8    removed by leaving water
    AURA_INTERRUPT_FLAG_NOT_SHEATHED                = 0x00000200,   // 9    removed by unsheathing
    AURA_INTERRUPT_FLAG_TALK                        = 0x00000400,   // 10   talk to npc / loot? action on creature
    AURA_INTERRUPT_FLAG_USE                         = 0x00000800,   // 11   mine/use/open action on gameobject
    AURA_INTERRUPT_FLAG_MELEE_ATTACK                = 0x00001000,   // 12   removed by attacking
    AURA_INTERRUPT_FLAG_SPELL_ATTACK                = 0x00002000,   // 13   ???
    AURA_INTERRUPT_FLAG_UNK14                       = 0x00004000,   // 14
    AURA_INTERRUPT_FLAG_TRANSFORM                   = 0x00008000,   // 15   removed by transform?
    AURA_INTERRUPT_FLAG_UNK16                       = 0x00010000,   // 16
    AURA_INTERRUPT_FLAG_MOUNT                       = 0x00020000,   // 17   misdirect, aspect, swim speed
    AURA_INTERRUPT_FLAG_NOT_SEATED                  = 0x00040000,   // 18   removed by standing up (used by food and drink mostly and sleep/Fake Death like)
    AURA_INTERRUPT_FLAG_CHANGE_MAP                  = 0x00080000,   // 19   leaving map/getting teleported
    AURA_INTERRUPT_FLAG_IMMUNE_OR_LOST_SELECTION    = 0x00100000,   // 20   removed by auras that make you invulnerable, or make other to lose selection on you
    AURA_INTERRUPT_FLAG_UNK21                       = 0x00200000,   // 21
    AURA_INTERRUPT_FLAG_TELEPORTED                  = 0x00400000,   // 22
    AURA_INTERRUPT_FLAG_ENTER_PVP_COMBAT            = 0x00800000,   // 23   removed by entering pvp combat
    AURA_INTERRUPT_FLAG_DIRECT_DAMAGE               = 0x01000000,   // 24   removed by any direct damage
    AURA_INTERRUPT_FLAG_LANDING                     = 0x02000000,   // 25   removed by hitting the ground
    AURA_INTERRUPT_FLAG_UNK25                       = 0x04000000,   // 26
    AURA_INTERRUPT_FLAG_TAKE_DAMAGE_AMOUNT          = 0x08000000,   // 27   removed by any damages, difference with AURA_INTERRUPT_FLAG_TAKE_DAMAGE must be caster/target stuff
    AURA_INTERRUPT_FLAG_ENTER_COMBAT                = 0x10000000,   // 28   removed by entering pvp
    AURA_INTERRUPT_FLAG_UNK29                       = 0x20000000,   // 29
    AURA_INTERRUPT_FLAG_UNK30                       = 0x40000000,   // 30
    AURA_INTERRUPT_FLAG_LEAVE_COMBAT                = 0x80000000,   // 31   removed by leaving combat

    AURA_INTERRUPT_FLAG_NOT_VICTIM = (AURA_INTERRUPT_FLAG_HITBYSPELL | AURA_INTERRUPT_FLAG_TAKE_DAMAGE | AURA_INTERRUPT_FLAG_TAKE_DAMAGE_AMOUNT | AURA_INTERRUPT_FLAG_DIRECT_DAMAGE),
};

enum SpellModOp
{
    SPELLMOD_DAMAGE                     = 0,
    SPELLMOD_DURATION                   = 1,
    SPELLMOD_THREAT                     = 2,
    SPELLMOD_EFFECT1                    = 3,
    SPELLMOD_CHARGES                    = 4,
    SPELLMOD_RANGE                      = 5,
    SPELLMOD_RADIUS                     = 6,
    SPELLMOD_CRITICAL_CHANCE            = 7,
    SPELLMOD_ALL_EFFECTS                = 8,
    SPELLMOD_NOT_LOSE_CASTING_TIME      = 9,
    SPELLMOD_CASTING_TIME               = 10,
    SPELLMOD_COOLDOWN                   = 11,
    SPELLMOD_EFFECT2                    = 12,
    SPELLMOD_IGNORE_ARMOR               = 13,
    SPELLMOD_COST                       = 14, ///< Used when SpellPowerEntry::PowerIndex == 0
    SPELLMOD_CRIT_DAMAGE_BONUS          = 15,
    SPELLMOD_RESIST_MISS_CHANCE         = 16,
    SPELLMOD_JUMP_TARGETS               = 17,
    SPELLMOD_CHANCE_OF_SUCCESS          = 18,
    SPELLMOD_ACTIVATION_TIME            = 19,
    SPELLMOD_DAMAGE_MULTIPLIER          = 20,
    SPELLMOD_GLOBAL_COOLDOWN            = 21,
    SPELLMOD_DOT                        = 22,
    SPELLMOD_EFFECT3                    = 23,
    SPELLMOD_BONUS_MULTIPLIER           = 24,
    SPELLMOD_UNK_25                     = 25,
    SPELLMOD_PROC_PER_MINUTE            = 26,
    SPELLMOD_VALUE_MULTIPLIER           = 27,
    SPELLMOD_RESIST_DISPEL_CHANCE       = 28,
    SPELLMOD_CRIT_DAMAGE_BONUS_2        = 29, ///< One not used spell
    SPELLMOD_SPELL_COST_REFUND_ON_FAIL  = 30,
    SPELLMOD_STACK_AMOUNT               = 31,
    SPELLMOD_EFFECT4                    = 32,
    SPELLMOD_EFFECT5                    = 33,
    SPELLMOD_SPELL_COST2                = 34, ///< Used when SpellPowerEntry::PowerIndex == 1
    SPELLMOD_JUMP_DISTANCE              = 35,
    SPELLMOD_UNK_36                     = 36,
    SPELLMOD_STACK_AMOUNT2              = 37  ///< same as SPELLMOD_STACK_AMOUNT but affects tooltips
};

#define MAX_SPELLMOD 38

enum SpellValueMod
{
    SPELLVALUE_BASE_POINT0,
    SPELLVALUE_BASE_POINT1,
    SPELLVALUE_BASE_POINT2,
    SPELLVALUE_BASE_POINT3,
    SPELLVALUE_BASE_POINT4,
    SPELLVALUE_BASE_POINT5,
    SPELLVALUE_RADIUS_MOD,
    SPELLVALUE_MAX_TARGETS,
    SPELLVALUE_AURA_STACK,
};

typedef std::pair<SpellValueMod, int32>     CustomSpellValueMod;
class CustomSpellValues : public std::vector<CustomSpellValueMod>
{
    public:
        CustomSpellValues() :
        m_CustomCritChance(-1.0f)
        {
        }

        void AddSpellMod(SpellValueMod mod, int32 value)
        {
            push_back(std::make_pair(mod, value));
        }

        float GetCustomCritChance() const
        {
            return m_CustomCritChance;
        }

        void SetCustomCritChance(float p_CustomProcChance)
        {
            m_CustomCritChance = p_CustomProcChance;
        }

    private:
        float m_CustomCritChance;
};

enum SpellFacingFlags
{
    SPELL_FACING_FLAG_INFRONT = 0x0001
};

#define BASE_MINDAMAGE 1.0f
#define BASE_MAXDAMAGE 2.0f
#define BASE_ATTACK_TIME 2000

// byte value (UNIT_FIELD_BYTES_1, 0)
enum UnitStandStateType
{
    UNIT_STAND_STATE_STAND             = 0,
    UNIT_STAND_STATE_SIT               = 1,
    UNIT_STAND_STATE_SIT_CHAIR         = 2,
    UNIT_STAND_STATE_SLEEP             = 3,
    UNIT_STAND_STATE_SIT_LOW_CHAIR     = 4,
    UNIT_STAND_STATE_SIT_MEDIUM_CHAIR  = 5,
    UNIT_STAND_STATE_SIT_HIGH_CHAIR    = 6,
    UNIT_STAND_STATE_DEAD              = 7,
    UNIT_STAND_STATE_KNEEL             = 8,
    UNIT_STAND_STATE_SUBMERGED         = 9
};

// byte flag value (UNIT_FIELD_BYTES_1, 2)
enum UnitStandFlags
{
    UNIT_STAND_FLAGS_UNK1         = 0x01,
    UNIT_STAND_FLAGS_CREEP        = 0x02,
    UNIT_STAND_FLAGS_UNTRACKABLE  = 0x04,
    UNIT_STAND_FLAGS_UNK4         = 0x08,
    UNIT_STAND_FLAGS_UNK5         = 0x10,
    UNIT_STAND_FLAGS_ALL          = 0xFF
};

// byte flags value (UNIT_FIELD_BYTES_1, 3)
enum UnitBytes1_Flags
{
    UNIT_BYTE1_FLAG_ALWAYS_STAND    = 0x01,
    UNIT_BYTE1_FLAG_HOVER           = 0x02,
    UNIT_BYTE1_FLAG_UNK_3           = 0x04,
    UNIT_BYTE1_FLAG_KNEEL           = 0x08,
    UNIT_BYTE1_FLAG_ALL             = 0xFF
};

// high byte (3 from 0..3) of UNIT_FIELD_BYTES_2
enum ShapeshiftForm
{
    FORM_NONE               = 0x00,
    FORM_CAT                = 0x01,
    FORM_TREE               = 0x02,
    FORM_STAG               = 0x03,
    FORM_AQUA               = 0x04,
    FORM_BEAR               = 0x05,
    FORM_AMBIENT            = 0x06,
    FORM_GHOUL              = 0x07,
    FORM_DIREBEAR           = 0x08, // Removed in 4.0.1
    FORM_SPIRITED_CRANE     = 0x09,
    FORM_THARONJA_SKELETON  = 0x0A,
    FORM_TEST_OF_STRENGTH   = 0x0B,
    FORM_BLB_PLAYER         = 0x0C,
    FORM_SHADOW_DANCE       = 0x0D,
    FORM_CREATUREBEAR       = 0x0E,
    FORM_CREATURECAT        = 0x0F,
    FORM_GHOSTWOLF          = 0x10,
    FORM_BATTLESTANCE       = 0x11,
    FORM_DEFENSIVESTANCE    = 0x12,
    FORM_BERSERKERSTANCE    = 0x13,
    FORM_WISE_SERPENT       = 0x14,
    FORM_ZOMBIE             = 0x15,
    FORM_METAMORPHOSIS      = 0x16,
    FORM_STURDY_OX          = 0x17,
    FORM_FIERCE_TIGER       = 0x18,
    FORM_UNDEAD             = 0x19,
    FORM_MASTER_ANGLER      = 0x1A,
    FORM_FLIGHT_EPIC        = 0x1B,
    FORM_SHADOW             = 0x1C,
    FORM_FLIGHT             = 0x1D,
    FORM_STEALTH            = 0x1E,
    FORM_MOONKIN            = 0x1F,
    FORM_SPIRITOFREDEMPTION = 0x20,
    FORM_GLADIATORSTANCE    = 0x21
};

// low byte (0 from 0..3) of UNIT_FIELD_BYTES_2
enum SheathState
{
    SHEATH_STATE_UNARMED  = 0,                              // non prepared weapon
    SHEATH_STATE_MELEE    = 1,                              // prepared melee weapon
    SHEATH_STATE_RANGED   = 2                               // prepared ranged weapon
};

#define MAX_SHEATH_STATE    3

// byte (1 from 0..3) of UNIT_FIELD_BYTES_2
enum UnitPVPStateFlags
{
    UNIT_BYTE2_FLAG_PVP         = 0x01,
    UNIT_BYTE2_FLAG_UNK1        = 0x02,
    UNIT_BYTE2_FLAG_FFA_PVP     = 0x04,
    UNIT_BYTE2_FLAG_SANCTUARY   = 0x08,
    UNIT_BYTE2_FLAG_UNK4        = 0x10,
    UNIT_BYTE2_FLAG_UNK5        = 0x20,
    UNIT_BYTE2_FLAG_UNK6        = 0x40,
    UNIT_BYTE2_FLAG_UNK7        = 0x80
};

// byte (2 from 0..3) of UNIT_FIELD_BYTES_2
enum UnitRename
{
    UNIT_CAN_BE_RENAMED     = 0x01,
    UNIT_CAN_BE_ABANDONED   = 0x02,
};

#define CREATURE_MAX_SPELLS     8
#define MAX_SPELL_CHARM         4
#define MAX_SPELL_VEHICLE       6
#define MAX_SPELL_POSSESS       8
#define MAX_SPELL_CONTROL_BAR   10
#define MAX_AGGRO_RESET_TIME    10      // in seconds
#define MAX_AGGRO_RADIUS        45.0f   // yards

enum VictimState
{
    VICTIMSTATE_INTACT         = 0, // set when attacker misses
    VICTIMSTATE_HIT            = 1, // victim got clear/blocked hit
    VICTIMSTATE_DODGE          = 2,
    VICTIMSTATE_PARRY          = 3,
    VICTIMSTATE_INTERRUPT      = 4,
    VICTIMSTATE_BLOCKS         = 5, // unused? not set when blocked, even on full block
    VICTIMSTATE_EVADES         = 6,
    VICTIMSTATE_IS_IMMUNE      = 7,
    VICTIMSTATE_DEFLECTS       = 8
};

enum HitInfo
{
    HITINFO_NORMALSWING         = 0x00000000,
    HITINFO_UNK1                = 0x00000001,               // req correct packet structure
    HITINFO_AFFECTS_VICTIM      = 0x00000002,
    HITINFO_OFFHAND             = 0x00000004,
    HITINFO_UNK2                = 0x00000008,
    HITINFO_MISS                = 0x00000010,
    HITINFO_FULL_ABSORB         = 0x00000020,
    HITINFO_PARTIAL_ABSORB      = 0x00000040,
    HITINFO_FULL_RESIST         = 0x00000080,
    HITINFO_PARTIAL_RESIST      = 0x00000100,
    HITINFO_CRITICALHIT         = 0x00000200,               // critical hit
    /// 0x00000400
    /// 0x00000800
    /// 0x00001000
    HITINFO_BLOCK               = 0x00002000,               // blocked damage
    /// 0x00004000                                           // Hides worldtext for 0 damage
    /// 0x00008000                                           // Related to blood visual
    HITINFO_GLANCING            = 0x00010000,
    HITINFO_CRUSHING            = 0x00020000,
    HITINFO_NO_ANIMATION        = 0x00040000,
    /// 0x00080000
    /// 0x00100000
    HITINFO_SWINGNOHITSOUND     = 0x00200000,               // unused?
    /// 0x00400000
    HITINFO_RAGE_GAIN           = 0x00800000,
    /// 0x01000000
    /// 0x02000000
    HITINFO_MULTISTRIKE         = 0x04000000
    /// 0x08000000
    /// 0x10000000
    /// 0x20000000
    /// 0x40000000
    /// 0x80000000
};

//i would like to remove this: (it is defined in item.h
enum InventorySlot
{
    NULL_BAG                   = 0,
    NULL_SLOT                  = 255
};

struct FactionTemplateEntry;
struct SpellValue;
struct SpellDestination;

class AuraApplication;
class Aura;
class UnitAura;
class AuraEffect;
class Creature;
class Spell;
class SpellInfo;
class DynamicObject;
class AreaTrigger;
class Conversaton;
class GameObject;
class Item;
class Pet;
class PetAura;
class Minion;
class Guardian;
class UnitAI;
class Totem;
class Transport;
class Vehicle;
class TransportBase;

typedef std::list<Unit*> UnitList;
typedef std::list< std::pair<Aura*, uint8> > DispelChargesList;

struct SpellImmune
{
    uint32 type;
    uint32 spellId;
};

struct StackOnDuration
{
    std::vector<std::pair<uint64, int32>> m_StackDuration;

    StackOnDuration() {}

    StackOnDuration(uint64 p_Duration, int32 p_Amount)
    {
        m_StackDuration.push_back(std::make_pair(p_Duration, p_Amount));
    }

    std::vector<std::pair<uint64, int32>> GetStackDuration() const { return m_StackDuration; }

    void DecreaseDuration(int8 p_StackNb, uint32 p_Time)
    {
        m_StackDuration[p_StackNb].first -= p_Time;
    }

    int32 GetTotalAmount() const
    {
        int32 l_TotalAmount = 0;

        for (std::pair<uint32, int32> l_Stack : m_StackDuration)
            l_TotalAmount += l_Stack.second;

        return l_TotalAmount;
    }
};

typedef std::list<SpellImmune> SpellImmuneList;

enum UnitModifierType
{
    BASE_VALUE              = 0,
    BASE_PCT_EXCLUDE_CREATE = 1,    // percent modifier affecting all stat values from auras and gear but not player base for level
    BASE_PCT                = 2,
    TOTAL_VALUE             = 3,
    TOTAL_PCT               = 4,
    MODIFIER_TYPE_END       = 5
};

enum WeaponDamageRange
{
    MINDAMAGE,
    MAXDAMAGE
};

enum AuraRemoveMode
{
    AURA_REMOVE_NONE = 0,
    AURA_REMOVE_BY_DEFAULT = 1,       // scripted remove, remove by stack with aura with different ids and sc aura remove
    AURA_REMOVE_BY_CANCEL,
    AURA_REMOVE_BY_ENEMY_SPELL,       // dispel and absorb aura destroy
    AURA_REMOVE_BY_EXPIRE,            // aura duration has ended
    AURA_REMOVE_BY_DEATH
};

enum TriggerCastFlags
{
    TRIGGERED_NONE                                  = 0x00000000,   //! Not triggered
    TRIGGERED_IGNORE_GCD                            = 0x00000001,   //! Will ignore GCD
    TRIGGERED_IGNORE_SPELL_AND_CATEGORY_CD          = 0x00000002,   //! Will ignore Spell and Category cooldowns
    TRIGGERED_IGNORE_POWER_AND_REAGENT_COST         = 0x00000004,   //! Will ignore power and reagent cost
    TRIGGERED_IGNORE_CAST_ITEM                      = 0x00000008,   //! Will not take away cast item or update related achievement criteria
    TRIGGERED_IGNORE_AURA_SCALING                   = 0x00000010,   //! Will ignore aura scaling
    TRIGGERED_IGNORE_CAST_IN_PROGRESS               = 0x00000020,   //! Will not check if a current cast is in progress
    TRIGGERED_IGNORE_COMBO_POINTS                   = 0x00000040,   //! Will ignore combo point requirement
    TRIGGERED_CAST_DIRECTLY                         = 0x00000080,   //! In Spell::prepare, will be cast directly without setting containers for executed spell
    TRIGGERED_IGNORE_AURA_INTERRUPT_FLAGS           = 0x00000100,   //! Will ignore interruptible aura's at cast
    TRIGGERED_IGNORE_SET_FACING                     = 0x00000200,   //! Will not adjust facing to target (if any)
    TRIGGERED_IGNORE_SHAPESHIFT                     = 0x00000400,   //! Will ignore shapeshift checks
    TRIGGERED_IGNORE_CASTER_AURASTATE               = 0x00000800,   //! Will ignore caster aura states including combat requirements and death state
    TRIGGERED_IGNORE_CASTER_MOUNTED_OR_ON_VEHICLE   = 0x00002000,   //! Will ignore mounted/on vehicle restrictions
    TRIGGERED_IGNORE_CASTER_AURAS                   = 0x00010000,   //! Will ignore caster aura restrictions or requirements
    TRIGGERED_DISALLOW_PROC_EVENTS                  = 0x00020000,   //! Disallows proc events from triggered spell (default)
    TRIGGERED_DONT_REPORT_CAST_ERROR                = 0x00040000,   //! Will return SPELL_FAILED_DONT_REPORT in CheckCast functions
    TRIGGERED_FULL_MASK                             = 0xFFFFFFFF,
};

enum UnitMods
{
    UNIT_MOD_STAT_STRENGTH,                                 // UNIT_MOD_STAT_STRENGTH..UNIT_MOD_STAT_SPIRIT must be in existed order, it's accessed by index values of Stats enum.
    UNIT_MOD_STAT_AGILITY,
    UNIT_MOD_STAT_STAMINA,
    UNIT_MOD_STAT_INTELLECT,
    UNIT_MOD_STAT_SPIRIT,
    UNIT_MOD_HEALTH,
    UNIT_MOD_MANA,                                          // UNIT_MOD_MANA..UNIT_MOD_RUNIC_POWER must be in existed order, it's accessed by index values of Powers enum.
    UNIT_MOD_RAGE,
    UNIT_MOD_FOCUS,
    UNIT_MOD_ENERGY,
    UNIT_MOD_UNUSED,                                        // Old UNIT_MOD_HAPPINESS
    UNIT_MOD_RUNE,
    UNIT_MOD_RUNIC_POWER,
    UNIT_MOD_SOUL_SHARDS,
    UNIT_MOD_ECLIPSE,
    UNIT_MOD_HOLY_POWER,
    UNIT_MOD_ALTERNATIVE,
    UNIT_MOD_UNK,
    UNIT_MOD_CHI,
    UNIT_MOD_SHADOW_ORB,
    UNIT_MOD_BURNING_EMBERS,
    UNIT_MOD_DEMONIC_FURY,
    UNIT_MOD_ARMOR,                                         // UNIT_MOD_ARMOR..UNIT_MOD_RESISTANCE_ARCANE must be in existed order, it's accessed by index values of SpellSchools enum.
    UNIT_MOD_RESISTANCE_HOLY,
    UNIT_MOD_RESISTANCE_FIRE,
    UNIT_MOD_RESISTANCE_NATURE,
    UNIT_MOD_RESISTANCE_FROST,
    UNIT_MOD_RESISTANCE_SHADOW,
    UNIT_MOD_RESISTANCE_ARCANE,
    UNIT_MOD_ATTACK_POWER,
    UNIT_MOD_ATTACK_POWER_RANGED,
    UNIT_MOD_DAMAGE_MAINHAND,
    UNIT_MOD_DAMAGE_OFFHAND,
    UNIT_MOD_DAMAGE_RANGED,
    UNIT_MOD_BONUS_ARMOR,
    UNIT_MOD_END,
    // synonyms
    UNIT_MOD_STAT_START = UNIT_MOD_STAT_STRENGTH,
    UNIT_MOD_STAT_END = UNIT_MOD_STAT_SPIRIT + 1,
    UNIT_MOD_RESISTANCE_START = UNIT_MOD_ARMOR,
    UNIT_MOD_RESISTANCE_END = UNIT_MOD_RESISTANCE_ARCANE + 1,
    UNIT_MOD_POWER_START = UNIT_MOD_MANA,
    UNIT_MOD_POWER_END = UNIT_MOD_DEMONIC_FURY + 1
};

enum BaseModGroup
{
    CRIT_PERCENTAGE,
    RANGED_CRIT_PERCENTAGE,
    OFFHAND_CRIT_PERCENTAGE,
    SHIELD_BLOCK_VALUE,
    BASEMOD_END
};

enum BaseModType
{
    FLAT_MOD,
    PCT_MOD
};

#define MOD_END (PCT_MOD+1)

enum DeathState
{
    ALIVE       = 0,
    JUST_DIED   = 1,
    CORPSE      = 2,
    DEAD        = 3,
    JUST_RESPAWNED = 4,
};

enum UnitState
{
    UNIT_STATE_DIED            = 0x00000001,                     // player has fake death aura
    UNIT_STATE_MELEE_ATTACKING = 0x00000002,                     // player is melee attacking someone
    //UNIT_STATE_MELEE_ATTACK_BY = 0x00000004,                     // player is melee attack by someone
    UNIT_STATE_STUNNED         = 0x00000008,
    UNIT_STATE_ROAMING         = 0x00000010,
    UNIT_STATE_CHASE           = 0x00000020,
    //UNIT_STATE_SEARCHING       = 0x00000040,
    UNIT_STATE_FLEEING         = 0x00000080,
    UNIT_STATE_IN_FLIGHT       = 0x00000100,                     // player is in flight mode
    UNIT_STATE_FOLLOW          = 0x00000200,
    UNIT_STATE_ROOT            = 0x00000400,
    UNIT_STATE_CONFUSED        = 0x00000800,
    UNIT_STATE_DISTRACTED      = 0x00001000,
    UNIT_STATE_ISOLATED        = 0x00002000,                     // area auras do not affect other players
    UNIT_STATE_ATTACK_PLAYER   = 0x00004000,
    UNIT_STATE_CASTING         = 0x00008000,
    UNIT_STATE_POSSESSED       = 0x00010000,
    UNIT_STATE_CHARGING        = 0x00020000,
    UNIT_STATE_JUMPING         = 0x00040000,
    UNIT_STATE_ONVEHICLE       = 0x00080000,
    UNIT_STATE_MOVE            = 0x00100000,
    UNIT_STATE_ROTATING        = 0x00200000,
    UNIT_STATE_EVADE           = 0x00400000,
    UNIT_STATE_ROAMING_MOVE    = 0x00800000,
    UNIT_STATE_CONFUSED_MOVE   = 0x01000000,
    UNIT_STATE_FLEEING_MOVE    = 0x02000000,
    UNIT_STATE_CHASE_MOVE      = 0x04000000,
    UNIT_STATE_FOLLOW_MOVE     = 0x08000000,
    UNIT_STATE_IGNORE_PATHFINDING = 0x10000000,                 // do not use pathfinding in any MovementGenerator
    UNIT_STATE_UNATTACKABLE    = (UNIT_STATE_IN_FLIGHT | UNIT_STATE_ONVEHICLE),
    // for real move using movegen check and stop (except unstoppable flight)
    UNIT_STATE_MOVING          = UNIT_STATE_ROAMING_MOVE | UNIT_STATE_CONFUSED_MOVE | UNIT_STATE_FLEEING_MOVE | UNIT_STATE_CHASE_MOVE | UNIT_STATE_FOLLOW_MOVE ,
    UNIT_STATE_CONTROLLED      = (UNIT_STATE_CONFUSED | UNIT_STATE_STUNNED | UNIT_STATE_FLEEING),
    UNIT_STATE_LOST_CONTROL    = (UNIT_STATE_CONTROLLED | UNIT_STATE_JUMPING | UNIT_STATE_CHARGING),
    UNIT_STATE_SIGHTLESS       = (UNIT_STATE_LOST_CONTROL | UNIT_STATE_EVADE),
    UNIT_STATE_CANNOT_AUTOATTACK     = (UNIT_STATE_LOST_CONTROL | UNIT_STATE_CASTING),
    UNIT_STATE_CANNOT_TURN     = (UNIT_STATE_LOST_CONTROL | UNIT_STATE_ROTATING),
    // stay by different reasons
    UNIT_STATE_NOT_MOVE        = UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DIED | UNIT_STATE_DISTRACTED,
    UNIT_STATE_ALL_STATE       = 0xffffffff                      //(UNIT_STATE_STOPPED | UNIT_STATE_MOVING | UNIT_STATE_IN_COMBAT | UNIT_STATE_IN_FLIGHT)
};

enum UnitMoveType
{
    MOVE_WALK           = 0,
    MOVE_RUN            = 1,
    MOVE_RUN_BACK       = 2,
    MOVE_SWIM           = 3,
    MOVE_SWIM_BACK      = 4,
    MOVE_TURN_RATE      = 5,
    MOVE_FLIGHT         = 6,
    MOVE_FLIGHT_BACK    = 7,
    MOVE_PITCH_RATE     = 8
};

#define MAX_MOVE_TYPE     9

extern float baseMoveSpeed[MAX_MOVE_TYPE];
extern float playerBaseMoveSpeed[MAX_MOVE_TYPE];

enum WeaponAttackType
{
    BaseAttack   = 0,
    OffAttack    = 1,
    RangedAttack = 2,
    MaxAttack
};

// Last check : 6.0.3
enum CombatRating
{
    CR_UNUSED_1                         = 0,    //< Deprecated, CR_UNUSED_1 in PaperDollFrame.lua previously CR_WEAPON_SKILL
    CR_DEFENSE_SKILL                    = 1,    //< Deprecated
    CR_DODGE                            = 2,
    CR_PARRY                            = 3,
    CR_BLOCK                            = 4,
    CR_HIT_MELEE                        = 5,    //< Deprecated
    CR_HIT_RANGED                       = 6,    //< Deprecated
    CR_HIT_SPELL                        = 7,    //< Deprecated
    CR_CRIT_MELEE                       = 8,
    CR_CRIT_RANGED                      = 9,
    CR_CRIT_SPELL                       = 10,
    CR_MULTISTRIKE                      = 11,
    CR_READINESS                        = 12,   //< Deprecated
    CR_SPEED                            = 13,
    CR_RESILIENCE_CRIT_TAKEN            = 14,
    CR_RESILIENCE_PLAYER_DAMAGE_TAKEN   = 15,
    CR_LIFESTEAL                        = 16,
    CR_HASTE_MELEE                      = 17,
    CR_HASTE_RANGED                     = 18,
    CR_HASTE_SPELL                      = 19,
    CR_AVOIDANCE                        = 20,
    CR_UNUSED_2                         = 21,   //< Deprecated, CR_UNUSED_2 in PaperDollFrame.lua previously CR_WEAPON_SKILL_OFFHAND
    CR_WEAPON_SKILL_RANGED              = 22,   //< Deprecated
    CR_EXPERTISE                        = 23,   //< Deprecated
    CR_ARMOR_PENETRATION                = 24,   //< Deprecated
    CR_MASTERY                          = 25,
    CR_PVP_POWER                        = 26,   //< romoved since from the paper doll, but the stat still exist for MoP items
    CR_UNUSED_4                         = 27,   //< CR_UNUSED_4 in PaperDollFrame.lua
    CR_VERSATILITY_DAMAGE_DONE          = 28,
    CR_VERSATILITY_DAMAGE_TAKEN         = 30,
};

#define MAX_COMBAT_RATING         31

enum DamageEffectType
{
    DIRECT_DAMAGE           = 0,                            // used for normal weapon damage (not for class abilities or spells)
    SPELL_DIRECT_DAMAGE     = 1,                            // spell/class abilities damage
    DOT                     = 2,
    HEAL                    = 3,
    NODAMAGE                = 4,                            // used also in case when damage applied to health but not applied to spell channelInterruptFlags/etc
    SELF_DAMAGE             = 5
};

// Value masks for UNIT_FIELD_FLAGS
enum eUnitFlags
{
    UNIT_FLAG_SERVER_CONTROLLED     = 0x00000001,           // set only when unit movement is controlled by server - by SPLINE/MONSTER_MOVE packets, together with UNIT_FLAG_STUNNED; only set to units controlled by client; client function CGUnit_C::IsClientControlled returns false when set for owner
    UNIT_FLAG_NON_ATTACKABLE        = 0x00000002,           // not attackable
    UNIT_FLAG_DISABLE_MOVE          = 0x00000004,
    UNIT_FLAG_PVP_ATTACKABLE        = 0x00000008,           // allow apply pvp rules to attackable state in addition to faction dependent state
    UNIT_FLAG_RENAME                = 0x00000010,
    UNIT_FLAG_PREPARATION           = 0x00000020,           // don't take reagents for spells with SPELL_ATTR5_NO_REAGENT_WHILE_PREP
    UNIT_FLAG_UNK_6                 = 0x00000040,
    UNIT_FLAG_NOT_ATTACKABLE_1      = 0x00000080,           // ?? (UNIT_FLAG_PVP_ATTACKABLE | UNIT_FLAG_NOT_ATTACKABLE_1) is NON_PVP_ATTACKABLE
    UNIT_FLAG_IMMUNE_TO_PC          = 0x00000100,           // disables combat/assistance with PlayerCharacters (PC) - see Unit::_IsValidAttackTarget, Unit::_IsValidAssistTarget
    UNIT_FLAG_IMMUNE_TO_NPC         = 0x00000200,           // disables combat/assistance with NonPlayerCharacters (NPC) - see Unit::_IsValidAttackTarget, Unit::_IsValidAssistTarget
    UNIT_FLAG_LOOTING               = 0x00000400,           // loot animation
    UNIT_FLAG_PET_IN_COMBAT         = 0x00000800,           // in combat?, 2.0.8
    UNIT_FLAG_PVP                   = 0x00001000,           // changed in 3.0.3
    UNIT_FLAG_SILENCED              = 0x00002000,           // silenced, 2.1.1
    UNIT_FLAG_UNK_14                = 0x00004000,           // 2.0.8
    UNIT_FLAG_UNK_15                = 0x00008000,
    UNIT_FLAG_UNK_16                = 0x00010000,           // Cant attack
    UNIT_FLAG_PACIFIED              = 0x00020000,           // 3.0.3 ok
    UNIT_FLAG_STUNNED               = 0x00040000,           // 3.0.3 ok
    UNIT_FLAG_IN_COMBAT             = 0x00080000,
    UNIT_FLAG_TAXI_FLIGHT           = 0x00100000,           // disable casting at client side spell not allowed by taxi flight (mounted?), probably used with 0x4 flag
    UNIT_FLAG_DISARMED              = 0x00200000,           // 3.0.3, disable melee spells casting..., "Required melee weapon" added to melee spells tooltip.
    UNIT_FLAG_CONFUSED              = 0x00400000,
    UNIT_FLAG_FLEEING               = 0x00800000,
    UNIT_FLAG_PLAYER_CONTROLLED     = 0x01000000,           // used in spell Eyes of the Beast for pet... let attack by controlled creature
    UNIT_FLAG_NOT_SELECTABLE        = 0x02000000,
    UNIT_FLAG_SKINNABLE             = 0x04000000,
    UNIT_FLAG_MOUNT                 = 0x08000000,
    UNIT_FLAG_UNK_28                = 0x10000000,
    UNIT_FLAG_UNK_29                = 0x20000000,           // used in Feing Death spell
    UNIT_FLAG_SHEATHE               = 0x40000000,
    UNIT_FLAG_UNK_31                = 0x80000000
};

// Value masks for UNIT_FIELD_FLAGS_2
enum eUnitFlags2
{
    UNIT_FLAG2_FEIGN_DEATH                  = 0x00000001,
    UNIT_FLAG2_UNK1                         = 0x00000002,   // Hide unit model (show only player equip)
    UNIT_FLAG2_IGNORE_REPUTATION            = 0x00000004,
    UNIT_FLAG2_COMPREHEND_LANG              = 0x00000008,
    UNIT_FLAG2_MIRROR_IMAGE                 = 0x00000010,
    UNIT_FLAG2_INSTANTLY_APPEAR_MODEL       = 0x00000020,   // Unit model instantly appears when summoned (does not fade in)
    UNIT_FLAG2_FORCE_MOVEMENT               = 0x00000040,
    UNIT_FLAG2_DISARM_OFFHAND               = 0x00000080,
    UNIT_FLAG2_DISABLE_PRED_STATS           = 0x00000100,   // Player has disabled predicted stats (Used by raid frames)
    UNIT_FLAG2_UNK16                        = 0x00000200,
    UNIT_FLAG2_DISARM_RANGED                = 0x00000400,   // this does not disable ranged weapon display (maybe additional flag needed?)
    UNIT_FLAG2_REGENERATE_POWER             = 0x00000800,
    UNIT_FLAG2_RESTRICT_PARTY_INTERACTION   = 0x00001000,   // Restrict interaction to party or raid
    UNIT_FLAG2_PREVENT_SPELL_CLICK          = 0x00002000,   // Prevent spellclick
    UNIT_FLAG2_ALLOW_ENEMY_INTERACT         = 0x00004000,
    UNIT_FLAG2_DISABLE_TURN                 = 0x00008000,
    UNIT_FLAG2_UNK2                         = 0x00010000,
    UNIT_FLAG2_PLAY_DEATH_ANIM              = 0x00020000,   // Plays special death animation upon death
    UNIT_FLAG2_ALLOW_CHEAT_SPELLS           = 0x00040000,   // allows casting spells with AttributesEx7 & SPELL_ATTR7_IS_CHEAT_SPELL
    UNIT_FLAG2_NO_ACTIONS                   = 0x00080000,   ///< Remove Hightlight on cursor
    UNIT_FLAG2_UNK4                         = 0x00100000,
    UNIT_FLAG2_UNK5                         = 0x00200000,
    UNIT_FLAG2_UNK6                         = 0x00400000,
    UNIT_FLAG2_UNK7                         = 0x00800000,
    UNIT_FLAG2_UNK8                         = 0x01000000,
    UNIT_FLAG2_UPDATE_REACTION              = 0x02000000,
    UNIT_FLAG2_SELECTION_DISABLED           = 0x04000000,   ///< Cant select (even in GM mode)
    UNIT_FLAG2_UNK11                        = 0x08000000,
    UNIT_FLAG2_UNK12                        = 0x10000000,   ///< Cant target, hide highlight, hide name (work on faction 14/7 but not 35)
    UNIT_FLAG2_UNK13                        = 0x20000000,
    UNIT_FLAG2_UNK14                        = 0x40000000,
    UNIT_FLAG2_UNK15                        = 0x80000000
};

// Value masks for UNIT_FIELD_FLAGS3
enum eUnitFlags3
{
    UNIT_FLAG3_UNK1                         = 0x00000001,
    UNIT_FLAG3_UNK2                         = 0x00000002,
    UNIT_FLAG3_CAN_FIGHT_WITHOUT_DISMOUNT   = 0x00000004,
    UNIT_FLAG3_UNK4                         = 0x00000008,
    UNIT_FLAG3_UNK5                         = 0x00000010,
    UNIT_FLAG3_UNK6                         = 0x00000020,
    UNIT_FLAG3_UNK7                         = 0x00000040,
    UNIT_FLAG3_UNK8                         = 0x00000080,
    UNIT_FLAG3_UNK9                         = 0x00000100,
    UNIT_FLAG3_UNK10                        = 0x00000200,
    UNIT_FLAG3_UNK11                        = 0x00000400,
    UNIT_FLAG3_UNK12                        = 0x00000800,
    UNIT_FLAG3_UNK13                        = 0x00001000,
    UNIT_FLAG3_UNK14                        = 0x00002000,
    UNIT_FLAG3_UNK15                        = 0x00004000,
    UNIT_FLAG3_UNK16                        = 0x00008000,
    UNIT_FLAG3_UNK17                        = 0x00010000,
    UNIT_FLAG3_UNK18                        = 0x00020000,
    UNIT_FLAG3_UNK19                        = 0x00040000,
    UNIT_FLAG3_UNK20                        = 0x00080000,
    UNIT_FLAG3_UNK21                        = 0x00100000,
    UNIT_FLAG3_UNK22                        = 0x00200000,
    UNIT_FLAG3_UNK23                        = 0x00400000,
    UNIT_FLAG3_UNK24                        = 0x00800000,
    UNIT_FLAG3_UNK25                        = 0x01000000,
    UNIT_FLAG3_UNK26                        = 0x02000000,
    UNIT_FLAG3_UNK27                        = 0x04000000,
    UNIT_FLAG3_UNK28                        = 0x08000000,
    UNIT_FLAG3_UNK29                        = 0x10000000,
    UNIT_FLAG3_UNK30                        = 0x20000000,
    UNIT_FLAG3_UNK31                        = 0x40000000,
    UNIT_FLAG3_UNK32                        = 0x80000000
};

/// Non Player Character flags
enum NPCFlags
{
    UNIT_NPC_FLAG_NONE                  = 0x00000000,
    UNIT_NPC_FLAG_GOSSIP                = 0x00000001,       // 100%
    UNIT_NPC_FLAG_QUESTGIVER            = 0x00000002,       // 100%
    UNIT_NPC_FLAG_UNK1                  = 0x00000004,
    UNIT_NPC_FLAG_UNK2                  = 0x00000008,
    UNIT_NPC_FLAG_TRAINER               = 0x00000010,       // 100%
    UNIT_NPC_FLAG_TRAINER_CLASS         = 0x00000020,       // 100%
    UNIT_NPC_FLAG_TRAINER_PROFESSION    = 0x00000040,       // 100%
    UNIT_NPC_FLAG_VENDOR                = 0x00000080,       // 100%
    UNIT_NPC_FLAG_VENDOR_AMMO           = 0x00000100,       // 100%, general goods vendor
    UNIT_NPC_FLAG_VENDOR_FOOD           = 0x00000200,       // 100%
    UNIT_NPC_FLAG_VENDOR_POISON         = 0x00000400,       // guessed
    UNIT_NPC_FLAG_VENDOR_REAGENT        = 0x00000800,       // 100%
    UNIT_NPC_FLAG_REPAIR                = 0x00001000,       // 100%
    UNIT_NPC_FLAG_FLIGHTMASTER          = 0x00002000,       // 100%
    UNIT_NPC_FLAG_SPIRITHEALER          = 0x00004000,       // guessed
    UNIT_NPC_FLAG_SPIRITGUIDE           = 0x00008000,       // guessed
    UNIT_NPC_FLAG_INNKEEPER             = 0x00010000,       // 100%
    UNIT_NPC_FLAG_BANKER                = 0x00020000,       // 100%
    UNIT_NPC_FLAG_PETITIONER            = 0x00040000,       // 100% 0xC0000 = guild petitions, 0x40000 = arena team petitions
    UNIT_NPC_FLAG_TABARDDESIGNER        = 0x00080000,       // 100%
    UNIT_NPC_FLAG_BATTLEMASTER          = 0x00100000,       // 100%
    UNIT_NPC_FLAG_AUCTIONEER            = 0x00200000,       // 100%
    UNIT_NPC_FLAG_STABLEMASTER          = 0x00400000,       // 100%
    UNIT_NPC_FLAG_GUILD_BANKER          = 0x00800000,       // cause client to send 997 opcode
    UNIT_NPC_FLAG_SPELLCLICK            = 0x01000000,       // cause client to send 1015 opcode (spell click)
    UNIT_NPC_FLAG_PLAYER_VEHICLE        = 0x02000000,       // players with mounts that have vehicle data should have it set
    UNIT_NPC_FLAG_MAILBOX               = 0x04000000,       // acces to the mailbox (NYI)
    UNIT_NPC_FLAG_REFORGER              = 0x08000000,       // reforging, deprecated
    UNIT_NPC_FLAG_TRANSMOGRIFIER        = 0x10000000,       // transmogrification
    UNIT_NPC_FLAG_VAULTKEEPER           = 0x20000000,       // void storage
    UNIT_NPC_FLAG_PETBATTLE             = 0x40000000,       // pet battle
    UNIT_NPC_FLAG_BLACK_MARKET          = 0x80000000        // black market auction house
};

/// Non Player Character flags
enum NPCFlags2
{
    UNIT_NPC_FLAG2_NONE                         = 0x00000000,
    UNIT_NPC_FLAG2_ITEM_UPGRADE                 = 0x00000001,       /// Item Upgrade
    UNIT_NPC_FLAG2_GARRISON_ARCHITECT           = 0x00000002,       /// Garrison Architect
    UNIT_NPC_FLAG2_AI_OBSTACLE                  = 0x00000004,       /// AI Obstacle manager
    UNIT_NPC_FLAG2_STEERING                     = 0x00000008,       /// ?
    UNIT_NPC_FLAG2_SHIPYARD_MISSION_NPC         = 0x00000020,       /// Garrison Shipyard Mission NPC -- TODO WTF ? UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER == UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER ??????
    UNIT_NPC_FLAG2_GARRISON_SHIPMENT_CRAFTER    = 0x00000020,       /// Garrison Shipment Crafter
    UNIT_NPC_FLAG2_GARRISON_MISSION_NPC         = 0x00000040,       /// Garrison Mission NPC
    UNIT_NPC_FLAG2_TRADESKILL_NPC               = 0x00000080        /// Garrison tradeskill NPC
};

enum MovementFlags
{
    MOVEMENTFLAG_NONE                  = 0x00000000,
    MOVEMENTFLAG_FORWARD               = 0x00000001,
    MOVEMENTFLAG_BACKWARD              = 0x00000002,
    MOVEMENTFLAG_STRAFE_LEFT           = 0x00000004,
    MOVEMENTFLAG_STRAFE_RIGHT          = 0x00000008,
    MOVEMENTFLAG_LEFT                  = 0x00000010,
    MOVEMENTFLAG_RIGHT                 = 0x00000020,
    MOVEMENTFLAG_PITCH_UP              = 0x00000040,
    MOVEMENTFLAG_PITCH_DOWN            = 0x00000080,
    MOVEMENTFLAG_WALKING               = 0x00000100,               // Walking
    MOVEMENTFLAG_DISABLE_GRAVITY       = 0x00000200,               // Former MOVEMENTFLAG_LEVITATING. This is used when walking is not possible.
    MOVEMENTFLAG_ROOT                  = 0x00000400,               // Must not be set along with MOVEMENTFLAG_MASK_MOVING
    MOVEMENTFLAG_FALLING               = 0x00000800,               // damage dealt on that type of falling
    MOVEMENTFLAG_FALLING_FAR           = 0x00001000,
    MOVEMENTFLAG_PENDING_STOP          = 0x00002000,
    MOVEMENTFLAG_PENDING_STRAFE_STOP   = 0x00004000,
    MOVEMENTFLAG_PENDING_FORWARD       = 0x00008000,
    MOVEMENTFLAG_PENDING_BACKWARD      = 0x00010000,
    MOVEMENTFLAG_PENDING_STRAFE_LEFT   = 0x00020000,
    MOVEMENTFLAG_PENDING_STRAFE_RIGHT  = 0x00040000,
    MOVEMENTFLAG_PENDING_ROOT          = 0x00080000,
    MOVEMENTFLAG_SWIMMING              = 0x00100000,               // appears with fly flag also
    MOVEMENTFLAG_ASCENDING             = 0x00200000,               // press "space" when flying
    MOVEMENTFLAG_DESCENDING            = 0x00400000,
    MOVEMENTFLAG_CAN_FLY               = 0x00800000,               // Appears when unit can fly AND also walk
    MOVEMENTFLAG_FLYING                = 0x01000000,               // unit is actually flying. pretty sure this is only used for players. creatures use disable_gravity
    MOVEMENTFLAG_SPLINE_ELEVATION      = 0x02000000,               // used for flight paths
    MOVEMENTFLAG_WATERWALKING          = 0x04000000,               // prevent unit from falling through water
    MOVEMENTFLAG_FALLING_SLOW          = 0x08000000,               // active rogue safe fall spell (passive)
    MOVEMENTFLAG_HOVER                 = 0x10000000,               // hover, cannot jump
    MOVEMENTFLAG_DISABLE_COLLISION     = 0x20000000,

    // TODO: Check if PITCH_UP and PITCH_DOWN really belong here..
    MOVEMENTFLAG_MASK_MOVING =
        MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_BACKWARD | MOVEMENTFLAG_STRAFE_LEFT | MOVEMENTFLAG_STRAFE_RIGHT |
        MOVEMENTFLAG_PITCH_UP | MOVEMENTFLAG_PITCH_DOWN | MOVEMENTFLAG_FALLING | MOVEMENTFLAG_FALLING_FAR | MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING |
        MOVEMENTFLAG_SPLINE_ELEVATION,

    MOVEMENTFLAG_MASK_TURNING =
        MOVEMENTFLAG_LEFT | MOVEMENTFLAG_RIGHT,

    MOVEMENTFLAG_MASK_MOVING_FLY =
        MOVEMENTFLAG_FLYING | MOVEMENTFLAG_ASCENDING | MOVEMENTFLAG_DESCENDING,

    // Movement flags allowed for creature in CreateObject - we need to keep all other enabled serverside
    // to properly calculate all movement
    MOVEMENTFLAG_MASK_CREATURE_ALLOWED =
        MOVEMENTFLAG_FORWARD | MOVEMENTFLAG_DISABLE_GRAVITY | MOVEMENTFLAG_ROOT | MOVEMENTFLAG_SWIMMING |
        MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_WATERWALKING | MOVEMENTFLAG_FALLING_SLOW | MOVEMENTFLAG_HOVER,

    //! TODO if needed: add more flags to this masks that are exclusive to players
    MOVEMENTFLAG_MASK_PLAYER_ONLY =
        MOVEMENTFLAG_FLYING,

    /// Movement flags that have change status opcodes associated for players
    MOVEMENTFLAG_MASK_HAS_PLAYER_STATUS_OPCODE = MOVEMENTFLAG_DISABLE_GRAVITY | MOVEMENTFLAG_ROOT |
                                                 MOVEMENTFLAG_CAN_FLY | MOVEMENTFLAG_WATERWALKING | MOVEMENTFLAG_FALLING_SLOW | MOVEMENTFLAG_HOVER | MOVEMENTFLAG_DISABLE_COLLISION

};
enum MovementFlags2
{
    MOVEMENTFLAG2_NONE                          = 0x00000000,
    MOVEMENTFLAG2_NO_STRAFE                     = 0x00000001,
    MOVEMENTFLAG2_NO_JUMPING                    = 0x00000002,
    MOVEMENTFLAG2_FULL_SPEED_TURNING            = 0x00000004,
    MOVEMENTFLAG2_FULL_SPEED_PITCHING           = 0x00000008,
    MOVEMENTFLAG2_ALWAYS_ALLOW_PITCHING         = 0x00000010,
    MOVEMENTFLAG2_UNK7                          = 0x00000020,
    MOVEMENTFLAG2_DISMISS_CONTROLLED_VEHICLE    = 0x00000040,
    MOVEMENTFLAG2_UNK9                          = 0x00000080,
    MOVEMENTFLAG2_UNK10                         = 0x00000100,
    MOVEMENTFLAG2_INTERPOLATED_MOVEMENT         = 0x00000200,
    MOVEMENTFLAG2_INTERPOLATED_TURNING          = 0x00000400,
    MOVEMENTFLAG2_INTERPOLATED_PITCHING         = 0x00000800,
};

enum UnitTypeMask
{
    UNIT_MASK_NONE                  = 0x00000000,
    UNIT_MASK_SUMMON                = 0x00000001,
    UNIT_MASK_MINION                = 0x00000002,
    UNIT_MASK_GUARDIAN              = 0x00000004,
    UNIT_MASK_TOTEM                 = 0x00000008,
    UNIT_MASK_PET                   = 0x00000010,
    UNIT_MASK_VEHICLE               = 0x00000020,
    UNIT_MASK_PUPPET                = 0x00000040,
    UNIT_MASK_HUNTER_PET            = 0x00000080,
    UNIT_MASK_CONTROLABLE_GUARDIAN  = 0x00000100,
    UNIT_MASK_ACCESSORY             = 0x00000200,
};

namespace Movement{
    class MoveSpline;
}

enum DiminishingLevels
{
    DIMINISHING_LEVEL_1             = 0,
    DIMINISHING_LEVEL_2             = 1,
    DIMINISHING_LEVEL_3             = 2,
    DIMINISHING_LEVEL_IMMUNE        = 3,
    DIMINISHING_LEVEL_4             = 3,
    DIMINISHING_LEVEL_TAUNT_IMMUNE  = 4,
};

struct DiminishingReturn
{
    DiminishingReturn(DiminishingGroup group, uint32 t, uint32 count)
        : DRGroup(group), stack(0), hitTime(t), hitCount(count)
    {}

    DiminishingGroup        DRGroup:16;
    uint16                  stack:16;
    uint32                  hitTime;
    uint32                  hitCount;
};

enum MeleeHitOutcome
{
    MELEE_HIT_EVADE, MELEE_HIT_MISS, MELEE_HIT_DODGE, MELEE_HIT_BLOCK, MELEE_HIT_PARRY,
    MELEE_HIT_GLANCING, MELEE_HIT_CRIT, MELEE_HIT_CRUSHING, MELEE_HIT_NORMAL
};

struct HealDone
{
    HealDone(uint32 heal, uint32 time)
    : s_heal(heal), s_timestamp(time) {}

    uint32 s_heal;
    uint32 s_timestamp;
};

struct HealTaken
{
    HealTaken(uint32 heal, uint32 time)
    : s_heal(heal), s_timestamp(time) {}

    uint32 s_heal;
    uint32 s_timestamp;
};

struct DamageDone
{
    DamageDone(uint32 dmg, uint32 time, uint32 spellId)
    : s_damage(dmg), s_timestamp(time), s_spellId(spellId) {}

    uint32 s_damage;
    uint32 s_timestamp;
    uint32 s_spellId;
};

struct DamageTaken
{
    DamageTaken(uint32 dmg, uint32 time)
    : s_damage(dmg), s_timestamp(time) {}

    uint32 s_damage;
    uint32 s_timestamp;
};

class DispelInfo
{
public:
    explicit DispelInfo(Unit* dispeller, uint32 dispellerSpellId, uint8 chargesRemoved) :
    _dispellerUnit(dispeller), _dispellerSpell(dispellerSpellId), _chargesRemoved(chargesRemoved) {}

    Unit* GetDispeller() const { return _dispellerUnit; }
    uint32 GetDispellerSpellId() const { return _dispellerSpell; }
    uint8 GetRemovedCharges() const { return _chargesRemoved; }
    void SetRemovedCharges(uint8 amount)
    {
        _chargesRemoved = amount;
    }
private:
    Unit* _dispellerUnit;
    uint32 _dispellerSpell;
    uint8 _chargesRemoved;
};

struct CleanDamage
{
    CleanDamage(uint32 mitigated, uint32 absorbed, WeaponAttackType _attackType, MeleeHitOutcome _hitOutCome) :
    absorbed_damage(absorbed), mitigated_damage(mitigated), attackType(_attackType), hitOutCome(_hitOutCome) {}

    uint32 absorbed_damage;
    uint32 mitigated_damage;

    WeaponAttackType attackType;
    MeleeHitOutcome hitOutCome;
};

struct CalcDamageInfo;

class DamageInfo
{
private:
    Unit* const m_attacker;
    Unit* const m_victim;
    uint32 m_damage;
    SpellInfo const* const m_spellInfo;
    SpellSchoolMask const m_schoolMask;
    DamageEffectType const m_damageType;
    WeaponAttackType m_attackType;
    uint32 m_absorb;
    uint32 m_resist;
    uint32 m_block;
public:
    explicit DamageInfo(Unit* _attacker, Unit* _victim, uint32 _damage, SpellInfo const* _spellInfo, SpellSchoolMask _schoolMask, DamageEffectType _damageType);
    explicit DamageInfo(CalcDamageInfo& dmgInfo);

    void ModifyDamage(int32 amount);
    void ModifyAbsorb(int32 amount);
    void AbsorbDamage(uint32 amount);
    void ResistDamage(uint32 amount);
    void BlockDamage(uint32 amount);

    Unit* GetAttacker() const { return m_attacker; };
    Unit* GetVictim() const { return m_victim; };

    SpellInfo const* GetSpellInfo() const { return m_spellInfo; };
    SpellSchoolMask GetSchoolMask() const { return m_schoolMask; };
    DamageEffectType GetDamageType() const { return m_damageType; };
    WeaponAttackType GetAttackType() const { return m_attackType; };
    uint32 GetDamage() const { return m_damage; };
    uint32 GetAbsorb() const { return m_absorb; };
    uint32 GetResist() const { return m_resist; };
    uint32 GetBlock() const { return m_block; };
};

class HealInfo
{
private:
    Unit* const m_healer; ///< unused
    Unit* const m_target; ///< unused
    uint32 m_heal;
    uint32 m_absorb;
    SpellInfo const* const m_spellInfo; ///< unused
    SpellSchoolMask const m_schoolMask; ///< unused
public:
    explicit HealInfo(Unit* _healer, Unit* _target, uint32 _heal, SpellInfo const* _spellInfo, SpellSchoolMask _schoolMask)
        : m_healer(_healer), m_target(_target), m_heal(_heal), m_spellInfo(_spellInfo), m_schoolMask(_schoolMask)
    {
        m_absorb = 0;
    }
    void AbsorbHeal(uint32 amount)
    {
        amount = std::min(amount, GetHeal());
        m_absorb += amount;
        m_heal -= amount;
    }

    SpellInfo const* GetSpellInfo() const { return m_spellInfo; };
    Unit* GetTarget() const { return m_target; };
    Unit* GetHealer() const { return m_healer; };
    SpellSchoolMask GetSchoolMask() const { return m_schoolMask; };
    uint32 GetHeal() const { return m_heal; };
};

class ProcEventInfo
{
private:
    Unit* const _actor;
    Unit* const _actionTarget;
    Unit* const _procTarget;
    uint32 _typeMask;
    uint32 _spellTypeMask;
    uint32 _spellPhaseMask;
    uint32 _hitMask;
    Spell* _spell;
    DamageInfo* _damageInfo;
    HealInfo* _healInfo;
public:
    explicit ProcEventInfo(Unit* actor, Unit* actionTarget, Unit* procTarget, uint32 typeMask, uint32 spellTypeMask, uint32 spellPhaseMask, uint32 hitMask, Spell* spell, DamageInfo* damageInfo, HealInfo* healInfo);
    Unit* GetActor() { return _actor; };
    Unit* GetActionTarget() const { return _actionTarget; }
    Unit* GetProcTarget() const { return _procTarget; }
    uint32 GetTypeMask() const { return _typeMask; }
    uint32 GetSpellTypeMask() const { return _spellTypeMask; }
    uint32 GetSpellPhaseMask() const { return _spellPhaseMask; }
    uint32 GetHitMask() const { return _hitMask; }
    SpellInfo const* GetSpellInfo() const { return NULL; }
    SpellSchoolMask GetSchoolMask() const { return SPELL_SCHOOL_MASK_NONE; }
    DamageInfo* GetDamageInfo() const { return _damageInfo; }
    HealInfo* GetHealInfo() const { return _healInfo; }
    Spell* GetSpell() const { return _spell; }
};

// Struct for use in Unit::CalculateMeleeDamage
// Need create structure like in SMSG_ATTACKER_STATE_UPDATE opcode
struct CalcDamageInfo
{
    Unit  *attacker;             // Attacker
    Unit  *target;               // Target for damage
    uint32 damageSchoolMask;
    uint32 damage;
    uint32 absorb;
    uint32 resist;
    uint32 blocked_amount;
    uint32 HitInfo;
    uint32 TargetState;
// Helper
    WeaponAttackType attackType; //
    uint32 procAttacker;
    uint32 procVictim;
    uint32 procEx;
    uint32 cleanDamage;          // Used only for rage calculation
    MeleeHitOutcome hitOutCome;  // TODO: remove this field (need use TargetState)
};

// Spell damage info structure based on structure sending in SMSG_SPELL_NON_MELEE_DAMAGE_LOG opcode
struct SpellNonMeleeDamage{
    SpellNonMeleeDamage(Unit* _attacker, Unit* _target, uint32 _SpellID, uint32 _schoolMask)
        : target(_target), attacker(_attacker), SpellID(_SpellID), damage(0), overkill(0), schoolMask(_schoolMask),
        absorb(0), resist(0), physicalLog(false), unused(false), blocked(0), HitInfo(0), cleanDamage(0)
    {}

    Unit   *target;
    Unit   *attacker;
    uint32 SpellID;
    uint32 damage;
    uint32 overkill;
    uint32 schoolMask;
    uint32 absorb;
    uint32 resist;
    bool   physicalLog;
    bool   unused;
    uint32 blocked;
    uint32 HitInfo;
    // Used for help
    uint32 cleanDamage;
};

struct SpellPeriodicAuraLogInfo
{
    SpellPeriodicAuraLogInfo(AuraEffect const* _auraEff, uint32 _damage, uint32 _overDamage, uint32 _absorb, uint32 _resist, float _multiplier, bool _critical, bool _multistrike = false)
        : auraEff(_auraEff), damage(_damage), overDamage(_overDamage), absorb(_absorb), resist(_resist), multiplier(_multiplier), critical(_critical), multistrike(_multistrike){}

    AuraEffect const* auraEff;
    uint32 damage;
    uint32 overDamage;                                      // overkill/overheal
    uint32 absorb;
    uint32 resist;
    float  multiplier;
    bool   critical;
    bool   multistrike;
};

uint32 createProcExtendMask(SpellNonMeleeDamage* damageInfo, SpellMissInfo missCondition);

#define MAX_DECLINED_NAME_CASES 5

struct DeclinedName
{
    std::string name[MAX_DECLINED_NAME_CASES];
};

enum CurrentSpellTypes
{
    CURRENT_MELEE_SPELL             = 0,
    CURRENT_GENERIC_SPELL           = 1,
    CURRENT_CHANNELED_SPELL         = 2,
    CURRENT_AUTOREPEAT_SPELL        = 3
};

#define CURRENT_FIRST_NON_MELEE_SPELL 1
#define CURRENT_MAX_SPELL             4

struct GlobalCooldown
{
    explicit GlobalCooldown(uint32 _dur = 0, uint32 _time = 0) : duration(_dur), cast_time(_time) {}

    uint32 duration;
    uint32 cast_time;
};

typedef std::unordered_map<uint32 /*category*/, GlobalCooldown> GlobalCooldownList;

class GlobalCooldownMgr                                     // Shared by Player and CharmInfo
{
public:
    GlobalCooldownMgr() {}

public:
    bool HasGlobalCooldown(SpellInfo const* spellInfo) const;
    void AddGlobalCooldown(SpellInfo const* spellInfo, uint32 gcd);
    void CancelGlobalCooldown(SpellInfo const* spellInfo);

private:
    GlobalCooldownList m_GlobalCooldowns;
};

enum ActiveStates
{
    ACT_PASSIVE  = 0x01,                                    // 0x01 - passive
    ACT_DISABLED = 0x81,                                    // 0x80 - castable
    ACT_ENABLED  = 0xC1,                                    // 0x40 | 0x80 - auto cast + castable
    ACT_COMMAND  = 0x07,                                    // 0x01 | 0x02 | 0x04
    ACT_REACTION = 0x06,                                    // 0x02 | 0x04
    ACT_DECIDE   = 0x00                                     // custom
};

enum ReactStates
{
    REACT_PASSIVE    = 0,
    REACT_DEFENSIVE  = 1,
    REACT_AGGRESSIVE = 2,
    REACT_HELPER     = 3
};

enum CommandStates
{
    COMMAND_STAY    = 0,
    COMMAND_FOLLOW  = 1,
    COMMAND_ATTACK  = 2,
    COMMAND_ABANDON = 3,
    COMMAND_MOVE_TO = 4
};

#define UNIT_ACTION_BUTTON_ACTION(X) (uint32(X) & 0x00FFFFFF)
#define UNIT_ACTION_BUTTON_TYPE(X)   ((uint32(X) & 0xFF000000) >> 24)
#define MAKE_UNIT_ACTION_BUTTON(A, T) (uint32(A) | (uint32(T) << 24))

struct UnitActionBarEntry
{
    UnitActionBarEntry() : packedData(uint32(ACT_DISABLED) << 24) {}

    uint32 packedData;

    // helper
    ActiveStates GetType() const { return ActiveStates(UNIT_ACTION_BUTTON_TYPE(packedData)); }
    uint32 GetAction() const { return UNIT_ACTION_BUTTON_ACTION(packedData); }
    bool IsActionBarForSpell() const
    {
        ActiveStates Type = GetType();
        return Type == ACT_DISABLED || Type == ACT_ENABLED || Type == ACT_PASSIVE;
    }

    void SetActionAndType(uint32 action, ActiveStates type)
    {
        packedData = MAKE_UNIT_ACTION_BUTTON(action, type);
    }

    void SetType(ActiveStates type)
    {
        packedData = MAKE_UNIT_ACTION_BUTTON(UNIT_ACTION_BUTTON_ACTION(packedData), type);
    }

    void SetAction(uint32 action)
    {
        packedData = (packedData & 0xFF000000) | UNIT_ACTION_BUTTON_ACTION(action);
    }
};

typedef std::list<Player*> SharedVisionList;

enum CharmType
{
    CHARM_TYPE_CHARM,
    CHARM_TYPE_POSSESS,
    CHARM_TYPE_VEHICLE,
    CHARM_TYPE_CONVERT,
};

typedef UnitActionBarEntry CharmSpellInfo;

enum ActionBarIndex
{
    ACTION_BAR_INDEX_START = 0,
    ACTION_BAR_INDEX_PET_SPELL_START = 3,
    ACTION_BAR_INDEX_PET_SPELL_END = 7,
    ACTION_BAR_INDEX_END = 10,
};

#define MAX_UNIT_ACTION_BAR_INDEX (ACTION_BAR_INDEX_END-ACTION_BAR_INDEX_START)

class CharmInfo
{
    public:
        explicit CharmInfo(Unit* unit);
        ~CharmInfo();
        void RestoreState();
        uint32 GetPetNumber() const { return m_petnumber; }
        uint32 GetRealmPetNumber() const 
        { 
#ifdef CROSS
            return m_RealmPetNumber;
#else
            return m_petnumber;
#endif
        }
        void SetPetNumber(uint32 petnumber, bool statwindow);

        void SetCommandState(CommandStates st) { m_CommandState = st; }
        CommandStates GetCommandState() const { return m_CommandState; }
        bool HasCommandState(CommandStates state) const { return (m_CommandState == state); }

        void InitPossessCreateSpells();
        void InitCharmCreateSpells();
        void InitPetActionBar();
        void InitEmptyActionBar(bool withAttack = true);

                                                            //return true if successful
        bool AddSpellToActionBar(SpellInfo const* spellInfo, ActiveStates newstate = ACT_DECIDE);
        bool RemoveSpellFromActionBar(uint32 spell_id);
        void LoadPetActionBar(const std::string& data);
        void BuildActionBar(WorldPacket* data);
        void SetSpellAutocast(SpellInfo const* spellInfo, bool state);
        void SetActionBar(uint8 index, uint32 spellOrAction, ActiveStates type)
        {
            PetActionBar[index].SetActionAndType(spellOrAction, type);
        }
        UnitActionBarEntry const* GetActionBarEntry(uint8 index) const { return &(PetActionBar[index]); }

        void ToggleCreatureAutocast(SpellInfo const* spellInfo, bool apply);

        CharmSpellInfo* GetCharmSpell(uint8 index) { return &(m_charmspells[index]); }

        GlobalCooldownMgr& GetGlobalCooldownMgr() { return m_GlobalCooldownMgr; }

        void SetIsCommandAttack(bool val);
        bool IsCommandAttack();
        void SetIsCommandFollow(bool val);
        bool IsCommandFollow();
        void SetIsAtStay(bool val);
        bool IsAtStay();
        void SetIsFollowing(bool val);
        bool IsFollowing();
        void SetIsReturning(bool val);
        bool IsReturning();
        void SaveStayPosition();
        void GetStayPosition(float &x, float &y, float &z);

        CharmType GetCharmType() const { return m_CharmType; }

    private:

        Unit* m_unit;
        UnitActionBarEntry PetActionBar[MAX_UNIT_ACTION_BAR_INDEX];
        CharmSpellInfo     m_charmspells[4];
        CommandStates      m_CommandState;
        uint32             m_petnumber;
#ifdef CROSS
        uint32             m_RealmPetNumber;
#endif /* CROSS */
        bool               m_barInit;
        CharmType          m_CharmType;

        //for restoration after charmed
        ReactStates     m_oldReactState;

        bool m_isCommandAttack;
        bool _isCommandFollow;
        bool m_isAtStay;
        bool m_isFollowing;
        bool m_isReturning;
        float m_stayX;
        float m_stayY;
        float m_stayZ;

        GlobalCooldownMgr m_GlobalCooldownMgr;
};

// for clearing special attacks
#define REACTIVE_TIMER_START 4000

enum ReactiveType
{
    REACTIVE_DEFENSE      = 0,
    REACTIVE_HUNTER_PARRY = 1,
    REACTIVE_OVERPOWER    = 2
};

#define MAX_REACTIVE 3
#define SUMMON_SLOT_PET     0
#define SUMMON_SLOT_TOTEM   1
#define MAX_TOTEM_SLOT      5
#define SUMMON_SLOT_MINIPET 5
#define SUMMON_SLOT_QUEST   6
#define MAX_SUMMON_SLOT     9

#define MAX_GAMEOBJECT_SLOT 4

enum PlayerTotemType
{
    SUMMON_TYPE_TOTEM_FIRE   = 63,
    SUMMON_TYPE_TOTEM_FIRE2  = 3403,
    SUMMON_TYPE_TOTEM_FIRE3  = 3599,
    SUMMON_TYPE_TOTEM_FIRE4  = 3211,

    SUMMON_TYPE_TOTEM_EARTH  = 81,
    SUMMON_TYPE_TOTEM_EARTH2 = 3400,
    SUMMON_TYPE_TOTEM_EARTH3 = 3404,

    SUMMON_TYPE_TOTEM_WATER  = 82,
    SUMMON_TYPE_TOTEM_WATER2 = 3402,

    SUMMON_TYPE_TOTEM_AIR    = 83,
    SUMMON_TYPE_TOTEM_AIR2   = 3405,
    SUMMON_TYPE_TOTEM_AIR3   = 3407,
    SUMMON_TYPE_TOTEM_AIR4   = 3406,
    SUMMON_TYPE_TOTEM_AIR5   = 3399,
    SUMMON_TYPE_TOTEM_AIR6   = 3401
};

/// Spell cooldown flags sent in SMSG_SPELL_COOLDOWN
enum CooldownFlags
{
    CooldownFlagNone                  = 0x0,
    CooldownFlagIncludeGCD            = 0x1,  ///< Starts GCD in addition to normal cooldown specified in the packet
    CooldownFlagIncludeEventCooldowns = 0x2   ///< Starts GCD for spells that should start their cooldown on events, requires CooldownFlagIncludeGCD set
};

enum Stagger
{
    LIGHT_STAGGER       = 124275,
    MODERATE_STAGGER    = 124274,
    HEAVY_STAGGER       = 124273
};

// delay time next attack to prevent client attack animation problems
#define ATTACK_DISPLAY_DELAY 200
#define MAX_PLAYER_STEALTH_DETECT_RANGE 30.0f               // max distance for detection targets by player

struct SpellProcEventEntry;                                 // used only privately

#define STATS_CHANCE_SIZE 4
float const g_BaseMissChancePhysical[STATS_CHANCE_SIZE] =
{
    -4.5f,
    -3.0f,
    -1.5f,
    0.0f
};

float const g_BaseMissChanceSpell[STATS_CHANCE_SIZE] =
{
    -9.0f,
    -6.0f,
    -3.0f,
    0.0f
};

float const g_BaseEnemyParryChance[STATS_CHANCE_SIZE] =
{
    -1.5f,
    0.0f,
    1.5f,
    3.0f
};

float const g_BaseEnemyDodgeChance[STATS_CHANCE_SIZE] =
{
    -4.5f,
    -3.0f,
    -1.5f,
    0.0f
};

enum LossOfControlType
{
    TypeNone            = 0,
    TypePossess         = 1,
    TypeConfuse         = 2,
    TypeCharm           = 3,
    TypeFear            = 4,
    TypeStun            = 5,
    TypePacify          = 6,
    TypeRoot            = 7,
    TypeSilence         = 8,
    TypePacifySilence   = 9,
    TypeDisarm          = 10,
    TypeSchoolInterrupt = 11,
    TypeStunMechanic    = 12,
    TypeFearMechanic    = 13,
    TypeSilenceHarmful  = 14
};

class Unit : public WorldObject
{
    public:
        typedef std::set<Unit*> AttackerSet;
        typedef std::set<Unit*> ControlList;
        typedef std::pair<uint32, uint8> spellEffectPair;
        typedef std::multimap<uint32,  Aura*> AuraMap;
        typedef std::multimap<uint32,  AuraApplication*> AuraApplicationMap;
        typedef std::multimap<uint32,  AuraApplication*> AuraStateAurasMap;
        typedef std::list<AuraEffect*> AuraEffectList;
        typedef std::list<Aura*> AuraList;
        typedef std::list<AuraApplication *> AuraApplicationList;
        typedef std::map<uint32, StackOnDuration> AuraStackOnDurationMap;
        typedef std::list<DiminishingReturn> Diminishing;
        typedef std::set<uint32> ComboPointHolderSet;
        typedef std::vector<uint32> AuraIdList;
        typedef std::map<uint8, AuraApplication*> VisibleAuraMap;
        typedef std::set<Powers> PowerTypeSet;

        virtual ~Unit();

        UnitAI* GetAI() { return i_AI; }
        void SetAI(UnitAI* newAI) { i_AI = newAI; }

        void AddToWorld();
        void RemoveFromWorld();

        void CleanupBeforeRemoveFromMap(bool finalCleanup);
        void CleanupsBeforeDelete(bool finalCleanup = true);                        // used in ~Creature/~Player (or before mass creature delete to remove cross-references to already deleted units)

        DiminishingLevels GetDiminishing(DiminishingGroup  group);
        void IncrDiminishing(DiminishingGroup group);
        float ApplyDiminishingToDuration(DiminishingGroup  group, int32 &duration, Unit* caster, DiminishingLevels Level, int32 limitduration);
        void ApplyDiminishingAura(DiminishingGroup  group, bool apply);
        void ClearDiminishings() { m_Diminishing.clear(); }

        // target dependent range checks
        float GetSpellMaxRangeForTarget(Unit const* target, SpellInfo const* spellInfo) const;
        float GetSpellMinRangeForTarget(Unit const* target, SpellInfo const* spellInfo) const;

        virtual void Update(uint32 time);

        void setAttackTimer(WeaponAttackType type, uint32 time) { m_attackTimer[type] = time; }
        void resetAttackTimer(WeaponAttackType type = WeaponAttackType::BaseAttack);
        uint32 getAttackTimer(WeaponAttackType type) const { return m_attackTimer[type]; }
        bool isAttackReady(WeaponAttackType type = WeaponAttackType::BaseAttack) const { return m_attackTimer[type] <= 0; }
        bool haveOffhandWeapon() const;
        bool CanDualWield() const { return m_canDualWield; }
        void SetCanDualWield(bool value) { m_canDualWield = value; }
        float GetCombatReach() const { return m_floatValues[UNIT_FIELD_COMBAT_REACH]; }
        float GetBoundaryRadius() const { return m_floatValues[UNIT_FIELD_BOUNDING_RADIUS]; }
        float GetMeleeReach() const { float reach = m_floatValues[UNIT_FIELD_COMBAT_REACH]; return reach > MIN_MELEE_REACH ? reach : MIN_MELEE_REACH; }
        bool IsWithinCombatRange(const Unit* obj, float dist2compare) const;
        bool IsWithinMeleeRange(const Unit* obj, float dist = MELEE_RANGE) const;
        bool IsWithinBoundaryRadius(Unit const* p_Unit) const;
        void GetRandomContactPoint(const Unit* target, float &x, float &y, float &z, float distance2dMin, float distance2dMax) const;
        uint32 m_extraAttacks;
        bool m_canDualWield;
        int32 m_InsightCount;

        /// Used for Bandit's Guile
        void SetInsightCount(uint8 p_Value) { m_InsightCount = p_Value; }
        uint8 GetInsightCount() const { return m_InsightCount; }

        void _addAttacker(Unit* pAttacker)                  // must be called only from Unit::Attack(Unit*)
        {
            m_attackers.insert(pAttacker);
        }
        void _removeAttacker(Unit* pAttacker)               // must be called only from Unit::AttackStop()
        {
            m_attackers.erase(pAttacker);
        }
        Unit* getAttackerForHelper() const                 // If someone wants to help, who to give them
        {
            if (getVictim() != NULL)
                return getVictim();

            if (!m_attackers.empty())
                return *(m_attackers.begin());

            return NULL;
        }
        bool Attack(Unit* victim, bool meleeAttack);
        void CastStop(uint32 except_spellid = 0);
        bool AttackStop();
        void RemoveAllAttackers();
        AttackerSet const& getAttackers() const { return m_attackers; }
        bool isAttackingPlayer() const;
        Unit* getVictim() const { return m_attacking; }
        // Use this only when 100% sure there is a victim
        Unit* EnsureVictim() const
        {
            ASSERT(m_attacking);
            return m_attacking;
        }
        void CombatStop(bool includingCast = false);
        void CombatStopWithPets(bool includingCast = false);
        void StopAttackFaction(uint32 faction_id);
        void GetAttackableUnitListInRange(std::list<Unit*> &list, float fMaxSearchRange) const;
        void GetAreatriggerListInRange(std::list<AreaTrigger*>& p_List, float p_Range) const;
        void GetConversationListInRange(std::list<Conversation*>& p_List, float p_Range) const;
        void GetAreaTriggerListWithSpellIDInRange(std::list<AreaTrigger*>& p_List, uint32 p_SpellID, float p_Range) const;
        Unit* SelectNearbyTarget(Unit* exclude = NULL, float dist = NOMINAL_MELEE_RANGE, uint32 p_ExludeAuraID = 0, bool p_ExcludeVictim = true, bool p_Alive = true, bool p_ExcludeStealthVictim = false, bool p_CheckValidAttack = false) const;
        Unit* SelectNearbyAlly(Unit* exclude = NULL, float dist = NOMINAL_MELEE_RANGE, bool p_CheckValidAssist = false) const;
        Unit* SelectNearbyMostInjuredAlly(Unit* p_Exculde = nullptr, float p_Dist = NOMINAL_MELEE_RANGE, uint32 p_ExcludeEntry = 0) const;
        void SendMeleeAttackStop(Unit* victim = NULL);
        void SendMeleeAttackStart(Unit* victim);
        bool IsVisionObscured(Unit* victim, SpellInfo const* spellInfo);

        // Part of Evade mechanics
        time_t GetLastDamagedTime() const { return _lastDamagedTime; }
        void SetLastDamagedTime(time_t val) { _lastDamagedTime = val; }

        void AddUnitState(uint32 f) { m_state |= f; }
        bool HasUnitState(const uint32 f) const { return (m_state & f); }
        void ClearUnitState(uint32 f) { m_state &= ~f; }
        void ClearAllUnitState() { m_state = 0; }
        bool CanFreeMove() const
        {
            return !HasUnitState(UNIT_STATE_CONFUSED | UNIT_STATE_FLEEING | UNIT_STATE_IN_FLIGHT |
                UNIT_STATE_ROOT | UNIT_STATE_STUNNED | UNIT_STATE_DISTRACTED) && GetOwnerGUID() == 0;
        }

        uint32 HasUnitTypeMask(uint32 mask) const { return mask & m_unitTypeMask; }
        void AddUnitTypeMask(uint32 mask) { m_unitTypeMask |= mask; }
        bool isSummon() const   { return m_unitTypeMask & UNIT_MASK_SUMMON; }
        bool isGuardian() const { return m_unitTypeMask & UNIT_MASK_GUARDIAN; }
        bool isPet() const      { return m_unitTypeMask & UNIT_MASK_PET; }
        bool isHunterPet() const{ return m_unitTypeMask & UNIT_MASK_HUNTER_PET; }
        bool isTotem() const    { return m_unitTypeMask & UNIT_MASK_TOTEM; }
        bool isStatue() const   { return (GetEntry() == 60849 || GetEntry() == 61146); }
        bool IsVehicle() const  { return m_unitTypeMask & UNIT_MASK_VEHICLE; }

        bool IsPetGuardianStuff() const { return m_unitTypeMask & ( UNIT_MASK_SUMMON | UNIT_MASK_GUARDIAN | UNIT_MASK_PET | UNIT_MASK_HUNTER_PET | UNIT_MASK_TOTEM ); }

        uint8 getLevel() const { return uint8(GetUInt32Value(UNIT_FIELD_LEVEL)); }
        uint8 getLevelForTarget(WorldObject const* /*target*/) const { return getLevel(); }
        void SetLevel(uint8 lvl);
        uint8 getRace() const { return GetByteValue(UNIT_FIELD_SEX, 0); }
        uint32 getRaceMask() const { return 1 << (getRace()-1); }
        uint8 getClass() const { return GetByteValue(UNIT_FIELD_SEX, 1); }
        uint32 getClassMask() const { return 1 << (getClass()-1); }
        uint8 getGender() const { return GetByteValue(UNIT_FIELD_SEX, 3); }

        float GetStat(Stats stat) const { return float(GetUInt32Value(UNIT_FIELD_STATS+stat)); }
        void SetStat(Stats stat, int32 val) { SetStatInt32Value(UNIT_FIELD_STATS+stat, val); }
        uint32 GetArmor() const { return GetResistance(SPELL_SCHOOL_NORMAL); }
        void SetArmor(int32 val) { SetResistance(SPELL_SCHOOL_NORMAL, val); }

        uint32 GetResistance(SpellSchools school) const { return GetUInt32Value(UNIT_FIELD_RESISTANCES+school); }
        uint32 GetResistance(SpellSchoolMask mask) const;
        void SetResistance(SpellSchools school, int32 val) { SetStatInt32Value(UNIT_FIELD_RESISTANCES+school, val); }

        uint32 GetHealth()    const { return GetUInt32Value(UNIT_FIELD_HEALTH); }
        uint32 GetMaxHealth() const { return GetUInt32Value(UNIT_FIELD_MAX_HEALTH); }

        bool IsFullHealth() const { return GetHealth() == GetMaxHealth(); }
        bool HealthBelowPct(int32 pct) const { return GetHealth() < CountPctFromMaxHealth(pct); }
        bool HealthBelowPctDamaged(int32 pct, uint32 damage) const { return int64(GetHealth()) - int64(damage) < int64(CountPctFromMaxHealth(pct)); }
        bool HealthAbovePct(int32 pct) const { return GetHealth() > CountPctFromMaxHealth(pct); }
        bool HealthAbovePctHealed(int32 pct, uint32 heal) const { return uint64(GetHealth()) + uint64(heal) > CountPctFromMaxHealth(pct); }
        float GetHealthPct() const { return GetMaxHealth() ? 100.0f * GetHealth() / GetMaxHealth() : 0.0f; }
        uint32 CountPctFromMaxHealth(int32 pct) const { return CalculatePct(GetMaxHealth(), pct); }
        uint32 CountPctFromMaxHealth(float p_Percent) const { return uint32(CalculatePct((float)GetMaxHealth(), p_Percent)); }
        uint32 CountPctFromCurHealth(int32 pct) const { return CalculatePct(GetHealth(), pct); }
        uint32 CountPctFromMaxMana(int32 pct) const { return CalculatePct(GetMaxPower(POWER_MANA), pct); }
        uint32 CountPctFromCurMana(int32 pct) const { return CalculatePct(GetPower(POWER_MANA), pct); }
        uint32 CountPctFromMaxPower(int32 pct, Powers power) const { return CalculatePct(GetMaxPower(power), pct); }
        uint32 CountPctFromCurPower(int32 pct, Powers power) const { return CalculatePct(GetPower(power), pct); }

        void SetHealth(uint32 val);
        void SetMaxHealth(uint32 val);
        inline void SetFullHealth() { SetHealth(GetMaxHealth()); }
        int32 ModifyHealth(int32 val);
        int32 GetHealthGain(int32 dVal);

        Powers getPowerType() const { return Powers(GetUInt32Value(UNIT_FIELD_DISPLAY_POWER)); }
        void setPowerType(Powers power);
        int32 GetPower(Powers power) const;
        float GetPowerPct(Powers power) const { return GetMaxPower(power) ? 100.0f * GetPower(power) / GetMaxPower(power) : 0.0f; }
        int32 GetMinPower(Powers power) const { return power == POWER_ECLIPSE ? -10000 : 0; }
        int32 GetMaxPower(Powers power) const;
        int32 GetPowerCoeff(Powers p_powerType) const;
        void SetPower(Powers power, int32 val, bool regen = false);
        void SetMaxPower(Powers power, int32 val);

        // returns the change in power
        int32 ModifyPower(Powers power, int32 val);
        int32 ModifyPowerPct(Powers power, float pct, bool apply = true);

        void AddComboPoints(int8 p_Count);
        void ClearComboPoints();

        PowerTypeSet GetUsablePowers() const;
        uint32 GetPowerIndex(uint32 powerId, uint32 classId) const;

        uint32 GetAttackTime(WeaponAttackType att) const
        {
           float f_BaseAttackTime = GetFloatValue(UNIT_FIELD_ATTACK_ROUND_BASE_TIME+att) / m_modAttackSpeedPct[att];
           return (uint32)f_BaseAttackTime;
        }

        void SetAttackTime(WeaponAttackType att, uint32 val) { SetFloatValue(UNIT_FIELD_ATTACK_ROUND_BASE_TIME+att, val*m_modAttackSpeedPct[att]); }
        void ApplyAttackTimePercentMod(WeaponAttackType att, float val, bool apply);
        void ApplyCastTimePercentMod(float val, bool apply);

        SheathState GetSheath() const { return SheathState(GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 0)); }
        virtual void SetSheath(SheathState sheathed) { SetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 0, sheathed); }

        // faction template id
        uint32 getFaction() const { return GetUInt32Value(UNIT_FIELD_FACTION_TEMPLATE); }
        void setFaction(uint32 faction) { SetUInt32Value(UNIT_FIELD_FACTION_TEMPLATE, faction); }
        FactionTemplateEntry const* getFactionTemplateEntry() const;

        ReputationRank GetReactionTo(Unit const* target) const;
        ReputationRank static GetFactionReactionTo(FactionTemplateEntry const* factionTemplateEntry, Unit const* target);

        bool IsHostileTo(Unit const* unit) const;
        bool IsHostileToPlayers() const;
        bool IsFriendlyTo(Unit const* unit) const;
        bool IsNeutralToAll() const;
        bool IsInPartyWith(Unit const* unit) const;
        bool IsInRaidWith(Unit const* unit) const;
        void GetPartyMembers(std::list<Unit*> &p_Members);
        void GetRaidMembers(std::list<Unit*> &units);
        bool IsContestedGuard() const
        {
            if (FactionTemplateEntry const* entry = getFactionTemplateEntry())
                return entry->IsContestedGuardFaction();

            return false;
        }
        bool IsPvP() const { return HasByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_PVP); }
        void SetPvP(bool state)
        {
            if (state)
                SetByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_PVP);
            else
                RemoveByteFlag(UNIT_FIELD_SHAPESHIFT_FORM, 1, UNIT_BYTE2_FLAG_PVP);
        }
        uint32 GetCreatureType() const;
        uint32 GetCreatureTypeMask() const
        {
            uint32 creatureType = GetCreatureType();
            return (creatureType >= 1) ? (1 << (creatureType - 1)) : 0;
        }

        uint8 getStandState() const { return GetByteValue(UNIT_FIELD_ANIM_TIER, 0); }
        bool IsSitState() const;
        bool IsStandState() const;
        void SetStandState(uint8 state);

        void  SetStandFlags(uint8 flags) { SetByteFlag(UNIT_FIELD_ANIM_TIER, 2, flags); }
        void  RemoveStandFlags(uint8 flags) { RemoveByteFlag(UNIT_FIELD_ANIM_TIER, 2, flags); }

        bool IsMounted() const { return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNT); }
        uint32 GetMountID() const { return GetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID); }
        void Mount(uint32 mount, uint32 vehicleId = 0, uint32 creatureEntry = 0);
        void Dismount();
        void SendMountResult(MountResult error);
        MountCapabilityEntry const* GetMountCapability(uint32 mountType) const;

        void SendDurabilityLoss(Player* receiver, uint32 percent);
        void PlayOneShotAnimKit(uint32 id);
        void PlayOrphanSpellVisual(G3D::Vector3 p_Source, G3D::Vector3 p_Orientation, G3D::Vector3 p_Target, int32 p_Visual, float p_TravelSpeed = 1.0f, uint64 p_TargetGuid = 0, bool p_SpeedAsTime = true);
        void CancelOrphanSpellVisual(int32 p_SpellVisualID);

        uint16 GetMaxSkillValueForLevel(Unit const* target = NULL) const { return (target ? getLevelForTarget(target) : getLevel()) * 5; }
        void DealDamageMods(Unit* victim, uint32 &damage, uint32* absorb);
        uint32 DealDamage(Unit* victim, uint32 damage, CleanDamage const* cleanDamage = NULL, DamageEffectType damagetype = DIRECT_DAMAGE, SpellSchoolMask damageSchoolMask = SPELL_SCHOOL_MASK_NORMAL, SpellInfo const* spellProto = NULL, bool durabilityLoss = true);
        uint32 CalcStaggerDamage(Player* victim, uint32 damage, SpellSchoolMask damageSchoolMask, SpellInfo const* spellProto);
        void Kill(Unit* victim, bool durabilityLoss = true, SpellInfo const* spellProto = NULL);
        int32 DealHeal(Unit* victim, uint32 addhealth, SpellInfo const* spellProto = NULL);

        void ProcDamageAndSpell(Unit* victim, uint32 procAttacker, uint32 procVictim, uint32 procEx, uint32 amount, uint32 absorb = 0, WeaponAttackType attType = WeaponAttackType::BaseAttack, SpellInfo const* procSpell = NULL, SpellInfo const* procAura = NULL, AuraEffect const* ownerAuraEffect = NULL);
        void ProcDamageAndSpellFor(bool isVictim, Unit* target, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, SpellInfo const* procSpell, uint32 damage, uint32 absorb = 0, SpellInfo const* procAura = NULL, AuraEffect const* ownerAuraEffect = NULL);

        bool IsNoBreakingCC(bool isVictim, Unit* target, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, SpellInfo const* procSpell, uint32 damage, uint32 absorb, SpellInfo const* procAura, SpellInfo const* spellProto) const;

        void GetProcAurasTriggeredOnEvent(std::list<AuraApplication*>& aurasTriggeringProc, std::list<AuraApplication*>* procAuras, ProcEventInfo eventInfo);
        void TriggerAurasProcOnEvent(CalcDamageInfo& damageInfo);
        void TriggerAurasProcOnEvent(std::list<AuraApplication*>* myProcAuras, std::list<AuraApplication*>* targetProcAuras, Unit* actionTarget, uint32 typeMaskActor, uint32 typeMaskActionTarget, uint32 spellTypeMask, uint32 spellPhaseMask, uint32 hitMask, Spell* spell, DamageInfo* damageInfo, HealInfo* healInfo);
        void TriggerAurasProcOnEvent(ProcEventInfo& eventInfo, std::list<AuraApplication*>& procAuras);

        void HandleEmoteCommand(uint32 anim_id);
        void AttackerStateUpdate (Unit* victim, WeaponAttackType attType = WeaponAttackType::BaseAttack, bool extra = false);

        void CalculateMeleeDamage(Unit* victim, uint32 damage, CalcDamageInfo* damageInfo, WeaponAttackType attackType = WeaponAttackType::BaseAttack);
        void DealMeleeDamage(CalcDamageInfo* damageInfo, bool durabilityLoss);
        void HandleProcExtraAttackFor(Unit* victim);

        void CalculateSpellDamageTaken(SpellNonMeleeDamage* damageInfo, int32 damage, SpellInfo const* spellInfo, WeaponAttackType attackType = WeaponAttackType::BaseAttack, bool crit = false);
        void DealSpellDamage(SpellNonMeleeDamage* damageInfo, bool durabilityLoss);

        void ApplyResilience(const Unit* victim, int32 * damage) const;

        float CalculateDamageDealtFactor(Unit* p_Unit, Creature* p_Creature);
        float CalculateDamageTakenFactor(Unit* p_Unit, Creature* p_Creature);

        float GetDiminishingPVPDamage(SpellInfo const* p_Spellproto) const;

        float MeleeSpellMissChance(const Unit* p_Victim, SpellInfo const* p_Spell, WeaponAttackType p_AttType) const;
        float MagicSpellMissChance(const Unit* p_Victim, SpellInfo const* p_Spell) const;
        SpellMissInfo MeleeSpellHitResult(Unit* victim, SpellInfo const* spell);
        SpellMissInfo MagicSpellHitResult(Unit* p_Victim, SpellInfo const* p_Spell);
        SpellMissInfo SpellHitResult(Unit* victim, SpellInfo const* spell, bool canReflect = false);

        float GetUnitDodgeChance(Unit const* p_Attacker) const;
        float GetUnitParryChance(Unit const* p_Attacker) const;
        float GetUnitBlockChance(Unit const* p_Attacker) const;
        float GetUnitMissChancePhysical(Unit const* p_Attacker, WeaponAttackType attType) const;
        float GetUnitMissChanceSpell(Unit const* p_Attacker) const;
        float GetUnitCriticalChance(WeaponAttackType attackType, const Unit* victim) const;
        int32 GetMechanicResistChance(const SpellInfo* spell);
        uint32 GetDodgeChance(const Unit* p_Victim);
        uint32 GetParryChance(const Unit* p_Victim);
        uint32 GetBlockChance(const Unit* p_Victim);

        bool CanUseAttackType(uint8 attacktype) const
        {
            switch (attacktype)
            {
                case WeaponAttackType::BaseAttack: return !HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);
                case WeaponAttackType::OffAttack: return !HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISARM_OFFHAND);
                case WeaponAttackType::RangedAttack: return !HasFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_DISARM_RANGED);
            }
            return true;
        }

        virtual uint32 GetBlockPercent() { return 30; }

        uint32 GetUnitMeleeSkill(Unit const* target = NULL) const { return (target ? getLevelForTarget(target) : getLevel()) * 5; }
        float GetWeaponProcChance() const;
        float GetPPMProcChance(uint32 WeaponSpeed, float PPM,  const SpellInfo* spellProto) const;

        MeleeHitOutcome RollMeleeOutcomeAgainst (Unit* victim, WeaponAttackType attType);
        MeleeHitOutcome RollMeleeOutcomeAgainst (Unit* victim, WeaponAttackType attType, int32 crit_chance, int32 miss_chance, int32 dodge_chance, int32 parry_chance, int32 block_chance);

        bool isVendor()       const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_VENDOR); }
        bool isTrainer()      const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_TRAINER); }
        bool isQuestGiver()   const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER); }
        bool isGossip()       const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP); }
        bool isTaxi()         const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_FLIGHTMASTER); }
        bool isGuildMaster()  const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_PETITIONER); }
        bool isBattleMaster() const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_BATTLEMASTER); }
        bool isBanker()       const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_BANKER); }
        bool isInnkeeper()    const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_INNKEEPER); }
        bool isSpiritHealer() const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER); }
        bool isSpiritGuide()  const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITGUIDE); }
        bool isTabardDesigner()const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_TABARDDESIGNER); }
        bool isAuctioner()    const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_AUCTIONEER); }
        bool isArmorer()      const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_REPAIR); }
        bool isServiceProvider() const
        {
            return HasFlag(UNIT_FIELD_NPC_FLAGS,
                UNIT_NPC_FLAG_VENDOR | UNIT_NPC_FLAG_TRAINER | UNIT_NPC_FLAG_FLIGHTMASTER |
                UNIT_NPC_FLAG_PETITIONER | UNIT_NPC_FLAG_BATTLEMASTER | UNIT_NPC_FLAG_BANKER |
                UNIT_NPC_FLAG_INNKEEPER | UNIT_NPC_FLAG_SPIRITHEALER |
                UNIT_NPC_FLAG_SPIRITGUIDE | UNIT_NPC_FLAG_TABARDDESIGNER | UNIT_NPC_FLAG_AUCTIONEER);
        }
        bool isSpiritService() const { return HasFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPIRITHEALER | UNIT_NPC_FLAG_SPIRITGUIDE); }

        bool isInFlight()  const { return HasUnitState(UNIT_STATE_IN_FLIGHT); }

        bool isInCombat()  const { return HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT); }
        void CombatStart(Unit* target, bool initialAggro = true);
        void SetInCombatState(bool PvP, Unit* enemy = NULL, bool isControlled = false);
        void SetInCombatWith(Unit* enemy);
        void ClearInCombat();
        uint32 GetCombatTimer() const { return m_CombatTimer; }

        bool HasAuraTypeWithFamilyFlags(AuraType auraType, uint32 familyName, uint32 familyFlags) const;
        bool virtual HasSpell(uint32 /*spellID*/) const { return false; }
        bool HasBreakableByDamageAuraType(AuraType type, uint32 excludeAura = 0) const;
        bool HasBreakableByDamageCrowdControlAura(Unit* excludeCasterChannel = NULL) const;
        bool HasAurasPreventCasting() const;

        bool HasStealthAura()      const { return HasAuraType(SPELL_AURA_MOD_STEALTH); }
        bool HasInvisibilityAura() const { return HasAuraType(SPELL_AURA_MOD_INVISIBILITY); }
        bool isFeared()  const { return HasAuraType(SPELL_AURA_MOD_FEAR) || HasAuraType(SPELL_AURA_MOD_FEAR_2); }
        bool isInRoots() const { return HasAuraType(SPELL_AURA_MOD_ROOT) || HasAuraType(SPELL_AURA_MOD_ROOT_2); }
        bool isInStun() const { return HasAuraType(SPELL_AURA_MOD_STUN); }
        bool isConfused() const { return HasAuraType(SPELL_AURA_MOD_CONFUSE); }
        bool IsPolymorphed() const;

        bool isFrozen() const;

        bool isTargetableForAttack(bool checkFakeDeath = true) const;

        bool IsValidAttackTarget(Unit const* target) const;
        bool _IsValidAttackTarget(Unit const* target, SpellInfo const* bySpell, WorldObject const* obj = NULL) const;

        bool IsValidAssistTarget(Unit const* target) const;
        bool _IsValidAssistTarget(Unit const* target, SpellInfo const* bySpell, bool duelFlag = true) const;

        virtual bool IsInWater() const;
        virtual bool IsUnderWater() const;
        virtual void UpdateUnderwaterState(Map* m, float x, float y, float z);
        bool isInAccessiblePlaceFor(Creature const* c) const;

        void SendHealSpellLog(Unit* victim, uint32 SpellID, uint32 Damage, uint32 OverHeal, uint32 Absorb, bool critical = false, bool multistrike = false);
        int32 HealBySpell(Unit* victim, SpellInfo const* spellInfo, uint32 addHealth, bool critical = false, bool multistrike = false);
        void SendEnergizeSpellLog(Unit* victim, uint32 SpellID, uint32 Damage, Powers powertype);
        void EnergizeBySpell(Unit* victim, uint32 SpellID, int32 Damage, Powers powertype);
        uint32 SpellNonMeleeDamageLog(Unit* victim, uint32 spellID, uint32 damage);

        void CastSpell(SpellCastTargets const& targets, SpellInfo const* spellInfo, CustomSpellValues const* value, TriggerCastFlags triggerFlags = TRIGGERED_NONE, Item* castItem = NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0, float periodicDamageModifier = 0.0f);
        void CastSpell(Unit* victim, uint32 spellId, bool triggered, Item* castItem = NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0, float periodicDamageModifier = 0.0f);
        void CastSpell(Unit* victim, uint32 spellId, TriggerCastFlags triggerFlags = TRIGGERED_NONE, Item* castItem = NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0, float periodicDamageModifier = 0.0f);
        void CastSpell(Unit* victim, SpellInfo const* spellInfo, bool triggered, Item* castItem= NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastSpell(Unit* victim, SpellInfo const* spellInfo, TriggerCastFlags triggerFlags = TRIGGERED_NONE, Item* castItem= NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0, float periodicDamageModifier = 0.0f);
        void CastSpell(Position const p_Pos, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem = nullptr, AuraEffect const* p_AurEff = nullptr, uint64 p_OriginalCaster = 0);
        void CastSpell(SpellDestination const* p_Dest, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem = nullptr, AuraEffect const* p_AurEff = nullptr, uint64 p_OriginalCaster = 0);
        void CastSpell(WorldLocation const* p_Loc, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem = nullptr, AuraEffect const* p_AurEff = nullptr, uint64 p_OriginalCaster = 0);
        void CastSpell(uint32 p_LocEntry, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem = nullptr, AuraEffect const* p_AurEff = nullptr, uint64 p_OriginalCaster = 0);
        void CastSpell(G3D::Vector3 p_Pos, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem = nullptr, AuraEffect const* p_AurEff = nullptr, uint64 p_OriginalCaster = 0);
        void CastSpell(float x, float y, float z, uint32 spellId, bool triggered, Item* castItem = NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastSpell(GameObject* go, uint32 spellId, bool triggered, Item* castItem = NULL, AuraEffect* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastSpell(Item* p_ItemTarget, uint32 p_SpellID, bool p_Triggered, Item* p_CastItem = nullptr, AuraEffect* p_TriggeredByAura = nullptr, uint64 p_OriginalCaster = 0);

        void CastCustomSpell(Unit* Victim, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item* castItem= NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastCustomSpell(Unit* Victim, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, int32 const* bp3, int32 const* bp4, int32 const* bp5, bool triggered, Item* castItem= NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastCustomSpell(uint32 spellId, SpellValueMod mod, int32 value, Unit* Victim = NULL, bool triggered = true, Item* castItem = NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastCustomSpell(float x, float y, float z, uint32 spellId, int32 const* bp0, int32 const* bp1, int32 const* bp2, bool triggered, Item* castItem= NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);
        void CastCustomSpell(uint32 spellId, CustomSpellValues const &value, Unit* Victim = NULL, bool triggered = true, Item* castItem = NULL, AuraEffect const* triggeredByAura = nullptr, uint64 originalCaster = 0);

        Aura* ToggleAura(uint32 spellId, Unit* target);
        Aura* AddAura(uint32 spellId, Unit* target);
        Aura* AddAura(SpellInfo const* spellInfo, uint32 effMask, Unit* target);
        void SetAuraStack(uint32 spellId, Unit* target, uint32 stack);
        void SendPlaySpellVisualKit(uint32 p_KitRecID, uint32 p_KitType, int32 p_Duration = 0);
        void CancelSpellVisualKit(int32 p_SpellVisualKitID);
        void SendPlaySpellVisual(uint32 p_ID, Unit* p_Target, float p_Speed, float p_Orientation, Position p_Pos, bool p_ThisAsPos = false, bool p_SpeedAsTime = false);
        void CancelSpellVisual(int32 p_SpellVisualID);

        void DeMorph();

        void SendAttackStateUpdate(CalcDamageInfo* damageInfo);
        void SendAttackStateUpdate(uint32 HitInfo, Unit* target, uint8 SwingType, SpellSchoolMask damageSchoolMask, uint32 Damage, uint32 AbsorbDamage, uint32 Resist, VictimState TargetState, uint32 BlockedAmount);
        void SendSpellNonMeleeDamageLog(SpellNonMeleeDamage* log);
        void SendSpellNonMeleeDamageLog(Unit* target, uint32 SpellID, uint32 Damage, SpellSchoolMask damageSchoolMask, uint32 AbsorbedDamage, uint32 Resist, bool PhysicalDamage, uint32 Blocked, bool CriticalHit = false);
        void SendPeriodicAuraLog(SpellPeriodicAuraLogInfo* pInfo);
        /// Build and send SMSG_SPELL_MISS_LOG packet
        /// @p_Target   : Target for the missed spell
        /// @p_SpellId  : Missed spell ID
        /// @p_MissInfo : Miss type
        void SendSpellMiss(Unit* p_Target, uint32 p_SpellID, SpellMissInfo p_MissInfo);
        void SendSpellDamageResist(Unit* target, uint32 spellId);
        void SendSpellDamageImmune(Unit* target, uint32 spellId);
        void SendMessageUnfriendlyToSetInRange(WorldPacket* data, float fist);

        void NearTeleportTo(float x, float y, float z, float orientation, bool casting = false);
        void NearTeleportTo(Position const p_Position, bool p_Cast = false) { NearTeleportTo(p_Position.m_positionX, p_Position.m_positionY, p_Position.m_positionZ, p_Position.m_orientation, p_Cast); }
        void NearTeleportTo(uint32 p_LocEntry, bool p_Cast = false)
        {
            WorldSafeLocsEntry const* l_Loc = sWorldSafeLocsStore.LookupEntry(p_LocEntry);
            if (l_Loc == nullptr)
                return;

            NearTeleportTo(l_Loc->x, l_Loc->y, l_Loc->z, l_Loc->o, p_Cast);
        }
        void SendTeleportPacket(Position &p_NewPosition);
        virtual bool UpdatePosition(float x, float y, float z, float ang, bool teleport = false);
        // returns true if unit's position really changed
        bool UpdatePosition(const Position &pos, bool teleport = false) { return UpdatePosition(pos.GetPositionX(), pos.GetPositionY(), pos.GetPositionZ(), pos.GetOrientation(), teleport); }
        void UpdateOrientation(float orientation);
        void UpdateHeight(float newZ);

        void SendMoveKnockBack(Player* player, float speedXY, float speedZ, float vcos, float vsin);
        void KnockbackFrom(float x, float y, float speedXY, float speedZ, Unit* p_Caster = nullptr);
        void JumpTo(float speedXY, float speedZ, bool forward = true);
        void JumpTo(WorldObject* obj, float speedZ);

        void MonsterMoveWithSpeed(float x, float y, float z, float speed, bool generatePath = false, bool forceDestination = false);
        //void SetFacing(float ori, WorldObject* obj = NULL);
        //void SendMonsterMove(float NewPosX, float NewPosY, float NewPosZ, uint8 type, uint32 MovementFlags, uint32 Time, Player* player = NULL);

        void SendCanTurnWhileFalling(bool apply);

        void SetInFront(WorldObject const* target);
        void SetFacingTo(float ori);
        void SetFacingToObject(WorldObject* object);

        void SendChangeCurrentVictimOpcode(HostileReference* pHostileReference);
        void SendClearThreatListOpcode();
        void SendRemoveFromThreatListOpcode(HostileReference* pHostileReference);
        void SendThreatListUpdate();

        void SendClearTarget();

        void BuildHeartBeatMsg(WorldPacket* data) const;

        bool isAlive() const { return (m_deathState == ALIVE); };
        bool isDying() const { return (m_deathState == JUST_DIED); };
        bool isDead() const { return (m_deathState == DEAD || m_deathState == CORPSE); };
        DeathState getDeathState() { return m_deathState; };
        virtual void setDeathState(DeathState s);           // overwrited in Creature/Player/Pet

        uint64 GetOwnerGUID() const { return GetGuidValue(UNIT_FIELD_SUMMONED_BY); }
        void SetOwnerGUID(uint64 owner);
        uint64 GetCreatorGUID() const { return GetGuidValue(UNIT_FIELD_CREATED_BY); }
        void SetCreatorGUID(uint64 creator) { SetGuidValue(UNIT_FIELD_CREATED_BY, creator); }
        uint64 GetMinionGUID() const { return GetGuidValue(UNIT_FIELD_SUMMON); }
        void SetMinionGUID(uint64 guid) { SetGuidValue(UNIT_FIELD_SUMMON, guid); }
        uint64 GetCharmerGUID() const { return GetGuidValue(UNIT_FIELD_CHARMED_BY); }
        void SetCharmerGUID(uint64 owner) { SetGuidValue(UNIT_FIELD_CHARMED_BY, owner); }
        uint64 GetCharmGUID() const { return  GetGuidValue(UNIT_FIELD_CHARM); }
        void SetPetGUID(uint64 guid) { m_SummonSlot[SUMMON_SLOT_PET] = guid; }
        uint64 GetPetGUID() const { return m_SummonSlot[SUMMON_SLOT_PET]; }
        void SetCritterGUID(uint64 guid) { SetGuidValue(UNIT_FIELD_CRITTER, guid); }
        uint64 GetCritterGUID() const { return GetGuidValue(UNIT_FIELD_CRITTER); }
        uint64 GetTargetGUID() const { return GetGuidValue(UNIT_FIELD_TARGET); }

        bool IsControlledByPlayer() const { return m_ControlledByPlayer; }
        uint64 GetCharmerOrOwnerGUID() const { return GetCharmerGUID() ? GetCharmerGUID() : GetOwnerGUID(); }
        uint64 GetCharmerOrOwnerOrOwnGUID() const
        {
            if (uint64 guid = GetCharmerOrOwnerGUID())
                return guid;
            return GetGUID();
        }
        bool isCharmedOwnedByPlayerOrPlayer() const { return IS_PLAYER_GUID(GetCharmerOrOwnerOrOwnGUID()); }

        bool IsWarlockPet() const;
        Player* GetSpellModOwner() const;

        Unit* GetOwner() const;
        Unit* GetOwnerOrUnitItself() const;

        Guardian *GetGuardianPet() const;
        Minion *GetFirstMinion() const;
        Unit* GetCharmer() const;
        Unit* GetCharm() const;
        Unit* GetCharmerOrOwner() const { return GetCharmerGUID() ? GetCharmer() : GetOwner(); }
        Unit* GetCharmerOrOwnerOrSelf() const
        {
            if (Unit* u = GetCharmerOrOwner())
                return u;

            return (Unit*)this;
        }
        Player* GetCharmerOrOwnerPlayerOrPlayerItself() const;
        Player* GetAffectingPlayer() const;

        void SetMinion(Minion *minion, bool apply, PetSlot slot, bool stampeded = false);
        void GetAllMinionsByEntry(std::list<Creature*>& Minions, uint32 entry);
        void RemoveAllMinionsByEntry(uint32 entry);
        void SetCharm(Unit* target, bool apply);
        Unit* GetNextRandomRaidMemberOrPet(float radius);
        bool SetCharmedBy(Unit* charmer, CharmType type, AuraApplication const* aurApp = NULL);
        void RemoveCharmedBy(Unit* charmer);
        void RestoreFaction();

        ControlList m_Controlled;
        Unit* GetFirstControlled() const;
        void RemoveAllControlled();

        bool isCharmed() const { return GetCharmerGUID() != 0; }
        bool isPossessed() const { return HasUnitState(UNIT_STATE_POSSESSED); }
        bool isPossessedByPlayer() const { return HasUnitState(UNIT_STATE_POSSESSED) && IS_PLAYER_GUID(GetCharmerGUID()); }
        bool isPossessing() const
        {
            if (Unit* u = GetCharm())
                return u->isPossessed();
            else
                return false;
        }
        bool isPossessing(Unit* u) const { return u->isPossessed() && GetCharmGUID() == u->GetGUID(); }

        CharmInfo* GetCharmInfo() { return m_charmInfo; }
        CharmInfo* InitCharmInfo();
        void DeleteCharmInfo();
        void UpdateCharmAI();
        //Player* GetMoverSource() const;
        Player* m_movedPlayer;
        SharedVisionList const& GetSharedVisionList() { return m_sharedVision; }
        void AddPlayerToVision(Player* player);
        void RemovePlayerFromVision(Player* player);
        bool HasSharedVision() const { return !m_sharedVision.empty(); }
        void RemoveBindSightAuras();
        void RemoveCharmAuras();

        Pet* CreateTamedPetFrom(Creature* creatureTarget, uint32 spell_id = 0);
        Pet* CreateTamedPetFrom(uint32 creatureEntry, uint32 spell_id = 0);
        bool InitTamedPet(Pet* pet, uint8 level, uint32 spell_id);

        // aura apply/remove helpers - you should better not use these
        Aura* _TryStackingOrRefreshingExistingAura(SpellInfo const* newAura, uint32 effMask, Unit* caster, int32* baseAmount = NULL, Item* castItem = NULL, uint64 casterGUID = 0, int32 castItemLevel = -1);
        void _AddAura(UnitAura* aura, Unit* caster);
        AuraApplication * _CreateAuraApplication(Aura* aura, uint32 effMask);
        void _ApplyAuraEffect(Aura* aura, uint32 effIndex);
        void _ApplyAura(AuraApplication * aurApp, uint32 effMask);
        void _UnapplyAura(AuraApplicationMap::iterator &i, AuraRemoveMode removeMode);
        void _UnapplyAura(AuraApplication * aurApp, AuraRemoveMode removeMode);
        void _RemoveNoStackAuraApplicationsDueToAura(Aura* aura);
        void _RemoveNoStackAurasDueToAura(Aura* aura);
        bool _IsNoStackAuraDueToAura(Aura* appliedAura, Aura* existingAura) const;
        void _RegisterAuraEffect(AuraEffect* aurEff, bool apply);

        // m_ownedAuras container management
        AuraMap      & GetOwnedAuras()       { return m_ownedAuras; }
        AuraMap const& GetOwnedAuras() const { return m_ownedAuras; }

        void RemoveOwnedAura(AuraMap::iterator &i, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        void RemoveOwnedAura(uint32 spellId, uint64 casterGUID = 0, uint32 reqEffMask = 0, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        void RemoveOwnedAura(Aura* aura, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);

        Aura* GetOwnedAura(uint32 spellId, uint64 casterGUID = 0, uint64 itemCasterGUID = 0, uint32 reqEffMask = 0, Aura* except = nullptr) const;

        // m_appliedAuras container management
        AuraApplicationMap      & GetAppliedAuras()       { return m_appliedAuras; }
        AuraApplicationMap const& GetAppliedAuras() const { return m_appliedAuras; }

        AuraStackOnDurationMap      & GetAurasStackOnDuration()       { return m_StackOnDurationMap; }
        AuraStackOnDurationMap const& GetAurasStackOnDuration() const { return m_StackOnDurationMap; }

        void RemoveAura(AuraApplicationMap::iterator &i, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAura(uint32 spellId, uint64 casterGUID = 0, uint32 reqEffMask = 0, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAura(AuraApplication * aurApp, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAura(Aura* aur, AuraRemoveMode mode = AURA_REMOVE_BY_DEFAULT);

        void RemoveAurasDueToSpell(uint32 spellId, uint64 casterGUID = 0, uint32 reqEffMask = 0, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAuraFromStack(uint32 spellId, uint64 casterGUID = 0, AuraRemoveMode removeMode = AURA_REMOVE_BY_DEFAULT);
        void RemoveAurasDueToSpellByDispel(uint32 spellId, uint32 dispellerSpellId, uint64 casterGUID, Unit* dispeller, uint8 chargesRemoved = 1);
        void RemoveAurasDueToSpellBySteal(uint32 spellId, uint64 casterGUID, Unit* stealer);
        void RemoveAurasDueToItemSpell(Item* castItem, uint32 spellId);
        void RemoveAurasByType(AuraType auraType, uint64 casterGUID = 0, Aura* aura = nullptr, uint32 exceptAuraId = 0, bool negative = true, bool positive = true);
        void RemoveEffectsByType(AuraType auraType, uint64 casterGUID = 0, Aura* aura = nullptr, uint32 exceptAuraId = 0, bool negative = true, bool positive = true);
        void RemoveNotOwnSingleTargetAuras(uint32 newPhase = 0x0);
        void RemoveAurasWithInterruptFlags(uint32 flag, uint32 except = 0);
        void RemoveFlagsAuras();
        void RemoveAurasWithAttribute(uint32 flags);
        void RemoveAurasWithFamily(SpellFamilyNames family, uint32 familyFlag1, uint32 familyFlag2, uint32 familyFlag3, uint64 casterGUID);
        void RemoveEffectsWithMechanic(uint32 mechanic_mask, AuraRemoveMode removemode = AURA_REMOVE_BY_DEFAULT, uint32 except = 0);
        void RemoveAurasWithMechanic(uint32 mechanic_mask, AuraRemoveMode removemode = AURA_REMOVE_BY_DEFAULT, uint32 except = 0, uint8 count = 0);
        void RemoveMovementImpairingAuras();
        void RemoveAurasBreakableByDamage();

        void RemoveAreaAurasDueToLeaveWorld();
        void RemoveAllAuras();
        void RemoveNonPassivesAuras();
        void RemoveArenaAuras();
        void RemoveAllAurasOnDeath();
        void RemoveNegativeAuras();
        void RemoveAllAurasRequiringDeadTarget();
        void RemoveAllAurasExceptType(AuraType type);
        void RemoveAllAurasByType(AuraType type);
        void RemoveAllAurasByCaster(uint64 p_Guid);
        void DelayOwnedAuras(uint32 spellId, uint64 caster, int32 delaytime);

        void _RemoveAllAuraStatMods();
        void _ApplyAllAuraStatMods();

        AuraEffectList const& GetAuraEffectsByType(AuraType type) const { return m_modAuras[type]; }
        AuraEffectList GetAuraEffectsByMechanic(uint32 mechanic_mask) const;

        AuraList      & GetSingleCastAuras()       { return m_scAuras; }
        AuraList const& GetSingleCastAuras() const { return m_scAuras; }

        AuraEffect* GetAuraEffect(uint32 spellId, uint8 effIndex, uint64 casterGUID = 0) const;
        AuraEffect* GetAuraEffectOfRankedSpell(uint32 spellId, uint8 effIndex, uint64 casterGUID = 0) const;
        AuraEffect* GetAuraEffect(AuraType type, SpellFamilyNames name, uint32 iconId, uint8 effIndex) const; // spell mustn't have familyflags
        AuraEffect* GetAuraEffect(AuraType type, SpellFamilyNames family, uint32 familyFlag1, uint32 familyFlag2, uint32 familyFlag3, uint64 casterGUID =0);
        inline AuraEffect* GetDummyAuraEffect(SpellFamilyNames name, uint32 iconId, uint32 effIndex) const { return GetAuraEffect(SPELL_AURA_DUMMY, name, iconId, effIndex);}

        AuraApplication * GetAuraApplication(uint32 spellId, uint64 casterGUID = 0, uint64 itemCasterGUID = 0, uint32 reqEffMask = 0, AuraApplication * except = NULL) const;
        Aura* GetAura(uint32 spellId, uint64 casterGUID = 0, uint64 itemCasterGUID = 0, uint32 reqEffMask = 0) const;

        AuraApplication * GetAuraApplicationOfRankedSpell(uint32 spellId, uint64 casterGUID = 0, uint64 itemCasterGUID = 0, uint32 reqEffMask = 0, AuraApplication * except = NULL) const;
        Aura* GetAuraOfRankedSpell(uint32 spellId, uint64 casterGUID = 0, uint64 itemCasterGUID = 0, uint32 reqEffMask = 0) const;

        void GetDispellableAuraList(Unit* caster, uint32 dispelMask, DispelChargesList& dispelList);

        StackOnDuration* GetStackOnDuration(uint32 p_SpellID);
        void AddToStackOnDuration(uint32 p_SpellID, uint64 p_DurationTime, int32 p_Amount);
        void RemoveStackOnDuration(uint32 p_SpellID);

        bool HasAuraEffect(uint32 spellId, uint8 effIndex, uint64 caster = 0) const;
        uint32 GetAuraCount(uint32 spellId) const;
        bool HasAura(uint32 spellId, uint64 casterGUID = 0, uint64 itemCasterGUID = 0, uint32 reqEffMask = 0) const;
        bool HasAuraType(AuraType auraType) const;
        bool HasAuraTypeWithCaster(AuraType auratype, uint64 caster) const;
        bool HasAuraTypeWithMiscvalue(AuraType auratype, int32 miscvalue) const;
        bool HasAuraTypeWithAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const;
        bool HasAuraTypeWithValue(AuraType auratype, int32 value) const;
        bool HasNegativeAuraWithInterruptFlag(uint32 flag, uint64 guid = 0);
        bool HasNegativeAuraWithAttribute(uint32 flag, uint64 guid = 0);
        bool HasAuraWithMechanic(uint32 mechanicMask);
        bool HasAuraWithNegativeCaster(uint32 spellid);

        void RemoveSoulSwapDOT(Unit* target);
        void ApplySoulSwapDOT(Unit* caster, Unit* target);
        void SetSoulSwapDotTarget(uint64 targetGUID) { soulSwapTargetGUID = targetGUID; }
        void RemoveSoulSwapDotTarget() { soulSwapTargetGUID = 0;  }
        uint64 GetSoulSwapDotTargetGUID() { return soulSwapTargetGUID; }
        Unit* GetSoulSwapDotTarget();
        void SetSoulSwapRefreshDuration(bool refresh) { soulSwapRefreshDuration = refresh; }
        bool GetSoulSwapRefreshDuration() { return soulSwapRefreshDuration; }

        AuraEffect* IsScriptOverriden(SpellInfo const* spell, int32 script) const;
        uint32 GetDiseasesByCaster(uint64 casterGUID, bool remove = false);
        uint32 GetDoTsByCaster(uint64 casterGUID) const;

        int32 GetTotalAuraModifier(AuraType auratype, AuraEffect const* excludeAura = nullptr, AuraEffect* includeAura = nullptr) const;
        float GetTotalAuraMultiplier(AuraType auratype) const;
        int32 GetMaxPositiveAuraModifier(AuraType auratype);
        int32 GetMaxNegativeAuraModifier(AuraType auratype) const;

        int32 GetTotalAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask, AuraEffect const* excludeAura = nullptr, AuraEffect* includeAura = nullptr) const;
        int32 GetTotalAuraModifierByMiscBMask(AuraType auratype, uint32 misc_mask, AuraEffect const* excludeAura = nullptr, AuraEffect* includeAura = nullptr) const;
        float GetTotalAuraMultiplierByMiscMask(AuraType auratype, uint32 misc_mask) const;
        int32 GetMaxPositiveAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask, AuraEffect const* except = nullptr) const;
        int32 GetMaxNegativeAuraModifierByMiscMask(AuraType auratype, uint32 misc_mask) const;

        int32 GetTotalAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const;
        float GetTotalAuraMultiplierByMiscValue(AuraType auratype, int32 misc_value) const;
        int32 GetMaxPositiveAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const;
        int32 GetMaxNegativeAuraModifierByMiscValue(AuraType auratype, int32 misc_value) const;

        int32 GetTotalAuraModifierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const;
        float GetTotalAuraMultiplierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const;
        int32 GetMaxPositiveAuraModifierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const;
        int32 GetMaxNegativeAuraModifierByAffectMask(AuraType auratype, SpellInfo const* affectedSpell) const;

        float GetResistanceBuffMods(SpellSchools school, bool positive) const { return GetFloatValue(positive ? UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE+school : UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE+school); }
        void SetResistanceBuffMods(SpellSchools school, bool positive, float val) { SetFloatValue(positive ? UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE+school : UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE+school, val); }
        void ApplyResistanceBuffModsMod(SpellSchools school, bool positive, float val, bool apply) { ApplyModSignedFloatValue(positive ? UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE+school : UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE+school, val, apply); }
        void ApplyResistanceBuffModsPercentMod(SpellSchools school, bool positive, float val, bool apply) { ApplyPercentModFloatValue(positive ? UNIT_FIELD_RESISTANCE_BUFF_MODS_POSITIVE+school : UNIT_FIELD_RESISTANCE_BUFF_MODS_NEGATIVE+school, val, apply); }
        void InitStatBuffMods()
        {
            for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i) SetFloatValue(UNIT_FIELD_STAT_POS_BUFF+i, 0);
            for (uint8 i = STAT_STRENGTH; i < MAX_STATS; ++i) SetFloatValue(UNIT_FIELD_STAT_NEG_BUFF+i, 0);
        }
        void ApplyStatBuffMod(Stats stat, float val, bool apply) { ApplyModSignedFloatValue((val > 0 ? UNIT_FIELD_STAT_POS_BUFF+stat : UNIT_FIELD_STAT_NEG_BUFF+stat), val, apply); }
        void ApplyStatPercentBuffMod(Stats stat, float val, bool apply)
        {
            ApplyPercentModFloatValue(UNIT_FIELD_STAT_POS_BUFF+stat, val, apply);
            ApplyPercentModFloatValue(UNIT_FIELD_STAT_NEG_BUFF+stat, val, apply);
        }
        void SetCreateStat(Stats stat, float val) { m_createStats[stat] = val; }
        void SetCreateHealth(uint32 val) { SetUInt32Value(UNIT_FIELD_BASE_HEALTH, val); }
        uint32 GetCreateHealth() const { return GetUInt32Value(UNIT_FIELD_BASE_HEALTH); }
        void SetCreateMana(uint32 val) { SetUInt32Value(UNIT_FIELD_BASE_MANA, val); }
        uint32 GetCreateMana() const { return GetUInt32Value(UNIT_FIELD_BASE_MANA); }
        int32 GetCreatePowers(Powers power) const;
        float GetPosStat(Stats stat) const { return GetFloatValue(UNIT_FIELD_STAT_POS_BUFF+stat); }
        float GetNegStat(Stats stat) const { return GetFloatValue(UNIT_FIELD_STAT_NEG_BUFF+stat); }
        float GetCreateStat(Stats stat) const { return m_createStats[stat]; }

        void SetCurrentCastedSpell(Spell* pSpell);
        virtual void ProhibitSpellSchool(SpellSchoolMask /*idSchoolMask*/, uint32 /*unTimeMs*/) { }
        void InterruptSpell(CurrentSpellTypes spellType, bool withDelayed = true, bool withInstant = true);
        void FinishSpell(CurrentSpellTypes spellType, bool ok = true);

        // set withDelayed to true to account delayed spells as casted
        // delayed+channeled spells are always accounted as casted
        // we can skip channeled or delayed checks using flags
        bool IsNonMeleeSpellCasted(bool withDelayed, bool skipChanneled = false, bool skipAutorepeat = false, bool isAutoshoot = false, bool skipInstant = true) const;

        // set withDelayed to true to interrupt delayed spells too
        // delayed+channeled spells are always interrupted
        void InterruptNonMeleeSpells(bool withDelayed, uint32 spellid = 0, bool withInstant = true);
        void InterruptNonMeleeSpellsExcept(bool withDelayed, uint32 except, bool withInstant = true);

        Spell* GetCurrentSpell(CurrentSpellTypes spellType) const { return m_currentSpells[spellType]; }
        Spell* GetCurrentSpell(uint32 spellType) const { return m_currentSpells[spellType]; }
        Spell* FindCurrentSpellBySpellId(uint32 spell_id) const;
        int32 GetCurrentSpellCastTime(uint32 spell_id) const;

        uint32 m_addDmgOnce;
        uint64 m_SummonSlot[MAX_SUMMON_SLOT];
        uint64 m_ObjectSlot[MAX_GAMEOBJECT_SLOT];

        ShapeshiftForm GetShapeshiftForm() const { return ShapeshiftForm(GetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 3)); }
        void SetShapeshiftForm(ShapeshiftForm form)
        {
            SetByteValue(UNIT_FIELD_SHAPESHIFT_FORM, 3, form);
        }
        inline bool IsInFeralForm() const
        {
            ShapeshiftForm form = GetShapeshiftForm();
            return form == FORM_CAT || form == FORM_BEAR;
        }
        inline bool IsTravelForm(ShapeshiftForm p_Form) const
        {
            return p_Form == FORM_AQUA || p_Form == FORM_STAG || p_Form == FORM_FLIGHT || p_Form == FORM_FLIGHT_EPIC;
        }
        bool IsInDisallowedMountForm() const;

        float m_modMeleeHitChance;
        float m_modRangedHitChance;
        float m_modSpellHitChance;
        int32 m_baseSpellCritChance;

        float m_threatModifier[MAX_SPELL_SCHOOL];
        float m_modAttackSpeedPct[3];

        // Event handler
        EventProcessor m_Events;

        // stat system
        bool HandleStatModifier(UnitMods unitMod, UnitModifierType modifierType, float amount, bool apply);
        void SetModifierValue(UnitMods unitMod, UnitModifierType modifierType, float value) { m_auraModifiersGroup[unitMod][modifierType] = value; }
        float GetModifierValue(UnitMods unitMod, UnitModifierType modifierType) const;
        float GetTotalStatValue(Stats stat, bool l_IncludeCreateStat = true) const;
        float GetTotalAuraModValue(UnitMods unitMod) const;
        SpellSchools GetSpellSchoolByAuraGroup(UnitMods unitMod) const;
        Stats GetStatByAuraGroup(UnitMods unitMod) const;
        Powers GetPowerTypeByAuraGroup(UnitMods unitMod) const;
        bool CanModifyStats() const { return m_canModifyStats; }
        void SetCanModifyStats(bool modifyStats) { m_canModifyStats = modifyStats; }
        virtual bool UpdateStats(Stats stat) = 0;
        virtual bool UpdateAllStats() = 0;
        virtual void UpdateResistances(uint32 school) = 0;
        virtual void UpdateArmor() = 0;
        virtual void UpdateMaxHealth() = 0;
        virtual void UpdateMaxPower(Powers power) = 0;
        virtual void UpdateAttackPowerAndDamage(bool ranged = false) = 0;
        virtual void UpdateDamagePhysical(WeaponAttackType attType, bool l_NoLongerDualWields = false) = 0;
        float GetTotalAttackPowerValue(WeaponAttackType attType) const;
        float GetWeaponDamageRange(WeaponAttackType attType, WeaponDamageRange type) const;
        void SetBaseWeaponDamage(WeaponAttackType attType, WeaponDamageRange damageRange, float value) { m_weaponDamage[attType][damageRange] = value; }

        bool isInFrontInMap(Unit const* target, float distance, float arc = M_PI) const;
        bool isInBackInMap(Unit const* target, float distance, float arc = M_PI) const;

        // Visibility system
        bool IsVisible() const { return (m_serverSideVisibility.GetValue(SERVERSIDE_VISIBILITY_GM) > SEC_PLAYER) ? false : true; }
        void SetVisible(bool x);

        // common function for visibility checks for player/creatures with detection code
        void SetPhaseMask(uint32 newPhaseMask, bool update);// overwrite WorldObject::SetPhaseMask
        void UpdateObjectVisibility(bool forced = true);

        SpellImmuneList m_spellImmune[MAX_SPELL_IMMUNITY];
        uint32 m_lastSanctuaryTime;

        // Threat related methods
        bool CanHaveThreatList() const;
        void AddThreat(Unit* victim, float fThreat, SpellSchoolMask schoolMask = SPELL_SCHOOL_MASK_NORMAL, SpellInfo const* threatSpell = NULL);
        float ApplyTotalThreatModifier(float fThreat, SpellSchoolMask schoolMask = SPELL_SCHOOL_MASK_NORMAL);
        void DeleteThreatList();
        void TauntApply(Unit* victim);
        void TauntFadeOut(Unit* taunter);
        ThreatManager& getThreatManager() { return m_ThreatManager; }
        void addHatedBy(HostileReference* pHostileReference) { m_HostileRefManager.insertFirst(pHostileReference); };
        void removeHatedBy(HostileReference* /*pHostileReference*/) { /* nothing to do yet */ }
        HostileRefManager& getHostileRefManager() { return m_HostileRefManager; }

        VisibleAuraMap const* GetVisibleAuras() { return &m_visibleAuras; }
        AuraApplication * GetVisibleAura(uint8 slot)
        {
            VisibleAuraMap::iterator itr = m_visibleAuras.find(slot);
            if (itr != m_visibleAuras.end())
                return itr->second;
            return 0;
        }
        void SetVisibleAura(uint8 slot, AuraApplication * aur){ m_visibleAuras[slot]=aur; UpdateAuraForGroup(slot);}
        void RemoveVisibleAura(uint8 slot){ m_visibleAuras.erase(slot); UpdateAuraForGroup(slot);}

        uint32 GetInterruptMask() const { return m_interruptMask; }
        void AddInterruptMask(uint32 mask) { m_interruptMask |= mask; }
        void UpdateInterruptMask();

        uint32 GetDisplayId() const { return GetUInt32Value(UNIT_FIELD_DISPLAY_ID); }
        void SetDisplayId(uint32 modelId);
        uint32 GetNativeDisplayId() const { return GetUInt32Value(UNIT_FIELD_NATIVE_DISPLAY_ID); }
        void RestoreDisplayId();
        void SetNativeDisplayId(uint32 modelId) { SetUInt32Value(UNIT_FIELD_NATIVE_DISPLAY_ID, modelId); }
        void setTransForm(uint32 spellid) { m_transform = spellid;}
        uint32 getTransForm() const { return m_transform;}

        // DynamicObject management
        void _RegisterDynObject(DynamicObject* dynObj);
        void _UnregisterDynObject(DynamicObject* dynObj);
        DynamicObject* GetDynObject(uint32 spellId);
        int32 CountDynObject(uint32 spellId);
        void GetDynObjectList(std::list<DynamicObject*> &list, uint32 spellId);
        void RemoveDynObject(uint32 spellId);
        void RemoveAllDynObjects();

        // AreaTrigger management
        void _RegisterAreaTrigger(AreaTrigger* areaTrigger);
        void _UnregisterAreaTrigger(AreaTrigger* areaTrigger);
        AreaTrigger* GetAreaTrigger(uint32 spellId);
        int32 CountAreaTrigger(uint32 spellId);
        void GetAreaTriggerList(std::list<AreaTrigger*> &list, uint32 spellId);
        void RemoveAreaTrigger(uint32 spellId);
        void RemoveAllAreasTrigger();

        GameObject* GetGameObject(uint32 spellId) const;
        void AddGameObject(GameObject* gameObj);
        void RemoveGameObject(GameObject* gameObj, bool del);
        void RemoveGameObject(uint32 spellid, bool del);
        void RemoveAllGameObjects();

        uint32 CalculateDamage(WeaponAttackType attType, bool normalized, bool addTotalPct);
        float GetAPMultiplier(WeaponAttackType attType, bool normalized);
        void ModifyAuraState(AuraStateType flag, bool apply);
        uint32 BuildAuraStateUpdateForTarget(Unit* target) const;
        bool HasAuraState(AuraStateType flag, SpellInfo const* spellProto = NULL, Unit const* Caster = NULL) const;
        void UnsummonAllTotems();
        Unit* GetMagicHitRedirectTarget(Unit* victim, SpellInfo const* spellInfo);
        Unit* GetMeleeHitRedirectTarget(Unit* victim, SpellInfo const* spellInfo = NULL);

        int32  SpellBaseDamageBonusDone(SpellSchoolMask schoolMask) const;
        int32  SpellBaseDamageBonusTaken(SpellSchoolMask schoolMask) const;
        uint32 SpellDamageBonusDone(Unit* victim, SpellInfo const *spellProto, uint32 pdamage, uint8 effIndex, DamageEffectType damagetype, uint32 stack = 1);
        float  SpellDamagePctDone(Unit* victim, SpellInfo const* spellProto, DamageEffectType damagetype) const;
        uint32 SpellDamageBonusTaken(Unit* caster, SpellInfo const *spellProto, uint32 pdamage, DamageEffectType damagetype, uint32 stack = 1);
        int32  SpellBaseHealingBonusDone(SpellSchoolMask schoolMask);
        int32  SpellBaseHealingBonusTaken(SpellSchoolMask schoolMask);
        uint32 SpellHealingBonusDone(Unit* victim, SpellInfo const *spellProto, uint32 healamount, uint8 effIndex, DamageEffectType damagetype, uint32 stack = 1);
        float  SpellHealingPctDone(Unit* victim, SpellInfo const* spellProto) const;
        uint32 SpellHealingBonusTaken(Unit* caster, SpellInfo const *spellProto, uint32 healamount, DamageEffectType damagetype, uint32 stack = 1);

        uint32 MeleeDamageBonusDone(Unit *pVictim, uint32 damage, WeaponAttackType attType, SpellInfo const *spellProto = NULL);
        uint32 MeleeDamageBonusTaken(Unit* attacker, uint32 pdamage,WeaponAttackType attType, SpellInfo const *spellProto = NULL);


        bool   isSpellBlocked(Unit* victim, SpellInfo const* spellProto, WeaponAttackType attackType = WeaponAttackType::BaseAttack);
        bool   isBlockCritical();
        bool   IsSpellMultistrike() const;
        uint32 GetMultistrikeBasePoints(uint32 p_Damage) const;
        uint8  ProcMultistrike(SpellInfo const* p_ProcSpell, Unit* p_Target, uint32 p_ProcFlag, uint32 p_ProcExtra, uint32 p_Damage, WeaponAttackType p_AttType = WeaponAttackType::BaseAttack, SpellInfo const* p_ProcAura = NULL, AuraEffect const* p_OwnerAuraEffect = NULL);
        uint8  ProcTimesMultistrike(SpellInfo const* p_ProcSpell, Unit* p_Target);
        void   ProcAuraMultistrike(SpellInfo const* p_ProcSpell, Unit* p_Target, int32& p_Amount);
        bool   IsSpellCrit(Unit* victim, SpellInfo const* spellProto, SpellSchoolMask schoolMask, WeaponAttackType attackType = WeaponAttackType::BaseAttack) const;
        bool   IsAuraAbsorbCrit(SpellInfo const* spellProto, SpellSchoolMask schoolMask) const;
        float  GetUnitSpellCriticalChance(Unit* victim, SpellInfo const* spellProto, SpellSchoolMask schoolMask, WeaponAttackType attackType = WeaponAttackType::BaseAttack) const;
        uint32 MeleeCriticalDamageBonus(SpellInfo const* p_SpellProto, uint32 p_Damage, Unit* p_Victim, WeaponAttackType p_AttackType);
        uint32 SpellCriticalDamageBonus(SpellInfo const* p_SpellProto, uint32 p_Damage, Unit* p_Victim);
        uint32 SpellCriticalHealingBonus(SpellInfo const* p_SpellProto, uint32 p_Damage, Unit* p_Victim);
        uint32 SpellCriticalAuraAbsorbBonus(SpellInfo const* p_SpellProto, uint32 p_Damage);
        bool   IsUnitAbleToCrit() const;

        void SetContestedPvP(Player* attackedPlayer = NULL);

        uint32 GetCastingTimeForBonus(SpellInfo const* spellProto, DamageEffectType damagetype, uint32 CastingTime) const;
        float CalculateDefaultCoefficient(SpellInfo const *spellInfo, DamageEffectType damagetype) const;

        uint32 GetRemainingPeriodicAmount(uint64 caster, uint32 spellId, AuraType auraType, uint8 effectIndex = 0) const;

        void ApplyUberImmune(uint32 spellid, bool apply);
        void ApplySpellImmune(uint32 spellId, uint32 op, uint32 type, bool apply);
        void ApplySpellDispelImmunity(const SpellInfo* spellProto, DispelType type, bool apply);
        virtual bool IsImmunedToSpell(SpellInfo const* spellInfo);
        uint32 GetSchoolImmunityMask() const;
        uint32 GetMechanicImmunityMask() const;
                                                            // redefined in Creature
        bool IsImmunedToDamage(SpellSchoolMask meleeSchoolMask);
        bool IsImmunedToDamage(SpellInfo const* spellInfo);
        virtual bool IsImmunedToSpellEffect(SpellInfo const* spellInfo, uint32 index) const;
                                                            // redefined in Creature
        static bool IsDamageReducedByArmor(SpellSchoolMask damageSchoolMask, SpellInfo const* spellInfo = nullptr, uint8 effIndex = SpellEffIndex::MAX_EFFECTS);
        uint32 CalcArmorReducedDamage(Unit* victim, const uint32 damage, SpellInfo const* spellInfo, WeaponAttackType attackType=WeaponAttackType::MaxAttack);
        SpellSchoolMask CalcAbsorbResist(Unit* victim, SpellSchoolMask schoolMask, DamageEffectType damagetype, const uint32 damage, uint32 *absorb, uint32 *resist, SpellInfo const* spellInfo = nullptr);
        void CalcHealAbsorb(Unit* victim, const SpellInfo* spellProto, uint32 &healAmount, uint32 &absorb);
        bool IsSpellResisted(Unit* victim, SpellSchoolMask schoolMask, SpellInfo const* spellInfo);

        void  UpdateSpeed(UnitMoveType mtype, bool forced);
        float GetSpeed(UnitMoveType mtype) const
        {
            return m_speed_rate[mtype] * (IsControlledByPlayer() ? playerBaseMoveSpeed[mtype] : baseMoveSpeed[mtype]);
        }
        float GetSpeedRate(UnitMoveType mtype) const { return m_speed_rate[mtype]; }
        void SetSpeed(UnitMoveType mtype, float rate, bool forced = false);

        void SendAdjustSplineDuration(float p_Scale);

        /// This is used to send the current spline percentage
        /// Send values from 0 to 1, depending on the current waypoint of the current spline
        void SendFlightSplineSync(float p_SplineDist);

        float m_TempSpeed;

        bool isHover() const { return HasAuraType(SPELL_AURA_HOVER); }
        bool isCamouflaged() const { return HasAuraType(SPELL_AURA_MOD_CAMOUFLAGE); }

        float ApplyEffectModifiers(SpellInfo const* spellProto, uint8 effect_index, float value) const;
        int32 CalculateSpellDamage(Unit const* p_Target, SpellInfo const* p_SpellProto, uint8 p_EffectIndex, int32 const* p_BasePoints = nullptr, int32 itemLevel = -1, bool p_Log = false) const;
        int32 CalcSpellDuration(SpellInfo const* spellProto);
        int32 ModSpellDuration(SpellInfo const* spellProto, Unit const* target, int32 duration, bool positive, uint32 effectMask);
        void  ModSpellCastTime(SpellInfo const* spellProto, int32 & castTime, Spell* spell = nullptr);

        void addFollower(FollowerReference* pRef) { m_FollowingRefManager.insertFirst(pRef); }
        void removeFollower(FollowerReference* /*pRef*/) { /* nothing to do yet */ }
        static Unit* GetUnit(WorldObject const& object, uint64 guid);
        static Player* GetPlayer(WorldObject const& object, uint64 guid);
        static Creature* GetCreature(WorldObject const& object, uint64 guid);

        MotionMaster* GetMotionMaster() { return &i_motionMaster; }
        const MotionMaster* GetMotionMaster() const { return &i_motionMaster; }

        bool IsStopped() const { return !(HasUnitState(UNIT_STATE_MOVING)); }
        void StopMoving();

        void AddUnitMovementFlag(uint32 f) { m_movementInfo.flags |= f; }
        void RemoveUnitMovementFlag(uint32 f) { m_movementInfo.flags &= ~f; }
        bool HasUnitMovementFlag(uint32 f) const { return (m_movementInfo.flags & f) == f; }
        uint32 GetUnitMovementFlags() const { return m_movementInfo.flags; }
        void SetUnitMovementFlags(uint32 f)
        {
            m_movementInfo.flags = f;
        }

        void ClearMovementData()
        {
            m_movementInfo.Alive32 = 0;
            m_movementInfo.HasFallData = false;
            m_movementInfo.hasFallDirection = false;
        }
        void AddExtraUnitMovementFlag(uint16 f) { m_movementInfo.flags2 |= f; }
        void RemoveExtraUnitMovementFlag(uint16 f) { m_movementInfo.flags2 &= ~f; }
        uint16 HasExtraUnitMovementFlag(uint16 f) const { return m_movementInfo.flags2 & f; }
        uint16 GetExtraUnitMovementFlags() const { return m_movementInfo.flags2; }
        void SetExtraUnitMovementFlags(uint16 f) { m_movementInfo.flags2 = f; }
        bool IsSplineEnabled() const;
        bool IsSplineFinished() const;

        void WriteMovementUpdate(WorldPacket &data) const;

        float GetPositionZMinusOffset() const
        {
            float offset = 0.0f;
            if (HasUnitMovementFlag(MOVEMENTFLAG_HOVER))
                offset = GetFloatValue(UNIT_FIELD_HOVER_HEIGHT);

            return GetPositionZ() - offset;
        }

        /// Get interpolated player position based on last received movement informations
        /// @p_AtClientScreen : Interpolated with client network delay ?
        /// @p_ProjectTime    : Time target of prediction
        Position GetInterpolatedPosition(bool p_AtClientScreen, uint32 p_ProjectTime);

        void SetControlled(bool apply, UnitState state);

        /// Control Alert
        void SendLossOfControlAuraUpdate(AuraApplication const* p_AurApp, Mechanics p_Mechanic, SpellEffIndex p_EffIndex, LossOfControlType p_Type);
        void SendClearLossOfControl();
        void SendAddLossOfControl(AuraApplication const* p_AurApp, Mechanics p_Mechanic, LossOfControlType p_Type);
        void SendRemoveLossOfControl(AuraApplication const* p_AurApp, LossOfControlType p_Type);

        ///----------Pet responses methods-----------------
        void SendPetCastFail(uint32 spellid, SpellCastResult msg, uint8 l_CastCount);
        void SendPetActionFeedback (uint32 p_SpellID, uint8 p_Reason);
        void SendPetTalk (uint32 pettalk);
        void SendPetAIReaction(uint64 guid);
        ///----------End of Pet responses methods----------

        void SendItemBonusDebug(uint32 p_Quantity, std::string p_Text, Player* p_Target = nullptr);

        void propagateSpeedChange() { GetMotionMaster()->propagateSpeedChange(); }

        // reactive attacks
        void ClearAllReactives();
        void StartReactiveTimer(ReactiveType reactive) { m_reactiveTimer[reactive] = REACTIVE_TIMER_START;}
        void UpdateReactives(uint32 p_time);

        // group updates
        void UpdateAuraForGroup(uint8 slot);

        /// Stacks Updates
        void UpdateStackOnDuration(uint32 p_time);

        // proc trigger system
        bool CanProc(){return !m_procDeep;}
        void SetCantProc(bool apply)
        {
            if (apply)
                ++m_procDeep;
            else
            {
                ASSERT(m_procDeep);
                --m_procDeep;
            }
        }

        // pet auras
        typedef std::set<PetAura const*> PetAuraSet;
        PetAuraSet m_petAuras;
        void AddPetAura(PetAura const* petSpell);
        void RemovePetAura(PetAura const* petSpell);

        uint32 GetModelForForm(ShapeshiftForm form);
        uint32 GetModelForTotem(PlayerTotemType totemType);

        void SetReducedThreatPercent(uint32 pct, uint64 guid)
        {
            m_reducedThreatPercent = pct;
            m_misdirectionTargetGUID = guid;
        }
        uint32 GetReducedThreatPercent() { return m_reducedThreatPercent; }
        Unit* GetMisdirectionTarget() { return m_misdirectionTargetGUID ? GetUnit(*this, m_misdirectionTargetGUID) : NULL; }

        bool IsAIEnabled, NeedChangeAI;
        bool CreateVehicleKit(uint32 id, uint32 creatureEntry);
        void RemoveVehicleKit(bool dismount = false);
        Vehicle* GetVehicleKit()const { return m_vehicleKit; }
        Vehicle* GetVehicle()   const { return m_vehicle; }
        bool IsOnVehicle() const { return m_vehicle != NULL; }
        bool IsOnVehicle(const Unit* vehicle) const { return m_vehicle && m_vehicle == vehicle->GetVehicleKit(); }
        Unit* GetVehicleBase()  const;
        Creature* GetVehicleCreatureBase() const;
        uint64 GetTransGUID()   const;
        // Returns the transport this unit is on directly (if on vehicle and transport, return vehicle)
        TransportBase* GetDirectTransport() const;

        bool m_ControlledByPlayer;

        bool HandleSpellClick(Unit* clicker, int8 seatId = -1);
        void EnterVehicle(Unit* base, int8 seatId = -1, bool fullTriggered = false);
        void ExitVehicle(Position const* exitPosition = NULL);
        void ChangeSeat(int8 seatId, bool next = true);

        // Should only be called by AuraEffect::HandleAuraControlVehicle(AuraApplication const* auraApp, uint8 mode, bool apply) const;
        void _ExitVehicle(Position const* exitPosition = NULL);
        void _EnterVehicle(Vehicle* vehicle, int8 seatId, AuraApplication const* aurApp = NULL);

        virtual bool CanFly() const = 0;

        bool IsLevitating() const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY); }
        bool IsWalking()    const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_WALKING); }
        bool IsHovering()   const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_HOVER); }
        bool IsFlying()     const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_FLYING | MOVEMENTFLAG_DISABLE_GRAVITY); }
        bool IsMoving()     const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_MASK_MOVING); }
        bool IsTurning()    const { return m_movementInfo.HasMovementFlag(MOVEMENTFLAG_MASK_TURNING); }
        bool IsFalling()    const;
        bool SetWalk(bool p_Apply);
        bool SetDisableGravity(bool p_Disable, bool p_PacketOnly = false);
        bool SetFall(bool p_Apply);
        bool SetSwim(bool p_Apply);
        bool SetCanFly(bool p_Apply);
        bool SetWaterWalking(bool p_Apply, bool packetOnly = false);
        bool SetFeatherFall(bool p_Apply, bool packetOnly = false);
        bool SetHover(bool p_Apply, bool packetOnly = false);
        bool SetCollision(bool p_Apply);

        void RewardRage(float baseRage);

        virtual float GetFollowAngle() const { return static_cast<float>(M_PI/2); }

        void OutDebugInfo() const;
        virtual bool isBeingLoaded() const { return false;}
        bool IsDuringRemoveFromWorld() const {return m_duringRemoveFromWorld;}

        Pet* ToPet() { if (isPet()) return reinterpret_cast<Pet*>(this); else return NULL; }
        Pet const* ToPet() const { if (isPet()) return reinterpret_cast<Pet const*>(this); else return NULL; }

        Totem* ToTotem() { if (isTotem()) return reinterpret_cast<Totem*>(this); else return NULL; }
        Totem const* ToTotem() const { if (isTotem()) return reinterpret_cast<Totem const*>(this); else return NULL; }

        TempSummon* ToTempSummon() { if (isSummon()) return reinterpret_cast<TempSummon*>(this); else return NULL; }
        TempSummon const* ToTempSummon() const { if (isSummon()) return reinterpret_cast<TempSummon const*>(this); else return NULL; }

        void SetTarget(uint64 guid)
        {
            if (!_focusSpell)
                SetGuidValue(UNIT_FIELD_TARGET, guid);
        }

        // Handling caster facing during spell cast
        void FocusTarget(Spell const* p_FocusSpell, WorldObject* p_Target);
        void ReleaseFocus(Spell const* focusSpell);

        uint32 GetDamageDoneInPastSecsBySpell(uint32 secs, uint32 spellId);
        void SetDamageDone(DamageDone* dmgDone) { m_dmgDone.push_back(dmgDone); }

        // Movement info
        Movement::MoveSpline * movespline;


        /// Helper for Rushing Jade Wind
        bool GetHelperTargets(uint32 p_SpellID, uint64 p_Guid) { return m_HelperCountTargetsGUID[p_SpellID].count(p_Guid); }
        int8 GetHelperCountNbNbTargets (uint32 p_SpellID) { return m_HelperCountTargetsGUID[p_SpellID].size(); }
        void AddHelperCountNbTargets(uint32 p_SpellID, uint64 p_Guid) { m_HelperCountTargetsGUID[p_SpellID].insert(p_Guid); }
        void CleanHelperCountNbTargets(uint32 p_SpellID) { m_HelperCountTargetsGUID[p_SpellID].clear(); }

        /// Helper for Glaive of Toss
        uint64 GetGlaiveOfTossTargetGUID() { return m_GlaiveOfTossTargetGUID;  }
        void SetGlaiveTossTarget(uint64 guid) { m_GlaiveOfTossTargetGUID = guid; }
        void removeGlaiveTossTarget() { m_GlaiveOfTossTargetGUID = 0; }

        /// Helper for Fists of Fury
        bool GetFistsOfFuryStunTargets(uint64 p_Guid) { return m_FistsOfFuryStunTargetsGUID.count(p_Guid); }
        void AddFistsOfFuryStunTargets(uint64 p_Guid) { m_FistsOfFuryStunTargetsGUID.insert(p_Guid); }
        void CleanFistsOfFuryStunTargets() { m_FistsOfFuryStunTargetsGUID.clear(); }

        // helper for dark simulacrum spell
        Unit* GetSimulacrumTarget();
        void setSimulacrumTarget(uint64 guid) { simulacrumTargetGUID = guid; }
        void removeSimulacrumTarget() { simulacrumTargetGUID = 0; }

        // helper for Comet Storm
        void SetAmountOfComets(uint32 amount) { m_AmountOfComets = amount; }
        void SetCometStartCoordinateX(float X) { m_CometCoordinateX = X; }
        void SetCometStartCoordinateY(float Y) { m_CometCoordinateY = Y; }
        float GetCometStartCoordinateX() { return m_CometCoordinateX; }
        float GetCometStartCoordinateY() { return m_CometCoordinateY; }
        uint32 GetAmountOfComets() { return m_AmountOfComets; }

        // helper for dispels cooldown
        void SetDispelSuccessful(bool success) { m_IsDispelSuccessful = success; }
        bool IsDispelSuccessful() { return m_IsDispelSuccessful; }

        // helpers for Icicles spells
        uint64 GetIciclesTarget() const { return iciclesTargetGUID; }
        void SetIciclesTarget(uint64 guid) { iciclesTargetGUID = guid; }

        /// helepers for Shooting Stars
        uint64 GetLastMoonfireTarget() const { return lastMoonfireTargetGUID; }
        void SetLastMoonfireTarget(uint64 guid) { lastMoonfireTargetGUID = guid; }
        uint64 GetLastSunfireTarget() const { return lastSunfireTargetGUID; }
        void SetLastSunfireTarget(uint64 guid) { lastSunfireTargetGUID = guid; }

        /// helpers for Death from above
        uint8 GetDFOComboPoints() const { return dfoComboPoints; }
        void SetDFOComboPoints(uint8 p_ConboPoints) { dfoComboPoints = p_ConboPoints; }

        /// helpers for Psychic Horror
        bool GetPsychicHorrorGainedPower() const { return psychicHorrorGainedPower; }
        void SetPsychicHorrorGainedPower(bool gained) { psychicHorrorGainedPower = gained; }

        /// helpers for LEAP_BACK spell, if need to handle something after landing
        void SetLastUsedLeapBackSpell(uint32 l_CurrentSpellId) { l_LastUsedLeapBackSpell = l_CurrentSpellId; }
        void ClearLastUsedLeapBackSpell() { l_LastUsedLeapBackSpell = 0; }
        uint32 GetLastUsedLeapBackSpell() { return l_LastUsedLeapBackSpell; }

        /// helpers for Devouring Plague DOT damage
        void SetDevouringPlagueDamage(uint32 l_CurrentDamage) { m_DevouringPlagueDamage = l_CurrentDamage; }
        uint32 GetDevouringPlagueDamage() { return m_DevouringPlagueDamage; }

        /// helper for Command Demon
        int32 GetCommandDemonSpellByEntry(uint32 p_Entry);

        void DisableHealthRegen() { m_disableHealthRegen = true; }
        void ReenableHealthRegen() { m_disableHealthRegen = false; }
        bool HealthRegenIsDisable() const { return m_disableHealthRegen; }

        void DisableEvadeMode() { m_disableEnterEvadeMode = true; }
        void ReenableEvadeMode() { m_disableEnterEvadeMode = false; }
        bool EvadeModeIsDisable() const { return m_disableEnterEvadeMode; }

        void BuildEncounterFrameData(WorldPacket* p_Data, bool p_Engage, uint8 p_TargetFramePriority = 0);

        bool AddPoisonTarget(uint32 p_SpellID, uint32 p_LowGuid);
        bool HasPoisonTarget(uint32 p_LowGuid) const;
        void RemovePoisonTarget(uint32 p_LowGuid, uint32 p_SpellID);
        void ClearPoisonTargets();
        ///     LowGuid          SpellIDs
        std::map<uint32, std::set<uint32>> m_PoisonTargets;

        void SetChannelSpellID(uint32 p_SpellID);
        void SetChannelSpellID(SpellInfo const* p_SpellInfo);

        void SetHealingRainTrigger(uint64 p_Guid) { m_HealingRainTrigger = p_Guid; }
        uint64 GetHealingRainTrigger() const { return m_HealingRainTrigger; }
        uint64 m_HealingRainTrigger;

        void SetPersonnalChauffeur(uint64 p_Guid) { m_PersonnalChauffeur = p_Guid; }
        uint64 GetPersonnalChauffeur() const { return m_PersonnalChauffeur; }
        uint64 m_PersonnalChauffeur;

        void SetRooted(bool apply);

        Position m_LastAreaPosition;
        Position m_LastZonePosition;
        uint32 m_LastAreaId;
        uint32 m_LastZoneId;
        uint32 GetZoneId(bool p_ForceRecalc = false) const;
        uint32 GetAreaId(bool p_ForceRecalc = false) const;
        void GetZoneAndAreaId(uint32& p_ZoneId, uint32& p_AreaId, bool p_ForceRecalc = false) const;

        Position m_LastNotifyPosition;
        Position m_LastOutdoorPosition;
        bool m_LastOutdoorStatus;
        bool IsOutdoors();

#ifdef CROSS
        int32 GetMapSwitchDestination() const { return m_MapSwitchDestination; }
        void SetMapSwitchDestination(int32 p_MapId) { m_MapSwitchDestination = p_MapId; }

#endif /* CROSS */
    public:
        uint64 _petBattleId;

    protected:
        explicit Unit (bool isWorldObject);

        void BuildValuesUpdate(uint8 updatetype, ByteBuffer* data, Player* target) const;

        UnitAI* i_AI, *i_disabledAI;

        void _UpdateSpells(uint32 time);
        void _DeleteRemovedAuras();

        void _UpdateAutoRepeatSpell();

        bool m_AutoRepeatFirstCast;

        int32 m_attackTimer[WeaponAttackType::MaxAttack];

        float m_createStats[MAX_STATS];

        AttackerSet m_attackers;
        Unit* m_attacking;

        DeathState m_deathState;

        int32 m_procDeep;

        typedef std::list<DynamicObject*> DynObjectList;
        DynObjectList m_dynObj;

        typedef std::list<AreaTrigger*> AreaTriggerList;
        AreaTriggerList m_AreaTrigger;

        typedef std::list<GameObject*> GameObjectList;
        GameObjectList m_gameObj;
        bool m_isSorted;
        uint32 m_transform;

        Spell* m_currentSpells[CURRENT_MAX_SPELL];

        AuraMap m_ownedAuras;
        AuraApplicationMap m_appliedAuras;
        AuraList m_removedAuras;
        AuraMap::iterator m_auraUpdateIterator;
        uint32 m_removedAurasCount;
        AuraStackOnDurationMap m_StackOnDurationMap;
        AuraEffectList m_modAuras[TOTAL_AURAS];
        AuraList m_scAuras;                        // casted singlecast auras
        AuraApplicationList m_interruptableAuras;             // auras which have interrupt mask applied on unit
        AuraStateAurasMap m_auraStateAuras;        // Used for improve performance of aura state checks on aura apply/remove
        uint32 m_interruptMask;
        AuraIdList _SoulSwapDOTList;
        struct SoulSwapAurasData
        {
            SoulSwapAurasData(uint32 id, int32 duration, uint8 stacks, int32 damage, int32 amplitude) : m_id(id), m_duration(duration),
            m_stacks(stacks), m_damage(damage),
            m_amplitude(amplitude){}
            uint32 m_id;
            int32 m_duration;
            uint8 m_stacks;
            int32 m_damage;
            int32 m_amplitude;
        };
        std::set<SoulSwapAurasData*> _SoulSwapDOTData;

        typedef std::list<DamageDone*> DmgDoneList;
        DmgDoneList m_dmgDone;

        float m_auraModifiersGroup[UNIT_MOD_END][MODIFIER_TYPE_END];
        float m_weaponDamage[WeaponAttackType::MaxAttack][2];
        bool m_canModifyStats;
        VisibleAuraMap m_visibleAuras;

        float m_speed_rate[MAX_MOVE_TYPE];

        CharmInfo* m_charmInfo;
        SharedVisionList m_sharedVision;

        virtual SpellSchoolMask GetMeleeDamageSchoolMask() const;

        MotionMaster i_motionMaster;

        uint32 m_reactiveTimer[MAX_REACTIVE];
        uint32 m_RegenPowerTimer;

        ThreatManager m_ThreatManager;

        Vehicle* m_vehicle;
        Vehicle* m_vehicleKit;

        uint32 m_unitTypeMask;
        LiquidTypeEntry const* _lastLiquid;

        // Zone Skip Update
        uint32 _skipCount;
        uint32 _skipDiff;

        bool m_IsInKillingProcess;
        bool m_disableHealthRegen;
        bool m_disableEnterEvadeMode;

        bool IsAlwaysVisibleFor(WorldObject const* seer) const;
        bool IsAlwaysDetectableFor(WorldObject const* seer) const;

        void DisableSpline();

        uint32 m_SendTransportMoveTimer;

        uint32 m_lastRegenTime[MAX_POWERS];
        uint32 m_powers[MAX_POWERS];

    private:
        bool IsTriggeredAtSpellProcEvent(Unit* victim, Aura* aura, SpellInfo const* procSpell, uint32 procFlag, uint32 procExtra, WeaponAttackType attType, bool isVictim, bool active, SpellProcEventEntry const*& spellProcEvent);
        bool RollProcResult(Unit* victim, Aura* aura, WeaponAttackType attType, bool isVictim, SpellProcEventEntry const* spellProcEvent);
        bool HandleAuraProcOnPowerAmount(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const *procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleDummyAuraProc(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleHasteAuraProc(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleSpellCritChanceAuraProc(Unit* victim, uint32 damage, AuraEffect* triggredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleObsModEnergyAuraProc(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleModDamagePctTakenAuraProc(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleAuraProc(Unit* victim, uint32 damage, Aura* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown, bool * handled);
        bool HandleProcTriggerSpell(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 procFlag, uint32 procEx, uint32 cooldown);
        bool HandleOverrideClassScriptAuraProc(Unit* victim, uint32 damage, AuraEffect* triggeredByAura, SpellInfo const* procSpell, uint32 cooldown);
        bool HandleAuraRaidProcFromChargeWithValue(AuraEffect* triggeredByAura);
        bool HandleAuraRaidProcFromCharge(AuraEffect* triggeredByAura);

        // player or player's pet
        float GetCombatRatingReduction(CombatRating cr) const;

    public:
        void UpdateSplineMovement(uint32 t_diff);
        void UpdateSplinePosition();

    protected:
        void SendMoveRoot(uint32 value);
        void SendMoveUnroot(uint32 value);
        void SetFeared(bool apply);
        void SetConfused(bool apply);
        void SetStunned(bool apply);

    private:
        class AINotifyTask;
        class VisibilityUpdateTask;
        Position m_lastVisibilityUpdPos;
        bool m_VisibilityUpdScheduled;
        uint32 m_rootTimes;

        uint32 m_state;                                     // Even derived shouldn't modify
        uint32 m_CombatTimer;
        TimeTrackerSmall m_movesplineTimer;
        TimeTrackerSmall m_FlightSplineSyncTimer;

        uint64 simulacrumTargetGUID;
        uint64 m_GlaiveOfTossTargetGUID;
        std::set<uint64> m_FistsOfFuryStunTargetsGUID;
        std::map<uint32, std::set<uint64>> m_HelperCountTargetsGUID;
        uint64 iciclesTargetGUID;
        uint64 lastMoonfireTargetGUID;
        uint64 lastSunfireTargetGUID;
        uint8 dfoComboPoints;
        uint32 m_AmountOfComets;
        float m_CometCoordinateX;
        float m_CometCoordinateY;
        bool m_IsDispelSuccessful;
        bool psychicHorrorGainedPower;
        uint64 soulSwapTargetGUID;
        bool soulSwapRefreshDuration;
        uint32 l_LastUsedLeapBackSpell;
        uint32 m_DevouringPlagueDamage;

        Diminishing m_Diminishing;
        // Manage all Units that are threatened by us
        HostileRefManager m_HostileRefManager;

        FollowerRefManager m_FollowingRefManager;

        ComboPointHolderSet m_ComboPointHolders;

        uint32 m_reducedThreatPercent;
        uint64 m_misdirectionTargetGUID;

        bool m_cleanupDone; // lock made to not add stuff after cleanup before delete
        bool m_duringRemoveFromWorld; // lock made to not add stuff after beginning removing from world

        Spell const* _focusSpell;   ///> Locks the target during spell cast for proper facing
        bool _isWalkingBeforeCharm; // Are we walking before we were charmed?

        time_t _lastDamagedTime;
#ifdef CROSS

        int32 m_MapSwitchDestination;
#endif /* CROSS */
};

namespace JadeCore
{
    // Binary predicate for sorting Units based on percent value of a power
    class PowerPctOrderPred
    {
        public:
            PowerPctOrderPred(Powers power, bool ascending = true) : m_power(power), m_ascending(ascending) { }
            bool operator() (const Unit* a, const Unit* b) const
            {
                float rA = a->GetMaxPower(m_power) ? float(a->GetPower(m_power)) / float(a->GetMaxPower(m_power)) : 0.0f;
                float rB = b->GetMaxPower(m_power) ? float(b->GetPower(m_power)) / float(b->GetMaxPower(m_power)) : 0.0f;
                return m_ascending ? rA < rB : rA > rB;
            }
        private:
            const Powers m_power;
            const bool m_ascending;
    };


    // Binary predicate for sorting WorldObject based on value of distance of an other WorldObject
    class WorldObjectDistanceCompareOrderPred
    {
        public:
            WorldObjectDistanceCompareOrderPred(const WorldObject* source, bool ascending = true) : m_object(source), m_ascending(ascending) {}
            bool operator() (const WorldObject* a, const WorldObject* b) const
            {
                return m_ascending ? a->GetDistance(m_object) < b->GetDistance(m_object) :
                                     a->GetDistance(m_object) > b->GetDistance(m_object);
            }
        private:
            const WorldObject* m_object;
            const bool m_ascending;
    };

    // Binary predicate for sorting Units based on percent value of health
    class HealthPctOrderPred
    {
        public:
            HealthPctOrderPred(bool ascending = true) : m_ascending(ascending) {}
            bool operator() (const Unit* a, const Unit* b) const
            {
                float rA = a->GetMaxHealth() ? float(a->GetHealth()) / float(a->GetMaxHealth()) : 0.0f;
                float rB = b->GetMaxHealth() ? float(b->GetHealth()) / float(b->GetMaxHealth()) : 0.0f;
                return m_ascending ? rA < rB : rA > rB;
            }
            bool operator() (const WorldObject* a, const WorldObject* b) const
            {
                if (a->GetTypeId() != TYPEID_UNIT || b->GetTypeId() != TYPEID_UNIT)
                    return false;

                float rA = a->ToUnit()->GetMaxHealth() ? float(a->ToUnit()->GetHealth()) / float(a->ToUnit()->GetMaxHealth()) : 0.0f;
                float rB = b->ToUnit()->GetMaxHealth() ? float(b->ToUnit()->GetHealth()) / float(b->ToUnit()->GetMaxHealth()) : 0.0f;
                return m_ascending ? rA < rB : rA > rB;
            }
        private:
            const bool m_ascending;
    };

    // Binary predicate for sorting DynamicObjects based on value of duration
    class AreaTriggerDurationPctOrderPred
    {
        public:
            AreaTriggerDurationPctOrderPred(bool ascending = true) : m_ascending(ascending) {}
            bool operator() (const AreaTrigger* a, const AreaTrigger* b) const
            {
                int32 rA = a->GetDuration() ? a->GetDuration() : 0;
                int32 rB = b->GetDuration() ? b->GetDuration() : 0;
                return m_ascending ? rA < rB : rA > rB;
            }
        private:
            const bool m_ascending;
    };

    // Binary predicate for sorting Units based on value of distance of an GameObject
    class DistanceCompareOrderPred
    {
        public:
            DistanceCompareOrderPred(const WorldObject* object, bool ascending = true) : m_object(object), m_ascending(ascending) {}
            bool operator() (const WorldObject* a, const WorldObject* b) const
            {
                return m_ascending ? a->GetDistance(m_object) < b->GetDistance(m_object) :
                                     a->GetDistance(m_object) > b->GetDistance(m_object);
            }
        private:
            const WorldObject* m_object;
            const bool m_ascending;
    };

    class DistanceOrderPred
    {
        public:
            DistanceOrderPred(Unit* searcher) :  _searcher(searcher) { }
            bool operator() (const Unit* a, const Unit* b) const
            {
                float rA = _searcher->GetDistance(a);
                float rB = _searcher->GetDistance(b);
                return rA < rB;
            }
        private:
            Unit const* _searcher;
    };
}
#endif
