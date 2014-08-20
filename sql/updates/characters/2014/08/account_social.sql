ALTER TABLE `character_social` CHANGE `guid` `account_id` INT(10) UNSIGNED DEFAULT '0' NOT NULL COMMENT 'Account Identifier';
RENAME TABLE `character_social` TO `account_social`;