-- LEVEL 1

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-24 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-24','0','-234186','2.89861','5.66747','0.113892','5.78927'), -- iron trap
	('-24','0','-238761','15.78','-2.32415','-0.000916','1.39927'), -- work order
	('-24','0','85044','14.269','-5.41791','0.500244','3.97223'), -- zlagk
	('-24','0','85048','20.0751','-5.15757','0.485855','3.23789'), -- farmer lok lub
	('-24','0','85047','17.3167','-9.62589','0.501129','2.44071'), -- farmer krella
	('-24','0','79816','10.0741','3.05366','0.633469','0.076652'); -- drokil

	#85048/NPC - Farmer Lok lub <Work Orders>

		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(85048, "Fermier Lok'lub", "Commandes", "workorders", 0, 0, 7, 31438, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 5, 1, 3, 3, 1735, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(85048, 1, 3367, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
		UPDATE creature_template SET npcflag = npcflag | 1, npcflag2 = 32, subname = "Work Orders", scriptname = "npc_FarmerLokLub_Garr" WHERE entry = 85048;
		INSERT INTO gossip_menu (`entry`, `text_id`) VALUES(16908, 24605) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `text_id` = VALUES(`text_id`);
		REPLACE INTO creature_queststarter VALUES
		(85048, 36344),
		(85048, 36345),
		(85048, 36346);

		REPLACE INTO creature_questender VALUES
		(85048, 36344),
		(85048, 36345),
		(85048, 36346);

	#85044/NPC - Zlagk

		INSERT INTO creature_template (`entry`, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(85044, "Zlagk", "Boucher", "", 0, 0, 7, 3753, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1.1, 1, 1, 0, 0, 0, 0, 1, 9, 9, 2110, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `name` = VALUES(`name`), `subname` = VALUES(`subname`), `IconName` = VALUES(`IconName`), `RacialLeader` = VALUES(`RacialLeader`), `family` = VALUES(`family`), `type` = VALUES(`type`), `modelid1` = VALUES(`modelid1`), `modelid2` = VALUES(`modelid2`), `modelid3` = VALUES(`modelid3`), `modelid4` = VALUES(`modelid4`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `unit_flags` = VALUES(`unit_flags`), `unit_flags2` = VALUES(`unit_flags2`), `Health_mod` = VALUES(`Health_mod`), `Mana_mod` = VALUES(`Mana_mod`), `Armor_mod` = VALUES(`Armor_mod`), `rank` = VALUES(`rank`), `KillCredit1` = VALUES(`KillCredit1`), `KillCredit2` = VALUES(`KillCredit2`), `movementId` = VALUES(`movementId`), `scale` = VALUES(`scale`), `minlevel` = VALUES(`minlevel`), `maxlevel` = VALUES(`maxlevel`), `faction` = VALUES(`faction`), `baseattacktime` = VALUES(`baseattacktime`), `rangeattacktime` = VALUES(`rangeattacktime`), `unit_class` = VALUES(`unit_class`), `resistance1` = VALUES(`resistance1`), `resistance2` = VALUES(`resistance2`), `resistance3` = VALUES(`resistance3`), `resistance4` = VALUES(`resistance4`), `resistance5` = VALUES(`resistance5`), `resistance6` = VALUES(`resistance6`), `speed_walk` = VALUES(`speed_walk`), `speed_run` = VALUES(`speed_run`), `spell1` = VALUES(`spell1`), `spell2` = VALUES(`spell2`), `spell3` = VALUES(`spell3`), `spell4` = VALUES(`spell4`), `spell5` = VALUES(`spell5`), `spell6` = VALUES(`spell6`), `spell7` = VALUES(`spell7`), `spell8` = VALUES(`spell8`), `BuildVerified` = VALUES(`BuildVerified`);
		INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(85044, 1, 2827, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
		UPDATE creature_template SET npcflag = npcflag | 1, gossip_menu_id = 90005 WHERE entry = 85044;
		REPLACE INTO gossip_menu VALUE (90005, 92003);
		REPLACE INTO npc_text (ID, text0_0, text0_1) VALUE (92003, "Meat for the meat god!", "Meat for the meat god!");
		REPLACE INTO locales_npc_text (entry, text0_0_loc2) VALUE (92003, "Du saignant et des carnes pour mon seigneur barbaque !");

	#234186/GameObject - Iron Trap

		INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(234186, 3, 14212, "Piиge de Fer", "", "", "", 94, 0, 0.2, 0, 0, 0, 0, 0, 0, 57, 55126, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);
		REPLACE INTO gameobject_loot_template VALUE (55126, 113991, 100, 1, 0, 1, 1, ''); -- iron trap
		REPLACE INTO gameobject_loot_template VALUE (55126, 115009, 100, 1, 0, 1, 1, ''); -- improved iron trap
		REPLACE INTO gameobject_loot_template VALUE (55126, 115010, 100, 1, 0, 1, 1, ''); -- deadly iron trap


	-- 4 = CONDITION_SOURCE_TYPE_GAMEOBJECT_LOOT_TEMPLATE, 39 = CONDITION_HAS_BUILDING_TYPE, 3 = BuildingType::Type::Barn
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = 55126;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1, ConditionValue2) VALUES (4, 55126, 113991, 39, 3, 1);
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1, ConditionValue2) VALUES (4, 55126, 115009, 39, 3, 2);
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1, ConditionValue2) VALUES (4, 55126, 115010, 39, 3, 3);

	#83925/NPC - Iron Trap

		UPDATE creature_template SET minlevel = 90, maxlevel = 90, faction = 35, unit_flags = unit_flags | 0x02000000, scriptname = "npc_IronTrap_Garr" WHERE entry IN (84773, 83925, 84774);

	#85093/NPC - Farmer Lok lub (Summoned after capture)

		UPDATE creature_template SET scriptname = "npc_FarmerLokLub_Trap" WHERE entry = 85093;
		DELETE FROM creature_text WHERE entry = 85093;
		INSERT INTO creature_text VALUES
		(85093, 0, 0, 'A mighty beast, well done! We will bring it back right away.', 12, 0, 100, 0, 0, 0, ''),
		(85093, 1, 0, 'A mighty kill, you should be quite proud!', 12, 0, 100, 0, 0, 0, ''),
		(85093, 2, 0, 'Ah, the air of Nagrand smells as I remember. Fresh, with a hint of blood!', 12, 0, 100, 0, 0, 0, ''),
		(85093, 3, 0, 'An excellent catch, commander, I will have it taken back to the stronghold.', 12, 0, 100, 0, 0, 0, ''),
		(85093, 4, 0, 'I smelled blood in the air, and was not disappointed!', 12, 0, 100, 0, 0, 0, ''),
		(85093, 5, 0, 'Quite a fine creature, it will surely produce many materials for us.', 12, 0, 100, 0, 0, 0, ''),
		(85093, 6, 0, 'The hide of this beast is like iron! I will sharpen my blades.', 12, 0, 100, 0, 0, 0, ''),
		(85093, 7, 0, 'The smell of this one reminds me of the boar farms in Durotar.', 12, 0, 100, 0, 0, 0, '');
		
		DELETE FROM locales_creature_text WHERE entry = 85093;
		-- frFR, esES, esMX, deDE, ruRU
		INSERT INTO locales_creature_text (entry, textgroup, id, text_loc2, text_loc6, text_loc7, text_loc3, text_loc8) VALUES
		(85093, 0, 0, 'Une belle bete, bien fait! Nous allons la ramener tout de suite.', 'Una bestia fuerte, bien hecho! La llevaremos enseguida.', 'Una bestia fuerte, bien hecho! La llevaremos enseguida.', 'So ein machtiges Biest, gut gemacht! Wir werden es sofort zuruckbringen.', "Могучий зверь, молодец! Мы сразу вернём его обратно."),
		(85093, 1, 0, 'Une belle prise, vous devriez etre sacrement fier !', 'Una muerte formidable, debes sentirte orgulloso!', 'Una muerte formidable, debes sentirte orgulloso!', 'So eine machtige Jagdbeute, du solltest au?erordentlich stolz sein!', "Это было добротное убийство, ты должен очень гордиться!"),
		(85093, 2, 0, "Ah, le parfum de l'air de Nagrand n'a pas change. Cette petite odeur de gibier !", 'Ah, el aire de Nagrand huele como lo recuerdo. Fresco, con un toque de sangre.', 'Ah, el aire de Nagrand huele como lo recuerdo. Fresco, con un toque de sangre.', 'Ah, die Luft in Nagrand riecht wie in meiner Erinnerung : frisch mit einer Spur Blut!', "Ах, запах воздуха Награнда такой же, каким я его помню. Свежий, с ноткой крови!"),
		(85093, 3, 0, 'Une belle prise, commandant. Je vais ramener зa а la forteresse.', 'Una captura excelente, comandante. ?La devolvere enseguida a la fortaleza!', 'Una captura excelente, comandante. ?La devolvere enseguida a la fortaleza!', 'Ein vorzuglicher Fang, Kommandant. Ich werde ihn nach dem Bollwerk zuruckbringen lassen.', "Отличный улов ,командир,он будет тут же переправлен в крепость"),
		(85093, 4, 0, "Il m'avait bien semble sentir l'odeur du sang flotter dans l'air, et je ne m'etais pas trompe !", 'Olia sangre en el aire, ?y no me equivocaba!', 'Olia sangre en el aire, ?y no me equivocaba!', 'Ich hatte Blut in der Luft gerochen, und ich war nicht enttauscht!', "Я почувствовал кровь в воздухе, и не был разочарован!"),
		(85093, 5, 0, 'Une tres belle creature, je vais sans aucun doute produire beaucoup avec.', 'Una buena criatura, ?seguro que produce muchos materiales para nosotros!', 'Una buena criatura, ?seguro que produce muchos materiales para nosotros!', 'So eine schone Kreatur, sie wird bestimmt viele Materialien fur uns ergeben.', "Довольно хорошее существо, из него выйдет много материалов для нас."),
		(85093, 6, 0, 'La peau de cette bete est dure comme de la pierre ! Je vais aiguiser mes lames.', '?La piel de esta bestia es como de hierro! Afilara mis espadas.', '?La piel de esta bestia es como de hierro! Afilara mis espadas.', 'Das Fell dieses Biestes ist wie Eisen! Ich werde meine Klingen schleifen.', "Шкура этого зверя словно железо! Я заострю мои ножи."),
		(85093, 7, 0, "L'odeur de celle-ci me rappelle les fermes de sangliers de Durotar.", 'EEl olor de esta me recuerda a las granjas de jabalies de Durotar.', 'El olor de esta me recuerda a las granjas de jabalies de Durotar.', 'Dieser Geruch erinnert mich an die Eberfarmen in Durotar.', "Запах вот от этого напоминает мне о свинофермах в Дуротаре.");

	#234186/GameObject - Iron Trap

		UPDATE gameobject_template SET scriptname = "gob_IronTrap_Garrison" WHERE entry = 234186;

	#238761/GameObject - Barn Work Order

		INSERT INTO gameobject_template (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(238761, 45, 13845, "Barn Work Order", "", "", "", 0, 0, 1, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `type` = VALUES(`type`), `displayId` = VALUES(`displayId`), `name` = VALUES(`name`), `IconName` = VALUES(`IconName`), `castBarCaption` = VALUES(`castBarCaption`), `unk1` = VALUES(`unk1`), `faction` = VALUES(`faction`), `flags` = VALUES(`flags`), `size` = VALUES(`size`), `questItem1` = VALUES(`questItem1`), `questItem2` = VALUES(`questItem2`), `questItem3` = VALUES(`questItem3`), `questItem4` = VALUES(`questItem4`), `questItem5` = VALUES(`questItem5`), `questItem6` = VALUES(`questItem6`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), `data8` = VALUES(`data8`), `data9` = VALUES(`data9`), `data10` = VALUES(`data10`), `data11` = VALUES(`data11`), `data12` = VALUES(`data12`), `data13` = VALUES(`data13`), `data14` = VALUES(`data14`), `data15` = VALUES(`data15`), `data16` = VALUES(`data16`), `data17` = VALUES(`data17`), `data18` = VALUES(`data18`), `data19` = VALUES(`data19`), `data20` = VALUES(`data20`), `data21` = VALUES(`data21`), `data22` = VALUES(`data22`), `data23` = VALUES(`data23`), `data24` = VALUES(`data24`), `data25` = VALUES(`data25`), `data26` = VALUES(`data26`), `data27` = VALUES(`data27`), `data28` = VALUES(`data28`), `data29` = VALUES(`data29`), `data30` = VALUES(`data30`), `data31` = VALUES(`data31`), `data32` = VALUES(`data32`), `unkInt32` = VALUES(`unkInt32`), `BuildVerified` = VALUES(`BuildVerified`);
		UPDATE gameobject_template SET scriptname = "go_garrison_shipment_container" WHERE entry = 238761;

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-25 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`id`, `plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('2843','-25','0','-238761','15.5945','2.38366','-0.125381','2.9245'),
    ('2844','-25','0','79816','12.0749','2.06138','0.469604','0.014597'),
    ('2845','-25','0','85048','16.58','4.98577','0.36731','4.82517'),
    ('2846','-25','0','85044','13.1936','-5.14766','0.489212','2.09985'),
    ('2847','-25','0','85047','10.1498','11.3051','0.90799','4.73878'),
    ('2849','-25','0','-234186','6.04173','6.11555','0.580612','5.59486');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-133 AND faction_index = 0;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES
	('-133','0','85048','12.8026','4.88438','0.482361','0.036915'),
	('-133','0','-238761','12.0751','-2.77916','-0.008026','0.81446'),
	('-133','0','-234186','-9.53767','-0.463832','0.371262','0.457103'),
	('-133','0','85047','-6.74789','-6.29642','0.676895','3.10783'),
	('-133','0','85044','4.30868','9.45035','0.872009','1.35639'),
	('-133','0','79816','3.39922','-1.32076','0.872482','1.40352');
