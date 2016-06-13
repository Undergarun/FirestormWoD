-- Quest 34728 Bonus Objective: The Shimmer Moor
DELETE FROM `gameobject_loot_template` WHERE entry IN(230610, 230613);
INSERT INTO `gameobject_loot_template` (
  `entry`,  `item`,  `ChanceOrQuestChance`,  `lootmode`,  `groupid`,  `mincountOrRef`,  `maxcount`,  `itemBonuses`) 
VALUES
  (
    '230610',    '111559',    '-100',    '1',    '0',    '1',    '1',    ''
  ),
  
  (
    '230613',    '111559',    '-100',    '1',    '0',    '1',    '1',    ''
  ) ;
UPDATE gameobject_template SET data1=230613 WHERE entry=230613;
UPDATE gameobject_template SET data1=230610 WHERE entry=230610;
 