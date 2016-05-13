UPDATE `gameobject_template` SET `flags`=32 WHERE `entry`=202244;
UPDATE `gameobject_template` SET `flags`=32 WHERE `entry`=202245;
UPDATE `creature_template` SET `unit_flags`=33588032, `dynamicflags`=8, `faction_A`=974, `faction_H`=974 WHERE `entry` IN (37813, 38402, 38582, 38583);
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN (36853, 38265, 38266, 38267);
UPDATE `creature_template_addon` SET `bytes1`=0 WHERE `entry` IN (37534, 38219);
-- UPDATE `creature_addon` SET `bytes1`=0 WHERE `guid`=115556;
UPDATE `creature_template_addon` SET `bytes1`=0 WHERE `entry` IN (37533, 38220);
-- UPDATE `creature_addon` SET `bytes1`=0 WHERE `guid`=115557;
DELETE FROM `creature_template_addon` WHERE `entry` in (36853, 38265, 38266, 38267);
-- DELETE FROM `creature_addon` WHERE `guid`=348694;
UPDATE `creature_template` SET `unit_flags`=33554434 WHERE `entry`=16980;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=37945;
DELETE FROM `creature` WHERE `id`=38458 AND `map`=631;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=202078;
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_the_lich_king_play_movie';
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(71278, 'spell_putricide_choking_gas_damage'),
(72460, 'spell_putricide_choking_gas_damage'),
(72619, 'spell_putricide_choking_gas_damage'),
(72620, 'spell_putricide_choking_gas_damage'),
(71279, 'spell_putricide_choking_gas_explosion_damage'),
(72459, 'spell_putricide_choking_gas_explosion_damage'),
(72621, 'spell_putricide_choking_gas_explosion_damage'),
(72622, 'spell_putricide_choking_gas_explosion_damage'),
(62207, 'spell_elder_brightleaf_unstable_sun_beam');

UPDATE `creature_template` SET `ScriptName`='npc_scourge_prisoner' WHERE `entry`=25610;
UPDATE `creature_template` SET `ScriptName`='npc_sindragosas_ward' WHERE `entry`=37503;
UPDATE `creature_template` SET `ScriptName`='npc_blighted_abomination' WHERE `entry`=37022;
UPDATE `creature_template` SET `ScriptName`='npc_vengeful_fleshreapert' WHERE `entry`=37038;
UPDATE `creature_template` SET `ScriptName`='npc_plague_scientist' WHERE `entry`=37023;
UPDATE `creature_template` SET `ScriptName`='npc_pustulating_horror' WHERE `entry`=10404;
UPDATE `creature_template` SET `ScriptName`='npc_decaying_colossus' WHERE `entry`=36880;

REPLACE INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES 
(5623, 'at_icc_start_sindragosa_gauntlet');

REPLACE INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(13012, 11, 0, 0, 'achievement_once_bitten_twice_shy_n25'),
(13013, 11, 0, 0, 'achievement_once_bitten_twice_shy_v25');


DELETE FROM `creature` WHERE `id`=22515 AND `map`=631;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(22515, 631, 15, 1, 16925, 0, 494.024, -2172.17, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 500.323, -2188.93, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 477.042, -2164.2, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 530.625, -2166.15, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 470.72, -2183.84, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 512.805, -2172.62, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 530.698, -2182.86, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 542.559, -2151.41, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 567.837, -2144.41, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 567.837, -2144.41, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 554.196, -2166.77, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 503.969, -2075.85, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 466.25, -2071.3, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 486.092, -2061.76, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 482.924, -2078.25, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 521.602, -2079.36, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 515.104, -2058.46, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 548.729, -2110.41, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 568.533, -2106.96, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 542.776, -2071.84, 845.878, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 539.628, -2089.51, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 16925, 0, 550.62, -2132.89, 840.94, 0, 300, 0, 0, 4120, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 0, 0, 4241.19, 2557.49, 364.952, 4.72984, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 631, 15, 1, 0, 0, 4241.34, 2411.52, 364.952, 1.5708, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 631, 10, 1, 0, 0, 4166.02, 2557.49, 364.952, 4.72984, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 631, 10, 1, 0, 0, 4241.34, 2411.52, 364.952, 1.5708, 120, 0, 0, 0, 0, 0, 0, 0, 0),
(22515, 631, 10, 1, 0, 0, 4166.17, 2411.52, 364.952, 1.5708, 120, 0, 0, 0, 0, 0, 0, 0, 0);

