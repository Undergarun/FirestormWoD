#
# SQL Export
# Created by Querious (974)
# Created: May 13, 2015 at 11:31:38 AM CDT
# Encoding: Unicode (UTF-8)
#


DROP TABLE IF EXISTS `item_x_bonus_tree`;
DROP TABLE IF EXISTS `item_sparse`;
DROP TABLE IF EXISTS `item_modified_appearance`;
DROP TABLE IF EXISTS `item_extended_cost`;
DROP TABLE IF EXISTS `item_effect`;
DROP TABLE IF EXISTS `item_currency_cost`;
DROP TABLE IF EXISTS `item_bonus_tree_node`;
DROP TABLE IF EXISTS `item_bonus`;
DROP TABLE IF EXISTS `item_appearance`;
DROP TABLE IF EXISTS `item`;


CREATE TABLE `item` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Class` int(11) NOT NULL DEFAULT '0',
  `SubClass` int(11) NOT NULL DEFAULT '0',
  `SoundOverrideSubclass` int(11) NOT NULL DEFAULT '0',
  `Material` int(11) NOT NULL DEFAULT '0',
  `InventoryType` int(11) NOT NULL DEFAULT '0',
  `Sheath` int(11) NOT NULL DEFAULT '0',
  `DisplayId` int(11) NOT NULL DEFAULT '0',
  `GroupSoundsID` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `item_appearance` (
  `ID` int(10) unsigned NOT NULL,
  `DisplayID` int(10) unsigned DEFAULT NULL,
  `Unk` int(10) unsigned DEFAULT NULL,
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `item_bonus` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Entry` int(11) NOT NULL DEFAULT '0',
  `Type` int(11) NOT NULL DEFAULT '0',
  `Value1` int(11) NOT NULL DEFAULT '0',
  `Value2` int(11) NOT NULL DEFAULT '0',
  `Index` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `item_bonus_tree_node` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `Category` int(11) NOT NULL DEFAULT '0',
  `Difficulty` int(11) NOT NULL DEFAULT '0',
  `LinkedCategory` int(11) NOT NULL DEFAULT '0',
  `ItemBonusEntry` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `item_currency_cost` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `ItemID` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `item_effect` (
  `ID` int(10) unsigned NOT NULL,
  `ItemID` int(10) unsigned DEFAULT NULL,
  `EffectIndex` int(10) unsigned DEFAULT NULL,
  `SpellID` int(10) unsigned DEFAULT NULL,
  `SpellTrigger` int(10) DEFAULT NULL,
  `SpellCharge` int(10) DEFAULT NULL,
  `SpellCooldown` int(10) DEFAULT NULL,
  `SpellCategory` int(10) unsigned DEFAULT NULL,
  `SpellCategoryCooldown` int(10) DEFAULT NULL,
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `item_extended_cost` (
  `ID` int(11) NOT NULL DEFAULT '0',
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
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `item_modified_appearance` (
  `ID` int(10) unsigned NOT NULL,
  `ItemID` int(10) DEFAULT NULL,
  `Unk` int(10) DEFAULT NULL,
  `AppearanceID` int(10) DEFAULT NULL,
  `Unk2` int(10) DEFAULT NULL,
  `Index` int(10) DEFAULT NULL,
  `BuildVerified` int(10) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


CREATE TABLE `item_sparse` (
  `ID` int(10) unsigned NOT NULL DEFAULT '0',
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
  `Color0` int(11) NOT NULL DEFAULT '0',
  `Color1` int(11) NOT NULL DEFAULT '0',
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
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;


CREATE TABLE `item_x_bonus_tree` (
  `ID` int(11) NOT NULL DEFAULT '0',
  `ItemID` int(11) NOT NULL DEFAULT '0',
  `ItemBonusTreeCategory` int(11) NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;




