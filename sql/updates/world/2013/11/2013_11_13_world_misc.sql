UPDATE battleground_template SET MinPlayersPerTeam=20, MinLvl=91, MaxLvl=91, Weight=0  WHERE id=30;

DELETE FROM `spell_proc_event` WHERE `entry` IN (89003);
INSERT INTO `spell_proc_event` VALUES (89003, 0x00, 0x04, 0x00000000, 0x40000000, 0x00000000, 0x00000010, 0x00000000, 0, 100, 0);

DELETE FROM `spell_proc_event` WHERE `entry` IN (54939);
INSERT INTO `spell_proc_event` VALUES (54939, 0x00, 0x0A, 0x00008000, 0x00000000, 0x00000000, 0x00004400, 0x00000000, 0, 100, 0);

DELETE FROM spell_script_names WHERE ScriptName='spell_pri_train_of_thought';
DELETE FROM spell_script_names WHERE ScriptName='spell_pri_spirit_of_redemption';
DELETE FROM spell_script_names WHERE ScriptName='spell_warl_demonic_leap_jump';

UPDATE creature_template SET unit_flags=8388608 WHERE entry=1964;

UPDATE spell_bonus_data SET direct_bonus='0.8926' WHERE entry=32379;

UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 69070; 

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(31224, 81549, 2, 'Rogue - Cloak of Shadows');

UPDATE creature_template SET scriptname='npc_mirror_image' WHERE entry in (31216, 53438);
UPDATE creature_template SET minlevel=85, maxlevel=85, faction_A=35, faction_H=35, scriptname='npc_mirror_image' WHERE entry in (31216, 53438);
UPDATE creature_template SET exp=3, mindmg=550, maxdmg=950, attackpower=36, dmg_multiplier=7, unit_class=8, unit_flags=24, unit_flags2=2064, flags_extra=0 WHERE entry in (53438);
UPDATE creature_template SET dynamicflags=12, spell1=0, spell2=0 WHERE entry in (31216);

REPLACE INTO `spell_proc_event` VALUES (86701, 0, 0, 0, 0, 0, 0, 1027, 0, 100, 0);
REPLACE INTO `spell_proc_event` VALUES (86703, 0, 0, 0, 0, 0, 0, 1027, 0, 100, 0);

DELETE FROM `spell_proc_event` WHERE `entry` IN (51522, 10400, 33757);
INSERT INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES 
(33757, 0, 0, 0, 0, 0, 0, 1027, 0, 100, 3),
(10400, 0, 0, 0, 0, 0, 0, 1027, 0, 0, 0),
(51522, 0, 0, 0, 0, 0, 0, 1027, 0, 0, 0);

DELETE FROM spell_script_names WHERE scriptname='spell_sha_lava_surge';

DELETE FROM `spell_bonus_data` WHERE (`entry`='56641');
DELETE FROM `spell_bonus_data` WHERE (`entry`='3044');
