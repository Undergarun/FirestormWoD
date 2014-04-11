DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_glyph_of_decoy';
INSERT INTO spell_script_names VALUE (1856, 'spell_rog_glyph_of_decoy');
UPDATE creature_template SET unit_class = 4, ScriptName = 'npc_rogue_decoy', flags_extra = 0 WHERE entry = 62261;