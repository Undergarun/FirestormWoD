UPDATE `creature_template` SET difficulty_entry_4 = 367966 WHERE entry = 67966;
DELETE FROM `creature_template` WHERE `entry` = 367966;
INSERT INTO `creature_template` VALUES
('367966','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47499','0','0','0','Whirl Turtle (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','0','0','0','0','1','0','0','1','0','0','0','0','0','0','0','0','0','0','0','1','2097256','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','24.075','1','1','1','0','0','0','0','0','0','0','164','1','0','0','0','','17614');
UPDATE `creature_template` SET difficulty_entry_4 = 367977, flags_extra = 0x01 WHERE entry = 67977;
DELETE FROM `creature_template` WHERE `entry` = 367977;
INSERT INTO `creature_template` VALUES
('367977','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46559','0','0','0','Tortos (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','15000','30000','0','0','10','2000','0','4','0','0','0','0','0','0','0','0','0','0','0','1','2097260','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2653','2800000','2900000','','0','3','1','1100.568','1','1','1','0','0','0','0','0','0','0','0','1','0','617299839','1','','17614');
UPDATE `creature_template` SET difficulty_entry_4 = 368497 WHERE entry = 68497;
DELETE FROM `creature_template` WHERE `entry` = 368497;
INSERT INTO `creature_template` VALUES
('368497','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46824','0','0','0','Vampiric Cave Bat (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','1','72','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','4','1','11','1','1','1','0','0','0','0','0','0','0','133','1','0','0','0','','17614');
UPDATE `creature_template` SET difficulty_entry_4 = 369351 WHERE entry = 69351;
DELETE FROM `creature_template` WHERE `entry` = 369351;
INSERT INTO `creature_template` VALUES
('369351','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','48248','0','0','0','Greater Cave Bat (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','1','72','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','4','1','72','1','1','1','0','0','0','0','0','0','0','130','1','0','0','0','','17614');
UPDATE `creature_template` SET difficulty_entry_4 = 369352 WHERE entry = 69352;
DELETE FROM `creature_template` WHERE `entry` = 369352;
INSERT INTO `creature_template` VALUES
('369352','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','46824','0','0','0','Vampiric Cave Bat (4)','','','0','92','92','4','0','14','14','0','0','1','1.14286','1.14286','1','1','15000','30000','0','0','7','2000','0','1','0','0','0','0','0','0','0','0','0','0','0','1','72','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','4','1','24','1','1','1','0','0','0','0','0','0','0','130','1','0','0','0','','17614');

