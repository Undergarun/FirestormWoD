-- Kor'thik Swarmguard
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags2 = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Health_mod = 15, ScriptName = "mob_korthik_swarmguard"
WHERE entry = 64916;

-- Sra'thik Ambercaller
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Mana_mod = 15, Health_mod = 10,
ScriptName = "mob_srathik_ambercaller"
WHERE entry = 64917;

-- Kor'thik Fleshrender
UPDATE creature_template SET
MinLevel = 92, MaxLevel = 92,
faction_A = 16, faction_H = 16,
unit_flags = 32768, unit_flags = 2099200,
baseattacktime = 2000, rangeattacktime = 2000,
mindmg = 4750, maxdmg = 11118,
dmg_multiplier = 2, attackpower = 55538,
Health_mod = 25,
ScriptName = "mob_korthik_fleshrender"
WHERE entry = 64902;