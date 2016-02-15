DELETE FROM instance_encounters WHERE entry IN (
219,
220,
221,
222,
224,
225,
226,
276,
277,
278,
279,
280,
430,
431,
432,
433,
434,
435,
436,
437,
439,
440,
441,
443,
445,
1522,
1530);

DELETE FROM instance_encounters WHERE entry IN (1407, 1409);

UPDATE instance_encounters SET creditentry = 58124 WHERE entry IN (296, 300);
UPDATE instance_encounters SET credittype = 0, creditentry = 35119 WHERE entry IN (338, 339);
UPDATE instance_encounters SET credittype = 0, creditentry = 28234 WHERE entry IN (567, 568);
UPDATE instance_encounters SET credittype = 0, creditentry = 68065 WHERE entry = 1578;
UPDATE instance_encounters SET lastEncounterDungeon = 0 WHERE entry = 447;
UPDATE instance_encounters SET lastEncounterDungeon = 0 WHERE entry = 448;
UPDATE instance_encounters SET lastEncounterDungeon = 0 WHERE entry = 1422;