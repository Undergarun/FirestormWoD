-- fix Marked for Death
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_marked_for_death';
INSERT INTO `spell_script_names` VALUES ('140149', 'spell_rog_marked_for_death'); -- visual

DELETE FROM `spell_linked_spell` WHERE spell_trigger = 137619;
INSERT INTO `spell_linked_spell` VALUES (137619, 140149, 2, 'Marked for Death (check caster)');

-- fix Nightfall proc from Corruption
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_nightfall';
INSERT INTO `spell_script_names` VALUES (146739, 'spell_warl_nightfall');

-- fix Ember Tap stacking
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_ember_tap_glyph';
INSERT INTO `spell_script_names` VALUES (114635, 'spell_warl_ember_tap_glyph');

-- fix Chaos Bolt, removes stacks of Backdraft
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_chaos_bolt';
INSERT INTO `spell_script_names` VALUES (116858, 'spell_warl_chaos_bolt');

-- fix Divine Insight (shadow) if proc while player is casting
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_mind_blast';
INSERT INTO `spell_script_names` VALUES ('8092', 'spell_pri_mind_blast');

-- fix Remove Curse, should remove Hex
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_remove_curse';
INSERT INTO `spell_script_names` VALUES (475, 'spell_mage_remove_curse');

-- fix Doom Bolt, damage calculation
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_doom_bolt';
INSERT INTO `spell_script_names` VALUES (85692, 'spell_gen_doom_bolt');

-- fix Chaos Wave, trigger Molten Core
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_chaos_wave';
INSERT INTO `spell_script_names` VALUES (124916, 'spell_warl_chaos_wave');

-- fix Kill Command damage
DELETE FROM `spell_bonus_data` WHERE `entry`='83381';

-- fix Explosive Shot damage
DELETE FROM `spell_bonus_data` WHERE `entry`='53301';
INSERT INTO `spell_bonus_data` (`entry`, `ap_bonus`, `ap_dot_bonus`, `comments`) VALUES ('53301', '0.553', '0.553', 'Hunter - Explosive Shot');

-- fix Item - Hunter WoD PvP Marksmanship 4P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_aimed_shot';
INSERT INTO `spell_script_names` VALUES (19434, 'spell_hun_aimed_shot');

-- fix Poisoned Ammo
DELETE FROM `spell_bonus_data` WHERE `entry`='162543';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_poisoned_ammo';
INSERT INTO `spell_script_names` VALUES (162543, 'spell_hun_poisoned_ammo');

-- fix Item - Priest WoD PvP Shadow 2P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pri_dispersion';
INSERT INTO `spell_script_names` VALUES (47585, 'spell_pri_dispersion');

-- fix Item - Mage WoD PvP Frost 2P Bonus
DELETE FROM `areatrigger_template` WHERE `spell_id`=180723;
INSERT INTO `areatrigger_template` (`spell_id`, `entry`, `scale_x`, `scale_y`, `flags`, `ScriptName`) VALUES (180723, 180723, 1, 1, 16384, 'at_mage_wod_frost_2p_bonus');

-- fix Explosive Shot
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_explosive_shot';

-- fix Item – Rogue WoD PvP Subtlety 4P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_feint';
INSERT INTO `spell_script_names` VALUES (1966, 'spell_rog_feint');

-- fix Comet Storm
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_comet_storm';
INSERT INTO `spell_script_names` VALUES (153595, 'spell_mage_comet_storm');
INSERT INTO `spell_script_names` VALUES (153596, 'spell_mage_comet_storm');

-- fix Fingers of Frost
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_fingers_of_frost';
INSERT INTO `spell_script_names` VALUES (44544, 'spell_mage_fingers_of_frost');

-- fix Dream of Cenarius (feral)
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_dream_of_cenarius_feral';
INSERT INTO `spell_script_names` VALUES (5185, 'spell_dru_dream_of_cenarius_feral');
INSERT INTO `spell_script_names` VALUES (774, 'spell_dru_dream_of_cenarius_feral');

-- fix Item - Druid WoD PvP Restoration 2P Bonus
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_wod_pvp_2p_restoration';
INSERT INTO `spell_script_names` VALUES (22812, 'spell_dru_wod_pvp_2p_restoration');
INSERT INTO `spell_script_names` VALUES (102342, 'spell_dru_wod_pvp_2p_restoration');

-- fix Backstab
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_backstab';
INSERT INTO `spell_script_names` VALUES (53, 'spell_rog_backstab');

-- fix Subterfuge
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_stealth';
INSERT INTO `spell_script_names` VALUES (1784, 'spell_rog_stealth');
INSERT INTO `spell_script_names` VALUES (115191, 'spell_rog_stealth');
INSERT INTO `spell_script_names` VALUES (115192, 'spell_rog_stealth');

