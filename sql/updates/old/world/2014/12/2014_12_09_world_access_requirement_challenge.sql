-- Challenge mode data
DELETE FROM `access_requirement` WHERE `difficulty` = 8; -- Remove all challenge mode

-- Add the WoD entries
INSERT INTO `access_requirement` (`mapId`, `difficulty`, `level_min`, `level_max`, `item`, `item2`, `quest_done_A`, `quest_done_H`, `completed_achievement`, `leader_achievement`, `itemlevel_min`, `itemlevel_max`, `quest_failed_text`, `comment`)
VALUES
    (1175, 8, 101, 0, 0, 0, 0, 0, 0, 0, 500, 0, NULL, 'Challenge : Bloodmaul Slag Mines'),
    (1195, 8, 101, 0, 0, 0, 0, 0, 0, 0, 520, 0, NULL, 'Challenge : Iron Docks'),
    (1182, 8, 101, 0, 0, 0, 0, 0, 0, 0, 540, 0, NULL, 'Challenge : Auchindoun'),
    (1209, 8, 101, 0, 0, 0, 0, 0, 0, 0, 580, 0, NULL, 'Challenge : Skyreach'),
    (1208, 8, 101, 0, 0, 0, 0, 0, 0, 0, 580, 0, NULL, 'Challenge : Grimrail Depot'),
    (1176, 8, 101, 0, 0, 0, 0, 0, 0, 0, 595, 0, NULL, 'Challenge : Shadowmoon Burial Grounds'),
    (1279, 8, 101, 0, 0, 0, 0, 0, 0, 0, 595, 0, NULL, 'Challenge : The Everbloom'),
    (1358, 8, 101, 0, 0, 0, 0, 0, 0, 0, 595, 0, NULL, 'Challenge : Upper Blackrock Spire');