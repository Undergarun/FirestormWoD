-- LEVEL 1

UPDATE `creature_template` SET gossip_menu_id = 16986, `npcflag`=`npcflag`|17, trainer_type=2 WHERE `entry`=79892;
DELETE FROM `npc_trainer` WHERE `entry`=79892;
insert into `npc_trainer` (`entry`, `spell`, `spellcost`, `reqskill`, `reqskillvalue`, `reqlevel`) values
('79892','7733','100','356','0','1'),
('79892','7734','500','356','50','1'),
('79892','54083','10000','356','125','1'),
('79892','18249','25000','356','200','1'),
('79892','54084','100000','356','275','1'),
('79892','51293','150000','356','350','1'),
('79892','88869','250000','356','425','1'),
('79892','110412','300000','356','500','1');


UPDATE `creature_template` SET `npcflag`=`npcflag`|128 WHERE `entry`=79971;
DELETE FROM `npc_vendor` WHERE `entry` = 79971 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(79971, 0, 6256, 0, 0, 0, 1), 
(79971, 0, 6365, 1, 3600, 0, 1), 
(79971, 0, 6529, 0, 0, 0, 1), 
(79971, 0, 6530, 0, 0, 0, 1), 
(79971, 0, 6532, 0, 0, 0, 1), 
(79971, 0, 6533, 2, 3600, 0, 1);

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-64 AND faction_index = 0;
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','79892','4.10929','-1.38352','0.583206','0.213478');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','44880','2.56324','5.39814','4.2152','0.408808');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','44880','1.28986','-0.327592','8.7592','6.09967');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','79971','-0.343123','7.68761','0.713196','0.655077');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','62953','25.7635','-22.1997','6.16921','1.42364');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','44880','2.56324','5.39814','5.7152','0.408808');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','-232440','8.77231','3.06427','0.000198','4.7124');
INSERT INTO `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) VALUES('-64','0','-230764','1.15135','7.00761','0.089203','0.633848');

-- Quest Chain : 34758, 36141, 36131, 36132
UPDATE quest_template SET PrevQuestId = 0, NextQuestId = 36141 WHERE Id = 34758;
UPDATE quest_template SET PrevQuestId = 34758, NextQuestId = 36131 WHERE Id = 36141;
UPDATE quest_template SET PrevQuestId = 36141, NextQuestId = 36132 WHERE Id = 36131;
UPDATE quest_template SET PrevQuestId = 36131, NextQuestId = 0 WHERE Id = 36132;
UPDATE quest_template SET RequiredSkillId = 0 WHERE id IN (34758, 36141, 36131, 36132);

REPLACE INTO `creature_queststarter` (`id`, `quest`) VALUES
('79892','34758'),
('79896','36131'),
('79896','36132'),
('79896','36141');

REPLACE INTO `creature_questender` (`id`, `quest`) VALUES
('79892','36132'),
('79896','34758'),
('79896','36131'),
('79896','36141');

-- Horde daily quests (35071, 35006, 35072, 35073, 35074, 35075)

UPDATE quest_template SET PrevQuestId = 36132 WHERE Id IN (35071, 35006, 35072, 35073, 35074, 35075);

DELETE FROM pool_quest WHERE pool_entry = 30005;
DELETE FROM pool_template WHERE entry = 30005;
INSERT INTO pool_template VALUE (30005, 1, "Horde Garrison Fishing daily quests");
INSERT INTO pool_quest VALUES
(35071, 30005, "Jawless Skulker"),
(35006, 30005, "Fire Ammonite"),
(35072, 30005, "Fat Sleeper"),
(35073, 30005, "Blind Lake Sturgeon"),
(35074, 30005, "Blackwater Whiptail"),
(35075, 30005, "Abyssal Gulper Eel");

-- Updates

UPDATE creature_template SET scriptname = "npc_MakJin_Garr" WHERE entry = 79892;

-- zone frostwall - 7004

-- LEVEL 2

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -134 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','0','79971','0.614584','8.96304','0.892303','5.88444');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','0','79892','0.87929','-0.074721','0.50116','6.08473');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','0','-232440','7.55463','0.407227','0.00116','2.56614');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-134','0','-230764','3.97624','7.17488','0.100479','3.04523');


-- LEVEL 3

	DELETE FROM garrison_plot_content WHERE plot_type_or_building = -135 AND faction_index = 0;
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','0','79971','1.62224','9.4448','0.949738','5.90643');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','0','79892','0.171587','0.551334','0.503006','5.2035');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','0','-232440','9.48394','1.06858','-0.000076','3.22037');
	insert into `garrison_plot_content` (`plot_type_or_building`, `faction_index`, `creature_or_gob`, `x`, `y`, `z`, `o`) values('-135','0','-230764','3.94956','7.22322','0.107117','3.22822');


-- Loots

DELIMITER @@

CREATE PROCEDURE `FillConditionAndLootsTables`(IN `p_ZoneID` INT, IN `p_GroupID` INT, IN `p_Level` INT, IN `p_ItemEntry` INT, IN `p_DropChance` FLOAT, IN `p_Reference` INT)
BEGIN

	REPLACE INTO reference_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (-`p_Reference`, p_ItemEntry, `p_DropChance`, 1, 1, 1, 1, '');

	-- 39 = CONDITION_HAS_BUILDING_TYPE, 24 = BuildingType::Type::FishingHut, p_Level = Building Level
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_ZoneID AND SourceEntry = p_ItemEntry;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1, ConditionValue2) VALUES (1, p_ZoneID, p_ItemEntry, 39, 24, p_Level);
END @@
DELIMITER ;

	DELETE FROM fishing_loot_template  WHERE entry = 7004;

	-- Frostwall
	SET @l_ZoneID = 7004;

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

	REPLACE INTO fishing_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (7004, 118424, 3, 1, 0, 1, 1, '');

	DROP PROCEDURE `FillConditionAndLootsTables`;