REPLACE INTO `gameobject` (`id`, `map`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES 
(202078, 631, 15, 1, 503.62, -2124.65, 836.607, 3.14159, 0, 0, 0, 1, 604800, 255, 1),
(202141, 631, 15, 1, 473.748, -2096.48, 840.857, 3.14159, 0, 0, 1, 0, 604800, 100, 1),
(202142, 631, 15, 1, 473.748, -2152.83, 840.857, -1.5708, 0, 0, 1, 0, 604800, 100, 1),
(8202143, 631, 15, 1, 533.56, -2152.83, 840.857, 0, 0, 0, 1, 0, 604800, 100, 1),
(202144, 631, 15, 1, 533.56, -2096.48, 840.857, 1.5708, 0, 0, 1, 0, 604800, 100, 1),
(202161, 631, 15, 1, 503.62, -2124.66, 836.607, 3.14159, 0, 0, 0, 1, 604800, 255, 1),
(202188, 631, 15, 1, 503.62, -2124.66, 836.607, 3.14159, 0, 0, 0, 1, 6000, 100, 0),
(202189, 631, 15, 1, 503.62, -2124.66, 836.607, 3.14159, 0, 0, 0, 1, 6000, 100, 1),
(202190, 631, 15, 1, 503.62, -2124.65, 840.522, 3.14159, 0, 0, 1, 0, 604800, 100, 1),
(202223, 631, 15, 1, 4356.93, 2769.41, 355.955, -2.35619, 0, 0, 1, 0, 604800, 100, 1),
(202436, 631, 15, 1, 426.561, -2123.86, 864.889, -3.12412, 0, 0, 0, 1, -604800, 255, 1),
(202437, 631, 15, 1, 425.089, -2123.31, 858.675, 0, 0, 0, 0, 1, 604800, 255, 1),
(202438, 631, 15, 1, 425.089, -2123.31, 858.675, 0, 0, 0, 0, 1, -604800, 255, 1);


-- 933
UPDATE `spell_proc_event` SET `procEx` = 0x0010000 WHERE `entry` = 69762;

-- 930
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 69127;
INSERT INTO `spell_linked_spell` VALUES
(69127, -28282, 1, 'EXPLOIT!!! Possible Lich King farm with Corrupted Ashbringer');

-- 916
UPDATE `creature_template` SET `InhabitType`=4,`flags_extra`=128,`unit_flags`=0x2000000,`ScriptName`='npc_arthas_teleport_visual' WHERE `entry`=30298; -- Invisible Stalker (Float, Uninteractible, LargeAOI)

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_icc_soul_missile';
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES
(72585,'spell_icc_soul_missile');

DELETE FROM `creature_template_addon` WHERE `entry`=30298;
INSERT INTO `creature_template_addon` (`entry`,`mount`,`bytes1`,`bytes2`,`auras`) VALUES
(30298,0,0x0,0x1,''); 

-- 912
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_buff_remover' WHERE `entry` IN (39372,39371);

DELETE FROM `trinity_string` WHERE `entry` IN (13074,13075);
INSERT INTO `trinity_string` (`entry`,`content_default`,`content_loc8`) VALUES
(13074, 'The commander Infernal Shout, affairs of Nordskola demand your attention. We will cope with the King-lichom and its servants without your help.','Командир Адский Крик, дела Нордскола требуют вашего внимания. Мы справимся с Королем-личом и его прислужниками без вашей помощи.'),
(13075, 'With all due respect, your Highness, we can do this alone.','При всем уважении, Ваше Величество, мы можем сделать это в одиночку');

-- 908
UPDATE `locales_creature_text` SET `text_loc8` = 'Тени скапливаются и роятся вокруг $N!' WHERE `entry` = 37955 AND `textGroup` = 5;

-- -- 905 
-- INSERT IGNORE INTO `locales_creature_text` VALUES 
-- ('16980', '0', '0', null, null, null, null, null, null, null, 'Чужаки ворвались во внутренние покои. Уничтожьте зеленого дракона! Оставьте только кости и прах для воскрешения!'),
-- ('36597', '0', '0', null, null, null, null, null, null, null, 'Неужели прибыли, наконец, хваленые силы света? Мне бросить Ледяную скорбь и сдаться на твою милость, Фордринг?'),
-- ('36597', '1', '0', null, null, null, null, null, null, null, 'Ты пройдешь через эти мучения сам, и будешь молить о пощаде, но я не буду слушать. Твои отчаянные крики послужат доказательством моей безграничной мощи...'),
-- ('36597', '2', '0', null, null, null, null, null, null, null, 'Я оставлю тебя в живых, чтобы ты увидел финал. Не могу допустить, чтобы величайший служитель света пропустил рождение моего мира.'),
-- ('36597', '3', '0', null, null, null, null, null, null, null, 'Идите, герои, покажите мне свой гнев!'),
-- ('36597', '4', '0', null, null, null, null, null, null, null, 'Я проморожу вас насквозь и вы разлетитесь на ледяные осколки!'),
-- ('36597', '5', '0', null, null, null, null, null, null, null, 'Смотрите, как мир рушится вокруг вас!'),
-- ('36597', '6', '0', null, null, null, null, null, null, null, 'Валькира, твой господин зовет!'),
-- ('36597', '7', '0', null, null, null, null, null, null, null, 'Ледяная Скорбь жаждет крови...'),
-- ('36597', '8', '0', null, null, null, null, null, null, null, 'Ледяная Скорбь, повинуйся мне!'),
-- ('36597', '9', '0', null, null, null, null, null, null, null, 'Ледяная Скорбь поглотит душу вашего товарища!'),
-- ('36597', '10', '0', null, null, null, null, null, null, null, 'Надежда тает!'),
-- ('36597', '10', '1', null, null, null, null, null, null, null, 'Пришел конец!'),
-- ('36597', '11', '0', null, null, null, null, null, null, null, 'Встречайте! Трагический финал!'),
-- ('36597', '12', '0', null, null, null, null, null, null, null, '%s начинает чтение заклинания Осквернение!'),
-- ('36597', '13', '0', null, null, null, null, null, null, null, '|TInterfaceIconsability_creature_disease_02.blp:16|tВы были инфицированы |cFFCF00FFМертвящей чумой!|r'),
-- ('36597', '14', '0', null, null, null, null, null, null, null, 'Сомнений нет - вы величайшие герои Азерота. Вы преодолели все препятствия, которые я воздвиг перед вами. Сильнейшие из моих слуг пали под вашим натиском... сгорели в пламени вашей ярости...'),
-- ('36597', '15', '0', null, null, null, null, null, null, null, 'Что движет вами? Праведность? Не знаю...'),
-- ('36597', '16', '0', null, null, null, null, null, null, null, 'Ты отлично обучил их, Фордринг. Ты привел сюда лучших воинов, которых знал мир... и отдал их в мои руки, как я и рассчитывал. Тебя следует наградить за этот невольный подарок.'),
-- ('36597', '17', '0', null, null, null, null, null, null, null, 'Смотри, как я буду воскрешать их и превращать в воинов Плети. Они повергнут этот мир в пучину хаоса. Азерот падет от их рук и ты станешь первой жертвой.'),
-- ('36597', '18', '0', null, null, null, null, null, null, null, 'Мне по душе эта ирония...'),
-- ('36597', '19', '0', null, null, null, null, null, null, null, 'Невозможно...'),
-- ('36597', '20', '0', null, null, null, null, null, null, null, 'И вот я стою, как лев перед овцами... и не дрожат они.'),
-- ('36597', '21', '0', null, null, null, null, null, null, null, 'Им неведом страх.'),
-- ('36626', '0', '0', null, null, null, null, null, null, null, 'НЕЕЕЕТ! Вы убили Вонючку! Сейчас получите!'),
-- ('36626', '1', '0', null, null, null, null, null, null, null, 'Повеселимся!'),
-- ('36626', '2', '0', null, null, null, null, null, null, null, '%s пускает газы.'),
-- ('36626', '3', '0', null, null, null, null, null, null, null, '%s выпускает Газовые Споры!'),
-- ('36626', '4', '0', null, null, null, null, null, null, null, 'Уууу... Что-то мне нехорошо...'),
-- ('36626', '5', '0', null, null, null, null, null, null, null, '%s начинает чтение заклинания |cFFFF7F00Едкая гниль!|r'),
-- ('36626', '6', '0', null, null, null, null, null, null, null, '%s рвет.'),
-- ('36626', '7', '0', null, null, null, null, null, null, null, 'Папочка, у меня получилось!'),
-- ('36626', '7', '1', null, null, null, null, null, null, null, 'Мертвец, мертвец, мертвец!'),
-- ('36626', '8', '0', null, null, null, null, null, null, null, 'Веселью конец!'),
-- ('36626', '9', '0', null, null, null, null, null, null, null, 'Пааа... Пааапа...'),
-- ('36627', '0', '0', null, null, null, null, null, null, null, 'Что? Прелесть? Неееееееет!!!'),
-- ('36627', '1', '0', null, null, null, null, null, null, null, 'УИИИИИИИ!'),
-- ('36627', '2', '0', null, null, null, null, null, null, null, '%s начинает чтение заклинания Брызги слизи!'),
-- ('36627', '3', '0', null, null, null, null, null, null, null, 'Липучка-вонючка.'),
-- ('36627', '4', '0', null, null, null, null, null, null, null, '|TInterfaceIconsspell_shadow_unstableaffliction_2.blp:16|t%s начинает чтение заклинания |cFFFF0000Взрыв нестабильного слизняка!|r'),
-- ('36627', '5', '0', null, null, null, null, null, null, null, 'Я сделал очень злую каку. Сейчас взорвется!'),
-- ('36627', '6', '0', null, null, null, null, null, null, null, 'Папочка сделает новые игрушки из тебя!'),
-- ('36627', '6', '1', null, null, null, null, null, null, null, 'Я это заломал...'),
-- ('36627', '7', '0', null, null, null, null, null, null, null, 'Тихий час!'),
-- ('36627', '8', '0', null, null, null, null, null, null, null, 'Папочка, не огорчайся...'),
-- ('36627', '9', '0', null, null, null, null, null, null, null, '|TInterfaceIconsability_creature_disease_02.blp:16|tВы получили |cFF00FF00Мутировавшую инфекцию!|r'),
-- ('36678', '0', '0', null, null, null, null, null, null, null, 'Это обычное облако газа. Только будьте осторожнее, не такое уж оно и обычное!'),
-- ('36678', '1', '0', null, null, null, null, null, null, null, 'Тухлопуз, ты всегда был моим любимчиком, как и Гниломорд. Молодец, что оставил столько газа, я его даже чувствую!'),
-- ('36678', '2', '0', null, null, null, null, null, null, null, 'Отличные новости, народ! Слизь снова потекла!'),
-- ('36678', '2', '1', null, null, null, null, null, null, null, 'Отличные новости, народ! Я починил трубы для подачи ядовитой слизи!'),
-- ('36678', '3', '0', null, null, null, null, null, null, null, 'Ужасные новости, народ, Гниломорд сдох! Но есть и отличные новости, он оставил море слизи, хоть плыви по ней в круизы! Ого! Да я поэт! Потрясающе!'),
-- ('36678', '4', '0', null, null, null, null, null, null, null, 'Отличные новости, народ! Я усовершенствовал штамм чумы, которая уничтожит весь Азерот!'),
-- ('36678', '5', '0', null, null, null, null, null, null, null, '%s начинает чтение заклинания Непредсказуемый эксперимент!'),
-- ('36678', '6', '0', null, null, null, null, null, null, null, 'Два слизняка в одной комнате! Может получиться что-то любопытное...'),
-- ('36678', '7', '0', null, null, null, null, null, null, null, 'Хмм. Что-то я ничего не чувствую. Чтооо...? А это еще откуда?'),
-- ('36678', '8', '0', null, null, null, null, null, null, null, 'На вкус как вишенка! Ой! Извините!'),
-- ('36678', '9', '0', null, null, null, null, null, null, null, '|TInterfaceIconsinv_misc_herb_evergreenmoss.blp:16|t%s читает заклинание |cFF00FF00Вязкая гадость!|r'),
-- ('36678', '10', '0', null, null, null, null, null, null, null, '%s читает заклинание |cFFFF7F00Удушливая Газовая Бомба!|r'),
-- ('36678', '11', '0', null, null, null, null, null, null, null, 'Хмм... Интересно...'),
-- ('36678', '11', '1', null, null, null, null, null, null, null, 'О, как неожиданно!'),
-- ('36678', '12', '0', null, null, null, null, null, null, null, 'Прекрасные новости народ!'),
-- ('36678', '13', '0', null, null, null, null, null, null, null, 'Плохие новости, народ! Похоже у меня ничего не выйдет.'),
-- ('36789', '0', '0', null, null, null, null, null, null, null, 'Герои, вы должны мне помочь. Мои силы на исходе. Залечите мои раны!'),
-- ('36789', '1', '0', null, null, null, null, null, null, null, 'Я открыла портал в Изумрудный Сон. Там вы найдете спасение, герои...'),
-- ('36789', '2', '0', null, null, null, null, null, null, null, 'Силы возвращаются ко мне! Герои, еще немного!'),
-- ('36789', '3', '0', null, null, null, null, null, null, null, 'Я долго не продержусь!'),
-- ('36789', '4', '0', null, null, null, null, null, null, null, 'Простите меня! Я... не могу... остано... ВСЕ ВО ВЛАСТИ КОШМАРА!'),
-- ('36789', '5', '0', null, null, null, null, null, null, null, 'Прискорбная потеря...'),
-- ('36789', '6', '0', null, null, null, null, null, null, null, 'НЕУДАЧНИКИ!'),
-- ('36789', '7', '0', null, null, null, null, null, null, null, 'Я излечилась! Изера, даруй мне силу покончить с этими нечестивыми тварями!'),
-- ('37119', '0', '0', null, null, null, null, null, null, null, 'Вот мы и на месте. То, что произойдёт здесь, войдёт в легенды. Неважно, каким будет исход, - для истории мы будем героями, сражавшимися за свободу и покой своего народа!'),
-- ('37119', '1', '0', null, null, null, null, null, null, null, 'Помните, герои, страх -- ваш самый опасный противник в этих проклятых залах. Скрепите свои сердца и ваши души воссияют ярче тысячи солнц. Завидев вас, враг отступит. Враг падет, когда свет праведности коснется его!'),
-- ('37119', '2', '0', null, null, null, null, null, null, null, 'Наш поход на Цитадель Ледяной Короны начинается!'),
-- ('37119', '3', '0', null, null, null, null, null, null, null, 'АРТАС! Я поклялся, что ты умрешь, а Плеть будет разбита! Я закончу то, что начал у Часовни Последней Надежды!'),
-- ('37119', '4', '0', null, null, null, null, null, null, null, 'Сила света не ведает преград, Саурфанг. Его душа безмерно страдает, и все же - пока он жив.'),
-- ('37119', '5', '0', null, null, null, null, null, null, null, 'Да будет так, во имя Света!'),
-- ('37122', '0', '0', null, null, null, null, null, null, null, 'Никогда... не мог дотянуться... до верхней полки...'),
-- ('37122', '1', '0', null, null, null, null, null, null, null, 'Жалкие невежи. Да вы и хорошей болотной личинки за всю жизнь не нашли.'),
-- ('37122', '2', '0', null, null, null, null, null, null, null, 'А это вам за то, что принесли тухлую змеиную слизь!'),
-- ('37122', '3', '0', null, null, null, null, null, null, null, 'Не дайте... Финкельштейну... пустить меня на опыты...'),
-- ('37122', '4', '0', null, null, null, null, null, null, null, 'Гибель наших товарищей была неизбежна. Они жили и умерли во имя Серебряного Авангарда.'),
-- ('37122', '5', '0', null, null, null, null, null, null, null, 'Лучше умереть сейчас, чем опять собирать реагенты для этого больного Финкельштейна.'),
-- ('37123', '0', '0', null, null, null, null, null, null, null, 'Теперь меня ничто не спасет. Продолжайте сражаться, братья...'),
-- ('37123', '1', '0', null, null, null, null, null, null, null, 'Что? Это сила...? Боль прошла! Присоединитесь ко мне в этом упоении смертью! '),
-- ('37123', '2', '0', null, null, null, null, null, null, null, 'Больше не болит, правда?'),
-- ('37123', '3', '0', null, null, null, null, null, null, null, 'Простите...'),
-- ('37123', '4', '0', null, null, null, null, null, null, null, 'Вы так бесстрашно сражались с Плетью. Да благословит вас свет.'),
-- ('37124', '0', '0', null, null, null, null, null, null, null, 'Прошу... сожгите мои останки. Хочу согреться хоть после смерти...'),
-- ('37124', '1', '0', null, null, null, null, null, null, null, 'Нет! Нет! Почему мне не дали умереть в огне? Сгорите все!'),
-- ('37124', '2', '0', null, null, null, null, null, null, null, 'Вы чувствуете жар?'),
-- ('37124', '3', '0', null, null, null, null, null, null, null, 'Что... что я наделал? Нет!'),
-- ('37124', '4', '0', null, null, null, null, null, null, null, 'Кому же под силу очистить это пристанище нечестивости? Благодарю вас...'),
-- ('37125', '0', '0', null, null, null, null, null, null, null, 'Я... достойно жил после смерти.'),
-- ('37125', '1', '0', null, null, null, null, null, null, null, 'От воли Короля Лича не уйти. Готовьтесь к жаркой схватке!'),
-- ('37125', '2', '0', null, null, null, null, null, null, null, 'Вот, что бывает, если стоять близко к бомбе!'),
-- ('37125', '3', '0', null, null, null, null, null, null, null, 'Какой... феерический финал!'),
-- ('37125', '4', '0', null, null, null, null, null, null, null, 'Впереди вас ждет много опасностей... постарайтесь сохранить все части тела.'),
-- ('37126', '0', '0', null, null, null, null, null, null, null, 'Однажды мы с тобой сражались плечом к плечу, Крок, но ты оказался предателем. Иди же, встречай свою вторую смерть!'),
-- ('37126', '1', '0', null, null, null, null, null, null, null, 'Жалкие создания! Умрите!'),
-- ('37126', '2', '0', null, null, null, null, null, null, null, 'Глупый Крок. Ты сам привел мне подкрепление. Восстаньте, Чемпионы Серебряного Авангарда, и служите Королю Личу!'),
-- ('37493', '1', '0', null, null, null, null, null, null, null, 'Что? Это сила...? Боль прошла! Присоединитесь ко мне в этом упоении смертью! '),
-- ('37126', '4', '0', null, null, null, null, null, null, null, 'Ну и союзника ты выбрал, Крок!'),
-- ('37126', '5', '0', null, null, null, null, null, null, null, 'Как? Они очень быстро умерли? Ну да ладно.'),
-- ('37126', '6', '0', null, null, null, null, null, null, null, 'Может быть... ты и прав, Крок.'),
-- ('37126', '7', '0', null, null, null, null, null, null, null, '%s пронзила $N!'),
-- ('37126', '8', '0', null, null, null, null, null, null, null, 'Эфирный щит %s был разрушен $N!'),
-- ('37129', '0', '0', null, null, null, null, null, null, null, 'Обнажите клинки, соратники. Врайкулы насмерть будут стоять за Королеву Льда'),
-- ('37129', '1', '0', null, null, null, null, null, null, null, 'Довольно пустой болтовни! Прибыли наши герои, с их помощью мы пробьемся через зал!'),
-- ('37129', '2', '0', null, null, null, null, null, null, null, 'Выманите их сюда, и мы вам поможем.'),
-- ('37129', '3', '0', null, null, null, null, null, null, null, 'Скорее, вперед!'),
-- ('37129', '4', '0', null, null, null, null, null, null, null, 'Сейчас прибудет ее подкрепление, нужно с ней быстро покончить!'),
-- ('37129', '5', '0', null, null, null, null, null, null, null, 'Я отвлеку ее на себя. Верните наших братьев обратно в могилы, а потом мы покончим с ней!'),
-- ('37129', '6', '0', null, null, null, null, null, null, null, 'Мне нужно отдохнуть.'),
-- ('37129', '7', '0', null, null, null, null, null, null, null, 'Герои, я больше не могу ее сдерживать!'),
-- ('37129', '8', '0', null, null, null, null, null, null, null, 'Одного желания отомстить... было недостаточно!'),
-- ('37181', '0', '0', null, null, null, null, null, null, null, 'Вы находитесь в самом сердце земель Плети. Здесь вас не защитит Свет. Ничто вас не защитит...'),
-- ('37181', '1', '0', null, null, null, null, null, null, null, 'Ты бы мог стать величайшим из моих рыцарей, Фордринг... той силой тьмы, которая прокатилась бы по этому миру и перевернула страницу истории.'),
-- ('37181', '2', '0', null, null, null, null, null, null, null, 'Но тебе уже не суждено им стать. Скоро у меня будет другой рыцарь.'),
-- ('37181', '3', '0', null, null, null, null, null, null, null, 'Сломить его было очень трудно. И я подверг его душу ужасным мучениям. Он так долго сопротивлялся, но скоро он склонится перед своим королем.'),
-- ('37181', '4', '0', null, null, null, null, null, null, null, 'В конце концов, все вы будете служить мне.'),
-- ('37183', '0', '0', null, null, null, null, null, null, null, 'НИКОГДА! Я никогда не буду служить тебе!'),
-- ('37491', '1', '0', null, null, null, null, null, null, null, 'Жалкие невежи. Да вы и хорошей болотной личинки за всю жизнь не нашли.'),
-- ('37491', '2', '0', null, null, null, null, null, null, null, 'А это вам за то, что принесли тухлую змеиную слизь!'),
-- ('37491', '3', '0', null, null, null, null, null, null, null, 'Не дайте... Финкельштейну... пустить меня на опыты...'),
-- ('37955', '0', '0', null, null, null, null, null, null, null, 'Это было... неразумное... решение.'),
-- ('37955', '1', '0', null, null, null, null, null, null, null, 'Я только попробую на вкус...'),
-- ('37955', '1', '1', null, null, null, null, null, null, null, 'Я голодна!'),
-- ('37955', '2', '0', null, null, null, null, null, null, null, 'Нет... Моя прелесть! Приятного аппетита!'),
-- ('37955', '3', '0', null, null, null, null, null, null, null, 'Вы жаждите крови!'),
-- ('37955', '4', '0', null, null, null, null, null, null, null, 'Страдайте же!'),
-- ('37955', '5', '0', null, null, null, null, null, null, null, 'Тени скапливаются и роятся вокруг $ N!'),
-- ('37955', '6', '0', null, null, null, null, null, null, null, 'Как вам такое?'),
-- ('37955', '7', '0', null, null, null, null, null, null, null, 'Начинаем представление.'),
-- ('37955', '8', '0', null, null, null, null, null, null, null, 'Вот как... у тебя не получилось?'),
-- ('37955', '8', '1', null, null, null, null, null, null, null, 'Какая жалость.'),
-- ('37955', '9', '0', null, null, null, null, null, null, null, 'Не повезло...'),
-- ('37955', '10', '0', null, null, null, null, null, null, null, 'СЕЙЧАС ВСЕ КОНЧИТСЯ!'),
-- ('37955', '11', '0', null, null, null, null, null, null, null, 'Но... мы так хорошо... ладили...'),
-- ('37970', '0', '0', null, null, null, null, null, null, null, 'Наксанар был досадным недоразумением! Сила сферы поможет Валанару совершить отмщение!'),
-- ('37970', '1', '0', null, null, null, null, null, null, null, 'Вызов крови переходит к Принцу Валанару!'),
-- ('37970', '2', '0', null, null, null, null, null, null, null, 'Моя чаша полна.'),
-- ('37970', '3', '0', null, null, null, null, null, null, null, '%s начинает чтение заклинания Могучий вихрь!'),
-- ('37970', '4', '0', null, null, null, null, null, null, null, 'Кушать... подано.'),
-- ('37970', '4', '1', null, null, null, null, null, null, null, 'Теперь вы видите, насколько мы сильны?'),
-- ('37970', '5', '0', null, null, null, null, null, null, null, "ПАДИТЕ НИЦ ПЕРЕД САН'ЛЕЙН!"),
-- ('37970', '6', '0', null, null, null, null, null, null, null, '...почему...?'),
-- ('37972', '0', '0', null, null, null, null, null, null, null, 'Темная сфера сделала меня неуязвимым!'),
-- ('37972', '1', '0', null, null, null, null, null, null, null, 'Вызов крови переходит к Принцу Келесету!'),
-- ('37972', '2', '0', null, null, null, null, null, null, null, 'И прольется кровь!'),
-- ('37972', '3', '0', null, null, null, null, null, null, null, 'Не стоило и отвлекаться на тебя?'),
-- ('37972', '3', '1', null, null, null, null, null, null, null, 'Истина в смерти.'),
-- ('37972', '5', '0', null, null, null, null, null, null, null, 'Моя королева... они идут...'),
-- ('37973', '0', '0', null, null, null, null, null, null, null, 'Трепещите перед Талдарамом, смертные, ибо сила сферы питает меня!'),
-- ('37973', '1', '0', null, null, null, null, null, null, null, 'Вызов крови переходит к Принцу Талдараму!'),
-- ('37973', '2', '0', null, null, null, null, null, null, null, 'Насладитесь болью!'),
-- ('37973', '3', '0', null, null, null, null, null, null, null, 'Огни Инферно движутся к $N!'),
-- ('37973', '4', '0', null, null, null, null, null, null, null, 'Пища для червей.'),
-- ('37973', '4', '1', null, null, null, null, null, null, null, 'Никому не будет пощады!'),
-- ('37973', '5', '0', null, null, null, null, null, null, null, 'Ха-ха-ха-ха-ха.'),
-- ('38004', '0', '0', null, null, null, null, null, null, null, 'Глупые смертные. Думали, что одолели нас? Сан\'лейн -- непобедимые воины Короля Лича! Теперь наши силы едины!'),
-- ('38004', '1', '0', null, null, null, null, null, null, null, 'Восстаньте, братья, и уничтожьте наших врагов.'),
-- ('38995', '0', '0', null, null, null, null, null, null, null, 'Мы даруем тебе быструю смерть, Артас. Более быструю, чем ты заслуживаешь за то, что замучил и погубил десятки тысяч душ.'),
-- ('38995', '1', '0', null, null, null, null, null, null, null, 'Да будет так. Герои, в атаку!'),
-- ('38995', '2', '0', null, null, null, null, null, null, null, 'СВЕТ, ДАРУЙ МНЕ ПОСЛЕДНЕЕ БЛАГОСЛОВЕНИЕ. ДАЙ МНЕ СИЛУ... РАЗБИТЬ ЭТИ ОКОВЫ!'),
-- ('38995', '3', '0', null, null, null, null, null, null, null, 'Хватит, Артас! Твоя ненависть не заберет больше ни одной жизни!'),
-- ('37493', '2', '0', null, null, null, null, null, null, null, 'Больше не болит, правда?'),
-- ('37493', '3', '0', null, null, null, null, null, null, null, 'Простите...'),
-- ('37494', '1', '0', null, null, null, null, null, null, null, 'Нет! Нет! Почему мне не дали умереть в огне? Сгорите все!'),
-- ('37494', '2', '0', null, null, null, null, null, null, null, 'Вы чувствуете жар?'),
-- ('37494', '3', '0', null, null, null, null, null, null, null, 'Что... что я наделал? Нет!'),
-- ('37495', '1', '0', null, null, null, null, null, null, null, 'От воли Короля Лича не уйти. Готовьтесь к жаркой схватке!'),
-- ('37495', '2', '0', null, null, null, null, null, null, null, 'Вот, что бывает, если стоять близко к бомбе!'),
-- ('37495', '3', '0', null, null, null, null, null, null, null, 'Какой... феерический финал!'),
-- ('37126', '3', '0', null, null, null, null, null, null, null, 'Нападай, Гроза Плети. Сейчас я покажу господину, кто из нас может называться \"Чемпионом\"!');

-- 903
DELETE FROM `creature_text` WHERE `entry` = 37503;
INSERT INTO `creature_text` VALUES
(37503, 0, 0, 'You must not approach the Frost Queen. Quickly, stop them!', 14, 0, 0, 0, 0, 17024, 'Sindragosa Ward - SAY_AGGRO');

DELETE FROM `locales_creature_text` WHERE `entry` = 37503;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(37503, 0, 0, 'Они не должны прорваться к Синдрагосе! Скорее, остановите их!');

DELETE FROM `creature_template_addon` WHERE `entry` IN (37502,38198,37501,38197);

-- 890
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_sindragosa_collision_filter';

-- 889
DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_deathbringer_boiling_blood';

DELETE FROM `locales_creature_text` WHERE `entry` = 36612;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(36612, 0, 0, 'Смертные, ваш путь закончится так и не начавшись! Никто не смеет входить  в Обитель Господина!'),
(36612, 1, 0, 'Плеть накроет этот мир волной смерти и разрушения!'),
(36612, 2, 0, 'ВИХРЬ КОСТЕЙ!'),
(36612, 3, 0, 'Костяная хватка!'),
(36612, 3, 1, 'Постой-ка тут!'),
(36612, 3, 2, 'Единственный выход - это СМЕРТЬ!'),
(36612, 4, 0, 'Кости, для жертвоприношений!'),
(36612, 4, 1, 'Сгинь в вечных муках!'),
(36612, 5, 0, 'Я вижу... лишь тьму'),
(36612, 6, 0, 'Во мне бушует ярость господина!'),
(36612, 7, 0, '%s распадается, и его кости начинают вращаться');

DELETE FROM `locales_creature_text` WHERE `entry` = 36855;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(36855, 0, 0, 'Вы здесь потому, что наделены особым знанием: вы понимаете, что на мир пало проклятье слепоты!'),
(36855, 1, 0, 'Вам удалось сквозь пелену лжи разглядеть источник истиной силы!'),
(36855, 2, 0, 'Посмотрите на свои руки, задумайтесь над нелепостью их строения!'),
(36855, 3, 0, 'Кожа, мускулы, кровь, что пульсирует в жилах, все это говорит о вашем не совершенстве, вашей слабостью. Создатели жестоко пошутили над вами!'),
(36855, 4, 0, 'Чем раньше вы осознаете собственную ущербность, тем скорее выберете иной путь!'),
(36855, 5, 0, 'Для нашего господина нет ничего невозможного, его сила не знает границ, ничто не может сломить его волю!'),
(36855, 6, 0, 'Тот, кто откажется повиноваться будет уничтожен. Тот же, кто будет служить ему верой и правдой, достигнет таких высот, о которых вы не в силах даже помыслить!'),
(36855, 7, 0, 'Как вы смеете ступать в эти священные покои? Это место станет вашей могилой!'),
(36855, 8, 0, 'Довольно! Пришла пора взять все в свои руки!'),
(36855, 9, 0, 'Барьер маны %s мерцает и исчезает!'),
(36855, 10, 0, 'Ты не в силах противится моей воле!'),
(36855, 11, 0, 'Прими это благословение и покажи чужакам мощь нашего господина!'),
(36855, 12, 0, 'Я освобождаю тебя от проклятья плоти, мой верный слуга!'),
(36855, 13, 0, 'Восстань и обрети истинную форму!'),
(36855, 14, 0, 'Вы осознали бессмысленность своих действий?'),
(36855, 14, 1, 'Ступай во тьму... Вечную тьму!'),
(36855, 15, 0, 'Мне надоел этот фарс!'),
(36855, 16, 0, 'На все воля господина...Ваша смерть неизбежна...');

DELETE FROM `locales_creature_text` WHERE `entry` IN (37813,37187,37200);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(37813, 0, 0, 'Все павшие войны Орды. Все дохлые псы Альянса. Все пополнят Армию Короля-лича. Даже сейчас Валь`киры воскрешают ваших покойников, чтобы те стали частью Плети!'),
(37813, 1, 0, 'Сейчас всё будет ещё хуже. Идите сюда, я покажу вам какой силой меня наделил Король-лич!'),
(37813, 2, 0, 'Дворфы...'),
(37813, 4, 0, 'Присоединись ко мне, отец. Перейди на мою сторону, и вместе мы разрушим этот мир во имя Плети и во славу Короля-лича!'),
(37813, 5, 0, 'Старый упрямец. У тебя нет шансов! Я сильнее и могущественнее, чем ты можешь представить!'),
(37813, 6, 0, 'Жалкий старик! Ну что ж, герои. Хотите узнать, сколь могущественна Плеть?'),
(37813, 7, 0, 'ВО ИМЯ КОРОЛЯ-ЛИЧА!'),
(37813, 8, 0, 'Земля обагрится вашей кровью!'),
(37813, 9, 0, 'Веселитесь, слуги мои!'),
(37813, 10, 0, 'Ты ничтожество!'),
(37813, 10, 1, 'Твоя душа не обретет покоя!'),
(37813, 11, 0, '%s впадает в бешенство!'),
(37813, 12, 0, 'Я... Cтал СМЕРТЬЮ!'),
(37813, 13, 0, 'Я... Освободился.'),
(37813, 14, 0, 'Кровавые чудовища Саурфанга Смертононого почуяли запах крови!'),

(37187, 0, 0, 'Кор''крон, выдвигайтесь! Герои, будьте начеку. Плеть только что...'),
(37187, 1, 0, 'Мой мальчик погиб у Врат Гнева. Я здесь, чтобы забрать его тело.'),
(37187, 2, 0, 'Мы назвали его Дранош - на орочьем это значит "Сердце Дренора". Я бы не позволил чернокнижникам збрать его. Он был бы в безопасности в Гарадаре под защитой старейшин.'),
(37187, 3, 0, 'Я пообещал его матери, когда она умирала, что пройду через Темный Портал один. неважно, умер бы я или выжил - мой сын остался бы цел. И чист...'),
(37187, 4, 0, 'Сегодня я исполню это обещание.'),
(37187, 5, 0, '%s атакует!'),
(37187, 6, 0, 'У тебя за спиной лежит тело моего сына. Ничто не помешает мне его забрать.'),
(37187, 11, 0, '%s кашляет.'),
(37187, 12, 0, '%s плачет над трупом своего сын.!'),
(37187, 13, 0, 'Мы похороним тебя как подобает, в Награнде, рядом с матерью и предками...'),
(37187, 14, 0, 'Помните о чести, герои... какой бы жестокой не была битва... никогда не трекайтесь от неё.'),

(37200, 0, 0, 'Тогда выдвигаемся! Быст...'),
(37200, 1, 0, 'Один орк против войска Альянса???'),
(37200, 2, 0, 'В атаку!'),
(37200, 3, 0, '%s задыхается.'),
(37200, 4, 0, 'Это был сын Саурфанга – предводитель Орды у Врат Гнева. Страшный конец...'),
(37200, 5, 0, 'Да что за... Там, вдалеке!');

DELETE FROM `locales_creature_text` WHERE `entry` IN (36948,36939);
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(36948, 0, 0, 'Запускайте двигатели! Летим навстречу судьбе.'),
(36948, 9, 0, 'Разрушители, сержанты, в бой!'),
(36948, 13, 0, 'Ну не говорите потом, что я не предупреждал. В атаку, братья и сестры!'),
(36939, 0, 0, "Воспряньте, сыны и дочери Орды! Сегодня мы будем биться со смертельным врагом! ЛОК'ТАР ОГАР!"),
(36939, 10, 0, 'Альянс повержен. Вперед, к Королю-личу!');

-- 884
DELETE FROM `creature_questrelation` WHERE id = 38316;
DELETE FROM `creature_involvedrelation` WHERE id = 38316;

UPDATE `creature_template` SET `npcflag` = 1, `ScriptName` = 'npc_icc_ring_recover' WHERE `entry` = 39509;

DELETE FROM `npc_text` WHERE `id` = 537006;
INSERT INTO `npc_text` (`id`, `text0_0`) VALUES
(537006, 'He lost his ring that obtained by Ashen reputation in Veredict? $B $B He might get it again according to its current reputation. $B $B If it recovers after you increase your reputation and can not get the new quest, delete his ring and speak to me again.');

DELETE FROM `locales_npc_text` WHERE `entry` = 537006;
INSERT INTO `locales_npc_text` (entry, `Text0_0_loc8`) VALUES
(537006, 'Потеряли кольцо, полученное от Пепельного союза? Я помогу его восстановить всего за 200 золотых монет');

DELETE FROM `trinity_string` WHERE `entry` IN (13050,13051,13052,13053,13054,13055,13056,13057,13058,13059,13060,13061,13062,13063,13064,13065,13066,13067,13068,13069,13070,13071,13072,13073);
INSERT INTO `trinity_string` (`entry`, `content_default`, `content_loc8`) VALUES
(13050, 'You have a ring!', 'Герой, у тебя уже есть одно кольцо Пепельного союза. Чтобы получить возможность носить другое, тебе нужно избавиться от старого!'),
(13051, 'Not enough reputation!', 'Герой, у тебя недостаточно репутации для получения кольца Пепельного союза!'),
(13052, 'Caster DPS (Friendly)', 'Получить Пепельное кольцо разрушения (Дружелюбие)'),
(13053, 'Healer (Friendly)', 'Получить Пепельное кольцо мудрости (Дружелюбие)'),
(13054, 'Melee - Strength (Friendly)', 'Получить Пепельное кольцо мощи (Дружелюбие)'),
(13055, 'Melee - Agility (Friendly)', 'Получить Пепельное кольцо отмщения (Дружелюбие)'),
(13056, 'Tank (Friendly)', 'Получить Пепельное кольцо отваги (Дружелюбие)'),
(13057, 'Caster DPS (Honored)', 'Получить Пепельное кольцо разрушения (Уважение)'),
(13058, 'Healer (Honored)', 'Получить Пепельное кольцо мудрости (Уважение)'),
(13059, 'Melee - Strength (Honored)', 'Получить Пепельное кольцо мощи (Уважение)'),
(13060, 'Melee - Agility (Honored)', 'Получить Пепельное кольцо отмщения (Уважение)'),
(13061, 'Tank (Honored)', 'Получить Пепельное кольцо отваги (Уважение)'),
(13062, 'Caster DPS (Revered)', 'Получить Пепельное кольцо разрушения (Почтение)'),
(13063, 'Healer (Revered)', 'Получить Пепельное кольцо мудрости (Почтение)'),
(13064, 'Melee - Strength (Revered)', 'Получить Пепельное кольцо мощи (Почтение)'),
(13065, 'Melee - Agility (Revered)', 'Получить Пепельное кольцо отмщения (Почтение)'),
(13066, 'Tank (Revered)', 'Получить Пепельное кольцо отваги (Почтение)'),
(13067, 'Caster DPS (Exalted)', 'Получить Пепельное кольцо разрушения (Превознесение)'),
(13068, 'Healer (Exalted)', 'Получить Пепельное кольцо мудрости (Превознесение)'),
(13069, 'Melee - Strength (Exalted)', 'Получить Пепельное кольцо мощи (Превознесение)'),
(13070, 'Melee - Agility (Exalted)', 'Получить Пепельное кольцо отмщения (Превознесение)'),
(13071, 'Tank (Exalted)', 'Получить Пепельное кольцо отваги (Превознесение)'),
(13072, 'Delete all rings!', 'Я готов разрушить свое старое кольцо Пепельного союза, чтобы получить новое!'),
(13073, 'Are you sure?', 'Вы действительно согласны удалить ваше кольцо Пепельного союза, чтобы иметь возможность выбрать другое?');

-- 875
-- Correct speed and immunity
UPDATE `creature_template` SET `speed_walk` = 0.9, `speed_run` = 0.7, `mechanic_immune_mask` = mechanic_immune_mask|0x00000400 WHERE `entry` IN (37562,38602,38760,38761,37697,38604,38758,38759);

-- 848
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-69674,-71224,-73022,-73023);

