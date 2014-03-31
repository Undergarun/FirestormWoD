DELETE FROM `creature` WHERE `map`=720 AND `id`=52659;

UPDATE `creature` SET `spawnmask`=15 WHERE `id`=52558 AND `map`=720;

REPLACE INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES (52558, 98843, 1);

REPLACE INTO `vehicle_template_accessory` (`entry`, `accessory_entry`, `seat_id`, `description`, `summontype`, `summontimer`) VALUES 
(52558, 53087, 1, 'Rhyolith right foot', 8, 0),
(52558, 52577, 0, 'Rhyolith left foot', 8, 0);

UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=52659;
UPDATE `creature_template` SET `npcflag`=16777216, `unit_flags`=0, `dynamicflags`=0, `type_flags`=8 WHERE `entry` IN (52558, 52559, 52560, 52561);
UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=0, `type_flags`=8 WHERE `entry`=53772;
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|0x20000000 WHERE `entry` IN 
(52558, 52559, 52560, 52561, 52577, 53358, 53719, 53720, 53087, 53359, 53721, 53722);
UPDATE `creature_template` SET `modelid1`=38415, `modelid2`=0 WHERE `entry` IN (52577, 53358, 53719, 53720);
UPDATE `creature_template` SET `modelid1`=38416, `modelid2`=0 WHERE `entry` IN (53087, 53359, 53721, 53722);
UPDATE `creature_template` SET `modelid1`=38054, `modelid2`=0 WHERE `entry`=52582;
UPDATE `creature_template` SET `modelid1`=38063, `modelid2`=0 WHERE `entry`=52866;
UPDATE `creature_template` SET `faction_A`=14, `faction_H`=14, `minlevel`=87, `maxlevel`=87 WHERE `entry` IN (52866, 52582);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN 
(52577, 53358, 53719, 53720, 53087, 53359, 53721, 53722, 53772, 53773, 53774, 53775, 52558, 52559, 52560, 52561);
UPDATE `creature_template` SET `Health_mod`=152.517 WHERE `entry`=52577;
UPDATE `creature_template` SET `Health_mod`=152.517 WHERE `entry`=53087;
UPDATE `creature_template` SET `Health_mod`=465.701 WHERE `entry`=53358;
UPDATE `creature_template` SET `Health_mod`=465.701 WHERE `entry`=53359;
UPDATE `creature_template` SET `Health_mod`=232.851 WHERE `entry`=53719;
UPDATE `creature_template` SET `Health_mod`=232.851 WHERE `entry`=53721;
UPDATE `creature_template` SET `Health_mod`=698.552 WHERE `entry`=53720;
UPDATE `creature_template` SET `Health_mod`=698.552 WHERE `entry`=53722;
UPDATE `creature_model_info` SET `bounding_radius`=7, `combat_reach`=7 WHERE `modelid`=38414;
UPDATE `creature_model_info` SET `bounding_radius`=7, `combat_reach`=7 WHERE `modelid`=38594;

UPDATE `creature_template` SET `ScriptName`='boss_lord_rhyolith' WHERE `entry`=52558;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_left_foot' WHERE `entry`=52577;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_right_foot' WHERE `entry`=53087;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_volcano' WHERE `entry`=52582;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_crater' WHERE `entry`=52866;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_spark_of_rhyolith' WHERE `entry`=53211;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_fragment_of_rhyolith' WHERE `entry`=52620;
UPDATE `creature_template` SET `ScriptName`='npc_lord_rhyolith_liquid_obsidian' WHERE `entry`=52619;

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(97282, 'spell_lord_rhyolith_conclusive_stomp'),
(100411, 'spell_lord_rhyolith_conclusive_stomp'),
(100968, 'spell_lord_rhyolith_conclusive_stomp'),
(100969, 'spell_lord_rhyolith_conclusive_stomp'),
(97225, 'spell_lord_rhyolith_magma_flow'),
(99875, 'spell_lord_rhyolith_fuse'),
(98034, 'spell_lord_rhyolith_drink_magma'),
(98276, 'spell_lord_rhyolith_lava_strike');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (17450, 18097, 17446, 17447);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(17450, 11, 0, 0, 'achievement_not_an_ambi_turner');

