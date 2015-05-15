
-- Fire Ammonite School
Delete from gameobject_loot_template where entry = 52518;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52518, 111673, 25, 1, 0, 1, 1),
    (52518, 110291, 25, 1, 0, 1, 1),
    (52518, 116748, -100, 1, 0, 1, 1),
    (52518, 111656, 25, 1, 0, 1, 1);

-- Lagoon Pool
-- template not exist
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`)
VALUES
    (226967, 25, 11049, 'Lagoon Pool', '', '', '', 0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 226967, 3, 5, 1628, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19116);

Delete from gameobject_loot_template where entry = 226967;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (226967, 108738, 100, 1, 0, 1, 1);


-- Jawless Skulker School
Delete from gameobject_loot_template where entry = 52515;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52515, 111676, 25, 1, 0, 1, 1),
    (52515, 111669, 25, 1, 0, 1, 1),
    (52515, 116752, -100, 1, 0, 1, 1),
    (52515, 111650, 25, 1, 0, 1, 1);

-- Fat Sleeper School
Delete from gameobject_loot_template where entry = 52516;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52516, 111675, 25, 1, 0, 1, 1),
    (52516, 111668, 25, 1, 0, 1, 1),
    (52516, 116753, -100, 1, 0, 1, 1),
    (52516, 111651, 25, 1, 0, 1, 1);

-- Sea Scorpion School
Delete from gameobject_loot_template where entry = 52519;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52519, 111672, 25, 1, 0, 1, 1),
    (52519, 111665, 25, 1, 0, 1, 1),
    (52519, 111658, 25, 1, 0, 1, 1);

-- Blind Lake Sturgeon School
Delete from gameobject_loot_template where entry = 52517;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52517, 111667, 25, 1, 0, 1, 1),
    (52517, 111674, 25, 1, 0, 1, 1),
    (52517, 116750, -100, 1, 0, 1, 1),
    (52517, 111652, 25, 1, 0, 1, 1);

-- Abyssal Gulper School
Delete from gameobject_loot_template where entry = 52520;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52520, 111664, 25, 1, 0, 1, 1),
    (52520, 111671, 25, 1, 0, 1, 1),
    (52520, 116751, -100, 1, 0, 1, 1),
    (52520, 111659, 25, 1, 0, 1, 1);

-- Blackwater Whiptail School
Delete from gameobject_loot_template where entry = 56672;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (56672, 118046, 25, 1, 0, 1, 1),
    (56672, 111672, 25, 1, 0, 1, 1),
    (56672, 111658, 25, 1, 0, 1, 1),
    (56672, 111665, 25, 1, 0, 1, 1);

-- fish by area
-- Talador
Delete from fishing_loot_template where entry = 6662;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (6662, 111670, 37, 1, 1, 1, 1),
    (6662, 111601, 17, 1, 1, 1, 1),
    (6662, 111663, 16, 1, 1, 1, 1),
    (6662, 111595, 7, 1, 1, 1, 1),
    (6662, 111662, 7, 1, 1, 1, 1),
    (6662, 110294, 4, 1, 1, 1, 1),
    (6662, 111672, 1.3, 1, 1, 1, 1),
    (6662, 111589, 1.3, 1, 1, 1, 1),
    (6662, 111665, 1.3, 1, 1, 1, 1),
    (6662, 118414, 1.3, 1, 1, 1, 48),
    (6662, 110508, 1.3, 1, 1, 1, 1),
    (6662, 6529, 1.3, 1, 1, 1, 1),
    (6662, 118391, 1.3, 1, 1, 1, 1),
    (6662, 34861, 1.3, 1, 1, 1, 1),
    (6662, 111658, 0.3, 1, 1, 1, 1),
    (6662, 6367, 0.3, 1, 1, 1, 1),
    (6662, 6256, 0.3, 1, 1, 1, 1),
    (6662, 6365, 0.3, 1, 1, 1, 1),
    (6662, 116749, 0.3, 1, 1, 1, 1),
    (6662, 118415, 0.3, 1, 1, 1, 1),
    (6662, 110292, 0.3, 1, 1, 1, 1),
    (6662, 112623, 0.1, 1, 1, 1, 1),
    (6662, 118393, 0.1, 1, 1, 1, 1),
    (6662, 116817, 0.1, 1, 1, 1, 1),
    (6662, 111356, 0.1, 1, 1, 1, 1),
    (6662, 111675, 0.1, 1, 1, 1, 1),
    (6662, 111668, 0.1, 1, 1, 1, 1);

-- FrostFire
Delete from fishing_loot_template where entry = 6720;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (6720, 111595, 24, 1, 1, 1, 1),
    (6720, 111589, 16, 1, 1, 1, 1),
    (6720, 111601, 9, 1, 1, 1, 1),
    (6720, 111664, 5, 1, 1, 1, 1),
    (6720, 111666, 4, 1, 1, 1, 1),
    (6720, 111663, 3, 1, 1, 1, 1),
    (6720, 111667, 3, 1, 1, 1, 1),
    (6720, 111668, 3, 1, 1, 1, 1),
    (6720, 111669, 3, 1, 1, 1, 1),
    (6720, 111665, 3, 1, 1, 1, 1),
    (6720, 111656, 3, 1, 1, 1, 1),
    (6720, 114625, 3, 1, 1, 1, 1),
    (6720, 111656, 3, 1, 1, 1, 1),
    (6720, 114625, 3, 1, 1, 1, 1),
    (6720, 111671, 3, 1, 1, 1, 1),
    (6720, 111673, 3, 1, 1, 1, 1),
    (6720, 111659, 0.3, 1, 1, 1, 1),
    (6720, 111650, 0.3, 1, 1, 1, 1),
    (6720, 111652, 0.3, 1, 1, 1, 1),
    (6720, 111651, 0.3, 1, 1, 1, 1),
    (6720, 111672, 0.3, 1, 1, 1, 1),
    (6720, 111662, 0.3, 1, 1, 1, 1),
    (6720, 111658, 0.3, 1, 1, 1, 1),
    (6720, 111670, 0.3, 1, 1, 1, 1),
    (6720, 111674, 0.3, 1, 1, 1, 1),
    (6720, 111676, 0.3, 1, 1, 1, 1),
    (6720, 110293, 0.3, 1, 1, 1, 1),
    (6720, 111675, 0.3, 1, 1, 1, 1),
    (6720, 110291, 0.3, 1, 1, 1, 1),
    (6720, 110508, 0.3, 1, 1, 1, 1),
    (6720, 118414, 0.3, 1, 1, 1, 1),
    (6720, 110294, 0.3, 1, 1, 1, 1),
    (6720, 110290, 0.3, 1, 1, 1, 1),
    (6720, 110289, 0.3, 1, 1, 1, 1),
    (6720, 110274, 0.3, 1, 1, 1, 1),
    (6720, 34861, 0.3, 1, 1, 1, 1),
    (6720, 6529, 0.3, 1, 1, 1, 1),
    (6720, 118391, 0.3, 1, 1, 1, 1),
    (6720, 108738, 0.3, 1, 1, 1, 1),
    (6720, 110292, 0.3, 1, 1, 1, 1),
    (6720, 118424, 0.3, 1, 1, 1, 1),
    (6720, 118415, 0.3, 1, 1, 1, 1),
    (6720, 6365, 0.3, 1, 1, 1, 1),
    (6720, 6256, 0.3, 1, 1, 1, 1),
    (6720, 6367, 0.3, 1, 1, 1, 1),
    (6720, 118414, 1.3, 1, 1, 1, 48),
    (6720, 116819, 0.1, 1, 1, 1, 1);

-- Shadowmoon Valley
Delete from fishing_loot_template where entry = 6719;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (6719, 111670, 37, 1, 1, 1, 1),
    (6719, 111601, 17, 1, 1, 1, 1),
    (6719, 111663, 16, 1, 1, 1, 1),
    (6719, 111595, 7, 1, 1, 1, 1),
    (6719, 111662, 7, 1, 1, 1, 1),
    (6719, 110294, 4, 1, 1, 1, 1),
    (6719, 111672, 1.3, 1, 1, 1, 1),
    (6719, 111589, 1.3, 1, 1, 1, 1),
    (6719, 111665, 1.3, 1, 1, 1, 1),
    (6719, 118414, 1.3, 1, 1, 1, 48),
    (6719, 110508, 1.3, 1, 1, 1, 1),
    (6719, 6529, 1.3, 1, 1, 1, 1),
    (6719, 118391, 1.3, 1, 1, 1, 1),
    (6719, 34861, 1.3, 1, 1, 1, 1),
    (6719, 111658, 0.3, 1, 1, 1, 1),
    (6719, 6367, 0.3, 1, 1, 1, 1),
    (6719, 6256, 0.3, 1, 1, 1, 1),
    (6719, 6365, 0.3, 1, 1, 1, 1),
    (6719, 116749, 0.3, 1, 1, 1, 1),
    (6719, 118415, 0.3, 1, 1, 1, 1),
    (6719, 110292, 0.3, 1, 1, 1, 1),
    (6719, 112623, 0.1, 1, 1, 1, 1),
    (6719, 118393, 0.1, 1, 1, 1, 1),
    (6719, 116817, 0.1, 1, 1, 1, 1),
    (6719, 111356, 0.1, 1, 1, 1, 1),
    (6719, 118381, 0.1, 1, 1, 1, 1),
    (6719, 116750, 0.1, 1, 1, 1, 1),
    (6719, 116820, 0.1, 1, 1, 1, 1),
    (6719, 118041, 0.1, 1, 1, 1, 1);


-- Gorgrond
Delete from fishing_loot_template where entry = 6721;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (6721, 111670, 37, 1, 1, 1, 1),
    (6721, 111601, 17, 1, 1, 1, 1),
    (6721, 111663, 16, 1, 1, 1, 1),
    (6721, 111595, 7, 1, 1, 1, 1),
    (6721, 111662, 7, 1, 1, 1, 1),
    (6721, 110294, 4, 1, 1, 1, 1),
    (6721, 111672, 1.3, 1, 1, 1, 1),
    (6721, 111589, 1.3, 1, 1, 1, 1),
    (6721, 111665, 1.3, 1, 1, 1, 1),
    (6721, 118414, 1.3, 1, 1, 1, 48),
    (6721, 110508, 1.3, 1, 1, 1, 1),
    (6721, 6529, 1.3, 1, 1, 1, 1),
    (6721, 118391, 1.3, 1, 1, 1, 1),
    (6721, 34861, 1.3, 1, 1, 1, 1),
    (6721, 111658, 0.3, 1, 1, 1, 1),
    (6721, 6367, 0.3, 1, 1, 1, 1),
    (6721, 6256, 0.3, 1, 1, 1, 1),
    (6721, 6365, 0.3, 1, 1, 1, 1),
    (6721, 116749, 0.3, 1, 1, 1, 1),
    (6721, 118415, 0.3, 1, 1, 1, 1),
    (6721, 110292, 0.3, 1, 1, 1, 1),
    (6721, 118415, 0.3, 1, 1, 1, 1),
    (6721, 118380, 0.3, 1, 1, 1, 1),
    (6721, 116822, 0.3, 1, 1, 1, 1);

-- Spires of Arak
Delete from fishing_loot_template where entry = 6722;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (6722, 111670, 37, 1, 1, 1, 1),
    (6722, 111601, 17, 1, 1, 1, 1),
    (6722, 111663, 16, 1, 1, 1, 1),
    (6722, 111595, 7, 1, 1, 1, 1),
    (6722, 111662, 7, 1, 1, 1, 1),
    (6722, 110294, 4, 1, 1, 1, 1),
    (6722, 111672, 1.3, 1, 1, 1, 1),
    (6722, 111589, 1.3, 1, 1, 1, 1),
    (6722, 111665, 1.3, 1, 1, 1, 1),
    (6722, 118414, 1.3, 1, 1, 1, 48),
    (6722, 110508, 1.3, 1, 1, 1, 1),
    (6722, 6529, 1.3, 1, 1, 1, 1),
    (6722, 118391, 1.3, 1, 1, 1, 1),
    (6722, 34861, 1.3, 1, 1, 1, 1),
    (6722, 111658, 0.3, 1, 1, 1, 1),
    (6722, 6367, 0.3, 1, 1, 1, 1),
    (6722, 6256, 0.3, 1, 1, 1, 1),
    (6722, 6365, 0.3, 1, 1, 1, 1),
    (6722, 116749, 0.3, 1, 1, 1, 1),
    (6722, 118415, 0.3, 1, 1, 1, 1),
    (6722, 110292, 0.3, 1, 1, 1, 1),
    (6722, 118415, 0.1, 1, 1, 1, 1),
    (6722, 118046, 0.1, 1, 1, 1, 1),
    (6722, 116818, 0.1, 1, 1, 1, 1),
    (6722, 111663, 0.1, 1, 1, 1, 1),
    (6722, 111670, 0.1, 1, 1, 1, 1),
    (6722, 113426, 0.1, 1, 1, 1, 1),
    (6722, 111662, 0.1, 1, 1, 1, 1);


-- Nagrand
Delete from fishing_loot_template where entry = 6755;
INSERT INTO `fishing_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (6755, 111670, 37, 1, 1, 1, 1),
    (6755, 111601, 17, 1, 1, 1, 1),
    (6755, 111663, 16, 1, 1, 1, 1),
    (6755, 111595, 7, 1, 1, 1, 1),
    (6755, 111662, 7, 1, 1, 1, 1),
    (6755, 110294, 4, 1, 1, 1, 1),
    (6755, 111672, 1.3, 1, 1, 1, 1),
    (6755, 111589, 1.3, 1, 1, 1, 1),
    (6755, 111665, 1.3, 1, 1, 1, 1),
    (6755, 118414, 1.3, 1, 1, 1, 48),
    (6755, 110508, 1.3, 1, 1, 1, 1),
    (6755, 6529, 1.3, 1, 1, 1, 1),
    (6755, 118391, 1.3, 1, 1, 1, 1),
    (6755, 34861, 1.3, 1, 1, 1, 1),
    (6755, 111658, 0.3, 1, 1, 1, 1),
    (6755, 6367, 0.3, 1, 1, 1, 1),
    (6755, 6256, 0.3, 1, 1, 1, 1),
    (6755, 6365, 0.3, 1, 1, 1, 1),
    (6755, 116749, 0.3, 1, 1, 1, 1),
    (6755, 118415, 0.3, 1, 1, 1, 1),
    (6755, 110292, 0.3, 1, 1, 1, 1),
    (6755, 113426, 0.1, 1, 1, 1, 1),
    (6755, 111670, 0.1, 1, 1, 1, 1),
    (6755, 116821, 0.1, 1, 1, 1, 1),
    (6755, 110291, 0.1, 1, 1, 1, 1),
    (6755, 118380, 0.1, 1, 1, 1, 1),
    (6755, 111663, 0.1, 1, 1, 1, 1);


