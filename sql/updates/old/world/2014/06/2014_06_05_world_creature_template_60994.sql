#60994/NPC - Lun-Chi

	DELETE FROM creature WHERE id = 60994;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('60994','870','5841','6174','1','4294967295','0','0','3455.44','396.495','587.98','0.977601','300','0','0','787882','0','0','0','0','0','2048','0','0',NULL);

	UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `ScriptName` = 'mob_lun_chi' WHERE `entry` = 60994;
	UPDATE `creature_template` SET `gossip_menu_id` = 60013 WHERE `entry` = 60994;
	
	DELETE FROM gossip_menu_option WHERE menu_id = 69970;
	INSERT INTO gossip_menu_option VALUES (69970, 0, 0, "Test your might !", 0, 0, 0, 0, 0, 0, NULL);

	DELETE FROM gossip_menu WHERE entry = 60013;
	INSERT INTO gossip_menu VALUES (60013, 69970);
	
