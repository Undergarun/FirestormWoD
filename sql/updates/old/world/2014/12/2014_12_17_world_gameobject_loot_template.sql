/* Herbals nodes */


-- Fireweed
Delete from gameobject_loot_template where entry = 52249;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52249, 109125, 65, 1, 0, 1, 15),
    (52249, 116053, 20, 1, 0, 1, 3),
    (52249, 109625, 5, 1, 0, 1, 9),
    (52249, 120945, 0.3, 1, 0, 1, 1),
    (52249, 111350, 0.1, 1, 0, 1, 1);

-- Frostweed
Delete from gameobject_loot_template where entry = 233117;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (233117, 109124, 65, 1, 0, 1, 15),
    (233117, 116053, 20, 1, 0, 1, 3),
    (233117, 109624, 5, 1, 0, 1, 9),
    (233117, 120945, 0.3, 1, 0, 1, 1),
    (233117, 111350, 0.1, 1, 0, 1, 1);

Delete from gameobject_loot_template where entry = 52208;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52208, 109124, 65, 1, 0, 1, 15),
    (52208, 116053, 20, 1, 0, 1, 3),
    (52208, 109624, 5, 1, 0, 1, 9),
    (52208, 120945, 0.3, 1, 0, 1, 1),
    (52208, 111350, 0.1, 1, 0, 1, 1);

-- Gorgrond flytrap
Delete from gameobject_loot_template where entry = 52268;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52268, 109126, 65, 1, 0, 1, 15),
    (52268, 116053, 20, 1, 0, 1, 3),
    (52268, 109624, 5, 1, 0, 1, 9),
    (52268, 109626, 0.3, 1, 0, 1, 7);


-- Nagrand Arrowbloom
Delete from gameobject_loot_template where entry = 52306;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52306, 109128, 65, 1, 0, 1, 15),
    (52306, 116053, 20, 1, 0, 1, 3),
    (52306, 109628, 5, 1, 0, 1, 9),
    (52306, 109626, 0.3, 1, 0, 1, 7);

-- Starflower
Delete from gameobject_loot_template where entry = 52287;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52287, 109127, 65, 1, 0, 1, 15),
    (52287, 116053, 20, 1, 0, 1, 3),
    (52287, 109627, 5, 1, 0, 1, 9),
    (52287, 120945, 0.3, 1, 0, 1, 1),
    (52287, 111350, 0.1, 1, 0, 1, 1);

-- Talador Orchid
Delete from gameobject_loot_template where entry = 52327;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`)
VALUES
    (52327, 109129, 65, 1, 0, 1, 15),
    (52327, 116053, 20, 1, 0, 1, 3),
    (52327, 109629, 5, 1, 0, 1, 9),
    (52327, 120945, 0.3, 1, 0, 1, 1),
    (52327, 111350, 0.1, 1, 0, 1, 1);



