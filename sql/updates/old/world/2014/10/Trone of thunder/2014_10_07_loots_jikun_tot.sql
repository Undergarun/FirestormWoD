-- Ji-Kun Loots

-- Normal modes
SET @REF_JI_KUN_NORMAL = 69712;
DELETE FROM reference_loot_template WHERE entry = @REF_JI_KUN;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_JI_KUN, 95572, 0, 1, 1, 1, 1), -- Leggings of the Crackling Vanquisher
(@REF_JI_KUN, 95581, 0, 1, 1, 1, 1), -- Leggings of the Crackling Protector
(@REF_JI_KUN, 95576, 0, 1, 1, 1, 1), -- Leggings of the Cracking Conqueror
(@REF_JI_KUN, 94805, 0, 1, 1, 1, 1), -- Giorgio's Caduceus of Pure Moods
(@REF_JI_KUN, 94812, 0, 1, 1, 1, 1), -- Pinionfeather Greatcloak
(@REF_JI_KUN, 94813, 0, 1, 1, 1, 1), -- Cord off Cacophonous Cawing
(@REF_JI_KUN, 94527, 0, 1, 1, 1, 1), -- Ji-Kun's Rising Winds
(@REF_JI_KUN, 94806, 0, 1, 1, 1, 1), -- Robe of Midnight Down
(@REF_JI_KUN, 94810, 0, 1, 1, 1, 1), -- Talonrender Chestplate
(@REF_JI_KUN, 94811, 0, 1, 1, 1, 1), -- Featherflight Belt
(@REF_JI_KUN, 94809, 0, 1, 1, 1, 1), -- Crown of Potentiated Birth
(@REF_JI_KUN, 94807, 0, 1, 1, 1, 1), -- Egg-Shard Grips
(@REF_JI_KUN, 94808, 0, 1, 1, 1, 1), -- Grasp of the Ruthless Mother
(@REF_JI_KUN, 95059, 0, 1, 1, 1, 1), -- Clutch of Ji-Kun
(@REF_JI_KUN, 94835, 0, 1, 1, 1, 1); -- Ji-Kun Hatchling

-- Mode 10 normal
SET @10NM_JIKUN = 69712;
UPDATE `creature_template` SET `lootid`= @10NM_JIKUN WHERE `entry`= @10NM_JIKUN;
DELETE FROM `creature_loot_template` WHERE `entry`= @10NM_JIKUN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10NM_JIKUN, 1, 100, 1, 0, -@REF_JI_KUN_NORMAL, 2);

-- Mode 25 normal
SET @25NM_JIKUN = 469712;
UPDATE creature_template SET lootid = @25NM_JIKUN WHERE entry = @25NM_JIKUN;
DELETE FROM creature_loot_template WHERE entry = @25NM_JIKUN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_JIKUN, 1, 100, 1, 0, -@REF_JI_KUN_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_JIKUN_HEROIC = 569712;
DELETE FROM reference_loot_template WHERE entry = @REF_JIKUN_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_JIKUN_HEROIC, 96631, 0, 1, 1, 1, 1), -- Leggings of the Crackling Vanquisher
(@REF_JIKUN_HEROIC, 96633, 0, 1, 1, 1, 1), -- Leggings of the Crackling Protector
(@REF_JIKUN_HEROIC, 96632, 0, 1, 1, 1, 1), -- Leggings of the Crackling Conqueror
(@REF_JIKUN_HEROIC, 96461, 0, 1, 1, 1, 1), -- Pinionfeather Greatcloak
(@REF_JIKUN_HEROIC, 96468, 0, 1, 1, 1, 1), -- Talonrender Chestplate
(@REF_JIKUN_HEROIC, 96471, 0, 1, 1, 1, 1), -- Ji-Kun's Rising Winds
(@REF_JIKUN_HEROIC, 96470, 0, 1, 1, 1, 1), -- Fabled Feather of Ji-Kun
(@REF_JIKUN_HEROIC, 96464, 0, 1, 1, 1, 1), -- Giogio's Caduceus of Pure Moods
(@REF_JIKUN_HEROIC, 96469, 0, 1, 1, 1, 1), -- Egg-Shard Grips
(@REF_JIKUN_HEROIC, 96463, 0, 1, 1, 1, 1), -- Robe of Midnight Down
(@REF_JIKUN_HEROIC, 96466, 0, 1, 1, 1, 1), -- Grasp of the Ruthless Mother
(@REF_JIKUN_HEROIC, 96467, 0, 1, 1, 1, 1), -- Crown of the Potentiated Birth
(@REF_JIKUN_HEROIC, 95059, 0, 1, 1, 1, 1), -- Clutch of Ji-Kun
(@REF_JIKUN_HEROIC, 94835, 0, 1, 1, 1, 1); -- Ji-Kun Hatchling

