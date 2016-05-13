-- LEVEL 1

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-24 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-24','1','88126','15.8253','-4.65711','0.817719','0.163344'), -- maybell (useless)
	('-24','1','84796','10.1482','-3.03874','0.817497','0.076947'), -- mom champierreux (useless)
	('-24','1','84524','4.26283','-4.29326','0.817497','1.51108'), -- homer
	('-24','1','84795','3.34265','-0.084008','0.817497','0.022747'), -- tommy
	('-24','1','-234186','14.8351','2.71689','0.317497','4.69979'), -- iron trap
	('-24','1','-238761','17.275','-4.5227','0.317329','3.14973'); -- work order

	UPDATE quest_template_objective SET TYPE = 0 WHERE questid IN (36345, 36271);

	#82481/NPC - Fiona

	#88126/NPC - Maybell Maclure-Stonefield

	#84796/NPC - Ma Stonefield <Grillmaster>

		UPDATE creature_template SET npcflag = npcflag | 1, gossip_menu_id = 90006 WHERE entry = 84796;
		REPLACE INTO gossip_menu VALUE (90006, 92004);
		REPLACE INTO npc_text (ID, text0_0, text0_1) VALUE (92004, "Yes, Sugar ?", "Yes, Sugar ?");
		REPLACE INTO locales_npc_text (entry, text0_0_loc2) VALUE (92004, "Oui, mon tresor ?");

	#84524/NPC - Homer Stonefield

		REPLACE INTO creature_queststarter VALUES
		(84524, 36271),
		(84524, 36272),
		(84524, 36274);

		REPLACE INTO creature_questender VALUES
		(84524, 36271),
		(84524, 36272),
		(84524, 36274);

		UPDATE creature_template SET npcflag = npcflag | 1 | 2, npcflag2 = 32, subname = "Work Orders", scriptname = "npc_HomerStonefield_Garr" WHERE entry = 84524;

	#84795/NPC - Tommy Joe Stonefield

	#83968/NPC - Tommy Joe Stonefield (summoned while hunting)

		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(83968, "Tommy Joe Champierreux", "", "", 0, 0, 7, 3331, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 35, 1500, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		UPDATE creature_template SET faction = 35 WHERE entry = 83968;
		insert into `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) values('83968','0','0','0','54','0','100','0','0','0','0','0','41','8000','0','0','0','0','0','0','0','0','0','0','0','0','0','Despawn after timer');


	#83967/NPC - Homer Stonefield (summoned while hunting)

		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(83968, "Tommy Joe Champierreux", "", "", 0, 0, 7, 3331, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 35, 1500, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		UPDATE creature_template SET faction = 35, scriptname = "npc_HomerStonefield_Garr_Trap" WHERE entry = 83967;
		
		DELETE FROM creature_text WHERE entry = 83967;
		INSERT INTO creature_text VALUES
		(83967, 0, 0, 'Back in my day, wolves would eat both of our feet, both ways to and from Ironforge, in the snow.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 1, 0, 'Folks back at the garrison will appreciate this one.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 2, 0, 'Guess you cannot make milk without breaking a few eggs!', 12, 0, 100, 0, 0, 0, ''),
		(83967, 3, 0, 'Holy light, did ya hafta get the worst-smelling one of the bunch?', 12, 0, 100, 0, 0, 0, ''),
		(83967, 4, 0, 'Interesting markings on this little guy.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 5, 0, 'Now do not be touching that, Tommy Joe.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 6, 0, 'Oh no, not Clyde.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 7, 0, 'This one is even bigger than the last one!', 12, 0, 100, 0, 0, 0, ''),
		(83967, 10, 0, 'Tommy Joe, last time you took a bath was before the Cataclysm.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 11, 0, 'Tommy Joe, last time you tried that you ended up with a bad case of broken face.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 12, 0, 'We will cart this sucker back to base immediately, commander!', 12, 0, 100, 0, 0, 0, ''),
		(83967, 13, 0, 'We well take her from here!', 12, 0, 100, 0, 0, 0, ''),
		(83967, 14, 0, 'Wowee, that is a big one!', 12, 0, 100, 0, 0, 0, ''),
		(83967, 15, 0, 'Ya know, hauling these things back aint a walk in the park.', 12, 0, 100, 0, 0, 0, ''),
		(83967, 16, 0, 'Yep, sure bigger and... wait, what?', 12, 0, 100, 0, 0, 0, '');


		
-- LEVEL 2
		
	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-25 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-25','1','88126','1.50372','-3.02134','1.11149','0.278774'),
	('-25','1','84796','2.54598','3.19272','1.11213','1.75532'),
	('-25','1','84524','9.02583','-2.32332','1.11144','0.447631'),
	('-25','1','84795','7.901','3.39216','1.11144','2.80775'),
	('-25','1','-234186','11.1508','2.53829','0.61274','5.97684'),
	('-25','1','-238761','20.4724','-3.34327','0.067924','2.69387');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-133 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-133','1','84524','9.86922','-4.29116','0.77552','1.41916'),
	('-133','1','84795','6.30178','2.64393','0.77552','5.02021'),
	('-133','1','84796','0.346503','-6.63675','0.775261','4.59138'),
	('-133','1','88126','-4.86952','0.566086','0.775429','0.087129'),
	('-133','1','-234186','0.017007','0.259515','0.275429','6.15198'),
	('-133','1','-238761','19.2267','-3.17755','0.000122','1.75767');
