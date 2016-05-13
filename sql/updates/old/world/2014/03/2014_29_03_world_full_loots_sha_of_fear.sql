-- Normal mode
SET @REF_SHA_OF_FEAR_NORMAL = 60999;
DELETE FROM `reference_loot_template` WHERE entry = @REF_SHA_OF_FEAR_NORMAL;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SHA_OF_FEAR_NORMAL, 89236, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Protector */,
(@REF_SHA_OF_FEAR_NORMAL, 89234, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Vanquisher */,
(@REF_SHA_OF_FEAR_NORMAL, 89235, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Conqueror */,
(@REF_SHA_OF_FEAR_NORMAL, 86386, 0, 1, 1, 1, 1) /* 3Shin'ka, Execution of Dominion */,
(@REF_SHA_OF_FEAR_NORMAL, 86388, 0, 1, 1, 1, 1) /* 3Essence of Terror */,
(@REF_SHA_OF_FEAR_NORMAL, 86387, 0, 1, 1, 1, 1) /* 3Kilrak, Jaws of Terror */,
(@REF_SHA_OF_FEAR_NORMAL, 89886, 0, 1, 1, 1, 1) /* 3Wrap of Instant Petrification */,
(@REF_SHA_OF_FEAR_NORMAL, 89887, 0, 1, 1, 1, 1) /* 3Robes of Pinioned Eyes */,
(@REF_SHA_OF_FEAR_NORMAL, 89839, 0, 1, 1, 1, 1) /* 3Shadowgrip Girdle */,
(@REF_SHA_OF_FEAR_NORMAL, 86389, 0, 1, 1, 1, 1) /* 3Dreadwoven Leggings of Failure */;

-- Heroic mode
SET @REF_SHA_OF_FEAR_HEROIC = 80999;
DELETE FROM `reference_loot_template` WHERE entry = @REF_SHA_OF_FEAR_HEROIC;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SHA_OF_FEAR_HEROIC, 89260, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Protector */,
(@REF_SHA_OF_FEAR_HEROIC, 89258, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Vanquisher */,
(@REF_SHA_OF_FEAR_HEROIC, 89259, 0, 1, 1, 1, 1) /* 3Helm of the Shadowy Conqueror */,
(@REF_SHA_OF_FEAR_HEROIC, 87176, 0, 1, 1, 1, 1) /* 3Shin'ka, Execution of Dominion */,
(@REF_SHA_OF_FEAR_HEROIC, 87175, 0, 1, 1, 1, 1) /* 3Essence of Terror */,
(@REF_SHA_OF_FEAR_HEROIC, 87173, 0, 1, 1, 1, 1) /* 3Kilrak, Jaws of Terror */,
(@REF_SHA_OF_FEAR_HEROIC, 89950, 0, 1, 1, 1, 1) /* 3Wrap of Instant Petrification */,
(@REF_SHA_OF_FEAR_HEROIC, 89949, 0, 1, 1, 1, 1) /* 3Robes of Pinioned Eyes */,
(@REF_SHA_OF_FEAR_HEROIC, 89951, 0, 1, 1, 1, 1) /* 3Shadowgrip Girdle */,
(@REF_SHA_OF_FEAR_HEROIC, 87174, 0, 1, 1, 1, 1) /* 3Dreadwoven Leggings of Failure */;

-- Mode 10 Normal
SET @10NM_SHA_OF_FEAR = 60999;
UPDATE `creature_template` SET `lootid`= @10NM_SHA_OF_FEAR WHERE `entry`= @10NM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @10NM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10NM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_NORMAL, 2) /* REF 1*/;

-- Mode 25 Normal
SET @25NM_SHA_OF_FEAR = 81999;
UPDATE `creature_template` SET `lootid`= @25NM_SHA_OF_FEAR WHERE `entry`= @25NM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @25NM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25NM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_NORMAL, 2) /* REF 1*/;

-- Mode 10 Heroic
SET @10HM_SHA_OF_FEAR = 80999;
UPDATE `creature_template` SET `lootid`= @10HM_SHA_OF_FEAR WHERE `entry`= @10HM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @10HM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10HM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_HEROIC, 5) /* REF 1*/;

-- Mode 25 Heroic
SET @25HM_SHA_OF_FEAR = 82999;
UPDATE `creature_template` SET `lootid`= @25HM_SHA_OF_FEAR WHERE `entry`= @25HM_SHA_OF_FEAR;
DELETE FROM `creature_loot_template` WHERE `entry`= @25HM_SHA_OF_FEAR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25HM_SHA_OF_FEAR, 1, 100, 1, 0, -@REF_SHA_OF_FEAR_HEROIC, 5) /* REF 1*/;

-- Currencies
DELETE FROM creature_loot_currency WHERE creature_id IN (@10NM_SHA_OF_FEAR, @25NM_SHA_OF_FEAR, @10HM_SHA_OF_FEAR, @25HM_SHA_OF_FEAR);
REPLACE INTO creature_loot_currency (`creature_id`, `CurrencyId1`, `CurrencyCount1`) VALUES
(@10NM_SHA_OF_FEAR, 396, 4000),
(@25NM_SHA_OF_FEAR, 396, 4000),
(@10HM_SHA_OF_FEAR, 396, 4000),
(@25HM_SHA_OF_FEAR, 396, 4000); 