-- 842
UPDATE `creature_template` SET `unit_flags` = 4 WHERE `entry` = 38028;
UPDATE `smart_scripts` SET `target_type` = 5 WHERE `entryorguid` = 36829 AND `id` = 0;
UPDATE `creature_template` SET `flags_extra` = 256 WHERE `entry` IN (37007,38031);

-- 772
-- Servant of the Throne
SET @ENTRY := 36724;
SET @SOURCETYPE := 0;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` = @ENTRY LIMIT 1;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,6000,12000,11,71029,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Servant of the Throne - Cast Glacial Blast');

UPDATE `creature_template` SET mechanic_immune_mask = mechanic_immune_mask|256|33554432 WHERE `entry` = @ENTRY;

-- Ancient Skeletal Soldier
SET @ENTRY := 37012;
SET @SOURCETYPE := 0;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,6000,8000,11,70964,2,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Ancient Skeletal Soldier - Cast Shield Bash(70964)');

-- Nerub'ar Broodkeeper
SET @ENTRY := 36725;
SET @SOURCETYPE :=0;
UPDATE creature_template SET AIName='SmartAI', ScriptName='' WHERE entry=@ENTRY;
DELETE FROM smart_scripts WHERE source_type=0 AND entryorguid=@ENTRY;
INSERT INTO smart_scripts (entryorguid,source_type,id,link,event_type,event_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action_type,action_param1,action_param2,action_param3,action_param4,action_param5,action_param6,target_type,target_param1,target_param2,target_param3,target_x,target_y,target_z,target_o,COMMENT) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,30,5000,7000,5000,9000,11,70965,0,0,0,0,0,4,0,0,0,0,0,0,0, 'Nerub''ar Broodkeeper - In Combat - Cast Crypt Scarabs'),
(@ENTRY,@SOURCETYPE,1,0,0,0,100,30,5000,7000,5000,9000,11,70980,0,0,0,0,0,4,0,0,0,0,0,0,0, 'Nerub''ar Broodkeeper - In Combat - Cast Web Wrap'),
(@ENTRY,@SOURCETYPE,2,0,14,0,100,30,5000,30,8000,13000,11,36725,0,0,0,0,0,7,0,0,0,0,0,0,0, 'Nerub''ar Broodkeeper - In Combat - Cast Dark Mending on Friendlies');

-- Web Wrap
SET @ENTRY := 38028;
SET @SOURCETYPE :=0;
UPDATE creature_template SET AIName='SmartAI', ScriptName='' WHERE entry=@ENTRY;
DELETE FROM smart_scripts WHERE source_type=0 AND entryorguid=@ENTRY;
INSERT INTO smart_scripts (entryorguid,source_type,id,link,event_type,event_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action_type,action_param1,action_param2,action_param3,action_param4,action_param5,action_param6,target_type,target_param1,target_param2,target_param3,target_x,target_y,target_z,target_o,COMMENT) VALUES
(@ENTRY,@SOURCETYPE,0,0,6,0,100,30,0,0,0,0,28,71010,2,0,0,0,0,21,1,0,0,0,0,0,0, 'Web Wrap - On die - Remove stun aura');

-- Deathbound Ward
SET @ENTRY := 37007;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,6000,8000,11,71021,2,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathbound Ward - Saber Lash'),
(@ENTRY,@SOURCETYPE,1,0,0,0,100,0,6000,8000,18000,22000,11,71022,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathbound Ward - Disrupting Shout');

UPDATE `gameobject_template` SET `data4` = 1 WHERE `entry` IN (201814,201815,201816,201817);

DELETE FROM `locales_creature_text` WHERE `entry` = 37007;
INSERT INTO `locales_creature_text` (`entry`, `textGroup`, `id`, `text_loc8`) VALUES
(37007, 0, 0, 'В покои господина проникли!'),
(37007, 0, 1, 'Я пробудился...'),
(37007, 0, 2, 'Кто... идет?');

DELETE FROM `creature_template_addon` WHERE `entry` = 36725;
-- DELETE FROM `creature_addon` WHERE guid IN (79310,79313,136208,136209,136110,136111,136117,136132);
UPDATE `creature_template` SET `mechanic_immune_mask` = 650854271 WHERE `entry` IN (36725,38058);

DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = -70980;
INSERT INTO `spell_linked_spell` VALUES
(-70980, 71010, 0, 'Web Wrap');

UPDATE `creature_template` SET `unit_flags` = 0 WHERE `entry` IN (36725,38058);

-- UPDATE `creature` SET `position_z` = 35.2 WHERE `guid` IN (136117,136132);
-- UPDATE `creature` SET `position_z` = 41.99 WHERE `guid` IN (136110,136111);
-- UPDATE `creature` SET `position_z` = 42.56 WHERE `guid` IN (79310,79313);
-- UPDATE `creature` SET `position_z` = 37.98 WHERE `guid` IN (136208,136209);

-- Deathspeaker Zealot
SET @ENTRY := 36808;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,6000,8000,11,69492,2,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker Zealot - Shadow Cleave');

-- Deathspeaker Servant
SET @ENTRY := 36805;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,6000,8000,11,69576,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker Servant - Chaos Bolt'),
(@ENTRY,@SOURCETYPE,1,0,0,0,100,0,3000,6000,6000,8000,11,69405,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker Servant - Consuming Shadows'),
(@ENTRY,@SOURCETYPE,2,0,0,0,100,0,3000,6000,6000,8000,11,69404,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker Servant - Curse of Agony');

-- Deathspeaker Attendant
SET @ENTRY := 36811;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,6000,8000,11,69387,0,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker Attendant - Cast Shadow Bolt'),
(@ENTRY,@SOURCETYPE,1,0,0,0,100,0,3000,6000,6000,8000,11,69355,0,0,0,0,0,18,15,0,0,0.0,0.0,0.0,0.0,'Deathspeaker Attendant - Cast Shadow Nova on 15 yard distance');

-- Deathspeaker Disciple
DELETE FROM smart_scripts WHERE entryorguid = 36807 AND source_type = 0;
SET @ENTRY := 36807;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,30,0,3000,4000,6000,11,69387,0,0,0,0,0,2,0,0,0,0,0,0,0,'Deathspeaker Disciple - Cast Shadow Bolt'),
(@ENTRY,@SOURCETYPE,1,0,16,0,100,30,69391,40,15000,20000,11,69391,1,0,0,0,0,7,0,0,0,0,0,0,0,'Deathspeaker Disciple - Cast Dark Blessing'),
(@ENTRY,@SOURCETYPE,2,0,14,0,100,30,90000,40,15000,20000,11,69389,0,0,0,0,0,7,0,0,0,0,0,0,0,'Deathspeaker Disciple - Cast Shadow Mend');

-- Deathspeaker High Priest
SET @ENTRY := 36829;
SET @SOURCETYPE := 0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=@ENTRY AND `source_type`=@SOURCETYPE;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@ENTRY LIMIT 1;
INSERT INTO `smart_scripts` (`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`) VALUES
(@ENTRY,@SOURCETYPE,0,0,0,0,100,0,3000,6000,10000,15000,11,69483,2,0,0,0,0,2,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker High Priest - Cast Dark Reckoning'),
(@ENTRY,@SOURCETYPE,1,0,4,0,100,1,0,0,0,0,11,69491,0,0,0,0,0,1,0,0,0,0.0,0.0,0.0,0.0,'Deathspeaker High Priest - Cast Aura of Darkness On Self');


-- 761
UPDATE `creature_template` SET `ScriptName` = 'npc_drudge_ghoul_icc' WHERE `entry` = 37695;

-- 747
UPDATE `creature_template` SET `mindmg` = 422, `maxdmg` = 586, `attackpower` = 642, `dmg_multiplier` = 99 WHERE `entry` IN (38135,38395,38634,38635);

-- 741
UPDATE `creature_template` SET `mechanic_immune_mask` = 650854271 WHERE `entry` IN (37116,38256,38693,38694,37117,38257,38677,38678);

-- 725
-- Gunship Battle - Muradin & Saurfang
UPDATE `creature_template` SET `unit_flags` = 32768 WHERE entry IN (36948,38157,38639,38640,36939,38156,38637,38638);

-- 695
UPDATE `creature_template` SET `mechanic_immune_mask` = 617299839 WHERE `entry` IN (37116,38256,38693,38694,37117,38257,38677,38678);

-- 663
DELETE FROM `creature` WHERE `id`=37215 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36939 AND `map`=631;
DELETE FROM `creature` WHERE `id`=32780 AND `map`=631;
DELETE FROM `creature` WHERE `id`=37117 AND `map`=631;
DELETE FROM `creature` WHERE `id`=37547 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36968 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36968 AND `map`=631;
DELETE FROM `creature` WHERE `id`=37540 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36948 AND `map`=631;
DELETE FROM `creature` WHERE `id`=37182 AND `map`=631;
DELETE FROM `creature` WHERE `id`=50004 AND `map`=631;
DELETE FROM `creature` WHERE `id`=37184 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36970 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36838 AND `map`=631;
DELETE FROM `creature` WHERE `id`=37116 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36969 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36978 AND `map`=631;
DELETE FROM `creature` WHERE `id`=50006 AND `map`=631;
DELETE FROM `creature` WHERE `id`=36839 AND `map`=631;

-- 658

-- Muradin & Saurfang
DELETE FROM `creature_template` WHERE `entry` IN (300000, 300001);
INSERT INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES
(300000, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'High Overlord Saurfang', '', '', 0, 80, 80, 2, 1802, 1802, 0, 0, 0, 1, 1, 252, 357, 0, 304, '7.5', 2000, 0, 1, 33554432, 8, 0, 0, 0, 0, 0, 215, 320, 44, 7, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1000, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 2, '', 12340),
(300001, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Muradin Bronzebeard', '', '', 10875, 83, 83, 2, 1802, 1802, 1, 1, '1.14286', 1, 1, 468, 702, 0, 175, '47.2', 2000, 2000, 1, 33554432, 8, 0, 0, 0, 0, 0, 374, 562, 140, 7, 76, 0, 0, 0, 0, 0, 0, 0, 0, 0, 15284, 70309, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 175, 1, 1, 0, 0, 0, 0, 0, 0, 0, 164, 1, 0, 0, 2, '', 12340);

-- Miscs
UPDATE `creature_template` SET `npcflag` = 1 WHERE `entry` IN (36939, 38156, 38637, 38638, 37184);

-- Templates del trash
UPDATE `creature_template` SET `faction_A` = 1801, `faction_H` = 1801, `baseattacktime` = 2000, `rangeattacktime` = 2000, `unit_flags` = 32832, `spell1` = 71339, `spell2` = 70161, `spell3` = 72539, `mechanic_immune_mask` = 73745 WHERE `entry` IN (36968, 38403, 38675, 38676);
UPDATE `creature_template` SET `faction_A` = 1802, `faction_H` = 1802, `baseattacktime` = 2000, `rangeattacktime` = 2000, `unit_flags` = 32832, `spell1` = 71335, `spell2` = 70162, `spell3` = 72566, `mechanic_immune_mask` = 73745 WHERE `entry` IN (36969, 38408, 38689, 38690);

-- Spawn for the sorcerer and the vindicator
DELETE FROM `creature` WHERE `id` IN (37003, 37026);
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`modelid`,`equipment_id`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`currentwaypoint`,`curhealth`,`curmana`,`MovementType`,`npcflag`,`unit_flags`,`dynamicflags`) VALUES
(37003, 631, 15, 1, 0, 0, -562.385, 2217.58, 199.969, 3.46228, 86400, 0, 0, 398434, 41690, 0, 0, 0, 0),
(37026, 631, 15, 1, 0, 0, -571.091, 2219.71, 199.969, 5.06842, 86400, 0, 0, 269600, 91600, 0, 0, 0, 0);

-- Scriptnames
UPDATE `creature_template` SET `ScriptName` = 'npc_muradin_gunship' WHERE `entry` = 36948;
UPDATE `creature_template` SET `ScriptName` = 'npc_saurfang_gunship' WHERE `entry` = 36939;
UPDATE `creature_template` SET `ScriptName` = 'npc_zafod_boombox' WHERE `entry` = 37184;
UPDATE `creature_template` SET `ScriptName` = 'npc_korkron_defender' WHERE `entry` = 37032;
UPDATE `creature_template` SET `ScriptName` = 'npc_korkron_primalist' WHERE `entry` = 37030;
UPDATE `creature_template` SET `ScriptName` = 'npc_skybreaker_vindicator' WHERE `entry` = 37003;
UPDATE `creature_template` SET `ScriptName` = 'npc_skybreaker_protector' WHERE `entry` = 36998;
UPDATE `creature_template` SET `ScriptName` = 'npc_icc_spire_frostwyrm' WHERE `entry` = 37230;
UPDATE `creature_template` SET `ScriptName` = 'npc_korkron_axethrower_rifleman' WHERE `entry` = 36968;
UPDATE `creature_template` SET `ScriptName` = 'npc_korkron_axethrower_rifleman' WHERE `entry` = 36969;
UPDATE `creature_template` SET `ScriptName` = 'npc_mortar_soldier_or_rocketeer', AIName = '' WHERE `entry` = 36982;
UPDATE `creature_template` SET `ScriptName` = 'npc_mortar_soldier_or_rocketeer', AIName = '' WHERE `entry` = 36978;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_mage' WHERE `entry` = 37117;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_mage' WHERE `entry` = 37116;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_trigger' WHERE `entry` = 37547;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_portal' WHERE `entry` = 37227;
UPDATE `creature_template` SET `ScriptName` = 'npc_marine_or_reaver' WHERE `entry` = 36957;
UPDATE `creature_template` SET `ScriptName` = 'npc_marine_or_reaver' WHERE `entry` = 36950;
UPDATE `creature_template` SET `ScriptName` = 'npc_sergeant' WHERE `entry` = 36960;
UPDATE `creature_template` SET `ScriptName` = 'npc_sergeant' WHERE `entry` = 36961;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_skybreaker' WHERE `entry` = 37540;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_orgrimmar' WHERE `entry` = 37215;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_cannon' WHERE `entry` = 36838;
UPDATE `creature_template` SET `ScriptName` = 'npc_gunship_cannon' WHERE `entry` = 36839;
UPDATE `gameobject_template` SET `ScriptName` = 'transport_gunship' WHERE `entry` IN (201580, 201812);
REPLACE INTO `item_script_names` (`Id`, `ScriptName`) VALUES 
(49278, 'item_icc_rocket_pack');

-- Areatriggers Script
DELETE FROM `areatrigger_scripts` WHERE `entry` IN (5630, 5628);
INSERT INTO `areatrigger_scripts` (`entry`,`ScriptName`) VALUES
(5628,'at_icc_land_frostwyrm'),
(5630,'at_icc_land_frostwyrm');

-- Spell Scripts
DELETE FROM `spell_script_names` WHERE `spell_id` IN (70713, 68645, 69471, 69488, 69402, 70175, 71335, 71339);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
('70713', 'spell_icc_remove_rocket_pack'),
('68645', 'spell_rocket_pack'),
('69471', 'spell_gb_heat_drain'),
('69488', 'spell_gb_overheat_drain'),
('69402', 'spell_gb_incinerating_blast'),
('70175', 'spell_gb_incinerating_blast'),
('71335', 'spell_gb_burning_pitch'),
('71339', 'spell_gb_burning_pitch');

-- Transport
DELETE FROM transports WHERE entry IN (201580, 201581, 201811, 201812);
DELETE FROM creature WHERE id IN (37230, 36838, 36839, 36948, 36939);
DELETE FROM `creature_template_addon` WHERE (`entry`=37547);
UPDATE `creature_template` SET `flags_extra` = 130 WHERE `entry` = 37547;

-- Cannon template
UPDATE `creature_template` SET `exp` = 2, `VehicleId` = 554, `RegenHealth` = 0, `npcflag` = 16777216, `spell1` = 69399, `spell2` = 69401 WHERE entry = 36839;
UPDATE `creature_template` SET `exp` = 2, `VehicleId` = 554, `RegenHealth` = 0, `npcflag` = 16777216, `spell1` = 70172, `spell2` = 70174 WHERE entry = 36838;
DELETE FROM `npc_spellclick_spells` WHERE npc_entry IN (36838, 36839);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES
('36838', '46598', '1'),
('36839', '46598', '1');
DELETE FROM `creature_template_addon` WHERE `entry` IN (36838, 36839);
INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(36838, 0, 0, 0, 1, 0, 69470),
(36839, 0, 0, 0, 1, 0, 69470);
UPDATE `creature_template` SET `difficulty_entry_1` = '38129',`difficulty_entry_2` = '38701', `difficulty_entry_3` = '38702' WHERE `entry` =37215;
UPDATE `creature_template` SET `faction_A` = 35, `faction_H` = 35, `minlevel` = 83, `maxlevel` = 83, `unit_flags` = 4, `flags_extra` = 2, `modelid1` = 1126, `modelid2` = 11686 WHERE `entry` IN (37215, 38129, 38701, 38702, 37540, 38128, 38699, 38700);

-- Fix for Cannons
UPDATE `creature_template` SET `difficulty_entry_1`=0 WHERE `entry`=36839 LIMIT 1;
UPDATE `creature_template` SET `speed_walk`=0 ,`speed_run`=0 WHERE `entry` IN (36839,36838);

-- Spell effects
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 71193 AND `spell_effect` = -71188;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 71195 AND `spell_effect` = -71193;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 68645 AND `spell_effect` = 69193;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 73077 AND `spell_effect` = 69188;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 73077 AND `spell_effect` = 68721;
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` = 69193 AND `spell_effect` = 69192;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(71193, -71188, 0, 'Gunship Battle - Veteran removes Experimented'),
(71195, -71193, 0, 'Gunship Battle - Elite removes Veteran'),
(73077, 69188, 2, 'Gunship Battle - Fix visual for Rocket Pack'),
(68645, 69193, 0, 'Gunship Battle - Fix visual for Rocket Pack'),
(69193, 69192, 0, 'Gunship Battle - Cast Rocket Burst when landing with Rocket Pack');

