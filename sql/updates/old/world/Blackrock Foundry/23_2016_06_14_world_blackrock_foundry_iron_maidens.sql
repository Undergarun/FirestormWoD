SET NAMES utf8;

DELETE FROM areatrigger_scripts WHERE entry IN (9945, 10243);
INSERT INTO areatrigger_scripts VALUES
(9945, "areatrigger_at_foundry_iron_maidens_boat"),
(10243, "areatrigger_at_foundry_iron_maidens_entrance");

SET @REF_SORKA = 77231;
SET @REF_GARAN = 77557;
SET @REF_MARAK = 77477;
SET @REF_COSMETIC_BLACKHAND = 76831;

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

DELETE FROM spell_script_names WHERE spell_id IN (158078, 164271, 164279, 156214, 158315, 158009, 156601, 158148, 158724, 158849, 157854, 157867, 156626);
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
(157867, "spell_foundry_detonation_sequence"),
(156626, "spell_foundry_rapid_fire_periodic");

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

DELETE FROM `reference_loot_template` WHERE entry = @REF_GARAN;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GARAN, 113977, 0, 1, 1, 1, 1),
(@REF_GARAN, 113965, 0, 1, 1, 1, 1),
(@REF_GARAN, 113966, 0, 1, 1, 1, 1),
(@REF_GARAN, 113967, 0, 1, 1, 1, 1),
(@REF_GARAN, 113968, 0, 1, 1, 1, 1),
(@REF_GARAN, 113969, 0, 1, 1, 1, 1),
(@REF_GARAN, 113970, 0, 1, 1, 1, 1),
(@REF_GARAN, 113971, 0, 1, 1, 1, 1),
(@REF_GARAN, 113972, 0, 1, 1, 1, 1),
(@REF_GARAN, 113973, 0, 1, 1, 1, 1),
(@REF_GARAN, 113974, 0, 1, 1, 1, 1),
(@REF_GARAN, 113975, 0, 1, 1, 1, 1),
(@REF_GARAN, 113976, 0, 1, 1, 1, 1),
(@REF_GARAN, 113978, 0, 1, 1, 1, 1);

# Set tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_GARAN + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GARAN + 1, 119306, 0, 14, 1, 1, 1),
(@REF_GARAN + 1, 119311, 0, 14, 1, 1, 1),
(@REF_GARAN + 1, 119319, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_GARAN WHERE `entry`= @REF_GARAN;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_GARAN;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_GARAN, 1, 100, 1, 0, -@REF_GARAN, 6),
(@REF_GARAN, 2, 100, 2, 0, -(@REF_GARAN + 1), 2),
(@REF_GARAN, 3, 100, 4, 0, -(@REF_GARAN + 1), 1),
(@REF_GARAN, 4, 100, 8, 0, -(@REF_GARAN + 1), 1);

DELETE FROM `reference_loot_template` WHERE entry = @REF_SORKA;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SORKA, 113977, 0, 1, 1, 1, 1),
(@REF_SORKA, 113965, 0, 1, 1, 1, 1),
(@REF_SORKA, 113966, 0, 1, 1, 1, 1),
(@REF_SORKA, 113967, 0, 1, 1, 1, 1),
(@REF_SORKA, 113968, 0, 1, 1, 1, 1),
(@REF_SORKA, 113969, 0, 1, 1, 1, 1),
(@REF_SORKA, 113970, 0, 1, 1, 1, 1),
(@REF_SORKA, 113971, 0, 1, 1, 1, 1),
(@REF_SORKA, 113972, 0, 1, 1, 1, 1),
(@REF_SORKA, 113973, 0, 1, 1, 1, 1),
(@REF_SORKA, 113974, 0, 1, 1, 1, 1),
(@REF_SORKA, 113975, 0, 1, 1, 1, 1),
(@REF_SORKA, 113976, 0, 1, 1, 1, 1),
(@REF_SORKA, 113978, 0, 1, 1, 1, 1);

# Set tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_SORKA + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SORKA + 1, 119306, 0, 14, 1, 1, 1),
(@REF_SORKA + 1, 119311, 0, 14, 1, 1, 1),
(@REF_SORKA + 1, 119319, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_SORKA WHERE `entry`= @REF_SORKA;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_SORKA;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_SORKA, 1, 100, 1, 0, -@REF_SORKA, 6),
(@REF_SORKA, 2, 100, 2, 0, -(@REF_SORKA + 1), 2),
(@REF_SORKA, 3, 100, 4, 0, -(@REF_SORKA + 1), 1),
(@REF_SORKA, 4, 100, 8, 0, -(@REF_SORKA + 1), 1);

