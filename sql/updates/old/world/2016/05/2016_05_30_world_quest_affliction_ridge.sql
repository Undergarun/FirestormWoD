-- Quest 36473 - Bonus Objective: Affliction Ridge
DELETE FROM `npc_spellclick_spells` WHERE npc_entry=85524;
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, cast_flags, user_type) VALUES(85524, 170769, 1, 0);
UPDATE `creature_template` SET npcflag= `npcflag` | 0x01000000 WHERE entry= 85524;

DELETE FROM `spell_script_names` WHERE `spell_id` = 170769;
INSERT INTO `spell_script_names`(spell_id, ScriptName) VALUES (170769, "spell_quest_gorgrond_burn_ancient_corpse");