-- 61348 - Lorewalker Cho
delete from creature_template where entry = 61348;
insert into `creature_template` (`entry`, `difficulty_entry_1`, `difficulty_entry_2`, `difficulty_entry_3`, `difficulty_entry_4`, `difficulty_entry_5`, `difficulty_entry_6`, `difficulty_entry_7`, `difficulty_entry_8`, `difficulty_entry_9`, `difficulty_entry_10`, `KillCredit1`, `KillCredit2`, `modelid1`, `modelid2`, `modelid3`, `modelid4`, `name`, `subname`, `IconName`, `gossip_menu_id`, `minlevel`, `maxlevel`, `exp`, `exp_unk`, `faction_A`, `faction_H`, `npcflag`, `npcflag2`, `speed_walk`, `speed_run`, `speed_fly`, `scale`, `rank`, `mindmg`, `maxdmg`, `dmgschool`, `attackpower`, `dmg_multiplier`, `baseattacktime`, `rangeattacktime`, `unit_class`, `unit_flags`, `unit_flags2`, `dynamicflags`, `family`, `trainer_type`, `trainer_spell`, `trainer_class`, `trainer_race`, `minrangedmg`, `maxrangedmg`, `rangedattackpower`, `type`, `type_flags`, `type_flags2`, `lootid`, `pickpocketloot`, `skinloot`, `resistance1`, `resistance2`, `resistance3`, `resistance4`, `resistance5`, `resistance6`, `spell1`, `spell2`, `spell3`, `spell4`, `spell5`, `spell6`, `spell7`, `spell8`, `PetSpellDataId`, `VehicleId`, `mingold`, `maxgold`, `AIName`, `MovementType`, `InhabitType`, `HoverHeight`, `Health_mod`, `Mana_mod`, `Mana_mod_extra`, `Armor_mod`, `RacialLeader`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `movementId`, `RegenHealth`, `equipment_id`, `mechanic_immune_mask`, `flags_extra`, `ScriptName`, `WDBVerified`) values('61348','0','0','0','0','0','0','0','0','0','0','0','0','41918','0','0','0','Lorewalker Cho','','','55015','92','92','4','0','35','35','1','0','1','1.42857','1.14286','1','1','4750','11118','0','55538','4','1500','2000','1','0','0','0','0','0','0','0','0','0','0','0','10','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','','0','3','1','60','10','1','1','0','0','0','0','0','0','0','144','1','0','0','0','npc_lorewalker_cho','16048');

-- Complete waypoints for Lorewalker Cho
delete from script_waypoint where entry = 61348;
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','0','3900.57','1064.84','485.614','0','');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','1','3900.01','1075.86','481.137','0','');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','2','3897.72','1104.57','481.133','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','3','3869.48','1123.95','481.24','0','Stop pre-Stone guard Death');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','4','3897.51','1137.19','481.132','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','5','3897.32','1168.86','466.17','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','6','3892.19','1255.06','466.17','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','7','3906.83','1257.49','466.17','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','8','3905.96','1268.11','470.868','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','9','3896.58','1309.2','470.898','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','10','3899.3','1341.95','466.561','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','11','3946.54','1342.56','461.944','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','12','3964.7','1341.5','461.951','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','13','3995.25','1340.71','461.959','0','Stop Pre-Feng');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','14','4008.08','1339.36','466.305','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','15','4063.92','1341.85','466.305','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','16','4067.35','1341.7','466.305','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','17','4077.38','1344.72','466.305','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','18','4090.92','1348.87','461.961','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','19','4128.93','1351.51','461.886','0','Stop post Feng');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','20','4128.8','1342.19','461.885','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','21','4204.53','1345.82','461.961','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','22','4212.95','1343.36','461.741','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','23','4219.24','1343.6','460.782','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','24','4225.88','1343.94','458.941','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','25','4233.2','1341.78','455.802','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','26','4238.84','1342.84','454.154','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','27','4278.35','1340.6','454.466','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','28','4277.14','1378.05','454.153','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','29','4277.28','1387','453.795','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','31','4277.27','1399.46','450.918','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','32','4277.26','1407.08','447.588','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','33','4279.28','1437.58','445.378','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','34','4277.24','1496.59','445.589','0','Enter Spirit Kings room');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','35','4291.3','1498.88','445.582','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','36','4292.26','1511.96','445.645','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','37','4292.95','1525.81','438.804','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','38','4292.84','1533.07','438.804','0','Gossip Menu');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','39','4285.79','1532.27','438.804','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','40','4259.03','1533.09','423.936','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','41','4231.8','1521.87','423.935','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','42','4226.74','1486.16','423.935','0','Spirit Kings waiting 1');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','43','4228.21','1549.84','423.935','0','Stop post Spirit Kings');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','44','4228.11','1569.59','423.961','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','45','4228.61','1594.85','410.369','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','46','4226.02','1607.5','410.455','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','47','4211.58','1607.36','410.359','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','48','4186.26','1607.58','396.767','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','49','4169.14','1608.07','396.766','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','50','4169.58','1591.7','396.788','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','51','4170.3','1565.78','383.218','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','52','4165.91','1549.27','383.227','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','53','4134.51','1550.38','383.234','0','Modelers Hall entered');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','54','4128.19','1550.24','383.287','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','55','4116.24','1550.64','378.833','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','56','4073.54','1550.4','378.833','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','57','4073.17','1567.82','378.829','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','58','4073.06','1573.1','380.334','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','59','4073.36','1587.37','380.231','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','60','4073.3','1596.8','376.898','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','61','4073.1','1625.14','376.909','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','62','4073.75','1645.96','376.878','0','Elegon hall entry');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','63','4072.12','1671.95','363.277','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','64','4072.12','1695.75','363.312','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','65','4022.02','1697.37','363.31','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','66','4023.23','1720.15','363.337','0','Elegon entry');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','67','4022.7','1735.81','363.331','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','68','4022.83','1747.53','363.274','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','69','4023.52','1760.95','358.803','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','70','4022.23','1859.68','358.789','0','Front of Elegon\'s Gob');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','71','4049.3','1807.55','358.855','0','Waits while Elegon\'s fight');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','72','4037.54','1807.81','358.84','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','73','4022.27','1783.01','358.789','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','74','4023.52','1760.95','358.903','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','75','4023.44','1747.29','363.276','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','76','4023.13','1696.75','363.317','0','Just get out of Elegon\'s room');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','77','4070.71','1695.35','363.31','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','78','4071.74','1672.55','363.278','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','79','4072.36','1647.85','376.841','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','80','4073.2','1597.44','376.9','0','Enter Hall Secret Keepers');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','81','4072.77','1573.35','380.334','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','82','4072.74','1567.25','378.83','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','83','4061.53','1550.69','378.834','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','84','4030.73','1550.82','378.833','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','85','4018.69','1550.85','374.37','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','86','3985.13','1550.86','374.22','0','Just entered into Will a Emperor place');
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','87','3975.12','1551.11','369.562','0',NULL);
insert into `script_waypoint` (`entry`, `pointid`, `location_x`, `location_y`, `location_z`, `waittime`, `point_comment`) values('61348','88','3947.46','1550.02','369.562','0','In front of the console');

