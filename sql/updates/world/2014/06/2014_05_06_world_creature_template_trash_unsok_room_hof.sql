-- Amber Searsting
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 14, faction_H = 14,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Health_mod = 40, ScriptName = "mob_amber_searsting"
WHERE entry = 63569;

-- Amber-Ridden Mushan
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Health_mod = 20, ScriptName = "mob_amberridden_mushan"
WHERE entry = 63568;

-- Coagulated Amber
UPDATE creature_template SET
MinLevel = 90, MaxLevel = 90,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 1000, rangeattacktime = 1000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Health_mod = 3, ScriptName = "mob_coagulated_amber"
WHERE entry IN (63594, 63597);

-- Living Amber
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 1000, rangeattacktime = 1000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Health_mod = 5, ScriptName = "mob_living_amber"
WHERE entry = 62691;

-- Sra'thik Pool-Tender
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Mana_mod = 10, Health_mod = 10, ScriptName = "mob_srathik_pooltender"
WHERE entry = 63570;