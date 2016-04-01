DELETE FROM areatrigger_template WHERE spell_id = 147490;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags)
VALUE (147490, 1, 7329, 10, 10, 16388);

UPDATE creature_template SET modelid1 = 11686, modelid2 = 0, ScriptName = "spell_npc_sha_healing_rain" WHERE entry = 73400;
UPDATE creature_template_addon SET auras = "" WHERE entry = 73400;
