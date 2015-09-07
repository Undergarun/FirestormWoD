DELETE FROM `pet_stats` WHERE `entry` = 1;
INSERT INTO `pet_stats` (`entry`, `speed`, `powerstatbase`, `armor_coef`, `apsp_coef`, `health_coef`, `damage_coef`, `attackspeed`, `powertype`, `createpower`, `secondarystat_coef`, `comment`)
VALUES
    (1, 1.14, 0, 1.05, 0.6, 0.7, 1, 2, 2, -100, 0.6, 'Hunter Pet');
