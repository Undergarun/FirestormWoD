-- Quest 34504 - Bonus Objective: Bloodthorn Cave
DELETE FROM `npc_spellclick_spells` WHERE npc_entry IN (78902, 78903, 78904);
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, cast_flags, user_type) VALUES(78904, 159303, 1, 0), (78903, 159303, 1, 0), (78902, 159303, 1, 0);
UPDATE `creature_template` SET npcflag= `npcflag` | 0x01000000, `unit_flags2` = `unit_flags2` | 0x00008000, `unit_flags` = `unit_flags` & ~0x02000000 WHERE entry IN (78902, 78903, 78904);
DELETE FROM `spell_script_names` WHERE `spell_id` = 159303;
INSERT INTO `spell_script_names`(spell_id, ScriptName) VALUES (159303, "spell_quest_shadowmoon_squeezing");
 