-- Mode 10 Heroic
SET @10HM_JIKUN = 569712;
UPDATE creature_template SET lootid = @10HM_JIKUN WHERE entry = @10HM_JIKUN;
DELETE FROM creature_loot_template WHERE entry = @10HM_JIKUN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_JIKUN, 1, 100, 1, 0, -@REF_JIKUN_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_JIKUN = 669712;
UPDATE creature_template SET lootid = @25HM_JIKUN WHERE entry = @25HM_JIKUN;
DELETE FROM creature_loot_template WHERE entry = @25HM_JIKUN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_JIKUN, 1, 100, 1, 0, -@REF_JIKUN_HEROIC, 5); /* REF 2 */

-- LFR Mode
SET @LFR_JIKUN = 769712;
DELETE FROM reference_loot_template WHERE entry = @LFR_JIKUN;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@LFR_JIKUN, 95718, 0, 1, 1, 1, 1), -- Cord of Cacophonous Cawing
(@LFR_JIKUN, 95723, 0, 1, 1, 1, 1), -- Crown of Potentiated Birth
(@LFR_JIKUN, 95725, 0, 1, 1, 1, 1), -- Egg-Shard Grips
(@LFR_JIKUN, 95726, 0, 1, 1, 1, 1), -- Fabled Feather of Ji-Kun
(@LFR_JIKUN, 95721, 0, 1, 1, 1, 1), -- Featherflight Belt
(@LFR_JIKUN, 95720, 0, 1, 1, 1, 1), -- Giorgio's Caduceus of Pure Moods
(@LFR_JIKUN, 95722, 0, 1, 1, 1, 1), -- Grassp of the Ruthless Mother
(@LFR_JIKUN, 95727, 0, 1, 1, 1, 1), -- Ji-Kun's Rising Winds
(@LFR_JIKUN, 95888, 0, 1, 1, 1, 1), -- Leggings of the Crackling Conqueror
(@LFR_JIKUN, 95889, 0, 1, 1, 1, 1), -- Leggings of the Crackling Protector
(@LFR_JIKUN, 95887, 0, 1, 1, 1, 1), -- Leggings of the Crackling Vanquisher
(@LFR_JIKUN, 95717, 0, 1, 1, 1, 1), -- Pinionfeather Greatcloak
(@LFR_JIKUN, 95719, 0, 1, 1, 1, 1), -- Robe of the Midnight Down
(@LFR_JIKUN, 95724, 0, 1, 1, 1, 1); -- Talonrender Chestplate

-- Mode 10 Heroic
UPDATE creature_template SET lootid = @LFR_JIKUN WHERE entry = @LFR_JIKUN;
DELETE FROM creature_loot_template WHERE entry = @LFR_JIKUN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@LFR_JIKUN, 1, 100, 1, 0, -@LFR_JIKUN, 5); /* REF 3 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_JIKUN, @25NM_UNSOK);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_JIKUN, 396, 4000),
(@25NM_JIKUN, 396, 4000),
(@10HM_JIKUN, 396, 4000),
(@25HM_JIKUN, 396, 4000),
(@LFR_JIKUN,  396, 4000);
