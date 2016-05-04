DROP TABLE IF EXISTS `character_stats_wod`;

CREATE TABLE `character_stats_wod` (
  `guid` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'Global Unique Identifier, Low part',
  `strength` int(10) unsigned NOT NULL DEFAULT '0',
  `agility` int(10) unsigned NOT NULL DEFAULT '0',
  `stamina` int(10) unsigned NOT NULL DEFAULT '0',
  `intellect` int(10) unsigned NOT NULL DEFAULT '0',
  `critPct` float unsigned NOT NULL DEFAULT '0',
  `haste` float unsigned NOT NULL DEFAULT '0',
  `mastery` float unsigned NOT NULL DEFAULT '0',
  `spirit` int(10) unsigned NOT NULL DEFAULT '0',
  `armorBonus` int(10) unsigned NOT NULL DEFAULT '0',
  `multistrike` float unsigned NOT NULL DEFAULT '0',
  `leech` float unsigned NOT NULL DEFAULT '0',
  `versatility` float unsigned NOT NULL DEFAULT '0',
  `avoidance` float unsigned NOT NULL DEFAULT '0',
  `attackDamage` varchar(255) NOT NULL,
  `attackPower` int(10) unsigned NOT NULL DEFAULT '0',
  `attackSpeed` float unsigned NOT NULL DEFAULT '0',
  `spellPower` int(10) unsigned NOT NULL DEFAULT '0',
  `manaRegen` int(10) unsigned NOT NULL DEFAULT '0',
  `armor` int(10) unsigned NOT NULL DEFAULT '0',
  `dodgePct` float unsigned NOT NULL DEFAULT '0',
  `parryPct` float unsigned NOT NULL DEFAULT '0',
  `blockPct` float unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`guid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;