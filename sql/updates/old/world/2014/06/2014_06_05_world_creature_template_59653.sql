#59653/NPC - Chi-Ji

	UPDATE `creature_template` SET `ScriptName` = 'npc_chi_ji' WHERE `entry` = 59653;
	UPDATE `creature_template` SET `gossip_menu_id` = 60012 WHERE `entry` = 59653;
	
	DELETE FROM gossip_menu WHERE entry = 60012;
	INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES('60012','75009');
	
	DELETE FROM gossip_menu_option WHERE menu_id = 75009;
	INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) VALUES('75009','0','0','<The crane does not speak, but his words fill your mind.>\r\n\r\nGo... While you can...\r\n\r\nAll is... lost.','0','0','0','0','0','0',NULL);

	UPDATE creature SET spawntimesecs = 1 WHERE id = 59653;
	
