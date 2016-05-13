DROP TABLE IF EXISTS `character_daily_loot_cooldown`;
CREATE TABLE `character_daily_loot_cooldown`
(
    `guid` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `entry` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`guid`, `entry`)
);