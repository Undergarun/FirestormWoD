UPDATE `creature_template` SET `ScriptName`='mob_snobold_vassal' WHERE `entry` = 34800;
UPDATE `creature_template` SET `modelid1`=25144, `flags_extra`=0 WHERE `entry` IN (34606, 34649);

-- Trial of the Crusader Death knight Death grip scriptname
DELETE FROM `spell_script_names` WHERE `spell_id` IN (66017, 68753, 68754, 68755);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66017, 'spell_faction_champion_death_grip'),
(68753, 'spell_faction_champion_death_grip'),
(68754, 'spell_faction_champion_death_grip'),
(68755, 'spell_faction_champion_death_grip');

-- Trial of the Crusader shaman heroism/bloodlust
DELETE FROM `spell_script_names` WHERE `spell_id` IN (65983, 65980);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(65983, 'spell_toc_heroism'),
(65980, 'spell_toc_bloodlust');

-- impale scriptname
DELETE FROM `spell_script_names` WHERE `spell_id`=65919;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(65919, 'spell_impale');

-- firebomb scriptname and modelid (ToC Gormok encounter)
UPDATE `creature_template` SET `ScriptName`="npc_firebomb" WHERE `entry`=34854;

-- frost sphere corrections
UPDATE `creature_template` SET `minlevel`=80, `InhabitType`=4, `RegenHealth`=0 WHERE `entry` IN (34606, 34649);

-- Dark / Light essence removing
DELETE FROM `spell_linked_spell` WHERE `spell_trigger` IN (-67222, -67223, -67224, -65686, -67176, -67177, -67178, -65684);
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(-67222, -67511, 0, 'Light Essence 25M'),
(-67223, -67512, 0, 'Light Essence 10M H'),
(-67224, -67513, 0, 'Light Essence 25M H'),
(-65686, -65811, 0, 'Light Essence 10M'),
(-67176, -67179, 0, 'Dark Essence 25M'),
(-67177, -67180, 0, 'Dark Essence 10M H'),
(-67178, -67181, 0, 'Dark Essence 25M H'),
(-65684, -65827, 0, 'Dark Essence 10M');

-- Gormoks Fire Bomb scriptname
DELETE FROM `spell_script_names` WHERE `spell_id`=66313;
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66313, 'spell_gormok_fire_bomb');

-- correcting hitbox of Anub'Arak
UPDATE `creature_model_info` SET `bounding_radius`=1.085, `combat_reach`=10.5 WHERE `modelid`=29268;

-- correcting hitbox of Acidmaw
UPDATE `creature_model_info` SET `bounding_radius`=1.24, `combat_reach`=12 WHERE `modelid`=29815;

-- spawn the Anub'arak gate in all versions of the instance
-- UPDATE `gameobject` SET `spawnMask`=15 WHERE `guid`=151192;

-- adding ToC boss immunities
UPDATE `creature_template` SET `mechanic_immune_mask`=`mechanic_immune_mask` | 1 | 2 | 4 | 8 | 16 | 32 | 64 | 128 | 256 | 512 | 1024 | 2048 | 4096 | 8192 | 65536 | 131072 | 524288 | 4194304 | 8388608 | 67108864 | 536870912 
	WHERE `entry` IN (34796, 35438, 35439, 35440, 34799, 35514, 35515, 35516, 35144, 35511, 35512, 35513, 34797, 35447, 35448, 35449, 34780, 35216, 35268, 35269, 36066, 35347, 35348, 35349, 34497, 35350, 35351, 35352, 34564, 34566, 35615, 35616);

-- adding Teleport locations to Trial of the Crusader/champion for GMs
DELETE FROM `game_tele` WHERE `name` LIKE '%TrialOfTheCrusader%' OR `name` LIKE '%TrialOfTheChampion%';
INSERT INTO `game_tele` (`position_x`, `position_y`, `position_z`, `orientation`, `map`, `name`) VALUES
(8515.63, 714.174, 558.248, 1.57298, 571, 'TrialOfTheCrusader'),
(8588.42, 791.888, 558.236, 3.23819, 571, 'TrialOfTheChampion');

