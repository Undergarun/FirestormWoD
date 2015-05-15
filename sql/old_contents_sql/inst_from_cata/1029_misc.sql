-- Item - Paladin T12 Protection 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99074;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99074, 0, 10, 0, 1048576, 0, 0, 1027, 0, 0, 0);

-- Item - Paladin T12 Retribution 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99093;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99093, 0, 10, 0, 32768, 0, 0, 1027, 0, 0, 0);

-- Item - Paladin T12 Holy 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99067;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99067, 0, 10, 0, 65536, 0, 0, 1027, 0, 0, 0);

-- Item - Death Knight T12 DPS 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=98970;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(98970, 0, 15, 0, 1073741824, 0, 0, 0, 0, 0, 0);

-- Item - Death Knight T12 DPS 4P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=98996;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(98996, 0, 15, 0, 134348800, 0, 0, 1027, 0, 0, 0);

-- Item  Druid T12 Feral 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99001;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99001, 0, 7, 34816, 1088, 0, 0, 1027, 0, 0, 0);

-- Item  Druid T12 Feral 4P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99009;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99009, 0, 7, 8388608, 128, 200000, 16, 1027, 0, 100, 0);

-- Item  Druid T12 Balance 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99019;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99019, 0, 7, 5, 0, 0, 0, 1027, 0, 20, 45);

-- Item  Druid T12 Restoration 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99013;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99013, 0, 7, 0, 16, 0, 0, 1027, 0, 0, 0);

-- Item  Druid T12 Restoration 4P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99015;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99015, 0, 7, 0, 2, 0, 0, 1027, 0, 0, 0);

-- Item  Hunter T12 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99057;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99057, 0, 9, 0, 1, 4194304, 0, 1027, 0, 0, 0);

-- Item  Mage T12 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99061;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99061, 0, 3, 536870945, 4096, 8, 0, 1027, 0, 0, 45);

UPDATE `creature_template` SET `equipment_id`=31216, `ScriptName`='npc_mirror_image' WHERE `entry`=53438;

-- Item  Priest Healer T12 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99134;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99134, 0, 6, 7168, 0, 0, 0, 1027, 0, 0, 0);

UPDATE `creature_template` SET `spell1`=102129, `spell2`=99153 WHERE `entry`=53475;

-- Item  Rogue T12 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99174;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99174, 0, 0, 0, 0, 0, 0, 1026, 0, 0, 0);

-- Item  Shaman T12 Enhancement 4P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99213;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99213, 0, 11, 0, 16777217, 0, 16, 1027, 0, 100, 0);

-- Item  Shaman T12 Elemental 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99204;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99204, 0, 11, 1, 0, 0, 65536, 1027, 0, 0, 0);

-- Item  Shaman T12 Restoration 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99190;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99190, 0, 11, 0, 0, 16, 0, 1027, 0, 0, 0);

-- Item  Warlock T12 4P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99229;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99229, 0, 5, 16385, 192, 0, 0, 1027, 0, 0, 0);

-- Item  Warrior T12 Protection 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99239;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99239, 0, 4, 0, 512, 0, 16, 1027, 0, 100, 0);

-- Item  Warrior T12 DPS 2P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99234;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(99234, 0, 4, 65536, 128, 0, 17408, 1027, 0, 0, 0);

-- Item  Warrior T12 DPS 4P Bonus
DELETE FROM `spell_proc_event` WHERE `entry`=99239;
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(99239, 0, 4, 33554432, 67117056, 0, 16, 1027, 0, 0, 0);