DELETE FROM spell_script_names WHERE ScriptName = 'spell_sha_molten_earth';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_sha_molten_earth_periodic';
DELETE FROM spell_script_names WHERE ScriptName = 'spell_sha_molten_earth_damage';
INSERT INTO spell_script_names VALUES
(170374, 'spell_mastery_molten_earth'),
(170377, 'spell_mastery_molten_earth_periodic'),
(170379, 'spell_mastery_molten_earth_damage');