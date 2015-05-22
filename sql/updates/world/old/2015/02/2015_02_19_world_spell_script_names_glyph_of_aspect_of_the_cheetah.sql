DELETE FROM spell_script_names WHERE ScriptName = 'spell_hun_glyph_of_aspect_of_the_cheetah';
INSERT INTO spell_script_names VALUE (5118, 'spell_hun_glyph_of_aspect_of_the_cheetah');
DELETE FROM spell_proc_event WHERE entry = 5118;
INSERT INTO spell_proc_event (entry, procFlags) VALUE (5118, 0x222A8);