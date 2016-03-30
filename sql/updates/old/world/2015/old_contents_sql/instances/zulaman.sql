DELETE FROM `creature` WHERE `id`=24858 AND `map`=568;
DELETE FROM `creature` WHERE `id`=52638 AND `map`=568;
DELETE FROM `creature` WHERE `id`=24225 AND `map`=568;
DELETE FROM `creature` WHERE `id`=24159 AND `map`=568;
DELETE FROM `creature` WHERE `id`=23818 AND `map`=568;
DELETE FROM `creature` WHERE `id`=23598 AND `map`=568;
DELETE FROM `creature` WHERE `id`=23920 AND `map`=568;
DELETE FROM `creature` WHERE `id`=23817 AND `map`=568;

UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry`=24858;
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry`=52638;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `unit_flags`=0 WHERE `entry`=23920;
UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16 WHERE `entry`=23577;

UPDATE `creature` SET `position_x`=-80.46, `position_y`=1297.48, `position_z`=48.50, `orientation`=1.65 WHERE `id`=23576;

UPDATE `instance_template` SET `script`='instance_zulaman' WHERE  `map`=568;
UPDATE `access_requirement` SET `level_min`=85 WHERE  `mapId`=568 AND `difficulty`=0;
UPDATE `access_requirement` SET `level_min`=85 WHERE  `mapId`=568 AND `difficulty`=1;

UPDATE `gameobject_template` SET `ScriptName`='go_strange_gong' WHERE `entry`=187359;

UPDATE `creature_template` SET `ScriptName`='boss_akilzon' WHERE `entry`=23574;
UPDATE `creature_template` SET `ScriptName`='npc_akilzon_soaring_eagle' WHERE `entry`=24858;
DELETE FROM `spell_script_names` WHERE `spell_id`=43648;
INSERT INTO `spell_script_names` VALUES (43648, 'spell_akilzon_electrical_storm');
DELETE FROM `spell_script_names` WHERE `spell_id`=43657;
DELETE FROM `spell_script_names` WHERE `spell_id`=97300;
INSERT INTO `spell_script_names` VALUES (43657, 'spell_akilzon_electrical_storm_dmg');
INSERT INTO `spell_script_names` VALUES (97300, 'spell_akilzon_electrical_storm_dmg');

UPDATE `creature_template` SET `ScriptName`='boss_janalai' WHERE `entry`=23578;
UPDATE `creature_template` SET `ScriptName`='npc_janalai_firebomb' WHERE `entry`=23920;
UPDATE `creature_template` SET `ScriptName`='npc_janalai_hatcher' WHERE `entry`=23818;
UPDATE `creature_template` SET `ScriptName`='npc_janalai_hatcher' WHERE `entry`=24504;
UPDATE `creature_template` SET `ScriptName`='npc_janalai_hatchling' WHERE `entry`=23598;
UPDATE `creature_template` SET `ScriptName`='npc_janalai_egg' WHERE `entry`=23817;
DELETE FROM `spell_script_names` WHERE `spell_id`=43140;
DELETE FROM `spell_script_names` WHERE `spell_id`=97855;
INSERT INTO `spell_script_names` VALUES (43140, 'spell_janalai_flame_breath');
INSERT INTO `spell_script_names` VALUES (97855, 'spell_janalai_flame_breath');


UPDATE `creature_template` SET `ScriptName`='boss_halazzi' WHERE `entry`=23577;
UPDATE `creature_template` SET `ScriptName`='npc_halazzi_lynx' WHERE `entry`=24143;
UPDATE `creature_template` SET `ScriptName`='npc_halazzi_water_totem' WHERE `entry`=52755;
UPDATE `creature_template` SET `ScriptName`='npc_halazzi_lightning_totem' WHERE `entry`=24224;

DELETE FROM `creature_text` WHERE `entry`=23574;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23574, 0, 0, 'I be da predator! You da prey...', 14, 0, 0, 0, 0, 12013, 'Akilzon aggro'),
(23574, 1, 0, 'Feed, me bruddahs!', 14, 0, 0, 0, 0, 12014, 'Akilzon eagle'),
(23574, 2, 0, 'Stop your cryin\'!', 14, 0, 0, 0, 0, 12017, 'Akilzon slay1'),
(23574, 2, 1, 'Ya got nothin\'!', 14, 0, 0, 0, 0, 12018, 'Akilzon slay2'),
(23574, 3, 0, 'You can\'t... kill... me spirit!', 14, 0, 0, 0, 0, 12019, 'Akilzon death');

