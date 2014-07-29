-- Wind Lord Mel'jarak
UPDATE creature_template SET
baseattacktime = 2000
WHERE entry = 62397;

-- Corrosive Resin Pool
UPDATE creature_template SET
minlevel = 90, MaxLevel = 90,
faction_A = 14, faction_H = 14,
baseattacktime = 2000, rangeattacktime = 2000,
unit_flags = 33555200, unit_flags2 = 2048
WHERE entry = 67046;

-- Amber Prison
UPDATE creature_template SET
minlevel = 93, maxlevel = 93, unit_flags2 = 2048,
baseattacktime = 2000, rangeattacktime = 2000
WHERE entry = 62531;

-- Kor'thik Elite Blademaster / Sra'thik Amber-Trapper / Zar'thik Battle-Mender
UPDATE creature_template SET
baseattacktime = 2000, rangeattacktime = 2000,
unit_flags2 = 2099200
WHERE entry IN (62402, 62405, 62408);