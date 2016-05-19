-- Dungeons :

-- Upper Blackrock Spire
-- Skyreach
-- Bloodmaul Slag Mines
-- Iron Docks
-- Auchindoun
-- Everbloom
-- Shadowmoon Burial Grounds
-- Grimrail Depot

DROP TABLE IF EXISTS `tavern_datas`; -- Only for self help, not needed live
create table `tavern_datas` (
	`QuestID` bigint (10),
	`GameObjectID` bigint (10),
	`RewardItemID` bigint (10),
	`KillCreditID` bigint (10),
	`QuestGiverID` bigint (10),
	`QuestTakerID` bigint (10),
	`QuestType` blob 
); 

INSERT INTO tavern_datas VALUES
-- Vanity Rewards
(37162, 237469, 118624, 77120, 88001, 88001, 'Vanity Rewards (one shot quest) - Upper Blackrock Spire'), -- Damsels and Dragons
(37161, 237468, 118623, 77120, 88000, 88000, 'Vanity Rewards (one shot quest) - Upper Blackrock Spire'), -- Family Traditions
(37148, 237481, 118650, 77120, 88024, 88024, 'Vanity Rewards (one shot quest) - Upper Blackrock Spire'), -- Oralius' Adventure
(37159, 237467, 118622, 76266, 87999, 87999, 'Vanity Rewards (one shot quest) - Skyreach'), -- Aviana's Request
(37158, 237466, 118621, 76266, 87998, 87998, 'Vanity Rewards (one shot quest) - Skyreach'), -- Gloriously Incandescent
(37152, 0,      118534, 74790, 87991, 87991, 'Vanity Rewards (one shot quest) - Bloodmaul Slag Mines (reward dropped from mobs)'), -- Cro's Revenge
(37153, 237461, 118616, 74790, 87992, 87992, 'Vanity Rewards (one shot quest) - Bloodmaul Slag Mines'), -- Time-Lost Vikings
(37157, 237462, 118617, 83612, 87994, 87994, 'Vanity Rewards (one shot quest) - Iron Docks'), -- Feeling A Bit Morose
(37155, 237463, 118618, 83612, 87995, 87995, 'Vanity Rewards (one shot quest) - Iron Docks'), -- The Brass Compass
(37154, 237465, 118620, 77734, 87997, 87997, 'Vanity Rewards (one shot quest) - Auchindoun'), -- The Cure For Death
(37156, 237464, 118619, 77734, 87996, 87996, 'Vanity Rewards (one shot quest) - Auchindoun'), -- The Soulcutter
(37165, 237472, 118627, 83846, 88004, 88004, 'Vanity Rewards (one shot quest) - Everbloom'), -- Cenarion Concerns
(37166, 237473, 118628, 83846, 88005, 88005, 'Vanity Rewards (one shot quest) - Everbloom'), -- Titanic Evolution
(37150, 237483, 118652, 83846, 88025, 88025, 'Vanity Rewards (one shot quest) - Everbloom'), -- For the Birds
(37163, 237470, 118625, 76407, 88002, 88002, 'Vanity Rewards (one shot quest) - Shadowmoon Burial Grounds'), -- Shadowy Secrets
(37164, 237471, 118626, 76407, 88003, 88003, 'Vanity Rewards (one shot quest) - Shadowmoon Burial Grounds'), -- The Huntresses
(37149, 237482, 118651, 76407, 88027, 88027, 'Vanity Rewards (one shot quest) - Shadowmoon Burial Grounds'), -- The Void-Gate
(37167, 237474, 118643, 80005, 88006, 88006, 'Vanity Rewards (one shot quest) - Grimrail Depot'), -- And No Maces!
(37160, 237475, 118644, 80005, 88007, 88007, 'Vanity Rewards (one shot quest) - Grimrail Depot'), -- Cleaving Time
(37151, 237484, 118653, 80005, 88026, 88026, 'Vanity Rewards (one shot quest) - Grimrail Depot'), -- Cold Steel

