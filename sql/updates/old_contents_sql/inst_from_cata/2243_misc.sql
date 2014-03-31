DELETE FROM `creature` WHERE `map`=967 AND `id` IN
(57268, 57297, 56599, 56781, 
56855, 56587, 56854, 56848, 
56923, 56427, 57190, 56598,
56922);

DELETE FROM `waypoint_data` WHERE `id` IN 
(344648, 344653, 344655, 344661, 344710, 344735, 370500);

REPLACE INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES 
(56781, 46598, 1, 0);

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(57190, 0, 0, 0, 1, 0, '107799'),
(57268, 0, 0, 0, 1, 0, '109247 108053');


UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry` IN (56781, 57937, 57938, 57939, 56922);
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry`=57190;
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry` IN (56855, 57701, 57838, 57840, 56587, 57700, 57837, 57839);
UPDATE `creature_template` SET `unit_flags`=`unit_flags`|16777216 WHERE `entry` IN (56781, 57937, 57938, 57939);
UPDATE `creature_template` SET `dynamicflags`=`dynamicflags`&~8 WHERE `entry` IN (56427, 57699, 57847, 57848);
UPDATE `creature_template` SET `modelid2`=0 WHERE `entry` IN (56598, 57698, 57845, 57846);
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=57297;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry`=57268;
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0 WHERE `entry`=57268;
UPDATE `creature_template` SET `minlevel`=88, `maxlevel`=88, `exp`=3 WHERE `entry` IN (56598, 57698, 57845, 57846);

UPDATE `creature_template` SET `mechanic_immune_mask`=617296863 WHERE `entry` IN
(56923, 57704, 57717, 57718);
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN
(56427, 57699, 57847, 57848,
56781, 57937, 57938, 57939,
56855, 57701, 57838, 57840,
56587, 57700, 57837, 57839,
56854, 57702, 57841, 57842,
56848, 57703, 57843, 57844,
56922);

UPDATE `gameobject_template` SET `flags`=32 WHERE `entry`=210210;

UPDATE `creature_template` SET `ScriptName`='boss_warmaster_blackhorn' WHERE `entry`=56427;
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_goriona' WHERE `entry`=56781;
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_skyfire' WHERE `entry`=56598;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_sky_captain_swayze' WHERE `entry`=55870;
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_twilight_assault_drake' WHERE `entry` IN (56855, 56587);
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_twilight_elite_dreadblade_slayer' WHERE `entry` IN (56854, 56848);
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_skyfire_harpoon_gun' WHERE `entry`=56681;
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_twilight_infiltrator' WHERE `entry`=56922;
UPDATE `creature_template` SET `ScriptName`='npc_warmaster_blackhorn_twilight_sapper' WHERE `entry`=56923;

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(107439, 109203, 109204, 109205,
106401, 108862, 109226, 109227, 
107595, 109013, 109014, 109015,
110214, 110598, 108045, 110137,
110153);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(107439, 'spell_warmaster_blackhorn_twilight_barrage_dmg'),
(109203, 'spell_warmaster_blackhorn_twilight_barrage_dmg'),
(109204, 'spell_warmaster_blackhorn_twilight_barrage_dmg'),
(109205, 'spell_warmaster_blackhorn_twilight_barrage_dmg'),
(106401, 'spell_warmaster_blackhorn_twilight_onslaught_dmg'),
(108862, 'spell_warmaster_blackhorn_twilight_onslaught_dmg'),
(109226, 'spell_warmaster_blackhorn_twilight_onslaught_dmg'),
(109227, 'spell_warmaster_blackhorn_twilight_onslaught_dmg'),
(107595, 'spell_warmaster_blackhorn_blade_rush_dmg'),
(109013, 'spell_warmaster_blackhorn_blade_rush_dmg'),
(109014, 'spell_warmaster_blackhorn_blade_rush_dmg'),
(109015, 'spell_warmaster_blackhorn_blade_rush_dmg'),
(110214, 'spell_warmaster_blackhorn_consuming_shroud'),
(110598, 'spell_warmaster_blackhorn_consuming_shroud'),
(108045, 'spell_warmaster_blackhorn_vengeance'),
(110137, 'spell_warmaster_blackhorn_shockwave_aoe'),
(110153, 'spell_warmaster_blackhorn_broadside');

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(107287, 107501, 107589, 107518,
108040, 108041, 109213, 110312,
110153, 110157);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 107287, 0, 31, 0, 3, 56598, 'Warmaster Blackhorn - Twilight Barrage dummy'),
(13, 1, 107501, 0, 31, 0, 3, 56598, 'Warmaster Blackhorn - Twilight Barrage dmg 2'),
(13, 1, 107589, 0, 31, 0, 3, 56598, 'Warmaster Blackhorn - Twilight Onslaught dmg 2'),
(13, 2, 107518, 0, 31, 0, 3, 56598, 'Warmaster Blackhorn - Detonate'),
(13, 1, 108040, 0, 31, 0, 3, 56781, 'Warmaster Blackhorn - Artillery Barrage'),
(13, 1, 108041, 0, 31, 0, 3, 56781, 'Warmaster Blackhorn - Artillery Barrage dmg'),
(13, 1, 109213, 0, 31, 0, 3, 56781, 'Warmaster Blackhorn - Artillery Barrage dmg'),
(13, 1, 110312, 0, 31, 0, 3, 56781, 'Warmaster Blackhorn - Siphon Vitality'),
(13, 1, 110153, 0, 31, 0, 3, 56598, 'Warmaster Blackhorn - Broadside'),
(13, 1, 110157, 0, 31, 0, 3, 56598, 'Warmaster Blackhorn - Broadside');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (18444, 18485, 18450, 18464);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(18444, 11, 0, 0, 'achievement_deck_defender');

