-- beta portal
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`)
VALUES
    (227833, 22, 15350, 'Portal to Frostfire Ridge', '', '', '', 0, 0, 0.8, 0, 0, 0, 0, 0, 0, 156773, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19116);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES ('156773', '0', '1116', '5859.56', '4477.74', '117.71', '0');

-- custom
INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `data24`, `data25`, `data26`, `data27`, `data28`, `data29`, `data30`, `data31`, `data32`, `unkInt32`, `AIName`, `ScriptName`, `WDBVerified`)
VALUES
    (10227833, 22, 15350, 'TEMP Portal to shadowmoon', '', '', '', 0, 0, 0.8, 0, 0, 0, 0, 0, 0, 156774, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 19116);
INSERT INTO `spell_target_position` (`id`, `effIndex`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES ('156774', '0', '1116', '1585.22', '104.39', '65.76', '0');

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('17', '0', '156774', '0', '0', '27', '0', '90', '3', '0', '0', '0', '', 'level 90 or higher for use the portal to draenor');
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES ('17', '0', '156773', '0', '0', '27', '0', '90', '3', '0', '0', '0', '', 'level 90 or higher for use the portal to draenor');

-- custom pos
INSERT INTO `gameobject` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`, `isActive`, `protect_anti_doublet`)
VALUES
    (10227833, 0, 1519, 5428, 1, 65535, -8205.44, 413.021, 117.997, 1.57076, 0, 0, 0.707094, 0.70712, 300, 0, 1, 0, NULL);