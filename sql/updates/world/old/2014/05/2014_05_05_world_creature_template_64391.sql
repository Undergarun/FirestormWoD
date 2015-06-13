#64391/NPC - Sheepie

	DELETE FROM creature WHERE id = 64391;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('64391','870','5805','6064','1','4294967295','0','0','-79.3317','1498.08','361.497','0.399237','300','0','0','393941','0','0','0','0','0','2048','0','0',NULL);

	UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'mob_sheepie' WHERE `entry` = 64391;
	
