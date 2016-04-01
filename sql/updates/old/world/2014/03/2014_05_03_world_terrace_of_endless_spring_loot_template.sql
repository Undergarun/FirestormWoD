-- Reference parchemin

SET @REF_PARCHO = 100000;
DELETE FROM `reference_loot_template` WHERE entry = @REF_PARCHO;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_PARCHO, 86281, 0, 1, 0, 1, 1) /* Pattern: Nightfire Robe */,
(@REF_PARCHO, 86380, 0, 1, 0, 1, 1) /* Pattern: Imperial Silk Gloves */,
(@REF_PARCHO, 86284, 0, 1, 0, 1, 1) /* Pattern: Raven Lord's Gloves */,
(@REF_PARCHO, 87408, 0, 1, 0, 1, 1) /* Plans: Unyielding Bloodplate */,
(@REF_PARCHO, 86280, 0, 1, 0, 1, 1) /* Pattern: Murderer's Gloves */,
(@REF_PARCHO, 86381, 0, 1, 0, 1, 1) /* Pattern: Legacy of the Emperor */,
(@REF_PARCHO, 87412, 0, 1, 0, 1, 1) /* Plans: Chestplate of Limitless Faith */,
(@REF_PARCHO, 87411, 0, 1, 0, 1, 1) /* Plans: Bloodforged Warfists */;


/**********************************************************************
***                  PROTECTORS OF THE ENDLESS                      ***
***********************************************************************/

-- REF NM

-- ANCIENT ASANI
SET @REF_ASANI_1 = 60586;
DELETE FROM `reference_loot_template` WHERE entry = @REF_ASANI_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ASANI_1, 86231, 0, 1, 1, 1, 1) /* Regail's Band of the Endless */,
(@REF_ASANI_1, 86390, 0, 1, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_ASANI_1, 86315, 0, 1, 1, 1, 1) /* Watersoul Signet */,
(@REF_ASANI_1, 86316, 0, 1, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_ASANI_1, 86233, 0, 1, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_ASANI_1, 86234, 0, 1, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_ASANI_1, 86232, 0, 1, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_ASANI_1, 86317, 0, 1, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_ASANI_1, 86320, 0, 1, 1, 1, 1) /* Asani's Uncleansed Sandals */,
(@REF_ASANI_1, 86230, 0, 1, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_ASANI_1, 86318, 0, 1, 1, 1, 1) /* Casque of Expelled Corruptione */,
(@REF_ASANI_1, 89885, 0, 1, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_ASANI_1, 89841, 0, 1, 1, 1, 1) /* Legguards of Failing Purification */,
(@REF_ASANI_1, 86319, 0, 1, 1, 1, 1) /* Lightning Prisoner's Boots */;

-- ANCIENT REGAIL
SET @REF_REGAIL_1 = 60585;
DELETE FROM `reference_loot_template` WHERE entry = @REF_REGAIL_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_REGAIL_1, 86231, 0, 1, 1, 1, 1) /* Regail's Band of the Endless */,
(@REF_REGAIL_1, 86390, 0, 1, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_REGAIL_1, 86315, 0, 1, 1, 1, 1) /* Watersoul Signet */,
(@REF_REGAIL_1, 86316, 0, 1, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_REGAIL_1, 86233, 0, 1, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_REGAIL_1, 86234, 0, 1, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_REGAIL_1, 86232, 0, 1, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_REGAIL_1, 86317, 0, 1, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_REGAIL_1, 86320, 0, 1, 1, 1, 1) /* Asani's Uncleansed Sandals */,
(@REF_REGAIL_1, 86230, 0, 1, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_REGAIL_1, 86318, 0, 1, 1, 1, 1) /* Casque of Expelled Corruptione */,
(@REF_REGAIL_1, 89885, 0, 1, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_REGAIL_1, 89841, 0, 1, 1, 1, 1) /* Legguards of Failing Purification */,
(@REF_REGAIL_1, 86319, 0, 1, 1, 1, 1) /* Lightning Prisoner's Boots */;

