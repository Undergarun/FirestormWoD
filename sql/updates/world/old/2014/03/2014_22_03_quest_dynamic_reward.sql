SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `quest_dynamic_reward`
-- ----------------------------
DROP TABLE IF EXISTS `quest_dynamic_reward`;
CREATE TABLE `quest_dynamic_reward` (
  `questId` int(8) NOT NULL,
  `itemId` int(8) DEFAULT NULL,
  `itemCount` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

SET FOREIGN_KEY_CHECKS = 1;
