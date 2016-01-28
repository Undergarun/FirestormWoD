-- Tanaan Jungle

	-- Spells

		DELETE FROM spell_script_names where spell_id = 165988;
		INSERT INTO spell_script_names VALUES (165988, "spell_tanaan_inferno");

	-- Quests

		#35933/Quest - Azeroth s Last Stand

			DELETE FROM `creature_questrelation` WHERE `quest` = 35933;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35933;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35933;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78558, 35933);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78558;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35933;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35933;
			UPDATE `quest_template` SET `Flags` = 327688 WHERE `Id` = 35933;
			
		#34420/Quest - The cost of war

			DELETE FROM `creature_questrelation` WHERE `quest` = 34420;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34420;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34420;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78558, 34420);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78558;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34420;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34420;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 34420);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;

		#34393/Quest - The Portal s Power

			DELETE FROM `creature_questrelation` WHERE `quest` = 34393;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34393;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34393;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78558, 34393);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78558;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34393;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34393;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78558, 34393);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78558;

		#34392/Quest - Onslaught s end

			DELETE FROM `creature_questrelation` WHERE `quest` = 34392;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34392;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34392;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34392;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34392;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78558, 34392);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78558;

		#34398/Quest - The Dark Portal

			DELETE FROM `creature_questrelation` WHERE `quest` = 34398;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34398;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34398;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34398;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34398;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78558, 34398);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78558;
			
		#35019/Quest - Prepare for Battle
			
			DELETE FROM `creature_questrelation` WHERE `quest` = 35019;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35019;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35019;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78554, 35019);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78554;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35019;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35019;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78554, 35019);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78554;

		#34432/Quest - The Shadowmoon Clan
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 34432;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34432;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34432;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78554, 34432);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78554;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34432;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34432;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79316, 34432);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79316;

		#34422/Quest - Blaze of Glory
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 34422;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34422;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34422;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78559, 34422);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78559;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34422;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34422;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 34422);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;
		
		#35242/Quest - Vengeance for the Fallen
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35242;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35242;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81763, 35242);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81763;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35242;

		#35240/Quest - Bled Dry
		
			DELETE FROM `creature_questrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35240;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35240;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81762, 35240);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81762;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35240;
			DELETE FROM `creature_questrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35240;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35240;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81762, 35240);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81762;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35240;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35240;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 35240);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;


		#35241/Quest - Vengeance for the Fallen

			DELETE FROM `creature_questrelation` WHERE `quest` = 35241;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35241;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35241;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81761, 35241);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81761;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35241;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35241;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78553, 35241);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78553;
			DELETE FROM `creature_questrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35242;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35242;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (81763, 35242);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 81763;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35242;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35242;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78554, 35242);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78554;

		#34423/Quest - Altar Altercation

			DELETE FROM `creature_questrelation` WHERE `quest` = 34423;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34423;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34423;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78559, 34423);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78559;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34423;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34423;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78560, 34423);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78560;

		#34425/Quest - The Kargathar Proving Grounds

			DELETE FROM `creature_questrelation` WHERE `quest` = 34425;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34425;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34425;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34425);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34425;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34425;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78560, 34425);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78560;

		#34478/Quest - A Potential Ally

			DELETE FROM `creature_questrelation` WHERE `quest` = 34478;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34478;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34478;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34478);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34478;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34478;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79537, 34478);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79537;

		#34429/Quest - Kill your hundred

			DELETE FROM `creature_questrelation` WHERE `quest` = 34429;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34429;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34429;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34429);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34429;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34429;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78561, 34429);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78561;

		#34737/Quest - Masters of Shadow

			DELETE FROM `creature_questrelation` WHERE `quest` = 34737;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34737;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34737;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79661, 34737);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79661;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34737;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34737;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79675, 34737);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79675;
			UPDATE `quest_template` SET `RequiredClasses` = 0, `RequiredRaces` = 33555378 WHERE `Id` = 34737;

		#34431/Quest - Masters of Shadow

			DELETE FROM `creature_questrelation` WHERE `quest` = 34431;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34431;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34431;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79661, 34431);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79661;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34431;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34431;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79537, 34431);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79537;
			UPDATE `quest_template` SET `RequiredClasses` = 0, `RequiredRaces` = 18875469 WHERE `Id` = 34431;

		#34739/Quest - The Shadowmoon Clan

			DELETE FROM `creature_questrelation` WHERE `quest` = 34739;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34739;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34739;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78553, 34739);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78553;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34739;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34739;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79315, 34739);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79315;

		#34434/Quest - Yrel

			DELETE FROM `creature_questrelation` WHERE `quest` = 34434;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34434;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34434;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79794, 34434);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79794;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34434;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34434;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78994, 34434);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78994;


		#34740/Quest - Yrel

			DELETE FROM `creature_questrelation` WHERE `quest` = 34740;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34740;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34740;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79794, 34740);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79794;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34740;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34740;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78994, 34740);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78994;

		#34436/Quest - Keli Dan the Breaker

			DELETE FROM `creature_questrelation` WHERE `quest` = 34436;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34436;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34436;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79537, 34436);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79537;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34436;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34436;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78562, 34436);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78562;

		#34741/Quest - Keli Dan the Breaker

			DELETE FROM `creature_questrelation` WHERE `quest` = 34741;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34741;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34741;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79675, 34741);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79675;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34741;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34741;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78562, 34741);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78562;

		#35005/Quest - Prepare for Battle

			DELETE FROM `creature_questrelation` WHERE `quest` = 35005;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35005;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35005;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78553, 35005);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78553;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35005;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35005;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78553, 35005);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78553;

		#34439/Quest - The Battle of the Forge

			DELETE FROM `creature_questrelation` WHERE `quest` = 34439;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34439;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34439;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78430, 34439);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78430;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34439;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34439;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78996, 34439);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78996;

		#34442/Quest - Ga nar of the Frostwolf

			DELETE FROM `creature_questrelation` WHERE `quest` = 34442;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34442;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34442;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78996, 34442);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78996;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34442;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34442;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79917, 34442);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79917;

		#34987/Quest - The Gunpowder Plot

			DELETE FROM `creature_questrelation` WHERE `quest` = 34987;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34987;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34987;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78569, 34987);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78569;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34987;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34987;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78569, 34987);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78569;

		#34958/Quest - The Shadow of the Worldbreaker

			DELETE FROM `creature_questrelation` WHERE `quest` = 34958;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34958;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34958;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78568, 34958);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78568;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34958;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34958;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78568, 34958);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78568;

		#34925/Quest - Polishing the Iron Throne

			DELETE FROM `creature_questrelation` WHERE `quest` = 34925;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34925;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34925;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79917, 34925);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79917;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34925;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34925;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (79917, 34925);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=79917;

		#34437/Quest - The Prodigal Frostwolf

			DELETE FROM `creature_questrelation` WHERE `quest` = 34437;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34437;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34437;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (79917, 34437);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 79917;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34437;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34437;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78996, 34437);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78996;

		#35747/Quest - Taking a Trip to the Top of the Tank

			DELETE FROM `creature_questrelation` WHERE `quest` = 35747;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35747;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35747;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78563, 35747);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78563;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35747;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35747;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (80521, 35747);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=80521;

		#34445/Quest - a Taste of Iron

			DELETE FROM `creature_questrelation` WHERE `quest` = 34445;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34445;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34445;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (80521, 34445);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 80521;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34445;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34445;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (80521, 34445);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=80521;

		#35884/Quest - The Home Stretch

			DELETE FROM `creature_questrelation` WHERE `quest` = 35884;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 35884;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 35884;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (80521, 35884);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 80521;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 35884;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 35884;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78563, 35884);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78563;

		#34446/Quest - The Home Stretch

			DELETE FROM `creature_questrelation` WHERE `quest` = 34446;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34446;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34446;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (80521, 34446);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 80521;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34446;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34446;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78563, 34446);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78563;

		#34421/Quest - Bled Dry

			DELETE FROM `creature_questrelation` WHERE `quest` = 34421;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34421;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34421;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78573, 34421);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78573;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34421;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34421;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78559, 34421);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78559;

		#34427/Quest - A Potential Ally

			DELETE FROM `creature_questrelation` WHERE `quest` = 34427;
			DELETE FROM `gameobject_questrelation` WHERE `quest` = 34427;
			UPDATE `item_template` SET `StartQuest`=0 WHERE `StartQuest` = 34427;
			INSERT INTO `creature_questrelation` (`id`, `quest`) VALUES (78560, 34427);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry` = 78560;
			DELETE FROM `creature_involvedrelation` WHERE `quest` = 34427;
			DELETE FROM `gameobject_involvedrelation` WHERE `quest` = 34427;
			INSERT INTO `creature_involvedrelation` (`id`, `quest`) VALUES (78996, 34427);
			UPDATE `creature_template` SET `npcflag`=`npcflag`|2 WHERE `entry`=78996;
			
		-- Misc
		
			UPDATE quest_template_objective SET `index` = 0 WHERE id = 273635;
			UPDATE quest_template_objective SET flags = 0 WHERE questid = 34393;
			UPDATE `quest_template` SET `Flags` = 8 WHERE `Id` = 35241;
			UPDATE quest_template_objective SET flags = 0 WHERE questid = 34423;
			UPDATE `quest_template` SET `Method` = 0 WHERE `Id` = 34420;
			UPDATE `quest_template` SET `RequiredRaces` = 67108863 WHERE `Id` IN(34398,35933,34392,34393,34420,34422,34425,34429,34439,34442,34987,34958,34442,34925,34437,35747,34445);
			UPDATE `quest_template` SET `RequiredRaces` = 18875469 WHERE `Id` IN(35240,35242,34423,34478,34432,34431,34434,34436,35019,35884);
			UPDATE `quest_template` SET `RequiredRaces` = 33555378 WHERE `Id` IN(34421,35241,35005,34427,34739,34737,34740,34741,35005,34446);
			UPDATE creature_template SET scriptname = "npc_shattered_hand_brawler" WHERE entry = 82057;

			-- /!\ TP TO TANAAN FROM BLASTED LANDS
			UPDATE creature_template SET scriptname = "npc_archmage_khadgar_gossip" WHERE entry = 76643; -- /!\ TP TO TANAAN FROM BLASTED LANDS
			-- /!\ TP TO TANAAN FROM BLASTED LANDS
			
		-- Quests suit

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 35933 WHERE `Id` = 34398;
			UPDATE `quest_template` SET `PrevQuestId` = 34398, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 34392 WHERE `Id` = 35933;
			UPDATE `quest_template` SET `PrevQuestId` = 35933, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 34393 WHERE `Id` = 34392;
			UPDATE `quest_template` SET `PrevQuestId` = 34392, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 34420 WHERE `Id` = 34393;
			UPDATE `quest_template` SET `PrevQuestId` = 34393, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34420;

			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 34423, `ExclusiveGroup` = -34422, `NextQuestIdChain` = 0 WHERE `Id` = 34422;
			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 34423, `ExclusiveGroup` = -34422, `NextQuestIdChain` = 0 WHERE `Id` = 35240;
			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 34425, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34423;

			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 35005, `ExclusiveGroup` = -35241, `NextQuestIdChain` = 0 WHERE `Id` = 35241;
			UPDATE `quest_template` SET `PrevQuestId` = 34420, `NextQuestId` = 35005, `ExclusiveGroup` = -35241, `NextQuestIdChain` = 0 WHERE `Id` = 34421;
			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 34425, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35005;

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34425;

			UPDATE `quest_template` SET `PrevQuestId` = 34425, `NextQuestId` = 34429, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34478;

			UPDATE `quest_template` SET `PrevQuestId` = 34425, `NextQuestId` = 34429, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34427;

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34429;

			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34434, `ExclusiveGroup` = -34432, `NextQuestIdChain` = 0 WHERE `Id` = 34432;
			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34434, `ExclusiveGroup` = -34432, `NextQuestIdChain` = 0 WHERE `Id` = 34431;
			UPDATE `quest_template` SET `PrevQuestId` = 34432, `NextQuestId` = 34436, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34434;
			UPDATE `quest_template` SET `PrevQuestId` = 34434, `NextQuestId` = 35019, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34436;
			UPDATE `quest_template` SET `PrevQuestId` = 34436, `NextQuestId` = 34439, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35019;

			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34740, `ExclusiveGroup` = -34739, `NextQuestIdChain` = 0 WHERE `Id` = 34739;
			UPDATE `quest_template` SET `PrevQuestId` = 34429, `NextQuestId` = 34740, `ExclusiveGroup` = -34739, `NextQuestIdChain` = 0 WHERE `Id` = 34737;
			UPDATE `quest_template` SET `PrevQuestId` = 34739, `NextQuestId` = 34741, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34740;
			UPDATE `quest_template` SET `PrevQuestId` = 34434, `NextQuestId` = 35019, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34741;
			UPDATE `quest_template` SET `PrevQuestId` = 34741, `NextQuestId` = 34439, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35005;

			UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 34442, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34439;

			UPDATE `quest_template` SET `PrevQuestId` = 34439, `NextQuestId` = 34925, `ExclusiveGroup` = -34442, `NextQuestIdChain` = 0 WHERE `Id` = 34442;
			UPDATE `quest_template` SET `PrevQuestId` = 34439, `NextQuestId` = 34925, `ExclusiveGroup` = -34442, `NextQuestIdChain` = 0 WHERE `Id` = 34987;
			UPDATE `quest_template` SET `PrevQuestId` = 34439, `NextQuestId` = 34925, `ExclusiveGroup` = -34442, `NextQuestIdChain` = 0 WHERE `Id` = 34958;

			UPDATE `quest_template` SET `PrevQuestId` = 34442, `NextQuestId` = 35747, `ExclusiveGroup` = -34925, `NextQuestIdChain` = 0 WHERE `Id` = 34925;
			UPDATE `quest_template` SET `PrevQuestId` = 34442, `NextQuestId` = 35747, `ExclusiveGroup` = -34925, `NextQuestIdChain` = 0 WHERE `Id` = 34437;

			UPDATE `quest_template` SET `PrevQuestId` = 34925, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34445;

			UPDATE `quest_template` SET `PrevQuestId` = 34445, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 35884;

			UPDATE `quest_template` SET `PrevQuestId` = 34445, `NextQuestId` = 0, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34446;
			UPDATE `quest_template` SET `PrevQuestId` = 34740, `NextQuestId` = 35019, `ExclusiveGroup` = 0, `NextQuestIdChain` = 0 WHERE `Id` = 34741;

			UPDATE `quest_template` SET `ExclusiveGroup` = 0 WHERE `Id` = 34442; -- (before -34442)
			UPDATE `quest_template` SET `ExclusiveGroup` = 0 WHERE `Id` = 34925; -- (before -34925)
			UPDATE `quest_template` SET `ExclusiveGroup` = 0 WHERE `Id` = 34437; -- (before -34925)
			UPDATE `quest_template` SET `PrevQuestId` = 35747 WHERE `Id` = 34445;

		-- gob 233197 Kelidan the breaker door

			UPDATE gameobject SET state = 0 WHERE id = 233197 AND areaId = 7042;