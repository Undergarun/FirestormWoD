UPDATE `creature` SET `spawnmask`=15 WHERE `id`=52409 AND `map`=720;

DELETE FROM `creature` WHERE `map`=720 AND `id` IN 
(53266, 53261, 53231, 53952,
53393, 53086, 53140, 54147, 
54074, 53953, 53268, 53420,
53419, 53729, 53186, 53485,
53473, 53875, 53876, 53872,
54203, 54127, 54184, 53621);

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN 
(53266, 53268, 53086, 53363,
53420, 53419, 53952, 53393,
53186, 53485, 53729, 53186,
53473, 53953, 54203, 54127,
54184, 54074, 53621, 53585);

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (53231, 53816, 53817, 53818);

UPDATE `creature_template` SET `speed_walk`=1, `speed_run`=1 WHERE `entry`=53363;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry` IN 
(53473, 53086, 53186, 53266,
53268, 53363, 53393, 53393, 
53419, 53420, 53485, 53420, 
53729, 53952, 53953, 54074,
54203, 54127, 54184, 54074);

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(53729, 0, 0, 0, 0, 0, '99908'),
(53473, 0, 0, 0, 0, 0, '99128'),
(54184, 0, 0, 0, 0, 0, '100858');


UPDATE `creature_template` SET `ScriptName`='boss_ragnaros_firelands' WHERE `entry`=52409;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_sulfuras_smash' WHERE `entry` IN (53266, 53268);
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_magma_trap' WHERE `entry`=53086;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_splitting_blow' WHERE `entry`=53393;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_sulfuras_hand_of_ragnaros' WHERE `entry` IN (53420, 53419);
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_son_of_flame' WHERE `entry`=53140;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_molten_seed_caster' WHERE `entry`=53186;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_molten_elemental' WHERE `entry`=53189;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_lava_scion' WHERE `entry`=53231;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_blazing_heat' WHERE `entry`=53473;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_living_meteor' WHERE `entry`=53500;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_malfurion_stormrage' WHERE `entry`=53875;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_cenarius' WHERE `entry`=53872;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_hamuul_runetotem' WHERE `entry`=53876;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_dreadflame_spawn' WHERE `entry`=54203;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_dreadflame' WHERE `entry`=54127;
UPDATE `creature_template` SET `ScriptName`='npc_ragnaros_firelands_cloudburst' WHERE `entry`=54147;

DELETE FROM `conditions` WHERE `SourceEntry` IN 
(99054, 99051, 99172, 100175, 
100176, 100177, 99235, 100178, 
100179, 100180, 99236, 100181, 
100182, 100183, 98498, 100579,
100580, 100581, 100158, 100302,
99216, 99217, 99218, 99145,
101088, 101102, 100342, 100344,
100345, 100907, 100567, 100653,
101237);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 99054, 0, 31, 0,  3, 53140, 'Ragnaros Firelands - Invoke Sons aoe 1'),
(13, 1, 99051, 0, 31, 0,  3, 53140, 'Ragnaros Firelands - Invoke Sons aoe 2'),
(13, 1, 99172, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100175, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100176, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100177, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99235, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100178, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100179, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100180, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99236, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100181, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100182, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 100183, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99216, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99217, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 1, 99218, 0, 31, 0,  3, 53485, 'Ragnaros Firelands - Engulfing Flame'),
(13, 2, 98498, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 2, 100579, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 2, 100580, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 2, 100581, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Seed aoe 2'),
(13, 7, 100158, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Power'),
(13, 7, 100302, 0, 31, 0,  3, 53189, 'Ragnaros Firelands - Molten Power'),
(13, 1, 99145, 0, 31, 0,  3, 53140, 'Ragnaros Firelands - Blazing Heat heal'),
(13, 1, 101088, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Lavalogged'),
(13, 1, 101102, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Lavalogged'),
(13, 7, 100342, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Draw Out Firelord'),
(13, 7, 100344, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Draw Out Firelord'),
(13, 7, 100345, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Draw Out Firelord'),
(13, 1, 100907, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Freeze'),
(13, 7, 100567, 0, 31, 0,  3, 53500, 'Ragnaros Firelands - Breadth of Frost'),
(13, 3, 100653, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Entrapping Roots'),
(13, 3, 101237, 0, 31, 0,  3, 52409, 'Ragnaros Firelands - Entrapping Roots');

DELETE FROM `spell_script_names` WHERE `spell_id` IN 
(98259, 98260, 100110, 100111, 
98159, 98703, 98708, 100256, 
100257, 100258, 99054, 99012,
98473, 98518, 100252, 100253,
100254, 99172, 100175, 100176, 
100177, 99235, 100178, 100179,
100180, 99236, 100181, 100182,
100183, 99216, 99217, 99218,
99125, 99171, 100172, 100173,
100174, 100171, 100190, 99279,
99267, 101387, 101388, 101389,
100503, 100594, 100915, 101107);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(98259, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(98260, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(100110, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(100111, 'spell_ragnaros_firelands_wrath_of_ragnaros_aoe'),
(98159, 'spell_ragnaros_firelands_magma_trap_aoe'),
(98703, 'spell_ragnaros_firelands_sulfuras_smash_aoe'),
(98708, 'spell_ragnaros_firelands_sulfuras_smash'),
(100256, 'spell_ragnaros_firelands_sulfuras_smash'),
(100257, 'spell_ragnaros_firelands_sulfuras_smash'),
(100258, 'spell_ragnaros_firelands_sulfuras_smash'),
(99012, 'spell_ragnaros_firelands_splitting_blow'),
(99054, 'spell_ragnaros_firelands_invoke_sons_aoe_1'),
(98473, 'spell_ragnaros_firelands_blazing_speed'),
( 99172, 'spell_ragnaros_firelands_engulfing_flames'),
(100175, 'spell_ragnaros_firelands_engulfing_flames'),
(100176, 'spell_ragnaros_firelands_engulfing_flames'),
(100177, 'spell_ragnaros_firelands_engulfing_flames'),
(99235, 'spell_ragnaros_firelands_engulfing_flames'),
(100178, 'spell_ragnaros_firelands_engulfing_flames'),
(100179, 'spell_ragnaros_firelands_engulfing_flames'),
(100180, 'spell_ragnaros_firelands_engulfing_flames'),
(99236, 'spell_ragnaros_firelands_engulfing_flames'),
(100181, 'spell_ragnaros_firelands_engulfing_flames'),
(100182, 'spell_ragnaros_firelands_engulfing_flames'),
(100183, 'spell_ragnaros_firelands_engulfing_flames'),
(99216, 'spell_ragnaros_firelands_engulfing_flames'),
(99217, 'spell_ragnaros_firelands_engulfing_flames'),
(99218, 'spell_ragnaros_firelands_engulfing_flames'),
(99125, 'spell_ragnaros_firelands_blazing_heat_dummy'),
(99171, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100172, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100173, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100174, 'spell_ragnaros_firelands_engulfing_flame_script'),
(100171, 'spell_ragnaros_firelands_world_in_flame'),
(100190, 'spell_ragnaros_firelands_world_in_flame'),
(99267, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(101387, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(101388, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(101389, 'spell_ragnaros_firelands_living_meteor_aoe_1'),
(99279, 'spell_ragnaros_firelands_living_meteor_aoe_2'),
(100503, 'spell_ragnaros_firelands_breadth_of_frost_script'),
(100594, 'spell_ragnaros_firelands_superheated_dmg'),
(100915, 'spell_ragnaros_firelands_superheated_dmg'),
(101107, 'spell_ragnaros_firelands_rage_of_ragnaros_aoe');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN 
(17434, 18102, 17436, 17437, 18103, 18104, 18105, 18106, 17863);

-- UPDATE `access_requirement` SET `leader_achievement`=5802 WHERE `mapId`=720 AND `difficulty`=2;
-- UPDATE `access_requirement` SET `leader_achievement`=5802 WHERE `mapId`=720 AND `difficulty`=3;

UPDATE `creature_template` SET `lootid`=0 WHERE `entry` IN (52409, 53797, 53798, 53799);

DELETE FROM `creature_text` WHERE `entry`=52409;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(52409, 0, 0, 'Come forth, my servants, defend your master!', 14, 0, 100, 0, 0, 24513, 'Ragnaros adds 1'),
(52409, 0, 1, 'Minions of fire, purge the outsiders!', 14, 0, 100, 0, 0, 24514, 'Ragnaros adds 2'),
(52409, 0, 2, 'Denizens of flame, come to me!', 14, 0, 100, 0, 0, 24515, 'Ragnaros adds 3'),
(52409, 0, 3, 'Arise, servants of fire! Consume their flesh!', 14, 0, 100, 0, 0, 24516, 'Ragnaros adds 4'),
(52409, 1, 0, 'Mortal insects! You dare trespass into MY domain? Your arrogance will be purged in living flame.', 14, 0, 100, 0, 0, 24517, 'Ragnaros intro'),
(52409, 2, 0, 'No, noooo- This was to be my hour of triumph...', 14, 0, 100, 0, 0, 24518, 'Ragnaros death heroic'),
(52409, 3, 0, 'Too soon! ... You have come too soon...', 14, 0, 100, 0, 0, 24519, 'Ragnaros death normal'),
(52409, 4, 0, 'You will be crushed!', 14, 0, 100, 0, 0, 24520, 'Ragnaros hammer drop 1'),
(52409, 4, 1, 'Die!', 14, 0, 100, 0, 0, 24521, 'Ragnaros hammer drop 2'),
(52409, 4, 2, 'Your judgement has come!', 14, 0, 100, 0, 0, 24522, 'Ragnaros hammer drop 3'),
(52409, 5, 0, 'Enough! I will finish this!', 14, 0, 100, 0, 0, 24523, 'Ragnaros hummer pickup 1'),
(52409, 5, 1, 'Fall to your knees, mortals! This ends now!', 14, 0, 100, 0, 0, 24524, 'Ragnaros hummer pickup 2'),
(52409, 5, 2, 'Sulfuras will be your end!', 14, 0, 100, 0, 0, 24525, 'Ragnaros hummer pickup 3'),
(52409, 6, 0, 'When I finish this, your pathetic mortal world will burn with my vengeance!', 14, 0, 100, 0, 0, 24526, 'Ragnaros heroic 03'),
(52409, 7, 0, 'Arrggh, outsiders - this is not your realm!', 14, 0, 100, 0, 0, 24527, 'Ragnaros heroic 02'),
(52409, 8, 0, 'Too soon...', 14, 0, 100, 0, 0, 24528, 'Ragnaros heroic 01'),
(52409, 9, 0, 'This is my realm!', 14, 0, 100, 0, 0, 24529, 'Ragnaros kill 03'),
(52409, 9, 1, 'Pathetic!', 14, 0, 100, 0, 0, 24530, 'Ragnaros kill 02'),
(52409, 9, 2, 'Die, insect!', 14, 0, 100, 0, 0, 24531, 'Ragnaros kill 01'),
(52409, 10, 0, 'By fire be purged!', 14, 0, 25, 0, 0, 24532, 'Ragnaros purge'),
(52409, 11, 0, 'Deathwing has prepared me to face the vessel of Tarecgosa, your fools journey ends here!', 14, 0, 100, 0, 0, 25162, 'Ragnaros event'),
(52409, 12, 0, 'Begone from my realm, insects.', 14, 0, 100, 0, 0, 24533, 'Ragnaros aggro 1'),
(52409, 12, 1, 'I will see you all burn.', 14, 0, 100, 0, 0, 24534, 'Ragnaros aggro 2'),
(52409, 12, 2, 'Be consumed by flame!', 14, 0, 100, 0, 0, 24535, 'Ragnaros aggro 3'),
(52409, 12, 3, 'The realm of fire will consume you!', 14, 0, 100, 0, 0, 24536, 'Ragnaros aggro 4');

DELETE FROM `locales_creature_text` WHERE `entry`=52409;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(52409, 0, 0, 'Придите, мои слуги! Защитите своего хозяина!'),
(52409, 0, 1, 'Слуги пламени, испепелите чужаков!'),
(52409, 0, 2, 'Дети огня, придите ко мне!'),
(52409, 0, 3, 'Восстаньте, слуги пламени! Поглотите их плоть!'),
(52409, 1, 0, 'Презренные смертные! Вы осмелились вторгнуться в мои владения? Огонь очистит от вас эти земли!'),
(52409, 2, 0, 'Нет, нееет... Триумф был так близок...'),
(52409, 3, 0, 'Слишком рано!.. Вы пришли слишком рано...'),
(52409, 4, 0, 'Я сокрушу вас!'),
(52409, 4, 1, 'Умрите!'),
(52409, 4, 2, 'Ваш час пробил!'),
(52409, 5, 0, 'Довольно! Пора покончить с этим.'),
(52409, 5, 1, 'На колени, смертные!'),
(52409, 5, 2, 'Сульфурас уничтожит вас!'),
(52409, 6, 0, 'Когда я покончу с вами, моя ненависть испепелит ваш жалкий мир!'),
(52409, 7, 0, 'Прочь, чужаки, это не ваш мир!'),
(52409, 8, 0, 'Слишком рано...'),
(52409, 9, 0, 'Это мой мир!'),
(52409, 9, 1, 'Жалкое зрелище.'),
(52409, 9, 2, 'Умри, насекомое!'),
(52409, 10, 0, 'Огонь очистит все!'),
(52409, 11, 0, 'Смертокрыл подготовил меня к встрече с хранителем души Таресгосы. Твоя никчемная жизнь окончена.'),
(52409, 12, 0, 'Прочь из моего царства!'),
(52409, 12, 1, 'Вы все сгорите.'),
(52409, 12, 2, 'Вас поглотит пламя!'),
(52409, 12, 3, 'Царство огня поглотит вас!');

DELETE FROM `creature_text` WHERE `entry`=53872;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53872, 0, 0, 'Ragnaros has perished. But the primal powers he represents can never be vanquished. Another will rise to power, someday.', 14, 0, 100, 0, 0, 25158, 'Cenarius event 3'),
(53872, 1, 0, 'No, fiend. Your time is NOW.', 14, 0, 100, 0, 0, 25159, 'Cenarius event 1'),
(53872, 2, 0, 'Perhaps...', 14, 0, 100, 0, 0, 25160, 'Cenarius event 2'),
(53872, 3, 0, 'Indeed.', 14, 0, 100, 0, 0, 25161, 'Cenarius event 4');

DELETE FROM `locales_creature_text` WHERE `entry`=53872;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53872, 0, 0, 'Рагнарос погиб. Но те изначальные силы, воплощением которых он был, непобедимы. Когда-нибудь вместо него придет кто-то другой.'),
(53872, 1, 0, 'Нет, чудовище. Твое время пришло.'),
(53872, 2, 0, 'Возможно...'),
(53872, 3, 0, 'Конечно.');

DELETE FROM `creature_text` WHERE `entry`=53876;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53876, 0, 0, 'Yes, Cenarius. We must maintain a constant vigil over this realm. For let us celebrate this day, and the great victory we have earned here.', 14, 0, 100, 0, 0, 25168, 'Hamuul event 1');

DELETE FROM `locales_creature_text` WHERE `entry`=53876;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53876, 0, 0, 'Да, Кенарий. Мы должны всегда оставаться на страже. Но сейчас позволь нам отпраздновать этот день – день нашей великой победы.');

DELETE FROM `creature_text` WHERE `entry`=53875;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(53875, 0, 0, 'Heroes! He is bound. Finish him!', 14, 0, 100, 0, 0, 25169, 'Malfurion event 1'),
(53875, 1, 0, 'It is finished then!', 14, 0, 100, 0, 0, 25170, 'Malfurion event 2'),
(53875, 2, 0, 'Heroes, the world owns you a great debt.', 14, 0, 100, 0, 0, 25171, 'Malfurion event 3');

DELETE FROM `locales_creature_text` WHERE `entry`=53875;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(53875, 0, 0, 'Герои! Убейте его!'),
(53875, 1, 0, 'Ну, вот и все.'),
(53875, 2, 0, 'Герои! Мир перед вами в огромном долгу.');