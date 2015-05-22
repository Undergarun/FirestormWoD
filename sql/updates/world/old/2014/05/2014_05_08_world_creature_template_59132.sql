#59132/NPC - Rit-Rit

	DELETE FROM creature WHERE id = 59132;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('59132','870','5805','6064','1','4294967295','0','0','289.24','1501.49','438.192','3.92203','300','0','0','787882','0','0','0','0','0','2048','0','0',NULL);

	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `mindmg` = 9838, `maxdmg` = 14431, `attackpower` = 42296, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_rit_rit' WHERE `entry` = 59132;