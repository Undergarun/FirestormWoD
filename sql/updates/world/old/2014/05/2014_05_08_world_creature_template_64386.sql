#64386/NPC - Sheepie

	DELETE FROM creature WHERE id = 64386;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('64386','870','5805','5992','1','4294967295','0','0','113.768','811.015','160.819','0.344262','300','0','0','393941','0','0','0','0','0','2048','0','0',NULL);

	UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'mob_second_sheepie' WHERE `entry` = 64386;