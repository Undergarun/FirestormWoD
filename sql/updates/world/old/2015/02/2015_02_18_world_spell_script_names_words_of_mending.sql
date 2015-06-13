DELETE FROM spell_proc_event WHERE entry = 152117;
INSERT INTO `spell_proc_event` (`entry`, `procFlags`) VALUES ('152117', '16384');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_pri_words_of_mending';
INSERT INTO spell_script_names VALUES (152117, 'spell_pri_words_of_mending');