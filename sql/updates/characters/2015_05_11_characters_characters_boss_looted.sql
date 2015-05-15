DROP TABLE IF EXISTS `characters_boss_looted`;
CREATE TABLE `characters_boss_looted`
(
    `guid` INT(10) UNSIGNED NOT NULL,
    `boss_entry` INT(10) UNSIGNED NOT NULL,
    `boss_model_id` INT(10) UNSIGNED NOT NULL,
    PRIMARY KEY (`guid`, `boss_entry`)
);

DELETE FROM worldstates WHERE worldstates.`entry` = 20008;
INSERT INTO worldstates VALUE (20008, 0, 'Boss looted reset time');