DELETE FROM creature WHERE id IN (60999, 62442);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES('60999','996','6067','6515','8','65535','0','0','-1017.58','-2771.66','38.5722','4.73273','300','0','0','0','0','0','0','0','0','0','0');
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES('62442','996','6067','6515','8','65535','0','0','-1017.95','-2807.68','38.2767','1.53812','300','0','0','0','0','0','0','0','0','0','0');

DELETE FROM `spell_script_names` WHERE spell_id IN(122768, 125843, 122789);
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (125843, 'spell_dread_shadows_damage');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122768, 'spell_dread_shadows_malus');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122789, 'spell_sunbeam');

UPDATE creature_template SET scriptname = "npc_sunbeam" WHERE entry = 62849;