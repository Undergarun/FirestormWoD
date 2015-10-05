/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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

#ifndef TRINITY_DBCSFRM_H
#define TRINITY_DBCSFRM_H

// x - skip<uint32>, X - skip<uint8>, s - char*, f - float, i - uint32, b - uint8, d - index (not included)
// n - index (included), l - bool, p - field present in sql dbc, a - field absent in sql dbc

const char Achievementfmt[]                   = "niiissiiiiisiii";
const std::string CustomAchievementfmt        = "pppaaaapapaappa";
const std::string CustomAchievementIndex      = "ID";
char const AnimKitfmt[] = "nxxx";
const char AchievementCriteriafmt[]           = "niiiixiiiisiiiiixxiiiii";
const char AreaTableEntryfmt[]                = "iiinixxxxxxxisiiiiixxxxxxxxxx";
const char AreaGroupEntryfmt[]                = "niiiiiii";
const char AreaTriggerEntryfmt[]              = "nifffxxxfffffxxxx";
const char ArmorLocationfmt[]                 = "nfffff";
const char AuctionHouseEntryfmt[]             = "niiix";
const char BankBagSlotPricesEntryfmt[]        = "ni";
const char BarberShopStyleEntryfmt[]          = "nixxxiii";
const char BattlemasterListEntryfmt[]         = "niiiiiiiiiiiiiiiiixsxiiiixxxxxx";
const char CharStartOutfitEntryfmt[]          = "dbbbbiiiiiiiiiiiiiiiiiiiiiiiixx";
const char CharTitlesEntryfmt[]               = "nxsxix";
const char ChatChannelsEntryfmt[]             = "nixsx";
const char ChrClassesEntryfmt[]               = "nixsxxxixiiiiixxxxi";
const char ChrRacesEntryfmt[]                 = "niixiixixxxxixsxxxxxxxxxxxxxxxxxxxxxxxxx";
const char ChrClassesXPowerTypesfmt[]         = "nii";
const char ChrSpecializationsfmt[]            = "nsiixiiixxisxsxi";
const char CinematicCameraEntryfmt[]          = "nsiffff";
const char CinematicSequencesEntryfmt[]       = "nxixxxxxxx";
const char CreatureDisplayInfofmt[]           = "nixifxxxxxxxxxxxxxxxx";
char const CreatureDisplayInfoExtrafmt[]      = "dixxxxxxxxxxxxxxxxxxxx";
const char CreatureModelDatafmt[]             = "nixxxxxxxxxxxxxffxxxxxxxxxxxxxxxxx";
const char CreatureFamilyfmt[]                = "nfifiiiiixsx";
const char CreatureSpellDatafmt[]             = "niiiixxxx";
const char CreatureTypefmt[]                  = "nxx";
const char DestructibleModelDatafmt[]         = "ixxixxxixxxixxxixxxxxxxx";
const char DifficultyEntryfmt[]               = "niiiiiiiiiisi";
const char DungeonEncounterfmt[]              = "iiiiisiii";
const char DurabilityCostsfmt[]               = "niiiiiiiiiiiiiiiiiiiiiiiiiiiii";
const char DurabilityQualityfmt[]             = "nf";
const char EmotesEntryfmt[]                   = "nsiiiiii";
const char EmotesTextEntryfmt[]               = "nxixxxxxxxxxxxxxxxx";
const char FactionEntryfmt[]                  = "niiiiiiiiiiiiiiiiiiffiissiii";
const char FactionTemplateEntryfmt[]          = "niiiiiiiiiiiii";
const char FileDatafmt[]                      = "nss";
const char GameObjectDisplayInfofmt[]         = "nixxxxxxxxxxffffffxxx";
const char GemPropertiesEntryfmt[]            = "nixxii";
const char GlyphPropertiesfmt[]               = "niiii";
const char GlyphSlotfmt[]                     = "nii";
char const gtArmorMitigationByLvlfmt[]        = "df";
const char GtBarberShopCostBasefmt[]          = "xf";
const char GtCombatRatingsfmt[]               = "xf";
const char GtOCTHpPerStaminafmt[]             = "df";
const char GtChanceToMeleeCritBasefmt[]       = "xf";
const char GtChanceToMeleeCritfmt[]           = "xf";
const char GtChanceToSpellCritBasefmt[]       = "xf";
const char GtChanceToSpellCritfmt[]           = "xf";
const char GtOCTClassCombatRatingScalarfmt[]  = "df";
const char GtOCTLevelExperiencefmt[]          = "nf";
const char GtOCTRegenHPfmt[]                  = "f";
//const char GtOCTRegenMPfmt[]                ="f";
const char GtRegenMPPerSptfmt[]               = "xf";
const char GtSpellScalingfmt[]                = "df";
const char GtOCTBaseHPByClassfmt[]            = "df";
const char GtOCTBaseMPByClassfmt[]            = "df";
const char GuildPerkSpellsfmt[]               = "dii";
const char ImportPriceArmorfmt[]              = "nffff";
const char ImportPriceQualityfmt[]            = "nf";
const char ImportPriceShieldfmt[]             = "nf";
const char ImportPriceWeaponfmt[]             = "nf";
const char ItemSetSpellFmt[]                  = "diiii";
const char ItemPriceBasefmt[]                 = "diff";
const char ItemBagFamilyfmt[]                 = "nx";
const char ItemArmorQualityfmt[]              = "nfffffffi";
const char ItemArmorShieldfmt[]               = "nifffffff";
const char ItemArmorTotalfmt[]                = "niffff";
const char ItemClassfmt[]                     = "difx";
const char ItemDamagefmt[]                    = "nfffffffi";
const char gtItemSocketCostPerLevelfmt[]      = "df";
const char CriteriaTreefmt[]                  = "niiiiiis";
const char Criteriafmt[]                      = "niiiiiiiiiii";
const char ModifierTreefmt[]                  = "niiiiii";
const char ItemDisenchantLootfmt[]            = "niiiiii";
//const char ItemDisplayInfoEntryfmt[]        = "nxxxxxxxxxxxxxxxxxxxxxx";
const char ItemLimitCategoryEntryfmt[]        = "nxii";
const char ItemRandomPropertiesfmt[]          = "nxiiiiis";
const char ItemRandomSuffixfmt[]              = "nsxiiiiiiiiii";
const char ItemSetEntryfmt[]                  = "dsiiiiiiiiiiiiiiiiiii";
const char ItemSpecEntryfmt[]                 = "iiiiiii";
const char ItemSpecOverrideEntryfmt[]         = "nii";
const char LFGDungeonEntryfmt[]               = "nxiiiiiiiiixxixixiiiixxxxixxx";
const char LiquidTypefmt[]                    = "nxxixixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
const char LockEntryfmt[]                     = "niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx";
const char PhaseEntryfmt[]                    = "ni";
const char MailTemplateEntryfmt[]             = "nxs";
const char MapEntryfmt[]                      = "nxiixxsixxixiffxiiiixx";
const char MapDifficultyEntryfmt[]            = "niisiiii";
const char MinorTalentfmt[]                   = "diii";
const char MovieEntryfmt[]                    = "nxxxx";
const char MountCapabilityfmt[]               = "niiiiiii";
const char MountTypefmt[]                     = "niiiiiiiiiiiiiiiiiiiiiiii";
const char NameGenfmt[]                       = "dsii";
const char PowerDisplayfmt[]                  = "nisbbb";
const char PvPDifficultyfmt[]                 = "diiii";
const char QuestSortEntryfmt[]                = "nx";
char const QuestV2fmt[]                       = "ni";
const char QuestXPfmt[]                       = "niiiiiiiiii";
const char QuestFactionRewardfmt[]            = "niiiiiiiiii";
const char QuestPOIPointfmt[]                 = "niii";
const char RandomPropertiesPointsfmt[]        = "niiiiiiiiiiiiiii";
const char ResearchBranchfmt[]                = "ixxixi";
const char ResearchProjectfmt[]               = "nxxiiixxi";
const char ResearchSitefmt[]                  = "niixx";
const char ScalingStatDistributionfmt[]       = "niii";
const char ScenarioEntryfmt[]                 = "nsi";
const char ScenarioStepEntryfmt[]             = "niiissiiii";
const char ScalingStatValuesfmt[]             = "iniiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiix";
const char SkillLinefmt[]                     = "nisxixiii";
const char SkillLineAbilityfmt[]              = "niiiiiiiiiixx";
const char SpecializationSpellsfmt[]          = "niiix";
const char SpellCastTimefmt[]                 = "nixx";
const char SpellCategoriesEntryfmt[]          = "niiiiiiiii";
const char SpellCategoryEntryfmt[]            = "niisii";
const char SpellDifficultyfmt[]               = "niiii";
const std::string CustomSpellDifficultyfmt    = "ppppp";
const std::string CustomSpellDifficultyIndex  = "id";
const char SpellDurationfmt[]                 = "niii";
const char SpellEffectEntryfmt[]              = "niifiiiffiiiiiifiifiiiiixiiiixf";
const char SpellEffectScalingEntryfmt[]       = "xfffn";
const char SpellEntryfmt[]                    = "nssxxixxiiiiiiiiiiiiiiii";
const std::string CustomSpellEntryfmt         = "papppppppppppapapaaaaaaaaaaapaaapapppppppaaaaapaapaaaaaaaaaaaaaaaaaappppppppppppppppppppppppppppppppppppaaaaaapppppppppaaapppppppppaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaappppppppapppaaaaappaaaaaaa";
const std::string CustomSpellEntryIndex       = "Id";
const char SpellFocusObjectfmt[]              = "nx";
const char SpellItemEnchantmentfmt[]          = "nxiiiiiiiiisiiiiiiiiiiifff";
const char SpellItemEnchantmentConditionfmt[] = "nbbbbbxxxxxbbbbbbbbbbiiiiixxxxx";
const char SpellRadiusfmt[]                   = "nfxxf";
const char SpellRangefmt[]                    = "nffffixx";
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
const char TotemCategoryEntryfmt[]            = "nxii";
char const TransportAnimationfmt[]            = "diifffx";
char const TransportRotationfmt[]             = "diiffff";
const char VehicleEntryfmt[]                  = "nixffffiiiiiiiifffffffffffffffssssfifiixx";
const char VehicleSeatEntryfmt[]              = "niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiixxxxxxxxxxxxxxxxxxxxx";
const std::string CustomVehicleSeatEntryfmt   = "ppppppaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaapaaaaaaaaaaaaaaaaaaaaa";
const std::string CustomVehicleSeatEntryIndex = "ID";
const char WMOAreaTableEntryfmt[]             = "niiixxxxxiixxxx";
const char WorldMapAreaEntryfmt[]             = "xinxffffixxxii";
const char WorldMapOverlayEntryfmt[]          = "nxiiiixxxxxxxxxx";
const char World_PVP_AreaEntryfmt[]           = "niiiiiii";
const char WorldSafeLocsEntryfmt[]            = "niffffx";
const char GtBattlePetTypeDamageModfmt[]      = "xf";
const char GtBattlePetXPfmt[]                 = "xf";
const char SpellProcsPerMinuteEntryfmt[]      = "nfx";
const char WorldStateEntryfmt[]               = "n";
const char WorldStateExpressionEntryfmt[]     = "ns";

#endif