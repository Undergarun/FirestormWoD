#Loots for War mill...

DELIMITER @@

CREATE PROCEDURE `FillConditionTable`(IN `p_LootId` INT, IN `p_DropChance` FLOAT)

BEGIN
	REPLACE INTO creature_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (p_LootId, 117491, p_DropChance, 1, 0, 1, 3, '');
	-- 39 = CONDITION_HAS_BUILDING_TYPE, 13 = BuildingType::Type::MageTower
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_LootId AND SourceEntry = 117491;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1) VALUE (1, p_LootId, 117491, 39, 10);
	
END @@
DELIMITER ;

DELIMITER @@

CREATE PROCEDURE `Test`()
BEGIN
	
	DECLARE done INT DEFAULT FALSE;
	DECLARE l_Entry INT;
	DECLARE l_EntryCursor CURSOR FOR SELECT entry FROM creature_loot_template WHERE item = 113681;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	OPEN l_EntryCursor;
	
	read_loop: LOOP
		FETCH l_EntryCursor INTO l_Entry;
		IF done THEN
			LEAVE read_loop;
		END IF;
		
		IF l_Entry != 0 THEN
			SET @v1 = (SELECT ChanceOrQuestChance FROM creature_loot_template WHERE item = 113681 AND entry = l_Entry);
			CALL `FillConditionTable` (l_Entry, @v1);
		END IF;
	END LOOP;

	CLOSE l_EntryCursor;
	
END @@
DELIMITER ;

CALL `Test`();
	
	
DROP PROCEDURE `Test`;
DROP PROCEDURE `FillConditionTable`;



-- =========================================================
-- =========================================================
-- =========================================================
-- =========================================================
-- =========================================================
-- =========================================================
-- =========================================================
-- =========================================================
-- =========================================================

-- PIG ALERT
-- PIG ALERT

DELIMITER @@

CREATE PROCEDURE `Test3`(IN `p_NextQuestID` INT, IN `p_NPCID` INT, OUT `p_Boule` BOOL)

BEGIN
	DECLARE done INT DEFAULT FALSE;
	DECLARE l_ResultItr INT;
	DECLARE l_ResultCursor CURSOR FOR SELECT id FROM creature_queststarter WHERE quest = p_NextQuestID;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	OPEN l_ResultCursor;
	
	read_loop: LOOP
		FETCH l_ResultCursor INTO l_ResultItr;
		
		IF done THEN
			LEAVE read_loop;
		END IF;
		
		IF l_ResultItr = p_NPCID THEN
			SET p_Boule = TRUE;
		END IF;
		
	END LOOP;

	CLOSE l_ResultCursor;
	
END @@
DELIMITER ;

-- =========================================================
-- =========================================================
-- =========================================================

DELIMITER @@

CREATE PROCEDURE `Test2`(IN `p_QuestID` INT, IN `p_NextQuestID` INT, OUT `p_Boule` BOOL)

BEGIN
	DECLARE done INT DEFAULT FALSE;
	DECLARE l_ResultItr INT;
	DECLARE l_ResultCursor CURSOR FOR SELECT id FROM creature_questender WHERE quest = p_QuestID;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	OPEN l_ResultCursor;
	
	read_loop: LOOP
		FETCH l_ResultCursor INTO l_ResultItr;
		
		IF done THEN
			LEAVE read_loop;
		END IF;
		
		CALL `Test3`(p_NextQuestID, l_ResultItr, @testbool);
		
		SET p_Boule = @testbool;
	END LOOP;

	CLOSE l_ResultCursor;
	
END @@
DELIMITER ;

-- =========================================================
-- =========================================================
-- =========================================================

DELIMITER @@

CREATE PROCEDURE `Test`()
BEGIN
	
	DECLARE done INT DEFAULT FALSE;
	DECLARE l_Quest, l_NextQuest INT;
	DECLARE l_ResultCursor CURSOR FOR SELECT id, nextquestidchain FROM quest_template WHERE id IN (SELECT NextQuestIdChain FROM quest_template WHERE NextQuestIdChain != 0 AND ExclusiveGroup = 0 AND NOT flags & 0x00200000) AND ExclusiveGroup = 0;
	DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

	OPEN l_ResultCursor;
	
	read_loop: LOOP
		FETCH l_ResultCursor INTO l_Quest, l_NextQuest;
		IF done THEN
			LEAVE read_loop;
		END IF;
		
		CALL `Test2`(l_Quest, l_NextQuest, @booltest);
		
		IF (@booltest = TRUE) THEN
			UPDATE quest_template SET flags = flags | 0x00200000 WHERE id = l_NextQuest;
		END IF;
	END LOOP;

	CLOSE l_ResultCursor;
	
END @@
DELIMITER ;

CALL `Test`();
	
DROP PROCEDURE `Test`;
DROP PROCEDURE `Test2`;
DROP PROCEDURE `Test3`;



-- =========================================================
-- =========================================================
-- =========================================================

-- END ALERT
-- GL HF