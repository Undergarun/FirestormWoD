ALTER TABLE `character_archaeology` DROP COLUMN `completed`;

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `character_archaeology_projects`
-- ----------------------------
DROP TABLE IF EXISTS `character_archaeology_projects`;
CREATE TABLE `character_archaeology_projects` (
  `guid` INT(10) UNSIGNED NOT NULL,
  `project` INT(8) UNSIGNED NOT NULL,
  `count` INT(8) UNSIGNED NOT NULL,
  `first_date` INT(8) UNSIGNED NOT NULL,
  PRIMARY KEY (`guid`, `project`)
) ENGINE=INNODB DEFAULT CHARSET=latin1;

SET FOREIGN_KEY_CHECKS = 1;