DELETE FROM `reference_loot_template` WHERE entry = @REF_MARAK;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MARAK, 113977, 0, 1, 1, 1, 1),
(@REF_MARAK, 113965, 0, 1, 1, 1, 1),
(@REF_MARAK, 113966, 0, 1, 1, 1, 1),
(@REF_MARAK, 113967, 0, 1, 1, 1, 1),
(@REF_MARAK, 113968, 0, 1, 1, 1, 1),
(@REF_MARAK, 113969, 0, 1, 1, 1, 1),
(@REF_MARAK, 113970, 0, 1, 1, 1, 1),
(@REF_MARAK, 113971, 0, 1, 1, 1, 1),
(@REF_MARAK, 113972, 0, 1, 1, 1, 1),
(@REF_MARAK, 113973, 0, 1, 1, 1, 1),
(@REF_MARAK, 113974, 0, 1, 1, 1, 1),
(@REF_MARAK, 113975, 0, 1, 1, 1, 1),
(@REF_MARAK, 113976, 0, 1, 1, 1, 1),
(@REF_MARAK, 113978, 0, 1, 1, 1, 1);

# Set tokens
DELETE FROM `reference_loot_template` WHERE entry = @REF_MARAK + 1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MARAK + 1, 119306, 0, 14, 1, 1, 1),
(@REF_MARAK + 1, 119311, 0, 14, 1, 1, 1),
(@REF_MARAK + 1, 119319, 0, 14, 1, 1, 1);

UPDATE `creature_template` SET `lootid`= @REF_MARAK WHERE `entry`= @REF_MARAK;

DELETE FROM `creature_loot_template` WHERE `entry`= @REF_MARAK;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_MARAK, 1, 100, 1, 0, -@REF_MARAK, 6),
(@REF_MARAK, 2, 100, 2, 0, -(@REF_MARAK + 1), 2),
(@REF_MARAK, 3, 100, 4, 0, -(@REF_MARAK + 1), 1),
(@REF_MARAK, 4, 100, 8, 0, -(@REF_MARAK + 1), 1);

DELETE FROM npc_text WHERE ID IN (87897, 87898, 87921, 87899, 87937, 87900, 87922, 87923, 87902, 87924, 87901, 87925);
INSERT INTO npc_text (ID, SoundID, text0_0, text0_1) VALUES
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

