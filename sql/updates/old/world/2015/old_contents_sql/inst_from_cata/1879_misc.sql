DELETE FROM `spell_script_names` WHERE `spell_id`=75973 AND `ScriptName`='spell_x53_touring_rocket';
DELETE FROM `spell_script_names` WHERE `spell_id`=75614 AND `ScriptName`='spell_celestial_steed';
DELETE FROM `spell_script_names` WHERE `spell_id`=72286 AND `ScriptName`='spell_invincible';
DELETE FROM `spell_script_names` WHERE `spell_id`=71342 AND `ScriptName`='spell_big_love_rocket';
DELETE FROM `spell_script_names` WHERE `spell_id`=74805 AND `ScriptName`='spell_halion_summon_exit_portals';
DELETE FROM `spell_script_names` WHERE `spell_id`=65917 AND `ScriptName`='spell_gen_magic_rooster';
DELETE FROM `spell_script_names` WHERE `spell_id`=54846 AND `ScriptName`='spell_dru_glyph_of_starfire';
DELETE FROM `spell_script_names` WHERE `spell_id`=54729 AND `ScriptName`='spell_winged_steed_of_the_ebon_blade';
DELETE FROM `spell_script_names` WHERE `spell_id`=47977 AND `ScriptName`='spell_magic_broom';
DELETE FROM `spell_script_names` WHERE `spell_id`=-52284 AND `ScriptName`='spell_dk_will_of_the_necropolis';
DELETE FROM `spell_script_names` WHERE `spell_id`=31687 AND `ScriptName`='spell_mage_summon_water_elemental';
DELETE FROM `spell_script_names` WHERE `spell_id`=24531 AND `ScriptName`='spell_item_refocus';
DELETE FROM `spell_script_names` WHERE `spell_id`=-11113 AND `ScriptName`='spell_mage_blast_wave';
DELETE FROM `spell_script_names` WHERE `spell_id`=74856 AND `ScriptName`='spell_blazing_hippogryph';
DELETE FROM `spell_script_names` WHERE `spell_id`=48025 AND `ScriptName`='spell_headless_horseman_mount';
DELETE FROM `spell_script_names` WHERE `spell_id`=69366 AND `ScriptName`='spell_dru_moonkin_form_passive';
DELETE FROM `spell_script_names` WHERE `spell_id`=28414 AND `ScriptName`='spell_item_ashbringer';
DELETE FROM `spell_script_names` WHERE `spell_id`=20625 AND `ScriptName`='spell_gen_default_count_pct_from_max_hp';
DELETE FROM `spell_script_names` WHERE `spell_id`=20425 AND `ScriptName`='spell_pal_judgement_of_command';
DELETE FROM `spell_script_names` WHERE `spell_id`=13161 AND `ScriptName`='spell_hun_aspect_of_the_beast';
DELETE FROM `spell_script_names` WHERE `spell_id`=-6143 AND `ScriptName`='spell_mage_incanters_absorbtion_absorb' ;
DELETE FROM `spell_script_names` WHERE `spell_id`=-33851 AND `ScriptName`='spell_dru_primal_tenacity';
DELETE FROM `spell_script_names` WHERE `spell_id`=-49145 AND `ScriptName`='spell_dk_spell_deflection';
DELETE FROM `spell_script_names` WHERE `spell_id`=-49158 AND `ScriptName`='spell_dk_corpse_explosion';
DELETE FROM `spell_script_names` WHERE `spell_id`=47496 AND `ScriptName`='spell_dk_ghoul_explode';
DELETE FROM `spell_script_names` WHERE `spell_id`=-55428 AND `ScriptName`='spell_gen_lifeblood';
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mage_frost_warding_trigger';

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(66118, 'spell_gen_leeching_swarm'),
(67630, 'spell_gen_leeching_swarm'),
(68646, 'spell_gen_leeching_swarm'),
(68647, 'spell_gen_leeching_swarm'),
(24750, 'spell_gen_trick'),
(24751, 'spell_gen_trick_or_treat'),
(82636, 'spell_ignis_rising_flames');

REPLACE INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(10570, 11, 0, 0, 'achievement_he_feeds_on_your_tears'),
(10568, 11, 0, 0, 'achievement_he_feeds_on_your_tears'),
(15593, 11, 0, 0, 'achievement_headed_south');

INSERT INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES 
(6341, 'at_bt_entrance');

UPDATE `creature_template` SET `ScriptName`='npc_winter_reveler' WHERE `entry`=15760;
UPDATE `creature_template` SET `ScriptName`='npc_silverlaine_worgen' WHERE `entry`=50834 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='npc_silverlaine_worgen' WHERE `entry`=50851 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='npc_silverlaine_worgen' WHERE `entry`=50857 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='npc_silverlaine_worgen' WHERE `entry`=50869 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='npc_springvale_wailing_guardsman' WHERE `entry`=50613 LIMIT 1;
UPDATE `creature_template` SET `ScriptName`='npc_springvale_tormented_officer' WHERE `entry`=50615 LIMIT 1;
UPDATE `creature_template` SET `AIName`='', `ScriptName`='npc_apothecary_hummel' WHERE `entry`=36296;
UPDATE `creature_template` SET `ScriptName`='mob_crystal_spike' WHERE `entry`=27099;
UPDATE `creature_template` SET `ScriptName`='mob_crystalspawn_giant' WHERE `entry`=42810;
UPDATE `creature_template` SET `ScriptName`='mob_impp' WHERE `entry`=43014;
UPDATE `creature_template` SET `ScriptName`='mob_millhouse_manastorm' WHERE `entry`=43391;
UPDATE `creature_template` SET `ScriptName`='npc_ignacious_inferno_leap' WHERE `entry`=47040;
UPDATE `creature_template` SET `flags_extra`=128, `ScriptName`='npc_feludius_water_bomb ' WHERE `entry`=44201;
