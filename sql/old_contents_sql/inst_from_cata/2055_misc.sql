
-- DELETE FROM `gameobject` WHERE `guid`=191520 AND `id`=210132 AND `map`=967;
DELETE FROM `creature` WHERE `map`=967 AND `id` IN
(56136, 56165, 56108, 55695, 
57867, 57929, 57809, 57817, 
57821, 57823, 57921, 57807, 
57819, 57860, 56104, 56700,
57924);

UPDATE `creature` SET `spawnmask`=15, `position_x`=13587.4, `position_y`=13612.0, `position_z`=122.43, `orientation`=5.93 WHERE `id`=55689 AND `map`=967;

-- UPDATE `creature_addon` SET `auras`=108203 WHERE `guid` IN (343192, 343211, 344315, 344192);

UPDATE `creature_template_addon` SET `auras`='109247' WHERE `entry`=55695;
UPDATE `creature_template_addon` SET `auras`='109247 109318 109295' WHERE `entry`=57860;
UPDATE `creature_template_addon` SET `auras`='109247 109440 109444' WHERE `entry`=57921;
UPDATE `creature_template_addon` SET `auras`='95716' WHERE `entry`=57809;
UPDATE `creature_template_addon` SET `auras`='105265' WHERE `entry`=56104;
UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN 
(57860, 57809, 57929, 57867, 56104);
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0 WHERE `entry`=56108;
UPDATE `creature_template` SET `modelid1`=39335, `modelid2`=40058, `modelid3`=0 WHERE `entry` IN (56136, 57813, 57832, 57833);
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry` IN (57867, 57868);
UPDATE `creature_template` SET `modelid1`=39575, `modelid2`=0 WHERE `entry`=56165;
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry` IN (57287);
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (57287);
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (55689, 57462, 57955, 57956);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822 WHERE `entry` IN 
(55689, 57462, 57955, 57956);
UPDATE `creature_template` SET `dmg_multiplier`=97 WHERE `entry` IN
(55689, 57462);
UPDATE `creature_template` SET `dmg_multiplier`=110 WHERE `entry` IN
(57955, 57956);

UPDATE `creature_template` SET `ScriptName`='boss_hagara_the_stormbinder' WHERE `entry`=55689;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_stormborn_myrmidon' WHERE `entry`=57817;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_stormbinder_adept' WHERE `entry`=57823;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_tornado_stalker' WHERE `entry`=57921;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_twilight_frost_evoker' WHERE `entry`=57807;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_lieutenant_shara' WHERE `entry`=57821;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_icy_tomb' WHERE `entry`=55695;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_ice_lance' WHERE `entry`=56108;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_ice_wave' WHERE `entry`=56104;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_collapsing_icicle' WHERE `entry`=57867;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_frozen_binding_crystal' WHERE `entry`=56136;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_crystal_conductor' WHERE `entry`=56165;
UPDATE `creature_template` SET `ScriptName`='npc_hagara_the_stormbinder_bound_lightning_elemental' WHERE `entry`=56700;
UPDATE `gameobject_template` SET `ScriptName`='go_hagara_the_stormbinder_the_focusing_iris' WHERE `entry`=210132;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_teleport' WHERE `entry` IN (57287, 57377, 57379, 57882);

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(109317, 104448, 104449, 104451, 105287, 105367, 109557);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109317, 'spell_dragon_soul_lieutenant_shara_frozen_grasp'),
(104448, 'spell_hagara_the_stormbinder_icy_tomb_aoe'),
(104449, 'spell_hagara_the_stormbinder_icy_tomb_dummy'),
(104451, 'spell_hagara_the_stormbinder_icy_tomb'),
(105287, 'spell_hagara_the_stormbinder_ice_lance_target'),
(105367, 'spell_hagara_the_stormbinder_crystal_conduit_target'),
(109557, 'spell_hagara_the_stormbinder_storm_pillars');

