DELETE FROM `creature` WHERE `id`=47242 AND `map`=36;

-- UPDATE `creature` SET `MovementType`=0 WHERE `guid`=319043;
-- UPDATE `creature_addon` SET `path_id`=0 WHERE `guid`=319043;

DELETE FROM `waypoint_data`WHERE `id`=319043;

DELETE FROM `spell_proc_event` WHERE `entry`=88736;

UPDATE `creature_template` SET `mindmg`=532, `maxdmg`=798, `dmg_multiplier`=60 WHERE `entry` IN 
(48941, 48936, 48940, 48939, 48943, 48944, 49541);

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(48295, 0, 0, 0, 1, 0, '89734'),
(48276, 0, 0, 0, 1, 0, '89734'),
(48299, 0, 0, 0, 1, 0, '89734'),
(48293, 0, 0, 0, 1, 0, '89734'),
(48298, 0, 0, 0, 1, 0, '89734'),
(48302, 0, 0, 0, 1, 0, '89734');

-- UPDATE `creature_addon` SET `auras`='87239' WHERE `guid`=319033;

UPDATE `creature_template` SET `unit_flags`=67108928 WHERE `entry` IN (43778, 48941);

UPDATE `creature_template` SET `dynamicflags`=`dynamicflags`&~8 WHERE `entry` IN (47739, 48944);

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|0x20000000 WHERE `entry` IN (47739, 48944);

UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `spell3`=0, `spell4`=0, `ScriptName`='npc_admiral_ripsnarl_vapor' WHERE `entry`=47714;
UPDATE `creature_template` SET `spell1`=0, `spell2`=0, `spell3`=0, `spell4`=0 WHERE `entry`=49241;

DELETE FROM `spell_script_names` WHERE `spell_id` IN
(89268, 89740, 90561, 90562, 90582, 90583, 90563, 90584, 90564, 90585, 90565, 90586, 92042);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(89268, 'spell_captain_cookie_throw_food_targeting'),
(89740, 'spell_captain_cookie_throw_food_targeting'),
(90561, 'spell_captain_cookie_throw_food_targeting'),
(90562, 'spell_captain_cookie_throw_food_targeting'),
(90582, 'spell_captain_cookie_throw_food_targeting'),
(90583, 'spell_captain_cookie_throw_food_targeting'),
(90563, 'spell_captain_cookie_throw_food_targeting'),
(90584, 'spell_captain_cookie_throw_food_targeting'),
(90564, 'spell_captain_cookie_throw_food_targeting'),
(90585, 'spell_captain_cookie_throw_food_targeting'),
(90565, 'spell_captain_cookie_throw_food_targeting'),
(90586, 'spell_captain_cookie_throw_food_targeting'),
(92042, 'spell_admiral_ripsnarl_coalesce');

DELETE from `creature_text` where `entry` = 47162;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(47162, 0, 0, 'TOO...MUCH...POWER!!!', 14, 0, 0, 0, 0, 21145, 'VO_DM_GlubtokBoth_Death01'),
(47162, 1, 0, 'ARCANE POWER!!!', 14, 0, 0, 0, 0, 21146, 'VO_DM_GlubtokBoth_Spell03'),
(47162, 2, 0, 'Glubtok show you da power of de arcane!', 14, 0, 0, 0, 0, 21151, 'VO_DM_GlubtokHead1_Aggro01'),
(47162, 3, 0, 'Ha ha ha ha!', 14, 0, 0, 0, 0, 21152, 'VO_DM_GlubtokHead1_Kill01'),
(47162, 4, 0, 'Fists of flame!', 14, 0, 0, 0, 0, 21153, 'VO_DM_GlubtokHead1_Spell01'),
(47162, 5, 0, 'Glubtok ready?', 14, 0, 0, 0, 0, 21154, 'VO_DM_GlubtokHead1_Spell02'),
(47162, 3, 1, '\'Sploded dat one!', 14, 0, 0, 0, 0, 21155, 'VO_DM_GlubtokHead2_Kill01'),
(47162, 6, 0, 'Fists of frost!', 14, 0, 0, 0, 0, 21156, 'VO_DM_GlubtokHead2_Spell01'),
(47162, 7, 0, 'Let\'s do it!', 14, 0, 0, 0, 0, 21157, 'VO_DM_GlubtokHead2_Spell02');

