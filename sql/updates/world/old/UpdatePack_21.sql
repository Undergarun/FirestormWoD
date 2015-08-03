/*

    Ajout des loot sur les créatures : 
        [npc=80782]
        [npc=80241]
        [npc=78314]
        [npc=82202]
        [npc=76905]
        [npc=74254]
    Ajustement des loot sur :
        [npc= 79205]
    Ajout du gain de réputation sur les créatures : 
        [npc=74254]
        [npc=87356]
        [npc=87343]
        [npc=87333]
        [npc=72348]
        [npc=76905]
        [npc=72348]
        [npc=87357]
    Ajout des créatures : 
        [npc=71665]
        [npc=88569]
        [npc=76918]
        [npc=78867] Patch de Latra

*/




-- #8554
-- Icespine Stinger
UPDATE `creature_template` SET `lootid`=80782 WHERE `entry`=80782;
DELETE FROM `creature_loot_template` WHERE `entry`=80782;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(80782, 106390, 0.01, 1, 0, 1, 1) /* Bladespire Breastplate */, 
(80782, 106391, 0.02, 1, 0, 1, 1) /* Bladespire Gauntlets */, 
(80782, 106392, 0.02, 1, 0, 1, 1) /* Bladespire Girdle */, 
(80782, 106393, 0.02, 1, 0, 1, 1) /* Bladespire Helm */, 
(80782, 106394, 0.02, 1, 0, 1, 1) /* Bladespire Legplates */, 
(80782, 106395, 0.02, 1, 0, 1, 1) /* Bladespire Pauldrons */, 
(80782, 106396, 0.02, 1, 0, 1, 1) /* Bladespire Sabatons */, 
(80782, 106397, 0.02, 1, 0, 1, 1) /* Frostbitten Shield */, 
(80782, 106398, 0.02, 1, 0, 1, 1) /* Bladespire Vambraces */, 
(80782, 106399, 0.03, 1, 0, 1, 1) /* Coldsnout Bindings */, 
(80782, 106400, 0.02, 1, 0, 1, 1) /* Coldsnout Boots */, 
(80782, 106401, 0.01, 1, 0, 1, 1) /* Coldsnout Britches */, 
(80782, 106402, 0.02, 1, 0, 1, 1) /* Coldsnout Gloves */, 
(80782, 106403, 0.01, 1, 0, 1, 1) /* Coldsnout Hood */, 
(80782, 106404, 0.01, 1, 0, 1, 1) /* Coldsnout Jerkin */, 
(80782, 106405, 0.02, 1, 0, 1, 1) /* Coldsnout Shoulders */, 
(80782, 106406, 0.02, 1, 0, 1, 1) /* Coldsnout Waistband */, 
(80782, 106407, 0.01, 1, 0, 1, 1) /* Lunarglow Cord */, 
(80782, 106408, 0.02, 1, 0, 1, 1) /* Lunarglow Cowl */, 
(80782, 106409, 0.02, 1, 0, 1, 1) /* Lunarglow Handwraps */, 
(80782, 106410, 0.02, 1, 0, 1, 1) /* Lunarglow Leggings */, 
(80782, 106411, 0.02, 1, 0, 1, 1) /* Lunarglow Robe */, 
(80782, 106412, 0.02, 1, 0, 1, 1) /* Lunarglow Sandals */, 
(80782, 106413, 0.02, 1, 0, 1, 1) /* Lunarglow Shoulderpads */, 
(80782, 106414, 0.01, 1, 0, 1, 1) /* Lunarglow Wristwraps */, 
(80782, 106415, 0.02, 1, 0, 1, 1) /* Frostlink Belt */, 
(80782, 106416, 0.02, 1, 0, 1, 1) /* Frostlink Bracers */, 
(80782, 106417, 0.02, 1, 0, 1, 1) /* Frostlink Gauntlets */, 
(80782, 106418, 0.01, 1, 0, 1, 1) /* Frostlink Greaves */, 
(80782, 106419, 0.02, 1, 0, 1, 1) /* Frostlink Helm */, 
(80782, 106420, 0.02, 1, 0, 1, 1) /* Frostlink Legguards */, 
(80782, 106421, 0.03, 1, 0, 1, 1) /* Frostlink Spaulders */, 
(80782, 106422, 0.01, 1, 0, 1, 1) /* Frostlink Vest */, 
(80782, 106423, 0.02, 1, 0, 1, 1) /* Rimeplate Breastplate */, 
(80782, 106424, 0.02, 1, 0, 1, 1) /* Rimeplate Gauntlets */, 
(80782, 106425, 0.01, 1, 0, 1, 1) /* Rimeplate Girdle */, 
(80782, 106426, 0.02, 1, 0, 1, 1) /* Rimeplate Helm */, 
(80782, 106427, 0.02, 1, 0, 1, 1) /* Rimeplate Legplates */, 
(80782, 106428, 0.01, 1, 0, 1, 1) /* Rimeplate Pauldrons */, 
(80782, 106429, 0.01, 1, 0, 1, 1) /* Rimeplate Sabatons */, 
(80782, 106430, 0.02, 1, 0, 1, 1) /* Rimeplate Shield */, 
(80782, 106431, 0.01, 1, 0, 1, 1) /* Rimeplate Vambraces */, 
(80782, 106432, 0.01, 1, 0, 1, 1) /* Voidcaller Bindings */, 
(80782, 106433, 0.02, 1, 0, 1, 1) /* Voidcaller Boots */, 
(80782, 106434, 0.02, 1, 0, 1, 1) /* Voidcaller Britches */, 
(80782, 106435, 0.01, 1, 0, 1, 1) /* Voidcaller Gloves */, 
(80782, 106436, 0.02, 1, 0, 1, 1) /* Voidcaller Hood */, 
(80782, 106437, 0.02, 1, 0, 1, 1) /* Voidcaller Jerkin */, 
(80782, 106438, 0.02, 1, 0, 1, 1) /* Voidcaller Shoulders */, 
(80782, 106439, 0.02, 1, 0, 1, 1) /* Voidcaller Waistband */, 
(80782, 106440, 0.02, 1, 0, 1, 1) /* Anchorite Cord */, 
(80782, 106441, 0.02, 1, 0, 1, 1) /* Anchorite Cowl */, 
(80782, 106442, 0.02, 1, 0, 1, 1) /* Anchorite Handwraps */, 
(80782, 106443, 0.02, 1, 0, 1, 1) /* Anchorite Leggings */, 
(80782, 106444, 0.01, 1, 0, 1, 1) /* Anchorite Robe */, 
(80782, 106445, 0.02, 1, 0, 1, 1) /* Anchorite Sandals */, 
(80782, 106446, 0.02, 1, 0, 1, 1) /* Anchorite Shoulderpads */, 
(80782, 106447, 0.01, 1, 0, 1, 1) /* Anchorite Wristwraps */, 
(80782, 106448, 0.02, 1, 0, 1, 1) /* Gronnling Belt */, 
(80782, 106449, 0.02, 1, 0, 1, 1) /* Gronnling Bracers */, 
(80782, 106450, 0.01, 1, 0, 1, 1) /* Gronnling Gauntlets */, 
(80782, 106451, 0.02, 1, 0, 1, 1) /* Gronnling Greaves */, 
(80782, 106452, 0.03, 1, 0, 1, 1) /* Gronnling Helm */, 
(80782, 106453, 0.02, 1, 0, 1, 1) /* Gronnling Legguards */, 
(80782, 106454, 0.01, 1, 0, 1, 1) /* Gronnling Spaulders */, 
(80782, 106455, 0.02, 1, 0, 1, 1) /* Gronnling Vest */, 
(80782, 106825, 6, 1, 0, 1, 1) /* Razor Talon */, 
(80782, 106867, 90, 1, 0, 1, 1) /* Dulled Claw */, 
(80782, 106869, 3, 1, 0, 1, 1) /* Ossified Venom Gland */, 
(80782, 106889, 0.09, 1, 0, 1, 1) /* Serrated Incisor */, 
(80782, 114679, -22, 1, 0, 1, 1) /* Icespine Stinger */, 
(80782, 116457, 100, 1, 0, 1, 1) /* Frostbitten Axe */, 
(80782, 116458, 0.02, 1, 0, 1, 1) /* Frostbitten Broadaxe */, 
(80782, 116459, 0.03, 1, 0, 1, 1) /* Frostbitten Bow */, 
(80782, 116460, 0.02, 1, 0, 1, 1) /* Frostbitten Crossbow */, 
(80782, 116461, 0.02, 1, 0, 1, 1) /* Frostbitten Dagger */, 
(80782, 116462, 0.02, 1, 0, 1, 1) /* Coldsinger Spellblade */, 
(80782, 116463, 0.02, 1, 0, 1, 1) /* Frostbitten Knuckles */, 
(80782, 116464, 0.01, 1, 0, 1, 1) /* Frostbitten Gun */, 
(80782, 116465, 0.03, 1, 0, 1, 1) /* Frostbitten Mace */, 
(80782, 116466, 0.02, 1, 0, 1, 1) /* Coldsinger Scepter */, 
(80782, 116467, 0.02, 1, 0, 1, 1) /* Frostbitten Hammer */, 
(80782, 116468, 0.02, 1, 0, 1, 1) /* Frostbitten Spear */, 
(80782, 116469, 0.02, 1, 0, 1, 1) /* Frostbitten Staff */, 
(80782, 116470, 0.02, 1, 0, 1, 1) /* Frostbitten Staff */, 
(80782, 116471, 0.02, 1, 0, 1, 1) /* Frostbitten Sword */, 
(80782, 116472, 0.02, 1, 0, 1, 1) /* Frostbitten Greatsword */, 
(80782, 116473, 0.02, 1, 0, 1, 1) /* Coldsinger Wand */, 
(80782, 116474, 0.02, 1, 0, 1, 1) /* Lunarglow Axe */, 
(80782, 116475, 0.02, 1, 0, 1, 1) /* Lunarglow Broadaxe */, 
(80782, 116476, 0.02, 1, 0, 1, 1) /* Lunarglow Bow */, 
(80782, 116477, 0.02, 1, 0, 1, 1) /* Lunarglow Crossbow */, 
(80782, 116478, 0.01, 1, 0, 1, 1) /* Lunarglow Dagger */, 
(80782, 116479, 0.01, 1, 0, 1, 1) /* Moonwhisper Spellblade */, 
(80782, 116480, 0.02, 1, 0, 1, 1) /* Lunarglow Knuckles */, 
(80782, 116481, 0.02, 1, 0, 1, 1) /* Lunarglow Gun */, 
(80782, 116482, 0.02, 1, 0, 1, 1) /* Lunarglow Mace */, 
(80782, 116483, 0.01, 1, 0, 1, 1) /* Moonwhisper Scepter */, 
(80782, 116484, 0.01, 1, 0, 1, 1) /* Lunarglow Hammer */, 
(80782, 116485, 0.02, 1, 0, 1, 1) /* Lunarglow Spear */, 
(80782, 116486, 0.01, 1, 0, 1, 1) /* Lunarglow Staff */, 
(80782, 116487, 0.02, 1, 0, 1, 1) /* Lunarglow Staff */, 
(80782, 116488, 0.03, 1, 0, 1, 1) /* Lunarglow Sword */, 
(80782, 116489, 0.02, 1, 0, 1, 1) /* Lunarglow Greatsword */, 
(80782, 116490, 0.01, 1, 0, 1, 1) /* Moonwhisper Wand */, 
(80782, 116677, 0.02, 1, 0, 1, 1) /* Coldsinger Amulet */, 
(80782, 116678, 0.01, 1, 0, 1, 1) /* Moonwhisper Amulet */, 
(80782, 116693, 0.02, 1, 0, 1, 1) /* Coldsinger Cloak */, 
(80782, 116694, 0.02, 1, 0, 1, 1) /* Coldsinger Crystal */, 
(80782, 116695, 0.02, 1, 0, 1, 1) /* Coldsinger Ring */, 
(80782, 116696, 0.02, 1, 0, 1, 1) /* Moonwhisper Cloak */, 
(80782, 116697, 0.01, 1, 0, 1, 1) /* Moonwhisper Crystal */, 
(80782, 116698, 0.02, 1, 0, 1, 1) /* Moonwhisper Ring */, 
(80782, 116920, 0.02, 1, 0, 1, 1) /* True Steel Lockbox */, 
(80782, 118340, 100, 1, 0, 1, 1) /* Cyclical Power Converter */, 
(80782, 118341, 0.01, 1, 0, 1, 1) /* Cyclical Power Housing */, 
(80782, 118342, 100, 1, 0, 1, 1) /* Cyclical Power Framing */, 
(80782, 118824, 100, 1, 0, 1, 1) /* Noxiia's Shackles */, 
(80782, 118835, 0.01, 1, 0, 1, 1) /* Kiraser's Gloves of Precision */, 
(80782, 118847, 0.01, 1, 0, 1, 1) /* Amulet of Quel'Riq */, 
(80782, 118867, 100, 1, 0, 1, 1) /* Evie's Quartz Loop */, 
(80782, 118877, 0.01, 1, 0, 1, 1) /* Fizzlebang's Folly */;

