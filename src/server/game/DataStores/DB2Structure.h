////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_DB2STRUCTURE_H
#define TRINITY_DB2STRUCTURE_H

#include "Common.h"
#include "DBCEnums.h"
#include "Define.h"
#include "Path.h"
#include "Util.h"
#include "SharedDefines.h"
#include "ItemPrototype.h"

/// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack(1)
#else
#pragma pack(push, 1)
#endif

/// Always order this file.

////////////////////////////////////////////////////////////////////
/// Auction House DB2
////////////////////////////////////////////////////////////////////

struct AuctionHouseEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  FactionID;                                      // 1        m_factionID             m_ID of faction.dbc for player factions associated with city
    uint32  DepositRate;                                    // 2        m_depositRate           1/3 from real
    uint32  ConsignmentRate;                                // 3        m_consignementRate
    //char*     name;                                       // 4        m_name_lang
};

////////////////////////////////////////////////////////////////////
/// Achaeology DB2
////////////////////////////////////////////////////////////////////

struct ResearchBranchEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     m_Name;                                     // 1        m_NameLang
    //uint32    m_ResearchFieldID;                          // 2        m_ResearchFieldID
    uint32  CurrencyID;                                     // 3        m_CurrencyID
    //char*     m_Texture;                                  // 4        m_Texture
    uint32  ItemID;                                         // 5        m_ItemID
};

struct ResearchProjectEntry
{
    uint32  ID;                                             // 0        m_ID
    //char*     m_ID;                                       // 1        m_NameLang
    //char*     m_NameLang;                                 // 2        m_DescriptionLang
    uint32  rare;                                           // 3        m_Rarity
    uint32  branchId;                                       // 4        m_ResearchBranchID
    uint32  spellId;                                        // 5        m_SpellID
    //uint32    m_NumSockets;                               // 6        m_NumSockets
    //char*     m_Texture;                                  // 7        m_Texture
    uint32  req_currency;                                   // 8        m_RequiredWeight
    
};

struct ResearchSiteEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  mapId;                                          // 1        m_MapID
    uint32  POIid;                                          // 2        m_QuestPOIBlobID
    //char*     m_NameLang;                                 // 3        m_NameLang
    //uint32    AreaPOIIconEnum;                            // 4        m_AreaPOIIconEnum
};

////////////////////////////////////////////////////////////////////
/// Area DB2
////////////////////////////////////////////////////////////////////
/// @todo implement AreaGroupEntry DB2 Format

struct AreaGroupEntry
{
    uint32 id;                                                      ///< 0 areagroup id (old dbc)
};

