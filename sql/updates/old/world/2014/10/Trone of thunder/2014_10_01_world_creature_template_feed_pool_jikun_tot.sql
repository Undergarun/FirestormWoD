-- Feed pool
UPDATE creature_template SET
minlevel = 92, maxlevel = 92,
faction_A = 14, faction_H = 14,
unit_flags = 256, unit_flags2 = 34816,
baseattacktime = 2000, rangeattacktime = 2000,
modelid1 = 11686, modelid2 = 11686,
ScriptName = "mob_feed_pool"
WHERE entry = 68188;