DELETE FROM spell_script_names WHERE ScriptName LIKE "spell_whirling_blade%" OR spell_id IN (121896, 121897, 121898);
INSERT INTO spell_script_names VALUES
(121897, "spell_whirling_blade_sword"),
(121898, "spell_whirling_blade_damages");