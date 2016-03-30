DELETE FROM spell_script_names WHERE ScriptName = 'spell_dru_shooting_stars';
INSERT INTO spell_script_names VALUE (93400, 'spell_dru_shooting_stars');
DELETE FROM spell_script_names WHERE ScriptName = 'spell_dru_shooting_stars_proc';
INSERT INTO spell_script_names VALUES
(8921, 'spell_dru_shooting_stars_proc'),
(93402, 'spell_dru_shooting_stars_proc'),
(164812, 'spell_dru_shooting_stars_proc'),
(164815, 'spell_dru_shooting_stars_proc');
DELETE FROM spell_proc_event WHERE entry = 93399;
INSERT INTO spell_proc_event (entry, procFlags) VALUE (93399, 0x240000);