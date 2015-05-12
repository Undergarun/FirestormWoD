CREATE TABLE `quest_package_item` (
  `ID` int(10) unsigned NOT NULL,
  `PackageID` int(10) unsigned NOT NULL DEFAULT '0',
  `ItemId` int(10) unsigned NOT NULL DEFAULT '0',
  `Count` int(10) unsigned NOT NULL DEFAULT '0',
  `Type` int(10) unsigned NOT NULL DEFAULT '0',
  `BuildVerified` int(10) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
