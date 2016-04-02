-- Council of Elders Loots

-- Kaz'ra Jin
-- Normal Mode
SET @REF_COUNCIL_NORMAL = 69134;
DELETE FROM reference_loot_template WHERE entry = @REF_COUNCIL_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_COUNCIL_NORMAL, 96242, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Arrowflight (ThunderForged) */
(@REF_COUNCIL_NORMAL, 95060, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Arrowflight */
(@REF_COUNCIL_NORMAL, 96243, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Renewal (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95064, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Renewal */
(@REF_COUNCIL_NORMAL, 94760, 0, 1, 1, 1, 1), /* Amun-Thoth, Sul's Spiritrending Talons */
(@REF_COUNCIL_NORMAL, 94523, 0, 1, 1, 1, 1), /* Bad Juju */
(@REF_COUNCIL_NORMAL, 96249, 0, 1, 1, 1, 1), /* Bo-Ris, Horror in the Night (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95498, 0, 1, 1, 1, 1), /* Bo-Ris, Horror in the Night */
(@REF_COUNCIL_NORMAL, 96234, 0, 1, 1, 1, 1), /* Darkwood Spiritstaff (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95507, 0, 1, 1, 1, 1), /* Darkwood Spiritstaff */
(@REF_COUNCIL_NORMAL, 96248, 0, 1, 1, 1, 1), /* Do-tharak, the Swordbreaker (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95502, 0, 1, 1, 1, 1), /* Do-tharak, the Swordbreaker */
(@REF_COUNCIL_NORMAL, 96240, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Moon (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95065, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Moon */
(@REF_COUNCIL_NORMAL, 96241, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Sun (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95062, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Sun */
(@REF_COUNCIL_NORMAL, 94516, 0, 1, 1, 1, 1), /* Fortitude of the Zandalari */
(@REF_COUNCIL_NORMAL, 95501, 0, 1, 1, 1, 1), /* Fyn's Flickering Dagger */
(@REF_COUNCIL_NORMAL, 96232, 0, 1, 1, 1, 1), /* Fyn's Flickering Dagger (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95575, 0, 1, 1, 1, 1), /* Gauntlets of the Crackling Conqueror */
(@REF_COUNCIL_NORMAL, 95580, 0, 1, 1, 1, 1), /* Gauntlets of the Crackling Protector */
(@REF_COUNCIL_NORMAL, 95570, 0, 1, 1, 1, 1), /* Gauntlets of the Crackling Vanquisher */
(@REF_COUNCIL_NORMAL, 94763, 0, 1, 1, 1, 1), /* Gaze of Gara'jal */
(@REF_COUNCIL_NORMAL, 95505, 0, 1, 1, 1, 1), /* Greatsword of Frozen Hells */
(@REF_COUNCIL_NORMAL, 96247, 0, 1, 1, 1, 1), /* Greatsword of Frozen Hells (Thunderforged) */
(@REF_COUNCIL_NORMAL, 96230, 0, 1, 1, 1, 1), /* Invocation of the Dawn (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95499, 0, 1, 1, 1, 1), /* Invocation of the Dawn */
(@REF_COUNCIL_NORMAL, 95500, 0, 1, 1, 1, 1), /* Jerthud, Graceful Hand of the Savior */
(@REF_COUNCIL_NORMAL, 96239, 0, 1, 1, 1, 1), /* Jerthud, Graceful Hand of the Savior (Thunderforged) */
(@REF_COUNCIL_NORMAL, 94759, 0, 1, 1, 1, 1), /* Kura-Kura, Kazra'jin's Skullcleaver */
(@REF_COUNCIL_NORMAL, 94767, 0, 1, 1, 1, 1), /* Loa-Ridden Bracers */
(@REF_COUNCIL_NORMAL, 95061, 0, 1, 1, 1, 1), /* Lost Shoulders of Fire */
(@REF_COUNCIL_NORMAL, 96235, 0, 1, 1, 1, 1), /* Lost Shoulders of Fire (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95067, 0, 1, 1, 1, 1), /* Lost Shoulders of Fluidity */
(@REF_COUNCIL_NORMAL, 96237, 0, 1, 1, 1, 1), /* Lost Shoulders of Fluidity (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95066, 0, 1, 1, 1, 1), /* Lost Shoulders of Healing */
(@REF_COUNCIL_NORMAL, 94762, 0, 1, 1, 1, 1), /* Mar'li's Bloodstained Sandals) */
(@REF_COUNCIL_NORMAL, 95503, 0, 1, 1, 1, 1), /* Miracoran, the Vehement Chord */
(@REF_COUNCIL_NORMAL, 96231, 0, 1, 1, 1, 1), /* Miracoran, the Vehement Chord (Thunderforged) */
(@REF_COUNCIL_NORMAL, 96238, 0, 1, 1, 1, 1), /* Nadagast's Exsanguinator (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95506, 0, 1, 1, 1, 1), /* Nadagast's Exsanguinator */
(@REF_COUNCIL_NORMAL, 94764, 0, 1, 1, 1, 1), /* Overloaded Bladebreaker Cuirass */
(@REF_COUNCIL_NORMAL, 96246, 0, 1, 1, 1, 1), /* Reconstructed Bloody Shoulderplates (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95068, 0, 1, 1, 1, 1), /* Reconstructed Bloody Shoulderplates */
(@REF_COUNCIL_NORMAL, 95063, 0, 1, 1, 1, 1), /* Reconstructed Furious Shoulderplates */
(@REF_COUNCIL_NORMAL, 96245, 0, 1, 1, 1, 1), /* Reconstructed Furious Shoulderplates (Thunderforged) */
(@REF_COUNCIL_NORMAL, 95069, 0, 1, 1, 1, 1), /* Reconstructed Holy Shoulderplates */
(@REF_COUNCIL_NORMAL, 96244, 0, 1, 1, 1, 1), /* Reconstructed Holy Shoulderplates (Thunderforged) */
(@REF_COUNCIL_NORMAL, 94765, 0, 1, 1, 1, 1), /* Robes of Treacherous Ground */
(@REF_COUNCIL_NORMAL, 94766, 0, 1, 1, 1, 1), /* Talisman of Angry Spirits */
(@REF_COUNCIL_NORMAL, 97128, 0, 1, 1, 1, 1), /* Tia-Tia, the Scything Star (Thunderforged) */
(@REF_COUNCIL_NORMAL, 97126, 0, 1, 1, 1, 1), /* Tia-Tia, the Scything Star) */
(@REF_COUNCIL_NORMAL, 95516, 0, 1, 1, 1, 1), /* Visage of the Doomed */
(@REF_COUNCIL_NORMAL, 96250, 0, 1, 1, 1, 1), /* Visage of the Doomed (Thunderforged) */
(@REF_COUNCIL_NORMAL, 94513, 0, 1, 1, 1, 1), /* Wushoolay's Final Choice */
(@REF_COUNCIL_NORMAL, 94761, 0, 1, 1, 1, 1), /* Zandalari Robes of the Final Rite */
(@REF_COUNCIL_NORMAL, 95504, 0, 1, 1, 1, 1), /* Zeeg's Ancient Kegsmasher */
(@REF_COUNCIL_NORMAL, 96233, 0, 1, 1, 1, 1), /* Zeeg's Ancient Kegsmasher (Thunderforged) */
(@REF_COUNCIL_NORMAL, 94758, 0, 1, 1, 1, 1); /* Zerat, Malakk's Soulburning Greatsword */

-- Heroic Mode
SET @REF_COUNCIL_HEROIC = 81134;
DELETE FROM reference_loot_template WHERE entry = @REF_COUNCIL_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_COUNCIL_HEROIC, 96614, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Arrowflight */
(@REF_COUNCIL_HEROIC, 96986, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Arrowflight (ThunderForged) */
(@REF_COUNCIL_HEROIC, 96987, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Renewal (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96615, 0, 1, 1, 1, 1), /* Abandoned Spaulders of Renewal */
(@REF_COUNCIL_HEROIC, 96414, 0, 1, 1, 1, 1), /* Amun-Thoth, Sul's Spiritrending Talons */
(@REF_COUNCIL_HEROIC, 96409, 0, 1, 1, 1, 1), /* Bad Juju */
(@REF_COUNCIL_HEROIC, 96621, 0, 1, 1, 1, 1), /* Bo-Ris, Horror in the Night */
(@REF_COUNCIL_HEROIC, 96993, 0, 1, 1, 1, 1), /* Bo-Ris, Horror in the Night (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96978, 0, 1, 1, 1, 1), /* Darkwood Spiritstaff (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96606, 0, 1, 1, 1, 1), /* Darkwood Spiritstaff */
(@REF_COUNCIL_HEROIC, 96992, 0, 1, 1, 1, 1), /* Do-tharak, the Swordbreaker (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96620, 0, 1, 1, 1, 1), /* Do-tharak, the Swordbreaker */
(@REF_COUNCIL_HEROIC, 95065, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Moon */
(@REF_COUNCIL_HEROIC, 96612, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Moon (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96985, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Sun (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96613, 0, 1, 1, 1, 1), /* Forgotten Mantle of the Sun */
(@REF_COUNCIL_HEROIC, 96421, 0, 1, 1, 1, 1), /* Fortitude of the Zandalari */
(@REF_COUNCIL_HEROIC, 96604, 0, 1, 1, 1, 1), /* Fyn's Flickering Dagger */
(@REF_COUNCIL_HEROIC, 96976, 0, 1, 1, 1, 1), /* Fyn's Flickering Dagger (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96600, 0, 1, 1, 1, 1), /* Gauntlets of the Crackling Conqueror */
(@REF_COUNCIL_HEROIC, 96601, 0, 1, 1, 1, 1), /* Gauntlets of the Crackling Protector */
(@REF_COUNCIL_HEROIC, 96599, 0, 1, 1, 1, 1), /* Gauntlets of the Crackling Vanquisher */
(@REF_COUNCIL_HEROIC, 96416, 0, 1, 1, 1, 1), /* Gaze of Gara'jal */
(@REF_COUNCIL_HEROIC, 96991, 0, 1, 1, 1, 1), /* Greatsword of Frozen Hells (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96619, 0, 1, 1, 1, 1), /* Greatsword of Frozen Hells */
(@REF_COUNCIL_HEROIC, 96127, 0, 1, 1, 1, 1), /* Hydra-Scale Bloodcloak (Thunderforged) */
(@REF_COUNCIL_HEROIC, 9662, 0, 1, 1, 1, 1), /* Invocation of the Dawn */
(@REF_COUNCIL_HEROIC, 96611, 0, 1, 1, 1, 1), /* Jerthud, Graceful Hand of the Savior */
(@REF_COUNCIL_HEROIC, 96983, 0, 1, 1, 1, 1), /* Jerthud, Graceful Hand of the Savior (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96410, 0, 1, 1, 1, 1), /* Kura-Kura, Kazra'jin's Skullcleaver */
(@REF_COUNCIL_HEROIC, 94767, 0, 1, 1, 1, 1), /* Loa-Ridden Bracers */
(@REF_COUNCIL_HEROIC, 96607, 0, 1, 1, 1, 1), /* Lost Shoulders of Fire */
(@REF_COUNCIL_HEROIC, 96979, 0, 1, 1, 1, 1), /* Lost Shoulders of Fire (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96981, 0, 1, 1, 1, 1), /* Lost Shoulders of Fluidity (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96609, 0, 1, 1, 1, 1), /* Lost Shoulders of Fluidity */
(@REF_COUNCIL_HEROIC, 96980, 0, 1, 1, 1, 1), /* Lost Shoulders of Healing (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96608, 0, 1, 1, 1, 1), /* Lost Shoulders of Healing */
(@REF_COUNCIL_HEROIC, 96975, 0, 1, 1, 1, 1), /* Miracoran, the Vehement Chord (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96603, 0, 1, 1, 1, 1), /* Miracoran, the Vehement Chord */
(@REF_COUNCIL_HEROIC, 96610, 0, 1, 1, 1, 1), /* Nadagast's Exsanguinator */
(@REF_COUNCIL_HEROIC, 96982, 0, 1, 1, 1, 1), /* Nadagast's Exsanguinator (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96418, 0, 1, 1, 1, 1), /* Overloaded Bladebreaker Cuirass */
(@REF_COUNCIL_HEROIC, 96990, 0, 1, 1, 1, 1), /* Reconstructed Bloody Shoulderplates (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96618, 0, 1, 1, 1, 1), /* Reconstructed Bloody Shoulderplates */
(@REF_COUNCIL_HEROIC, 96617, 0, 1, 1, 1, 1), /* Reconstructed Furious Shoulderplates */
(@REF_COUNCIL_HEROIC, 96989, 0, 1, 1, 1, 1), /* Reconstructed Furious Shoulderplates (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96616, 0, 1, 1, 1, 1), /* Reconstructed Holy Shoulderplates */
(@REF_COUNCIL_HEROIC, 96988, 0, 1, 1, 1, 1), /* Reconstructed Holy Shoulderplates (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96415, 0, 1, 1, 1, 1), /* Robes of Treacherous Ground */
(@REF_COUNCIL_HEROIC, 97130, 0, 1, 1, 1, 1), /* Tia-Tia, the Scything Star (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96994, 0, 1, 1, 1, 1), /* Visage of the Doomed (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96622, 0, 1, 1, 1, 1), /* Visage of the Doomed */
(@REF_COUNCIL_HEROIC, 96413, 0, 1, 1, 1, 1), /* Wushoolay's Final Choice */
(@REF_COUNCIL_HEROIC, 96412, 0, 1, 1, 1, 1), /* Zandalari Robes of the Final Rite */
(@REF_COUNCIL_HEROIC, 96977, 0, 1, 1, 1, 1), /* Zeeg's Ancient Kegsmasher (Thunderforged) */
(@REF_COUNCIL_HEROIC, 96605, 0, 1, 1, 1, 1), /* Zeeg's Ancient Kegsmasher */
(@REF_COUNCIL_HEROIC, 96419, 0, 1, 1, 1, 1); /* Zerat, Malakk's Soulburning Greatsword */

-- Mode 10 normal
SET @10NM_KAZ_RA_JIN = 69134;
UPDATE creature_template SET lootid = @10NM_KAZ_RA_JIN WHERE entry = @10NM_KAZ_RA_JIN;
DELETE FROM creature_loot_template WHERE entry = @10NM_KAZ_RA_JIN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_KAZ_RA_JIN, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_KAZ_RA_JIN = 80134;
UPDATE creature_template SET lootid = @25NM_KAZ_RA_JIN WHERE entry = @25NM_KAZ_RA_JIN;
DELETE FROM creature_loot_template WHERE entry = @25NM_KAZ_RA_JIN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_KAZ_RA_JIN, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 5); /* REF 1 */

-- Mode 10 Heroic
SET @10HM_KAZ_RA_JIN = 81134;
UPDATE creature_template SET lootid = @10HM_KAZ_RA_JIN WHERE entry = @10HM_KAZ_RA_JIN;
DELETE FROM creature_loot_template WHERE entry = @10HM_KAZ_RA_JIN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_KAZ_RA_JIN, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_KAZ_RA_JIN = 82134;
UPDATE creature_template SET lootid = @25HM_KAZ_RA_JIN WHERE entry = @25HM_KAZ_RA_JIN;
DELETE FROM creature_loot_template WHERE entry = @25HM_KAZ_RA_JIN;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_KAZ_RA_JIN, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_KAZ_RA_JIN, @25NM_KAZ_RA_JIN, @10HM_KAZ_RA_JIN, @25HM_KAZ_RA_JIN);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_KAZ_RA_JIN, 396, 4000),
(@25NM_KAZ_RA_JIN, 396, 4000),
(@10HM_KAZ_RA_JIN, 396, 4000),
(@25HM_KAZ_RA_JIN, 396, 4000);


-- Mar'Li
-- Mode 10 normal
SET @10NM_MAR_LI = 69132;
UPDATE creature_template SET lootid = @10NM_MAR_LI WHERE entry = @10NM_MAR_LI;
DELETE FROM creature_loot_template WHERE entry = @10NM_MAR_LI;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_MAR_LI, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_MAR_LI = 80132;
UPDATE creature_template SET lootid = @25NM_MAR_LI WHERE entry = @25NM_MAR_LI;
DELETE FROM creature_loot_template WHERE entry = @25NM_MAR_LI;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_MAR_LI, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 5); /* REF 1 */

-- Mode 10 Heroic
SET @10HM_MAR_LI = 81132;
UPDATE creature_template SET lootid = @10HM_MAR_LI WHERE entry = @10HM_MAR_LI;
DELETE FROM creature_loot_template WHERE entry = @10HM_MAR_LI;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_MAR_LI, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_MAR_LI = 82132;
UPDATE creature_template SET lootid = @25HM_MAR_LI WHERE entry = @25HM_MAR_LI;
DELETE FROM creature_loot_template WHERE entry = @25HM_MAR_LI;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_MAR_LI, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_MAR_LI, @25NM_MAR_LI, @10HM_MAR_LI, @25HM_MAR_LI);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_MAR_LI, 396, 4000),
(@25NM_MAR_LI, 396, 4000),
(@10HM_MAR_LI, 396, 4000),
(@25HM_MAR_LI, 396, 4000);


-- Sul the SandCrawler
-- Mode 10 normal
SET @10NM_SUL = 69078;
UPDATE creature_template SET lootid = @10NM_SUL WHERE entry = @10NM_SUL;
DELETE FROM creature_loot_template WHERE entry = @10NM_SUL;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_SUL, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_SUL = 80078;
UPDATE creature_template SET lootid = @25NM_SUL WHERE entry = @25NM_SUL;
DELETE FROM creature_loot_template WHERE entry = @25NM_SUL;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_SUL, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 5); /* REF 1 */

-- Mode 10 Heroic
SET @10HM_SUL = 81078;
UPDATE creature_template SET lootid = @10HM_SUL WHERE entry = @10HM_SUL;
DELETE FROM creature_loot_template WHERE entry = @10HM_SUL;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_SUL, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_SUL = 82078;
UPDATE creature_template SET lootid = @25HM_SUL WHERE entry = @25HM_SUL;
DELETE FROM creature_loot_template WHERE entry = @25HM_SUL;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_SUL, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_SUL, @25NM_SUL, @10HM_SUL, @25HM_SUL);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_SUL, 396, 4000),
(@25NM_SUL, 396, 4000),
(@10HM_SUL, 396, 4000),
(@25HM_SUL, 396, 4000);


-- Frost King Malakk
-- Mode 10 normal
SET @10NM_MALAKK = 69131;
UPDATE creature_template SET lootid = @10NM_MALAKK WHERE entry = @10NM_MALAKK;
DELETE FROM creature_loot_template WHERE entry = @10NM_MALAKK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10NM_MALAKK, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 2); /* REF 1 */

-- Mode 25 normal
SET @25NM_MALAKK = 80131;
UPDATE creature_template SET lootid = @25NM_MALAKK WHERE entry = @25NM_MALAKK;
DELETE FROM creature_loot_template WHERE entry = @25NM_MALAKK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25NM_MALAKK, 1, 100, 1, 0, -@REF_COUNCIL_NORMAL, 5); /* REF 1 */

-- Mode 10 Heroic
SET @10HM_MALAKK = 81131;
UPDATE creature_template SET lootid = @10HM_MALAKK WHERE entry = @10HM_MALAKK;
DELETE FROM creature_loot_template WHERE entry = @10HM_MALAKK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@10HM_MALAKK, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 2); /* REF 2 */

-- Mode 10 Heroic
SET @25HM_MALAKK = 82131;
UPDATE creature_template SET lootid = @25HM_MALAKK WHERE entry = @25HM_MALAKK;
DELETE FROM creature_loot_template WHERE entry = @25HM_MALAKK;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@25HM_MALAKK, 1, 100, 1, 0, -@REF_COUNCIL_HEROIC, 5); /* REF 2 */

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_MALAKK, @25NM_MALAKK, @10HM_MALAKK, @25HM_MALAKK);
REPLACE INTO creature_loot_currency (creature_id, CurrencyId1, CurrencyCount1) VALUES
(@10NM_MALAKK, 396, 4000),
(@25NM_MALAKK, 396, 4000),
(@10HM_MALAKK, 396, 4000),
(@25HM_MALAKK, 396, 4000);