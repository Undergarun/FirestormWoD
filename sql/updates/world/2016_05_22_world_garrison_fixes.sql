-- Altar of bones

	UPDATE creature_template SET minlevel = 100, maxlevel = 100, npcflag = npcflag | 0x01000000 | 0x00000001, npcflag2 = npcflag2 | 0x00000020, unit_flags2 = unit_flags2 | 0x04000000 | 0x00008000 | 0x00002000, unit_flags = unit_flags | 0x00000004 | 0x00000100, scriptname = "npc_AltarOfBones", inhabittype = 4 WHERE entry = 86639;

-- Kuros - 86677

	UPDATE creature_template SET scriptname = "npc_Kuros_Garr", InhabitType = 4, npcflag = 3 WHERE entry = 86677;
	
	replace into creature_queststarter value (86677, 36876);
	replace into creature_questender value (86677, 36876);
	
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -159 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','86639','4.61352','0.178786','0.991531','6.04645');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','-236744','3.5327','4.20775','0.605667','6.07079');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','-236721','3.40289','4.23528','1.45593','5.94908');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','1','86677','4.97842','0.486778','2.2811','0.16855');
	
	-- Raza'kul - 86629

	UPDATE creature_template SET scriptname = "npc_RazaKul_Garr", InhabitType = 4, npcflag = 3 WHERE entry = 86629;
	
	REPLACE INTO creature_queststarter VALUE (86629, 36874);
	REPLACE INTO creature_questender VALUE (86629, 36874);
	
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -159 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','86629','4.04582','-1.19722','1.76241','0.209029');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','-236744','5.242','4.2493','0.541428','5.88353');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','-236721','5.242','4.2493','1.55293','5.82463');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-159','0','86639','4.37512','-0.176027','1.08931','0.008749');