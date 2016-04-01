DELETE FROM spell_script_names WHERE spell_id IN (165431, 106951, 166639, 135286, 167714);
INSERT INTO spell_script_names VALUE
(165431, 'spell_dru_item_t17_feral_2p_bonus'),
(106951, 'spell_dru_item_t17_feral_4p_bonus'),
(166639, 'spell_dru_item_t17_feral_4p_bonus_proc_driver'),
(135286, 'spell_dru_item_t17_guardian_2p_bonus'),
(167714, 'spell_dru_item_t17_restoration_4p_bonus');

DELETE FROM spell_proc_event WHERE entry IN (165431, 166639, 167714);
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(165431, 0x40000),
(166639, 0x51010),
(167714, 0x255414);