-- Loots for Lei Shi
DELETE FROM gameobject_template WHERE entry IN (213076, 213075);
INSERT INTO gameobject_template VALUES
-- Normal mode
(213076, 3, 10316, 'Box of Fancy Stuff', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 213076, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode
(213075, 3, 10316, 'Box of Fancy Stuff', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 213075, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595);

-- Normal mode
SET @REF_LEI_SHI_1 = 213076;
DELETE FROM `reference_loot_template` WHERE entry = @REF_LEI_SHI_1;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_LEI_SHI_1, 86342, 0, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */,
(@REF_LEI_SHI_1, 86333, 0, 1, 1, 1, 1) /* 3Cuirass of the Animated Protector */,
(@REF_LEI_SHI_1, 86336, 0, 1, 1, 1, 1) /* 3Darkmist Vortex */,
(@REF_LEI_SHI_1, 86337, 0, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */,
(@REF_LEI_SHI_1, 86338, 0, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */,
(@REF_LEI_SHI_1, 86335, 0, 1, 1, 1, 1) /* 3Jin'ya, Orb of the Waterspeaker */,
(@REF_LEI_SHI_1, 86383, 0, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */,
(@REF_LEI_SHI_1, 86385, 0, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */,
(@REF_LEI_SHI_1, 86384, 0, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */,
(@REF_LEI_SHI_1, 86343, 0, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */,
(@REF_LEI_SHI_1, 86334, 0, 1, 1, 1, 1) /* 3Robes of the Unknown Fear */,
(@REF_LEI_SHI_1, 89246, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Conqueror */,
(@REF_LEI_SHI_1, 89247, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Protector */,
(@REF_LEI_SHI_1, 89248, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Vanquishe */,
(@REF_LEI_SHI_1, 86339, 0, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */,
(@REF_LEI_SHI_1, 86391, 0, 1, 1, 1, 1) /* 3Spiritsever */,
(@REF_LEI_SHI_1, 86341, 0, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */,
(@REF_LEI_SHI_1, 86331, 0, 1, 1, 1, 1) /* 3Taoren, the Soul Burner */,
(@REF_LEI_SHI_1, 86332, 0, 1, 1, 1, 1) /* 3Terror in the Mists */,
(@REF_LEI_SHI_1, 86340, 0, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */;
-- Mode 10/25
SET @NM_ENTRY_LEI_SHI = 213076; 
UPDATE `gameobject_template` SET `data1`= @NM_ENTRY_LEI_SHI WHERE `entry`= @NM_ENTRY_LEI_SHI;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @NM_ENTRY_LEI_SHI ;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@NM_ENTRY_LEI_SHI, 1, 100, 1, 0, -@REF_LEI_SHI_1, 2) /* REF 1*/; 

-- Heroic Mode
SET @REF_LEI_SHI_2 = 213075;
DELETE FROM `reference_loot_template` WHERE entry = @REF_LEI_SHI_2;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_LEI_SHI_2, 87183, 0, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */,
(@REF_LEI_SHI_2, 87171, 0, 1, 1, 1, 1) /* 3Cuirass of the Animated Protector */,
(@REF_LEI_SHI_2, 87172, 0, 1, 1, 1, 1) /* 3Darkmist Vortex */,
(@REF_LEI_SHI_2, 87178, 0, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */,
(@REF_LEI_SHI_2, 87177, 0, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */,
(@REF_LEI_SHI_2, 87170, 0, 1, 1, 1, 1) /* 3Jin'ya, Orb of the Waterspeaker */,
(@REF_LEI_SHI_2, 87184, 0, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */,
(@REF_LEI_SHI_2, 87186, 0, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */,
(@REF_LEI_SHI_2, 87185, 0, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */,
(@REF_LEI_SHI_2, 87182, 0, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */,
(@REF_LEI_SHI_2, 87169, 0, 1, 1, 1, 1) /* 3Robes of the Unknown Fear */,
(@REF_LEI_SHI_2, 89262, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Conqueror */,
(@REF_LEI_SHI_2, 89263, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Protector */,
(@REF_LEI_SHI_2, 89261, 0, 1, 1, 1, 1) /* 3Shoulders of the Shadowy Vanquishe */,
(@REF_LEI_SHI_2, 87179, 0, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */,
(@REF_LEI_SHI_2, 87166, 0, 1, 1, 1, 1) /* 3Spiritsever */,
(@REF_LEI_SHI_2, 87180, 0, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */,
(@REF_LEI_SHI_2, 87168, 0, 1, 1, 1, 1) /* 3Taoren, the Soul Burner */,
(@REF_LEI_SHI_2, 87167, 0, 1, 1, 1, 1) /* 3Terror in the Mists */,
(@REF_LEI_SHI_2, 87181, 0, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */;
-- Mode 10/25
SET @HM_ENTRY_LEI_SHI = 213075; 
DELETE FROM `gameobject_loot_template` WHERE `entry`= @HM_ENTRY_LEI_SHI ;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@HM_ENTRY_LEI_SHI, 1, 100, 1, 0, -@REF_LEI_SHI_2, 4) /* REF 1*/;