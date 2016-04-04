DELETE FROM spell_script_names WHERE spell_id IN (50421, 77535, 167655, 170205);
INSERT INTO spell_script_names VALUE
(50421, 'spell_dk_item_t17_blood_2p_bonus'),
(77535, 'spell_dk_blood_shield'),
(167655, 'spell_dk_item_t17_frost_4p_driver'),
(170205, 'spell_dk_item_t17_frost_4p_driver_periodic');

DELETE FROM spell_proc_event WHERE entry IN (167655);
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(167655, 0x11010);