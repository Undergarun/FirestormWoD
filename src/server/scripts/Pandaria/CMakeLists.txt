#
#  MILLENIUM-STUDIO
#  Copyright 2016 Millenium-studio SARL
#  All Rights Reserved.
#

set(scripts_STAT_SRCS
  ${scripts_STAT_SRCS}
  Pandaria/boss_sha_of_anger.cpp
  Pandaria/boss_galion.cpp
  Pandaria/boss_chi_ji.cpp
  Pandaria/boss_nalak.cpp
  Pandaria/boss_niuzao.cpp
  Pandaria/boss_ordos.cpp
  Pandaria/boss_xuen.cpp
  Pandaria/boss_yu_lon.cpp
  Pandaria/boss_oondasta.cpp

  Pandaria/MogushanPalace/instance_mogu_shan_palace.cpp
  Pandaria/MogushanPalace/boss_trial_of_the_king.cpp
  Pandaria/MogushanPalace/boss_gekkan.cpp
  Pandaria/MogushanPalace/boss_xin_the_weaponmaster.cpp

  Pandaria/TempleJadeSerpent/instance_temple_of_jade_serpent.cpp
  Pandaria/TempleJadeSerpent/boss_wise_mari.cpp
  Pandaria/TempleJadeSerpent/boss_lorewalker_stonestep.cpp
  Pandaria/TempleJadeSerpent/boss_liu_flameheart.cpp
  Pandaria/TempleJadeSerpent/boss_sha_of_doubt.cpp

  Pandaria/StormstoutBrewery/instance_stormstout_brewery.cpp
  Pandaria/StormstoutBrewery/stormstout_brewery.cpp
  Pandaria/StormstoutBrewery/stormstout_brewery.h
  Pandaria/StormstoutBrewery/boss_ook_ook.cpp

  Pandaria/GateSettingSun/instance_gate_setting_sun.cpp
  Pandaria/GateSettingSun/gate_setting_sun.cpp
  Pandaria/GateSettingSun/gate_setting_sun.h
  Pandaria/GateSettingSun/boss_saboteur_kiptilak.cpp
  Pandaria/GateSettingSun/boss_striker_gadok.cpp
  Pandaria/GateSettingSun/boss_commander_rimok.cpp
  Pandaria/GateSettingSun/boss_raigonn.cpp

  Pandaria/ShadopanMonastery/instance_shadopan_monastery.cpp
  Pandaria/ShadopanMonastery/shadopan_monastery.cpp
  Pandaria/ShadopanMonastery/shadopan_monastery.h
  Pandaria/ShadopanMonastery/boss_gu_cloudstrike.cpp
  Pandaria/ShadopanMonastery/boss_master_snowdrift.cpp
  Pandaria/ShadopanMonastery/boss_sha_of_violence.cpp
  Pandaria/ShadopanMonastery/boss_taran_zhu.cpp
  
  Pandaria/SiegeNiuzaoTemple/instance_siege_of_the_niuzao_temple.cpp
  Pandaria/SiegeNiuzaoTemple/siege_of_the_niuzao_temple.cpp
  Pandaria/SiegeNiuzaoTemple/siege_of_the_niuzao_temple.h
  Pandaria/SiegeNiuzaoTemple/boss_jinbak.cpp

  Pandaria/MogushanVault/instance_mogu_shan_vault.cpp
  Pandaria/MogushanVault/mogu_shan_vault.cpp
  Pandaria/MogushanVault/mogu_shan_vault.h
  Pandaria/MogushanVault/boss_stone_guard.cpp
  Pandaria/MogushanVault/boss_feng.cpp
  Pandaria/MogushanVault/boss_garajal.cpp
  Pandaria/MogushanVault/boss_spirit_kings.cpp
  Pandaria/MogushanVault/boss_elegon.cpp
  Pandaria/MogushanVault/boss_will_of_emperor.cpp
  
  Pandaria/HeartOfFear/instance_heart_of_fear.cpp
  Pandaria/HeartOfFear/heart_of_fear.cpp
  Pandaria/HeartOfFear/heart_of_fear.h
  Pandaria/HeartOfFear/boss_zorlok.cpp
  Pandaria/HeartOfFear/boss_tayak.cpp
  Pandaria/HeartOfFear/boss_garalon.cpp
  Pandaria/HeartOfFear/boss_meljarak.cpp
  Pandaria/HeartOfFear/boss_unsok.cpp
  Pandaria/HeartOfFear/boss_shekzeer.cpp
  
  Pandaria/TerraceOfEndlessSpring/instance_terrace_of_endless_spring.cpp
  Pandaria/TerraceOfEndlessSpring/terrace_of_endless_spring.cpp
  Pandaria/TerraceOfEndlessSpring/terrace_of_endless_spring.h
  Pandaria/TerraceOfEndlessSpring/boss_protectors_of_the_endless.cpp
  Pandaria/TerraceOfEndlessSpring/boss_tsulong.cpp
  Pandaria/TerraceOfEndlessSpring/boss_lei_shi.cpp
  Pandaria/TerraceOfEndlessSpring/boss_sha_of_fear.cpp

  Pandaria/SiegeOfOrgrimmar/instance_siege_of_orgrimmar.cpp
  Pandaria/SiegeOfOrgrimmar/siege_of_orgrimmar.cpp
  Pandaria/SiegeOfOrgrimmar/siege_of_orgrimmar.h
  Pandaria/SiegeOfOrgrimmar/boss_immerseus.cpp
  Pandaria/SiegeOfOrgrimmar/boss_fallen_protectors.cpp
  Pandaria/SiegeOfOrgrimmar/boss_norushen.cpp
  Pandaria/SiegeOfOrgrimmar/boss_sha_of_pride.cpp
  Pandaria/SiegeOfOrgrimmar/boss_galakras.cpp
  Pandaria/SiegeOfOrgrimmar/boss_iron_juggernaut.cpp
  Pandaria/SiegeOfOrgrimmar/boss_korkron_dark_shamans.cpp
  Pandaria/SiegeOfOrgrimmar/boss_general_nazgrim.cpp
  Pandaria/SiegeOfOrgrimmar/boss_malkorok.cpp
  Pandaria/SiegeOfOrgrimmar/boss_spoils_of_pandaria.cpp
  Pandaria/SiegeOfOrgrimmar/boss_thok_the_bloodthirsty.cpp
  Pandaria/SiegeOfOrgrimmar/boss_siegecrafter_blackfuse.cpp
  Pandaria/SiegeOfOrgrimmar/boss_paragons_of_the_klaxxi.cpp
  Pandaria/SiegeOfOrgrimmar/boss_garrosh_hellscream.cpp

  Pandaria/WanderingIsland/WanderingIsland_North.cpp
  Pandaria/WanderingIsland/WanderingIsland_East.cpp
  Pandaria/WanderingIsland/WanderingIsland_West.cpp
  Pandaria/WanderingIsland/WanderingIsland_South.cpp

  Pandaria/ThroneOfThunder/instance_throne_of_thunder.cpp
  Pandaria/ThroneOfThunder/throne_of_thunder.h
  Pandaria/ThroneOfThunder/throne_of_thunder.cpp
  Pandaria/ThroneOfThunder/boss_jin_rokh_the_breaker.cpp
  Pandaria/ThroneOfThunder/boss_council_of_elders.cpp
  Pandaria/ThroneOfThunder/boss_horridon.cpp
  Pandaria/ThroneOfThunder/boss_tortos.cpp
  Pandaria/ThroneOfThunder/boss_megaera.cpp
  Pandaria/ThroneOfThunder/boss_ji_kun.cpp
  Pandaria/ThroneOfThunder/boss_primordius.cpp
  Pandaria/ThroneOfThunder/boss_durumu.cpp
  Pandaria/ThroneOfThunder/boss_dark_animus.cpp
  Pandaria/ThroneOfThunder/boss_iron_qon.cpp
  Pandaria/ThroneOfThunder/boss_twin_consorts.cpp
  Pandaria/ThroneOfThunder/boss_lei_shen.cpp

  Pandaria/townlong_steppes.cpp
  Pandaria/townlong_steppes.h
  Pandaria/dread_wastes.cpp
  Pandaria/dread_wastes.h
  Pandaria/valley_of_the_four_winds.cpp
  Pandaria/valley_of_the_four_winds.h
  Pandaria/krasarang_wilds.cpp
  Pandaria/krasarang_wilds.h
  Pandaria/kun_lai_summit.h
  Pandaria/kun_lai_summit.cpp
  Pandaria/jade_forest.cpp
  Pandaria/jade_forest.h
  Pandaria/vale_of_eternal_blossoms.cpp
  Pandaria/vale_of_eternal_blossoms.h
  Pandaria/timeless_isle.cpp
  Pandaria/timeless_isle.h
  Pandaria/the_veiled_stair.cpp
  Pandaria/the_veiled_stair.h
)

message("  -> Prepared: Pandaria")