struct AreaGroupMemberEntry
{
    uint32 ID;                                                      ///< 0
    uint32 AreaGroupID;                                             ///< 1
    uint32 AreaID;                                                  ///< 2
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

////////////////////////////////////////////////////////////////////
/// Areatrigger DB2
////////////////////////////////////////////////////////////////////

struct CurvePointEntry
{
    uint32 ID;                                                      ///< 0
    uint32 CurveID;                                                 ///< 1
    uint32 Index;                                                   ///< 2
    float X;                                                        ///< 3
    float Y;                                                        ///< 4
};

////////////////////////////////////////////////////////////////////
/// Barber DB2
////////////////////////////////////////////////////////////////////

struct BarberShopStyleEntry
{
    uint32  ID;                                             // 0        m_ID
    uint32  Type;                                           // 1        m_type                  Value 0 -> hair, value 2 -> facial hair
    //char*     m_DisplayName_lang;                         // 2        m_DisplayName_lang
    //uint32    m_Description_lang;                         // 3        m_Description_lang
    //float     m_CostMultiplier;                           // 4        m_Cost_Modifier
    uint32  Race;                                           // 5        m_race
    uint32  Sex;                                            // 6        m_sex
    uint32  Data;                                           // 7        m_data                  Real ID to hair/facial hair
};


////////////////////////////////////////////////////////////////////
/// BattlePet DB2
////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////
/// Creature DB2
////////////////////////////////////////////////////////////////////

struct CreatureDisplayInfoEntry
{
    uint32  Displayid;                                              ///< 0      m_ID
    uint32  ModelId;                                                ///< 1      m_ModelID
    //uint32    m_sounID;                                           ///< 2      m_SoundID
    uint32  ExtendedDisplayInfoID;                                  ///< 3      m_ExtendedDisplayInfoID
    float   scale;                                                  ///< 4      m_CreatureModelScale
    //float     620_unk                                             ///< 5
    //uint32    m_creatureModelAlpha;                               ///< 6      m_CreatureModelAlpha
    //char*     m_textureName;                                      ///< 7-9    m_TextureVariation[3]
    //char*     m_portraitTextureName ;                             ///< 10      m_PortraitTextureName
    //uint32    m_PortraitCreatureDisplayInfoID                     ///< 11     m_PortraitCreatureDisplayInfoID
    //uint32    m_SizeClass;                                        ///< 12     m_SizeClasss
    //uint32    m_BloodID;                                          ///< 13     m_BloodID
    //uint32    m_NPCSoundID;                                       ///< 14     m_NPCSoundID
    //uint32    m_ParticleColorID;                                  ///< 15     m_ParticleColorID
    //uint32    m_CreatureGeosetData;                               ///< 16     m_CreatureGeosetData
    //uint32    m_ObjectEffectPackageID;                            ///< 17     m_ObjectEffectPackageID
    //uint32    m_AnimReplacementSetID;                             ///< 18     m_AnimReplacementSetID
    //uint32    m_Flags;                                            ///< 19     m_Flags
    //int32     m_Gender;                                           ///< 20     m_Gender
    //uint32    m_StateSpellVisualKitID;                            ///< 21     m_StateSpellVisualKitID
};

struct CreatureTypeEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    //char*     nameLang;                                           ///< 1      m_name_lang
    //uint32    flags;                                              ///< 2      m_flags       1 = no_xp
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

struct ChrClassXPowerTypesEntry
{
    uint32   entry;                                          // 0
    uint32   classId;                                        // 1
    uint32   power;                                          // 2
};

////////////////////////////////////////////////////////////////////
/// CurrencyCategorie.db2 is used as enum

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

////////////////////////////////////////////////////////////////////
/// Destructible GameObject DB2
////////////////////////////////////////////////////////////////////

struct DestructibleModelDataEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  DamagedDisplayId;                                       ///< 1      m_State0Wmo
    //uint32    m_State0ImpactEffectDoodadSet;                      ///< 2      m_State0ImpactEffectDoodadSet
    //uint32    m_State0AmbientDoodadSet;                           ///< 3      m_State0AmbientDoodadSet
    //uint32    m_State0NameSet;                                    ///< 4      m_State0NameSet
    uint32  DestroyedDisplayId;                                     ///< 5      m_State1Wmo
    //uint32    m_State1DestructionDoodadSet;                       ///< 6      m_State1DestructionDoodadSet
    //uint32    m_State1ImpactEffectDoodadSet;                      ///< 7      m_State1ImpactEffectDoodadSet
    //uint32    m_State1AmbientDoodadSet;                           ///< 8      m_State1AmbientDoodadSet
    //uint32    m_State1NameSet;                                    ///< 9      m_State1NameSet
    uint32  RebuildingDisplayId;                                    ///< 10     m_State2Wmo
    //uint32    m_State2DestructionDoodadSet;                       ///< 11     m_State2DestructionDoodadSet
    //uint32    m_State2ImpactEffectDoodadSet;                      ///< 12     m_State2ImpactEffectDoodadSet
    //uint32    m_State2AmbientDoodadSet;                           ///< 13     m_State2AmbientDoodadSet
    //uint32    m_State2NameSet;                                    ///< 14     m_State2NameSet
    uint32  SmokeDisplayId;                                         ///< 15     m_State3Wmo
    //uint32    m_State3InitDoodadSet;                              ///< 16     m_State3InitDoodadSet
    //uint32    m_State3AmbientDoodadSet;                           ///< 17     m_State3AmbientDoodadSet
    //uint32    m_State3NameSet;                                    ///< 18     m_State3NameSet
    //uint32    m_EjectDirection;                                   ///< 19     m_EjectDirection
    //uint32    m_RepairGroundFx;                                   ///< 20     m_RepairGroundFx
    //uint32    m_DoNotHighlight;                                   ///< 21     m_DoNotHighlight
    //uint32    m_HealEffect;                                       ///< 22     m_HealEffect
    //uint32    m_HealEffectSpeed;                                  ///< 23     m_HealEffectSpeed;
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
    uint32 UITextureKitID;                                          ///< 4
    float  TownHallX;                                               ///< 5
    float  TownHallY;                                               ///< 6
    uint32 MovieID;                                                 ///< 7
    uint32 Level2;                                                  ///< 8
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
    uint32 GarrPlotUICategoryID;                                    ///< 1
    uint32 PlotType;                                                ///< 2
    uint32 Flags;                                                   ///< 3      alway 0 (6.2.0)
    LocalizedString const* Name;                                    ///< 4
    uint32 MinCount;                                                ///< 5
    uint32 MaxCount;                                                ///< 6
    uint32 AllianceConstructionGameObjectID;                        ///< 7
    uint32 HordeConstructionGameObjectID;                           ///< 8
};

