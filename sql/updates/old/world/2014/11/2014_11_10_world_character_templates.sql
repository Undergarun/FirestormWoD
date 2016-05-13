ALTER TABLE character_template_item ADD COLUMN faction TINYINT(3) UNSIGNED DEFAULT 0 AFTER itemID;

DROP TABLE IF EXISTS `character_template_reputation`;
CREATE TABLE `character_template_reputation` (
  `id` int(10) unsigned NOT NULL DEFAULT '0',
  `factionID` int(10) unsigned NOT NULL DEFAULT '0',
  `reputation` int(10) unsigned NOT NULL DEFAULT '0'
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
