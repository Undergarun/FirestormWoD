DELETE FROM `achievement_criteria_data` WHERE `criteria_id` IN (12307,12308,11559,11560,11561,12309) AND `TYPE` = 14;

DELETE FROM `spell_dbc` WHERE `Id` = 68575;
INSERT INTO `spell_dbc` (Id, Dispel, Mechanic, Attributes, AttributesEx, AttributesEx2, AttributesEx3, AttributesEx4, AttributesEx5, AttributesEx6, AttributesEx7, Stances, StancesNot, Targets, CastingTimeIndex, AuraInterruptFlags, ProcFlags, ProcChance, ProcCharges, MaxLevel, BaseLevel, SpellLevel, DurationIndex, RangeIndex, StackAmount, EquippedItemClass, EquippedItemSubClassMask, EquippedItemInventoryTypeMask, Effect1, Effect2, Effect3, EffectDieSides1, EffectDieSides2, EffectDieSides3, EffectRealPointsPerLevel1, EffectRealPointsPerLevel2, EffectRealPointsPerLevel3, EffectBasePoints1, EffectBasePoints2, EffectBasePoints3, EffectMechanic1, EffectMechanic2, EffectMechanic3, EffectImplicitTargetA1, EffectImplicitTargetA2, EffectImplicitTargetA3, EffectImplicitTargetB1, EffectImplicitTargetB2, EffectImplicitTargetB3, EffectRadiusIndex1, EffectRadiusIndex2, EffectRadiusIndex3, EffectApplyAuraName1, EffectApplyAuraName2, EffectApplyAuraName3, EffectAmplitude1, EffectAmplitude2, EffectAmplitude3, EffectMultipleValue1, EffectMultipleValue2, EffectMultipleValue3, EffectMiscValue1, EffectMiscValue2, EffectMiscValue3, EffectMiscValueB1, EffectMiscValueB2, EffectMiscValueB3, EffectTriggerSpell1, EffectTriggerSpell2, EffectTriggerSpell3, EffectSpellClassMaskA1, EffectSpellClassMaskA2, EffectSpellClassMaskA3, EffectSpellClassMaskB1, EffectSpellClassMaskB2, EffectSpellClassMaskB3, EffectSpellClassMaskC1, EffectSpellClassMaskC2, EffectSpellClassMaskC3, MaxTargetLevel, SpellFamilyName, SpellFamilyFlags1, SpellFamilyFlags2, SpellFamilyFlags3, MaxAffectedTargets, DmgClass, PreventionType, DmgMultiplier1, DmgMultiplier2, DmgMultiplier3, AreaGroupId, SchoolMask, COMMENT) VALUES
(68575, 0, 0, 536870912, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 101, 0, 0, 0, 0, 0, 1, 0, -1, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 18, 0, 0, 16, 0, 0, 28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, "Argent Champion credit marker - Eadric The Pure");

UPDATE `creature_template` SET Health_mod = 16 WHERE `entry` = 35451;
UPDATE `creature_template` SET Health_mod = 22 WHERE `entry` = 35490;
UPDATE `creature_template` SET Health_mod = 10 WHERE `entry` IN (35306,35308,35310);

UPDATE `creature` SET `phasemask`=1 WHERE `map`=650;

DELETE FROM `spell_script_names` WHERE `spell_id` IN (66515, 66867, 67534, 67830);
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES 
(66515, 'spell_paletress_shield'),
(66867, 'spell_eadric_hammer_of_righteous'),
(67534, 'spell_toc5_hex_mending'),
(67830, 'spell_toc5_ride_mount');

UPDATE `creature_template` SET `ScriptName`='npc_argent_lightwielder' WHERE `entry`=35309;
UPDATE `creature_template` SET `ScriptName`='npc_argent_monk' WHERE `entry`=35305;
UPDATE `creature_template` SET `ScriptName`='npc_argent_priest' WHERE `entry`=35307;

UPDATE `creature_template` SET `unit_flags`=0, `dynamicflags`=8 WHERE `entry` IN
(35572, 35569, 35571, 35570,
35617, 34705, 34702, 34701, 
34657, 34703, 35328, 35329, 
35331, 35332, 35330, 35314, 
35325, 35327, 35323, 35326,
36086, 36083, 36082, 36087, 
36088, 36085, 36091, 36090, 
36089, 36084);

UPDATE `creature_template` SET `unit_flags`=2 WHERE `entry` IN
(34996, 34994, 34995, 34990, 34992);

