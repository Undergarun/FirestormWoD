UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 4,
EXP = 4,
Health_mod = 1000,
mechanic_immune_mask = 617299839,
ScriptName = 'boss_oondasta'
WHERE entry = 69161;

INSERT INTO spell_script_names VALUE (138390, 'spell_alpha_male_threat');