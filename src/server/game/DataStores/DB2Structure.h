/*
 * Copyright (C) 2011 TrintiyCore <http://www.trinitycore.org/>
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

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"
#include "SharedDefines.h"
#include "ItemPrototype.h"

#include <map>
#include <set>
#include <vector>

/// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct CurvePointEntry
{
    uint32 ID;                                                      ///< 0
    uint32 CurveID;                                                 ///< 1
    uint32 Index;                                                   ///< 2
    float X;                                                        ///< 3
    float Y;                                                        ///< 4
};

/// @issue : #284
struct CurrencyTypesEntry
{
    uint32    ID;                                                   ///< 0      m_ID
    uint32    Category;                                             ///< 1      m_CategoryID
    LocalizedString const*    m_NameLang;                           ///< 2      m_NameLang
    LocalizedString const*    m_InventoryIcon;                      ///< 3      m_InventoryIcon
    LocalizedString const*    m_InventoryIcon2;                     ///< 4      m_InventoryIcon2
    uint32    m_SpellWeight;                                        ///< 5      m_SpellWeight
    uint32    m_SpellCategory;                                      ///< 6      m_SpellCategory
    uint32    TotalCap;                                             ///< 7      m_MaxQty
    uint32    WeekCap;                                              ///< 8      m_MaxEarnablePerWeek
    uint32    Flags;                                                ///< 9      m_Flags
    uint32    m_Quality;                                            ///< 10     m_Quality
    LocalizedString const*    m_DescriptionLang;                    ///< 11     m_DescriptionLang

    bool HasPrecision() const   { return Flags & CURRENCY_FLAG_HIGH_PRECISION; }
    bool HasSeasonCount() const { return Flags & CURRENCY_FLAG_HAS_SEASON_COUNT; }
    float GetPrecision() const  { return HasPrecision() ? 100.0f : 1.0f; }
};

struct GroupFinderActivityEntry
{
    uint32 ID;                                                      ///< 0
    uint32 ActivityGroupID;                                         ///< 1
    int32  Unk1;                                                    ///< 2
    int32  Unk2;                                                    ///< 3
    int32  Unk3;                                                    ///< 4
    int32  Unk4;                                                    ///< 5
    int32  Unk5;                                                    ///< 6
    int32  Unk6;                                                    ///< 7
    int32  Unk7;                                                    ///< 8
    int32  Unk8;                                                    ///< 9
    int32  Unk9;                                                    ///< 10
    int32  Unk10;                                                   ///< 11
    int32  Unk11;                                                   ///< 12
    LocalizedString const* Name;                                    ///< 13
    LocalizedString const* Difficulty;                              ///< 14
};

struct GroupFinderCategoryEntry
{
    uint32 ID;                                                      ///< 0
    LocalizedString const* Name;                                    ///< 1
    int32  Unk1;                                                    ///< 2
    int32  Unk2;                                                    ///< 3
};

struct GlyphRequiredSpecEntry
{
    uint32 ID;                                                      ///< 0
    uint32 GlyphID;                                                 ///< 1
    uint32 SpecID;                                                  ///< 2
};

struct PathNodeEntry
{
    uint32 ID;                                                      ///< 0
    uint32 PathID;                                                  ///< 1
    uint32 Order;                                                   ///< 2
    uint32 LocationID;                                              ///< 3
};

struct LocationEntry
{
    uint32 ID;                                                      ///< 0
    float  X;                                                       ///< 1
    float  Y;                                                       ///< 2
    float  Z;                                                       ///< 3
    float  Unk1;                                                    ///< 4
    float  Unk2;                                                    ///< 5
    float  Unk3;                                                    ///< 6
};

/// Structures used to access raw DB2 data and required packing to portability
struct ItemEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Class;                                                   ///< 1
    uint32 SubClass;                                                ///< 2
    int32  SoundOverrideSubclass;                                   ///< 3
    int32  Material;                                                ///< 4
    uint32 InventoryType;                                           ///< 5
    uint32 Sheath;                                                  ///< 6
    uint32 DisplayId;                                               ///< 7
    uint32 GroupSoundsID;                                           ///< 8
};

struct ItemBonusEntry
{
    ///uint32 ID;                                                   ///< 0
    uint32 Entry;                                                   ///< 1
    uint32 Type;                                                    ///< 2
    uint32 Value[2];                                                ///< 3-4
    uint32 Index;                                                   ///< 5
};

struct ItemBonusTreeNodeEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Category;                                                ///< 1
    uint32 Context;                                                 ///< 2
    uint32 LinkedCategory;                                          ///< 3
    uint32 ItemBonusEntry;                                          ///< 4
};

struct ItemXBonusTreeEntry
{
    uint32 ID;                                                      ///< 0
    uint32 ItemId;                                                  ///< 1
    uint32 ItemBonusTreeCategory;                                   ///< 2
};

struct ItemCurrencyCostEntry
{
    uint32 Id;                                                      ///< 0
    uint32 ItemId;                                                  ///< 1
};

struct ItemSparseEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Quality;                                                 ///< 1
    uint32 Flags;                                                   ///< 2
    uint32 Flags2;                                                  ///< 3
    uint32 Flags3;                                                  ///< 4
    float  Unk430_1;                                                ///< 5
    float  Unk430_2;                                                ///< 6
    uint32 BuyCount;                                                ///< 7
    uint32 BuyPrice;                                                ///< 8
    uint32 SellPrice;                                               ///< 9
    uint32 InventoryType;                                           ///< 10
    int32  AllowableClass;                                          ///< 11
    int32  AllowableRace;                                           ///< 12
    uint32 ItemLevel;                                               ///< 13
    int32  RequiredLevel;                                           ///< 14
    uint32 RequiredSkill;                                           ///< 15
    uint32 RequiredSkillRank;                                       ///< 16
    uint32 RequiredSpell;                                           ///< 17
    uint32 RequiredHonorRank;                                       ///< 18
    uint32 RequiredCityRank;                                        ///< 19
    uint32 RequiredReputationFaction;                               ///< 20
    uint32 RequiredReputationRank;                                  ///< 21
    uint32 MaxCount;                                                ///< 22
    uint32 Stackable;                                               ///< 23
    uint32 ContainerSlots;                                          ///< 24
    int32  ItemStatType[MAX_ITEM_PROTO_STATS];                      ///< 25-34
    int32  ItemStatValue[MAX_ITEM_PROTO_STATS];                     ///< 35-44
    int32  ScalingValue[MAX_ITEM_PROTO_STATS];                      ///< 45-54
    float  SocketCostRate[MAX_ITEM_PROTO_STATS];                    ///< 55-64
    uint32 ScalingStatDistribution;                                 ///< 65
    uint32 DamageType;                                              ///< 66
    uint32 Delay;                                                   ///< 67
    float  RangedModRange;                                          ///< 68
    uint32 Bonding;                                                 ///< 69
    LocalizedString const* Name;                                    ///< 70
    LocalizedString const* Name2;                                   ///< 71
    LocalizedString const* Name3;                                   ///< 72
    LocalizedString const* Name4;                                   ///< 73
    LocalizedString const* Description;                             ///< 74
    uint32 PageText;                                                ///< 75
    uint32 LanguageID;                                              ///< 76
    uint32 PageMaterial;                                            ///< 77
    uint32 StartQuest;                                              ///< 78
    uint32 LockID;                                                  ///< 79
    int32  Material;                                                ///< 80
    uint32 Sheath;                                                  ///< 81
    uint32 RandomProperty;                                          ///< 82
    uint32 RandomSuffix;                                            ///< 83
    uint32 ItemSet;                                                 ///< 84
    uint32 Area;                                                    ///< 85
    uint32 Map;                                                     ///< 86
    uint32 BagFamily;                                               ///< 87
    uint32 TotemCategory;                                           ///< 88
    uint32 Color[MAX_ITEM_PROTO_SOCKETS];                           ///< 89-91
    int32  SocketBonus;                                             ///< 92
    uint32 GemProperties;                                           ///< 93
    float  ArmorDamageModifier;                                     ///< 94
    uint32 Duration;                                                ///< 95
    uint32 ItemLimitCategory;                                       ///< 96
    uint32 HolidayId;                                               ///< 97
    float  StatScalingFactor;                                       ///< 98
    int32  CurrencySubstitutionId;                                  ///< 99
    int32  CurrencySubstitutionCount;                               ///< 100
    int32  ItemNameDescriptionID;                                   ///< 101
};

struct ItemEffectEntry
{
    uint32 ID;                                                      ///< 0
    uint32 ItemID;                                                  ///< 1
    uint32 EffectIndex;                                             ///< 2
    uint32 SpellID;                                                 ///< 3
    uint32 SpellTrigger;                                            ///< 4
    int32  SpellCharge;                                             ///< 5
    uint32 SpellCooldown;                                           ///< 6
    uint32 SpellCategory;                                           ///< 7
    uint32 SpellCategoryCooldown;                                   ///< 8
};

#define MAX_HEIRLOOM_UPGRADE_LEVEL 2

struct HeirloomEntry
{
    uint32 ID;                                                      ///< 0
    uint32 ItemID;                                                  ///< 1
    LocalizedString const* Description[2];                          ///< 2-4    0 = horde, 1 = ally, if 0 is NULL then 1 is for both
    uint32 MaxHeirloomUpgrade;                                      ///< 5
    uint32 OldHeirloomID[2];                                        ///< 6-8
    uint32 HeroicVersion;                                           ///< 9
    uint32 UpgradableByItemID[MAX_HEIRLOOM_UPGRADE_LEVEL];          ///< 10
    uint32 UpgradeIemBonusID[MAX_HEIRLOOM_UPGRADE_LEVEL];           ///< 11
};

struct PvpItemEntry
{
    uint32 RecordID;                                                ///< 0
    uint32 itemId;                                                  ///< 1
    uint32 ilvl;                                                    ///< 2
};

struct ItemUpgradeEntry
{
    uint32 Id;                                                      ///< 0
    uint32 itemUpgradePath;                                         ///< 1
    uint32 itemLevelUpgrade;                                        ///< 2
    uint32 precItemUpgradeId;                                       ///< 3
    uint32 currencyId;                                              ///< 4
    uint32 currencyCost;                                            ///< 5
};

struct ItemModifiedAppearanceEntry
{
    uint32 ID;                                                      ///< 0
    uint32 ItemID;                                                  ///< 1
    uint32 AppearanceModID;                                         ///< 2
    uint32 AppearanceID;                                            ///< 3
    uint32 IconFileDataID;                                          ///< 4
    uint32 Index;                                                   ///< 5
};

struct ItemAppearanceEntry
{
    uint32 ID;                                                      ///< 0
    uint32 DisplayID;                                               ///< 1
    uint32 IconFileDataID;                                          ///< 2
};

struct RulesetItemUpgradeEntry
{
    uint32 Id;                                                      ///< 0
    uint32 unk;                                                     ///< 1
    uint32 itemUpgradeId;                                           ///< 2
    uint32 itemid;                                                  ///< 3
};

#define MAX_ITEM_EXT_COST_ITEMS         5
#define MAX_ITEM_EXT_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32 ID;                                                      ///< 0      extended-cost entry id
    uint32 RequiredArenaSlot;                                       ///< 1      arena slot restrictions (min slot value)
    uint32 RequiredItem[MAX_ITEM_EXT_COST_ITEMS];                   ///< 2-6    required item id
    uint32 RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS];              ///< 7-11   required count of 1st item
    uint32 RequiredPersonalArenaRating;                             ///< 12     required personal arena rating
    uint32 ItemPurchaseGroup;                                       ///< 13     only 0
    uint32 RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];          ///< 14-18  required curency id
    uint32 RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];     ///< 19-23  required curency count
    uint32 RequiredFactionId;                                       ///< 24
    uint32 RequiredFactionStanding;                                 ///< 25
    uint32 RequirementFlags;                                        ///< 26
    uint32 RequiredAchievement;                                     ///< 27
    uint32 OverrideBuyPrice;                                        ///< 28
};

struct QuestPackageItemEntry
{
    uint32 ID;                                                      ///< 0
    uint32 PackageID;                                               ///< 1
    uint32 ItemId;                                                  ///< 2
    uint32 Count;                                                   ///< 3
    uint32 Type;                                                    ///< 4
};

enum class PackageItemRewardType : uint8
{
    SpecializationReward = 0,
    ClassReward          = 1,
    DefaultHiddenReward  = 2,
    NoRequire            = 3
};

struct MapChallengeModeEntry
{
    uint32 ID;                                                      ///< 0
    uint32 MapID;                                                   ///< 1
    uint32 Field2;                                                  ///< 2
    uint32 Field3;                                                  ///< 3
    uint32 Field4;                                                  ///< 4
    uint32 BronzeTime;                                              ///< 5
    uint32 SilverTime;                                              ///< 6
    uint32 GoldTime;                                                ///< 7
    uint32 Field8;                                                  ///< 8
    uint32 Field9;                                                  ///< 9
};

struct SceneScriptEntry
{
    uint32 Entry;                                                   ///< 0
    LocalizedString const* Name;                                    ///< 1
    LocalizedString const* Script;                                  ///< 2
    uint32 PrevScript;                                              ///< 3
    uint32 NextScript;                                              ///< 4
};

struct SceneScriptPackageEntry
{
    uint32 Entry;                                                   ///< 0
    LocalizedString const* Name;                                    ///< 1
};

struct SoundEntriesEntry
{
    uint32 Id;                                                      ///< 0      m_ID
    uint32 Type;                                                    ///< 1      m_SoundType
    LocalizedString const* InternalName;                            ///< 2      m_Name
    uint32 m_FileDataID[20];                                        ///< 3-22   m_FileDataID
    uint32 m_Freq[20];                                              ///< 23-42  m_Freq
    float  m_volumeFloat;                                           ///< 43     m_VolumeFloat
    uint32 m_Flags;                                                 ///< 44     m_Flags
    float  m_MinDistance;                                           ///< 45     m_MinDistance
    float  m_DistanceCutoff;                                        ///< 46     m_DistanceCutoff
    uint32 m_EAXDef;                                                ///< 47     m_EAXDef
    uint32 m_SoundEntriesAdvancedID;                                ///< 48     m_SoundEntriesAdvancedID
    float  m_Volumevariationplus;                                   ///< 49     m_Volumevariationplus
    float  m_Volumevariationminus;                                  ///< 50     m_Volumevariationminus
    float  m_Pitchvariationplus;                                    ///< 51     m_Pitchvariationplus
    float  m_Pitchvariationminus;                                   ///< 52     m_Pitchvariationminus
    float  m_PitchAdjust;                                           ///< 53     m_PitchAdjust
    uint32 m_Dialogtype;                                            ///< 54     m_Dialogtype
    uint32 m_BusOverwriteID;                                        ///< 55     m_BusOverwriteID
};

#define MAX_SPELL_REAGENTS 8

struct SpellReagentsEntry
{
    ///uint32    Id;                                                ///< 0      m_ID
    int32  Reagent[MAX_SPELL_REAGENTS];                             ///< 1-8    m_reagent
    uint32 ReagentCount[MAX_SPELL_REAGENTS];                        ///< 9-16   m_reagentCount
    uint32 CurrencyID;                                              ///< 17     m_CurrencyID
    uint32 CurrencyCount;                                           ///< 18     m_CurrencyCount
};

struct SpellReagent
{
    SpellReagent()
    {
        reagents[0] = NULL;
        reagents[1] = NULL;
        reagents[2] = NULL;
        reagents[3] = NULL;
        reagents[4] = NULL;
        reagents[5] = NULL;
        reagents[6] = NULL;
        reagents[7] = NULL;
    }
    SpellReagentsEntry const* reagents[MAX_SPELL_REAGENTS];
};

typedef std::map<uint32, SpellReagent> SpellReagentMap;

struct TaxiPathNodeEntry
{
    uint32 ID;                                                      ///< 0      m_ID
    uint32 path;                                                    ///< 1      m_PathID
    uint32 index;                                                   ///< 2      m_NodeIndex
    uint32 mapid;                                                   ///< 3      m_ContinentID
    float  x;                                                       ///< 4      m_LocX
    float  y;                                                       ///< 5      m_LocY
    float  z;                                                       ///< 6      m_LocZ
    uint32 actionFlag;                                              ///< 7      m_flags
    uint32 delay;                                                   ///< 8      m_delay
    uint32 arrivalEventID;                                          ///< 9      m_arrivalEventID
    uint32 departureEventID;                                        ///< 10     m_departureEventID
};

struct TaxiNodesEntry
{
    uint32 ID;                                                      ///< 0
    uint32 map_id;                                                  ///< 1
    float  x;                                                       ///< 2
    float  y;                                                       ///< 3
    float  z;                                                       ///< 4
    LocalizedString const* name;                                    ///< 5
    uint32 MountCreatureID[2];                                      ///< 6-7
    uint32 m_ConditionID;                                           ///< 8
    uint32 LearnableIndex;                                          ///< 9      some kind of index only for learnable nodes
    uint32 m_Flags;                                                 ///< 10
    float  m_MapOffsetX;                                            ///< 11
    float  m_MapOffsetY;                                            ///< 12
};

struct SpellRuneCostEntry
{
    uint32 ID;                                                      ///< 0
    uint32 RuneCost[4];                                             ///< 1-3    0=blood, 1=frost, 2=unholy, 3=unk
    uint32 runePowerGain;                                           ///< 4

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return runePowerGain == 0; }
};

struct SpellCastingRequirementsEntry
{
    uint32 Id;                                                      ///< 0      m_ID
    uint32 FacingCasterFlags;                                       ///< 1      m_facingCasterFlags
    uint32 MinFactionId;                                            ///< 2      m_minFactionID not used
    uint32 MinReputation;                                           ///< 3      m_minReputation not used
    int32  AreaGroupId;                                             ///< 4      m_requiredAreaGroupId
    uint32 RequiredAuraVision;                                      ///< 5      m_requiredAuraVision not used
    uint32 RequiresSpellFocus;                                      ///< 6      m_requiresSpellFocus
};

struct SpellAuraRestrictionsEntry
{
    uint32 Id;                                                      ///< 0      m_ID
    uint32 CasterAuraState;                                         ///< 1      Flag
    uint32 TargetAuraState;                                         ///< 2      Flag
    uint32 CasterAuraStateNot;                                      ///< 3      m_excludeCasterAuraState
    uint32 TargetAuraStateNot;                                      ///< 4      m_excludeTargetAuraState
    uint32 casterAuraSpell;                                         ///< 5      m_casterAuraSpell
    uint32 targetAuraSpell;                                         ///< 6      m_targetAuraSpell
    uint32 excludeCasterAuraSpell;                                  ///< 7      m_excludeCasterAuraSpell
    uint32 excludeTargetAuraSpell;                                  ///< 8      m_excludeTargetAuraSpell
};

struct AreaPOIEntry
{
    uint32 id;                                                      ///< 0
    uint32 m_Importance;                                            ///< 1 
    uint32 unk;                                                     ///< 2
    uint32 unk2;                                                    ///< 3
    uint32 mapId;                                                   ///< 4      m_ContinentID
    uint32 zoneId;                                                  ///< 5      m_AreaID
    uint32 unk3;                                                    ///< 6
    float x;                                                        ///< 7      m_Pos
    float y;                                                        ///< 8      m_Pos
    LocalizedString const*m_NameLang;                               ///< 9      m_NameLang
    LocalizedString const*m_DescriptionLang;                        ///< 10     m_DescriptionLang
    uint32 worldState;                                              ///< 11     m_WorldStateID
    uint32 m_PlayerConditionID;                                     ///< 12     @TODO
    uint32 unk4;                                                    ///< 13
    uint32 unk5;                                                    ///< 14
};

#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 16
#define MAX_HOLIDAY_FLAGS 10

struct HolidaysEntry
{
    uint32 Id;                                                      ///< 0      m_ID
    uint32 Duration[MAX_HOLIDAY_DURATIONS];                         ///< 1-10   m_duration
    uint32 Date[MAX_HOLIDAY_DATES];                                 ///< 11-26  m_date (dates in unix time starting at January, 1, 2000)
    uint32 Region;                                                  ///< 27     m_region (wow region)
    uint32 Looping;                                                 ///< 28     m_looping
    uint32 CalendarFlags[MAX_HOLIDAY_FLAGS];                        ///< 29-38  m_calendarFlags
    uint32 holidayNameId;                                           ///< 39     m_holidayNameID (HolidayNames.dbc)
    uint32 holidayDescriptionId;                                    ///< 40     m_holidayDescriptionID (HolidayDescriptions.dbc)
    uint32 Priority;                                                ///< 41     m_priority
    LocalizedString const* TextureFilename;                         ///< 42     m_textureFilename
    int32  CalendarFilterType;                                      ///< 43     m_calendarFilterType (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    uint32 flags;                                                   ///< 44     m_flags (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

#define MAX_OVERRIDE_SPELL 10

struct OverrideSpellDataEntry
{
    uint32 id;                                                      ///< 0
    uint32 spellId[MAX_OVERRIDE_SPELL];                             ///< 1-10
    uint32 Flags;                                                   ///< 11
    uint32 PlayerActionbarFileDataID;                               ///< 12
};

struct SpellEffectGroupSizeEntry
{
    uint32 Id;                                                      ///< 0
    uint32 SpellEffectID;                                           ///< 1
    float  Coefficient;                                             ///< 2
};

struct SpellMiscEntry
{
    uint32 Id;                                                      ///< 0      m_ID
    uint32 Attributes;                                              ///< 1      m_attribute
    uint32 AttributesEx;                                            ///< 2      m_attributesEx
    uint32 AttributesEx2;                                           ///< 3      m_attributesExB
    uint32 AttributesEx3;                                           ///< 4      m_attributesExC
    uint32 AttributesEx4;                                           ///< 5      m_attributesExD
    uint32 AttributesEx5;                                           ///< 6      m_attributesExE
    uint32 AttributesEx6;                                           ///< 7      m_attributesExF
    uint32 AttributesEx7;                                           ///< 8      m_attributesExG (0x20 - totems, 0x4 - paladin auras, etc...)
    uint32 AttributesEx8;                                           ///< 9      m_attributesExH
    uint32 AttributesEx9;                                           ///< 10     m_attributesExI
    uint32 AttributesEx10;                                          ///< 11     m_attributesExJ
    uint32 AttributesEx11;                                          ///< 12     m_attributesExK
    uint32 AttributesEx12;                                          ///< 13     m_attributesExL
    uint32 AttributesEx13;                                          ///< 14     m_attributesExM
    uint32 CastingTimeIndex;                                        ///< 15     m_castingTimeIndex
    uint32 DurationIndex;                                           ///< 16     m_durationIndex
    uint32 rangeIndex;                                              ///< 17     m_rangeIndex
    float  speed;                                                   ///< 18     m_speed
    uint32 SpellVisual[2];                                          ///< 19-20  m_spellVisualID
    uint32 SpellIconID;                                             ///< 21     m_spellIconID
    uint32 activeIconID;                                            ///< 22     m_activeIconID
    uint32 SchoolMask;                                              ///< 23     m_schoolMask
    float  MultistrikeSpeedMod;                                     ///< 24     @TODO
};

struct SpellPowerEntry
{
    uint32 Id;                                                      ///< 0
    uint32 SpellId;                                                 ///< 1
    uint32 PowerIndex;                                              ///< 2
    uint32 PowerType;                                               ///< 3
    uint32 Cost;                                                    ///< 4      ManaCost
    uint32 CostPerlevel;                                            ///< 5      ManaCostPerLevel
    uint32 CostPerSecond;                                           ///< 6      ManaCostPerSecond
    uint32 ManaCostAdditional;                                      ///< 7      Spell uses [ManaCost, ManaCost+ManaCostAdditional] power - affects tooltip parsing as multiplier on SpellEffectEntry::EffectPointsPerResource
                                                                    ///<        only SPELL_EFFECT_WEAPON_DAMAGE_NOSCHOOL, SPELL_EFFECT_WEAPON_PERCENT_DAMAGE, SPELL_EFFECT_WEAPON_DAMAGE, SPELL_EFFECT_NORMALIZED_WEAPON_DMG
    uint32 PowerDisplayID;                                          ///< 8
    uint32 UnitPowerBarID;                                          ///< 9
    float  CostBasePercentage;                                      ///< 10     ManaCostPercentage
    float  CostPerSecondPercentage;                                 ///< 11     ManaCostPercentagePerSecond
    uint32 RequiredAuraSpellId;                                     ///< 12     RequiredAura
    float  CostMaxPercentage;                                       ///< 13     HealthCostPercentage
};

#define MAX_SPELL_TOTEMS            2

struct SpellTotemsEntry
{
    uint32 Id;                                                      ///< 0      m_ID
    uint32 TotemCategory[MAX_SPELL_TOTEMS];                         ///< 1      m_requiredTotemCategoryID
    uint32 Totem[MAX_SPELL_TOTEMS];                                 ///< 2      m_totem
};

struct SpellTotem
{
    SpellTotem()
    {
        totems[0] = NULL;
        totems[1] = NULL;
    }
    SpellTotemsEntry const* totems[MAX_SPELL_TOTEMS];
};

typedef std::map<uint32, SpellTotem> SpellTotemMap;

struct SpellClassOptionsEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  modalNextSpell;                                         ///< 1      m_modalNextSpell not used
    flag128 SpellFamilyFlags;                                       ///< 2-5
    uint32  SpellFamilyName;                                        ///< 6      m_spellClassSet
};

////////////////////////////////////////////////////////////////////
/// Garrison DB2
////////////////////////////////////////////////////////////////////

struct GarrSiteLevelEntry
{
    uint32 SiteLevelID;                                             ///< 0
    uint32 Level;                                                   ///< 1
    uint32 MapID;                                                   ///< 2
    uint32 SiteID;                                                  ///< 3
    uint32 MapTextureUiTextureKitID;                                ///< 4
    float  TownHallX;                                               ///< 5
    float  TownHallY;                                               ///< 6
    uint32 CreationMovie;                                           ///< 7
    uint32 Unk5;                                                    ///< 8
    uint32 UpgradeCost;                                             ///< 9
    uint32 UpgradeMoneyCost;                                        ///< 10
};

struct GarrSiteLevelPlotInstEntry
{
    uint32 ID;                                                      ///< 0
    uint32 SiteLevelID;                                             ///< 1
    uint32 PlotInstanceID;                                          ///< 2
    float  MinimapX;                                                ///< 3
    float  MinimapY;                                                ///< 4
    uint32 Unk1;                                                    ///< 5
};

struct GarrPlotInstanceEntry
{
    uint32 InstanceID;                                              ///< 0
    uint32 PlotID;                                                  ///< 1
    LocalizedString const* Name;                                    ///< 2
};

struct GarrPlotEntry
{
    uint32 PlotID;                                                  ///< 0
    uint32 PlotUiCategoryID;                                        ///< 1
    uint32 PlotType;                                                ///< 2
    uint32 Unk2;                                                    ///< 3      alway 0 (6.1.2)
    LocalizedString const* Name;                                    ///< 4
    uint32 Unk4;                                                    ///< 5
    uint32 Unk5;                                                    ///< 6
    uint32 BuildingGameObject[2];                                   ///< 7-8
};

struct GarrPlotUICategoryEntry
{
    uint32 PlotUiCategoryID;                                        ///< 0
    LocalizedString const* Name;                                    ///< 1
    uint32 Type;                                                    ///< 2
};

struct GarrMissionEntry
{
    uint32 MissionRecID;                                            ///< 0
    int32  RequiredLevel;                                           ///< 1
    int32  RequiredItemLevel;                                       ///< 2
    uint32 GarrMechanicTypeRecID;                                   ///< 3
    uint32 RequiredFollowersCount;                                  ///< 4
    uint32 TravelTime;                                              ///< 5
    uint32 Duration;                                                ///< 6
    uint32 OfferTime;                                               ///< 7
    uint32 LocPrefixID;                                             ///< 8
    uint32 Category;                                                ///< 9      @Todo this is the mission categorie see http://www.wowhead.com/missions=35
    LocalizedString const* Name;                                    ///< 10
    LocalizedString const* Description;                             ///< 11
    LocalizedString const* Location;                                ///< 12
    uint32 SubCategory2;                                            ///< 13
    uint32 SubCategory1;                                            ///< 14
    uint32 GarrisonCurrencyStartCost;                               ///< 15
    uint32 Flags;                                                   ///< 16
    uint32 RewardFollowerExperience;                                ///< 17
    uint32 BaseBronzeChestChance;                                   ///< 18
};

struct GarrMissionRewardEntry
{
    uint32 MissionRewardID;                                         ///< 0
    uint32 MissionID;                                               ///< 1
    uint32 BonusRewardXP;                                           ///< 2
    uint32 ItemID;                                                  ///< 3
    uint32 ItemQuantity;                                            ///< 4
    uint32 RewardCurrencyID;                                        ///< 5
    uint32 RewardCurrencyAmount;                                    ///< 6
    uint32 Unk1;                                                    ///< 7
    uint32 Unk2;                                                    ///< 8
    uint32 Unk3;                                                    ///< 9
};

struct GarrMissionXEncouterEntry
{
    uint32 ID;                                                      ///< 0
    uint32 MissionID;                                               ///< 1
    uint32 EncounterID;                                             ///< 2
};

struct GarrBuildingEntry
{
    uint32 BuildingID;                                              ///< 0
    uint32 GameObjects[2];                                          ///< 1 - 2
    uint32 Unk2;                                                    ///< 3
    uint32 BuildingType;                                            ///< 4
    uint32 BuildingLevel;                                           ///< 5
    LocalizedString const* NameA;                                   ///< 6
    LocalizedString const* NameH;                                   ///< 7
    LocalizedString const* Brief;                                   ///< 8
    LocalizedString const* Description;                             ///< 9
    uint32 BuildTime;                                               ///< 10
    uint32 BuildCostCurrencyID;                                     ///< 11
    uint32 BuildCostCurrencyAmount;                                 ///< 12
    uint32 TexturePrefixA;                                          ///< 13
    uint32 TexturePrefixH;                                          ///< 14
    uint32 IconID;                                                  ///< 15
    uint32 Unk7;                                                    ///< 16     slot number ex : Menagerie Attracts elite pets you can battle for rewards. Also lets 5 of your battle pets hang out in your garrison.
    uint32 BuildingCategory;                                        ///< 17
    uint32 BuiltScene[2];                                           ///< 18 - 19
    uint32 BonusAmount;                                             ///< 20
    uint32 Unk12;                                                   ///< 21     GarrAbilityID http://www.wowhead.com/building=27/barracks#rel-garrisonabilities
    uint32 PassiveEffect;                                           ///< 22
    uint32 MoneyCost;                                               ///< 23
};

struct GarrPlotBuildingEntry
{
    uint32 ID;                                                      ///< 0
    uint32 PlotId;                                                  ///< 1
    uint32 BuildingID;                                              ///< 2
};

struct GarrFollowerEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Entry[2];                                                ///< 1 - 2
    uint32 Class[2];                                                ///< 3 - 4
    uint32 Quality;                                                 ///< 5
    uint32 HordeGarrClassSecID;                                     ///< 6      GarrClassSpec.db2
    uint32 AllianceGarrClassSecID;                                  ///< 7
    uint32 HordeGarrFollItemSetID;                                  ///< 8
    uint32 AllianceGarrFollItemSetID;                               ///< 9
    int32  Level;                                                   ///< 10
    int32  ItemLevelWeapon;                                         ///< 11
    int32  ItemLevelArmor;                                          ///< 12
    uint32 Unk8;                                                    ///< 13
    uint32 Flags;                                                   ///< 14
    char * HordeSourceText;                                         ///< 15     Descr1 : zoneH (where you find this follower)
    char * AllinaceSourceText;                                      ///< 16     Descr2 : zoneA (where you find this follower)
    int32  Unk10;                                                   ///< 17     H
    int32  Unk11;                                                   ///< 18     A
    uint32 HordePortraitIconID;                                     ///< 19
    uint32 AlliancePortraitIconID;                                  ///< 20
};

struct GarrAbilityEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Flags;                                                   ///< 1
    char * Name;                                                    ///< 2
    char * Description;                                             ///< 3
    uint32 IconID;                                                  ///< 4
    uint32 OtherfactionGarrAbilityID;                               ///< 5
    uint32 Category;                                                ///< 6
};

struct GarrAbilityEffectEntry
{
    uint32 EffectID;                                                ///< 0
    uint32 EffectType;                                              ///< 1
    uint32 AbilityID;                                               ///< 2
    uint32 TargetMask;                                              ///< 3
    uint32 CounterMechanicTypeID;                                   ///< 4
    uint32 Unk3;                                                    ///< 5
    float  ModMin;                                                  ///< 6
    float  ModMax;                                                  ///< 7
    float  Amount;                                                  ///< 8
    uint32 MiscValueA;                                              ///< 9
    uint32 MiscValueB;                                              ///< 10
};

struct GarrFollowerXAbilityEntry
{
    uint32 ID;                                                      ///< 0
    uint32 FollowerID;                                              ///< 1
    uint32 AbilityID;                                               ///< 2
    uint32 FactionIndex;                                            ///< 3
};

struct GarrBuildingPlotInstEntry
{
    uint32 ID;                                                      ///< 0
    uint32 BuildingID;                                              ///< 1
    uint32 UIAtlasTextureMemberID;                                  ///< 2
    uint32 SiteLevelPlotInstID;                                     ///< 3
    uint32 MinimapX;                                                ///< 4
    uint32 MinimapY;                                                ///< 5
};

struct GarrMechanicEntry
{
    uint32 ID;                                                      ///< 0
    uint32 MechanicTypeID;                                          ///< 1
    float  Unk2;                                                    ///< 2
};

struct GarrMechanicTypeEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Type;                                                    ///< 1
    LocalizedString const* Environment;                             ///< 2
    LocalizedString const* EnvironmentDesc;                         ///< 3
    uint32 EnvironmentTextureID;                                    ///< 4
};

struct GarrEncouterXMechanicEntry
{
    uint32 ID;                                                      ///< 0
    uint32 EncounterID;                                             ///< 1
    uint32 MechanicID;                                              ///< 2
};

struct GarrFollowerLevelXPEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Level;                                                   ///< 1
    uint32 RequiredExperience;                                      ///< 2
};

struct GarrSpecializationEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Unk1;                                                    ///< 1      IconID
    uint32 Unk2;                                                    ///< 2
    uint32 Unk3;                                                    ///< 3
    uint32 Unk4;                                                    ///< 4
    float  BasePoint;                                               ///< 5
    uint32 Unk6;                                                    ///< 6
    LocalizedString const* Name;                                    ///< 7
    LocalizedString const* Description;                             ///< 8
};

namespace CharShipmentFlags
{
    enum
    {
        Quest       = 0x1,
        Internal    = 0x8
    };
}

struct CharShipmentEntry
{
    uint32 ID;                                                      ///< 0
    uint32 ShipmentContainerID;                                     ///< 1
    uint32 Unk1;                                                    ///< 2
    uint32 Flags;                                                   ///< 3
    uint32 Duration;                                                ///< 4
    uint32 SpellID;                                                 ///< 5
    uint32 ResultItemID;                                            ///< 6
};

struct CharShipmentContainerEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Unk1;                                                    ///< 1
    uint32 BuildingType;                                            ///< 2
    LocalizedString const* Name;                                    ///< 3
    uint32 TextureKitID;                                            ///< 4
    uint32 Unk2;                                                    ///< 5
    uint32 Unk3;                                                    ///< 6
    uint32 Unk4;                                                    ///< 7
    uint32 Unk5;                                                    ///< 8
    uint32 Unk6;                                                    ///< 9
    LocalizedString const* Description;                             ///< 10
    uint32 Unk7;                                                    ///< 11
    uint32 Unk8;                                                    ///< 12
};

#define MAX_BATTLEPET_PROPERTIES 6

struct BattlePetAbilityEntry
{
    uint32 id;                                                      ///< 0
    uint32 petType;                                                 ///< 1
    uint32 family;                                                  ///< 2
    uint32 cooldown;                                                ///< 3
    uint32 visualId;                                                ///< 4      BattlePetVisual.dbc
    uint32 flags;                                                   ///< 5
    const char * name;                                              ///< 6
    const char * description;                                       ///< 7
};

struct BattlePetAbilityEffectEntry
{
    uint32 id;                                                      ///< 0
    uint32 abilityTurnId;                                           ///< 1      BattlePetAbilityTurn.dbc
    uint32 visualId;                                                ///< 2      BattlePetVisual.dbc
    uint32 triggerAbility;                                          ///< 3
    uint32 effect;                                                  ///< 4
    uint32 effectIndex;                                             ///< 5
    int32  prop[MAX_BATTLEPET_PROPERTIES];                          ///< 6-12   See BattlePetEffectProperties.dbc with effect
};

struct BattlePetAbilityTurnEntry
{
    uint32 id;                                                      ///< 0
    uint32 abilityId;                                               ///< 1      BattlePetAbility.dbc
    uint32 visualId;                                                ///< 2      BattlePetVisual.dbc
    uint32 turn;                                                    ///< 3
    uint32 hasProcType;                                             ///< 4      if 1 then value <> -1
    int32  procType;                                                ///< 5
};

struct BattlePetAbilityStateEntry
{
    uint32 id;                                                      ///< 0
    uint32 abilityId;                                               ///< 1      BattlePetAbility.dbc
    uint32 stateId;                                                 ///< 2      BattlePetState.dbc
    int32  value;                                                   ///< 3
};

struct BattlePetStateEntry
{
    uint32 id;                                                      ///< 0
    uint32 parent;                                                  ///< 1      BattlePetState.dbc
    LocalizedString const* name;                                    ///< 3
    uint32 flags;                                                   ///< 4
};

enum BattlePetEffectFlags
{
    BATTLEPET_EFFECT_FLAG_POSITIVE = 1 << 0,
    BATTLEPET_EFFECT_FLAG_NEGATIVE = 1 << 1,
    /*
    BATTLEPET_EFFECT_CATEGORY_HEAL = 37,
    BATTLEPET_EFFECT_CATEGORY_DAMAGE = 38,
    BATTLEPET_EFFECT_CATEGORY_PERIODIC_POSITIVE_BUFF = 47,
    BATTLEPET_EFFECT_CATEGORY_POSITIVE_BUFF = 49,
    BATTLEPET_EFFECT_CATEGORY_CONTROL_BUFF = 50,
    BATTLEPET_EFFECT_CATEGORY_PERIODIC_NEGATIVE_BUFF = 51,
    BATTLEPET_EFFECT_CATEGORY_NEGATIVE_BUFF = 94,
    */
};

