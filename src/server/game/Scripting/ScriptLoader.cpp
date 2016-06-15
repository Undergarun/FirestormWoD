////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "AnticheatMgr.h"
#include "ScriptLoader.h"

/// Customs
void AddSC_event_april();
void AddSC_promotion_new_players();
void AddSC_first_time_connexion();
void AddSC_user_reporting();
void AddSC_warning_update_client();
void AddSC_DuelReset();
void AddSC_npc_custom();
void AddSC_Webshop_Delivery();
void AddSC_Character_Renderer();
void AddSC_StressTest();

/// Spells
void AddSC_deathknight_spell_scripts();
void AddSC_druid_spell_scripts();
void AddSC_generic_spell_scripts();
void AddSC_hunter_spell_scripts();
void AddSC_mage_spell_scripts();
void AddSC_paladin_spell_scripts();
void AddSC_priest_spell_scripts();
void AddSC_rogue_spell_scripts();
void AddSC_shaman_spell_scripts();
void AddSC_warlock_spell_scripts();
void AddSC_warrior_spell_scripts();
void AddSC_monk_spell_scripts();
void AddSC_mastery_spell_scripts();
void AddSC_quest_spell_scripts();
void AddSC_item_spell_scripts();
void AddSC_example_spell_scripts();
void AddSC_holiday_spell_scripts();
void AddSC_spell_skill();
void AddSC_npc_spell_scripts();
void AddSC_areatrigger_spell_scripts();
void AddSC_spell_item_enchantment();
void AddSC_spell_toy();
void AddSC_npc_toy();
void AddSC_go_toy();
void AddSC_SmartSCripts();
void AddSC_petbattle_abilities();
void AddSC_npc_PetBattleTrainer();
void AddSC_PetBattlePlayerScript();

/// Commands
void AddSC_anticheat_commandscript();
void AddSC_account_commandscript();
void AddSC_achievement_commandscript();
void AddSC_ban_commandscript();
void AddSC_bf_commandscript();
void AddSC_cast_commandscript();
void AddSC_character_commandscript();
void AddSC_cheat_commandscript();
void AddSC_debug_commandscript();
void AddSC_disable_commandscript();
void AddSC_event_commandscript();
void AddSC_gm_commandscript();
void AddSC_go_commandscript();
void AddSC_at_commandscript();
void AddSC_conversation_commandscript();
void AddSC_gobject_commandscript();
void AddSC_guild_commandscript();
void AddSC_honor_commandscript();
void AddSC_instance_commandscript();
void AddSC_learn_commandscript();
void AddSC_list_commandscript();
void AddSC_lookup_commandscript();
void AddSC_message_commandscript();
void AddSC_misc_commandscript();
void AddSC_modify_commandscript();
void AddSC_npc_commandscript();
void AddSC_quest_commandscript();
void AddSC_reload_commandscript();
void AddSC_reset_commandscript();
void AddSC_server_commandscript();
void AddSC_tele_commandscript();
void AddSC_ticket_commandscript();
void AddSC_titles_commandscript();
void AddSC_wp_commandscript();
void AddSC_garrison_commandscript();
void AddSC_hotfix_commandscript();
void AddSC_battlepay_commandscript();
void AddSC_items_commandscript();
void AddSC_spellog_commandscript();
void AddSC_mmaps_commandscript();

/// World
void AddSC_areatrigger_scripts();
void AddSC_generic_creature();
void AddSC_go_scripts();
void AddSC_guards();
void AddSC_item_scripts();
void AddSC_npc_professions();
void AddSC_npc_innkeeper();
void AddSC_npcs_special();
void AddSC_npc_taxi();
void AddSC_npc_companions();
void AddSC_achievement_scripts();
void AddSC_EncounterScripts();
void AddSC_WeeklyEventBonusScripts();

/// Player
void AddSC_chat_log();

#ifdef SCRIPTS
/// Eastern kingdoms
/// Alterac Valley
void AddSC_alterac_valley();
void AddSC_boss_balinda();
void AddSC_boss_drekthar();
void AddSC_boss_galvangar();
void AddSC_boss_vanndar();

/// Blackrock Depths
void AddSC_blackrock_depths();
void AddSC_boss_ambassador_flamelash();
void AddSC_boss_anubshiah();
void AddSC_boss_draganthaurissan();
void AddSC_boss_general_angerforge();
void AddSC_boss_gorosh_the_dervish();
void AddSC_boss_grizzle();
void AddSC_boss_high_interrogator_gerstahn();
void AddSC_boss_magmus();
void AddSC_boss_moira_bronzebeard();
void AddSC_boss_tomb_of_seven();
void AddSC_instance_blackrock_depths();

/// Blackwing lair
void AddSC_boss_razorgore();
void AddSC_boss_vael();
void AddSC_boss_broodlord();
void AddSC_boss_firemaw();
void AddSC_boss_ebonroc();
void AddSC_boss_flamegor();
void AddSC_boss_chromaggus();
void AddSC_boss_nefarian();
void AddSC_boss_victor_nefarius();

/// Deadmines
void AddSC_instance_deadmines();
void AddSC_deadmines();
void AddSC_boss_glubtok();
void AddSC_boss_helix_gearbreaker();
void AddSC_boss_foereaper5000();
void AddSC_boss_admiral_ripsnarl();
void AddSC_boss_captain_cookie();
void AddSC_boss_vanessa_vancleef();

/// Gnomeregan
void AddSC_gnomeregan();
void AddSC_instance_gnomeregan();

///Karazhan
void AddSC_boss_attumen();
void AddSC_boss_curator();
void AddSC_boss_maiden_of_virtue();
void AddSC_boss_shade_of_aran();
void AddSC_boss_malchezaar();
void AddSC_boss_terestian_illhoof();
void AddSC_boss_moroes();
void AddSC_bosses_opera();
void AddSC_boss_netherspite();
void AddSC_instance_karazhan();
void AddSC_karazhan();
void AddSC_boss_nightbane();

/// Magister's Terrace
void AddSC_boss_felblood_kaelthas();
void AddSC_boss_selin_fireheart();
void AddSC_boss_vexallus();
void AddSC_boss_priestess_delrissa();
void AddSC_instance_magisters_terrace();
void AddSC_magisters_terrace();

///Molten core
void AddSC_boss_lucifron();
void AddSC_boss_magmadar();
void AddSC_boss_gehennas();
void AddSC_boss_garr();
void AddSC_boss_baron_geddon();
void AddSC_boss_shazzrah();
void AddSC_boss_golemagg();
void AddSC_boss_sulfuron();
void AddSC_boss_majordomo();
void AddSC_boss_ragnaros();
void AddSC_instance_molten_core();

/// Scarlet Enclave
void AddSC_the_scarlet_enclave();
void AddSC_the_scarlet_enclave_c1();
void AddSC_the_scarlet_enclave_c2();
void AddSC_the_scarlet_enclave_c5();

/// Scarlet Monastery
void AddSC_boss_arcanist_doan();
void AddSC_boss_azshir_the_sleepless();
void AddSC_boss_bloodmage_thalnos();
void AddSC_boss_headless_horseman();
void AddSC_boss_herod();
void AddSC_boss_high_inquisitor_fairbanks();
void AddSC_boss_houndmaster_loksey();
void AddSC_boss_interrogator_vishas();
void AddSC_boss_scorn();
void AddSC_instance_scarlet_monastery();
void AddSC_boss_mograine_and_whitemane();

/// Scholomance
void AddSC_boss_darkmaster_gandling();
void AddSC_boss_death_knight_darkreaver();
void AddSC_boss_theolenkrastinov();
void AddSC_boss_illuciabarov();
void AddSC_boss_instructormalicia();
void AddSC_boss_jandicebarov();
void AddSC_boss_kormok();
void AddSC_boss_lordalexeibarov();
void AddSC_boss_lorekeeperpolkelt();
void AddSC_boss_rasfrost();
void AddSC_boss_theravenian();
void AddSC_boss_vectus();
void AddSC_instance_scholomance();

/// Shadowfang keep
void AddSC_boss_baron_ashbury();
void AddSC_boss_baron_silverlaine();
void AddSC_boss_commander_springvale();
void AddSC_boss_lord_valden();
void AddSC_boss_lord_godfrey();
void AddSC_shadowfang_keep();
void AddSC_instance_shadowfang_keep();

/// Stratholme
void AddSC_boss_magistrate_barthilas();
void AddSC_boss_maleki_the_pallid();
void AddSC_boss_nerubenkan();
void AddSC_boss_cannon_master_willey();
void AddSC_boss_baroness_anastari();
void AddSC_boss_ramstein_the_gorger();
void AddSC_boss_timmy_the_cruel();
void AddSC_boss_postmaster_malown();
void AddSC_boss_baron_rivendare();
void AddSC_boss_dathrohan_balnazzar();
void AddSC_boss_order_of_silver_hand();
void AddSC_instance_stratholme();
void AddSC_stratholme();

/// Sunken Temple
void AddSC_sunken_temple();
void AddSC_instance_sunken_temple();

/// Sunwell Plateau
void AddSC_instance_sunwell_plateau();
void AddSC_boss_kalecgos();
void AddSC_boss_brutallus();
void AddSC_boss_felmyst();
void AddSC_boss_eredar_twins();
void AddSC_boss_muru();
void AddSC_boss_kiljaeden();
void AddSC_sunwell_plateau();

/// Uldaman
void AddSC_boss_archaedas();
void AddSC_boss_ironaya();
void AddSC_uldaman();
void AddSC_instance_uldaman();

