
CREATE TABLE `character_garrison` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `CharacterGuid` int(10) unsigned NOT NULL,
  `Level` int(10) NOT NULL DEFAULT '1',
  `BlueRecipes` text NOT NULL,
  `Specializations` text NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

CREATE TABLE `character_garrison_building` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `GarrisonID` int(10) unsigned NOT NULL,
  `PlotInstanceID` int(10) unsigned NOT NULL,
  `BuildingID` int(10) unsigned NOT NULL,
  `SpecID` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeBuiltStart` int(10) unsigned NOT NULL DEFAULT '0',
  `TimeBuiltEnd` int(10) unsigned NOT NULL DEFAULT '0',
  `Active` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=latin1;

CREATE TABLE `character_garrison_mission` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `GarrisonID` int(10) unsigned NOT NULL,
  `MissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `OfferTime` int(10) unsigned NOT NULL,
  `OfferMaxDuration` int(10) unsigned NOT NULL DEFAULT '0',
  `StartTime` int(10) unsigned NOT NULL,
  `State` int(10) unsigned NOT NULL DEFAULT '10',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

CREATE TABLE `character_garrison_follower` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `GarrisonID` int(10) unsigned NOT NULL DEFAULT '0',
  `FollowerID` int(10) unsigned NOT NULL DEFAULT '0',
  `Level` int(10) unsigned NOT NULL DEFAULT '0',
  `XP` int(10) unsigned NOT NULL DEFAULT '0',
  `Quality` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemLevelArmor` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemLevelWeapon` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrentMissionID` int(10) unsigned NOT NULL DEFAULT '0',
  `CurrentBuildingID` int(10) unsigned NOT NULL DEFAULT '0',
  `Abilities` text NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
