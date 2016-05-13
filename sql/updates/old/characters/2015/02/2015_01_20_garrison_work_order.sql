CREATE TABLE `character_garrison_work_order` (
  `id` INT(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  `garrison_id` INT(10) UNSIGNED NOT NULL,
  `plot_instance_id` INT(10) UNSIGNED NOT NULL,
  `shipment_id` INT(10) UNSIGNED NOT NULL,
  `creation_time` INT(10) UNSIGNED NOT NULL,
  `complete_time` INT(10) UNSIGNED NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=INNODB AUTO_INCREMENT=18 DEFAULT CHARSET=latin1;