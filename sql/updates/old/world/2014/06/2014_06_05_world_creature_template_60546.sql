#60546/NPC - Champion of Chi-Ji

	DELETE FROM creature WHERE id = 60546;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('60546','870','6134','6155','1','4294967295','0','0','-2692.02','1319.27','35.6454','0.365399','300','0','0','1969705','0','0','0','0','0','2048','0','0',NULL);
	
	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `ScriptName` = 'mob_champion_of_chi_ji' WHERE `entry` = 60546;

