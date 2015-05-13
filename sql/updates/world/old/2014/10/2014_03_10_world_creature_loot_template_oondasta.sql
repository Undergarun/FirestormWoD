SET @REF_OONDASTA = 69161;
DELETE FROM `reference_loot_template` WHERE `entry` = @REF_OONDASTA;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_OONDASTA, 94228, 0, 1, 1, 1, 1) /* 3Reins of the Cobalt Primordial Direhorn */,
(@REF_OONDASTA, 95147, 0, 1, 1, 1, 1) /* 3Fancifully Frilled Tunic */,
(@REF_OONDASTA, 95148, 0, 1, 1, 1, 1) /* 3Vest of the Bordomorono */,
(@REF_OONDASTA, 95149, 0, 1, 1, 1, 1) /* 3Gorgoraptor Scale Chest */,
(@REF_OONDASTA, 95150, 0, 1, 1, 1, 1) /* 3Mail of the Mosschopper */,
(@REF_OONDASTA, 95151, 0, 1, 1, 1, 1) /* 3Scorched Spiritfire Drape */,
(@REF_OONDASTA, 95152, 0, 1, 1, 1, 1) /* 3Breastplate of the Iguanocolossus */,
(@REF_OONDASTA, 95153, 0, 1, 1, 1, 1) /* 3Tyrant King Battleplate */,
(@REF_OONDASTA, 95163, 0, 1, 1, 1, 1) /* 3Ring of Shamuru */,
(@REF_OONDASTA, 95164, 0, 1, 1, 1, 1) /* 3Eye of Oondasta */,
(@REF_OONDASTA, 95165, 0, 1, 1, 1, 1) /* 3Achillobator Ring */,
(@REF_OONDASTA, 95166, 0, 1, 1, 1, 1) /* 3Forzarra's Last Meal */,
(@REF_OONDASTA, 95167, 0, 1, 1, 1, 1) /* 3Ring of King Kangrom */,
(@REF_OONDASTA, 95177, 0, 1, 1, 1, 1) /* 3Choker of Stygimolochy */,
(@REF_OONDASTA, 95178, 0, 1, 1, 1, 1) /* 3Lootraptor's Amulet */,
(@REF_OONDASTA, 95179, 0, 1, 1, 1, 1) /* 3Necklace of the Hazillosaurus */,
(@REF_OONDASTA, 95180, 0, 1, 1, 1, 1) /* 3Overcompensating Chain of the Alpha Male */,
(@REF_OONDASTA, 95181, 0, 1, 1, 1, 1) /* 3Amulet of the Titanorex */,
(@REF_OONDASTA, 95182, 0, 1, 1, 1, 1) /* 3Robes of Zalmoxes */,
(@REF_OONDASTA, 95183, 0, 1, 1, 1, 1) /* 3Waistband of Elder Falcarius */,
(@REF_OONDASTA, 95184, 0, 1, 1, 1, 1) /* 3Girdle of Dimorphodontics */,
(@REF_OONDASTA, 95185, 0, 1, 1, 1, 1) /* 3Terrorful Weave */,
(@REF_OONDASTA, 95186, 0, 1, 1, 1, 1) /* 3Belt of Crushed Dreams */,
(@REF_OONDASTA, 95187, 0, 1, 1, 1, 1) /* 3Girdle of the Derrodoccus */,
(@REF_OONDASTA, 95188, 0, 1, 1, 1, 1) /* 3Belt of the Arch Avimimus */,
(@REF_OONDASTA, 95189, 0, 1, 1, 1, 1) /* 3Belt of the Dying Diemetradon */,
(@REF_OONDASTA, 95190, 0, 1, 1, 1, 1) /* 3Waistband of Furious Stomping */,
(@REF_OONDASTA, 95191, 0, 1, 1, 1, 1) /* 3Voolar's Bloodied Belt */,
(@REF_OONDASTA, 95192, 0, 1, 1, 1, 1) /* 3Belt of the Tyrannotitan */,
(@REF_OONDASTA, 95193, 0, 1, 1, 1, 1) /* 3Gloves of Varsoon the Greater */,
(@REF_OONDASTA, 95194, 0, 1, 1, 1, 1) /* 3Gloves of Tyranomancy */,
(@REF_OONDASTA, 95195, 0, 1, 1, 1, 1) /* 3Gloves of Unliving Fossil */,
(@REF_OONDASTA, 95196, 0, 1, 1, 1, 1) /* 3Gloves of Gastric Rumbling */,
(@REF_OONDASTA, 95197, 0, 1, 1, 1, 1) /* 3Therapsid Scale Gloves */,
(@REF_OONDASTA, 95198, 0, 1, 1, 1, 1) /* 3Vulcanodon Gauntlets */,
(@REF_OONDASTA, 95199, 0, 1, 1, 1, 1) /* 3Carnotaur Battlegloves */,
(@REF_OONDASTA, 95200, 0, 1, 1, 1, 1) /* 3Orndo Mando's Gloves */,
(@REF_OONDASTA, 95201, 0, 1, 1, 1, 1) /* 3Skullsmashing Gauntlets */,
(@REF_OONDASTA, 95601, 0, 1, 1, 1, 1) /* 3Shiny Pile of Refuse */;

UPDATE `creature_template` SET `lootid` = @REF_OONDASTA WHERE `entry` = @REF_OONDASTA;
DELETE FROM `creature_loot_template` WHERE `entry` = @REF_OONDASTA;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@REF_OONDASTA, 1, 100, 1, 0, -@REF_OONDASTA, 5) /* Ref 1*/;