SET NAMES utf8;

DELETE FROM creature WHERE id = 88206;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, position_x, position_y, position_z, orientation) VALUES
(88206, 0, 4, 4, 1, 4294967295, -10761.14, -2957.668, 58.74221, 3.043509),
(88206, 1190, 4, 4, 1, 4294967295, -10761.14, -2957.668, 58.74221, 3.043509);

UPDATE creature_template SET npcflag = 1, unit_flags = 768, gossip_menu_id = 88206, ScriptName = "npc_blasted_lands_zidormi" WHERE entry = 88206;

DELETE FROM gossip_menu WHERE entry IN (88206, 88207) AND text_id = 90355;
INSERT INTO gossip_menu VALUE
(88206, 90355),
(88207, 90355);

DELETE FROM npc_text WHERE ID = 90355;
INSERT INTO npc_text (ID, text0_0, text0_1) VALUE
(
    90355,
    "War does not simply visit these lands, it lives here. The violence thrives in past, present, and future...",
    "War does not simply visit these lands, it lives here. The violence thrives in past, present, and future..."
);

DELETE FROM locales_npc_text WHERE entry = 90355;
--                                   French                      German                      Spanish                     Russian
INSERT INTO locales_npc_text (entry, Text0_0_loc2, Text0_1_loc2, Text0_0_loc3, Text0_1_loc3, Text0_0_loc6, Text0_1_loc6, Text0_0_loc8, Text0_1_loc8) VALUE
(
    90355,
    "La guerre ne s'est pas simplement invitée ici. Cette terre et elle sont indissociables. La violence fait depuis longtemps partie du paysage, et c'est bien loin d'être fini...",
    "La guerre ne s'est pas simplement invitée ici. Cette terre et elle sont indissociables. La violence fait depuis longtemps partie du paysage, et c'est bien loin d'être fini...",
    "Der Krieg besucht dieses Land nicht einfach, er lebt hier. Die Gewalt gedeiht in der Vergangenheit, der Gegenwart und der Zukunft...",
    "Der Krieg besucht dieses Land nicht einfach, er lebt hier. Die Gewalt gedeiht in der Vergangenheit, der Gegenwart und der Zukunft...",
    "La guerra no visita simplemente estas tierras, sino que vive aquí. La violencia vive en el pasado, presente y futuro...",
    "La guerra no visita simplemente estas tierras, sino que vive aquí. La violencia vive en el pasado, presente y futuro...",
    "Война здесь не просто бывает, она тут не кончается. Жестокость процветает в прошлом, настоящем и будущем.",
    "Война здесь не просто бывает, она тут не кончается. Жестокость процветает в прошлом, настоящем и будущем."
);

DELETE FROM gossip_menu_option WHERE menu_id IN (88206, 88207);
INSERT INTO gossip_menu_option VALUES
(88206, 0, 0, "Show me the Blasted Lands before the invasion.", 1, 3, 0, 0, 0, 0, ''),
(88207, 0, 0, "Take me back to the present.", 1, 3, 0, 0, 0, 0, '');

DELETE FROM locales_gossip_menu_option WHERE menu_id IN (88206, 88207);
--                                                   French            German            Spanish           Russian
INSERT INTO locales_gossip_menu_option (menu_id, id, option_text_loc2, option_text_loc3, option_text_loc6, option_text_loc8) VALUES
(
    88206, 0,
    "Montrez-moi les terres Foudroyées avant l'invasion.",
    "Zeigt mir die Verwüsteten Lande vor der Invasion.",
    "Muéstrame las Tierras Devastadas antes de la invasión.",
    "Покажи мне Выжженные земли до вторжения."
),
(
    88207, 0,
    "Renvoyez-moi dans le présent.",
    "Bringt mich zurück in die Gegenwart.",
    "Llévame de vuelta al presente.",
    "Верни меня в настоящее."
);