DELETE FROM `locales_creature_text` WHERE `entry`=47162;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(47162, 0, 0, 'Слишком... Много... Силы...'),
(47162, 1, 0, 'СИЛА МААААГИИИ!'),
(47162, 2, 0, 'Глубток покажет вам силу магии!'),
(47162, 3, 0, 'Ха-ха-ха!'),
(47162, 4, 0, ''),
(47162, 5, 0, 'Глубток готов?'),
(47162, 3, 1, ''),
(47162, 6, 0, 'Ледяные кулаки!'),
(47162, 7, 0, 'Сделаем это!');

DELETE from `creature_text` where `entry` in (47296, 47297);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(47296, 0, 0, 'The scales...have...tipped...', 14, 0, 0, 0, 0, 20844, 'VO_DM_Helix_HelixDies01'),
(47296, 1, 0, 'Only ten copper? You\'re not even worth killing!', 14, 0, 0, 0, 0, 20845, 'VO_DM_Helix_Kill01'),
(47296, 2, 0, 'I didn\'t need him! Not when I\'ve got YOU oafs!', 14, 0, 0, 0, 0, 20846, 'VO_DM_Helix_OafDies01'),
(47296, 3, 0, 'Bombs away!', 14, 0, 0, 0, 0, 20847, 'VO_DM_Helix_Spell01'),
(47296, 4, 0, 'Ready Oafie? Throw!', 14, 0, 0, 0, 0, 20848, 'VO_DM_Helix_Spell02'),
(47296, 5, 0, 'The mistress will pay me handsomely for your heads!', 14, 0, 0, 0, 0, 20849, 'VO_DM_Helix_Aggro01'),
(47297, 0, 0, 'No...NO! OAF SMASH!!', 14, 0, 0, 0, 0, 20854, 'VO_DM_LumberingOaf_HelixDies01'),
(47297, 1, 0, '', 14, 0, 0, 0, 0, 20855, 'VO_DM_LumberingOaf_OafDies01');

DELETE from `creature_text` where `entry` = 43778;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(43778, 0, 0, 'Foe Reaper 5000 on-line. All systems nominal.', 14, 0, 0, 0, 0, 22137, 'VO_DM_FoeReaper_Aggro01'),
(43778, 1, 0, 'Overheat threshold exceeded. System failure. Wheat clog in port two. Shutting down.', 14, 0, 0, 0, 0, 22138, 'VO_DM_FoeReaper_Death01'),
(43778, 2, 0, 'Target destroyed.', 14, 0, 0, 0, 0, 22139, 'VO_DM_FoeReaper_Kill01'),
(43778, 3, 0, 'Acquiring target...', 14, 0, 0, 0, 0, 22140, 'VO_DM_FoeReaper_Spell01'),
(43778, 4, 0, 'Target acquired. Harvesting servos engaged.', 14, 0, 0, 0, 0, 22141, 'VO_DM_FoeReaper_Spell02'),
(43778, 5, 0, 'Overdrive engine activated.', 14, 0, 0, 0, 0, 22142, 'VO_DM_FoeReaper_Spell03'),
(43778, 6, 0, 'Safety restrictions off-line. Catastrophic system failure imminent.', 14, 0, 0, 0, 0, 22143, 'VO_DM_FoeReaper_Spell04');

