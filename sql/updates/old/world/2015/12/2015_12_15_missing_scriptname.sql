UPDATE creature_template SET ScriptName="iron_docks_mob_hammer_stalker" WHERE entry=86534;
UPDATE creature_template SET ScriptName="iron_docks_nokgar_mob_flameslinger" WHERE entry=81279;
UPDATE creature_template SET ScriptName="iron_docks_skulloc_mob_turret" WHERE entry=84215;
UPDATE creature_template SET ScriptName="iron_docks_grimrail_mob_train" WHERE entry=83673;
UPDATE creature_template SET ScriptName="iron_docks_grimrail_mob_ogre_trap" WHERE entry=88758;
UPDATE creature_template SET ScriptName="iron_docks_grimrail_mob_lava_wave" WHERE entry=95353;
UPDATE creature_template SET ScriptName="iron_docks_oshir_mob_rylak_dest" WHERE entry=89021;
UPDATE creature_template SET ScriptName="iron_docks_oshir_mob_wolf_dest" WHERE entry=89022;
UPDATE creature_template SET ScriptName="auchindon_creature_sargerei_soulbinder" WHERE entry=77812;
UPDATE creature_template SET ScriptName="auchindon_creature_soul_priest" WHERE entry=76595;
UPDATE creature_template SET ScriptName="auchindon_nyami_bubble" WHERE entry=342652;
UPDATE gameobject_template SET ScriptName="gob_talador_portal" WHERE entry=236689;
UPDATE creature_template SET ScriptName="the_everbloom_Dreadpital_Toxin" WHERE entry=81864;
UPDATE creature_template SET ScriptName="the_everbloom_naturalist" WHERE entry=81819;
UPDATE creature_template SET ScriptName="the_everbloom_venom_sprayer" WHERE entry=86547;
UPDATE creature_template SET ScriptName="auchindon_creature_blazing_trickster" WHERE entry=79511;
UPDATE gameobject_template SET ScriptName="go_blackened_urn" WHERE entry=194092;
UPDATE creature_template SET ScriptName="npc_darkheart" WHERE entry=24246;
UPDATE creature_template SET ScriptName="npc_slither" WHERE entry=24242;
UPDATE creature_template SET ScriptName="npc_alyson_antille" WHERE entry=24240;
UPDATE creature_template SET ScriptName="npc_zulaman_forest_frog" WHERE entry=24396;
UPDATE creature_template SET ScriptName="npc_lurking_tempest" WHERE entry=45704;
UPDATE creature_template SET ScriptName="npc_chromie" WHERE entry=26527;
UPDATE creature_template SET ScriptName="npc_engineer_spark_overgrind" WHERE entry=17243;
UPDATE creature_template SET ScriptName="npc_stillpine_capitive" WHERE entry=17375;
UPDATE creature_template SET ScriptName="mob_fel_orc_convert" WHERE entry=17083;
UPDATE creature_template SET ScriptName="gatewatcher_gyro_kill" WHERE entry=19218;
UPDATE creature_template SET ScriptName="npc_argent_monk" WHERE entry=35305;
UPDATE creature_template SET ScriptName="npc_argent_lightwielder" WHERE entry=35309;
UPDATE creature_template SET ScriptName="npc_argent_priest" WHERE entry=35307;
UPDATE creature_template SET ScriptName="npc_hadronox_nerubian" WHERE entry=29064;
UPDATE creature_template SET ScriptName="npc_hadronox_nerubian" WHERE entry=29062;
UPDATE creature_template SET ScriptName="npc_verdisa_belgaristrasz_eternos" WHERE entry=28012;
UPDATE creature_template SET ScriptName="npc_ymirjar_deathbringer" WHERE entry=38125;
UPDATE creature_template SET ScriptName="npc_ymirjar_frostbinder" WHERE entry=37127;
UPDATE creature_template SET ScriptName="npc_ymirjar_battlemaiden" WHERE entry=37132;
UPDATE creature_template SET ScriptName="npc_ymirjar_huntress" WHERE entry=37134;
UPDATE creature_template SET ScriptName="npc_ymirjar_warlord" WHERE entry=37133;
UPDATE creature_template SET ScriptName="npc_muradin_gunship" WHERE entry=36948;
UPDATE creature_template SET ScriptName="npc_saurfang_gunship" WHERE entry=36939;
UPDATE creature_template SET ScriptName="npc_zafod_boombox" WHERE entry=37184;
UPDATE creature_template SET ScriptName="npc_korkron_axethrower_rifleman" WHERE entry=36968;
UPDATE creature_template SET ScriptName="npc_korkron_axethrower_rifleman" WHERE entry=36969;
UPDATE creature_template SET ScriptName="npc_mortar_soldier_or_rocketeer" WHERE entry=36978;
UPDATE creature_template SET ScriptName="npc_mortar_soldier_or_rocketeer" WHERE entry=36982;
UPDATE creature_template SET ScriptName="npc_gunship_mage" WHERE entry=37116;
UPDATE creature_template SET ScriptName="npc_gunship_mage" WHERE entry=37117;
UPDATE creature_template SET ScriptName="npc_gunship_cannon" WHERE entry=79511;
UPDATE creature_template SET ScriptName="npc_gunship_cannon" WHERE entry=79511;
UPDATE creature_template SET ScriptName="npc_sergeant" WHERE entry=36960;
UPDATE creature_template SET ScriptName="npc_sergeant" WHERE entry=36961;
UPDATE creature_template SET ScriptName="npc_marine_or_reaver" WHERE entry=36950;
UPDATE creature_template SET ScriptName="npc_marine_or_reaver" WHERE entry=36957;
UPDATE creature_template SET ScriptName="npc_gunship_skybreaker" WHERE entry=37540;
UPDATE creature_template SET ScriptName="npc_gunship_orgrimmar" WHERE entry=37215;
UPDATE creature_template SET ScriptName="npc_shadow_orb" WHERE entry=40083;
UPDATE creature_template SET ScriptName="npc_shadow_orb" WHERE entry=40100;
UPDATE creature_template SET ScriptName="npc_shadow_orb" WHERE entry=40468;
UPDATE creature_template SET ScriptName="npc_shadow_orb" WHERE entry=40469;
UPDATE creature_template SET ScriptName="boss_wase_mari" WHERE entry=56448;

