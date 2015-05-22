UPDATE creature_template SET flags_extra = 2, modelid1=32803, gossip_menu_id=83, EXP=4, unit_flags=768, unit_flags2=2048, type_flags=2 WHERE entry=6491;
UPDATE creature_template_addon SET auras="" WHERE entry=6491;
UPDATE gossip_menu_option SET option_id = 6, npc_option_npcflag=16384 WHERE menu_id=83;