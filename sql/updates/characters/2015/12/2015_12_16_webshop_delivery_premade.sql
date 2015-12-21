CREATE TABLE `webshop_delivery_premade` (
  `transaction` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `delivery` tinyint(1) unsigned NOT NULL DEFAULT '0',
  `templateId` int(11) unsigned NOT NULL DEFAULT '0',
  `faction` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `account` int(11) unsigned NOT NULL DEFAULT '0',
  `type` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`transaction`),
  UNIQUE KEY `transaction` (`transaction`),
  KEY `account` (`account`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;