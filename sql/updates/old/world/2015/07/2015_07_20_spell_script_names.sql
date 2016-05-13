-- fix Divine Purpose
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_divine_purpose' AND `spell_id`=157048;
INSERT INTO `spell_script_names` VALUES (157048, 'spell_pal_divine_purpose');

-- fix Metamorphosis : Demonic Circle : Teleport
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_demonic_circle_teleport';
INSERT INTO `spell_script_names` VALUES (48020, 'spell_warl_demonic_circle_teleport');
INSERT INTO `spell_script_names` VALUES (114794, 'spell_warl_demonic_circle_teleport');
INSERT INTO `spell_script_names` VALUES (104136, 'spell_warl_demonic_circle_teleport');

-- fix Item - Druid WoD PvP Balance 2P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_wod_pvp_balance_2p';
INSERT INTO `spell_script_names` VALUES (339, 'spell_dru_wod_pvp_balance_2p');

-- fix Glyph of Energy Flows
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_glyph_of_energy_flows';
INSERT INTO `spell_script_names` VALUES (159636, 'spell_rog_glyph_of_energy_flows');

-- fix Glyph of Energy Flows
DELETE FROM `spell_proc_event` WHERE `entry`=159636;
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES (159636, 0x0000004 + 0x0000010);

-- fix Lunar Inspiration
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_lunar_inspiration';
INSERT INTO `spell_script_names` VALUES (155580, 'spell_dru_lunar_inspiration');

-- fix Find Weakness
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_find_weakness';
INSERT INTO `spell_script_names` VALUES (91023, 'spell_rog_find_weakness');
