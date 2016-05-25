# Thogar trashes
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_man_at_arms" WHERE entry = 78832;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_raider" WHERE entry = 81197;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_iron_crack_shot" WHERE entry = 81315;
UPDATE creature_template SET dmg_multiplier = 4, ScriptName = "npc_foundry_gromkar_firemender" WHERE entry = 87841;

REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(81197, 1, 107703, 107703, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(87841, 1, 108772, 107801, 0);
REPLACE INTO creature_equip_template (entry, id, itemEntry1, itemEntry2, itemEntry3) VALUE(81315, 1, 0, 0, 108426);

DELETE FROM spell_script_names WHERE ScriptName = "spell_foundry_cauterizing_bolt" AND spell_id = 160140;
INSERT INTO spell_script_names VALUE (160140, "spell_foundry_cauterizing_bolt");