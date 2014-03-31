-- DELETE FROM `creature` WHERE `guid`=335685 AND `id`=52430;
-- DELETE FROM `creature` WHERE `guid`=336878 AND `id`=52624;
-- DELETE FROM `creature` WHERE `guid`=337684 AND `id`=52379;
-- DELETE FROM `creature` WHERE `guid`=339055 AND `id`=52340;

DELETE FROM `creature` WHERE `map`=859 AND `id` IN (52430, 52400, 52407, 52730, 52222);

INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(52730, 859, 3, 2, 37749, 0, -11739.2, -1592.38, 40.8066, 5.21864, 20, 0, 0, 0, 0, 0, 0, 0, 0),
(52730, 859, 3, 2, 37749, 0, -11836.6, -1698.67, 40.7978, 4.38919, 20, 0, 0, 0, 0, 0, 0, 0, 0),
(52730, 859, 3, 2, 37749, 0, -11842.5, -1596.23, 40.8064, 6.04886, 20, 0, 0, 0, 0, 0, 0, 0, 0),
(52730, 859, 3, 2, 37749, 0, -11734.2, -1691.69, 40.8075, 2.58224, 20, 0, 0, 0, 0, 0, 0, 0, 0);

UPDATE `creature_template` SET `dmg_multiplier`=70 WHERE `entry` IN (52053, 52148, 52151, 52155);

UPDATE `creature` SET `spawnmask`=2, `phasemask`=3 WHERE `id`=52148;

UPDATE `creature` SET `phasemask`=2, `spawntimesecs`=15 WHERE `id` IN (52624, 52532);

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=52430;
UPDATE `creature_template` SET `unit_flags`=0x100 WHERE `entry`=52222;
UPDATE `creature_template` SET `unit_flags`=32768, `dynamicflags`=0 WHERE `entry`=52148;
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0, `flags_extra`=0 WHERE `entry`=52430;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry` IN (52400, 52407);
UPDATE `creature_template` SET `mechanic_immune_mask`=653213695 WHERE `entry`=52430;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN (52730, 52222);
UPDATE `creature_template` SET `dmgschool`=6, `speed_run`=0.4 WHERE `entry`=52624;
UPDATE `creature_template` SET `dmg_multiplier`=30 WHERE `entry`=52624;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=14, `faction_H`=14 WHERE `entry`=52400;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=14, `faction_H`=14 WHERE `entry`=52407;
UPDATE `creature_template` SET `spell1`=0 WHERE `entry`=52400;
UPDATE `creature_template_addon` SET `auras`='97031 96970' WHERE `entry`=52400;

UPDATE `creature_template` SET `ScriptName`='boss_jindo_the_godbreaker' WHERE `entry`=52148;
UPDATE `creature_template` SET `ScriptName`='npc_jindo_gurubashi_spirit' WHERE `entry`=52730;
UPDATE `creature_template` SET `ScriptName`='npc_jindo_spirit_of_hakkar' WHERE `entry`=52222;
UPDATE `creature_template` SET `ScriptName`='npc_jindo_chains_of_hakkar' WHERE `entry`=52430;
UPDATE `creature_template` SET `ScriptName`='npc_jindo_spirit_portal' WHERE `entry`=52532;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (97158, 97152, 97597);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(97158, 'spell_jindo_shadow_spike_target'),
(97152, 'spell_jindo_summon_spirit_target'),
(97597, 'spell_jindo_spirit_warrior_gaze_target');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=16834;

DELETE FROM `creature_text` WHERE `entry`=52148;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52148, 0, 0, 'Welcome to de great show, friends. Just wait \'til ya see what I got in store for ya.', 14, 0, 0, 0, 0, 24254, 'Jindo aggro'),
(52148, 1, 0, 'No... NOOOO!', 14, 0, 0, 0, 0, 24255, 'Jindo death'),
(52148, 2, 0, 'No one gonna stop Jin\'do!', 14, 0, 0, 0, 0, 24256, 'Jindo kill 01'),
(52148, 2, 1, 'You nothing!', 14, 0, 0, 0, 0, 24257, 'Jindo kill 01 phase'),
(52148, 3, 0, 'No one gonna stop Jin\'do!', 14, 0, 0, 0, 0, 24258, 'Jindo kill 02'),
(52148, 3, 1, 'You nothing!', 14, 0, 0, 0, 0, 24259, 'Jindo kill 02 phase'),
(52148, 4, 0, 'Hey! Grats, mon!', 14, 0, 0, 0, 0, 24260, 'Jindo kill mandokir'),
(52148, 5, 0, 'Now, have a taste of Jin\'do\'s true powah!', 14, 0, 0, 0, 0, 24267, 'Jindo phase 2'),
(52148, 6, 0, 'Meddlesome insects! Now you will feel my wrath!', 14, 0, 0, 0, 0, 24268, 'Jindo spawn');

DELETE FROM `locales_creature_text` WHERE `entry`=52148;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(52148, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Добро пожаловать на наше представление, друзья! Спешите видеть, что я для вас приготовил!'),
(52148, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нет! НЕЕЕЕТ!'),
(52148, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Никто не остановит Джин\'до!'),
(52148, 2, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты ничтожество!'),
(52148, 3, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Никто не остановит Джин\'до!'),
(52148, 3, 1, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты ничтожество!'),
(52148, 5, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'А теперь узрите всю силу Джин\'до!'),
(52148, 6, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Мерзкие насекомые! Я вам сейчас покажу!');

DELETE FROM `creature_text` WHERE `entry`=52222;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52222, 0, 0, 'Be freed, insects. I will deal with you another time.', 14, 0, 0, 0, 0, 24243, 'Hakkar end 2'),
(52222, 1, 0, 'You overstepped your bounds, Jin\'do. You toy with powers that are beyond you. Have you forgotten who I am? Have you forgotten what I can do?', 14, 0, 0, 0, 0, 24244, 'Hakkar end 1'),
(52222, 2, 0, 'Your deceit is unforgivable, Jin\'do. You spit in the face of a god!', 14, 0, 0, 0, 0, 24246, 'Hakkar phase 2');

DELETE FROM `locales_creature_text` WHERE `entry`=52222;
INSERT INTO `locales_creature_text` (`entry`, `textgroup`, `id`, `text_loc1`, `text_loc2`, `text_loc3`, `text_loc4`, `text_loc5`, `text_loc6`, `text_loc7`, `text_loc8`) VALUES
(52222, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Живите пока, букашки. Я с вами еще разберусь.'),
(52222, 1, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Ты перешел все границы, Джин\'до. Ты играешь с силами, которые тебе неподвластны. Ты забыл, кто я? Забыл, что я могу с тобой сделать?!'),
(52222, 2, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 'Нет тебе прощения, Джин\'до! Это плевок в лицо бога!');