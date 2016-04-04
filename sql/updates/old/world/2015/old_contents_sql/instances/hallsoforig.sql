DELETE FROM `creature` WHERE `map`=644 AND `id` IN 
(40283, 40183, 39444, 40458,
 40808, 40450, 41257, 41144,
 41055, 41208, 41371, 40585,
 40669, 51329, 40592, 40622,
 40630, 40550, 40716, 39634,
 39635, 47922, 39370, 39369,
 39720, 39721, 39722, 39795,
 40503, 41194, 41245, 40835,
 47040, 41168, 40620, 40927);
 
UPDATE `creature_template_addon` SET `auras`='' WHERE `entry` IN
(40283, 40183, 39444, 40458,
 40808, 40450, 41257, 41144,
 41055, 41208, 41371, 40585,
 40669, 51329, 40592, 40622,
 40630, 40550, 40716, 39634,
 39635, 47922, 39370, 39369,
 39720, 39721, 39722, 39795,
 40503, 41194, 41245, 40787,
 49301, 40835, 41168, 41364, 
 49303);
 
UPDATE `creature` SET `spawnmask`=0 WHERE `id` IN (40251, 39804, 40252, 48707);

UPDATE `creature_template` SET `flags_extra`=`flags_extra`|128 WHERE `entry` IN (40790, 40459);

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN 
(40787, 49301, 39803, 39908,
39425, 39428, 39788, 39587,
39731, 39732, 39378, 39801,
39800, 39802, 48710, 48714,
48715, 48776, 48815, 48892,
48894, 48895, 48896, 48902,
49262, 41364, 49303);

REPLACE INTO `spell_target_position` (`id`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) 
VALUES (82916, 644, -536.462, 193.262, 80.25, 6.28);

UPDATE `creature_template` SET `unit_class`=4 WHERE `entry` IN (48815, 39378, 39731, 48715);

UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16, `minlevel`=85, `maxlevel`=85 WHERE `entry`=40283;
UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16, `minlevel`=85, `maxlevel`=85, `flags_extra`=128 WHERE `entry`=41144;
UPDATE `creature_template` SET `faction_a`=16, `faction_h`=16, `minlevel`=85, `maxlevel`=85, `flags_extra`=128 WHERE `entry`=39634;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=203133;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=203136;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=206207;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=206506;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202307;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202306;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202308;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202309;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202310;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202311;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202312;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202313;
UPDATE `gameobject_template` SET `flags`=4 WHERE `entry`=202314;
UPDATE `creature` SET `movementtype`=0 WHERE `map`=644;

UPDATE `instance_template` SET `script`='instance_halls_of_origination' WHERE `map`=644;

UPDATE `creature_template` SET `ScriptName`='boss_temple_guardian_anhuur' WHERE `entry`=39425;
UPDATE `creature_template` SET `ScriptName`='npc_pit_snake' WHERE `entry`=39444;
UPDATE `gameobject_template` SET `ScriptName`='go_beacon_of_light' WHERE `entry`=203133;
UPDATE `gameobject_template` SET `ScriptName`='go_beacon_of_light' WHERE `entry`=203136;
UPDATE `gameobject_template` SET `ScriptName`='go_halls_of_origination_transit_device' WHERE `entry`=204979;

DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=15988;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(15988, 11, 0, 0, 'achievement_i_hate_that_song');

UPDATE `creature_template` SET `ScriptName`='boss_earthrager_ptah' WHERE `entry`=39428;
UPDATE `creature_template` SET `ScriptName`='npc_ptah_dustbone_horror' WHERE `entry`=40450;

