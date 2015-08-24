#83746/NPC - Rukhmar

	SET @RUKHMAR = 83746;

	UPDATE
		`creature_template`
	SET
		`InhabitType` = 4,
		`movementtype` = 0,
		`speed_walk` = 2,
		`speed_run` = 2,
		`speed_fly` = 2,
		`unit_flags` = 0,
		`dmg_multiplier` = 10,
		`scriptname` = "boss_rukhmar",
		`mechanic_immune_mask` = 769638399,
		`trackingquestid` = 37464,
		`rank` = 3
	WHERE `entry` = @RUKHMAR;

	DELETE FROM creature WHERE id = @RUKHMAR;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('83746','1116','6722','6722','1','1','0','0','131.766','2506.42','135.821','2.64288','900','0','0','43331200','0','2','0','0','0','2176','0','0','0','0',NULL);
--	insert into creature_addon (guid, path_id) value ((select distinct guid from creature where id = @RUKHMAR), @RUKHMAR);


#83769/NPC - Energized Phoenix

	UPDATE creature_template SET inhabittype = 4, faction = 14, scriptname = "npc_energized_phoenix" WHERE entry = 83769;

#83783/NPC - Depleted Phoenix

	DELETE FROM creature_template_addon WHERE entry = 83783;
	INSERT INTO creature_template_addon (entry, auras) VALUE (83783, "169704");

#83768/Piles of Ash

	UPDATE creature_template SET unit_flags = unit_flags |  0x02000000 WHERE entry = 83768;
	UPDATE creature_template SET unit_flags = unit_flags |  0x00000004 WHERE entry = 83768;

#167629/167647/Spell - Blaze Of Glory/Loose Quills

	DELETE FROM spell_script_names WHERE spell_id IN (167629, 167647, 167679);
	INSERT INTO spell_script_names VALUE (167629, "spell_rukhmar_blaze_of_glory");
	INSERT INTO spell_script_names VALUE (167647, "spell_rukhmar_loose_quills");
	INSERT INTO spell_script_names VALUE (167679, "spell_aura_pierced_armor");

-- Waypoints

DELETE FROM waypoint_data WHERE id = 83746;
/*insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','1','109.04','2529.92','98.394','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','2','46.0705','2607.37','96.1203','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','3','-75.2959','2574.59','60.547','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','4','-282.416','2402.54','58.2095','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','5','-311.81','2208.17','97.8865','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','6','-215.77','2094.11','156.292','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','7','-192.961','1983.35','170.978','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','8','-320.39','1846.72','165.739','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','9','-486.295','1903.25','164.057','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','10','-517.462','2090.23','173.098','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','11','-344.784','2202.69','152.241','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','12','-223.176','2141.83','139.732','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','13','-165.713','2040.04','170.019','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','14','-51.3877','2073.45','212.529','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','15','76.4955','2207.15','254.127','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','16','143.84','2333.23','128.493','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','17','209.969','2448.18','41.7817','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','18','147.927','2489.04','88.0799','0','0','0','0','100','0');
*/