UPDATE `creature_template` SET `unit_flags`=64 WHERE `entry` IN (35451, 35490);

UPDATE `creature_template` SET `unit_flags`=0 WHERE `entry` IN
(35717, 35568, 35546, 35310, 
35308, 35306, 35518, 35519, 
35544, 35532, 35529, 35539, 
35524, 35522, 35534, 35525, 
35523, 35533, 35520, 35542, 
35527, 35537, 35535, 35543, 
35528, 35521, 35540, 35536, 
35530, 35538, 35517, 35531, 
35541, 35119, 34928, 34942, 
35029, 35030, 35031, 35032, 
35033, 35034, 35036, 35037, 
35038, 35039, 35040, 35041, 
35042, 35043, 35044, 35045, 
35046, 35047, 35048, 35049, 
35050, 35051, 35052, 35028, 
35309, 35305, 35307, 35451, 
35545, 35564, 35590);

UPDATE `creature_template` SET `dynamicflags`=8 WHERE `entry` IN
(35717, 35568, 35546, 35310, 
35308, 35306, 35518, 35519, 
35544, 35532, 35529, 35539, 
35524, 35522, 35534, 35525, 
35523, 35533, 35520, 35542, 
35527, 35537, 35535, 35543, 
35528, 35521, 35540, 35536, 
35530, 35538, 35517, 35531, 
35541, 35119, 34928, 34942, 
35029, 35030, 35031, 35032, 
35033, 35034, 35036, 35037, 
35038, 35039, 35040, 35041, 
35042, 35043, 35044, 35045, 
35046, 35047, 35048, 35049, 
35050, 35051, 35052, 35028, 
35309, 35305, 35307, 35451, 
35545, 35564, 35590, 35451,
35490,  34995, 34990, 34992,
35004, 35005, 34996, 34994);

UPDATE `creature_template` SET `ScriptName`='generic_vehicleAI_toc5' WHERE `entry` IN 
(33217, 33316, 33317, 33318,
33319, 33320, 33321, 33322,
33323, 33324, 35640, 35325,
35638, 35314, 35634, 35327,
35641, 35323, 35635, 35326,
35637, 35328, 35330, 35768,
35636, 35329, 34658, 35332,
35633, 35331);

DELETE FROM `creature` WHERE `map`=650 AND `id` IN 
(34887, 34900, 34906, 34908, 34909, 34910, 35005, 36557, 36559);

