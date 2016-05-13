DELETE FROM item_script_names WHERE Id = 123857;
INSERT INTO item_script_names VALUE
(123857, 'item_runic_pouch');

DELETE FROM item_loot_template WHERE entry = 123857;
INSERT INTO item_loot_template VALUES
(123857, 118632, 100, 1, 0, 1, 1, ''), -- Focus Augment Rune
(123857, 118630, 100, 1, 0, 1, 1, ''), -- Hyper Augment Rune
(123857, 118631, 100, 1, 0, 1, 1, ''); -- Stout Augment Rune