DELETE FROM `creature_text` WHERE `entry`=23576;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23576, 0, 0, 'Get da move on, guards! It be killin\' time!', 14, 0, 0, 0, 0, 12066, 'Nalorakk wave 1'),
(23576, 1, 0, 'Guards, go already! Who you more afraid of, dem... or me?', 14, 0, 0, 0, 0, 12067, 'Nalorakk wave 2'),
(23576, 2, 0, 'Ride now! Ride out dere and bring me back some heads!', 14, 0, 0, 0, 0, 12068, 'Nalorakk wave 3'),
(23576, 3, 0, 'I be losin\' patience! Go on: make dem wish dey was never born!', 14, 0, 0, 0, 0, 12069, 'Nalorakk wave 4'),
(23576, 4, 0, 'You be dead soo enough!', 14, 0, 0, 0, 0, 12070, 'Nalorakk aggro 1'),
(23576, 4, 1, 'Da Amani gonna rule again!', 14, 0, 0, 0, 0, 12076, 'Nalorakk aggro 2'),
(23576, 5, 0, 'I bring da pain!', 14, 0, 0, 0, 0, 12071, 'Nalorakk surge'),
(23576, 6, 0, 'You call on da beast, you gonna get more dan you bargain for!', 14, 0, 0, 0, 0, 12072, 'Nalorakk bear'),
(23576, 7, 0, 'Make way for da Nalorakk!', 14, 0, 0, 0, 0, 12073, 'Nalorakk troll'),
(23576, 8, 0, 'Mua-ha-ha! Now whatchoo got to say?', 14, 0, 0, 0, 0, 12075, 'Nalorakk kill'),
(23576, 9, 0, 'I... be waitin\' on da udda side...', 14, 0, 0, 0, 0, 12077, 'Nalorakk death');

DELETE FROM `creature_text` WHERE `entry`=23578;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23578, 0, 0, 'Spirits of da wind be your doom!', 14, 0, 0, 0, 0, 12031, 'Janalai aggro'),
(23578, 1, 0, 'I burn ya now!', 14, 0, 0, 0, 0, 12032, 'Janalai firebombs'),
(23578, 2, 0, 'Where ma hatcha? Get to work on dem eggs!', 14, 0, 0, 0, 0, 12033, 'Janalai hatchers'),
(23578, 3, 0, 'I show you strength... in numbers!', 14, 0, 0, 0, 0, 12034, 'Janalai all eggs'),
(23578, 4, 0, 'It all be over now, mon!', 14, 0, 0, 0, 0, 12036, 'Janalai kill'),
(23578, 5, 0, 'Tazaga-choo!', 14, 0, 0, 0, 0, 12037, 'Janalai death');

DELETE FROM `creature_text` WHERE `entry`=23577;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23577, 0, 0, 'Get on your knees and bow to da fang and claw!', 14, 0, 0, 0, 0, 12020, 'Halazzi aggro'),
(23577, 1, 0, 'I fight wit\' untamed spirit...', 14, 0, 0, 0, 0, 12021, 'Halazzi split'),
(23577, 2, 0, 'Spirit, come back to me!', 14, 0, 0, 0, 0, 12022, 'Halazzi combine'),
(23577, 3, 0, 'Me gonna carve you now!', 14, 0, 0, 0, 0, 12023, 'Halazzi melee01'),
(23577, 3, 1, 'You gonna leave in pieces', 14, 0, 0, 0, 0, 12024, 'Halazzi melee02'),
(23577, 4, 0, 'Chaga... chocka\'jinn.', 14, 0, 0, 0, 0, 12028, 'Halazzi death');

DELETE FROM `creature_text` WHERE `entry`=24239;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(24239, 0, 0, 'Da shadow gonna fall on you...', 14, 0, 0, 0, 0, 12041, 'Malacrass aggro'),
(24239, 1, 0, 'Dis a nightmare ya don\' wake up from!', 14, 0, 0, 0, 0, 12043, 'Malacrass kill 1'),
(24239, 1, 1, 'Azzaga choogo zinn!', 14, 0, 0, 0, 0, 12044, 'Malacrass kill 2'),
(24239, 2, 0, 'You will belong ta me now!', 14, 0, 0, 0, 0, 12045, 'Malacrass charm'),
(24239, 3, 0, 'Your soul gonna bleed!', 14, 0, 0, 0, 0, 12047, 'Malacrass shadow 2'),
(24239, 4, 0, 'It not gonna make no difference.', 14, 0, 0, 0, 0, 12048, 'Malacrass pet dies 01'),
(24239, 4, 1, 'You gonna die worse dan him.', 14, 0, 0, 0, 0, 12049, 'Malacrass pet dies 02'),
(24239, 4, 2, 'Dat no bodda me.', 14, 0, 0, 0, 0, 12050, 'Malacrass pet dies 03'),
(24239, 5, 0, 'Dis not... da end of me...', 14, 0, 0, 0, 0, 12041, 'Malacrass death');

