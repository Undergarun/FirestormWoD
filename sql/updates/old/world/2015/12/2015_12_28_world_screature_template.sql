-- fix Doomguard and Terrorguard
UPDATE `creature_template` SET `ScriptName`='spell_npc_warl_doomguard' WHERE `entry` IN (78158, 78215);

-- fix Searing Totem size
UPDATE `creature_template` SET `scale`='3' WHERE `entry`='2523';

-- fix Black ox Statue
UPDATE `creature_template` SET `unit_flags`='131596' WHERE `entry`='61146';