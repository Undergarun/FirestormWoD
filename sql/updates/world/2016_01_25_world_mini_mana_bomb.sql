DELETE FROM creature_template_addon WHERE entry = 65903;
INSERT INTO creature_template_addon VALUE
(65903, 0, 0, 0, 0, 0, "105729", 0);

UPDATE creature_template SET ScriptName = "npc_toy_mini_maba_bomb" WHERE entry = 65903;