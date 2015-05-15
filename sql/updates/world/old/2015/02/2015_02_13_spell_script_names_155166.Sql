DELETE FROM spell_proc_event WHERE entry = 155166;
INSERT INTO `spell_proc_event` (entry, procFlags) VALUES (155166, 0x00050000);
DELETE FROM spell_script_names WHERE ScriptName = 'spell_dk_mark_of_sindragosa';
INSERT INTO spell_script_names VALUES (155166, 'spell_dk_mark_of_sindragosa');