DELETE FROM `creature_addon` WHERE `guid` NOT IN (SELECT `guid` FROM `creature`);

UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=125 WHERE `entry` IN
(55265, 57409, 57773, 57774, 55312, 55313, 55308, 55309, 55689, 57462, 55294, 56576, 56427, 57699);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=143 WHERE `entry` IN
(57771, 57772, 56577, 56578, 55310, 55311, 55314, 55315, 57955, 57956, 57847, 57848, 57995, 57996);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=80 WHERE `entry` IN
(56781, 57937, 56854, 57702, 56848, 57703);
UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822, `dmg_multiplier`=100 WHERE `entry` IN
(57938, 57939, 57841, 57842, 57843, 57844);

UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (56427, 57699, 57847, 57848);

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(56427, 396, 614, 0, 12000, 1, 0),
(57699, 396, 614, 0, 14000, 1, 0),
(57847, 396, 614, 0, 12000, 1, 0),
(57848, 396, 614, 0, 14000, 1, 0);

DELETE FROM `creature_text` WHERE `entry`=56427;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(56427, 0, 0, 'You won\'t get near the Master. Dragonriders, attack!', 14, 0, 100, 0, 0, 26210, 'Warmaster Blackhorn aggro'),
(56427, 1, 0, 'Well...done, heh. But I wonder if you\'re good enough...to best him.', 14, 0, 100, 0, 0, 26211, 'Warmaster Blackhorn death'),
(56427, 2, 0, 'Looks like I\'m doing this myself. Good!', 14, 0, 100, 0, 0, 26212, 'Warmaster Blackhorn event 01'),
(56427, 3, 0, 'We\'re flying a little too close. It\'s been a good fight, but I\'m ending it, now.', 14, 0, 100, 0, 0, 26213, 'Warmaster Blackhorn event 02'),
(56427, 4, 0, 'Hah! I was hoping you\'d make it this far. You\'d best be ready for a real fight.', 14, 0, 100, 0, 0, 26214, 'Warmaster Blackhorn intro'),
(56427, 5, 0, 'Mess with the bull....', 14, 0, 100, 0, 0, 26215, 'Warmaster Blackhorn slay 01'),
(56427, 5, 1, 'Warmaster Blackhorn laughs.', 16, 0, 100, 0, 0, 26216, 'Warmaster Blackhorn slay 02'),
(56427, 5, 2, 'Down you go!', 14, 0, 100, 0, 0, 26217, 'Warmaster Blackhorn slay 03'),
(56427, 5, 3, 'Get up! Oh... weakling!', 14, 0, 100, 0, 0, 26218, 'Warmaster Blackhorn slay 04'),
(56427, 6, 0, 'Goriona! Give them hell!', 14, 0, 100, 0, 0, 26219, 'Warmaster Blackhorn spell 01'),
(56427, 7, 0, 'Warmaster Blackhorn roars happily.', 16, 0, 100, 0, 0, 26220, 'Warmaster Blackhorn spell 02'),
(56427, 8, 0, 'How\'s THIS?', 14, 0, 100, 0, 0, 26221, 'Warmaster Blackhorn spell 03'),
(56427, 8, 1, 'Come on!', 14, 0, 100, 0, 0, 26222, 'Warmaster Blackhorn spell 04'),
(56427, 9, 0, 'A drake swoops down to drop a Twilight Sapper onto the deck!', 41, 0, 100, 0, 0, 0, 'Warmaster Blackhorn sapper yell');

