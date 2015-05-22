#83746/NPC - Rukhmar

	DELETE FROM creature WHERE id = 83746;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('83746','1116','6722','6722','1','1','0','0','131.766','2506.42','135.821','2.64288','300','0','0','43331200','0','2','0','0','0','2176','0','0','0','0',NULL);
	UPDATE `creature_template` SET `InhabitType` = 7, movementtype = 2, speed_walk = 2, speed_run = 2, speed_fly = 2, unit_flags = 0, scriptname = "boss_rukhmar" WHERE `entry` = 83746;
	UPDATE creature_template SET mechanic_immune_mask = 8388624 WHERE entry = 83746;
	UPDATE creature_template SET trackingquestid = 37464 WHERE entry = 83746;

#83769/NPC - Energized Phoenix

	UPDATE creature_template SET inhabittype = 7, faction = 14, scriptname = "npc_energized_phoenix" WHERE entry = 83769;

#167629/167647/Spell - Blaze Of Glory/Loose Quills

	DELETE FROM spell_script_names WHERE spell_id IN (167629, 167647);
	INSERT INTO spell_script_names VALUE (167629, "spell_rukhmar_blaze_of_glory");
	INSERT INTO spell_script_names VALUE (167647, "spell_rukhmar_loose_quills");

-- Waypoints

insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','1','159.467','2485.19','95.2812','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','2','131.642','2541.78','99.7272','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','3','92.9132','2593.45','101.869','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','4','92.9132','2593.45','101.869','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','5','18.0673','2584.03','108.494','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','6','-75.3318','2568.56','98.2835','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','7','-118.821','2533.89','105.375','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','8','-187.939','2458.2','118.235','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','9','-241.013','2421.04','88.5874','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','10','-291.123','2345.03','108.93','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','11','-267.152','2253.33','147.047','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','12','-267.152','2253.33','147.047','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','13','-249.363','2177.66','170.006','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','14','-233.294','2107.85','156.603','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','15','-229.992','2022.79','178.508','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','16','-257.973','1925.27','175.231','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','17','-366.153','1897.23','187.782','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','18','-437.733','1921.32','184.12','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','19','-483.198','2002.62','154.456','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','20','-482.504','2091.79','154.218','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','21','-407.019','2158.63','149.375','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','22','-321.443','2163.32','165.182','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','23','-254.914','2130.66','157.13','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','24','-148.944','2053.54','166.264','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','25','-22.8946','2029.25','163.046','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','26','26.2592','2108.34','216.546','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','27','73.467','2204.19','229.218','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','28','115.75','2301.41','178.647','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','29','158.243','2380.17','137.714','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','30','194.891','2405.99','126.108','0','0','0','0','100','0');
