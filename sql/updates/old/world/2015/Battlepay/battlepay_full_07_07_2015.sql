/*
SQLyog Ultimate v9.02 
MySQL - 5.6.19-log : Database - ashran_world
*********************************************************************
*/

/*!40101 SET NAMES utf8 */;

/*!40101 SET SQL_MODE=''*/;

/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;
/*Table structure for table `battlepay_display_info` */

DROP TABLE IF EXISTS `battlepay_display_info`;

CREATE TABLE `battlepay_display_info` (
  `DisplayInfoId` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `CreatureDisplayInfoID` int(11) unsigned NOT NULL DEFAULT '0',
  `FileDataID` int(11) unsigned DEFAULT NULL,
  `Name1` varchar(255) NOT NULL,
  `Name2` varchar(255) NOT NULL,
  `Name3` varchar(255) NOT NULL,
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  UNIQUE KEY `DisplayInfoId` (`DisplayInfoId`)
) ENGINE=InnoDB AUTO_INCREMENT=97 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_display_info` */

insert  into `battlepay_display_info`(`DisplayInfoId`,`CreatureDisplayInfoID`,`FileDataID`,`Name1`,`Name2`,`Name3`,`Flags`) values (1,51482,0,'Reins of the Kor\'kron War Wolf','','The Kor\'kron use only the mightiest wolves for their mounts, bedecking them in armor that would break the backs of lesser creatures.',0),(2,15676,0,'Black Qiraji Resonating Crystal','','Although many varieties of Qiraji Battle Tank can still be found in the ruins of Ahn\'Qiraj today, the darkest of the species were only seen on the day the Scarab gong was rung.',0),(3,39530,0,'Tyrael\'s Charger','','A symbol of justice throughout all known realms.',0),(4,28040,0,'Reins of the Black Proto-Drake','','Bred specifically to honor those heroes who showed exceptional resilience and fortitude facing the dangers in Northrend.',0),(5,31958,0,'Celestial Steed','','A beacon of hope; a guiding light in the darkness.',0),(6,38260,0,'Winged Guardian','','Unearthed recently in an ancient tomb, this mighty winged lion may have once served one of the ancient Tol\'vir princes.',0),(7,40029,0,'Heart of the Aspects','','You are Azeroth\'s true guardians and the future of this world is in your hands, for the dawning of the age of mortals has begun. - Alexstrasza',0),(8,46729,0,'Swift Windsteed','','One of Pandaria\'s most elusive creatures, said to bring good fortune to its rider.',0),(9,48020,0,'Armored Bloodwing','','This monstrous, flesh-eating bat is the undisputed ruler of the night skies.',0),(10,48714,0,'Enchanted Fey Dragon','','This majestic creature is Brightwing\'s older brother. He affirms that she was, in fact, adopted.',0),(11,53038,0,'Iron Skyreaver','','Iron Horde beastmasters bred this rylak for war and then girded it for battle in black steel and smoldering flame.',0),(12,55896,0,'Warforged Nightmare','','A nightmarish steed of hardened steel and vengeful spirits, this companion will follow you to the depths of the Firelands and back without complaint.',0),(13,55907,0,'Grinning Reaver','','The difference between a smile and a grin is the difference between a kiss and tasting.',0),(14,53774,0,'Reins of the Dread Raven','','Filled with the hatred of an ancient god, the dread raven are the Arakkoa\'s most trusted guardians.',0),(15,21973,0,'Reins of the Spectral Tiger','','In true goblin fashion, Landro Longshot has put a stranglehold on the distribution of these mysterious incorporeal mounts.',0),(16,31992,0,'X-53 Touring Rocket','','Riding a giant cylinder filled with highly explosive goblin rocket fuel all by yourself is just crazy...so bring a friend.',0),(17,61484,0,'Armored Razorback','','Never... ever... EVER... let it smell bacon on you.',0),(18,53307,0,'Tundra Icehoof','','A translucent layer of down hair covers this breed\'s leathery hide, acting as the perfect insulation for cold environments.',0),(19,59746,0,'Mudback Riverbeast','','Special webbing that grows on the soles of the mudback\'s feet gives them extra traction in mud and slick terrain.',0),(20,59536,0,'Warsong Direfang','','Has a will of iron but is a sweetheart if you give it a clefthoof bone to chew on.',0),(21,48931,0,'Hearthsteed','','Each step echoes with the rage of your three opponents.',0),(22,42498,0,'Heart of the Nightwing','','When you can\'t run, you crawl, and when you can\'t crawl you find someone to carry you.',0),(23,45521,0,'Reins of the Ashen Pandaren Phoenix','','Both the name and color of this majestic phoenix are believed to symbolize rebirth.',0),(24,58772,0,'Core Hound Chain','','Molten to the core!',0),(25,62148,0,'Mystic Runesaber','','Infused with arcane power, the Mystic Runesaber has long been hidden by chaotic nether energies. Recently they have emerged from the shadows in search of ley lines to sate their thirst for magic.',0),(26,38046,0,'Corrupted Egg of Millagazor','','Millagazor was a fire hawk loyal to Ragnaros. At this time it is unknown how many of Millagazor\'s eggs were corrupted.',0),(27,61363,0,'Cindermane Charger','','Formed by Ragnaros himself as a gift for the daughter he never had.',0),(28,39546,0,'Spectral Gryphon','','When you said an old friend was coming back, this was not quite what I expected. -Bralla Cloudwing',0),(29,48934,0,'Blossoming Ancient','','',0),(30,16943,0,'Cenarion Hatchling','','',0),(31,46720,0,'Cinder Kitten','','',0),(32,30412,0,'Gryphon Hatchling','','',0),(33,30507,0,'Lil\' Phylactery','','',0),(34,37541,0,'Lil\' Ragnaros','','',0),(35,32031,0,'Lil\' XT','','',0),(36,42299,0,'Moonkin Egg','','',0),(37,30414,0,'Pandaren Monk','','',0),(38,40019,0,'Soul of the Aspects','','',0),(39,30413,0,'Wind Rider Cub','','',0),(40,25900,0,'Tyrael\'s Hilt','','',0),(41,10990,0,'Panda Collar','','',0),(42,22966,0,'Dragon Kite','','',0),(43,16205,0,'Elwynn Lamb','','',0),(44,45960,0,'Enchanted Broom','','',0),(45,25002,0,'Soul-Trader Beacon','','',0),(46,21362,0,'Banana Charm','','',0),(47,30409,0,'Spectral Tiger Cub','','',0),(48,4732,0,'Tirisfal Batling','','',0),(49,4185,0,'Vampiric Batling','','',0),(50,30462,0,'Core Hound Pup','','',0),(51,17723,0,'Netherwhelp\'s Collar','','',0),(52,28456,0,'Frosty\'s Collar','','',0),(53,31722,0,'Ice Chip','','',0),(54,15398,0,'Lurky\'s Egg','','',0),(55,15369,0,'Blue Murloc Egg','','',0),(56,29348,0,'Heavy Murloc Egg','','',0),(57,15984,0,'Pink Murloc Egg','','',0),(58,28734,0,'Murkimus\' Little Spear','','',0),(59,22938,0,'Silver Pig Coin','','',0),(60,22389,0,'Muckbreath\'s Bucket','','',0),(61,23506,0,'Chuck\'s Bucket','','',0),(62,23507,0,'Snarly\'s Bucket','','',0),(63,24620,0,'Competitor\'s Souvenir','','',0),(64,28502,0,'Mulgore Hatchling','','',0),(65,26452,0,'Unhatched Mr. Chilly','','',0),(66,16189,0,'Baby Blizzard Bear','','',0),(67,38359,0,'Guardian Cub','','',0),(68,37846,0,'Nightsaber Cub','','',0),(69,35338,0,'Lil\' Deathwing','','',0),(70,0,929921,'Deathweb Spaulders','','',0),(71,0,1033987,'Sésame - Niveau 100','','Propulsez votre personnage au niveau 100 !',0),(72,0,614740,'Sésame - Niveau 90','','Propulsez votre personnage au niveau 90 !',0),(73,0,614740,'Changement de nom','','Renommez votre personnage !',0),(74,0,614740,'Changement de faction','','Changez la faction de votre personnage !',0),(75,0,614740,'Changement de race','','Changez la race de votre personnage !',0),(76,0,614740,'Changement d\'apparence','','Changez l\'apparence de votre personnage !',0),(77,0,1033988,'Personnage pré-équipé niveau 100','','Créez un personnage niveau 100 de la classe de votre choix avec un équipement complet niveau 610 !',0),(78,0,133789,'10.000 pièces d\'or','','Achetez 10.000 pièces d\'or !',0),(79,0,133787,'50.000 pièces d\'or','','Achetez 50.000 pièces d\'or !',0),(80,0,133784,'200.000 pièces d\'or','','Achetez 200.000 pièces d\'or !',0),(81,0,133785,'500.000 pièces d\'or','','Achetez 500.000 pièces d\'or !',0),(82,0,136240,'Alchimie - 700','','Apprenez l\'alchimie niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(83,0,441139,'Archéologie - 700','','Apprenez l\'archéologie niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(84,0,136241,'Forge - 700','','Apprenez la forge niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(85,0,571695,'Cuisine - 700','','Apprenez la cuisine niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(86,0,136244,'Enchantement - 700','','Apprenez l\'enchantement niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(87,0,136243,'Ingénierie - 700','','Apprenez l\'ingénierie niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(88,0,1014021,'Secourisme - 700','','Apprenez le secourisme niveau 700/700 avec toutes les recettes jusqu\'a Mist of Pandaria.',0),(89,0,136245,'Pêche - 700','','Apprenez la pêche niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(90,0,136246,'Herboriste - 700','','Apprenez l\'herboristerie niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(91,0,237171,'Calligraphie - 700','','Apprenez la calligraphie niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(92,0,134071,'Joaillerie - 700','','Apprenez la joaillerie niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(93,0,134366,'Dépeçage - 700','','Apprenez le dépeçage niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(94,0,136249,'Couture - 700','','Apprenez la couture niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(95,0,136248,'Minage - 700','','Apprenez le minage niveau 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0),(96,0,136247,'Travail du cuir - 700','','Apprenez le travail du cuir 700/700 avec toutes les recette jusqu\'a Mist of Pandaria.',0);

/*Table structure for table `battlepay_product` */

DROP TABLE IF EXISTS `battlepay_product`;

CREATE TABLE `battlepay_product` (
  `ProductID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `NormalPriceFixedPoint` bigint(20) unsigned NOT NULL,
  `CurrentPriceFixedPoint` bigint(20) unsigned NOT NULL,
  `Type` tinyint(3) unsigned NOT NULL,
  `ChoiceType` tinyint(3) unsigned NOT NULL,
  `Flags` int(11) unsigned NOT NULL,
  `DisplayInfoID` int(11) unsigned NOT NULL,
  `ScriptName` text NOT NULL,
  PRIMARY KEY (`ProductID`)
) ENGINE=InnoDB AUTO_INCREMENT=97 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_product` */

insert  into `battlepay_product`(`ProductID`,`NormalPriceFixedPoint`,`CurrentPriceFixedPoint`,`Type`,`ChoiceType`,`Flags`,`DisplayInfoID`,`ScriptName`) values (1,125,125,0,2,47,1,''),(2,125,125,0,2,47,2,''),(3,125,125,0,2,47,3,''),(4,125,125,0,2,47,4,''),(5,125,125,0,2,47,5,''),(6,125,125,0,2,47,6,''),(7,125,125,0,2,47,7,''),(8,100,100,0,2,47,8,''),(9,125,125,0,2,47,9,''),(10,125,125,0,2,47,10,''),(11,125,125,0,2,47,11,''),(12,150,150,0,2,47,12,''),(13,150,150,0,2,47,13,''),(14,125,125,0,2,47,14,''),(15,100,100,0,2,47,15,''),(16,150,150,0,2,47,16,''),(17,125,125,0,2,47,17,''),(18,125,125,0,2,47,18,''),(19,125,125,0,2,47,19,''),(20,125,125,0,2,47,20,''),(21,125,125,0,2,47,21,''),(22,125,125,0,2,47,22,''),(23,150,150,0,2,47,23,''),(24,135,150,0,2,47,24,''),(25,160,250,0,2,47,25,''),(26,130,130,0,2,47,26,''),(27,125,125,0,2,47,27,''),(28,100,100,0,2,47,28,''),(29,75,75,0,2,47,29,''),(30,75,75,0,2,47,30,''),(31,75,75,0,2,47,31,''),(32,75,75,0,2,47,32,''),(33,100,100,0,2,47,33,''),(34,100,100,0,2,47,34,''),(35,100,100,0,2,47,35,''),(36,85,85,0,2,47,36,''),(37,75,75,0,2,47,37,''),(38,90,90,0,2,47,38,''),(39,75,75,0,2,47,39,''),(40,250,250,0,2,47,40,''),(41,250,250,0,2,47,41,''),(42,100,100,0,2,47,42,''),(43,75,75,0,2,47,43,''),(44,75,75,0,2,47,44,''),(45,75,75,0,2,47,45,''),(46,75,75,0,2,47,46,''),(47,90,90,0,2,47,47,''),(48,75,75,0,2,47,48,''),(49,75,75,0,2,47,49,''),(50,100,100,0,2,47,50,''),(51,75,75,0,2,47,51,''),(52,85,85,0,2,47,52,''),(53,75,75,0,2,47,53,''),(54,75,75,0,2,47,54,''),(55,75,75,0,2,47,55,''),(56,75,75,0,2,47,56,''),(57,75,75,0,2,47,57,''),(58,75,75,0,2,47,58,''),(59,75,75,0,2,47,59,''),(60,75,75,0,2,47,60,''),(61,75,75,0,2,47,61,''),(62,75,75,0,2,47,62,''),(63,90,90,0,2,47,63,''),(64,75,75,0,2,47,64,''),(65,75,75,0,2,47,65,''),(66,75,75,0,2,47,66,''),(67,75,75,0,2,47,67,''),(68,90,90,0,2,47,68,''),(69,100,100,0,2,47,69,''),(70,100,100,0,2,4748,70,''),(71,150,120,0,2,47,71,'battlepay_service_level100'),(72,100,75,0,2,47,72,'battlepay_service_level90'),(73,50,45,0,2,47,73,'battlepay_service_rename'),(74,85,70,0,2,47,74,'battlepay_service_change_faction'),(75,55,40,0,2,47,75,'battlepay_service_change_race'),(76,50,35,0,2,47,76,'battlepay_service_customize'),(77,450,350,0,2,47,77,'battlepay_service_premade'),(78,50,35,0,2,47,78,'battlepay_gold_10k'),(79,100,80,0,2,47,79,'battlepay_gold_50k'),(80,250,190,0,2,47,80,'battlepay_gold_200k'),(81,400,290,0,2,47,81,'battlepay_gold_500k'),(82,200,175,0,2,47,82,'battlepay_profession_alchemy'),(83,200,175,0,2,47,83,'battlepay_profession_archaeology'),(84,200,175,0,2,47,84,'battlepay_profession_blacksmithing'),(85,200,175,0,2,47,85,'battlepay_profession_cooking'),(86,200,175,0,2,47,86,'battlepay_profession_enchanting'),(87,200,175,0,2,47,87,'battlepay_profession_engineering'),(88,200,175,0,2,47,88,'battlepay_profession_first_aid'),(89,200,175,0,2,47,89,'battlepay_profession_fishing'),(90,200,175,0,2,47,90,'battlepay_profession_herbalism'),(91,200,175,0,2,47,91,'battlepay_profession_inscription'),(92,200,175,0,2,47,92,'battlepay_profession_jewelcrafting'),(93,200,175,0,2,47,93,'battlepay_profession_skinning'),(94,200,175,0,2,47,94,'battlepay_profession_tailoring'),(95,200,175,0,2,47,95,'battlepay_profession_mining'),(96,200,175,0,2,47,96,'battlepay_profession_leatherworking');

/*Table structure for table `battlepay_product_group` */

DROP TABLE IF EXISTS `battlepay_product_group`;

CREATE TABLE `battlepay_product_group` (
  `GroupID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `Name` varchar(255) NOT NULL,
  `IconFileDataID` int(11) NOT NULL,
  `DisplayType` tinyint(3) unsigned NOT NULL,
  `Ordering` int(11) NOT NULL,
  PRIMARY KEY (`GroupID`)
) ENGINE=InnoDB AUTO_INCREMENT=7 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_product_group` */

insert  into `battlepay_product_group`(`GroupID`,`Name`,`IconFileDataID`,`DisplayType`,`Ordering`) values (1,'Mount',939379,0,1),(2,'Battle Pets',939380,0,2),(3,'Services',939382,0,3),(4,'Golds',940857,0,4),(5,'Professions',940858,0,5),(6,'Toys',940867,0,6);

/*Table structure for table `battlepay_product_item` */

DROP TABLE IF EXISTS `battlepay_product_item`;

CREATE TABLE `battlepay_product_item` (
  `ID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `ProductID` int(11) unsigned NOT NULL,
  `ItemID` int(11) unsigned NOT NULL,
  `Quantity` int(11) unsigned NOT NULL,
  `DisplayID` int(11) DEFAULT NULL,
  `PetResult` tinyint(2) unsigned NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=71 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_product_item` */

insert  into `battlepay_product_item`(`ID`,`ProductID`,`ItemID`,`Quantity`,`DisplayID`,`PetResult`) values (1,1,104246,1,0,0),(2,2,21176,1,0,0),(3,3,76755,1,0,0),(4,4,44164,1,0,0),(5,5,54811,1,0,0),(6,6,69846,1,0,0),(7,7,78924,1,0,0),(8,8,92724,1,0,0),(9,9,95341,1,0,0),(10,10,97989,1,0,0),(11,11,107951,1,0,0),(12,12,112326,1,0,0),(13,13,112327,1,0,0),(14,14,109013,1,0,0),(15,15,49283,1,0,0),(16,16,54860,1,0,0),(17,17,116669,1,0,0),(18,18,116658,1,0,0),(19,19,116769,1,0,0),(20,20,116780,1,0,0),(21,21,98618,1,0,0),(22,22,83086,1,0,0),(23,23,90710,1,0,0),(24,24,115484,1,0,0),(25,25,122469,1,0,0),(26,26,69230,1,0,0),(27,27,118515,1,0,0),(28,28,76889,1,0,0),(29,29,98550,1,0,0),(30,30,70099,1,0,0),(31,31,92707,1,0,0),(32,32,49662,1,0,0),(33,33,49693,1,0,0),(34,34,68385,1,0,0),(35,35,54847,1,0,0),(36,36,68384,1,0,0),(37,37,49665,1,0,0),(38,38,78916,1,0,0),(39,39,49663,1,0,0),(40,40,39656,1,0,0),(41,41,13583,1,0,0),(42,42,34493,1,0,0),(43,43,44974,1,0,0),(44,44,44982,1,0,0),(45,45,38050,1,0,0),(46,46,32588,1,0,0),(47,47,49343,1,0,0),(48,48,44971,1,0,0),(49,49,38658,1,0,0),(50,50,49646,1,0,0),(51,51,25535,1,0,0),(52,52,39286,1,0,0),(53,53,53641,1,0,0),(54,54,30360,1,0,0),(55,55,20371,1,0,0),(56,56,46802,1,0,0),(57,57,22114,1,0,0),(58,58,45180,1,0,0),(59,59,34519,1,0,0),(60,60,33818,1,0,0),(61,61,35350,1,0,0),(62,62,35349,1,0,0),(63,63,37298,1,0,0),(64,64,44980,1,0,0),(65,65,41133,1,0,0),(66,66,44819,1,0,0),(67,67,72068,1,0,0),(68,68,68841,1,0,0),(69,69,62540,1,0,0),(70,70,106586,1,0,0);

/*Table structure for table `battlepay_shop_entry` */

DROP TABLE IF EXISTS `battlepay_shop_entry`;

CREATE TABLE `battlepay_shop_entry` (
  `EntryID` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `GroupID` int(11) unsigned NOT NULL,
  `ProductID` int(11) unsigned NOT NULL,
  `Ordering` int(11) NOT NULL,
  `Flags` int(11) unsigned NOT NULL DEFAULT '0',
  `BannerType` tinyint(3) unsigned NOT NULL,
  `DisplayInfoID` int(11) unsigned NOT NULL,
  PRIMARY KEY (`EntryID`)
) ENGINE=InnoDB AUTO_INCREMENT=97 DEFAULT CHARSET=utf8;

/*Data for the table `battlepay_shop_entry` */

insert  into `battlepay_shop_entry`(`EntryID`,`GroupID`,`ProductID`,`Ordering`,`Flags`,`BannerType`,`DisplayInfoID`) values (1,1,1,1,0,0,0),(2,1,2,2,0,0,0),(3,1,3,3,0,0,0),(4,1,4,4,0,0,0),(5,1,5,5,0,0,0),(6,1,6,6,0,0,0),(7,1,7,7,0,0,0),(8,1,8,8,0,0,0),(9,1,9,9,0,0,0),(10,1,10,10,0,0,0),(11,1,11,11,0,0,0),(12,1,12,12,0,0,0),(13,1,13,13,0,0,0),(14,1,14,14,0,0,0),(15,1,15,15,0,0,0),(16,1,16,16,0,0,0),(17,1,17,17,0,0,0),(18,1,18,18,0,0,0),(19,1,19,19,0,0,0),(20,1,20,20,0,0,0),(21,1,21,21,0,0,0),(22,1,22,22,0,0,0),(23,1,23,23,0,0,0),(24,1,24,24,0,0,0),(25,1,25,25,0,0,0),(26,1,26,26,0,0,0),(27,1,27,27,0,0,0),(28,1,28,28,0,0,0),(29,2,29,1,0,0,0),(30,2,30,2,0,0,0),(31,2,31,3,0,0,0),(32,2,32,4,0,0,0),(33,2,33,5,0,0,0),(34,2,34,6,0,0,0),(35,2,35,7,0,0,0),(36,2,36,8,0,0,0),(37,2,37,9,0,0,0),(38,2,38,10,0,0,0),(39,2,39,11,0,0,0),(40,2,40,12,0,0,0),(41,2,41,13,0,0,0),(42,2,42,14,0,0,0),(43,2,43,15,0,0,0),(44,2,44,16,0,0,0),(45,2,45,17,0,0,0),(46,2,46,18,0,0,0),(47,2,47,19,0,0,0),(48,2,48,20,0,0,0),(49,2,49,21,0,0,0),(50,2,50,22,0,0,0),(51,2,51,23,0,0,0),(52,2,52,24,0,0,0),(53,2,53,25,0,0,0),(54,2,54,26,0,0,0),(55,2,55,27,0,0,0),(56,2,56,28,0,0,0),(57,2,57,29,0,0,0),(58,2,58,30,0,0,0),(59,2,59,31,0,0,0),(60,2,60,32,0,0,0),(61,2,61,33,0,0,0),(62,2,62,34,0,0,0),(63,2,63,35,0,0,0),(64,2,64,36,0,0,0),(65,2,65,37,0,0,0),(66,2,66,38,0,0,0),(67,2,67,39,0,0,0),(68,2,68,40,0,0,0),(69,2,69,41,0,0,0),(70,2,70,42,0,0,0),(71,3,71,1,0,0,0),(72,3,72,2,0,0,0),(73,3,73,3,0,0,0),(74,3,74,4,0,0,0),(75,3,75,5,0,0,0),(76,3,76,6,0,0,0),(77,3,77,7,0,0,0),(78,4,78,1,0,0,0),(79,4,79,2,0,0,0),(80,4,80,3,0,0,0),(81,4,81,4,0,0,0),(82,5,82,1,0,0,0),(83,5,83,2,0,0,0),(84,5,84,3,0,0,0),(85,5,85,4,0,0,0),(86,5,86,5,0,0,0),(87,5,87,6,0,0,0),(88,5,88,7,0,0,0),(89,5,89,8,0,0,0),(90,5,90,9,0,0,0),(91,5,91,10,0,0,0),(92,5,92,11,0,0,0),(93,5,93,12,0,0,0),(94,5,94,13,0,0,0),(95,5,95,14,0,0,0),(96,5,96,15,0,0,0);

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;
