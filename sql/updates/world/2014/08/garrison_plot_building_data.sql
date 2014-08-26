CREATE TABLE `garrison_plot_building_content` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `plot_type` int(10) unsigned NOT NULL DEFAULT '0',
  `faction_index` int(10) unsigned NOT NULL DEFAULT '0',
  `creature_or_gob` int(10) NOT NULL DEFAULT '0',
  `x` float NOT NULL DEFAULT '0',
  `y` float NOT NULL DEFAULT '0',
  `z` float NOT NULL DEFAULT '0',
  `o` float NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;