/// Zul'Aman
void AddSC_boss_akilzon();
void AddSC_boss_halazzi();
void AddSC_boss_hex_lord_malacrass();
void AddSC_boss_janalai();
void AddSC_boss_nalorakk();
void AddSC_boss_daakara();
void AddSC_instance_zulaman();
void AddSC_zulaman();

/// Zul'Gurub
void AddSC_boss_grilek();
void AddSC_boss_hazzarah();
void AddSC_boss_jindo_the_godbreaker();
void AddSC_boss_kilnara();
void AddSC_boss_mandokir();
void AddSC_boss_renataki();
void AddSC_boss_venoxis();
void AddSC_boss_wushoolay();
void AddSC_boss_zanzil();
void AddSC_instance_zulgurub();

/// Zones
void AddSC_arathi_highlands();
void AddSC_blasted_lands();
void AddSC_burning_steppes();
void AddSC_duskwood();
void AddSC_eastern_plaguelands();
void AddSC_eversong_woods();
void AddSC_elwyn_forest();
void AddSC_dun_morogh();
void AddSC_ghostlands();
void AddSC_hinterlands();
void AddSC_ironforge();
void AddSC_isle_of_queldanas();
void AddSC_loch_modan();
void AddSC_redridge_mountains();
void AddSC_silvermoon_city();
void AddSC_silverpine_forest();
void AddSC_stormwind_city();
void AddSC_stranglethorn_vale();
void AddSC_swamp_of_sorrows();
void AddSC_tirisfal_glades();
void AddSC_undercity();
void AddSC_western_plaguelands();
void AddSC_westfall();
void AddSC_wetlands();
void AddSC_gilneas();
void AddSC_ashenvale();
void AddSC_azshara();
void AddSC_azuremyst_isle();
void AddSC_bloodmyst_isle();
void AddSC_darkshore();
void AddSC_desolace();
void AddSC_durotar();
void AddSC_dustwallow_marsh();
void AddSC_felwood();
void AddSC_feralas();
void AddSC_moonglade();
void AddSC_mulgore();
void AddSC_orgrimmar();
void AddSC_silithus();
void AddSC_southern_barrens();
void AddSC_stonetalon_mountains();
void AddSC_tanaris();
void AddSC_teldrassil();
void AddSC_the_barrens();
void AddSC_thousand_needles();
void AddSC_thunder_bluff();
void AddSC_uldum();
void AddSC_ungoro_crater();
void AddSC_winterspring();

/// Upper Blackrock Spire
void AddSC_upper_blackrock_spire();
void AddSC_instance_upper_blackrock_spire();
void AddSC_boss_orebender_gorashan();
void AddSC_boss_kyrak_the_corruptor();
void AddSC_boss_commander_tharbek();
void AddSC_boss_ragewing_the_untamed();
void AddSC_boss_warlord_zaela();

/// Kalimdor

/// Ragefire
void AddSC_instance_ragefire_chasm();
void AddSC_ragefire_chasm();
void AddSC_boss_lava_guard_gordoth();

/// Blackfathom Depths
void AddSC_blackfathom_deeps();
void AddSC_boss_gelihast();
void AddSC_boss_kelris();
void AddSC_boss_aku_mai();
void AddSC_instance_blackfathom_deeps();

/// CoT Battle for Mt. Hyjal
void AddSC_hyjal();
void AddSC_boss_archimonde();
void AddSC_instance_mount_hyjal();
void AddSC_hyjal_trash();
void AddSC_boss_rage_winterchill();
void AddSC_boss_anetheron();
void AddSC_boss_kazrogal();
void AddSC_boss_azgalor();

/// CoT Old Hillsbrad
void AddSC_boss_captain_skarloc();
void AddSC_boss_epoch_hunter();
void AddSC_boss_lieutenant_drake();
void AddSC_instance_old_hillsbrad();
void AddSC_old_hillsbrad();

/// CoT The Dark Portal
void AddSC_boss_aeonus();
void AddSC_boss_chrono_lord_deja();
void AddSC_boss_temporus();
void AddSC_dark_portal();
void AddSC_instance_dark_portal();

/// CoT Culling Of Stratholme
void AddSC_boss_epoch();
void AddSC_boss_infinite_corruptor();
void AddSC_boss_salramm();
void AddSC_boss_mal_ganis();
void AddSC_boss_meathook();
void AddSC_culling_of_stratholme();
void AddSC_instance_culling_of_stratholme();

/// Maraudon
void AddSC_boss_celebras_the_cursed();
void AddSC_boss_landslide();
void AddSC_boss_noxxion();
void AddSC_boss_ptheradras();

/// Onyxia's Lair
void AddSC_boss_onyxia();
void AddSC_instance_onyxias_lair();

/// Razorfen Downs
void AddSC_boss_amnennar_the_coldbringer();
void AddSC_razorfen_downs();
void AddSC_instance_razorfen_downs();

/// Razorfen Kraul
void AddSC_razorfen_kraul();
void AddSC_instance_razorfen_kraul();

/// Ruins of ahn'qiraj
void AddSC_boss_kurinnaxx();
void AddSC_boss_rajaxx();
void AddSC_boss_moam();
void AddSC_boss_buru();
void AddSC_boss_ayamiss();
void AddSC_boss_ossirian();
void AddSC_instance_ruins_of_ahnqiraj();

/// Temple of ahn'qiraj
void AddSC_boss_cthun();
void AddSC_boss_fankriss();
void AddSC_boss_huhuran();
void AddSC_bug_trio();
void AddSC_boss_sartura();
void AddSC_boss_skeram();
void AddSC_boss_twinemperors();
void AddSC_boss_ouro();
void AddSC_mob_anubisath_sentinel();
void AddSC_instance_temple_of_ahnqiraj();

/// Wailing caverns
void AddSC_wailing_caverns();
void AddSC_instance_wailing_caverns();
/// Zul'Farrak generic
void AddSC_zulfarrak();

/// Zul'Farrak instance script
void AddSC_instance_zulfarrak();

/// Outland
/// Auchindoun Auchenai Crypts
void AddSC_boss_exarch_maladaar();
void AddSC_boss_shirrak_the_dead_watcher();

/// Auchindoun Mana Tombs
void AddSC_boss_nexusprince_shaffar();
void AddSC_boss_pandemonius();

/// Auchindoun Sekketh Halls
void AddSC_boss_darkweaver_syth();
void AddSC_boss_talon_king_ikiss();
void AddSC_instance_sethekk_halls();

/// Auchindoun Shadow Labyrinth
void AddSC_instance_shadow_labyrinth();
void AddSC_boss_ambassador_hellmaw();
void AddSC_boss_blackheart_the_inciter();
void AddSC_boss_grandmaster_vorpil();
void AddSC_boss_murmur();

/// Black Temple
void AddSC_black_temple();
void AddSC_boss_illidan();
void AddSC_boss_shade_of_akama();
void AddSC_boss_supremus();
void AddSC_boss_gurtogg_bloodboil();
void AddSC_boss_mother_shahraz();
void AddSC_boss_reliquary_of_souls();
void AddSC_boss_teron_gorefiend();
void AddSC_boss_najentus();
void AddSC_boss_illidari_council();
void AddSC_instance_black_temple();

/// CR Serpent Shrine Cavern
void AddSC_boss_fathomlord_karathress();
void AddSC_boss_hydross_the_unstable();
void AddSC_boss_lady_vashj();
void AddSC_boss_leotheras_the_blind();
void AddSC_boss_morogrim_tidewalker();
void AddSC_instance_serpentshrine_cavern();
void AddSC_boss_the_lurker_below();

/// CR Steam Vault
void AddSC_boss_hydromancer_thespia();
void AddSC_boss_mekgineer_steamrigger();
void AddSC_boss_warlord_kalithresh();
void AddSC_instance_steam_vault();

/// CR Underbog
void AddSC_boss_hungarfen();
void AddSC_boss_the_black_stalker();

/// Gruul's Lair
void AddSC_boss_gruul();
void AddSC_boss_high_king_maulgar();
void AddSC_instance_gruuls_lair();

/// HC Blood Furnace
void AddSC_boss_broggok();
void AddSC_boss_kelidan_the_breaker();
void AddSC_boss_the_maker();
void AddSC_instance_blood_furnace();

/// HC Magtheridon's Lair
void AddSC_boss_magtheridon();
void AddSC_instance_magtheridons_lair();

/// HC Shattered Halls
void AddSC_boss_grand_warlock_nethekurse();
void AddSC_boss_warbringer_omrogg();
void AddSC_boss_warchief_kargath_bladefist();
void AddSC_instance_shattered_halls();

/// HC Ramparts
void AddSC_boss_watchkeeper_gargolmar();
void AddSC_boss_omor_the_unscarred();
void AddSC_boss_vazruden_the_herald();
void AddSC_instance_ramparts();

/// TK Arcatraz
void AddSC_arcatraz();
void AddSC_boss_harbinger_skyriss();
void AddSC_instance_arcatraz();

/// TK Botanica
void AddSC_boss_high_botanist_freywinn();
void AddSC_boss_laj();
void AddSC_boss_warp_splinter();

/// TK The Eye
void AddSC_boss_alar();
void AddSC_boss_kaelthas();
void AddSC_boss_void_reaver();
void AddSC_boss_high_astromancer_solarian();
void AddSC_instance_the_eye();
void AddSC_the_eye();

/// TK The Mechanar
void AddSC_boss_gatewatcher_iron_hand();
void AddSC_boss_nethermancer_sepethrea();
void AddSC_boss_pathaleon_the_calculator();
void AddSC_boss_mechano_lord_capacitus();
void AddSC_instance_mechanar();
void AddSC_mechanar();