struct GarrPlotUICategoryEntry
{
    uint32 PlotUICategoryID;                                        ///< 0
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
    uint32 Category;                                                ///< 9
    uint32 MissionType;                                             ///< 10
    uint32 FollowerType;                                            ///< 11
    LocalizedString const* Name;                                    ///< 12
    LocalizedString const* Description;                             ///< 13
    LocalizedString const* Location;                                ///< 14
    uint32 SubCategory2;                                            ///< 15
    uint32 SubCategory1;                                            ///< 16
    uint32 CurrencyCost;                                            ///< 17
    uint32 CurrencyID;                                              ///< 18
    uint32 Flags;                                                   ///< 19
    uint32 RewardFollowerExperience;                                ///< 20
    uint32 BaseBonusChance;                                         ///< 21
    uint32 LostChance;                                              ///< 22
    uint32 MapX;                                                    ///< 23 X for the ship mission map
    uint32 MapY;                                                    ///< 24 Y for the ship mission map
};

struct GarrMissionRewardEntry
{
    uint32 MissionRewardID;                                         ///< 0
    uint32 MissionID;                                               ///< 1
    uint32 PlayerRewardXP;                                          ///< 2
    uint32 ItemID;                                                  ///< 3
    uint32 ItemQuantity;                                            ///< 4
    uint32 RewardCurrencyID;                                        ///< 5
    uint32 RewardCurrencyAmount;                                    ///< 6
    uint32 Unk1;                                                    ///< 7
    uint32 Unk2;                                                    ///< 8
    uint32 Unk3;                                                    ///< 9
    uint32 Unk4;                                                    ///< 10 6.2.0 Related to Category: Ship-Bonus data on ship mission map
};

struct GarrMissionXEncouterEntry
{
    uint32 ID;                                                      ///< 0
    uint32 MissionID;                                               ///< 1
    uint32 EncounterID;                                             ///< 2
};

struct GarrBuildingEntry
{
    uint32 ID;                                                      ///< 0
    uint32 GameObjects[2];                                          ///< 1 - 2
    uint32 Unk;                                                     ///< 3
    uint32 Type;                                                    ///< 4
    uint32 Level;                                                   ///< 5
    LocalizedString const* NameAlliance;                            ///< 6
    LocalizedString const* NameHorde;                               ///< 7
    LocalizedString const* Description;                             ///< 8
    LocalizedString const* Tooltip;                                 ///< 9
    uint32 BuildDuration;                                           ///< 10
    uint32 CostCurrencyID;                                          ///< 11
    uint32 CostCurrencyAmount;                                      ///< 12
    uint32 HordeTexPrefixKitID;                                     ///< 13
    uint32 AllianceTexPrefixKitID;                                  ///< 14
    uint32 IconFileDataID;                                          ///< 15
    uint32 BonusAmount;                                             ///< 16     slot number ex : Menagerie Attracts elite pets you can battle for rewards. Also lets 5 of your battle pets hang out in your garrison.
    uint32 BuildingCategory;                                        ///< 17     Know as "Flags" on TC
    uint32 AllianceActivationScenePackageID;                        ///< 18
    uint32 HordeActivationScenePackageID;                           ///< 19
    uint32 MaxShipments;                                            ///< 20
    uint32 FollowerRequiredGarrAbilityID;                           ///< 21     GarrAbilityID http://www.wowhead.com/building=27/barracks#rel-garrisonabilities
    uint32 FollowerGarrAbilityEffectID;                             ///< 22
    uint32 CostMoney;                                               ///< 23
};

struct GarrPlotBuildingEntry
{
    uint32 ID;                                                      ///< 0
    uint32 PlotID;                                                  ///< 1
    uint32 BuildingID;                                              ///< 2
};

