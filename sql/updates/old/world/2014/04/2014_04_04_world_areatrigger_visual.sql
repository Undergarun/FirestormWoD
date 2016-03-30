DROP TABLE IF EXISTS `areatrigger_visual`;

CREATE TABLE `areatrigger_visual` (
    `entry` INT(8) UNSIGNED NOT NULL,
    `visual_radius` FLOAT NOT NULL,
    PRIMARY KEY (`entry`)
) ENGINE = INNODB DEFAULT CHARSET = latin1;

INSERT INTO areatrigger_visual VALUES
(116011, 3.5), -- Rune of Power
(116235, 3.5); -- Amethyst Pool