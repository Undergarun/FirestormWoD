-- Raza'kul - 86629

	UPDATE creature_template SET scriptname = "npc_RazaKul_Garr", InhabitType = 4, npcflag = 3 WHERE entry = 86629;
	
	REPLACE INTO creature_queststarter VALUE (86629, 36874);
	REPLACE INTO creature_questender VALUE (86629, 36874);
	
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -159 AND faction_index = 0;