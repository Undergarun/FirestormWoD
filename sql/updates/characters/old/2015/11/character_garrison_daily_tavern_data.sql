DROP TABLE IF EXISTS character_garrison_daily_tavern_data;
CREATE TABLE `character_garrison_daily_tavern_data` (
  `CharacterGuid` int(10) unsigned NOT NULL DEFAULT '0',
  `NpcEntry` int(8) NOT NULL,
  PRIMARY KEY (`CharacterGuid`, `NpcEntry`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;
