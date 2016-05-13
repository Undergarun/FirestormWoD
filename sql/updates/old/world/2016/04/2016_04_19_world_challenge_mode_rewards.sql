ALTER TABLE `challenge_mode_rewards` ADD COLUMN `gold_achievement` INT(10) UNSIGNED DEFAULT 0 NOT NULL AFTER `title`;
ALTER TABLE `challenge_mode_rewards` ADD COLUMN `comment` TEXT NOT NULL AFTER `gold_achievement`;

UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9620, `comment` = "Bloodmaul Slag Mines" WHERE map_id = 1175;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9626, `comment` = "Shadowmoon Burial Grounds" WHERE map_id = 1176;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9622, `comment` = "Auchindoun" WHERE map_id = 1182;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9621, `comment` = "Iron Docks" WHERE map_id = 1195;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9625, `comment` = "Grimrail Depot" WHERE map_id = 1208;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9623, `comment` = "Skyreach" WHERE map_id = 1209;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9624, `comment` = "The Everbloom" WHERE map_id = 1279;
UPDATE `challenge_mode_rewards` SET `gold_achievement` = 9627, `comment` = "Upper Blackrock Spire" WHERE map_id = 1358;