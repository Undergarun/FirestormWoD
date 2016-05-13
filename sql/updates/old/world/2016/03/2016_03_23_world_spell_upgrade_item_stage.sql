DELETE FROM spell_script_names WHERE ScriptName = "spell_skill_ensorcelled_tarot_upgrade";

DROP TABLE IF EXISTS `spell_upgrade_item_stage`;
CREATE TABLE `spell_upgrade_item_stage` (
  `ItemBonusTreeCategory` int(10) unsigned NOT NULL,
  `ItemClass` int(10) NOT NULL DEFAULT '0',
  `ItemSubClassMask` int(10) NOT NULL DEFAULT '0',
  `InventoryTypeMask` int(10) NOT NULL DEFAULT '0',
  `MaxIlevel` int(10) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

insert into `spell_upgrade_item_stage`(`ItemBonusTreeCategory`,`ItemClass`,`ItemSubClassMask`,`InventoryTypeMask`,`MaxIlevel`) values (197,4,0,4096,685),(197,4,0,8388608,675);