-- #8548
-- Blazing Pyreclaw
UPDATE `creature_template` SET `lootid`=80241 WHERE `entry`=80241;
DELETE FROM `creature_loot_template` WHERE `entry`=80241;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(80241, 106393, 0.15, 1, 0, 1, 1) /* Bladespire Helm */, 
(80241, 106406, 0.08, 1, 0, 1, 1) /* Coldsnout Waistband */, 
(80241, 106428, 0.15, 1, 0, 1, 1) /* Rimeplate Pauldrons */, 
(80241, 106451, 0.46, 1, 0, 1, 1) /* Gronnling Greaves */, 
(80241, 106825, 41, 1, 0, 1, 1) /* Razor Talon */, 
(80241, 106869, 17, 1, 0, 1, 1) /* Ossified Venom Gland */, 
(80241, 107528, 41, 1, 0, 1, 1) /* Tough Leathery Skin */, 
(80241, 109133, 34, 1, 0, 1, 1) /* Rylak Egg */, 
(80241, 111557, 47, 1, 0, 1, 3) /* Sumptuous Fur */;

-- #8545
-- Glacial Icemaw
UPDATE `creature_template` SET `lootid`=78314 WHERE `entry`=78314;
DELETE FROM `creature_loot_template` WHERE `entry`=78314;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(78314, 106441, 0.01, 1, 0, 1, 1) /* Anchorite Cowl */, 
(78314, 106825, 4, 1, 0, 1, 1) /* Razor Talon */, 
(78314, 106869, 4, 1, 0, 1, 1) /* Ossified Venom Gland */, 
(78314, 107528, 92, 1, 0, 1, 1) /* Tough Leathery Skin */, 
(78314, 109133, 35, 1, 0, 1, 1) /* Rylak Egg */, 
(78314, 111557, 48, 1, 0, 1, 3) /* Sumptuous Fur */, 
(78314, 116475, 0.04, 1, 0, 1, 1) /* Lunarglow Broadaxe */, 
(78314, 116487, 0.07, 1, 0, 1, 1) /* Lunarglow Staff */;

