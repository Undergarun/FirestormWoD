DELETE FROM creature_text WHERE entry = 69374;
INSERT INTO creature_text VALUES
(69374, 0, 0, 'Welcome, weaklings, to the rebirth of the Zandalari Empire!', 14, 0, 100, 0, 0, 36009, 'JALAK_INTRO_01'),
(69374, 1, 0, 'The tribes have assembled - ye face not one force, but the combined might of all of the troll empire! The hand of Zul will span all the continents of Azeroth once again! An ye lesser races will know pain!', 14, 0, 100, 0, 0, 36010, 'JALAK_INTRO_02'),
(69374, 2, 0, 'Now, witness the true might of the Beast Ward. D''akala di''chuka HORRIDON! Kalimaste!', 14, 0, 100, 0, 0, 36011, 'JALAK_INTRO_03'),
(69374, 3, 0, 'Farraki tribe, flay their flesh wit the fury of the sands!', 14, 0, 100, 0, 0, 36015, 'JALAK_FARRAKI'),
(69374, 4, 0, 'Gurubashi tribe, send their bloated corpses down the river with ya jungle poisons.', 14, 0, 100, 0, 0, 36014, 'JALAK_GURUBASHI'),
(69374, 5, 0, 'Drakkari tribe, show them the might of the frozen North!', 14, 0, 100, 0, 0, 36013, 'JALAK_DRAKKARI'),
(69374, 6, 0, 'Amani tribe, avenge ye fallen warlords, in the name of Zul''jin!', 14, 0, 100, 0, 0, 36016, 'JALAK_AMANI'),
(69374, 7, 0, 'Hahaha! Now it be my turn! Ye gonna see what it means to be a War-God!', 14, 0, 100, 0, 0, 36007, 'JALAK_AGGRO'),
(69374, 8, 0, 'Ya skull gonna make a fine ornament for my tusks.', 14, 0, 100, 0, 0, 36012, 'JALAK_SLAY'),
(69374, 9, 0, 'Da''kala koraste...Horridon...destroy them...', 14, 0, 100, 0, 0, 36008, 'JALAK_DEATH');

DELETE FROM locales_creature_text WHERE entry = 69374;
INSERT INTO locales_creature_text VALUES
(69374, 0, 0, '', 'L''empire Zandalari renaît, bienvenue pauvres vermisseaux !', '', '', '', '', '', '', '', ''),
(69374, 1, 0, '', 'Les tribus elles se sont rassemblées - vous ne faites pas face à une armée, mais à la puissance de tout l''empire des troll ! La main de Zul elle va recouvrir tous les continents d''Azeroth à nouveau ! Toutes les races inférieures vont connaître la souffrance !', '', '', '', '', '', '', '', ''),
(69374, 2, 0, '', 'Maintenant contemplez la vraie puissance de la bête ! D''akala d''chuka HORRIDON ! Kalimaste !', '', '', '', '', '', '', '', ''),
(69374, 3, 0, '', 'Farraki, fouettez leur chair avec la fureur des sables !', '', '', '', '', '', '', '', ''),
(69374, 4, 0, '', 'Gurubashi ! Que vos poisons précipitent leurs corps tuméfiés au fond du fleuve.', '', '', '', '', '', '', '', ''),
(69374, 5, 0, '', 'Drakkari ! Montrez-leur la force du grand Nord !', '', '', '', '', '', '', '', ''),
(69374, 6, 0, '', 'Amani ! Vengez vos seigneurs de guerre tombés au combat, au nom de Zul''jin !', '', '', '', '', '', '', '', ''),
(69374, 7, 0, '', 'Hahahahaha ! Maintenant c’est à moi ! Vous allez voir ce que c''est qu''un dieu-guerrier !', '', '', '', '', '', '', '', ''),
(69374, 8, 0, '', 'Votre crâne sera une décoration parfaite pour mes défenses.', '', '', '', '', '', '', '', ''),
(69374, 9, 0, '', 'Da''kala koraste… Horridon… Détruis-les...', '', '', '', '', '', '', '', '');

