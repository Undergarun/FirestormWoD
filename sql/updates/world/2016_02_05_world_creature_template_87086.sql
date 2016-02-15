-- Gezz'ran

	UPDATE creature_template SET minlevel = 96, maxlevel = 96, faction = 7, type_flags = 0 WHERE entry = 87086;
	DELETE FROM creature WHERE id = 87086;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('87086','1116','6662','6662','1','4294967295','0','0','2264.87','3976.4','185.419','5.58651','300','0','0','200','0','0','0','0','0','2048','0','0','0','0',NULL);