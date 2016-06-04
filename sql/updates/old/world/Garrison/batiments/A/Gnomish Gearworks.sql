-- LEVEL 1

	-- Zee <Master Steamsmith>
	-- Lunarfall Engineer (84980)

	-- Plot content
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -162 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-162','1','84286','9.4447','-13.7488','1.17109','0.114666'),
	('-162','1','84980','7.97175','-9.43137','1.22384','2.14492'),
	('-162','1','84980','11.9157','-11.5916','1.20908','4.70767');

	UPDATE creature_template SET faction = 35 WHERE entry IN (84286, 84980);

	-- 84286/Zee <Master Steamsmith>
	REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES('84286','37091');
	REPLACE INTO `creature_questender` (`id`, `quest`) VALUES('84286','37091');

	UPDATE creature_template SET scriptname = "npc_Zee_Garrison" WHERE entry = 84286;


	UPDATE quest_template SET rewardmoney = 114000, RewardMoneyMaxLevel = 5 WHERE id = 37091;

	-- Overcharged Siege Engine
	UPDATE creature_template SET vehicleid = 3946, minlevel = 100, maxlevel = 100, faction = 2203, spell1 = 177291, spell2 = 177290, spell3 = 177293, spell4 = 177292 WHERE entry = 88694;

	-- Overcharged Demolisher
	UPDATE creature_template SET vehicleid = 3946, minlevel = 100, maxlevel = 100, faction = 2203, spell1 = 177291, spell2 = 177290, spell3 = 177293, spell4 = 177292 WHERE entry = 88695;

	-- Sentry Turret
	UPDATE creature_template SET minlevel = 100, maxlevel = 100, faction = 2203, unit_flags = 33554436, scriptname = "npc_GearshopWorkshopTurret_Garr" WHERE entry =  84143;

-- LEVEL 2

-- LEVEL 3