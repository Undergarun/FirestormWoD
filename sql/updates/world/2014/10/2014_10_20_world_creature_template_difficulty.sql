CREATE TABLE `creature_template_difficulty` (
  `entry` mediumint(8) unsigned NOT NULL,
  `difficulty` mediumint(8) unsigned NOT NULL,
  `difficulty_entry` mediumint(8) unsigned DEFAULT NULL,
  PRIMARY KEY (`entry`,`difficulty`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

ALTER TABLE `creature_template_difficulty` CHANGE `difficulty` `difficulty` ENUM('NONE_DIFFICULTY','REGULAR_5_DIFFICULTY','HEROIC_5_DIFFICULTY','LEGACY_MAN10_DIFFICULTY','LEGACY_MAN25_DIFFICULTY','LEGACY_MAN10_HEROIC_DIFFICULTY','LEGACY_MAN25_HEROIC_DIFFICULTY','RAID_TOOL_DIFFICULTY','CHALLENGE_MODE_DIFFICULTY','MAN40_DIFFICULTY','DIFFICULTY_ENTRY_10','SCENARIO_HEROIC_DIFFICULTY','SCENARIO_DIFFICULTY','DIFFICULTY_ENTRY_13','NORMAL_DIFFICULTY','HEROIC_DIFFICULTY','MYTHIC_DIFFICULTY','DIFFICULTY_ENTRY_17','EVENT_0_DIFFICULTY','EVENT_1_DIFFICULTY','EVENT_SCENARIO') NOT NULL;

INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 0, difficulty_entry_1 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 1, difficulty_entry_2 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 2, difficulty_entry_3 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 3, difficulty_entry_4 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 4, difficulty_entry_5 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 5, difficulty_entry_6 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 6, difficulty_entry_7 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 7, difficulty_entry_8 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 8, difficulty_entry_9 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 9, difficulty_entry_10 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 10, difficulty_entry_11 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 11, difficulty_entry_12 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 12, difficulty_entry_13 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 13, difficulty_entry_14 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 14, difficulty_entry_15 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 15, difficulty_entry_16 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 16, difficulty_entry_17 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 17, difficulty_entry_18 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 18, difficulty_entry_19 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 19, difficulty_entry_20 + 1  FROM creature_template);
INSERT INTO creature_template_difficulty(entry, difficulty, difficulty_entry) (SELECT entry, 20, difficulty_entry_21 + 1  FROM creature_template);

DELETE FROM creature_template_difficulty WHERE difficulty_entry = 0;

ALTER TABLE `creature_template` DROP COLUMN `difficulty_entry_1`, DROP COLUMN `difficulty_entry_2`, DROP COLUMN `difficulty_entry_3`, DROP COLUMN `difficulty_entry_4`, DROP COLUMN `difficulty_entry_5`, DROP COLUMN `difficulty_entry_6`, DROP COLUMN `difficulty_entry_7`, DROP COLUMN `difficulty_entry_8`, DROP COLUMN `difficulty_entry_9`, DROP COLUMN `difficulty_entry_10`
, DROP COLUMN `difficulty_entry_11`, DROP COLUMN `difficulty_entry_12`, DROP COLUMN `difficulty_entry_13`, DROP COLUMN `difficulty_entry_14`, DROP COLUMN `difficulty_entry_15`, DROP COLUMN `difficulty_entry_16`, DROP COLUMN `difficulty_entry_17`, DROP COLUMN `difficulty_entry_18`, DROP COLUMN `difficulty_entry_19`, DROP COLUMN `difficulty_entry_20`, DROP COLUMN `difficulty_entry_21`;

ALTER TABLE `creature_template` DROP COLUMN `difficulty_entry_21`;