#59582/NPC - Innkeeper Lei Lan

	UPDATE `creature_template` SET `ScriptName` = 'mob_lei_lan' WHERE `entry` = 59582;
	
	DELETE FROM creature WHERE id = 59582;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('59582','870','5805','6039','1','4294967295','0','0','-196.595','632.986','165.409','5.79466','300','0','0','393941','0','0','0','0','0','2048','0','0',NULL);
