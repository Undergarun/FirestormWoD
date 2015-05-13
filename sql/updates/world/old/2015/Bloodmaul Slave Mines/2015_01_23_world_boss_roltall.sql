DELETE FROM creature WHERE id IN (75853, 75854, 75828);
DELETE FROM gameobject WHERE id = 224487 AND state = 1;

UPDATE creature SET spawnMask = 262, spawntimesecs = 7200 WHERE map = 1175;
UPDATE gameobject SET spawnMask = 262 WHERE map = 1175;

UPDATE creature_template SET ScriptName = 'boss_roltall', mechanic_immune_mask = 617299839, flags_extra = flags_extra | 0x01 WHERE entry = 75786;
UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, faction = 14, ScriptName = 'npc_fiery_boulder' WHERE entry IN (75853, 75854, 75828);

DELETE FROM spell_script_names WHERE ScriptName IN ('spell_roltall_scorching_aura');
INSERT INTO spell_script_names VALUE
(167738, 'spell_roltall_scorching_aura');

DELETE FROM areatrigger_template WHERE spell_id IN (152742, 152835, 152867, 152913);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(152913, 0, 6022, 1, 1, 16384, 'areatrigger_roltall_burning_slag'),
(152867, 0, 7214, 1, 1, 4096, 'areatrigger_roltall_heat_wave');