DELETE FROM `conditions` WHERE `SourceEntry` IN (105312, 105481);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 105312, 0, 31, 0, 3, 55689, 'Hagara the Stormbinder - Crystalline Overload'),
(13, 1, 105481, 0, 31, 0, 3, 56165, 'Hagara the Stormbinder - Overload');

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(105367, 105371, 1, 'Hagara the Stormbinder - Lightning Conduit');

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(55689, 396, 614, 0, 12000, 1, 0),
(57462, 396, 614, 0, 14000, 1, 0),
(57955, 396, 614, 0, 12000, 1, 0),
(57956, 396, 614, 0, 14000, 1, 0);

DELETE FROM `creature_text` WHERE `entry`=55689;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55689, 0, 0, 'Heh! Even with the aspect of time on your side, you stumble foolishly into a trap.', 14, 0, 100, 0, 0, 26223, 'hagara adds 1'),
(55689, 1, 0, 'Don\'t preen just yet little pups, we\'ll cleanse this world of your kind.', 14, 0, 100, 0, 0, 26224, 'hagara adds 2'),
(55689, 2, 0, 'You won\'t leave this place alive.', 14, 0, 100, 0, 0, 26225, 'hagara adds 3'),
(55689, 3, 0, 'Not one of you will live to see the final Cataclysm! Finish them!', 14, 0, 100, 0, 0, 26226, 'hagara adds 4'),
(55689, 4, 0, 'You cross the Stormbinder! I\'ll slaughter you all.', 14, 0, 100, 0, 0, 26227, 'hagara aggro'),
(55689, 5, 0, 'What are you doing?', 14, 0, 100, 0, 0, 26228, 'hagara circuit 01'),
(55689, 5, 1, 'You\'re toying with death.', 14, 0, 100, 0, 0, 26229, 'hagara circuit 02'),
(55689, 5, 2, 'You think you can play with my lightning?', 14, 0, 100, 0, 0, 26230, 'hagara circuit 03'),
(55689, 5, 3, 'No! More... lightning...', 14, 0, 100, 0, 0, 26232, 'hagara circuit 05'),
(55689, 5, 4, 'Enough of your games! You won\'t live to do it again.', 14, 0, 100, 0, 0, 26233, 'hagara circuit 06'),
(55689, 5, 5, 'I\'ll finish you now pups!', 14, 0, 100, 0, 0, 26234, 'hagara circuit 07'),
(55689, 6, 0, 'The time I spent binding that, WASTED!', 14, 0, 100, 0, 0, 26235, 'hagara crystal dead 01'),
(55689, 6, 1, 'You\'ll PAY for that.', 14, 0, 100, 0, 0, 26236, 'hagara crystal dead 02'),
(55689, 6, 2, 'Enough!', 14, 0, 100, 0, 0, 26237, 'hagara crystal dead 03'),
(55689, 6, 3, 'Again?!', 14, 0, 100, 0, 0, 26239, 'hagara crystal dead 05'),
(55689, 6, 4, 'Impudent pup!', 14, 0, 100, 0, 0, 26240, 'hagara crystal dead 06'),
(55689, 6, 5, 'The one remaining is still enough to finish you.', 14, 0, 100, 0, 0, 26241, 'hagara crystal dead 07'),
(55689, 7, 0, 'Cowards! You pack of weakling...dogs...', 14, 0, 100, 0, 0, 26243, 'hagara dead'),
(55689, 8, 0, 'You face more than my axes, this close.', 14, 0, 100, 0, 0, 26244, 'hagara frostray 01'),
(55689, 8, 1, 'See what becomes of those who stand before me!', 14, 0, 100, 0, 0, 26245, 'hagara frostray 02'),
(55689, 8, 2, 'Feel a chill up your spine...?', 14, 0, 100, 0, 0, 26246, 'hagara frostray 03'),
(55689, 9, 0, 'You can\'t outrun the storm.', 14, 0, 100, 0, 0, 26247, 'hagara glacier 01'),
(55689, 9, 1, 'Die beneath the ice.', 14, 0, 100, 0, 0, 26248, 'hagara glacier 02'),
(55689, 10, 0, 'Stay, pup.', 14, 0, 100, 0, 0, 26249, 'hagara icetomb 01'),
(55689, 10, 1, 'Hold still.', 14, 0, 100, 0, 0, 26250, 'hagara icetomb 02'),
(55689, 11, 0, 'Swagger all you like; you pups don\'t stand a chance. Flee now, while you can.', 14, 0, 100, 0, 0, 26251, 'hagara intro 01'),
(55689, 12, 0, 'Suffer the storm\'s wrath!', 14, 0, 100, 0, 0, 26252, 'hagara lightning 01'),
(55689, 12, 1, 'Thunder and lightning dance at my call!', 14, 0, 100, 0, 0, 26253, 'hagara lightning 02'),
(55689, 13, 0, 'Feh!', 14, 0, 100, 0, 0, 26254, 'hagara slay 02'),
(55689, 13, 1, 'You should have run, dog!', 14, 0, 100, 0, 0, 26255, 'hagara slay 01'),
(55689, 13, 2, 'Down, pup.', 14, 0, 100, 0, 0, 26256, 'hagara slay 03'),
(55689, 13, 3, 'A waste of my time.', 14, 0, 100, 0, 0, 26257, 'hagara slay 04');


