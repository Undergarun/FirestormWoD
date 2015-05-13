DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_fists_of_fury';

DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_fists_of_fury_damage';
INSERT INTO spell_script_names VALUES (117418, 'spell_monk_fists_of_fury_damage');

DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_fists_of_fury_stun';
INSERT INTO spell_script_names VALUES (120086, 'spell_monk_fists_of_fury_stun');