-- Daily Heroic Dungeons
(37237, 237469, 118624, 77120, 88001, 88001, 'Daily Heroic Dungeons - Upper Blackrock Spire'), -- A Hero's Quest is Never Complete
(37236, 237468, 118623, 77120, 88000, 88000, 'Daily Heroic Dungeons - Upper Blackrock Spire'), -- Like Father, Like Son
(37235, 237467, 118622, 76266, 87999, 87999, 'Daily Heroic Dungeons - Skyreach'), -- A Plea to the Sky
(37234, 237466, 118621, 76266, 87998, 87998, 'Daily Heroic Dungeons - Skyreach'), -- The Dark Within
(37228, 0,      118534, 74790, 87991, 87991, 'Daily Heroic Dungeons - Bloodmaul Slag Mines (reward dropped from mobs)'), -- A Fruitful Proposition
(37229, 237461, 118616, 74790, 87992, 87992, 'Daily Heroic Dungeons - Bloodmaul Slag Mines'), -- Like A Dwarf In A Mine
(37230, 237462, 118617, 83612, 87994, 87994, 'Daily Heroic Dungeons - Iron Docks'), -- Bloody Expensive
(37231, 237463, 118618, 83612, 87995, 87995, 'Daily Heroic Dungeons - Iron Docks'), -- The Search Continues
(37232, 237464, 118619, 77734, 87996, 87996, 'Daily Heroic Dungeons - Auchindoun'), -- Soulcarver Voss
(37233, 237465, 118620, 77734, 87997, 87997, 'Daily Heroic Dungeons - Auchindoun'), -- Vessel of Virtue
(37241, 237473, 118628, 83846, 88005, 88005, 'Daily Heroic Dungeons - Everbloom'), -- Lessons of the Past
(37240, 237472, 118627, 83846, 88004, 88004, 'Daily Heroic Dungeons - Everbloom'), -- The Leaf-Reader
(37239, 237471, 118626, 76407, 88003, 88003, 'Daily Heroic Dungeons - Shadowmoon Burial Grounds'), -- Fate of the Fallen
(37238, 237470, 118625, 76407, 88002, 88002, 'Daily Heroic Dungeons - Shadowmoon Burial Grounds'), -- Secrets of Soulbinding
(37243, 237475, 118644, 80005, 88007, 88007, 'Daily Heroic Dungeons - Grimrail Depot'), -- An Axe to Grind
(37242, 237474, 118643, 80005, 88006, 88006, 'Daily Heroic Dungeons - Grimrail Depot'), -- Learning Is Painful

-- Daily Dungeons

(37244, 237481, 118650, 77120, 88024, 88024, 'Daily Dungeons - Upper Blackrock Spire'), -- Flamefly Trap
(37147, 237480, 118649, 76407, 88027, 88027, 'Daily Dungeons - Skyreach'), -- Sky Dancers
(37142, 237477, 118646, 74790, 88013, 88013, 'Daily Dungeons - Bloodmaul Slag Mines'), -- Ogre Ancestry
(37145, 237478, 118647, 83612, 88017, 88017, 'Daily Dungeons - Iron Docks'), -- Budd's Gambit
(37146, 237479, 118648, 77734, 88022, 88022, 'Daily Dungeons - Auchindoun'), -- Go Fetch
(37227, 237483, 118652, 83846, 88025, 88025, 'Daily Dungeons - Everbloom'), -- Put a Bird on It
(37245, 237482, 118651, 76407, 88027, 88027, 'Daily Dungeons - Shadowmoon Burial Grounds'), -- Whispers in the Darkness
(37209, 237484, 118653, 80005, 80026, 80026, 'Daily Dungeons - Grimrail Depot'); -- Cold Steel Part II

SELECT * FROM gameobject_template WHERE entry IN (
237469, -- FOUND
237468, -- FOUND
237481, -- FOUND
237461, -- FOUND
237463, -- FOUND
237465, -- FOUND
237480, -- FOUND
237477, -- FOUND
237478, -- FOUND
237467, -- MISSING, SKYREACH______HEROIC
237466, -- MISSING, SKYREACH______HEROIC
237462, -- MISSING, IRON DOCKS____HEROIC
237464, -- MISSING, AUCHINDOUN____HEROIC
237479, -- MISSING, AUCHINDOUN____NORMAL
237483, -- MISSING, EVERBLOOM_____NORMAL
237472, -- MISSING, EVERBLOOM_____HEROIC
237473, -- MISSING, EVERBLOOM_____HEROIC
237470, -- MISSING, SBG___________HEROIC
237471, -- MISSING, SBG___________HEROIC
237482, -- MISSING, SBG___________NORMAL
237474, -- MISSING, GD____________HEROIC
237475, -- MISSING, GD____________HEROIC
237484  -- MISSING, GD____________NORMAL
);

