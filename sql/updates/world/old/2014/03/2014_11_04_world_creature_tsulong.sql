DELETE FROM creature WHERE id IN (62442);
INSERT INTO `creature` (`id`, `map`, `zoneId`, `areaId`, `spawnMask`, `phaseMask`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `npcflag2`, `unit_flags`, `dynamicflags`, `isActive`) VALUES('62442','996','6067','6515','8','65535','0','0','-1017.95','-2807.68','38.2767','1.53812','300','0','0','0','0','0','0','0','0','0','0');

DELETE FROM `spell_script_names` WHERE spell_id IN(122768, 125843, 122789);
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (125843, 'spell_dread_shadows_damage');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122768, 'spell_dread_shadows_malus');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122789, 'spell_sunbeam');

UPDATE creature_template SET scriptname = "npc_sunbeam" WHERE entry = 62849;

-- The Dark of Night
UPDATE creature_template SET modelid1 = 42973, modelid2 = 42973, ScriptName = "npc_dark_of_night" WHERE entry = 63346;
-- Tsulong
UPDATE creature_template SET unit_class = 4, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 4, ScriptName = "boss_tsulong" WHERE entry = 62442;
-- Embodied Terror

UPDATE creature_template SET faction_H = 16, faction_A = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.5, ScriptName = "npc_embodied_terror" WHERE entry = 62969;

INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (122855, 'spell_sun_breath');
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES (123018, 'spell_terrorize_player');

-- Tiny Terror
UPDATE creature_template SET faction_H = 16, faction_A = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.1, ScriptName = "npc_tiny_terror" WHERE entry = 62977;

-- Unstable sha
UPDATE creature_template SET faction_A = 16, faction_H = 16, minlevel = 92, maxlevel = 92, mindmg = 25000, maxdmg = 50000, attackpower = 60000, dmg_multiplier = 2.25, ScriptName ="npc_unstable_sha" WHERE entry = 62919;

-- instability
INSERT INTO `spell_script_names` (spell_id, ScriptName) VALUES(123697, 'spell_instability');

DELETE FROM creature_text WHERE entry = 62442;
INSERT INTO creature_text VALUES
(62442, 0, 0, 'You do not belong here! The waters must be protected... I will cast you out, or slay you!', 14, 0, 100, 0, 0, 29350, 'VO_TES_SERPENT_AGGRO_01'),
(62442, 1, 0, 'Protect... the waters...', 14, 0, 100, 0, 0, 29351, 'VO_TES_SERPENT_DEATH_01'),
(62442, 1, 1, 'I thank you, strangers. I have been freed..', 14, 0, 100, 0, 0, 29352, 'VO_TES_SERPENT_DEATH_02'),
(62442, 2, 0, 'Day falls to night...', 14, 0, 100, 0, 0, 29353, 'VO_TES_SERPENT_EVENT_DAYTONIGHT_01'),
(62442, 2, 1, ' Darkness grows...', 14, 0, 100, 0, 0, 29354, 'VO_TES_SERPENT_EVENT_DAYTONIGHT_02'),
(62442, 2, 2, 'Light fades..', 14, 0, 100, 0, 0, 29355, 'VO_TES_SERPENT_EVENT_DAYTONIGHT_03'),
(62442, 3, 0, 'Night... becomes day!', 14, 0, 100, 0, 0, 29356, 'VO_TES_SERPENT_EVENT_NIGHTTODAY_01'),
(62442, 3, 1, 'The sun... shines through...', 14, 0, 100, 0, 0, 29357, 'VO_TES_SERPENT_EVENT_NIGHTTODAY_02'),
(62442, 3, 2, 'Once more... the dawn...', 14, 0, 100, 0, 0, 29358, 'VO_TES_SERPENT_EVENT_NIGHTTODAY_03'),
(62442, 4, 0, 'No... the waters... I must... resist... I shall not... fear...', 14, 0, 100, 0, 0, 29359, 'VO_TES_SERPENT_INTRO_01'),
(62442, 5, 0, 'Die in darkness.', 14, 0, 100, 0, 0, 29360, 'VO_TES_SERPENT_SLAY_01'),
(62442, 5, 1, 'The night surrounds you...', 14, 0, 100, 0, 0, 29361, 'VO_TES_SERPENT_SLAY_02'),
(62442, 6, 0, 'No...!', 14, 0, 100, 0, 0, 29362, 'VO_TES_SERPENT_SLAY_03'),
(62442, 6, 1, 'Forgive me!', 14, 0, 100, 0, 0, 29363, 'VO_TES_SERPENT_SLAY_04'),
(62442, 7, 0, 'Overwhelming... fear!', 14, 0, 100, 0, 0, 29364, 'VO_TES_SERPENT_SPELL_NIGGHTMARE_01'),
(62442, 7, 1, 'Flee this place... or die!', 14, 0, 100, 0, 0, 29365, 'VO_TES_SERPENT_SPELL_NIGGHTMARE_02');