UPDATE `creature_template` SET `spell1`=161887, `spell2`=161896 WHERE `entry`=78314;

-- #8547
-- Bladespire Raider
UPDATE `creature_template` SET `spell1`=147117, `spell2`=148805, `spell3`=148852 WHERE `entry`=80172;

-- #8546
-- Coldsnout Piglet
UPDATE `creature_template` SET `spell1`=31358 WHERE `entry`=75417;

-- #8385
-- Pale Devourer
UPDATE `creature_loot_template` SET `ChanceOrQuestChance` = '0.01' WHERE (`entry`='79205' AND `item`='118782') ;
UPDATE `creature_loot_template` SET `ChanceOrQuestChance` = '0.01' WHERE (`entry`='79205' AND `item`='106876');

-- #8298
-- Big Pete
UPDATE `creature_template` SET `lootid`=82202 WHERE `entry`=82202;
DELETE FROM `creature_loot_template` WHERE `entry`=82202;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(82202, 106661, 0.02, 1, 0, 1, 1) /* Talon Guard Shield */, 
(82202, 106867, 88, 1, 0, 1, 1) /* Dulled Claw */, 
(82202, 106868, 5, 1, 0, 1, 1) /* Venomous Claw */, 
(82202, 106869, 8, 1, 0, 1, 1) /* Ossified Venom Gland */, 
(82202, 116723, 0.05, 1, 0, 1, 1) /* Oshu'gun Cloak */;