struct GarrFollowerEntry
{
    uint32 ID;                                                      ///< 0
    uint32 Type;                                                    ///< 1
    uint32 CreatureID[2];                                           ///< 2-3
    uint32 HordeUiAnimRaceInfoID;                                   ///< 4
    uint32 AllianceUiAnimRaceInfoID;                                ///< 5
    uint32 Quality;                                                 ///< 6
    uint32 HordeGarrClassSecID;                                     ///< 7      GarrClassSpec.db2
    uint32 AllianceGarrClassSecID;                                  ///< 8
    uint32 HordeGarrFollItemSetID;                                  ///< 9
    uint32 AllianceGarrFollItemSetID;                               ///< 10
    int32  Level;                                                   ///< 11
    int32  ItemLevelWeapon;                                         ///< 12
    int32  ItemLevelArmor;                                          ///< 13
    uint32 Unk;                                                     ///< 14
    uint32 Flags;                                                   ///< 15
    char * HordeSourceText;                                         ///< 16     Descr1 : zoneH (where you find this follower)
    char * AllinaceSourceText;                                      ///< 17     Descr2 : zoneA (where you find this follower)
    int32  Unk1;                                                    ///< 18     H
    int32  Unk2;                                                    ///< 19     A
    uint32 HordePortraitIconID;                                     ///< 20
    uint32 AlliancePortraitIconID;                                  ///< 21
};

struct GarrFollowerTypeEntry
{
    uint32 ID;                                                      ///< 0
    uint32 SoftCap;                                                 ///< 1
    uint32 SoftCapBuildingIncreaseID;                               ///< 2
    uint32 MaxItemLevel;                                            ///< 3
    uint32 Flags;                                                   ///< 4
};

struct GarrAbilityEntry
{
    uint32 ID;                                                      ///< 0
    uint32 AbilityType;                                             ///< 1
    char * Name;                                                    ///< 2
    char * Description;                                             ///< 3
    uint32 IconID;                                                  ///< 4
    uint32 OtherfactionGarrAbilityID;                               ///< 5
    uint32 Category;                                                ///< 6
    uint32 FollowerType;                                            ///< 7
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
    uint32 OverrideDisplayIfNotNull;                                ///< 5 - shipyard override displayID
    uint32 OverrideDisplayID1;                                      ///< 6
    uint32 OverrideIfAmountMet[2];                                  ///< 7-8
    uint32 ShipmentAmountNeeded[2];                                 ///< 9-10
    LocalizedString const* Description;                             ///< 11
    uint32 Unk8;                                                    ///< 12
    uint32 Unk9;                                                    ///< 13
    uint32 Unk10;                                                   ///< 14
    uint32 Unk11;                                                   ///< 15
};

////////////////////////////////////////////////////////////////////
/// Glyph DB2
////////////////////////////////////////////////////////////////////

struct GlyphRequiredSpecEntry
{
    uint32 ID;                                                      ///< 0
    uint32 GlyphID;                                                 ///< 1
    uint32 SpecID;                                                  ///< 2
};
struct GlyphSlotEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  TypeFlags;                                              ///< 1      m_Type
    uint32  Order;                                                  ///< 2      m_Tooltip
};

////////////////////////////////////////////////////////////////////
/// Guild DB2
////////////////////////////////////////////////////////////////////

struct GuildPerkSpellsEntry
{
    //uint32    Id;                                                 ///< 0      m_ID
    uint32  Level;                                                  ///< 1      m_GuildLevel
    uint32  SpellId;                                                ///< 2      m_SpellID
};

////////////////////////////////////////////////////////////////////
/// GroupFinder DB2
////////////////////////////////////////////////////////////////////

struct GroupFinderActivityEntry
{
    uint32 ID;                                                      ///< 0
    int32  CategoryID;                                              ///< 1
    uint32 ActivityGroupID;                                         ///< 2
    int32  Unk1;                                                    ///< 3
    int32  MinLevel;                                                ///< 4
    int32  MaxLevel;                                                ///< 5
    int32  RequiredILvl;                                            ///< 6
    int32  MapID;                                                   ///< 7
    int32  Difficulty;                                              ///< 8
    int32  ZoneID;                                                  ///< 9
    int32  Type;                                                    ///< 10
    int32  Unk10;                                                   ///< 11
    int32  MaxPlayers;                                              ///< 12
    LocalizedString const* Name;                                    ///< 13
    LocalizedString const* DifficultyString;                        ///< 14
};

