-- Remove Imp sript
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 416;
-- Remove Searing totem script
UPDATE `creature_template` SET `ScriptName` = '' WHERE `ScriptName` = 'spell_npc_searing_totem';
-- Rename Frozen Orb script
UPDATE `creature_template` SET `ScriptName` = 'spell_npc_mage_frozen_orb' WHERE `ScriptName` = 'npc_frozen_orb';