-- fix Vanish
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_vanish';
INSERT INTO `spell_script_names` VALUES (131361, 'spell_rog_vanish');

-- fix Sudden Death
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_sudden_death';

-- fix Glyph of Ignite
DELETE FROM `spell_linked_spell` WHERE `comment` LIKE '%Glyph of Ignite%';
INSERT INTO `spell_linked_spell` VALUES (-182287, -12654, 0, 'Glyph of Ignite - remove ignite with speed decrease effect');

-- fix Improved Rake
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_rake_triggered';
INSERT INTO `spell_script_names` VALUES (155722, 'spell_dru_rake_triggered');

-- fix Spirit Wolf (Feral Spirit spell)
DELETE FROM `pet_stats` WHERE `entry`=29264;
INSERT INTO `pet_stats` VALUES (29264, '1.14', 1, '1', '0.5', '0.3', '0.85', '2', '0', '-100', '1', 'Spirit Wolf');

-- fix Sanctified Wrath
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_sanctified_wrath';
INSERT INTO `spell_script_names` VALUES (31884, 'spell_pal_sanctified_wrath');

-- fix attack speed of all pets
UPDATE `pet_stats` SET `attackspeed`='2' WHERE `entry`='1';

-- fix Power Word: Shield absorb amount calculation
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_shield_discipline' AND `spell_id`=17;

-- fix Empowered Moonkin
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_empowered_moonkin';
INSERT INTO `spell_script_names` VALUES (5176, 'spell_dru_empowered_moonkin');
INSERT INTO `spell_script_names` VALUES (78674, 'spell_dru_empowered_moonkin');
INSERT INTO `spell_script_names` VALUES (152221, 'spell_dru_empowered_moonkin');
INSERT INTO `spell_script_names` VALUES (2912, 'spell_dru_empowered_moonkin');

-- fix Cat Form , Claws of Shirvallah with Savage Roar
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_cat_form';
INSERT INTO `spell_script_names` VALUES (768, 'spell_dru_cat_form');
INSERT INTO `spell_script_names` VALUES (171745, 'spell_dru_cat_form');

-- fix Pulverize
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_pulverize';
INSERT INTO `spell_script_names` VALUES (80313, 'spell_dru_pulverize');

-- Mastery: Primal Tenacity
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_primal_tenacity';
INSERT INTO `spell_script_names` VALUES (159195, 'spell_mastery_primal_tenacity');

-- fix usebug with Carrying Seaforium
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_carrying_seaforium';
INSERT INTO `spell_script_names` VALUES (52410, 'spell_gen_carrying_seaforium');

-- fix Enrage proc on absorb
DELETE FROM `spell_proc_event` WHERE `entry`=13046;
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES (13046, 0x0000002 + 0x0000400);

-- fix Mastery: Master Demonologist
UPDATE `spell_script_names` SET `spell_id`=104027 WHERE `spell_id`=6353 AND `ScriptName`='spell_mastery_master_demonologist';
INSERT INTO `spell_script_names` VALUES (157695, 'spell_mastery_master_demonologist');

-- fix Faerie Swarm damage
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_faerie_swarm_damage';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dru_faerie_swarm_speed_aura';
INSERT INTO `spell_script_names` VALUES (770, 'spell_dru_faerie_swarm_damage');
INSERT INTO `spell_script_names` VALUES (102355, 'spell_dru_faerie_swarm_damage');

-- fix Mastery: Master Demonologist 
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_master_demonologist';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_master_demonologist_aura';

-- fix Mastery: Weapons Master
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_weapons_master';

-- fix Divine Purpose
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_divine_purpose' AND `spell_id`=157048;
INSERT INTO `spell_script_names` VALUES (157048, 'spell_pal_divine_purpose');

-- fix Gargoyle Strike
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_gargoyle_strike';
INSERT INTO `spell_script_names` VALUES (51963, 'spell_dk_gargoyle_strike');

-- fix Single-Minded Fury
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_single_minded_fury';
INSERT INTO `spell_script_names` VALUES (81099, 'spell_warr_single_minded_fury');

-- fix Lock and Load
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_lock_and_load';
INSERT INTO `spell_script_names` VALUES (168980, 'spell_hun_lock_and_load');

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

-- fix Deadly Poison
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_deadly_poison_instant_damage';
INSERT INTO `spell_script_names` VALUES (2818, 'spell_rog_deadly_poison_instant_damage');

-- fix Selfless Healer proc on absorb
DELETE FROM `spell_proc_event` WHERE `entry`=85804;
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES (85804, 0x0000001 + 0x0000400);

-- fix Touch of the Grave
UPDATE `spell_proc_event` SET `SchoolMask`='0' WHERE `entry`='5227';