UPDATE `creature_template` SET `ScriptName`='npc_halls_of_origination_brann_bronzebeard' WHERE `entry`=39908;
UPDATE `creature_template` SET `ScriptName`='npc_air_warden' WHERE `entry`=39803;
UPDATE `creature_template` SET `ScriptName`='npc_flame_warden' WHERE `entry`=39800;
UPDATE `creature_template` SET `ScriptName`='npc_water_warden' WHERE `entry`=39802;
UPDATE `creature_template` SET `ScriptName`='npc_earth_warden' WHERE `entry`=39801;
UPDATE `creature_template` SET `ScriptName`='npc_water_warden_water_bubble' WHERE `entry`=41257;

UPDATE `creature_template` SET `ScriptName`='boss_anraphet' WHERE `entry`=39788;
UPDATE `creature_template` SET `ScriptName`='npc_alpha_beam' WHERE `entry`=41144;

UPDATE `creature_template` SET `ScriptName`='boss_setesh' WHERE `entry`=39732;
UPDATE `creature_template` SET `ScriptName`='npc_setesh_chaos_portal' WHERE `entry`=41055;
UPDATE `creature_template` SET `ScriptName`='npc_setesh_void_sentinel' WHERE `entry`=41208;
UPDATE `creature_template` SET `ScriptName`='npc_setesh_void_seeker' WHERE `entry`=41371;

UPDATE `creature_template` SET `ScriptName`='boss_ammunae' WHERE `entry`=39731;
UPDATE `creature_template` SET `ScriptName`='npc_ammunae_seedling_pod' WHERE `entry`=40550;
UPDATE `creature_template` SET `ScriptName`='npc_ammunae_spore' WHERE `entry`=40585;

DELETE FROM `conditions` WHERE `SourceEntry` IN (75657, 94970);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
('13', '1', '75657', '0', '31', '0',  '3', '39731', 'Ammunae - Energize'),
('13', '1', '94970', '0', '31', '0',  '3', '39731', 'Ammunae - Energize');

UPDATE `creature_template` SET `ScriptName`='boss_rajh' WHERE `entry`=39378;
UPDATE `creature_template` SET `ScriptName`='npc_rajh_solar_wind' WHERE `entry`=39634;
DELETE FROM `achievement_criteria_data` WHERE `criteria_id`=15990;
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `value1`, `value2`, `ScriptName`) VALUES
(15990, 11, 0, 0, 'achievement_sun_of_a');

UPDATE `creature_template` SET `ScriptName`='boss_isiset' WHERE `entry`=39587;
UPDATE `creature_template` SET `ScriptName`='npc_isiset_astral_rain' WHERE `entry`=39720;
UPDATE `creature_template` SET `ScriptName`='npc_isiset_celestial_call' WHERE `entry`=39721;
UPDATE `creature_template` SET `ScriptName`='npc_isiset_veil_of_sky' WHERE `entry`=39722;
UPDATE `creature_template` SET `ScriptName`='npc_isiset_astral_familiar' WHERE `entry`=39795;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (74137, 76670, 90758);
INSERT INTO `spell_script_names` VALUES 
(74137, 'spell_isiset_supernova_dis'),
(76670, 'spell_isiset_supernova_dmg'),
(90758, 'spell_isiset_supernova_dmg');

/* Anhuur */
DELETE FROM `creature_text` WHERE `entry`=39425;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39425, 0, 0, 'What... have you... done?', 14, 0, 0, 0, 0, 18579, 'VO_HO_Anhuur_Death'),
(39425, 1, 0, 'Turn back, intruders! These halls must not be disturbed!', 14, 0, 0, 0, 0, 18580, 'VO_HO_Anhuur_Engage'),
(39425, 2, 0, 'Beacons of light, bestow upon me your aegis!', 14, 0, 0, 0, 0, 18581, 'VO_HO_Anhuur_Event'),
(39425, 3, 0, 'I regret nothing!', 14, 0, 0, 0, 0, 18582, 'VO_HO_Anhuur_Slay01'),
(39425, 3, 1, 'A product of your own insolence!', 14, 0, 0, 0, 0, 18583, 'VO_HO_Anhuur_Slay02');