-- PROTECTOR KAOLAN
SET @REF_KAOLAN_1 = 60583;
DELETE FROM `reference_loot_template` WHERE entry = @REF_KAOLAN_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAOLAN_1, 86231, 0, 1, 1, 1, 1) /* Regail's Band of the Endless */,
(@REF_KAOLAN_1, 86390, 0, 1, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_KAOLAN_1, 86315, 0, 1, 1, 1, 1) /* Watersoul Signet */,
(@REF_KAOLAN_1, 86316, 0, 1, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_KAOLAN_1, 86233, 0, 1, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_KAOLAN_1, 86234, 0, 1, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_KAOLAN_1, 86232, 0, 1, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_KAOLAN_1, 86317, 0, 1, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_KAOLAN_1, 86320, 0, 1, 1, 1, 1) /* Asani's Uncleansed Sandals */,
(@REF_KAOLAN_1, 86230, 0, 1, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_KAOLAN_1, 86318, 0, 1, 1, 1, 1) /* Casque of Expelled Corruptione */,
(@REF_KAOLAN_1, 89885, 0, 1, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_KAOLAN_1, 89841, 0, 1, 1, 1, 1) /* Legguards of Failing Purification */,
(@REF_KAOLAN_1, 86319, 0, 1, 1, 1, 1) /* Lightning Prisoner's Boots */,
(@REF_KAOLAN_1, 90517, 0, 2, 1, 1, 1) /* Regail's Band of the Endless */, 
(@REF_KAOLAN_1, 90529, 0, 2, 1, 1, 1) /* Lightning Prisoner's Boots */,
(@REF_KAOLAN_1, 90520, 0, 2, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_KAOLAN_1, 90526, 0, 2, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_KAOLAN_1, 90530, 0, 2, 1, 1, 1) /* Casque of Expelled Corruption */,
(@REF_KAOLAN_1, 90522, 0, 2, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_KAOLAN_1, 90525, 0, 2, 1, 1, 1) /* Watersoul Signet */,
(@REF_KAOLAN_1, 90523, 0, 2, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_KAOLAN_1, 90524, 0, 2, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_KAOLAN_1, 90527, 0, 2, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_KAOLAN_1, 90519, 0, 2, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_KAOLAN_1, 90518, 0, 2, 1, 1, 1) /* Legguards of Failing Purification */,
(@REF_KAOLAN_1, 90521, 0, 2, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_KAOLAN_1, 90528, 0, 2, 1, 1, 1) /* Asani's Uncleansed Sandals */;

-- MODE : 10_NM (60586-60585-60583)

SET @10_NM_ENTRY_ASANI = 60586;
UPDATE `creature_template` SET `lootid`= @10_NM_ENTRY_ASANI WHERE `entry`= @10_NM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_NM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@10_NM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_1, 2) /* REF 1*/,
(@10_NM_ENTRY_ASANI, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @10_NM_ENTRY_REGAIL = 60585;
UPDATE `creature_template` SET `lootid`= @10_NM_ENTRY_REGAIL WHERE `entry`= @10_NM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_NM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@10_NM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_1, 2) /* REF 1*/,
(@10_NM_ENTRY_REGAIL, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @10_NM_ENTRY_KAOLAN = 60583;
UPDATE `creature_template` SET `lootid`= @10_NM_ENTRY_KAOLAN WHERE `entry`= @10_NM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_NM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@10_NM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_1, 2) /* REF 1*/,
(@10_NM_ENTRY_KAOLAN, 2, 100, 2, 0, -@REF_KAOLAN_1, 2) /* REF 1*/,
(@10_NM_ENTRY_KAOLAN, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

-- MODE : 25_NM (60586-60585-60583)

SET @25_NM_ENTRY_ASANI = 81586;
UPDATE `creature_template` SET `lootid`= @25_NM_ENTRY_ASANI WHERE `entry`= @25_NM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_NM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@25_NM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_1, 4) /* REF 1*/,
(@25_NM_ENTRY_ASANI, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @25_NM_ENTRY_REGAIL = 81585;
UPDATE `creature_template` SET `lootid`= @25_NM_ENTRY_REGAIL WHERE `entry`= @25_NM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_NM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@25_NM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_1, 4) /* REF 1*/,
(@25_NM_ENTRY_REGAIL, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;

SET @25_NM_ENTRY_KAOLAN = 81583;
UPDATE `creature_template` SET `lootid`= @25_NM_ENTRY_KAOLAN WHERE `entry`= @25_NM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_NM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@25_NM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_1, 4) /* REF 1*/,
(@25_NM_ENTRY_KAOLAN, 2, 100, 2, 0, -@REF_KAOLAN_1, 4) /* REF 1*/,
(@25_NM_ENTRY_KAOLAN, 3, 0.5, 1, 0, -@REF_PARCHO, 1) /* REF parcho*/;



-- Ref HM 

