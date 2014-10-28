ALTER TABLE `creature_classlevelstats` ADD `attackpower` SMALLINT(6)  NOT NULL  AFTER `basearmor`;
ALTER TABLE `creature_classlevelstats` ADD `rangedattackpower` SMALLINT(6)  NULL  DEFAULT NULL  AFTER `attackpower`;
ALTER TABLE `creature_classlevelstats` ADD `damage_base` FLOAT DEFAULT '0'  AFTER `rangedattackpower`;
ALTER TABLE `creature_classlevelstats` ADD `damage_exp1` FLOAT DEFAULT '0'  AFTER `damage_base`;
ALTER TABLE `creature_classlevelstats` ADD `damage_exp2` FLOAT DEFAULT '0'  AFTER `damage_exp1`;
ALTER TABLE `creature_classlevelstats` ADD `damage_exp3` FLOAT DEFAULT '0'  AFTER `damage_exp2`;
ALTER TABLE `creature_classlevelstats` ADD `damage_exp4` FLOAT DEFAULT '0'  AFTER `damage_exp3`;
ALTER TABLE `creature_classlevelstats` ADD `damage_exp5` FLOAT DEFAULT '0'  AFTER `damage_exp4`;
ALTER TABLE `creature_classlevelstats` ADD `comment` BLOB NULL DEFAULT ''  AFTER `damage_exp5`;