-- Blade Lord Ta'yak

-- Normal Mode
SET @REF_TAYAK_NORMAL = 62543;
DELETE FROM reference_loot_template WHERE entry = @REF_TAYAK_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TAYAK_NORMAL, 86166, 0, 1, 1, 1, 1), /* Choker of the Unleashed Storm */
(@REF_TAYAK_NORMAL, 86169, 0, 1, 1, 1, 1), /* Drape of Gathering Clouds */
(@REF_TAYAK_NORMAL, 86167, 0, 1, 1, 1, 1), /* Boots of Blowing Wind */
(@REF_TAYAK_NORMAL, 86170, 0, 1, 1, 1, 1), /* Twisting Wind Bracers */
(@REF_TAYAK_NORMAL, 86163, 0, 1, 1, 1, 1), /* Bracers of Unseen Strike */
(@REF_TAYAK_NORMAL, 89831, 0, 1, 1, 1, 1), /* Hood of Stilled Winds */
(@REF_TAYAK_NORMAL, 90738, 0, 1, 1, 1, 1), /* Kaz'tik's Stormseizer Gauntlets */
(@REF_TAYAK_NORMAL, 86168, 0, 1, 1, 1, 1), /* Bracers of Tempestuous Fury */
(@REF_TAYAK_NORMAL, 89830, 0, 1, 1, 1, 1), /* Sword Dancer's Leggings */
(@REF_TAYAK_NORMAL, 86171, 0, 1, 1, 1, 1), /* Tornado-Summoning Censer */
(@REF_TAYAK_NORMAL, 86164, 0, 1, 1, 1, 1), /* Waistplate of Overwhelming Assault */
(@REF_TAYAK_NORMAL, 86165, 0, 1, 1, 1, 1), /* Windblade Talons */
(@REF_TAYAK_NORMAL, 89828, 0, 1, 1, 1, 1), /* Pauldrons of the Broken Blade */
(@REF_TAYAK_NORMAL, 86162, 0, 1, 1, 1, 1); /* Ring of the Bladed Tempest */

-- Mode 10 normal
SET @10NM_TAYAK = 62543;
UPDATE creature_template SET lootid = @10NM_TAYAK WHERE entry = @10NM_TAYAK;
DELETE FROM creature_loot_template WHERE entry = @10NM_TAYAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_TAYAK, 1, 100, 1, 0, -@REF_TAYAK_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_TAYAK = 80543;
UPDATE creature_template SET lootid = @25NM_TAYAK WHERE entry = @25NM_TAYAK;
DELETE FROM creature_loot_template WHERE entry = @25NM_TAYAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_TAYAK, 1, 100, 1, 0, -@REF_TAYAK_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_TAYAK_HEROIC = 81543;
DELETE FROM reference_loot_template WHERE entry = @REF_TAYAK_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TAYAK_HEROIC, 89922, 0, 1, 1, 1, 1), /* Hood of Stilled Winds */
(@REF_TAYAK_HEROIC, 90740, 0, 1, 1, 1, 1), /* Kaz'tik's Stormseizer Gauntlets */
(@REF_TAYAK_HEROIC, 89920, 0, 1, 1, 1, 1), /* Sword Dancer's Leggings */
(@REF_TAYAK_HEROIC, 86960, 0, 1, 1, 1, 1), /* Tornado-Summoning Censer */
(@REF_TAYAK_HEROIC, 89921, 0, 1, 1, 1, 1), /* Pauldrons of the Broken Blade */
(@REF_TAYAK_HEROIC, 86956, 0, 1, 1, 1, 1); /* Windblade Talons */

-- Mode 10 Heroic
SET @10HM_TAYAK = 81543;
UPDATE creature_template SET lootid = @10HM_TAYAK WHERE entry = @10HM_TAYAK;
DELETE FROM creature_loot_template WHERE entry = @10HM_TAYAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_TAYAK, 1, 100, 1, 0, -@REF_TAYAK_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_TAYAK = 82543;
UPDATE creature_template SET lootid = @25HM_TAYAK WHERE entry = @25HM_TAYAK;
DELETE FROM creature_loot_template WHERE entry = @25HM_TAYAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_TAYAK, 1, 100, 1, 0, -@REF_TAYAK_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_TAYAK, @25NM_TAYAK);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_TAYAK, 396, 4000),
(@25NM_TAYAK, 396, 4000),
(@10HM_TAYAK, 396, 4000),
(@25HM_TAYAK, 396, 4000);
