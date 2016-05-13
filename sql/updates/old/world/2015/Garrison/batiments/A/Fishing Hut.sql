-- LEVEL 1

	INSERT INTO npc_text (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`) VALUES(87206, "Think you can help us catch some fish around here?", "Think you can help us catch some fish around here?", 0, 100) ON DUPLICATE KEY UPDATE `ID` = VALUES(`ID`), `text0_0` = VALUES(`text0_0`), `text0_1` = VALUES(`text0_1`), `lang0` = VALUES(`lang0`), `prob0` = VALUES(`prob0`);
	INSERT INTO gossip_menu (`entry`, `text_id`) VALUES(16986, 87206) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `text_id` = VALUES(`text_id`);
	INSERT INTO gossip_menu_option (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `box_coded`, `box_money`, `box_text`, `npc_option_npcflag`) VALUES(16986, 1, 3, "I'm interested in fishing these savage lands.", 5, 0, 0, "", 16) ON DUPLICATE KEY UPDATE `menu_id` = VALUES(`menu_id`), `id` = VALUES(`id`), `option_icon` = VALUES(`option_icon`), `option_text` = VALUES(`option_text`), `option_id` = VALUES(`option_id`), `box_coded` = VALUES(`box_coded`), `box_money` = VALUES(`box_money`), `box_text` = VALUES(`box_text`), `npc_option_npcflag` = VALUES(`npc_option_npcflag`);
	UPDATE `creature_template` SET gossip_menu_id = 16986, `npcflag`=`npcflag`|17, trainer_type=2, ScriptName="npc_RonAshton_Garr" WHERE `entry`=77733;
	DELETE FROM `npc_trainer` WHERE `entry`=77733;
	INSERT INTO `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) VALUES
	('77733','7733','100','356','0','1'),
	('77733','7734','500','356','50','1'),
	('77733','54083','10000','356','125','1'),
	('77733','18249','25000','356','200','1'),
	('77733','54084','100000','356','275','1'),
	('77733','51293','150000','356','350','1'),
	('77733','88869','250000','356','425','1'),
	('77733','110412','300000','356','500','1');

	UPDATE `creature_template` SET `npcflag`=`npcflag`|128, ScriptName="npc_Segumi_Garr" WHERE `entry`=85708;
	INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(85708, 1, 84660, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
	DELETE FROM `npc_vendor` WHERE `entry` = 85708 AND `type` = 1;
	INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
	(85708, 0, 6256, 0, 0, 0, 1), 
	(85708, 0, 6365, 1, 3600, 0, 1), 
	(85708, 0, 6529, 0, 0, 0, 1), 
	(85708, 0, 6530, 0, 0, 0, 1), 
	(85708, 0, 6532, 0, 0, 0, 1), 
	(85708, 0, 6533, 2, 3600, 0, 1);

	UPDATE creature_template_addon SET auras="" WHERE entry = 82436;
	UPDATE `creature_template` SET ScriptName="npc_TharisStrongcast_Garr" WHERE `entry`=82436;
	INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(82436, 1, 1117, 0, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-64 AND faction_index = 1;
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','1','85708','2.28928','-2.15955','0.596008','0.2334');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','1','82436','2.97392','-6.4922','0.369904','1.32348');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','1','77733','14.3749','1.34486','0.503105','6.15809');
	INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','1','-232268','6.38951','-0.564783','0.000008','4.24988');

	-- Quest Chain : 34194, 36199, 36201, 36202
	UPDATE quest_template SET PrevQuestId = 0, NextQuestId = 36199 WHERE Id = 34194;
	UPDATE quest_template SET PrevQuestId = 34194, NextQuestId = 36201 WHERE Id = 36199;
	UPDATE quest_template SET PrevQuestId = 36199, NextQuestId = 36202 WHERE Id = 36201;
	UPDATE quest_template SET PrevQuestId = 36201, NextQuestId = 0 WHERE Id = 36202;
	UPDATE quest_template SET RequiredSkillId = 0 WHERE id IN (34194, 36199, 36201, 36202);

	REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
	('77733','34194'),
	('84372','36199'),
	('84372','36201'),
	('84372','36202');

	REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
	('77733','36202'),
	('84372','34194'),
	('84372','36199'),
	('84372','36201');

	-- Alliance daily quests (36517, 36515, 36514, 36513, 36510, 36511)

	UPDATE quest_template SET PrevQuestId = 36202 WHERE Id IN (36511, 36510, 36513, 36514, 36515, 36517);

	DELETE FROM pool_quest WHERE pool_entry = 30006;
	DELETE FROM pool_template WHERE entry = 30006;
	INSERT INTO pool_template VALUE (30006, 1, "Alliance Garrison Fishing daily quests");
	INSERT INTO pool_quest VALUES
	(36511, 30006, "Jawless Skulker"),
	(36510, 30006, "Fire Ammonite"),
	(36513, 30006, "Fat Sleeper"),
	(36514, 30006, "Blind Lake Sturgeon"),
	(36515, 30006, "Blackwater Whiptail"),
	(36517, 30006, "Abyssal Gulper Eel");

	-- zone lunarfall - 7078

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-134 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','1','85708','13.089','-0.749152','0.500114','6.03866');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','1','82436','7.06115','9.60846','0.500175','5.94363');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','1','77733','11.8191','-11.1024','0.292328','4.89748');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','1','-232268','8.2991','1.22775','-0.000221','0.880167');