SELECT DISTINCT id FROM gameobject WHERE id IN (
237469, -- FOUND
237468, -- FOUND
237481, -- FOUND
237467,
237461, -- FOUND
237462,
237463, -- FOUND
237465, -- FOUND
237464, -- FOUND
237472,
237473,
237483,
237470,
237471,
237482,
237474,
237475,
237484,
237466,
237480, -- FOUND
237477, -- FOUND
237478, -- FOUND
237479,
237483);

DELETE FROM quest_template_objective WHERE QuestID IN (
37162,
37161,
37148,
37159,
37158,
37152,
37153,
37157,
37155,
37154,
37156,
37165,
37166,
37150,
37163,
37164,
37149,
37167,
37160,
37151,
37237,
37236,
37235,
37234,
37228,
37229,
37230,
37231,
37232,
37233,
37241,
37240,
37239,
37238,
37243,
37242,
37244,
37147,
37142,
37145,
37146,
37227,
37245,
37209);

INSERT INTO quest_template_objective (QuestID, `Type`, `Index`, ObjectID, Amount) VALUES
(37162, 1, 0, 118624, 1),
(37161, 1, 0, 118623, 1),
(37148, 1, 0, 118650, 1),
(37159, 1, 0, 118622, 1),
(37158, 1, 0, 118621, 1),
(37152, 1, 0, 118534, 1),
(37153, 1, 0, 118616, 1),
(37157, 1, 0, 118617, 1),
(37155, 1, 0, 118618, 1),
(37154, 1, 0, 118620, 1),
(37156, 1, 0, 118619, 1),
(37165, 1, 0, 118627, 1),
(37166, 1, 0, 118628, 1),
(37150, 1, 0, 118652, 1),
(37163, 1, 0, 118625, 1),
(37164, 1, 0, 118626, 1),
(37149, 1, 0, 118651, 1),
(37167, 1, 0, 118643, 1),
(37160, 1, 0, 118644, 1),
(37151, 1, 0, 118653, 1),
(37237, 1, 0, 118624, 1),
(37236, 1, 0, 118623, 1),
(37235, 1, 0, 118622, 1),
(37234, 1, 0, 118621, 1),
(37228, 1, 0, 118534, 1),
(37229, 1, 0, 118616, 1),
(37230, 1, 0, 118617, 1),
(37231, 1, 0, 118618, 1),
(37232, 1, 0, 118619, 1),
(37233, 1, 0, 118620, 1),
(37241, 1, 0, 118628, 1),
(37240, 1, 0, 118627, 1),
(37239, 1, 0, 118626, 1),
(37238, 1, 0, 118625, 1),
(37243, 1, 0, 118644, 1),
(37242, 1, 0, 118643, 1),
(37244, 1, 0, 118650, 1),
(37147, 1, 0, 118649, 1),
(37142, 1, 0, 118646, 1),
(37145, 1, 0, 118647, 1),
(37146, 1, 0, 118648, 1),
(37227, 1, 0, 118652, 1),
(37245, 1, 0, 118651, 1),
(37209, 1, 0, 118653, 1),
(37162, 0, 1,  77120, 1),
(37161, 0, 1,  77120, 1),
(37148, 0, 1,  77120, 1),
(37159, 0, 1,  76266, 1),
(37158, 0, 1,  76266, 1),
(37152, 0, 1,  74790, 1),
(37153, 0, 1,  74790, 1),
(37157, 0, 1,  83612, 1),
(37155, 0, 1,  83612, 1),
(37154, 0, 1,  77734, 1),
(37156, 0, 1,  77734, 1),
(37165, 0, 1,  83846, 1),
(37166, 0, 1,  83846, 1),
(37150, 0, 1,  83846, 1),
(37163, 0, 1,  76407, 1),
(37164, 0, 1,  76407, 1),
(37149, 0, 1,  76407, 1),
(37167, 0, 1,  80005, 1),
(37160, 0, 1,  80005, 1),
(37151, 0, 1,  80005, 1),
(37237, 0, 1,  77120, 1),
(37236, 0, 1,  77120, 1),
(37235, 0, 1,  76266, 1),
(37234, 0, 1,  76266, 1),
(37228, 0, 1,  74790, 1),
(37229, 0, 1,  74790, 1),
(37230, 0, 1,  83612, 1),
(37231, 0, 1,  83612, 1),
(37232, 0, 1,  77734, 1),
(37233, 0, 1,  77734, 1),
(37241, 0, 1,  83846, 1),
(37240, 0, 1,  83846, 1),
(37239, 0, 1,  76407, 1),
(37238, 0, 1,  76407, 1),
(37243, 0, 1,  80005, 1),
(37242, 0, 1,  80005, 1),
(37244, 0, 1,  77120, 1),
(37147, 0, 1,  76407, 1),
(37142, 0, 1,  74790, 1),
(37145, 0, 1,  83612, 1),
(37146, 0, 1,  77734, 1),
(37227, 0, 1,  83846, 1),
(37245, 0, 1,  76407, 1),
(37209, 0, 1,  80005, 1);

