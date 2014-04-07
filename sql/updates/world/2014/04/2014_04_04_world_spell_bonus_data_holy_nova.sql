DELETE FROM spell_script_names WHERE ScriptName = 'spell_pri_glyph_of_holy_nova' OR ScriptName = 'spell_pri_holy_nova' OR ScriptName = 'spell_pri_holy_nova_heal';
INSERT INTO spell_script_names VALUES
(125045, 'spell_pri_glyph_of_holy_nova'),
(132157, 'spell_pri_holy_nova'),
(23455, 'spell_pri_holy_nova_heal');

DELETE FROM spell_bonus_data WHERE entry IN (23455, 132157);
INSERT INTO spell_bonus_data VALUES
(132157, 0.143, 0, 0, 0, 'Priest - Holy Nova'),
(23455, 0.05, 0, 0, 0, 'Priest - Holy Nova (heal)');