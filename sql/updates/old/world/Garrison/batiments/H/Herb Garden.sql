-- LEVEL 1

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-29 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','0','85783','-9.16787','16.1029','2.29719','3.6194');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','0','81981','-10.7658','-2.22524','0.3862','3.02969');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','0','79916','-25.4734','23.7395','3.0022','3.92699');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-29','0','-239238','-4.98841','11.0472','1.0202','3.75067');

	#81981/NPC - Tarnon

		DELETE FROM creature_queststarter WHERE quest = 34193;
		INSERT INTO creature_queststarter VALUE (81981, 34193);
		UPDATE creature_template SET scriptname = "npc_Tarnon_Garrison", npcflag = 131 WHERE entry = 81981;

	#85344/NPC - Naron Bloomthistle

		UPDATE creature_template SET npcflag = npcflag | 128 WHERE entry = 85344;

	#85783/NPC - Nali Softsoil

		UPDATE creature_template SET npcflag = npcflag | 1, npcflag2 = 32, scriptname = "npc_NaliSoftOil_Garr" WHERE entry = 85783;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-136 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','0','85783','-9.16787','16.1029','2.29719','3.6194');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','0','81981','-10.7658','-2.22524','0.3862','3.02969');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','0','79916','-25.4734','23.7395','3.0022','3.92699');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-136','0','-239238','-4.98841','11.0472','1.0202','3.75067');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-137 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','0','85783','-9.16787','16.1029','2.29719','3.6194');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','0','81981','-10.7658','-2.22524','0.3862','3.02969');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','0','79916','-25.4734','23.7395','3.0022','3.92699');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-137','0','-239238','-4.98841','11.0472','1.0202','3.75067');