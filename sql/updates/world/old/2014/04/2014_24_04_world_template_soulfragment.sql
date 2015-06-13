UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
baseattacktime = 1500,
rangeattacktime = 2000,
unit_flags = 32832,
unit_flags2 = 2099200,
Health_mod = 6.48,
ScriptName = 'mob_soul_fragment'
WHERE entry = 60781;

UPDATE creature_template SET Health_mod = 7.2 WHERE entry IN (60781, 77083);
UPDATE creature_template SET Health_mod = 9.6 WHERE entry = 77094;