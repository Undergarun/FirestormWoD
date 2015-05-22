DROP TABLE pet_levelstats;


CREATE TABLE `pet_stats` (
  `entry` int(11) unsigned NOT NULL COMMENT 'entry of the pet, 1 for hunter pet',
  `speed` float NOT NULL DEFAULT '1.14' COMMENT 'speed of the pet',
  `powerstatbase` int(11) unsigned NOT NULL DEFAULT '0' COMMENT 'which owner''s stat using to compute attack & spell power (0 = AP, 1 = SP)',
  `armor_coef` float NOT NULL DEFAULT '1' COMMENT '% of armor of the owner the pet have',
  `apsp_coef` float NOT NULL DEFAULT '1' COMMENT '% of the attack power / spellpower of the owner the pet have',
  `health_coef` float NOT NULL DEFAULT '1' COMMENT '% of health of the owner the pet have',
  `damage_coef` float NOT NULL DEFAULT '0.85' COMMENT '% of the attack power / spell power to use to compute min/max dmg',
  `attackspeed` float NOT NULL DEFAULT '1.82' COMMENT '% of the base attack speed of the pet',
  `powertype` int(11) unsigned NOT NULL,
  `createpower` float NOT NULL,
  `secondarystat_coef` float NOT NULL,
  `comment` varchar(250) DEFAULT NULL,
  PRIMARY KEY (`entry`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;


insert  into `pet_stats`(`entry`,`speed`,`powerstatbase`,`armor_coef`,`apsp_coef`,`health_coef`,`damage_coef`,`attackspeed`,`powertype`,`createpower`,`secondarystat_coef`,`comment`) values (1,1.14,0,1.7,0.6,0.7,0.85,1.82,2,-100,1,'Hunter Pet'),(89,1.14,1,3,1,0.5,0.85,2,0,0,1,'Infernal (Missing data)'),(416,1.14,1,3,1,0.3,0.57,5,3,-200,1,'Imp'),(417,1.14,1,3,1,0.4,0.57,2,3,-200,1,'Felhunter'),(510,1.14,1,3,0.75,0.5,1,2,0,1,1,'Water Elemental'),(1860,1.14,1,3,1,0.5,0.57,2,3,-200,1,'Voidwalker'),(1863,1.14,1,3,1,0.4,0.42,3,3,-200,0.5,'Succubus'),(4277,1.14,1,3,1,0.3,0,2,0,0,1,'Eye of Kilrogg'),(11859,1.14,1,1,1,1,0.85,2,0,-100,1,'Doomguard (Missing data)'),(15352,1.14,0,1,1,1,0.6,2,0,0,1,'Greater Earth Elemental (Missing data)'),(15438,1.14,0,1,1,0.75,0.6,2,0,0.268,1,'Greater Fire Elemental (Missing data)'),(17252,1.14,1,3,1,0.5,0.62,2,3,-200,1,'Felguard'),(19668,1.14,1,3,1,1,0.85,2,0,0.268,1,'Shadowfiend (Missing data)'),(26125,1.14,0,1.25,0.5,0.5,0.47,2.2,3,-100,1,'Risen Ally'),(27829,1.14,0,1,1,0.5,0.85,2,0,0,1,'Ebon Gargoyle (Missing data)'),(58959,1.14,1,3,1,0.4,0.68,2,3,-200,1,'Fel imp'),(58960,1.14,1,4,1,0.6,0.68,2,3,-200,1,'Voidlord'),(58963,1.14,1,3,1,0.5,0.34,3,3,-200,0.33,'Shivarra'),(58964,1.14,1,3,1,0.5,0.68,2,3,-200,1,'Observer'),(58965,1.14,1,3,1,0.6,0.5,2,3,-200,0.66,'Wrathguard'),(58997,1.14,0,3,1,0.6,0.85,2,0,0,1,'Abyssal (Missing data)'),(59000,1.14,1,3,1,0.5,0.85,2,3,-100,1,'Terrorguard (Missing data)'),(61029,1.14,0,1.8,1.8,0.9,1.08,2,0,0,1,'Primal Fire Elemental (Missing data)'),(61056,1.14,0,1.8,1.8,1.5,1.08,2,0,0,1,'Primal Earth Elemental (Missing data)'),(62982,1.14,1,3,1,1,0.85,2,0,0.268,1,'Mindbender (Missing data)'),(63508,1.14,1,2,1,1,0.85,2,0,0,1,'Xuen (Missing data)'),(69680,1.14,0,1,1,0.1,0.65,1.5,0,21626,1,'Storm spirit (Missing data)'),(69791,1.14,0,1,1,0.1,0.65,1.5,0,21626,1,'Fire Spirit (Missing data)'),(69792,1.14,0,1,1,0.1,0.65,1.5,0,21626,1,'Earth Spirit (Missing data)');
