DELETE FROM `pet_stats` WHERE `entry` = 78116;
INSERT INTO `pet_stats` (`entry`, `speed`, `powerstatbase`, `armor_coef`, `apsp_coef`, `health_coef`, `damage_coef`, `attackspeed`, `powertype`, `createpower`, `secondarystat_coef`, `comment`)
VALUES
    (78116, 1.14, 1, 3, 0.75, 0.5, 1, 1.8, 0, 1, 0.094, 'Improved Water Elemental');
