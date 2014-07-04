-- Sha of fear
UPDATE creature_template SET
minlevel = 85, maxlevel = 85,
Health_mod = 9, Mana_mod = 7,
faction_A = 16, faction_H = 16,
unit_flags = 33554496, unit_flags2 = 2099200,
baseattacktime = 5000, rangeattacktime = 5000,
ScriptName = "mob_sha_of_fear"
WHERE entry = 63942;