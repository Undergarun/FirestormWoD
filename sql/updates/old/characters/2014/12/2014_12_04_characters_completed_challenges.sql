DROP TABLE IF EXISTS `character_completed_challenges`;
CREATE TABLE `character_completed_challenges`(
    `guid` INT(10) UNSIGNED NOT NULL,
    `map_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `best_time` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `last_time` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `best_medal` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `best_medal_date` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`guid`, `map_id`)
);