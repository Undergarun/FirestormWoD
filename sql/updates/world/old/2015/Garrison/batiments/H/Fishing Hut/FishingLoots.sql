DELIMITER @@

CREATE PROCEDURE `FillConditionAndLootsTables`(IN `p_ZoneID` INT, IN `p_GroupID` INT, IN `p_Level` INT, IN `p_ItemEntry` INT, IN `p_DropChance` FLOAT, IN `p_Reference` INT)
BEGIN

	REPLACE INTO reference_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (-`p_Reference`, p_ItemEntry, `p_DropChance`, 1, 1, 1, 1, '');

	-- 39 = CONDITION_HAS_BUILDING_TYPE, 13 = BuildingType::Type::MageTower, p_Level = Building Level
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_ZoneID AND SourceEntry = p_ItemEntry;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1, ConditionValue2) VALUES (1, p_ZoneID, p_ItemEntry, 39, 13, p_Level);
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