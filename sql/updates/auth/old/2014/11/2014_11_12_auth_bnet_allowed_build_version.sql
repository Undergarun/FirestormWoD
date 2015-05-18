ALTER TABLE `bnet_allowed_build`     ADD COLUMN `version` VARCHAR(40) NULL AFTER `build`;
UPDATE `bnet_allowed_build` SET `version`='6.0.3.19116' WHERE `build`='19116' AND `version` IS NULL;