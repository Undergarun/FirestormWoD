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
//#include "Vehicle.h"
#include "SharedDefines.h"
#include "ItemPrototype.h"

#include <map>
#include <set>
#include <vector>

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

struct CurvePointEntry
{
    uint32 ID;                                                      // 0
    uint32 CurveID;                                                 // 1
    uint32 Index;                                                   // 2
    float X;                                                        // 3
    float Y;                                                        // 4
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
    float X;                                                        ///< 1
    float Y;                                                        ///< 2
    float Z;                                                        ///< 3
    float Unk1;                                                     ///< 4
    float Unk2;                                                     ///< 5
    float Unk3;                                                     ///< 6
};

// Structures used to access raw DB2 data and required packing to portability
struct ItemEntry
{
    uint32   ID;                                             // 0
    uint32   Class;                                          // 1
    uint32   SubClass;                                       // 2
    int32    SoundOverrideSubclass;                          // 3
    int32    Material;                                       // 4
    uint32   InventoryType;                                  // 5
    uint32   Sheath;                                         // 6
    uint32   DisplayId;                                      // 7
    uint32   unk;                                            // 8
};

struct ItemBonusEntry
{
    // ID;
    uint32 Entry;
    uint32 Type;
    uint32 Value[2];
    uint32 Index;
};

struct ItemBonusTreeNodeEntry
{
    //uint32 ID;                                            // 0
    uint32 Category;                                        // 1
    uint32 Difficulty;                                      // 2
    uint32 LinkedCategory;                                  // 3
    uint32 ItemBonusEntry;                                  // 4
};

struct ItemXBonusTreeEntry
{
    //uint32 ID;                                             // 0
    uint32 ItemId;                                           // 1
    uint32 ItemBonusTreeCategory;                            // 2
};

struct ItemCurrencyCostEntry
{
    //uint32  Id;
    uint32  ItemId;
};

struct ItemSparseEntry
{
    uint32     ID;                                           // 0
    uint32     Quality;                                      // 1
    uint32     Flags;                                        // 2
    uint32     Flags2;                                       // 3
    uint32     Flags3;                                       // 4 NYI
    float      Unk430_1;                                     // 5
    float      Unk430_2;                                     // 6
    uint32     BuyCount;                                     // 7
    uint32     BuyPrice;                                     // 8
    uint32     SellPrice;                                    // 9
    uint32     InventoryType;                                // 10
    int32      AllowableClass;                               // 11
    int32      AllowableRace;                                // 12
    uint32     ItemLevel;                                    // 13
    int32      RequiredLevel;                                // 14
    uint32     RequiredSkill;                                // 15
    uint32     RequiredSkillRank;                            // 16
    uint32     RequiredSpell;                                // 17
    uint32     RequiredHonorRank;                            // 18
    uint32     RequiredCityRank;                             // 19
    uint32     RequiredReputationFaction;                    // 20
    uint32     RequiredReputationRank;                       // 21
    uint32     MaxCount;                                     // 22
    uint32     Stackable;                                    // 23
    uint32     ContainerSlots;                               // 24
    int32      ItemStatType[MAX_ITEM_PROTO_STATS];           // 25 - 34
    int32      ItemStatValue[MAX_ITEM_PROTO_STATS];          // 35 - 44
    int32      ScalingValue[MAX_ITEM_PROTO_STATS];           // 45 - 54
    float      SocketCostRate[MAX_ITEM_PROTO_STATS];         // 55 - 64
    uint32     ScalingStatDistribution;                      // 65
    uint32     DamageType;                                   // 66
    uint32     Delay;                                        // 67
    float      RangedModRange;                               // 68
    uint32     Bonding;                                      // 69
    char*      Name;                                         // 70
    char*      Name2;                                        // 71
    char*      Name3;                                        // 72
    char*      Name4;                                        // 73
    char*      Description;                                  // 74
    uint32     PageText;                                     // 75
    uint32     LanguageID;                                   // 76
    uint32     PageMaterial;                                 // 77
    uint32     StartQuest;                                   // 78
    uint32     LockID;                                       // 79
    int32      Material;                                     // 80
    uint32     Sheath;                                       // 81
    uint32     RandomProperty;                               // 82
    uint32     RandomSuffix;                                 // 83
    uint32     ItemSet;                                      // 84
    uint32     Area;                                         // 85
    uint32     Map;                                          // 86
    uint32     BagFamily;                                    // 87
    uint32     TotemCategory;                                // 88
    uint32     Color[MAX_ITEM_PROTO_SOCKETS];                // 89 - 91
    int32      SocketBonus;                                  // 92
    uint32     GemProperties;                                // 93
    float      ArmorDamageModifier;                          // 94
    uint32     Duration;                                     // 95
    uint32     ItemLimitCategory;                            // 96
    uint32     HolidayId;                                    // 97
    float      StatScalingFactor;                            // 98
    int32      CurrencySubstitutionId;                       // 99
    int32      CurrencySubstitutionCount;                    // 100
    int32      ItemNameDescriptionID;                        // 101
};

