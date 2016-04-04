DROP TABLE IF EXISTS `log_store_title`;

CREATE TABLE `log_store_title` (
  `id` int(10) DEFAULT NULL,
  `transaction` int(10) DEFAULT NULL,
  `guid` int(10) DEFAULT NULL,
  `title` int(10) DEFAULT NULL,
  `date` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
) ENGINE=InnoDB DEFAULT CHARSET=latin1;