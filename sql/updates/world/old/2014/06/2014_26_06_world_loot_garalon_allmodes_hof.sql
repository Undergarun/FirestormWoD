-- Garalon

-- Normal Mode
SET @REF_GARALON_NORMAL = 62164;
DELETE FROM reference_loot_template WHERE entry = @REF_GARALON_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GARALON_NORMAL, 86177, 0, 1, 1, 1, 1), /* Necklace of Congealed Weaknesses */
(@REF_GARALON_NORMAL, 86173, 0, 1, 1, 1, 1), /* Legbreaker Greatcloak */
(@REF_GARALON_NORMAL, 86182, 0, 1, 1, 1, 1), /* Stormwake Mistcloak */
(@REF_GARALON_NORMAL, 86178, 0, 1, 1, 1, 1), /* Sandals of the Unbidden */
(@REF_GARALON_NORMAL, 89833, 0, 1, 1, 1, 1), /* Shoulders of Foaming Fluids */
(@REF_GARALON_NORMAL, 86181, 0, 1, 1, 1, 1), /* Xaril's Hood of Intoxicating Vapors */
(@REF_GARALON_NORMAL, 86176, 0, 1, 1, 1, 1), /* Bonebreaker Gauntlets */
(@REF_GARALON_NORMAL, 86180, 0, 1, 1, 1, 1), /* Robes of Eighty Lights */
(@REF_GARALON_NORMAL, 86175, 0, 1, 1, 1, 1), /* Grips of the Leviathan */
(@REF_GARALON_NORMAL, 89834, 0, 1, 1, 1, 1), /* Vestments of Steaming  Ichor */
(@REF_GARALON_NORMAL, 89832, 0, 1, 1, 1, 1), /* Garalon's Graven Carapace */
(@REF_GARALON_NORMAL, 86174, 0, 1, 1, 1, 1), /* Garalon's Hollow Skull */
(@REF_GARALON_NORMAL, 86179, 0, 1, 1, 1, 1), /* Grasps of Panic */
(@REF_GARALON_NORMAL, 86172, 0, 1, 1, 1, 1); /* Ring of the Shattered Shell */

-- Mode 10 normal
SET @10NM_GARALON = 62164;
UPDATE creature_template SET lootid = @10NM_GARALON WHERE entry = @10NM_GARALON;
DELETE FROM creature_loot_template WHERE entry = @10NM_GARALON;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_GARALON, 1, 100, 1, 0, -@REF_GARALON_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_GARALON = 80164;
UPDATE creature_template SET lootid = @25NM_GARALON WHERE entry = @25NM_GARALON;
DELETE FROM creature_loot_template WHERE entry = @25NM_GARALON;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_GARALON, 1, 100, 1, 0, -@REF_GARALON_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_GARALON_HEROIC = 81164;
DELETE FROM reference_loot_template WHERE entry = @REF_GARALON_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GARALON_HEROIC, 89924, 0, 1, 1, 1, 1), /* Shoulders of Foaming Fluids */
(@REF_GARALON_HEROIC, 86970, 0, 1, 1, 1, 1), /* Xaril's Hood of Intoxicating Vapors */
(@REF_GARALON_HEROIC, 86964, 0, 1, 1, 1, 1), /* Bonebreaker Gauntlets */
(@REF_GARALON_HEROIC, 86972, 0, 1, 1, 1, 1), /* Robes of Eighty Lights */
(@REF_GARALON_HEROIC, 86965, 0, 1, 1, 1, 1), /* Grips of the Leviathan */
(@REF_GARALON_HEROIC, 89925, 0, 1, 1, 1, 1), /* Vestments of Steaming  Ichor */
(@REF_GARALON_HEROIC, 89923, 0, 1, 1, 1, 1), /* Garalon's Graven Carapace */
(@REF_GARALON_HEROIC, 86966, 0, 1, 1, 1, 1), /* Garalon's Hollow Skull */
(@REF_GARALON_HEROIC, 86973, 0, 1, 1, 1, 1), /* Grasps of Panic */
(@REF_GARALON_HEROIC, 86967, 0, 1, 1, 1, 1), /* Necklace of Congealed Weaknesses */
(@REF_GARALON_HEROIC, 86963, 0, 1, 1, 1, 1), /* Legbreaker Greatcloak */
(@REF_GARALON_HEROIC, 86971, 0, 1, 1, 1, 1), /* Stormwake Mistcloak */
(@REF_GARALON_HEROIC, 86969, 0, 1, 1, 1, 1), /* Sandals of the Unbidden */
(@REF_GARALON_HEROIC, 86968, 0, 1, 1, 1, 1); /* Ring of the Shattered Shell */

-- Mode 10 Heroic
SET @10HM_GARALON = 81164;
UPDATE creature_template SET lootid = @10HM_GARALON WHERE entry = @10HM_GARALON;
DELETE FROM creature_loot_template WHERE entry = @10HM_GARALON;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_GARALON, 1, 100, 1, 0, -@REF_GARALON_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_GARALON = 82164;
UPDATE creature_template SET lootid = @25HM_GARALON WHERE entry = @25HM_GARALON;
DELETE FROM creature_loot_template WHERE entry = @25HM_GARALON;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_GARALON, 1, 100, 1, 0, -@REF_GARALON_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_GARALON, @25NM_GARALON);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_GARALON, 396, 4000),
(@25NM_GARALON, 396, 4000),
(@10HM_GARALON, 396, 4000),
(@25HM_GARALON, 396, 4000);