struct ItemEffectEntry
{
    uint32 ID;                                              // 0
    uint32 ItemID;                                          // 1
    uint32 EffectIndex;                                     // 2
    uint32 SpellID;                                         // 3
    uint32 SpellTrigger;                                    // 4
    int32  SpellCharge;                                     // 5
    uint32 SpellCooldown;                                   // 6
    uint32 SpellCategory;                                   // 7
    uint32 SpellCategoryCooldown;                           // 8
};

struct PvpItemEntry
{
    uint32 itemId;
    uint32 ilvl;
};

struct ItemUpgradeEntry
{
    uint32 Id;
    uint32 itemUpgradePath;
    uint32 itemLevelUpgrade;
    uint32 precItemUpgradeId;
    uint32 currencyId;
    uint32 currencyCost;
};

struct ItemModifiedAppearanceEntry
{
    uint32 ID;
    uint32 ItemID;
    uint32 Unk;
    uint32 AppearanceID;
    uint32 Unk2;
    uint32 Index;
};

struct ItemAppearanceEntry
{
    uint32 ID;
    uint32 DisplayID;
    uint32 Unk;
};

struct RulesetItemUpgradeEntry
{
    uint32 Id;
    uint32 unk;
    uint32 itemUpgradeId;
    uint32 itemid;
};

#define MAX_ITEM_EXT_COST_ITEMS         5
#define MAX_ITEM_EXT_COST_CURRENCIES    5

struct ItemExtendedCostEntry
{
    uint32      ID;                                                     // 0 extended-cost entry id
    //uint32    reqhonorpoints;                                         // 1 required honor points, only 0
    //uint32    reqarenapoints;                                         // 2 required arena points, only 0
    uint32      RequiredArenaSlot;                                      // 3 arena slot restrictions (min slot value)
    uint32      RequiredItem[MAX_ITEM_EXT_COST_ITEMS];                  // 4-8 required item id
    uint32      RequiredItemCount[MAX_ITEM_EXT_COST_ITEMS];             // 9-13 required count of 1st item
    uint32      RequiredPersonalArenaRating;                            // 14 required personal arena rating
    //uint32    ItemPurchaseGroup;                                      // 15, only 0
    uint32      RequiredCurrency[MAX_ITEM_EXT_COST_CURRENCIES];         // 16-20 required curency id
    uint32      RequiredCurrencyCount[MAX_ITEM_EXT_COST_CURRENCIES];    // 21-25 required curency count
    //uint32    RequiredFactionId;                                      // 26
    //uint32    RequiredFactionStanding;                                // 27
    //uint32    RequirementFlags;                                       // 28
    //uint32    RequiredGuildLevel;                                     // 29 removed in 6.X
};

struct QuestPackageItemEntry
{
    uint32 ID;                  // 0
    uint32 PackageID;           // 1
    uint32 ItemId;              // 2
    uint32 Count;               // 3
    uint32 Type;                // 4
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
    uint32 ID;                  // 0
    uint32 MapID;               // 1
    //uint32 Field2;            // 2
    //uint32 Field3;            // 3
    //uint32 Field4;            // 4
    uint32 BronzeTime;          // 5
    uint32 SilverTime;          // 6
    uint32 GoldTime;            // 7
    //uint32 Field8;            // 8
    //uint32 Field9;            // 9
};