-- fix Will of the Forsaken and Stoneform sharable cooldown with PvP Trinkets
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_pvp_trinket_cooldown';
INSERT INTO `spell_script_names` VALUES (20594, 'spell_gen_pvp_trinket_cooldown');
INSERT INTO `spell_script_names` VALUES (7744, 'spell_gen_pvp_trinket_cooldown');
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_cooldown_from_pvp_trinket';
INSERT INTO `spell_script_names` VALUES (42292, 'spell_gen_cooldown_from_pvp_trinket');

-- fix Dark Soul with Archimonde's Darkness
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_dark_soul_archimondes';
INSERT INTO `spell_script_names` VALUES (113860, 'spell_warl_dark_soul_archimondes');

-- fix Backdraft with Chaos Bolt and Incinerate
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_incinerate';
INSERT INTO `spell_script_names` VALUES (29722, 'spell_warl_incinerate');
INSERT INTO `spell_script_names` VALUES (108685, 'spell_warl_incinerate');

-- fix Pyroblast Clearcasting Driver
DELETE FROM `spell_proc_event` WHERE `entry`='44448';
INSERT INTO `spell_proc_event` (`entry`, `procEx`) VALUES ('44448', 0x00000001 + 0x00000002 + 0x00000400);

-- fix Command Demon
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_command_demon_spells';
INSERT INTO `spell_script_names` VALUES (119905, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119907, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119909, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119910, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119911, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119913, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119914, 'spell_warl_command_demon_spells');
INSERT INTO `spell_script_names` VALUES (119915, 'spell_warl_command_demon_spells');

-- fix Elixir of Wandering Spirits
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_elixir_of_wandering_spirits';
INSERT INTO `spell_script_names` VALUES (147412, 'spell_gen_elixir_of_wandering_spirits');

-- fix Infernal (Demonic Servitude)
DELETE FROM `pet_stats` WHERE `entry`=78217;
INSERT INTO `pet_stats` VALUES (78217, 1.14, 1, 3, 1, 0.4, 1, 2, 3, -100, 0.065934, 'Infernal (Demonic Servitude)');

-- fix Dark Infusion with Breath of Sindragosa
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_breath_of_sindragosa';
INSERT INTO `spell_script_names` VALUES (152279 , 'spell_dk_breath_of_sindragosa');

-- fix Taunt
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=355 AND `spell_effect`=71;
REPLACE INTO `spell_linked_spell` VALUES (355, 71, 0, 'Taunt trigger Defensive Stance');

-- fix Shield Charge increase damage of Heroic Strike and Revenge
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_shield_charge_damage';
INSERT INTO `spell_script_names` VALUES (78, 'spell_warr_shield_charge_damage');
INSERT INTO `spell_script_names` VALUES (6572, 'spell_warr_shield_charge_damage');

-- fix Snake Trap with Trap Launcher
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_trap_launcher';
INSERT INTO `spell_script_names` VALUES ('77769', 'spell_hun_trap_launcher');

-- fix Cripple
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_cripple_doomguard';
INSERT INTO `spell_script_names` VALUES ('170995', 'spell_warl_cripple_doomguard');

-- fix Glyph of the Weaponmaster
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_weaponmaster';
INSERT INTO `spell_script_names` VALUES (147367, 'spell_warr_weaponmaster');

-- fix Ebon Gargoyle
UPDATE `pet_stats` SET `secondarystat_coef`='0.46625' WHERE `entry`='27829';

-- fix Blessing of Might and Blessing of Kings stacking
DELETE FROM `spell_group` WHERE `id`=1002;
DELETE FROM `spell_group_stack_rules` WHERE `group_id`=1002;
INSERT INTO `spell_group` VALUES (1002, 19740);
INSERT INTO `spell_group` VALUES (1002, 20217);
INSERT INTO `spell_group_stack_rules` VALUES (1002, 2);

-- fix Battle/Commanding shout stacking
DELETE FROM `spell_group` WHERE `id`=1003;
DELETE FROM `spell_group_stack_rules` WHERE `group_id`=1003;
INSERT INTO `spell_group` VALUES (1003, 469);
INSERT INTO `spell_group` VALUES (1003, 6673);
INSERT INTO `spell_group_stack_rules` VALUES (1003, 1);

-- cleanup spell_group not needed group, that breakes 1002 and 1003 group
DELETE FROM `spell_group` WHERE `id`=1004;
DELETE FROM `spell_group_stack_rules` WHERE `group_id`=1004;

-- fix Molten Earth damage
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mastery_molten_earth_damage';
INSERT INTO `spell_script_names` VALUES (170379, 'spell_mastery_molten_earth_damage');