struct GroupFinderCategoryEntry
{
    uint32 ID;                                                      ///< 0
    LocalizedString const* Name;                                    ///< 1
    int32  Unk1;                                                    ///< 2
    int32  Unk2;                                                    ///< 3
};

////////////////////////////////////////////////////////////////////
/// Holidays DB2
////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////
/// Import DB2
////////////////////////////////////////////////////////////////////

struct ImportPriceArmorEntry
{
    uint32  InventoryType;                                          // 0        m_ID
    float   ClothFactor;                                            // 1        m_ClothModifier
    float   LeatherFactor;                                          // 2        m_LeatherModifier
    float   MailFactor;                                             // 3        m_ChainModifier
    float   PlateFactor;                                            // 4        m_PlateModifier
};

struct ImportPriceQualityEntry
{
    uint32  QualityId;                                              ///< 0      m_ID
    float   Factor;                                                 ///< 1      m_data
};

struct ImportPriceShieldEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    float   Factor;                                                 ///< 1      m_data
};

struct ImportPriceWeaponEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    float   Factor;                                                 ///< 1      m_data
};

////////////////////////////////////////////////////////////////////
/// Item DB2
////////////////////////////////////////////////////////////////////

struct ItemClassEntry
{
    uint32  Class;                                                  ///< 0      m_ClassID
    //uint32    m_Flags;                                            ///< 1      m_Flags
    float   PriceFactor;                                            ///< 2      m_PriceModifier
    //char*     m_ClassNameLang;                                    ///< 3      m_ClassNameLang
};

struct ItemDisenchantLootEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  ItemClass;                                              ///< 1      m_Class
    int32   ItemSubClass;                                           ///< 2      m_Subclass
    uint32  ItemQuality;                                            ///< 3      m_Quality
    uint32  MinItemLevel;                                           ///< 4      m_MinLevel
    uint32  MaxItemLevel;                                           ///< 5      m_MaxLevel
    uint32  RequiredDisenchantSkill;                                ///< 6      m_SkillRequired
};


#define MAX_ITEM_ENCHANTMENT_EFFECTS 5

struct ItemRandomPropertiesEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    //char*     internalName                                        ///< 1      m_Name
    uint32  enchant_id[MAX_ITEM_ENCHANTMENT_EFFECTS];               ///< 2-6    m_Enchantment
    char*   nameSuffix;                                             ///< 7      m_name_lang
};

struct ItemSpecEntry
{
    uint32 ID;                                                      ///< 0
    uint32 MinLevel;                                                ///< 1
    uint32 MaxLevel;                                                ///< 2
    uint32 ItemType;                                                ///< 3
    uint32 PrimaryStat;                                             ///< 4
    uint32 SecondaryStat;                                           ///< 5
    uint32 SpecializationID;                                        ///< 6
};

struct ItemSpecOverrideEntry
{
    uint32  ID;                                                     ///< 1      m_ID
    uint32  itemEntry;                                              ///< 2      m_IemID
    uint32  specID;                                                 ///< 3      m_SpecID
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
    int32  SpellTrigger;                                            ///< 4
    int32  SpellCharge;                                             ///< 5
    int32  SpellCooldown;                                           ///< 6
    uint32 SpellCategory;                                           ///< 7
    int32  SpellCategoryCooldown;                                   ///< 8
    uint32 ChrSpecializationID;                                     ///< 9 @todo implement me NYI
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

////////////////////////////////////////////////////////////////////
/// Mail DB2
////////////////////////////////////////////////////////////////////

struct MailTemplateEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    //char*     subject;                                            ///< 1      m_subject_lang
    char*   content;                                                ///< 2      m_body_lang
};

////////////////////////////////////////////////////////////////////
/// Mount DB2
////////////////////////////////////////////////////////////////////

struct MountEntry
{
    uint32 Id;                                                      ///< 0
    uint32 SpellID;                                                 ///< 1
    uint32 Category;                                                ///< 2
    uint32 CreatureDisplayID;                                       ///< 3
    uint32 Flags;                                                   ///< 4
    LocalizedString const* Name;                                    ///< 5
    LocalizedString const* Description;                             ///< 6
    LocalizedString const* Icon;                                    ///< 7
    int32  FilterCategory;                                          ///< 8      Used for filter un Mount page in-game
    uint32 PlayerConditionID;                                       ///< 9
};

#define MAX_MOUNT_CAPABILITIES 23