struct SceneScriptEntry
{
    uint32 Entry;
    const char* Name;
    const char* Script;
    uint32 PrevScript;
    uint32 NextScript;
};

struct SceneScriptPackageEntry
{
    uint32 Entry;
    const char* Name;
};

#define MAX_SPELL_REAGENTS 8

struct SpellReagentsEntry
{
    //uint32    Id;                                         // 0        m_ID
    int32     Reagent[MAX_SPELL_REAGENTS];                  // 1-8      m_reagent
    uint32    ReagentCount[MAX_SPELL_REAGENTS];             // 9-16     m_reagentCount
    uint32    CurrencyID;                                   // 17       m_CurrencyID
    uint32    CurrencyCount;                                // 18       m_CurrencyCount
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
    //uint32    ID;                                         // 0        m_ID
    uint32    path;                                         // 1        m_PathID
    uint32    index;                                        // 2        m_NodeIndex
    uint32    mapid;                                        // 3        m_ContinentID
    float     x;                                            // 4        m_LocX
    float     y;                                            // 5        m_LocY
    float     z;                                            // 6        m_LocZ
    uint32    actionFlag;                                   // 7        m_flags
    uint32    delay;                                        // 8        m_delay
    uint32    arrivalEventID;                               // 9        m_arrivalEventID
    uint32    departureEventID;                             // 10       m_departureEventID
};

struct SpellRuneCostEntry
{
    uint32  ID;                                             // 0
    uint32  RuneCost[4];                                    // 1-3 (0=blood, 1=frost, 2=unholy, 3=wtf)
    uint32  runePowerGain;                                  // 4

    bool NoRuneCost() const { return RuneCost[0] == 0 && RuneCost[1] == 0 && RuneCost[2] == 0; }
    bool NoRunicPowerGain() const { return runePowerGain == 0; }
};

// SpellCastingRequirements.db2
struct SpellCastingRequirementsEntry
{
    //uint32    Id;                                         // 0      m_ID
    uint32    FacingCasterFlags;                            // 1      m_facingCasterFlags
    //uint32    MinFactionId;                               // 2      m_minFactionID not used
    //uint32    MinReputation;                              // 3      m_minReputation not used
    int32     AreaGroupId;                                  // 4      m_requiredAreaGroupId
    //uint32    RequiredAuraVision;                         // 5      m_requiredAuraVision not used
    uint32    RequiresSpellFocus;                           // 6      m_requiresSpellFocus
};

// SpellAuraRestrictions.db2
struct SpellAuraRestrictionsEntry
{
    //uint32    Id;                                         // 0 m_ID
    uint32    CasterAuraState;                              // 1 Flag
    uint32    TargetAuraState;                              // 2 Flag
    uint32    CasterAuraStateNot;                           // 3 m_excludeCasterAuraState
    uint32    TargetAuraStateNot;                           // 4 m_excludeTargetAuraState
    uint32    casterAuraSpell;                              // 5 m_casterAuraSpell
    uint32    targetAuraSpell;                              // 6 m_targetAuraSpell
    uint32    excludeCasterAuraSpell;                       // 7 m_excludeCasterAuraSpell
    uint32    excludeTargetAuraSpell;                       // 8 m_excludeTargetAuraSpell
};

struct AreaPOIEntry
{
    uint32 id;                                              // 0
    //uint32 unk;                                           // 1 m_Importance
    //uint32 unk;                                           // 2
    //uint32 unk;                                           // 3
    uint32 mapId;                                           // 4 m_ContinentID
    uint32 zoneId;                                          // 5 m_AreaID
    //uint32 unk;                                           // 6
    float x;                                                // 7 m_Pos
    float y;                                                // 8 m_Pos
    //char* m_NameLang;                                     // 9 m_NameLang
    //char* m_DescriptionLang;                              // 10 m_DescriptionLang
    uint32 worldState;                                      // 11 m_WorldStateID
    //uint32 unk;                                           // 12 m_PlayerConditionID @todo
    //uint32 unk                                            // 13
    //uint32 unk;                                           // 14
};