enum BattlePetEffectCategory
{
    BATTLEPET_EFFECT_CATEGORY_DEAL = 9,
    BATTLEPET_EFFECT_CATEGORY_PERIODIC = 11,
    BATTLEPET_EFFECT_CATEGORY_BUFF = 12,
    BATTLEPET_EFFECT_CATEGORY_AURA = 23,
};

struct BattlePetEffectPropertiesEntry
{
    uint32 effect;                                                  ///< 0
    uint32 flags;                                                   ///< 1
    LocalizedString const* propName[MAX_BATTLEPET_PROPERTIES];      ///< 2-8
    uint32 propIsId[MAX_BATTLEPET_PROPERTIES];                      ///< 9-15   Only set to 1 for AuraID
};

struct BattlePetBreedQualityEntry
{
    uint32 id;                                                      ///< 0
    uint32 quality;                                                 ///< 1
    float factor;                                                   ///< 2
};

struct BattlePetBreedStateEntry
{
    uint32 id;                                                      ///< 0
    uint32 breed;                                                   ///< 1
    uint32 stateId;                                                 ///< 2
    int32 value;                                                    ///< 3
};

enum BattlePetSpeciesFlags
{
    BATTLEPET_SPECIES_FLAG_CONDITIONAL  = 0x002,
    BATTLEPET_SPECIES_FLAG_CAGEABLE     = 0x010,
    BATTLEPET_SPECIES_FLAG_UNTAMEABLE   = 0x020,
    BATTLEPET_SPECIES_FLAG_UNIQUE       = 0x040,
    BATTLEPET_SPECIES_FLAG_COMPANION    = 0x080,
    BATTLEPET_SPECIES_FLAG_ELITE        = 0x400,
};

