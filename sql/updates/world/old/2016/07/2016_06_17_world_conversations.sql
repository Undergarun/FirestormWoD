DROP TABLE IF EXISTS `conversation_template`;
CREATE TABLE `conversation_template`
(
    `Entry` INT(10) UNSIGNED NOT NULL,
    `Duration` INT(10) UNSIGNED NOT NULL,
    `ActorsCount` INT(10) UNSIGNED NOT NULL,
    `Actors` TEXT NOT NULL,
    `Comment` TEXT NOT NULL,
    PRIMARY KEY (`Entry`)
);

DROP TABLE IF EXISTS `conversation_lines`;
CREATE TABLE `conversation_lines`
(
    `Entry` INT(10) UNSIGNED NOT NULL,
    `LineID` INT(10) UNSIGNED NOT NULL DEFAULT 0,
    `BroadcastTextID` INT(10) UNSIGNED NOT NULL DEFAULT 0,
    `UnkValue` INT(10) UNSIGNED NOT NULL DEFAULT 0,
    `Timer` INT(10) UNSIGNED NOT NULL DEFAULT 0,
    `Type` INT(10) UNSIGNED NOT NULL DEFAULT 0,
    PRIMARY KEY (`Entry`, `LineID`)
);

DELIMITER @@
CREATE PROCEDURE AddSoundColumn()
BEGIN
  IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS 
      WHERE table_name = "npc_text" AND column_name = "SoundID") THEN
    ALTER TABLE `npc_text` ADD COLUMN `SoundID` INT(10) UNSIGNED DEFAULT 0 NOT NULL AFTER `ID`;
  END IF;
END@@

DELIMITER ;

CALL AddSoundColumn;

DROP PROCEDURE AddSoundColumn;