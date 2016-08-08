replace into `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) values
('1','0' ,'0','I would like to place an order.','0','0','0','0','0','0',NULL),
('1','1' ,'0','I would like to get Sumptuous Fur.','0','0','0','0','0','0',NULL),
('1','2' ,'0','I would like to get Raw Beast Hide.','0','0','0','0','0','0',NULL),
('1','3' ,'0','I would like to get Savage Feast.','0','0','0','0','0','0',NULL),
('1','4' ,'0','I would like to get Sumptuous Fur with Savage blood.','0','0','0','0','0','0',NULL),
('1','5' ,'0','I would like to get Raw Beast Hide with Savage blood.','0','0','0','0','0','0',NULL),
('1','6' ,'0','I would like to get Savage Feast with Savage blood.','0','0','0','0','0','0',NULL),
('1','7' ,'0','I want to browse your goods.','0','0','0','0','0','0',NULL),
('1','8' ,'1','Access to my bank.','0','0','0','0','0','0',NULL),
('1','9' ,'0','Frostweed.','0','0','0','0','0','0',NULL),
('1','10','0','Starflower.','0','0','0','0','0','0',NULL),
('1','11','0','Fireweed.','0','0','0','0','0','0',NULL),
('1','12','0','Talador Orchid.','0','0','0','0','0','0',NULL),
('1','13','0','Gorgrond Flytrap.','0','0','0','0','0','0',NULL),
('1','14','0','Nagrand Arrowbloom.','0','0','0','0','0','0',NULL),
('1','15','0','I\'d like to requisition my seal for the week.','0','0','0','0','0','0',NULL),
('1','16','0','Can you refine this draenic stone into ore for me?','0','0','0','0','0','0',NULL),
('1','17','1','I need you to do something for me.','0','0','0','0','0','0',NULL),
('1','18','0','Create me a garrison.','0','0','0','0','0','0',NULL),
('1','19','0','Time to get back to work.','0','0','0','0','0','0',NULL),
('1','20','0','Gather Shelly\'s report.','0','0','0','0','0','0',NULL),
('1','21','0','Gazlowe needs you.','0','0','0','0','0','0',NULL),
('1','22','0','I would like to recruit a follower','0','0','0','0','0','0',NULL),
('1','23','0','I would like to pick what we plant next.','0','0','0','0','0','0',NULL);

-- need another parse of BroadcastText.db2 for all used locales, updated to 6.2.3 DBCs.
REPLACE INTO locales_gossip_menu_option (menu_id, id, option_text_loc1, option_text_loc2, option_text_loc3, option_text_loc8, option_text_loc9) VALUE
(1, 0, "I would like to place an order.", "Je voudrais passer une commande.", "Ich möchte einen Arbeitsauftrag erteilen.", "Я хочу сделать заказ.", "Me gustaría realizar un pedido.");