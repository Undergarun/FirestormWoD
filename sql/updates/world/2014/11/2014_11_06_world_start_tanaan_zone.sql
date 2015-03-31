-- Tanaan Jungle

	-- Spells

		DELETE FROM spell_script_names where spell_id = 165988;
		INSERT INTO spell_script_names VALUES (165988, "spell_tanaan_inferno");

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
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82188','1265','7025','7045','1','1','0','1','3568.39','-2130.44','17.8266','1.77576','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82188','1265','7025','7037','1','1','0','0','4054.23','-2360.82','87.4181','1.55796','300','0','0','87515','0','0','0','0','0','2048','0','0',NULL);

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_mumper' WHERE `entry` = 82188;

		#78883/NPC - Iron Grunt

			-- DELETE FROM creature WHERE id = 78883 AND areaId = 7037;
			UPDATE `creature_template` SET `exp` = 5, faction = 14, `ScriptName` = 'npc_iron_grunt' WHERE `entry` = 78883;
			
		#81990/NPC - Moriccalas

			DELETE FROM creature WHERE id = 81990 AND areaId = 7037;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81990','1265','7025','7045','1','1','0','1','3682.2','-2045.1','11.0711','1.32942','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81990','1265','7025','7037','1','1','0','0','4078.91','-2346.17','77.7762','1.56974','300','0','0','87515','0','0','0','0','0','2048','0','0',NULL);

			UPDATE `creature_template` SET `exp` = 5, `ScriptName` = 'npc_moriccalas' WHERE `entry` = 81990;
			
		#83538/NPC - Warsong Commander

			UPDATE `creature_template` SET `exp` = 5, faction = 14, `ScriptName` = '' WHERE `entry` = 83538;
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

			UPDATE creature_template SET faction = 2580 WHERE entry = 82012;
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
			UPDATE `creature_template` SET `minlevel` = 93, `maxlevel` = 93, `exp` = 5, `InhabitType` = 4, `Health_mod` = 3, `Mana_mod` = 3, faction = 35,  unit_flags = 33686308, scriptname = "" WHERE `entry` = 81696;

		#81695/NPC - Cho Gall
		
			UPDATE `creature_template` SET `unit_flags` = 33686276, scriptname = "" WHERE `entry` = 81695;
		
		#78333/NPC - Gul Dan
		
			UPDATE `creature_template` SET `unit_flags` = 33686308 WHERE `entry` = 78333;
		
		#82647/NPC - Tormented Soul
		
			UPDATE `creature_template` SET `unit_flags` = 50332484 WHERE `entry` = 82647;
		
		#78558/NPC - Archmage Khadgar
		
			UPDATE `creature_template` SET `ScriptName` = 'npc_archmage_khadgar' WHERE `entry` = 78558;
		
		#78559/NPC - Archmage Khadgar
		
			UPDATE `creature_template` SET `ScriptName` = 'npc_archmage_khadgar' WHERE `entry` = 78559;
			
		#82012/NPC - Kengtus Pranch the Patient

			DELETE FROM creature WHERE id = 82012;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82012','1265','7025','7037','1','1','0','1','4040.23','-2288.61','64.9604','0.903204','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('82012','1265','7025','7045','1','1','0','1','3770.24','-2044.35','14.1326','0.903204','120','0','0','1','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `exp` = 5, faction=14, `ScriptName` = 'npc_kengtus_pranch_the_patient' WHERE `entry` = 82012;

		#81762/NPC - Taag

			UPDATE creature_template SET scriptname = "npc_tanaan_taag" WHERE entry = 81762;
			DELETE FROM creature WHERE id = 81762;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('81762','1265','7025','7041','1','1','0','0','3990.82','-2553.8','66.0799','0.0878151','120','0','0','1','0','0','2','0','0','0','0','0',NULL);

		#85142/NPC - Karabor Slave
		
			UPDATE `creature_template` SET `unit_flags` = 131844 WHERE `entry` = 85142;
		
		#76643/NPC - Archmage Khadgar

			DELETE FROM creature WHERE id = 76643;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('76643','1190','4','72','1','4294967295','0','0','-11821.7','-3209.23','-29.7129','3.27012','300','0','0','3','0','0','0','0','0','2048','0','0',NULL);
			UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'npc_archmage_khadgar_gossip' WHERE `entry` = 76643;

		#300003/NPC - Gul Dan Trigger (Tanaan) (custom)

			DELETE FROM creature_template WHERE entry = 300003;
			DELETE FROM creature WHERE id = 300003;
			insert into `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('300003','0','0','11686','0','0','0','Gul Dan trigger (Tanaan)',NULL,NULL,'0','1','1','0','0','35','0','0','1','1.14286','1.14286','1','0','0','1','0','0','1','1','0','33554950','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','npc_gul_dan_trigger','1');
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('300003','1265','7025','7037','1','4294967295','0','0','4100.54','-2377.3','79.6406','3.03056','300','0','0','42','0','0','0','0','0','2048','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('300003','1265','7025','7037','1','1','0','0','4036.25','-2377.51','79.6502','3.19942','300','0','0','42','0','0','0','0','0','2048','0','0',NULL);

		#82001/NPC - Monishot

			DELETE FROM creature WHERE id = 82001;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82001','1265','7025','7037','1','1','0','1','4068.36','-2274.85','64.9704','0.758156','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82001','1265','7025','7045','1','1','0','1','3703.74','-2039.62','10.5628','4.12717','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE creature_template SEt faction = 35 WHERE entry = 82001;

		#82014/NPC - Archon

			DELETE FROM creature WHERE id = 82014;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82014','1265','7025','7037','1','65535','0','1','4083.9','-2306.21','65.0406','0.0832898','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82014','1265','7025','7045','1','65535','0','1','3960.82','-2019.81','24.8669','0.0832898','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

		#82647/NPC - Tormented Soul

			UPDATE `creature_template` SET faction = 7, `unit_flags` = 512, `ScriptName` = 'npc_tormented_soul' WHERE `entry` = 82647;

		#82002/NPC - Alya

			DELETE FROM creature WHERE id = 82002;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82002','1265','7025','7037','1','65535','0','1','4086.72','-2313.24','64.8113','0.0501919','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82002','1265','7025','7045','1','65535','0','1','3963.5','-1995.19','26.4083','4.56414','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_alya' WHERE `entry` = 82002;

		#81996/NPC - Turkina

			DELETE FROM creature WHERE id = 81996;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81996','1265','7025','7045','1','65535','0','0','3687.14','-2078.4','7.47598','1.82787','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81996','1265','7025','7037','1','65535','0','0','4076.54','-2288.55','64.9968','1.82787','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_turkina' WHERE `entry` = 81996;

		#81998/NPC - Anruin

			DELETE FROM creature WHERE id = 81998;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81998','1265','7025','7037','1','65535','0','1','4085.89','-2286.46','64.9908','0.628133','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81998','1265','7025','7045','1','65535','0','1','3939.04','-2001.51','22.4796','0.628133','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_anruin' WHERE `entry` = 81998;

		#79062/NPC - Arnold Croman

			DELETE FROM creature WHERE id = 79062;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('79062','1265','7025','7037','1','65535','0','1','4143.53','-2322.31','64.6627','5.55645','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

		#81993/NPC - Starcaller Astrylian

			DELETE FROM creature WHERE id = 81993;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81993','1265','7025','7045','1','65535','0','0','3892.93','-2013.85','19.8355','0.0102745','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81993','1265','7025','7037','1','65535','0','0','4075.67','-2306.26','64.9208','1.58058','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_starcaller_astrylian' WHERE `entry` = 81993;

		#81995/NPC - Maelgwyn

			DELETE FROM creature WHERE id = 81995;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81995','1265','7025','7037','1','1','0','0','4069.73','-2315.9','64.7455','1.39489','300','0','0','87515','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81995','1265','7025','7045','1','4294967295','0','1','3716.59','-2035.08','11.7287','1.36721','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_maelgwyn' WHERE `entry` = 81995;

		#82000/NPC - Elaynea Welton the Wind and Sea

			DELETE FROM creature WHERE id = 82000;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82000','1265','7025','7037','1','65535','0','1','4055.82','-2276.56','64.973','1.42816','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82000','1265','7025','7045','1','65535','0','1','3812.44','-2084.12','14.6447','1.42816','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_elaynea_welton' WHERE `entry` = 82000;

		#82001/NPC - Monishot

			DELETE FROM creature WHERE id = 82001;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82001','1265','7025','7037','1','4294967295','0','0','4068.34','-2278.94','64.9206','4.58748','300','0','0','87515','0','0','0','0','262144','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82001','1265','7025','7045','1','65535','0','1','3703.74','-2039.62','10.5628','4.12717','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_monishot' WHERE `entry` = 82001;

		#82003/NPC - Ed

			DELETE FROM creature WHERE id = 82003;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82003','1265','7025','7037','1','1','0','0','4100.1','-2269.93','64.487','0.271995','300','0','0','87515','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82003','1265','7025','7045','1','4294967295','0','1','3846.23','-2061.96','16.0158','4.15417','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_tanaan_ed' WHERE `entry` = 82003;

		#82004/NPC - Salty Futz

			DELETE FROM creature WHERE id = 82004;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82004','1265','7025','7037','1','65535','0','1','4069.36','-2295.16','65.0164','5.76947','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82004','1265','7025','7045','1','65535','0','1','3767.21','-2067.1','15.2172','0.640037','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_salty_futz' WHERE `entry` = 82004;

		#82005/NPC - Challe Tebrilinde

			DELETE FROM creature WHERE id = 82005;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82005','1265','7025','7037','1','4294967295','0','1','4035.3','-2289.21','64.9977','2.20653','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82005','1265','7025','7045','1','4294967295','0','1','3920.82','-2006.44','20.4369','2.20653','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_challe_tebrilinde' WHERE `entry` = 82005;

		#82008/NPC - Malothas

			DELETE FROM creature WHERE id = 82008;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82008','1265','7025','7037','1','65535','0','1','4048.26','-2283.59','64.9846','1.62065','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82008','1265','7025','7045','1','65535','0','1','3753.14','-2056.05','13.7327','5.84369','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_malothas' WHERE `entry` = 82008;

		#78554/NPC - Vindicator Maraad

			DELETE FROM creature WHERE id = 78554;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7037','1','4294967295','0','1','4068.79','-2372.78','94.6872','1.66015','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7040','1','4294967295','0','1','4221.55','-2814.22','17.1296','0.800784','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7040','1','4294967295','0','1','4386.65','-2811.78','4.91145','1.78075','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7041','1','4294967295','0','1','3834.89','-2775.64','94.0558','3.19539','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7041','1','4294967295','0','1','3958.91','-2520.15','69.7493','5.84448','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7042','1','4294967295','0','1','4526.04','-2632.74','1.89463','0.10128','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7043','1','4294967295','0','1','4380.66','-2065.69','2.33573','2.34674','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7043','1','4294967295','0','1','4522.78','-2181.86','-15.762','6.10103','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7043','1','4294967295','0','1','4613.97','-2223.53','9.63803','1.28408','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78554','1265','7025','7045','1','4294967295','0','1','3537.82','-2121.15','17.2781','0.0564164','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);

		#82025/NPC - Yoori

			DELETE FROM creature WHERE id = 82025;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82025','1265','7025','7037','1','4294967295','0','1','4061.38','-2295.95','65.0185','2.12653','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82025','1265','7025','7045','1','4294967295','0','1','3981.92','-2019.71','27.5168','3.75962','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_yoori' WHERE `entry` = 82025;

		#82016/NPC - Durphorn the Bullheaded

			DELETE FROM creature WHERE id = 82016;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82016','1265','7025','7037','1','4294967295','0','1','4057.16','-2289.11','64.9979','2.00044','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82016','1265','7025','7045','1','4294967295','0','1','3860.92','-2030.94','17.6147','5.60851','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_durphorn_the_bullheaded' WHERE `entry` = 82016;

		#82260/NPC - Rainiara the Kingslayer

			DELETE FROM creature WHERE id = 82260;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82260','1265','7025','7037','1','4294967295','0','1','4033.17','-2303.81','71.0954','0.788713','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82260','1265','7025','7045','1','4294967295','0','1','3653.91','-2083.01','7.47502','1.30584','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_rainiara_the_kingslayer' WHERE `entry` = 82260;

		#82264/NPC - Johnny Oshimo

			DELETE FROM creature WHERE id = 82264;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82264','1265','7025','7037','1','1','0','0','4092.7','-2306.96','64.9211','2.51801','300','0','0','87515','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82264','1265','7025','7045','1','4294967295','0','1','3736.49','-2045.19','12.3724','3.16184','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_johnny_oshimo' WHERE `entry` = 82264;

		#82014/NPC - Archon

			DELETE FROM creature WHERE id = 82014;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82014','1265','7025','7037','1','4294967295','0','1','4083.9','-2306.21','65.0406','0.0832898','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82014','1265','7025','7045','1','4294967295','0','1','3960.82','-2019.81','24.8669','0.0832898','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_tanaan_archon' WHERE `entry` = 82014;

		#82009/NPC - Barbery the Crazy Cat Lady

			DELETE FROM creature WHERE id = 82009;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82009','1265','7025','7037','1','4294967295','0','1','4093.24','-2298.36','65.0266','0.461711','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82009','1265','7025','7045','1','4294967295','0','1','3925.39','-2026.74','19.604','0.454164','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_barbery_the_cray_cat_lady' WHERE `entry` = 82009;

		#82075/NPC - Ryii the Shameless

			DELETE FROM creature WHERE id = 82075;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82075','1265','7025','7037','1','4294967295','0','1','4095.91','-2289.02','64.9987','1.39051','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82075','1265','7025','7045','1','4294967295','0','1','3689.23','-2069.73','7.68034','1.39051','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_ryii_the_shameless' WHERE `entry` = 82075;

		#82017/NPC - Miserain Starsorrow

			DELETE FROM creature WHERE id = 82017;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82017','1265','7025','7045','1','4294967295','0','0','3716.66','-2054.56','10.1277','3.22075','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82017','1265','7025','7037','1','4294967295','0','0','4084.98','-2295.92','65.0191','0.670326','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_miserain_starsorrow' WHERE `entry` = 82017;

		#82189/NPC - Lupas

			DELETE FROM creature WHERE id = 82189;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82189','1265','7025','7037','1','4294967295','0','1','4067.34','-2283.15','64.9841','1.76585','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82189','1265','7025','7045','1','4294967295','0','1','3736.97','-2043.52','12.4679','3.39335','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_tanaan_lupas' WHERE `entry` = 82189;

		#82263/NPC - Agios Lumen

			DELETE FROM creature WHERE id = 82263;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82263','1265','7025','7037','1','4294967295','0','1','4014.39','-2298.41','59.3297','3.98826','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82263','1265','7025','7044','1','4294967295','0','1','4014.03','-1998.66','31.9664','3.16719','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_agios_lumen' WHERE `entry` = 82263;

		#300004/NPC - Blaze of Glory Trigger

			DELETE FROM creature_template WHERE entry = 300004;
			insert into `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('300004','0','0','11686','0','0','0','Blaze of Glory Trigger (Tanaan)',NULL,NULL,'0','1','1','0','0','35','0','0','1','1.14286','1.14286','1','0','0','1','0','0','1','1','0','33554950','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','1');
			DELETE FROM creature WHERE id = 300004;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('300004','1265','7025','7041','1','4294967295','0','0','4045.68','-2587.89','52.0885','2.10206','300','0','0','42','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('300004','1265','7025','7041','1','4294967295','0','0','3944.56','-2654.11','54.7813','4.13624','300','0','0','42','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('300004','1265','7025','7041','1','4294967295','0','0','3999.03','-2667.21','47.3317','4.36793','300','0','0','42','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('300004','1265','7025','7038','1','4294967295','0','0','3766.49','-2581.04','58.1768','2.46727','300','0','0','42','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('300004','1265','7025','7041','1','4294967295','0','0','3849.51','-2535.05','65.301','2.09028','300','0','0','42','0','0','0','0','0','2048','0','0','0','0',NULL);

		#83652/NPC - Battered Draenei Prisoner

			UPDATE creature_template SET faction = 35 WHERE entry = 83652;

		#78556/NPC - Ariok

			DELETE FROM creature WHERE id = 78556;
			insert into `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('10123139','78556','1265','7025','7041','1','4294967295','0','1','3936.05','-2520.35','69.7308','3.9808','120','0','0','1','0','0','3','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('10123140','78556','1265','7025','7037','1','4294967295','0','1','4005.06','-2345.96','73.3521','0.176713','120','0','0','1','0','0','3','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('10123141','78556','1265','7025','7041','1','4294967295','0','1','3842.37','-2787.32','94.3774','5.56085','120','0','0','1','0','0','3','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`guid`, `id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('10123142','78556','1265','7025','7037','1','4294967295','0','1','4066.56','-2375.84','95.1081','1.66777','120','0','0','1','0','0','3','0','0','0','0','0','0','0',NULL);
			UPDATE `creature_template` SET `ScriptName` = 'npc_tanaan_ariok' WHERE `entry` = 78556;

		#78507/NPC - Bleeding Hollow Savage

			DELETE FROM creature WHERE id = 78507;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7038','1','4294967295','0','1','3813.13','-2647.02','76.6225','5.00997','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7038','1','4294967295','0','1','3824.45','-2606.24','68.4936','2.93846','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3913.97','-2670.92','60.018','2.33773','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3965.82','-2717.17','58.1562','0','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3938.04','-2667.07','54.8641','1.04654','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','4031.32','-2591.82','52.1708','5.94756','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','4049.78','-2581.44','52.4712','3.51829','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3886.69','-2535.21','68.949','3.48892','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3901.78','-2633.34','69.1469','1.47195','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3997.7','-2682.78','46.7833','6.2165','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3997.6','-2662.39','45.8952','5.09835','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3926.9','-2528.65','67.2492','5.49972','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3899.14','-2578.67','66.7021','0.726227','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3868.25','-2538.94','65.6961','3.54324','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7038','1','4294967295','0','1','3826.23','-2572.5','59.1139','0.239924','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7038','1','4294967295','0','1','3804.06','-2606.63','61.5268','2.92314','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7038','1','4294967295','0','1','3814.09','-2602.29','64.6878','0.164095','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3977.09','-2623.43','54.7167','1.98993','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3964.3','-2718.54','58.1771','0','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3944.13','-2506.16','69.3792','6.10107','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3941.69','-2512.73','69.3792','1.24429','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3928.2','-2509.1','70.3047','5.48109','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7041','1','4294967295','0','1','3933.87','-2515.33','69.8392','3.35953','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7039','1','4294967295','0','1','4087.69','-2803.83','37.4625','3.03785','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7129','1','4294967295','0','1','4107.38','-2796.67','36.5274','0.173898','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7039','1','4294967295','0','1','4098.55','-2780.25','37.9847','1.73043','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7039','1','4294967295','0','1','4054.99','-2812.4','43.9728','0.262582','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('78507','1265','7025','7039','1','4294967295','0','1','4085.38','-2787.35','38.5265','5.00986','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

			UPDATE `creature_template` SET `unit_flags` = 512, `ScriptName` = 'npc_bleeding_hollow_sauvage' WHERE `entry` = 78507;

		#81274/NPC - Mana Bombs Conversation Controller

			DELETE FROM creature WHERE id = 81274;
--			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81274','1265','7025','7037','1','4294967295','0','0','4066.48','-2374.12','95.8355','1.99491','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

		#82647/NPC - Tormented Soul

			DELETE FROM creature WHERE id = 82647;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4097.15','-2409.48','85.6964','4.20933','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4037.09','-2414.07','85.7199','5.17088','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4077.23','-2430.74','86.7388','2.979','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4087.65','-2389.55','69.5353','0.998699','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4041.42','-2399.02','69.535','4.33305','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4083.68','-2389.12','71.4021','2.33791','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4086.02','-2409.78','71.5768','1.85001','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4085.74','-2411','69.5349','1.75095','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4053.16','-2384.11','69.5353','4.02966','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4052.55','-2423.21','69.5352','1.49249','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4065.16','-2422.77','69.8573','2.15943','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4066.64','-2375.36','75.966','5.93413','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4073.86','-2422.48','69.5356','1.94682','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4038.44','-2400.77','86.4515','4.79376','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4055.67','-2432.83','84.9332','4.9045','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4077.65','-2431.57','86.7388','3.06507','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4049.07','-2396.69','86.4515','5.38548','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4087.23','-2390.67','69.5349','2.24862','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4040.75','-2399.83','70.747','0.342092','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4085.39','-2391.87','71.429','1.95758','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4073.2','-2426.73','69.5359','2.39061','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4067.04','-2374.08','76.0378','5.93404','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4082.86','-2385.75','71.846','2.41921','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4037.99','-2403.76','70.8322','4.47927','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4084.21','-2394.95','69.535','3.73435','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('82647','1265','7025','7037','1','4294967295','0','0','4038.08','-2405.78','70.8342','4.93399','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);

		#83670/NPC - Blood Ritual Orb

			DELETE FROM creature WHERE id = 83670;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('83670','1265','7025','7039','1','4294967295','0','0','3984.02','-2948.06','92.9','1.75538','300','0','0','40','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('83670','1265','7025','7039','1','4294967295','0','0','3976.39','-2900.08','64.0878','0','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('83670','1265','7025','7039','1','4294967295','0','0','3982.33','-2936.48','65.9404','0','120','0','0','1','0','0','0','0','0','0','0','0','0','0',NULL);
			UPDATE creature_template SET scriptname = "npc_blood_ritual_orb", npcflag = 1 WHERE entry = 83670;

		#78560/NPC - Archmage Khadgar (Bridge)

			UPDATE creature_template SET scriptname = "npc_archmage_khadgar_bridge", npcflag = 1 WHERE entry = 78560;

		#81696/NPC - Teron Gor

			DELETE FROM creature WHERE id = 81696;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('81696','1265','7025','7037','1','4294967295','0','0','3964','-2272.47','59.9632','4.74577','300','0','0','80892','0','0','0','0','0','2048','0','0','0','0',NULL);

		#79097/NPC - Kargath Bladefist

			UPDATE `creature_template` SET `ScriptName` = 'npc_kargath_bladefist' WHERE `entry` = 79097;
			UPDATE `creature_template` SET `unit_flags` = 33554692 WHERE `entry` = 79097;

		#82057/NPC - Shattered Hand Brawler

			UPDATE creature_template SET scriptname = "npc_shattered_hand_brawler" WHERE entry = 82057;

		#78561/NPC - Archmage Khadgar

			UPDATE creature_template SET scriptname = "npc_archmage_khadgar_shadowmoon" WHERE entry = 78561;

		#79583/NPC - Ungra

			UPDATE `creature_template` SET `ScriptName` = 'npc_tanaan_ungra' WHERE `entry` = 79583;

		#79585/NPC - Taskmaster Gurran

			UPDATE `creature_template` SET `ScriptName` = 'npc_taskmaster_gurran' WHERE `entry` = 79585;

		#79593/NPC - Ankova the Fallen

			UPDATE `creature_template` SET `ScriptName` = 'npc_ankova_the_fallen' WHERE `entry` = 79593;

		#78994/NPC - Yrel

			UPDATE creature_template SET npcflag = 2, scriptname = "npc_tanaan_yrel_summon" WHERE entry = 78994;

		#79794/NPC - Yrel

			UPDATE creature_template SET npcflag = 2, scriptname = "npc_tanaan_yrel" WHERE entry = 79794;

		#79537/NPC - Exarch Maladaar

			UPDATE creature_template SEt scriptname = "npc_exarch_maladaar_tanaan_cave" WHERE entry = 79537;

		#79702/NPC - Keli Dan the Breaker

			UPDATE `creature_template` SET `ScriptName` = 'npc_keli_dan_the_breaker' WHERE `entry` = 79702;

		#78430/NPC - Cordana Felsong

			UPDATE creature_template SET scriptname = "npc_cordana_felsong_blackrock" WHERE entry = 78430;

		#79675/NPC - Lady Liadrin

			UPDATE `creature_template` SET `ScriptName` = 'npc_lady_liadrin_blackrock' WHERE `entry` = 79675;

		#80786/NPC - Blackrock Grunt

			UPDATE `creature_template` SET `ScriptName` = 'npc_blackrock_grunt' WHERE `entry` = 80786;

		#80775-81294/NPC - Ogron Warcrusher

			UPDATE `creature_template` SET `ScriptName` = 'npc_ogron_warcrusher' WHERE `entry` IN (80775, 81294);

		#78996/NPC - Farseer Drek Thar

			UPDATE creature_template SET scriptname = "npc_farseer_drek_thar" WHERE entry = 78996;

		#79917/NPC - Ga Nar

		DELETE FROM creature WHERE id = 79917;
		insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('79917','1265','7025','7045','1','4294967295','0','0','3538.48','-2133.2','17.2782','0.391606','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);
		insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('79917','1265','7025','7043','1','4294967295','0','0','4169.52','-2061.3','1.62979','3.11442','120','0','0','1','0','0','2','0','0','0','0','0','0','0',NULL);

		#78568/NPC - Thaelin Darkanvil

			UPDATE `creature_template` SET `ScriptName` = 'npc_thaelin_darkanvil_tanaan' WHERE `entry` = 78568;

		#86039/NPC - Gogluk

			UPDATE creature_template SET vehicleid = 3902, scriptname = "npc_tanaan_gogluk", faction = 14, minlevel = 90, maxlevel = 90 WHERE entry = 86039;

		#86691-86690/NPC - Gogluk Adds

			DELETE FROM creature WHERE id IN (86691, 86690);
			UPDATE creature_template SET scriptname = "npc_tanaan_gogluk_adds" WHERE entry IN (86691, 86690);

		#80521/NPC - Thaelin Darkanvil

			DELETE FROM creature WHERE id = 80521;
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('80521','1265','7025','7044','1','4294967295','0','0','4063.27','-2022.57','75.5059','6.11318','300','0','0','87515','0','0','0','0','0','2048','0','0','0','0',NULL);
			insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) values('80521','1265','7025','7044','1','4294967295','0','1','4063.77','-2020.12','75.4733','2.50024','120','0','0','1','0','0','4739','0','0','0','0','0','0','0',NULL);
			UPDATE creature_template SET scriptname = "npc_thaelin_tanaan_questgiver" WHERE entry = 80521;

		#78553/NPC - Thrall

			UPDATE `creature_template` SET `ScriptName` = 'npc_thrall_tanaan_boats' WHERE `entry` = 78553;

		#81711/NPC - Shadowmoon Ritualist (Tanaan Jungle)

			REPLACE INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES('81711','0','0','59618','59619','59620','0','Shadowmoon Ritualist','','','0','90','90','0','0','14','0','0','1','1.14286','1.14286','1','0','0','1','2000','2000','1','1','2','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','157355','158396','1604','0','0','0','0','0','0','0','0','0','','0','3','1','0.9','1','1','1','0','113579','0','0','0','0','0','0','1','0','0','0','','19116');
			UPDATE creature_template SET scriptname = "npc_shadowmoon_ritualist"  WHERE entry = 81711;

		#82006/NPC - Pazo Stonehoof

			UPDATE creature_template SET scriptname = "npc_generic_tanaan_guardian" WHERE entry = 82006;

		#81895/NPC - Bleeding Hollow Savage

			UPDATE `creature_template` SET `unit_flags` = 33554436 WHERE `entry` = 81895;

		#78563/NPC - Archmage Khadgar

			UPDATE creature_template SET scriptname = "" WHERE entry = 78563;

		#300006/NPC - Final Tanaan Trigger

			insert into `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('300006','0','0','11686','0','0','0','Black Rock trigger (Tanaan)',NULL,NULL,'0','1','1','0','0','35','0','0','1','1.14286','1.14286','1','0','0','1','0','0','1','1','0','33554950','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','npc_final_tanaan_trigger','1');

		#78563/NPC - Archmage Khadgar

			UPDATE `creature_template` SET `ScriptName` = 'npc_tanaan_khadgar_final' WHERE `entry` = 78563;

		#86780/NPC - Napestone Riverbeast

			UPDATE creature_template SET faction = 14, scriptname = "npc_tanaan_napestone_riverbeast" WHERE entry = 86780;

		#82530/NPC - Mandragora

			UPDATE creature_template SET minlevel = 90, maxlevel = 90, faction = 14, scriptname = "npc_tanaan_mandragora" WHERE entry = 82530;

		#80574/NPC - Overseer Gotrigg

			UPDATE creature_template SET scriptname = "npc_tanaan_overseer_gotrigg" WHERE entry = 80574;

		#80803 - 85145 - 85146 - 80804/NPC - Liberated Tanaan Prisonner

			UPDATE creature_template SET scriptname = "npc_tanaan_arena_helper" WHERE entry IN (80803, 80804, 85145, 85146);
		-- Game Objects

		#233104/Game Object - Stasis Rune

			DELETE FROM gameobject WHERE id = 233104;
			insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('233104','1265','7025','7037','1','1','4066.84','-2403.46','77.7679','1.56486','0','0','0.705006','0.709201','300','0','1','0',NULL);
			UPDATE `gameobject_template` SET `ScriptName` = 'gob_static_rune' WHERE `entry` = 233104;

		#229598/Game Object - Mark of the Burning Blade

			DELETE FROM gameobject WHERE id = 229598;
			insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('229598','1265','7025','7037','1','1','4099.81','-2402.92','69.8151','3.13566','0','0','0.999996','0.00296636','300','0','1','0',NULL);

		#233614/Game Object - Chamber Door

			DELETE FROM gameobject WHERE id = 233614;

		#229350/Game Object - Bleeding Hollow Cage

			UPDATE `gameobject_template` SET `ScriptName` = 'go_bleeding_hollow_cage' WHERE `entry` = 229350;

		#229325/Game Object - Bleeding Hollow Cage

			UPDATE `gameobject_template` SET `ScriptName` = 'go_bleeding_hollow_cage' WHERE `entry` = 229325;


		#23356/233057/Game Object - Mark of (Tanaan)

			UPDATE gameobject_template SET scriptname = "gob_mark_of_tanaan" WHERE entry IN (233056, 233057);

		#231119/Game Object - Blackrock Powder Keg

			UPDATE `gameobject_template` SET `data1` = 231119 WHERE `entry` = 231119;
			UPDATE `gameobject_template` SET `data1` = 231119 WHERE `entry` = 231119;
			UPDATE `gameobject_template` SET `data2` = 0, `data11` = 1, `data13` = 1 WHERE `entry` = 231119;
			UPDATE `gameobject_template` SET `flags` = 0 WHERE `entry` = 231119;
			UPDATE `gameobject_template` SET `data3` = 1, `data15` = 1 WHERE `entry` = 231119;
			UPDATE `gameobject_template` SET `data0` = 43 WHERE `entry` = 231119;
			UPDATE `gameobject_template` SET `data10` = 1, `data12` = 1, `data14` = 0, `data21` = 80 WHERE `entry` = 231119;
			DELETE FROM gameobject_loot_template WHERE entry = 231119;
			INSERT INTO `gameobject_loot_template` VALUE (231119, 112323, -100, 1, 0, 1, 1, '');

		#231066/Game Object - Makeshift Plunger

			UPDATE gameobject_template SET scriptname = "gob_makeshift_plunger" WHERE entry = 231066;

		#231261/Game Object - Worldbreaker Side Turret

			UPDATE gameobject_template SET scriptname = "gob_worldbreaker_side_turret" WHERE entry = 231261;

			DELETE FROM gameobject WHERE id = 231261;
			insert into `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`) values('231261','1265','7025','7044','1','65535','4051.4','-2021.76','77.4','3.10846','0','0','0','0','0','255','1','0',NULL);

		#300005/NPC - Black Rock Trigger

			insert into `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('300005','0','0','11686','0','0','0','Black Rock trigger (Tanaan)',NULL,NULL,'0','1','1','0','0','35','0','0','1','1.14286','1.14286','1','0','0','1','0','0','1','1','0','33554950','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','1','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','npc_black_rock_trigger','1');

		#232538/Game Object - Main Cannon Trigger

			UPDATE gameobject_template SET scriptname = "gob_main_cannon_trigger" WHERE entry = 232538;
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
			DELETE FROM `creature_questrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35240;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35240;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81762, 35240);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81762;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35240;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 35240);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;


		#35241/Quest - Vengeance for the Fallen

			DELETE FROM `creature_questrelation` WHERE `quest` = 35241;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35241;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35241;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81761, 35241);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81761;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35241;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35241;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78553, 35241);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78553;
			DELETE FROM `creature_questrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35242;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35242;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81763, 35242);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81763;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35242;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78554, 35242);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78554;

		#34423/Quest - Altar Altercation

			DELETE FROM `creature_questrelation` WHERE `quest` = 34423;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34423;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34423;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78559, 34423);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78559;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34423;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34423;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78560, 34423);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78560;

		#34425/Quest - The Kargathar Proving Grounds

			DELETE FROM `creature_questrelation` WHERE `quest` = 34425;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34425;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34425;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34425);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34425;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34425;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78560, 34425);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78560;

		#34478/Quest - A Potential Ally

			DELETE FROM `creature_questrelation` WHERE `quest` = 34478;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34478;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34478;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34478);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34478;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34478;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79537, 34478);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79537;

		#34429/Quest - Kill your hundred

			DELETE FROM `creature_questrelation` WHERE `quest` = 34429;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34429;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34429;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34429);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34429;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34429;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78561, 34429);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78561;

		#34737/Quest - Masters of Shadow

			DELETE FROM `creature_questrelation` WHERE `quest` = 34737;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34737;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34737;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79661, 34737);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79661;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34737;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34737;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79675, 34737);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79675;
			UPDATE `quest_template` SET `RequiredClasses` = 0, `RequiredRaces` = 33555378 WHERE `Id` = 34737;

		#34431/Quest - Masters of Shadow

			DELETE FROM `creature_questrelation` WHERE `quest` = 34431;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34431;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34431;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79661, 34431);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79661;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34431;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34431;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79537, 34431);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79537;
			UPDATE `quest_template` SET `RequiredClasses` = 0, `RequiredRaces` = 18875469 WHERE `Id` = 34431;

		#34739/Quest - The Shadowmoon Clan

			DELETE FROM `creature_questrelation` WHERE `quest` = 34739;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34739;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34739;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78553, 34739);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78553;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34739;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34739;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79315, 34739);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79315;

		#34434/Quest - Yrel

			DELETE FROM `creature_questrelation` WHERE `quest` = 34434;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34434;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34434;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79794, 34434);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79794;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34434;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34434;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78994, 34434);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78994;


		#34740/Quest - Yrel

			DELETE FROM `creature_questrelation` WHERE `quest` = 34740;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34740;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34740;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79794, 34740);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79794;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34740;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34740;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78994, 34740);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78994;

		#34436/Quest - Keli Dan the Breaker

			DELETE FROM `creature_questrelation` WHERE `quest` = 34436;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34436;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34436;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79537, 34436);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79537;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34436;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34436;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78562, 34436);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78562;

		#34741/Quest - Keli Dan the Breaker

			DELETE FROM `creature_questrelation` WHERE `quest` = 34741;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34741;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34741;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79675, 34741);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79675;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34741;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34741;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78562, 34741);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78562;

		#35005/Quest - Prepare for Battle

			DELETE FROM `creature_questrelation` WHERE `quest` = 35005;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35005;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35005;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78553, 35005);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78553;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35005;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35005;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78553, 35005);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78553;

		#34439/Quest - The Battle of the Forge

			DELETE FROM `creature_questrelation` WHERE `quest` = 34439;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34439;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34439;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78430, 34439);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78430;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34439;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34439;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78996, 34439);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78996;

		#34442/Quest - Ga nar of the Frostwolf

			DELETE FROM `creature_questrelation` WHERE `quest` = 34442;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34442;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34442;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78996, 34442);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78996;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34442;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34442;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79917, 34442);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79917;

		#34987/Quest - The Gunpowder Plot

			DELETE FROM `creature_questrelation` WHERE `quest` = 34987;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34987;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34987;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78569, 34987);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78569;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34987;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34987;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78569, 34987);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78569;

		#34958/Quest - The Shadow of the Worldbreaker

			DELETE FROM `creature_questrelation` WHERE `quest` = 34958;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34958;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34958;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78568, 34958);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78568;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34958;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34958;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78568, 34958);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78568;

		#34925/Quest - Polishing the Iron Throne

			DELETE FROM `creature_questrelation` WHERE `quest` = 34925;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34925;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34925;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79917, 34925);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79917;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34925;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34925;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79917, 34925);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79917;

		#34437/Quest - The Prodigal Frostwolf

			DELETE FROM `creature_questrelation` WHERE `quest` = 34437;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34437;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34437;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79917, 34437);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79917;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34437;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34437;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78996, 34437);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78996;

		#35747/Quest - Taking a Trip to the Top of the Tank

			DELETE FROM `creature_questrelation` WHERE `quest` = 35747;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35747;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35747;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78563, 35747);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78563;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35747;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35747;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (80521, 35747);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=80521;

		#34445/Quest - a Taste of Iron

			DELETE FROM `creature_questrelation` WHERE `quest` = 34445;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34445;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34445;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (80521, 34445);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 80521;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34445;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34445;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (80521, 34445);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=80521;

		#35884/Quest - The Home Stretch

			DELETE FROM `creature_questrelation` WHERE `quest` = 35884;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35884;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35884;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (80521, 35884);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 80521;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35884;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35884;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78563, 35884);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78563;

		#34446/Quest - The Home Stretch

			DELETE FROM `creature_questrelation` WHERE `quest` = 34446;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34446;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34446;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (80521, 34446);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 80521;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34446;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34446;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78563, 34446);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78563;

		#34421/Quest - Bled Dry

			DELETE FROM `creature_questrelation` WHERE `quest` = 34421;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34421;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34421;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78573, 34421);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78573;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34421;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34421;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 34421);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;

		#34427/Quest - A Potential Ally

			DELETE FROM `creature_questrelation` WHERE `quest` = 34427;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34427;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34427;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34427);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34427;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34427;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78996, 34427);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78996;

		-- Phases (1st try)

			UPDATE creature SET phasemask = 4294967295 WHERE map = 1265;
			UPDATE gameobject SET phasemask = 65535 WHERE map = 1265;
			UPDATE gameobject SET phasemask = 32 WHERE id IN (232502, 232503, 232504); -- débris
			UPDATE gameobject SET phasemask = 32 WHERE id = 237667; -- portail
			UPDATE gameobject SET phasemask = 32 WHERE id = 231763; -- iron horde capsule
			UPDATE creature SET phasemask = 1 WHERE id = 78333;
			UPDATE gameobject SET phasemask = 1 WHERE id = 234622;
			UPDATE gameobject SET phasemask = 32 WHERE id = 232500; -- Destroyed Iron Horde War Machine Type 01
			UPDATE creature SET phasemask = 3 WHERE id IN
			( 78430, 79316, 78553, 78568, 78556, 78554, 78569, 79315, 79675, 78558, 78965, 76793)
			AND areaId = 7037 AND position_z < 95.2 AND position_z > 90;
			UPDATE creature SET phasemask = 8 WHERE id IN
			( 78430, 79316, 78553, 78568, 78556, 78554, 78569, 79315, 79675, 78558, 78559)
			AND areaId = 7041 AND position_x < 3963 AND position_x > 3931 AND position_y < -2501 AND position_y > -2537;
			UPDATE gameobject SET phasemask = 2 WHERE id = 231197;
			UPDATE gameobject SET phasemask = 65536 WHERE id IN (232239, 232240, 232241, 232242) AND map = 1265;

		-- Phases (2nd try)

			UPDATE gameobject SET phasemask = 1 WHERE id = 237670;
			UPDATE gameobject SET phasemask = 2 WHERE id = 234622;
			UPDATE creature SET phasemask = 4 WHERE id = 81695;
			UPDATE gameobject SET phasemask = 8 WHERE id = 232538;
			UPDATE creature SET phasemask = 8 WHERE id = 81696;
			UPDATE gameobject SET phasemask = 8 WHERE id = 233707;
			UPDATE creature SET phasemask = 16 WHERE id IN (82647, 78333);
