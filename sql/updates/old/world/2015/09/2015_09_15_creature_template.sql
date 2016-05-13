UPDATE `creature_template` SET `ScriptName` = '' WHERE `ScriptName` = 'npc_doomguard';
UPDATE `creature_template` SET `ScriptName` = 'npc_doomguard' WHERE `entry` = 78158;

UPDATE `pet_stats` SET `entry` = 78158 WHERE `entry` = 11859;