/// Zone
void AddSC_blades_edge_mountains();
void AddSC_boss_doomlordkazzak();
void AddSC_boss_doomwalker();
void AddSC_hellfire_peninsula();
void AddSC_nagrand();
void AddSC_netherstorm();
void AddSC_shadowmoon_valley();
void AddSC_shattrath_city();
void AddSC_terokkar_forest();
void AddSC_zangarmarsh();

/// Northrend
/// Gundrak
void AddSC_boss_slad_ran();
void AddSC_boss_moorabi();
void AddSC_boss_drakkari_colossus();
void AddSC_boss_gal_darah();
void AddSC_boss_eck();
void AddSC_instance_gundrak();

/// Azjol-Nerub
void AddSC_boss_krik_thir();
void AddSC_boss_hadronox();
void AddSC_boss_anub_arak();
void AddSC_instance_azjol_nerub();
void AddSC_instance_ahnkahet();

/// Azjol-Nerub Ahn'kahet
void AddSC_boss_amanitar();
void AddSC_boss_taldaram();
void AddSC_boss_jedoga_shadowseeker();
void AddSC_boss_elder_nadox();
void AddSC_boss_volazj();

/// Trial of the Champion
void AddSC_boss_argent_challenge();
void AddSC_boss_black_knight();
void AddSC_boss_grand_champions();
void AddSC_instance_trial_of_the_champion();
void AddSC_trial_of_the_champion();

/// Trial of the Crusader
void AddSC_boss_anubarak_trial();
void AddSC_boss_faction_champions();
void AddSC_boss_jaraxxus();
void AddSC_boss_northrend_beasts();
void AddSC_boss_twin_valkyr();
void AddSC_trial_of_the_crusader();
void AddSC_instance_trial_of_the_crusader();

/// Naxxramas
void AddSC_boss_anubrekhan();
void AddSC_boss_maexxna();
void AddSC_boss_patchwerk();
void AddSC_boss_grobbulus();
void AddSC_boss_razuvious();
void AddSC_boss_kelthuzad();
void AddSC_boss_loatheb();
void AddSC_boss_noth();
void AddSC_boss_gluth();
void AddSC_boss_sapphiron();
void AddSC_boss_four_horsemen();
void AddSC_boss_faerlina();
void AddSC_boss_heigan();
void AddSC_boss_gothik();
void AddSC_boss_thaddius();
void AddSC_instance_naxxramas();

/// The Nexus Nexus
void AddSC_boss_magus_telestra();
void AddSC_boss_anomalus();
void AddSC_boss_ormorok();
void AddSC_boss_keristrasza();
void AddSC_instance_nexus();

/// The Nexus The Oculus
void AddSC_boss_drakos();
void AddSC_boss_urom();
void AddSC_boss_varos();
void AddSC_boss_eregos();
void AddSC_instance_oculus();
void AddSC_oculus();

/// The Nexus: Eye of Eternity
void AddSC_boss_malygos();
void AddSC_instance_eye_of_eternity();

/// Obsidian Sanctum
void AddSC_boss_sartharion();
void AddSC_instance_obsidian_sanctum();

/// Ulduar Halls of Lightning
void AddSC_boss_bjarngrim();
void AddSC_boss_loken();
void AddSC_boss_ionar();
void AddSC_boss_volkhan();
void AddSC_instance_halls_of_lightning();

/// Ulduar Halls of Stone
void AddSC_boss_maiden_of_grief();
void AddSC_boss_krystallus();
void AddSC_boss_sjonnir();
void AddSC_instance_halls_of_stone();
void AddSC_halls_of_stone();

/// Ulduar
void AddSC_instance_ulduar();
void AddSC_ulduar_teleporter();
void AddSC_ulduar_scripts();
void AddSC_boss_flame_leviathan();
void AddSC_boss_ignis();
void AddSC_boss_razorscale();
void AddSC_boss_xt002();
void AddSC_boss_assembly_of_iron();
void AddSC_boss_kologarn();
void AddSC_boss_auriaya();
void AddSC_boss_hodir();
void AddSC_boss_freya();
void AddSC_boss_mimiron();
void AddSC_boss_thorim();
void AddSC_boss_general_vezax();
void AddSC_boss_algalon_the_observer();

/// Utgarde Keep
void AddSC_boss_keleseth();
void AddSC_boss_skarvald_dalronn();
void AddSC_boss_ingvar_the_plunderer();
void AddSC_instance_utgarde_keep();

/// Utgarde pinnacle
void AddSC_boss_svala();
void AddSC_boss_palehoof();
void AddSC_boss_skadi();
void AddSC_boss_ymiron();
void AddSC_instance_utgarde_pinnacle();
void AddSC_utgarde_keep();

/// Vault of Archavon
void AddSC_boss_archavon();
void AddSC_boss_emalon();
void AddSC_boss_koralon();
void AddSC_boss_toravon();
void AddSC_instance_archavon();

/// Drak'Tharon Keep
void AddSC_boss_trollgore();
void AddSC_boss_novos();
void AddSC_boss_dred();
void AddSC_boss_tharon_ja();
void AddSC_instance_drak_tharon();

/// Violet Hold
void AddSC_boss_cyanigosa();
void AddSC_boss_erekem();
void AddSC_boss_ichoron();
void AddSC_boss_lavanthor();
void AddSC_boss_moragg();
void AddSC_boss_xevozz();
void AddSC_boss_zuramat();
void AddSC_instance_violet_hold();
void AddSC_violet_hold();

/// Forge of Souls
void AddSC_instance_forge_of_souls();
void AddSC_forge_of_souls();
void AddSC_boss_bronjahm();
void AddSC_boss_devourer_of_souls();

/// Pit of Saron
void AddSC_instance_pit_of_saron();
void AddSC_pit_of_saron();
void AddSC_boss_garfrost();
void AddSC_boss_ick();
void AddSC_boss_tyrannus();

/// Halls of Reflection
void AddSC_instance_halls_of_reflection();
void AddSC_halls_of_reflection();
void AddSC_boss_falric();
void AddSC_boss_marwyn();
void AddSC_boss_lich_king_hr();

/// Icecrown Citadel
void AddSC_instance_icecrown_citadel();
void AddSC_icecrown_citadel();
void AddSC_icecrown_citadel_teleport();
void AddSC_boss_lord_marrowgar();
void AddSC_boss_lady_deathwhisper();
void AddSC_boss_gunship_battle();
void AddSC_boss_deathbringer_saurfang();
void AddSC_boss_festergut();
void AddSC_boss_rotface();
void AddSC_boss_professor_putricide();
void AddSC_boss_blood_prince_council();
void AddSC_boss_blood_queen_lana_thel();
void AddSC_boss_valithria_dreamwalker();
void AddSC_boss_sindragosa();
void AddSC_boss_the_lich_king();

/// Ruby Sanctum
void AddSC_instance_ruby_sanctum();
void AddSC_ruby_sanctum();
void AddSC_boss_baltharus_the_warborn();
void AddSC_boss_saviana_ragefire();
void AddSC_boss_general_zarithrian();
void AddSC_boss_halion();

/// Zones
void AddSC_dalaran();
void AddSC_borean_tundra();
void AddSC_dragonblight();
void AddSC_grizzly_hills();
void AddSC_howling_fjord();
void AddSC_icecrown();
void AddSC_sholazar_basin();
void AddSC_storm_peaks();
void AddSC_wintergrasp();
void AddSC_zuldrak();
void AddSC_crystalsong_forest();
void AddSC_isle_of_conquest();

/// Cataclysm
/// Throne of the Tides
void AddSC_instance_throne_of_the_tides();
void AddSC_throne_of_the_tides();
void AddSC_boss_lady_nazjar();
void AddSC_boss_commander_ulthok();
void AddSC_boss_erunak_stonespeaker();
void AddSC_boss_ozumat();

/// Blackrock Caverns
void AddSC_instance_blackrock_caverns();
void AddSC_boss_romogg_bonecrusher();
void AddSC_boss_corla_herald_of_twilight();
void AddSC_boss_karsh_steelbender();
void AddSC_boss_beauty();
void AddSC_boss_ascendant_lord_obsidius();

/// The Vortex Pinnacle
void AddSC_instance_the_vortex_pinnacle();
void AddSC_the_vortex_pinnacle();
void AddSC_boss_grand_vizier_ertan();
void AddSC_boss_altairus();
void AddSC_boss_asaad();

/// Baradin Hold
void AddSC_instance_baradin_hold();
void AddSC_boss_argaloth();
void AddSC_boss_occuthar();
void AddSC_boss_alizabal();

/// The Stonecore
void AddSC_instance_the_stonecore();
void AddSC_the_stonecore();
void AddSC_boss_corborus();
void AddSC_boss_slabhide();
void AddSC_boss_ozruk();
void AddSC_boss_high_priestess_azil();

/// Grim Batol
void AddSC_instance_grim_batol();
void AddSC_grim_batol();
void AddSC_boss_general_umbriss();
void AddSC_boss_forgemaster_throngus();
void AddSC_boss_drahga_shadowburner();
void AddSC_boss_erudax();

/// Lost City of the Tol'Vir
void AddSC_instance_lost_city_of_the_tolvir();
void AddSC_lost_city_of_the_tolvir();
void AddSC_boss_general_husam();
void AddSC_boss_lockmaw_augh();
void AddSC_boss_high_prophet_barim();
void AddSC_boss_siamat();

/// Halls of Origination
void AddSC_instance_halls_of_origination();
void AddSC_halls_of_origination();
void AddSC_boss_temple_guardian_anhuur();
void AddSC_boss_earthrager_ptah();
void AddSC_boss_anraphet();
void AddSC_boss_ammunae();
void AddSC_boss_isiset();
void AddSC_boss_setesh();
void AddSC_boss_rajh();

