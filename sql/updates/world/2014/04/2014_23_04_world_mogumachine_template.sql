-- Ancient mogu machine (Will of emperor)
UPDATE creature_template SET
minlevel = 90,
maxlevel = 90,
faction_A = 14,
faction_H = 14,
baseattacktime = 1500,
rangeattacktime = 2000,
unit_flags = 32768,
unit_flags2 = 2099200
WHERE entry = 60648;