DELETE FROM locales_npc_text WHERE entry IN (87897, 87898, 87921, 87899, 87937, 87900, 87922, 87923, 87902, 87924, 87901, 87925);
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUES
(
    87897,
    "Patience, mes sœurs. Plus vite nous aurons terminé, plus vite nous écraserons nos ennemis.",
    "Patience, mes sœurs. Plus vite nous aurons terminé, plus vite nous écraserons nos ennemis.",
    "Weiter so, Schwestern. Je schneller wir unsere Arbeit erledigen, desto eher vernichten wir unsere Feinde.",
    "Weiter so, Schwestern. Je schneller wir unsere Arbeit erledigen, desto eher vernichten wir unsere Feinde.",
    "Paciencia, hermanas. Entre más pronto terminemos, más pronto aplastaremos al enemigo.",
    "Paciencia, hermanas. Entre más pronto terminemos, más pronto aplastaremos al enemigo.",
    "Терпение, сестры. Чем скорее мы закончим, тем скорее сокрушим врага.",
    "Терпение, сестры. Чем скорее мы закончим, тем скорее сокрушим врага."
),
(
    87898,
    "Sorka ! Assure-toi que ces péons gardent le rythme !",
    "Sorka ! Assure-toi que ces péons gardent le rythme !",
    "Sorka! Stell sicher, dass die Peons ihr Pensum absolvieren!",
    "Sorka! Stell sicher, dass die Peons ihr Pensum absolvieren!",
    "¡Sorka! ¡Mira que los peones mantengan el ritmo!",
    "¡Sorka! ¡Mira que los peones mantengan el ritmo!",
    "Сорка, следи, чтобы батраки держали темп!",
    "Сорка, следи, чтобы батраки держали темп!"
),
(
    87921,
    "Avec plaisir.",
    "Avec plaisir.",
    "Mit Vergnügen.",
    "Mit Vergnügen.",
    "Un placer.",
    "Un placer.",
    "С радостью.",
    "С радостью."
),
(
    87899,
    "Marak ! Charge les munitions dans le canon principal !",
    "Marak ! Charge les munitions dans le canon principal !",
    "Marak! Lade die Munition in die Hauptkanone!",
    "Marak! Lade die Munition in die Hauptkanone!",
    "Marak, ¡carga las municiones en el cañón principal!",
    "Marak, ¡carga las municiones en el cañón principal!",
    "Марак! Заряжай главную пушку!",
    "Марак! Заряжай главную пушку!"
),
(
    87937,
    "D'accord.",
    "D'accord.",
    "Alles klar.",
    "Alles klar.",
    "Bien.",
    "Bien.",
    "Хорошо.",
    "Хорошо."
),
(
    87900,
    "Quoi ? Les trains ont du retard ?",
    "Quoi ? Les trains ont du retard ?",
    "Was? Die Züge fahren nicht mehr nach Zeitplan?",
    "Was? Die Züge fahren nicht mehr nach Zeitplan?",
    "¡¿Qué?! ¿Los trenes van retrasados?",
    "¡¿Qué?! ¿Los trenes van retrasados?",
    "Что?! Поезда задерживаются?",
    "Что?! Поезда задерживаются?"
),
(
    87922,
    "... Je crois savoir d'où ça vient.",
    "... Je crois savoir d'où ça vient.",
    "... Ich denke, ich weiß, wo das Problem liegt.",
    "... Ich denke, ich weiß, wo das Problem liegt.",
    "... Creo que veo el problema.",
    "... Creo que veo el problema.",
    "...Кажется, я знаю, почему.",
    "...Кажется, я знаю, почему."
),
(
    87923,
    "Voyons si ton entraînement avec nos soldats a porté ses fruits, Gar'an.",
    "Voyons si ton entraînement avec nos soldats a porté ses fruits, Gar'an.",
    "Mal sehen, wie gut Ihr unsere Soldaten trainiert habt, Ga'ran.",
    "Mal sehen, wie gut Ihr unsere Soldaten trainiert habt, Ga'ran.",
    "Veamos qué tan bien entrenaste a los soldados, Gar'an.",
    "Veamos qué tan bien entrenaste a los soldados, Gar'an.",
    "Посмотрим, как ты подготовила солдат, Гар'ан.",
    "Посмотрим, как ты подготовила солдат, Гар'ан."
),
(
    87902,
    "Sale petite...",
    "Sale petite...",
    "Du widerspenstiges...",
    "Du widerspenstiges...",
    "¡Eres insoportable!",
    "¡Eres insoportable!",
    "Ах ты, несносная...",
    "Ах ты, несносная..."
),
(
    87924,
    "Ha ! Toute cette brutalité, tout ce sang. C'est magnifique.",
    "Ha ! Toute cette brutalité, tout ce sang. C'est magnifique.",
    "Ha! Welch Brutalität, welch Blutvergießen. Herrlich.",
    "Ha! Welch Brutalität, welch Blutvergießen. Herrlich.",
    "¡Ja! Qué brutalidad, qué matanza. Hermoso.",
    "¡Ja! Qué brutalidad, qué matanza. Hermoso.",
    "Ха! Какая ярость, сколько крови! Красота.",
    "Ха! Какая ярость, сколько крови! Красота."
),
(
    87901,
    "Sorka, on ne joue pas. Descends de là !",
    "Sorka, on ne joue pas. Descends de là !",
    "Sorka, das ist kein Spiel. Runter da!",
    "Sorka, das ist kein Spiel. Runter da!",
    "Sorka, no es un juego. ¡Baja de ahí!",
    "Sorka, no es un juego. ¡Baja de ahí!",
    "Сорка, это тебе не игра! Иди сюда!",
    "Сорка, это тебе не игра! Иди сюда!"
),
(
    87925,
    "Ne t'inquiète pas, j'arrive. On va bien s'amuser.",
    "Ne t'inquiète pas, j'arrive. On va bien s'amuser.",
    "Keine Sorge, ich komme. Könnte lustig werden.",
    "Keine Sorge, ich komme. Könnte lustig werden.",
    "No te preocupes, ya voy. Se ve divertido.",
    "No te preocupes, ya voy. Se ve divertido.",
    "Иду, иду. Будет весело.",
    "Иду, иду. Будет весело."
);

