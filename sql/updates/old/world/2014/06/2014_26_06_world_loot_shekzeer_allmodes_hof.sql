-- Amber-Shaper Un'sok

-- Normal Mode
SET @REF_SHEKZEER_NORMAL = 62837;
DELETE FROM reference_loot_template WHERE entry = @REF_SHEKZEER_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SHEKZEER_NORMAL, 89238, 0, 1, 1, 1, 1), /* Chest of the Shadowy Protector */
(@REF_SHEKZEER_NORMAL, 89239, 0, 1, 1, 1, 1), /* Chest of the Shadowy Vanquisher */
(@REF_SHEKZEER_NORMAL, 89237, 0, 1, 1, 1, 1), /* Chest of the Shadowy Conqueror */
(@REF_SHEKZEER_NORMAL, 86227, 0, 1, 1, 1, 1), /* Kri'tak, Imperial Scepter of the Swarm */
(@REF_SHEKZEER_NORMAL, 89836, 0, 1, 1, 1, 1), /* Shadow Heart Spaulders */
(@REF_SHEKZEER_NORMAL, 86228, 0, 1, 1, 1, 1), /* Crown of the Doomed Empress */
(@REF_SHEKZEER_NORMAL, 86226, 0, 1, 1, 1, 1), /* Claws of Shek'zeer */
(@REF_SHEKZEER_NORMAL, 86229, 0, 1, 1, 1, 1), /* Leggings of the Shadow Infestation */
(@REF_SHEKZEER_NORMAL, 89835, 0, 1, 1, 1, 1), /* Hood of Dark Dreams */
(@REF_SHEKZEER_NORMAL, 89837, 0, 1, 1, 1, 1); /* Legplates of Regal Reinforcement */

-- Mode 10 normal
SET @10NM_SHEKZEER = 62837;
UPDATE creature_template SET lootid = @10NM_SHEKZEER WHERE entry = @10NM_SHEKZEER;
DELETE FROM creature_loot_template WHERE entry = @10NM_SHEKZEER;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_SHEKZEER, 1, 100, 1, 0, -@REF_SHEKZEER_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_SHEKZEER = 80837;
UPDATE creature_template SET lootid = @25NM_SHEKZEER WHERE entry = @25NM_SHEKZEER;
DELETE FROM creature_loot_template WHERE entry = @25NM_SHEKZEER;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_SHEKZEER, 1, 100, 1, 0, -@REF_SHEKZEER_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_SHEKZEER_HEROIC = 81837;
DELETE FROM reference_loot_template WHERE entry = @REF_SHEKZEER_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SHEKZEER_HEROIC, 89251, 0, 1, 1, 1, 1), /* Chest of the Shadowy Protector */
(@REF_SHEKZEER_HEROIC, 89249, 0, 1, 1, 1, 1), /* Chest of the Shadowy Vanquisher */
(@REF_SHEKZEER_HEROIC, 89250, 0, 1, 1, 1, 1), /* Chest of the Shadowy Conqueror */
(@REF_SHEKZEER_HEROIC, 86990, 0, 1, 1, 1, 1), /* Kri'tak, Imperial Scepter of the Swarm */
(@REF_SHEKZEER_HEROIC, 86991, 0, 1, 1, 1, 1), /* Crown of the Doomed Empress */
(@REF_SHEKZEER_HEROIC, 86988, 0, 1, 1, 1, 1), /* Claws of Shek'zeer */
(@REF_SHEKZEER_HEROIC, 86989, 0, 1, 1, 1, 1), /* Leggings of Shadow Infestation */
(@REF_SHEKZEER_HEROIC, 89928, 0, 1, 1, 1, 1), /* Legplates of Regal Reinforcement */
(@REF_SHEKZEER_HEROIC, 89927, 0, 1, 1, 1, 1), /* Hood of Dark Dreams */
(@REF_SHEKZEER_HEROIC, 89926, 0, 1, 1, 1, 1); /* Shadow Heart Spaulders */

-- Mode 10 Heroic
SET @10HM_SHEKZEER = 81837;
UPDATE creature_template SET lootid = @10HM_SHEKZEER WHERE entry = @10HM_SHEKZEER;
DELETE FROM creature_loot_template WHERE entry = @10HM_SHEKZEER;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_SHEKZEER, 1, 100, 1, 0, -@REF_SHEKZEER_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_SHEKZEER = 82837;
UPDATE creature_template SET lootid = @25HM_SHEKZEER WHERE entry = @25HM_SHEKZEER;
DELETE FROM creature_loot_template WHERE entry = @25HM_SHEKZEER;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_SHEKZEER, 1, 100, 1, 0, -@REF_SHEKZEER_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_SHEKZEER, @25NM_SHEKZEER);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_SHEKZEER, 396, 4000),
(@25NM_SHEKZEER, 396, 4000),
(@10HM_SHEKZEER, 396, 4000),
(@25HM_SHEKZEER, 396, 4000);
