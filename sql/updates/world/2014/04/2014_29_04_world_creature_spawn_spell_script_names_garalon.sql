-- Spawn for Garalon
DELETE FROM creature WHERE id = 62164;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES ('62164','1009','6297','6297','8','1','0','0','-1926.32','475.93','470.96','3.14159','604800000','0','0','1','0','0','0','0','0','0','0','0','0');

-- Spellscripts
DELETE FROM spell_script_names WHERE spell_id IN (123100, 123120);
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES
(123120, "spell_garalon_pheromones_trail_dmg"),
(123100, "spell_garalon_pheromones_switch");
