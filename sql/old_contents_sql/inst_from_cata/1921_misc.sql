
UPDATE `creature` SET `spawnmask`=0 WHERE `map`=967 AND `id` IN 
(55969, 55967, 56141, 56143,
56142, 56144, 57474, 55913,
56139, 55911, 55912, 55914,
57746, 57749, 57751, 56140,
56140, 57473, 57475, 56250, 
56252, 32639, 55971, 56251,
57795, 57259,  56249, 58153);

UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN 
(55265, 57409, 57771, 57772,
56427, 57699, 57847, 57848,
55294, 56576, 56577, 56578,
55689, 57462, 57955, 57956,
55312, 55313, 55314, 55315,
55308, 55309, 55310, 55311);

UPDATE `creature` SET `spawnmask`=0 WHERE `map`=967 AND `id` IN 
(55294, 55312, 55308, 55689, 56427);

UPDATE `creature_template_addon` SET `auras`='103494' WHERE `entry`=55346;
-- UPDATE `creature_addon` SET `auras`='109247' WHERE `guid`=343391;
-- UPDATE `creature_addon` SET `auras`='109247' WHERE `guid`=343384;
-- UPDATE `creature_addon` SET `auras`='109247' WHERE `guid`=343301;
-- UPDATE `creature_addon` SET `auras`='109247' WHERE `guid`=343403;

UPDATE `creature_template` SET `speed_walk`=1 WHERE `entry`=58226;
UPDATE `creature_template` SET `speed_walk`=1 WHERE `entry`=55265;
UPDATE `creature_template` SET `speed_run`=1.14286 WHERE `entry`=55265;
UPDATE `creature_template` SET `speed_run`=1.14286 WHERE `entry`=58226;
UPDATE `creature_template` SET `scale`=1 WHERE `entry`=55265;
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0, `scale`=1 WHERE `entry`=55346;
UPDATE `creature_template` SET `speed_run`=2 WHERE `entry`=55265;
UPDATE `creature_template` SET `speed_run`=2 WHERE `entry`=57773;
UPDATE `creature_template` SET `speed_run`=2 WHERE `entry`=57774;
UPDATE `creature_template` SET `speed_run`=2 WHERE `entry`=57995;
UPDATE `creature_template` SET `speed_run`=2 WHERE `entry`=57996;
UPDATE `creature_template` SET `scale`=1 WHERE `entry`=57772;
UPDATE `creature_template` SET `scale`=1 WHERE `entry`=57771;
UPDATE `creature_template` SET `scale`=1 WHERE `entry`=57409;
UPDATE `creature_template` SET `speed_walk`=1 WHERE `entry`=57772;
UPDATE `creature_template` SET `speed_walk`=1 WHERE `entry`=57771;
UPDATE `creature_template` SET `speed_walk`=1 WHERE `entry`=57409;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=54969;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (55265, 57409, 57771, 57772, 57773, 57774, 57995, 57996);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=97 WHERE `entry` IN (55265, 57773, 57409, 57774);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=110 WHERE `entry` IN (57771, 57772, 57995, 57996);

-- UPDATE `creature` SET `MovementType`=0 WHERE `guid` IN 
-- (343408, 343447, 343448, 343445,
-- 343410, 343412, 343413, 343415,
-- 343418, 343420, 343422, 343424,
-- 343425, 343427, 343430, 343432,
-- 343436, 343441, 343755, 343757,
-- 343409, 343411, 343414, 343416,
-- 343417, 343419, 343423, 343426,
-- 343428, 343429, 343431, 343433);
-- UPDATE `creature_addon` SET `path_id`=0 WHERE `guid` IN 
-- (343408, 343447, 343448, 343445,
-- 343410, 343412, 343413, 343415,
-- 343418, 343420, 343422, 343424,
-- 343425, 343427, 343430, 343432,
-- 343436, 343441, 343755, 343757,
-- 343409, 343411, 343414, 343416,
-- 343417, 343419, 343423, 343426,
-- 343428, 343429, 343431, 343433);
-- DELETE FROM `waypoint_data` WHERE `id` IN 
-- (343408, 343447, 343448, 343445,
-- 343410, 343412, 343413, 343415,
-- 343418, 343420, 343422, 343424,
-- 343425, 343427, 343430, 343432,
-- 343436, 343441, 343755, 343757,
-- 343409, 343411, 343414, 343416,
-- 343417, 343419, 343423, 343426,
-- 343428, 343429, 343431, 343433);

