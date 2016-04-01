DELETE FROM spell_script_names WHERE ScriptName IN ('spell_warl_glyph_of_demon_hunting', 'spell_warl_dark_apotheosis');
INSERT INTO spell_script_names VALUES
(63303, 'spell_warl_glyph_of_demon_hunting'),
(114168, 'spell_warl_dark_apotheosis');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_warl_fury_ward';
INSERT INTO spell_script_names VALUE (119839, 'spell_warl_fury_ward');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_warl_demonic_slash';
INSERT INTO spell_script_names VALUE (114175, 'spell_warl_demonic_slash');

DELETE FROM spell_bonus_data WHERE entry = 114175;
INSERT INTO spell_bonus_data VALUE (114175, 0.666, 0, 0, 0, 'Warlock - Demonic Slash');