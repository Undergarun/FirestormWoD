CREATE TABLE `garrison_plot_building_content` (
  `ID` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `PlotType` int(10) unsigned NOT NULL DEFAULT '0',
  `FactionIndex` int(10) unsigned NOT NULL DEFAULT '0',
  `CreatureOrGob` int(10) NOT NULL DEFAULT '0',
  `X` float NOT NULL DEFAULT '0',
  `Y` float NOT NULL DEFAULT '0',
  `Z` float NOT NULL DEFAULT '0',
  `O` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1