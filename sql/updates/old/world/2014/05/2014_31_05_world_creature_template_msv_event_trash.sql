-- Trash mob before Spirit Kings event - Other difficulty modes
UPDATE creature_template SET faction_A = 14, faction_H = 14 WHERE entry IN (77013, 77014, 77015, 77016, 77057, 77058, 77059, 77060, 77062, 77063, 77100, 77101, 77102, 77103, 77104, 77105, 77106, 77107, 77017, 77018, 77019, 77020, 77061, 77064);

-- Adding missing templates
UPDATE creature_template SET difficulty_entry_4 = 77017, difficulty_entry_5 = 77061 WHERE entry = 61421;
UPDATE creature_template SET difficulty_entry_4 = 77018 WHERE entry = 61423;
UPDATE creature_template SET difficulty_entry_4 = 77019 WHERE entry = 61427;
UPDATE creature_template SET difficulty_entry_4 = 77020, difficulty_entry_5 = 77064 WHERE entry = 61429;