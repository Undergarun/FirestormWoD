#59608/NPC - Anduin Wrynn

	DELETE FROM creature WHERE id = 59608;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('59608','870','6134','6048','1','4294967295','0','0','-1714.01','1062.47','29.5197','3.69707','300','0','0','84','0','0','0','0','0','2048','0','0',NULL);
	UPDATE creature_template SET scriptname = "mob_anduin_wrynn" WHERE entry = 59608;