DELETE FROM creature_text WHERE entry IN (@REF_GARAN, @REF_SORKA, @REF_MARAK);
INSERT INTO creature_text VALUES
(@REF_GARAN, 0, 0, "Sisters, to battle!", 14, 0, 100, 0, 0, 41830, "Aggro"),
(@REF_GARAN, 1, 0, "|TInterface\\Icons\\ability_hunter_rapidregeneration.blp:20|t Admiral Gar'an looks towards you and readies |cFFFF0000|Hspell:156626|h[Rapid Fire]|h|r!", 42, 0, 100, 0, 0, 0, "Rapid Fire warn"),
(@REF_GARAN, 2, 0, "Rapid fire!", 14, 0, 100, 0, 0, 41839, "Rapid Fire"),
(@REF_GARAN, 3, 0, "Admiral Gar'an prepares to man the Dreadnaught's Main Cannon!", 41, 0, 100, 0, 0, 0, "Dreadnaught warn"),
(@REF_GARAN, 4, 0, "You will burn!", 14, 0, 100, 0, 0, 41836, "Dreadnaught"),
(@REF_GARAN, 5, 0, "Lining up my sights...", 14, 0, 100, 0, 0, 41837, "Penetrating Shot"),
(@REF_GARAN, 6, 0, "...than battle to death!", 14, 0, 100, 0, 0, 41840, "Iron Will"),
(@REF_GARAN, 7, 0, "None will stand against the Iron Horde!", 14, 0, 100, 0, 0, 41832, "Slay 1"),
(@REF_GARAN, 7, 1, "Enemy down.", 14, 0, 100, 0, 0, 41833, "Slay 2"),
(@REF_GARAN, 8, 0, "Sisters, I... failed...", 14, 0, 100, 0, 0, 41831, "Death"),

(@REF_SORKA, 0, 0, "I'm going to enjoy this...", 14, 0, 100, 0, 0, 41856, "Aggro"),
(@REF_SORKA, 1, 0, "Too slow!", 14, 0, 100, 0, 0, 41861, "Blade Dash"),
(@REF_SORKA, 2, 0, "Catch!", 14, 0, 100, 0, 0, 41862, "Convulsive Shadows"),
(@REF_SORKA, 3, 0, "You...", 14, 0, 100, 0, 0, 41863, "Dark Hunt begin"),
(@REF_SORKA, 4, 0, "...die now!", 14, 0, 100, 0, 0, 41864, "Dark Hunt end"),
(@REF_SORKA, 5, 0, "Enforcer Sorka prepares to man the Dreadnaught's Main Cannon!", 41, 0, 100, 0, 0, 0, "Dreadnaught warn"),
(@REF_SORKA, 6, 0, "All will burn!", 14, 0, 100, 0, 0, 41865, "Dreadnaught"),
(@REF_SORKA, 7, 0, "No greater pleasure...", 14, 0, 100, 0, 0, 41866, "Iron Will"),
(@REF_SORKA, 8, 0, "Oh, look... I broke it.", 14, 0, 100, 0, 0, 41858, "Slay 1"),
(@REF_SORKA, 8, 1, "I'm just too good.", 14, 0, 100, 0, 0, 41859, "Slay 2"),
(@REF_SORKA, 9, 0, "It feels... so...", 14, 0, 100, 0, 0, 41857, "Death"),

(@REF_MARAK, 0, 0, "Time to die.", 14, 0, 100, 0, 0, 41711, "Aggro"),
(@REF_MARAK, 1, 0, "Death...", 14, 0, 100, 0, 0, 41717, "Blood Ritual"),
(@REF_MARAK, 2, 0, "Marak the Blooded begins to channel a |cFFFF0000|Hspell:158079|h[Blood Ritual]|h|r on you!", 42, 0, 100, 0, 0, 0, "Blood Ritual warn"),
(@REF_MARAK, 3, 0, "Ancestors, guide my axe...", 14, 0, 100, 0, 0, 41718, "Bloodsoaked Heartseeker 1"),
(@REF_MARAK, 3, 1, "Dance with my blade!", 14, 0, 100, 0, 0, 41716, "Bloodsoaked Heartseeker 2"),
(@REF_MARAK, 4, 0, "You will know pain...", 14, 0, 100, 0, 0, 41719, "Dreadnaught"),
(@REF_MARAK, 5, 0, "Marak the Blooded prepares to man the Dreadnaught's Main Cannon!", 41, 0, 100, 0, 0, 0, "Dreadnaught warn"),
(@REF_MARAK, 6, 0, "Lok'tar ogar...", 14, 0, 100, 0, 0, 41720, "Iron Will"),
(@REF_MARAK, 7, 0, "Feast, Koloch'na!", 14, 0, 100, 0, 0, 41713, "Slay 1"),
(@REF_MARAK, 7, 1, "I win.", 14, 0, 100, 0, 0, 41714, "Slay 2"),
(@REF_MARAK, 8, 0, "My... blood...", 14, 0, 100, 0, 0, 41712, "Death");

