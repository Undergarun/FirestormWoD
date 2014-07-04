-- Normal mode
SET @REF_ZONE_LOOT = 600000;
DELETE FROM `reference_loot_template` WHERE entry = @REF_ZONE_LOOT;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ZONE_LOOT, 95060, 0, 1, 1, 1, 1) /* Abandoned Spaulders of Arrowflight */,
(@REF_ZONE_LOOT, 95507, 0, 1, 1, 1, 1) /* Darkwood Spiritstaff */,
(@REF_ZONE_LOOT, 95502, 0, 1, 1, 1, 1) /* Do-tharak, the Swordbreaker */,
(@REF_ZONE_LOOT, 95062, 0, 1, 1, 1, 1) /* Forgotten Mantle of the Sun */,
(@REF_ZONE_LOOT, 95501, 0, 1, 1, 1, 1) /* Fyn's Flickering Dagger */,
(@REF_ZONE_LOOT, 95505, 0, 1, 1, 1, 1) /* Greatsword of Frozen Hells */,
(@REF_ZONE_LOOT, 95499, 0, 1, 1, 1, 1) /* Invocation of the Dawn */,
(@REF_ZONE_LOOT, 95500, 0, 1, 1, 1, 1) /* Jerthud, Graceful Hand of the Savior */,
(@REF_ZONE_LOOT, 95061, 0, 1, 1, 1, 1) /* Lost Shoulders of Fire */,
(@REF_ZONE_LOOT, 95067, 0, 1, 1, 1, 1) /* Lost Shoulders of Fluidity */,
(@REF_ZONE_LOOT, 95506, 0, 1, 1, 1, 1) /* Nadagast's Exsanguinator */,
(@REF_ZONE_LOOT, 95068, 0, 1, 1, 1, 1) /* Reconstructed Bloody Shoulderplates */,
(@REF_ZONE_LOOT, 97126, 0, 1, 1, 1, 1) /* Tia-Tia, the Scything Star */,
(@REF_ZONE_LOOT, 95064, 0, 1, 1, 1, 1) /* Abandoned Spaulders of Renewal */,
(@REF_ZONE_LOOT, 95065, 0, 1, 1, 1, 1) /* Forgotten Mantle of the Moon */,
(@REF_ZONE_LOOT, 95066, 0, 1, 1, 1, 1) /* Lost Shoulders of Healing */,
(@REF_ZONE_LOOT, 95504, 0, 1, 1, 1, 1) /* Zeeg's Ancient Kegsmasher */,
(@REF_ZONE_LOOT, 95063, 0, 1, 1, 1, 1) /* Reconstructed Furious Shoulderplates */,
(@REF_ZONE_LOOT, 95069, 0, 1, 1, 1, 1) /* Reconstructed Holy Shoulderplates */,
(@REF_ZONE_LOOT, 95498, 0, 1, 1, 1, 1) /* Bo-Ris, Horror in the Night */,
(@REF_ZONE_LOOT, 95503, 0, 1, 1, 1, 1) /* Miracoran, the Vehement Chord */,
(@REF_ZONE_LOOT, 95516, 0, 1, 1, 1, 1) /* Visage of the Doomed */;

SET @REF_JIN_ROKH_NORMAL = 69465;
DELETE FROM `reference_loot_template` WHERE entry = @REF_JIN_ROKH_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_JIN_ROKH_NORMAL, 94738, 0, 1, 1, 1, 1) /* Jin'rokh's Dreamshard */,
(@REF_JIN_ROKH_NORMAL, 95510, 0, 1, 1, 1, 1) /* Sign of the Bloodied God */,
(@REF_JIN_ROKH_NORMAL, 94512, 0, 1, 1, 1, 1) /* Renataki's Soul Charm */,
(@REF_JIN_ROKH_NORMAL, 94739, 0, 1, 1, 1, 1) /* Al'set's Tormented Leggings */,
(@REF_JIN_ROKH_NORMAL, 94726, 0, 1, 1, 1, 1) /* Cloudbreaker Greatbelt */,
(@REF_JIN_ROKH_NORMAL, 94723, 0, 1, 1, 1, 1) /* Chestplate of Violent Detonation */,
(@REF_JIN_ROKH_NORMAL, 94735, 0, 1, 1, 1, 1) /* Drape of Booming Nights */,
(@REF_JIN_ROKH_NORMAL, 94733, 0, 1, 1, 1, 1) /* Fissure-Split Shoulderwraps */,
(@REF_JIN_ROKH_NORMAL, 94724, 0, 1, 1, 1, 1) /* Lightning-Eye Hood */,
(@REF_JIN_ROKH_NORMAL, 94737, 0, 1, 1, 1, 1) /* Lightningweaver Gauntlets */,
(@REF_JIN_ROKH_NORMAL, 94730, 0, 1, 1, 1, 1) /* Soulblade of the Breaking Storm */,
(@REF_JIN_ROKH_NORMAL, 94725, 0, 1, 1, 1, 1) /* Static-Shot Shoulderguards */,
(@REF_JIN_ROKH_NORMAL, 94728, 0, 1, 1, 1, 1) /* Spearman's Jingling Leggings */,
(@REF_JIN_ROKH_NORMAL, 94722, 0, 1, 1, 1, 1) /* Worldbreaker's Stormscythe */,
(@REF_JIN_ROKH_NORMAL, 94736, 0, 1, 1, 1, 1) /* Ghostbinder Greatboots */,
(@REF_JIN_ROKH_NORMAL, 94732, 0, 1, 1, 1, 1) /* Infinitely Conducting Bracers */,
(@REF_JIN_ROKH_NORMAL, 94734, 0, 1, 1, 1, 1) /* Ionized Yojamban Carapace */,
(@REF_JIN_ROKH_NORMAL, 94729, 0, 1, 1, 1, 1) /* Jin'rokh's Soulcrystal */,
(@REF_JIN_ROKH_NORMAL, 94731, 0, 1, 1, 1, 1) /* Robes of Static Bursts */,
(@REF_JIN_ROKH_NORMAL, 94727, 0, 1, 1, 1, 1) /* Bracers of Constant Implosion */;

-- Mode 10 Normal
SET @10NM_JIN_ROKH = 69465;
UPDATE `creature_template` SET `lootid`= @10NM_JIN_ROKH WHERE `entry`= @10NM_JIN_ROKH;
DELETE FROM `creature_loot_template` WHERE `entry`= @10NM_JIN_ROKH;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10NM_JIN_ROKH, 1, 100, 1, 0, -@REF_JIN_ROKH_NORMAL, 2) /* REF 1*/,
(@10NM_JIN_ROKH, 3, 0.5, 1, 0, -@REF_ZONE_LOOT, 1) /* REF ZONE DROP */;

-- Mode 25 Normal
SET @25NM_JIN_ROKH = 369465;
UPDATE `creature_template` SET `lootid`= @25NM_JIN_ROKH WHERE `entry`= @25NM_JIN_ROKH;
DELETE FROM `creature_loot_template` WHERE `entry`= @25NM_JIN_ROKH;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25NM_JIN_ROKH, 1, 100, 1, 0, -@REF_JIN_ROKH_NORMAL, 5) /* REF 1*/,
(@25NM_JIN_ROKH, 3, 0.5, 1, 0, -@REF_ZONE_LOOT, 1) /* REF ZONE DROP */;

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_JIN_ROKH, @25NM_JIN_ROKH);
REPLACE INTO creature_loot_currency (`creature_id`, `CurrencyId1`, `CurrencyCount1`) VALUES
(@10NM_JIN_ROKH, 396, 4000),
(@10NM_JIN_ROKH, 396, 4000); 