DELETE from `creature_text` where `entry`=47626;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(47626, 0, 0, 'You will...NOT find her...until it is too late...', 14, 0, 0, 0, 0, 19875, 'VO_DM_Ripsnarl_Death01'),
(47626, 1, 0, 'Your blood only increases my hunger!', 14, 0, 0, 0, 0, 19876, 'VO_DM_Ripsnarl_Kill01'),
(47626, 2, 0, 'Do you feel that chill running up your spine? The fog is rolling in...', 14, 0, 0, 0, 0, 19877, 'VO_DM_Ripsnarl_Spell01'),
(47626, 3, 0, 'The fog is rolling in...', 14, 0, 0, 0, 0, 19878, 'VO_DM_Ripsnarl_Spell02'),
(47626, 4, 0, 'I can smell your fear...', 14, 0, 0, 0, 0, 19879, 'VO_DM_Ripsnarl_Spell03'),
(47626, 5, 0, 'I will rip your heart from your chest!', 1, 0, 0, 0, 0, 19880, 'VO_DM_Ripsnarl_Spell04'),
(47626, 6, 0, '', 14, 0, 0, 0, 0, 19881, 'VO_DM_Ripsnarl_Spell05'),
(47626, 7, 0, 'Ah, fresh meat!', 14, 0, 0, 0, 0, 19882, 'VO_DM_Ripsnarl_Aggro01');

DELETE FROM `locales_creature_text` WHERE `entry`=47626;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(47626, 0, 0, 'Вы найдете ее... Но будет слишком поздно...'),
(47626, 1, 0, 'Твоя кровь пробуждает мой аппетит.'),
(47626, 2, 0, 'Чувствуешь, как холодок пробежал по спине?'),
(47626, 3, 0, 'Опускается туман...'),
(47626, 4, 0, 'Ты источаешь запах страха.'),
(47626, 5, 0, 'Я вырву сердце из твоей груди!'),
(47626, 6, 0, ''),
(47626, 7, 0, 'Ааааа... Свежее мясо.');

DELETE FROM `creature_text` WHERE `entry`=49429;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(49429, 0, 0, "Poor Glubtok. When his powers manifested, his own ogre mound was the first to burn.", 12, 0, 100, 0, 0, 24602, 'Vanessa VanCleef glubtok 1'),
(49429, 1, 0, "Deep within his soul, the one thing he feared most of all was...himself.", 12, 0, 100, 0, 0, 24603, 'Vanessa VanCleef glubtok 2'),
(49429, 2, 0, "Most rogues prefer to cloak themselves in the shadows, but not Helix.", 12, 0, 100, 0, 0, 24604, 'Vanessa VanCleef helix 1'),
(49429, 3, 0, "You never know what skitters in the darkness.", 12, 0, 100, 0, 0, 24605, 'Vanessa VanCleef helix 2'),
(49429, 4, 0, "Can you imagine the life of a machine?", 12, 0, 100, 0, 0, 24606, 'Vanessa VanCleef foereaper 1'),
(49429, 5, 0, "A simple spark can mean the difference between life...and death.", 12, 0, 100, 0, 0, 24607, 'Vanessa VanCleef foereaper 2'),
(49429, 6, 0, "Ripsnarl wasn\'t always a bloodthirsty savage. Once, he even had a family.", 12, 0, 100, 0, 0, 24608, 'Vanessa VanCleef admiral 1'),
(49429, 7, 0, "He was called James Harrington. A tragedy in three parts.", 12, 0, 100, 0, 0, 24609, 'Vanessa VanCleef admiral 2'),
(49429, 8, 0, "I've been waiting a long time for this, you know.", 12, 0, 100, 0, 0, 24613, 'Vanessa VanCleef intro 1'),
(49429, 9, 0, "Biding my time, building my forces, studying the minds of my enemies.", 12, 0, 100, 0, 0, 24614, 'Vanessa VanCleef intro 2'),
(49429, 10, 0, "I was never very good at hand-to-hand combat, you know. Not like my father.", 12, 0, 100, 0, 0, 24615, 'Vanessa VanCleef intro 3'),
(49429, 11, 0, "But I always excelled at poisons.", 12, 0, 100, 0, 0, 24616, 'Vanessa VanCleef intro 4'),
(49429, 12, 0, "Especially venoms that affect the mind.", 12, 0, 100, 0, 0, 24617, 'Vanessa VanCleef intro 5');

