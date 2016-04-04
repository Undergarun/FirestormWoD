DROP TABLE IF EXISTS `character_charges_cooldown`;
CREATE TABLE `character_charges_cooldown`
(
    `guid` INT(10) UNSIGNED NOT NULL,
    `spell_id` INT(10) UNSIGNED NOT NULL,
    `charge` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `time` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`guid`, `spell_id`, `charge`)
);