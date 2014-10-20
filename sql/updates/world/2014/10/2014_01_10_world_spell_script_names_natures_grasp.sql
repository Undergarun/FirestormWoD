DELETE FROM spell_script_names WHERE spell_id = 16689 AND ScriptName = 'spell_dru_natures_grasp';
INSERT INTO spell_script_names VALUE (16689, 'spell_dru_natures_grasp');

DELETE FROM spell_proc_event WHERE entry = 16689;
INSERT INTO spell_proc_event (entry, procFlags, CustomChance) VALUE (16689, 0x28, 100);