DELETE FROM locales_creature_text WHERE entry = 62442;
INSERT INTO locales_creature_text VALUES
(62442, 0, 0, '', 'Vous n\'avez pas votre place ici ! Les eaux doivent être protégées… je vais vous renvoyer, ou vous tuer !', '', '', '', '', '', '', '', ''),
(62442, 1, 0, '', 'Protéger ... les eaux', '', '', '', '', '', '', '', ''),
(62442, 1, 1, '', 'Je vous remercie, étrangers. J\'ai été libéré.', '', '', '', '', '', '', '', ''),
(62442, 2, 0, '', 'Le jour s\'efface devant la nuit...', '', '', '', '', '', '', '', ''),
(62442, 2, 1, '', 'Les ombres s\'allongent...', '', '', '', '', '', '', '', ''),
(62442, 2, 2, '', 'La lumière s’estompe...', '', '', '', '', '', '', '', ''),
(62442, 3, 0, '', 'La nuit... devient le jour !', '', '', '', '', '', '', '', ''),
(62442, 3, 1, '', 'Le soleil... éclaire les ombres...', '', '', '', '', '', '', '', ''),
(62442, 3, 2, '', 'À nouveau... l\'aurore...', '', '', '', '', '', '', '', ''),
(62442, 4, 0, '', 'Non ... les eaux ... Je doit ... résister ... Je n\'aurais pas ... peur ...', '', '', '', '', '', '', '', ''),
(62442, 5, 0, '', 'Meurs dans les ténèbres.', '', '', '', '', '', '', '', ''),
(62442, 5, 1, '', 'Que la nuit t\'enveloppe', '', '', '', '', '', '', '', ''),
(62442, 6, 0, '', 'Non... !', '', '', '', '', '', '', '', ''),
(62442, 6, 1, '', 'Pardonnez-moi !', '', '', '', '', '', '', '', ''),
(62442, 7, 0, '', 'Irrésistible… terreur !', '', '', '', '', '', '', '', ''),
(62442, 7, 1, '', 'Fuyez ce lieu… ou mourez !', '', '', '', '', '', '', '', '');

