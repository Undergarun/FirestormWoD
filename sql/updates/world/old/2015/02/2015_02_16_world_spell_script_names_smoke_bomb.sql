DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_smoke_bomb';
DELETE FROM areatrigger_template WHERE spell_id = 76577;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, flags, data0, data1, data2, data3, data4, data5, ScriptName) VALUE
(76577, 0, 5742, 8192, 8, 8, 4, 4, 1, 1, 'spell_areatrigger_smoke_bomb');