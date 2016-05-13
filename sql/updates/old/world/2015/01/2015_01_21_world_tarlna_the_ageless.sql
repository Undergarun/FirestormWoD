DELETE FROM creature WHERE id = 81535;
INSERT INTO creature (id, map, zoneId, areaId, spawnMask, phaseMask, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs) VALUE
(81535, 1116, 6721, 6914, 1, 1, 0, 4872.012, 1226.342, 121.3324, 3.470886, 7200);

UPDATE creature_template SET unit_flags = unit_flags & ~0x200000, dmg_multiplier = 15, TrackingQuestID = 37462, ScriptName = 'boss_tarlna_the_ageless', mechanic_immune_mask = 617299839 WHERE entry = 81535;
UPDATE creature_template SET minlevel = 102, maxlevel = 102, dmg_multiplier = 8, faction = 2741, ScriptName = 'npc_untamed_mandragora' WHERE entry = 88150;
UPDATE creature_template SET minlevel = 100, maxlevel = 100, dmg_multiplier = 8, faction = 2741, ScriptName = 'npc_giant_lasher' WHERE entry = 88142;
REPLACE INTO creature_model_info VALUE (57232, 3.1, 30, 0, 0);
REPLACE INTO creature_model_info VALUE (55767, 3, 15, 0, 0);

DELETE FROM trinity_string WHERE entry = 14063;
INSERT INTO trinity_string (entry, content_default, content_loc2, content_loc6) VALUE
(14063, '|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Tarlna begins to channel |cFFFF0404|Hspell:175979|h[Genesis]|h|r.',
	'|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Tarlna commence à canaliser |cFFFF0404|Hspell:175979|h[Genèse]|h|r.',
	'|TInterface\\Icons\\Ability_Druid_EmpoweredTouch:20|t Tarlna comienza a canalizar |cFFFF0404|Hspell:175979|h[Génesis]|h|r.');
	
DELETE FROM conditions WHERE SourceEntry IN (175978);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2) VALUES
(13, 1, 175978, 31, 0, 3, 88142);

DELETE FROM areatrigger_template WHERE spell_id = 176035;
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUE
(176035, 0, 8054, 1, 1, 16384, 'areatrigger_tarlna_noxious_spit');

UPDATE creature_template SET creature_template.`lootid` = 81535 WHERE entry = 81535;

DELETE FROM reference_loot_template WHERE entry = 81535;
INSERT INTO reference_loot_template VALUES
(81535, 115428, 0, 1, 1, 1, 1, ''), -- Gauntlets of Impenetrability
(81535, 115426, 0, 1, 1, 1, 1, ''), -- Grips of Natural Fury
(81535, 120091, 0, 1, 1, 1, 1, ''), -- Robes of the Ageless
(81535, 120088, 0, 1, 1, 1, 1, ''), -- Witherleaf Chestguard
(81535, 115429, 0, 1, 1, 1, 1, ''), -- Belt of Singing Hooves
(81535, 120090, 0, 1, 1, 1, 1, ''), -- Falling Leaf Breastplate
(81535, 115430, 0, 1, 1, 1, 1, ''), -- Cord of Ruination
(81535, 115432, 0, 1, 1, 1, 1, ''), -- Waistplate of Bladed Force
(81535, 115427, 0, 1, 1, 1, 1, ''), -- Chainhoof Grips
(81535, 115425, 0, 1, 1, 1, 1, ''), -- Quadripedal Grips
(81535, 115431, 0, 1, 1, 1, 1, ''), -- Chain of Natural Fury
(81535, 120089, 0, 1, 1, 1, 1, ''); -- Chestguard of Rejuvenation

DELETE FROM creature_loot_template WHERE entry = 81535;
INSERT INTO creature_loot_template VALUES
(81535, 1, 100, 1, 0, -81535, 4, '');