-- misc fixes for higher boss brackets
UPDATE `creature_template` SET `dmg_multiplier`=70 WHERE `entry` IN (35440, 35513, 35516, 35449, 35269, 35352, 35349, 35616, 35664, 35670, 35673, 35676, 35682, 35685, 35688, 35691, 35694, 35697, 35701, 35704, 35707, 35710, 35713, 35716, 35720, 35723, 35726, 35730, 35733, 35736, 35739, 35742, 35745, 35748, 35749);
UPDATE `creature_template` SET `flags_extra`=`flags_extra` | 1 WHERE `entry` IN (35438, 35439, 35440, 35511, 35512, 35513, 35514, 35515, 35516, 35662, 35663, 35664, 35665, 35666, 35667, 35668, 35669, 35670, 35671, 35672, 35673, 35674, 35675, 35676, 35680, 35681, 35682, 35683, 35684, 35685, 35686, 35687, 35688, 35689, 35690, 35691, 35692, 35693, 35694, 35695, 35696, 35697, 35699, 35700, 35701, 35702, 35703, 35704, 35705, 35706, 35707, 35708, 35709, 35710, 35711, 35712, 35713, 35714, 35715, 35716, 35718, 35719, 35720, 35721, 35722, 35723, 35724, 35725, 35726, 35728, 35729, 35730, 35731, 35732, 35733, 35734, 35735, 35736, 35737, 35738, 35739, 35740, 35741, 35742, 35743, 35744, 35745, 35746, 35747, 35748, 34442, 34443, 35749);
UPDATE `creature_template` SET `speed_walk`=2.8, `speed_run`=1.71429 WHERE `entry` IN (35350, 35351, 35352, 35347, 35348, 35349);
UPDATE `creature_template` SET `speed_walk`=2, `speed_run`=1.14286 WHERE `entry` IN (34566, 35615, 35616);
UPDATE `creature_template` SET `mindmg`=388, `maxdmg`=583, `attackpower`=146 WHERE `entry` IN (35711, 35712, 35713);
UPDATE `creature_template` SET `mindmg`=468, `maxdmg`=702, `attackpower`=175 WHERE `entry` IN (35699, 35700, 35701);
UPDATE `creature_template` SET `dmg_multiplier`=35 WHERE `entry` IN (34472, 34454);
UPDATE `creature_template` SET `unit_class`=1 WHERE `entry` IN (34461, 35743, 35744, 35745);

-- adding Jaraxxus add immunities
UPDATE `creature_template` SET `mechanic_immune_mask`=`mechanic_immune_mask` | 1024 | 2048 WHERE `entry` IN (34815, 35262, 35263, 35264, 34826, 35270, 35271, 35272);
UPDATE `creature_template` SET `mechanic_immune_mask`=`mechanic_immune_mask` | 262144 WHERE `entry` IN (35263, 35264);
-- adding Nether portal and Infernal volcano immunities to knockout/grip
UPDATE `creature_template` SET `mechanic_immune_mask`=`mechanic_immune_mask` | 32 | 8192 WHERE `entry` IN (34825, 35278, 35279, 35280, 34813, 35265, 35266, 35267);

-- cast Forbearance together with Divine shield (ToC Faction Champions paladin)
DELETE FROM `spell_linked_spell` WHERE `spell_trigger`=66010;
INSERT INTO `spell_linked_spell` (`spell_trigger`, `spell_effect`, `type`, `comment`) VALUES
(66010, 25771, 0, 'Divine Shield - Forbearance');

-- make all Diminishing returns rules apply in faction champions
UPDATE `creature_template` SET `flags_extra`=`flags_extra` | 1048576 WHERE `entry` IN
(34445,35705,35706,35707,
34459,35686,35687,35688,
34447,35683,35684,35685,
34455,35680,35681,35682,
34453,35718,35719,35720,
34458,35692,35693,35694,
34454,35711,35712,35713,
34448,35724,35725,35726,
34441,34442,34443,35749,
34450,35695,35696,35697,
35610,35774,35775,35776,
35465,36301,36302,36303,
34451,35671,35672,35673,
34449,35689,35690,35691,
34444,35740,35741,35742,
34456,35708,35709,35710,
34460,35702,35703,35704,
34461,35743,35744,35745,
34463,35734,35735,35736,
34465,35746,35747,35748,
34466,35665,35666,35667,
34467,35662,35663,35664,
34468,35721,35722,35723,
34469,35714,35715,35716,
34470,35728,35729,35730,
34473,35674,35675,35676,
34474,35731,35732,35733,
34475,35737,35738,35739,
34471,35668,35669,35670,
34472,35699,35700,35701);

-- correcting faction champions dmg multipliers
UPDATE `creature_template` SET `dmg_multiplier`=10.8 WHERE `entry` IN (34445,34459,34447,34455,34453,34458,34454,34448,34441,34450,35610,35465,34451,34449,34444,34456,34460,34461,34463,34465,34466,34467,34468,34469,34470,34473,34474,34475,34472, 34471);
UPDATE `creature_template` SET `dmg_multiplier`=16.1 WHERE `entry` IN (35705,35706,35686,35687,35683,35684,35680,35681,35718,35719,35692,35693,35711,35712,35724,35725,34442,34443,35695,35696,35774,35775,36301,36302,35671,35672,35689,35690,35740,35741,35708,35709,35702,35703,35743,35744,35734,35735,35746,35747,35665,35666,35662,35663,35721,35722,35714,35715,35728,35729,35674,35675,35731,35732,35737,35738,35699,35700, 35668, 35669);
UPDATE `creature_template` SET `dmg_multiplier`=21.5 WHERE `entry` IN (35707,35688,35685,35682,35720,35694,35713,35726,35749,35697,35776,36303,35673,35691,35742,35710,35704,35745,35736,35748,35667,35664,35723,35716,35730,35676,35733,35739,35701, 35670);

-- ToC warlock pet db corrections
UPDATE `creature_template` SET `minlevel`=80, `maxlevel`=80, `exp`=2, `faction_A`=16, `faction_H`=16, `mindmg`=417, `maxdmg`=582, `attackpower`=608, `unit_class`=2, `dynamicflags`=8, `minrangedmg`=341, `maxrangedmg`=506, `rangedattackpower`=80 WHERE `entry` IN (36301, 36302, 36303);
UPDATE `creature_template` SET `faction_A`=16, `faction_H`=16, `difficulty_entry_1`=36301, `difficulty_entry_2`=36302, `difficulty_entry_3`=36303 WHERE `entry`=35465;
UPDATE `creature_template` SET `name`="Zhaagrym (1)" WHERE `entry`=36301;

