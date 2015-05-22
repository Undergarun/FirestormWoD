DELETE FROM spell_script_names WHERE ScriptName = 'spell_mage_meteor';
INSERT INTO spell_script_names VALUE (153561, 'spell_mage_meteor');
DELETE FROM areatrigger_template WHERE spell_id = 175396;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUE
(175396, 0, 6211, 1, 1, 0, 'spell_areatrigger_meteor_burn');