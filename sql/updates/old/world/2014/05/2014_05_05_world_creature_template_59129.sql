#59129/NPC - Maaka

	DELETE FROM creature WHERE id = 59129;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('59129','870','5805','5805','1','4294967295','0','0','-148.072','1495.95','325.686','0.482234','300','0','0','787882','0','0','0','0','0','2048','0','0',NULL);

	UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `mindmg` = 9838, `maxdmg` = 14431, `attackpower` = 42296, `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'mob_maaka' WHERE `entry` = 59129;
	
