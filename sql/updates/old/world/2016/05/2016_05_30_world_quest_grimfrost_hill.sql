-- Quest 33145 - Bonus Objective: Grimfrost Hill
UPDATE `creature_template` SET ScriptName="npc_frostfire_ridge_captured_frost_wolf" WHERE entry=73284;
UPDATE `creature_template` SET npcflag= `npcflag` | 0x01000000 WHERE entry= 73284;
UPDATE `creature` SET npcflag= `npcflag` | 0x01000000 WHERE id= 73284;