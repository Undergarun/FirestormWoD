DELETE FROM game_graveyard_zone WHERE id IN (4830, 4922, 4921);
INSERT INTO game_graveyard_zone VALUE
(4830, 7004, 67),
(4922, 7004, 67),
(4921, 7004, 67);

DELETE FROM game_graveyard_zone WHERE id IN (4723, 4858, 4728);
INSERT INTO game_graveyard_zone VALUE
(4723, 7078, 469),
(4858, 7078, 469),
(4728, 7078, 469);

DELETE FROM creature WHERE id=6491 AND map IN (1330, 1153, 1152, 1159, 1158, 1331);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('6491','1330','7004','7210','2','4294967295','0','0','5408.63','4379.37','152.836','1.63172','300','0','0','3965','0','0','0','0','0','2048','0','0','0','0',NULL);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('6491','1153','7004','7103','2','4294967295','0','0','5606.99','4665.41','142.458','5.4001','300','0','0','3965','0','0','0','0','0','2048','0','0','0','0',NULL);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('6491','1152','7004','7096','2','4294967295','0','0','5412.02','4373.92','153.999','1.82831','300','0','0','3965','0','0','0','0','0','2048','0','0','0','0',NULL);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('6491','1159','7078','7086','2','4294967295','0','0','1772.5','276.769','76.9762','3.36541','300','0','0','3965','0','0','0','0','0','2048','0','0','0','0',NULL);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('6491','1158','7078','7085','2','4294967295','0','0','1773.31','269.803','76.6613','1.75396','300','0','0','3965','0','0','0','0','0','2048','0','0','0','0',NULL);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUES('6491','1331','7078','7211','2','4294967295','0','0','1774.45','269.473','76.6014','1.80077','300','0','0','3965','0','0','0','0','0','2048','0','0','0','0',NULL);
