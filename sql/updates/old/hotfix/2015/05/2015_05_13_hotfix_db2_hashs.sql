/*
SQLyog Ultimate - MySQL GUI v8.2 
MySQL - 5.6.17 : Database - 540_hotfix
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `_hashs` */

DROP TABLE IF EXISTS `_hashs`;

CREATE TABLE `_hashs` (
  `ID` int(11) unsigned NOT NULL,
  `Name` varchar(100) NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

/*Data for the table `_hashs` */

insert  into `_hashs`(`ID`,`Name`) values (35137211,'BroadcastText'),(48336690,'SpellVisualEffectName'),(149245792,'PathProperty'),(153052569,'Cfg_Configs'),(157446066,'AreaGroupMember'),(234224168,'GarrMission'),(261693969,'QuestV2CliTask'),(264699590,'GarrPlotUICategory'),(331613093,'GameObjects'),(366509520,'BattlePetSpeciesState'),(367806651,'UiTextureKit'),(438801950,'SpellRuneCost'),(440872980,'TextureFileData'),(458903206,'BattlePetBreedQuality'),(518419645,'GarrSpecialization'),(545961245,'SpellVisualKitAreaModel'),(583472526,'GarrSiteLevel'),(599752208,'SpellEffectGroupSize'),(634137177,'PhaseXPhaseGroup'),(666345498,'SpellXSpellVisual'),(680438657,'SpellClassOptions'),(690463935,'HolidayDescriptions'),(708957985,'GarrFollItemSetMember'),(752402434,'GarrPlot'),(779857838,'Heirloom'),(793851790,'CurrencyTypes'),(803663642,'GroupFinderCategory'),(844464874,'ItemXBonusTree'),(852966632,'GarrAbilityCategory'),(878382519,'GarrBuilding'),(896541910,'GroupFinderActivityGrp'),(930182777,'SpellMissileMotion'),(943410215,'MapChallengeMode'),(961296167,'Location'),(983446676,'DeviceBlacklist'),(1000230050,'PathNode'),(1012231747,'BattlePetAbilityState'),(1065724855,'Locale'),(1073915313,'ItemEffect'),(1107630156,'GarrFollowerQuality'),(1109793673,'ItemAppearance'),(1143173908,'BattlePetSpeciesXAbility'),(1165344527,'GarrClassSpec'),(1187330571,'GarrEncounterXMechanic'),(1240462528,'AreaPOIState'),(1272569722,'Curve'),(1282325123,'ObjectEffectGroup'),(1298280559,'CharacterFaceBoneSet'),(1326512502,'DriverBlacklist'),(1344507586,'Item'),(1356405368,'TaxiNodes'),(1369604944,'SpellVisualMissile'),(1377377290,'HighlightColor'),(1419319386,'HolidayNames'),(1440631488,'ItemToMountSpell'),(1477136115,'WbAccessControlList'),(1491544390,'SpellVisualColorEffect'),(1562431239,'LookAtController'),(1563357608,'ItemToBattlePet'),(1581158180,'ObjectEffectPackage'),(1581608254,'GarrFollItemSet'),(1627543382,'SpellCastingRequirements'),(1653915679,'GarrMissionXEncounter'),(1657342891,'ObjectEffectModifier'),(1672086436,'GarrMechanic'),(1672791226,'BattlePetEffectProperties'),(1707731536,'TrophyInstance'),(1734738485,'ObjectEffect'),(1754233351,'SpellMissile'),(1773314988,'GarrSiteLevelPlotInst'),(1794847238,'BattlePetBreedState'),(1821637041,'BattlePetSpecies'),(1837770388,'KeyChain'),(1840711788,'RulesetItemUpgrade'),(1870037451,'TrophyType'),(1876974313,'ItemCurrencyCost'),(1879459387,'ItemUpgrade'),(1880017466,'CurvePoint'),(1904564379,'ItemToBattlePetSpecies'),(1904724485,'LfgRoleRequirement'),(1919757118,'SpellMechanic'),(1924290945,'WindSettings'),(1930364469,'ModelNameToManifest'),(1939361897,'SpellEffectCameraShakes'),(1945261867,'GarrPlotBuilding'),(1972272076,'Holidays'),(2063803484,'Toy'),(2124448848,'SpellPowerDifficulty'),(2143331989,'SpellMiscDifficulty'),(2199425034,'ItemBonus'),(2226175447,'SpellActionBarPref'),(2240070130,'GarrMissionReward'),(2287306173,'WbCertWhitelist'),(2303540067,'UiTextureAtlas'),(2361489494,'QuestLineXQuest'),(2385530453,'GlyphRequiredSpec'),(2403627824,'BattlePetState'),(2412384869,'QuestPOIPointCliTask'),(2437774747,'GarrFollowerXAbility'),(2442913102,'ItemSparse'),(2494151198,'GarrBuildingPlotInst'),(2499044245,'Path'),(2524150337,'Mount'),(2583026706,'ChrUpgradeTier'),(2663146609,'GarrBuildingDoodadSet'),(2665962237,'QuestObjectiveCliTask'),(2712461791,'SpellPower'),(2715021741,'MarketingPromotionsXLocale'),(2753361231,'GarrAbilityEffect'),(2753853165,'WeaponTrail'),(2769259057,'SpellTotems'),(2793276977,'ItemBonusTreeNode'),(2807278738,'GarrEncounter'),(2810085133,'Cfg_Regions'),(2810793959,'CharShipmentContainer'),(2832751941,'GarrFollower'),(2837408936,'GarrMechanicType'),(2872091740,'TaxiPath'),(2875640223,'SpellReagents'),(2889024414,'SoundEntriesAdvanced'),(2939349194,'GroupFinderActivity'),(2956113239,'CharShipment'),(3039853142,'UiTextureAtlasMember'),(3108775943,'PlayerCondition'),(3130494798,'SpellAuraRestrictions'),(3146089301,'ItemExtendedCost'),(3282955075,'BattlePetVisual'),(3322146344,'SpellMisc'),(3324329676,'ModelManifest'),(3325606100,'SoundEmitters'),(3326241424,'ChrUpgradeBucketSpell'),(3354229117,'Cfg_Categories'),(3386291891,'Creature'),(3386943305,'CreatureDifficulty'),(3396722460,'OverrideSpellData'),(3408032706,'ObjectEffectPackageElem'),(3416538071,'BattlePetAbility'),(3425666288,'QuestPackageItem'),(3450573023,'WbCertBlacklist'),(3490179113,'DeviceDefaultSettings'),(3568395212,'SceneScript'),(3634485004,'SoundEmitterPillPoints'),(3638153752,'GarrMissionType'),(3689805315,'SpellLearnSpell'),(3716901134,'BattlePetAbilityEffect'),(3720095159,'ChrUpgradeBucket'),(3727959107,'GarrFollowerLevelXP'),(3745449290,'AreaPOI'),(3777145859,'GlyphExclusiveCategory'),(3830298396,'SceneScriptPackageMember'),(3832747704,'GarrAbility'),(3834752085,'ItemModifiedAppearance'),(3857287356,'TaxiPathNode'),(3897115516,'LanguageWords'),(3905641993,'SceneScriptPackage'),(3948389411,'QuestLine'),(3973639388,'BattlePetAbilityTurn'),(3978279757,'RulesetRaidLootUpgrade'),(4021368146,'Vignette'),(4033975491,'SpellVisualKitModelAttach'),(4047189852,'Trophy'),(4060454394,'BattlePetNPCTeamMember'),(4093144214,'GarrPlotInstance'),(4095124812,'WorldBossLockout'),(4102286043,'SpellVisualKit'),(4141571979,'AreaGroup'),(4146370265,'SpellVisual'),(4163366139,'WbPermissions'),(4187421273,'PvpItem'),(4192035179,'SpellAuraRestrictionsDifficulty'),(4200993301,'SoundEntries'),(4263624740,'PathNodeProperty');

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
