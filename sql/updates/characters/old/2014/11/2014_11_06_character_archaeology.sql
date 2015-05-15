ALTER TABLE `character_archaeology` DROP COLUMN `sites4`, DROP COLUMN `sites3`, DROP COLUMN `sites2`, DROP COLUMN `sites1`, DROP COLUMN `sites0`;

CREATE TABLE `character_archaeology_sites` 
(
	`guid` INT(11) UNSIGNED NOT NULL ,
	`map` INT(11) UNSIGNED NOT NULL ,
	`sites` TEXT NOT NULL 
);

 ALTER TABLE `character_archaeology_sites` ADD PRIMARY KEY (`guid`, `map`); 