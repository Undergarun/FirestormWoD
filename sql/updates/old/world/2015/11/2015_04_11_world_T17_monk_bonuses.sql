DELETE FROM spell_script_names WHERE spell_id IN (165353, 113656, 118674, 167718);
INSERT INTO spell_script_names VALUE
(165353, 'spell_monk_item_t17_brewmaster_2p_bonus'),
(113656, 'spell_monk_fists_of_fury'),
(118674, 'spell_monk_item_t17_mistweaver_2p_bonus'),
(167718, 'spell_monk_item_t17_mistweaver_4p_bonus');

DELETE FROM spell_proc_event WHERE entry IN (165353, 167718);
INSERT INTO spell_proc_event (entry, procFlags, procEx) VALUE
(165353, 0x222A8, 0x10),
(167718, 0x4400, 0x10000000);