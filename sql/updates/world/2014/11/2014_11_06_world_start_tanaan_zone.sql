-- Tanaan Jungle

	-- Game Objects
	
		UPDATE gameobject SET phasemask = 2 WHERE id = 237667;
		UPDATE `gameobject_template` SET `ScriptName` = 'go_platform_tanaan' WHERE `entry` IN (237667, 237670);

	-- NPCs

		#83863/NPC - Archmage Khadgar <Sons of Lothar>

			DELETE FROM creature WHERE id = 83863 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('83863','0','1519','5148','1','4294967295','0','0','-8881.9','577.091','93.2319','3.80681','300','0','0','200','234849','0','0','0','0','2048','0','0',NULL);
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('83863','1','1637','5170','1','4294967295','0','0','1537.69','-4420.53','19.5444','1.57628','300','0','0','200','234849','0','0','0','0','2048','0','0',NULL);

		#85210/NPC - Savage Flaskataur, Esq. <Consumables Vendor>

			DELETE FROM creature WHERE id = 85210 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('85210','1','1637','5170','1','4294967295','0','0','1523.98','-4423.34','22.0036','1.39956','300','0','0','20','0','0','0','0','0','2048','0','0',NULL);
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('85210','0','1519','5148','1','4294967295','0','0','-8868.74','604.364','92.8365','0.0211847','300','0','0','20','0','0','0','0','0','2048','0','0',NULL);

		#82188/NPC - Mumper

			DELETE FROM creature WHERE id = 82188 AND areaId = 7037;
			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_mumper' WHERE `entry` = 82188;

		#78883/NPC - Iron Grunt

			DELETE FROM creature WHERE id = 78883 AND areaId = 7037;
			UPDATE `creature_template` SET `exp` = 5, faction = 14, `ScriptName` = 'npc_iron_grunt' WHERE `entry` = 78883;
			
		#81990/NPC - Moriccalas

			DELETE FROM creature WHERE id = 81990 AND areaId = 7037;
			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_moriccalas' WHERE `entry` = 81990;
			
		#83538/NPC - Warsong Commander

			UPDATE `creature_template` SET `exp` = 5, faction = 14, `ScriptName` = 'npc_warsong_commander' WHERE `entry` = 83538;
			DELETE FROM creature WHERE id = 83538 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('83538','1265','7025','7037','1','4294967295','0','0','4065.2','-2277.73','64.9206','4.70883','300','0','0','1','0','0','0','0','0','2048','0','0',NULL);
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('83538','1265','7025','7037','1','4294967295','0','0','4005.49','-2282.36','58.8881','4.52977','300','0','0','1','0','0','0','0','0','2048','0','0',NULL);

		#82007/NPC - Tore

			DELETE FROM creature WHERE id = 82007 AND areaId = 7037;
			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_tore' WHERE `entry` = 82007;			
			INSERT INTO creature (id, map, position_x, position_y, position_z, orientation, spawnMask, npcflag) VALUES(82007, 1265, 4052.837, -2294.998, 65.01549, 3.980502, 1, 0);
			INSERT INTO creature (id, map, position_x, position_y, position_z, orientation, spawnMask, npcflag) VALUES(82007, 1265, 3668.781, -2086.755, 7.411091, 5.195292, 1, 0);

			
		#82010/NPC - Bonesaw

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_bonesaw' WHERE `entry` = 82010;
			DELETE FROM creature WHERE id = 82010 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('82010','1265','7025','7037','1','4294967295','0','0','4050.46','-2312.04','64.5531','3.33075','300','0','0','5','9916','0','0','0','0','2048','0','0',NULL);

		#81994/NPC - Corselle

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_corselle' WHERE `entry` = 81994;
			DELETE FROM creature WHERE id = 81994 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('81994','1265','7025','7037','1','4294967295','0','0','4048.3','-2304.92','64.921','2.63488','300','0','0','5','9916','0','0','0','0','2048','0','0',NULL);

		#82011/NPC - Northpaul

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_northpaul' WHERE `entry` = 82011;
			DELETE FROM creature WHERE id = 82011 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('82011','1265','7025','7037','1','4294967295','0','0','4116.03','-2376.24','78.8099','3.13545','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#81997/NPC - Roague

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_roague' WHERE `entry` = 81997;
			DELETE FROM creature WHERE id = 81997 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('81997','1265','7025','7037','1','4294967295','0','0','4039.84','-2276.38','64.9202','5.15444','300','0','0','5','0','0','0','0','0','2048','0','0',NULL);

		#82082/NPC - Racy

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_racy' WHERE `entry` = 82082;
			DELETE FROM creature WHERE id = 82082 AND areaId = 7037;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('82082','1265','7025','7037','1','4294967295','0','0','4038.37','-2426.59','94.6834','2.76778','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#82191/NPC - High Warlord Shoju

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_high_warlord_shoju' WHERE `entry` = 82191;
			DELETE FROM creature WHERE id = 82191;
			INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('82191','1265','7025','7037','1','4294967295','0','0','4037.77','-2420.96','94.5996','2.92044','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#82012/NPC - Kengtus the patient
		
			DELETE FROM creature WHERE id = 82012;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82012','1265','7025','7037','1','1','0','0','4040.23','-2288.61','64.9604','0.903204','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82012','1265','7025','7045','1','1','0','0','3770.24','-2044.35','14.1326','0.903204','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#81993/NPC - Starcaller Astrylian

			DELETE FROM creature WHERE id = 81993;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81993','1265','7025','7037','1','1','0','0','4075.67','-2306.26','64.9208','1.58058','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81993','1265','7025','7045','1','1','0','0','3892.93','-2013.85','19.8355','0.0102745','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#81996/NPC - Turkina
		
			DELETE FROM creature WHERE id = 81996;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81996','1265','7025','7037','1','1','0','0','4076.54','-2288.55','64.9968','1.82787','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81996','1265','7025','7045','1','1','0','0','3687.14','-2078.4','7.47598','1.82787','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#82017/NPC - Miserain Starsorrow
		
			DELETE FROM creature WHERE id = 82017;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82017','1265','7025','7037','1','1','0','0','4084.98','-2295.92','65.0191','0.670326','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82017','1265','7025','7045','1','1','0','0','3716.66','-2054.56','10.1277','3.22075','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

		#82484/NPC - Iron Gronnling
		
			DELETE FROM creature WHERE id = 82484;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82484','1265','7025','7037','1','1','0','0','4051.26','-2238.4','50.6377','4.68633','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82484','1265','7025','7037','1','1','0','0','4079.32','-2238.21','50.6051','4.65281','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

			UPDATE `creature_template` SET `ScriptName` = 'npc_iron_gronnling', faction = 14 WHERE `entry` = 82484;
			
		#81711/NPC - Shadowmoon ritualist
		
			DELETE FROM creature WHERE id = 81711;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4060.75','-2314.19','64.5794','0.0403959','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4071.86','-2312.82','64.8204','1.38084','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4072.63','-2313.95','64.7967','3.19282','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4074.88','-2316.55','64.742','2.97604','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4076.34','-2312.34','64.6935','0.338716','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4080.11','-2312.61','64.7916','0.314911','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4083.06','-2313.43','64.8075','0.0501579','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4088.62','-2304.59','64.9208','3.47112','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4089.84','-2307.51','64.9208','2.4341','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4090.52','-2303.94','65.0406','3.77034','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81711','1265','7025','7037','1','1','0','0','4090.79','-2308.32','65.0406','2.33256','120','0','0','1','0','0','0','0','0','0','0','0',NULL);

			UPDATE `creature_template` SET `ScriptName` = 'npc_shadowmoon_ritualist' WHERE `entry` = 81711;
			
		#82305/NPC - Khadgar Shield Target
		
			UPDATE `creature_template` SET `flags_extra` = 128 WHERE `entry` = 82305;
			
		#82417/NPC - Iron Horde War Machine (original modelid1 = 56977, original aura = 165978)
		
			UPDATE `creature_template` SET `modelid1` = 11686 , `unit_flags` = 34603008 WHERE `entry` = 82417;
			DELETE FROM creature_template_addon WHERE entry = 82417;
			
		#81696/NPC - Teron gor

			DELETE FROM creature WHERE id = 81696;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81696','1265','7025','7037','1','4294967295','0','0','3964.79','-2773.55','66.6518','4.7544','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `minlevel` = 93, `maxlevel` = 93, `exp` = 5, `InhabitType` = 4, `Health_mod` = 3, `Mana_mod` = 3, faction = 35 WHERE `entry` = 81696;

		#81695/NPC - Cho Gall
		
			UPDATE `creature_template` SET `unit_flags` = 33686276 WHERE `entry` = 81695;
		
		#78333/NPC - Gul Dan
		
			UPDATE `creature_template` SET `unit_flags` = 33686308 WHERE `entry` = 78333;
		
		#82647/NPC - Tormented Soul
		
			UPDATE `creature_template` SET `unit_flags` = 50332484 WHERE `entry` = 82647;
		
		#78558/NPC - Archmage Khadgar
		
			UPDATE `creature_template` SET `ScriptName` = 'npc_archmage_khadgar' WHERE `entry` = 78558;
		
		#78559/NPC - The cost Of War
		
			UPDATE `creature_template` SET `ScriptName` = 'npc_archmage_khadgar' WHERE `entry` = 78559;
			
		#81762/NPC - Taag
		
			DELETE FROM creature WHERE id = 81762;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81762','1265','7025','7041','1','1','0','0','3990.82','-2553.8','66.0799','0.0878151','120','0','0','1','0','0','2','0','0','0','0','0',NULL);

		#85142/NPC - Karabor Slave
		
			UPDATE `creature_template` SET `unit_flags` = 131844 WHERE `entry` = 85142;
		
		-- Quests

		#35933/Quest - Azeroth s Last Stand

			DELETE FROM `creature_questrelation` WHERE `quest` = 35933;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35933;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35933;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78558, 35933);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78558;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35933;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35933;
			UPDATE `quest_template` SET `Flags` = 327688 WHERE `Id` = 35933;
			
		#34420/Quest - The cost of war

			DELETE FROM `creature_questrelation` WHERE `quest` = 34420;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34420;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34420;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78558, 34420);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78558;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34420;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34420;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 34420);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;

		#34393/Quest - The Portal s Power

			DELETE FROM `creature_questrelation` WHERE `quest` = 34393;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34393;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34393;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78558, 34393);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78558;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34393;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34393;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78558, 34393);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78558;

		#34392/Quest - Onslaught s end

			DELETE FROM `creature_questrelation` WHERE `quest` = 34392;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34392;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34392;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34392;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34392;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78558, 34392);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78558;

		#34398/Quest - The Dark Portal

			DELETE FROM `creature_questrelation` WHERE `quest` = 34398;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34398;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34398;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34398;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34398;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78558, 34398);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78558;
			
		#35019/Quest - Prepare for Battle
			
			DELETE FROM `creature_questrelation` WHERE `quest` = 35019;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35019;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35019;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78554, 35019);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78554;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35019;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35019;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78554, 35019);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78554;

		#34432/Quest - The Shadowmoon Clan
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 34432;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34432;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34432;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78554, 34432);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78554;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34432;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34432;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79316, 34432);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79316;

		#34422/Quest - Blaze of Glory
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 34422;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34422;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34422;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78559, 34422);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78559;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34422;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34422;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 34422);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;
		
		#35242/Quest - Vengeance for the Fallen
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35242;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35242;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81763, 35242);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81763;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35242;

		#35240/Quest - Bled Dry
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35240;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35240;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81762, 35240);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81762;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35240;