-- Jaraxxus Mistress Kiss
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_mistress_kiss_area';
INSERT INTO `spell_script_names` (spell_id, `ScriptName`) VALUES
(66336, 'spell_mistress_kiss_area'),
(67076, 'spell_mistress_kiss_area'),
(67077, 'spell_mistress_kiss_area'),
(67078, 'spell_mistress_kiss_area');

DELETE FROM `spell_script_names` WHERE `ScriptName` = 'spell_mistress_kiss';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66334, 'spell_mistress_kiss'),
(67905, 'spell_mistress_kiss'),
(67906, 'spell_mistress_kiss'),
(67907, 'spell_mistress_kiss');

-- Gormoks Rising anger targeting
DELETE FROM `conditions` WHERE `SourceEntry`=66636;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `ConditionTypeOrReference`, `ConditionValue1`, `ConditionValue2`, `Comment`) VALUES
(13, 1, 66636, 31, 3, 34796, 'Rising Anger');

-- Twins loot correction
UPDATE `creature_loot_template` SET `maxcount`=1 WHERE `entry`=34497 AND `mincountOrRef` IN (-34296, -34302);
-- Anubarak loot correction
UPDATE `creature_loot_template` SET `maxcount`=2 WHERE `entry`=34564 AND `mincountOrRef` IN (-34298, -34304);

DELETE FROM `creature` WHERE `id`=36095;

-- ToCr orbs

SET @NPC_ORB1 := 34606; -- Frost Sphere
SET @NPC_ORB2 := 34649; -- Frost Sphere

UPDATE `creature_template` SET `speed_walk`=1.2,`speed_run`=1.42,`InhabitType`=7 WHERE `entry` IN (@NPC_ORB1,@NPC_ORB2);

DELETE FROM `creature_template_addon` WHERE `entry` IN (@NPC_ORB1,@NPC_ORB2);
INSERT INTO `creature_template_addon` (`entry`,`mount`,`bytes1`,`bytes2`,`auras`) VALUES
(@NPC_ORB1,0,0x3000000,0x1,''),
(@NPC_ORB2,0,0x3000000,0x1,'');

-- East Porticullis(195648): close and corrections
-- DELETE FROM `gameobject` WHERE `guid`=207834 and `id`=195648;
UPDATE `gameobject` SET `phaseMask`=1,`rotation2`=0.7071065,`rotation3`=0.707107,`spawntimesecs`=7200,`animprogress`=255,`state`=0 WHERE `id`=195648;

-- North Portcullis(195650): delete extra spawn and corrections
-- DELETE FROM `gameobject` WHERE `guid`=207833 and `id`=195650;
-- UPDATE `gameobject` SET `phaseMask`=1,`rotation2`=0.7071065,`rotation3`=0.707107,`spawntimesecs`=7200,`animprogress`=255,`state`=1 WHERE `guid`=28044;

-- Web Door(195485): corrections
-- UPDATE `gameobject` SET `spawnMask`=15,`rotation2`=0.7071067,`rotation3`=0.7071068,`spawntimesecs`=7200,`animprogress`=255 WHERE `guid`=2358;

-- Added trigger flags to triggers
UPDATE `creature_template` SET `flags_extra` = flags_extra | 128 WHERE `entry` = 34862;
UPDATE `creature_template` SET `unit_flags` = unit_flags | 256 WHERE `entry` = 34862;

DELETE FROM `script_texts` WHERE `entry` BETWEEN -1649999 AND -1649000;
DELETE FROM `creature_text` WHERE `entry` IN (34996,34990,34995,36095,34796,35144,34799,34797,34780,35458,34496,34497,16980,35877,34564,34660);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
-- Highlord Tirion Fordring
    -- Northrend Beasts
(34996, 0, 0, 'Welcome, champions! You have heard the call of the Argent Crusade and you have boldly answered! It is here, in the Crusaders'' Coliseum, that you will face your greatest challenges. Those of you who survive the rigors of the coliseum will join the Argent Crusade on its march to Icecrown Citadel.', 14, 0, 100, 0, 0, 16036, 'Highlord Tirion Fordring - Welcome'),
(34996, 1, 0, 'Hailing from the deepest, darkest caverns of the Storm Peaks, Gormok the Impaler! Battle on, heroes!', 14, 0, 100, 0, 0, 16038, 'Highlord Tirion Fordring - Summing Gormok the Impaler'),
(34996, 2, 0, 'Steel yourselves, heroes, for the twin terrors, Acidmaw and Dreadscale, enter the arena!', 14, 0, 100, 0, 0, 16039, 'Highlord Tirion Fordring - Summing Acidmaw and Dreadscale'),
(34996, 3, 0, 'The air itself freezes with the introduction of our next combatant, Icehowl! Kill or be killed, champions!', 14, 0, 100, 0, 0, 16040, 'Highlord Tirion Fordring Summing Icehowl'),
(34996, 4, 0, 'The monstrous menagerie has been vanquished!', 14, 0, 100, 0, 0, 16041, 'Highlord Tirion Fordring - Northrend Beasts Done'),
(34996, 5, 0, 'Tragic... They fought valiantly, but the beasts of Northrend triumphed. Let us observe a moment of silence for our fallen heroes.', 14, 0, 0, 0, 0, 16042, 'Highlord Tirion Fordring - Northrend Beasts FAIL'),
    -- Lord Jaraxxus