-- #8568
-- Vrok the Ancient
UPDATE `creature_template` SET `spell1`=172683, `spell2`=176229, `spell3`=176237 WHERE `entry`=87356;
delete from creature_onkill_reputation where creature_id = 87356;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (87356, 1445, 0, 0, 0, 400, 0, 0, 0, 0);

-- #8567
-- Flamewing Skytalon
delete from creature_onkill_reputation where creature_id = 87343;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (87343, 1445, 0, 0, 0, 1, 0, 0, 0, 0);

-- #8566
-- Frostfire Cragstomper
delete from creature_onkill_reputation where creature_id = 87333;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (87333, 1445, 0, 0, 0, 8, 0, 0, 0, 0);

-- #8563
-- Burning Slagmaw
delete from creature_onkill_reputation where creature_id = 72348;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (72348, 1445, 0, 0, 0, 5, 0, 0, 0, 0);

-- #8562
-- Vicious Acidmaw
delete from creature_onkill_reputation where creature_id = 76905;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (76905, 1445, 0, 0, 0, 8, 0, 0, 0, 0);
UPDATE `creature_template` SET `lootid`=76905 WHERE `entry`=76905;
DELETE FROM `creature_loot_template` WHERE `entry`=76905;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(76905, 102091, -10, 1, 0, 1, 1) /* Mote of Frostfire */, 
(76905, 106687, 0.02, 1, 0, 1, 1) /* Ironfist Breastplate */, 
(76905, 106688, 0.01, 1, 0, 1, 1) /* Ironfist Gauntlets */, 
(76905, 106689, 0.02, 1, 0, 1, 1) /* Ironfist Girdle */, 
(76905, 106690, 0.01, 1, 0, 1, 1) /* Ironfist Helm */, 
(76905, 106691, 0.02, 1, 0, 1, 1) /* Ironfist Legplates */, 
(76905, 106692, 0.01, 1, 0, 1, 1) /* Ironfist Pauldrons */, 
(76905, 106694, 0.01, 1, 0, 1, 1) /* Ironfist Shield */, 
(76905, 106695, 0.03, 1, 0, 1, 1) /* Ironfist Vambraces */, 
(76905, 106696, 0.02, 1, 0, 1, 1) /* Sabermaw Bindings */, 
(76905, 106697, 0.01, 1, 0, 1, 1) /* Sabermaw Boots */, 
(76905, 106698, 0.02, 1, 0, 1, 1) /* Sabermaw Britches */, 
(76905, 106699, 0.04, 1, 0, 1, 1) /* Sabermaw Gloves */, 
(76905, 106700, 0.03, 1, 0, 1, 1) /* Sabermaw Hood */, 
(76905, 106701, 0.02, 1, 0, 1, 1) /* Sabermaw Jerkin */, 
(76905, 106702, 0.01, 1, 0, 1, 1) /* Sabermaw Shoulders */, 
(76905, 106703, 0.02, 1, 0, 1, 1) /* Sabermaw Waistband */, 
(76905, 106704, 0.01, 1, 0, 1, 1) /* Voidwrap Cord */, 
(76905, 106705, 0.03, 1, 0, 1, 1) /* Voidwrap Cowl */, 
(76905, 106706, 0.02, 1, 0, 1, 1) /* Voidwrap Handwraps */, 
(76905, 106707, 0.01, 1, 0, 1, 1) /* Voidwrap Leggings */, 
(76905, 106708, 0.01, 1, 0, 1, 1) /* Voidwrap Robe */, 
(76905, 106709, 0.03, 1, 0, 1, 1) /* Voidwrap Sandals */, 
(76905, 106710, 0.03, 1, 0, 1, 1) /* Voidwrap Shoulderpads */, 
(76905, 106711, 0.02, 1, 0, 1, 1) /* Voidwrap Wristwraps */, 
(76905, 106712, 0.01, 1, 0, 1, 1) /* Sunspring Belt */, 
(76905, 106713, 0.01, 1, 0, 1, 1) /* Sunspring Bracers */, 
(76905, 106714, 0.03, 1, 0, 1, 1) /* Sunspring Gauntlets */, 
(76905, 106715, 0.03, 1, 0, 1, 1) /* Sunspring Greaves */, 
(76905, 106716, 0.02, 1, 0, 1, 1) /* Sunspring Helm */, 
(76905, 106717, 0.02, 1, 0, 1, 1) /* Sunspring Legguards */, 
(76905, 106720, 0.02, 1, 0, 1, 1) /* Stonecrag Breastplate */, 
(76905, 106721, 0.01, 1, 0, 1, 1) /* Stonecrag Gauntlets */, 
(76905, 106722, 0.02, 1, 0, 1, 1) /* Stonecrag Girdle */, 
(76905, 106724, 0.01, 1, 0, 1, 1) /* Stonecrag Legplates */, 
(76905, 106725, 0.01, 1, 0, 1, 1) /* Stonecrag Pauldrons */, 
(76905, 106726, 0.01, 1, 0, 1, 1) /* Stonecrag Sabatons */, 
(76905, 106727, 0.02, 1, 0, 1, 1) /* Stonecrag Shield */, 
(76905, 106728, 0.01, 1, 0, 1, 1) /* Stonecrag Vambraces */, 
(76905, 106730, 0.01, 1, 0, 1, 1) /* Meadowstomper Boots */, 
(76905, 106731, 0.02, 1, 0, 1, 1) /* Meadowstomper Britches */, 
(76905, 106732, 0.02, 1, 0, 1, 1) /* Meadowstomper Gloves */, 
(76905, 106733, 0.01, 1, 0, 1, 1) /* Meadowstomper Hood */, 
(76905, 106734, 0.02, 1, 0, 1, 1) /* Meadowstomper Jerkin */, 
(76905, 106735, 0.01, 1, 0, 1, 1) /* Meadowstomper Shoulders */, 
(76905, 106737, 0.02, 1, 0, 1, 1) /* Dark Star Cord */, 
(76905, 106738, 0.01, 1, 0, 1, 1) /* Dark Star Cowl */, 
(76905, 106739, 0.02, 1, 0, 1, 1) /* Dark Star Handwraps */, 
(76905, 106740, 0.01, 1, 0, 1, 1) /* Dark Star Leggings */, 
(76905, 106741, 0.03, 1, 0, 1, 1) /* Dark Star Robe */, 
(76905, 106742, 0.02, 1, 0, 1, 1) /* Dark Star Sandals */, 
(76905, 106743, 0.02, 1, 0, 1, 1) /* Dark Star Shoulderpads */, 
(76905, 106746, 0.01, 1, 0, 1, 1) /* Packrunner Bracers */, 
(76905, 106747, 0.03, 1, 0, 1, 1) /* Packrunner Gauntlets */, 
(76905, 106748, 0.02, 1, 0, 1, 1) /* Packrunner Greaves */, 
(76905, 106749, 0.01, 1, 0, 1, 1) /* Packrunner Helm */, 
(76905, 106750, 0.02, 1, 0, 1, 1) /* Packrunner Legguards */, 
(76905, 106751, 0.03, 1, 0, 1, 1) /* Packrunner Spaulders */, 
(76905, 106752, 0.02, 1, 0, 1, 1) /* Packrunner Vest */, 
(76905, 106888, 69, 1, 0, 1, 1) /* Chitinous Armor Fragment */, 
(76905, 106889, 25, 1, 0, 1, 1) /* Serrated Incisor */, 
(76905, 106890, 5, 1, 0, 1, 1) /* Luminescent Heartglobe */, 
(76905, 116610, 0.03, 1, 0, 1, 1) /* Gorian Axe */, 
(76905, 116611, 0.02, 1, 0, 1, 1) /* Gorian Broadaxe */, 
(76905, 116612, 0.03, 1, 0, 1, 1) /* Gorian Bow */, 
(76905, 116613, 0.01, 1, 0, 1, 1) /* Gorian Crossbow */, 
(76905, 116614, 0.01, 1, 0, 1, 1) /* Gorian Dagger */, 
(76905, 116615, 0.02, 1, 0, 1, 1) /* Ancestral Spellblade */, 
(76905, 116616, 0.02, 1, 0, 1, 1) /* Gorian Knuckles */, 
(76905, 116617, 0.02, 1, 0, 1, 1) /* Gorian Gun */, 
(76905, 116618, 0.02, 1, 0, 1, 1) /* Gorian Mace */, 
(76905, 116619, 0.02, 1, 0, 1, 1) /* Ancestral Scepter */, 
(76905, 116620, 0.01, 1, 0, 1, 1) /* Gorian Hammer */, 
(76905, 116621, 0.02, 1, 0, 1, 1) /* Gorian Spear */, 
(76905, 116622, 0.02, 1, 0, 1, 1) /* Gorian Staff */, 
(76905, 116623, 0.03, 1, 0, 1, 1) /* Gorian Staff */, 
(76905, 116625, 0.02, 1, 0, 1, 1) /* Gorian Greatsword */, 
(76905, 116628, 0.03, 1, 0, 1, 1) /* Howling Broadaxe */, 
(76905, 116629, 0.03, 1, 0, 1, 1) /* Howling Bow */, 
(76905, 116630, 0.01, 1, 0, 1, 1) /* Howling Crossbow */, 
(76905, 116631, 0.02, 1, 0, 1, 1) /* Howling Dagger */, 
(76905, 116633, 0.03, 1, 0, 1, 1) /* Howling Knuckles */, 
(76905, 116634, 0.01, 1, 0, 1, 1) /* Howling Gun */, 
(76905, 116635, 0.01, 1, 0, 1, 1) /* Howling Mace */, 
(76905, 116637, 0.02, 1, 0, 1, 1) /* Howling Hammer */, 
(76905, 116638, 0.02, 1, 0, 1, 1) /* Howling Spear */, 
(76905, 116639, 0.02, 1, 0, 1, 1) /* Howling Staff */, 
(76905, 116640, 0.02, 1, 0, 1, 1) /* Howling Staff */, 
(76905, 116641, 0.02, 1, 0, 1, 1) /* Howling Sword */, 
(76905, 116642, 0.02, 1, 0, 1, 1) /* Howling Greatsword */, 
(76905, 116643, 0.03, 1, 0, 1, 1) /* Oshu'gun Wand */, 
(76905, 116686, 0.02, 1, 0, 1, 1) /* Ancestral Amulet */, 
(76905, 116687, 0.03, 1, 0, 1, 1) /* Oshu'gun Amulet */, 
(76905, 116722, 0.02, 1, 0, 1, 1) /* Ancestral Ring */, 
(76905, 116723, 0.02, 1, 0, 1, 1) /* Oshu'gun Cloak */, 
(76905, 116724, 0.01, 1, 0, 1, 1) /* Oshu'gun Basin */, 
(76905, 116725, 0.01, 1, 0, 1, 1) /* Oshu'gun Ring */, 
(76905, 116920, 0.04, 1, 0, 1, 1) /* True Steel Lockbox */, 
(76905, 118340, 0.02, 1, 0, 1, 1) /* Cyclical Power Converter */, 
(76905, 118341, 0.02, 1, 0, 1, 1) /* Cyclical Power Housing */, 
(76905, 118342, 0.02, 1, 0, 1, 1) /* Cyclical Power Framing */, 
(76905, 118343, 0.01, 1, 0, 1, 1) /* Cyclical Power Sequencer */, 
(76905, 118839, 0.01, 1, 0, 1, 1) /* Bonechain Bracers */;

