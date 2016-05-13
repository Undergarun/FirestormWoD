-- Amber-Shaper Un'sok

-- Normal Mode
SET @REF_MELJARAK_NORMAL = 62397;
DELETE FROM reference_loot_template WHERE entry = @REF_MELJARAK_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MELJARAK_NORMAL, 89241, 0, 1, 1, 1, 1), /* Gauntlets of the Shadowy Protector */
(@REF_MELJARAK_NORMAL, 89242, 0, 1, 1, 1, 1), /* Gauntlets of the Shadowy Vanquisher */
(@REF_MELJARAK_NORMAL, 89240, 0, 1, 1, 1, 1), /* Gauntlets of the Shadowy Conqueror */
(@REF_MELJARAK_NORMAL, 86200, 0, 1, 1, 1, 1), /* Painful Thorned Ring */
(@REF_MELJARAK_NORMAL, 86201, 0, 1, 1, 1, 1), /* Impaling Treads */
(@REF_MELJARAK_NORMAL, 86205, 0, 1, 1, 1, 1), /* Korven's Amber-Sealed Beetle */
(@REF_MELJARAK_NORMAL, 86204, 0, 1, 1, 1, 1), /* Wingslasher Pauldrons */
(@REF_MELJARAK_NORMAL, 86514, 0, 1, 1, 1, 1), /* Clutches of Dying Hope */
(@REF_MELJARAK_NORMAL, 86513, 0, 1, 1, 1, 1), /* Robes of Torn Nightmares */
(@REF_MELJARAK_NORMAL, 86202, 0, 1, 1, 1, 1); /* Cloak of Raining Blades */

-- Mode 10 normal
SET @10NM_MELJARAK = 62397;
UPDATE creature_template SET lootid = @10NM_MELJARAK WHERE entry = @10NM_MELJARAK;
DELETE FROM creature_loot_template WHERE entry = @10NM_MELJARAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_MELJARAK, 1, 100, 1, 0, -@REF_MELJARAK_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_MELJARAK = 80397;
UPDATE creature_template SET lootid = @25NM_MELJARAK WHERE entry = @25NM_MELJARAK;
DELETE FROM creature_loot_template WHERE entry = @25NM_MELJARAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_MELJARAK, 1, 100, 1, 0, -@REF_MELJARAK_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_MELJARAK_HEROIC = 81397;
DELETE FROM reference_loot_template WHERE entry = @REF_MELJARAK_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MELJARAK_HEROIC, 89255, 0, 1, 1, 1, 1), /* Gauntlets of the Shadowy Protector */
(@REF_MELJARAK_HEROIC, 89257, 0, 1, 1, 1, 1), /* Gauntlets of the Shadowy Vanquisher */
(@REF_MELJARAK_HEROIC, 89256, 0, 1, 1, 1, 1), /* Gauntlets of the Shadowy Conqueror */
(@REF_MELJARAK_HEROIC, 86978, 0, 1, 1, 1, 1), /* Wingslasher Pauldrons */
(@REF_MELJARAK_HEROIC, 86977, 0, 1, 1, 1, 1), /* Clutches of Dying Hope */
(@REF_MELJARAK_HEROIC, 86975, 0, 1, 1, 1, 1); /* Robes of Torn Nightmares */

-- Mode 10 Heroic
SET @10HM_MELJARAK = 81397;
UPDATE creature_template SET lootid = @10HM_MELJARAK WHERE entry = @10HM_MELJARAK;
DELETE FROM creature_loot_template WHERE entry = @10HM_MELJARAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_MELJARAK, 1, 100, 1, 0, -@REF_MELJARAK_HEROIC, 2); /* REF 2 */

-- Mode 25 Heroic
SET @25HM_MELJARAK = 82397;
UPDATE creature_template SET lootid = @25HM_MELJARAK WHERE entry = @25HM_MELJARAK;
DELETE FROM creature_loot_template WHERE entry = @25HM_MELJARAK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_MELJARAK, 1, 100, 1, 0, -@REF_MELJARAK_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_MELJARAK, @25NM_MELJARAK);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_MELJARAK, 396, 4000),
(@25NM_MELJARAK, 396, 4000),
(@10HM_MELJARAK, 396, 4000),
(@25HM_MELJARAK, 396, 4000);