-- Normal mode
SET @REF_TORTOS = 67977;
SET @REF_TORTOS_HM = 367977;
DELETE FROM `reference_loot_template` WHERE entry = @REF_TORTOS OR `entry` = @REF_TORTOS_HM;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TORTOS, 94768, 0, 1, 1, 1, 1) /* Shellsplitter Greataxe */, 
(@REF_TORTOS, 94769, 0, 1, 1, 1, 1) /* Shattered Tortoiseshell Longbow */, 
(@REF_TORTOS, 94770, 0, 1, 1, 1, 1) /* Rockfall Ribwraps */, 
(@REF_TORTOS, 94771, 0, 1, 1, 1, 1) /* Shell-Coated Wristplates */, 
(@REF_TORTOS, 94772, 0, 1, 1, 1, 1) /* Quakestompers */, 
(@REF_TORTOS, 94773, 0, 1, 1, 1, 1) /* Shoulderguards of Centripetal Destruction */, 
(@REF_TORTOS, 94774, 0, 1, 1, 1, 1) /* Beakbreaker Greatcloak */, 
(@REF_TORTOS, 94775, 0, 1, 1, 1, 1) /* Beady-Eye Bracers */, 
(@REF_TORTOS, 94776, 0, 1, 1, 1, 1) /* Amulet of the Primal Turtle */, 
(@REF_TORTOS, 94777, 0, 1, 1, 1, 1) /* Grips of Vampiric Cruelty */, 
(@REF_TORTOS, 94778, 0, 1, 1, 1, 1) /* Tortos' Discarded Shell */, 
(@REF_TORTOS, 94779, 0, 1, 1, 1, 1) /* Robes of Concussive Shocks */, 
(@REF_TORTOS, 94780, 0, 1, 1, 1, 1) /* Crystal-Claw Gloves */, 
(@REF_TORTOS, 94781, 0, 1, 1, 1, 1) /* Azure Shell Bracers */, 
(@REF_TORTOS, 94782, 0, 1, 1, 1, 1) /* Stonegaze Hood */, 
(@REF_TORTOS, 94783, 0, 1, 1, 1, 1) /* Spaulders of Quaking Fear */, 
(@REF_TORTOS, 94784, 0, 1, 1, 1, 1) /* Refreshing Abalone Girdle */, 
(@REF_TORTOS, 94785, 0, 1, 1, 1, 1) /* Shimmershell Cape */, 
(@REF_TORTOS, 94786, 0, 1, 1, 1, 1) /* Vampire Bat-Hide Bracers */, 
(@REF_TORTOS, 94787, 0, 1, 1, 1, 1) /* Tortos' Shellseizers */, 
(@REF_TORTOS, 95060, 0, 1, 1, 1, 1) /* Abandoned Spaulders of Arrowflight */, 
(@REF_TORTOS, 95061, 0, 1, 1, 1, 1) /* Lost Shoulders of Fire */, 
(@REF_TORTOS, 95062, 0, 1, 1, 1, 1) /* Forgotten Mantle of the Sun */, 
(@REF_TORTOS, 95063, 0, 1, 1, 1, 1) /* Reconstructed Furious Shoulderplates */, 
(@REF_TORTOS, 95064, 0, 1, 1, 1, 1) /* Abandoned Spaulders of Renewal */, 
(@REF_TORTOS, 95065, 0, 1, 1, 1, 1) /* Forgotten Mantle of the Moon */, 
(@REF_TORTOS, 95066, 0, 1, 1, 1, 1) /* Lost Shoulders of Healing */, 
(@REF_TORTOS, 95067, 0, 1, 1, 1, 1) /* Lost Shoulders of Fluidity */, 
(@REF_TORTOS, 95068, 0, 1, 1, 1, 1) /* Reconstructed Bloody Shoulderplates */, 
(@REF_TORTOS, 95069, 0, 1, 1, 1, 1) /* Reconstructed Holy Shoulderplates */, 
(@REF_TORTOS, 95498, 0, 1, 1, 1, 1) /* Bo-Ris, Horror in the Night */, 
(@REF_TORTOS, 95499, 0, 1, 1, 1, 1) /* Invocation of the Dawn */, 
(@REF_TORTOS, 95500, 0, 1, 1, 1, 1) /* Jerthud, Graceful Hand of the Savior */, 
(@REF_TORTOS, 95501, 0, 1, 1, 1, 1) /* Fyn's Flickering Dagger */, 
(@REF_TORTOS, 95502, 0, 1, 1, 1, 1) /* Do-tharak, the Swordbreaker */, 
(@REF_TORTOS, 95503, 0, 1, 1, 1, 1) /* Miracoran, the Vehement Chord */, 
(@REF_TORTOS, 95504, 0, 1, 1, 1, 1) /* Zeeg's Ancient Kegsmasher */, 
(@REF_TORTOS, 95505, 0, 1, 1, 1, 1) /* Greatsword of Frozen Hells */, 
(@REF_TORTOS, 95506, 0, 1, 1, 1, 1) /* Nadagast's Exsanguinator */, 
(@REF_TORTOS, 95507, 0, 1, 1, 1, 1) /* Darkwood Spiritstaff */, 
(@REF_TORTOS, 95516, 0, 1, 1, 1, 1) /* Visage of the Doomed */,
(@REF_TORTOS, 96051, 0, 1, 1, 1, 1) /* Rockfall Ribwraps */,
(@REF_TORTOS, 96056, 0, 1, 1, 1, 1) /* Shell-Coated Wristplates */,
(@REF_TORTOS, 96066, 0, 1, 1, 1, 1) /* Robes of Concussive Shocks */,
(@REF_TORTOS, 97126, 0, 1, 1, 1, 1) /* Tia-Tia, the Scything Star */,
(@REF_TORTOS_HM, 94768, 0, 1, 1, 1, 1) /* Shellsplitter Greataxe */, 
(@REF_TORTOS_HM, 94769, 0, 1, 1, 1, 1) /* Shattered Tortoiseshell Longbow */, 
(@REF_TORTOS_HM, 94770, 0, 1, 1, 1, 1) /* Rockfall Ribwraps */, 
(@REF_TORTOS_HM, 94771, 0, 1, 1, 1, 1) /* Shell-Coated Wristplates */, 
(@REF_TORTOS_HM, 94772, 0, 1, 1, 1, 1) /* Quakestompers */, 
(@REF_TORTOS_HM, 94773, 0, 1, 1, 1, 1) /* Shoulderguards of Centripetal Destruction */, 
(@REF_TORTOS_HM, 94774, 0, 1, 1, 1, 1) /* Beakbreaker Greatcloak */, 
(@REF_TORTOS_HM, 94775, 0, 1, 1, 1, 1) /* Beady-Eye Bracers */, 
(@REF_TORTOS_HM, 94776, 0, 1, 1, 1, 1) /* Amulet of the Primal Turtle */, 
(@REF_TORTOS_HM, 94777, 0, 1, 1, 1, 1) /* Grips of Vampiric Cruelty */, 
(@REF_TORTOS_HM, 94778, 0, 1, 1, 1, 1) /* Tortos' Discarded Shell */, 
(@REF_TORTOS_HM, 94779, 0, 1, 1, 1, 1) /* Robes of Concussive Shocks */, 
(@REF_TORTOS_HM, 94780, 0, 1, 1, 1, 1) /* Crystal-Claw Gloves */, 
(@REF_TORTOS_HM, 94781, 0, 1, 1, 1, 1) /* Azure Shell Bracers */, 
(@REF_TORTOS_HM, 94782, 0, 1, 1, 1, 1) /* Stonegaze Hood */, 
(@REF_TORTOS_HM, 94783, 0, 1, 1, 1, 1) /* Spaulders of Quaking Fear */, 
(@REF_TORTOS_HM, 94784, 0, 1, 1, 1, 1) /* Refreshing Abalone Girdle */, 
(@REF_TORTOS_HM, 94785, 0, 1, 1, 1, 1) /* Shimmershell Cape */, 
(@REF_TORTOS_HM, 94786, 0, 1, 1, 1, 1) /* Vampire Bat-Hide Bracers */, 
(@REF_TORTOS_HM, 94787, 0, 1, 1, 1, 1) /* Tortos' Shellseizers */, 
(@REF_TORTOS_HM, 95060, 0, 1, 1, 1, 1) /* Abandoned Spaulders of Arrowflight */, 
(@REF_TORTOS_HM, 95061, 0, 1, 1, 1, 1) /* Lost Shoulders of Fire */, 
(@REF_TORTOS_HM, 95062, 0, 1, 1, 1, 1) /* Forgotten Mantle of the Sun */, 
(@REF_TORTOS_HM, 95063, 0, 1, 1, 1, 1) /* Reconstructed Furious Shoulderplates */, 
(@REF_TORTOS_HM, 95064, 0, 1, 1, 1, 1) /* Abandoned Spaulders of Renewal */, 
(@REF_TORTOS_HM, 95065, 0, 1, 1, 1, 1) /* Forgotten Mantle of the Moon */, 
(@REF_TORTOS_HM, 95066, 0, 1, 1, 1, 1) /* Lost Shoulders of Healing */, 
(@REF_TORTOS_HM, 95067, 0, 1, 1, 1, 1) /* Lost Shoulders of Fluidity */, 
(@REF_TORTOS_HM, 95068, 0, 1, 1, 1, 1) /* Reconstructed Bloody Shoulderplates */, 
(@REF_TORTOS_HM, 95069, 0, 1, 1, 1, 1) /* Reconstructed Holy Shoulderplates */, 
(@REF_TORTOS_HM, 95498, 0, 1, 1, 1, 1) /* Bo-Ris, Horror in the Night */, 
(@REF_TORTOS_HM, 95499, 0, 1, 1, 1, 1) /* Invocation of the Dawn */, 
(@REF_TORTOS_HM, 95500, 0, 1, 1, 1, 1) /* Jerthud, Graceful Hand of the Savior */, 
(@REF_TORTOS_HM, 95501, 0, 1, 1, 1, 1) /* Fyn's Flickering Dagger */, 
(@REF_TORTOS_HM, 95502, 0, 1, 1, 1, 1) /* Do-tharak, the Swordbreaker */, 
(@REF_TORTOS_HM, 95503, 0, 1, 1, 1, 1) /* Miracoran, the Vehement Chord */, 
(@REF_TORTOS_HM, 95504, 0, 1, 1, 1, 1) /* Zeeg's Ancient Kegsmasher */, 
(@REF_TORTOS_HM, 95505, 0, 1, 1, 1, 1) /* Greatsword of Frozen Hells */, 
(@REF_TORTOS_HM, 95506, 0, 1, 1, 1, 1) /* Nadagast's Exsanguinator */, 
(@REF_TORTOS_HM, 95507, 0, 1, 1, 1, 1) /* Darkwood Spiritstaff */, 
(@REF_TORTOS_HM, 95516, 0, 1, 1, 1, 1) /* Visage of the Doomed */,
(@REF_TORTOS_HM, 96051, 0, 1, 1, 1, 1) /* Rockfall Ribwraps */,
(@REF_TORTOS_HM, 96056, 0, 1, 1, 1, 1) /* Shell-Coated Wristplates */,
(@REF_TORTOS_HM, 96066, 0, 1, 1, 1, 1) /* Robes of Concussive Shocks */,
(@REF_TORTOS_HM, 97126, 0, 1, 1, 1, 1) /* Tia-Tia, the Scything Star */;

UPDATE `creature_template` SET `lootid` = @REF_TORTOS WHERE `entry` = @REF_TORTOS;
DELETE FROM `creature_loot_template` WHERE `entry` = @REF_TORTOS;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TORTOS, 1, 100, 1, 0, -@REF_TORTOS, 3);
UPDATE `creature_template` SET `lootid` = @REF_TORTOS_HM WHERE `entry` = @REF_TORTOS_HM;
DELETE FROM `creature_loot_template` WHERE `entry` = @REF_TORTOS_HM;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_TORTOS_HM, 1, 100, 1, 0, -@REF_TORTOS_HM, 5);