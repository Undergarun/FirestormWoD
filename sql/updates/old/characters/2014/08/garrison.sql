CREATE TABLE `character_garrison` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `character_guid` int(10) unsigned NOT NULL,
  `level` int(10) NOT NULL DEFAULT '1',
  `blue_recipes` text NOT NULL,
  `specializations` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=latin1;

CREATE TABLE `character_garrison_building` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL,
  `plot_instance_id` int(10) unsigned NOT NULL,
  `building_id` int(10) unsigned NOT NULL,
  `spec_id` int(10) unsigned NOT NULL DEFAULT '0',
  `time_built_start` int(10) unsigned NOT NULL DEFAULT '0',
  `time_built_end` int(10) unsigned NOT NULL DEFAULT '0',
  `active` tinyint(1) unsigned NOT NULL DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

CREATE TABLE `character_garrison_mission` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL DEFAULT '0',
  `mission_id` int(10) unsigned NOT NULL DEFAULT '0',
  `offer_time` int(10) unsigned NOT NULL DEFAULT '0',
  `offer_max_duration` int(10) unsigned NOT NULL DEFAULT '0',
  `start_time` int(10) unsigned NOT NULL DEFAULT '0',
  `state` int(10) unsigned NOT NULL DEFAULT '10',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=3 DEFAULT CHARSET=latin1;

CREATE TABLE `character_garrison_follower` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `garrison_id` int(10) unsigned NOT NULL DEFAULT '0',
  `follower_id` int(10) unsigned NOT NULL DEFAULT '0',
  `level` int(10) unsigned NOT NULL DEFAULT '0',
  `xp` int(10) unsigned NOT NULL DEFAULT '0',
  `quality` int(10) unsigned NOT NULL DEFAULT '0',
  `item_level_armor` int(10) unsigned NOT NULL DEFAULT '0',
  `item_level_weapon` int(10) unsigned NOT NULL DEFAULT '0',
  `current_mission_id` int(10) unsigned NOT NULL DEFAULT '0',
  `current_building_id` int(10) unsigned NOT NULL DEFAULT '0',
  `abilities` text NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=4 DEFAULT CHARSET=latin1;
