DROP TABLE IF EXISTS `fun_tarrenmill_reward`;

CREATE TABLE `fun_tarrenmill_reward` 
(
  `kills` int(10) unsigned NOT NULL,
  `title_h` int(10) unsigned NOT NULL,
  `title_a` int(10) unsigned NOT NULL,
  `item_h` int(10) unsigned NOT NULL,
  `item_a` int(10) unsigned NOT NULL,
  `aura_a` int(10) unsigned NOT NULL,
  `aura_h` int(10) unsigned NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;