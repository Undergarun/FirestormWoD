DROP TABLE IF EXISTS `creature_loot_currency_personnal`;
CREATE TABLE `creature_loot_currency_personnal`
(
    `creature_id` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `CurrencyId1` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `CurrencyId2` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `CurrencyId3` SMALLINT(5) UNSIGNED NOT NULL DEFAULT '0',
    `CurrencyCount1` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `CurrencyCount2` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `CurrencyCount3` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`creature_id`)
);