DELETE FROM `npc_vendor` WHERE entry = 82877 AND item = 127365;
DELETE FROM `npc_vendor` WHERE entry = 93911 AND item = 127365;
INSERT INTO `npc_vendor` (entry, slot, item, maxcount, incrtime, ExtendedCost, TYPE, PlayerConditionID) VALUES 
(82877, 0, 127365, 0, 0, 5318, 1, 0),
(93911, 0, 127365, 0, 0, 5318, 1, 0);