struct BattlePetSpeciesEntry
{
    uint32 id;                                                      ///< 0
    uint32 entry;                                                   ///< 1
    uint32 iconId;                                                  ///< 2
    uint32 spellId;                                                 ///< 3
    uint32 type;                                                    ///< 4
    uint32 obtainmentCategoryDescription;                           ///< 5
    uint32 flags;                                                   ///< 6
    LocalizedString const* source;                                  ///< 7
    LocalizedString const* description;                             ///< 8
};

struct BattlePetSpeciesStateEntry
{
    uint32 id;                                                      ///< 0
    uint32 speciesId;                                               ///< 1
    uint32 stateId;                                                 ///< 2
    int32  value;                                                   ///< 3
};

struct BattlePetSpeciesXAbilityEntry
{
    uint32 id;                                                      ///< 0
    uint32 speciesId;                                               ///< 1
    uint32 abilityId;                                               ///< 2
    uint32 level;                                                   ///< 3
    uint32 tier;                                                    ///< 4
};

struct MountEntry
{
    uint32 Id;                                                      ///< 0
    uint32 Category;                                                ///< 2
    uint32 CreatureDisplayID;                                       ///< 3
    uint32 Flags;                                                   ///< 4
    LocalizedString const* Name;                                    ///< 5
    LocalizedString const* Description;                             ///< 6
    LocalizedString const* Icon;                                    ///< 7
    int32  FilterCategory;                                          ///< 8      Used for filter un Mount page in-game
    uint32 SpellID;                                                 ///< 9
    uint32 PlayerConditionID;                                       ///< 10
};