-- Spells
UPDATE `creature_template` SET `unit_flags` = 33554564, `flags_extra` = 2 WHERE `entry` IN (37044, 37041);
UPDATE `creature_template` SET `faction_A` = 1801, `faction_H` = 1801 WHERE `entry` = 37044;
UPDATE `creature_template` SET `faction_A` = 1802, `faction_H` = 1802 WHERE `entry` = 37041;
DELETE FROM `creature_template_addon` WHERE `entry` IN (37044, 37041);
SET @KORKRON := 37044;     -- Kor'kron Battle Standard
SET @SKYBREAKER := 37041;  -- Skybreaker Battle Standard
SET @HSPELL := 69809;
SET @ASPELL := 69808;
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry` IN (@KORKRON, @SKYBREAKER);
DELETE FROM `smart_scripts` WHERE `source_type`=0 AND `entryorguid` IN (@KORKRON, @SKYBREAKER);
INSERT INTO `smart_scripts`
(`entryorguid`,`source_type`,`id`,`link`,`event_type`,`event_phase_mask`,`event_chance`,`event_flags`,`event_param1`,`event_param2`,`event_param3`,`event_param4`,`action_type`,`action_param1`,`action_param2`,`action_param3`,`action_param4`,`action_param5`,`action_param6`,`target_type`,`target_param1`,`target_param2`,`target_param3`,`target_x`,`target_y`,`target_z`,`target_o`,`comment`)
VALUES
(@KORKRON ,0,0,0,25,0,100,1,0,0,0,0,11,@HSPELL,2,0,0,0,0,1,0,0,0,0,0,0,0, 'Kor''kron Battle Standard - On spawn & reset - Cast spell 69809'),
(@SKYBREAKER,0,0,0,25,0,100,1,0,0,0,0,11,@ASPELL,2,0,0,0,0,1,0,0,0,0,0,0,0, 'Skybreaker Battle Standard - On spawn & reset - Cast spell 69808');

-- Creature Text
SET @MURADIN := 36948;
SET @OVERLORD := 36939;
SET @MURADIN_INVISIBLE := 300001;
SET @OVERLORD_INVISIBLE := 300000;
SET @PRIMALIST := 37030;
SET @INVOKER := 37033;
SET @DEFENDER := 37032;
SET @VINDICATOR := 37003;
SET @SORCERER := 37026;
SET @PROTECTOR := 36998;
SET @FROSTWYRM := 37230;
SET @SOUNDID :=0;

DELETE FROM `creature_text` WHERE `entry` IN (36948, 36939, 37030, 37033, 37032, 37003, 37026, 36998, 37230, 300000, 300001);
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(@MURADIN_INVISIBLE,  0, 0, 'Move yer jalopy or we''ll blow it out of the sky, orc! The Horde''s got no business here!', 14, 0, 0, 0, 0, 16969, 'Muradin Bronzebeard - INVISIBLE_0'),
(@MURADIN_INVISIBLE,  1, 0, 'Marines, Sergeants, attack!', 14, 0, 0, 0, 0, 16956, 'Muradin Bronzebeard - INVISIBLE_1'),
(@MURADIN,  0, 0, 'Fire up the engines! We got a meetin'' with destiny, lads!', 14, 0, 100, 0, 0, 16962, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_0'),
(@MURADIN,  1, 0, 'Hold on to yer hats!', 14, 0, 100, 0, 0, 16963, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_1'),
(@MURADIN,  2, 0, 'What in the world is that? Grab me spyglass, crewman!', 14, 0, 100, 0, 0, 16964, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_2'),
(@MURADIN,  3, 0, 'By me own beard! HORDE SAILIN'' IN FAST ''N HOT!', 14, 0, 100, 0, 0, 16965, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_3'),
(@MURADIN,  4, 0, 'EVASIVE ACTION! MAN THE GUNS!', 14, 0, 100, 0, 0, 16966, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_4'),
(@MURADIN,  5, 0, 'Cowardly dogs! Ye blindsided us!', 14, 0, 100, 0, 0, 16967, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_5'),
(@MURADIN,  6, 0, 'Not me battle? I dunnae who ye? think ye are, mister, but I got a score to settle with Arthas and yer not gettin'' in me way! FIRE ALL GUNS! FIRE! FIRE!', 14, 0, 100, 0, 0, 16968, 'Muradin Bronzebeard - SAY_INTRO_ALLIANCE_7'),
(@MURADIN,  7, 0, 'Move yer jalopy or we''ll blow it out of the sky, orc! The Horde''s got no business here!', 14, 0, 100, 0, 0, 16969, 'Muradin Bronzebeard - SAY_INTRO_HORDE_3'),
(@MURADIN,  8, 0, 'What''s this then?! Ye won''t be takin'' this son o'' Ironforge''s vessel without a fight!.', 14, 0, 100, 0, 0, 16958, 'Muradin Bronzebeard - SAY_BOARDING_SKYBREAKER_1'),
(@MURADIN,  9, 0, 'Marines, Sergeants, attack!', 14, 0, 100, 0, 0, 16956, 'Muradin Bronzebeard - SAY_BOARDING_ORGRIMS_HAMMER_0'),
(@MURADIN, 10, 0, 'Riflemen, shoot faster!', 14, 0, 100, 0, 0, 16954, 'Muradin Bronzebeard - SAY_NEW_RIFLEMEN_SPAWNED'),
(@MURADIN, 11, 0, 'Mortar team, reload!', 14, 0, 100, 0, 0, 16955, 'Muradin Bronzebeard - SAY_NEW_MORTAR_TEAM_SPAWNED'),
(@MURADIN, 12, 0, 'We''re taking hull damage, get a sorcerer out here to shut down those cannons!', 14, 0, 100, 0, 0, 16957, 'Muradin Bronzebeard - SAY_NEW_MAGE_SPAWNED'),
(@MURADIN, 13, 0, 'Don''t say I didn''t warn ya, scoundrels! Onward, brothers and sisters!', 14, 0, 100, 0, 0, 16959, 'Muradin Bronzebeard - SAY_ALLIANCE_VICTORY'),
(@MURADIN, 14, 0, 'Captain Bartlett, get us out of here! We''re taken too much damage to stay afloat!', 14, 0, 100, 0, 0, 16960, 'Muradin Bronzebeard - SAY_ALLIANCE_DEFEAT'),
(@OVERLORD_INVISIBLE, 0, 0, 'This is not your battle, dwarf. Back down or we will be forced to destroy your ship.', 14, 0, 0, 0, 0, 17093, 'High Overlord Saurfang - INVISIBLE_0'),
(@OVERLORD_INVISIBLE, 1, 0, 'Reavers, Sergeants, attack!', 14, 0, 0, 0, 0, 17081, 'High Overlord Saurfang - INVISIBLE_1'),
(@OVERLORD, 0, 0, 'Rise up, sons and daughters of the Horde! Today we battle a hated enemy of the Horde! LOK''TAR OGAR!', 14, 0, 100, 0, 0, 17087, 'High Overlord Saurfang - SAY_INTRO_HORDE_0'),
(@OVERLORD, 1, 0, 'Kor''kron, take us out!', 14, 0, 100, 0, 0, 17088, 'High Overlord Saurfang - SAY_INTRO_HORDE_1_1'),
(@OVERLORD, 2, 0, 'What is that?! Something approaching in the distance!', 14, 0, 100, 0, 0, 17089, 'High Overlord Saurfang - SAY_INTRO_HORDE_1'),
(@OVERLORD, 3, 0, 'ALLIANCE GUNSHIP! ALL HANDS ON DECK!', 14, 0, 100, 0, 0, 17090, 'High Overlord Saurfang - SAY_INTRO_HORDE_2'),
(@OVERLORD, 4, 0, 'You will know our business soon! KOR''KRON, ANNIHILATE THEM!', 14, 0, 100, 0, 0, 17092, 'High Overlord Saurfang - SAY_INTRO_HORDE_4'),
(@OVERLORD, 5, 0, 'Reavers, Sergeants, attack!', 14, 0, 100, 0, 0, 17081, 'High Overlord Saurfang - SAY_BOARDING_SKYBREAKER_0'),
(@OVERLORD, 6, 0, 'You DARE board my ship? Your death will come swiftly.', 14, 0, 100, 0, 0, 17083, 'High Overlord Saurfang - SAY_BOARDING_ORGRIMS_HAMMER_1'),
(@OVERLORD, 7, 0, 'Axethrowers, hurl faster!', 14, 0, 100, 0, 0, 17079, 'High Overlord Saurfang - SAY_NEW_AXETHROWER_SPAWNED'),
(@OVERLORD, 8, 0, 'Rocketeers, reload!', 14, 0, 100, 0, 0, 17080, 'High Overlord Saurfang - SAY_NEW_ROCKETEERS_SPAWNED'),
(@OVERLORD, 9, 0, 'We''re taking hull damage, get a battle-mage out here to shut down those cannons!', 14, 0, 100, 0, 0, 17082, 'High Overlord Saurfang - SAY_NEW_BATTLE_MAGE_SPAWNED'),
(@OVERLORD, 10, 0, 'The Alliance falter. Onward to the Lich King!', 14, 0, 100, 0, 0, 17084, 'High Overlord Saurfang - SAY_HORDE_VICTORY'),
(@OVERLORD, 11, 0, 'Damage control! Put those fires out! You haven''t seen the last of the Horde!', 14, 0, 100, 0, 0, 17085, 'High Overlord Saurfang - SAY_HORDE_DEFEAT'),
(@OVERLORD, 12, 0, 'This is not your battle, dwarf. Back down or we will be forced to destroy your ship.', 14, 0, 100, 0, 0, 17093, 'High Overlord Saurfang - SAY_INTRO_ALLIANCE_6'),

(@PRIMALIST,0, 0, 'Thank the spirits for you, brothers and sisters. The Skybreaker has already left. Quickly now, to Orgrim''s Hammer! If you leave soon, you may be able to catch them.', 12, 0, 100, 0, 0, @SOUNDID, 'Kor''kron Primalist - SAY_FIRST_SQUAD_RESCUED_HORDE_0'),
(@INVOKER,  0, 0, 'This should be helpin''ya!', 12, 0, 100, 0, 0, @SOUNDID, 'Kor''kron Invoker - SAY_FIRST_SQUAD_RESCUED_HORDE_1'),
(@INVOKER,  1, 0, '%s summons a Kor''kron Battle Standard.', 16, 0, 100, 0, 0, @SOUNDID, 'Kor''kron Invoker - SAY_SUMMON_BATTLE_STANDARD'),
(@DEFENDER, 0, 0, 'Aka''Magosh, brave warriors. The alliance is in great number here.', 12, 0, 100, 0, 0, @SOUNDID, 'Kor''kron Defender - SAY_SECOND_SQUAD_RESCUED_HORDE_0'),
(@DEFENDER, 1, 0, 'Captain Saurfang will be pleased to see you aboard Orgrim''s Hammer. Make haste, we will secure the area until you are ready for take-off.', 12, 0, 100, 0, 0, @SOUNDID, 'Kor''kron Defender - SAY_SECOND_SQUAD_RESCUED_HORDE_1'),
(@DEFENDER, 2, 0, 'A screeching cry pierces the air above!',41, 0, 100, 0, 0, @SOUNDID, 'Frostwyrm - SAY_FROSTWYRM_SUMMON_0'),
(@VINDICATOR, 0, 0, 'Thank goodness you arrived when you did, heroes. Orgrim''s Hammer has already left. Quickly now, to The Skybreaker! If you leave soon, you may be able to catch them.', 12, 0, 100, 0, 0, @SOUNDID, 'Skybreaker Vindicator - SAY_FIRST_SQUAD_RESCUED_ALLIANCE_0'),
(@SORCERER, 0, 0, 'This ought to help!', 12, 0, 100, 0, 0, @SOUNDID, 'Skybreaker Sorcerer - SAY_FIRST_SQUAD_RESCUED_ALLIANCE_1'),
(@SORCERER, 1, 0, '%s summons a Skybreaker Battle Standard.', 16, 0, 100, 0, 0, @SOUNDID, 'Skybreaker Sorcerer - SAY_SUMMON_BATTLE_STANDARD'),
(@PROTECTOR,0, 0, 'You have my thanks. We were outnumbered until you arrived.', 12, 0, 100, 0, 0, @SOUNDID, 'Skybreaker Protector - SAY_SECOND_SQUAD_RESCUED_ALLIANCE_0'),
(@PROTECTOR,1, 0, 'Captain Muradin is waiting aboard The Skybreaker. We''ll secure the area until you are ready for take off.', 12, 0, 100, 0, 0, @SOUNDID, 'Skybreaker Protector - SAY_SECOND_SQUAD_RESCUED_ALLIANCE_1'),
(@PROTECTOR,2, 0, 'Skybreaker infantry, hold position!', 12, 0, 100, 0, 0, @SOUNDID, 'Skybreaker Protector - SAY_SECOND_SQUAD_RESCUED_ALLIANCE_2'),
(@PROTECTOR,3, 0, 'A screeching cry pierces the air above!',41, 0, 100, 0, 0, @SOUNDID, 'Frostwyrm - SAY_FROSTWYRM_SUMMON_0'),
(@FROSTWYRM,0, 0, 'A Spire Frostwyrm lands just before Orgrim''s Hammer.', 16, 0, 100, 0, 0, @SOUNDID, 'Frostwyrm - SAY_FROSTWYRM_LAND_H_1'),
(@FROSTWYRM,1, 0, 'A Spire Frostwyrm lands just before The Skybreaker. ', 16, 0, 100, 0, 0, @SOUNDID, 'Frostwyrm - SAY_FROSTWYRM_LAND_A_2');

-- CONDITIONS
-- Additionnal Conditions (by 2010phenix - https://github.com/2010phenix)
SET @ENTRYA := 37540;
SET @ENTRYH := 37215;
SET @ENTRYA10H := 38128;
SET @ENTRYH10H := 38129;
SET @ENTRYA25 := 38699;
SET @ENTRYH25 := 38701;
SET @ENTRYA25H := 38700;
SET @ENTRYH25H := 38702;

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry` IN (69400, 70173, 69402, 70175, 70374, 70383, 72959, 69705);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`) VALUES
('13', '69400', '18', '1', @ENTRYA),    -- Horde Cannon (Cannon Blast) set to hit only Skybreaker (effect 2)
('13', '69400', '18', '1', @ENTRYA10H), -- Horde Cannon (Cannon Blast) set to hit only Skybreaker - 10H (effect 2)
('13', '69400', '18', '1', @ENTRYA25),  -- Horde Cannon (Cannon Blast) set to hit only Skybreaker - 25Normal (effect 2)
('13', '69400', '18', '1', @ENTRYA25H), -- Horde Cannon (Cannon Blast) set to hit only Skybreaker -- 25H (effect 2)
('13', '70173', '18', '1', @ENTRYH),    -- Alliance Cannon (Cannon Blast) set to hit only Orgrimms Hammer (effect 2)
('13', '70173', '18', '1', @ENTRYH10H), -- Alliance Cannon (Cannon Blast) set to hit only Orgrimms Hammer 10H (effect 2)
('13', '70173', '18', '1', @ENTRYH25),  -- Alliance Cannon (Cannon Blast) set to hit only Orgrimms Hammer - 25Normal (effect 2)
('13', '70173', '18', '1', @ENTRYH25H), -- Alliance Cannon (Cannon Blast) set to hit only Orgrimms Hammer -- 25H (effect 2)
('13', '69402', '18', '1', @ENTRYA),    -- Horde Cannon (Incinerating Blast) set to hit only Skybreaker (effect 2)
('13', '69402', '18', '1', @ENTRYA10H), -- Horde Cannon (Incinerating Blast) set to hit only Skybreaker 10H(effect 2)
('13', '69402', '18', '1', @ENTRYA25),  -- Horde Cannon (Incinerating Blast) set to hit only Skybreaker - 25Normal (effect 2)
('13', '69402', '18', '1', @ENTRYA25H), -- Horde Cannon (Incinerating Blast) set to hit only Skybreaker -- 25H (effect 2)
('13', '70175', '18', '1', @ENTRYH),    -- Alliance Cannon (Incinerating Blast) set to hit only Orgrimms Hammer (effect 2)
('13', '70175', '18', '1', @ENTRYH10H), -- Alliance Cannon (Incinerating Blast) set to hit only Orgrimms Hammer 10H (effect 2)
('13', '70175', '18', '1', @ENTRYH25),  -- Alliance Cannon (Incinerating Blast) set to hit only Orgrimms Hammer - 25Normal (effect 2)
('13', '70175', '18', '1', @ENTRYH25H), -- Alliance Cannon (Incinerating Blast) set to hit only Orgrimms Hammer -- 25H (effect 2)
('13', '70374', '18', '1', @ENTRYA),    -- Burning Pitch
('13', '70374', '18', '1', @ENTRYA10H), -- Burning Pitch
('13', '70374', '18', '1', @ENTRYA25),  -- Burning Pitch
('13', '70374', '18', '1', @ENTRYA25H), -- Burning Pitch
('13', '70383', '18', '1', @ENTRYH),    -- Burning Pitch
('13', '70383', '18', '1', @ENTRYH10H), -- Burning Pitch
('13', '70383', '18', '1', @ENTRYH25),  -- Burning Pitch
('13', '70383', '18', '1', @ENTRYH25H), -- Burning Pitch
('13', '72959', '18', '1', 0),          -- Achievement (Radius: 50000 yards)
('13', '69705' ,'18', '1', 36838),      -- Gunship Battle - Spell 69705 (Below Zero) target creature 36838
('13', '69705' ,'18', '1', 36839);      -- Gunship Battle - Spell 69705 (Below Zero) target creature 36839

-- Update condition by TrinityCore:
DELETE FROM conditions WHERE SourceTypeOrReferenceId=13 AND SourceEntry IN (69400, 70173, 69402, 70175, 70374, 70383, 69705, 72959);
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorTextId, ScriptName, Comment) VALUES
-- Alliance
(13, 3, 69400, 0, 0, 31, 0, 3, 37540, 0, 0, 0, '', NULL),
(13, 1, 70374, 0, 0, 31, 0, 3, 37540, 0, 0, 0, '', NULL),
(13, 3, 69402, 0, 0, 31, 0, 3, 37540, 0, 0, 0, '', NULL),
(13, 1, 69705, 0, 0, 31, 0, 3, 36838, 0, 0, 0, '', NULL),
-- Horde
(13, 3, 70175, 0, 1, 31, 0, 3, 37215, 0, 0, 0, '', NULL),
(13, 1, 70383, 0, 1, 31, 0, 3, 37215, 0, 0, 0, '', NULL),
(13, 3, 70173, 0, 1, 31, 0, 3, 37215, 0, 0, 0, '', NULL),
(13, 1, 69705, 0, 1, 31, 0, 3, 36839, 0, 0, 0, '', NULL);



-- Loot templates
-- Fix by TrinityCore
-- Set Variables
SET @Gunship10N := 28057; -- Data1 for 201872 & 202177
SET @Gunship10H := 28045; -- Data1 for 201873 & 202178
SET @Gunship25N := 28072; -- Data1 for 201874 & 202179
SET @Gunship25H := 28090; -- Data1 for 201875 & 202180
SET @emblem := 49426; -- Emblem of Frost
SET @SfShard := 50274; -- Shadowfrost Shard
SET @PriSar := 49908; -- Primordial Saronite
-- Dug up the old references i created for UP34:
SET @Ref10J := 34329;
SET @Ref25J := 34251;
SET @RefJ10H := 34263;
SET @RefJ25H := 34275;

-- Gunship_Armory10N_reference
DELETE FROM `reference_loot_template` WHERE `entry` IN (@Ref10J,@Ref25J,@RefJ10H,@RefJ25H);
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(@Ref10J,50791,0,1,1,1,1), -- Saronite Gargoyle Cloak
(@Ref10J,50795,0,1,1,1,1), -- Cord of Dark Suffering
(@Ref10J,50794,0,1,1,1,1), -- Neverending Winter
(@Ref10J,50787,0,1,1,1,1), -- Frost Giant's Cleaver
(@Ref10J,50793,0,1,1,1,1), -- Midnight Sun
(@Ref10J,50788,0,1,1,1,1), -- Bone Drake's Enameled Boots
(@Ref10J,50792,0,1,1,1,1), -- Pauldrons of Lost Hope
(@Ref10J,50340,0,1,1,1,1), -- Muradin's Spyglass
(@Ref10J,50797,0,1,1,1,1), -- Ice-Reinforced Vrykul Helm
(@Ref10J,50790,0,1,1,1,1), -- Abomination's Bloody Ring
(@Ref10J,50789,0,1,1,1,1), -- Icecrown Rampart Bracers
(@Ref10J,50796,0,1,1,1,1), -- Bracers of Pale Illumination
-- Gunship_Armory25N_reference
(@Ref25J,50011,0,1,1,1,1), -- Gunship Captain's Mittens
(@Ref25J,50002,0,1,1,1,1), -- Polar Bear Claw Bracers
(@Ref25J,50006,0,1,1,1,1), -- Corp'rethar Ceremonial Crown
(@Ref25J,50010,0,1,1,1,1), -- Waistband of Righteous Fury
(@Ref25J,50003,0,1,1,1,1), -- Boneguard Commander's Pauldrons
(@Ref25J,50000,0,1,1,1,1), -- Scourge Hunter's Vambraces
(@Ref25J,50359,0,1,1,1,1), -- Althor's Abacus
(@Ref25J,50352,0,1,1,1,1), -- Corpse Tongue Coin
(@Ref25J,49999,0,1,1,1,1), -- Skeleton Lord's Circle
(@Ref25J,50009,0,1,1,1,1), -- Boots of Unnatural Growth
(@Ref25J,50008,0,1,1,1,1), -- Ring of Rapid Ascent
(@Ref25J,49998,0,1,1,1,1), -- Shadowvault Slayer's Cloak
(@Ref25J,50005,0,1,1,1,1), -- Amulet of the Silent Eulogy
(@Ref25J,50411,0,1,1,1,1), -- Scourgeborne Waraxe
(@Ref25J,50001,0,1,1,1,1), -- Ikfirus's Sack of Wonder
-- Gunship_Armory10H_reference
(@RefJ10H,51912,0,1,1,1,1), -- Saronite Gargoyle Cloak (heroic)
(@RefJ10H,51908,0,1,1,1,1), -- Cord of Dark Suffering (heroic)
(@RefJ10H,51909,0,1,1,1,1), -- Neverending Winter (heroic)
(@RefJ10H,51916,0,1,1,1,1), -- Frost Giant's Cleaver (heroic)
(@RefJ10H,51910,0,1,1,1,1), -- Midnight Sun (heroic)
(@RefJ10H,51915,0,1,1,1,1), -- Bone Drake's Enameled Boots (heroic)
(@RefJ10H,51911,0,1,1,1,1), -- Pauldrons of Lost Hope (heroic)
(@RefJ10H,50345,0,1,1,1,1), -- Muradin's Spyglass (heroic)
(@RefJ10H,51906,0,1,1,1,1), -- Ice-Reinforced Vrykul Helm (heroic)
(@RefJ10H,51913,0,1,1,1,1), -- Abomination's Bloody Ring (heroic)
(@RefJ10H,51914,0,1,1,1,1), -- Icecrown Rampart Bracers (heroic)
(@RefJ10H,51907,0,1,1,1,1), -- Bracers of Pale Illumination (heroic)
-- Gunship_Armory25H_reference
(@RefJ25H,50663,0,1,1,1,1), -- Gunship Captain's Mittens (heroic)
(@RefJ25H,50659,0,1,1,1,1), -- Polar Bear Claw Bracers (heroic)
(@RefJ25H,50661,0,1,1,1,1), -- Corp'rethar Ceremonial Crown (heroic)
(@RefJ25H,50667,0,1,1,1,1), -- Waistband of Righteous Fury (heroic)
(@RefJ25H,50660,0,1,1,1,1), -- Boneguard Commander's Pauldrons (heroic)
(@RefJ25H,50655,0,1,1,1,1), -- Scourge Hunter's Vambraces (heroic)
(@RefJ25H,50366,0,1,1,1,1), -- Althor's Abacus (heroic)
(@RefJ25H,50349,0,1,1,1,1), -- Corpse Tongue Coin (heroic)
(@RefJ25H,50657,0,1,1,1,1), -- Skeleton Lord's Circle (heroic)
(@RefJ25H,50665,0,1,1,1,1), -- Boots of Unnatural Growth (heroic)
(@RefJ25H,50664,0,1,1,1,1), -- Ring of Rapid Ascent (heroic)
(@RefJ25H,50653,0,1,1,1,1), -- Shadowvault Slayer's Cloak (heroic)
(@RefJ25H,50658,0,1,1,1,1), -- Amulet of the Silent Eulogy (heroic)
(@RefJ25H,50654,0,1,1,1,1), -- Scourgeborne Waraxe (heroic)
(@RefJ25H,50656,0,1,1,1,1); -- Ikfirus's Sack of Wonder (heroic)

-- Bind the refs to the objects
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (@Gunship10N,@Gunship10H,@Gunship25N,@Gunship25H);
INSERT INTO `gameobject_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
-- Gunship10N
(@Gunship10N,@emblem,100,1,0,2,2), -- Gunship Armory 2xemblems 10N
(@Gunship10N,1,100,1,0,-@Ref10J,2), -- 2 from reference 10Normal
-- Gunship25N
(@Gunship25N,@emblem,100,1,0,2,2), -- Gunship Armory 2xemblems 25N
(@Gunship25N,1,100,1,0,-@Ref25J,2), -- 3 from reference 25Normal
(@Gunship25N,@SfShard,-35,1,0,1,1), -- Shadowfrost Shard @ 38% chance
(@Gunship25N,@PriSar,38,1,0,1,1), -- Primordial Saronite @ 10% chance
-- Gunship10H
(@Gunship10H,@emblem,100,1,0,2,2), -- Gunship Armory 2xemblems 10H
(@Gunship10H,1,100,1,0,-@RefJ10H,2), -- 2 from reference 10Heroic
(@Gunship10H,@PriSar,38,1,0,1,1), -- Primordial Saronite @ 38% chance
-- Gunship25H
(@Gunship25H,@emblem,100,1,0,2,2), -- Gunship Armory 2xemblems 25H
(@Gunship25H,1,100,1,0,-@RefJ25H,2), -- 3 from reference 25Heroic
(@Gunship25H,@SfShard,-75,1,0,1,1), -- Shadowfrost Shard @ 75% chance
(@Gunship25H,@PriSar,50,1,0,1,1); -- Primordial Saronite @ 50% chance

