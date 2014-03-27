DELETE FROM creature WHERE id IN (60999, 62442);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES('60999','996','6067','6515','8','65535','0','0','-1017.58','-2771.66','38.5722','4.73273','300','0','0','0','0','0','0','0','0','0','0');
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES('62442','996','6067','6515','8','65535','0','0','-1017.95','-2807.68','38.2767','1.53812','300','0','0','0','0','0','0','0','0','0','0');

DELETE FROM `spell_script_names` WHERE spell_id IN(122768, 125843, 122789);
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (125843, 'spell_dread_shadows_damage');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122768, 'spell_dread_shadows_malus');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122789, 'spell_sunbeam');

UPDATE creature_template SET scriptname = "npc_sunbeam" WHERE entry = 62849;

-- The Dark of Night
UPDATE creature_template SET modelid1 = 42973, modelid2 = 42973, ScriptName = "npc_dark_of_night" WHERE entry = 63346;
-- Tsulong
UPDATE creature_template SET unit_class = 4, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 4, ScriptName = "boss_tsulong" WHERE entry = 62442;
-- Embodied Terror

UPDATE creature_template SET faction_H = 16, faction_A = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.5, ScriptName = "npc_embodied_terror" WHERE entry = 62969;

INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122855, 'spell_sun_breath');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (123018, 'spell_terrorize_player');

-- Tiny Terror
UPDATE creature_template SET faction_H = 16, faction_A = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.1, ScriptName = "npc_tiny_terror" WHERE entry = 62977;

-- Unstable sha
UPDATE creature_template SET faction_A = 16, faction_H = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.25, ScriptName ="npc_unstable_sha" WHERE entry = 62919;