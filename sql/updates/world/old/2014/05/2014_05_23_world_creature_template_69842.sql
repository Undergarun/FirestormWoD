#69842/NPC - Zandalari Warbringer

	DELETE FROM creature WHERE id = 69842;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('69842','870','5842','5842','1','1','47682','69842','1278.27','4977.66','120.288','1.9477','600','0','0','21079600','0','0','0','0','0','0','0','0','2104136');
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('69842','870','5841','6076','1','4294967295','0','69842','2794.34','148.732','448.252','2.47804','300','0','0','100','0','0','0','0','134250560','2048','0','0',NULL);

	UPDATE `creature_template` SET 
	`mindmg` = 4750, 
	`maxdmg` = 11118, 
	`attackpower` = 55538, 
	`dmg_multiplier` = 7.5, 
	`baseattacktime` = 2000, 
	`rangeattacktime` = 2000, 
	`ScriptName` = 'mob_zandalari_warbringer' 
	WHERE `entry` = 69842;
	
	UPDATE `creature_template` SET `lootid`=69842 WHERE `entry`=69842;
	DELETE FROM `creature_loot_template` WHERE `entry`=69842;

	INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
	(69842, 93194, -4, 1, 0, 1, 1) /* 4Blood-Soaked Invitation */, 
	(69842, 94158, 30, 1, 0, 1, 1) /* 4Big Bag of Zandalari Supplies */, 
	(69842, 94159, 98, 1, 0, 1, 1) /* 5Small Bag of Zandalari Supplies */, 
	(69842, 94223, 56, 1, 0, 1, 3) /* 4Stolen Shado-Pan Insignia */, 
	(69842, 94225, 56, 1, 0, 1, 3) /* 4Stolen Celestial Insignia */, 
	(69842, 94226, 57, 1, 0, 1, 3) /* 4Stolen Klaxxi Insignia */, 
	(69842, 94227, 58, 1, 0, 1, 3) /* 4Stolen Golden Lotus Insignia */, 
	(69842, 94231, 5, 1, 0, 1, 1) /* 3Reins of the Jade Primordial Direhorn */;

