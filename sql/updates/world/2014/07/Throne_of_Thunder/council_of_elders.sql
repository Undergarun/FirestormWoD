-- Adjusts some templates

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`exp` = 4, 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 43563, 
`maxdmg` = 69309, 
`attackpower` = 62114, 
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 4,
`unit_class` = 4,
`ScriptName` = 'boss_kazra_jin',
`flags_extra` = 0x01,
`Health_mod` = 147.2,
`mechanic_immune_mask` = 769638399
WHERE `entry` = 69134;

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`faction_A` = 14, 
`faction_H` = 14,
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0,
`unit_class` = 4,
`ScriptName` = 'boss_sul_the_sandcrawler',
`flags_extra` = 0x01,
`exp` = 4, 
`mindmg` = 43563, 
`maxdmg` = 69309, 
`Health_mod` = 147.2,
`attackpower` = 62114,
`mechanic_immune_mask` = 769638399
WHERE `entry` = 69078;

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`faction_A` = 14, 
`faction_H` = 14, 
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000,
`VehicleId` = 2665,
`unit_class` = 4, 
`ScriptName` = 'boss_king_malakk',
`exp` = 4, 
`mindmg` = 43563, 
`maxdmg` = 69309, 
`attackpower` = 62114, 
`Health_mod` = 147.2, 
`mechanic_immune_mask` = 769638399,
`flags_extra` = 0x01
WHERE `entry` = 69131;

UPDATE `creature_template` SET 
`minlevel` = 93, 
`maxlevel` = 93, 
`faction_A` = 14, 
`faction_H` = 14, 
`dmg_multiplier` = 2, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0, 
`ScriptName` = 'boss_high_priestress_mar_li',
`mindmg` = 43563, 
`maxdmg` = 69309, 
`attackpower` = 62114, 
`Health_mod` = 137.4, 
`mechanic_immune_mask` = 769638399,
`flags_extra` = 0x01
WHERE `entry` = 69132;

UPDATE `creature_template` SET 
`type_flags` = 1049600, 
`ScriptName` = 'npc_gara_jal_s_soul' ,
`modelid1` = 11686, 
`modelid2` = 0
WHERE `entry` = 69182;

UPDATE `creature_template` SET
`minlevel` = 90,
`maxlevel` = 90,
`exp` = 4,
`mindmg` = 9838,
`maxdmg` = 14331,
`attackpower` = 42296,
`baseattacktime` = 2000,
`rangeattacktime` = 2000,
`unit_flags` = 33554438,
`ScriptName` = 'npc_gara_jal_the_spirit_binder'
WHERE `entry` = 69135;

UPDATE `creature_template` SET 
`modelid1` = 11686, 
`modelid2` = 0
WHERE `entry` = 68553;

UPDATE `creature_template` SET 
`ScriptName` = 'mob_living_sand', 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000,
`exp` = 4
WHERE `entry` = 69153;

UPDATE `creature_template` SET 
`faction_A` = 7, 
`faction_H` = 7, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`ScriptName` = 'mob_blessed_loa_spirit'
WHERE `entry` = 69480;

UPDATE `creature_template` SET 
`faction_A` = 7, 
`faction_H` = 7, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`ScriptName` = 'mob_shadowed_lua_spirit'
WHERE `entry` = 69548;

UPDATE `creature_template` SET 
`exp` = 4, 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0, 
`ScriptName` = 'mob_first_twisted_fate' 
WHERE `entry` = 69740;

UPDATE `creature_template` SET 
`exp` = 4, 
`faction_A` = 14, 
`faction_H` = 14, 
`mindmg` = 9838, 
`maxdmg` = 14331, 
`attackpower` = 42296, 
`baseattacktime` = 2000, 
`rangeattacktime` = 2000, 
`type_flags` = 0, 
`ScriptName` = 'mob_second_twisted_fate' 
WHERE `entry` = 69746;

UPDATE `creature_template` SET
`modelid1` = 11686,
`modelid2` = 0
WHERE `entry` = 69453;

-- Scriptnames

