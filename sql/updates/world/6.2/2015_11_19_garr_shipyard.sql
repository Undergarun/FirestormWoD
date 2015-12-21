DELETE FROM creature_template WHERE entry = 94399;
INSERT INTO `creature_template` (`entry`, `name`, `femaleName`, `subname`, `IconName`, `type_flags`, `type_flags2`, `type`, `family`, `rank`, `KillCredit1`, `KillCredit2`, `PetSpellDataId`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `Health_mod`, `Mana_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `exp_unk`, `BuildVerified`) VALUES
(94399, 'Fleet Command Table', '', '', 'missions', 1611792400, 6, 10, 0, 0, 0, 0, 0, 64244, 0, 0, 0, 1.35, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20574); -- 94399

UPDATE creature_template SET faction = 190, unit_flags = 768, unit_flags2 = 67143680, npcflag = 1, npcflag2 = 32, gossip_menu_id = 18536, scriptName = "npc_FleetCommandTable" WHERE entry = 94399;

DELETE FROM gossip_menu WHERE entry = 18536;
INSERT INTO gossip_menu VALUE (18536, 6957);

DELETE FROM gossip_menu_option WHERE menu_id = 18536;
INSERT INTO gossip_menu_option (menu_id, id, option_icon, option_text, option_id, npc_option_npcflag) VALUES (18536, 0, 27, "Greetings and salutations, hero!  I have the latest news from both continents and points beyond for your consideration.", 1, 1);

DELETE FROM creature_model_info WHERE modelid = 64244;
INSERT INTO `creature_model_info` (`modelid`, `bounding_radius`, `combat_reach`) VALUES
(64244, 2.4, 1.6); -- 64244


DELETE FROM instance_template WHERE map IN (1159, 1153, 1473, 1474);
INSERT INTO instance_template VALUES
(1159, 1116, "instance_Garrison_A3", 1),
(1153, 1116, "instance_Garrison_H3", 1),
(1473, 1116, "instance_Garrison_AS", 1),
(1474, 1116, "instance_Garrison_HS", 1);


DELETE FROM spell_script_names WHERE ScriptName = "spell_garrison_shipyard";
INSERT INTO spell_script_names VALUES 
(185915, "spell_garrison_shipyard"),
(186007, "spell_garrison_shipyard");
