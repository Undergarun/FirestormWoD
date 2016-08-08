DELETE FROM spell_upgrade_item_stage WHERE ItemBonusTreeCategory = 201;
DELETE FROM spell_upgrade_item_stage WHERE ItemBonusTreeCategory = 202;
INSERT INTO spell_upgrade_item_stage VALUES
(201, 4, 4, 1018, 685),
(201, 4, 8, 1018, 685),
(201, 4, 0, 65536, 685), 
(202, 4, 2, 1018, 685),
(202, 4, 0, 65536, 685);