UPDATE `access_requirement` SET `level_min`=85 WHERE `mapId`=967;
UPDATE `instance_template` SET `script`='instance_dragon_soul' WHERE `map`=967;

UPDATE `creature_template` SET `ScriptName`='boss_morchok' WHERE `entry`=55265;
UPDATE `creature_template` SET `ScriptName`='npc_morchok_kohcrom' WHERE `entry`=57773;
UPDATE `creature_template` SET `ScriptName`='npc_morchok_resonating_crystal' WHERE `entry`=55346;

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(103534, -103536, 1, 'Morchok - Danger'),
(103534, -103541, 1, 'Morchok - Danger'),
(103536, -103534, 1, 'Morchok - Warning'),
(103536, -103541, 1, 'Morchok - Warning'),
(103541, -103534, 1, 'Morchok - Safe'),
(103541, -103536, 1, 'Morchok - Safe');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(103851, 103785, 108570, 110288,
110287, 103414, 108571, 109033, 
109034, 103528, 104573, 103545,
108572, 110041, 110040);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(103851, 'spell_morchok_black_blood_of_the_earth'),
(103785, 'spell_morchok_black_blood_of_the_earth_dmg'),
(108570, 'spell_morchok_black_blood_of_the_earth_dmg'),
(110288, 'spell_morchok_black_blood_of_the_earth_dmg'),
(110287, 'spell_morchok_black_blood_of_the_earth_dmg'),
(103414, 'spell_morchok_stomp'),
(108571, 'spell_morchok_stomp'),
(109033, 'spell_morchok_stomp'),
(109034, 'spell_morchok_stomp'),
(103528, 'spell_morchok_target_selected'),
(104573, 'spell_morchok_target_selected'),
(103545, 'spell_morchok_resonating_crystal_dmg'),
(108572, 'spell_morchok_resonating_crystal_dmg'),
(110041, 'spell_morchok_resonating_crystal_dmg'),
(110040, 'spell_morchok_resonating_crystal_dmg');

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(55265, 396, 614, 0, 12000, 1, 0),
(57409, 396, 614, 0, 14000, 1, 0),
(57771, 396, 614, 0, 12000, 1, 0),
(57772, 396, 614, 0, 14000, 1, 0);

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN 
(18454, 18455, 18607, 18480, 18445);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(18607, 11, 0, 0, 'achievement_dont_stay_so_close_to_me');