(34996, 6, 0, 'Grand Warlock Wilfred Fizzlebang will summon forth your next challenge. Stand by for his entry.', 14, 0, 100, 0, 0, 16043, 'Highlord Tirion Fordring - Summing Wilfred Fizzlebang'),
(34996, 7, 0, 'Quickly, heroes, destroy the demon lord before it can open a portal to its twisted demonic realm!', 14, 0, 100, 5, 0, 16044, 'Highlord Tirion Fordring to Wilfred Fizzlebang - Lord Jaraxxus Intro'),
(34996, 8, 0, 'The loss of Wilfred Fizzlebang, while unfortunate, should be a lesson to those that dare dabble in dark magic. Alas, you are victorious and must now face the next challenge.', 14, 0, 100, 0, 0, 16045, 'Highlord Tirion Fordring - Lord Jaraxxus Outro'),
(34996, 9, 0, 'Everyone calm down! Compose yourselves! There is no conspiracy at play here! The warlock acted on his own volition, outside of influences from the Alliance. The tournament must go on!', 14, 0, 100, 5, 0, 16046, 'Highlord Tirion Fordring - Lord Jaraxxus Outro'),
    -- Faction Champions
(34996, 10, 0, 'The next battle will be against the Argent Crusade''s most powerful knights! Only by defeating them will you be deemed worthy...', 14, 0, 100, 0, 0, 16047, 'Highlord Tirion Fordring - Faction Champions Intro'),
(34996, 11, 0, 'Very well. I will allow it. Fight with honor!', 14, 0, 100, 1, 0, 16048, 'Highlord Tirion Fordring - Faction Champions Intro'),
(34996, 12, 0, 'A shallow and tragic victory. We are weaker as a whole from the losses suffered today. Who but the Lich King could benefit from such foolishness? Great warriors have lost their lives. And for what? The true threat looms ahead - the Lich King awaits us all in death.', 14, 0, 100, 0, 0, 16049, 'Highlord Tirion Fordring - Faction Champions Outro'),
    -- Twin Val'kyr
(34996, 13, 0, 'Only by working together will you overcome the final challenge. From the depths of Icecrown come two of the Scourge''s most powerful lieutenants: fearsome val''kyr, winged harbingers of the Lich King!', 14, 0, 100, 0, 0, 16050, 'Highlord Tirion Fordring - Twin Val''kyr Intro'),
(34996, 14, 0, 'Let the games begin!', 14, 0, 100, 0, 0, 16037, 'Highlord Tirion Fordring - Twin Val''kyr Intro'),
    -- Anub''arak
(34996, 15, 0, 'A mighty blow has been dealt to the Lich King! You have proven yourselves as able bodied champions of the Argent Crusade. Together we will strike against Icecrown Citadel and destroy what remains of the Scourge! There is no challenge that we cannot face united!', 14, 0, 100, 5, 0, 16051, 'Highlord Tirion Fordring - Anub''arak Intro'),
(34996, 16, 0, 'Arthas! You are hopelessly outnumbered! Lay down Frostmourne and I will grant you a just death.', 14, 0, 100, 25, 0, 16052, 'Highlord Tirion Fordring'),

-- King Varian Wrynn
(34990, 0, 0, 'Your beasts will be no match for my champions, Tirion!', 14, 0, 0, 0, 0, 16069, 'King Varian Wrynn'),
(34990, 1, 0, 'The Alliance doesn''t need the help of a demon lord to deal with Horde filth! Come, pig!', 14, 0, 100, 5, 0, 16064, 'King Varian Wrynn'),
(34990, 2, 0, 'Our honor has been besmirched! They make wild claims and false accusations against us. I demand justice! Allow my champions to fight in place of your knights, Tirion. We challenge the Horde!', 14, 0, 100, 5, 0, 16066, 'King Varian Wrynn'),
(34990, 3, 0, 'Fight for the glory of the Alliance, heroes! Honor your king and your people!', 14, 0, 100, 5, 0, 16065, 'King Varian Wrynn'),
(34990, 4, 0, 'GLORY TO THE ALLIANCE!', 14, 0, 0, 0, 0, 16067, 'King Varian Wrynn'),
(34990, 5, 0, 'Not even the Lich King most powerful minions can stand against the Alliance! All hail our victors!', 14, 0, 0, 0, 0, 16068, 'King Varian Wrynn'),
(34990, 6, 0, 'Hardly a challenge.', 14, 0, 100, 274, 0, 16061, 'King Varian Wrynn - Faction Champions Kill Player'),
(34990, 6, 1, 'HAH!', 14, 0, 100, 5, 0, 16060, 'King Varian Wrynn - Faction Champions Kill Player'),
(34990, 6, 2, 'Is this the best the Horde has to offer?', 14, 0, 100, 6, 0, 16063, 'King Varian Wrynn  - Faction Champions Kill Player'),
(34990, 6, 3, 'Worthless scrub.', 14, 0, 100, 25, 0, 16062, 'King Varian Wrynn  - Faction Champions Kill Player'),

