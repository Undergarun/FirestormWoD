DROP TABLE IF EXISTS `store_title`;
CREATE TABLE `store_title`(
    `guid` INT(10) UNSIGNED NOT NULL ,
    `title` INT(10) UNSIGNED NOT NULL ,
    `transaction` INT(10) UNSIGNED NOT NULL ,
    PRIMARY KEY (`guid`)
); 