UPDATE `instance_template` SET `parent` = '870', `script` = 'instance_siege_of_the_niuzao_temple' WHERE `map` = '1011';
UPDATE `instance_template` SET `parent` = '870', `script` = 'instance_stormstout_brewery' WHERE `map` = '961';


UPDATE gameobject_template SET ScriptName="go_bristlelimb_cage" WHERE entry=181714;
UPDATE gameobject_template SET ScriptName="go_ravager_cage" WHERE entry=181849;
UPDATE gameobject_template SET ScriptName="go_skull_pile" WHERE entry=185913;
UPDATE gameobject_template SET ScriptName="gob_talador_portal" WHERE entry=236689;
UPDATE gameobject_template SET ScriptName="gob_talador_portal" WHERE entry=236689;
UPDATE gameobject_template SET ScriptName="gob_talador_portal" WHERE entry=236689;
UPDATE gameobject_template SET ScriptName="gob_talador_portal" WHERE entry=236689;
UPDATE gameobject_template SET ScriptName="gob_talador_portal" WHERE entry=236689;

delete from spell_script_names where spell_id in (154356, 156921, 173052, 177731, 142423);
INSERT INTO spell_script_names VALUES 
(154356, "auchindon_spell_psychic_terror"),
(156921, "auchindon_teronogor_seed_of_malevolence"),
(173052, "the_everbloom_venom_spray"),
(177731, "the_everbloom_agitated_waters"),
(142423, "spell_special_swiftmend");