REPLACE INTO `creature` ( `id`, `map`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES 
(35491, 650, 3, 1, 0, 0, 799.806, 639.236, 475.88, 2.18738, 86400, 0, 0, 189000, 0, 0, 0, 0, 0);

UPDATE `gameobject_template` SET `flags`=0 WHERE `entry` IN (195709, 195710, 195374, 195375, 195323, 195324);

DELETE FROM `creature_template_addon` WHERE `entry` IN 
(35572, 35569, 35571, 35570, 35617, 34705, 34702, 34701, 34657, 34703, 35328, 35329, 35331, 35332, 35330, 35314, 
35325, 35327, 35323, 35326, 36086, 36083, 36082, 36087, 36088, 36085, 36091, 36090, 36089, 36084, 34996, 34994, 
34995, 34990, 34992, 35451, 35490, 35717, 35568, 35546, 35544, 35532, 35529, 35539, 35524, 35522, 35534, 35525, 
35523, 35533, 35520, 35542, 35527, 35537, 35535, 35543, 35528, 35521, 35540, 35536, 35530, 35538, 35517, 35531, 
35541, 35119, 34928, 34942, 35029, 35030, 35031, 35032, 35033, 35034, 35036, 35037, 35038, 35039, 35040, 35041, 
35042, 35043, 35044, 35045, 35046, 35047, 35048, 35049, 35050, 35051, 35052, 35028, 35309, 35305, 35307, 35451, 
35545, 35564, 35590, 35717, 35568, 35546, 35310, 35308, 35544, 35532, 35529, 35539, 35524, 35522, 35534, 35525, 
35523, 35533, 35520, 35542, 35527, 35537, 35535, 35543, 35528, 35521, 35540, 35536, 35530, 35538, 35517, 35531, 
35541, 35119, 34928, 34942, 35029, 35030, 35031, 35032, 35033, 35034, 35036, 35037, 35038, 35039, 35040, 35041, 
35042, 35043, 35044, 35045, 35046, 35047, 35048, 35049, 35050, 35051, 35052, 35028, 35309, 35305, 35307, 35451, 
35545, 35564, 35590, 35451,35490,  34995, 34990, 34992, 35004, 35005, 34996, 34994, 33217, 33316, 33317, 33318,
33319, 33320, 33321, 33322, 33323, 33324, 35640, 35325, 35638, 35314, 35634, 35327, 35641, 35323, 35635, 35326,
35637, 35328, 35330, 35768, 35636, 35329, 34658, 35332, 35633, 35331, 34887, 34900, 34906, 34908, 34909, 34910, 
35306, 35518, 35519, 35005, 36557, 36559,  35518, 35519, 35310, 35308, 35306);

REPLACE INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES 
(33217, 0, 0, 0, 2305, 0, NULL),
(33316, 0, 0, 0, 2305, 0, NULL),
(33317, 0, 0, 0, 2305, 0, NULL),
(33318, 0, 0, 0, 2305, 0, NULL),
(33319, 0, 0, 0, 2305, 0, NULL),
(33320, 0, 0, 0, 2305, 0, NULL),
(33321, 0, 0, 0, 2305, 0, NULL),
(33322, 0, 0, 0, 2305, 0, NULL),
(33323, 0, 0, 0, 2305, 0, NULL),
(33324, 0, 0, 0, 2305, 0, NULL),
(34657, 0, 0, 0, 0, 0, '63406 18950'),
(34701, 0, 0, 0, 0, 0, '63423 18950'),
(34702, 0, 0, 0, 0, 0, '63396 18950'),
(34703, 0, 0, 0, 0, 0, '63427 18950'),
(34705, 0, 0, 0, 0, 0, '62594 18950'),
(34887, 0, 0, 0, 0, 0, '63606'),
(34900, 0, 0, 0, 0, 0, '62595'),
(34906, 0, 0, 0, 0, 0, '63425'),
(34908, 0, 0, 0, 0, 0, '63421'),
(34909, 0, 0, 0, 0, 0, '63404'),
(34910, 0, 0, 0, 0, 0, '63394'),
(34928, 0, 0, 0, 0, 0, '63501 18950'),
(35119, 0, 0, 0, 0, 0, '63501 18950 66865'),
(35305, 0, 0, 0, 1, 27, '63501'),
(35306, 0, 0, 0, 1, 27, '63501'),
(35307, 0, 0, 0, 1, 375, '63501'),
(35308, 0, 0, 0, 1, 375, '63501'),
(35309, 0, 0, 0, 1, 375, '63501'),
(35310, 0, 0, 0, 1, 375, '63501'),
(35314, 0, 14335, 0, 0, 0, '63432'),
(35323, 0, 14343, 0, 1, 0, '64723'),
(35325, 0, 28556, 0, 0, 0, '63435'),
(35326, 0, 28607, 0, 0, 0, '63402'),
(35327, 0, 28605, 0, 0, 0, '63429'),
(35328, 0, 29043, 0, 0, 0, '62596'),
(35329, 0, 28612, 0, 0, 0, '63426'),
(35330, 0, 28606, 0, 0, 0, '63422'),
(35331, 0, 28571, 0, 0, 0, '63395'),
(35332, 0, 14333, 0, 0, 0, '63405'),
(35451, 0, 0, 0, 1, 0, NULL),
(35490, 0, 0, 0, 1, 0, NULL),
(35517, 0, 0, 0, 0, 0, '63501 18950'),
(35518, 0, 0, 0, 0, 0, '63501 18950 66865'),
(35570, 0, 0, 0, 0, 0, '63399 18950'),
(35571, 0, 0, 0, 0, 0, '63436 18950'),
(35572, 0, 0, 0, 0, 0, '63433 18950'),
(35617, 0, 0, 0, 0, 0, '63430 18950'),
(36082, 0, 0, 0, 0, 0, '63396 18950'),
(36083, 0, 0, 0, 0, 0, '63423 18950'),
(36084, 0, 0, 0, 0, 0, '63430 18950'),
(36086, 0, 0, 0, 0, 0, '63406 18950'),
(36087, 0, 0, 0, 0, 0, '63427 18950'),
(36088, 0, 0, 0, 0, 0, '62594 18950'),
(36089, 0, 0, 0, 0, 0, '63433 18950'),
(36090, 0, 0, 0, 0, 0, '63436 18950'),
(36091, 0, 0, 0, 0, 0, '63399 18950');

REPLACE INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
('34928','1','0','Well then, let us begin.','14','0','100','0','0','16247','Paletress - Combat - Aggro'),
('34928','2','0','Take this time to consider your past deeds.','14','0','100','0','0','16248','Paletress - Combat - Summon Memory'),
('34928','3','0','Take your rest.','14','0','100','0','0','16250','Paletress - Combat - Slay 1'),
('34928','3','1','Be at ease.','14','0','100','0','0','16251','Paletress - Combat - Slay 2'),
('34928','4','0','Excellent work!','14','0','100','0','0','16252','Paletress - Combat - Death'),
('34928','5','0','Even the darkest memory fades when confronted.','14','0','100','0','0','16249','Paletress - Combat - Memory dies'),
('34928','10','0','Thank you, good herald. Your words are too kind.','12','0','100','1','0','16245','Paletress - Intro'),
('34928','11','0','May the Light give me strength to provide a worthy challenge.','12','0','100','1','0','16246','Paletress - Intro'),
('34990','0','0','Your beasts will be no match for my champions, Tirion!','14','0','0','0','0','16069','King Varian Wrynn'),
('34990','1','0','The Alliance doesn\'t need the help of a demon lord to deal with Horde filth! Come, pig!','14','0','100','5','0','16064','King Varian Wrynn'),
('34990','2','0','Our honor has been besmirched! They make wild claims and false accusations against us. I demand justice! Allow my champions to fight in place of your knights, Tirion. We challenge the Horde!','14','0','100','5','0','16066','King Varian Wrynn'),
('34990','3','0','Fight for the glory of the Alliance, heroes! Honor your king and your people!','14','0','100','5','0','16065','King Varian Wrynn'),
('34990','4','0','GLORY TO THE ALLIANCE!','14','0','0','0','0','16067','King Varian Wrynn'),
('34990','5','0','Not even the Lich King most powerful minions can stand against the Alliance! All hail our victors!','14','0','0','0','0','16068','King Varian Wrynn'),
('34990','6','0','Hardly a challenge.','14','0','100','274','0','16061','King Varian Wrynn - Faction Champions Kill Player'),
('34990','6','1','HAH!','14','0','100','5','0','16060','King Varian Wrynn - Faction Champions Kill Player'),
('34990','6','2','Is this the best the Horde has to offer?','14','0','100','6','0','16063','King Varian Wrynn  - Faction Champions Kill Player'),
('34990','6','3','Worthless scrub.','14','0','100','25','0','16062','King Varian Wrynn  - Faction Champions Kill Player'),
('34992','0','0','They are worthy fighters, I see.','12','0','100','1','0','0','Jaina - Intro'),
('34992','1','0','Of course they will.','12','0','100','1','0','0','Garrosh - Intro'),
('34994','0','0','Fight well, Horde! Lok tar Ogar!','14','0','100','1','0','0','Thrall - Intro'),
('34994','1','0','Garrosh is enough.','12','0','100','1','0','0','Jaina - Intro'),
('34995','0','0','The Horde demands justice! We challenge the Alliance. Allow us to battle in place of your knights, paladin. We will show these dogs what it means to insult the Horde!','14','0','100','1','0','16023','Garrosh Hellscream'),
('34995','1','0','I\'ve seen more worthy challenges in the Ring of Blood. You waste our time, paladin.','14','0','100','1','0','16026','Garrosh Hellscream'),
('34995','2','0','Treacherous Alliance dogs! You summon a demon lord against warriors of the Horde? Your deaths will be swift!','14','0','100','5','0','16021','Garrosh Hellscream'),
('34995','3','0','That was just a taste of what the future brings. FOR THE HORDE!','14','0','100','1','0','16024','Garrosh Hellscream'),
('34995','4','0','Show them no mercy, Horde champions! LOK\'TAR OGAR!','14','0','0','0','0','16022','Garrosh - Faction Champions Intro'),
('34995','5','0','Do you still question the might of the Horde, paladin? We will take on all comers!','14','0','100','1','0','16025','Garrosh Hellscream'),
('34995','6','0','Weakling!','14','0','0','0','0','16017','Garrosh Hellscream - Faction Champions Kill Player'),
('34995','6','1','Pathetic!','14','0','0','0','0','16018','Garrosh Hellscream - Faction Champions Kill Player'),
('34995','6','2','Overpowered.','14','0','0','0','0','16019','Garrosh Hellscream - Faction Champions Kill Player'),
('34995','6','3','Lok\'tar!','14','0','0','0','0','16020','Garrosh Hellscream - Faction Champions Kill Player'),
('34996','0','0','Welcome, champions! You have heard the call of the Argent Crusade and you have boldly answered! It is here, in the Crusaders\' Coliseum, that you will face your greatest challenges. Those of you who survive the rigors of the coliseum will join the Argent Crusade on its march to Icecrown Citadel.','14','0','100','0','0','16036','Highlord Tirion Fordring - Welcome'),
('34996','1','0','Hailing from the deepest, darkest caverns of the Storm Peaks, Gormok the Impaler! Battle on, heroes!','14','0','100','0','0','16038','Highlord Tirion Fordring - Summing Gormok the Impaler'),
('34996','2','0','Steel yourselves, heroes, for the twin terrors, Acidmaw and Dreadscale, enter the arena!','14','0','100','0','0','16039','Highlord Tirion Fordring - Summing Acidmaw and Dreadscale'),
('34996','3','0','The air itself freezes with the introduction of our next combatant, Icehowl! Kill or be killed, champions!','14','0','100','0','0','16040','Highlord Tirion Fordring Summing Icehowl'),
('34996','4','0','The monstrous menagerie has been vanquished!','14','0','100','0','0','16041','Highlord Tirion Fordring - Northrend Beasts Done'),
('34996','5','0','Tragic... They fought valiantly, but the beasts of Northrend triumphed. Let us observe a moment of silence for our fallen heroes.','14','0','0','0','0','16042','Highlord Tirion Fordring - Northrend Beasts FAIL'),
('34996','6','0','Grand Warlock Wilfred Fizzlebang will summon forth your next challenge. Stand by for his entry.','14','0','100','0','0','16043','Highlord Tirion Fordring - Summing Wilfred Fizzlebang'),
('34996','7','0','Quickly, heroes, destroy the demon lord before it can open a portal to its twisted demonic realm!','14','0','100','5','0','16044','Highlord Tirion Fordring to Wilfred Fizzlebang - Lord Jaraxxus Intro'),
('34996','8','0','The loss of Wilfred Fizzlebang, while unfortunate, should be a lesson to those that dare dabble in dark magic. Alas, you are victorious and must now face the next challenge.','14','0','100','0','0','16045','Highlord Tirion Fordring - Lord Jaraxxus Outro'),
('34996','9','0','Everyone calm down! Compose yourselves! There is no conspiracy at play here! The warlock acted on his own volition, outside of influences from the Alliance. The tournament must go on!','14','0','100','5','0','16046','Highlord Tirion Fordring - Lord Jaraxxus Outro'),
('34996','10','0','The next battle will be against the Argent Crusade\'s most powerful knights! Only by defeating them will you be deemed worthy...','14','0','100','0','0','16047','Highlord Tirion Fordring - Faction Champions Intro'),
('34996','11','0','Very well. I will allow it. Fight with honor!','14','0','100','1','0','16048','Highlord Tirion Fordring - Faction Champions Intro'),
('34996','12','0','A shallow and tragic victory. We are weaker as a whole from the losses suffered today. Who but the Lich King could benefit from such foolishness? Great warriors have lost their lives. And for what? The true threat looms ahead - the Lich King awaits us all in death.','14','0','100','0','0','16049','Highlord Tirion Fordring - Faction Champions Outro'),
('34996','13','0','Only by working together will you overcome the final challenge. From the depths of Icecrown come two of the Scourge\'s most powerful lieutenants: fearsome val\'kyr, winged harbingers of the Lich King!','14','0','100','0','0','16050','Highlord Tirion Fordring - Twin Val\'kyr Intro'),
('34996','14','0','Let the games begin!','14','0','100','0','0','16037','Highlord Tirion Fordring - Twin Val\'kyr Intro'),
('34996','15','0','A mighty blow has been dealt to the Lich King! You have proven yourselves as able bodied champions of the Argent Crusade. Together we will strike against Icecrown Citadel and destroy what remains of the Scourge! There is no challenge that we cannot face united!','14','0','100','5','0','16051','Highlord Tirion Fordring - Anub\'arak Intro'),
('34996','16','0','Arthas! You are hopelessly outnumbered! Lay down Frostmourne and I will grant you a just death.','14','0','100','25','0','16052','Highlord Tirion Fordring'),
('35004','0','0','The Sunreavers are proud to present to their representatives in these tests of combat.','14','0','100','1','0','0','Announcer - Player Horde Champions - Intro'),
('35004','1','0','Here comes the small but deadly Ambrose Boltspark, Grand Champion of Gnomeregan.','14','0','100','1','0','0','Announcer - Alliance Champions - Intro Mage'),
('35004','2','0','Colosos, the huge Grand Champion of the Exodar, is out the door.','14','0','100','1','0','0','Announcer - Alliance Champions - Intro Shaman'),
('35004','3','0','Entering the arena is the Grand Champion of Darnassus, the skilled sentinel Jaelyne Evensong.','14','0','100','1','0','0','Announcer - Alliance Champions - Intro Hunter'),
('35004','4','0','Proud and strong, give a cheer for Marshal Jacob Alerius, the Grand Champion of Stormwind!','14','0','100','1','0','0','Announcer - Alliance Champions - Intro Warrior'),
('35004','5','0','Today the power of the dwarfs is represented by the Grand Champion of Ironforge, Lana Stouthammer.','14','0','100','1','0','0','Announcer - Alliance Champions - Intro Rouge'),
('35004','11','0','Entering the arena, a paladin who is no stranger to the battlefield or tournament ground, the Grand Champion of the Argent Crusade. Eadric the Pure!','14','0','100','1','0','0','Announcer - Eadric - Intro'),
('35004','12','0','The next combatant is second to none is her passion for upholding the Light. I give you Argent Confessor Paletress!','14','0','100','1','0','0','Announcer - Paletress - Intro'),
('35004','13','0','What s that. up near the rafters?','12','0','100','1','0','0','Announcer - Black Knight - Intro'),
('35005','0','0','Silver Covenant is delighted to present their fighters for this event, High Lord.','14','0','100','1','0','0','Announcer - Player Alliance Champions - Intro'),
('35005','1','0','Coming out of the gate is Eressea Dawnsinger, skilled mage and Grand Champion of Silvermoon!','14','0','100','1','0','0','Announcer - Horde Champions - Intro Mage'),
('35005','2','0','Tall in the saddle of his kodo, here is the venerable Runok Wildmane, Grand Champion of Thunder Bluff!','14','0','100','1','0','0','Announcer - Horde Champions - Intro Shaman'),
('35005','3','0','Entering the dry sand but dangerous Zul tore, Grand Champion of Sen jin!','14','0','100','1','0','0','Announcer - Horde Champions - Intro Hunter'),
('35005','4','0','We presented the Grand Champion of Orgrimmar, the Mokra the Skullcrusher! ','14','0','100','1','0','0','Announcer - Horde Champions - Intro Warrior'),
('35005','5','0','Representing the tenacity of the Forsaken, here is the Grand Champion of the Undercity, Deathstalker Visceri!','14','0','100','1','0','0','Announcer - Horde Champions - Intro Rouge'),
('35005','11','0','Entering the arena, a paladin who is no stranger to the battlefield or tournament ground, the Grand Champion of the Argent Crusade. Eadric the Pure!','14','0','100','1','0','0','Announcer - Eadric - Intro'),
('35005','12','0','The next combatant is second to none is her passion for upholding the Light. I give you Argent Confessor Paletress!','14','0','100','1','0','0','Announcer - Paletress - Intro'),
('35005','13','0','What s that. up near the rafters?','12','0','100','1','0','0','Announcer - Black Knight - Intro'),
('35119','1','0','Prepare yourselves!','14','0','100','0','0','16135','Eadric - Combat - Aggro'),
('35119','2','0','Hammer of the Righteous!','14','0','100','0','0','16136','Eadric - Combat - Hammer'),
('35119','3','0','You... You need more practice!','14','0','100','0','0','16137','Eadric - Combat - Slay 1'),
('35119','3','1','Nay! Nay! And I say yet again nay! Not good enough!','14','0','100','0','0','16138','Eadric - Combat - Slay 2'),
('35119','4','0','I yield! I submit. Excellent work. May I run away now?','14','0','100','0','0','16139','Eadric - Combat - Death'),
('35119','5','0','%s begins to radiate light. Shield your eyes!','41','0','100','0','0','0','Eadric - Combat - Warning - Radiance'),
('35119','10','0','Are you up to the challenge? I will not hold back.','12','0','100','1','0','16134','Eadric - Intro');

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