-- #72348
-- Burning Slagmaw
delete from creature_onkill_reputation where creature_id = 72348;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (72348, 1445, 0, 0, 0, 5, 0, 0, 0, 0);

-- #7443
-- Giant-Slayer Kul
delete from creature where id = 71665;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`)
VALUES
    (71665, 1116, 6720, 7006, 1, 1, 0, 0, 7364.51, 4158.21, 118.849, 2.60345, 300, 0, 0, 70012, 0, 0, 0, 0, 0, 2048, 0, 0, 0, 0, NULL);

-- #7444
-- Primalist Mur'og
delete from creature where id = 76918;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`)
VALUES
    (76918, 1116, 6720, 6744, 1, 1, 0, 0, 6893.44, 5245.96, 243.219, 5.15285, 300, 0, 0, 140024, 0, 0, 0, 0, 0, 2048, 0, 0, 0, 0, NULL);

-- #7498
-- Blood Guard Axelash <Primal Conquest Quartermaster>
delete from creature where id = 88569;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`)
VALUES
    (88569, 1116, 6941, 7333, 1, 4294967295, 0, 1, 5256.84, -3928.56, 17.9645, 4.76729, 300, 0, 0, 1467809, 0, 0, 0, 0, 0, 2048, 0, 0, 0, 0, NULL);

-- Fix from Latra
-- #8593 
-- Breathless
delete from creature where id = 78867;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`) VALUES
(78867, 1116, 5720, 7047, 1, 1, 0, 1, 6226.614746, 5799.523926, 134.618256, 1.397078, 120, 5, 0, 1, 0, 1, 0, 0, 0, 0, 0, 14);

