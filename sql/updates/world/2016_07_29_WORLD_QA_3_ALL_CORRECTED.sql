DELETE FROM item_loot_template WHERE entry = 94553;
INSERT INTO item_loot_template VALUES (94553, 94571, 100, 1, 0, 1, 1, ""),
(94553, 94569, 100, 1, 0, 1, 1, ""),
(94553, 94568, 100, 1, 0, 1, 1, ""),
(94553, 94570, 100, 1, 0, 1, 1, ""),
(94553, 94552, 100, 1, 0, 1, 1, ""),
(94553, 94572, 100, 1, 0, 1, 1, ""),
(94553, 94567, 100, 1, 0, 1, 1, "");
DELETE FROM skill_extra_item_template WHERE SpellID = 172540;
INSERT INTO skill_extra_item_template VALUE (172540, 158741, 100, 3);
UPDATE spell_proc_event SET ppmRate = 8.5 WHERE  entry = 51530;DELETE FROM spell_script_names WHERE spell_id = 126389 AND ScriptName = "spell_gen_golbin_glider";
INSERT INTO spell_script_names VALUE(126389,"spell_gen_golbin_glider");DELETE FROM spell_script_names WHERE spell_Id = 143646 AND ScriptName = "spell_item_Plans_Balanced_Trillium_Ingot_and_Its_Uses";
INSERT INTO spell_script_names VALUE (143646, "spell_item_Plans_Balanced_Trillium_Ingot_and_Its_Uses");DELETE FROM spell_script_names WHERE spell_id = 167950 AND ScriptName = "spell_inscription_research";DELETE FROM spell_script_names WHERE spell_id IN (107903, 107904) AND ScriptName = "spell_pri_glyph_of_shadowy_friend";
INSERT INTO spell_script_names VALUE (107903, "spell_pri_glyph_of_shadowy_friend");
INSERT INTO spell_script_names VALUE (107904, "spell_pri_glyph_of_shadowy_friend");UPDATE creature SET phasemask = 4294967295 WHERE id = 70860;
DELETE FROM smart_scripts WHERE entryorguid = 70860 AND id = 0;

UPDATE quest_template_objective SET flags = 0 WHERE id = 271034;
UPDATE quest_template_objective SET flags = 2 WHERE id = 273038;
UPDATE creature_template SET ainame = "SmartAI" WHERE entry IN (74811, 174811, 174812, 174813);DELETE FROM spell_script_names WHERE spell_id = 125557 AND ScriptName = "spell_Tailoring_Research";
INSERT INTO spell_script_names VALUE (125557, "spell_Tailoring_Research");
DELETE FROM skill_extra_item_template WHERE spellID IN (156563,156569,156572,156576,156560,156561,156571,156564,156568,156584,162403,156582,175869,175866,175868,175853,175867,175865);
INSERT INTO skill_extra_item_template VALUES(156563, 28677, 17, 4),
(156569, 28677, 17, 4),
(156572, 28677, 17, 4),
(156576, 28677, 17, 4),
(156560, 28677, 17, 4),
(156561, 28677, 17, 4),
(156571, 28677, 17, 4),
(156564, 28677, 17, 4),
(156568, 28677, 17, 4),
(156584, 28677, 17, 4),
(162403, 28677, 17, 4),
(156582, 28677, 17, 4),
(175869, 28677, 17, 4),
(175866, 28677, 17, 4),
(175868, 28677, 17, 4),
(175853, 28677, 17, 4),
(175867, 28677, 17, 4),
(175865, 28677, 17, 4);DELETE FROM spell_upgrade_item_stage WHERE ItemBonusTreeCategory IN(197, 200, 219, 204, 201, 202, 205);

INSERT INTO spell_upgrade_item_stage VALUES
(197, 2, 0, 0x20000, 	675),
(197, 4, 0, 0x1000,	685),
(197, 4, 0, 0x800000,   675),
(200, 4, 4, 0x3fa, 	685),
(219, 4, 6, 0x4000,	675),
(219, 2, 0, 0x22000, 	675),
(204, 2, 0, 0x4000000,	675),
(204, 4, 0, 0x2,	685),
(201, 4, 2, 0x103fa,	685),
(202, 4, 1, 0x103fa,	685),
(205, 4, 0, 0x804,	685);