SET @Twins := 34329; 
SET @emblem := 49426;
DELETE FROM `reference_loot_template` WHERE `entry`=@Twins;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@Twins,47296,0,1,1,1,1), -- Greaves of Ruthless Judgment
(@Twins,47297,0,1,1,1,1), -- The Executioner's Vice
(@Twins,47298,0,1,1,1,1), -- Armguards of the Shieldmaiden
(@Twins,47299,0,1,1,1,1), -- Belt of the Pitiless Killer
(@Twins,47300,0,1,1,1,1), -- Gouge of the Frigid Heart
(@Twins,47301,0,1,1,1,1), -- Skyweaver Vestments
(@Twins,47302,0,1,1,1,1), -- Twin's Pact
(@Twins,47303,0,1,1,1,1), -- Death's Choice
(@Twins,47304,0,1,1,1,1), -- Legplates of Ascension
(@Twins,47305,0,1,1,1,1); -- Legionnaire's Gorget
SET @Gunship10N := 28057; -- Data1 for 201872 & 202177
SET @Ref10J := 12036;
-- Gunship_Armory10N_reference
DELETE FROM `reference_loot_template` WHERE `entry`=@Ref10J;
INSERT INTO `reference_loot_template` (`entry`,`item`,`ChanceOrQuestChance`,`lootmode`,`groupid`,`mincountOrRef`,`maxcount`) VALUES
(@Ref10J,50791,0,1,1,1,1), -- Saronite Gargoyle Cloak
(@Ref10J,50795,0,1,1,1,1), -- Cord of Dark Suffering
(@Ref10J,50794,0,1,1,1,1), -- Neverending Winter
(@Ref10J,50787,0,1,1,1,1), -- Frost Giant's Cleaver
(@Ref10J,50793,0,1,1,1,1), -- Midnight Sun
(@Ref10J,50788,0,1,1,1,1), -- Bone Drake's Enameled Boots
(@Ref10J,50792,0,1,1,1,1), -- Pauldrons of Lost Hope
(@Ref10J,50340,0,1,1,1,1), -- Muradin's Spyglass
(@Ref10J,50797,0,1,1,1,1), -- Ice-Reinforced Vrykul Helm
(@Ref10J,50790,0,1,1,1,1), -- Abomination's Bloody Ring
(@Ref10J,50789,0,1,1,1,1), -- Icecrown Rampart Bracers
(@Ref10J,50796,0,1,1,1,1); -- Bracers of Pale Illumination

