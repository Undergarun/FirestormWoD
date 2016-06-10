-- Quest 34667 - Bonus Objective: Court of Souls
UPDATE creature_template SET ScriptName="npc_talador_warlock_soulstealer" WHERE entry=79482;
UPDATE creature_template SET VehicleId=3528, spell1=160656 WHERE entry=79523;

DELETE FROM `npc_spellclick_spells` WHERE npc_entry=79523;
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, cast_flags, user_type) VALUES(79523, 160655, 1, 0);

UPDATE `creature_template` SET `KillCredit1` = '88822' WHERE `entry` = '79514';

UPDATE creature_template_addon SET auras = "33356" WHERE entry = 79523;