DELETE FROM `creature_text` WHERE `entry`=23863;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23863, 0, 0, 'Got me some new tricks... like me bruddah bear!', 14, 0, 0, 0, 0, 24220, 'Daakara bear'),
(23863, 1, 0, 'Mebbe me fall... but da Amani empire... never going to die.', 14, 0, 0, 0, 0, 24222, 'Daakara death'),
(23863, 2, 0, 'You don\' have to look to da sky to see da dragonhawk!', 14, 0, 0, 0, 0, 24223, 'Daakara dragonhawk'),
(23863, 3, 0, 'Dere be no hidin\' from da eagle!', 14, 0, 0, 0, 0, 24224, 'Daakara eagle'),
(23863, 4, 0, 'Fire kill you just as quick!', 14, 0, 0, 0, 0, 24225, 'Daakara fire breath'),
(23863, 5, 0, 'Da Amani de chuka!', 14, 0, 0, 0, 0, 24226, 'Daakara kill 1'),
(23863, 5, 1, 'Lot more gonna fall like you!', 14, 0, 0, 0, 0, 24227, 'Daakara kill2'),
(23863, 5, 2, 'Oh, look you... dead.', 14, 0, 0, 0, 0, 24238, 'Daakara kill 3'),
(23863, 6, 0, 'De Zandalari give us strength. Nobody push around de Amani no more!', 14, 0, 0, 0, 0, 24232, 'Daakara aggro');

UPDATE `creature_template` SET `ScriptName`='npc_zulaman_forest_frog' WHERE `entry`=24396;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=3684 AND `type`=5;
DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=3879 AND `type`=7;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=16848;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(16848, 11, 0, 0, 'achievement_ohganot_so_fast');

UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=186667;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=186672;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=186648;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=186667;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=186672;
UPDATE `creature_template` SET `faction_A`=7, `faction_H`=7 WHERE `entry`=52945;
UPDATE `gameobject_template` SET `flags`=36 WHERE `entry`=186306;
DELETE FROM `creature` WHERE `id`=24246 AND `map`=568;
UPDATE `gameobject_template` SET `faction`=0 WHERE `entry`=186728;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=16835;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES 
(16835, 11, 0, 0, 'achievement_ring_out');

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN (52055, 52054);

DELETE FROM `creature` WHERE `map`=859 AND `id` IN (52062, 52055, 52054);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (52062, 52055, 52054);

UPDATE `creature_template` SET `ScriptName`='boss_zanzil' WHERE `entry`=52053;
UPDATE `creature_template` SET `ScriptName`='npc_zanzil_berserker' WHERE `entry`=52054;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (96342, 96914, 96331);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(96342, 'spell_zanzil_pursuit'),
(96914, 'spell_zanzil_fire'),
(96331, 'spell_frostburn_formula');

DELETE FROM `creature_text` WHERE `entry`=52053;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52053, 0, 0, 'What? You\'ve come to laugh at Zanzil, too? Not again! I\ll make you pay!', 14, 0, 0, 0, 0, 24337, 'Zanzil aggro'),
(52053, 1, 0, 'You\'ll all suffer for this! Zul\'Gurub is NOTHING without Zan... Zan...', 14, 0, 0, 0, 0, 24338, 'Zanzil death'),
(52053, 2, 0, 'Hahahaha!', 14, 0, 0, 0, 0, 24340, 'Zanzil gas 01'),
(52053, 2, 1, 'Breathe deep, friends! Breathe it all in!', 14, 0, 0, 0, 0, 24341, 'Zanzil gas 02'),
(52053, 3, 0, 'Don\'t worry, Zanzil won\'t leave you dead for long...', 14, 0, 0, 0, 0, 24342, 'Zanzil kill 01'),
(52053, 3, 1, 'How does it taste?', 14, 0, 0, 0, 0, 24343, 'Zanzil kill 02'),
(52053, 3, 2, 'Oooh, pretty... Zanzil\'s got big plans for this one.', 14, 0, 0, 0, 0, 24344, 'Zanzil kill 03'),
(52053, 4, 0, 'Maybe you need someone bigger to play with!', 14, 0, 0, 0, 0, 24349, 'Zanzil raise brute 01'),
(52053, 4, 1, 'Go, little one! Fight them! KILL THEM!', 14, 0, 0, 0, 0, 24350, 'Zanzil raise brute 02'),
(52053, 5, 0, 'Who needs friends? Zanzil just makes his own!', 14, 0, 0, 0, 0, 24351, 'Zanzil raise trolls 01'),
(52053, 5, 1, 'Rise up! Zanzil\'s elixir gives you life!', 14, 0, 0, 0, 0, 24352, 'Zanzil raise trolls 02');

UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16 WHERE `entry`=23574;
UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16 WHERE `entry`=23578;
UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16 WHERE `entry`=24239;
UPDATE `creature_template` SET `ScriptName`='boss_hex_lord_malacrass' WHERE `entry`=24239;
UPDATE `creature_template` SET `ScriptName`='npc_gazakroth' WHERE `entry`=24244;
UPDATE `creature_template` SET `ScriptName`='npc_darkheart' WHERE `entry`=24246;
UPDATE `creature_template` SET `ScriptName`='npc_slither' WHERE `entry`=24242;
UPDATE `creature_template` SET `ScriptName`='npc_alyson_antille' WHERE `entry`=24240;

DELETE FROM `spell_script_names` WHERE `spell_id`=43522;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (43522, 'spell_hexlord_unstable_affliction');

DELETE FROM `conditions` WHERE `SourceEntry`=82699;
replace INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '44132', '31', '1', '24239', 'Drain Power');

UPDATE `creature_template` SET `ScriptName`='boss_halfus_wyrmbreaker' WHERE `entry`=44600;
UPDATE `creature_template` SET `ScriptName`='boss_theralion' WHERE `entry`=45993;
UPDATE `creature_template` SET `ScriptName`='boss_valiona' WHERE `entry`=45992;

UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=47501;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=47040;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=41264;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `faction_A`=16, `faction_H`=16, `flags_extra`=128 WHERE `entry`=49432;
UPDATE `creature_template` SET `ScriptName`='npc_ignacious_inferno_rush' WHERE `entry`=47501;
UPDATE `creature_template` SET `ScriptName`='npc_ignacious_inferno_leap' WHERE `entry`=47040;
UPDATE `creature_template` SET `ScriptName`='npc_feludius_water_bomb' WHERE `entry`=41264;
UPDATE `creature_template` SET `ScriptName`='npc_ignacious_flame_strike' WHERE `entry`=41264;

DELETE FROM `spell_script_names` WHERE `spell_id`=82860;
DELETE FROM `spell_script_names` WHERE `spell_id`=92523;
DELETE FROM `spell_script_names` WHERE `spell_id`=92524;
DELETE FROM `spell_script_names` WHERE `spell_id`=92525;
INSERT INTO `spell_script_names` VALUES(82860, 'spell_ignacious_inferno_rush');
INSERT INTO `spell_script_names` VALUES(92523, 'spell_ignacious_inferno_rush');
INSERT INTO `spell_script_names` VALUES(92524, 'spell_ignacious_inferno_rush');
INSERT INTO `spell_script_names` VALUES(92525, 'spell_ignacious_inferno_rush');