UPDATE gameobject_template SET faction = 0, flags = 64, data0 = 86, data3 = 0, data5 = 0, data13 = 1, data23 = 0 WHERE entry IN (218193, 218374, 218375, 218376); -- Farraki, Gurubashi, Drakkari, Amani
UPDATE gameobject_template SET ScriptName = 'go_horridon_intro_door' WHERE entry = 218667;

DELETE FROM conditions WHERE SourceEntry IN (137433, 137442, 137443, 137444, 137458);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 2, 137433, 31, 0, 3, 68476),
(13, 2, 137442, 31, 0, 3, 68476),
(13, 2, 137443, 31, 0, 3, 68476),
(13, 2, 137444, 31, 0, 3, 68476),
(13, 2, 137458, 31, 0, 3, 69374),
(13, 2, 137458, 31, 0, 3, 69172),
(13, 2, 137458, 31, 0, 3, 69173),
(13, 2, 137458, 31, 0, 3, 69175),
(13, 2, 137458, 31, 0, 3, 69221),
(13, 2, 137458, 31, 0, 3, 69167),
(13, 2, 137458, 31, 0, 3, 69164),
(13, 2, 137458, 31, 0, 3, 69184),
(13, 2, 137458, 31, 0, 3, 69185),
(13, 2, 137458, 31, 0, 3, 69178),
(13, 2, 137458, 31, 0, 3, 69168),
(13, 2, 137458, 31, 0, 3, 69169),
(13, 2, 137458, 31, 0, 3, 69176),
(13, 2, 137458, 31, 0, 3, 69177);

DELETE FROM spell_script_names WHERE spell_id IN (137433, 137442, 137443, 137444, 137294, 137458, 136739, 136740);
INSERT INTO spell_script_names VALUES
(137433, 'spell_control_of_horridon'),
(137442, 'spell_control_of_horridon'),
(137443, 'spell_control_of_horridon'),
(137444, 'spell_control_of_horridon'),
(137294, 'spell_headache'),
(137458, 'spell_dire_call'),
(136739, 'spell_double_swipe'),
(136740, 'spell_double_swipe');

DELETE FROM game_graveyard_zone WHERE id = 4490;
INSERT INTO game_graveyard_zone VALUE (4490, 6507, 0);

DELETE FROM spell_proc_event WHERE entry = 136709;
INSERT INTO spell_proc_event (entry, procFlags, procEx, Cooldown) VALUE (136709, 0x04, 0x03, 5);

DELETE FROM spell_proc_event WHERE entry = 136512;
INSERT INTO spell_proc_event (entry, procFlags, CustomChance, Cooldown) VALUE (136512, 0x015554, 50, 2);

UPDATE creature_model_info SET
bounding_radius = 3.5,
combat_reach = 24.5
WHERE modelid = 47235;

UPDATE creature_model_info SET
bounding_radius = 2.61,
combat_reach = 7.5
WHERE modelid = 47235;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 4,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 656,
mechanic_immune_mask = 617299839,
ScriptName = 'boss_horridon'
WHERE entry = 68476;

UPDATE creature_template SET
minlevel = 93,
maxlevel = 93,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 2,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 32,
mechanic_immune_mask = 617299839,
ScriptName = 'boss_war_god_jalak'
WHERE entry = 69374;

UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 2.4,
ScriptName = 'mob_sul_lithuz_stonegazer'
WHERE entry = 69172;

UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 3,
ScriptName = 'mob_farraki_skirmisher'
WHERE entry = 69173;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 10.4,
ScriptName = 'mob_farraki_wastewalker'
WHERE entry = 69175;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
EXP = 4,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_sand_trap'
WHERE entry = 69346;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 6.8,
ScriptName = 'mob_zandalari_dinomancer'
WHERE entry = 69221;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 9.2,
ScriptName = 'mob_gurubashi_bloodlord'
WHERE entry = 69167;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 7.8,
ScriptName = 'mob_gurubashi_venom_priest'
WHERE entry = 69164;

UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
modelid3 = 0,
modelid4 = 0,
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_venomous_effusion'
WHERE entry = 69314;

UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 1.2,
ScriptName = 'mob_risen_drakkari_warrior'
WHERE entry = 69184;

UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 3.6,
ScriptName = 'mob_risen_drakkari_champion'
WHERE entry = 69185;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 12.96,
ScriptName = 'mob_drakkari_frozen_warlord'
WHERE entry = 69178;

UPDATE creature_template SET
modelid1 = 11686,
modelid2 = 0,
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
speed_walk = 0,
speed_run = 0,
speed_fly = 0,
unit_flags = 0x04,
ScriptName = 'mob_frozen_orb'
WHERE entry = 69268;

UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 2.4,
ScriptName = 'mob_amani_shi_flame_caster'
WHERE entry = 69168;

UPDATE creature_template SET
minlevel = 91,
maxlevel = 91,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 4.8,
ScriptName = 'mob_amani_shi_protector'
WHERE entry = 69169;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 5.2,
ScriptName = 'mob_amani_shi_beast_shaman'
WHERE entry = 69176;

UPDATE creature_template SET
modelid1 = 47354,
modelid2 = 0,
minlevel = 92,
maxlevel = 92,
faction_A = 14,
faction_H = 14,
ScriptName = 'mob_lightning_nova_totem'
WHERE entry = 69215;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 15000,
maxdmg = 30000,
baseattacktime = 2000,
dmg_multiplier = 1,
EXP = 4,
faction_A = 14,
faction_H = 14,
VehicleId = 2666,
Health_mod = 5.8,
ScriptName = 'mob_amani_warbear'
WHERE entry = 69177;

UPDATE creature_template SET
minlevel = 92,
maxlevel = 92,
mindmg = 150000,
maxdmg = 300000,
baseattacktime = 2000,
dmg_multiplier = 2,
EXP = 4,
faction_A = 14,
faction_H = 14,
Health_mod = 800,
ScriptName = 'mob_direhorn_spirit'
WHERE entry = 70688;

