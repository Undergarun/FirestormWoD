UPDATE creature_template SET IconName = "", gossip_menu_id = 0, npcflag = (npcflag | 0x80), ScriptName = "npc_ethereal_soul_trader" WHERE entry = 27914;

DELETE FROM npc_vendor WHERE entry = 27914;
INSERT INTO npc_vendor VALUES
(27914, 0, 38308, 0, 0, 2411, 1),
(27914, 0, 38300, 0, 0, 2411, 1),
(27914, 0, 38294, 0, 0, 2412, 1),
(27914, 0, 38291, 0, 0, 2408, 1),
(27914, 0, 38163, 0, 0, 2408, 1),
(27914, 0, 38160, 0, 0, 2410, 1),
(27914, 0, 38286, 0, 0, 2407, 1),
(27914, 0, 38285, 0, 0, 2408, 1),
(27914, 0, 38161, 0, 0, 2409, 1),
(27914, 0, 38162, 0, 0, 2409, 1);

DELETE FROM spell_script_names WHERE spell_id IN (50051, 50101);
INSERT INTO spell_script_names VALUES
(50051, "spell_ethereal_pet_aura"),
(50101, "spell_ethereal_pet_onkill_steal_essence");