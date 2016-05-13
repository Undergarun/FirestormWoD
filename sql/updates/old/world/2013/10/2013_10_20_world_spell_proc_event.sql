-- Selfless Healer
DELETE FROM spell_proc_event where entry = 85804;
DELETE FROM spell_proc_event where entry = 114250;
INSERT INTO `spell_proc_event` VALUES (114250, 0, 10, 0x40000000, 0, 0, 16384, 0, 0, 100, 0);