DROP TABLE IF EXISTS `item_effect`;
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

DROP TABLE IF EXISTS `item_appearance`;
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

DROP TABLE IF EXISTS `item_appearance`;
CREATE TABLE `item_appearance` (
  `ID` int(10) unsigned NOT NULL,
  `DisplayID` int(10) unsigned DEFAULT NULL,
  `Unk` int(10) unsigned DEFAULT NULL,
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item_sparse`;
CREATE TABLE `item_sparse` (
	`ID` INT UNSIGNED NOT NULL DEFAULT '0',
	`Quality` INT NOT NULL DEFAULT '0',
	`Flags` INT UNSIGNED NOT NULL DEFAULT '0',
	`Flags2` INT UNSIGNED NOT NULL DEFAULT '0',
	`Flags3` INT UNSIGNED NOT NULL DEFAULT '0',
	`Unk430_1` FLOAT NOT NULL DEFAULT '0',
	`Unk430_2` FLOAT NOT NULL DEFAULT '0',
	`BuyCount` INT NOT NULL DEFAULT '0',
	`BuyPrice` INT NOT NULL DEFAULT '0',
	`SellPrice` INT NOT NULL DEFAULT '0',
	`InventoryType` INT NOT NULL DEFAULT '0',
	`AllowableClass` INT NOT NULL DEFAULT '0',
	`AllowableRace` INT NOT NULL DEFAULT '0',
	`ItemLevel` INT NOT NULL DEFAULT '0',
	`RequiredLevel` INT NOT NULL DEFAULT '0',
	`RequiredSkill` INT NOT NULL DEFAULT '0',
	`RequiredSkillRank` INT NOT NULL DEFAULT '0',
	`RequiredSpell` INT NOT NULL DEFAULT '0',
	`RequiredHonorRank` INT NOT NULL DEFAULT '0',
	`RequiredCityRank` INT NOT NULL DEFAULT '0',
	`RequiredReputationFaction` INT NOT NULL DEFAULT '0',
	`RequiredReputationRank` INT NOT NULL DEFAULT '0',
	`MaxCount` INT NOT NULL DEFAULT '0',
	`Stackable` INT NOT NULL DEFAULT '0',
	`ContainerSlots` INT NOT NULL DEFAULT '0',
	`ItemStatType0` INT NOT NULL DEFAULT '0',
	`ItemStatType1` INT NOT NULL DEFAULT '0',
	`ItemStatType2` INT NOT NULL DEFAULT '0',
	`ItemStatType3` INT NOT NULL DEFAULT '0',
	`ItemStatType4` INT NOT NULL DEFAULT '0',
	`ItemStatType5` INT NOT NULL DEFAULT '0',
	`ItemStatType6` INT NOT NULL DEFAULT '0',
	`ItemStatType7` INT NOT NULL DEFAULT '0',
	`ItemStatType8` INT NOT NULL DEFAULT '0',
	`ItemStatType9` INT NOT NULL DEFAULT '0',
	`ItemStatValue0` INT NOT NULL DEFAULT '0',
	`ItemStatValue1` INT NOT NULL DEFAULT '0',
	`ItemStatValue2` INT NOT NULL DEFAULT '0',
	`ItemStatValue3` INT NOT NULL DEFAULT '0',
	`ItemStatValue4` INT NOT NULL DEFAULT '0',
	`ItemStatValue5` INT NOT NULL DEFAULT '0',
	`ItemStatValue6` INT NOT NULL DEFAULT '0',
	`ItemStatValue7` INT NOT NULL DEFAULT '0',
	`ItemStatValue8` INT NOT NULL DEFAULT '0',
	`ItemStatValue9` INT NOT NULL DEFAULT '0',
	`ScalingValue0` INT NOT NULL DEFAULT '0',
	`ScalingValue1` INT NOT NULL DEFAULT '0',
	`ScalingValue2` INT NOT NULL DEFAULT '0',
	`ScalingValue3` INT NOT NULL DEFAULT '0',
	`ScalingValue4` INT NOT NULL DEFAULT '0',
	`ScalingValue5` INT NOT NULL DEFAULT '0',
	`ScalingValue6` INT NOT NULL DEFAULT '0',
	`ScalingValue7` INT NOT NULL DEFAULT '0',
	`ScalingValue8` INT NOT NULL DEFAULT '0',
	`ScalingValue9` INT NOT NULL DEFAULT '0',
	`SocketCostRate0` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate1` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate2` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate3` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate4` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate5` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate6` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate7` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate8` FLOAT NOT NULL DEFAULT '0',
	`SocketCostRate9` FLOAT NOT NULL DEFAULT '0',
	`ScalingStatDistribution` INT NOT NULL DEFAULT '0',
	`DamageType` INT NOT NULL DEFAULT '0',
	`Delay` INT NOT NULL DEFAULT '0',
	`RangedModRange` FLOAT NOT NULL DEFAULT '0',
	`Bonding` INT NOT NULL DEFAULT '0',
	`Name` TEXT NOT NULL,
	`Name2` TEXT NOT NULL,
	`Name3` TEXT NOT NULL,
	`Name4` TEXT NOT NULL,
	`Description` TEXT NOT NULL,
	`PageText` INT NOT NULL DEFAULT '0',
	`LanguageID` INT NOT NULL DEFAULT '0',
	`PageMaterial` INT NOT NULL DEFAULT '0',
	`StartQuest` INT NOT NULL DEFAULT '0',
	`LockID` INT NOT NULL DEFAULT '0',
	`Material` INT NOT NULL DEFAULT '0',
	`Sheath` INT NOT NULL DEFAULT '0',
	`RandomProperty` INT NOT NULL DEFAULT '0',
	`RandomSuffix` INT NOT NULL DEFAULT '0',
	`ItemSet` INT NOT NULL DEFAULT '0',
	`Area` INT NOT NULL DEFAULT '0',
	`Map` INT NOT NULL DEFAULT '0',
	`BagFamily` INT NOT NULL DEFAULT '0',
	`TotemCategory` INT NOT NULL DEFAULT '0',
	`Color0` INT NOT NULL DEFAULT '0',
	`Color1` INT NOT NULL DEFAULT '0',
	`Color3` INT NOT NULL DEFAULT '0',
	`SocketBonus` INT NOT NULL DEFAULT '0',
	`GemProperties` INT NOT NULL DEFAULT '0',
	`ArmorDamageModifier` FLOAT NOT NULL DEFAULT '0',
	`Duration` INT NOT NULL DEFAULT '0',
	`ItemLimitCategory` INT NOT NULL DEFAULT '0',
	`HolidayId` INT NOT NULL DEFAULT '0',
	`StatScalingFactor` FLOAT NOT NULL DEFAULT '0',
	`CurrencySubstitutionId` INT NOT NULL DEFAULT '0',
	`CurrencySubstitutionCount` INT NOT NULL DEFAULT '0',
	`ItemNameDescriptionID` INT NOT NULL DEFAULT '0',
    `BuildVerified` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item_currency_cost`;
CREATE TABLE `item_currency_cost` (
	`ID` INT NOT NULL DEFAULT '0',
	`ItemID` INT NOT NULL DEFAULT '0',
    `BuildVerified` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item`;
CREATE TABLE `item` (
	`ID` INT NOT NULL DEFAULT '0',
	`Class` INT NOT NULL DEFAULT '0',
	`SubClass` INT NOT NULL DEFAULT '0',
	`SoundOverrideSubclass` INT NOT NULL DEFAULT '0',
	`Material` INT NOT NULL DEFAULT '0',
	`InventoryType` INT NOT NULL DEFAULT '0',
	`Sheath` INT NOT NULL DEFAULT '0',
	`DisplayId` INT NOT NULL DEFAULT '0',
	`GroupSoundsID` INT NOT NULL DEFAULT '0',
    `BuildVerified` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item_extended_cost`;
CREATE TABLE `item_extended_cost` (
	`ID` INT NOT NULL DEFAULT '0',
	`RequiredArenaSlot` INT NOT NULL DEFAULT '0',
	`RequiredItem1` INT NOT NULL DEFAULT '0',
	`RequiredItem2` INT NOT NULL DEFAULT '0',
	`RequiredItem3` INT NOT NULL DEFAULT '0',
	`RequiredItem4` INT NOT NULL DEFAULT '0',
	`RequiredItem5` INT NOT NULL DEFAULT '0',
	`RequiredItemCount1` INT NOT NULL DEFAULT '0',
	`RequiredItemCount2` INT NOT NULL DEFAULT '0',
	`RequiredItemCount3` INT NOT NULL DEFAULT '0',
	`RequiredItemCount4` INT NOT NULL DEFAULT '0',
	`RequiredItemCount5` INT NOT NULL DEFAULT '0',
	`RequiredPersonalArenaRating` INT NOT NULL DEFAULT '0',
	`ItemPurchaseGroup` INT NOT NULL DEFAULT '0',
	`RequiredCurrency1` INT NOT NULL DEFAULT '0',
	`RequiredCurrency2` INT NOT NULL DEFAULT '0',
	`RequiredCurrency3` INT NOT NULL DEFAULT '0',
	`RequiredCurrency4` INT NOT NULL DEFAULT '0',
	`RequiredCurrency5` INT NOT NULL DEFAULT '0',
	`RequiredCurrencyCount1` INT NOT NULL DEFAULT '0',
	`RequiredCurrencyCount2` INT NOT NULL DEFAULT '0',
	`RequiredCurrencyCount3` INT NOT NULL DEFAULT '0',
	`RequiredCurrencyCount4` INT NOT NULL DEFAULT '0',
	`RequiredCurrencyCount5` INT NOT NULL DEFAULT '0',
	`RequiredFactionId` INT NOT NULL DEFAULT '0',
	`RequiredFactionStanding` INT NOT NULL DEFAULT '0',
	`RequirementFlags` INT NOT NULL DEFAULT '0',
	`RequiredAchievement` INT NOT NULL DEFAULT '0',
	`OverrideBuyPrice` INT NOT NULL DEFAULT '0',
    `BuildVerified` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item_bonus`;
CREATE TABLE `item_bonus` (
	`ID` INT NOT NULL DEFAULT '0',
	`Entry` INT NOT NULL DEFAULT '0',
	`Type` INT NOT NULL DEFAULT '0',
	`Value1` INT NOT NULL DEFAULT '0',
	`Value2` INT NOT NULL DEFAULT '0',
	`Index` INT NOT NULL DEFAULT '0',
    `BuildVerified` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item_bonus_tree_node`;
CREATE TABLE `item_bonus_tree_node` (
	`ID` INT NOT NULL DEFAULT '0',
	`Category` INT NOT NULL DEFAULT '0',
	`Difficulty` INT NOT NULL DEFAULT '0',
	`LinkedCategory` INT NOT NULL DEFAULT '0',
	`ItemBonusEntry` INT NOT NULL DEFAULT '0',
    `BuildVerified` int(10) unsigned DEFAULT NULL,
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `item_x_bonus_tree`;
CREATE TABLE `item_x_bonus_tree` (
	`ID` INT NOT NULL DEFAULT '0',
	`ItemID` INT NOT NULL DEFAULT '0',
	`ItemBonusTreeCategory` INT NOT NULL DEFAULT '0',
	PRIMARY KEY (`ID`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;
