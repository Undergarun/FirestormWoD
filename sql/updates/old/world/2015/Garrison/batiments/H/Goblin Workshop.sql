-- LEVEL 1

	-- Vee <Master Fuselighter>
	-- Frostwall Engineer (87279)

	-- Plot content
	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -162 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-162','0','87279','-6.20813','12.8778','2.41223','0.010684'),
	('-162','0','87279','4.45644','-6.09971','2.40456','5.05529'),
	('-162','0','85369','-11.1515','13.193','2.40927','2.94257');

	UPDATE creature_template SET faction = 35 WHERE entry IN (87279, 85369);

	-- 85369/Vee <Master Fuselighter>
	REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES('85369','37044');
	REPLACE INTO `creature_questender` (`id`, `quest`) VALUES('85369','37044');

	UPDATE creature_template SET scriptname = "npc_Vee_Garrison" WHERE entry = 85369;


	UPDATE gameobject_template SET type = 3, data1 = 114974, data0 = 57, data3 = 1, data5 = 0, data6 = 0, data17 = 27647 WHERE entry = 234095; -- before, type = 22, data0 = 169292, data3 = 0, data5 = 57647, data6 = 1, data17 = 0 and data1 = 2147483647
	REPLACE INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES('114974','114974','100','1','1','1','1','');
	REPLACE INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES('55106','114983','100','1','1','1','1','');
	REPLACE INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES('57269','119158','100','1','1','1','1','');
	REPLACE INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES('54946','114246','100','1','1','1','1','');

	REPLACE INTO spell_script_names VALUE (168655, "spell_aura_sticky_grenade");
	REPLACE INTO spell_script_names VALUE (168555, "spell_pneumatic_power_gauntlet");

-- LEVEL 2

-- LEVEL 3