DELETE FROM `creature_text` WHERE `entry`=49541;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(49541, 0, 0, "I will not share my father\'s fate! Your tale ends here!", 14, 0, 100, 0, 0, 24599, 'Vanessa VanCleef aggro'),
(49541, 1, 0, "ENOUGH! I will not give you the pleasure.", 14, 0, 100, 0, 0, 24610, 'Vanessa VanCleef death 1'),
(49541, 1, 1, "If I'm going to die, I\'m taking you all with me!", 14, 0, 100, 0, 0, 24611, 'Vanessa VanCleef death 2'),
(49541, 1, 2, "MY FATE IS MY OWN!", 14, 0, 100, 0, 0, 24612, 'Vanessa VanCleef death 3'),
(49541, 2, 0, "The first of many!", 14, 0, 100, 0, 0, 24618, 'Vanessa VanCleef kill'),
(49541, 3, 0, "Did you really think I would come to this fight alone!?", 14, 0, 100, 0, 0, 24619, 'Vanessa VanCleef adds'),
(49541, 4, 0, "Fools! This entire ship is rigged with explosives! Enjoy your fiery deaths!", 14, 0, 100, 0, 0, 24620, 'Vanessa VanCleef spell 1'),
(49541, 5, 0, "You didn't honestly think I would only plant ONE set of explosives, did you?", 14, 0, 100, 0, 0, 24621, 'Vanessa VanCleef spell 2');

DELETE FROM `locales_creature_text` WHERE `entry`=49429;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(49429, 0, 0, 'Бедняга Глубток. Первое проявление его силы выжгло всю его родную деревню.'),
(49429, 1, 0, 'В глубине души больше всего он боялся... самого себя.'),
(49429, 2, 0, 'Большинство разбойников предпочитают держаться в тени, но только не Хеликс.'),
(49429, 3, 0, 'Никогда не знаешь, что скрывается в темноте.'),
(49429, 4, 0, 'Вообразите жизнь механизма.'),
(49429, 5, 0, 'Одна искра определяет его жизнь... и смерть.'),
(49429, 6, 0, 'Терзающий Рев не всегда был кровожадным дикарем. Когда-то у него даже была семья.'),
(49429, 7, 0, 'Его звали Джеймс Харрингтон. Это трагедия в трех действиях.'),
(49429, 8, 0, 'Как же долго я этого ждала.'),
(49429, 9, 0, 'Все это время я терпеливо собирала силы, изучала своих врагов.'),
(49429, 10, 0, 'Я никогда не была сильна в рукопашном бою. Не то что мой отец.'),
(49429, 11, 0, 'Зато я преуспела в изготовлении ядов.'),
(49429, 12, 0, 'Особенно тех, что отравляют разум.');

DELETE FROM `locales_creature_text` WHERE `entry`=49541;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(49541, 0, 0, 'Я не повторю судьбу своего отца! Рассказ окончен!'),
(49541, 1, 0, 'Я не доставлю вам такого удовольствия!'),
(49541, 1, 1, 'Если мне суждено умереть, то я заберу вас всех с собой!'),
(49541, 1, 2, 'Я – хозяйка своей судьбы!'),
(49541, 2, 0, 'Первая смерть из многих.'),
(49541, 3, 0, 'Вы и впрямь считали, что я решила сразиться с вами в одиночку?'),
(49541, 4, 0, 'Идиоты! Весь корабль начинен взрывчаткой! Наслаждайтесь вашей пламенной смертью!'),
(49541, 5, 0, 'Вы же не думали, что я заложила только одну бомбу?');