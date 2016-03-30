DROP TABLE IF EXISTS `account_toys`;
CREATE TABLE `account_toys`(
    `account_id` INT(10) UNSIGNED NOT NULL,
    `item_id` INT(10) UNSIGNED NOT NULL,
    `is_favorite` BOOL NOT NULL DEFAULT '0',
    PRIMARY KEY (`account_id`, `item_id`));