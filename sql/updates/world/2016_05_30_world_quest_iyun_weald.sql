-- Quest 36571 - Bonus Objective: Iyun Weald
DELETE FROM `npc_spellclick_spells` WHERE npc_entry=85809;
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, cast_flags, user_type) VALUES(85809, 174732, 1, 0);
UPDATE `creature_template` SET npcflag= `npcflag` | 0x01000000 WHERE entry=85809;

DELETE FROM `spell_script_names` WHERE `spell_id` = 174732;
INSERT INTO `spell_script_names`(spell_id, ScriptName) VALUES (174732, "spell_quest_gorgrond_punt_podling");

UPDATE creature_template SET faction = 35 WHERE entry = 85809;