DELETE FROM `locales_creature_text` WHERE `entry`=55689;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55689, 0, 0, 'Хм, даже Аспект Времени не помог вам избежать моей ловушки.'),
(55689, 1, 0, 'Не обольщайтесь, щенки. Мы избавим от вас этот мир.'),
(55689, 2, 0, 'Ха, вы не уйдете отсюда живыми!'),
(55689, 3, 0, 'Ни один из вас не доживет до настоящего катаклизма! Прикончите их!'),
(55689, 4, 0, 'Вы осмелились бросить вызов владычице штормов?!'),
(55689, 5, 0, 'Что вы делаете?!'),
(55689, 5, 1, 'Вы играете со смертью.'),
(55689, 5, 2, 'Думаете, у вас тоже получится поиграть с моими молниями?'),
(55689, 5, 3, 'Нет! Больше... молний...'),
(55689, 5, 4, 'Хватит! Это будет последним, что вы сделали в этой жизни.'),
(55689, 5, 5, 'Я прикончу вас сейчас же, щенки!'),
(55689, 6, 0, 'Я потратила столько времени, чтобы создать их!'),
(55689, 6, 1, 'Вы ЗАПЛАТИТЕ за это.'),
(55689, 6, 2, 'Довольно!'),
(55689, 6, 3, 'Опять?!'),
(55689, 6, 4, 'Нахальный щенок!'),
(55689, 6, 5, 'И одного достаточно, чтобы вас прикончить.'),
(55689, 7, 0, 'Трусы! Стая жалких... псов...'),
(55689, 8, 0, 'В моем арсенале не только топоры!'),
(55689, 8, 1, 'Вот, что станет с теми, кто противостоит мне!'),
(55689, 8, 2, 'Чувствуете холодное дыхание смерти?..'),
(55689, 9, 0, 'Вам не перегнать снежную бурю.'),
(55689, 9, 1, 'Лед убьет вас.'),
(55689, 10, 0, 'Стоять, щенок.'),
(55689, 10, 1, 'Не двигайся.'),
(55689, 11, 0, 'Жалкие щенки. Вам не на что надеяться. Бегите, пока можете.'),
(55689, 12, 0, 'Ощутите ярость бури!'),
(55689, 12, 1, 'Мне покорны гром и молнии!'),
(55689, 13, 0, 'Фу!'),
(55689, 13, 1, 'Лучше бы тебе было сбежать, щенок.'),
(55689, 13, 2, 'Умри, щенок.'),
(55689, 13, 3, 'Вы зря тратите мое время!');