DELETE FROM locales_creature_text WHERE entry IN (@REF_GARAN, @REF_SORKA, @REF_MARAK);
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    @REF_GARAN, 0, 0,
    "Mes sœurs ! Au combat !",
    "In den Kampf, Schwestern!",
    "¡Hermanas! ¡A la batalla!",
    "Сестры! К бою!"
),
(
    @REF_GARAN, 1, 0,
    "|TInterface\\Icons\\ability_hunter_rapidregeneration.blp:20|tL'Amiral Garan vous regarde et prépare |cFFFF0000|Hspell:156626|h[Tir rapide]|h|r !",
    "|TInterface\\Icons\\ability_hunter_rapidregeneration.blp:20|t Admiralin Gar'an schaut Euch an und bereitet |cFFFF0000|Hspell:156626|h[Schnellfeuer]|h|r vor!",
    "|TInterface\\Icons\\ability_hunter_rapidregeneration.blp:20|t La almirante Gar'an voltea a verte y prepara |cFFFF0000|Hspell:156626|h[Fuego rápido]|h|r.",
    "|TInterface\\Icons\\ability_hunter_rapidregeneration.blp:20|t Адмирал Гар'ан оглядывается на вас и готовится открыть |cFFFF0000|Hspell:156626|h[беглый огонь]|h|r!"
),
(
    @REF_GARAN, 2, 0,
    "Tir en rafale... !",
    "Schnellfeuer...!",
    "¡Fuego rápido...!",
    "Беглый огонь!.."
),
(
    @REF_GARAN, 3, 0,
    "L'amiral Gar'an s'apprête à employer le canon principal du Cuirassier !",
    "Admiralin Gar'an bereitet sich darauf vor, die Hauptkanone des Schlachtschiffs zu bemannen!",
    "¡La almirante Gar'an se prepara para manejar el cañón principal del Acorazado!",
    "Адмирал Гар'ан готовится занять позицию у главного орудия дредноута!"
),
(
    @REF_GARAN, 4, 0,
    "Vous... allez... brûler !",
    "Ihr... werdet... brennen!",
    "¡Tu... vas... a arder!",
    "Ты... сгоришь!"
),
(
    @REF_GARAN, 5, 0,
    "En ligne de mire...",
    "Schön stillhalten...",
    "Alineando la mira...",
    "Прицеливаюсь..."
),
(
    @REF_GARAN, 6, 0,
    "... qu'un combat jusqu'à la mort !",
    "... als einen Kampf auf Leben und Tod!",
    "... ¡pelear hasta la muerte!",
    "...чем смертельная битва!"
),
(
    @REF_GARAN, 7, 0,
    "Personne ne tiendra tête à la Horde de Fer.",
    "Niemand widersetzt sich der Eisernen Horde.",
    "Nadie podrá contra la Horda de Hierro.",
    "Все падут пред Железной Ордой."
),
(
    @REF_GARAN, 7, 1,
    "Ennemi à terre.",
    "Feind gefallen.",
    "Enemigo derribado.",
    "Один готов."
),
(
    @REF_GARAN, 8, 0,
    "Mes sœurs... j'ai... échoué...",
    "Schwestern... Ich... habe versagt...",
    "Hermanas... he... fallado...",
    "Сестры... я... подвела... вас..."
),
(
    @REF_SORKA, 0, 0,
    "Je sens que ça va me plaire.",
    "Das werde ich genießen.",
    "Voy a disfrutar esto...",
    "Это мне по нраву."
),
(
    @REF_SORKA, 1, 0,
    "Trop lent.",
    "Zu langsam.",
    "¡Muy lento!",
    "Не успели."
),
(
    @REF_SORKA, 2, 0,
    "Attrapez !",
    "Fang!",
    "¡Atrápalo!",
    "Лови!"
),
(
    @REF_SORKA, 3, 0,
    "Vous...",
    "Du...",
    "Tú...",
    "Ты..."
),
(
    @REF_SORKA, 4, 0,
    "... allez mourir !",
    "...wirst jetzt sterben!",
    "... ¡muere ahora!",
    "...умрешь!"
),
(
    @REF_SORKA, 5, 0,
    "La massacreuse Sorka s'apprête à employer le canon principal du Cuirassier !",
    "Vollstreckerin Sorka bereitet sich darauf vor, die Hauptkanone des Schlachtschiffs zu bemannen!",
    "¡La déspota Sorka se prepara para manejar el cañón principal del Acorazado!",
    "Сорка Отчаянная готовится занять позицию у главного орудия дредноута!"
),
(
    @REF_SORKA, 6, 0,
    "Vous brûlerez tous !",
    "Alles wird brennen!",
    "¡Todo arderá!",
    "Сгорите все!"
),
(
    @REF_SORKA, 7, 0,
    "Il n'existe rien de plus beau...",
    "Es gibt kein größeres Vergnügen...",
    "No hay mayor placer...",
    "Нет радости больше..."
),
(
    @REF_SORKA, 8, 0,
    "Allons bon... je l'ai cassé !",
    "Oh, nein... Ich hab's kaputt gemacht!",
    "Oh, mira... ¡lo rompí!",
    "Ой... сломалось!"
),
(
    @REF_SORKA, 8, 1,
    "J'ai du talent pour ça.",
    "Ich bin einfach... zu gut.",
    "Es que soy... muy buena.",
    "Я великолепна."
),
(
    @REF_SORKA, 9, 0,
    "Je me... sens...",
    "Es fühlt sich... so...",
    "Se siente... tan...",
    "Это... так..."
),
(
    @REF_MARAK, 0, 0,
    "C'est l'heure de mourir.",
    "Zeit zu sterben.",
    "Hora de morir.",
    "Пора умирать."
),
(
    @REF_MARAK, 1, 0,
    "La mort...",
    "Tod...",
    "Muerte...",
    "Смерть..."
),
(
    @REF_MARAK, 2, 0,
    "Marak Premier-sang commence à canaliser |cFFFF0000|Hspell:158079|h[Rituel de sang]|h|r sur vous !",
    "Marak die Blutige beginnt, ein |cFFFF0000|Hspell:158079|h[Blutritual]|h|r auf Euch zu wirken!",
    "¡Marak la Ensangrentada comienza a canalizar un |cFFFF0000|Hspell:158079|h[Ritual de sangre]|h|r sobre ti!",
    "Марак Кровавая выбирает вас целью и начинает |cFFFF0000|Hspell:158079|h[ритуал крови]|h|r!"
),
(
    @REF_MARAK, 3, 0,
    "Ancêtres, guidez ma hache !",
    "Ihr Ahnen, leitet meine Axt!",
    "¡Ancestros, guíen mi hacha!",
    "Да направят предки мой топор!"
),
(
    @REF_MARAK, 3, 1,
    "Dansez avec ma lame.",
    "Tanzt mit meiner Klinge.",
    "Baila con mi hacha.",
    "Лови топор!"
),
(
    @REF_MARAK, 4, 0,
    "Vous allez connaître la douleur...",
    "Ihr werdet erfahren, was Schmerz ist...",
    "Conocerán el dolor...",
    "Познайте боль..."
),
(
    @REF_MARAK, 5, 0,
    "Marak Premier-sang s'apprête à employer le canon principal du Cuirassier !",
    "Marak die Blutige bereitet sich darauf vor, die Hauptkanone des Schlachtschiffs zu bemannen!",
    "¡Marak la Ensangrentada se prepara para manejar el cañón principal del Acorazado!",
    "Марак Кровавая готовится занять позицию у главного орудия дредноута!"
),
(
    @REF_MARAK, 6, 0,
    "Lok'tar ogar !",
    "Lok'tar ogar!",
    "¡Lok'tar ogar!",
    "Лок'тар огар!"
),
(
    @REF_MARAK, 7, 0,
    "Profite du festin, Koloch'na...",
    "Labe dich, Koloch'na...",
    "Come, Koloch'na...",
    "Пируй, Колох..."
),
(
    @REF_MARAK, 7, 1,
    "J'ai gagné.",
    "Ich gewinne.",
    "Gané.",
    "Тебе конец."
),
(
    @REF_MARAK, 8, 0,
    "Mon... sang...",
    "Mein... Blut...",
    "Mi... sangre...",
    "Моя... кровь..."
);

