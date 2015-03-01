-- Subterfuge script
INSERT INTO spell_script_names VALUES (115191, "spell_rog_stealth");
-- Internal Bleending
DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_internal_bleeding';
INSERT INTO spell_script_names VALUE (408, 'spell_rog_internal_bleeding');

DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_glyph_of_rapid_rolling';
INSERT INTO spell_script_names VALUE (109132, 'spell_monk_glyph_of_rapid_rolling');
INSERT INTO spell_script_names VALUE (115008, 'spell_monk_glyph_of_rapid_rolling');