-- Loots for Tsulong
DELETE FROM gameobject_template WHERE entry IN (215355, 215356, 215357, 212922);
INSERT INTO gameobject_template VALUES
-- Normal mode 10
(215355, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 215355, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Normal mode 25
(215356, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 215356, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode 10
(215357, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 215357, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595),
-- Heroic mode 25
(212922, 3, 12438, 'Cache of Tsulong', '', '', '', 0, 0, 3, 0, 0, 0, 0, 0, 0, 1634, 212922, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '', '', 15595);

-- MODE : 10_NM (215355)

SET @10_NM_ENTRY = 215355;
DELETE FROM `reference_loot_template` WHERE `entry`= @10_NM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY, 86328, 14, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@10_NM_ENTRY, 86324, 14, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@10_NM_ENTRY, 89883, 13, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@10_NM_ENTRY, 86327, 14, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@10_NM_ENTRY, 86321, 14, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@10_NM_ENTRY, 86330, 13, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@10_NM_ENTRY, 86322, 13, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@10_NM_ENTRY, 86323, 12, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@10_NM_ENTRY, 89843, 10, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@10_NM_ENTRY, 86326, 9, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@10_NM_ENTRY, 89884, 9, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@10_NM_ENTRY, 86325, 9, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@10_NM_ENTRY, 89842, 8, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@10_NM_ENTRY, 86329, 9, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@10_NM_ENTRY, 87209, 8, 1, 1, 1, 1) /* 2Sigil of Wisdom */, 
(@10_NM_ENTRY, 87208, 2, 1, 1, 1, 1) /* 2Sigil of Power */, 
(@10_NM_ENTRY, 86383, 2, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@10_NM_ENTRY, 86339, 1, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@10_NM_ENTRY, 80433, 1, 1, 1, 1, 2) /* 4Blood Spirit */, 
(@10_NM_ENTRY, 86343, 1, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@10_NM_ENTRY, 86338, 1, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@10_NM_ENTRY, 86384, 0.94, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */, 
(@10_NM_ENTRY, 86342, 0.9, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@10_NM_ENTRY, 86238, 0.85, 1, 1, 1, 1) /* 3Pattern: Chestguard of Nemeses */, 
(@10_NM_ENTRY, 87412, 0.65, 1, 1, 1, 1) /* 3Plans: Chestplate of Limitless Faith */, 
(@10_NM_ENTRY, 86385, 0.77, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@10_NM_ENTRY, 86341, 0.45, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@10_NM_ENTRY, 87408, 0.49, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@10_NM_ENTRY, 86280, 0.53, 1, 1, 1, 1) /* 3Pattern: Murderer's Gloves */, 
(@10_NM_ENTRY, 86281, 0.65, 1, 1, 1, 1) /* 3Pattern: Nightfire Robe */, 
(@10_NM_ENTRY, 87413, 0.57, 1, 1, 1, 1) /* 3Plans: Gauntlets of Unbound Devotion */, 
(@10_NM_ENTRY, 86337, 0.61, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@10_NM_ENTRY, 86272, 0.57, 1, 1, 1, 1) /* 3Pattern: Fists of Lightning */, 
(@10_NM_ENTRY, 86284, 0.65, 1, 1, 1, 1) /* 3Pattern: Raven Lord's Gloves */, 
(@10_NM_ENTRY, 86379, 0.49, 1, 1, 1, 1) /* 3Pattern: Robe of Eternal Rule */, 
(@10_NM_ENTRY, 86340, 0.53, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@10_NM_ENTRY, 87409, 0.45, 1, 1, 1, 1) /* 3Plans: Gauntlets of Battle Command */, 
(@10_NM_ENTRY, 87411, 0.53, 1, 1, 1, 1) /* 3Plans: Bloodforged Warfists */, 
(@10_NM_ENTRY, 86279, 0.45, 1, 1, 1, 1) /* 3Pattern: Liferuned Leather Gloves */, 
(@10_NM_ENTRY, 86283, 0.33, 1, 1, 1, 1) /* 3Pattern: Raiment of Blood and Bone */, 
(@10_NM_ENTRY, 86381, 0.41, 1, 1, 1, 1) /* 3Pattern: Legacy of the Emperor */, 
(@10_NM_ENTRY, 86380, 0.41, 1, 1, 1, 1) /* 3Pattern: Imperial Silk Gloves */, 
(@10_NM_ENTRY, 86297, 0.37, 1, 1, 1, 1) /* 3Pattern: Stormbreaker Chestguard */, 
(@10_NM_ENTRY, 86382, 0.24, 1, 1, 1, 1) /* 3Pattern: Touch of the Light */, 
(@10_NM_ENTRY, 87410, 0.28, 1, 1, 1, 1) /* 3Plans: Ornate Battleplate of the Master */;

DELETE FROM `gameobject_loot_template` WHERE `entry`= @10_NM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_NM_ENTRY, 1, 100, 1, 0, -@10_NM_ENTRY, 2);



-- MODE : 25_NM (215356)

SET @25_NM_ENTRY = 215356;
DELETE FROM `reference_loot_template` WHERE `entry`= @25_NM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY, 86328, 34, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@25_NM_ENTRY, 86324, 30, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@25_NM_ENTRY, 89883, 39, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@25_NM_ENTRY, 86327, 29, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@25_NM_ENTRY, 86321, 30, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@25_NM_ENTRY, 86330, 33, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@25_NM_ENTRY, 86322, 32, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@25_NM_ENTRY, 86323, 25, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@25_NM_ENTRY, 89843, 31, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@25_NM_ENTRY, 86326, 26, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@25_NM_ENTRY, 89884, 28, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@25_NM_ENTRY, 86325, 27, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@25_NM_ENTRY, 89842, 27, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@25_NM_ENTRY, 86329, 19, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@25_NM_ENTRY, 87209, 9, 1, 1, 1, 1) /* 2Sigil of Wisdom */, 
(@25_NM_ENTRY, 87208, 7, 1, 1, 1, 1) /* 2Sigil of Power */, 
(@25_NM_ENTRY, 86383, 4, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@25_NM_ENTRY, 86339, 2, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@25_NM_ENTRY, 80433, 0.26, 1, 1, 1, 2) /* 4Blood Spirit */, 
(@25_NM_ENTRY, 86343, 2, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@25_NM_ENTRY, 86338, 0.26, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@25_NM_ENTRY, 86384, 1, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */, 
(@25_NM_ENTRY, 86342, 1, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@25_NM_ENTRY, 86238, 0.26, 1, 1, 1, 1) /* 3Pattern: Chestguard of Nemeses */, 
(@25_NM_ENTRY, 87412, 2, 1, 1, 1, 1) /* 3Plans: Chestplate of Limitless Faith */, 
(@25_NM_ENTRY, 86385, 1, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@25_NM_ENTRY, 86341, 2, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@25_NM_ENTRY, 87408, 0.79, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@25_NM_ENTRY, 86280, 1, 1, 1, 1, 1) /* 3Pattern: Murderer's Gloves */, 
(@25_NM_ENTRY, 86281, 0.26, 1, 1, 1, 1) /* 3Pattern: Nightfire Robe */, 
(@25_NM_ENTRY, 87413, 0.79, 1, 1, 1, 1) /* 3Plans: Gauntlets of Unbound Devotion */, 
(@25_NM_ENTRY, 86337, 0.52, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@25_NM_ENTRY, 86272, 0.52, 1, 1, 1, 1) /* 3Pattern: Fists of Lightning */, 
(@25_NM_ENTRY, 86340, 0.52, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@25_NM_ENTRY, 87409, 0.52, 1, 1, 1, 1) /* 3Plans: Gauntlets of Battle Command */, 
(@25_NM_ENTRY, 86279, 0.26, 1, 1, 1, 1) /* 3Pattern: Liferuned Leather Gloves */, 
(@25_NM_ENTRY, 86381, 0.26, 1, 1, 1, 1) /* 3Pattern: Legacy of the Emperor */, 
(@25_NM_ENTRY, 86382, 0.52, 1, 1, 1, 1) /* 3Pattern: Touch of the Light */, 
(@25_NM_ENTRY, 87410, 0.26, 1, 1, 1, 1) /* 3Plans: Ornate Battleplate of the Master */;

DELETE FROM `gameobject_loot_template` WHERE `entry`= @25_NM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_NM_ENTRY, 1, 100, 1, 0, -@25_NM_ENTRY, 5);