/// Bastion of Twilight
void AddSC_instance_bastion_of_twilight();
void AddSC_bastion_of_twilight();
void AddSC_boss_halfus_wyrmbreaker();
void AddSC_boss_theralion_and_valiona();
void AddSC_boss_ascendant_council();
void AddSC_boss_chogall();
void AddSC_boss_sinestra();

/// End Time
void AddSC_instance_end_time();
void AddSC_end_time();
void AddSC_end_time_teleport();
void AddSC_boss_echo_of_tyrande();
void AddSC_boss_echo_of_sylvanas();
void AddSC_boss_echo_of_baine();
void AddSC_boss_echo_of_jaina();
void AddSC_boss_murozond();

/// Well of Eternity
void AddSC_instance_well_of_eternity();
void AddSC_well_of_eternity();
void AddSC_well_of_eternity_teleport();
void AddSC_boss_perotharn();
void AddSC_boss_queen_azshara();
void AddSC_boss_mannoroth();

/// Hour of Twilight
void AddSC_instance_hour_of_twilight();
void AddSC_hour_of_twilight();
void AddSC_boss_arcurion();
void AddSC_boss_asira_dawnslayer();
void AddSC_boss_archbishop_benedictus();

/// Pandaria
/// Temple of Jade Serpent
void AddSC_instance_temple_of_jade_serpent();
void AddSC_boss_wise_mari();
void AddSC_boss_lorewalker_stonestep();
void AddSC_boss_liu_flameheat();
void AddSC_boss_sha_of_doubt();

/// The Stormstout Brewery
void AddSC_instance_stormstout_brewery();
void AddSC_stormstout_brewery();
void AddSC_boss_ook_ook();

/// Gate of the Setting Sun
void AddSC_instance_gate_setting_sun();
void AddSC_gate_setting_sun();
void AddSC_boss_saboteur_kiptilak();
void AddSC_boss_striker_gadok();
void AddSC_boss_commander_rimok();
void AddSC_boss_raigonn();
void AddSC_boss_sha_of_anger();
void AddSC_boss_galion();
void AddSC_boss_oondasta();
void AddSC_boss_nalak();
//void AddSC_boss_xuen();
//void AddSC_boss_yu_lon();
//void AddSC_boss_chi_ji();
//void AddSC_boss_niuzao();
//void AddSC_boss_ordos();

/// Mogu'Shan Palace
void AddSC_instance_mogu_shan_palace();
void AddSC_boss_trial_of_the_king();
void AddSC_boss_gekkan();
void AddSC_boss_xin_the_weaponmaster();

/// Shadopan Monastery
void AddSC_instance_shadopan_monastery();
void AddSC_shadopan_monastery();
void AddSC_boss_gu_cloudstrike();
void AddSC_boss_master_snowdrift();
void AddSC_boss_sha_of_violence();
void AddSC_boss_taran_zhu();

/// Siege of the niuzao Temple
void AddSC_instance_siege_of_the_niuzao_temple();
void AddSC_siege_of_the_niuzao_temple();
void AddSC_boss_jinbak();

/// Mogu'Shan Vault
void AddSC_instance_mogu_shan_vault();
void AddSC_mogu_shan_vault();
void AddSC_boss_stone_guard();
void AddSC_boss_feng();
void AddSC_boss_garajal();
void AddSC_boss_spirit_kings();
void AddSC_boss_elegon();
void AddSC_boss_will_of_emperor();

/// Heart of Fear
void AddSC_instance_heart_of_fear();
void AddSC_heart_of_fear();
void AddSC_boss_zorlok();
void AddSC_boss_tayak();
void AddSC_boss_garalon();
void AddSC_boss_meljarak();
void AddSC_boss_unsok();
void AddSC_boss_shekzeer();

/// Terrace of Endless Spring
void AddSC_instance_terrace_of_endless_spring();
void AddSC_terrace_of_endless_spring();
void AddSC_boss_protectors_of_the_endless();
void AddSC_boss_tsulong();
void AddSC_boss_lei_shi();
void AddSC_boss_sha_of_fear();

/// Throne of Thunder
void AddSC_instance_throne_of_thunder();
void AddSC_throne_of_thunder();
void AddSC_boss_jin_rokh_the_breaker();
void AddSC_boss_council_of_elders();
void AddSC_boss_durumu();
void AddSC_boss_horridon();
void AddSC_boss_tortos();
void AddSC_boss_megaera();
void AddSC_boss_ji_kun();
void AddSC_boss_primordius();
void AddSC_boss_dark_animus();
void AddSC_boss_iron_qon();
void AddSC_boss_twin_consorts();
void AddSC_boss_lei_shen();

/// Siege of Orgrimmar
//void AddSC_instance_siege_of_orgrimmar();
//void AddSC_siege_of_orgrimmar();
//void AddSC_immerseus();
//void AddSC_fallen_protectors();
//void AddSC_norushen();
//void AddSC_sha_of_pride();
//void AddSC_galakras();
//void AddSC_iron_juggernaut();
//void AddSC_korkron_dark_shamans();
//void AddSC_general_nazgrim();
//void AddSC_malkorok();
//void AddSC_spoils_of_pandaria();
//void AddSC_thok_the_bloodthirsty();
//void AddSC_siegecrafter_blackfuse();
//void AddSC_paragons_of_the_klaxxi();
//void AddSC_Garrosh_Hellscream();

/// Zone
void AddSC_townlong_steppes();
void AddSC_dread_wastes();
void AddSC_valley_of_the_four_winds();
void AddSC_krasarang_wilds();
void AddSC_jade_forest();
void AddSC_the_veiled_stair();
void AddSC_vale_of_eternal_blossoms();
void AddSC_kun_lai_summit();
void AddSC_timeless_isle();

/// Wandering Island
void AddSC_WanderingIsland_North();
void AddSC_WanderingIsland_East();
void AddSC_WanderingIsland_West();
void AddSC_WanderingIsland_South();

/// Draenor
void AddSC_tanaan_jungle();
void AddSC_spires_of_arak();
void AddSC_frostfire_ridge();
void AddSC_draenor_shadowmoon_valley();
void AddSC_gorgrond();
void AddSC_talador();
void AddSC_Garrison_GO();
void AddSC_Garrison_NPC();
void AddSC_Garrison_Quest();
void AddSC_Garrison_PlayerConditions();
void AddSC_Garrison_A1();
void AddSC_Garrison_A2();
void AddSC_Garrison_A3();
void AddSC_Garrison_H1();
void AddSC_Garrison_H2();
void AddSC_Garrison_H3();
void AddSC_Garrison_AS();
void AddSC_Garrison_HS();
void AddSC_Garrison();
void AddSC_item_strongboxes();

/// Draenor Scenario
void AddSC_instance_scenario_fate_of_karabor();

/// Skyreach Dungeon
void AddSC_instance_Skyreach();
void AddSC_mob_instance_skyreach();
void AddSC_spell_instance_skyreach();
void AddSC_boss_ranjit();
void AddSC_boss_Araknath();
void AddSC_boss_Rukhran();
void AddSC_boss_HighSaveViryx();

/// Bloodmaul Slag Mines
void AddSC_instance_Bloodmaul();
void AddSC_mob_Bloodmaul();
void AddSC_spell_Bloodmaul();
void AddSC_areatrigger_Bloodmaul();
void AddSC_boss_SlaveWatcherCrushto();
void AddSC_boss_forgemaster_gogduh();
void AddSC_boss_roltall();
void AddSC_boss_gugrokk();

/// Highmaul
void AddSC_instance_highmaul();
void AddSC_highmaul();
void AddSC_boss_kargath_bladefist();
void AddSC_boss_the_butcher();
void AddSC_boss_brackenspore();
void AddSC_boss_tectus();
void AddSC_boss_twin_ogron();
void AddSC_boss_koragh();
void AddSC_boss_imperator_margok();

/// Blackrock Foundry
void AddSC_instance_blackrock_foundry();
void AddSC_blackrock_foundry();
void AddSC_boss_gruul_foundry();
void AddSC_boss_oregorger();
void AddSC_boss_blast_furnace();
void AddSC_boss_hansgar_and_franzok();
void AddSC_boss_flamebender_kagraz();
void AddSC_boss_kromog();
void AddSC_boss_beastlord_darmac();
void AddSC_boss_operator_thogar();
void AddSC_boss_iron_maidens();
void AddSC_boss_blackhand();

/* Iron Docks Dungeon */
void AddSC_iron_docks();
void AddSC_instance_iron_docks();
void AddSC_boss_nokgar();
void AddSC_boss_skulloc();
void AddSC_boss_grimrail();
void AddSC_boss_oshir();

/* Shadowmoon Burial Grounds Dungeon */
void AddSC_boss_nhalish();
void AddSC_boss_sadana();
void AddSC_boss_bonemaw();
void AddSC_boss_nerzul();
void AddSC_shadowmoon_burial_grounds();
void AddSC_instance_shadowmoon_burial_grounds();

/* Auchindoun Dungeon */
void AddSC_instance_auchindoun();
void AddSC_auchindoun();
void AddSC_boss_kaathar();
void AddSC_boss_nyami();
void AddSC_boss_azzakel();
void AddSC_boss_teronogor();

/* Everbloom Dungeon */
void AddSC_the_everbloom();
void AddSC_instance_everbloom();
void AddSC_boss_ancient_protectors();
void AddSC_boss_witherbark();
void AddSC_boss_xeritac();
void AddSC_boss_archmage_sol();
void AddSC_boss_yalnu();

