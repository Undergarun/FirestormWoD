UPDATE `creature_template` SET `faction` = '474' WHERE `entry` = '47337'; 

update `creature_template` set `npcflag` = 4194433 where gossip_menu_id = 9821;

delete from gossip_menu_option where menu_id = 9821;
insert into `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) values('9821','0','12','I\'d like to stable my pet here.','14','4194304','0','0','0','0','');
insert into `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) values('9821','1','1','I\'m looking for a lost companion.','3','128','0','0','0','0','');
insert into `gossip_menu_option` (`menu_id`, `id`, `option_icon`, `option_text`, `option_id`, `npc_option_npcflag`, `action_menu_id`, `action_poi_id`, `box_coded`, `box_money`, `box_text`) values('9821','2','0','I\'d like to heal and revive my battle pets.','0','0','0','0','0','1000','A small fee for supplies is required.');