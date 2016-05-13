DELETE FROM spell_script_names WHERE spell_id IN (165516, 165482, 51713);
INSERT INTO spell_script_names VALUE
(165516, 'spell_rog_item_t17_assassination_2p_bonus'),
(165482, 'spell_rog_item_t17_subtlety_2p_bonus'),
(51713, 'spell_rog_item_t17_subtlety_4p_bonus');

DELETE FROM spell_proc_event WHERE entry IN (165516, 165482);
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(165516, 0x10),
(165482, 0x4400);