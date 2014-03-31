
DELETE FROM `creature` WHERE `map`=547 AND `id` IN (25865, 25755, 25756, 25757, 25961, 25985);
DELETE FROM `gameobject` WHERE `map`=547 AND `id` IN (187882, 187892, 188192);
-- DELETE FROM `gameobject` WHERE `map`=547 AND `id`=195000 AND `guid`=198727;
DELETE FROM `item_loot_template` WHERE `entry`=54536 AND `item`=54806;

UPDATE `creature_template` SET `unit_flags`=64 WHERE `entry` IN (18206, 19901);
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=25755;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=25756;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=25757;
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=25757;
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=25756;
UPDATE `creature_template` SET `ScriptName`='' WHERE `entry`=25755;
UPDATE `creature_template` SET `mechanic_immune_mask`=650854239 WHERE `entry`=25865;
UPDATE `creature_template` SET `dynamicflags`=8 WHERE `entry` IN 
(25865, 26339, 25755, 26342, 25756, 26340, 25757, 26341, 25952, 25961, 25985, 26337);
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=25865;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=26339;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry`=25740;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=25755;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=25756;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=25757;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=25865;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=26339;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=26340;
UPDATE `creature_template` SET `minlevel`=85 WHERE `entry`=26341;
UPDATE `creature_template` SET `minlevel`=85, `maxlevel`=85 WHERE `entry`=26342;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=25961;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=25952;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=26339;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=26340;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=26341;
UPDATE `creature_template` SET `exp`=3 WHERE `entry`=26342;
UPDATE `creature_template` SET `maxlevel`=85 WHERE `entry`=26341;
UPDATE `creature_template` SET `Health_mod`=25 WHERE `entry`=26338;
UPDATE `creature_template` SET `Health_mod`=25 WHERE `entry`=26339;
UPDATE `creature_template` SET `Health_mod`=0.75 WHERE `entry`=26340;
UPDATE `creature_template` SET `Health_mod`=0.75 WHERE `entry`=26341;
UPDATE `creature_template` SET `Health_mod`=5 WHERE `entry`=26342;
UPDATE `creature_template` SET `mechanic_immune_mask`=650854239 WHERE `entry`=26339;
UPDATE `creature_template` SET `mechanic_immune_mask`=650854239 WHERE `entry`=26338;
UPDATE `creature_template` SET `mechanic_immune_mask`=650854239 WHERE `entry`=25740;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=187892;
UPDATE `gameobject_template` SET `faction`=0, `flags`=16 WHERE `entry`=188077;

UPDATE `creature_template` SET `ScriptName`='npc_frostlord_ahune' WHERE `entry`=25740;
UPDATE `creature_template` SET `ScriptName`='npc_ahune_ice_spear' WHERE `entry`=25985;
UPDATE `creature_template` SET `ScriptName`='npc_ahune_frozen_core' WHERE `entry`=25865;
UPDATE `gameobject_template` SET `ScriptName`='go_ahune_ice_stone' WHERE `entry`=187882;

DELETE FROM `smart_scripts` WHERE `entryorguid` IN (25755, 25756, 25757);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES 
(25755, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 75, 46542, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahunite Hailstone - Add Chilling Aura on Reset'),
(25755, 0, 1, 0, 0, 0, 100, 0, 2500, 5000, 6000, 7000, 11, 2676, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahunite Hailstone - Cast Pulverize'),
(25756, 0, 0, 0, 0, 0, 100, 0, 1000, 5000, 4000, 5000, 11, 46406, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Ahunite Coldwave - Cast Bitter Blast'),
(25757, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 75, 51620, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ahunite Frostwind - Add Lightning Shield on Reset'),
(25757, 0, 1, 0, 0, 0, 100, 0, 5000, 5000, 7000, 7000, 11, 46568, 0, 0, 0, 0, 0, 17, 8, 25, 0, 0, 0, 0, 0, 'Ahunite Frostwind - Cast Wind Buffet');

DELETE FROM `creature_text` WHERE `entry`=25697;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(25697, 0, 0, 'The Ice Stone has melted!', 12, 0, 100, 0, 0, 0, 'Ahune aggro');

DELETE FROM `locales_creature_text` WHERE `entry`=25697;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(25697, 0, 0, 'Камень Льда растаял!');

DELETE FROM `creature_text` WHERE `entry`=25740;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(25740, 0, 0, 'Ahune Retreats. His Defenses Diminish.', 41, 0, 100, 0, 0, 0, 'Ahune submerge'),
(25740, 1, 0, 'Ahune will soon resurface.', 41, 0, 100, 0, 0, 0, 'Ahune emerge soon');

DELETE FROM `locales_creature_text` WHERE `entry`=25740;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(25740, 0, 0, 'Ахун отступает. Его оборона слабеет.'),
(25740, 1, 0, 'Ахун скоро вернется.');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (45390, 45671, 45819, 45907);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(45390, 'spell_gen_ribbon_pole_dancer_check'),
(45671, 'spell_gen_juggle_torch_catch'),
(45819, 'spell_gen_throw_torch'),
(45907, 'spell_gen_torch_target_picker');

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(29531, 45390, 0, 'Ribbon Pole - Dancer Check Aura'),
(45390, 45406, 2, 'Ribbon Pole - Periodic Visual'),
(45723, 43313, 2, 'Torch Tossing - Target Indicator Visual'),
(46747, 45907, 0, 'Fling Torch - Torch Target Picker'),
(45792, 46120, 0, 'Torch Toss (Shadow), Slow'),
(45806, 46118, 0, 'Torch Toss (Shadow), Medium'),
(45816, 46117, 0, 'Torch Toss (Shadow), Fast');

/* REPLACE INTO `spell_dbc` (`Id`, `Dispel`, `Mechanic`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`, `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `Stances`, `StancesNot`, `Targets`, `CastingTimeIndex`, `AuraInterruptFlags`, `ProcFlags`, `ProcChance`, `ProcCharges`, `MaxLevel`, `BaseLevel`, `SpellLevel`, `DurationIndex`, `RangeIndex`, `StackAmount`, `EquippedItemClass`, `EquippedItemSubClassMask`, `EquippedItemInventoryTypeMask`, `Effect1`, `Effect2`, `Effect3`, `EffectDieSides1`, `EffectDieSides2`, `EffectDieSides3`, `EffectRealPointsPerLevel1`, `EffectRealPointsPerLevel2`, `EffectRealPointsPerLevel3`, `EffectBasePoints1`, `EffectBasePoints2`, `EffectBasePoints3`, `EffectMechanic1`, `EffectMechanic2`, `EffectMechanic3`, `EffectImplicitTargetA1`, `EffectImplicitTargetA2`, `EffectImplicitTargetA3`, `EffectImplicitTargetB1`, `EffectImplicitTargetB2`, `EffectImplicitTargetB3`, `EffectRadiusIndex1`, `EffectRadiusIndex2`, `EffectRadiusIndex3`, `EffectApplyAuraName1`, `EffectApplyAuraName2`, `EffectApplyAuraName3`, `EffectAmplitude1`, `EffectAmplitude2`, `EffectAmplitude3`, `EffectMultipleValue1`, `EffectMultipleValue2`, `EffectMultipleValue3`, `EffectMiscValue1`, `EffectMiscValue2`, `EffectMiscValue3`, `EffectMiscValueB1`, `EffectMiscValueB2`, `EffectMiscValueB3`, `EffectTriggerSpell1`, `EffectTriggerSpell2`, `EffectTriggerSpell3`, `EffectSpellClassMaskA1`, `EffectSpellClassMaskA2`, `EffectSpellClassMaskA3`, `EffectSpellClassMaskB1`, `EffectSpellClassMaskB2`, `EffectSpellClassMaskB3`, `EffectSpellClassMaskC1`, `EffectSpellClassMaskC2`, `EffectSpellClassMaskC3`, `MaxTargetLevel`, `SpellFamilyName`, `SpellFamilyFlags1`, `SpellFamilyFlags2`, `SpellFamilyFlags3`, `MaxAffectedTargets`, `DmgClass`, `PreventionType`, `DmgMultiplier1`, `DmgMultiplier2`, `DmgMultiplier3`, `AreaGroupId`, `SchoolMask`, `Comment`) VALUES 
(29710, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 101, 0, 0, 0, 0, 0, 1, 0, -1, 0, 0, 140, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 25, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 29531, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 'Ribbon Pole - Force Cast Ribbon Pole Channel'); */