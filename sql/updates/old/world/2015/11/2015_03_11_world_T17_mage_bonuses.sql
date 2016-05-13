DELETE FROM spell_script_names WHERE spell_id IN (165459, 165476);
INSERT INTO spell_script_names VALUE
(165459, 'spell_mage_item_t17_fire_4p_bonus'),
(165476, 'spell_mage_item_t17_arcane_4p_bonus');

DELETE FROM spell_proc_event WHERE entry IN (165459, 165476);
INSERT INTO spell_proc_event (entry, procFlags) VALUE
(165459, 0x4400),
(165476, 0x4000);