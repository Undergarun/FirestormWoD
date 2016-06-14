-- Quest 37280 - Bonus Objective: Snarlpaw Ledge
UPDATE creature_template SET faction=7, `npcflag`=1, ScriptName="npc_quest_frightened_spirit" WHERE entry IN (88811, 88812, 88813);
UPDATE creature SET `npcflag`=1 WHERE id IN (88811, 88812, 88813);