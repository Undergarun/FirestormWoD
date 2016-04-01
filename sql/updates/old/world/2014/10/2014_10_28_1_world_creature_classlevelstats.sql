ALTER TABLE `creature_classlevelstats` ADD `attackpower` SMALLINT(6)  NOT NULL  AFTER `basearmor`;
ALTER TABLE `creature_classlevelstats` ADD `rangedattackpower` SMALLINT(6)  NULL  DEFAULT NULL  AFTER `attackpower`;
ALTER TABLE `creature_classlevelstats` ADD `damage_base` FLOAT DEFAULT '0'  AFTER `rangedattackpower`;
ALTER TABLE `creature_classlevelstats` ADD `comment` BLOB NULL DEFAULT NULL  AFTER `damage_base`;