DELETE FROM creature_text WHERE entry = @REF_COSMETIC_BLACKHAND AND groupid = 13;
INSERT INTO creature_text VALUE
(@REF_COSMETIC_BLACKHAND, 13, 0, "The Iron Maidens have fallen? They were my favorite! Find these invaders and bring me their heads!", 14, 0, 100, 0, 0, 45461, "Maidens Killed");

DELETE FROM locales_creature_text WHERE entry = @REF_COSMETIC_BLACKHAND AND textGroup = 13;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUE
(
    @REF_COSMETIC_BLACKHAND, 13, 0,
    "Les vierges de Fer sont mortes ? C'étaient mes favorites ! Trouvez ces envahisseurs et rapportez-moi leurs têtes !",
    "Die Eisernen Jungfern sind gefallen? Sie waren mir die Liebsten! Findet diese Maden und bringt mir ihre Köpfe!",
    "¿Las Doncellas de Hierro murieron? ¡Eran mis favoritas! ¡Encuentra a esos invasores y tráiganme sus cabezas!",
    "Железные леди пали? Мои любимицы! Найдите чужаков и принесите мне их головы!"
);

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

DELETE FROM creature_groupsizestats WHERE entry = 78351;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78351, 14, 10, 5026400),
(78351, 14, 11, 5026400),
(78351, 14, 12, 5026400),
(78351, 14, 13, 5026400),
(78351, 14, 14, 5026400),
(78351, 14, 15, 5026400),
(78351, 14, 16, 5026400),
(78351, 14, 17, 5026400),
(78351, 14, 18, 5026400),
(78351, 14, 19, 5026400),
(78351, 14, 20, 5026400),
(78351, 14, 21, 5026400),
(78351, 14, 22, 5026400),
(78351, 14, 23, 5026400),
(78351, 14, 24, 5026400),
(78351, 14, 25, 5026400),
(78351, 14, 26, 5026400),
(78351, 14, 27, 5026400),
(78351, 14, 28, 5026400),
(78351, 14, 29, 5026400),
(78351, 14, 30, 5026400),
(78351, 15, 10, 7539696),
(78351, 15, 11, 7539696),
(78351, 15, 12, 7539696),
(78351, 15, 13, 7539696),
(78351, 15, 14, 7539696),
(78351, 15, 15, 7539696),
(78351, 15, 16, 7539696),
(78351, 15, 17, 7539696),
(78351, 15, 18, 7539696),
(78351, 15, 19, 7539696),
(78351, 15, 20, 7539696),
(78351, 15, 21, 7539696),
(78351, 15, 22, 7539696),
(78351, 15, 23, 7539696),
(78351, 15, 24, 7539696),
(78351, 15, 25, 7539696),
(78351, 15, 26, 7539696),
(78351, 15, 27, 7539696),
(78351, 15, 28, 7539696),
(78351, 15, 29, 7539696),
(78351, 15, 30, 7539696);

