DELETE FROM `creature` WHERE `id` IN 
(47314, 49136, 48957, 48958,
48976, 48975, 49039, 49041,
49042, 47403, 49229, 49208,
48301, 48297, 48300, 48296,
48006, 48294, 48295, 48276,
48299, 48293, 48298, 48302,
49495, 49494, 47714, 49550,
48672, 45979, 45242) AND `map`=36;

-- DELETE FROM `creature` WHERE `guid`=319369 AND `id`=46906;
-- DELETE FROM `creature` WHERE `guid`=327366 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327365 AND `id`=647;
-- DELETE FROM `creature` WHERE `guid`=327367 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=319370 AND `id`=46890;
-- DELETE FROM `creature` WHERE `guid`=319371 AND `id`=46890;
-- DELETE FROM `creature` WHERE `guid`=327422 AND `id`=42371;
-- DELETE FROM `creature` WHERE `guid`=327419 AND `id`=42697;
-- DELETE FROM `creature` WHERE `guid`=319372 AND `id`=46890;
-- DELETE FROM `creature` WHERE `guid`=319375 AND `id`=46902;
-- DELETE FROM `creature` WHERE `guid`=319372 AND `id`=46890;
-- DELETE FROM `creature` WHERE `guid`=319376 AND `id`=46889;
-- DELETE FROM `creature` WHERE `guid`=327356 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327410 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327372 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327392 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327399 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=319375 AND `id`=46902;
-- DELETE FROM `creature` WHERE `guid`=327395 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=327386 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327405 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327391 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=327361 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327378 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327380 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=327385 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327364 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=327397 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=327413 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327390 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327384 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327412 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327369 AND `id`=646;
-- DELETE FROM `creature` WHERE `guid`=327371 AND `id`=636;
-- DELETE FROM `creature` WHERE `guid`=327379 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327393 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327401 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327406 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327357 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327402 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327360 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327396 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327383 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327355 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327363 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327407 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327401 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327370 AND `id`=636;
-- DELETE FROM `creature` WHERE `guid`=327412 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327403 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=327361 AND `id`=1732;
-- DELETE FROM `creature` WHERE `guid`=327391 AND `id`=3947;
-- DELETE FROM `creature` WHERE `guid`=327386 AND `id`=657;
-- DELETE FROM `creature` WHERE `guid`=319352 AND `id`=45979;
-- DELETE FROM `creature` WHERE `guid`=327416 AND `id`=42700;
-- DELETE FROM `creature` WHERE `guid`=327106 AND `id`=49541;
-- DELETE FROM `creature` WHERE `guid`=327421 AND `id`=42698;
-- DELETE FROM `creature` WHERE `guid`=327420 AND `id`=42698;
-- DELETE FROM `creature` WHERE `guid`=327105 AND `id`=49536;
-- DELETE FROM `creature` WHERE `guid`=327104 AND `id`=49539;
-- DELETE FROM `creature` WHERE `guid`=327103 AND `id`=49532;
-- DELETE FROM `creature` WHERE `guid`=327102 AND `id`=49532;
-- DELETE FROM `creature` WHERE `guid`=327101 AND `id`=49532;
-- DELETE FROM `creature` WHERE `guid`=327100 AND `id`=49535;
-- DELETE FROM `creature` WHERE `guid`=327099 AND `id`=49532;
-- DELETE FROM `creature` WHERE `guid`=327098 AND `id`=49532;
-- DELETE FROM `creature` WHERE `guid`=327097 AND `id`=49532;
-- DELETE FROM `creature` WHERE `guid`=327096 AND `id`=49534;

DELETE FROM `npc_spellclick_spells` WHERE `spell_id`=90105;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN 
(48301, 48297, 48300, 48296, 48006, 48294,
48295, 48276, 48299, 48293, 48298, 48302);

UPDATE `creature` SET `spawnmask`=0 WHERE `id` IN 
(49520, 49521, 49671, 49454,
49670, 49674, 49495, 49494,
49681, 49520, 49521, 49532,
49535, 49534, 49536, 49539,
49682) AND `map`=36;

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(45979, 0, 0, 0, 1, 0, '88755'),
(47242, 0, 0, 0, 1, 0, '88768');