DELETE FROM `conditions` WHERE `SourceEntry`=82699;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '82699', '31', '1', '41264', 'Water Bomb');

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=43687;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=49612;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=49613;
UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry`=49614;

REPLACE INTO `spell_linked_spell` VALUES 
(82665, 82666, 2, 'Feludius - Heart of Ice + Frost Imbued'),
(82660, 82663, 2, 'Ignacious - Burning Blood + Flame Imbued');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (83718, 92541, 92542, 92543);
INSERT INTO `spell_script_names` VALUES
(83718, 'spell_terrastra_harden_skin'),
(92541, 'spell_terrastra_harden_skin'),
(92542, 'spell_terrastra_harden_skin'),
(92543, 'spell_terrastra_harden_skin');

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=187359;

DELETE FROM `creature` WHERE `map`=568 AND `id` IN (52850, 24187, 52839, 24136);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (24187, 52839, 24136);

UPDATE `creature_template` SET `Scriptname`='boss_daakara' WHERE `entry`=23863;
UPDATE `creature_template` SET `flags_extra`=128, `Scriptname`='npc_daakara_vortex' WHERE `entry`=24136;
UPDATE `creature_template` SET `Scriptname`='npc_daakara_amani_lynx' WHERE `entry`=52839;
UPDATE `creature_template` SET `flags_extra`=128, `Scriptname`='npc_daakara_pillar_of_flame' WHERE `entry`=24187;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (42583, 97647);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(42583, 'spell_daakara_claw_rage_charge'),
(97647, 'spell_daakara_sweeping_winds');

DELETE FROM `creature_text` WHERE `entry`=23863;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(23863, 0, 0, 'Got me some new tricks... like me bruddah bear!', 14, 0, 0, 0, 0, 24220, 'Daakara bear'),
(23863, 1, 0, 'Mebbe me fall... but da Amani empire... never going to die.', 14, 0, 0, 0, 0, 24222, 'Daakara death'),
(23863, 2, 0, 'You don\' have to look to da sky to see da dragonhawk!', 14, 0, 0, 0, 0, 24223, 'Daakara dragonhawk'),
(23863, 3, 0, 'Dere be no hidin\' from da eagle!', 14, 0, 0, 0, 0, 24224, 'Daakara eagle'),
(23863, 4, 0, 'Fire kill you just as quick!', 14, 0, 0, 0, 0, 24225, 'Daakara fire breath'),
(23863, 5, 0, 'Da Amani de chuka!', 14, 0, 0, 0, 0, 24226, 'Daakara kill 1'),
(23863, 5, 1, 'Lot more gonna fall like you!', 14, 0, 0, 0, 0, 24227, 'Daakara kill2'),
(23863, 5, 2, 'Oh, look you... dead.', 14, 0, 0, 0, 0, 24228, 'Daakara kill 3'),
(23863, 6, 0, 'Let me introduce you to me new bruddas: fang and claw!', 14, 0, 0, 0, 0, 24230, 'Daakara lynx'),
(23863, 7, 0, 'De Zandalari give us strength. Nobody push around de Amani no more!', 14, 0, 0, 0, 0, 24353, 'Daakara aggro');

DELETE FROM `creature` WHERE `map`=859 AND `id` IN (52288, 52525, 52197, 52288, 52302);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (52288, 52525, 52197, 52288, 52302);

UPDATE `creature` SET `position_x`=-12020.12, `position_y`=-1699.96, `position_z`=39.54, `orientation`=0.60 WHERE `id`=52155;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=208844;

UPDATE `creature_template` SET `flags_extra`=2 WHERE `entry` IN (52331, 52332);

UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, flags_extra=128 WHERE `entry`=52302;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, flags_extra=128 WHERE `entry`=52288;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, flags_extra=128 WHERE `entry`=52525;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, flags_extra=128 WHERE `entry`=52197;

UPDATE `access_requirement` SET `level_min`=85 WHERE  `mapId`=859 AND `difficulty`=1;
UPDATE `access_requirement` SET `level_min`=86 WHERE  `mapId`=859 AND `difficulty`=0;

UPDATE `instance_template` SET `script`='instance_zulgurub' WHERE  `map`=859;

UPDATE `creature_template` SET `ScriptName`='boss_venoxis' WHERE `entry`=52155;
UPDATE `creature_template` SET `ScriptName`='npc_venoxis_venomous_effusion_stalker' WHERE `entry`=52302;
UPDATE `creature_template` SET `ScriptName`='npc_venoxis_venomous_effusion' WHERE `entry`=52288;
UPDATE `creature_template` SET `ScriptName`='npc_venoxis_bloodvenom' WHERE `entry`=52525;
UPDATE `creature_template` SET `ScriptName`='npc_venoxis_venoxis_pool_of_acid_tears' WHERE `entry`=52197;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (96475, 96477, 96521, 97089);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(96475, 'spell_venoxis_toxic_link'),
(96477, 'spell_venoxis_toxic_link_aura'),
(96521, 'spell_venoxis_pool_of_acid_tears_dmg'),
(97089, 'spell_venoxis_pool_of_acid_tears_dmg');

DELETE FROM `creature_text` WHERE `entry`=52155;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52155, 0, 0, 'My death means... nothing...', 14, 0, 0, 0, 0, 24318, 'Venoxis death'),
(52155, 1, 0, 'Let the coils of death unfurl!', 14, 0, 0, 0, 0, 24319, 'Venoxis event 01'),
(52155, 2, 0, 'Yes... ssssuccumb to the venom...', 14, 0, 0, 0, 0, 24320, 'Venoxis event 02'),
(52155, 3, 0, 'Hisss word, FILLS me, MY BLOOD IS VENOM, AND YOU WILL BATHE IN THE GLORY OF THE SNAKE GOD!!!', 14, 0, 0, 0, 0, 24321, 'Venoxis event 03'),
(52155, 4, 0, 'The mortal coil unwindsss...', 14, 0, 0, 0, 0, 24322, 'Venoxis kill 01'),
(52155, 4, 1, 'Your sssacrifice pleases him.', 14, 0, 0, 0, 0, 24323, 'Venoxis kill 02'),
(52155, 5, 0, 'You dissssturb the plans of Gurubashi, little one. It\'sss to late for you. Too late for all of you!', 14, 0, 0, 0, 0, 24326, 'Venoxis quest event 01');

UPDATE `creature_template` SET `unit_flags`=33554434, `flags_extra`=2 WHERE `entry` IN (52608);

DELETE FROM `creature` WHERE `map`=859 AND `id` IN (52137, 52154, 52157, 52156, 52324);
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN (52157, 52156, 52324);

UPDATE `gameobject_template` SET `flags`=4 WHERE `entry` IN (208850, 208845, 208846, 208847, 208848, 208849);

UPDATE `creature_template` SET `ScriptName`='boss_mandokir' WHERE `entry`=52151;
UPDATE `creature_template` SET `ScriptName`='npc_mandokir_chained_spirit' WHERE `entry`=52156;
UPDATE `creature_template` SET `ScriptName`='npc_mandokir_ohgan' WHERE `entry`=52157;

DELETE FROM `spell_script_names` WHERE `spell_id`=96776;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES (96776, 'spell_mandokir_bloodletting');

DELETE FROM `creature_text` WHERE `entry`=52151;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52151, 0, 0, 'Make peace, wormd. I be deliverin\' you to Hakkar myself!', 14, 0, 0, 0, 0, 24289, 'Mandokir aggro'),
(52151, 1, 0, 'My blood feeds Hakkar! My soul... feeds... Jin\'do!', 14, 0, 0, 0, 0, 24290, 'Mandokir death'),
(52151, 2, 0, 'Off with your head!', 14, 0, 0, 0, 0, 24291, 'Mandokir decapitate 01'),
(52151, 2, 1, 'Heads gonna roll!', 14, 0, 0, 0, 0, 24292, 'Mandokir decapitate 02'),
(52151, 2, 2, 'Ha ha ha! Is that all you got?', 14, 0, 0, 0, 0, 24293, 'Mandokir kill 01'),
(52151, 2, 3, 'Your broken body belongs to Hakkar!', 14, 0, 0, 0, 0, 24294, 'Mandokir kill 02'),
(52151, 2, 4, 'Fall to my feet. Fall to Mandokir, the bloodlord!', 14, 0, 0, 0, 0, 24295, 'Mandokir kill 03'),
(52151, 3, 0, 'Ding!', 14, 0, 0, 0, 0, 24297, 'Mandokir levelup'),
(52151, 4, 0, 'Go an\' get dem, Ohgan! We WON\'T be fooled again!', 14, 0, 0, 0, 0, 24298, 'Mandokir ohgan 01'),
(52151, 5, 0, 'Rise, Ohgan! Rise, and fight for your master!', 14, 0, 0, 0, 0, 24299, 'Mandokir ohgan 02');

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52148, 0, 0, 0, 0, 0, 37789, 0, 0, 0, 'Jin\'do the Godbreaker', '', '', 0, 87, 87, 3, 14, 14, 0, 2, 1.85714, 1, 1, 532, 798, 0, 199, 70, 2000, 2000, 1, 32768, 134219776, 13, 0, 0, 0, 0, 0, 426, 638, 159, 7, 104, 52148, 0, 0, 0, 0, 0, 0, 0, 0, 97600, 97170, 101360, 97321, 0, 0, 0, 0, 0, 0, 20033, 20033, '', 0, 3, 1, 70, 1, 1, 0, 69774, 0, 0, 0, 0, 0, 178, 1, 617299967, 1, 'boss_jindo_the_godbreaker', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52730, 0, 0, 0, 0, 0, 37854, 0, 0, 0, 'Gurubashi Spirit', '', '', 0, 86, 86, 3, 14, 14, 0, 0.666668, 1.42857, 1, 1, 516, 774, 0, 193, 35, 2000, 2000, 1, 32768, 2048, 0, 0, 0, 0, 0, 0, 413, 619, 154, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 97198, 97088, 97597, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 4, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 0, 'npc_jindo_gurubashi_spirit', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52222, 0, 0, 0, 0, 0, 37811, 0, 0, 0, 'Spirit of Hakkar', '', '', 0, 87, 87, 3, 14, 14, 0, 1.6, 1.71429, 1, 1, 532, 798, 0, 199, 35, 2000, 2000, 1, 256, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 7, 96, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 200, 1, 1, 0, 0, 0, 0, 0, 0, 0, 167, 1, 617299967, 0, 'npc_jindo_spirit_of_hakkar', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52430, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Hakkar\'s Chains', '', '', 0, 87, 87, 3, 14, 14, 0, 1, 0.992063, 1, 1, 532, 798, 0, 199, 35, 2000, 2000, 1, 0, 134217728, 0, 0, 0, 0, 0, 0, 426, 638, 159, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 10, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 653213695, 0, 'npc_jindo_chains_of_hakkar', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52532, 0, 0, 0, 0, 0, 1126, 11686, 0, 0, 'Spirit Portal', '', '', 0, 85, 85, 3, 14, 14, 0, 1, 1.14286, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 2048, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'npc_jindo_spirit_portal', 15595);

replace into spell_script_names values
(97158, 'spell_jindo_shadow_spike_target'),
(97152, 'spell_jindo_summon_spirit_target'),
(97597, 'spell_jindo_spirit_warrior_gaze_target');

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52059, 52393, 0, 0, 0, 0, 37805, 0, 0, 0, 'High Priestess Kilnara', '', '', 0, 87, 87, 3, 14, 14, 0, 1, 2.14286, 1, 1, 495, 742, 0, 186, 70, 2000, 2000, 2, 32832, 2048, 12, 0, 0, 0, 0, 0, 396, 594, 149, 7, 72, 52059, 0, 0, 0, 0, 0, 0, 0, 0, 96594, 96958, 96531, 96592, 0, 0, 0, 0, 0, 0, 20005, 20005, '', 0, 3, 1, 45, 5, 1, 0, 0, 0, 0, 0, 0, 0, 188, 1, 617299967, 1, 'boss_kilnara', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52061, 0, 0, 0, 0, 0, 37988, 0, 0, 0, 'Pride of Bethekk', '', '', 0, 84, 85, 3, 14, 14, 0, 4.4, 1.57143, 1, 1, 500, 750, 0, 187, 35, 2000, 2000, 1, 32768, 2048, 0, 2, 0, 0, 0, 0, 400, 600, 150, 1, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 98239, 97358, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2, 7, 1, 0, 0, 0, 0, 0, 0, 0, 162, 1, 0, 0, 'npc_kilnara_pride_of_bethekk', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52147, 0, 0, 0, 0, 0, 37815, 0, 0, 0, 'Wave of Agony', '', '', 0, 87, 87, 0, 14, 14, 0, 8, 2.85714, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 2048, 0, 0, 0, 0, 0, 0, 1, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 231, 1, 0, 0, 'npc_kilnara_wave_of_agony', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52160, 0, 0, 0, 0, 0, 16946, 0, 0, 0, 'Wave of Agony', '', '', 0, 87, 87, 0, 14, 14, 0, 4.8, 1.71429, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 1, 2, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 5, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 174, 1, 0, 128, 'npc_kilnara_wave_of_agony', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52387, 0, 0, 0, 0, 0, 11686, 1126, 0, 0, 'Cave In Stalker', '', '', 0, 87, 87, 3, 14, 14, 0, 2.2, 0.785714, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33587264, 2048, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 16778240, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 9, 7, 1, 0, 0, 0, 0, 0, 0, 0, 98, 1, 0, 128, 'npc_kilnara_wave_of_agony', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (53088, 0, 0, 0, 0, 0, 2176, 1418, 1141, 0, 'Temple Rat', '', 'LootAll', 0, 2, 5, 3, 190, 190, 16777216, 1, 1.14286, 1, 0, 5, 6, 0, 2, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 4, 5, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 'npc_kilnara_temple_rat', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (53108, 0, 0, 0, 0, 0, 2176, 1418, 1141, 0, 'Temple Rat', '', '', 0, 3, 3, 0, 190, 190, 0, 1.2, 1.42857, 1, 0, 5, 6, 0, 2, 1, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 4, 5, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 147, 1, 0, 0, 'npc_kilnara_temple_rat', 15595);

replace into spell_script_names values
(98269, 'spell_kilnara_wave_of_agony_target'),
(96457, 'spell_kilnara_wave_of_agony_start'),
(98238, 'spell_kilnara_rat_lure');

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52151, 0, 0, 0, 0, 0, 37816, 0, 0, 0, 'Bloodlord Mandokir', '', '', 0, 87, 87, 3, 14, 14, 0, 1, 1.42857, 1, 1, 399, 598, 0, 150, 70, 1500, 1500, 1, 64, 2048, 4, 0, 0, 0, 0, 0, 319, 478, 120, 7, 2147485768, 52151, 0, 0, 0, 0, 0, 0, 0, 0, 96776, 96684, 96740, 96800, 0, 0, 0, 0, 0, 1583, 20013, 20013, '', 0, 3, 1, 58, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 617299967, 1, 'boss_mandokir', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52156, 52169, 0, 0, 0, 0, 37794, 37793, 37796, 37797, 'Chained Spirit', '', '', 0, 87, 87, 3, 35, 35, 0, 1, 1.42857, 1, 0, 550, 950, 0, 36, 7, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 426, 638, 159, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96484, 96494, 96493, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 0.15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 144, 1, 0, 0, 'npc_mandokir_chained_spirit', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52157, 0, 0, 0, 0, 0, 37787, 0, 0, 0, 'Ohgan', '', '', 0, 87, 87, 3, 14, 14, 0, 1, 1, 1, 1, 399, 598, 0, 150, 46.7, 1500, 1500, 1, 32832, 2048, 0, 11, 0, 0, 0, 0, 319, 478, 120, 6, 72, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 2.5, 1, 1, 0, 0, 0, 0, 0, 0, 0, 113, 1, 0, 0, 'npc_mandokir_ohgan', 15595);

replace into spell_script_names value
(96776, 'spell_mandokir_bloodletting');

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52155, 0, 0, 0, 0, 0, 37788, 0, 0, 0, 'High Priest Venoxis', '', '', 0, 87, 87, 3, 14, 14, 0, 1, 1.14286, 1, 1, 495, 742, 0, 186, 70, 2000, 2000, 2, 32832, 2048, 4, 0, 0, 0, 0, 0, 396, 594, 149, 7, 104, 52155, 0, 0, 0, 0, 0, 0, 0, 0, 97354, 96842, 96509, 96515, 0, 0, 0, 0, 0, 0, 20007, 20007, '', 0, 3, 1, 60, 10, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 617299967, 1, 'boss_venoxis', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52302, 0, 0, 0, 0, 0, 1126, 11686, 0, 0, 'Venomous Effusion Stalker', '', '', 0, 87, 87, 3, 35, 35, 0, 1, 1.14286, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 2048, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 128, 'npc_venoxis_venomous_effusion_stalker', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52288, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Venomous Effusion', '', '', 0, 87, 87, 0, 14, 14, 0, 2, 1.71429, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 33556480, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 128, 'npc_venoxis_venomous_effusion', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52525, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Bloodvenom', '', '', 0, 87, 87, 0, 14, 14, 0, 1.2, 0.428571, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 33556480, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 76, 1, 0, 128, 'npc_venoxis_bloodvenom', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52197, 0, 0, 0, 0, 0, 11686, 0, 0, 0, 'Pool of Acrid Tears', '', '', 0, 87, 87, 0, 14, 14, 0, 2, 1.71429, 1, 0, 1, 2, 0, 0, 1, 2000, 2000, 1, 33554432, 33556480, 0, 0, 0, 0, 0, 0, 1, 2, 0, 10, 1074790400, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 3, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 169, 1, 0, 128, 'npc_venoxis_venoxis_pool_of_acid_tears', 15595);

replace into spell_script_names values
(96475, 'spell_venoxis_toxic_link'),
(96477, 'spell_venoxis_toxic_link_aura'),
(96521, 'spell_venoxis_pool_of_acid_tears_dmg'),
(97089, 'spell_venoxis_pool_of_acid_tears_dmg');

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52053, 0, 0, 0, 0, 52882, 37813, 0, 0, 0, 'Zanzil', '', '', 0, 87, 87, 3, 28, 28, 0, 1, 1.14286, 1, 1, 495, 742, 0, 186, 70, 2000, 2000, 2, 32832, 2048, 4, 0, 0, 0, 0, 0, 396, 594, 149, 7, 72, 52053, 0, 0, 0, 0, 0, 0, 0, 0, 96488, 96487, 96486, 96349, 0, 0, 0, 0, 0, 0, 19995, 19995, '', 0, 3, 1, 50, 50, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 617299967, 1, 'boss_zanzil', 15595);

REPLACE INTO `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `faction_A`, `faction_H`, `npcflag`, `speed_walk`, `speed_run`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) VALUES (52054, 52142, 0, 0, 0, 0, 25491, 0, 0, 0, 'Zanzili Berserker', '', '', 0, 85, 85, 3, 107, 107, 0, 0.666668, 1.14286, 1, 1, 500, 750, 0, 187, 35, 2000, 2000, 1, 0, 2048, 0, 0, 0, 0, 0, 0, 400, 600, 150, 7, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 96332, 96341, 96306, 96342, 0, 0, 0, 0, 0, 0, 0, 0, '', 0, 4, 1, 15, 1, 1, 0, 0, 0, 0, 0, 0, 0, 869, 1, 617299967, 0, 'npc_zanzil_berserker', 15595);

replace into spell_script_names values
(96342, 'spell_zanzil_pursuit'),
(96914, 'spell_zanzil_fire'),
(96331, 'spell_frostburn_formula');

replace into instance_template values (859, 0, 'instance_zulgurub', 1);
update instance_template set parent=0 where script='instance_zulaman';

DELETE FROM areatrigger_teleport WHERE NAME LIKE "%gurub%";
insert into areatrigger_teleport values
(3930, 'ZulGurub Exit', 0, -11916.3, -1208.37, 92.2868, 1.61792),
(6775, 'ZulGurub Enter', 859, -11917, -1230.14, 92.29, 4.78),
(6769, 'ZulGurub Exit', 0, -11916.2, -1209.47, 92.2873, 1.5708);

replace into areatrigger_teleport values
(4738, 'ZulAman Enter', 568, 120.7, 1776, 43.46, 4.7713),
(4739, 'ZulAman Exit', 530, 6851.5, -7997.68, 192.36, 1.56688);

update access_requirement set level_min=85 where mapId IN (859, 568);
