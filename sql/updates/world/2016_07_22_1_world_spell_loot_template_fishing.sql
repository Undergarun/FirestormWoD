SELECT * FROM spell_loot_template WHERE entry IN (161283);




UPDATE spell_loot_template SET ChanceOrQuestChance = 100, groupid = 0,  mincountOrRef =20, maxcount = 35 WHERE entry = 161277; -- Enormous see Scorpion

UPDATE spell_loot_template SET maxcount = 10 WHERE entry = 161231;		-- Small Blind Lake Sturgeon
DELETE FROM spell_loot_template WHERE entry = 161231 AND item = 109139;

UPDATE spell_loot_template SET maxcount = 10 WHERE entry = 161232;	-- Small Fat Sleeper
UPDATE spell_loot_template SET ChanceOrQuestChance = 100, groupid = 0,  mincountOrRef =10, maxcount = 25 WHERE entry = 161273; -- Fat Sleeper
UPDATE spell_loot_template SET ChanceOrQuestChance = 100, groupid = 0,  mincountOrRef =20, maxcount = 35 WHERE entry = 161283; -- Enormous Fat Sleeper