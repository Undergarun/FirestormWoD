-- Amber-Shaper Un'sok

-- Normal Mode
SET @REF_UNSOK_NORMAL = 62511;
DELETE FROM reference_loot_template WHERE entry = @REF_UNSOK_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_UNSOK_NORMAL, 89244, 0, 1, 1, 1, 1), /* Leggings of the Shadowy Protector */
(@REF_UNSOK_NORMAL, 89245, 0, 1, 1, 1, 1), /* Leggings of the Shadowy Vanquisher */
(@REF_UNSOK_NORMAL, 89243, 0, 1, 1, 1, 1), /* Leggings of the Shadowy Conqueror */
(@REF_UNSOK_NORMAL, 86217, 0, 1, 1, 1, 1), /* Un'sok's Amber Scalpel */
(@REF_UNSOK_NORMAL, 86211, 0, 1, 1, 1, 1), /* Seal of the Profane */
(@REF_UNSOK_NORMAL, 86210, 0, 1, 1, 1, 1), /* Belt of Malleable Amber */
(@REF_UNSOK_NORMAL, 86214, 0, 1, 1, 1, 1), /* Monstruous Stompers */
(@REF_UNSOK_NORMAL, 86212, 0, 1, 1, 1, 1), /* Treads of Deadly Secretions */
(@REF_UNSOK_NORMAL, 86213, 0, 1, 1, 1, 1), /* Shoulderpads of Misshapen Life */
(@REF_UNSOK_NORMAL, 86219, 0, 1, 1, 1, 1); /* Scimitar of Seven Stars */

-- Mode 10 normal
SET @10NM_UNSOK = 62511;
UPDATE creature_template SET lootid = @10NM_UNSOK WHERE entry = @10NM_UNSOK;
DELETE FROM creature_loot_template WHERE entry = @10NM_UNSOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_UNSOK, 1, 100, 1, 0, -@REF_UNSOK_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_UNSOK = 80511;
UPDATE creature_template SET lootid = @25NM_UNSOK WHERE entry = @25NM_UNSOK;
DELETE FROM creature_loot_template WHERE entry = @25NM_UNSOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_UNSOK, 1, 100, 1, 0, -@REF_UNSOK_NORMAL, 5); /* REF 1 */

-- Heroic Mode
SET @REF_UNSOK_HEROIC = 81511;
DELETE FROM reference_loot_template WHERE entry = @REF_UNSOK_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_UNSOK_HEROIC, 86983, 0, 1, 1, 1, 1), /* Un'sok's Amber Scalpel */
(@REF_UNSOK_HEROIC, 86986, 0, 1, 1, 1, 1), /* Shoulderpads of Misshapen Life */
(@REF_UNSOK_HEROIC, 86987, 0, 1, 1, 1, 1), /* Scimitar of Seven Stars */
(@REF_UNSOK_HEROIC, 89252, 0, 1, 1, 1, 1), /* Leggings of the Shadowy Vanquisher */
(@REF_UNSOK_HEROIC, 89253, 0, 1, 1, 1, 1), /* Leggings of the Shadowy Conqueror */
(@REF_UNSOK_HEROIC, 89254, 0, 1, 1, 1, 1), /* Leggings of the Shadowy Protector */

-- Mode 10 Heroic
SET @10HM_UNSOK = 81511;
UPDATE creature_template SET lootid = @10HM_UNSOK WHERE entry = @10HM_UNSOK;
DELETE FROM creature_loot_template WHERE entry = @10HM_UNSOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_UNSOK, 1, 100, 1, 0, -@REF_UNSOK_HEROIC, 2); /* REF 2 */

-- Mode 25 Heroic
SET @25HM_UNSOK = 82511;
UPDATE creature_template SET lootid = @25HM_UNSOK WHERE entry = @25HM_UNSOK;
DELETE FROM creature_loot_template WHERE entry = @25HM_UNSOK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_UNSOK, 1, 100, 1, 0, -@REF_UNSOK_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_UNSOK, @25NM_UNSOK);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_UNSOK, 396, 4000),
(@25NM_UNSOK, 396, 4000),
(@10HM_UNSOK, 396, 4000),
(@25HM_UNSOK, 396, 4000);
