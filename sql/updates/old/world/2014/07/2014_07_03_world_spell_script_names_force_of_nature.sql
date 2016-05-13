INSERT INTO spell_script_names VALUE (142423, 'spell_special_swiftmend');
DELETE FROM spell_bonus_data WHERE entry IN (142421, 142424);
INSERT INTO spell_bonus_data VALUES
(142421, 1.29, 0, 0, 0, 'Treant - Swiftmend'),
(142424, 1.29, 0, 0, 0, 'Treant - Efflorescence');