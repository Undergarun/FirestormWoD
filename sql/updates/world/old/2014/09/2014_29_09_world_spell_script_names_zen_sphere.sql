DELETE FROM spell_script_names WHERE ScriptName = 'spell_monk_zen_sphere_hot';

UPDATE spell_bonus_data SET ap_dot_bonus = 0.09 WHERE entry = 124081;
UPDATE spell_bonus_data SET ap_bonus = 0.09 WHERE entry = 124098;
UPDATE spell_bonus_data SET ap_bonus = 0.234 WHERE entry = 124101;
UPDATE spell_bonus_data SET ap_bonus = 0.368 WHERE entry = 125033;

DELETE FROM spell_proc_event WHERE entry = 124081;
INSERT INTO spell_proc_event (entry, procFlags) VALUE (124081, 0x0A22A8);