#define MAX_HOLIDAY_DURATIONS 10
#define MAX_HOLIDAY_DATES 16
#define MAX_HOLIDAY_FLAGS 10

struct HolidaysEntry
{
    uint32 Id;                                              // 0        m_ID
    uint32 Duration[MAX_HOLIDAY_DURATIONS];                 // 1-10     m_duration
    uint32 Date[MAX_HOLIDAY_DATES];                         // 11-26    m_date (dates in unix time starting at January, 1, 2000)
    uint32 Region;                                          // 27       m_region (wow region)
    uint32 Looping;                                         // 28       m_looping
    uint32 CalendarFlags[MAX_HOLIDAY_FLAGS];                // 29-38    m_calendarFlags
    //uint32 holidayNameId;                                 // 39       m_holidayNameID (HolidayNames.dbc)
    //uint32 holidayDescriptionId;                          // 40       m_holidayDescriptionID (HolidayDescriptions.dbc)
    uint32 Priority;                                        // 41       m_priority
    char* TextureFilename;                                  // 42       m_textureFilename
    int32 CalendarFilterType;                               // 43       m_calendarFilterType (-1 = Fishing Contest, 0 = Unk, 1 = Darkmoon Festival, 2 = Yearly holiday)
    //uint32 flags;                                         // 44       m_flags (0 = Darkmoon Faire, Fishing Contest and Wotlk Launch, rest is 1)
};

#define MAX_OVERRIDE_SPELL 10

struct OverrideSpellDataEntry
{
    uint32      id;                                         // 0
    uint32      spellId[MAX_OVERRIDE_SPELL];                // 1-10
    //uint32      unk_0;                                    // 11
    //uint32      unk_1;                                    // 12 possibly flag
};

//SpellMisc.dbc
struct SpellMiscEntry
{
    uint32    Id;                                           // 0        m_ID
    uint32    Attributes;                                   // 1        m_attribute
    uint32    AttributesEx;                                 // 2        m_attributesEx
    uint32    AttributesEx2;                                // 3        m_attributesExB
    uint32    AttributesEx3;                                // 4        m_attributesExC
    uint32    AttributesEx4;                                // 5        m_attributesExD
    uint32    AttributesEx5;                                // 6        m_attributesExE
    uint32    AttributesEx6;                                // 7        m_attributesExF
    uint32    AttributesEx7;                                // 8        m_attributesExG (0x20 - totems, 0x4 - paladin auras, etc...)
    uint32    AttributesEx8;                                // 9        m_attributesExH
    uint32    AttributesEx9;                                // 10       m_attributesExI
    uint32    AttributesEx10;                               // 11       m_attributesExJ
    uint32    AttributesEx11;                               // 12       m_attributesExK
    uint32    AttributesEx12;                               // 13       m_attributesExL
    uint32    AttributesEx13;                               // 14       m_attributesExM
    uint32    CastingTimeIndex;                             // 15       m_castingTimeIndex
    uint32    DurationIndex;                                // 16       m_durationIndex
    uint32    rangeIndex;                                   // 17       m_rangeIndex
    float     speed;                                        // 18       m_speed
    uint32    SpellVisual[2];                               // 19-20    m_spellVisualID
    uint32    SpellIconID;                                  // 21       m_spellIconID
    uint32    activeIconID;                                 // 22       m_activeIconID
    uint32    SchoolMask;                                   // 23       m_schoolMask
    //float   Unknown;                                      // 24       unk 601 18612
};

struct SpellPowerEntry
{
    uint32    Id;                                           // 0
    uint32    SpellId;                                      // 1
    //uint32    unk_1;                                      // 2
    uint32    PowerType;                                    // 3
    uint32    Cost;                                         // 4
    uint32    CostPerlevel;                                 // 5
    uint32    CostPerSecond;                                // 6
    //uint32    unk_4;                                      // 7
    //uint32    unk_5;                                      // 8
    //uint32    unk_5;                                      // 9
    float     CostBasePercentage;                           // 10
    float     CostPerSecondPercentage;                      // 11
    uint32    RequiredAuraSpellId;                          // 12
    float     CostMaxPercentage;                            // 13
};

