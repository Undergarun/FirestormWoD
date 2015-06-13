-- Set'thik Windblade
UPDATE creature_template SET
Health_mod = 15,
minlevel = 92, maxlevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2097152,
baseattacktime = 1500, rangeattacktime = 1500,
mindmg = 4750, maxdmg = 11118,
attackpower = 55538, dmg_multiplier = 2,
ScriptName = "mob_add_setthik_windblade"
WHERE entry = 63589;

-- Kor'thik Reaver
UPDATE creature_template SET
Health_mod = 100,
minlevel = 92, maxlevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2097152,
baseattacktime = 1500, rangeattacktime = 1500,
mindmg = 4750, maxdmg = 11118,
attackpower = 55538, dmg_multiplier = 2,
ScriptName = "mob_korthik_reaver"
WHERE entry = 63591;

-- Sticky Resin
UPDATE creature_template SET
Health_mod = 1,
minlevel = 90, maxlevel = 90,
faction_A = 14, faction_H = 14,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
ScriptName = "mob_sticky_resin"
WHERE entry = 63730;
