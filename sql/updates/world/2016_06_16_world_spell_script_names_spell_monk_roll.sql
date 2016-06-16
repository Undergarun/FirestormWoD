DELETE FROM spell_script_names WHERE ScriptName = "spell_monk_glyph_of_freedom_roll";
DELETE FROM spell_script_names WHERE ScriptName = "spell_monk_roll" AND spell_id IN (101545, 109132, 115008);

INSERT INTO spell_script_names VALUE (101545, "spell_monk_roll");
INSERT INTO spell_script_names VALUE (109132, "spell_monk_roll");
INSERT INTO spell_script_names VALUE (115008, "spell_monk_roll");