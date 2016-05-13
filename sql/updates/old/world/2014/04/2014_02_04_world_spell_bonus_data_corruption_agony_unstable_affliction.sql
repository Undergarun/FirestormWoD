DELETE FROM spell_bonus_data WHERE entry IN (172, 146739); -- Deprecated
INSERT INTO spell_bonus_data VALUE (146739, 0, 0.165, 0, 0, 'Warlock - Corruption (triggered)');
UPDATE spell_bonus_data SET dot_bonus = 0.306 WHERE entry = 980;
UPDATE spell_bonus_data SET dot_bonus = 0.29 WHERE entry = 30108;