-- MODE : 10_HM (215357)

SET @10_HM_ENTRY = 215357;
DELETE FROM `reference_loot_template` WHERE `entry`= @10_HM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY, 80433, 9, 1, 1, 1, 2) /* 4Blood Spirit */, 
(@10_HM_ENTRY, 87164, 29, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@10_HM_ENTRY, 89945, 13, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@10_HM_ENTRY, 87162, 13, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@10_HM_ENTRY, 87163, 16, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@10_HM_ENTRY, 89946, 12, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@10_HM_ENTRY, 87157, 17, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@10_HM_ENTRY, 87156, 10, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@10_HM_ENTRY, 87408, 3, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@10_HM_ENTRY, 89947, 3, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@10_HM_ENTRY, 87158, 12, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@10_HM_ENTRY, 87160, 9, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@10_HM_ENTRY, 87159, 6, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@10_HM_ENTRY, 87161, 9, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@10_HM_ENTRY, 86283, 4, 1, 1, 1, 1) /* 3Pattern: Raiment of Blood and Bone */, 
(@10_HM_ENTRY, 89948, 4, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@10_HM_ENTRY, 87165, 1, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@10_HM_ENTRY, 87178, 3, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@10_HM_ENTRY, 87182, 1, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@10_HM_ENTRY, 87184, 1, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@10_HM_ENTRY, 87179, 1, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@10_HM_ENTRY, 87181, 3, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@10_HM_ENTRY, 87186, 3, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@10_HM_ENTRY, 87177, 3, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@10_HM_ENTRY, 87180, 3, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@10_HM_ENTRY, 87183, 3, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@10_HM_ENTRY, 87185, 3, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */;

DELETE FROM `gameobject_loot_template` WHERE `entry`= @10_HM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@10_HM_ENTRY, 1, 100, 1, 0, -@10_HM_ENTRY, 2);


