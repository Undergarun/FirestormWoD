-- creature_template.equipment_id deleted
-- creature_equip_template.entry == creature_template.entry
-- id field added to creature_equip_template -> PK(entry, id)
-- id field in creature_equip_template starts at 1
-- creature.equipment_id references id of creature_equip_template
-- creature.equipment_id = 0 means no equipment at all (default 1)
-- creature.equipment_id = -1 means pick a random equipment from creature_equip_template

-- Diff_entries should use the same template of the normal entry
UPDATE `creature_template` SET `equipment_id` = 0 WHERE `name` LIKE '%(1)' OR `name` LIKE '%(2)' OR `name` LIKE '%(3)' OR `name` LIKE '%(4)';

-- Delete unused templates
DROP TABLE IF EXISTS `temp_c_e`;
CREATE TABLE IF NOT EXISTS `temp_c_e` (`entry` MEDIUMINT(8));
ALTER TABLE `temp_c_e` ADD INDEX `ind` (`entry`);
INSERT INTO `temp_c_e` SELECT `equipment_id` FROM `creature_template` WHERE `equipment_id` != 0 UNION
                       SELECT `equipment_id` FROM `creature` WHERE `equipment_id` != 0 UNION
                       SELECT `equipment_id` FROM `game_event_model_equip` WHERE `equipment_id` != 0;
DELETE FROM `creature_equip_template` WHERE `entry` NOT IN (SELECT `entry` FROM `temp_c_e`);
DROP TABLE `temp_c_e`;

-- Create temporary table to hold the values of creature_equip_template with converted entry
DROP TABLE IF EXISTS `creature_equip_template2`;
CREATE TABLE IF NOT EXISTS `creature_equip_template2` (
  `entry` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  `id` TINYINT(3) UNSIGNED NOT NULL DEFAULT '1',
  `itemEntry1` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  `itemEntry2` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  `itemEntry3` MEDIUMINT(8) UNSIGNED NOT NULL DEFAULT '0',
  PRIMARY KEY (`entry`, `id`)
) ENGINE=MYISAM DEFAULT CHARSET=utf8;

INSERT INTO `creature_equip_template2` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`)
    SELECT `creature_template`.`entry`, 1, `itemEntry1`, `itemEntry2`, `itemEntry3`
    FROM `creature_template`
    JOIN `creature_equip_template` ON `creature_equip_template`.`entry` = `equipment_id`
    WHERE `equipment_id` != 0;

INSERT IGNORE INTO `creature_equip_template2` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`)
    SELECT `id`, 2, `itemEntry1`, `itemEntry2`, `itemEntry3`
    FROM `creature`
    JOIN `creature_equip_template` ON `creature_equip_template`.`entry` = `equipment_id`
    WHERE `equipment_id` != 0;

DROP TABLE `creature_equip_template`;
RENAME TABLE `creature_equip_template2` TO `creature_equip_template`;

UPDATE `creature` SET `equipment_id` = 2 WHERE `equipment_id` != 0;
UPDATE `creature` SET `equipment_id` = 1 WHERE `equipment_id` = 0;

-- From game_event_model_equip
UPDATE `creature` SET `equipment_id` = 1 WHERE `guid` IN (12088, 12093, 12095, 79670, 79675, 79676, 79690, 79792, 79807, 79814);
UPDATE `game_event_model_equip` SET `equipment_id` = 2 WHERE `guid` IN (12088, 12093, 12095, 79670, 79675, 79676, 79690, 79792, 79807, 79814);
DELETE FROM `creature_equip_template` WHERE `entry` IN (1976, 23585, 424) AND `id`=2;
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(1976, 2, 2715, 143, 0),
(23585, 2, 2715, 143, 0),
(424, 2, 2715, 143, 0);

-- ALTER TABLE `creature_equip_template` CHANGE `entry` `entry` mediumint(8) unsigned NOT NULL;
-- ALTER TABLE `creature_equip_template` ADD `id` tinyint(3) unsigned NOT NULL DEFAULT '1' AFTER `entry`;
-- ALTER TABLE `creature_equip_template` DROP INDEX `PRIMARY`, ADD PRIMARY KEY (`entry`, `id`);
ALTER TABLE `creature_template` DROP `equipment_id`;
ALTER TABLE `creature` CHANGE `equipment_id` `equipment_id` TINYINT(3) UNSIGNED NOT NULL DEFAULT '1';
ALTER TABLE `game_event_model_equip` CHANGE `equipment_id` `equipment_id` TINYINT(3) UNSIGNED NOT NULL DEFAULT '1';

