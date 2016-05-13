-- Fixing heroic template for soul fragment
UPDATE creature_template SET difficulty_entry_5 = 0 WHERE entry = 60781;
UPDATE creature_template SET faction_A = 14, faction_H = 14, minlevel = 93, maxlevel = 93, Health_mod = 1000 WHERE entry = 60627;