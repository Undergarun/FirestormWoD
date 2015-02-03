UPDATE creature_template SET scriptname = "boss_forgemaster_gogduh" WHERE entry = 74366;
UPDATE creature_template SET scriptname = "npc_ruination" WHERE entry = 74570;
UPDATE creature_template SET scriptname = "npc_calamity" WHERE entry = 74571;
UPDATE creature_template SET scriptname = "npc_calamity_firestorm" WHERE entry = 71971;
UPDATE creature_template SET scriptname = "npc_gugdoh_molten_elemental" WHERE entry = 74579;
UPDATE creature_template SET scriptname = "boss_magmolatus" WHERE entry = 74475;

DELETE FROM spell_script_names WHERE ScriptName IN 
(
"spell_rough_smash",
"spell_shatter_earth",
"spell_dancing_flames",
"spell_withering_flames",
"spell_molten_impact",
"spell_magma_barrage",
"spell_magma_barrage_damage",
"spell_magma_barrage_trigger"
);

INSERT INTO spell_script_names VALUES
(149941, "spell_rough_smash"),
(150324, "spell_shatter_earth"),
(149975, "spell_dancing_flames"),
(150032, "spell_withering_flames"),
(150045, "spell_molten_impact"),
(150004, "spell_magma_barrage"),
(150007, "spell_magma_barrage_trigger"),
(150306, "spell_magma_barrage_trigger"),
(150011, "spell_magma_barrage_damage");

DELETE FROM areatrigger_template WHERE spell_id IN (149970, 149969, 149968, 149959);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, ScriptName) VALUES(149959, 0, 5555, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_shatter_earth") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, ScriptName) VALUES(149968, 0, 5555, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_shatter_earth") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, ScriptName) VALUES(149969, 0, 5555, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_shatter_earth") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, ScriptName) VALUES(149970, 0, 5555, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_shatter_earth") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);

UPDATE creature_template SET unit_flags = (0x00000002 + 0x00000004 + 0x02000000 + 0x00020000), faction = 14, ScriptName = "npc_shatter_earth", modelid1 = 11686 WHERE entry = 74766;
UPDATE creature_template SET unit_flags = (0x00000002 + 0x00000004 + 0x02000000 + 0x00020000), faction = 14, modelid1 = 11686 WHERE entry = 71971;

DELETE FROM areatrigger_template WHERE spell_id IN (150050, 150051, 150052, 150053);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, scriptName) VALUES(150050, 0, 5559, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_volcanic_trantum") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, scriptName) VALUES(150051, 0, 5559, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_volcanic_trantum") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, scriptName) VALUES(150052, 0, 5559, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_volcanic_trantum") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, scriptName) VALUES(150053, 0, 5559, 1, 1, 16512, 0, 10000, 0, 0, 0, 0, 0, 0, 0, "areatrigger_volcanic_trantum") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);

DELETE FROM areatrigger_template WHERE spell_id IN (167465);
INSERT INTO areatrigger_template (`spell_id`, `eff_index`, `entry`, `scale_x`, `scale_y`, `flags`, `morph_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, ScriptName) VALUES(167465, 0, 7455, 3, 3, 16384, 0, 0, 0, 0, 0, 0, 0, 0, 0, "areatrigger_magma_barrage") ON DUPLICATE KEY UPDATE `spell_id` = VALUES(`spell_id`), `eff_index` = VALUES(`eff_index`), `entry` = VALUES(`entry`), `scale_x` = VALUES(`scale_x`), `scale_y` = VALUES(`scale_y`), `flags` = VALUES(`flags`), `morph_curve_id` = VALUES(`morph_curve_id`), `data0` = VALUES(`data0`), `data1` = VALUES(`data1`), `data2` = VALUES(`data2`), `data3` = VALUES(`data3`), `data4` = VALUES(`data4`), `data5` = VALUES(`data5`), `data6` = VALUES(`data6`), `data7` = VALUES(`data7`), scriptName = VALUES(scriptName);


DELETE FROM creature_text WHERE entry IN (74366, 74475);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(74366, 0, 1, 'Go away! I will fix this!', 14, 0, 100, 0, 0, 42051, 'aggro'),
(74366, 1, 0, 'Uh... stone!', 14, 0, 100, 0, 0, 42055, 'ruination spawn'),
(74366, 1, 1, 'More stone!', 14, 0, 100, 0, 0, 42056, 'ruination spawn'),
(74366, 1, 2, 'Uh... rocks!', 14, 0, 100, 0, 0, 42057, 'ruination spawn'),
(74366, 2, 0, 'Ha, rocks are dumb!', 14, 0, 100, 0, 0, 42053, 'ruination death'),
(74366, 3, 0, 'FIRE!', 14, 0, 100, 0, 0, 42058, 'calamity spawn'),
(74366, 3, 1, 'YES! FIRE!', 14, 0, 100, 0, 0, 42059, 'calamity spawn'),
(74366, 3, 2, 'Uh... Fire?', 14, 0, 100, 0, 0, 42060, 'calamity spawn'),
(74366, 4, 0, 'Hah hah! So weak.', 14, 0, 100, 0, 0, 42054, 'calamity death'),
(74366, 5, 0, 'Augh! Noooooo...', 14, 0, 100, 0, 0, 42052, 'death'),
(74475, 0, 0, 'Yes... the brute weakens...', 14, 0, 100, 0, 0, 41729, 'aggro'),
(74475, 0, 1, 'Kill him! I will be free!', 14, 0, 100, 0, 0, 41730, 'aggro'),
(74475, 1, 0, 'You are but a pebble amongst boulders...', 14, 0, 100, 0, 0, 41725, 'release'),
(74475, 1, 1, 'I am a rockslide!', 14, 0, 100, 0, 0, 41726, 'release'),
(74475, 1, 2, 'I will expunge your existence.', 14, 0, 100, 0, 0, 41727, 'release'),
(74475, 2, 0, 'Pound to dust...', 14, 0, 100, 0, 0, 41737, 'slag smash'),
(74475, 2, 1, 'Smash!', 14, 0, 100, 0, 0, 41738, 'slag smash'),
(74475, 2, 2, 'Crushed by earth!', 14, 0, 100, 0, 0, 41739, 'slag smash'),
(74475, 3, 0, 'Seared by flames...', 14, 0, 100, 0, 0, 41734, 'molten elemental'),
(74475, 3, 1, 'Burn unto nothingness!', 14, 0, 100, 0, 0, 41735, 'molten elemental'),
(74475, 3, 2, 'Servants, to me!', 14, 0, 100, 0, 0, 41736, 'molten elemental'),
(74475, 4, 0, 'Wasted material.', 14, 0, 100, 0, 0, 41731, 'killed unit'),
(74475, 4, 1, 'Imperfections.', 14, 0, 100, 0, 0, 41732, 'killed unit'),
(74475, 4, 2, 'Flesh burns.', 14, 0, 100, 0, 0, 41733, 'killed unit'),
(74475, 5, 0, 'I return to the earth...', 14, 0, 100, 0, 0, 41728, 'death');
