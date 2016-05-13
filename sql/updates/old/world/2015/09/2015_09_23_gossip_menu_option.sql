-- fix vendor menu for [npc=24468] & [npc=24510]
INSERT INTO `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`)
VALUES
    (8958, 7, 1, 'I want to browse your goods', 3, 128, 0, 0, 0, 0, '');