/* Ptah */
DELETE FROM `creature_text` WHERE `entry`=39428;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39428, 0, 0, 'Ptah... is... no more...', 14, 0, 0, 0, 0, 18905, 'VO_HO_Ptah_Death'),
(39428, 1, 0, 'More carrion for the swarm...', 14, 0, 0, 0, 0, 18906, 'VO_HO_Ptah_Engage'),
(39428, 2, 0, 'Dust to dust...', 14, 0, 0, 0, 0, 18907, 'VO_HO_Ptah_Slay01'),
(39428, 2, 1, 'HYAAAH!', 14, 0, 0, 0, 0, 18908, 'VO_HO_Ptah_Slay02');

/* Brann */
DELETE FROM `creature_text` WHERE `entry`=39908;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39908, 0, 0, 'Right, let\'s go. Just need to input the final entry sequence into the door mechanism and...', 12, 0, 0, 0, 0, 20897, 'VO_HO_Brann_Event01'),
(39908, 1, 0, ' That did the trick, the control room should be behind this--oh wow...', 12, 0, 0, 0, 0, 20898, 'VO_HO_Brann_Event02'),
(39908, 2, 0, 'What? This isn\'t the control room, there\'s another entire defensive mechanism in place and the blasted rock Troggs broke into here somehow. Troggs, why did it have to be Troggs...', 12, 0, 0, 0, 0, 20899, 'VO_HO_Brann_Event03'),
(39908, 3, 0, 'OK. Let me think a moment...', 12, 0, 0, 0, 0, 20900, 'VO_HO_Brann_Event04'),
(39908, 4, 0, 'Four platforms with huge elementals...', 12, 0, 0, 0, 0, 20901, 'VO_HO_Brann_Event05'),
(39908, 5, 0, 'Mirrors pointing all over the place...', 12, 0, 0, 0, 0, 20902, 'VO_HO_Brann_Event06'),
(39908, 6, 0, ' I got it! I saw a tablet that mention this chamber: this is the Vault of Lights! OK simple enough, I need you adventures to take out each of the four elementals to trigger the opening sequence to the far door.', 12, 0, 0, 0, 0, 20903, 'VO_HO_Brann_Event07'),
(39908, 7, 0, 'Brann Bronzeberd yells: One down!', 14, 0, 0, 0, 0, 20904, 'VO_HO_Brann_Event08'),
(39908, 8, 0, 'Brann Bronzeberd yells: Another one down! Just look at those light beams, they seem to be connecting to the far door.', 1, 0, 0, 0, 0, 20905, 'VO_HO_Brann_Event09'),
(39908, 9, 0, 'One more elemental to go! The door is almost open.', 14, 0, 0, 0, 0, 20906, 'VO_HO_Brann_Event10'),
(39908, 10, 0, 'That\'s it, you done it! The Vault door is opening, now we can--Oh no...', 14, 0, 0, 0, 0, 20907, 'VO_HO_Brann_Event11');

/* Anraphet */
DELETE FROM `creature_text` WHERE `entry`=39788;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39788, 0, 0, 'Anraphet unit shutting down...', 14, 0, 0, 0, 0, 20856, 'VO_HO_Anraphet_Death01'),
(39788, 1, 0, 'This unit has been activated outside normal operating protocols. Downloading new operational parameters. Download complete. Full unit self defense routines are now active. Destruction of foreign units in this system shall now commence.', 14, 0, 0, 0, 0, 20857, 'VO_HO_Anraphet_Intro01'),
(39788, 2, 0, 'Target Annihilated.', 14, 0, 0, 0, 0, 20858, 'VO_HO_Anraphet_Slay01'),
(39788, 2, 1, 'Purge Complete.', 14, 0, 0, 0, 0, 20859, 'VO_HO_Anraphet_Slay02'),
(39788, 3, 0, 'Alpha beams activated. Target tracking commencing.', 14, 0, 0, 0, 0, 20860, 'VO_HO_Anraphet_Special01'),
(39788, 4, 0, 'Omega Stance activated. Annihilation of foreign unit is now imminent.', 14, 0, 0, 0, 0, 20861, 'VO_HO_Anraphet_Special02'),
(39788, 5, 0, 'Purge of unauthorized entities commencing.', 14, 0, 0, 0, 0, 20862, 'VO_HO_Anraphet_Engage01');