/// Battlegrounds
void AddSC_BattlegroundAVcripts();
void AddSC_BattlegroundDGScripts();
void AddSC_BattlegroundSAScripts();

/// Outdoor pvp
void AddSC_outdoorpvp_hp();
void AddSC_outdoorpvp_na();
void AddSC_outdoorpvp_si();
void AddSC_outdoorpvp_tf();
void AddSC_outdoorpvp_zm();
void AddSC_AshranMgr();
void AddSC_AshranNPCAlliance();
void AddSC_AshranNPCHorde();
void AddSC_AshranSpells();
void AddSC_AshranAreaTrigger();
void AddSC_AshranNPCNeutral();
void AddSC_AshranQuest();

/// BattlePay
void AddSC_BattlePay_Services();
void AddSC_BattlePay_Golds();
void AddSC_BattlePay_Professions();

#endif

void AddScripts()
{
    AddSpellScripts();
    AddCommandScripts();
    AddWorldScripts();
    sAnticheatMgr->StartScripts();
    AddSC_SmartSCripts();
#ifdef SCRIPTS
    AddEasternKingdomsScripts();
    AddKalimdorScripts();
    AddOutlandScripts();
    AddNorthrendScripts();
    AddPandarieScripts();
    AddDraenorScripts();
    AddBattlegroundScripts();
    AddOutdoorPvPScripts();
    AddCustomScripts();
    AddBattlePayScripts();
#endif
}

void AddBattlePayScripts()
{
#ifdef SCRIPTS
    AddSC_BattlePay_Services();
    AddSC_BattlePay_Golds();
    AddSC_BattlePay_Professions();
#endif
}

void AddSpellScripts()
{
    AddSC_deathknight_spell_scripts();
    AddSC_druid_spell_scripts();
    AddSC_generic_spell_scripts();
    AddSC_hunter_spell_scripts();
    AddSC_mage_spell_scripts();
    AddSC_paladin_spell_scripts();
    AddSC_priest_spell_scripts();
    AddSC_rogue_spell_scripts();
    AddSC_shaman_spell_scripts();
    AddSC_warlock_spell_scripts();
    AddSC_warrior_spell_scripts();
    AddSC_monk_spell_scripts();
    AddSC_mastery_spell_scripts();
    AddSC_quest_spell_scripts();
    AddSC_item_spell_scripts();
    AddSC_holiday_spell_scripts();
    AddSC_spell_skill();
    AddSC_npc_spell_scripts();
    AddSC_areatrigger_spell_scripts();
    AddSC_spell_item_enchantment();
    AddSC_spell_toy();
    AddSC_npc_toy();
    AddSC_go_toy();
#ifdef SCRIPTS
    AddSC_petbattle_abilities();
    AddSC_npc_PetBattleTrainer();
    AddSC_PetBattlePlayerScript();
#endif
}

void AddCommandScripts()
{
    AddSC_anticheat_commandscript();
    AddSC_account_commandscript();
    AddSC_achievement_commandscript();
    AddSC_ban_commandscript();
    AddSC_bf_commandscript();
    AddSC_cast_commandscript();
    AddSC_character_commandscript();
    AddSC_cheat_commandscript();
    AddSC_debug_commandscript();
    AddSC_disable_commandscript();
    AddSC_event_commandscript();
    AddSC_gm_commandscript();
    AddSC_go_commandscript();
    AddSC_at_commandscript();
    AddSC_conversation_commandscript();
    AddSC_gobject_commandscript();
    AddSC_guild_commandscript();
    AddSC_honor_commandscript();
    AddSC_instance_commandscript();
    AddSC_learn_commandscript();
    AddSC_lookup_commandscript();
    AddSC_list_commandscript();
    AddSC_message_commandscript();
    AddSC_misc_commandscript();
    AddSC_modify_commandscript();
    AddSC_npc_commandscript();
    AddSC_quest_commandscript();
    AddSC_reload_commandscript();
    AddSC_reset_commandscript();
    AddSC_server_commandscript();
    AddSC_tele_commandscript();
    AddSC_ticket_commandscript();
    AddSC_titles_commandscript();
    AddSC_wp_commandscript();
    AddSC_garrison_commandscript();
    AddSC_hotfix_commandscript();
    AddSC_battlepay_commandscript();
    AddSC_items_commandscript();
    AddSC_mmaps_commandscript();
    AddSC_spellog_commandscript();
}

void AddWorldScripts()
{
    AddSC_areatrigger_scripts();
    AddSC_generic_creature();
    AddSC_go_scripts();
    AddSC_guards();
    AddSC_item_scripts();
    AddSC_npc_professions();
    AddSC_npc_innkeeper();
    AddSC_npcs_special();
    AddSC_npc_taxi();
    AddSC_achievement_scripts();
    AddSC_chat_log();
    AddSC_EncounterScripts();
    AddSC_WeeklyEventBonusScripts();
}

void AddEasternKingdomsScripts()
{
#ifdef SCRIPTS
    AddSC_alterac_valley();
    AddSC_boss_balinda();
    AddSC_boss_drekthar();
    AddSC_boss_galvangar();
    AddSC_boss_vanndar();
    AddSC_blackrock_depths();
    AddSC_boss_ambassador_flamelash();
    AddSC_boss_anubshiah();
    AddSC_boss_draganthaurissan();
    AddSC_boss_general_angerforge();
    AddSC_boss_gorosh_the_dervish();
    AddSC_boss_grizzle();
    AddSC_boss_high_interrogator_gerstahn();
    AddSC_boss_magmus();
    AddSC_boss_moira_bronzebeard();
    AddSC_boss_tomb_of_seven();
    AddSC_instance_blackrock_depths();
    AddSC_boss_razorgore();
    AddSC_boss_vael();
    AddSC_boss_broodlord();
    AddSC_boss_firemaw();
    AddSC_boss_ebonroc();
    AddSC_boss_flamegor();
    AddSC_boss_chromaggus();
    AddSC_boss_nefarian();
    AddSC_boss_victor_nefarius();
    AddSC_instance_deadmines();
    AddSC_deadmines();
    AddSC_boss_glubtok();
    AddSC_boss_helix_gearbreaker();
    AddSC_boss_foereaper5000();
    AddSC_boss_admiral_ripsnarl();
    AddSC_boss_captain_cookie();
    AddSC_boss_vanessa_vancleef();
    AddSC_gnomeregan();
    AddSC_instance_gnomeregan();
    AddSC_boss_attumen();
    AddSC_boss_curator();
    AddSC_boss_maiden_of_virtue();
    AddSC_boss_shade_of_aran();
    AddSC_boss_malchezaar();
    AddSC_boss_terestian_illhoof();
    AddSC_boss_moroes();
    AddSC_bosses_opera();
    AddSC_boss_netherspite();
    AddSC_instance_karazhan();
    AddSC_karazhan();
    AddSC_boss_nightbane();
    AddSC_boss_felblood_kaelthas();
    AddSC_boss_selin_fireheart();
    AddSC_boss_vexallus();
    AddSC_boss_priestess_delrissa();
    AddSC_instance_magisters_terrace();
    AddSC_magisters_terrace();
    AddSC_boss_lucifron();
    AddSC_boss_magmadar();
    AddSC_boss_gehennas();
    AddSC_boss_garr();
    AddSC_boss_baron_geddon();
    AddSC_boss_shazzrah();
    AddSC_boss_golemagg();
    AddSC_boss_sulfuron();
    AddSC_boss_majordomo();
    AddSC_boss_ragnaros();
    AddSC_instance_molten_core();
    AddSC_the_scarlet_enclave();
    AddSC_the_scarlet_enclave_c1();
    AddSC_the_scarlet_enclave_c2();
    AddSC_the_scarlet_enclave_c5();
    AddSC_boss_arcanist_doan();
    AddSC_boss_azshir_the_sleepless();
    AddSC_boss_bloodmage_thalnos();
    AddSC_boss_headless_horseman();
    AddSC_boss_herod();
    AddSC_boss_high_inquisitor_fairbanks();
    AddSC_boss_houndmaster_loksey();
    AddSC_boss_interrogator_vishas();
    AddSC_boss_scorn();
    AddSC_instance_scarlet_monastery();
    AddSC_boss_mograine_and_whitemane();
    AddSC_boss_darkmaster_gandling();
    AddSC_boss_death_knight_darkreaver();
    AddSC_boss_theolenkrastinov();
    AddSC_boss_illuciabarov();
    AddSC_boss_instructormalicia();
    AddSC_boss_jandicebarov();
    AddSC_boss_kormok();
    AddSC_boss_lordalexeibarov();
    AddSC_boss_lorekeeperpolkelt();
    AddSC_boss_rasfrost();
    AddSC_boss_theravenian();
    AddSC_boss_vectus();
    AddSC_instance_scholomance();
    AddSC_boss_baron_ashbury();
    AddSC_boss_baron_silverlaine();
    AddSC_boss_commander_springvale();
    AddSC_boss_lord_valden();
    AddSC_boss_lord_godfrey();
    AddSC_shadowfang_keep();
    AddSC_instance_shadowfang_keep();
    AddSC_boss_magistrate_barthilas();
    AddSC_boss_maleki_the_pallid();
    AddSC_boss_nerubenkan();
    AddSC_boss_cannon_master_willey();
    AddSC_boss_baroness_anastari();
    AddSC_boss_ramstein_the_gorger();
    AddSC_boss_timmy_the_cruel();
    AddSC_boss_postmaster_malown();
    AddSC_boss_baron_rivendare();
    AddSC_boss_dathrohan_balnazzar();
    AddSC_boss_order_of_silver_hand();
    AddSC_instance_stratholme();
    AddSC_stratholme();
    AddSC_sunken_temple();
    AddSC_instance_sunken_temple();
    AddSC_instance_sunwell_plateau();
    AddSC_boss_kalecgos();
    AddSC_boss_brutallus();
    AddSC_boss_felmyst();
    AddSC_boss_eredar_twins();
    AddSC_boss_muru();
    AddSC_boss_kiljaeden();
    AddSC_sunwell_plateau();
    AddSC_boss_archaedas();
    AddSC_boss_ironaya();
    AddSC_uldaman();
    AddSC_instance_uldaman();
    AddSC_boss_akilzon();
    AddSC_boss_halazzi();
    AddSC_boss_hex_lord_malacrass();
    AddSC_boss_janalai();
    AddSC_boss_nalorakk();
    AddSC_boss_daakara();
    AddSC_instance_zulaman();
    AddSC_zulaman();
    AddSC_boss_grilek();
    AddSC_boss_hazzarah();
    AddSC_boss_jindo_the_godbreaker();
    AddSC_boss_kilnara();
    AddSC_boss_mandokir();
    AddSC_boss_renataki();
    AddSC_boss_venoxis();
    AddSC_boss_wushoolay();
    AddSC_boss_zanzil();
    AddSC_instance_zulgurub();
    AddSC_arathi_highlands();
    AddSC_blasted_lands();
    AddSC_burning_steppes();
    AddSC_duskwood();
    AddSC_eastern_plaguelands();
    AddSC_eversong_woods();
    AddSC_elwyn_forest();
    AddSC_dun_morogh();
    AddSC_ghostlands();
    AddSC_hinterlands();
    AddSC_ironforge();
    AddSC_isle_of_queldanas();
    AddSC_loch_modan();
    AddSC_redridge_mountains();
    AddSC_silvermoon_city();
    AddSC_silverpine_forest();
    AddSC_stormwind_city();
    AddSC_stranglethorn_vale();
    AddSC_swamp_of_sorrows();
    AddSC_tirisfal_glades();
    AddSC_undercity();
    AddSC_western_plaguelands();
    AddSC_westfall();
    AddSC_wetlands();
    AddSC_upper_blackrock_spire();
    AddSC_instance_upper_blackrock_spire();
    AddSC_boss_orebender_gorashan();
    AddSC_boss_kyrak_the_corruptor();
    AddSC_boss_commander_tharbek();
    AddSC_boss_ragewing_the_untamed();
    AddSC_boss_warlord_zaela();

    AddSC_instance_throne_of_the_tides();
    AddSC_throne_of_the_tides();
    AddSC_boss_lady_nazjar();
    AddSC_boss_commander_ulthok();
    AddSC_boss_erunak_stonespeaker();
    AddSC_boss_ozumat();

    AddSC_instance_blackrock_caverns();
    AddSC_boss_romogg_bonecrusher();
    AddSC_boss_corla_herald_of_twilight();
    AddSC_boss_karsh_steelbender();
    AddSC_boss_beauty();
    AddSC_boss_ascendant_lord_obsidius();

    AddSC_instance_the_stonecore();
    AddSC_the_stonecore();
    AddSC_boss_corborus();
    AddSC_boss_slabhide();
    AddSC_boss_ozruk();
    AddSC_boss_high_priestess_azil();

    AddSC_instance_grim_batol();
    AddSC_grim_batol();
    AddSC_boss_general_umbriss();
    AddSC_boss_forgemaster_throngus();
    AddSC_boss_drahga_shadowburner();
    AddSC_boss_erudax();

    AddSC_instance_the_vortex_pinnacle();
    AddSC_the_vortex_pinnacle();
    AddSC_boss_grand_vizier_ertan();
    AddSC_boss_altairus();
    AddSC_boss_asaad();

    AddSC_instance_baradin_hold();
    AddSC_boss_argaloth();
    AddSC_boss_occuthar();
    AddSC_boss_alizabal();
#endif
}

