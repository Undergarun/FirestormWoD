DELETE FROM spell_script_names WHERE ScriptName = 'spell_drov_call_of_earth';
INSERT INTO spell_script_names VALUE (175827, 'spell_drov_call_of_earth');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_drov_colossal_slam';
INSERT INTO spell_script_names VALUE (175791, 'spell_drov_colossal_slam');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_drov_acid_breath';
INSERT INTO spell_script_names VALUE (175915, 'spell_drov_acid_breath');

UPDATE creature_template SET dmg_multiplier = 10, ScriptName = 'boss_drov_the_ruiner', mechanic_immune_mask = 617299839 WHERE entry = 81252;
UPDATE creature_template SET faction = 2716, minlevel = 100, maxlevel = 100, EXP = 5, ScriptName = 'npc_drov_rumbling_goren' WHERE entry = 88106;
UPDATE creature_template SET faction = 2716, minlevel = 100, maxlevel = 100, EXP = 5, dmg_multiplier = 5, ScriptName = 'npc_drov_frenzied_rumbler' WHERE entry = 88119;

DELETE FROM creature WHERE id = 81252;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation, spawntimesecs, unit_flags, unit_flags2) VALUES
(81252, 1116, 6721, 6891, 1, 1, 7412.34, 1437.11, 81.9027, 2.78522, 1800, 32832, 1073743872);
UPDATE creature SET spawntimesecs = 1800 WHERE id = 81535;