DROP TABLE IF EXISTS `character_worldstates`;

CREATE TABLE `character_worldstates` (
  `guid` INT(10) UNSIGNED NOT NULL,
  `worldstate` INT(10) UNSIGNED NOT NULL,
  `value` BIGINT(20) UNSIGNED NOT NULL,
  UNIQUE KEY `unique` (`guid`,`worldstate`),
  KEY `guid` (`guid`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;