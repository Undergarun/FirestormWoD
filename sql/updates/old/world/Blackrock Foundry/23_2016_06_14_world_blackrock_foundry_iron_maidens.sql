SET NAMES utf8;

DELETE FROM areatrigger_scripts WHERE entry IN (9945, 10243);
INSERT INTO areatrigger_scripts VALUES
(9945, "areatrigger_at_foundry_iron_maidens_boat"),
(10243, "areatrigger_at_foundry_iron_maidens_entrance");

SET @REF_SORKA = 77231;
SET @REF_GARAN = 77557;
SET @REF_MARAK = 77477;

UPDATE creature_template SET unit_flags = unit_flags & ~(0x200000 | 0x04), dmg_multiplier = 10, ScriptName = "boss_enforcer_sorka", mechanic_immune_mask = 617299839 WHERE entry = @REF_SORKA;
UPDATE creature_template SET unit_flags = unit_flags & ~(0x200000 | 0x04), dmg_multiplier = 10, ScriptName = "boss_admiral_garan", mechanic_immune_mask = 617299839 WHERE entry = @REF_GARAN;
UPDATE creature_template SET unit_flags = unit_flags & ~(0x200000 | 0x04), dmg_multiplier = 10, ScriptName = "boss_marak_the_blooded", mechanic_immune_mask = 617299839 WHERE entry = @REF_MARAK;
UPDATE creature_template SET dmg_multiplier = 4 WHERE entry = 85711;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_dockworker" WHERE entry = 84841;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_earthbinder" WHERE entry = 84860;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_mauler" WHERE entry = 85748;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_cleaver" WHERE entry = 84859;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_ukurogg" WHERE entry = 78341;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_uktar" WHERE entry = 78351;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_battle_medic_rogg" WHERE entry = 78352;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gorak" WHERE entry = 78343;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_eviscerator" WHERE entry = 78347;
UPDATE creature_template SET speed_walk = 2.5, speed_run = 8, speed_fly = 7, unit_flags = 768, unit_flags2 = 1140885504, InhabitType = 4, ScriptName = "npc_foundry_loading_chain" WHERE entry = 78767;
UPDATE creature_template SET minlevel = 102, maxlevel = 102, ScriptName = "npc_foundry_inferno_totem" WHERE entry = 85922;
UPDATE creature_template SET speed_walk = 2.5, speed_run = 8, speed_fly = 7, InhabitType = 4, ScriptName = "npc_foundry_zipline_stalker" WHERE entry = 82538;
UPDATE creature_template SET ScriptName = "npc_foundry_rapid_fire_stalker" WHERE entry = 77636;
UPDATE creature_template SET ScriptName = "npc_foundry_dominator_turret" WHERE entry = 78583;
UPDATE creature_template SET modelid1 = 55871, modelid2 = 0 WHERE entry = 80622;
UPDATE creature_template SET InhabitType = 4, ScriptName = "npc_foundry_iron_cannon" WHERE entry = 78152;
UPDATE creature_template SET ScriptName = "npc_foundry_cluster_bomb_alpha" WHERE entry = 78177;

REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (@REF_SORKA, 1, 114966, 113965, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (@REF_GARAN, 1, 0, 0, 112344);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (@REF_MARAK, 1, 114962, 114971, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (85711, 1, 0, 0, 108504);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE (78351, 1, 0, 0, 108507);

DELETE FROM `areatrigger_template` WHERE spell_id IN (171545, 158602, 158707, 158684);
INSERT INTO `areatrigger_template` (`spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`) VALUES
(171545, 0, 171545, 2, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_workers_solidarity"),
(158602, 0, 6641, 2, 1, 1, 8192, 1, 1, 3, 3, 0.5, 0.5, 0, 0, "areatrigger_foundry_dominator_blast"),
(158707, 0, 158707, 2, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_protective_earth"),
(158684, 0, 158684, 2, 1, 1, 4, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_foundry_corrupted_blood");

DELETE FROM spell_script_names WHERE spell_id IN (158078, 164271, 164279, 156214, 158315, 158009, 156601, 158148, 158724, 158849, 157854, 157867);
INSERT INTO spell_script_names VALUES
(158078, "spell_foundry_blood_ritual"),
(164271, "spell_foundry_penetrating_shot"),
(164279, "spell_foundry_penetrating_shot_damage"),
(156214, "spell_foundry_convulsive_shadows"),
(158315, "spell_foundry_dark_hunt"),
(158009, "spell_foundry_bloodsoaked_heartseeker_damage"),
(156601, "spell_foundry_sanguine_strikes_proc"),
(158148, "spell_foundry_sabotage"),
(158724, "spell_foundry_end_ship_phase"),
(158849, "spell_foundry_warming_up"),
(157854, "spell_foundry_bombardment_pattern_alpha"),
(157867, "spell_foundry_detonation_sequence");

DELETE FROM `conversation_template` WHERE `Entry` IN (118, 119);
INSERT INTO `conversation_template` VALUES
(118, 30000, 3, "77557 77231 77477", "Iron Maidens - Intro"),
(119, 10000, 3, "0 77557 77231", "Iron Maidens - After Trashes");

DELETE FROM `conversation_lines` WHERE `Entry` IN (118, 119);
INSERT INTO `conversation_lines` VALUES
(118, 452, 87897, 0, 0, 256),
(118, 453, 87898, 0, 5853, 256),
(118, 454, 87921, 0, 9253, 257),
(118, 455, 87899, 0, 10849, 256),
(118, 456, 87937, 0, 14547, 258),
(118, 457, 87900, 0, 18421, 256),
(118, 458, 87922, 0, 21417, 1),
(118, 459, 87923, 0, 23600, 257),
(118, 460, 87902, 0, 27884, 256),
(119, 461, 87924, 0, 0, 258),
(119, 462, 87901, 0, 5316, 257),
(119, 463, 87925, 0, 8190, 2);

DELIMITER @@
CREATE PROCEDURE AddWorldEffectColumn()
BEGIN
  IF NOT EXISTS (SELECT * FROM INFORMATION_SCHEMA.COLUMNS
      WHERE table_name = "gameobject_template" AND column_name = "WorldEffectID") THEN
    ALTER TABLE `gameobject_template` ADD COLUMN `WorldEffectID` INT(10) UNSIGNED DEFAULT 0 NOT NULL AFTER `unkInt32`;
  END IF;
END@@

DELIMITER ;

CALL AddWorldEffectColumn;

DROP PROCEDURE AddWorldEffectColumn;

UPDATE gameobject_template SET WorldEffectID = 3190 WHERE entry = 234028;

DELETE FROM npc_text WHERE ID IN (87897, 87898, 87921, 87899, 87937, 87900, 87922, 87923, 87902, 87924, 87901, 87925);
INSERT INTO npc_text (ID, SoundID, text0_0, text0_1) VALUE
(
    87897, 41824,
    "Patience, sisters. The sooner we finish our work, the sooner we crush our enemies.",
    "Patience, sisters. The sooner we finish our work, the sooner we crush our enemies."
),
(
    87898, 41825,
    "Sorka, ensure the peons are keeping pace!",
    "Sorka, ensure the peons are keeping pace!"
),
(
    87921, 41851,
    "My pleasure.",
    "My pleasure."
),
(
    87899, 41826,
    "Marak, load the ammunition into the main cannon!",
    "Marak, load the ammunition into the main cannon!"
),
(
    87937, 41710,
    "Right.",
    "Right."
),
(
    87900, 41827,
    "What? The trains are behind schedule?",
    "What? The trains are behind schedule?"
),
(
    87922, 41852,
    "I think I see the problem...",
    "I think I see the problem..."
),
(
    87923, 41853,
    "Let's see how well you trained our soldiers, Gar'an.",
    "Let's see how well you trained our soldiers, Gar'an."
),
(
    87902, 41829,
    "You insufferable...",
    "You insufferable..."
),
(
    87924, 41854,
    "What brutality... What bloodshed... Beautiful!",
    "What brutality... What bloodshed... Beautiful!"
),
(
    87901, 41828,
    "Sorka! This isn't a game! Get down there!",
    "Sorka! This isn't a game! Get down there!"
),
(
    87925, 41855,
    "Don't worry, I'm coming... this looks fun.",
    "Don't worry, I'm coming... this looks fun."
);

-- DELETE FROM locales_npc_text WHERE entry IN (87897, 87898, 87921, 87899, 87937, 87900, 87922, 87923, 87902, 87924, 87901, 87925);
--                                   French                      German                      Spanish                     Russian
-- INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE

DELETE FROM creature_groupsizestats WHERE entry = @REF_GARAN;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_GARAN, 14, 10, 31926892),
(@REF_GARAN, 14, 11, 37283828),
(@REF_GARAN, 14, 12, 42447036),
(@REF_GARAN, 14, 13, 47901729),
(@REF_GARAN, 14, 14, 53467667),
(@REF_GARAN, 14, 15, 59464364),
(@REF_GARAN, 14, 16, 64612262),
(@REF_GARAN, 14, 17, 70168205),
(@REF_GARAN, 14, 18, 75541907),
(@REF_GARAN, 14, 19, 81147926),
(@REF_GARAN, 14, 20, 86687265),
(@REF_GARAN, 14, 21, 92174779),
(@REF_GARAN, 14, 22, 97751458),
(@REF_GARAN, 14, 23, 103373957),
(@REF_GARAN, 14, 24, 108737430),
(@REF_GARAN, 14, 25, 114984090),
(@REF_GARAN, 14, 26, 120421526),
(@REF_GARAN, 14, 27, 125515492),
(@REF_GARAN, 14, 28, 131103640),
(@REF_GARAN, 14, 29, 136694519),
(@REF_GARAN, 14, 30, 142656206),
(@REF_GARAN, 15, 10, 39324609),
(@REF_GARAN, 15, 11, 46181880),
(@REF_GARAN, 15, 12, 53621277),
(@REF_GARAN, 15, 13, 59941123),
(@REF_GARAN, 15, 14, 67285419),
(@REF_GARAN, 15, 15, 74582463),
(@REF_GARAN, 15, 16, 81355797),
(@REF_GARAN, 15, 17, 88540258),
(@REF_GARAN, 15, 18, 94339392),
(@REF_GARAN, 15, 19, 101261708),
(@REF_GARAN, 15, 20, 108758343),
(@REF_GARAN, 15, 21, 115047015),
(@REF_GARAN, 15, 22, 122751891),
(@REF_GARAN, 15, 23, 128953874),
(@REF_GARAN, 15, 24, 135486871),
(@REF_GARAN, 15, 25, 143541667),
(@REF_GARAN, 15, 26, 150582150),
(@REF_GARAN, 15, 27, 157166252),
(@REF_GARAN, 15, 28, 164175964),
(@REF_GARAN, 15, 29, 170735557),
(@REF_GARAN, 15, 30, 177348397);

DELETE FROM creature_groupsizestats WHERE entry = @REF_SORKA;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_SORKA, 14, 10, 31400895),
(@REF_SORKA, 14, 11, 37684639),
(@REF_SORKA, 14, 12, 42731136),
(@REF_SORKA, 14, 13, 47986252),
(@REF_SORKA, 14, 14, 53423847),
(@REF_SORKA, 14, 15, 59155247),
(@REF_SORKA, 14, 16, 64497349),
(@REF_SORKA, 14, 17, 69956775),
(@REF_SORKA, 14, 18, 75471835),
(@REF_SORKA, 14, 19, 81684684),
(@REF_SORKA, 14, 20, 86509469),
(@REF_SORKA, 14, 21, 92272151),
(@REF_SORKA, 14, 22, 97580411),
(@REF_SORKA, 14, 23, 104142616),
(@REF_SORKA, 14, 24, 110017411),
(@REF_SORKA, 14, 25, 114142478),
(@REF_SORKA, 14, 26, 119486993),
(@REF_SORKA, 14, 27, 125093667),
(@REF_SORKA, 14, 28, 130753557),
(@REF_SORKA, 14, 29, 136167976),
(@REF_SORKA, 14, 30, 141674716),
(@REF_SORKA, 15, 10, 39218819),
(@REF_SORKA, 15, 11, 46108625),
(@REF_SORKA, 15, 12, 54536779),
(@REF_SORKA, 15, 13, 61573953),
(@REF_SORKA, 15, 14, 66790086),
(@REF_SORKA, 15, 15, 73835009),
(@REF_SORKA, 15, 16, 80572030),
(@REF_SORKA, 15, 17, 87504765),
(@REF_SORKA, 15, 18, 94894391),
(@REF_SORKA, 15, 19, 101486706),
(@REF_SORKA, 15, 20, 108117613),
(@REF_SORKA, 15, 21, 114847981),
(@REF_SORKA, 15, 22, 121951478),
(@REF_SORKA, 15, 23, 128824036),
(@REF_SORKA, 15, 24, 135842485),
(@REF_SORKA, 15, 25, 142589041),
(@REF_SORKA, 15, 26, 149517247),
(@REF_SORKA, 15, 27, 156370616),
(@REF_SORKA, 15, 28, 163286296),
(@REF_SORKA, 15, 29, 170185555),
(@REF_SORKA, 15, 30, 177348397);

DELETE FROM creature_groupsizestats WHERE entry = @REF_MARAK;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(@REF_MARAK, 14, 10, 32130847),
(@REF_MARAK, 14, 11, 37026618),
(@REF_MARAK, 14, 12, 42483033),
(@REF_MARAK, 14, 13, 47930727),
(@REF_MARAK, 14, 14, 53469615),
(@REF_MARAK, 14, 15, 59345468),
(@REF_MARAK, 14, 16, 64822807),
(@REF_MARAK, 14, 17, 70101520),
(@REF_MARAK, 14, 18, 75502064),
(@REF_MARAK, 14, 19, 81956945),
(@REF_MARAK, 14, 20, 87271144),
(@REF_MARAK, 14, 21, 92036982),
(@REF_MARAK, 14, 22, 97542474),
(@REF_MARAK, 14, 23, 103370938),
(@REF_MARAK, 14, 24, 108723200),
(@REF_MARAK, 14, 25, 114964108),
(@REF_MARAK, 14, 26, 120356998),
(@REF_MARAK, 14, 27, 125707077),
(@REF_MARAK, 14, 28, 132430305),
(@REF_MARAK, 14, 29, 137639543),
(@REF_MARAK, 14, 30, 142754234),
(@REF_MARAK, 15, 10, 40004552),
(@REF_MARAK, 15, 11, 46590920),
(@REF_MARAK, 15, 12, 53519696),
(@REF_MARAK, 15, 13, 60553441),
(@REF_MARAK, 15, 14, 68068824),
(@REF_MARAK, 15, 15, 74085669),
(@REF_MARAK, 15, 16, 81577533),
(@REF_MARAK, 15, 17, 88426166),
(@REF_MARAK, 15, 18, 95112734),
(@REF_MARAK, 15, 19, 102127909),
(@REF_MARAK, 15, 20, 108918142),
(@REF_MARAK, 15, 21, 116406296),
(@REF_MARAK, 15, 22, 123035048),
(@REF_MARAK, 15, 23, 129891957),
(@REF_MARAK, 15, 24, 137027740),
(@REF_MARAK, 15, 25, 143766150),
(@REF_MARAK, 15, 26, 150822037),
(@REF_MARAK, 15, 27, 157781971),
(@REF_MARAK, 15, 28, 164428796),
(@REF_MARAK, 15, 29, 170899241),
(@REF_MARAK, 15, 30, 179070467);

DELETE FROM creature_groupsizestats WHERE entry = 78583;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78583, 14, 10, 745634),
(78583, 14, 11, 755213),
(78583, 14, 12, 849717),
(78583, 14, 13, 996712),
(78583, 14, 14, 1087435),
(78583, 14, 15, 1147424),
(78583, 14, 16, 1112514),
(78583, 14, 17, 1227022),
(78583, 14, 18, 1362296),
(78583, 14, 19, 1783580),
(78583, 14, 20, 1441558),
(78583, 14, 21, 1970466),
(78583, 14, 22, 1919680),
(78583, 14, 23, 2117844),
(78583, 14, 24, 1759585),
(78583, 14, 25, 1788648),
(78583, 14, 26, 2078561),
(78583, 14, 27, 2250236),
(78583, 14, 28, 2068699),
(78583, 14, 29, 2468907),
(78583, 14, 30, 2240758),
(78583, 15, 10, 958491),
(78583, 15, 11, 1281746),
(78583, 15, 12, 1213177),
(78583, 15, 13, 1234689),
(78583, 15, 14, 1258928),
(78583, 15, 15, 1888647),
(78583, 15, 16, 1605700),
(78583, 15, 17, 1688287),
(78583, 15, 18, 1711368),
(78583, 15, 19, 1762144),
(78583, 15, 20, 1326520),
(78583, 15, 21, 2057583),
(78583, 15, 22, 2164276),
(78583, 15, 23, 2264708),
(78583, 15, 24, 2551446),
(78583, 15, 25, 2276223),
(78583, 15, 26, 2597317),
(78583, 15, 27, 2688080),
(78583, 15, 28, 2811810),
(78583, 15, 29, 2694501),
(78583, 15, 30, 3276639);