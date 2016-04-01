-- Grab and Fling, by Amber Monstruosity
DELETE FROM spell_script_names WHERE spell_id IN (122415, 122413, 122420);
INSERT INTO spell_script_names VALUES
(122415, "spell_grab"),
(122413, "spell_fling"),
(122420, "spell_fling_thrown");