-- Nest Guardian
UPDATE creature_template SET
minlevel = 92, maxlevel = 92,
faction_A = 14, faction_H = 14,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 15000, maxdmg = 30000,
Health_mod = 6, ScriptName = "mob_nest_guardian"
WHERE entry = 70134;