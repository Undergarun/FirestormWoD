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


-- Templates All Modes

-- Kaz'Ra Jin

UPDATE creature_template SET difficulty_entry_4 = 80134, difficulty_entry_5 = 81134, difficulty_entry_6 = 82134 WHERE entry = 69134;

-- N25

DELETE FROM creature_template WHERE entry = 80134;
insert into `creature_template` values('80134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (4)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80134;
-- H10

DELETE FROM creature_template WHERE entry = 81134;
insert into `creature_template` values('81134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (5)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81134;
-- H25

DELETE FROM creature_template WHERE entry = 82134;
insert into `creature_template` values('82134','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47229','0','0','0','Kazra\'jin (6)','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','4','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','912','1','0','769638399','1','boss_kazra_jin','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82134;

-- Sul the Sandcrawler

UPDATE creature_template SET difficulty_entry_4 = 80078, difficulty_entry_5 = 81078, difficulty_entry_6 = 82078 WHERE entry = 69078;

-- N25

DELETE FROM creature_template WHERE entry = 80078;
insert into `creature_template` values('80078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80078;
-- H10

DELETE FROM creature_template WHERE entry = 81078;
insert into `creature_template` values('81078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81078;
-- H25

DELETE FROM creature_template WHERE entry = 82078;
insert into `creature_template` values('82078','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47505','0','0','0','Sul the Sandcrawler','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','171.7','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_sul_the_sandcrawler','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82078;

-- Frost King Malakk

UPDATE creature_template SET difficulty_entry_4 = 80131, difficulty_entry_5 = 81131, difficulty_entry_6 = 82131 WHERE entry = 69131;

-- N25

DELETE FROM creature_template WHERE entry = 80131;
insert into `creature_template` values('80131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 440.7 WHERE `entry` = 80131;
-- H10

DELETE FROM creature_template WHERE entry = 81131;
insert into `creature_template` values('81131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 231.1 WHERE `entry` = 81131;
-- H25

DELETE FROM creature_template WHERE entry = 82131;
insert into `creature_template` values('82131','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47506','0','0','0','Frost King Malakk','','','0','93','93','4','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','4','0','0','0','0','0','0','0','0','0','0','0','7','2097228','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2665','0','0','','0','3','1','147.2','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_king_malakk','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 693.2 WHERE `entry` = 82131;

-- High Priestress Mar'Li

UPDATE creature_template SET difficulty_entry_4 = 80132, difficulty_entry_5 = 81132, difficulty_entry_6 = 82132 WHERE entry = 69132;

-- N25

DELETE FROM creature_template WHERE entry = 80132;
insert into `creature_template` values('80132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 6, `Health_mod` = 412 WHERE `entry` = 80131;
-- H10

DELETE FROM creature_template WHERE entry = 81132;
insert into `creature_template` values('81132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 7, `Health_mod` = 215.7 WHERE `entry` = 81131;
-- H25

DELETE FROM creature_template WHERE entry = 82132;
insert into `creature_template` values('82132','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','47730','0','0','0','High Priestess Mar\'li','','','0','93','93','0','0','14','14','0','0','1','1.14286','1.14286','1','3','43563','69309','0','62114','2','2000','2000','1','0','0','0','0','0','0','0','0','0','0','0','7','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','3000000','3100000','','0','3','1','137.4','1','1','1','0','0','0','0','0','0','0','169','1','0','769638399','1','boss_high_priestress_mar_li','17614');
UPDATE `creature_template` SET `dmg_multiplier` = 9, `Health_mod` = 647.04 WHERE `entry` = 82131;