SET @REF_ASANI_2 = 82586;
DELETE FROM `reference_loot_template` WHERE entry = @REF_ASANI_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_ASANI_2, 87150, 0, 1, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_ASANI_2, 87144, 0, 1, 1, 1, 1) /* Regail's Band of the Endless */,
(@REF_ASANI_2, 87152, 0, 1, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_ASANI_2, 87147, 0, 1, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_ASANI_2, 87151, 0, 1, 1, 1, 1) /* Watersoul Signet */,
(@REF_ASANI_2, 87148, 0, 1, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_ASANI_2, 89944, 0, 1, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_ASANI_2, 87145, 0, 1, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_ASANI_2, 87149, 0, 1, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_ASANI_2, 87155, 0, 1, 1, 1, 1) /* Casque of Expelled Corruption */,
(@REF_ASANI_2, 87146, 0, 1, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_ASANI_2, 87153, 0, 1, 1, 1, 1) /* Asani's Uncleansed Sandals */,
(@REF_ASANI_2, 87154, 0, 1, 1, 1, 1) /* Lightning Prisoner's Boots */,
(@REF_ASANI_2, 89943, 0, 1, 1, 1, 1) /* Legguards of Failing Purification */;

SET @REF_REGAIL_2 = 82585;
DELETE FROM `reference_loot_template` WHERE entry = @REF_REGAIL_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_REGAIL_2, 87150, 0, 1, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_REGAIL_2, 87144, 0, 1, 1, 1, 1) /* Regail's Band of the Endless */,
(@REF_REGAIL_2, 87152, 0, 1, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_REGAIL_2, 87147, 0, 1, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_REGAIL_2, 87151, 0, 1, 1, 1, 1) /* Watersoul Signet */,
(@REF_REGAIL_2, 87148, 0, 1, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_REGAIL_2, 89944, 0, 1, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_REGAIL_2, 87145, 0, 1, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_REGAIL_2, 87149, 0, 1, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_REGAIL_2, 87155, 0, 1, 1, 1, 1) /* Casque of Expelled Corruption */,
(@REF_REGAIL_2, 87146, 0, 1, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_REGAIL_2, 87153, 0, 1, 1, 1, 1) /* Asani's Uncleansed Sandals */,
(@REF_REGAIL_2, 87154, 0, 1, 1, 1, 1) /* Lightning Prisoner's Boots */,
(@REF_REGAIL_2, 89943, 0, 1, 1, 1, 1) /* Legguards of Failing Purification */;

SET @REF_KAOLAN_2 = 82583;
DELETE FROM `reference_loot_template` WHERE entry = @REF_KAOLAN_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_KAOLAN_2, 87150, 0, 1, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_KAOLAN_2, 87144, 0, 1, 1, 1, 1) /* Regail's Band of the Endless */,
(@REF_KAOLAN_2, 87152, 0, 1, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_KAOLAN_2, 87147, 0, 1, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_KAOLAN_2, 87151, 0, 1, 1, 1, 1) /* Watersoul Signet */,
(@REF_KAOLAN_2, 87148, 0, 1, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_KAOLAN_2, 89944, 0, 1, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_KAOLAN_2, 87145, 0, 1, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_KAOLAN_2, 87149, 0, 1, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_KAOLAN_2, 87155, 0, 1, 1, 1, 1) /* Casque of Expelled Corruption */,
(@REF_KAOLAN_2, 87146, 0, 1, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_KAOLAN_2, 87153, 0, 1, 1, 1, 1) /* Asani's Uncleansed Sandals */,
(@REF_KAOLAN_2, 87154, 0, 1, 1, 1, 1) /* Lightning Prisoner's Boots */,
(@REF_KAOLAN_2, 90504, 0, 2, 1, 1, 1) /* Legguards of Failing Purification */,
(@REF_KAOLAN_2, 90503, 0, 2, 1, 1, 1) /* Regail's Band of the Endless */, 
(@REF_KAOLAN_2, 90515, 0, 2, 1, 1, 1) /* Lightning Prisoner's Boots */,
(@REF_KAOLAN_2, 90506, 0, 2, 1, 1, 1) /* Bracers of Defiled Earth */,
(@REF_KAOLAN_2, 90512, 0, 2, 1, 1, 1) /* Cloak of Overwhelming Corruption */,
(@REF_KAOLAN_2, 90516, 0, 2, 1, 1, 1) /* Casque of Expelled Corruption */,
(@REF_KAOLAN_2, 90508, 0, 2, 1, 1, 1) /* Shackle of Eversparks */,
(@REF_KAOLAN_2, 90511, 0, 2, 1, 1, 1) /* Watersoul Signet */,
(@REF_KAOLAN_2, 90509, 0, 2, 1, 1, 1) /* Kaolan's Withering Necklace */,
(@REF_KAOLAN_2, 90510, 0, 2, 1, 1, 1) /* Cuffs of the Corrupted Waters */,
(@REF_KAOLAN_2, 90513, 0, 2, 1, 1, 1) /* Regail's Crackling Dagger */,
(@REF_KAOLAN_2, 90505, 0, 2, 1, 1, 1) /* Waterborne Shoulderguards */,
(@REF_KAOLAN_2, 90507, 0, 2, 1, 1, 1) /* Deepwater Greatboots */,
(@REF_KAOLAN_2, 90514, 0, 2, 1, 1, 1) /* Asani's Uncleansed Sandals */;