DELETE FROM spell_script_names WHERE spell_id IN (137131, 137117, 136904, 136467, 136917, 136990, 136922, 137149, 136878, 136895, 136507, 137643, 137641, 137166, 137943, 137964, 137972);
INSERT INTO spell_script_names VALUES
(137131, "spell_reckless_charge_movement"),
(137117, "spell_reckless_charge_rolling"),
(136904, "spell_frigid_assault"),
(136467, "spell_lingering_presence"),
(136917, "spell_biting_cold_malakk"),
(136990, "spell_frosbite_malakk"),
(136922, "spell_frosbite_malakk_aura"),
(137149, "spell_overload_discharge_kazra_jin"),
(137166, "spell_overload_discharge_kazra_jin"),
(136878, "spell_ensnared"),
(136895, "spell_sandstorm"),
(136507, "spell_dark_power"),
(137643, "spell_soul_fragment_switch"),
(137641, "spell_soul_fragment"),
(137943, "spell_first_twisted_fate"),
(137964, "spell_second_twisted_fate"),
(137972, "spell_twisted_fate_damage");


-- Boss Spawns

DELETE FROM creature WHERE id IN (69182, 69135, 69132, 69131, 69078, 69134);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `dynamicflags`, `isActive`, `protec_anti_doublet`) VALUES
		('69134','1098','6622','6705','760','1','0','0','6056.48','5386.44','136.171','2.11191','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Kazra'jin
		('69078','1098','6622','6705','760','1','0','0','6068.75','5396.01','136.171','2.32446','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Sul the Sandcrawler
		('69131','1098','6622','6705','760','1','0','0','6025.24','5393.14','136.171','0.699817','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Frost King Malakk
		('69132','1098','6622','6705','760','1','0','0','6035.9','5385.33','136.171','1.17586','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- High Priestess Mar'li
		('69182','1098','6622','6705','760','1','0','0','6046.76','5432.27','136.171','4.7123','604800','0','0','1','0','0','0','0','0','0','0','0',NULL), -- Gara'jal's Soul
		('69135','1098','6622','6705','760','1','0','0','6046.78','5432.15','137.694','4.69419','604800','0','0','1','0','0','0','0','0','0','0','0',NULL); -- Gara'jal the Spiritbin




-- Talks

-- Gara'Jal the SpiritBinder - 69135

DELETE FROM creature_text WHERE entry = 69135;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69135','0','0','Witness the powa of the Spiritbinder!','14','0','100','0','0','35398','Tot CoE aggro');

-- High Priestress Mar'Li - 69132

DELETE FROM creature_text WHERE entry = 69132;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','0','0','Death ta all who appose da empire!','14','0','100','0','0','35432','Mar\'Li aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','1','0','Da spiritbinder reveals yer soul ta me!','14','0','100','0','0','35434','Mar\'Li possession');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','2','0','Yer soul belongs ta me!','14','0','100','0','0','35439','Mar\'Li shadowed Loa 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','3','0','Succumb ta her venom!','14','0','100','0','0','35438','Mar\'Li shadowed Loa 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','4','0','Embrace yer demise!','14','0','100','0','0','35440','Mar\'Li shadowed Loa 3');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','5','0','Shadra\'s rage will destroy you!','14','0','100','0','0','35435','Mar\'Li shadowed Loa 4');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','6','0','She will feast on yer soul!','14','0','100','0','0','35437','Mar\'Li player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','7','0','Another offering to da loa!','14','0','100','0','0','35436','Mar\'Li player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69132','8','0','Shadra... save... me...','14','0','100','0','0','35433','Mar\'Li death');


-- Kaz'Ra Jin - 69134

DELETE FROM creature_text WHERE entry = 69134;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','0','0','Dis is gonna hurt!','14','0','100','0','0','35566','Kaz\'Ra Jin Aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','1','0','Ya shouldn\'t be messin\' wit da Zandalari!','14','0','100','0','0','35571','Kaz\'Ra Jin possession');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','2','0','Incoming!','14','0','100','0','0','35574','Kaz\'Ra Jin Reckless Charge 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','3','0','Out da way!','14','0','100','0','0','35573','Kaz\'Ra Jin Reckless Charge 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','4','0','Shocking!','14','0','100','0','0','35576','Kaz\'Ra Jin discharge');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','5','0','Da Amani empire cannot be stopped!','14','0','100','0','0','35570','Kaz\'Ra Jin player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','6','0','On ya knees!','14','0','100','0','0','35572','Kaz\'Ra Jin player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69134','7','0','Da thunder king... promised...','14','0','100','0','0','35567','Kaz\'Ra Jin death');