#define MAX_SPELL_TOTEMS            2

// SpellTotems.dbc
struct SpellTotemsEntry
{
    uint32    Id;                                           // 0  m_ID
    uint32    TotemCategory[MAX_SPELL_TOTEMS];              // 1  m_requiredTotemCategoryID
    uint32    Totem[MAX_SPELL_TOTEMS];                      // 2  m_totem
    //uint32    unk_1;                                      // 3
    //uint32    unk_2;                                      // 4  only 1925
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

// SpellClassOptions.dbc
struct SpellClassOptionsEntry
{
    //uint32    Id;                                         // 0       m_ID
    //uint32    modalNextSpell;                             // 1       m_modalNextSpell not used
    flag128   SpellFamilyFlags;                             // 2-5
    uint32    SpellFamilyName;                              // 6       m_spellClassSet
};

//////////////////////////////////////////////////////////////////////////
/// Garrison DB2
//////////////////////////////////////////////////////////////////////////
struct GarrSiteLevelEntry
{
    uint32      SiteLevelID;                            // 0
    uint32      Level;                                  // 1
    uint32      MapID;                                  // 2
    uint32      SiteID;                                 // 3
    uint32      MapTextureUiTextureKitID;               // 4
    float       TownHallX;                              // 5
    float       TownHallY;                              // 6
    uint32      CreationMovie;                          // 7
    uint32      Unk5;                                   // 8
    uint32      UpgradeCost;                            // 9
    uint32      UpgradeMoneyCost;                       // 10
};

struct GarrSiteLevelPlotInstEntry
{
    uint32      ID;                                     // 0
    uint32      SiteLevelID;                            // 1
    uint32      PlotInstanceID;                         // 2
    float       MinimapX;                               // 3
    float       MinimapY;                               // 4
    uint32      Unk1;                                   // 5
};

struct GarrPlotInstanceEntry
{
    uint32      InstanceID;                             // 0
    uint32      PlotID;                                 // 1
    char      * Name;                                   // 2
};

struct GarrPlotEntry
{
    uint32      PlotID;                                 // 0
    uint32      PlotUiCategoryID;                       // 1
    uint32      PlotType;                               // 2
    uint32      Unk2;                                   // 3
    uint32      Unk3;                                   // 4
    uint32      Unk4;                                   // 5
    uint32      Unk5;                                   // 6
    uint32      BuildingGameObject[2];                  // 8 - 9
};

struct GarrPlotUICategoryEntry
{
    uint32      PlotUiCategoryID;                       // 0
    char      * Name;                                   // 1
    uint32      Type;                                   // 2
};

struct GarrMissionEntry
{
    uint32 MissionRecID;                                // 0
    int32  RequiredLevel;                               // 1
    int32  RequiredItemLevel;                           // 2
    uint32 GarrMechanicTypeRecID;                       // 3
    uint32 RequiredFollowersCount;                      // 4
    uint32 TravelTime;                                  // 5
    uint32 Duration;                                    // 6
    uint32 OfferTime;                                   // 7
    uint32 LocPrefixID;                                 // 8
    uint32 Unk3;                                        // 9
    char * Name;                                        // 10
    char * Description;                                 // 11
    char * Location;                                    // 12
    uint32 Unk5;                                        // 13
    uint32 Unk6;                                        // 14
    uint32 GarrisonCurrencyStartCost;                   // 15
    uint32 Flags;                                       // 16
    uint32 RewardFollowerExperience;                    // 17
    uint32 BaseBronzeChestChance;                       // 18
};

struct GarrMissionRewardEntry
{
    uint32 MissionRewardID;                             // 0
    uint32 MissionID;                                   // 1
    uint32 BonusRewardXP;                               // 2
    uint32 ItemID;                                      // 3
    uint32 ItemQuantity;                                // 4
    uint32 RewardCurrencyID;                            // 5
    uint32 RewardCurrencyAmount;                        // 6
    uint32 Unk1;                                        // 7
    uint32 Unk2;                                        // 8
    uint32 Unk3;                                        // 9
};

struct GarrMissionXEncouterEntry
{
    uint32 ID;                                          // 0
    uint32 MissionID;                                   // 1
    uint32 EncounterID;                                 // 2
};

struct GarrBuildingEntry
{
    uint32 BuildingID;                                  // 0
    uint32 GameObjects[2];                              // 1 - 2
    uint32 Unk2;                                        // 3
    uint32 BuildingType;                                // 4
    uint32 BuildingLevel;                               // 5
    char * NameA;                                       // 6
    char * NameH;                                       // 7
    char * Brief;                                       // 8
    char * Description;                                 // 9
    uint32 BuildTime;                                   // 10
    uint32 BuildCostCurrencyID;                         // 11
    uint32 BuildCostCurrencyAmount;                     // 12
    uint32 TexturePrefixA;                              // 13
    uint32 TexturePrefixH;                              // 14
    uint32 IconID;                                      // 15
    uint32 Unk7;                                        // 16
    uint32 BuildingCategory;                            // 17
    uint32 BuiltScene[2];                               // 18 - 19
    uint32 BonusAmount;                                 // 20
    uint32 Unk12;                                       // 21
    uint32 PassiveEffect;                               // 22
    uint32 MoneyCost;                                   // 23
};

struct GarrPlotBuildingEntry
{
    uint32 ID;                                          // 0
    uint32 PlotId;                                      // 1
    uint32 BuildingID;                                  // 2
};

struct GarrFollowerEntry
{
    uint32 ID;                                          // 0
    uint32 Entry[2];                                    // 1 - 2
    uint32 Class[2];                                    // 3 - 4
    uint32 Quality;                                     // 5
    uint32 Unk4;                                        // 6
    uint32 Unk5;                                        // 7
    uint32 Unk6;                                        // 8
    uint32 Unk7;                                        // 9
    int32  Level;                                       // 10
    int32  ItemLevelWeapon;                             // 11
    int32  ItemLevelArmor;                              // 12
    uint32 Unk8;                                        // 13
    uint32 Unk9;                                        // 14
    char * UnkString1;                                  // 15
    char * UnkString2;                                  // 16
    int32  Unk10;                                       // 17
    int32  Unk11;                                       // 18
    uint32 Unk12;                                       // 19
    uint32 Unk13;                                       // 20
};

struct GarrAbilityEntry
{
    uint32 ID;                                          // 0
    uint32 Flags;                                       // 1
    char * Name;                                        // 2
    char * Description;                                 // 3
    uint32 IconID;                                      // 4
    uint32 Unk3;                                        // 5
    uint32 Category;                                    // 6
};

struct GarrAbilityEffectEntry
{
    uint32 EffectID;                                    // 0
    uint32 EffectType;                                  // 1
    uint32 AbilityID;                                   // 2
    uint32 TargetMask;                                  // 3
    uint32 CounterMechanicTypeID;                       // 4
    uint32 Unk3;                                        // 5
    float ModMin;                                       // 6
    float ModMax;                                       // 7
    float Amount;                                       // 8
    uint32 MiscValueA;                                  // 9
    uint32 MiscValueB;                                  // 10
};

struct GarrFollowerXAbilityEntry
{
    uint32 ID;                                          // 0
    uint32 FollowerID;                                  // 1
    uint32 AbilityID;                                   // 2
    uint32 FactionIndex;                                // 3
};

struct GarrBuildingPlotInstEntry
{
    uint32 ID;                                          // 0
    uint32 BuildingID;                                  // 1
    uint32 UIAtlasTextureMemberID;                      // 2
    uint32 SiteLevelPlotInstID;                         // 3
    uint32 MinimapX;                                    // 4
    uint32 MinimapY;                                    // 5
};

struct GarrMechanicEntry
{
    uint32 ID;                                          // 0
    uint32 MechanicTypeID;                              // 1
    float  Unk2;                                        // 2
};

struct GarrMechanicTypeEntry
{
    uint32 ID;                                          // 0
    uint32 Type;                                        // 1
    char * Environment;                                 // 2
    char * EnvironmentDesc;                             // 3
    uint32 EnvironmentTextureID;                        // 4
};

struct GarrEncouterXMechanicEntry
{
    uint32 ID;                                          // 0
    uint32 EncounterID;                                 // 1
    uint32 MechanicID;                                  // 2
};

struct GarrFollowerLevelXPEntry
{
    uint32 ID;                                          // 0
    uint32 Level;                                       // 1
    uint32 RequiredExperience;                          // 2
};

struct GarrSpecializationEntry
{
    uint32 ID;                                          // 0
    uint32 Unk1;                                        // 1
    uint32 Unk2;                                        // 2
    uint32 Unk3;                                        // 3
    uint32 Unk4;                                        // 4
    float  BasePoint;                                   // 5
    uint32 Unk6;                                        // 6
    char * Name;                                        // 7
    char * Description;                                 // 8
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
    uint32 ID;                                          // 0
    uint32 ShipmentContainerID;                         // 1
    uint32 Unk1;                                        // 2
    uint32 Flags;                                       // 3
    uint32 Duration;                                    // 4
    uint32 SpellID;                                     // 5
    uint32 ResultItemID;                                // 6
};

struct CharShipmentContainerEntry
{
    uint32 ID;                                          // 0
    uint32 Unk1;                                        // 1
    uint32 BuildingType;                                // 2
    char * Name;                                        // 3
    uint32 TextureKitID;                                // 4
    uint32 Unk2;                                        // 5
    uint32 Unk3;                                        // 6
    uint32 Unk4;                                        // 7
    uint32 Unk5;                                        // 8
    uint32 Unk6;                                        // 9
    char * Description;                                 // 10
    uint32 Unk7;                                        // 11
    uint32 Unk8;                                        // 12
};

#define MAX_BATTLEPET_PROPERTIES 6

// BattlePetAbility.dbc
struct BattlePetAbilityEntry
{
    uint32 id;
    uint32 petType;
    uint32 family;
    uint32 cooldown;
    uint32 visualId; // BattlePetVisual.dbc
    uint32 flags;
    const char * name;
    const char * description;
};

// BattlePetAbilityEffect.dbc
struct BattlePetAbilityEffectEntry
{
    uint32 id;
    uint32 abilityTurnId; // BattlePetAbilityTurn.dbc
    uint32 visualId; // BattlePetVisual.dbc
    uint32 triggerAbility;
    uint32 effect;
    uint32 effectIndex;
    int32 prop[MAX_BATTLEPET_PROPERTIES]; // See BattlePetEffectProperties.dbc with effect
};

// BattlePetAbilityTurn.dbc
struct BattlePetAbilityTurnEntry
{
    uint32 id;
    uint32 abilityId; // BattlePetAbility.dbc
    uint32 visualId; // BattlePetVisual.dbc
    uint32 turn;
    uint32 hasProcType; // if 1 then value <> -1
    int32 procType;
};

// BattlePetAbilityState.dbc
struct BattlePetAbilityStateEntry
{
    uint32 id;
    uint32 abilityId; // BattlePetAbility.dbc
    uint32 stateId; // BattlePetState.dbc
    int32 value;
};

// BattlePetState.dbc
struct BattlePetStateEntry
{
    uint32 id;
    uint32 parent; // BattlePetState.dbc
    const char * name;
    uint32 flags;
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

// BattlePetEffectProperties.dbc
struct BattlePetEffectPropertiesEntry
{
    uint32 effect;
    uint32 flags;
    const char * propName[MAX_BATTLEPET_PROPERTIES];
    uint32 propIsId[MAX_BATTLEPET_PROPERTIES]; // Only set to 1 for AuraID
};

// BattlePetBreedQuality.dbc
struct BattlePetBreedQualityEntry
{
    uint32 id;
    uint32 quality;
    float factor;
};

// BattlePetBreedState.dbc
struct BattlePetBreedStateEntry
{
    uint32 id;
    uint32 breed;
    uint32 stateId; // BattlePetState.dbc
    int32 value;
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

// BattlePetSpecies.dbc
struct BattlePetSpeciesEntry
{
    uint32 id;
    uint32 entry;
    uint32 iconId;
    uint32 spellId; // Spell.dbc
    uint32 type;
    uint32 obtainmentCategoryDescription;
    uint32 flags;
    const char * source;
    const char * description;
};

// BattlePetSpeciesState.dbc
struct BattlePetSpeciesStateEntry
{
    uint32 id;
    uint32 speciesId; // BattlePetSpecies.dbc
    uint32 stateId; // BattlePetState.dbc
    int32 value;
};

// BattlePetSpeciesXAbility.dbc
struct BattlePetSpeciesXAbilityEntry
{
    uint32 id;
    uint32 speciesId; // BattlePetSpecies.dbc
    uint32 abilityId; // BattlePetAbility.dbc
    uint32 level;
    uint32 tier;
};

/// Mount.db2
struct MountEntry
{
    uint32 Id;
    uint32 Category;
    uint32 CreatureDisplayID;
    uint32 Flags;
    char*  Name;
    char*  Description;
    char*  Icon;
    int32  Unknow;
    uint32 SpellID;
    uint32 UnknowEntry;
};

/// PlayerCondition.db2
struct PlayerConditionEntry
{
    uint32 ID;
    uint32 Flags;
    uint32 MinLevel;
    uint32 MaxLevel;
    uint32 RaceMask;
    uint32 ClassMask;
    uint32 Gender;
    uint32 NativeGender;
    uint32 SkillID[4];
    uint32 MinSkill[4];
    uint32 MaxSkill[4];
    uint32 SkillLogic;
    uint32 LanguageID;
    uint32 MinLanguage;
    uint32 MaxLanguage;
    uint32 MinFactionID[3];
    uint32 MaxFactionID;
    uint32 MinReputation[3];
    uint32 MaxReputation;
    uint32 ReputationLogic;
    uint32 MinPVPRank;
    uint32 MaxPVPRank;
    uint32 PvpMedal;
    uint32 PrevQuestLogic;
    uint32 PrevQuestID[4];
    uint32 CurrQuestLogic;
    uint32 CurrQuestID[4];
    uint32 CurrentCompletedQuestLogic;
    uint32 CurrentCompletedQuestID[4];
    uint32 SpellLogic;
    uint32 SpellID[4];
    uint32 ItemLogic;
    uint32 ItemID[4];
    uint32 ItemCount[4];
    uint32 ItemFlags;
    uint32 Explored[2];
    uint32 Time[2];
    uint32 AuraSpellLogic;
    uint32 AuraSpellID[4];
    uint32 WorldStateExpressionID;
    uint32 WeatherID;
    uint32 PartyStatus;
    uint32 LifetimeMaxPVPRank;
    uint32 AchievementLogic;
    uint32 Achievement[4];
    uint32 LfgLogic;
    uint32 LfgStatus[4];
    uint32 LfgCompare[4];
    uint32 LfgValue[4];
    uint32 AreaLogic;
    uint32 AreaID[4];
    uint32 CurrencyLogic;
    uint32 CurrencyID[4];
    uint32 CurrencyCount[4];
    uint32 QuestKillID;
    uint32 QuestKillLogic;
    uint32 QuestKillMonster[4];
    uint32 MinExpansionLevel;
    uint32 MaxExpansionLevel;
    uint32 MinExpansionTier;
    uint32 MaxExpansionTier;
    uint32 MinGuildLevel;
    uint32 MaxGuildLevel;
    uint32 PhaseUseFlags;
    uint32 PhaseID;
    uint32 PhaseGroupID;
    uint32 MinAvgItemLevel;
    uint32 MaxAvgItemLevel;
    uint32 MinAvgEquippedItemLevel;
    uint32 MaxAvgEquippedItemLevel;
    uint32 ChrSpecializationIndex;
    uint32 ChrSpecializationRole;
    char*  FailureDescriptionLang;
    uint32 PowerType;
    uint32 PowerTypeComp;
    uint32 PowerTypeValue;
};

/// Flags of PrevQuestLogic (PlayerConditionEntry) 
namespace PrevQuestLogicFlags
{
    enum 
    {
        TrackingQuest = 0x10000
    };
}

/// Vignette.db2
struct VignetteEntry
{
    uint32 Id;
    char*  Name;
    uint32 QuestFeedbackEffectId;
    uint32 Flags;
    float  X;
    float  Y;
};

// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

#endif