-- #7995
-- Betsi Boombasket
delete from creature where id = 84887;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`)
VALUES
    (84887, 1116, 6722, 7163, 1, 1, 0, 1, -2027.67, 1147.37, 29.3267, 3.12301, 120, 0, 0, 1, 0, 0, 0, 0, 32768, 2048, 0, 0, 0, 0, NULL);

-- #8389
-- Frostfire Ridge
Delete from game_graveyard_zone where ghost_zone = 6960;
Delete from game_graveyard_zone where ghost_zone = 6720;
INSERT INTO `game_graveyard_zone` (`id`, `ghost_zone`, `faction`)
VALUES
    (4600, 6720, 0),
    (4601, 6720, 0),
    (4602, 6720, 0),
    (4603, 6720, 0),
    (4604, 6720, 0),
    (4605, 6720, 0),
    (4606, 6720, 0),
    (4607, 6720, 0),
    (4608, 6720, 0),
    (4609, 6720, 0),
    (4611, 6720, 0),
    (4663, 6720, 0),
    (4664, 6720, 0),
    (4665, 6720, 0),
    (4668, 6720, 0),
    (4835, 6720, 0),
    (4843, 6720, 0),
    (4911, 6720, 0),
    (4946, 6720, 0);

-- #8561
-- Burning Slagmaw
delete from creature_onkill_reputation where creature_id = 87346;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (87346, 1445, 0, 0, 0, 16, 0, 0, 0, 0);

-- #8588
-- Dorogg the Ruthless
UPDATE `creature_template` SET `lootid`=74254 WHERE `entry`=74254;
DELETE FROM `creature_loot_template` WHERE `entry`=74254;

INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(74254, 106283, 0.14, 1, 0, 1, 1) /* Ripped Mail Shoulderpads */, 
(74254, 106285, 0.16, 1, 0, 1, 1) /* Acid Scarred Plate Belt */, 
(74254, 106293, 0.16, 1, 0, 1, 1) /* Sun-Baked Pants */, 
(74254, 106294, 0.04, 1, 0, 1, 1) /* Acid Scarred Plate Pants */, 
(74254, 106295, 0.12, 1, 0, 1, 1) /* Ripped Mail Pants */, 
(74254, 106296, 0.14, 1, 0, 1, 1) /* Scraggly Cloth Pants */, 
(74254, 106299, 0.1, 1, 0, 1, 1) /* Ripped Mail Boots */, 
(74254, 106301, 0.18, 1, 0, 1, 1) /* Sun-Baked Gloves */, 
(74254, 106303, 0.14, 1, 0, 1, 1) /* Ripped Mail Gloves */, 
(74254, 106305, 0.1, 1, 0, 1, 1) /* Acid Scarred Plate Helmet */, 
(74254, 106306, 0.16, 1, 0, 1, 1) /* Scraggly Hat */, 
(74254, 106307, 0.1, 1, 0, 1, 1) /* Sun-Baked Leather Helmet */, 
(74254, 106308, 0.12, 1, 0, 1, 1) /* Ripped Mail Circlet */, 
(74254, 106310, 0.14, 1, 0, 1, 1) /* Acid Scarred Plate Chestpiece */, 
(74254, 106311, 0.16, 1, 0, 1, 1) /* Ripped Mail Armor */, 
(74254, 106312, 0.18, 1, 0, 1, 1) /* Scraggly Cloth Vest */, 
(74254, 106313, 0.12, 1, 0, 1, 1) /* Sun-Baked Boots */, 
(74254, 106316, 0.1, 1, 0, 1, 1) /* Half-Handle Mace */, 
(74254, 106317, 0.16, 1, 0, 1, 1) /* Brittle Blade */, 
(74254, 106318, 0.12, 1, 0, 1, 1) /* Blunt Logging Axe */, 
(74254, 106321, 0.1, 1, 0, 1, 1) /* Cracked-Haft Cleaver */, 
(74254, 106322, 0.08, 1, 0, 1, 1) /* Brittle Greatsword */, 
(74254, 106324, 0.29, 1, 0, 1, 1) /* Robble's Staff of Herding */, 
(74254, 106395, 0.1, 1, 0, 1, 1) /* Bladespire Pauldrons */, 
(74254, 106439, 0.04, 1, 0, 1, 1) /* Voidcaller Waistband */, 
(74254, 106444, 0.02, 1, 0, 1, 1) /* Anchorite Robe */, 
(74254, 111364, 2, 1, 0, 1, 1) /* First Aid in Draenor */, 
(74254, 111387, 2, 1, 0, 1, 1) /* The Joy of Draenor Cooking */, 
(74254, 112566, -19, 1, 0, 1, 1) /* Mysterious Flask */, 
(74254, 113295, 0.23, 1, 0, 1, 1) /* Cracked Potion Vial */, 
(74254, 113429, 0.12, 1, 0, 1, 1) /* Cracked Hand Drum */, 
(74254, 113478, 0.6, 1, 0, 1, 1) /* Abandoned Medic Kit */, 
(74254, 114965, -15, 1, 0, 1, 1) /* Fractured Forge Hammer */, 
(74254, 114973, -18, 1, 0, 1, 1) /* Frostwolf Tailoring Kit */, 
(74254, 115008, -20, 1, 0, 1, 1) /* Enchanted Highmaul Bracer */, 
(74254, 115287, -16, 1, 0, 1, 1) /* Intricate Crimson Pendant */, 
(74254, 115593, -14, 1, 0, 1, 1) /* Illegible Sootstained Notes */, 
(74254, 116173, -15, 1, 0, 1, 1) /* Tattered Frostwolf Shroud */, 
(74254, 116438, -17, 1, 0, 1, 1) /* Burned-Out Hand Cannon */, 
(74254, 116471, 0.02, 1, 0, 1, 1) /* Frostbitten Sword */, 
(74254, 116677, 0.02, 1, 0, 1, 1) /* Coldsinger Amulet */, 
(74254, 116920, 0.06, 1, 0, 1, 1) /* True Steel Lockbox */;

-- #8588
-- Dorogg the Ruthless
delete from creature_onkill_reputation where creature_id = 74254;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (74254, 1445, 0, 0, 0, 350, 0, 0, 0, 0);
UPDATE `creature_template` SET `spell1`=149599, `spell2`=149663, `spell3`=149673, `spell4`=152008, `spell5`=162157 WHERE `entry`=74254;

-- #8587
-- Valkor
delete from creature_onkill_reputation where creature_id = 87357;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `IsTeamAward1`, `RewOnKillRepValue1`, `MaxStanding2`, `IsTeamAward2`, `RewOnKillRepValue2`, `TeamDependent`)
VALUES
    (87357, 1445, 0, 0, 0, 400, 0, 0, 0, 0);

-- #8663
-- Gorum
delete from creature where id =76380;
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `unit_flags2`, `unit_flags3`, `dynamicflags`, `WorldEffectID`, `isActive`, `protec_anti_doublet`)
VALUES
    (76380, 1116, 6719, 6719, 1, 1, 0, 0, 1175.77, 823.449, 105.444, 3.55392, 120, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, NULL);


