-- Intensify Ta'yak aura
DELETE FROM spell_script_names WHERE spell_id = 123470;

-- Storm unleashed
DELETE FROM spell_script_names WHERE spell_id IN (123814, 124024);
INSERT INTO spell_script_names VALUES
(123814, "spell_tayak_su_visual"),
(124014, "spell_su_dummy_visual");