-- Garrosh Hellscream
(34995, 0, 0, 'The Horde demands justice! We challenge the Alliance. Allow us to battle in place of your knights, paladin. We will show these dogs what it means to insult the Horde!', 14, 0, 100, 1, 0, 16023, 'Garrosh Hellscream'),
(34995, 1, 0, 'I''ve seen more worthy challenges in the Ring of Blood. You waste our time, paladin.', 14, 0, 100, 1, 0, 16026, 'Garrosh Hellscream'),
(34995, 2, 0, 'Treacherous Alliance dogs! You summon a demon lord against warriors of the Horde? Your deaths will be swift!', 14, 0, 100, 5, 0, 16021, 'Garrosh Hellscream'),
(34995, 3, 0, 'That was just a taste of what the future brings. FOR THE HORDE!', 14, 0, 100, 1, 0, 16024, 'Garrosh Hellscream'),
(34995, 4, 0, 'Show them no mercy, Horde champions! LOK''TAR OGAR!', 14, 0, 0, 0, 0, 16022, 'Garrosh - Faction Champions Intro'),
(34995, 5, 0, 'Do you still question the might of the Horde, paladin? We will take on all comers!', 14, 0, 100, 1, 0, 16025, 'Garrosh Hellscream'),
(34995, 6, 0, 'Weakling!', 14, 0, 0, 0, 0, 16017, 'Garrosh Hellscream - Faction Champions Kill Player'),
(34995, 6, 1, 'Pathetic!', 14, 0, 0, 0, 0, 16018, 'Garrosh Hellscream - Faction Champions Kill Player'),
(34995, 6, 2, 'Overpowered.', 14, 0, 0, 0, 0, 16019, 'Garrosh Hellscream - Faction Champions Kill Player'),
(34995, 6, 3, 'Lok''tar!', 14, 0, 0, 0, 0, 16020, 'Garrosh Hellscream - Faction Champions Kill Player'),

-- Highlord Tirion Fordring
(36095, 0, 0, 'Champions, you''re alive! Not only have you defeated every challenge of the Trial of the Crusader, but also thwarted Arthas'' plans! Your skill and cunning will prove to be a powerful weapon against the Scourge. Well done! Allow one of the Crusade''s mages to transport you to the surface!', 14, 0, 100, 5, 0, 16053, 'Highlord Tirion Fordring'),
(36095, 1, 0, 'Let me hand you the chests as a reward, and let its contents will serve you faithfully in the campaign against Arthas in the heart of the Icecrown Citadel!', 41, 0, 0, 0, 0, 0, 'Highlord Tirion Fordring'),

-- Gormok
(34796, 0, 0, 'My slaves! Destroy the enemy!', 41, 0, 0, 0, 0, 0, 'Gormok the Impaler - Snowball'),

-- Acidmaw
(35144, 0, 0, 'Upon seeing its companion perish, %s becomes enraged!', 41, 0, 100, 0, 0, 0, 'Acidmaw to Beasts Controller - Enrage'),

-- Dreadscale
(34799, 0, 0, 'Upon seeing its companion perish, %s becomes enraged!', 41, 0, 100, 0, 0, 0, 'Dreadscale to Beasts Controller - Enrage'),

-- Icehowl
(34797, 0, 0, '%s glares at $n and lets out a bellowing roar!', 41, 0, 100, 0, 0, 0, 'Icehowl - Start'),
(34797, 1, 0, '%s crashes into the Coliseum wall and is stunned!', 41, 0, 100, 0, 0, 0, 'Icehowl - Crash'),
(34797, 2, 0, 'Trampling combatants underfoot, %s goes into a frothing rage!', 41, 0, 100, 0, 0, 0, 'Icehowl - Fail'),

-- Wilfred Fizzlebang
(35458, 0, 0, 'Thank you, Highlord. Now, challengers, I will begin the ritual of summoning. When I am done a fearsome doomguard will appear!', 14, 0, 100, 2, 0, 16268, 'Wilfred Fizzlebang - Intro'),
(35458, 1, 0, 'Prepare for oblivion!', 14, 0, 100, 0, 0, 16269, 'Wilfred Fizzlebang - Intro'),
(35458, 2, 0, 'A-HA! I''ve done it! Behold the absolute power of Wilfred Fizzlebang, master summoner! You are bound to me, demon!', 14, 0, 100, 5, 0, 16270, 'Wilfred Fizzlebang to Wilfred Fizzlebang - Intro'),
(35458, 3, 0, 'But I''m in charge here...', 14, 0, 100, 5, 0, 16271, 'Wilfred Fizzlebang to Wilfred Fizzlebang - Death'),

