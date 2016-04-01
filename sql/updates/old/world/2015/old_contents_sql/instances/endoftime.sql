UPDATE `creature` SET `spawnmask`=3 WHERE `id`=`id`=52530;
UPDATE `creature_template` SET `lootid`=0 WHERE `entry`=52530;
UPDATE `creature_template` SET `lootid`=0 WHERE `entry`=54044;
UPDATE `creature_template` SET `lootid`=0 WHERE `entry`=54045;
UPDATE `creature_template` SET `lootid`=0 WHERE `entry`=54046;

DELETE FROM `creature` WHERE `map`=938 AND `id` IN 
(54574, 54580, 54581, 54582, 54939, 54940, 54941, 54942, 54197, 54952);

UPDATE `creature` SET `spawnmask`=0 WHERE `map`=938 AND `id` IN (54700, 54688, 54699, 54512, 54701);

UPDATE `creature` SET `position_x`=4169.71, `position_y`=-433.40, `position_z`=120.0, `orientation`=2.59 WHERE `id`=54432;
UPDATE `creature_template_addon` SET `bytes1`=0 WHERE `entry`=54432;
UPDATE `creature_template` SET `inhabittype`=1 WHERE `entry`=54432;

UPDATE `creature_template_addon` SET `auras`='102150' WHERE `entry` IN (54574, 54580, 54581, 54582);

UPDATE `creature_template` SET `unit_flags`=32832 WHERE `entry`=54544;
UPDATE `creature_template_addon` SET `bytes1`=0, `auras`='' WHERE `entry`=54544;
UPDATE `creature_template` SET `modelid2`=0, `Inhabittype`=7, `flags_extra`=128 WHERE `entry` IN (54574, 54580, 54581, 54582);
UPDATE `gameobject_template` SET `faction`=35, `flags`=0 WHERE `entry` IN (209438, 209441, 209440, 209442, 209443, 209437, 209439);

UPDATE `instance_template` SET `script`='instance_end_time' WHERE `map`=938;

UPDATE `gameobject_template` SET `ScriptName`='go_end_time_teleport' WHERE `entry` IN (209438, 209441, 209440, 209442, 209443, 209437, 209439);

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_tyrande' WHERE `entry`=54544;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_moonlance' WHERE `entry` IN (54574, 54580, 54581, 54582); 

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_sylvanas' WHERE `entry`=54123;

UPDATE `creature_template` SET `ScriptName`='boss_murozond' WHERE `entry`=54432;

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(102242, 'spell_echo_of_tyrande_tears_of_elune_script');

DELETE FROM `creature_text` WHERE `entry` = 54544;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54544, 0, 0, 'Let the peaceful light of Elune sooth your souls in this dark time.', 12, 0, 100, 0, 0, 25972, 'Tyrande '),
(54544, 1, 0, 'Moon goddess, your light dims! I am lost without your guidance!', 14, 0, 100, 0, 0, 25973, 'Tyrande 80%'),
(54544, 2, 0, 'Tyrande gains Lunar Guidance!', 41, 0, 100, 0, 0, 0, 'Tyrande 80% and 55% emote'),
(54544, 3, 0, 'The darkness closes in...my vision is clouded...', 14, 0, 100, 0, 0, 25974, 'Tyrande 55%'),
(54544, 4, 0, 'Mother moon, I can no longer see your light! Your daughter is alone in the darkness!', 14, 0, 100, 0, 0, 25975, 'Tyrande '),
(54544, 5, 0, 'Tears of Elune begin to rain from the sky!', 41, 0, 100, 0, 0, 0, 'Tyrande 30% emote'),
(54544, 6, 0, 'I can...see the light of the moon...so clearly now. It is...beautiful...', 12, 0, 100, 0, 0, 25976, 'Tyrande death'),
(54544, 7, 0, 'There is nothing left for you here, nothing but death and sorrow.', 12, 0, 100, 0, 0, 25977, 'Tyrande intro 01'),
(54544, 8, 0, 'The darkness surrounds you, the light of Elune is your only salvation.', 12, 0, 100, 0, 0, 25978, 'Tyrande event 02'),
(54544, 9, 0, 'The moonlight can bring rest to your weary souls in this forgotten place.', 12, 0, 100, 0, 0, 25979, 'Tyrande event 03'),
(54544, 10, 0, 'Give yourselves to the night, Elune will guide you from this mortal prison.', 12, 0, 100, 0, 0, 25980, 'Tyrande event 04'),
(54544, 11, 0, 'You have chosen a path of darkness. Mother moon, guide my hand; allow me to bring rest to these misbegotten souls.', 12, 0, 100, 0, 0, 25981, 'Tyrande event 05'),
(54544, 12, 0, 'Elune guide you through the night.', 12, 0, 100, 0, 0, 25982, 'Tyrande kill'),
(54544, 13, 0, 'Eyes of night, pierce this darkness!', 14, 0, 100, 0, 0, 25983, 'Tyrande spell 01'),
(54544, 14, 0, 'Spear of Elune, drive back the night!', 14, 0, 100, 0, 0, 25984, 'Tyrande spell 02');

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(102579, 938, 3833.332520, 1101.351929, 83.412987, 3.786126),
(102564, 938, 3711.591064, -375.388763, 113.064049, 2.235886),
(102126, 938, 2997.370850, 570.221863, 25.307350, 5.543086),
(104761, 938, 2959.332275, 64.801781, 11.46, 2.95),
(103868, 938, 4343.328613, 1294.588135, 147.503693, 0.765022),
(104764, 938, 4042.709717, -351.774353, 122.215546, 4.328253);

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES 
(101398, 938, 3845.51, 909.318, 56.1463, 1.309);