REPLACE INTO gameobject_loot_template VALUES
(237469, 118624, -100, 1, 1, 1, 1, ''),
(237468, 118623, -100, 1, 1, 1, 1, ''),
(237481, 118650, -100, 1, 1, 1, 1, ''),
(237461, 118616, -100, 1, 1, 1, 1, ''),
(237463, 118618, -100, 1, 1, 1, 1, ''),
(237465, 118620, -100, 1, 1, 1, 1, ''),
(237480, 118649, -100, 1, 1, 1, 1, ''),
(237477, 118646, -100, 1, 1, 1, 1, ''),
(237478, 118647, -100, 1, 1, 1, 1, ''),
(237467, 118622, -100, 1, 1, 1, 1, ''),
(237466, 118621, -100, 1, 1, 1, 1, ''),
(237462, 118617, -100, 1, 1, 1, 1, ''),
(237464, 118619, -100, 1, 1, 1, 1, ''),
(237479, 118648, -100, 1, 1, 1, 1, ''),
(237483, 118652, -100, 1, 1, 1, 1, ''),
(237472, 118627, -100, 1, 1, 1, 1, ''),
(237473, 118628, -100, 1, 1, 1, 1, ''),
(237470, 118625, -100, 1, 1, 1, 1, ''),
(237471, 118626, -100, 1, 1, 1, 1, ''),
(237482, 118651, -100, 1, 1, 1, 1, ''),
(237474, 118643, -100, 1, 1, 1, 1, ''),
(237475, 118644, -100, 1, 1, 1, 1, ''),
(237484, 118653, -100, 1, 1, 1, 1, '');

UPDATE gameobject_template SET data1 = entry, data17 = 0, flags = flags & ~0x00000004 WHERE entry IN (
237469,
237468,
237481,
237461,
237463,
237465,
237480,
237477,
237478,
237467,
237466,
237462,
237464,
237479,
237483,
237472,
237473,
237470,
237471,
237482,
237474,
237475,
237484);

-- Everbloom
    DELETE FROM `gameobject` WHERE `id` IN (237472, 237473, 237483) AND `map` = 1279;
    INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
    ("237472","1279","7109","7109","6","1","762.92","1442.21","88.4421","5.07356","0","0","0","0","0","255","1","0",0),
    ("237473","1279","7109","7109","6","1","326.981","1490.24","102.041","0.623439","0","0","0","0","0","255","1","0",0),
    ("237483","1279","7109","7109","2","1","746.571","1567.3","108.295","0.0687468","0","0","0","0","0","255","1","0",0);

    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237472","3","6807","Strangely-Glowing Frond","questinteract","Collecting","","0","4","6","118627","0","0","0","0","0","1691","56764","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237483","3","24596","Rustling Peachick Nest","questinteract","Collecting","","0","4","1","118652","0","0","0","0","0","1691","56775","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237473","3","20499","Overgrown Artifact","questinteract","Collecting","","0","4","1","118628","0","0","0","0","0","1691","56765","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    
-- Grimrail Depot
    DELETE FROM `gameobject` WHERE `id` IN (237474, 237475, 237484) AND `map` = 1208;
    INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
    ("237474","1208","7109","7383","6","1","1724.637","1610.083","7.6916531","3.157218","0","0","0","0","0","255","1","0",0),
    ("237475","1208","7109","7109","6","1","1645.542","1632.62","108.4119","4.327895","-0.5236163","-0.154665","-0.7469788","0.3793777","0","255","1","0",0),
    ("237484","1208","7109","7109","2","1","1656.748","1836.932","109.7467","5.318283","0","0","-0.4639521","0.8858603","0","255","1","0",0);

    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237474","3","17016","Huge Crate of Weapons","questinteract","Collecting","","0","4","0.5","118643","0","0","0","0","0","1691","0","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237475","3","23157","Iron Limbcleaver","questinteract","Collecting","","0","4","2","118644","0","0","0","0","0","1691","56767","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237484","3","16604","Iron Autocannon","questinteract","Collecting","","0","4","0.3","118653","0","0","0","0","0","1691","56776","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    
