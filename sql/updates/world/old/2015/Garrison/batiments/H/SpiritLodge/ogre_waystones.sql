DELIMITER @@

CREATE PROCEDURE `FillConditionTable`(IN `p_LootId` INT, IN `p_DropChance` FLOAT)
BEGIN

	REPLACE INTO creature_loot_template (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (p_LootId, 117491, p_DropChance, 1, 0, 1, 3, '');
	-- 39 = CONDITION_HAS_BUILDING_TYPE, 13 = BuildingType::Type::MageTower
	DELETE FROM conditions WHERE SourceTypeOrReferenceId = 1 AND SourceGroup = p_LootId AND SourceEntry = 117491;
	INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, ConditionTypeOrReference, ConditionValue1) VALUES (1, p_LootId, 117491, 39, 13);
END @@
DELIMITER ;


SET @l_Entry = 75194;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76101;
SET @l_DropRate = 0;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77036;
SET @l_DropRate = 0;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 85241;
SET @l_DropRate = 0;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75211;
SET @l_DropRate = 0;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87619;
SET @l_DropRate = 2.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74254;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78728;
SET @l_DropRate = 0.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76181;
SET @l_DropRate = 0;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76157;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77035;
SET @l_DropRate = 0.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87910;
SET @l_DropRate = 9.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73472;
SET @l_DropRate = 1.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73763;
SET @l_DropRate = 1.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75820;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76176;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74707;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75500;
SET @l_DropRate = 2.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75406;
SET @l_DropRate = 0.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81750;
SET @l_DropRate = 0.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73470;
SET @l_DropRate = 0.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 89824;
SET @l_DropRate = 3.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76846;
SET @l_DropRate = 2.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77037;
SET @l_DropRate = 0.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75814;
SET @l_DropRate = 17.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86290;
SET @l_DropRate = 8.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75057;
SET @l_DropRate = 0.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73373;
SET @l_DropRate = 0.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74475;
SET @l_DropRate = 0.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73371;
SET @l_DropRate = 0.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77020;
SET @l_DropRate = 0.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74706;
SET @l_DropRate = 0.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77927;
SET @l_DropRate = 0.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80335;
SET @l_DropRate = 0.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74704;
SET @l_DropRate = 0.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75240;
SET @l_DropRate = 0.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76918;
SET @l_DropRate = 1.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 82900;
SET @l_DropRate = 9.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77351;
SET @l_DropRate = 8.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76696;
SET @l_DropRate = 1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75209;
SET @l_DropRate = 0.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81269;
SET @l_DropRate = 12.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80312;
SET @l_DropRate = 0.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76876;
SET @l_DropRate = 7.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77034;
SET @l_DropRate = 0.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73469;
SET @l_DropRate = 1.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79651;
SET @l_DropRate = 27.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81780;
SET @l_DropRate = 17.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87846;
SET @l_DropRate = 4.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77349;
SET @l_DropRate = 1.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78003;
SET @l_DropRate = 3.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74787;
SET @l_DropRate = 0.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81808;
SET @l_DropRate = 23.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81807;
SET @l_DropRate = 24.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81810;
SET @l_DropRate = 22.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79267;
SET @l_DropRate = 2.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 88459;
SET @l_DropRate = 6.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78778;
SET @l_DropRate = 6.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79927;
SET @l_DropRate = 18.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81809;
SET @l_DropRate = 23.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 84260;
SET @l_DropRate = 24.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81811;
SET @l_DropRate = 26.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87538;
SET @l_DropRate = 39.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79024;
SET @l_DropRate = 2.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79266;
SET @l_DropRate = 3.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72873;
SET @l_DropRate = 1.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73676;
SET @l_DropRate = 30.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 84675;
SET @l_DropRate = 2.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86256;
SET @l_DropRate = 18.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79107;
SET @l_DropRate = 51.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78437;
SET @l_DropRate = 24.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79623;
SET @l_DropRate = 4.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80295;
SET @l_DropRate = 10.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76694;
SET @l_DropRate = 1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81806;
SET @l_DropRate = 19.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80294;
SET @l_DropRate = 10.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86330;
SET @l_DropRate = 22.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86326;
SET @l_DropRate = 24.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86329;
SET @l_DropRate = 23.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78174;
SET @l_DropRate = 8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81270;
SET @l_DropRate = 13.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74349;
SET @l_DropRate = 14.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79629;
SET @l_DropRate = 4.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86275;
SET @l_DropRate = 16.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80292;
SET @l_DropRate = 14.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87095;
SET @l_DropRate = 18.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79626;
SET @l_DropRate = 7.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81272;
SET @l_DropRate = 13.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78782;
SET @l_DropRate = 16.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 85225;
SET @l_DropRate = 14.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79650;
SET @l_DropRate = 22.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87223;
SET @l_DropRate = 20.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80283;
SET @l_DropRate = 9.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79621;
SET @l_DropRate = 9.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77995;
SET @l_DropRate = 3.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80948;
SET @l_DropRate = 44.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80949;
SET @l_DropRate = 45.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 82619;
SET @l_DropRate = 10.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76935;
SET @l_DropRate = 8.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 82528;
SET @l_DropRate = 23.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75426;
SET @l_DropRate = 40.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73555;
SET @l_DropRate = 5.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77033;
SET @l_DropRate = 19.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76599;
SET @l_DropRate = 12.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86155;
SET @l_DropRate = 13.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 88207;
SET @l_DropRate = 6.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 80172;
SET @l_DropRate = 9.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 82519;
SET @l_DropRate = 20.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 84765;
SET @l_DropRate = 9.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74746;
SET @l_DropRate = 32.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81767;
SET @l_DropRate = 23.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73490;
SET @l_DropRate = 36.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 88437;
SET @l_DropRate = 12.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 83848;
SET @l_DropRate = 6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75272;
SET @l_DropRate = 29.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79402;
SET @l_DropRate = 7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 84764;
SET @l_DropRate = 10.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87309;
SET @l_DropRate = 10.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 81764;
SET @l_DropRate = 16.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 74697;
SET @l_DropRate = 6.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87221;
SET @l_DropRate = 18.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79022;
SET @l_DropRate = 50.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 78578;
SET @l_DropRate = 6.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 83641;
SET @l_DropRate = 6.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75198;
SET @l_DropRate = 31.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87222;
SET @l_DropRate = 21;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 84760;
SET @l_DropRate = 12.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 83640;
SET @l_DropRate = 6.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75210;
SET @l_DropRate = 28.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75193;
SET @l_DropRate = 34.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72967;
SET @l_DropRate = 81.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 87310;
SET @l_DropRate = 10.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72752;
SET @l_DropRate = 37.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77993;
SET @l_DropRate = 5.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 88187;
SET @l_DropRate = 6.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73698;
SET @l_DropRate = 47.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77992;
SET @l_DropRate = 4.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 83577;
SET @l_DropRate = 7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 88657;
SET @l_DropRate = 92.1;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 76706;
SET @l_DropRate = 22.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 89127;
SET @l_DropRate = 13.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79023;
SET @l_DropRate = 59;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 84978;
SET @l_DropRate = 28.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 90620;
SET @l_DropRate = 8.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 83575;
SET @l_DropRate = 6.6;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79070;
SET @l_DropRate = 51.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72833;
SET @l_DropRate = 41.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79234;
SET @l_DropRate = 18.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 86279;
SET @l_DropRate = 87.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75191;
SET @l_DropRate = 34.2;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 77991;
SET @l_DropRate = 12.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 73643;
SET @l_DropRate = 86.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 79231;
SET @l_DropRate = 21.7;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72987;
SET @l_DropRate = 90.3;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75835;
SET @l_DropRate = 60.4;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72953;
SET @l_DropRate = 91.8;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 72955;
SET @l_DropRate = 90.5;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

SET @l_Entry = 75819;
SET @l_DropRate = 72.9;
CALL `FillConditionTable` (@l_Entry, @l_DropRate);

DROP PROCEDURE `FillConditionTable`;