DELETE FROM `spell_target_position` WHERE id IN (132621, 132627);

-- Alli
-- teleportation position
INSERT INTO `spell_target_position`(`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES ('132621','0','870','882.98','297.62','503.12','3.80');
-- portal spell
UPDATE `gameobject_template` SET `data0` = 0, ScriptName =  'go_mage_portal_pandaria' WHERE `entry` = 216057;


-- Horde
-- teleportation position
INSERT INTO `spell_target_position`(`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES ('132627','0','870','1604.37','920.83','470.63','0.19');
-- portal spell
UPDATE `gameobject_template` SET `data0` = 0,  ScriptName = 'go_mage_portal_pandaria' WHERE `entry` = 216058;