/* Setesh */
DELETE FROM `creature_text` WHERE `entry`=39732;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39732, 0, 0, 'Yes! Harness... your... hatred.', 14, 0, 0, 0, 0, 18552, 'VO_HO_Setesh_Death'),
(39732, 1, 0, 'You fear that which you cannot control. But can you control your fear?', 14, 0, 0, 0, 0, 18553, 'VO_HO_Setesh_Engage'),
(39732, 2, 0, 'Embrace the end!', 14, 0, 0, 0, 0, 18555, 'VO_HO_Setesh_Slay01'),
(39732, 2, 1, 'Do you understand now?', 14, 0, 0, 0, 0, 18556, 'VO_HO_Setesh_Slay02');

/* Ammunae */
DELETE FROM `creature_text` WHERE `entry`=39731;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39731, 0, 0, 'The cycle continues...', 14, 0, 0, 0, 0, 18569, 'VO_HO_Ammunae_Death'),
(39731, 1, 0, 'This chamber will flourish with your life energy!', 14, 0, 0, 0, 0, 18571, 'VO_HO_Ammunae_Engage'),
(39731, 2, 0, 'Your life, UNLEASHED!', 14, 0, 0, 0, 0, 18572, 'VO_HO_Ammunae_Event'),
(39731, 3, 0, 'Wither away!', 14, 0, 0, 0, 0, 18573, 'VO_HO_Ammunae_Slay01'),
(39731, 3, 1, 'Waste of energy!', 14, 0, 0, 0, 0, 18574, 'VO_HO_Ammunae_Slay02');

/* Iziset */
DELETE FROM `creature_text` WHERE `entry`=39587;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39587, 0, 0, 'Eons of darkness... by your hand.', 14, 0, 0, 0, 0, 18842, 'VO_HO_Isiset_Death'),
(39587, 1, 0, 'Gaze to the heavens! What do you see?', 14, 0, 0, 0, 0, 18843, 'VO_HO_Isiset_Engage'),
(39587, 0, 1, 'My luster... wanes.', 14, 0, 0, 0, 0, 18844, 'VO_HO_Isiset_Event01'),
(39587, 2, 0, 'Bask in my radiance!', 14, 0, 0, 0, 0, 18845, 'VO_HO_Isiset_Event02'),
(39587, 3, 0, 'Insignificant!', 14, 0, 0, 0, 0, 18846, 'VO_HO_Isiset_Slay01'),
(39587, 3, 1, 'The glimmer of your life, extinguished.!', 14, 0, 0, 0, 0, 18847, 'VO_HO_Isiset_Slay02');

/* Rajh */
DELETE FROM `creature_text` WHERE `entry`=39378;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(39378, 0, 0, 'Blazing rays of light, take me!', 14, 0, 0, 0, 0, 18910, 'VO_HO_Rajh_Death'),
(39378, 1, 0, 'Defilers! Wretches! Fiends! Begone from here!', 14, 0, 0, 0, 0, 18911, 'VO_HO_Rajh_Engage'),
(39378, 2, 0, 'Can you feel it? The blessed warmth of the sun?', 14, 0, 0, 0, 0, 18912, 'VO_HO_Rajh_Event01'),
(39378, 3, 0, 'I send you to your deity.', 14, 0, 0, 0, 0, 18913, 'VO_HO_Rajh_Slay01'),
(39378, 3, 1, 'I will take this life as an offering!', 14, 0, 0, 0, 0, 18914, 'VO_HO_Rajh_Slay02');
