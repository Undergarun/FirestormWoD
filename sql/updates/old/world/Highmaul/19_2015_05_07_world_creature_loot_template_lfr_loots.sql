DELETE FROM reference_loot_template WHERE entry = 1778491;
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 115999, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 116233, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 116294, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 116028, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 116208, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 116291, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 116288, 0, 1, 1, 1, 1);

DELETE FROM reference_loot_template WHERE entry = 1778714;
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116003, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116030, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116289, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116236, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116205, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116282, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116298, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 116360, 0, 1, 1, 1, 1);

DELETE FROM reference_loot_template WHERE entry = 1777404;
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116026, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116290, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116209, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 115998, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116230, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116297, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116281, 0, 1, 1, 1, 1);
INSERT INTO reference_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 116361, 0, 1, 1, 1, 1);

DELETE FROM creature_loot_template WHERE entry = 1778491;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778491, 1, 100, 1, 0, -1778491, 6);

DELETE FROM creature_loot_template WHERE entry = 1778714;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1778714, 1, 100, 1, 0, -1778714, 6);

DELETE FROM creature_loot_template WHERE entry = 1777404;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUE (1777404, 1, 100, 1, 0, -1777404, 6);

UPDATE creature_template SET lootid = 1778491 WHERE entry = 1778491;
UPDATE creature_template SET lootid = 1778714 WHERE entry = 1778714;
UPDATE creature_template SET lootid = 1777404 WHERE entry = 1777404;