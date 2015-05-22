alter table `character_currency` 
   add column `weekCap` INT(10) UNSIGNED DEFAULT '0' NOT NULL after `flags`, 
   add column `needResetCap` TINYINT(1) UNSIGNED DEFAULT '1' NOT NULL after `weekCap`;