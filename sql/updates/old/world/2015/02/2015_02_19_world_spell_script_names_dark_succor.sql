DELETE FROM spell_script_names WHERE ScriptName = 'spell_dk_dark_succor';
INSERT spell_script_names VALUES (178819, 'spell_dk_dark_succor');
DELETE FROM spell_proc_event WHERE entry = 178819;
INSERT INTO `spell_proc_event` (`entry`, `procFlags`) VALUES ('178819', '2');