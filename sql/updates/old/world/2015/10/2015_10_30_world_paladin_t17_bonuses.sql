DELETE FROM spell_script_names WHERE spell_id IN (31935, 167740);
INSERT INTO spell_script_names VALUES
(31935, 'spell_pal_avengers_shield'),
(167740, 'spell_pal_t17_protection_4p');

DELETE FROM spell_proc_event WHERE entry IN (167740);
INSERT INTO spell_proc_event (entry, procFlags, procEx) VALUES
(167740, 0x22A8, 0x40);