-- Quest 37280 - Bonus Objective: Snarlpaw Ledge
UPDATE creature_template SET faction=7, `npcflag`=0 WHERE entry IN (88811, 88812, 88813);
UPDATE creature SET `npcflag`=0 WHERE id IN (88811, 88812, 88813);