-- Arterial Bleeding
DELETE FROM spell_script_names WHERE spell_id = 123422;
DELETE FROM spell_proc_event WHERE entry = 123421;
INSERT INTO spell_proc_event VALUE(123421, 0, 0, 0, 0, 0, (0x4|0x10|0x40|0x100), 0, 0, 0, 0);