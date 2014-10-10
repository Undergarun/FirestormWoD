DELETE FROM game_graveyard_zone WHERE id = 4490;
INSERT INTO game_graveyard_zone VALUE (4490, 6622, 0);

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 14.72,
ScriptName = 'mob_zandalari_water_binder'
WHERE entry = 69455;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 9.6,
ScriptName = 'mob_zandalari_blade_initiate'
WHERE entry = 70230;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 8000,
maxdmg = 16000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 2.4
WHERE entry = 69459;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 32,
ScriptName = 'mob_zandalari_spear_shaper'
WHERE entry = 69388;

UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
EXP = 4,
ScriptName = 'mob_thrown_spear'
WHERE entry = 69438;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 21.12,
ScriptName = 'mob_zandalari_storm_caller'
WHERE entry = 69390;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
faction_A = 14,
faction_H = 14,
modelid1 = 47698,
modelid2 = 0,
ScriptName = 'mob_trash_focused_lightning'
WHERE entry = 70174;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 4,
ScriptName = 'mob_ancient_python'
WHERE entry = 70448;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
EXP = 4,
Health_mod = 1,
ScriptName = 'mob_spirit_flayer',
MovementType = 2,
InhabitType = 4
WHERE entry = 70246;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 40,
InhabitType = 4
WHERE entry = 70441;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 40,
ScriptName = 'mob_tourmented_spirit',
InhabitType = 4
WHERE entry = 70341;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 64,
ScriptName = 'mob_soul_fed_construct'
WHERE entry = 70308;

UPDATE creature_template SET
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 56,
ScriptName = 'mob_stormbringer_draz_kil'
WHERE entry = 70445;

UPDATE creature_template SET ScriptName = 'mob_dark_winds', flags_extra = 0x80 WHERE entry = 59394;

DELETE FROM spell_proc_event WHERE entry = 139218;
INSERT INTO spell_proc_event (entry, procFlags) VALUE (139218, 0x04);

DELETE FROM spell_script_names WHERE spell_id IN (139218, 139231, 139209, 139461, 138687, 138652, 138610, 139223, 140814, 135103, 135101);
INSERT INTO spell_script_names VALUES
(139218, 'spell_storm_weapon'),
(139231, 'spell_water_bolt'),
(139209, 'spell_focused_lightning_aoe'),
(139461, 'spell_spirit_light'),
(138687, 'spell_glacial_freeze'),
(138652, 'spell_eruption'),
(138610, 'spell_fiery_core'),
(139223, 'spell_judgement_of_the_loa'),
(140814, 'spell_waterspout'),
(135103, 'spell_drain_the_weak'),
(135101, 'spell_drain_the_weak_damage');

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 7.68,
ScriptName = 'mob_drakkari_frost_warden'
WHERE entry = 69910;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
EXP = 4,
Health_mod = 0.8,
ScriptName = 'mob_gacial_freeze_totem'
WHERE entry = 70047;

UPDATE creature SET MovementType = 0 WHERE id = 70441 AND map = 1098;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 12,
ScriptName = 'mob_gurubashi_berserker'
WHERE entry IN (69905, 69916);

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 7.68,
ScriptName = 'mob_amani_shi_flame_chanter'
WHERE entry = 69909;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 7.68,
ScriptName = 'mob_farraki_sand_conjurer'
WHERE entry = 69899;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 2.4,
ScriptName = 'mob_sand_elemental'
WHERE entry = 69944;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 9.6,
ScriptName = 'mob_zandalari_high_priest'
WHERE entry = 69906;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 11.52,
ScriptName = 'mob_zandalari_prophet'
WHERE entry = 70557;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 32,
ScriptName = 'mob_zandalari_warlord'
WHERE entry = 69911;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 16,
ScriptName = 'mob_zandalari_prelate'
WHERE entry = 69927;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
modelid1 = 11686,
modelid2 = 0,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_waterspout'
WHERE entry = 70147;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 8,
InhabitType = 4,
ScriptName = 'mob_vampiric_cave_bat'
WHERE entry = 69352;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
EXP = 4,
Health_mod = 24,
InhabitType = 4,
ScriptName = 'mob_greater_cave_bat'
WHERE entry = 69351;