-- Dissonance Field / Need to keep the old values to avoid they despawn too soon
UPDATE creature_template SET Health_mod = 60 WHERE entry IN (62847, 462847, 762847);
UPDATE creature_template SET Health_mod = 70 WHERE entry in (562847, 662847);