-- DELETE FROM `creature_addon` WHERE `guid` IN (83459,78383,81170,81165,81531);
DELETE FROM `gameobject_loot_template` WHERE `entry` IN (28045,28057,28072,28090);
DELETE FROM `reference_loot_template` WHERE `entry` IN (34329,12036,34251,34263,34275);

-- Make Muradin and Saufangs unattackable and unable to attack players (All difficulty modes)
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=36948;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=36939;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=38156;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=38638;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=38637;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=38157;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=38639;
UPDATE `creature_template` SET `unit_flags`=2, `flags_extra`=2 WHERE `entry`=38640;

-- Prevent players from exploiting the Rocket pack item outside of raid
UPDATE `item_template` SET `area` = 4812 WHERE `entry` = 49278;
UPDATE `item_template` SET `map` = 631 WHERE `entry` = 49278;

-- 631
UPDATE `creature_template` SET `speed_walk` = 1.14286, `speed_run` = 1 WHERE `entry` = 39189;
UPDATE `creature_template` SET `ScriptName` = 'npc_lich_king_spirit' WHERE `entry` IN (39190, 37799);

DELETE FROM `conditions` WHERE `SourceEntry` IN (74302,74342);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`) VALUES
(13, 1, 74302, 31, 4),
(13, 1, 74342, 31, 4);

-- 572
UPDATE `creature` SET `phaseMask` = 16 WHERE `id` = 37950;
UPDATE `creature_template` SET `flags_extra` = 0 WHERE `entry` = 37950;

UPDATE `creature_template` SET mechanic_immune_mask = mechanic_immune_mask|2|8|32|64|256|512|2048|4096|8192|65536|131072|524288|4194304|33554432|536870912 WHERE `entry` IN (36791,38169,38721,38722,37863,38171,38727,38737,37868,38167,38725,38735,37886,38166,38724,38734,37934,38170,38723,38733);

DELETE FROM `creature_template_addon` WHERE `entry` IN (37502,37501,37232);

DELETE FROM `creature` WHERE `id` = 37868;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
('37868','631','15','1','0','0','4230.53','2490.22','364.957','3.36849','604800','0','0','0','0','0','0','0','0'),
('37868','631','15','1','0','0','4230.44','2478.56','364.953','2.93215','604800','0','0','0','0','0','0','0','0'),
('37868','631','15','1','0','0','4223.4','2465.11','364.952','2.3911','604800','0','0','0','0','0','0','0','0'),
('37868','631','15','1','0','0','4222.86','2504.58','364.96','3.90954','604800','0','0','0','0','0','0','0','0');

-- DELETE FROM `creature_addon` WHERE `guid` IN (47622,47618,62056,77143,75886,61780);

-- 463
UPDATE `creature_template` SET `speed_run` = 1, `flags_extra` = 0 WHERE `entry` IN (37799,39284,39285,39286);

-- 385
DELETE FROM `creature` WHERE `id`=39190;
INSERT INTO `creature` (`id`,`map`,`spawnMask`,`phaseMask`,`position_x`,`position_y`,`position_z`,`orientation`,`spawntimesecs`,`spawndist`,`MovementType`) VALUES
(39190,631,12,1,505.1563,-2536.863,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,502.6493,-2531.328,1069.598,0,10,10,1), -- Wicked Spirit
(9190,631,12,1,503.8246,-2508.141,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,495.3629,-2538.215,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,506.8715,-2515.552,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,481.3021,-2509.222,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,481.5382,-2527.332,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,500.0807,-2523.289,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,486.6632,-2517.747,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,513.8802,-2511.795,1069.596,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,499.6406,-2500.783,1069.656,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,513.7101,-2530.931,1069.596,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,502.6811,-2522.542,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,518.7483,-2521.240,1069.596,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,478.9774,-2517.446,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,489.0104,-2538.535,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,511.6320,-2522.373,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,500.8976,-2527.198,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,484.7934,-2533.592,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,490.4132,-2504.323,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,485.0298,-2527.724,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,494.2899,-2511.384,1069.598,0,10,10,1), -- Wicked Spirit
(39190,631,12,1,491.7170,-2532.490,1069.598,0,10,10,1); -- Wicked Spirit

UPDATE `creature_template` SET `InhabitType` = 5, `unit_flags` = 32832, `speed_walk` = 2.8, `speed_run` = 1 WHERE `entry` IN (39190,39287,39288,39289);

-- 380
DELETE FROM `creature_template_addon` WHERE `entry` IN (37502,38198);

-- 359
UPDATE `creature_template` SET `faction_A` = 1665, `faction_H` = 1665 WHERE `entry` IN (36823);

-- 327
UPDATE `creature_template` SET `mindmg`=488, `maxdmg`=642, `dmgschool`=0, `attackpower`=782,
`baseattacktime`=0, `rangeattacktime`=0, `minrangedmg`=363, `maxrangedmg`=521, `rangedattackpower`=121 WHERE `entry` IN (38508, 38596, 38597, 38598);

UPDATE `creature_template` SET `dmg_multiplier` = 21 WHERE `entry` = 38508;
UPDATE `creature_template` SET `dmg_multiplier` = 27 WHERE `entry` IN (38596, 38597);
UPDATE `creature_template` SET `dmg_multiplier` = 32 WHERE `entry` = 38598;

-- 225
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 13 AND `SourceGroup` = 3 AND `SourceEntry` IN (69783, 69797, 69799, 69802) AND `ConditionTypeOrReference` = 33;
INSERT INTO `conditions` (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorTextId, ScriptName, COMMENT) VALUES
(13, 3, 69783, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self'),
(13, 3, 69797, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self'),
(13, 3, 69799, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self'),
(13, 3, 69802, 0, 0, 33, 1, 0, 0, 0, 1, 0, '', 'Rotface - Ooze Flood, not self');

DELETE FROM `creature` WHERE `id` = 37013;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
('37013','631','15','1','0','0','4312.14','3112.98','372.97','2.51327','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4428.96','3173.66','360.469','5.13127','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4490.13','3160.97','372.312','3.82227','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4401.32','3160.83','372.316','5.55015','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4223.47','3113.58','372.97','0.767945','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4469.68','3092.91','372.325','2.35619','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4421.79','3181.76','372.29','5.46288','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4428.15','3101.17','360.469','1.11701','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4222.44','3161.69','372.97','5.53269','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4463.88','3172.66','360.469','4.24115','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4480.85','3118.98','360.469','2.67035','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4469.67','3181.77','372.311','4.11898','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4419.03','3109.9','360.469','0.767945','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4243.89','3181.74','372.97','5.44543','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4471.93','3163.9','360.469','3.92699','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4401.28','3112.96','372.431','0.785398','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4419.42','3164.15','360.469','5.46288','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4409.69','3119.34','360.469','0.471239','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4291.18','3092.92','372.97','2.33874','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4481.93','3153.87','360.469','3.57792','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4490.33','3113.29','372.411','2.53073','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4312.36','3160.84','372.97','3.80482','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4411.07','3154.75','360.469','5.81195','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4291.45','3181.25','372.97','4.10152','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4463.86','3101.57','360.469','2.04204','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4244.04','3092.66','372.97','0.977384','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4472.55','3110.78','360.469','2.35619','86400','0','0','25200','0','0','0','0','0'),
('37013','631','15','1','0','0','4421.9','3092.42','372.456','0.994838','86400','0','0','25200','0','0','0','0','0');

-- 200
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=13 AND `SourceEntry`=69508;

DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_rotface_slime_spray';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(69507, 'spell_rotface_slime_spray'),
(71213, 'spell_rotface_slime_spray'),
(73189, 'spell_rotface_slime_spray'),
(73190, 'spell_rotface_slime_spray');

UPDATE `creature_template` SET `mechanic_immune_mask`=1071595519,`mindmg`=12000,`maxdmg`= 15000,`dmg_multiplier`= 4,attackpower=40250,speed_walk=0.65,speed_run=0.65 WHERE `entry` = 36899; -- Big Ooze 10
UPDATE `creature_template` SET `mechanic_immune_mask`=1071595519,`mindmg`=12000,`maxdmg`= 15000,`dmg_multiplier`= 5,attackpower=40250,speed_walk=0.65,speed_run=0.65 WHERE `entry` = 38123; -- Big Ooze 25
UPDATE `creature_template` SET `mindmg`=509,`maxdmg`=683,`attackpower`=805,`dmg_multiplier`=20,`mechanic_immune_mask`=1071594495,`flags_extra`=256 WHERE `entry`=36897; -- Little Ooze 10
UPDATE `creature_template` SET `mindmg`=509,`maxdmg`=683,`attackpower`=805,`dmg_multiplier`=25,`mechanic_immune_mask`=1071594495,`flags_extra`=256 WHERE `entry`=38138; -- Little Ooze 25

-- 127
UPDATE `creature_template` SET `attackpower` = 287 WHERE `entry` IN 
(37127,38126);

UPDATE `creature_template` SET `attackpower` = 326 WHERE `entry` IN 
(36805,38075,36807,38073,37949,38394,38625,38626);

UPDATE `creature_template` SET `attackpower` = 343 WHERE `entry` IN 
(36855,38106,38296,38297);

UPDATE `creature_template` SET `attackpower` = 608 WHERE `entry` IN 
(36724,38057,38136,38396,38632,38633,38010,38397,39000,39001,38125,38130,37134,38131);

UPDATE `creature_template` SET `attackpower` = 642 WHERE `entry` IN 
(37012,38059,38009,38398,38630,38631,38135,38395,38634,38635,37672,38605,38786,38787,
38285,38788,38789,38790,37132,38132,37133,38133,37863,38171,38727,38737,
37934,38170,38723,38733,37907,38168,38726,38736);

UPDATE `creature_template` SET `attackpower` = 670 WHERE `entry` IN 
(37122,38298,37123,38299,37491,37491,37493,38350,37868,38167,38725,38735);


UPDATE `creature_template` SET `attackpower` = 708 WHERE `entry` IN 
(37124,38303,37125,38304,37494,38351,37495,38352);

UPDATE `creature_template` SET `attackpower` = 726 WHERE `entry` IN 
(36829,38074,37126,38258,37534,38219,37533,38220);

UPDATE `creature_template` SET `attackpower` = 782 WHERE `entry` IN 
(37007,38031,36808,38076,37890,38393,38628,38629,38508,38596,38597,38598,36791,38169,38722,38721);

UPDATE `creature_template` SET `attackpower` = 805 WHERE `entry` IN 
(36612,37957,37958,37959,37813,38402,38582,38583,
36626,37504,37505,37506,36627,38390,38549,38550,
36678,38431,38585,38586,37970,38401,38784,38785,
37972,38399,38769,38770,37973,38400,38771,38772,
37955,38434,38435,38436,36853,38265,38266,38267,
37129,38000,37886,38166,38724,38734);

-- 109
UPDATE `creature_template` SET `speed_walk` = 0.642857, `speed_run` = 0.32132, `mechanic_immune_mask` = 617296767 WHERE `entry` IN (36609,39120,39121,39122);

UPDATE `creature_template` SET `InhabitType` = 5, `flags_extra` = 256 WHERE `entry` IN (37799,39284,39285,39286);

DELETE FROM `spell_script_names` WHERE `spell_id` = 74352;
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES
(74352, 'spell_the_lich_king_wings_of_the_damned');

-- 104
UPDATE `creature` SET `spawntimesecs` = 604800 WHERE `id` = 36612;

UPDATE `creature_template` SET ScriptName = 'npc_ymirjar_deathbringer' WHERE entry = 38125;
UPDATE `creature_template` SET ScriptName = 'npc_ymirjar_frostbinder' WHERE entry = 37127;
UPDATE `creature_template` SET ScriptName = 'npc_ymirjar_battlemaiden' WHERE entry = 37132;
UPDATE `creature_template` SET ScriptName = 'npc_ymirjar_huntress' WHERE entry = 37134;
UPDATE `creature_template` SET ScriptName = 'npc_ymirjar_warlord' WHERE entry = 37133;

DELETE FROM `creature_loot_template` WHERE `entry` IN (37126,38258) AND `item` = 49426;
INSERT INTO `creature_loot_template` VALUES
(37126, 49426, 100, 1, 0, 1, 1),
(38258, 49426, 100, 1, 0, 1, 1);

UPDATE creature_template SET `flags_extra` = 1, `mechanic_immune_mask` = 617298815 WHERE `entry` IN (37126,38258);
UPDATE creature_template SET `InhabitType` = 4, `HoverHeight` = 12 WHERE `entry` = 38258;

DELETE FROM `creature_template_addon` WHERE `entry` IN (37122,37123,37124,37125,38298,38299,38303,38304);

-- 95
UPDATE `creature_template` SET `mechanic_immune_mask` = 617298815, `flags_extra` = 1, `unit_flags` = 320 WHERE `entry` IN (37533, 38220);
UPDATE `creature_template` SET `mechanic_immune_mask` = 617298815, `flags_extra` = 1, `unit_flags` = 66 WHERE `entry` IN (37534, 38219);
UPDATE `creature_template` SET `mechanic_immune_mask` = 650853247 WHERE `entry` IN (36980, 38320, 38321, 38322);

DELETE FROM `spell_proc_event` WHERE `entry` = 69762;
INSERT INTO `spell_proc_event` (entry, SchoolMask, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, procFlags, procEx, ppmRate, CustomChance, Cooldown) VALUES
(69762, 0, 0, 0, 0, 0, 81920, 0, 0, 0, 1);

-- UPDATE `creature_addon` SET `bytes1` = 50331648, `bytes2` = 1 WHERE `guid` IN (115556,115557);

UPDATE `gameobject_template` SET `flags` = 16 WHERE `entry` = 202396;

-- 93
DELETE FROM `creature` WHERE `id` = 10404;
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
('10404','631','15','1','0','0','4363.11','2907.58','351.184','4.41568','86400','0','0','630000','0','0','0','0','0'),
('10404','631','15','1','0','0','4351.22','2908.73','351.184','4.71239','86400','0','0','630000','0','0','0','0','0');

-- 74
UPDATE `achievement_criteria_data` SET `value1` = 0 WHERE `criteria_id` = 12978 AND `type`=12;
UPDATE `achievement_criteria_data` SET `value1` = 1 WHERE `criteria_id` = 12971 AND `type`=12;
UPDATE `achievement_criteria_data` SET `value1` = 2 WHERE `criteria_id` = 12979 AND `type`=12;
UPDATE `achievement_criteria_data` SET `value1` = 3 WHERE `criteria_id` = 12980 AND `type`=12;

-- 45
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_archmage' WHERE `entry` = 37664;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_blood_knight' WHERE `entry` = 37595;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_noble' WHERE `entry` = 37663;
UPDATE `creature_template` SET `ScriptName` = 'npc_vampiric_fiend' WHERE `entry` = 37901;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_advisor' WHERE `entry` = 37371;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_commander' WHERE `entry` = 37662;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_lieutenant' WHERE `entry` = 37665;
UPDATE `creature_template` SET `ScriptName` = 'npc_darkfallen_tactician' WHERE `entry` = 37666;

-- 21
UPDATE `creature_template` SET `speed_walk` = 0.25, `speed_run` = 0.15 WHERE `entry` IN (36609, 39120, 39121, 39122);