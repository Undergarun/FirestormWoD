DROP TABLE IF EXISTS `instance_disabled_rankings`;
CREATE TABLE `instance_disabled_rankings`
(
    `EncounterID` INT(10) UNSIGNED NOT NULL,
    `Comment` TEXT(255) NOT NULL DEFAULT "",
    PRIMARY KEY (`EncounterID`)
); 

DELETE FROM `instance_disabled_rankings` WHERE `EncounterID` IN (1704, 1695, 1713, 1692, 1689, 1694, 1693);
INSERT INTO `instance_disabled_rankings` VALUES
(1704, "Not scripted"),
(1695, "Not scripted"),
(1713, "Not scripted"),
(1692, "Not scripted"),
(1689, "Not scripted"),
(1694, "Not scripted"),
(1693, "Delayed to 27/01");