SELECT * FROM spell_script_names WHERE ScriptName IN ("spell_skill_taladite_amplifier_upgrade", "spell_skill_burnished_essence_upgrade", "spell_skill_steelforged_essence_upgrade", "spell_skill_truesteel_essence_upgrade", "spell_skill_true_iron_trigger_upgrade", "spell_skill_linkgrease_locksprocket_upgrade", "spell_skill_weapon_crystal_upgrade", "spell_skill_hexweave_essence_upgrade", "spell_skill_upgrade_armor_or_weapon");
DELETE FROM spell_script_names WHERE ScriptName IN ("spell_skill_taladite_amplifier_upgrade", "spell_skill_burnished_essence_upgrade", "spell_skill_steelforged_essence_upgrade", "spell_skill_truesteel_essence_upgrade", "spell_skill_true_iron_trigger_upgrade", "spell_skill_linkgrease_locksprocket_upgrade", "spell_skill_weapon_crystal_upgrade", "spell_skill_hexweave_essence_upgrade", "spell_skill_upgrade_armor_or_weapon");
INSERT INTO spell_script_names VALUES
(197546, "spell_skill_upgrade_armor_or_weapon"),
(187537, "spell_skill_upgrade_armor_or_weapon"),
(187541, "spell_skill_upgrade_armor_or_weapon"),
(187539, "spell_skill_upgrade_armor_or_weapon"),
(187538, "spell_skill_upgrade_armor_or_weapon"),
(187551, "spell_skill_upgrade_armor_or_weapon"),
(187550, "spell_skill_upgrade_armor_or_weapon"),
(187552, "spell_skill_upgrade_armor_or_weapon"),
(187535, "spell_skill_upgrade_armor_or_weapon");DELETE FROM spell_script_names WHERE spell_id = 84342 AND ScriptName IN ("spell_Item_Loot_A_Rang");
INSERT INTO spell_script_names VALUE(84342, "spell_Item_Loot_A_Rang");DELETE FROM spell_script_names WHERE spell_id = 155166 AND ScriptName = "spell_dk_mark_of_sindragosa_damage";
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES ( 155166,'spell_dk_mark_of_sindragosa_damage'); DELETE FROM spell_script_names WHERE spell_id = 84342 AND ScriptName IN ("spell_item_loot_a_rang", "spell_Item_Loot_A_Rang");
INSERT INTO spell_script_names VALUE (84342, "spell_item_loot_a_rang");UPDATE quest_template SET RequiredRaces = 18875469 WHERE id = 11821;

DELETE FROM creature_queststarter WHERE id = 25937 AND quest = 11821;
DELETE FROM creature_queststarter WHERE id = 25903 AND quest = 11821;
INSERT INTO creature_queststarter VALUE (25903,11821);


DELETE FROM creature_questender WHERE id = 25903 AND quest = 11821;
INSERT INTO creature_questender VALUE (25903, 11821);UPDATE quest_template SET flags =0x200000 WHERE id = 32307;
UPDATE quest_template SET flags =0x200000 WHERE id = 32309;
UPDATE quest_template SET flags =0x200000 WHERE id = 32310;
UPDATE quest_template SET flags =0x210000 WHERE id = 32317;
UPDATE quest_template SET flags =0x210000 WHERE id = 32324;
UPDATE quest_template SET flags =0x200000 WHERE id = 32325;DELETE FROM spell_script_names WHERE ScriptName = "spell_warl_command_demon_spells" AND spell_id IN (171140, 171152, 171154);
INSERT INTO spell_script_names VALUES(171140, "spell_warl_command_demon_spells"),
(171152, "spell_warl_command_demon_spells"),
(171154, "spell_warl_command_demon_spells");UPDATE spell_upgrade_item_stage SET InventoryTypeMask = 0x220000 WHERE ItemBonusTreeCategory = 197 AND itemClass = 2;UPDATE creature_template SET ScriptName = "npc_imp" WHERE entry = 416;UPDATE spell_upgrade_item_stage SET ItemSubClassMask = 0x40 WHERE ItemBonusTreeCategory = 219 AND ItemClass = 4;
UPDATE spell_upgrade_item_stage SET ItemSubClassMask = 0x10 WHERE ItemBonusTreeCategory = 200 AND ItemClass = 4;
UPDATE spell_upgrade_item_stage SET itemsubclassmask = 0x4 WHERE ItemBonusTreeCategory = 201 AND ItemClass = 4;
UPDATE spell_upgrade_item_stage SET itemsubclassmask = 0x2 WHERE ItemBonusTreeCategory = 202 AND ItemClass = 4;DELETE FROM spell_script_names WHERE spell_id = 56248 AND ScriptName = "spell_warl_glyph_of_hand_of_guldan";
INSERT INTO spell_script_names VALUE (56248, "spell_warl_glyph_of_hand_of_guldan");

