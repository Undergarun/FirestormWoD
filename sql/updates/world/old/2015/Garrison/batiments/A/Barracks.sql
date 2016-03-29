-- LEVEL 1

	REPLACE INTO gameobject_template (entry, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `BuildVerified`) VALUES(236219, 7, 15617, "Chair", "", "", "", 0, 64, 0.9999999, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	REPLACE INTO creature_template (entry, `name`, `subname`, `IconName`, `RacialLeader`, `family`, `type`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `unit_flags`, `unit_flags2`, `Health_mod`, `Mana_mod`, `Armor_mod`, `rank`, `KillCredit1`, `KillCredit2`, `movementId`, `scale`, `minlevel`, `maxlevel`, `faction`, `baseattacktime`, `rangeattacktime`, `unit_class`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `speed_walk`, `speed_run`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `BuildVerified`) VALUES(77379, "Jonathan Stephens", "Steward", "", 0, 0, 7, 53844, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4096, 0, 1, 1, 1, 0, 0, 0, 0, 1, 90, 90, 1802, 2000, 2000, 1, 0, 0, 0, 0, 0, 0, 1, 1.142857, 0, 0, 0, 0, 0, 0, 0, 0, 19342);
	INSERT INTO creature_template_addon (entry, `bytes2`, `emote`) VALUES(77379, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `bytes2` = VALUES(`bytes2`), `emote` = VALUES(`emote`);
	INSERT INTO creature_model_info (`modelid`, `bounding_radius`, `combat_reach`, `gender`) VALUES(53844, 0.3366, 1.65, 0) ON DUPLICATE KEY UPDATE `modelid` = VALUES(`modelid`), `bounding_radius` = VALUES(`bounding_radius`), `combat_reach` = VALUES(`combat_reach`), `gender` = VALUES(`gender`);

	UPDATE creature_template SET ScriptName="npc_JonathanStephens_Garr" WHERE entry = 77379;

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-26 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-26','1','77379','16.4083','-3.00115','0.846085','5.73959');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-26','1','-236219','15.9425','-4.18765','0.31131','5.771');

-- LEVEL 2

-- LEVEL 3