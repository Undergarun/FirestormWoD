UPDATE creature SET spawntimesecs = 2419200 WHERE map = 1205;
UPDATE creature SET spawntimesecs = 120 WHERE id = 76865; -- Beastlord Darmac, for tests

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0 WHERE entry = 76796;
UPDATE creature_template SET dmg_multiplier = 2, unit_flags = unit_flags & ~0x00200000 WHERE entry = 77128;