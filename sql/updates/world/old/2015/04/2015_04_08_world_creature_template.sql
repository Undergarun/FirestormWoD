#83746/NPC - Rukhmar

	DELETE FROM creature WHERE id = 83746;
	INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`) VALUE ('83746','1116','6722','6722','1','1','0','0','131.766','2506.42','135.821','2.64288','300','0','0','43331200','0','2','0','0','0','2176','0','0','0','0',NULL);
	UPDATE `creature_template` SET `InhabitType` = 7, movementtype = 2, speed_walk = 2, speed_run = 2, speed_fly = 2, unit_flags = 0, scriptname = "boss_rukhmar" WHERE `entry` = 83746;

#83769/NPC - Energized Phoenix

	UPDATE creature_template SET inhabittype = 7, faction = 14, scriptname = "npc_energized_phoenix" WHERE entry = 83769;

#167629/167647/Spell - Blaze Of Glory/Loose Quills

	DELETE FROM spell_script_names WHERE spell_id IN (167629, 167647);
	INSERT INTO spell_script_names VALUE (167629, "spell_rukhmar_blaze_of_glory");
	INSERT INTO spell_script_names VALUE (167647, "spell_rukhmar_loose_quills");

insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','23','131.522','2432.68','169.563','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','22','78.886','2277.61','256.955','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','21','27.9047','2108.97','204.323','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','20','-23.4536','2064.19','187.723','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','19','-148.058','2066.57','184.298','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','18','-248.557','2115.8','163.153','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','17','-328.726','2153.99','151.22','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','16','-431.643','2148.71','151.13','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','15','-491.414','2052.99','152.572','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','14','-500.702','1932.53','159.579','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','13','-420.802','1855.36','155.66','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','12','-313.779','1852.22','151.363','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','11','-228.946','1901.49','155.93','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','10','-234.986','2008.37','161.793','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','9','-249.771','2115.97','163.267','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','8','-265.634','2227.69','155.873','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','7','-271.158','2333.12','139.949','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','6','-247.81','2439.22','110.047','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','5','-193.294','2483.9','91.3236','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','4','-131.968','2531.62','133.638','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','3','-9.79054','2587.94','134.577','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','2','47.6894','2560.34','135.523','0','0','0','0','100','0');
insert into `waypoint_data` (`id`, `point`, `position_x`, `position_y`, `position_z`, `orientation`, `delay`, `move_flag`, `action`, `action_chance`, `wpguid`) values('83746','1','105.782','2520.86','133.07','0','0','0','0','100','0');