-- Shadowmoon Burial Grounds
    DELETE FROM `gameobject` WHERE `id` IN (237470, 237471, 237482) AND `map` = 1176;
    INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
    ("237470","1176","7109","7383","6","1","1830.549","121.7431","294.5555","3.02442","0","0","0.9982843","0.05855254","0","255","1","0",0),
    ("237471","1176","7109","7109","6","1","1663.392","-231.9792","253.4646","4.6750775","-0.1768336","0.6938286","-0.1654396","0.6782046","0","255","1","0",0),
    ("237482","1176","7109","7109","2","1","1796.335","71.72569","264.1181","0","0","0","0","1","0","255","1","0",0);

    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237470","3","16790","Dark Parchment","questinteract","Collecting","","0","4","2","118625","0","0","0","0","0","1691","56762","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237471","3","11524","Silver-Lined Arrow","questinteract","Collecting","","0","4","1.5","118626","0","0","0","0","0","1691","56763","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237482","3","21962","Void-Gate Key","questinteract","Collecting","","0","4","1.5","118651","0","0","0","0","0","1691","56774","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    
-- Skyreach
    DELETE FROM `gameobject` WHERE `id` IN (237467, 237466) AND `map` = 1209;
    INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
    ("237467","1209","6988","6990","6","1","1236.616","1721.323","178.2672","2.991205","0","0","0.9971743","0.07512315","0","255","1","0",0),
    ("237466","1209","6988","6990","6","1","1663.392","-231.9792","253.4646","4.6750775","-0.1768336","0.6938286","-0.1654396","0.6782046","0","255","1","0",0);

    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237467","3","13726","Pristine Plumage","questinteract","Collecting","","0","4","1.5","118622","0","0","0","0","0","1691","56759","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237466","3","18978","Sun Crystal","questinteract","Collecting","","0","4","0.5","118621","0","0","0","0","0","1691","56758","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    
-- Auchindoun
    DELETE FROM `gameobject` WHERE `id` IN (237464, 237479) AND `map` = 1182;
    INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUES
    ("237464","1182","6912","6912","6","1","1685.444","2913.046","36.72615","6.200767","0.7028551","-0.03005886","-0.0282135","0.7101374","0","255","1","0",0),
    ("237479","1182","6912","6912","2","1","1734.241","2793.382","40.64299","6.212135","0","0","-0.03551769","0.999369","0","255","1","0",0);

    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237464","3","16683","Soulsever Blade","questinteract","Collecting","","0","4","1","118619","0","0","0","0","0","1691","56756","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","56766","0","0","0","","","0");
    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237479","3","5871","Nightmare Bell","questinteract","Collecting","","0","4","1","118648","0","0","0","0","0","1691","56771","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");
    
-- Iron Docks
    DELETE FROM `gameobject` WHERE `id` = 237462 AND `map` = 1195;
    INSERT INTO `gameobject` (`id`,`map`,`zoneId`,`areaId`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`rotation0`,`rotation1`,`rotation2`,`rotation3`,`spawntimesecs`,`animprogress`,`state`,`isActive`,`protect_anti_doublet`) VALUE
    ("237462","1195","6951","7309","6","1","6689.765","-768.1146","7.780828","0","0","0","0","1","0","255","1","0",0);

    REPLACE INTO `gameobject_template` (`entry`,`type`,`displayId`,`name`,`IconName`,`castBarCaption`,`unk1`,`faction`,`flags`,`size`,`questItem1`,`questItem2`,`questItem3`,`questItem4`,`questItem5`,`questItem6`,`data0`,`data1`,`data2`,`data3`,`data4`,`data5`,`data6`,`data7`,`data8`,`data9`,`data10`,`data11`,`data12`,`data13`,`data14`,`data15`,`data16`,`data17`,`data18`,`data19`,`data20`,`data21`,`data22`,`data23`,`data24`,`data25`,`data26`,`data27`,`data28`,`data29`,`data30`,`data31`,`data32`,`unkInt32`,`AIName`,`ScriptName`,`BuildVerified`) VALUES ("237462","3","23153","Horribly Acidic Solution","questinteract","Collecting","","0","4","2","118617","0","0","0","0","0","1691","56754","0","1","0","0","0","0","0","0","0","0","0","0","19676","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","0","","","0");