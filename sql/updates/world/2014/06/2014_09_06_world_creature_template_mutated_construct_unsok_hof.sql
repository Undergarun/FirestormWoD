-- Mutated Construct
UPDATE creature_template SET
faction_A = 16, faction_H = 16,
minlevel = 92, maxlevel = 92,
unit_flags = 32768, unit_flags2 = 2097152,
baseattacktime = 2000, rangeattacktime = 2000,
Health_mod = 3, ScriptName = "mob_mutated_construct"
WHERE entry = 62701;