struct MountCapabilities
{
    uint32 Capabilities[MAX_MOUNT_CAPABILITIES];
};

typedef std::unordered_map<uint32, MountCapabilities> MountCapabilitiesMap;

struct MountCapabilityEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  Flags;                                                  ///< 1      m_Flags
    uint32  RequiredRidingSkill;                                    ///< 2      m_ReqRidingSkill
    uint32  RequiredArea;                                           ///< 3      m_ReqAreaID
    uint32  RequiredAura;                                           ///< 4      m_ReqSpellAuraID
    uint32  RequiredSpell;                                          ///< 5      m_ReqSpellKnownID
    uint32  SpeedModSpell;                                          ///< 6      m_ModSpellAuraID
    int32   RequiredMap;                                            ///< 7      m_ReqMapID
};

struct MountTypeEntry
{
    uint32  Id;                                                     ///< 0      m_ID
};

struct MountTypeXCapabilityEntry
{
    uint32 ID;                                                      ///< 0
    uint32 MountTypeID;                                             ///< 1
    uint32 Index;                                                   ///< 2
    uint32 CapabilityID;                                            ///< 3
};

////////////////////////////////////////////////////////////////////
/// Name Generator DB2
////////////////////////////////////////////////////////////////////

struct NameGenEntry
{
    //uint32    id;                                                 ///< 0      m_ID
    LocalizedString const*   name;                                  ///< 1      m_Name
    uint32  race;                                                   ///< 2      m_RaceID
    uint32  gender;                                                 ///< 3      m_Sex
};

////////////////////////////////////////////////////////////////////
/// Player DB2
////////////////////////////////////////////////////////////////////

#define MAX_OUTFIT_ITEMS 24

struct CharStartOutfitEntry
{
    //uint32 m_ID;                                                  ///< 0      m_ID
    uint8  RaceID;                                                  ///< 1
    uint8  ClassID;                                                 ///< 2
    uint8  SexID;                                                   ///< 3
    uint8  OutfitID;                                                ///< 4
    int32 ItemId[MAX_OUTFIT_ITEMS];                                 ///< 5-28   m_itemID
    //uint32 m_petDisplayID;                                        ///< 29     m_petDisplayID
    //uint32 m_petFamilyId;                                         ///< 30     m_petFamilyId
};

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
    int32  AuraSpellID[5];                                          ///< 71-74
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

////////////////////////////////////////////////////////////////////
/// Quest DB2
////////////////////////////////////////////////////////////////////

struct QuestSortEntry
{
    uint32  id;                                                     ///< 0      m_ID
    //char* m_SortName_lang;                                        ///< 1      m_SortName_lang
};

struct QuestXPEntry
{
    uint32  id;                                                     ///< 0      m_ID
    uint32  Exp[10];                                                ///< 1-9    m_Difficulty
};

struct QuestV2Entry
{
    uint32      ID;                                                 ///< 0
    uint32      UniqueBitFlag;                                      ///< 1
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

////////////////////////////////////////////////////////////////////
/// Scaling DB2
////////////////////////////////////////////////////////////////////

struct ScalingStatDistributionEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  MinLevel;                                               ///< 1      m_Minlevel
    uint32  MaxLevel;                                               ///< 2      m_Maxlevel
    uint32  CurveProperties;                                        ///< 3      m_CurveProperties
};

////////////////////////////////////////////////////////////////////
/// Scenario DB2
////////////////////////////////////////////////////////////////////

struct ScenarioEntry
{
    uint32 ID;                                                      ///< 0
    char*  NameLang;                                                ///< 1
    uint32 Flags;                                                   ///< 2
};

////////////////////////////////////////////////////////////////////
/// SceneScript DB2
////////////////////////////////////////////////////////////////////

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
    uint32 Field8;                                                  ///< 8 no data
    uint32 Field9;                                                  ///< 9 no data
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

////////////////////////////////////////////////////////////////////
/// Spell DB2
////////////////////////////////////////////////////////////////////

struct SpecializationSpellEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  SpecializationEntry;                                    ///< 1      m_SpecID
    uint32  LearnSpell;                                             ///< 2      m_SpellID
    uint32  OverrideSpell;                                          ///< 3      m_OverridesSpellID
    //char*     m_DescriptionLang;                                  ///< 4      m_DescriptionLang       No data
    //uint32 unk6.2.0;                                              ///< 5      No Data
};