void AddKalimdorScripts()
{
#ifdef SCRIPTS
    AddSC_instance_ragefire_chasm();
    AddSC_ragefire_chasm();
    AddSC_boss_lava_guard_gordoth();
    AddSC_blackfathom_deeps();
    AddSC_boss_gelihast();
    AddSC_boss_kelris();
    AddSC_boss_aku_mai();
    AddSC_instance_blackfathom_deeps();
    AddSC_hyjal();
    AddSC_boss_archimonde();
    AddSC_instance_mount_hyjal();
    AddSC_hyjal_trash();
    AddSC_boss_rage_winterchill();
    AddSC_boss_anetheron();
    AddSC_boss_kazrogal();
    AddSC_boss_azgalor();
    AddSC_boss_captain_skarloc();
    AddSC_boss_epoch_hunter();
    AddSC_boss_lieutenant_drake();
    AddSC_instance_old_hillsbrad();
    AddSC_old_hillsbrad();
    AddSC_boss_aeonus();
    AddSC_boss_chrono_lord_deja();
    AddSC_boss_temporus();
    AddSC_dark_portal();
    AddSC_instance_dark_portal();
    AddSC_boss_epoch();
    AddSC_boss_infinite_corruptor();
    AddSC_boss_salramm();
    AddSC_boss_mal_ganis();
    AddSC_boss_meathook();
    AddSC_culling_of_stratholme();
    AddSC_instance_culling_of_stratholme();
    AddSC_boss_celebras_the_cursed();
    AddSC_boss_landslide();
    AddSC_boss_noxxion();
    AddSC_boss_ptheradras();
    AddSC_boss_onyxia();
    AddSC_instance_onyxias_lair();
    AddSC_boss_amnennar_the_coldbringer();
    AddSC_razorfen_downs();
    AddSC_instance_razorfen_downs();
    AddSC_razorfen_kraul();
    AddSC_instance_razorfen_kraul();
    AddSC_boss_kurinnaxx();
    AddSC_boss_rajaxx();
    AddSC_boss_moam();
    AddSC_boss_buru();
    AddSC_boss_ayamiss();
    AddSC_boss_ossirian();
    AddSC_instance_ruins_of_ahnqiraj();
    AddSC_boss_cthun();
    AddSC_boss_fankriss();
    AddSC_boss_huhuran();
    AddSC_bug_trio();
    AddSC_boss_sartura();
    AddSC_boss_skeram();
    AddSC_boss_twinemperors();
    AddSC_boss_ouro();
    AddSC_mob_anubisath_sentinel();
    AddSC_instance_temple_of_ahnqiraj();
    AddSC_wailing_caverns();
    AddSC_instance_wailing_caverns();
    AddSC_zulfarrak();
    AddSC_instance_zulfarrak();
    AddSC_gilneas();

    AddSC_ashenvale();
    AddSC_azshara();
    AddSC_azuremyst_isle();
    AddSC_bloodmyst_isle();
    AddSC_darkshore();
    AddSC_desolace();
    AddSC_durotar();
    AddSC_dustwallow_marsh();
    AddSC_felwood();
    AddSC_feralas();
    AddSC_moonglade();
    AddSC_mulgore();
    AddSC_orgrimmar();
    AddSC_silithus();
    AddSC_southern_barrens();
    AddSC_stonetalon_mountains();
    AddSC_tanaris();
    AddSC_teldrassil();
    AddSC_the_barrens();
    AddSC_thousand_needles();
    AddSC_thunder_bluff();
    AddSC_uldum();
    AddSC_ungoro_crater();
    AddSC_winterspring();

    AddSC_instance_lost_city_of_the_tolvir();
    AddSC_lost_city_of_the_tolvir();
    AddSC_boss_general_husam();
    AddSC_boss_lockmaw_augh();
    AddSC_boss_high_prophet_barim();
    AddSC_boss_siamat();

    AddSC_instance_halls_of_origination();
    AddSC_halls_of_origination();
    AddSC_boss_temple_guardian_anhuur();
    AddSC_boss_earthrager_ptah();
    AddSC_boss_anraphet();
    AddSC_boss_ammunae();
    AddSC_boss_isiset();
    AddSC_boss_setesh();
    AddSC_boss_rajh();

    AddSC_instance_bastion_of_twilight();
    AddSC_bastion_of_twilight();
    AddSC_boss_halfus_wyrmbreaker();
    AddSC_boss_theralion_and_valiona();
    AddSC_boss_ascendant_council();
    AddSC_boss_chogall();
    AddSC_boss_sinestra();

    AddSC_instance_end_time();
    AddSC_end_time();
    AddSC_end_time_teleport();
    AddSC_boss_echo_of_tyrande();
    AddSC_boss_echo_of_sylvanas();
    AddSC_boss_echo_of_baine();
    AddSC_boss_echo_of_jaina();
    AddSC_boss_murozond();

    AddSC_instance_well_of_eternity();
    AddSC_well_of_eternity();
    AddSC_well_of_eternity_teleport();
    AddSC_boss_perotharn();
    AddSC_boss_queen_azshara();
    AddSC_boss_mannoroth();

    AddSC_instance_hour_of_twilight();
    AddSC_hour_of_twilight();
    AddSC_boss_arcurion();
    AddSC_boss_asira_dawnslayer();
    AddSC_boss_archbishop_benedictus();
#endif
}

