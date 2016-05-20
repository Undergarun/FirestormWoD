////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef TRINITY_DBCSFRM_H
#define TRINITY_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - bool, p - field present in sql dbc, a - field absent in sql dbc

char const AnimKitfmt[]                       = "nxxx";
const char AreaTableEntryfmt[]                = "iiinixxxxxxxisiiiiixxxxxxxxxx";
const char AreaTriggerEntryfmt[]              = "nifffxxxfffffxxxx";
const char ArmorLocationfmt[]                 = "nfffff";
const char BankBagSlotPricesEntryfmt[]        = "ni";
const char BattlemasterListEntryfmt[]         = "niiiiiiiiiiiiiiiiixsxiiiixxxxxx";
const char CharTitlesEntryfmt[]               = "nxsxix";
const char ChatChannelsEntryfmt[]             = "nixsx";
const char ChrClassesEntryfmt[]               = "nixsxxxixiiiiixxxxi";
const char ChrRacesEntryfmt[]                 = "niixiixixxxxixsxxxxxxxxxxxxxxxxxxxxxxxxx";
const char ChrSpecializationsfmt[]            = "nsiixiiixxisxsxi";
const char CinematicCameraEntryfmt[]          = "nsiffff";
char const CreatureDisplayInfoExtrafmt[]      = "dixxxxxxxxxxxxxxxxxxxx";
const char CreatureModelDatafmt[]             = "nixxxxxxxxxxxxxffxxxxxxxxxxxxxxxxx";
const char CreatureFamilyfmt[]                = "nfifiiiiixsx";
const char DifficultyEntryfmt[]               = "niiiiiiiiiisix";
const char DungeonEncounterfmt[]              = "niiiisiii";
const char DurabilityCostsfmt[]               = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char EmotesEntryfmt[]                   = "nsiiiiii";
const char EmotesTextEntryfmt[]               = "nxixxxxxxxxxxxxxxxx";
const char FactionEntryfmt[]                  = "niiiiiiiiiiiiiiiiiiffiissiii";
const char FactionTemplateEntryfmt[]          = "niiiiiiiiiiiii";
const char FileDatafmt[]                      = "nss";
const char GameObjectDisplayInfofmt[]         = "nixxxxxxxxxxffffffxxx";
const char GemPropertiesEntryfmt[]            = "nixxii";
const char GlyphPropertiesfmt[]               = "niiii";
char const gtArmorMitigationByLvlfmt[]        = "df";
const char GtBarberShopCostBasefmt[]          = "xf";
const char GtCombatRatingsfmt[]               = "xf";
const char GtOCTHpPerStaminafmt[]             = "df";
const char GtChanceToMeleeCritBasefmt[]       = "xf";
const char GtChanceToMeleeCritfmt[]           = "xf";
const char GtChanceToSpellCritBasefmt[]       = "xf";
const char GtChanceToSpellCritfmt[]           = "xf";
const char GtOCTLevelExperiencefmt[]          = "nf";
const char GtOCTRegenHPfmt[]                  = "f";
const char GtRegenMPPerSptfmt[]               = "xf";
const char GtSpellScalingfmt[]                = "df";
const char GtOCTBaseHPByClassfmt[]            = "df";
const char GtOCTBaseMPByClassfmt[]            = "df";
const char ItemSetSpellFmt[]                  = "diiii";
const char ItemBagFamilyfmt[]                 = "nx";
const char ItemArmorQualityfmt[]              = "nfffffffi";
const char ItemArmorShieldfmt[]               = "nifffffff";
const char ItemArmorTotalfmt[]                = "niffff";
const char ItemDamagefmt[]                    = "nfffffffi";
const char gtItemSocketCostPerLevelfmt[]      = "df";
const char ItemSetEntryfmt[]                  = "dsiiiiiiiiiiiiiiiiiii";
const char LFGDungeonEntryfmt[]               = "nxiiiiiiiiixxixixiiiixxxxixxxx";
const char LiquidTypefmt[]                    = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char LockEntryfmt[]                     = "niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
const char PhaseEntryfmt[]                    = "ni";
const char MapEntryfmt[]                      = "nxiixxsixxixiffxiiiixx";
const char MapDifficultyEntryfmt[]            = "niisiiii";
const char MinorTalentfmt[]                   = "diii";
const char MovieEntryfmt[]                    = "nxxxx";
const char PowerDisplayfmt[]                  = "nisbbb";
const char PvPDifficultyfmt[]                 = "diiii";
const char QuestFactionRewardfmt[]            = "niiiiiiiiii";
const char RandomPropertiesPointsfmt[]        = "niiiiiiiiiiiiiii";
const char ScenarioStepEntryfmt[]             = "niiissiiii";
const char ScalingStatValuesfmt[]             = "iniiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiix";
const char SkillLinefmt[]                     = "nisxixiii";
const char SkillLineAbilityfmt[]              = "niiiiiiiiiixx";
const char SpellCategoriesEntryfmt[]          = "niiiiiiiii";
const char SpellCategoryEntryfmt[]            = "niisii";
const char SpellDifficultyfmt[]               = "niiii";
const std::string CustomSpellDifficultyfmt    = "ppppp";
const std::string CustomSpellDifficultyIndex  = "id";
const char SpellEffectEntryfmt[]              = "niifiiiffiiiiiifiifiiiiixiiiixf";
const char SpellEffectScalingEntryfmt[]       = "xfffn";
const char SpellEntryfmt[]                    = "nssxxixxiiiiiiiiiiiiiiii";
const std::string CustomSpellEntryfmt         = "papppppppppppapapaaaaaaaaaaapaaapapppppppaaaaapaapaaaaaaaaaaaaaaaaaappppppppppppppppppppppppppppppppppppaaaaaapppppppppaaapppppppppaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaappppppppapppaaaaappaaaaaaa";
const std::string CustomSpellEntryIndex       = "Id";
const char SpellFocusObjectfmt[]              = "nx";
const char SpellItemEnchantmentfmt[]          = "nxiiiiiiiiisiiiiiiiiiiifff";
const char SpellScalingEntryfmt[]             = "diiiifiii";
const char SpellTargetRestrictionsEntryfmt[]  = "niiffiiii";
const char SpellInterruptsEntryfmt[]          = "niiiiiii";
const char SpellEquippedItemsEntryfmt[]       = "niiiii";
const char SpellAuraOptionsEntryfmt[]         = "niiiiiiii";
const char SpellCooldownsEntryfmt[]           = "niiiii";
const char SpellLevelsEntryfmt[]              = "niiiii";
const char SpellShapeshiftEntryfmt[]          = "niiiii";
const char SpellShapeshiftFormfmt[]           = "nxxiixiiiiiiiiiiiiixx";
const char StableSlotPricesfmt[]              = "ni";
const char SummonPropertiesfmt[]              = "niiiii";
const char TalentEntryfmt[]                   = "niiiiiiiiis";
const char VehicleEntryfmt[]                  = "nixffffiiiiiiiifffffffffffffffssssfifiixx";
const char VehicleSeatEntryfmt[]              = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiixxxxxxxxxxxxxxxxxxxxx";
const std::string CustomVehicleSeatEntryfmt   = "ppppppaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaapaaaaaaaaaaaaaaaaaaaaa";
const std::string CustomVehicleSeatEntryIndex = "ID";
const char WMOAreaTableEntryfmt[]             = "niiixxxxxiixxxx";
const char WorldMapAreaEntryfmt[]             = "xinxffffixxxii";
const char World_PVP_AreaEntryfmt[]           = "niiiiiii";
const char WorldSafeLocsEntryfmt[]            = "niffffx";
const char GtBattlePetTypeDamageModfmt[]      = "xf";
const char GtBattlePetXPfmt[]                 = "xf";
const char WorldStateEntryfmt[]               = "n";
char const WorldMapTransformsfmt[]            = "diffffffiffxxxf";
const char WorldStateExpressionEntryfmt[]     = "ns";

#endif