struct SpellCastTimesEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    int32   CastTime;                                               ///< 1      m_Base
    //float     m_PerLevel;                                         ///< 2      m_PerLevel
    //int32     m_Minimum;                                          ///< 3      m_Minimum
};



struct SpellDurationEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    int32   Duration[3];                                            ///< 1-3    m_Duration + m_DurationPerLevel + m_MaxDuration
};

struct SpellItemEnchantmentConditionEntry
{
    uint32  ID;                                                     ///< 0        m_ID
    uint8   Color[5];                                               ///< 1-5      m_lt_operandType[5]
    //uint32  LT_Operand[5];                                        ///< 6-10     m_lt_operand[5]
    uint8   Comparator[5];                                          ///< 11-15    m_operator[5]
    uint8   CompareColor[5];                                        ///< 15-20    m_rt_operandType[5]
    uint32  Value[5];                                               ///< 21-25    m_rt_operand[5]
    //uint8   Logic[5]                                              ///< 25-30    m_logic[5]
};

struct SpellProcsPerMinuteEntry
{
    uint32 Id;                                                      ///< 0        m_ID
    float  ProcsPerMinute;                                          ///< 1        m_procsPerMinute
    //bool unk;                                                     ///< 2
};

struct SpellRadiusEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    float   radiusHostile;                                          ///< 1      m_Radius
    //float     m_RadiusPerLevel;                                   ///< 2      m_RadiusPerLevel
    float   radiusFriend;                                           ///< 3      m_RadiusMin
    //float     RadiusMax                                           ///< 4      m_RadiusMax
};

struct SpellRangeEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    float   minRangeHostile;                                        ///< 1      m_RangeMin
    float   minRangeFriend;                                         ///< 2      m_RangeMin
    float   maxRangeHostile;                                        ///< 3      m_RangeMax
    float   maxRangeFriend;                                         ///< 4      m_RangeMax
    uint32  type;                                                   ///< 5      m_Flags
    //char*     Name;                                               ///< 6      m_displayName_lang
    //char*     ShortName;                                          ///< 7      m_displayNameShort_lang
};

#define MAX_SPELL_REAGENTS 8

struct SpellReagentsEntry
{
    ///uint32    Id;                                                ///< 0      m_ID
    int32  Reagent[MAX_SPELL_REAGENTS];                             ///< 1-8    m_reagent
    uint32 ReagentCount[MAX_SPELL_REAGENTS];                        ///< 9-16   m_reagentCount
};

