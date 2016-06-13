-- Quest 35237 - Bonus Objective: Mor'gran Logworks
DELETE FROM `gameobject_loot_template` WHERE entry = 231962;
INSERT INTO `gameobject_loot_template` (
  `entry`,  `item`,  `ChanceOrQuestChance`,  `lootmode`,  `groupid`,  `mincountOrRef`,  `maxcount`,  `itemBonuses`) 
VALUES
  (
    '231962',    '112994',    '-100',    '1',    '0',    '1',    '3',    ''
  );
UPDATE gameobject_template SET data1=231962 WHERE entry=231962;