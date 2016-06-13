replace into `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) values
('1','0' ,'0','I would like to place an order.','0','0','0','0','0','0',NULL),
('1','1' ,'0','I would like to get Sumptuous Fur.','0','0','0','0','0','0',NULL),
('1','2' ,'0','I would like to get Raw Beast Hide.','0','0','0','0','0','0',NULL),
('1','3' ,'0','I would like to get Savage Feast.','0','0','0','0','0','0',NULL),
('1','4' ,'0','I would like to get Sumptuous Fur with Savage blood.','0','0','0','0','0','0',NULL),
('1','5' ,'0','I would like to get Raw Beast Hide with Savage blood.','0','0','0','0','0','0',NULL),
('1','6' ,'0','I would like to get Savage Feast with Savage blood.','0','0','0','0','0','0',NULL),
('1','7' ,'0','I want to browse your goods.','0','0','0','0','0','0',NULL),
('1','8' ,'1','Access to my bank.','0','0','0','0','0','0',NULL),
('1','9' ,'0','Frostweed.','0','0','0','0','0','0',NULL),
('1','10','0','Starflower.','0','0','0','0','0','0',NULL),
('1','11','0','Fireweed.','0','0','0','0','0','0',NULL),
('1','12','0','Talador Orchid.','0','0','0','0','0','0',NULL),
('1','13','0','Gorgrond Flytrap.','0','0','0','0','0','0',NULL),
('1','14','0','Nagrand Arrowbloom.','0','0','0','0','0','0',NULL),
('1','15','0','I\'d like to requisition my seal for the week.','0','0','0','0','0','0',NULL),
('1','16','0','Can you refine this draenic stone into ore for me?','0','0','0','0','0','0',NULL),
('1','17','1','I need you to do something for me.','0','0','0','0','0','0',NULL),
('1','18','0','Create me a garrison.','0','0','0','0','0','0',NULL),
('1','19','0','Time to get back to work.','0','0','0','0','0','0',NULL),
('1','20','0','Gather Shelly\'s report.','0','0','0','0','0','0',NULL),
('1','21','0','Gazlowe needs you.','0','0','0','0','0','0',NULL);

-- need another parse of BroadcastText.db2 for all used locales, updated to 6.2.3 DBCs.
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc1, option_text_loc2, option_text_loc3, option_text_loc8, option_text_loc9) VALUE
(1, 0, "I would like to place an order.", "Je voudrais passer une commande.", "Ich möchte einen Arbeitsauftrag erteilen.", "Я хочу сделать заказ.", "Me gustaría realizar un pedido.");

UPDATE creature_template SET ScriptName="npc_KristenStoneforge_Garr", gossip_menu_id = 17025, `npcflag`=`npcflag`|4227 WHERE entry = 77377;
UPDATE creature_template SET scriptname = "npc_DalanaClarke_Garr" WHERE entry = 89065;


	-- Building lvl 2 condition to sell some items
	REPLACE INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, conditionvalue1, conditionvalue2, COMMENT) VALUES
	(23, 77377, 113789, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113790, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113791, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113792, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113793, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113794, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113795, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113797, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113798, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113799, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113800, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113801, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113802, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113803, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113805, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113806, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113807, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113808, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113809, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113810, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113811, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113813, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113814, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113815, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113816, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113817, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113818, 39, 10, 2, "Item buyable with War Mill lvl 2"),
	(23, 77377, 113819, 39, 10, 2, "Item buyable with War Mill lvl 2");
    
    
    UPDATE creature_template SET scriptname = "npc_Magrish_Garr" WHERE entry = 89066;
    
    	
 	-- Building lvl 2 condition to sell some items
 	REPLACE INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, conditionvalue1, conditionvalue2, COMMENT) VALUES
 	(23, 79815, 113789, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113790, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113791, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113792, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113793, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113794, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113795, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113797, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113798, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113799, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113800, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113801, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113802, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113803, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113805, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113806, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113807, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113808, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113809, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113810, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113811, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113813, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113814, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113815, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113816, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113817, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113818, 39, 10, 2, "Item buyable with War Mill lvl 2"),
 	(23, 79815, 113819, 39, 10, 2, "Item buyable with War Mill lvl 2");
  
 DROP PROCEDURE `FillConditionTable`;
 
 -- Fix all work order NPCs
 
 	UPDATE 
 	  creature_template 
 	SET
 	  minlevel = 90,
 	  maxlevel = 90,
 	  iconname = "workorders",
 	  EXP= 5,
 	  faction = 35,
 	  npcflag = npcflag | 1,
 	  npcflag2 = 32,
 	  unit_flags = 0,
 	  type_flags = 4096 
 	WHERE subname LIKE "%work order%" ;
 
 -- Fix architect tables (lvl 3)
 
 UPDATE 
   creature_template 
 SET
   unit_flags2 = 0x4008000,
   unit_flags = 768,
   minlevel = 100,
   maxlevel = 100,
   faction = 190,
   type_flags = 0,
   type_flags2 = 0 ,
   npcflag2 = 2
 WHERE NAME LIKE "architect table" ; 
 
 	UPDATE creature_template SET name = "Farmer Lok'Lub" WHERE entry = 85048;
 
 #Loots for War mill...
 
 DELIMITER @@
 
 CREATE PROCEDURE `FillConditionTable`(IN `p_LootId` INT, IN `p_DropChance` FLOAT)
 
 BEGIN
 	REPLACE INTO creature_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (p_LootId, 117491, p_DropChance, 1, 0, 1, 3, '');
 	-- 39 = CONDITION_HAS_BUILDING_TYPE, 13 = BuildingType::Type::MageTower
 	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_LootId AND SourceEntry = 117491;
 	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1) VALUE (1, p_LootId, 117491, 39, 10);
 	
 END @@
 DELIMITER ;
 
 DELIMITER @@
 
 CREATE PROCEDURE `Test`()
 BEGIN
 	
 	DECLARE done INT DEFAULT FALSE;
 	DECLARE l_Entry INT;
 	DECLARE l_EntryCursor CURSOR FOR SELECT entry FROM creature_loot_template WHERE item = 113681;
 	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;
 
 	OPEN l_EntryCursor;
 	
 	read_loop: LOOP
 		FETCH l_EntryCursor INTO l_Entry;
 		IF done THEN
 			LEAVE read_loop;
 		END IF;
 		
 		IF l_Entry != 0 THEN
 			SET @v1 = (SELECT ChanceOrQuestChance FROM creature_loot_template WHERE item = 113681 AND entry = l_Entry);
 			CALL `FillConditionTable` (l_Entry, @v1);
 		END IF;
 	END LOOP;
 
 	CLOSE l_EntryCursor;
 	
 END @@
 DELIMITER ;
 
 CALL `Test`();
 	
 	
 DROP PROCEDURE `Test`;
 DROP PROCEDURE `FillConditionTable`; 
 
 UPDATE creature_template SET scriptname = "npc_GarrisonWalter" WHERE entry = 77894;
 
 DELETE FROM spell_loot_template WHERE entry = 178445;
	INSERT INTO spell_loot_template VALUES
	(178445, 114128, 50, 1, 1, 1, 1, ''),
	(178445, 114129, 35, 1, 1, 1, 1, ''),
	(178445, 114131, 15, 1, 1, 1, 1, '');

#85048/NPC - Farmer LokLub

	UPDATE creature_template SET name = "Farmer Lok'Lub" WHERE entry = 85048;