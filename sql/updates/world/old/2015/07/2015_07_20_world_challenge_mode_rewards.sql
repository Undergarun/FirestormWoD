ALTER TABLE `challenge_mode_rewards` ADD COLUMN `title` INT(10) UNSIGNED DEFAULT '0' NOT NULL AFTER `gold_money`;

DELETE FROM `challenge_mode_rewards` WHERE map_id NOT IN (1175, 1176, 1182, 1195, 1208, 1209, 1279, 1358);

# Title rewards
UPDATE `challenge_mode_rewards` SET `title` = 431 WHERE `map_id` = 1182; -- the Soul Preserver
UPDATE `challenge_mode_rewards` SET `title` = 437 WHERE `map_id` = 1208; -- the Grimrail Suplexer
UPDATE `challenge_mode_rewards` SET `title` = 436 WHERE `map_id` = 1279; -- the Violet Guardian
UPDATE `challenge_mode_rewards` SET `title` = 429 WHERE `map_id` = 1175; -- the Mine Master
UPDATE `challenge_mode_rewards` SET `title` = 432 WHERE `map_id` = 1209; -- Scion of Rukhmar
UPDATE `challenge_mode_rewards` SET `title` = 430 WHERE `map_id` = 1195; -- Dockmaster
UPDATE `challenge_mode_rewards` SET `title` = 434 WHERE `map_id` = 1358; -- Lord of Blackrock
UPDATE `challenge_mode_rewards` SET `title` = 433 WHERE `map_id` = 1176; -- Spiritwalker