DELETE FROM `creature` WHERE `map`=938 AND `id` IN 
(54594, 54508, 54496, 54434, 
54585, 54494, 54446, 54639, 
54641, 54566, 54795);

UPDATE `creature_model_info` SET `bounding_radius`=5, `combat_reach`=5 WHERE `modelid`=39559;
UPDATE `creature_template` SET `speed_walk`=0.5, `speed_run`=0.5 WHERE `entry` = 54191;
UPDATE `creature_template` SET `unit_flags`=33555206, `flags_extra`=128 WHERE `entry`=54550;
UPDATE `creature_template` SET `flags_extra`=128 WHERE `entry` IN (54197, 54508, 54542, 54446);
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0, `flags_extra`=0 WHERE `entry`=54434;
UPDATE `creature_template` SET `modelid1`=11686, `modelid2`=0, `flags_extra`=128 WHERE `entry`=54639;
UPDATE `creature_template` SET `unit_flags`=33024 WHERE `entry`=54867;
UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry`=54191;
UPDATE `creature_template` SET `unit_flags`=32832 WHERE `entry`=54445;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87 WHERE `entry`=54446;
UPDATE `creature_template` SET `minlevel`=87, `maxlevel`=87, `flags_extra`=128 WHERE `entry`=54494;
UPDATE `gameobject_template` SET `flags`=0 WHERE `entry`=209249;


UPDATE `creature_template` SET `ScriptName`='npc_echo_of_tyrande_pool_of_moonlight' WHERE `entry`=54508;

REPLACE INTO `areatrigger_scripts` (`entry`, `ScriptName`) VALUES 
(6955, 'at_et_tyrande');

DELETE FROM `conditions` WHERE `SourceEntry` = 101842;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '3', '101842', '0', '31', '0',  '3', '54512', 'Echo of Tyrande - Moonlight'),
('13', '3', '101842', '1', '31', '0',  '3', '54701', 'Echo of Tyrande - Moonlight'),
('13', '3', '101842', '2', '31', '0',  '3', '54688', 'Echo of Tyrande - Moonlight'),
('13', '3', '101842', '3', '31', '0',  '3', '54699', 'Echo of Tyrande - Moonlight'),
('13', '3', '101842', '4', '31', '0',  '3', '54700', 'Echo of Tyrande - Moonlight');

UPDATE `creature_template` SET `ScriptName`='npc_echo_of_sylvanas_ghoul' WHERE `entry`=54197;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_sylvanas_risen_ghoul' WHERE `entry`=54191;

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101257, 'spell_echo_of_sylvanas_wracking_pain_dmg'),
(101397, 'spell_echo_of_sylvanas_death_grip_aoe'),
(103175, 'spell_echo_of_sylvanas_seeping_shadows');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (18138, 18499);
REPLACE INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(18499, 11, 0, 0, 'achievement_several_ties');

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_baine' WHERE `entry`=54431;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_baine_baines_totem' WHERE `entry`=54434;

REPLACE INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES 
(101603, -107837, 0, 'Echo of Baine - Throw Totem'),
(101603, -101601, 0, 'Echo of Baine - Throw Totem');

REPLACE INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(101625, 'spell_echo_of_baine_pulverize_aoe');

UPDATE `creature_template` SET `ScriptName`='boss_echo_of_jaina' WHERE `entry`=54445;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_jaina_blink_target' WHERE `entry`=54542;
UPDATE `creature_template` SET `ScriptName`='npc_echo_of_jaina_flarecore' WHERE `entry`=54446;
UPDATE `gameobject_template` SET `ScriptName`='go_echo_of_jaina_jaina_staff_fragment' WHERE `entry`=209318;

UPDATE `gameobject_template` SET `ScriptName`='go_murozond_hourglass_of_time' WHERE `entry`=209249;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_nozdormu' WHERE `entry`=54751;
UPDATE `creature_template` SET `ScriptName`='npc_murozond_mirror_image' WHERE `entry`=54435;


UPDATE `creature_template` SET `ScriptName`='npc_end_time_image_of_nozdormu' WHERE `entry`=54867;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_infinite_warden' WHERE `entry`=54923;
UPDATE `creature_template` SET `ScriptName`='npc_end_time_infinite_suppressor' WHERE `entry`=54920;

DELETE FROM `creature_text` WHERE `entry`=54867;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54867, 0, 0, 'There is an unnatural darkness to this place, a perpetual midnight. Take caution, heroes, and do not stray from the light.', 12, 0, 100, 0, 0, 25955, 'Nozdormu intro 01'),
(54867, 1, 0, 'The undying flames are all that remain of this sacred place. I sense much anger here...a seething rage, barely held in check. Be on your guard.', 12, 0, 100, 0, 0, 25956, 'Nozdormu intro 02'),
(54867, 2, 0, 'This is all that is left of the Blue Dragonshrine. A great battle shattered the timewaves leading out of this forsaken place. You must reconstruct the fragments strewn across the ground and defeat the trapped spirit to proceed.', 12, 0, 100, 0, 0, 25957, 'Nozdormu intro 03'),
(54867, 3, 0, 'This is where she stood, heroes, and this is where she fell. The time-lost echo of Sylvanas Windrunner will reverberate through the rotting limbs of the Dragonshrine for all eternity.', 12, 0, 100, 0, 0, 25958, 'Nozdormu intro 04'),
(54867, 4, 0, 'You must give peace to these lands if you are to face Murozond.', 12, 0, 100, 0, 0, 25959, 'Nozdormu intro 05');

DELETE FROM `creature_text` WHERE `entry`=54751;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54751, 0, 0, 'Mortals! I cannot follow you any further - accept my blessing and use the Hourglass of Time to defeat Murozond!', 14, 0, 100, 0, 0, 25943, 'Nozdormu event 01'),
(54751, 1, 0, 'At last it has come to pass. The moment of my demise. The loop is closed. My future self will cause no more harm.', 12, 0, 100, 0, 0, 25944, 'Nozdormu event 02'),
(54751, 2, 0, 'Still, in time, I will... fall to madness. And you, heroes... will vanquish me. The cycle will repeat. So it goes.', 12, 0, 100, 0, 0, 25945, 'Nozdormu event 03'),
(54751, 3, 0, 'What matters is that Azeroth did not fall; that we survived to fight another day.', 12, 0, 100, 0, 0, 25946, 'Nozdormu event 04'),
(54751, 4, 0, 'Nozdormu turns away from where Murozond died and looks up at the Hourglass of Time.', 16, 0, 100, 0, 0, 0, 'Nozdormu emote'),
(54751, 5, 0, 'All that matters... is this moment.', 12, 0, 100, 0, 0, 25947, 'Nozdormu event 05');

DELETE FROM `creature_text` WHERE `entry`=54432;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54432, 0, 0, 'So be it.', 14, 0, 100, 0, 0, 25927, 'Murozond aggro'),
(54432, 1, 0, 'You know not what you have done. Aman\'Thul... What I... have... seen...', 14, 0, 100, 0, 0, 25928, 'Murozond death'),
(54432, 2, 0, 'The powers of the Hourglass do nothing to me!', 14, 0, 100, 0, 0, 25929, 'Murozond event 01'),
(54432, 3, 0, 'To repeat the same action and expect different results is madness.', 14, 0, 100, 0, 0, 25930, 'Murozond event 02'),
(54432, 4, 0, 'Another chance will make no difference. You will fail.', 14, 0, 100, 0, 0, 25931, 'Murozond event 03'),
(54432, 5, 0, 'Again...? Is this your plot, your scheme?', 14, 0, 100, 0, 0, 25932, 'Murozond event 04'),
(54432, 6, 0, 'The Hourglass\' power is exhausted. No more games, mortals. Relent, or perish.', 14, 0, 100, 0, 0, 25933, 'Murozond event 05'),
(54432, 7, 0, 'The "End Time," I once called this place. I had not seen, by then; I did not know. You hope to... what? Stop me, here? Change the fate I worked so tirelessly to weave?', 14, 0, 100, 0, 0, 25934, 'Murozond intro 01'),
(54432, 8, 0, 'You crawl unwitting, like a blind, writhing worm, towards endless madness and despair. I have witnessed the true End Time. This? This is a blessing you simply cannot comprehend.', 14, 0, 100, 0, 0, 25935, 'Murozond intro 02'),
(54432, 9, 0, 'You asked for it.', 14, 0, 100, 0, 0, 25936, 'Murozond kill 01'),
(54432, 9, 1, 'The sand has run out.', 14, 0, 100, 0, 0, 25937, 'Murozond kill 02'),
(54432, 9, 2, 'Time ends.', 14, 0, 100, 0, 0, 25938, 'Murozond kill 03');

DELETE FROM `creature_text` WHERE `entry`=54445;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54445, 0, 0, 'You asked for it.', 14, 0, 100, 0, 0, 25917, 'Jaina aggro 01'),
(54445, 0, 1, 'I hate resorting to violence.', 14, 0, 100, 0, 0, 25918, 'Jaina aggro 02'),
(54445, 1, 0, 'I understand, now. Farewell, and good luck.', 12, 0, 100, 0, 0, 25919, 'Jaina death'),
(54445, 2, 0, 'I don\'t know who you are, but I\'ll defend this shrine with my life. Leave, now, before we come to blows.', 12, 0, 100, 0, 0, 25920, 'Jaina intro'),
(54445, 3, 0, 'You forced my hand.', 14, 0, 100, 0, 0, 25921, 'Jaina kill 01'),
(54445, 3, 1, 'I didn\'t want to do that.', 14, 0, 100, 0, 0, 25922, 'Jaina kill 02'),
(54445, 3, 2, 'I wish you\'d surrendered!', 14, 0, 100, 0, 0, 25923, 'Jaina kill 03'),
(54445, 4, 0, 'Perhaps this will cool your heads...', 14, 0, 100, 0, 0, 25924, 'Jaina spell 01'),
(54445, 4, 1, 'A little ice ought to quench the fire in your hearts...', 14, 0, 100, 0, 0, 25925, 'Jaina spell 02'),
(54445, 4, 2, 'Why won\'t you give up?!', 14, 0, 100, 0, 0, 25926, 'Jaina spell 03');

DELETE FROM `creature_text` WHERE `entry`=54431;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54431, 0, 0, 'What dark horrors have you brought in this place? By my ancestors honor - I shall take you to task!', 14, 0, 100, 0, 0, 25909, 'Baine aggro'),
(54431, 1, 0, 'Where... is this place? What... have I done? Forgive me, my father...', 14, 0, 100, 0, 0, 25910, 'Baine death'),
(54431, 2, 0, 'You! Are you the ones responsible for what has happened here...?', 14, 0, 100, 0, 0, 25911, 'Baine intro'),
(54431, 3, 0, 'This is the price you pay.', 14, 0, 100, 0, 0, 25912, 'Baine kill 01'),
(54431, 3, 1, 'A just punishment!', 14, 0, 100, 0, 0, 25913, 'Baine kill 02'),
(54431, 3, 2, 'Suffer for your arrogance!', 14, 0, 100, 0, 0, 25914, 'Baine kill 03'),
(54431, 4, 0, 'There will be no escape!', 14, 0, 100, 0, 0, 25915, 'Baine spell 01'),
(54431, 4, 1, 'My wrath knows no bounds!', 14, 0, 100, 0, 0, 25916, 'Baine spell 02');

DELETE FROM `creature_text` WHERE `entry`=54123;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54123, 0, 0, 'Another band of Deathwing\'s converts? I\'ll be sure your death is especially painful!', 14, 0, 100, 0, 0, 25966, 'Sylvanas aggro'),
(54123, 1, 0, 'This... isn\'t... how it\'s supposed... to end.', 14, 0, 100, 0, 0, 25967, 'Sylvanas death'),
(54123, 2, 0, 'And so ends your story!', 14, 0, 100, 0, 0, 25968, 'Sylvanas reset'),
(54123, 3, 0, 'Cry havoc!', 14, 0, 100, 0, 0, 25969, 'Sylvanas kill'),
(54123, 4, 0, 'Watch, heathens, as death surrounds you!', 14, 0, 100, 0, 0, 25970, 'Sylvanas spell 01');

DELETE FROM `creature_text` WHERE `entry` = 54544;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(54544, 0, 0, 'Let the peaceful light of Elune sooth your souls in this dark time.', 12, 0, 100, 0, 0, 25972, 'Tyrande '),
(54544, 1, 0, 'Moon goddess, your light dims! I am lost without your guidance!', 14, 0, 100, 0, 0, 25973, 'Tyrande 80%'),
(54544, 2, 0, 'Tyrande gains Lunar Guidance!', 41, 0, 100, 0, 0, 0, 'Tyrande 80% and 55% emote'),
(54544, 3, 0, 'The darkness closes in...my vision is clouded...', 14, 0, 100, 0, 0, 25974, 'Tyrande 55%'),
(54544, 4, 0, 'Mother moon, I can no longer see your light! Your daughter is alone in the darkness!', 14, 0, 100, 0, 0, 25975, 'Tyrande '),
(54544, 5, 0, 'Tears of Elune begin to rain from the sky!', 41, 0, 100, 0, 0, 0, 'Tyrande 30% emote'),
(54544, 6, 0, 'I can...see the light of the moon...so clearly now. It is...beautiful...', 12, 0, 100, 0, 0, 25976, 'Tyrande death'),
(54544, 7, 0, 'There is nothing left for you here, nothing but death and sorrow.', 12, 0, 100, 0, 0, 25977, 'Tyrande intro 01'),
(54544, 8, 0, 'The darkness surrounds you, the light of Elune is your only salvation.', 12, 0, 100, 0, 0, 25978, 'Tyrande event 02'),
(54544, 9, 0, 'The moonlight can bring rest to your weary souls in this forgotten place.', 12, 0, 100, 0, 0, 25979, 'Tyrande event 03'),
(54544, 10, 0, 'Give yourselves to the night, Elune will guide you from this mortal prison.', 12, 0, 100, 0, 0, 25980, 'Tyrande event 04'),
(54544, 11, 0, 'You have chosen a path of darkness. Mother moon, guide my hand; allow me to bring rest to these misbegotten souls.', 12, 0, 100, 0, 0, 25981, 'Tyrande event 05'),
(54544, 12, 0, 'Elune guide you through the night.', 12, 0, 100, 0, 0, 25982, 'Tyrande kill'),
(54544, 13, 0, 'Eyes of night, pierce this darkness!', 14, 0, 100, 0, 0, 25983, 'Tyrande spell 01'),
(54544, 14, 0, 'Spear of Elune, drive back the night!', 14, 0, 100, 0, 0, 25984, 'Tyrande spell 02'),
(54544, 15, 0, 'A pool of moonlight appears!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight 1'),
(54544, 16, 0, 'A pool of moonlight appears to the west!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight 2'),
(54544, 17, 0, 'A pool of moonlight appears to the south!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight 3'),
(54544, 18, 0, 'A pool of moonlight appears to the east!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight 4'),
(54544, 19, 0, 'A pool of moonlight appears to the north!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight 5'),
(54544, 20, 0, 'A pool of dark moonlight appears nearby!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight 6'),
(54544, 21, 0, 'The moonlight fades into darkness!', 41, 0, 100, 0, 0, 0, 'Tyrande moonlight left');