-- 			UPDATE creature SET phasemask = 32 WHERE id IN => platform group
-- 			UPDATE creature SET phasemask = 64 WHERE id IN => house group
			UPDATE creature SET phasemask = 128 WHERE id = 85142;
			UPDATE creature SET phasemask = 256 WHERE id = 82871;
			UPDATE creature SET phasemask = 512 WHERE id = 78529;
			UPDATE creature SET phasemask = 1024 WHERE id = 85141;
--			UPDATE creature SET phasemask = 2048 WHERE id IN => altar group
			UPDATE creature SET phasemask = 4096 WHERE id = 78556;
			UPDATE gameobject SET phasemask = 8192 WHERE id = 231137;
			UPDATE gameobject SET phasemask = 131072 WHERE id IN (231958, 231957) AND map = 1265;
			UPDATE gameobject SET phasemask = 1048576 WHERE id IN (232239, 232240, 232241, 232242) AND map = 1265;
			UPDATE gameobject SET phasemask = 2097152 WHERE id = 231197 AND areaid = 7042;
			UPDATE gameobject SET phasemask = 536870912 WHERE id = 237667;
			UPDATE gameobject SET phasemask = 1073741824 WHERE id = 231261;
		-- Misc

			DELETE FROM creature WHERE id IN
			(82125, 81776, 79185, 79659, 81885, 78965, 82973, 79770, 88354);
			DELETE FROM creature WHERE id = 81711;
			DELETE FROM creature_template_addon WHERE entry = 82260;
			DELETE FROM creature_template_addon WHERE entry = 78507;
			UPDATE creature SET spawntimesecs = 15 WHERE id = 78883;
			DELETE FROM creature_model_info WHERE modelid = 53438;
			insert into `creature_model_info` values('53438','2.5','2.5','0','0');
			UPDATE creature_template SET scriptname = "npc_generic_tanaan_guardian", faction = 2580 WHERE entry IN
			(82188, 81990, 82007, 82010, 81994, 82011, 81997, 82082, 82191, 82012, 82014, 82002,
			81996, 81998, 79062, 81993, 81995, 82000, 82001, 82003, 82004, 82005, 82008, 82025,
			82016, 82260, 82264, 82014, 82009, 82075, 82017, 82189, 82263, 82259, 81993, 82006,
			82013, 78569, 79315, 82187);
			UPDATE quest_template_objective SET `index` = 0 WHERE id = 273635;
			UPDATE quest_template_objective SET flags = 0 WHERE questid = 34393;
			UPDATE `quest_template` SET `Flags` = 8 WHERE `Id` = 35241;
			UPDATE `creature_template` SET `type_flags` = 4 WHERE `entry` = 78553;
			UPDATE quest_template_objective SET flags = 0 WHERE questid = 34423;
			UPDATE creature SET npcflag = 0 WHERE id = 78556;
			UPDATE creature_template SET npcflag = 1 WHERE entry = 78556;
			UPDATE creature SET npcflag = 1 WHERE id = 76643;
			UPDATE `creature_template`
			SET `minlevel` = 100,
			`maxlevel` = 100,
			`unit_flags` = 896,
			faction = 35,
			scriptname = "npc_generic_tanaan_escorter" WHERE `entry` IN
			(82125, 81776, 79185, 79659, 81885, 78965, 82973, 79770, 88354, 76793);
			DELETE FROM creature_template_addon WHERE entry = 81696 AND auras LIKE "%166539%";
			INSERT INTO creature_template_addon (entry, auras) VALUE (81696, "166539"); -- Ghost visual
			UPDATE creature_template SET scriptname = "npc_tanaan_khadgar_bridge" WHERE entry = 80244;
			UPDATE `quest_template` SET `Method` = 0 WHERE `Id` = 34420;
			UPDATE creature SET phasemask = 1 WHERE id IN (82057, 82141) AND areaid = 7040;
			UPDATE gameobject SET phasemask = 1 WHERE id IN (232239, 232240, 232241) AND areaid IN (7040, 7130);
			UPDATE gameobject_template SET data2 = 8000 WHERE entry = 233197;
			UPDATE `quest_template` SET `RequiredRaces` = 67108863 WHERE `Id` IN(34398,35933,34392,34393,34420,34422,34425,34429,34439,34442,34987,34958,34442,34925,34437,35747,34445);
			UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `Id` IN(35240,35242,34423,34478,34432,34431,34434,34436,35019,35884);
			UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `Id` IN(34421,35241,35005,34427,34739,34737,34740,34741,35005,34446);
			DELETE FROM creature WHERE id = 78568 AND npcflag = 0;
			DELETE FROM vehicle_template_accessory WHERE entry = 86039;
			INSERT INTO vehicle_template_accessory VALUE (86039, 86690, 0, 1, 'Cannon Turret', 5, 0);
			INSERT INTO vehicle_template_accessory VALUE (86039, 86691, 1, 1, 'Gun Turret', 5, 0);
			DELETE FROM npc_spellclick_spells WHERE npc_entry = 86039;
			INSERT INTO npc_spellclick_spells VALUE (86039, 173426, 1, 0);
			SELECT * FROM vehicle_template_accessory WHERE entry = 86039;
			UPDATE gameobject SET state = 2 WHERE id = 231261;
			UPDATE gameobject SET state = 1 WHERE id = 232539;
			DELETE FROM creature_template_addon WHERE entry = 78430;
			DELETE FROM creature_template_addon WHERE entry = 79593;
			DELETE FROM creature_template_addon WHERE entry IN (81996, 81999, 82000, 82001, 82004, 82007, 82008, 82189);
			DELETE FROM creature_template_addon WHERE entry IN (78554, 81994);
			DELETE FROM creature_template_addon WHERE entry = 78562;
			DELETE FROM creature_template_addon WHERE entry = 78563;
			DELETE FROM creature_template_addon WHERE entry IN (78650, 78651, 78906);
			UPDATE `creature_template` SET `unit_flags` = 570687620, `dynamicflags` = 32 WHERE `entry` IN (78650, 78651, 78906);
			DELETE FROM creature_template_addon WHERE entry = 78996;
			DELETE FROM creature_template_addon WHERE entry IN (82016, 82263);
			DELETE FROM creature_template_addon WHERE entry = 79589;
			DELETE FROM creature_template_addon WHERE entry = 79590;
			DELETE FROM creature_template_addon WHERE entry = 79702;
			DELETE FROM creature_template_addon WHERE entry = 82647;
			DELETE FROM creature_template_addon WHERE entry = 80290;
			DELETE FROM creature_template_addon WHERE entry = 80775;
			DELETE FROM creature_template_addon WHERE entry IN (80786, 81014);
			DELETE FROM creature_template_addon WHERE entry = 81063;
			DELETE FROM creature_template_addon WHERE entry = 81129;
			DELETE FROM creature_template_addon WHERE entry = 81357;
			DELETE FROM creature_template_addon WHERE entry = 81762;
			UPDATE `creature_template` SET `unit_flags` = 537133058, `dynamicflags` = 32 WHERE `entry` = 81762;
			DELETE FROM creature_template_addon WHERE entry = 81993;
			DELETE FROM creature_template_addon WHERE entry = 82259;
			DELETE FROM creature_template_addon WHERE entry IN (79316, 81995);
			DELETE FROM creature_template_addon WHERE entry = 81998;
			DELETE FROM creature_template_addon WHERE entry = 82002;
			DELETE FROM creature_template_addon WHERE entry = 82005;
			DELETE FROM creature_template_addon WHERE entry = 82057;
			DELETE FROM creature_template_addon WHERE entry = 82003;
			DELETE FROM creature_template_addon WHERE entry = 82014;
			DELETE FROM creature_template_addon WHERE entry IN (82264, 82006);
			DELETE FROM creature_template_addon WHERE entry = 82009;
			DELETE FROM creature_template_addon WHERE entry = 82187;
			DELETE FROM creature_template_addon WHERE entry IN (82012, 82013, 82015, 82025);
			DELETE FROM creature_template_addon WHERE entry = 82011;
			DELETE FROM creature_template_addon WHERE entry = 82373;
			DELETE FROM creature_template_addon WHERE entry = 82469;
			DELETE FROM creature_template_addon WHERE entry = 82484;
			DELETE FROM creature_template_addon WHERE entry = 83441;
			UPDATE `creature_template` SET `unit_flags` = 33554688 WHERE `entry` = 83441;
			DELETE FROM creature_template_addon WHERE entry = 83538;
			DELETE FROM creature_template_addon WHERE entry = 78820;
			DELETE FROM creature_template_addon WHERE entry IN (78883, 82075, 82082, 82188, 82191);
			UPDATE `quest_template` SET `Flags` = 1376264 WHERE `Id` = 35933;
			UPDATE creature_template SET faction = 35 WHERE entry = 80804;
			UPDATE creature_template SET faction = 35 WHERE entry = 85146;


		-- Adjust Scriptnames

			UPDATE creature_template SET scriptname = "npc_iron_grunt" WHERE entry = 78883;
			UPDATE creature_template SET scriptname = "npc_iron_gronnling" WHERE entry = 82484;
			UPDATE creature_template SET scriptname = "npc_gul_dan_trigger" WHERE entry = 300003;
			UPDATE creature_template SET scriptname = "npc_tormented_soul" WHERE entry = 82647;
			UPDATE creature_template SET scriptname = "npc_tanaan_ariok" WHERE entry = 78556;
			UPDATE creature_template SET scriptname = "npc_bleeding_hollow_sauvage" WHERE entry = 78507;
			UPDATE creature_template SET scriptname = "npc_blood_ritual_orb" WHERE entry = 83670;
			UPDATE creature_template SET scriptname = "npc_archmage_khadgar_bridge" WHERE entry = 78560;
			UPDATE creature_template SET scriptname = "npc_kargath_bladefist" WHERE entry = 79097;
			UPDATE creature_template SET scriptname = "npc_shattered_hand_brawler" WHERE entry = 82057;
			-- /!\ TP TO TANAAN FROM BLASTED LANDS
			UPDATE creature_template SET scriptname = "npc_archmage_khadgar_gossip" WHERE entry = 76643; -- /!\ TP TO TANAAN FROM BLASTED LANDS
			-- /!\ TP TO TANAAN FROM BLASTED LANDS
			
		-- Quests suit

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 35933 WHERE `Id` = 34398;
			UPDATE `quest_template` SET `PrevQuestId` = 34398, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 34392 WHERE `Id` = 35933;
			UPDATE `quest_template` SET `PrevQuestId` = 35933, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 34393 WHERE `Id` = 34392;
			UPDATE `quest_template` SET `PrevQuestId` = 34392, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 34420 WHERE `Id` = 34393;
			UPDATE `quest_template` SET `PrevQuestId` = 34393, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34420;

			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 34423, `ExclusiveGroup` = -34422, `NextQuestIdChain` = 0 WHERE `Id` = 34422;
			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 34423, `ExclusiveGroup` = -34422, `NextQuestIdChain` = 0 WHERE `Id` = 35240;
			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 34425, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34423;

			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 35005, `ExclusiveGroup` = -35241, `NextQuestIdChain` = 0 WHERE `Id` = 35241;
			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 35005, `ExclusiveGroup` = -35241, `NextQuestIdChain` = 0 WHERE `Id` = 34421;
			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 34425, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35005;

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34425;

			UPDATE `quest_template` SET `PrevQuestId` = 34425, `NextQuestId` = 34429, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34478;

			UPDATE `quest_template` SET `PrevQuestId` = 34425, `NextQuestId` = 34429, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34427;

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34429;

			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34434, `ExclusiveGroup` = -34432, `NextQuestIdChain` = 0 WHERE `Id` = 34432;
			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34434, `ExclusiveGroup` = -34432, `NextQuestIdChain` = 0 WHERE `Id` = 34431;
			UPDATE `quest_template` SET `PrevQuestId` = 34432, `NextQuestId` = 34436, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34434;
			UPDATE `quest_template` SET `PrevQuestId` = 34434, `NextQuestId` = 35019, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34436;
			UPDATE `quest_template` SET `PrevQuestId` = 34436, `NextQuestId` = 34439, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35019;

			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34740, `ExclusiveGroup` = -34739, `NextQuestIdChain` = 0 WHERE `Id` = 34739;
			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34740, `ExclusiveGroup` = -34739, `NextQuestIdChain` = 0 WHERE `Id` = 34737;
			UPDATE `quest_template` SET `PrevQuestId` = 34739, `NextQuestId` = 34741, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34740;
			UPDATE `quest_template` SET `PrevQuestId` = 34434, `NextQuestId` = 35019, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34741;
			UPDATE `quest_template` SET `PrevQuestId` = 34741, `NextQuestId` = 34439, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35005;

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 34442, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34439;

			UPDATE `quest_template` SET `PrevQuestId` = 34439, `NextQuestId` = 34925, `ExclusiveGroup` = -34442, `NextQuestIdChain` = 0 WHERE `Id` = 34442;
			UPDATE `quest_template` SET `PrevQuestId` = 34439, `NextQuestId` = 34925, `ExclusiveGroup` = -34442, `NextQuestIdChain` = 0 WHERE `Id` = 34987;
			UPDATE `quest_template` SET `PrevQuestId` = 34439, `NextQuestId` = 34925, `ExclusiveGroup` = -34442, `NextQuestIdChain` = 0 WHERE `Id` = 34958;

			UPDATE `quest_template` SET `PrevQuestId` = 34442, `NextQuestId` = 35747, `ExclusiveGroup` = -34925, `NextQuestIdChain` = 0 WHERE `Id` = 34925;
			UPDATE `quest_template` SET `PrevQuestId` = 34442, `NextQuestId` = 35747, `ExclusiveGroup` = -34925, `NextQuestIdChain` = 0 WHERE `Id` = 34437;

			UPDATE `quest_template` SET `PrevQuestId` = 34925, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34445;

			UPDATE `quest_template` SET `PrevQuestId` = 34445, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35884;

			UPDATE `quest_template` SET `PrevQuestId` = 34445, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34446;
			UPDATE `quest_template` SET `PrevQuestId` = 34740, `NextQuestId` = 35019, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34741;

			UPDATE `quest_template` SET `ExclusiveGroup` = 0 WHERE `Id` = 34442; -- (before -34442)
			UPDATE `quest_template` SET `ExclusiveGroup` = 0 WHERE `Id` = 34925; -- (before -34925)
			UPDATE `quest_template` SET `ExclusiveGroup` = 0 WHERE `Id` = 34437; -- (before -34925)
			UPDATE `quest_template` SET `PrevQuestId` = 35747 WHERE `Id` = 34445;
			-- Talks

			DELETE FROM creature_text WHERE entry = 82125;
			insert into `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) values('82125','0','0','I can open up a portal to Stormwind only briefly.','12','0','100','0','0','0','');
			insert into `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) values('82125','1','0','The Vanguard is falling. We will not let their great sacrifice be in vain !','12','0','100','0','0','0','Khadgar start Tanaan escort');
			insert into `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) values('82125','2','0','In Here ! ','12','0','100','0','0','0','Khadgar reaches house');
