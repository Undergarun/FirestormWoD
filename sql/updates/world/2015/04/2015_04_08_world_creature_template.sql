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






/*
DEV LINE : 

	SCRIPT PILE OF ASH
	SCRIPT DEPLETED PHOENIX
	SCRIPT ENERGIZED PHOENIX

SELECT * FROM waypoint_data WHERE id = 83746;
*/