/*Table structure for table `character_template` */

DROP TABLE IF EXISTS `character_template`;

CREATE TABLE `character_template` (
  `id` int(10) unsigned NOT NULL DEFAULT '1',
  `class` tinyint(3) unsigned NOT NULL DEFAULT '0',
  `name` varchar(256) NOT NULL,
  `description` varchar(2048) NOT NULL,
  `level` tinyint(3) unsigned NOT NULL DEFAULT '1',
  `money` bigint(20) unsigned NOT NULL DEFAULT '0',
  `alianceX` float DEFAULT '0',
  `alianceY` float NOT NULL DEFAULT '0',
  `alianceZ` float NOT NULL DEFAULT '0',
  `alianceO` float NOT NULL DEFAULT '0',
  `alianceMap` smallint(6) NOT NULL DEFAULT '0',
  `hordeX` float NOT NULL DEFAULT '0',
  `hordeY` float NOT NULL DEFAULT '0',
  `hordeZ` float NOT NULL DEFAULT '0',
  `hordeO` float NOT NULL DEFAULT '0',
  `hordeMap` smallint(6) NOT NULL DEFAULT '0',
  `disabled` tinyint(3) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `character_template_item` */

DROP TABLE IF EXISTS `character_template_item`;

CREATE TABLE `character_template_item` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `itemID` int(10) unsigned NOT NULL DEFAULT '0',
  `count` int(10) unsigned NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

/*Table structure for table `character_template_spell` */

DROP TABLE IF EXISTS `character_template_spell`;

CREATE TABLE `character_template_spell` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `spellId` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
