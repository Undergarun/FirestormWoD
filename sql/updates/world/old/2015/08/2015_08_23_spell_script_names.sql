-- fix Selfless Healer proc on absorb
DELETE FROM `spell_proc_event` WHERE `entry`=85804;
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES (85804, 0x0000002 + 0x0000400);

-- fix Touch of the Grave
UPDATE `spell_proc_event` SET `SchoolMask`='0' WHERE `entry`='5227';

-- fix Will of the Forsaken and Stoneform sharable cooldown with PvP Trinkets
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_pvp_trinket_cooldown';
INSERT INTO `spell_script_names` VALUES (20594, 'spell_gen_pvp_trinket_cooldown');
INSERT INTO `spell_script_names` VALUES (7744, 'spell_gen_pvp_trinket_cooldown');
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_cooldown_from_pvp_trinket';
INSERT INTO `spell_script_names` VALUES (42292, 'spell_gen_cooldown_from_pvp_trinket');

DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_dk_unholy_presence', 'spell_dk_improved_blood_presence');

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_improved_presences';
INSERT INTO `spell_script_names` VALUES
(48263, 'spell_dk_improved_presences'),
(48265, 'spell_dk_improved_presences'),
(48266, 'spell_dk_improved_presences');