DELETE FROM `creature_text` WHERE `entry`=55265;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55265, 0, 0, 'You seek to halt an avalanche. I will bury you.', 14, 0, 100, 0, 0, 26268, 'morchok aggro'),
(55265, 1, 0, 'Impossible. This cannot be. The tower...must...fall.', 14, 0, 100, 0, 0, 26269, 'morchok death'),
(55265, 2, 0, 'Cowards! Weaklings! Come down and fight or I will bring you down!', 14, 0, 100, 0, 0, 26270, 'morchok event 1'),
(55265, 3, 0, 'You cannot hide in this temple forever, shaman! Wyrmrest will fall. All will be dust.', 14, 0, 100, 0, 0, 26271, 'morchok event 2'),
(55265, 4, 0, 'I will turn this tower to rubble and scatter it across the wastes.', 14, 0, 100, 0, 0, 26272, 'morchok event 3'),
(55265, 5, 0, ' Wyrmrest will fall. All will be dust.', 14, 0, 100, 0, 0, 26273, 'morchok event 4'),
(55265, 6, 0, 'The stone calls...', 14, 0, 100, 0, 0, 26274, 'morchok ground 1 1'),
(55265, 6, 1, 'The ground shakes...', 14, 0, 100, 0, 0, 26275, 'morchok ground 1 2'),
(55265, 6, 2, 'The rocks tremble...', 14, 0, 100, 0, 0, 26276, 'morchok ground 1 3'),
(55265, 6, 3, 'The surface quakes...', 14, 0, 100, 0, 0, 26277, 'morchok ground 1 4'),
(55265, 7, 0, '...and the black blood of the earth consumes you.', 14, 0, 100, 0, 0, 26278, 'morchok ground 2 1'),
(55265, 7, 1, '...and there is no escape from the old gods.', 14, 0, 100, 0, 0, 26279, 'morchok ground 2 2'),
(55265, 7, 2, '...and the rage of the true gods follows.', 14, 0, 100, 0, 0, 26280, 'morchok ground 2 3'),
(55265, 7, 3, '...and you drown in the hate of The Master.', 14, 0, 100, 0, 0, 26281, 'morchok ground 2 4'),
(55265, 8, 0, 'No mortal shall turn me from my task!', 14, 0, 100, 0, 0, 26282, 'morchok intro'),
(55265, 9, 0, 'Flee and die!', 14, 0, 100, 0, 0, 26283, 'morchok orb 1'),
(55265, 9, 1, 'Run, and perish.', 14, 0, 100, 0, 0, 26284, 'morchok orb 2'),
(55265, 10, 0, 'I am unstoppable.', 14, 0, 100, 0, 0, 26285, 'morchok slay 1'),
(55265, 10, 1, 'It was inevitable.', 14, 0, 100, 0, 0, 26286, 'morchok slay 2'),
(55265, 10, 2, 'Ground to dust.', 14, 0, 100, 0, 0, 26287, 'morchok slay 3'),
(55265, 11, 0, 'You thought to fight me alone? The earth splits to swallow and crush you.', 14, 0, 100, 0, 0, 26288, 'morchok split');

DELETE FROM `locales_creature_text` WHERE `entry`=55265;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55265, 0, 0, 'Попробуйте остановить лавину и умрете.'),
(55265, 1, 0, 'Этого не может быть. Башня... должна быть... разрушена...'),
(55265, 2, 0, 'Трусы. Слабаки. Спускайтесь вниз и сражайтесь.'),
(55265, 3, 0, 'Ты не сможешь прятаться в этом храме вечно, шаман!'),
(55265, 4, 0, 'Я превращу эту башню в пыль и развею ее по пустошам.'),
(55265, 5, 0, 'Башня должна быть разрушена. Все обратится в пыль.'),
(55265, 6, 0, 'Земля взывает...'),
(55265, 6, 1, 'Земля содрогается...'),
(55265, 6, 2, 'Камни дрожат...'),
(55265, 6, 3, 'Земля трясется...'),
(55265, 7, 0, '...и черная кровь пожирает вас.'),
(55265, 7, 1, '...и нет спасения от гнева древних богов.'),
(55265, 7, 2, '...и вас настигает ярость истинных богов.'),
(55265, 7, 3, '...и вы тонете в море ненависти Повелителя.'),
(55265, 8, 0, 'Ни один смертный не сможет мне помешать.'),
(55265, 9, 0, 'Бегите, и вы умрете.'),
(55265, 9, 1, 'Бегите, и вам конец.'),
(55265, 10, 0, 'Вы меня не остановите.'),
(55265, 10, 1, 'Это было неизбежно.'),
(55265, 10, 2, 'Прах к праху.'),
(55265, 11, 0, 'Вы думали, что я буду сражаться один? Земля разделится и поглотит вас.');