DELETE FROM `locales_creature_text` WHERE `entry`=56427;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(56427, 0, 0, 'Вы не доберетесь до господина. Наездники на драконах, в атаку!'),
(56427, 1, 0, 'Вы... победили. Интересно, хватит ли вам сил, чтобы одержать верх... над ним.'),
(56427, 2, 0, 'Похоже, мне придется заняться этим самому. Чудесно!'),
(56427, 3, 0, 'Бой затягивается. Это было неплохо, но пора заканчивать.'),
(56427, 4, 0, 'Ха! Я вас уже заждался. Надеюсь, вы готовы к настоящему бою.'),
(56427, 5, 0, 'Не стоило дразнить быка.'),
(56427, 5, 1, 'Воевода Черный Рог смеется.'),
(56427, 5, 2, 'С тобой покончено!'),
(56427, 5, 3, 'Встать! Размазня...'),
(56427, 6, 0, 'Гориона! Задай им жару!'),
(56427, 7, 0, 'Воевода Черный Рог ревет.'),
(56427, 8, 0, 'А как вам ЭТО?'),
(56427, 8, 1, 'Это все, на что вы способны?'),
(56427, 9, 0, 'Дракон пикирует на палубу, чтобы сбросить на нее сумеречного сапера!');

DELETE FROM `creature_text` WHERE `entry`=55870;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55870, 0, 0, 'Little hand says it\'s time to rock and roll! Everyone into the hole, we\'ve got a dragon to kill!', 14, 0, 100, 0, 0, 26289, 'Sky Captain Swayze blackhorn victory'),
(55870, 1, 0, 'The plates! He\'s coming apart! Tear up the plates and we\'ve got a shot at bringing him down!', 14, 0, 100, 0, 0, 26290, 'Sky Captain Swayze deathwing back 01'),
(55870, 2, 0, 'Don\'t worry about me. Stop the Destroyer! Enshu-falah-nah, dorini.', 14, 0, 100, 0, 0, 26291, 'Sky Captain Swayze'),
(55870, 3, 0, 'All ahead full. Everything depends on our speed! We can\'t let the Destroyer get away.', 14, 0, 100, 0, 0, 26292, 'Sky Captain Swayze event 01'),
(55870, 4, 0, 'All hands to battle stations; get those monsters away from the ship!', 14, 0, 100, 0, 0, 26293, 'Sky Captain Swayze event 02'),
(55870, 5, 0, 'Concentrate everything on the armored drake!', 14, 0, 100, 0, 0, 26295, 'Sky Captain Swayze event 03'),
(55870, 6, 0, 'An Enemy Sapper has breached the Engine Room!', 14, 0, 100, 0, 0, 26296, 'Sky Captain Swayze event 04'),
(55870, 7, 0, 'The Skyfire can\'t take much more of this!', 14, 0, 100, 0, 0, 26297, 'Sky Captain Swayze event 05'),
(55870, 8, 0, 'Were going down, abandon the ship!', 14, 0, 100, 0, 0, 26298, 'Sky Captain Swayze event 06'),
(55870, 9, 0, 'Welcome aboard the Skyfire. You ready to chase down the end of the world?', 14, 0, 100, 0, 0, 26302, 'Sky Captain Swayze pre deathwing back 01'),
(55870, 10, 0, 'Our engines are damaged! We\'re sitting ducks up here!', 14, 0, 100, 0, 0, 26303, 'Sky Captain Swayze pre deathwing back 02');

DELETE FROM `locales_creature_text` WHERE `entry`=55870;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55870, 0, 0, 'Настало время повеселиться! Все по местам, пора нам подстрелить этого дракона!'),
(55870, 1, 0, 'Смотрите! Он разваливается! Оторвите пластины, и у нас появится шанс сбить его!'),
(55870, 2, 0, 'Не беспокойтесь обо мне. Остановите Разрушителя! Эншу-фалах-на, дорини.'),
(55870, 3, 0, 'Быстрее! Полный вперед! Мы не должны упустить его!'),
(55870, 4, 0, 'Все по местам. Бейте этих тварей. Не сбавлять ходу!'),
(55870, 5, 0, 'Сосредоточьте огонь на бронированном драконе!'),
(55870, 6, 0, 'Вражеский сапер прорвался в машинное отделение!'),
(55870, 7, 0, 'Еще немного – и "Небесный огонь" не выдержит!'),
(55870, 8, 0, 'Нас подбили! Покинуть корабль!'),
(55870, 9, 0, 'Добро пожаловать на борт. Готовы лететь к концу света?'),
(55870, 10, 0, 'Наши двигатели повреждены! Мы можем стать легкой добычей!');