DELETE FROM creature_groupsizestats WHERE entry = 78352;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78352, 14, 10, 8377445),
(78352, 14, 11, 8377445),
(78352, 14, 12, 8377445),
(78352, 14, 13, 8377445),
(78352, 14, 14, 8377445),
(78352, 14, 15, 8377445),
(78352, 14, 16, 8377445),
(78352, 14, 17, 8377445),
(78352, 14, 18, 8377445),
(78352, 14, 19, 8377445),
(78352, 14, 20, 8377445),
(78352, 14, 21, 8377445),
(78352, 14, 22, 8377445),
(78352, 14, 23, 8377445),
(78352, 14, 24, 8377445),
(78352, 14, 25, 8377445),
(78352, 14, 26, 8377445),
(78352, 14, 27, 8377445),
(78352, 14, 28, 8377445),
(78352, 14, 29, 8377445),
(78352, 14, 30, 8377445),
(78352, 15, 10, 12566160),
(78352, 15, 11, 12566160),
(78352, 15, 12, 12566160),
(78352, 15, 13, 12566160),
(78352, 15, 14, 12566160),
(78352, 15, 15, 12566160),
(78352, 15, 16, 12566160),
(78352, 15, 17, 12566160),
(78352, 15, 18, 12566160),
(78352, 15, 19, 12566160),
(78352, 15, 20, 12566160),
(78352, 15, 21, 12566160),
(78352, 15, 22, 12566160),
(78352, 15, 23, 12566160),
(78352, 15, 24, 12566160),
(78352, 15, 25, 12566160),
(78352, 15, 26, 12566160),
(78352, 15, 27, 12566160),
(78352, 15, 28, 12566160),
(78352, 15, 29, 12566160),
(78352, 15, 30, 12566160);