REPLACE INTO `spell_proc_event` (`entry`, `SchoolMask`, `SpellFamilyName`, `SpellFamilyMask0`, `SpellFamilyMask1`, `SpellFamilyMask2`, `procFlags`, `procEx`, `ppmRate`, `CustomChance`, `Cooldown`) VALUES
(88736, 0, 0, 0, 0, 0, 0, 1027, 0, 0, 5);

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry` IN (16399, 13965, 16400, 16397, 16398, 101833);

UPDATE `creature` SET `movementtype`=0 WHERE `id`=47626;
UPDATE `creature_template` SET `movementtype`=0 WHERE `entry`=47626;

UPDATE `gameobject_template` SET `ScriptName`='go_defias_cannon' WHERE `entry`=16398;

UPDATE `creature_template` SET `ScriptName`='boss_glubtok' WHERE `entry`=47162;

UPDATE `creature_template` SET `ScriptName`='boss_helix_gearbreaker' WHERE `entry`=47296;
UPDATE `creature_template` SET `ScriptName`='npc_lumbering_oaf' WHERE `entry`=47297;
UPDATE `creature_template` SET `ScriptName`='npc_sticky_bomb' WHERE `entry`=47314;
UPDATE `creature_template` SET `ScriptName`='npc_helix_crew' WHERE `entry`=49136;
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(88278, 'spell_helix_force_player_to_ride_oaf'),
(88352, 'spell_helix_chest_bomb');

UPDATE `creature_template` SET `ScriptName`='boss_foereaper5000' WHERE `entry`=43778;
UPDATE `creature_template` SET `ScriptName`='npc_foereaper_targeting_bunny' WHERE `entry` = 47468;

UPDATE `creature_template` SET `ScriptName`='boss_admiral_ripsnarl' WHERE `entry`=47626;
UPDATE `creature_template` SET `ScriptName`='npc_vapor' WHERE `entry`=47714;

UPDATE `creature_template` SET `ScriptName`='boss_captain_cookie' WHERE `entry`=47739;
UPDATE `creature_template` SET `ScriptName`='npc_captain_cookie_cauldron' WHERE `entry`=47754;
UPDATE `creature_template` SET `npcflag` = 1, `scriptname` = 'npc_captain_cookie_good_food' where entry in (48301, 48297, 48300, 48296, 48006, 48294);
UPDATE `creature_template` SET `npcflag` = 1, `scriptname` = 'npc_captain_cookie_bad_food' where entry in (48295, 48276, 48299, 48293, 48298, 48302);
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (89267, 'spell_captain_cookie_setiated');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92834, 'spell_captain_cookie_setiated');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (89732, 'spell_captain_cookie_nauseated');
REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (92066, 'spell_captain_cookie_nauseated');

DELETE from `creature_text` where `entry` = 47162;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 0, 0, 'TOO...MUCH...POWER!!!', 1, 0, 0, 0, 0, 21145, 'VO_DM_GlubtokBoth_Death01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 1, 0, 'ARCANE POWER!!!', 1, 0, 0, 0, 0, 21146, 'VO_DM_GlubtokBoth_Spell03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 2, 0, 'Glubtok show you da power of de arcane!', 1, 0, 0, 0, 0, 21151, 'VO_DM_GlubtokHead1_Aggro01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 3, 0, 'Ha ha ha ha!', 1, 0, 0, 0, 0, 21152, 'VO_DM_GlubtokHead1_Kill01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 4, 0, 'Fists of flame!', 1, 0, 0, 0, 0, 21153, 'VO_DM_GlubtokHead1_Spell01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 5, 0, 'Glubtok ready?', 1, 0, 0, 0, 0, 21154, 'VO_DM_GlubtokHead1_Spell02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 3, 1, '\'Sploded dat one!', 1, 0, 0, 0, 0, 21155, 'VO_DM_GlubtokHead2_Kill01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 6, 0, 'Fists of frost!', 1, 0, 0, 0, 0, 21156, 'VO_DM_GlubtokHead2_Spell01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47162, 7, 0, 'Let\'s do it!', 1, 0, 0, 0, 0, 21157, 'VO_DM_GlubtokHead2_Spell02');

DELETE from `creature_text` where `entry` in (47296, 47297);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47296, 0, 0, 'The scales...have...tipped...', 1, 0, 0, 0, 0, 20844, 'VO_DM_Helix_HelixDies01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47296, 1, 0, 'Only ten copper? You\'re not even worth killing!', 1, 0, 0, 0, 0, 20845, 'VO_DM_Helix_Kill01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47296, 2, 0, 'I didn\'t need him! Not when I\'ve got YOU oafs!', 1, 0, 0, 0, 0, 20846, 'VO_DM_Helix_OafDies01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47296, 3, 0, 'Bombs away!', 1, 0, 0, 0, 0, 20847, 'VO_DM_Helix_Spell01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47296, 4, 0, 'Ready Oafie? Throw!', 1, 0, 0, 0, 0, 20848, 'VO_DM_Helix_Spell02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47296, 5, 0, 'The mistress will pay me handsomely for your heads!', 1, 0, 0, 0, 0, 20849, 'VO_DM_Helix_Aggro01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47297, 0, 0, 'No...NO! OAF SMASH!!', 1, 0, 0, 0, 0, 20854, 'VO_DM_LumberingOaf_HelixDies01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47297, 1, 0, '', 1, 0, 0, 0, 0, 20855, 'VO_DM_LumberingOaf_OafDies01');

DELETE from `creature_text` where `entry` = 43778;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 0, 0, 'Foe Reaper 5000 on-line. All systems nominal.', 1, 0, 0, 0, 0, 22137, 'VO_DM_FoeReaper_Aggro01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 1, 0, 'Overheat threshold exceeded. System failure. Wheat clog in port two. Shutting down.', 1, 0, 0, 0, 0, 22138, 'VO_DM_FoeReaper_Death01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 2, 0, 'Target destroyed.', 1, 0, 0, 0, 0, 22139, 'VO_DM_FoeReaper_Kill01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 3, 0, 'Acquiring target...', 1, 0, 0, 0, 0, 22140, 'VO_DM_FoeReaper_Spell01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 4, 0, 'Target acquired. Harvesting servos engaged.', 1, 0, 0, 0, 0, 22141, 'VO_DM_FoeReaper_Spell02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 5, 0, 'Overdrive engine activated.', 1, 0, 0, 0, 0, 22142, 'VO_DM_FoeReaper_Spell03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (43778, 6, 0, 'Safety restrictions off-line. Catastrophic system failure imminent.', 1, 0, 0, 0, 0, 22143, 'VO_DM_FoeReaper_Spell04');

DELETE from `creature_text` where `entry` = 47626;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 0, 0, 'You will...NOT find her...until it is too late...', 1, 0, 0, 0, 0, 19875, 'VO_DM_Ripsnarl_Death01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 1, 0, 'Your blood only increases my hunger!', 1, 0, 0, 0, 0, 19876, 'VO_DM_Ripsnarl_Kill01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 2, 0, 'Do you feel that chill running up your spine? The fog is rolling in...', 1, 0, 0, 0, 0, 19877, 'VO_DM_Ripsnarl_Spell01');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 3, 0, '', 1, 0, 0, 0, 0, 19878, 'VO_DM_Ripsnarl_Spell02');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 4, 0, 'I can smell your fear...', 1, 0, 0, 0, 0, 19879, 'VO_DM_Ripsnarl_Spell03');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 5, 0, 'I will rip your heart from your chest!', 1, 0, 0, 0, 0, 19880, 'VO_DM_Ripsnarl_Spell04');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 6, 0, '', 1, 0, 0, 0, 0, 19881, 'VO_DM_Ripsnarl_Spell05');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) 
VALUES (47626, 7, 0, 'Ah, fresh meat!', 1, 0, 0, 0, 0, 19882, 'VO_DM_Ripsnarl_Aggro01');

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