-- Missing scriptnames in templates
update creature_template set scriptname = 'mob_stone_quilen' where entry = 60576;
update creature_template set scriptname = 'mob_zandalari_skullcharger' where entry in (60381, 60402);
update creature_template set scriptname = 'mob_zian' where entry in (77017, 77104, 77061);
update creature_template set scriptname = 'mob_sorcerer_mogu' where entry in (77013, 77100, 77057);
update creature_template set scriptname = 'mob_qiang' where entry in (77018, 77105, 77062);
update creature_template set scriptname = 'mob_mounted_mogu' where entry in (77014, 77101, 77058);
update creature_template set scriptname = 'mob_subetai' where entry in (77019, 77106, 77063);
update creature_template set scriptname = 'mob_mogu_archer' where entry in (77015, 77102, 77059);
update creature_template set scriptname = 'mob_meng' where entry in (77020, 77107, 77064);
update creature_template set scriptname = 'mob_kingsguard' where entry in (77016, 77103, 77060);
update creature_template set scriptname = 'mob_mogu_secret_keeper' where entry = 61131;
update creature_template set scriptname = 'mob_mogu_warden' where entry in (64061, 64063, 64947);
update creature_template set scriptname = 'mob_mogu_engine_keeper' where entry = 64068;

update gameobject_template set scriptname = 'go_ancien_control_console' where entry = 211584;
update gameobject_template set flags = 16 where entry = 211650;

-- Respawn time for Mogushan Vault Bosses
update creature set spawntimesecs = 604800000 where id in (60410, 60009, 60143, 60047, 60043, 59915, 60051, 60399, 60400, 60701, 60708, 60709, 60710)

-- Spawning Spirit Kings
insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('60701','1008','6125','6125','760','1','0','0','4226.97','1558.32','438.804','1.58495','604800000','0','0','1','0','0','0','0','0','0','0','0');
insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('60708','1008','6125','6125','760','1','0','0','4198.78','1590.29','438.841','6.26345','604800000','0','0','1','0','0','0','0','0','0','0','0');
insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('60709','1008','6125','6125','760','1','0','0','4226.33','1626.28','438.856','4.72348','604800000','0','0','1','0','0','0','0','0','0','0','0');
insert into `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`, `protec_anti_doublet`) values('60710','1008','6125','6125','760','1','0','0','4257.35','1591.36','438.841','3.13526','604800000','0','0','1','0','0','0','0','0','0','0','0');

-- Removing WOE's adds
DELETE FROM creature WHERE id IN (60396, 60397, 60398);

-- Removing 2 surnumerous Elegon
DELETE FROM creature WHERE id = 60410 AND guid != (SELECT MAX(guid) FROM (SELECT * FROM creature WHERE id = 60410) AS req);