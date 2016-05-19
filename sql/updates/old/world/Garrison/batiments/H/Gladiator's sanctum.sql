-- Raza'kul - 86629

	UPDATE creature_template SET scriptname = "npc_RazaKul_Garr", InhabitType = 4, npcflag = 3 WHERE entry = 86629;
	
	REPLACE INTO creature_queststarter VALUE (86629, 36874);
	REPLACE INTO creature_questender VALUE (86629, 36874);
	
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -159 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','86629','4.04582','-1.19722','1.76241','0.209029');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','-236744','5.242','4.2493','0.541428','5.88353');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','-236721','5.242','4.2493','1.55293','5.82463');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','86639','4.37512','-0.176027','1.08931','0.008749');