-- MODE : 25_HM (212922)
SET @25_HM_ENTRY = 212922;
DELETE FROM `reference_loot_template` WHERE `entry`= @25_HM_ENTRY ;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY, 87164, 25, 1, 1, 1, 1) /* 3Loshan, Terror Incarnate */, 
(@25_HM_ENTRY, 89945, 56, 1, 1, 1, 1) /* 3Gauntlets of the Shadow's Caress */, 
(@25_HM_ENTRY, 87162, 53, 1, 1, 1, 1) /* 3Sandals of the Blackest Night */, 
(@25_HM_ENTRY, 87163, 44, 1, 1, 1, 1) /* 3Spirits of the Sun */, 
(@25_HM_ENTRY, 86238, 3, 1, 1, 1, 1) /* 3Pattern: Chestguard of Nemeses */, 
(@25_HM_ENTRY, 89946, 47, 1, 1, 1, 1) /* 3Grasps of Serpentine Might */, 
(@25_HM_ENTRY, 87157, 25, 1, 1, 1, 1) /* 3Sunwrought Mail Hauberk */, 
(@25_HM_ENTRY, 87156, 38, 1, 1, 1, 1) /* 3Gao-Rei, Staff of the Legendary Protector */, 
(@25_HM_ENTRY, 87408, 3, 1, 1, 1, 1) /* 3Plans: Unyielding Bloodplate */, 
(@25_HM_ENTRY, 89947, 47, 1, 1, 1, 1) /* 3Shoulderpads of Twisted Fate */, 
(@25_HM_ENTRY, 87158, 25, 1, 1, 1, 1) /* 3Dread Shadow Ring */, 
(@25_HM_ENTRY, 87409, 3, 1, 1, 1, 1) /* 3Plans: Gauntlets of Battle Command */, 
(@25_HM_ENTRY, 87160, 25, 1, 1, 1, 1) /* 3Stuff of Nightmares */, 
(@25_HM_ENTRY, 87159, 28, 1, 1, 1, 1) /* 3Daybreak Drape */, 
(@25_HM_ENTRY, 87161, 22, 1, 1, 1, 1) /* 3Belt of Embodied Terror */, 
(@25_HM_ENTRY, 89948, 19, 1, 1, 1, 1) /* 3Fear-Blackened Leggings */, 
(@25_HM_ENTRY, 87165, 19, 1, 1, 1, 1) /* 3Sollerets of Instability */, 
(@25_HM_ENTRY, 87178, 6, 1, 1, 1, 1) /* 3Healer's Belt of Final Winter */, 
(@25_HM_ENTRY, 87182, 3, 1, 1, 1, 1) /* 3Ranger's Chain of Unending Summer */, 
(@25_HM_ENTRY, 87184, 3, 1, 1, 1, 1) /* 3Mender's Girdle of Endless Spring */, 
(@25_HM_ENTRY, 87179, 6, 1, 1, 1, 1) /* 3Sorcerer's Belt of Final Winter */, 
(@25_HM_ENTRY, 87181, 3, 1, 1, 1, 1) /* 3Weaver's Cord of Eternal Autumn */, 
(@25_HM_ENTRY, 87186, 3, 1, 1, 1, 1) /* 3Patroller's Girdle of Endless Spring */, 
(@25_HM_ENTRY, 87177, 6, 1, 1, 1, 1) /* 3Invoker's Belt of Final Winter */, 
(@25_HM_ENTRY, 87180, 6, 1, 1, 1, 1) /* 3Stalker's Cord of Eternal Autumn */, 
(@25_HM_ENTRY, 87183, 6, 1, 1, 1, 1) /* 3Binder's Chain of Unending Summer */, 
(@25_HM_ENTRY, 87185, 6, 1, 1, 1, 1) /* 3Protector's Girdle of Endless Spring */;
DELETE FROM `gameobject_loot_template` WHERE `entry`= @25_HM_ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(@25_HM_ENTRY, 1, 100, 1, 0, -@25_HM_ENTRY, 5);