-- fix Searing Totem size
UPDATE `creature_template` SET `scale`='3' WHERE `entry`='2523';

-- fix Grand Crusader
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_grand_crusader';
INSERT INTO `spell_script_names` VALUES (35395, 'spell_pal_grand_crusader');
INSERT INTO `spell_script_names` VALUES (53595, 'spell_pal_grand_crusader');

-- fix Breath of the Serpent
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_monk_breath_of_the_serpent_tick';
INSERT INTO `spell_script_names` VALUES (157627, 'spell_monk_breath_of_the_serpent_tick');
UPDATE `creature_template` SET `modelid1`='0' WHERE `entry`='78065';

-- fix Black ox Statue
UPDATE `creature_template` SET `unit_flags`='131596' WHERE `entry`='61146';

-- fix Leather Specialization (Feral)
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_gen_leather_specialization';
INSERT INTO `spell_script_names` VALUES (86097, 'spell_gen_leather_specialization')

-- fix Doomguard and Terrorguard
UPDATE `creature_template` SET `ScriptName`='spell_npc_warl_doomguard' WHERE `entry` IN (78158, 78215);

-- disable some spells for snake trap
DELETE FROM `disables` WHERE `entry` IN (34655, 25810);
INSERT INTO `disables` VALUES (0, 34655, 2, '', '', 'Snake Trap - Deadly Poison');
INSERT INTO `disables` VALUES (0, 25810, 2, '', '', 'Snake Trap - Mind Numbing Poison');

-- fix Sword of Light increase spell damage
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_sword_of_light_damage';
INSERT INTO `spell_script_names` VALUES (20271, 'spell_pal_sword_of_light_damage');
INSERT INTO `spell_script_names` VALUES (53385, 'spell_pal_sword_of_light_damage');
INSERT INTO `spell_script_names` VALUES (157048, 'spell_pal_sword_of_light_damage');
INSERT INTO `spell_script_names` VALUES (24275, 'spell_pal_sword_of_light_damage');
INSERT INTO `spell_script_names` VALUES (158392, 'spell_pal_sword_of_light_damage');

-- Fix Empowered Obliterate
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_empowered_obliterate_icy_touch';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_empowered_obliterate_howling_blast';

--- Fix Ruthlessness and Relentless Strikes
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rog_ruthlessness_and_relentless_strikes';
INSERT INTO `spell_script_names` VALUES (408, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (2098, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (73651, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (5171, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (26679, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (1943, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (32645, 'spell_rog_ruthlessness_and_relentless_strikes');
INSERT INTO `spell_script_names` VALUES (121411, 'spell_rog_ruthlessness_and_relentless_strikes');

-- fix Mastery: Unshackled Fury
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_mastery_unshackled_fury';
INSERT INTO `spell_script_names` VALUES (12880, 'spell_warr_mastery_unshackled_fury');

-- fix Dark Souls with Archimondes
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warl_dark_soul_archimondes';
INSERT INTO `spell_script_names` VALUES (113858, 'spell_warl_dark_soul_archimondes');
INSERT INTO `spell_script_names` VALUES (113860, 'spell_warl_dark_soul_archimondes');
INSERT INTO `spell_script_names` VALUES (113861, 'spell_warl_dark_soul_archimondes');

-- fix Glyph of Denounce
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_glyph_of_denounce';
INSERT INTO `spell_script_names` VALUES (115654, 'spell_pal_glyph_of_denounce');

-- fix Sword of Light
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_sword_of_light_damage' AND spell_id IN (157048, 53385);

-- fix Improved Death Grip
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_improved_death_grip';

-- fix Glyph of Levitate
DELETE FROM spell_script_names WHERE `ScriptName` = 'spell_pri_path_of_devout';
INSERT INTO spell_script_names VALUE (111758, 'spell_pri_path_of_devout');
DELETE FROM spell_script_names WHERE `ScriptName` = 'spell_pri_glyph_of_levitate';
INSERT INTO spell_script_names VALUE (108939, 'spell_pri_glyph_of_levitate');

-- fix Glyph of Rain of Frogs
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_sha_glyph_of_rain_of_frogs';
INSERT INTO `spell_script_names` VALUES (147707, 'spell_sha_glyph_of_rain_of_frogs');


DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_pal_sword_of_light_damage' AND spell_id IN (157048, 53385);

-- fix Shadow Infusion
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_shadow_infusion';

-- fix Might of the Frozen
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_dk_might_of_the_frozen_wastes';
INSERT INTO `spell_script_names` VALUES (81333, 'spell_dk_might_of_the_frozen_wastes');

-- fix Glyph of Raging Blow
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_warr_glyph_of_raging_blow';
INSERT INTO `spell_script_names` VALUES (159740, 'spell_warr_glyph_of_raging_blow');
