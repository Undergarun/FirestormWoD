-- Quest 36792 - Bonus Objective: Bladefist Hold
DELETE FROM `npc_spellclick_spells` WHERE npc_entry=86524;
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, cast_flags, user_type) VALUES(86524, 172944, 1, 0);
UPDATE `creature_template` SET npcflag= `npcflag` | 0x01000000 WHERE entry=86524;

DELETE FROM `spell_script_names` WHERE `spell_id` = 172944;
INSERT INTO `spell_script_names`(spell_id, ScriptName) VALUES (172944, "spell_quest_spires_of_arak_detonate_iron_grenade");