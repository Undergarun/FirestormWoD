-- Instructors in Ta'yak room
DELETE FROM creature WHERE id BETWEEN 64338 AND 64341;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES
('64338','1009','6297','6297','760','1','0','0','-2068.49','337.286','420.392','5.88186','7200','0','0','1','0','0','0','0','0','0','0','0',NULL),
('64339','1009','6297','6297','760','1','0','0','-2068.49','225.424','420.392','0.390725','7200','0','0','1','0','0','0','0','0','0','0','0',NULL),
('64340','1009','6297','6297','760','1','0','0','-2169.89','337.286','420.392','6.14358','7200','0','0','1','0','0','0','0','0','0','0','0',NULL),
('64341','1009','6297','6297','760','1','0','0','-2169.89','225.424','420.392','0.161204','7200','0','0','1','0','0','0','0','0','0','0','0',NULL);