DELETE FROM spell_script_names WHERE spell_id = 123197 AND ScriptName = "spell_warl_hand_of_guldan_damage";
INSERT INTO spell_script_names VALUE (123197, "spell_warl_hand_of_guldan_damage");UPDATE gameobject_loot_template SET maxcount = 1 WHERE item = 109586;UPDATE creature_template SET npcflag = npcflag | 1 WHERE entry = 16475;DELETE FROM spell_script_names WHERE spell_id = 126 AND ScriptName = "spell_warl_eye_of_kilrogg";
INSERT INTO spell_script_names VALUE (126, "spell_warl_eye_of_kilrogg");DELETE FROM spell_script_names WHERE spell_id = 53253 AND ScriptName = "spell_hun_invigoration";
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES ( '53253','spell_hun_invigoration'); 

DELETE FROM spell_script_names WHERE spell_id IN (135920,53397) AND ScriptName = "spell_hun_invigoration_proc";
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES ( '53397','spell_hun_invigoration_proc'); DELETE FROM spell_upgrade_item_stage WHERE ItemBonusTreeCategory = 201;
DELETE FROM spell_upgrade_item_stage WHERE ItemBonusTreeCategory = 202;
INSERT INTO spell_upgrade_item_stage VALUES
(201, 4, 4, 1018, 685),
(201, 4, 8, 1018, 685),
(201, 4, 0, 65536, 685), 
(202, 4, 2, 1018, 685),
(202, 4, 0, 65536, 685);DELETE FROM `conditions` WHERE SourceEntry = 101883;
INSERT INTO `conditions`
            (`SourceTypeOrReferenceId`,
             `SourceGroup`,
             `SourceEntry`,
             `SourceId`,
             `ElseGroup`,
             `ConditionTypeOrReference`,
             `ConditionTarget`,
             `ConditionValue1`,
             `ConditionValue2`,
             `ConditionValue3`,
             `NegativeCondition`,
             `ErrorTextId`,
             `ScriptName`,
             `Comment`)