-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building=-135 AND faction_index = 1;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','1','85708','12.8285','-0.56335','0.826401','0.032726');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','1','82436','7.02279','9.21214','0.590141','0.37045');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','1','77733','10.7628','-13.0515','0.136864','5.0043');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','1','-232268','10.6787','2.13678','0.26078','3.14683');

-- Loots

DELIMITER @@

CREATE PROCEDURE `FillConditionAndLootsTables`(IN `p_ZoneID` INT, IN `p_GroupID` INT, IN `p_Level` INT, IN `p_ItemEntry` INT, IN `p_DropChance` FLOAT, IN `p_Reference` INT)
BEGIN

	REPLACE INTO reference_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (-`p_Reference`, p_ItemEntry, `p_DropChance`, 1, 1, 1, 1, '');

	-- 39 = CONDITION_HAS_BUILDING_TYPE, 13 = BuildingType::Type::MageTower, p_Level = Building Level
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_ZoneID AND SourceEntry = p_ItemEntry;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1, ConditionValue2) VALUES (1, p_ZoneID, p_ItemEntry, 39, 13, p_Level);
END @@
DELIMITER ;

DELETE FROM fishing_loot_template  WHERE entry = 7078;

-- Lunarfall
SET @l_ZoneID = 7078;

-- little fishes

SET @l_Reference = -111650;
SET @l_BuildingLevel = 1;
SET @l_GroupID       = 1;

INSERT INTO fishing_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@l_ZoneID, 1, 100, 1, @l_GroupID, @l_Reference, 1, '');

SET @p_ItemEntry = 111650;
SET @l_DropRate  = 0.9;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111589;
SET @l_DropRate = 8;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111651;
SET @l_DropRate = 0.9;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111652;
SET @l_DropRate = 0.9;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111656;
SET @l_DropRate = 1.4;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111658;
SET @l_DropRate = 0.7;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111659;
SET @l_DropRate = 1;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111662;
SET @l_DropRate = 0.8;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

-- medium

SET @l_Reference = -111595;
SET @l_BuildingLevel = 2;
SET @l_GroupID       = 2;

INSERT INTO fishing_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@l_ZoneID, 2, 100, 1, @l_GroupID, @l_Reference, 1, '');

SET @p_ItemEntry = 111595;
SET @l_DropRate = 15;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111663;
SET @l_DropRate = 2;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111664;
SET @l_DropRate = 3;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111665;
SET @l_DropRate = 1.9;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111666;
SET @l_DropRate = 3;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111667;
SET @l_DropRate = 3;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111668;
SET @l_DropRate = 3;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111669;
SET @l_DropRate = 2;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 112633;
SET @l_DropRate = 1.6;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 116748;
SET @l_DropRate = 0.2;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 116819;
SET @l_DropRate = 0.1;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 116754;
SET @l_DropRate = 0.1;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 118391;
SET @l_DropRate = 0.1;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 118392;
SET @l_DropRate = 0.2;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 118393;
SET @l_DropRate = 0.1;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

-- big

SET @l_Reference = -111601;
SET @l_BuildingLevel = 3;
SET @l_GroupID       = 3;

INSERT INTO fishing_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@l_ZoneID, 3, 100, 1, @l_GroupID, @l_Reference, 1, '');

SET @p_ItemEntry = 111601;
SET @l_DropRate = 20;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111670;
SET @l_DropRate = 3;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111671;
SET @l_DropRate = 5;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111672;
SET @l_DropRate = 4;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111673;
SET @l_DropRate = 5;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111674;
SET @l_DropRate = 5;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111675;
SET @l_DropRate = 7;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 111676;
SET @l_DropRate = 2;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

-- hameçons

SET @l_Reference = -110274;
SET @l_BuildingLevel = 0;
SET @l_GroupID       = 4;

INSERT INTO fishing_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@l_ZoneID, 4, 100, 1, @l_GroupID, @l_Reference, 1, '');

SET @p_ItemEntry = 110274;
SET @l_DropRate = 0.5;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 110289;
SET @l_DropRate = 0.7;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 110290;
SET @l_DropRate = 0.6;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 110291;
SET @l_DropRate = 0.7;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 110292;
SET @l_DropRate = 0.6;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 110293;
SET @l_DropRate = 0.7;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

SET @p_ItemEntry = 110294;
SET @l_DropRate = 0.5;
CALL `FillConditionAndLootsTables` (@l_ZoneID, @l_GroupID, @l_BuildingLevel, @p_ItemEntry, @l_DropRate, @l_Reference);

-- no specific level needed

REPLACE INTO fishing_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (7078, 118424, 3, 1, 0, 1, 1, '');

DROP PROCEDURE `FillConditionAndLootsTables`;