-- Lord Jaraxxus
(34780, 0, 0, 'Trifling gnome! Your arrogance will be your undoing!', 14, 0, 100, 397, 0, 16143, 'Lord Jaraxxus to Wilfred Fizzlebang - Intro'),
(34780, 1, 0, 'You face Jaraxxus, Eredar Lord of the Burning Legion!', 14, 0, 100, 0, 0, 16144, 'Lord Jaraxxus - Aggro'),
(34780, 2, 0, '$n has |cFFFF0000Legion Flames!|r', 41, 0, 100, 0, 0, 0, 'Lord Jaraxxus - Legion Flame'),
(34780, 3, 0, '%s creates a Nether Portal!', 41, 0, 100, 0, 0, 16150, 'Lord Jaraxxus - Summing Nether Portal'),
(34780, 4, 0, 'Come forth, sister! Your master calls!', 14, 0, 100, 0, 0, 16150, 'Lord Jaraxxus - Summoning Mistress of Pain'),
(34780, 5, 0, '$n has |cFF00FFFFIncinerate Flesh!|r Heal $g him:her;!', 41, 0, 100, 0, 0, 16149, 'Lord Jaraxxus - Incinerate Flesh'),
(34780, 6, 0, 'FLESH FROM BONE!', 14, 0, 100, 0, 0, 16149, 'Lord Jaraxxus - Incinerate Flesh'),
(34780, 7, 0, '%s creates an |cFF00FF00Infernal Volcano!|r', 41, 0, 100, 0, 0, 16151, 'Lord Jaraxxus - Summoning Infernal Volcano emote'),
(34780, 8, 0, 'IN-FER-NO!', 14, 0, 100, 0, 0, 16151, 'Lord Jaraxxus - Summoning Infernals'),
(34780, 9, 0, 'Insignificant gnat!', 14, 0, 0, 0, 0, 16145, 'Lord Jaraxxus - Killing a player'),
(34780, 9, 1, 'Banished to the Nether!', 14, 0, 0, 0, 0, 16146, 'Lord Jaraxxus - Killing a player'),
(34780, 10, 0, 'Another will take my place. Your world is doomed...', 14, 0, 100, 0, 0, 16147, 'Lord Jaraxxus - Death'),
(34780, 11, 0,'<Laughs>', 14, 0, 0, 0, 0, 16148, 'Lord Jaraxxus - Berserk'),

-- Eydis Darkban
(34496, 0, 0, 'In the name of our dark master. For the Lich King. You. Will. Die.', 14, 0, 100, 0, 0, 16272, 'Eydis Darkbane - Aggro'),
(34496, 1, 0, 'Let the light consume you!', 14, 0, 100, 0, 0, 16279, 'Eydis Darkbane to Fjola Lightbane - Light Vortex'),
(34496, 2, 0, 'Let the dark consume you!', 14, 0, 100, 0, 0, 16278, 'Eydis Darkbane to Fjola Lightbane - Dark Vortex'),
(34496, 3, 0, '%s begins to cast |cFF9932CDDark Vortex!|r Switch to |cFF9932CDDark|r Essence!', 41, 0, 100, 0, 0, 16278, 'Eydis Darkbane to Fjola Lightbane - Dark Vortex emote'),
(34496, 4, 0, '%s begins to cast  |cFFFF0000Twin''s Pact!|r', 41, 0, 100, 0, 0, 16274, 'Eydis Darkbane to Fjola Lightbane - Twin''s Pact emote'),
(34496, 5, 0, 'CHAOS!', 14, 0, 100, 0, 0, 16274, 'Eydis Darkbane to Fjola Lightbane - Twin''s Pact'),
(34496, 6, 0, 'You have been measured and found wanting.', 14, 0, 100, 0, 0, 16276, 'Eydis Darkbane - Killing a player'),
(34496, 6, 1, 'UNWORTHY!', 14, 0, 100, 0, 0, 16276, 'Eydis Darkbane - Killing a player'),
(34496, 7, 0, 'YOU ARE FINISHED!', 14, 0, 0, 0, 0, 16273, 'Eydis Darkbane - Berserk'),
(34496, 8, 0, 'The Scourge cannot be stopped...', 14, 0, 100, 0, 0, 16275, 'Eydis Darkbane - Death'),

-- Fjola Lightbane
(34497, 0, 0, 'In the name of our dark master. For the Lich King. You. Will. Die.', 14, 0, 100, 0, 0, 16272, 'Fjola Lightbane - Aggro'),
(34497, 1, 0, 'Let the light consume you!', 14, 0, 100, 0, 0, 16279, 'Fjola Lightbane to Fjola Lightbane - Light Vortex'),
(34497, 2, 0, 'Let the dark consume you!', 14, 0, 100, 0, 0, 16278, 'Fjola Lightbane to Fjola Lightbane - Dark Vortex'),
(34497, 3, 0, '%s begins to cast |cFFFFFFFFLight Vortex!|r Switch to |cFFFFFFFFLight|r Essence!', 41, 0, 100, 0, 0, 16279, 'Fjola Lightbane to Fjola Lightbane - Light Vortex emote'),
(34497, 4, 0, '%s begins to cast Twin''s Pact!', 41, 0, 100, 0, 0, 16274, 'Fjola Lightbane to Fjola Lightbane - Twin''s Pact emote'),
(34497, 5, 0, 'CHAOS!', 14, 0, 100, 0, 0, 16274, 'Fjola Lightbane to Fjola Lightbane - Twin''s Pact'),
(34497, 6, 0, 'You have been measured and found wanting.', 14, 0, 100, 0, 0, 16276, 'Fjola Lightbane - Killing a player'),
(34497, 6, 1, 'UNWORTHY!', 14, 0, 100, 0, 0, 16276, 'Fjola Lightbane - Killing a player'),
(34497, 7, 0, 'YOU ARE FINISHED!', 14, 0, 0, 0, 0, 16273, 'Fjola Lightbane - Berserk'),
(34497, 8, 0, 'The Scourge cannot be stopped...', 14, 0, 100, 0, 0, 16275, 'Fjola Lightbane - Death'),