VALUES ('17',
        '0',
        '101883',
        '0',
        '0',
        '4',
        '0',
        '5861',
        '0',
        '0',
        '0',
        '0',
        '',
        'Heal Injured Carnie only usable on ShadowMoon');REPLACE INTO `creature_template` (`entry`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `femaleName`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_req`, `faction`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `dmgschool`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `baseVariance`, `rangeVariance`, `unit_class`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `TrackingQuestID`, `VignetteID`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `BuildVerified`) VALUES ('250000','0','0','27614','0','0','0','Jeanne',NULL,'Loyalty Points','','0','103','103','2','0','35','1','0','1','1.14286','1.14286','0.5','4','127','500','500','500','1','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','1','4','1','1.5','1','1','1','0','0','0','0','0','0','0','0','0','0','1','0','0','0','npc_loyalty_point','1'); 
DELETE FROM npc_text WHERE ID IN (92007, 92008);
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    92007,
    "Oh, so you came here to receive your loyalty points? Well, let me tell you! I have already given you your loyalty reward points, enjoy spending them on the Firestorm Shop. For more information about the loyalty points visit: http://fstorm.cc/loyalty-point.",
    "Oh, so you came here to receive your loyalty points? Well, let me tell you! I have already given you your loyalty reward points, enjoy spending them on the Firestorm Shop. For more information about the loyalty points visit: http://fstorm.cc/loyalty-point."
),
(
    92008,
    "Oh no! It looks like you have already claimed your loyalty reward points, please come back tomorrow!",
    "Oh no! It looks like you have already claimed your loyalty reward points, please come back tomorrow!"
);

DELETE FROM locales_npc_text WHERE entry IN (92007, 92008);

--                                   French                      Spanish
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc6, Text0_1_loc6) VALUES
(
    92007,
    "Bonjour $c, vous venez récupérer vos points de fidélité ? Tenez, les voici. Vous pouvez les utiliser sur la boutique du site internet firestorm. Pour plus d'informations à propos des points de fidélité, rendez-vous à l'adresse : http://fstorm.cc/point-fidelite",
    "Bonjour $c, vous venez récupérer vos points de fidélité ? Tenez, les voici. Vous pouvez les utiliser sur la boutique du site internet firestorm. Pour plus d'informations à propos des points de fidélité, rendez-vous à l'adresse : http://fstorm.cc/point-fidelite",
    "¡Acabo de hacer entrega de tus puntos de lealtad! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad",
    "¡Acabo de hacer entrega de tus puntos de lealtad! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad"
),
(
    92008,
    "Tu as déjà récupéré tes points de fidélité pour aujourd'hui ! Reviens demain.",
    "Tu as déjà récupéré tes points de fidélité pour aujourd'hui ! Reviens demain.",
	"¡Ya hice entrega de los puntos de lealtad que merecías antes! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad",
    "¡Ya hice entrega de los puntos de lealtad que merecías antes! Estos puntos pueden ser gastados en la página web de Firestorm www.fstorm.cc/W. Puedes obtener más información en este post del foro: www.fstorm.cc/Lealtad"
);

DELETE FROM `trinity_string` WHERE `entry` = 14116;
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `content_loc9`, `content_loc10`) VALUES('14116','You\'ve earn %u loyalty points!',NULL,'Vous avez gagné %u points de fidélité !',NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);DELETE FROM spell_script_names WHERE spell_id = 123986 AND ScriptName = "spell_monk_chi_burst_cast";
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES ( '123986','spell_monk_chi_burst_cast'); DELETE FROM spell_script_names WHERE spell_id = 104135 AND ScriptName = "spell_warl_demonic_circle_summon";
INSERT INTO spell_script_names VALUE (104135, "spell_warl_demonic_circle_summon");UPDATE spell_loot_template SET item = 109140 WHERE entry IN (161272, 161281);UPDATE gameobject_template SET data1 = 17500 WHERE entry IN (180683, 180752);
UPDATE prospecting_loot_template SET ChanceOrQuestChance = 100 WHERE Entry = 23425 AND item = 24243;DELETE FROM spell_script_names WHERE spell_id IN (161001, 160962, 160968, 160971, 160981, 160982, 160978, 160958, 160966, 160969, 160983, 160973, 160979, 160986, 160999, 160987, 161000, 160989, 160984, 173978, 173979, 161002) AND ScriptName != "spell_Cooking_DraenorRecipesRewards";

INSERT INTO spell_script_names VALUES
(161002,"spell_Skill_Cooking_GrilledSaberfish"),
(161001,"spell_Skill_Cooking_SaberfishBroth"),
(160962,"spell_Skill_Cooking_BlackrockHam"),
(160968,"spell_Skill_Cooking_BraisedRiverBeast"),
(160971,"spell_Skill_Cooking_ClefhoofSausages"),
(160981,"spell_Skill_Cooking_FatSleeperCakes"),
(160982,"spell_Skill_Cooking_FieryCalamari"),
(160978,"spell_Skill_Cooking_GrilledGulper"),
(160958,"spell_Skill_Cooking_HeartyElekkSteak"),
(160966,"spell_Skill_Cooking_PanSearedTalbuk"),
(160969,"spell_Skill_Cooking_RylakCrepes"),
(160983,"spell_Skill_Cooking_SkulkerChowder"),
(160973,"spell_Skill_Cooking_SteamedScorpion"),
(160979,"spell_Skill_Cooking_SturgeonStew"),
(160986,"spell_Skill_Cooking_BlackrockBarbecue"),
(160999,"spell_Skill_Cooking_CalamariCrepes"),
(160987,"spell_Skill_Cooking_FrostyStew"),
(161000,"spell_Skill_Cooking_GrogrondChowder"),
(160989,"spell_Skill_Cooking_SleeperSurprise"),
(160984,"spell_Skill_Cooking_TaladorSurfAndTurf"),
(173978,"spell_Skill_Cooking_FeastOfBlood"),
(173979,"spell_Skill_Cooking_FeastOfTheWaters");UPDATE `item_bonus_group` SET bonus=CONCAT(bonus, '301,302,303,304,305,306,307,308,309,310,311,312,313,314,315,316,317,318,319,320,321') WHERE id = 3;SELECT * FROM spell_loot_template WHERE entry IN (161283);




UPDATE spell_loot_template SET ChanceOrQuestChance = 100, groupid = 0,  mincountOrRef =20, maxcount = 35 WHERE entry = 161277; -- Enormous see Scorpion

UPDATE spell_loot_template SET maxcount = 10 WHERE entry = 161231;		-- Small Blind Lake Sturgeon
DELETE FROM spell_loot_template WHERE entry = 161231 AND item = 109139;

UPDATE spell_loot_template SET maxcount = 10 WHERE entry = 161232;	-- Small Fat Sleeper
UPDATE spell_loot_template SET ChanceOrQuestChance = 100, groupid = 0,  mincountOrRef =10, maxcount = 25 WHERE entry = 161273; -- Fat Sleeper
UPDATE spell_loot_template SET ChanceOrQuestChance = 100, groupid = 0,  mincountOrRef =20, maxcount = 35 WHERE entry = 161283; -- Enormous Fat Sleeper
DELETE FROM spell_script_names WHERE spell_id = 132563 AND ScriptName = "spell_item_throw_pigskin";
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES ( '132563','spell_item_throw_pigskin'); DELETE FROM spell_script_names WHERE spell_id = 132480 AND ScriptName = "spell_item_throw_pigskin";
INSERT INTO `spell_script_names`(`spell_id`,`ScriptName`) VALUES ( '132480','spell_item_throw_pigskin'); DROP TABLE IF EXISTS player_factionchange_quests;
CREATE TABLE `player_factionchange_quests` (
	`allianceQuestId` INT (10),
	`hordeQuestId` INT (10),
	`Comments` TEXT 
); 
INSERT INTO `player_factionchange_quests` (`allianceQuestId`, `hordeQuestId`, `Comments`) VALUES('32388','32430','A Change of Command');
INSERT INTO `player_factionchange_quests` (`allianceQuestId`, `hordeQuestId`, `Comments`) VALUES('32389','32431','The Lion Roars & Glory to the Horde');
INSERT INTO `player_factionchange_quests` (`allianceQuestId`, `hordeQuestId`, `Comments`) VALUES('32373','32427','The Measure of a Leader');
INSERT INTO `player_factionchange_quests` (`allianceQuestId`, `hordeQuestId`, `Comments`) VALUES('32374','32429','The Prince\'s Pursuit');
INSERT INTO `player_factionchange_quests` (`allianceQuestId`, `hordeQuestId`, `Comments`) VALUES('32474','32476','A Test of Valor');
INSERT INTO `player_factionchange_quests` (`allianceQuestId`, `hordeQuestId`, `Comments`) VALUES('32390','32432','Call of the Packmaster & The Soul of the Horde');
UPDATE creature SET phasemask = 4294967295 WHERE id = 79446;UPDATE gameobject_loot_template SET mincountOrRef = 1 WHERE item = 109586;