DELETE FROM `creature_text` WHERE `entry`=52558;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(52558, 0, 0, 'Hah? Hruumph? Soft little fleshy-things? Here? Nuisances, nuisances!', 14, 0, 100, 0, 0, 24537, 'Rhyolith aggro'),
(52558, 1, 0, 'Graaahh!', 14, 0, 100, 0, 0, 24540, 'Rhyolith chunk 01'),
(52558, 1, 1, 'Graaahh', 14, 0, 100, 0, 0, 24541, 'Rhyolith chunk 02'),
(52558, 1, 2, 'Augh - smooshy little pests, look what you\'ve done!', 14, 0, 100, 0, 0, 24542, 'Rhyolith chunk 03'),
(52558, 1, 3, 'Uurrghh now you... you infuriate me!', 14, 0, 100, 0, 0, 24543, 'Rhyolith chunk 04'),
(52558, 1, 4, 'Oh you little beasts...', 14, 0, 100, 0, 0, 24544, 'Rhyolith chunk 05'),
(52558, 2, 0, 'Broken. Mnngghhh... broken...', 14, 0, 100, 0, 0, 24545, 'Rhyolith death'),
(52558, 3, 0, 'Finished.', 14, 0, 100, 0, 0, 24546, 'Rhyolith kill 01'),
(52558, 3, 1, 'So soft!', 14, 0, 100, 0, 0, 24547, 'Rhyolith kill 02'),
(52558, 3, 2, 'Squeak, little pest.', 14, 0, 100, 0, 0, 24548, 'Rhyolith kill 03'),
(52558, 4, 0, 'Buuurrrnn!', 14, 0, 100, 0, 0, 24550, 'Rhyolith lava 01'),
(52558, 4, 1, 'Succumb to living flame.', 14, 0, 100, 0, 0, 24551, 'Rhyolith lava 02'),
(52558, 4, 2, 'My inner fire can never die!', 14, 0, 100, 0, 0, 24552, 'Rhyolith lava 03'),
(52558, 4, 3, 'Consuuuuuuume!', 14, 0, 100, 0, 0, 24553, 'Rhyolith lava 04'),
(52558, 4, 4, 'Flesh, buuurrrns.', 14, 0, 100, 0, 0, 24554, 'Rhyolith lava 05'),
(52558, 5, 0, 'I\'ll crush you underfoot!', 14, 0, 100, 0, 0, 24556, 'Rhyolith stomp 1'),
(52558, 5, 1, 'Stomp now.', 14, 0, 100, 0, 0, 24557, 'Rhyolith stomp 02'),
(52558, 6, 0, 'Eons I have slept undisturbed... Now this... Creatures of flesh, now you will BURN!', 14, 0, 100, 0, 0, 24558, 'Rhyolith transform');

DELETE FROM `locales_creature_text` WHERE `entry`=52558;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(52558, 0, 0, 'А? Что? Мягкие живые штучки... Здесь? Надоели. Надоели!'),
(52558, 1, 0, 'Граааа!'),
(52558, 1, 1, 'Граааа!'),
(52558, 1, 2, 'Ах! Маленькие гадкие создания, посмотрите, что вы наделали!'),
(52558, 1, 3, 'Вы... вы меня раздражаете!'),
(52558, 1, 4, 'Ах вы маленькие чудовища...'),
(52558, 2, 0, 'Сломан. Аааа... я сломан...'),
(52558, 3, 0, 'Вот и все.'),
(52558, 3, 1, 'Хорошо ломается.'),
(52558, 3, 2, 'Пищи, насекомое.'),
(52558, 4, 0, 'Сгорите!'),
(52558, 4, 1, 'Покоритесь живому пламени.'),
(52558, 4, 2, 'Огонь внутри меня никогда не погаснет!'),
(52558, 4, 3, 'Поглотиииить!'),
(52558, 4, 4, 'Плоть... Гориииии!'),
(52558, 5, 0, 'Растопчу.'),
(52558, 5, 1, 'Я раздавлю вас!'),
(52558, 6, 0, 'Целую вечность я спокойно спал... Маленькие кусочки мяса... Вы СГОРИТЕ!');

DELETE FROM `creature_text` WHERE `entry`=53772;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53772, 2, 0, 'Broken. Mnngghhh... broken...', 14, 0, 100, 0, 0, 24545, 'Rhyolith death'),
(53772, 3, 0, 'Finished.', 14, 0, 100, 0, 0, 24546, 'Rhyolith kill 01'),
(53772, 3, 1, 'So soft!', 14, 0, 100, 0, 0, 24547, 'Rhyolith kill 02'),
(53772, 3, 2, 'Squeak, little pest.', 14, 0, 100, 0, 0, 24548, 'Rhyolith kill 03'),
(53772, 5, 0, 'I\'ll crush you underfoot!', 14, 0, 100, 0, 0, 24556, 'Rhyolith stomp 1'),
(53772, 5, 1, 'Stomp now.', 14, 0, 100, 0, 0, 24557, 'Rhyolith stomp 02');

DELETE FROM `locales_creature_text` WHERE `entry`=53772;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53772, 2, 0, 'Сломан. Аааа... я сломан...'),
(53772, 3, 0, 'Вот и все.'),
(53772, 3, 1, 'Хорошо ломается.'),
(53772, 3, 2, 'Пищи, насекомое.'),
(53772, 5, 0, 'Растопчу.'),
(53772, 5, 1, 'Я раздавлю вас!');

UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (52558, 52559, 525560, 52561, 53772, 53773, 53774, 53775);