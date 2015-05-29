
UPDATE creature_template SET ScriptName="npc_EricBroadoak_Garr", npcFlag=131, npcFlag2=128, minlevel=90, maxlevel=90, gossip_menu_id = 0 WHERE entry=77372;
UPDATE creature_template SET ScriptName="npc_KurtBroadoak_Garr", npcFlag=3, npcFlag2=32, minlevel=90, maxlevel=90, gossip_menu_id = 0 WHERE entry=77777;

DELETE FROM `npc_vendor` WHERE `entry` = 77372 AND `type` = 1;
INSERT INTO `npc_vendor` (`entry`, `slot`, `item`, `maxcount`, `incrtime`, `ExtendedCost`, `type`) VALUES
(77372, 0, 38682, 0, 0, 0, 1), 
(77372, 0, 111923, 0, 0, 0, 1), 
(77372, 0, 118605, 0, 0, 5465, 1), 
(77372, 0, 118606, 0, 0, 5466, 1), 
(77372, 0, 118607, 0, 0, 5465, 1), 
(77372, 0, 118608, 0, 0, 5466, 1), 
(77372, 0, 118610, 0, 0, 5466, 1), 
(77372, 0, 118613, 0, 0, 5465, 1), 
(77372, 0, 118614, 0, 0, 5465, 1), 
(77372, 0, 118615, 0, 0, 5465, 1), 
(77372, 0, 120265, 0, 0, 5466, 1), 
(77372, 0, 120266, 0, 0, 5466, 1);

DELETE FROM creature_queststarter WHERE id=77372 AND quest=36647;
INSERT INTO creature_queststarter(`id`, `quest`) values('77372','36647');

DELETE FROM creature_questender WHERE id=77777 AND quest=36647;
INSERT INTO creature_questender(`id`, `quest`) values('77777','36647');


UPDATE gameobject_template SET flags=0, ScriptName="go_garrison_shipment_container" WHERE TYPE=45;

DELETE FROM garrison_plot_content WHERE plot_type_or_building=-95 AND faction_index = 1;
