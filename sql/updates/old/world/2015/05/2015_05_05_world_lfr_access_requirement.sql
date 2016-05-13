DELETE FROM `access_requirement` WHERE `mapId` = 409 AND `difficulty` = 18;
DELETE FROM `access_requirement` WHERE `difficulty` IN (7, 17);
INSERT INTO `access_requirement` VALUES
(409, 18, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Molten Core - WoW birthday');

DROP TABLE IF EXISTS `lfr_access_requirement`;
CREATE TABLE `lfr_access_requirement`
(
    `dungeon_id` INT(10) UNSIGNED NOT NULL,
    `level_min` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `level_max` TINYINT(3) UNSIGNED NOT NULL DEFAULT '0',
    `item` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `item2` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `quest_A` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `quest_H` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `achievement` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `leader_achievement` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `ilvl_min` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `ilvl_max` INT(10) UNSIGNED NOT NULL DEFAULT '0',
    `quest_failed_text` TEXT,
    `comment` TEXT,
    PRIMARY KEY (`dungeon_id`)
);

DELETE FROM `lfr_access_requirement` WHERE `dungeon_id` IN (416, 417, 526, 527, 528, 529, 530, 610, 611, 612, 613, 716, 717, 718, 719, 849, 850, 851, 847, 846, 848, 823);
INSERT INTO `lfr_access_requirement` VALUES
(416, 85, 85, 0, 0, 0, 0, 0, 0, 372, 0, '', 'Dragon Soul - Part 1 : Siege of Wyrmrest Temple'),
(417, 85, 85, 0, 0, 0, 0, 6106, 0, 372, 0, '', 'Dragon Soul - Part 2 : Fall of Deathwing'),
(526, 90, 90, 0, 0, 0, 0, 0, 0, 470, 0, '', 'Terrace of Endless Spring'),
(527, 90, 90, 0, 0, 0, 0, 0, 0, 460, 0, '', 'Mogu''shan Vaults - Part 1 : Guardians of Mogu''shan'),
(528, 90, 90, 0, 0, 0, 0, 6844, 0, 460, 0, '', 'Mogu''shan Vaults - Part 2 : The Vault of Mysteries'),
(529, 90, 90, 0, 0, 0, 0, 0, 0, 470, 0, '', 'Heart of Fear - Part 1 : The Dread Approach'),
(530, 90, 90, 0, 0, 0, 0, 6718, 0, 470, 0, '', 'Heart of Fear - Part 2 : Nightmare of Shek''zeer'),
(610, 90, 90, 0, 0, 0, 0, 0, 0, 480, 0, '', 'Throne of Thunder - Part 1 : Last Stand of the Zandalari'),
(611, 90, 90, 0, 0, 0, 0, 8069, 0, 480, 0, '', 'Throne of Thunder - Part 2 : Forgotten Depths'),
(612, 90, 90, 0, 0, 0, 0, 8070, 0, 480, 0, '', 'Throne of Thunder - Part 3 : Halls of Flesh-Shaping'),
(613, 90, 90, 0, 0, 0, 0, 8071, 0, 480, 0, '', 'Throne of Thunder - Part 4 : Pinnacle of Storms'),
(716, 90, 90, 0, 0, 0, 0, 0, 0, 496, 0, '', 'Siege of Orgrimmar - Part 1 : Vale of Eternal Sorrows'),
(717, 90, 90, 0, 0, 0, 0, 8458, 0, 496, 0, '', 'Siege of Orgrimmar - Part 2 : Gates of Retribution'),
(718, 90, 90, 0, 0, 0, 0, 8459, 0, 496, 0, '', 'Siege of Orgrimmar - Part 3 : The Underhold'),
(719, 90, 90, 0, 0, 0, 0, 8461, 0, 496, 0, '', 'Siege of Orgrimmar - Part 4 : Downfall'),
(849, 100, 100, 0, 0, 0, 0, 0, 0, 615, 0, '', 'Highmaul - Part 1 : The Walled City'),
(850, 100, 100, 0, 0, 0, 0, 8986, 0, 999, 999, '', 'Highmaul - Part 2 : Arcane Sanctum'),
(851, 100, 100, 0, 0, 0, 0, 8987, 0, 999, 999, '', 'Highmaul - Part 3 : Imperator''s Fall'),
(847, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Blackrock Foundry - Part 1 : Slagworks'),
(846, 100, 100, 0, 0, 0, 0, 8989, 0, 999, 999, '', 'Blackrock Foundry - Part 2 : Black Forge'),
(848, 100, 100, 0, 0, 0, 0, 8990, 0, 999, 999, '', 'Blackrock Foundry - Part 3 : Iron Assembly'),
(823, 100, 100, 0, 0, 0, 0, 8991, 0, 999, 999, '', 'Blackrock Foundry - Part 4 : Blackhand''s Crucible');

# Those are new, don't know why, but we must disable them
DELETE FROM `lfr_access_requirement` WHERE `dungeon_id` IN (830, 831, 832, 833, 834, 835, 836, 837, 838, 839, 840, 841, 842, 843, 844);
INSERT INTO `lfr_access_requirement` VALUES
(830, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(831, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(832, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(833, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(834, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(835, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(836, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(837, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(838, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(839, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(840, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(841, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(842, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(843, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled'),
(844, 100, 100, 0, 0, 0, 0, 0, 0, 999, 999, '', 'Disabled');

# LFG Entrances must be defined, if not, they're not in the available dungeon list
DELETE FROM `lfg_entrances` WHERE `dungeonId` IN (716, 717, 724, 725, 839, 840, 841, 842, 849, 850, 851, 847, 846, 848, 823);
INSERT INTO `lfg_entrances` VALUES
(716, 'Garrosh Raid - Entrance Target', 1440.89417, 263.046875, 283.557953, 1.5706),
(717, 'Garrosh Raid - Pre-Galakras Transfer Loc', 1440.24829, -5015.09033, 12.158349, 1.6646),
(724, 'Garrosh Raid - Pre-Malkorok Transfer Loc', 1795.51221, -4774.24854, -254.60051, 5.304984),
(725, 'Garrosh Raid - Underhold Nexus Transfer Loc', 1992.64929, -5169.936, -270.238434, 3.78254468),
(839, 'Garrosh Raid - Entrance Target', 1440.89417, 263.046875, 283.557953, 1.5706),
(840, 'Garrosh Raid - Pre-Galakras Transfer Loc', 1440.24829, -5015.09033, 12.158349, 1.6646),
(841, 'Garrosh Raid - Pre-Malkorok Transfer Loc', 1795.51221, -4774.24854, -254.60051, 5.304984),
(842, 'Garrosh Raid - Underhold Nexus Transfer Loc', 1992.64929, -5169.936, -270.238434, 3.78254468),
(849, 'Highmaul Raid - Instance Entrance', 3486.48, 7603.32, 10.4853, 4.025183),
(850, 'Highmaul Raid - Kargath Defeated', 3500.13, 7619.39, 55.3054, 0.899281),
(851, 'Highmaul Raid - Imperator''s Rise Entrance', 4180.11, 8575, 572.572, 3.0801396),
(847, 'Blackrock Foundry - Temp', 0, 0, 0, 0),
(846, 'Blackrock Foundry - Temp', 0, 0, 0, 0),
(848, 'Blackrock Foundry - Temp', 0, 0, 0, 0),
(823, 'Blackrock Foundry - Temp', 0, 0, 0, 0);

# LFR rewards for Highmaul and Blackrock Foundry
DELETE FROM `lfg_dungeon_rewards` WHERE `dungeonId` IN (788, 789, 849, 850, 851, 847, 846, 848, 823);
INSERT INTO `lfg_dungeon_rewards` VALUES
(788, 100, 37335, 0, 0, 37336, 0, 0),
(789, 100, 37333, 0, 0, 37334, 0, 0),
(849, 100, 37505, 0, 0, 37506, 0, 0),
(850, 100, 137505, 0, 0, 137506, 0, 0),
(851, 100, 237505, 0, 0, 237506, 0, 0),
(847, 100, 337505, 0, 0, 337506, 0, 0),
(846, 100, 437505, 0, 0, 437506, 0, 0),
(848, 100, 537505, 0, 0, 537506, 0, 0),
(823, 100, 637505, 0, 0, 637506, 0, 0);

DELETE FROM `quest_template` WHERE `Id` IN (37333, 37334, 37335, 37336, 37505, 37506, 137505, 137506, 237505, 237506, 337505, 337506, 437505, 437506, 537505, 537506, 637505, 637506);
INSERT INTO `quest_template` (Id, Method, LEVEL, MinLevel, MaxLevel, TYPE, RewardMoney, Flags, SpecialFlags, Title, RewardCurrencyId1, RewardCurrencyCount1) VALUES
(37333, 2, 100, 100, 100, 62, 693000, 5120, 8, '[Daily Heroic Random (1st) (Draenor)]', 824, 50),
(37334, 2, 100, 100, 100, 62, 308000, 5120, 8, '[Daily Heroic Random (Nth) (Draenor)]', 0, 0),
(37335, 2, 100, 100, 100, 62, 308000, 5120, 8, '[Daily Normal Random (1st) (Draenor)]', 0, 0),
(37336, 2, 100, 100, 100, 62, 154000, 5120, 8, '[Daily Normal Random (Nth)(Draenor)]', 0, 0),
(37505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(37506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0),
(137505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(137506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0),
(237505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(237506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0),
(337505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(337506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0),
(437505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(437506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0),
(537505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(537506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0),
(637505, 2, 100, 100, 100, 62, 1540000, 33792, 8, '[Raid Finder (1st)]', 0, 0),
(637506, 2, 100, 100, 100, 62, 616000, 33792, 8, '[Raid Finder (Nth)]', 0, 0);