struct SpellReagentsCurrencyEntry
{
    //uint32 ID
    uint32 SpellID;
    uint32 CurrencyID;
    uint32 CurrencyCount;
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
    uint32 SpellIconID;                                             ///< 19     m_spellIconID
    uint32 activeIconID;                                            ///< 20     m_activeIconID
    uint32 SchoolMask;                                              ///< 21     m_schoolMask
    float  MultistrikeSpeedMod;                                     ///< 22     @TODO implement me
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

struct SpellClassOptionsEntry
{
    uint32  Id;                                                     ///< 0      m_ID
    uint32  modalNextSpell;                                         ///< 1      m_modalNextSpell not used
    flag128 SpellFamilyFlags;                                       ///< 2-5
    uint32  SpellFamilyName;                                        ///< 6      m_spellClassSet
};

struct SpellXSpellVisualEntry
{
    uint32 Id;
    uint32 SpellId;
    uint32 DifficultyID;
    uint32 VisualID[MAX_SPELL_VISUAL];
    float Scale;
    uint32 ConditionID;
    uint32 Flags;
};

////////////////////////////////////////////////////////////////////
/// Sound DB2
////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////
/// Taxi DB2
////////////////////////////////////////////////////////////////////

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

struct TaxiPathEntry
{
    uint32 ID;                                                      // 0
    uint32 From;                                                    // 1
    uint32 To;                                                      // 2
    uint32 Cost;                                                    // 3
};

struct TaxiPathNodeEntry
{
    uint32 ID;                                                      // 0
    uint32 PathID;                                                  // 1
    uint32 NodeIndex;                                               // 2
    uint32 MapID;                                                   // 3
    float  x;                                                       ///< 4      m_LocX
    float  y;                                                       ///< 5      m_LocY
    float  z;                                                       ///< 6      m_LocZ
    uint32 Flags;                                                   // 7
    uint32 Delay;                                                   // 8
    uint32 ArrivalEventID;                                          // 9
    uint32 DepartureEventID;                                        // 10
};

struct TaxiNodesEntry
{
    uint32 ID;                                                      // 0
    uint32 MapID;                                                   // 1
    float  x;                                                       ///< 2
    float  y;                                                       ///< 3
    float  z;                                                       ///< 4
    LocalizedString* Name;                                          // 5
    uint32 MountCreatureID[2];                                      // 6-7
    uint32 ConditionID;                                             // 8
    uint32 LearnableIndex;                                          // 9 - some kind of index only for learnable nodes
    uint32 Flags;                                                   // 10
    float  m_MapOffsetX;                                            ///< 11
    float  m_MapOffsetY;                                            ///< 12
};

////////////////////////////////////////////////////////////////////
/// Totem DB2
////////////////////////////////////////////////////////////////////

struct TotemCategoryEntry
{
    uint32    ID;                                                   ///< 0      m_ID
    //char*   name;                                                 ///< 1      m_name_lang
    uint32    categoryType;                                         ///< 2      m_totemCategoryType     (one for specialization)
    uint32    categoryMask;                                         ///< 3      m_totemCategoryMask     (compatibility mask for same type: different for totems, compatible from high to low for rods)
};

#define MAX_SPELL_TOTEMS    2

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

////////////////////////////////////////////////////////////////////
/// Transport DB2
////////////////////////////////////////////////////////////////////

struct TransportAnimationEntry
{
    //uint32  Id;                                                   ///< 0
    uint32  TransportEntry;                                         ///< 1
    uint32  TimeSeg;                                                ///< 2
    float   X;                                                      ///< 3
    float   Y;                                                      ///< 4
    float   Z;                                                      ///< 5
    //uint32  MovementId;                                           ///< 6
};

struct TransportRotationEntry
{
    //uint32  Id;                                                   ///< 0
    uint32  TransportEntry;                                         ///< 1
    uint32  TimeSeg;                                                ///< 2
    float   X;                                                      ///< 3
    float   Y;                                                      ///< 4
    float   Z;                                                      ///< 5
    float   W;                                                      ///< 6
};

////////////////////////////////////////////////////////////////////
/// World DB2
////////////////////////////////////////////////////////////////////

#define MAX_WORLD_MAP_OVERLAY_AREA_IDX 4

struct WorldMapOverlayEntry
{
    uint32  ID;                                                     ///< 0      m_ID
    //uint32    worldMapAreaId;                                     ///< 1      m_MapAreaID
    uint32  areatableID[MAX_WORLD_MAP_OVERLAY_AREA_IDX];            ///< 2-5    m_AreaID
    //char*     mTextureName;                                       ///< 6      m_TextureName
    //uint32    m_TextureWidth;                                     ///< 7      m_TextureWidth
    //uint32    m_TextureHeight;                                    ///< 8      m_TextureHeight
    //uint32    m_OffsetX;                                          ///< 9      m_OffsetX
    //uint32    m_OffsetY;                                          ///< 10     m_OffsetY
    //uint32    m_HitRectTop;                                       ///< 11     m_HitRectTop
    //uint32    m_HitRectLeft;                                      ///< 12     m_HitRectLeft
    //uint32    m_HitRectBottom;                                    ///< 13     m_HitRectBottom
    //uint32    m_HitRectRight;                                     ///< 14     m_HitRectRight
    //uint32    m_PlayerConditionID;                                ///< 15     m_PlayerConditionID
};

/// GCC has alternative #pragma pack(N) syntax and old gcc version does not support pack(push, N), also any gcc version does not support it at some platform
#if defined(__GNUC__)
#pragma pack()
#else
#pragma pack(pop)
#endif

struct TaxiPathBySourceAndDestination
{
    TaxiPathBySourceAndDestination() : ID(0), price(0) { }
    TaxiPathBySourceAndDestination(uint32 _id, uint32 _price) : ID(_id), price(_price) { }

    uint32 ID;
    uint32 price;
};

typedef std::map<uint32, TaxiPathBySourceAndDestination> TaxiPathSetForSource;
typedef std::map<uint32, TaxiPathSetForSource> TaxiPathSetBySource;

typedef std::vector<TaxiPathNodeEntry const*> TaxiPathNodeList;
typedef std::vector<TaxiPathNodeList> TaxiPathNodesByPath;

#define TaxiMaskSize 217
typedef std::array<uint8, TaxiMaskSize> TaxiMask;

#endif
