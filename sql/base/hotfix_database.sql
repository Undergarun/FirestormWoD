# ************************************************************
# Sequel Pro SQL dump
# Version 4135
#
# http://www.sequelpro.com/
# http://code.google.com/p/sequel-pro/
#
# Host: 127.0.0.1 (MySQL 5.5.38)
# Database: hotfix
# Generation Time: 2015-06-19 14:02:40 +0000
# ************************************************************


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;


# Dump of table _custom_items
# ------------------------------------------------------------

DROP TABLE IF EXISTS `_custom_items`;

CREATE TABLE `_custom_items` (
  `ID` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table _hashs
# ------------------------------------------------------------

DROP TABLE IF EXISTS `_hashs`;

CREATE TABLE `_hashs` (
  `ID` int(11) unsigned NOT NULL,
  `Name` varchar(100) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table _hotfixs
# ------------------------------------------------------------

DROP TABLE IF EXISTS `_hotfixs`;

CREATE TABLE `_hotfixs` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Entry` int(11) unsigned NOT NULL,
  `Hash` int(11) unsigned NOT NULL,
  `Date` int(11) unsigned NOT NULL,
  `Comment` varchar(100) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table battle_pet_species
# ------------------------------------------------------------

DROP TABLE IF EXISTS `battle_pet_species`;

CREATE TABLE `battle_pet_species` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Entry` int(11) unsigned NOT NULL,
  `IconID` int(11) unsigned NOT NULL,
  `SpellID` int(11) unsigned NOT NULL,
  `Type` int(11) unsigned NOT NULL,
  `ObtainmentCategoryDescription` int(11) NOT NULL,
  `Flags` int(11) unsigned NOT NULL,
  `Source` text,
  `Description` text,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table battle_pet_species_locale
# ------------------------------------------------------------

DROP TABLE IF EXISTS `battle_pet_species_locale`;

CREATE TABLE `battle_pet_species_locale` (
  `ID` int(11) unsigned NOT NULL,
  `Source_koKR` text CHARACTER SET latin1,
  `Source_frFR` text CHARACTER SET latin1,
  `Source_deDE` text CHARACTER SET latin1,
  `Source_zhCN` text CHARACTER SET latin1,
  `Source_zhTW` text CHARACTER SET latin1,
  `Source_esES` text CHARACTER SET latin1,
  `Source_esMX` text CHARACTER SET latin1,
  `Source_ruRU` text CHARACTER SET latin1,
  `Source_ptPT` text CHARACTER SET latin1,
  `Source_itIT` text CHARACTER SET latin1,
  `Description_koKR` text CHARACTER SET latin1,
  `Description_frFR` text CHARACTER SET latin1,
  `Description_deDE` text CHARACTER SET latin1,
  `Description_zhCN` text CHARACTER SET latin1,
  `Description_zhTW` text CHARACTER SET latin1,
  `Description_esES` text CHARACTER SET latin1,
  `Description_esMX` text CHARACTER SET latin1,
  `Description_ruRU` text CHARACTER SET latin1,
  `Description_ptPT` text CHARACTER SET latin1,
  `Description_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



# Dump of table currency_types
# ------------------------------------------------------------

DROP TABLE IF EXISTS `currency_types`;

CREATE TABLE `currency_types` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Category` int(11) unsigned NOT NULL DEFAULT '0',
  `NameLang` text,
  `InventoryIcon` text,
  `InventoryIcon2` text,
  `SpellWeight` int(11) unsigned NOT NULL DEFAULT '0',
  `SpellCategory` int(11) unsigned NOT NULL DEFAULT '0',
  `TotalCap` int(11) unsigned NOT NULL DEFAULT '0',
  `WeekCap` int(11) unsigned NOT NULL DEFAULT '0',
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  `Quality` int(11) unsigned NOT NULL DEFAULT '0',
  `DescriptionLang` text,
  `BuildVerified` int(11) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table currency_types_locale
# ------------------------------------------------------------

DROP TABLE IF EXISTS `currency_types_locale`;

CREATE TABLE `currency_types_locale` (
  `ID` int(11) unsigned NOT NULL,
  `NameLang_koKR` text CHARACTER SET latin1,
  `NameLang_frFR` text CHARACTER SET latin1,
  `NameLang_deDE` text CHARACTER SET latin1,
  `NameLang_zhCN` text CHARACTER SET latin1,
  `NameLang_zhTW` text CHARACTER SET latin1,
  `NameLang_esES` text CHARACTER SET latin1,
  `NameLang_esMX` text CHARACTER SET latin1,
  `NameLang_ruRU` text CHARACTER SET latin1,
  `NameLang_ptPT` text CHARACTER SET latin1,
  `NameLang_itIT` text CHARACTER SET latin1,
  `InventoryIcon_koKR` text CHARACTER SET latin1,
  `InventoryIcon_frFR` text CHARACTER SET latin1,
  `InventoryIcon_deDE` text CHARACTER SET latin1,
  `InventoryIcon_zhCN` text CHARACTER SET latin1,
  `InventoryIcon_zhTW` text CHARACTER SET latin1,
  `InventoryIcon_esES` text CHARACTER SET latin1,
  `InventoryIcon_esMX` text CHARACTER SET latin1,
  `InventoryIcon_ruRU` text CHARACTER SET latin1,
  `InventoryIcon_ptPT` text CHARACTER SET latin1,
  `InventoryIcon_itIT` text CHARACTER SET latin1,
  `InventoryIcon2_koKR` text CHARACTER SET latin1,
  `InventoryIcon2_frFR` text CHARACTER SET latin1,
  `InventoryIcon2_deDE` text CHARACTER SET latin1,
  `InventoryIcon2_zhCN` text CHARACTER SET latin1,
  `InventoryIcon2_zhTW` text CHARACTER SET latin1,
  `InventoryIcon2_esES` text CHARACTER SET latin1,
  `InventoryIcon2_esMX` text CHARACTER SET latin1,
  `InventoryIcon2_ruRU` text CHARACTER SET latin1,
  `InventoryIcon2_ptPT` text CHARACTER SET latin1,
  `InventoryIcon2_itIT` text CHARACTER SET latin1,
  `DescriptionLang_koKR` text CHARACTER SET latin1,
  `DescriptionLang_frFR` text CHARACTER SET latin1,
  `DescriptionLang_deDE` text CHARACTER SET latin1,
  `DescriptionLang_zhCN` text CHARACTER SET latin1,
  `DescriptionLang_zhTW` text CHARACTER SET latin1,
  `DescriptionLang_esES` text CHARACTER SET latin1,
  `DescriptionLang_esMX` text CHARACTER SET latin1,
  `DescriptionLang_ruRU` text CHARACTER SET latin1,
  `DescriptionLang_ptPT` text CHARACTER SET latin1,
  `DescriptionLang_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



# Dump of table curve_point
# ------------------------------------------------------------

DROP TABLE IF EXISTS `curve_point`;

CREATE TABLE `curve_point` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CurveID` int(11) unsigned NOT NULL,
  `Index` int(11) unsigned NOT NULL,
  `X` float NOT NULL,
  `Y` float NOT NULL,
  `BuildVerified` int(11) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table item
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item`;

CREATE TABLE `item` (
  `ID` int(11) unsigned NOT NULL DEFAULT '0',
  `Class` int(11) NOT NULL DEFAULT '0',
  `SubClass` int(11) NOT NULL DEFAULT '0',
  `SoundOverrideSubclass` int(11) NOT NULL DEFAULT '0',
  `Material` int(11) NOT NULL DEFAULT '0',
  `InventoryType` int(11) NOT NULL DEFAULT '0',
  `Sheath` int(11) NOT NULL DEFAULT '0',
  `DisplayId` int(11) NOT NULL DEFAULT '0',
  `GroupSoundsID` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table item_appearance
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_appearance`;

CREATE TABLE `item_appearance` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `DisplayID` int(11) unsigned NOT NULL,
  `Unk` int(11) unsigned NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table item_bonus
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_bonus`;

CREATE TABLE `item_bonus` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Entry` int(11) NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  `Value1` int(11) NOT NULL DEFAULT '0',
  `Value2` int(11) NOT NULL DEFAULT '0',
  `Index` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table item_bonus_tree_node
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_bonus_tree_node`;

CREATE TABLE `item_bonus_tree_node` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Category` int(11) NOT NULL DEFAULT '0',
  `Difficulty` int(11) NOT NULL DEFAULT '0',
  `LinkedCategory` int(11) NOT NULL DEFAULT '0',
  `ItemBonusEntry` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table item_currency_cost
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_currency_cost`;

CREATE TABLE `item_currency_cost` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `ItemID` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table item_effect
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_effect`;

CREATE TABLE `item_effect` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ItemID` int(10) unsigned NOT NULL,
  `EffectIndex` int(10) unsigned NOT NULL,
  `SpellID` int(10) unsigned NOT NULL,
  `SpellTrigger` int(10) NOT NULL,
  `SpellCharge` int(10) NOT NULL,
  `SpellCooldown` int(10) NOT NULL,
  `SpellCategory` int(10) unsigned NOT NULL,
  `SpellCategoryCooldown` int(10) NOT NULL,
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table item_extended_cost
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_extended_cost`;

CREATE TABLE `item_extended_cost` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `RequiredArenaSlot` int(11) NOT NULL DEFAULT '0',
  `RequiredItem1` int(11) NOT NULL DEFAULT '0',
  `RequiredItem2` int(11) NOT NULL DEFAULT '0',
  `RequiredItem3` int(11) NOT NULL DEFAULT '0',
  `RequiredItem4` int(11) NOT NULL DEFAULT '0',
  `RequiredItem5` int(11) NOT NULL DEFAULT '0',
  `RequiredItemCount1` int(11) NOT NULL DEFAULT '0',
  `RequiredItemCount2` int(11) NOT NULL DEFAULT '0',
  `RequiredItemCount3` int(11) NOT NULL DEFAULT '0',
  `RequiredItemCount4` int(11) NOT NULL DEFAULT '0',
  `RequiredItemCount5` int(11) NOT NULL DEFAULT '0',
  `RequiredPersonalArenaRating` int(11) NOT NULL DEFAULT '0',
  `ItemPurchaseGroup` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrency1` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrency2` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrency3` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrency4` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrency5` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrencyCount1` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrencyCount2` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrencyCount3` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrencyCount4` int(11) NOT NULL DEFAULT '0',
  `RequiredCurrencyCount5` int(11) NOT NULL DEFAULT '0',
  `RequiredFactionId` int(11) NOT NULL DEFAULT '0',
  `RequiredFactionStanding` int(11) NOT NULL DEFAULT '0',
  `RequirementFlags` int(11) NOT NULL DEFAULT '0',
  `RequiredAchievement` int(11) NOT NULL DEFAULT '0',
  `OverrideBuyPrice` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table item_modified_appearance
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_modified_appearance`;

CREATE TABLE `item_modified_appearance` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `ItemID` int(10) unsigned NOT NULL,
  `Unk` int(10) NOT NULL,
  `AppearanceID` int(10) unsigned NOT NULL,
  `Unk2` int(10) NOT NULL,
  `Index` int(10) unsigned NOT NULL,
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table item_sparse
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_sparse`;

CREATE TABLE `item_sparse` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Quality` int(11) NOT NULL DEFAULT '0',
  `Flags` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags2` int(10) unsigned NOT NULL DEFAULT '0',
  `Flags3` int(10) unsigned NOT NULL DEFAULT '0',
  `Unk430_1` float NOT NULL DEFAULT '0',
  `Unk430_2` float NOT NULL DEFAULT '0',
  `BuyCount` int(11) NOT NULL DEFAULT '0',
  `BuyPrice` int(11) NOT NULL DEFAULT '0',
  `SellPrice` int(11) NOT NULL DEFAULT '0',
  `InventoryType` int(11) NOT NULL DEFAULT '0',
  `AllowableClass` int(11) NOT NULL DEFAULT '0',
  `AllowableRace` int(11) NOT NULL DEFAULT '0',
  `ItemLevel` int(11) NOT NULL DEFAULT '0',
  `RequiredLevel` int(11) NOT NULL DEFAULT '0',
  `RequiredSkill` int(11) NOT NULL DEFAULT '0',
  `RequiredSkillRank` int(11) NOT NULL DEFAULT '0',
  `RequiredSpell` int(11) NOT NULL DEFAULT '0',
  `RequiredHonorRank` int(11) NOT NULL DEFAULT '0',
  `RequiredCityRank` int(11) NOT NULL DEFAULT '0',
  `RequiredReputationFaction` int(11) NOT NULL DEFAULT '0',
  `RequiredReputationRank` int(11) NOT NULL DEFAULT '0',
  `MaxCount` int(11) NOT NULL DEFAULT '0',
  `Stackable` int(11) NOT NULL DEFAULT '0',
  `ContainerSlots` int(11) NOT NULL DEFAULT '0',
  `ItemStatType0` int(11) NOT NULL DEFAULT '0',
  `ItemStatType1` int(11) NOT NULL DEFAULT '0',
  `ItemStatType2` int(11) NOT NULL DEFAULT '0',
  `ItemStatType3` int(11) NOT NULL DEFAULT '0',
  `ItemStatType4` int(11) NOT NULL DEFAULT '0',
  `ItemStatType5` int(11) NOT NULL DEFAULT '0',
  `ItemStatType6` int(11) NOT NULL DEFAULT '0',
  `ItemStatType7` int(11) NOT NULL DEFAULT '0',
  `ItemStatType8` int(11) NOT NULL DEFAULT '0',
  `ItemStatType9` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue0` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue1` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue2` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue3` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue4` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue5` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue6` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue7` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue8` int(11) NOT NULL DEFAULT '0',
  `ItemStatValue9` int(11) NOT NULL DEFAULT '0',
  `ScalingValue0` int(11) NOT NULL DEFAULT '0',
  `ScalingValue1` int(11) NOT NULL DEFAULT '0',
  `ScalingValue2` int(11) NOT NULL DEFAULT '0',
  `ScalingValue3` int(11) NOT NULL DEFAULT '0',
  `ScalingValue4` int(11) NOT NULL DEFAULT '0',
  `ScalingValue5` int(11) NOT NULL DEFAULT '0',
  `ScalingValue6` int(11) NOT NULL DEFAULT '0',
  `ScalingValue7` int(11) NOT NULL DEFAULT '0',
  `ScalingValue8` int(11) NOT NULL DEFAULT '0',
  `ScalingValue9` int(11) NOT NULL DEFAULT '0',
  `SocketCostRate0` float NOT NULL DEFAULT '0',
  `SocketCostRate1` float NOT NULL DEFAULT '0',
  `SocketCostRate2` float NOT NULL DEFAULT '0',
  `SocketCostRate3` float NOT NULL DEFAULT '0',
  `SocketCostRate4` float NOT NULL DEFAULT '0',
  `SocketCostRate5` float NOT NULL DEFAULT '0',
  `SocketCostRate6` float NOT NULL DEFAULT '0',
  `SocketCostRate7` float NOT NULL DEFAULT '0',
  `SocketCostRate8` float NOT NULL DEFAULT '0',
  `SocketCostRate9` float NOT NULL DEFAULT '0',
  `ScalingStatDistribution` int(11) NOT NULL DEFAULT '0',
  `DamageType` int(11) NOT NULL DEFAULT '0',
  `Delay` int(11) NOT NULL DEFAULT '0',
  `RangedModRange` float NOT NULL DEFAULT '0',
  `Bonding` int(11) NOT NULL DEFAULT '0',
  `Name` text NOT NULL,
  `Name2` text NOT NULL,
  `Name3` text NOT NULL,
  `Name4` text NOT NULL,
  `Description` text NOT NULL,
  `PageText` int(11) NOT NULL DEFAULT '0',
  `LanguageID` int(11) NOT NULL DEFAULT '0',
  `PageMaterial` int(11) NOT NULL DEFAULT '0',
  `StartQuest` int(11) NOT NULL DEFAULT '0',
  `LockID` int(11) NOT NULL DEFAULT '0',
  `Material` int(11) NOT NULL DEFAULT '0',
  `Sheath` int(11) NOT NULL DEFAULT '0',
  `RandomProperty` int(11) NOT NULL DEFAULT '0',
  `RandomSuffix` int(11) NOT NULL DEFAULT '0',
  `ItemSet` int(11) NOT NULL DEFAULT '0',
  `Area` int(11) NOT NULL DEFAULT '0',
  `Map` int(11) NOT NULL DEFAULT '0',
  `BagFamily` int(11) NOT NULL DEFAULT '0',
  `TotemCategory` int(11) NOT NULL DEFAULT '0',
  `Color1` int(11) NOT NULL DEFAULT '0',
  `Color2` int(11) NOT NULL DEFAULT '0',
  `Color3` int(11) NOT NULL DEFAULT '0',
  `SocketBonus` int(11) NOT NULL DEFAULT '0',
  `GemProperties` int(11) NOT NULL DEFAULT '0',
  `ArmorDamageModifier` float NOT NULL DEFAULT '0',
  `Duration` int(11) NOT NULL DEFAULT '0',
  `ItemLimitCategory` int(11) NOT NULL DEFAULT '0',
  `HolidayId` int(11) NOT NULL DEFAULT '0',
  `StatScalingFactor` float NOT NULL DEFAULT '0',
  `CurrencySubstitutionId` int(11) NOT NULL DEFAULT '0',
  `CurrencySubstitutionCount` int(11) NOT NULL DEFAULT '0',
  `ItemNameDescriptionID` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table item_sparse_locale
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_sparse_locale`;

CREATE TABLE `item_sparse_locale` (
  `ID` int(11) unsigned NOT NULL,
  `Name_koKR` text CHARACTER SET latin1,
  `Name_frFR` text CHARACTER SET latin1,
  `Name_deDE` text CHARACTER SET latin1,
  `Name_zhCN` text CHARACTER SET latin1,
  `Name_zhTW` text CHARACTER SET latin1,
  `Name_esES` text CHARACTER SET latin1,
  `Name_esMX` text CHARACTER SET latin1,
  `Name_ruRU` text CHARACTER SET latin1,
  `Name_ptPT` text CHARACTER SET latin1,
  `Name_itIT` text CHARACTER SET latin1,
  `Name2_koKR` text CHARACTER SET latin1,
  `Name2_frFR` text CHARACTER SET latin1,
  `Name2_deDE` text CHARACTER SET latin1,
  `Name2_zhCN` text CHARACTER SET latin1,
  `Name2_zhTW` text CHARACTER SET latin1,
  `Name2_esES` text CHARACTER SET latin1,
  `Name2_esMX` text CHARACTER SET latin1,
  `Name2_ruRU` text CHARACTER SET latin1,
  `Name2_ptPT` text CHARACTER SET latin1,
  `Name2_itIT` text CHARACTER SET latin1,
  `Name3_koKR` text CHARACTER SET latin1,
  `Name3_frFR` text CHARACTER SET latin1,
  `Name3_deDE` text CHARACTER SET latin1,
  `Name3_zhCN` text CHARACTER SET latin1,
  `Name3_zhTW` text CHARACTER SET latin1,
  `Name3_esES` text CHARACTER SET latin1,
  `Name3_esMX` text CHARACTER SET latin1,
  `Name3_ruRU` text CHARACTER SET latin1,
  `Name3_ptPT` text CHARACTER SET latin1,
  `Name3_itIT` text CHARACTER SET latin1,
  `Name4_koKR` text CHARACTER SET latin1,
  `Name4_frFR` text CHARACTER SET latin1,
  `Name4_deDE` text CHARACTER SET latin1,
  `Name4_zhCN` text CHARACTER SET latin1,
  `Name4_zhTW` text CHARACTER SET latin1,
  `Name4_esES` text CHARACTER SET latin1,
  `Name4_esMX` text CHARACTER SET latin1,
  `Name4_ruRU` text CHARACTER SET latin1,
  `Name4_ptPT` text CHARACTER SET latin1,
  `Name4_itIT` text CHARACTER SET latin1,
  `Description_koKR` text CHARACTER SET latin1,
  `Description_frFR` text CHARACTER SET latin1,
  `Description_deDE` text CHARACTER SET latin1,
  `Description_zhCN` text CHARACTER SET latin1,
  `Description_zhTW` text CHARACTER SET latin1,
  `Description_esES` text CHARACTER SET latin1,
  `Description_esMX` text CHARACTER SET latin1,
  `Description_ruRU` text CHARACTER SET latin1,
  `Description_ptPT` text CHARACTER SET latin1,
  `Description_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



# Dump of table item_x_bonus_tree
# ------------------------------------------------------------

DROP TABLE IF EXISTS `item_x_bonus_tree`;

CREATE TABLE `item_x_bonus_tree` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `ItemID` int(11) NOT NULL,
  `ItemBonusTreeCategory` int(11) NOT NULL,
  `BuildVerified` int(10) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;



# Dump of table map_challenge_mode
# ------------------------------------------------------------

DROP TABLE IF EXISTS `map_challenge_mode`;

CREATE TABLE `map_challenge_mode` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `MapID` int(11) unsigned NOT NULL,
  `Unk1` int(11) unsigned NOT NULL,
  `Unk2` int(11) unsigned NOT NULL,
  `Unk3` int(11) unsigned NOT NULL,
  `BronzeTime` int(11) unsigned NOT NULL,
  `SilverTime` int(11) unsigned NOT NULL,
  `GoldTime` int(11) unsigned NOT NULL,
  `Unk4` int(11) unsigned NOT NULL,
  `Unk5` int(11) unsigned NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table mount
# ------------------------------------------------------------

DROP TABLE IF EXISTS `mount`;

CREATE TABLE `mount` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `Category` int(10) unsigned NOT NULL,
  `CreatureDisplayID` int(10) unsigned NOT NULL,
  `Flags` int(10) unsigned NOT NULL,
  `Name` text NOT NULL,
  `Description` text NOT NULL,
  `Icon` text NOT NULL,
  `FilterCategory` int(10) unsigned NOT NULL,
  `SpellID` int(10) unsigned NOT NULL,
  `PlayerConditionID` int(10) unsigned NOT NULL,
  `BuildVerified` int(10) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table mount_locale
# ------------------------------------------------------------

DROP TABLE IF EXISTS `mount_locale`;

CREATE TABLE `mount_locale` (
  `ID` int(11) unsigned NOT NULL,
  `Name_koKR` text CHARACTER SET latin1,
  `Name_frFR` text CHARACTER SET latin1,
  `Name_deDE` text CHARACTER SET latin1,
  `Name_zhCN` text CHARACTER SET latin1,
  `Name_zhTW` text CHARACTER SET latin1,
  `Name_esES` text CHARACTER SET latin1,
  `Name_esMX` text CHARACTER SET latin1,
  `Name_ruRU` text CHARACTER SET latin1,
  `Name_ptPT` text CHARACTER SET latin1,
  `Name_itIT` text CHARACTER SET latin1,
  `Description_koKR` text CHARACTER SET latin1,
  `Description_frFR` text CHARACTER SET latin1,
  `Description_deDE` text CHARACTER SET latin1,
  `Description_zhCN` text CHARACTER SET latin1,
  `Description_zhTW` text CHARACTER SET latin1,
  `Description_esES` text CHARACTER SET latin1,
  `Description_esMX` text CHARACTER SET latin1,
  `Description_ruRU` text CHARACTER SET latin1,
  `Description_ptPT` text CHARACTER SET latin1,
  `Description_itIT` text CHARACTER SET latin1,
  `Icon_koKR` text CHARACTER SET latin1,
  `Icon_frFR` text CHARACTER SET latin1,
  `Icon_deDE` text CHARACTER SET latin1,
  `Icon_zhCN` text CHARACTER SET latin1,
  `Icon_zhTW` text CHARACTER SET latin1,
  `Icon_esES` text CHARACTER SET latin1,
  `Icon_esMX` text CHARACTER SET latin1,
  `Icon_ruRU` text CHARACTER SET latin1,
  `Icon_ptPT` text CHARACTER SET latin1,
  `Icon_itIT` text CHARACTER SET latin1,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;



# Dump of table quest_package_item
# ------------------------------------------------------------

DROP TABLE IF EXISTS `quest_package_item`;

CREATE TABLE `quest_package_item` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `PackageID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId` int(10) unsigned NOT NULL DEFAULT '0',
  `Count` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table spell_aura_restrictions
# ------------------------------------------------------------

DROP TABLE IF EXISTS `spell_aura_restrictions`;

CREATE TABLE `spell_aura_restrictions` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CasterAuraState` int(11) unsigned NOT NULL,
  `TargetAuraState` int(11) unsigned NOT NULL,
  `CasterAuraStateNot` int(11) unsigned NOT NULL,
  `TargetAuraStateNot` int(11) unsigned NOT NULL,
  `CasterAuraSpell` int(11) unsigned NOT NULL,
  `TargetAuraSpell` int(11) unsigned NOT NULL,
  `ExcludeCasterAuraSpell` int(11) unsigned NOT NULL,
  `ExcludeTargetAuraSpell` int(11) unsigned NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table spell_casting_requirements
# ------------------------------------------------------------

DROP TABLE IF EXISTS `spell_casting_requirements`;

CREATE TABLE `spell_casting_requirements` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `FacingCasterFlags` int(11) unsigned NOT NULL,
  `MinFactionId` int(11) unsigned NOT NULL,
  `MinReputation` int(11) unsigned NOT NULL,
  `AreaGroupId` int(11) NOT NULL,
  `RequiredAuraVision` int(11) unsigned NOT NULL,
  `RequiresSpellFocus` int(11) unsigned NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table spell_effect_group_size
# ------------------------------------------------------------

DROP TABLE IF EXISTS `spell_effect_group_size`;

CREATE TABLE `spell_effect_group_size` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `SpellEffectID` int(11) unsigned NOT NULL,
  `Coefficient` float NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;



# Dump of table spell_misc
# ------------------------------------------------------------

DROP TABLE IF EXISTS `spell_misc`;

CREATE TABLE `spell_misc` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Attributes` int(11) unsigned NOT NULL,
  `AttributesEx` int(11) unsigned NOT NULL,
  `AttributesEx2` int(11) unsigned NOT NULL,
  `AttributesEx3` int(11) unsigned NOT NULL,
  `AttributesEx4` int(11) unsigned NOT NULL,
  `AttributesEx5` int(11) unsigned NOT NULL,
  `AttributesEx6` int(11) unsigned NOT NULL,
  `AttributesEx7` int(11) unsigned NOT NULL,
  `AttributesEx8` int(11) unsigned NOT NULL,
  `AttributesEx9` int(11) unsigned NOT NULL,
  `AttributesEx10` int(11) unsigned NOT NULL,
  `AttributesEx11` int(11) unsigned NOT NULL,
  `AttributesEx12` int(11) unsigned NOT NULL,
  `AttributesEx13` int(11) unsigned NOT NULL,
  `CastingTimeIndex` int(11) unsigned NOT NULL,
  `DurationIndex` int(11) unsigned NOT NULL,
  `RangeIndex` int(11) unsigned NOT NULL,
  `Speed` float NOT NULL,
  `SpellVisual0` int(11) unsigned NOT NULL,
  `SpellVisual1` int(11) unsigned NOT NULL,
  `SpellIconID` int(11) unsigned NOT NULL,
  `ActiveIconID` int(11) unsigned NOT NULL,
  `SchoolMask` int(11) unsigned NOT NULL,
  `MultistrikeSpeedMod` float NOT NULL,
  `BuildVerified` int(11) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;




/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
