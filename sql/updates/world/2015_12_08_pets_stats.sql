DELETE FROM `pet_stats` WHERE `entry` IN (15438, 61029);
INSERT INTO `pet_stats` (`entry`, `speed`, `powerstatbase`, `armor_coef`, `apsp_coef`, `health_coef`, `damage_coef`, `attackspeed`, `powertype`, `createpower`, `secondarystat_coef`, `comment`) VALUES
    (15438,1.14,1,1,1,0.75,0.6,2,0,1,1,'Greater Fire Elemental (Missing data)'),
    (61029,1.14,1,1.8,1,0.9,1.08,2,0,1,1,'Primal Fire Elemental (Missing data)');