DELETE FROM creature_groupsizestats WHERE entry = 78341;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78341, 14, 10, 5445486),
(78341, 14, 11, 5445486),
(78341, 14, 12, 5445486),
(78341, 14, 13, 5445486),
(78341, 14, 14, 5445486),
(78341, 14, 15, 5445486),
(78341, 14, 16, 5445486),
(78341, 14, 17, 5445486),
(78341, 14, 18, 5445486),
(78341, 14, 19, 5445486),
(78341, 14, 20, 5445486),
(78341, 14, 21, 5445486),
(78341, 14, 22, 5445486),
(78341, 14, 23, 5445486),
(78341, 14, 24, 5445486),
(78341, 14, 25, 5445486),
(78341, 14, 26, 5445486),
(78341, 14, 27, 5445486),
(78341, 14, 28, 5445486),
(78341, 14, 29, 5445486),
(78341, 14, 30, 5445486),
(78341, 15, 10, 8377440),
(78341, 15, 11, 8377440),
(78341, 15, 12, 8377440),
(78341, 15, 13, 8377440),
(78341, 15, 14, 8377440),
(78341, 15, 15, 8377440),
(78341, 15, 16, 8377440),
(78341, 15, 17, 8377440),
(78341, 15, 18, 8377440),
(78341, 15, 19, 8377440),
(78341, 15, 20, 8377440),
(78341, 15, 21, 8377440),
(78341, 15, 22, 8377440),
(78341, 15, 23, 8377440),
(78341, 15, 24, 8377440),
(78341, 15, 25, 8377440),
(78341, 15, 26, 8377440),
(78341, 15, 27, 8377440),
(78341, 15, 28, 8377440),
(78341, 15, 29, 8377440),
(78341, 15, 30, 8377440);

DELETE FROM creature_groupsizestats WHERE entry = 78343;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78343, 14, 10, 5026464),
(78343, 14, 11, 5026464),
(78343, 14, 12, 5026464),
(78343, 14, 13, 5026464),
(78343, 14, 14, 5026464),
(78343, 14, 15, 5026464),
(78343, 14, 16, 5026464),
(78343, 14, 17, 5026464),
(78343, 14, 18, 5026464),
(78343, 14, 19, 5026464),
(78343, 14, 20, 5026464),
(78343, 14, 21, 5026464),
(78343, 14, 22, 5026464),
(78343, 14, 23, 5026464),
(78343, 14, 24, 5026464),
(78343, 14, 25, 5026464),
(78343, 14, 26, 5026464),
(78343, 14, 27, 5026464),
(78343, 14, 28, 5026464),
(78343, 14, 29, 5026464),
(78343, 14, 30, 5026464),
(78343, 15, 10, 6283080),
(78343, 15, 11, 6283080),
(78343, 15, 12, 6283080),
(78343, 15, 13, 6283080),
(78343, 15, 14, 6283080),
(78343, 15, 15, 6283080),
(78343, 15, 16, 6283080),
(78343, 15, 17, 6283080),
(78343, 15, 18, 6283080),
(78343, 15, 19, 6283080),
(78343, 15, 20, 6283080),
(78343, 15, 21, 6283080),
(78343, 15, 22, 6283080),
(78343, 15, 23, 6283080),
(78343, 15, 24, 6283080),
(78343, 15, 25, 6283080),
(78343, 15, 26, 6283080),
(78343, 15, 27, 6283080),
(78343, 15, 28, 6283080),
(78343, 15, 29, 6283080),
(78343, 15, 30, 6283080);

DELETE FROM creature_groupsizestats WHERE entry = 78347;
INSERT INTO creature_groupsizestats (entry, difficulty, groupSize, health) VALUES
(78347, 14, 10, 404983),
(78347, 14, 11, 404983),
(78347, 14, 12, 404983),
(78347, 14, 13, 404983),
(78347, 14, 14, 404983),
(78347, 14, 15, 404983),
(78347, 14, 16, 404983),
(78347, 14, 17, 404983),
(78347, 14, 18, 404983),
(78347, 14, 19, 404983),
(78347, 14, 20, 404983),
(78347, 14, 21, 404983),
(78347, 14, 22, 404983),
(78347, 14, 23, 404983),
(78347, 14, 24, 404983),
(78347, 14, 25, 404983),
(78347, 14, 26, 404983),
(78347, 14, 27, 404983),
(78347, 14, 28, 404983),
(78347, 14, 29, 404983),
(78347, 14, 30, 404983),
(78347, 15, 10, 506150),
(78347, 15, 11, 506150),
(78347, 15, 12, 506150),
(78347, 15, 13, 506150),
(78347, 15, 14, 506150),
(78347, 15, 15, 506150),
(78347, 15, 16, 506150),
(78347, 15, 17, 506150),
(78347, 15, 18, 506150),
(78347, 15, 19, 506150),
(78347, 15, 20, 506150),
(78347, 15, 21, 506150),
(78347, 15, 22, 506150),
(78347, 15, 23, 506150),
(78347, 15, 24, 506150),
(78347, 15, 25, 506150),
(78347, 15, 26, 506150),
(78347, 15, 27, 506150),
(78347, 15, 28, 506150),
(78347, 15, 29, 506150),
(78347, 15, 30, 506150);