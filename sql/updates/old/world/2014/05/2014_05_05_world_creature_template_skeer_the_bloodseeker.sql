#63071/ NPC - Skeer the Bloodseeker

	DELETE FROM creature WHERE id = 63071;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('63071','870','6138','6418','1','4294967295','0','0','-390.386','4763.06','-29.4625','3.15923','300','0','0','3939410','0','0','0','0','0','2048','0','0',NULL);

	UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 63071;
	UPDATE `creature_template` SET `ScriptName` = 'mob_skeer_the_bloodseeker' WHERE `entry` = 63071;