/* Mining nodes */

-- blackrock deposit
Delete from gameobject_loot_template where entry = 55500;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (55500, 109118, 65, 1, 0, 2, 15),
    (55500, 115508, 20, 1, 0, 1, 3),
    (55500, 120945, 5, 1, 0, 1, 1),
    (55500, 109992, 0.3, 1, 0, 1, 7),
    (55500, 111349, 0.1, 1, 0, 1, 1);

Delete from gameobject_loot_template where entry = 52188;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52188, 109118, 65, 1, 0, 2, 15),
    (52188, 115508, 20, 1, 0, 1, 3),
    (52188, 120945, 5, 1, 0, 1, 1),
    (52188, 109992, 0.3, 1, 0, 1, 7),
    (52188, 111349, 0.1, 1, 0, 1, 1);

-- Rich blackrock deposit
Delete from gameobject_loot_template where entry = 55519;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (55519, 109118, 65, 1, 0, 2, 15),
    (55519, 115508, 20, 1, 0, 1, 5),
    (55519, 120945, 5, 1, 0, 1, 1),
    (55519, 109992, 0.3, 1, 0, 3, 9),
    (55519, 111349, 0.1, 1, 0, 1, 1);

Delete from gameobject_loot_template where entry = 52189;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52189, 109118, 65, 1, 0, 2, 15),
    (52189, 115508, 20, 1, 0, 1, 5),
    (52189, 120945, 5, 1, 0, 1, 1),
    (52189, 109992, 0.3, 1, 0, 3, 9),
    (52189, 111349, 0.1, 1, 0, 1, 1);


-- Rich true iron deposit
Delete from gameobject_loot_template where entry = 55524;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (55524, 109119, 65, 1, 0, 2, 20),
    (55524, 115508, 20, 1, 0, 1, 5),
    (55524, 120945, 5, 1, 0, 1, 1),
    (55524, 109991, 0.3, 1, 0, 3, 9),
    (55524, 111349, 0.1, 1, 0, 1, 1);

Delete from gameobject_loot_template where entry = 228510;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (228510, 109119, 65, 1, 0, 2, 20),
    (228510, 115508, 20, 1, 0, 1, 5),
    (228510, 120945, 5, 1, 0, 1, 1),
    (228510, 109991, 0.3, 1, 0, 3, 9),
    (228510, 111349, 0.1, 1, 0, 1, 1);

-- true iron deposit
Delete from gameobject_loot_template where entry = 55521;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (55521, 109119, 65, 1, 0, 2, 8),
    (55521, 115508, 20, 1, 0, 1, 3),
    (55521, 120945, 5, 1, 0, 1, 1),
    (55521, 109991, 0.3, 1, 0, 1, 9),
    (55521, 111349, 0.1, 1, 0, 1, 1);

Delete from gameobject_loot_template where entry = 52185;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52185, 109119, 65, 1, 0, 2, 8),
    (52185, 115508, 20, 1, 0, 1, 3),
    (52185, 120945, 5, 1, 0, 1, 1),
    (52185, 109991, 0.3, 1, 0, 1, 9),
    (52185, 111349, 0.1, 1, 0, 1, 1);

