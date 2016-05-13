-- Dissonance field
UPDATE creature_template SET
minlevel = 90, maxlevel = 90,
Health_mod = 0.0001423, mana_mod = 0.00605082,
faction_A = 16, faction_H = 16,
unit_flags2 = 4992, vehicleId = 2304,
baseattacktime = 2000, rangeattacktime = 2000,
ScriptName = "mob_dissonance_field"
WHERE entry = 62847;