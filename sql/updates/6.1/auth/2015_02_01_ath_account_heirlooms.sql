CREATE TABLE `account_heirlooms`
(
`account_id` INT(10) UNSIGNED NOT NULL DEFAULT 0,
`heirloom_id` INT(10) UNSIGNED NOT NULL DEFAULT 0,
`upgrade_flags` INT(10) UNSIGNED NOT NULL DEFAULT 0,
PRIMARY KEY (`account_id`, `heirloom_id`)
);
