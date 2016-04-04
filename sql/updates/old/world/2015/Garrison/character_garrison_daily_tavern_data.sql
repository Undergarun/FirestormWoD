CREATE TABLE `character_garrison_daily_tavern_data` (
  `CharacterGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `NpcEntry` int(8) NOT NULL,
  PRIMARY KEY (`CharacterGuid`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;