-- Conversion from SAI
UPDATE `smart_scripts` SET `action_param1` = 1 WHERE `entryorguid` = 2523901 AND `source_type` = 9 AND `id` = 3;
UPDATE `smart_scripts` SET `action_param1` = 0 WHERE `entryorguid` = 2523900 AND `source_type` = 9 AND `id` = 2;
UPDATE `smart_scripts` SET `action_param1` = 2 WHERE `entryorguid` = 32720   AND `source_type` = 0 AND `id` = 0;
DELETE FROM `creature_equip_template` WHERE `entry` = 25239 AND `id`=1;
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES 
(25239, 1, 6829, 0, 0);

ALTER TABLE `creature` CHANGE `equipment_id` `equipment_id` TINYINT(3) SIGNED NOT NULL DEFAULT '0';
ALTER TABLE `creature_equip_template` CHANGE `id` `id` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0';
ALTER TABLE `game_event_model_equip` CHANGE `equipment_id` `equipment_id` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0';

UPDATE `creature` SET `equipment_id`=0 WHERE `id` NOT IN (SELECT `entry` FROM `creature_equip_template`);

-- content
DELETE FROM `creature_equip_template` WHERE `entry` IN (25317, 2110, 26797, 25239) AND `id`=1;
DELETE FROM `creature_equip_template` WHERE `entry` IN (32720) AND `id`=2;
INSERT INTO `creature_equip_template` (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES
(25317, 1, 2178, 143, 0),
(2110, 1, 24324, 24324, 24319),
(26797, 1, 19970, 0, 0),
(25239, 1, 6829, 0, 0),
(32720, 2, 31308, 0, 0);

UPDATE `creature` SET `equipment_id`=0 WHERE `guid` IN (200908,200918,200968,200971,200980,200998,201004,201035,201050,201052,201068,201094,201103,201112,201113,201135,201149,201150,201192,201203,201206,201363,201438,201449,201486,201685,201710,201722,201740,201752,201756,201773,201782,201788,201814,201864,201908,201916,202023,202043,202146,202164,39317,39318,39319,39320,39321,39322,39323,39324,39325,39326,39327,39328,39329,39330,39331,39332,39333,40170,40171,40227,40563,40564,41319,41322,41323,41324,41325,41326,41327,41328,41669,41726,42616,42641,42668,42669,42680,42686,44193,44194,46975,47413,47414,47415,47416,47417,47418,47419,47420,47421,47422,47423,47424,47425,47426,47427,47428,47429,47430,47431,47445,47632,48383,49987,51458,51751,51982,51983,52926,52927,52928,52929,53166,53651,53844,53845,53846,53847,53848,53849,53850,53851,53951,57315,68749,68750,68751,68752,68753,76521,79676,79725,79758,79759,79760,79761,79762,79763,79764,117784,117785,117788,117789,117790,117796,117797,117798,117799,117800,117801,117802,112352,112353,112354,112355,112356,112357,112358,112359,112360,112361,112362,112363,112364,112365,112366,112367,112368,112371,112372);

-- 25317
UPDATE `creature` SET `equipment_id`=1 WHERE `guid` IN (117794);

-- 2110
UPDATE `creature` SET `equipment_id`=1 WHERE `guid` IN (102345,102347,102348,125999,126000,126001,126002,126003,126004,126005,126006,126007,126008,126009,126010,126011,126012,126013,126014,126015,126016,126095,126096,126097,126098,126099,126100,126101,126724,126725,126726,126727,126729,126730,126731,127489,127490,127491,127492,131859,131860,131861,131862,131863,13354,13355,13356,13357,16230,16231,16232,16233,16234,16235,16236,18316,18392,28779,28781,28782,31778,31779,31965,31973,32010,32011,4212,4214,4215,4227,42676,42683,42708,42714,42726,4274,42793,4286,4310,4313,4352,4358,4408,45397,45398,45399,45403,46795,46796,46799,46810,46860,46861,47175,47182,47184,47980,47981,47988,48220,48263,48265,48909,4939,5031,5191,52670,53043,53116,53119,53181,6007,6049,6050,6051,6056,6057,6058,6076,86393,86394,87158,87161,87163,87164,87165,87168,87225,87234,87236,87252);

-- 26797
UPDATE `creature` SET `equipment_id`=1 WHERE `guid` IN (112351, 112369, 112370);

UPDATE `smart_scripts` SET `action_param1` = 1 WHERE `entryorguid` = 2523900 AND `source_type` = 9 AND `id` = 2;
UPDATE `smart_scripts` SET `action_param1` = 0 WHERE `entryorguid` = 2523901 AND `source_type` = 9 AND `id` = 3;
UPDATE `smart_scripts` SET `action_param1` = 2 WHERE `entryorguid` = 32720   AND `source_type` = 0 AND `id` = 0;

UPDATE `creature_equip_template` SET `id`=1 WHERE `id`=0;