/// PlayerCondition.db2
struct PlayerConditionEntry
{
    int32  ID;                                                      ///< 0
    int32  Flags;                                                   ///< 1
    int32  MinLevel;                                                ///< 2
    int32  MaxLevel;                                                ///< 3
    int32  RaceMask;                                                ///< 4
    int32  ClassMask;                                               ///< 5
    int32  Gender;                                                  ///< 6
    int32  NativeGender;                                            ///< 7
    int32  SkillID[4];                                              ///< 8-11
    int32  MinSkill[4];                                             ///< 12-15
    int32  MaxSkill[4];                                             ///< 16-19
    int32  SkillLogic;                                              ///< 20
    int32  LanguageID;                                              ///< 21
    int32  MinLanguage;                                             ///< 22
    int32  MaxLanguage;                                             ///< 23
    int32  MinFactionID[3];                                         ///< 24-26
    int32  MaxFactionID;                                            ///< 27
    int32  MinReputation[3];                                        ///< 28-30
    int32  MaxReputation;                                           ///< 31
    int32  ReputationLogic;                                         ///< 32
    int32  MinPVPRank;                                              ///< 33
    int32  MaxPVPRank;                                              ///< 34
    int32  PvpMedal;                                                ///< 35
    int32  PrevQuestLogic;                                          ///< 36
    int32  PrevQuestID[4];                                          ///< 37-40
    int32  CurrQuestLogic;                                          ///< 41
    int32  CurrQuestID[4];                                          ///< 42-45
    int32  CurrentCompletedQuestLogic;                              ///< 46
    int32  CurrentCompletedQuestID[4];                              ///< 47-50
    int32  SpellLogic;                                              ///< 51
    int32  SpellID[4];                                              ///< 52-55
    int32  ItemLogic;                                               ///< 56
    int32  ItemID[4];                                               ///< 57-60
    int32  ItemCount[4];                                            ///< 61-64
    int32  ItemFlags;                                               ///< 65
    int32  Explored[2];                                             ///< 66-67
    int32  Time[2];                                                 ///< 68-69
    int32  AuraSpellLogic;                                          ///< 70
    int32  AuraSpellID[4];                                          ///< 71-74
    int32  WorldStateExpressionID;                                  ///< 75
    int32  WeatherID;                                               ///< 76
    int32  PartyStatus;                                             ///< 77
    int32  LifetimeMaxPVPRank;                                      ///< 78
    int32  AchievementLogic;                                        ///< 79
    int32  Achievement[4];                                          ///< 80-83
    int32  LfgLogic;                                                ///< 84
    int32  LfgStatus[4];                                            ///< 85-88
    int32  LfgCompare[4];                                           ///< 89-92
    int32  LfgValue[4];                                             ///< 93-96
    int32  AreaLogic;                                               ///< 97
    int32  AreaID[4];                                               ///< 98-101
    int32  CurrencyLogic;                                           ///< 102
    int32  CurrencyID[4];                                           ///< 103-106
    int32  CurrencyCount[4];                                        ///< 107-110
    int32  QuestKillID;                                             ///< 111
    int32  QuestKillLogic;                                          ///< 112
    int32  QuestKillMonster[4];                                     ///< 113-116
    int32  MinExpansionLevel;                                       ///< 117
    int32  MaxExpansionLevel;                                       ///< 118
    int32  MinExpansionTier;                                        ///< 119
    int32  MaxExpansionTier;                                        ///< 120
    int32  MinGuildLevel;                                           ///< 121
    int32  MaxGuildLevel;                                           ///< 122
    int32  PhaseUseFlags;                                           ///< 123
    int32  PhaseID;                                                 ///< 124
    int32  PhaseGroupID;                                            ///< 125
    int32  MinAvgItemLevel;                                         ///< 126
    int32  MaxAvgItemLevel;                                         ///< 127
    int32  MinAvgEquippedItemLevel;                                 ///< 128
    int32  MaxAvgEquippedItemLevel;                                 ///< 129
    int32  ChrSpecializationIndex;                                  ///< 130
    int32  ChrSpecializationRole;                                   ///< 131
    LocalizedString const* FailureDescriptionLang;                  ///< 132
    int32  PowerType;                                               ///< 133
    int32  PowerTypeComp;                                           ///< 134
    int32  PowerTypeValue;                                          ///< 135
};

/// Flags of PrevQuestLogic (PlayerConditionEntry)
namespace PrevQuestLogicFlags
{
    enum
    {
        TrackingQuest = 0x10000
    };
}

struct WbAccessControlListEntry
{
    uint32 ID;
    char*  Url;
    uint32 AccessFlags;
    uint32 Unk1;
    uint32 Restricted;
    uint32 Region;
};

struct WbCertWhitelistEntry
{
    uint32 ID;
    char*  Url;
    uint8  Unk1;
    uint8  Unk2;
    uint32 Unk3;
};

struct VignetteEntry
{
    uint32 Id;                                                      ///< 0
    LocalizedString const* Name;                                    ///< 1
    uint32 QuestFeedbackEffectId;                                   ///< 2
    uint32 Flags;                                                   ///< 3
    float  X;                                                       ///< 4
    float  Y;                                                       ///< 5
};

/// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

#endif