void AddOutlandScripts()
{
#ifdef SCRIPTS
    AddSC_boss_exarch_maladaar();
    AddSC_boss_shirrak_the_dead_watcher();
    AddSC_boss_nexusprince_shaffar();
    AddSC_boss_pandemonius();
    AddSC_boss_darkweaver_syth();
    AddSC_boss_talon_king_ikiss();
    AddSC_instance_sethekk_halls();
    AddSC_instance_shadow_labyrinth();
    AddSC_boss_ambassador_hellmaw();
    AddSC_boss_blackheart_the_inciter();
    AddSC_boss_grandmaster_vorpil();
    AddSC_boss_murmur();
    AddSC_black_temple();
    AddSC_boss_illidan();
    AddSC_boss_shade_of_akama();
    AddSC_boss_supremus();
    AddSC_boss_gurtogg_bloodboil();
    AddSC_boss_mother_shahraz();
    AddSC_boss_reliquary_of_souls();
    AddSC_boss_teron_gorefiend();
    AddSC_boss_najentus();
    AddSC_boss_illidari_council();
    AddSC_instance_black_temple();
    AddSC_boss_fathomlord_karathress();
    AddSC_boss_hydross_the_unstable();
    AddSC_boss_lady_vashj();
    AddSC_boss_leotheras_the_blind();
    AddSC_boss_morogrim_tidewalker();
    AddSC_instance_serpentshrine_cavern();
    AddSC_boss_the_lurker_below();
    AddSC_boss_hydromancer_thespia();
    AddSC_boss_mekgineer_steamrigger();
    AddSC_boss_warlord_kalithresh();
    AddSC_instance_steam_vault();
    AddSC_boss_hungarfen();
    AddSC_boss_the_black_stalker();
    AddSC_boss_gruul();
    AddSC_boss_high_king_maulgar();
    AddSC_instance_gruuls_lair();
    AddSC_boss_broggok();
    AddSC_boss_kelidan_the_breaker();
    AddSC_boss_the_maker();
    AddSC_instance_blood_furnace();
    AddSC_boss_magtheridon();
    AddSC_instance_magtheridons_lair();
    AddSC_boss_grand_warlock_nethekurse();
    AddSC_boss_warbringer_omrogg();
    AddSC_boss_warchief_kargath_bladefist();
    AddSC_instance_shattered_halls();
    AddSC_boss_watchkeeper_gargolmar();
    AddSC_boss_omor_the_unscarred();
    AddSC_boss_vazruden_the_herald();
    AddSC_instance_ramparts();
    AddSC_arcatraz();
    AddSC_boss_harbinger_skyriss();
    AddSC_instance_arcatraz();
    AddSC_boss_high_botanist_freywinn();
    AddSC_boss_laj();
    AddSC_boss_warp_splinter();
    AddSC_boss_alar();
    AddSC_boss_kaelthas();
    AddSC_boss_void_reaver();
    AddSC_boss_high_astromancer_solarian();
    AddSC_instance_the_eye();
    AddSC_the_eye();
    AddSC_boss_gatewatcher_iron_hand();
    AddSC_boss_nethermancer_sepethrea();
    AddSC_boss_pathaleon_the_calculator();
    AddSC_boss_mechano_lord_capacitus();
    AddSC_instance_mechanar();
    AddSC_mechanar();

    AddSC_blades_edge_mountains();
    AddSC_boss_doomlordkazzak();
    AddSC_boss_doomwalker();
    AddSC_hellfire_peninsula();
    AddSC_nagrand();
    AddSC_netherstorm();
    AddSC_shadowmoon_valley();
    AddSC_shattrath_city();
    AddSC_terokkar_forest();
    AddSC_zangarmarsh();
#endif
}

void AddNorthrendScripts()
{
#ifdef SCRIPTS
    AddSC_boss_slad_ran();
    AddSC_boss_moorabi();
    AddSC_boss_drakkari_colossus();
    AddSC_boss_gal_darah();
    AddSC_boss_eck();
    AddSC_instance_gundrak();
    AddSC_boss_amanitar();
    AddSC_boss_taldaram();
    AddSC_boss_elder_nadox();
    AddSC_boss_jedoga_shadowseeker();
    AddSC_boss_volazj();
    AddSC_instance_ahnkahet();
    AddSC_boss_argent_challenge();
    AddSC_boss_black_knight();
    AddSC_boss_grand_champions();
    AddSC_instance_trial_of_the_champion();
    AddSC_trial_of_the_champion();
    AddSC_boss_anubarak_trial();
    AddSC_boss_faction_champions();
    AddSC_boss_jaraxxus();
    AddSC_trial_of_the_crusader();
    AddSC_boss_twin_valkyr();
    AddSC_boss_northrend_beasts();
    AddSC_instance_trial_of_the_crusader();
    AddSC_boss_krik_thir();
    AddSC_boss_hadronox();
    AddSC_boss_anub_arak();
    AddSC_instance_azjol_nerub();
    AddSC_boss_anubrekhan();
    AddSC_boss_maexxna();
    AddSC_boss_patchwerk();
    AddSC_boss_grobbulus();
    AddSC_boss_razuvious();
    AddSC_boss_kelthuzad();
    AddSC_boss_loatheb();
    AddSC_boss_noth();
    AddSC_boss_gluth();
    AddSC_boss_sapphiron();
    AddSC_boss_four_horsemen();
    AddSC_boss_faerlina();
    AddSC_boss_heigan();
    AddSC_boss_gothik();
    AddSC_boss_thaddius();
    AddSC_instance_naxxramas();
    AddSC_boss_magus_telestra();
    AddSC_boss_anomalus();
    AddSC_boss_ormorok();
    AddSC_boss_keristrasza();
    AddSC_instance_nexus();
    AddSC_boss_drakos();
    AddSC_boss_urom();
    AddSC_boss_varos();
    AddSC_boss_eregos();
    AddSC_instance_oculus();
    AddSC_oculus();
    AddSC_boss_malygos();
    AddSC_instance_eye_of_eternity();
    AddSC_boss_sartharion();
    AddSC_instance_obsidian_sanctum();
    AddSC_boss_bjarngrim();
    AddSC_boss_loken();
    AddSC_boss_ionar();
    AddSC_boss_volkhan();
    AddSC_instance_halls_of_lightning();
    AddSC_boss_maiden_of_grief();
    AddSC_boss_krystallus();
    AddSC_boss_sjonnir();
    AddSC_instance_halls_of_stone();
    AddSC_halls_of_stone();
    AddSC_instance_ulduar();
    AddSC_ulduar_teleporter();
    AddSC_ulduar_scripts();
    AddSC_boss_flame_leviathan();
    AddSC_boss_ignis();
    AddSC_boss_razorscale();
    AddSC_boss_xt002();
    AddSC_boss_assembly_of_iron();
    AddSC_boss_kologarn();
    AddSC_boss_auriaya();
    AddSC_boss_hodir();
    AddSC_boss_freya();
    AddSC_boss_mimiron();
    AddSC_boss_thorim();
    AddSC_boss_general_vezax();
    AddSC_boss_algalon_the_observer();
    AddSC_boss_keleseth();
    AddSC_boss_skarvald_dalronn();
    AddSC_boss_ingvar_the_plunderer();
    AddSC_instance_utgarde_keep();
    AddSC_boss_svala();
    AddSC_boss_palehoof();
    AddSC_boss_skadi();
    AddSC_boss_ymiron();
    AddSC_instance_utgarde_pinnacle();
    AddSC_utgarde_keep();
    AddSC_boss_archavon();
    AddSC_boss_emalon();
    AddSC_boss_koralon();
    AddSC_boss_toravon();
    AddSC_instance_archavon();
    AddSC_boss_trollgore();
    AddSC_boss_novos();
    AddSC_boss_dred();
    AddSC_boss_tharon_ja();
    AddSC_instance_drak_tharon();
    AddSC_boss_cyanigosa();
    AddSC_boss_erekem();
    AddSC_boss_ichoron();
    AddSC_boss_lavanthor();
    AddSC_boss_moragg();
    AddSC_boss_xevozz();
    AddSC_boss_zuramat();
    AddSC_instance_violet_hold();
    AddSC_violet_hold();
    AddSC_instance_forge_of_souls();
    AddSC_forge_of_souls();
    AddSC_boss_bronjahm();
    AddSC_boss_devourer_of_souls();
    AddSC_instance_pit_of_saron();
    AddSC_pit_of_saron();
    AddSC_boss_garfrost();
    AddSC_boss_ick();
    AddSC_boss_tyrannus();
    AddSC_instance_halls_of_reflection();
    AddSC_halls_of_reflection();
    AddSC_boss_falric();
    AddSC_boss_marwyn();
    AddSC_boss_lich_king_hr();
    AddSC_instance_icecrown_citadel();
    AddSC_icecrown_citadel();
    AddSC_icecrown_citadel_teleport();
    AddSC_boss_lord_marrowgar();
    AddSC_boss_lady_deathwhisper();
    AddSC_boss_gunship_battle();
    AddSC_boss_deathbringer_saurfang();
    AddSC_boss_festergut();
    AddSC_boss_rotface();
    AddSC_boss_professor_putricide();
    AddSC_boss_blood_prince_council();
    AddSC_boss_blood_queen_lana_thel();
    AddSC_boss_valithria_dreamwalker();
    AddSC_boss_sindragosa();
    AddSC_boss_the_lich_king();
    AddSC_instance_ruby_sanctum();
    AddSC_ruby_sanctum();
    AddSC_boss_baltharus_the_warborn();
    AddSC_boss_saviana_ragefire();
    AddSC_boss_general_zarithrian();
    AddSC_boss_halion();

    AddSC_dalaran();
    AddSC_borean_tundra();
    AddSC_dragonblight();
    AddSC_grizzly_hills();
    AddSC_howling_fjord();
    AddSC_icecrown();
    AddSC_sholazar_basin();
    AddSC_storm_peaks();
    AddSC_wintergrasp();
    AddSC_zuldrak();
    AddSC_crystalsong_forest();
    AddSC_isle_of_conquest();
#endif
}

