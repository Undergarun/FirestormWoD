-- Cancel previous changes of health_mod (differences came from Ritual of Purification)
UPDATE creature_template SET Health_mod = 381.15  WHERE entry = 60999;
UPDATE creature_template SET Health_mod = 1122.66 WHERE entry = 80999;
UPDATE creature_template SET Health_mod = 1143.45 WHERE entry = 81999;
UPDATE creature_template SET Health_mod = 3367.98 WHERE entry = 82999;