-- Trashs mobs and Horridon
-- 25 Men
UPDATE creature_template SET difficulty_entry_4 = 368476 WHERE entry = 68476;
INSERT INTO `creature_template` VALUES
('368476','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47325','0','0','0','Horridon (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','15000','30000','0','0','10','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','1','2097260','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','2160','1','1','1','0','0','0','0','0','0','0','180','1','0','617299839','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369164 WHERE entry = 69164;
INSERT INTO `creature_template` VALUES
('369164','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47378','47263','47379','47264','Gurubashi Venom Priest (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','32.4','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369167 WHERE entry = 69167;
INSERT INTO `creature_template` VALUES
('369167','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47442','47443','47444','47445','Gurubashi Bloodlord (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','32.4','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369168 WHERE entry = 69168;
INSERT INTO `creature_template` VALUES
('369168','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','22307','22308','47335','47336','Amani\'shi Flame Caster (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','8.64','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369169 WHERE entry = 69169;
INSERT INTO `creature_template` VALUES
('369169','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','22263','22264','47337','47338','Amani\'shi Protector (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','17.28','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369172 WHERE entry = 69172;
INSERT INTO `creature_template` VALUES
('369172','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47469','45463','0','0','Sul\'lithuz Stonegazer (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','130','0','0','0','0','0','0','0','1','2097248','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','8.64','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369173 WHERE entry = 69173;
INSERT INTO `creature_template` VALUES
('369173','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47559','47560','47561','47562','Farraki Skirmisher (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','10.8','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369175 WHERE entry = 69175;
INSERT INTO `creature_template` VALUES
('369175','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47475','47477','47478','47479','Farraki Wastewalker (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','43.2','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369176 WHERE entry = 69176;
INSERT INTO `creature_template` VALUES
('369176','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47331','47332','47333','47334','Amani\'shi Beast Shaman (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','21.6','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369178 WHERE entry = 69178;
INSERT INTO `creature_template` VALUES
('369178','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47447','47448','47449','47450','Drakkari Frozen Warlord (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','54','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369184 WHERE entry = 69184;
INSERT INTO `creature_template` VALUES
('369184','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47446','27056','47558','0','Risen Drakkari Warrior (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','4.32','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369185 WHERE entry = 69185;
INSERT INTO `creature_template` VALUES
('369185','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47556','47557','0','0','Risen Drakkari Champion (4)','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','12.96','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369221 WHERE entry = 69221;
INSERT INTO `creature_template` VALUES
('369221','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47494','47495','47496','47497','Zandalari Dinomancer (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','21.6','1','1','1','0','0','0','0','0','0','0','180','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369374 WHERE entry = 69374;
INSERT INTO `creature_template` VALUES
('369374','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47502','0','0','0','War-God Jalak (4)','Ward of Beasts','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','8','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097260','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','96','1','1','1','0','0','0','0','0','0','0','180','1','0','617299839','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369899 WHERE entry = 69899;
INSERT INTO `creature_template` VALUES
('369899','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47775','47782','47783','47784','Farraki Sand Conjurer (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','19.2','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369905 WHERE entry = 69905;
INSERT INTO `creature_template` VALUES
('369905','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47766','47767','0','0','Gurubashi Berserker (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','30','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369906 WHERE entry = 69906;
INSERT INTO `creature_template` VALUES
('369906','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47761','47762','0','0','Zandalari High Priest (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','24','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369909 WHERE entry = 69909;
INSERT INTO `creature_template` VALUES
('369909','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','22307','22308','47335','47336','Amani\'shi Flame Chanter (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','19.2','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369910 WHERE entry = 69910;
INSERT INTO `creature_template` VALUES
('369910','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47769','47780','47781','0','Drakkari Frost Warden (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','19.2','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369911 WHERE entry = 69911;
INSERT INTO `creature_template` VALUES
('369911','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47760','0','0','0','Zandalari Warlord (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','80','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369916 WHERE entry = 69916;
INSERT INTO `creature_template` VALUES
('369916','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48181','0','0','0','Gurubashi Berserker (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','30','1','1','1','0','0','0','0','0','0','0','174','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369927 WHERE entry = 69927;
INSERT INTO `creature_template` VALUES
('369927','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47885','0','0','0','Zandalari Prelate (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','40','1','1','1','0','0','0','0','0','0','0','171','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 369944 WHERE entry = 69944;
INSERT INTO `creature_template` VALUES
('369944','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47677','0','0','0','Sand Elemental (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','4','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','6','1','1','1','0','0','0','0','0','0','0','165','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370047 WHERE entry = 70047;
INSERT INTO `creature_template` VALUES
('370047','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','31720','0','0','0','Glacial Freeze Totem','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','11','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','2','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370308 WHERE entry = 70308;
INSERT INTO `creature_template` VALUES
('370308','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47998','0','0','0','Soul-Fed Construct','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','9','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','192','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370341 WHERE entry = 70341;
INSERT INTO `creature_template` VALUES
('370341','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48048','0','0','0','Tormented Spirit','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','4','1','120','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370441 WHERE entry = 70441;
INSERT INTO `creature_template` VALUES
('370441','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47693','0','0','0','Lost Soul','','','0','91','91','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','6','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','4','1','50','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370445 WHERE entry = 70445;
INSERT INTO `creature_template` VALUES
('370445','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48049','0','0','0','Stormbringer Draz\'kil','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','168','1','1','1','0','0','0','0','0','0','0','0','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370448 WHERE entry = 70448;
INSERT INTO `creature_template` VALUES
('370448','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48052','48051','48050','0','Ancient Python','','','0','90','90','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','35','0','0','0','0','0','0','0','1','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','12','1','1','1','0','0','0','0','0','0','0','144','1','0','0','0','','17614');

UPDATE creature_template SET difficulty_entry_4 = 370557 WHERE entry = 70557;
INSERT INTO `creature_template` VALUES
('370557','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48121','0','0','0','Zandalari Prophet','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','7','2097224','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','11000','12000','','0','3','1','28.8','1','1','1','0','0','0','0','0','0','0','171','1','0','0','0','','17614');