UPDATE creature_template SET gossip_menu_id = 64227, ScriptName = "npc_frozen_trail_packer" WHERE entry = 64227;
REPLACE INTO creature_template_addon(entry, auras) VALUES(64227, "125234");

DELETE FROM creature WHERE id = 64227;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES('64227','870','5841','6156','1','1','40843','0','2434.25','2638.47','667.571','3.73093','21600','0','0','214985','0','0','1','0','0','0','0','0','461232');

DELETE FROM gossip_menu WHERE entry = 64227 AND text_id = 64227;
INSERT INTO gossip_menu VALUES (64227, 64227);

DELETE FROM npc_text WHERE ID = 64227;
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    64227,
    "The sherpen has clearly been frozen here for some time, but you see a glint of light from something in his pack.",
    "The sherpen has clearly been frozen here for some time, but you see a glint of light from something in his pack."
);

DELETE FROM locales_npc_text WHERE entry = 64227;
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    64227,
    "De toute évidence, ce sherpa est resté congelé ici pendant un bon moment, cependant, vous apercevez un éclat de lumière qui provient de l'intérieur de son sac.",
    "De toute évidence, ce sherpa est resté congelé ici pendant un bon moment, cependant, vous apercevez un éclat de lumière qui provient de l'intérieur de son sac.",
    "Der Sherpa ist sicherlich schon einige Weile hier eingefroren, aber Ihr seht einen Schimmer Licht in seinem Rucksack funkeln.",
    "Der Sherpa ist sicherlich schon einige Weile hier eingefroren, aber Ihr seht einen Schimmer Licht in seinem Rucksack funkeln.",
    "Salta a la vista que este sherpen lleva bastante tiempo congelado aquí, pero ves un destello de luz procedente de su mochila.",
    "Salta a la vista que este sherpen lleva bastante tiempo congelado aquí, pero ves un destello de luz procedente de su mochila.",
    "?????? ?????? ? ???? ????????? ??? ? ????? ????????? ?????-?? ?????. ? ??? ??? ???????? ? ??? ?????",
    "?????? ?????? ? ???? ????????? ??? ? ????? ????????? ?????-?? ?????. ? ??? ??? ???????? ? ??? ?????"
);

DELETE FROM gossip_menu_option WHERE menu_id = 64227;
INSERT INTO gossip_menu_option VALUES
(64227, 0, 0, "Take the item from his pack.", 1, 3, 0, 0, 0, 0, '');
DELETE FROM locales_gossip_menu_option WHERE menu_id = 64227;
--                                                   French            German            Spanish           Russian
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc3, option_text_loc6, option_text_loc8) VALUES
(
    64227, 0,
    "Prendre l'objet dans son ballot.",
    "Nehmt den Gegenstand aus seinem Gepäck.",
    "Coge el objeto de su mochila.",
    "????? ??????? ?? ????."
);