-- Frost King Malakk - 69131

DELETE FROM creature_text WHERE entry = 69131;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','0','0','Ya have met your match, fools!','14','0','100','0','0','35387','Malakk aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','1','0','Winter is coming...','14','0','100','0','0','35389','Malakk possession');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','2','0','Freeze!','14','0','100','0','0','35394','Malakk Frostbite 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','3','0','Getting cold?','14','0','100','0','0','35393','Malakk Frostbite 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','4','0','Da\' Drakkari will never fall to de\' likes of you!','14','0','100','0','0','35390','Malakk Body Heat');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','5','0','Death\'s cold embrace.','14','0','100','0','0','35391','Malakk player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','6','0','Witness da Drakkari\'s might.','14','0','100','0','0','35392','Malakk player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69131','7','0','Da... empire... can\'t... fall...','14','0','100','0','0','35388','Malakk\'s death');


-- Sul the Sandcrawler - 69078

DELETE FROM creature_text WHERE entry = 69078;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','0','0','Da sands will consume everyting!','14','0','100','0','0','35812','Sul aggro');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','1','0','I will bury ya all!','14','0','100','0','0','35813','Sul possessed');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','2','0','Da storm approaches!','14','0','100','0','0','35818','Sul Sandstorm');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','3','0','Watch yer step!','14','0','100','0','0','35817','Sul Quicksand');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','4','0','Dey tink dey can stop da Farraki?','14','0','100','0','0','35814','Sul Treacherous Ground');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','5','0','Da sands are endless.','14','0','100','0','0','35815','Sul player killed 1');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','6','0','Da first of many!','14','0','100','0','0','35816','Sul player killed 2');
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES('69078','7','0','I return... to... da... sands...','14','0','100','0','0','36454','Sul\'s death');


-- Templates All Modes

-- Kaz'Ra Jin

UPDATE creature_template SET difficulty_entry_4 = 80134, difficulty_entry_5 = 81134, difficulty_entry_6 = 82134 WHERE entry = 69134;

-- N25

DELETE FROM creature_template WHERE entry = 80134;
INSERT INTO `creature_template` VALUES('80134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80134;
-- H10

DELETE FROM creature_template WHERE entry = 81134;
INSERT INTO `creature_template` VALUES('81134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81134;
-- H25

DELETE FROM creature_template WHERE entry = 82134;
INSERT INTO `creature_template` VALUES('82134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82134;

-- Sul the Sandcrawler

UPDATE creature_template SET difficulty_entry_4 = 80078, difficulty_entry_5 = 81078, difficulty_entry_6 = 82078 WHERE entry = 69078;

-- N25

DELETE FROM creature_template WHERE entry = 80078;
INSERT INTO `creature_template` VALUES('80078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80078;
-- H10

DELETE FROM creature_template WHERE entry = 81078;
INSERT INTO `creature_template` VALUES('81078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81078;
-- H25

DELETE FROM creature_template WHERE entry = 82078;
INSERT INTO `creature_template` VALUES('82078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82078;

-- Frost King Malakk

UPDATE creature_template SET difficulty_entry_4 = 80131, difficulty_entry_5 = 81131, difficulty_entry_6 = 82131 WHERE entry = 69131;

-- N25

DELETE FROM creature_template WHERE entry = 80131;
INSERT INTO `creature_template` VALUES('80131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80131;
-- H10

DELETE FROM creature_template WHERE entry = 81131;
INSERT INTO `creature_template` VALUES('81131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81131;
-- H25

DELETE FROM creature_template WHERE entry = 82131;
INSERT INTO `creature_template` VALUES('82131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82131;

-- High Priestress Mar'Li

UPDATE creature_template SET difficulty_entry_4 = 80132, difficulty_entry_5 = 81132, difficulty_entry_6 = 82132 WHERE entry = 69132;

-- N25

DELETE FROM creature_template WHERE entry = 80132;
INSERT INTO `creature_template` VALUES('80132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 412 WHERE `entry` = 80131;
-- H10

DELETE FROM creature_template WHERE entry = 81132;
INSERT INTO `creature_template` VALUES('81132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 215.7 WHERE `entry` = 81131;
-- H25

DELETE FROM creature_template WHERE entry = 82132;
INSERT INTO `creature_template` VALUES('82132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 647.04 WHERE `entry` = 82131;