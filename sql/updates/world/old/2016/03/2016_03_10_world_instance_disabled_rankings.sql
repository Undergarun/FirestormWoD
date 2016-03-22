ALTER TABLE `instance_disabled_rankings` ADD COLUMN `DifficultyID` INT(10) UNSIGNED NOT NULL AFTER `EncounterID`, DROP PRIMARY KEY, ADD PRIMARY KEY(`EncounterID`, `DifficultyID`);

DELETE FROM `instance_disabled_rankings`;
INSERT INTO `instance_disabled_rankings` VALUES
(1692, 14, "Operator Thogar - Normal - Not scripted"),
(1692, 15, "Operator Thogar - Heroic - Not scripted"),
(1692, 17, "Operator Thogar - Mythic - Not scripted"),
(1694, 14, "Beastlord Darmac - Normal - Not scripted"),
(1694, 15, "Beastlord Darmac - Heroic - Not scripted"),
(1694, 17, "Beastlord Darmac - Mythic - Not scripted"),
(1695, 14, "The Iron Maidens - Normal - Not scripted"),
(1695, 15, "The Iron Maidens - Heroic - Not scripted"),
(1695, 17, "The Iron Maidens - Mythic - Not scripted"),
(1704, 14, "Blackhand - Normal - Not scripted"),
(1704, 15, "Blackhand - Heroic - Not scripted"),
(1704, 17, "Blackhand - Mythic - Not scripted"),
(1713, 14, "Kromog - Normal - Tests until 03/16"),
(1713, 15, "Kromog - Heroic - Tests until 03/16"),
(1713, 17, "Kromog - Mythic - Tests until 03/16");