-- The Lich King
(35877, 0, 0, 'You will have your challenge, Fordring.', 14, 0, 100, 0, 0, 16321, 'The Lich King'),
(35877, 1, 0, 'The souls of your fallen champions will be mine, Fordring.', 14, 0, 100, 0, 0, 16323, 'The Lich King'),
(35877, 2, 0, 'The Nerubians built an empire beneath the frozen wastes of Northrend. An empire that you so foolishly built your structures upon. MY EMPIRE.', 14, 0, 100, 11, 0, 16322, 'The Lich King'),

-- Anub''arak
(34564, 0, 0, 'Ahhh, our guests have arrived, just as the master promised.', 14, 0, 100, 0, 0, 16235, 'Anub''arak - Intro'),
(34564, 1, 0, 'This place will serve as your tomb!', 14, 0, 100, 0, 0, 16234, 'Anub''arak - Aggro'),
(34564, 2, 0, 'Auum na-l ak-k-k-k, isshhh. Rise, minions. Devour...', 14, 0, 100, 0, 0, 16240, 'Anub''arak - Submerge'),
(34564, 3, 0, '%s burrows into the ground!', 41, 0, 100, 0, 0, 16240, 'Anub''arak - Burrows'),
(34564, 4, 0, '%s emerges from the ground!', 41, 0, 100, 0, 0, 0, 'Anub''arak - Emerge emote'),
(34564, 5, 0, 'The swarm shall overtake you!', 14, 0, 100, 0, 0, 16241, 'Anub''arak - Leeching Swarm'),
(34564, 6, 0, '%s unleashes a Leeching Swarm to heal himself!', 41, 0, 100, 0, 0, 16241, 'Anub''arak - Leeching Swarm emote'),
(34564, 7, 0, 'F-lakkh shir!', 14, 0, 100, 0, 0, 16236, 'Anub''arak - Killing a player'),
(34564, 7, 1, 'Another soul to sate the host.', 14, 0, 100, 0, 0, 16237, 'Anub''arak - Killing a player'),
(34564, 8, 0, 'I have failed you, master...', 14, 0, 100, 0, 0, 16238, 'Anub''arak - Death'),

-- Anub''arak Spike
(34660, 0, 0, '%s''s spikes pursue $n!', 41, 0, 100, 0, 0, 0, 'Anub''arak - Spike target');

DELETE FROM `creature_text` WHERE `entry` IN (35119,34928) AND `groupid` = 6;
DELETE FROM `creature_text` WHERE `entry` = 34564 AND `groupid` = 9;
DELETE FROM `creature_text` WHERE `entry` IN (34996,34995,34990) AND `groupid` >= 20;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34564, 9, 0, '%s\'s spikes pursue $n!', 41, 0, 100, 0, 0, 0, 'Anub''arak - EMOTE_SPIKE'),
(35119, 6, 0, '%s targets $N with the Hammer of Righteous!', 41, 0, 100, 0, 0, 0, 'Eadric - SAY_EADRIC_HAMMER_WARNING'),
(34928, 6, 0, '%s begins to invoke the Waking Nightmare!', 41, 0, 100, 0, 0, 0, 'Paletress - SAY_PALETRESS_NIGHTMARE_WARNING'),
(34996, 20, 0, 'Welcome, champions. Today, before the eyes of your leaders and peers, you will prove yourselves worthy combatants.', 14, 0, 100, 1, 0, 0, 'Highlord Tirion Fordring - SAY_TIRION_GRAND_CHAMPION_INTRO_1'),
(34996, 21, 0, 'You will first be facing three of the Grand Champions of the Tournament! These fierce contenders have beaten out all others to reach the pinnacle of skill in the joust.', 14, 0, 100, 0, 0, 0, 'Highlord Tirion Fordring - SAY_TIRION_GRAND_CHAMPION_INTRO_2'),
(34996, 22, 0, 'Well fought! Your next challenge comes from the Crusade''s own ranks. You will be tested against their considerable prowess.', 14, 0, 100, 0, 0, 15882, 'Highlord Tirion Fordring - SAY_TIRION_INTRO_ARGENT_1'),
(34996, 23, 0, 'You may begin!', 14, 0, 100, 22, 0, 8574, 'Highlord Tirion Fordring - SAY_TIRION_INTRO_ARGENT_2'),
(34996, 24, 0, 'Well done. You have proven yourself today-', 14, 0, 100, 0, 0, 0, 'Highlord Tirion Fordring - SAY_TIRION_INTRO_BK_1'),
(34996, 25, 0, 'What is the meaning of this?', 14, 0, 100, 0, 0, 0, 'Highlord Tirion Fordring - SAY_TIRION_INTRO_BK_2'),
(34995, 20, 0, 'Finally, a fight worth watching.', 12, 0, 100, 396, 0, 0, 'Garrosh Hellscream - SAY_GARROSH_INTRO_CHAMPS_H'),
(34995, 21, 0, 'Admirably? Hah! I will enjoy watching your weak little champions fail, human.', 14, 0, 100, 22, 0, 0, 'Garrosh Hellscream - SAY_GARROSH_INTRO_CHAMPS_A'),
(34995, 22, 0, 'Tear him apart!', 14, 0, 100, 22, 0, 0, 'Garrosh Hellscream - SAY_GARROSH_INTRO_BK'),
(34990, 20, 0, 'I did not come here to watch animals tear at each other senselessly, Tirion.', 12, 0, 100, 1, 0, 0, 'King Varian Wrynn - SAY_VARIAN_INTRO_CHAMPS_H'),
(34990, 21, 0, 'I have no taste for these games, Tirion. Still... I trust they will perform admirably.', 12, 0, 100, 1, 0, 0, 'King Varian Wrynn - SAY_VARIAN_INTRO_CHAMPS_A'),
(34990, 22, 0, 'Don''t just stand there; kill him!', 14, 0, 100, 22, 0, 0, 'SAY_VARIAN_INTRO_BK');


