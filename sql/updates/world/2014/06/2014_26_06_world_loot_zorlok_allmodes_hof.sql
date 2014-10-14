-- Imperial Vizier Zor'lok <Voice of the Empress>

-- Normal Mode
SET @REF_ZORLOK_NORMAL = 62980;
DELETE FROM reference_loot_template WHERE entry = @REF_ZORLOK_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ZORLOK_NORMAL, 86156, 0, 1, 1, 1, 1), /* Fragment of Fear Made Flesh */
(@REF_ZORLOK_NORMAL, 89827, 0, 1, 1, 1, 1), /* Pheromone-Coated Choker */
(@REF_ZORLOK_NORMAL, 86157, 0, 1, 1, 1, 1), /* Attenuating Bracers */
(@REF_ZORLOK_NORMAL, 89829, 0, 1, 1, 1, 1), /* Scent-Soaked Sandals */
(@REF_ZORLOK_NORMAL, 86154, 0, 1, 1, 1, 1), /* Hisek's Chrysanthemum Cape */
(@REF_ZORLOK_NORMAL, 86153, 0, 1, 1, 1, 1), /* Boots of the Still Breath */
(@REF_ZORLOK_NORMAL, 89826, 0, 1, 1, 1, 1), /* Warbelt of Sealed Pods */
(@REF_ZORLOK_NORMAL, 87824, 0, 1, 1, 1, 1), /* Zor'lok's Fizzing Chestguard */
(@REF_ZORLOK_NORMAL, 86158, 0, 1, 1, 1, 1), /* Chestplate of the Forbidden Tower */
(@REF_ZORLOK_NORMAL, 86159, 0, 1, 1, 1, 1), /* Gauntlets of the Undesired Gifts */
(@REF_ZORLOK_NORMAL, 86161, 0, 1, 1, 1, 1), /* Gloves of Grasping Claws */
(@REF_ZORLOK_NORMAL, 86160, 0, 1, 1, 1, 1), /* Mail of Screaming Secrets */
(@REF_ZORLOK_NORMAL, 86203, 0, 1, 1, 1, 1), /* Articulated Legplates */
(@REF_ZORLOK_NORMAL, 86155, 0, 1, 1, 1, 1); /* Vizier's Ruby Signet */

-- Mode 10 normal
SET @10NM_ZORLOK = 62980;
UPDATE creature_template SET lootid = @10NM_ZORLOK WHERE entry = @10NM_ZORLOK;
DELETE FROM creature_loot_template WHERE entry = @10NM_ZORLOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_ZORLOK, 1, 100, 1, 0, -@REF_ZORLOK_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_ZORLOK = 63010;
UPDATE creature_template SET lootid = @25NM_ZORLOK WHERE entry = @25NM_ZORLOK;
DELETE FROM creature_loot_template WHERE entry = @25NM_ZORLOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_ZORLOK, 1, 100, 1, 0, -@REF_ZORLOK_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_ZORLOK_HEROIC = 63011;
DELETE FROM reference_loot_template WHERE entry = @REF_ZORLOK_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ZORLOK_HEROIC, 89917, 0, 1, 1, 1, 1), /* Pheromone-Coated Choker */
(@REF_ZORLOK_HEROIC, 86947, 0, 1, 1, 1, 1), /* Gloves of Grasping Claws */
(@REF_ZORLOK_HEROIC, 89918, 0, 1, 1, 1, 1), /* Scent-Soaked Sandals */
(@REF_ZORLOK_HEROIC, 86950, 0, 1, 1, 1, 1), /* Gauntlet of Undesired Gifts */
(@REF_ZORLOK_HEROIC, 87822, 0, 1, 1, 1, 1), /* Zor'lok's Fizzing Chestguard */
(@REF_ZORLOK_HEROIC, 86951, 0, 1, 1, 1, 1), /* Mail of Screaming Secrets */
(@REF_ZORLOK_HEROIC, 86944, 0, 1, 1, 1, 1), /* Articulated Legplates */
(@REF_ZORLOK_HEROIC, 86952, 0, 1, 1, 1, 1), /* Chestplate of the Forbidden Tower */
(@REF_ZORLOK_HEROIC, 89919, 0, 1, 1, 1, 1); /* Warbelt of Sealed Pods */

-- Mode 10 Heroic
SET @10HM_ZORLOK = 63011;
UPDATE creature_template SET lootid = @10HM_ZORLOK WHERE entry = @10HM_ZORLOK;
DELETE FROM creature_loot_template WHERE entry = @10HM_ZORLOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_ZORLOK, 1, 100, 1, 0, -@REF_ZORLOK_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_ZORLOK = 63012;
UPDATE creature_template SET lootid = @25HM_ZORLOK WHERE entry = @25HM_ZORLOK;
DELETE FROM creature_loot_template WHERE entry = @25HM_ZORLOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_ZORLOK, 1, 100, 1, 0, -@REF_ZORLOK_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_ZORLOK, @25NM_ZORLOK);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_ZORLOK, 396, 4000),
(@25NM_ZORLOK, 396, 4000),
(@10HM_ZORLOK, 396, 4000),
(@25HM_ZORLOK, 396, 4000);