REPLACE INTO `skill_fishing_base_level` (`entry`, `skill`)
VALUES
    (1, -70),
    (8, 130),
    (10, 55),
    (11, 55),
    (12, -70),
    (14, -70),
    (15, 130),
    (16, 205),
    (17, -20),
    (28, 205),
    (33, 130),
    (36, 130),
    (38, -20),
    (40, -20),
    (41, 330),
    (44, 55),
    (45, 130),
    (46, 330),
    (47, 205),
    (65, 380),
    (85, -70),
    (130, -20),
    (139, 330),
    (141, -70),
    (148, -20),
    (215, -70),
    (267, 55),
    (297, 205),
    (331, 55),
    (357, 205),
    (361, 205),
    (394, 380),
    (400, 130),
    (405, 130),
    (406, 55),
    (440, 205),
    (490, 205),
    (493, 205),
    (495, 380),
    (618, 330),
    (718, -20),
    (719, -20),
    (796, 130),
    (1112, 330),
    (1222, 330),
    (1227, 330),
    (1377, 330),
    (1417, 205),
    (1497, -20),
    (1519, -20),
    (1537, -20),
    (1581, -20),
    (1637, -20),
    (1638, -20),
    (1657, -20),
    (1977, 330),
    (2017, 330),
    (2057, 330),
    (2100, 205),
    (2817, 405),
    (3140, 330),
    (3430, -70),
    (3433, -20),
    (3483, 280),
    (3518, 380),
    (3519, 355),
    (3520, 280),
    (3521, 305),
    (3523, 380),
    (3524, -70),
    (3525, -20),
    (3537, 380),
    (3607, 305),
    (3614, 395),
    (3621, 395),
    (3625, 280),
    (3653, 305),
    (3656, 305),
    (3658, 305),
    (3690, 405),
    (3691, 405),
    (3692, 405),
    (3711, 435),
    (3805, 330),
    (3859, 405),
    (3979, 485),
    (4080, 355),
    (4197, 435),
    (4395, 435),
    (4813, 455),
    (305, -20),
    (702, -70),
    (3317, 205),
    (456, -20),
    (307, 205),
    (445, -20),
    (448, -20),
    (449, -20),
    (3636, 395),
    (896, 55),
    (295, 55),
    (294, 55),
    (454, -20),
    (2077, -20),
    (385, -20),
    (720, -20),
    (401, -20),
    (392, -20),
    (249, 330),
    (391, -20),
    (382, -20),
    (386, 5),
    (387, 5),
    (388, 5),
    (2717, 330),
    (328, 130),
    (3586, -20),
    (3514, -20),
    (3515, -20),
    (327, 130),
    (3513, -20),
    (414, 55),
    (3508, -20),
    (3472, -70),
    (3470, -70),
    (3558, -70),
    (3482, -70),
    (3473, -70),
    (3460, -70),
    (3911, -70),
    (4058, 380),
    (4479, 380),
    (4646, 380),
    (210, 455),
    (67, 455),
    (66, 380),
    (4151, 380),
    (4257, 380),
    (4166, 380),
    (4256, 380),
    (3461, -70),
    (3716, 305),
    (3659, 355),
    (3655, 355),
    (3528, -70),
    (3595, -20),
    (4055, 380),
    (3980, 380),
    (4109, 380),
    (4022, 380),
    (4123, 380),
    (4113, 380),
    (4039, 380),
    (4134, 380),
    (4043, 380),
    (4560, 435),
    (4567, 435),
    (4742, 460),
    (3456, 460),
    (4493, 460),
    (4273, 460),
    (4722, 460),
    (306, -20),
    (4411, -20),
    (37, 130),
    (300, 130),
    (2403, 130),
    (1477, 205),
    (2, -20),
    (115, -20),
    (298, 55),
    (299, 55),
    (150, 55),
    (1039, 55),
    (374, -70),
    (375, -70),
    (373, -70),
    (368, -70),
    (393, -70),
    (513, 130),
    (518, 130),
    (2079, 130),
    (2318, 130),
    (537, 205),
    (598, 130),
    (2406, 130),
    (2408, 130),
    (636, 55),
    (463, 55),
    (1108, 205),
    (1121, 205),
    (1120, 205),
    (1117, 205),
    (1116, 205),
    (2521, 205),
    (1234, 205),
    (878, 205),
    (3479, -70),
    (2597, 330),
    (4374, 395),
    (214, 55),
    (3455, 250),
    (4258, 430),
    (5034, 430),
    (35, 55),
    (1578, 55),
    (4747, 55),
    (5031, 350),
    (5038, 350),
    (5044, 350),
    (5389, 350),
    (5534, 350),
    (5535, 350),
    (5536, 350),
    (5538, 350),
    (5545, 350),
    (5546, 350),
    (4290, 435),
    (6721, 600),
    (6941, 600),
    (6662, 600),
    (6755, 600),
    (6722, 600),
    (6722, 600);

UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '5' WHERE (`entry`='4395' AND `item`='43646') LIMIT 1;
UPDATE fishing_loot_template set item = 93738 where item = 43650;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '35' WHERE (`entry`='6719' AND `item`='111670') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '35' WHERE (`entry`='6719' AND `item`='111670') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='6662' AND `item`='111670') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='4273' AND `item`='41814') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='4493' AND `item`='6297') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '45' WHERE (`entry`='3456' AND `item`='6456') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='4722' AND `item`='41805') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '45' WHERE (`entry`='1176' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '45' WHERE (`entry`='5396' AND `item`='53062') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '25' WHERE (`entry`='5396' AND `item`='53068') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='1477' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='6138' AND `item`='74857') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '14' WHERE (`entry`='6138' AND `item`='83064') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='3716' AND `item`='27422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='3519' AND `item`='27439') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='3519' AND `item`='27422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '32' WHERE (`entry`='331' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '42' WHERE (`entry`='3483' AND `item`='27441') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '25' WHERE (`entry`='3523' AND `item`='27422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='4197' AND `item`='41809') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='4197' AND `item`='41806') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='4197' AND `item`='41813') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='3518' AND `item`='27422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='3518' AND `item`='27435') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '19' WHERE (`entry`='5034' AND `item`='53069') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='5034' AND `item`='53066') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '14' WHERE (`entry`='5034' AND `item`='53070') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='4987' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='3805' AND `item`='53062') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='1638' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '51' WHERE (`entry`='46' AND `item`='6297') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '12' WHERE (`entry`='440' AND `item`='4603') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='12' AND `item`='6291') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='406' AND `item`='21153') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4709' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='1377' AND `item`='13889') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '18' WHERE (`entry`='1377' AND `item`='13760') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '35' WHERE (`entry`='719' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='5841' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='5841' AND `item`='83064') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '50' WHERE (`entry`='215' AND `item`='6291') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '25' WHERE (`entry`='215' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='616' AND `item`='52325') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='616' AND `item`='53062') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '42' WHERE (`entry`='66' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='357' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='357' AND `item`='13889') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='357' AND `item`='8365') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='361' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '21' WHERE (`entry`='65' AND `item`='40199') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='65' AND `item`='41802') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='65' AND `item`='41807') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '7' WHERE (`entry`='65' AND `item`='41803') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='11' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='11' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='405' AND `item`='6362') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='405' AND `item`='4603') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='405' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '12' WHERE (`entry`='405' AND `item`='6362') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='15' AND `item`='8365') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='17' AND `item`='6522') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '1' WHERE (`entry`='5861' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='5861' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '11' WHERE (`entry`='5861' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='148' AND `item`='6303') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='148' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='148' AND `item`='12238') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '41' WHERE (`entry`='6006' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='490' AND `item`='21153') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='490' AND `item`='13889') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='210' AND `item`='41803') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='210' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='16' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='40' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='139' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='1497' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='6457' AND `item`='6291') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '8' WHERE (`entry`='6457' AND `item`='45190') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='3711' AND `item`='41813') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='2817' AND `item`='41814') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='2817' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='2817' AND `item`='41814') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='6757' AND `item`='74863') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '42' WHERE (`entry`='4080' AND `item`='35285') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '35' WHERE (`entry`='1519' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='3525' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='6040' AND `item`='6362') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='6040' AND `item`='21153') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='6040' AND `item`='8365') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='130' AND `item`='21071') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='5145' AND `item`='53069') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='4' AND `item`='13889') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '35' WHERE (`entry`='5339' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '12' WHERE (`entry`='5339' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='28' AND `item`='8365') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='28' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '55' WHERE (`entry`='141' AND `item`='6303') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '34' WHERE (`entry`='44' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='44' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '55' WHERE (`entry`='4813' AND `item`='43572') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '42' WHERE (`entry`='4813' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='4813' AND `item`='45194') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='4813' AND `item`='3671') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='4813' AND `item`='45200') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='4813' AND `item`='45198') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4813' AND `item`='45190') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4813' AND `item`='4875') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4813' AND `item`='25447') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4813' AND `item`='43572') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4813' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '35' WHERE (`entry`='85' AND `item`='6291') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '15' WHERE (`entry`='85' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '21' WHERE (`entry`='394' AND `item`='40199') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '21' WHERE (`entry`='394' AND `item`='41809') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='394' AND `item`='41802') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='394' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='3433' AND `item`='6303') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='3433' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='3433' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '19' WHERE (`entry`='38' AND `item`='21071') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '19' WHERE (`entry`='38' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='3' AND `item`='4603') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '25' WHERE (`entry`='5144' AND `item`='53069') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='4815' AND `item`='53069') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='1537' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='3537' AND `item`='40199') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='45' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='47' AND `item`='6362') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '01' WHERE (`entry`='47' AND `item`='21153') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='47' AND `item`='8365') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='47' AND `item`='21153') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='493' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='3430' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='3430' AND `item`='6291') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='1657' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='8' AND `item`='13888') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '9' WHERE (`entry`='8' AND `item`='13422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '30' WHERE (`entry`='4706' AND `item`='6361') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='4706' AND `item`='6303') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='3524' AND `item`='6303') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='400' AND `item`='4603') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='400' AND `item`='13422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='495' AND `item`='41808') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='495' AND `item`='40199') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='495' AND `item`='41810') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '20' WHERE (`entry`='10' AND `item`='21071') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='10' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='10' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4922' AND `item`='53069') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='4922' AND `item`='53064') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '40' WHERE (`entry`='41' AND `item`='27422') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='41' AND `item`='33823') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='41' AND `item`='33824') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '20' WHERE (`entry`='4722' AND `item`='41805') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '35' WHERE (`entry`='1176' AND `item`='13758') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '30' WHERE (`entry`='5805' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '20' WHERE (`entry`='5805' AND `item`='74859') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '30' WHERE (`entry`='5842' AND `item`='74860') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '20' WHERE (`entry`='5842' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '30' WHERE (`entry`='6134' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='6134' AND `item`='74864') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='6134' AND `item`='74857') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '5' WHERE (`entry`='6134' AND `item`='74865') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '30' WHERE (`entry`='5840' AND `item`='74863') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '20' WHERE (`entry`='5840' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '20' WHERE (`entry`='267' AND `item`='21071') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='267' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '5' WHERE (`entry`='267' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '35' WHERE (`entry`='5389' AND `item`='53070') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '9' WHERE (`entry`='5389' AND `item`='53069') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '20' WHERE (`entry`='5042' AND `item`='53062') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='5042' AND `item`='53068') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '40' WHERE (`entry`='3607' AND `item`='21153') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '30' WHERE (`entry`='1637' AND `item`='6289') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='1637' AND `item`='6308') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='65' AND `item`='40199') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='65' AND `item`='41802') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '30' WHERE (`entry`='5785' AND `item`='74866') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '15' WHERE (`entry`='5785' AND `item`='74856') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='5785' AND `item`='74864') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='5785' AND `item`='74860') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='5785' AND `item`='74857') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '10' WHERE (`entry`='3537' AND `item`='41806') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '9' WHERE (`entry`='3537' AND `item`='40199') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '9' WHERE (`entry`='3537' AND `item`='45907') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance`= '9' WHERE (`entry`='3537' AND `item`='41802') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '4' WHERE (`entry`='1176' AND `item`='8365') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '4' WHERE (`entry`='1176' AND `item`='6358') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '4' WHERE (`entry`='1176' AND `item`='13759') LIMIT 1;
UPDATE `fishing_loot_template` SET `ChanceOrQuestChance` = '10' WHERE (`entry`='1176' AND `item`='13758') LIMIT 1;
