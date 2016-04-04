DELETE FROM spell_script_names WHERE ScriptName = 'spell_rog_anticipation';
INSERT INTO spell_script_names VALUE (114015, 'spell_rog_anticipation');
DELETE FROM spell_proc_event WHERE entry = 114015;
INSERT INTO spell_proc_event (entry, procFlags) VALUE (114015, 0x1110);