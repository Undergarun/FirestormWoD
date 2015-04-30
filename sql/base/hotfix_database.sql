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
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `item_modified_appearance` (
  `ID` int(10) unsigned NOT NULL,
  `ItemID` int(10) DEFAULT NULL,
  `Unk` int(10) DEFAULT NULL,
  `AppearanceID` int(10) DEFAULT NULL,
  `Unk2` int(10) DEFAULT NULL,
  `Index` int(10) DEFAULT NULL,
  `BuildVerified` int(10) DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

CREATE TABLE `item_appearance` (
  `ID` int(10) unsigned NOT NULL,
  `DisplayID` int(10) unsigned DEFAULT NULL,
  `Unk` int(10) unsigned DEFAULT NULL,
  `BuildVerified` int(10) unsigned DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;