DELETE FROM `creature_template_addon` WHERE `entry` = 36095;
INSERT INTO `creature_template_addon` (`entry`, `mount`, `bytes1`, `bytes2`, `auras`) VALUES
(36095,0,0x0,0x1,'57545');

DELETE FROM `spell_script_names` WHERE `spell_id` IN (66093, 67957, 67958, 67959);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66093, 'spell_faction_champion_dru_lifebloom'),
(67957, 'spell_faction_champion_dru_lifebloom'),
(67958, 'spell_faction_champion_dru_lifebloom'),
(67959, 'spell_faction_champion_dru_lifebloom');

UPDATE creature_template SET ScriptName = 'mob_snobold_vassal' WHERE entry = 34800;

UPDATE `creature_template_addon` SET `auras` = 66132 WHERE `entry` IN (34496,35347,35348,35349);
UPDATE `creature_template_addon` SET `auras` = 66133 WHERE `entry` IN (34497,35350,35351,35352);
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 34854;
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 34784;
UPDATE `creature_template` SET `mechanic_immune_mask` = 0 WHERE `entry` IN ('34465','35746','35747','35748','34469','35714','35715','35716','34466','35665','35666','35667','34463','35734','35735','35736','34475','35737','35738','35739','34461','35743','35744','35745','34472','35699','35700','35701','34467','35662','35663','35664','34473','35674','35675','35676','34474','35731','35732','35733','35610','35774','35775','35776','35465','36301','36302','36303','34460','35702','35703','35704','34468','35721','35722','35723','34445','35705','35706','35707','34447','35683','35684','35685','34455','35680','35681','35682','34456','35708','35709','35710','34458','35692','35693','35694','34471','35668','35669','35670','34451','35671','35672','35673','34449','35689','35690','35691','34454','35711','35712','35713','34444','35740','35741','35742','34459','35686','35687','35688','34453','35718','35719','35720','34448','35724','35725','35726','34441','34442','34443','35749','34470','35728','35729','35730','34450','35695','35696','35697');
UPDATE `gameobject_template` SET `faction` = 1375 WHERE `entry` = 195485;
UPDATE `creature_template` SET `unit_flags` = 33587200 WHERE `entry` IN (34660,35652,35653,35654);
DELETE FROM `creature` WHERE `id` = 34743;
UPDATE `creature_template` SET `unit_flags` = 33554688, `flags_extra` = 0 WHERE `entry` = 35651;


-- Trial of the Crusader scriptnames
DELETE FROM `spell_script_names` WHERE `spell_id` IN (66017, 68753, 68754, 68755, 65983, 65980);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(66017, 'spell_faction_champion_death_grip'),
(68753, 'spell_faction_champion_death_grip'),
(68754, 'spell_faction_champion_death_grip'),
(68755, 'spell_faction_champion_death_grip'),
(65983, 'spell_toc_heroism'),
(65980, 'spell_toc_bloodlust');

UPDATE `creature_template` SET `mechanic_immune_mask` = 650854271 WHERE `entry` IN (34813,35265,35266,35267,34825,35278,35279,35280);

-- DELETE FROM `creature` WHERE `guid`=399968 AND `id`=34816 AND `map`=649;

DELETE FROM `creature` WHERE `map`=649 AND `id` IN 
(16980, 22517, 23033, 30298, 35035, 35226, 35458, 35651, 35766, 35770, 35771, 35909, 36097, 51339);

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN 
(34800, 35441, 35442, 35443, 34854, 34564, 34566, 35615, 35616,
34605, 34650, 35658, 35659, 34607, 34648, 35655, 35656);

UPDATE `creature_template` SET `unit_flags`=256 WHERE `entry`=34862;