void AddPandarieScripts()
{
#ifdef SCRIPTS
    AddSC_instance_temple_of_jade_serpent();
    AddSC_boss_wise_mari();
    AddSC_boss_lorewalker_stonestep();
    AddSC_boss_liu_flameheat();
    AddSC_boss_sha_of_doubt();
    AddSC_instance_stormstout_brewery();
    AddSC_stormstout_brewery();
    AddSC_boss_ook_ook();
    AddSC_instance_mogu_shan_palace();
    AddSC_boss_trial_of_the_king();
    AddSC_boss_gekkan();
    AddSC_boss_xin_the_weaponmaster();
    AddSC_instance_gate_setting_sun();
    AddSC_gate_setting_sun();
    AddSC_boss_saboteur_kiptilak();
    AddSC_boss_striker_gadok();
    AddSC_boss_commander_rimok();
    AddSC_boss_raigonn();
    AddSC_boss_sha_of_anger();
    AddSC_boss_galion();
    AddSC_boss_oondasta();
    AddSC_boss_nalak();
    //AddSC_boss_xuen();
    //AddSC_boss_yu_lon();
    //AddSC_boss_chi_ji();
    //AddSC_boss_niuzao();
    //AddSC_boss_ordos();
    AddSC_instance_shadopan_monastery();
    AddSC_shadopan_monastery();
    AddSC_boss_gu_cloudstrike();
    AddSC_boss_master_snowdrift();
    AddSC_boss_sha_of_violence();
    AddSC_boss_taran_zhu();
    AddSC_instance_siege_of_the_niuzao_temple();
    AddSC_siege_of_the_niuzao_temple();
    AddSC_boss_jinbak();
    AddSC_instance_mogu_shan_vault();
    AddSC_mogu_shan_vault();
    AddSC_boss_stone_guard();
    AddSC_boss_feng();
    AddSC_boss_garajal();
    AddSC_boss_spirit_kings();
    AddSC_boss_elegon();
    AddSC_boss_will_of_emperor();
    AddSC_instance_heart_of_fear();
    AddSC_heart_of_fear();
    AddSC_boss_zorlok();
    AddSC_boss_tayak();
    AddSC_boss_garalon();
    AddSC_boss_meljarak();
    AddSC_boss_unsok();
    AddSC_boss_shekzeer();
    AddSC_instance_terrace_of_endless_spring();
    AddSC_terrace_of_endless_spring();
    AddSC_boss_protectors_of_the_endless();
    AddSC_boss_tsulong();
    AddSC_boss_lei_shi();
    AddSC_boss_sha_of_fear();
    AddSC_instance_throne_of_thunder();
    AddSC_throne_of_thunder();
    AddSC_boss_jin_rokh_the_breaker();
    AddSC_boss_horridon();
    AddSC_boss_council_of_elders();
    AddSC_boss_tortos();
    AddSC_boss_megaera();
    AddSC_boss_ji_kun();
    AddSC_boss_durumu();
    AddSC_boss_primordius();
    AddSC_boss_dark_animus();
    AddSC_boss_iron_qon();
    AddSC_boss_twin_consorts();
    AddSC_boss_lei_shen();
    //AddSC_instance_siege_of_orgrimmar();
    //AddSC_siege_of_orgrimmar();
    //AddSC_immerseus();
    //AddSC_fallen_protectors();
    //AddSC_norushen();
    //AddSC_sha_of_pride();
    //AddSC_galakras();
    //AddSC_iron_juggernaut();
    //AddSC_korkron_dark_shamans();
    //AddSC_general_nazgrim();
    //AddSC_malkorok();
    //AddSC_spoils_of_pandaria();
    //AddSC_thok_the_bloodthirsty();
    //AddSC_siegecrafter_blackfuse();
    //AddSC_paragons_of_the_klaxxi();
    //AddSC_Garrosh_Hellscream();

    AddSC_townlong_steppes();
    AddSC_dread_wastes();
    AddSC_valley_of_the_four_winds();
    AddSC_krasarang_wilds();
    AddSC_jade_forest();
    AddSC_the_veiled_stair();
    AddSC_kun_lai_summit();
    AddSC_vale_of_eternal_blossoms();

    AddSC_WanderingIsland_North();
    AddSC_WanderingIsland_East();
    AddSC_WanderingIsland_West();
    AddSC_WanderingIsland_South();

    AddSC_timeless_isle();
#endif
}

void AddDraenorScripts()
{
#ifdef SCRIPTS
    AddSC_tanaan_jungle();
    AddSC_spires_of_arak();
    AddSC_frostfire_ridge();
    AddSC_draenor_shadowmoon_valley();
    AddSC_gorgrond();
    AddSC_talador();

    /// Garrison
    AddSC_Garrison_GO();
    AddSC_Garrison_NPC();
    AddSC_Garrison_Quest();
    AddSC_Garrison_PlayerConditions();
    AddSC_Garrison_A1();
    AddSC_Garrison_A2();
    AddSC_Garrison_A3();
    AddSC_Garrison_H1();
    AddSC_Garrison_H2();
    AddSC_Garrison_H3();
    AddSC_Garrison_AS();
    AddSC_Garrison_HS();
    AddSC_Garrison();

    /// Instance Skyreach
    AddSC_instance_Skyreach();
    AddSC_mob_instance_skyreach();
    AddSC_spell_instance_skyreach();
    AddSC_boss_ranjit();
    AddSC_boss_Araknath();
    AddSC_boss_Rukhran();
    AddSC_boss_HighSaveViryx();

    /// Instance Bloodmaul Slag Mines
    AddSC_instance_Bloodmaul();
    AddSC_mob_Bloodmaul();
    AddSC_spell_Bloodmaul();
    AddSC_areatrigger_Bloodmaul();
    AddSC_boss_SlaveWatcherCrushto();
    AddSC_boss_forgemaster_gogduh();
    AddSC_boss_roltall();
    AddSC_boss_gugrokk();

    /// Highmaul
    AddSC_instance_highmaul();
    AddSC_highmaul();
    AddSC_boss_kargath_bladefist();
    AddSC_boss_the_butcher();
    AddSC_boss_brackenspore();
    AddSC_boss_tectus();
    AddSC_boss_twin_ogron();
    AddSC_boss_koragh();
    AddSC_boss_imperator_margok();

    /// Blackrock Foundry
    AddSC_instance_blackrock_foundry();
    AddSC_blackrock_foundry();
    AddSC_boss_gruul_foundry();
    AddSC_boss_oregorger();
    AddSC_boss_blast_furnace();
    AddSC_boss_hansgar_and_franzok();
    AddSC_boss_flamebender_kagraz();
    AddSC_boss_kromog();
    AddSC_boss_beastlord_darmac();
    AddSC_boss_operator_thogar();
    AddSC_boss_iron_maidens();
    AddSC_boss_blackhand();

    /// Instance Auchindoun
    AddSC_instance_auchindoun();
    AddSC_auchindoun();
    AddSC_boss_kaathar();
    AddSC_boss_nyami();
    AddSC_boss_azzakel();
    AddSC_boss_teronogor();

    /// Instance Everbloom
    AddSC_the_everbloom();
    AddSC_instance_everbloom();
    AddSC_boss_ancient_protectors();
    AddSC_boss_witherbark();
    AddSC_boss_xeritac();
    AddSC_boss_archmage_sol();
    AddSC_boss_yalnu();

    /// Instance Iron Docks
    AddSC_iron_docks();
    AddSC_instance_iron_docks();
    AddSC_boss_nokgar();
    AddSC_boss_skulloc();
    AddSC_boss_grimrail();
    AddSC_boss_oshir();

    /// Instance Shadowmoon Burial Grounds
    AddSC_boss_nhalish();
    AddSC_boss_sadana();
    AddSC_boss_bonemaw();
    AddSC_boss_nerzul();
    AddSC_shadowmoon_burial_grounds();
    AddSC_instance_shadowmoon_burial_grounds();

    /// Battlegrounds strongbox
    AddSC_item_strongboxes();

    /// Draenor Scenario
    AddSC_instance_scenario_fate_of_karabor();
#endif
}

void AddOutdoorPvPScripts()
{
#ifdef SCRIPTS
    AddSC_outdoorpvp_hp();
    AddSC_outdoorpvp_na();
    AddSC_outdoorpvp_si();
    AddSC_outdoorpvp_tf();
    AddSC_outdoorpvp_zm();
    AddSC_AshranMgr();
    AddSC_AshranNPCAlliance();
    AddSC_AshranNPCHorde();
    AddSC_AshranSpells();
    AddSC_AshranAreaTrigger();
    AddSC_AshranNPCNeutral();
    AddSC_AshranQuest();
#endif
}

void AddBattlegroundScripts()
{
#ifdef SCRIPTS
    AddSC_BattlegroundAVcripts();
    AddSC_BattlegroundDGScripts();
    AddSC_BattlegroundSAScripts();
#endif
}

#ifdef SCRIPTS
/* This is where custom scripts' loading functions should be declared. */

#endif

void AddCustomScripts()
{
#ifdef SCRIPTS
    AddSC_StressTest();
    AddSC_event_april();
    AddSC_promotion_new_players();
    AddSC_first_time_connexion();
    AddSC_user_reporting();
    AddSC_warning_update_client();
    AddSC_DuelReset();
    AddSC_npc_custom();
    AddSC_Webshop_Delivery();
    AddSC_Character_Renderer();
#endif
}
