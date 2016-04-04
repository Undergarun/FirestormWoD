
DELETE FROM `creature` WHERE `map`=967 AND `id`=57836;
DELETE FROM `creature` WHERE `map`=967 AND `id`=49794;

UPDATE `creature` SET `spawnmask`=15 WHERE `id`=55308 AND `map`=967;

DELETE FROM `creature` WHERE `map`=967 AND `id` IN (55416, 57875);
INSERT INTO `creature` (`id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(57875, 967, 15, 1, 28615, 0, -1792.2, -1988.63, -221.373, 1.41372, 7200, 0, 0, 2402190, 0, 0, 0, 0, 0),
(57875, 967, 15, 1, 28615, 0, -1839.37, -1895.09, -221.381, 5.98648, 7200, 0, 0, 2402190, 0, 0, 0, 0, 0),
(57875, 967, 15, 1, 28615, 0, -1696.95, -1941.09, -221.292, 1.90241, 7200, 0, 0, 2402190, 0, 0, 0, 0, 0);


UPDATE `creature_template_addon` SET `auras`='' WHERE `entry`=55334;
UPDATE `creature_template` SET `modelid1`=39108, `modelid2`=0 WHERE `entry`=55334;
UPDATE `creature_template` SET `modelid1`=39108, `modelid2`=0 WHERE `entry`=58473;
UPDATE `creature_template` SET `unit_flags`=`unit_flags`&~33554432 WHERE `entry` IN 
(55418, 55757, 55758, 55759, 55416, 55751, 55752, 55753, 55417, 55754, 55755, 55756);

UPDATE `creature_template` SET `mechanic_immune_mask`=617299967 WHERE `entry` IN 
(55418, 55757, 55758, 55759, 55416, 55751, 55752, 55753, 55417, 55754, 55755, 55756,
55308, 55309, 55310, 55311, 55334, 57877, 57878, 57890, 57891, 57875, 57876);
UPDATE `creature_template` SET `npcflag`=1 WHERE `entry` IN (57289, 57287);
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (57289, 57287);

UPDATE `creature_template` SET `dmg_multiplier`=`dmg_multiplier`*3 WHERE `entry` IN 
(57877, 57878, 57890, 57891, 55418, 55757, 55758, 55759, 55417, 55754, 55755, 55756,
57817, 57818, 57823, 57824, 57807, 57808, 57819, 57820, 57821, 57822);

UPDATE `creature_template` SET `mindmg`=548, `maxdmg`=822 WHERE `entry` IN 
(55308, 55309, 55310, 55311);
UPDATE `creature_template` SET `dmg_multiplier`=97 WHERE `entry` IN
(55308, 55309);
UPDATE `creature_template` SET `dmg_multiplier`=110 WHERE `entry` IN
(55310, 55311);

UPDATE `creature_template` SET `ScriptName`='boss_warlord_zonozz' WHERE `entry`=55308;
UPDATE `creature_template` SET `ScriptName`='npc_warlord_zonozz_void_of_the_unmaking' WHERE `entry`=55334;
UPDATE `creature_template` SET `ScriptName`='npc_warlord_zonozz_tentacle' WHERE `entry` IN (55418, 55416, 55417);
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_flail_of_gorath' WHERE `entry`=57877;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_claw_of_gorath' WHERE `entry`=57890;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_eye_of_gorath' WHERE `entry`=57875;
UPDATE `creature_template` SET `ScriptName`='npc_dragon_soul_teleport' WHERE `entry` IN (57289, 57287);

DELETE FROM `spell_script_names` WHERE `spell_id` IN
(109874, 109875, 109876, 109877, 109878, 109879, 109880);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(109874, 'spell_warlord_zonozz_whisper'),
(109875, 'spell_warlord_zonozz_whisper'),
(109876, 'spell_warlord_zonozz_whisper'),
(109877, 'spell_warlord_zonozz_whisper'),
(109878, 'spell_warlord_zonozz_whisper'),
(109879, 'spell_warlord_zonozz_whisper'),
(109880, 'spell_warlord_zonozz_whisper');

DELETE FROM `conditions` WHERE `SourceEntry` IN (104031);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 104031, 0, 31, 0, 3, 55308, 'Warlord Zon\'ozz - Void Diffusion debuff');

DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (18494, 18481, 18446, 18456, 18457, 18688);
INSERT INTO `achievement_criteria_data` (`criteria_id`, `type`, `ScriptName`) VALUES 
(18494, 11, 'achievement_ping_pong_champion');

REPLACE INTO `creature_loot_currency` (`creature_id`, `CurrencyId1`, `CurrencyId2`, `CurrencyId3`, `CurrencyCount1`, `CurrencyCount2`, `CurrencyCount3`) VALUES 
(55308, 396, 614, 0, 12000, 1, 0),
(55309, 396, 614, 0, 14000, 1, 0),
(55310, 396, 614, 0, 12000, 1, 0),
(55311, 396, 614, 0, 14000, 1, 0);

DELETE FROM `creature_text` WHERE `entry`=55308;
INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(55308, 0, 0, 'Zzof Shuul\'wah. Thoq fssh N\'Zoth!', 14, 0, 100, 0, 0, 26335, 'zonozz aggro'),
(55308, 1, 0, 'Uovssh thyzz... qwaz...', 14, 0, 100, 0, 0, 26336, 'zonozz death'),
(55308, 2, 0, 'Vwyq agth sshoq\'meg N\'Zoth vra zz shfk qwor ga\'halahs agthu. Uulg\'ma, ag qam.', 14, 0, 100, 0, 0, 26337, 'zonozz intro'),
(55308, 3, 0, 'Sk\'yahf agth huqth N\'Zoth qornaus.', 14, 0, 100, 0, 0, 26338, 'zonozz slay 1'),
(55308, 3, 1, 'Sk\'shuul agth vorzz N\'Zoth naggwa\'fssh.', 14, 0, 100, 0, 0, 26339, 'zonozz slay 2'),
(55308, 4, 0, 'Sk\'shgn eqnizz hoq.', 14, 0, 100, 0, 0, 26340, 'zonozz spell 1'),
(55308, 3, 2, 'Sk\'tek agth nuq N\'Zoth yyqzz.', 14, 0, 100, 0, 0, 26341, 'zonozz slay 3'),
(55308, 4, 1, 'Sk\'magg yawifk hoq.', 14, 0, 100, 0, 0, 26342, 'zonozz spell 2'),
(55308, 4, 2, 'Sk\'uuyat guulphg hoq.', 14, 0, 100, 0, 0, 26343, 'zonozz spell 3'),
(55308, 5, 0, 'N\'Zoth ga zyqtahg iilth.', 14, 0, 100, 0, 0, 26344, 'zonozz spell 4'),
(55308, 6, 0, 'Gul\'kafh an\'qov N\'Zoth.', 14, 0, 100, 0, 0, 26345, 'zonozz spell 5'),
(55308, 7, 0, 'Victory for Deathwing. For the glory of N\'Zoth!', 42, 0, 100, 0, 0, 0, 'zonozz aggro whisper'),
(55308, 8, 0, 'To have waited so long... for this...', 42, 0, 100, 0, 0, 0, 'zonozz death whisper'),
(55308, 9, 0, 'Once more shall the twisted flesh-banners of N\'Zoth chitter and howl above the fly-blown corpse of this world. After millennia, we have returned.', 42, 0, 100, 0, 0, 0, 'zonozz intro whisper'),
(55308, 10, 0, 'Your souls shall sate N\'Zoth\'s endless hunger.', 42, 0, 100, 0, 0, 0, 'zonozz slay 1 whisper'),
(55308, 10, 1, 'Your deaths shall sing of N\'Zoth\'s unending glory.', 42, 0, 100, 0, 0, 0, 'zonozz slay 2 whisper'),
(55308, 11, 0, 'Your fear drives me.', 42, 0, 100, 0, 0, 0, 'zonozz spell 1 whisper'),
(55308, 10, 2, 'Your skulls shall adorn N\'Zoth\'s writhing throne.', 42, 0, 100, 0, 0, 0, 'zonozz slay 3 whisper'),
(55308, 11, 1, 'Your suffering strengthens me.', 42, 0, 100, 0, 0, 0, 'zonozz spell 2 whisper'),
(55308, 11, 2, 'Your agony sustains me.', 42, 0, 100, 0, 0, 0, 'zonozz spell 3 whisper'),
(55308, 12, 0, 'The will of N\'Zoth corrupts you.', 42, 0, 100, 0, 0, 0, 'zonozz spell 4 whisper'),
(55308, 13, 0, 'Gaze into the heart of N\'Zoth.', 42, 0, 100, 0, 0, 0, 'zonozz spell 5 whisper');

DELETE FROM `locales_creature_text` WHERE `entry`=55308;
INSERT INTO `locales_creature_text` (`entry`,`textGroup`,`id`,`text_loc8`) VALUES
(55308, 0, 0, 'Ззоф Шуул\'уах. Ток фшш Н\'Зот!'),
(55308, 1, 0, 'Уовшш тызз... куаз...'),
(55308, 2, 0, 'Вуык агт шшок\'мег Н\'Зот вра зз шфк куор га\'халас агту. Уулг\'ма, аг куам.'),
(55308, 3, 0, 'Ск\'яхф агт хукт Н\'Зот куорнаус.'),
(55308, 3, 1, 'Ск\'шуул агт ворцц Н\'Зот наггуа\'фшш.'),
(55308, 4, 0, 'Ск\'шгн экницц хок.'),
(55308, 3, 2, 'Ск\'тек агт нук Н\'Зот ыыкц.'),
(55308, 4, 1, 'Ск\'магг яуифк хок.'),
(55308, 4, 2, 'Ск\'ууят гуулфг хок.'),
(55308, 5, 0, 'Н\'Зот га зыктааг иилт.'),
(55308, 6, 0, 'Гул\'каф ан\'ков Н\'Зот.'),
(55308, 7, 0, 'Победа во имя Смертокрыла. Во славу Н\'Зота!'),
(55308, 8, 0, 'Так долго... этого ждать...'),
(55308, 9, 0, 'Вновь взовьются над трупом этого мира сотканные из живой плоти знамена Н\'Зота. Мы вернулись – спустя тысячелетия.'),
(55308, 10, 0, 'Ваши души избавят Н\'Зота от его вечного голода.'),
(55308, 10, 1, 'Ваши смерти – гимн славе Н\'Зота'),
(55308, 11, 0, 'Твой страх движет мной.'),
(55308, 10, 2, 'Ваши черепа украсят трон Н\'Зота..'),
(55308, 11, 1, 'Твои страдания – моя мощь.'),
(55308, 11, 2, 'Твоя агония придает мне силы.'),
(55308, 12, 0, 'Воля Н\'Зота оскверняет тебя.'),
(55308, 13, 0, 'Вглядись же в сердце Н\'Зота...');