-- MODE : 10_HM (60586-60585-60583)

SET @10_HM_ENTRY_ASANI = 80586;
UPDATE `creature_template` SET `lootid`= @10_HM_ENTRY_ASANI WHERE `entry`= @10_HM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_HM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@10_HM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_2, 2) /* REF 2*/;

SET @10_HM_ENTRY_REGAIL = 80585;
UPDATE `creature_template` SET `lootid`= @10_HM_ENTRY_REGAIL WHERE `entry`= @10_HM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_HM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@10_HM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_2, 2) /* REF 2*/;

SET @10_HM_ENTRY_KAOLAN = 80583;
UPDATE `creature_template` SET `lootid`= @10_HM_ENTRY_KAOLAN WHERE `entry`= @10_HM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @10_HM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@10_HM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_2, 2) /* REF 2*/,
(@10_HM_ENTRY_KAOLAN, 2, 100, 2, 0, -@REF_KAOLAN_2, 2) /* REF 2*/;

-- MODE : 25_HM (60586-60585-60583)

SET @25_HM_ENTRY_ASANI = 82586;
UPDATE `creature_template` SET `lootid`= @25_HM_ENTRY_ASANI WHERE `entry`= @25_HM_ENTRY_ASANI;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_HM_ENTRY_ASANI;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY_ASANI, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@25_HM_ENTRY_ASANI, 1, 100, 1, 0, -@REF_ASANI_2, 4) /* REF 2*/;

SET @25_HM_ENTRY_REGAIL = 82585;
UPDATE `creature_template` SET `lootid`= @25_HM_ENTRY_REGAIL WHERE `entry`= @25_HM_ENTRY_REGAIL;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_HM_ENTRY_REGAIL;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY_REGAIL, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@25_HM_ENTRY_REGAIL, 1, 100, 1, 0, -@REF_REGAIL_2, 4) /* REF 2*/;

SET @25_HM_ENTRY_KAOLAN = 82583;
UPDATE `creature_template` SET `lootid`= @25_HM_ENTRY_KAOLAN WHERE `entry`= @25_HM_ENTRY_KAOLAN;
DELETE FROM `creature_loot_template` WHERE `entry`= @25_HM_ENTRY_KAOLAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY_KAOLAN, 87208, -20, 1, 0, 1, 1) /* Sigil of Power */,
(@25_HM_ENTRY_KAOLAN, 1, 100, 1, 0, -@REF_KAOLAN_2, 4) /* REF 2*/,
(@25_HM_ENTRY_KAOLAN, 2, 100, 2, 0, -@REF_KAOLAN_2, 4) /* REF 2*/;

-- CURRENCY

DELETE FROM creature_loot_currency WHERE creature_id IN
(@10_NM_ENTRY_ASANI,@10_NM_ENTRY_REGAIL,@10_NM_ENTRY_KAOLAN,@25_NM_ENTRY_ASANI,@25_NM_ENTRY_REGAIL,@25_NM_ENTRY_KAOLAN,
@10_HM_ENTRY_ASANI,@10_HM_ENTRY_REGAIL,@10_HM_ENTRY_KAOLAN,@25_HM_ENTRY_ASANI,@25_HM_ENTRY_REGAIL,@25_HM_ENTRY_KAOLAN);
REPLACE INTO creature_loot_currency (`creature_id`, `CurrencyId1`, `CurrencyCount1`) VALUES
(@10_NM_ENTRY_ASANI, 396, 4000),
(@10_NM_ENTRY_REGAIL, 396, 4000),
(@10_NM_ENTRY_KAOLAN, 396, 4000),
(@25_NM_ENTRY_ASANI, 396, 4000),
(@25_NM_ENTRY_REGAIL, 396, 4000),
(@25_NM_ENTRY_KAOLAN, 396, 4000),
(@10_HM_ENTRY_ASANI, 396, 4000),
(@10_HM_ENTRY_REGAIL, 396, 4000),
(@10_HM_ENTRY_KAOLAN, 396, 4000),
(@25_HM_ENTRY_ASANI, 396, 4000),
(@25_HM_ENTRY_REGAIL, 396, 4000),
(@25_HM_ENTRY_KAOLAN, 396, 4000);