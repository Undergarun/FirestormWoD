SET @REF_BRACKENSPORE_N = 77404;
SET @REF_BRACKENSPORE_H = 1577404;
SET @REF_BRACKENSPORE_M = 1677404;
SET @REF_BRACKENSPORE_LFR = 1777404;

DELETE FROM `reference_loot_template` WHERE entry = @REF_BRACKENSPORE_N;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_N, 113652, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113653, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113654, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113655, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113656, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113657, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113658, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113659, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113660, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113661, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113662, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_N, 113664, 0, 1, 1, 1, 1);

DELETE FROM `reference_loot_template` WHERE entry = @REF_BRACKENSPORE_LFR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_LFR, 115999, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116233, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116294, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116028, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116208, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116291, 0, 1, 1, 1, 1),
(@REF_BRACKENSPORE_LFR, 116288, 0, 1, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_BRACKENSPORE_N WHERE `entry`= @REF_BRACKENSPORE_N;
UPDATE `creature_template` SET `lootid`= @REF_BRACKENSPORE_N WHERE `entry`= @REF_BRACKENSPORE_H;
UPDATE `creature_template` SET `lootid`= @REF_BRACKENSPORE_N WHERE `entry`= @REF_BRACKENSPORE_M;
UPDATE `creature_template` SET `lootid`= @REF_BRACKENSPORE_LFR WHERE `entry`= @REF_BRACKENSPORE_LFR;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BRACKENSPORE_N;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_N, 1, 100, 1, 0, -@REF_BRACKENSPORE_N, 6);

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_BRACKENSPORE_LFR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_BRACKENSPORE_LFR, 1, 100, 1, 0, -@REF_BRACKENSPORE_LFR, 6);

UPDATE creature_template SET
minlevel = 103,
maxlevel = 103,
faction = 14,
dmg_multiplier = 10,
ScriptName = 'boss_brackenspore',
mechanic_immune_mask = 617299839,
flags_extra = 0x01
WHERE entry = 78491;

DELETE FROM spell_script_names WHERE spell_id IN (159220);
INSERT INTO spell_script_names VALUES
(159220, 'spell_highmaul_necrotic_breath');