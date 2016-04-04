CREATE TABLE IF NOT EXISTS `character_queststatus_objective` (
  `guid` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `objectiveId` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  `amount` INT(10) UNSIGNED NOT NULL DEFAULT '0',
  PRIMARY KEY (`objectiveId`,`guid`)
) ENGINE=INNODB DEFAULT CHARSET=utf8;

ALTER TABLE `character_queststatus`
DROP COLUMN `mobcount1`,
DROP COLUMN `mobcount2`,
DROP COLUMN `mobcount3`,
DROP COLUMN `mobcount4`,
DROP COLUMN `itemcount1`,
DROP COLUMN `itemcount2`,
DROP COLUMN `itemcount3`,
DROP COLUMN `itemcount4`,
DROP COLUMN `playercount`;