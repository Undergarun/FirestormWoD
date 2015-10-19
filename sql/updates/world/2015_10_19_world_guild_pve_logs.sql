DROP TABLE IF EXISTS `guild_raid_progress`;
CREATE TABLE `guild_raid_progress`
(
    `GuildID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `MapID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `DifficultyID` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `GuildName` TEXT(255) NOT NULL,
    `Faction` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `RealmID` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `EncountersKilled` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `MaxEncounters` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`GuildID`, `MapID`, `DifficultyID`)
);

DROP TABLE IF EXISTS `guild_encounter_progress`;
CREATE TABLE `guild_encounter_progress`
(
    `GuildID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `MapID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `DifficultyID` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `EncounterID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `FirstKillDate` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `PullCount` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `FastestKill` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`GuildID`, `MapID`, `DifficultyID`)
);

DROP TABLE IF EXISTS `guild_encounter_attempts`;
CREATE TABLE `guild_encounter_attempts`
(
    `AttemptID` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT UNIQUE,
    `GuildID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `MapID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `DifficultyID` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `EncounterID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `KillDate` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `KillTime` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`GuildID`, `MapID`, `DifficultyID`)
);

DROP TABLE IF EXISTS `guild_encounter_attempts_datas`;
CREATE TABLE `guild_encounter_attempts_datas`
(
    `AttemptID` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `MemberGuidLow` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `MemberName` TEXT(255) NOT NULL,
    `MemberClass` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `MemberSpec` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `MemberRole` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    PRIMARY KEY (`AttemptID`)
);