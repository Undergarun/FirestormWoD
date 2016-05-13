
-- Normal loots.
SET @ENTRY = 75964;
UPDATE creature_template SET lootid = @ENTRY WHERE entry=@ENTRY;
DELETE FROM `creature_loot_template` WHERE entry=@ENTRY;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@ENTRY, 1, 100, 1, 0, -@ENTRY, 2, ''); 
DELETE FROM `reference_loot_template` WHERE entry=@ENTRY;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(@ENTRY, 109824, 0, 1, 1, 1, 1, ''), /* Cord of Arcane Mystery */ 
(@ENTRY, 109825, 0, 1, 1, 1, 1, ''), /* Frost-Touched Cord */ 
(@ENTRY, 109826, 0, 1, 1, 1, 1, ''), /* Cord of Swirling Light */ 
(@ENTRY, 109827, 0, 1, 1, 1, 1, ''), /* Lightbinder Girdle */ 
(@ENTRY, 109828, 0, 1, 1, 1, 1, ''), /* Felflame Belt */ 
(@ENTRY, 109829, 0, 1, 1, 1, 1, ''), /* Belt of Burning Focus */ 
(@ENTRY, 109830, 0, 1, 1, 1, 1, ''), /* Bloodfeather Girdle */ 
(@ENTRY, 109831, 0, 1, 1, 1, 1, ''), /* Crystalbinder Belt */ 
(@ENTRY, 109832, 0, 1, 1, 1, 1, ''), /* Leafmender Girdle */ 
(@ENTRY, 109833, 0, 1, 1, 1, 1, ''), /* Streamslither Belt */ 
(@ENTRY, 109834, 0, 1, 1, 1, 1, ''), /* Sharpeye Belt */ 
(@ENTRY, 109835, 0, 1, 1, 1, 1, ''), /* Rockhide Links */ 
(@ENTRY, 109836, 0, 1, 1, 1, 1, ''), /* Lavalink Girdle */ 
(@ENTRY, 109837, 0, 1, 1, 1, 1, ''), /* Incarnadine Girdle */ 
(@ENTRY, 109838, 0, 1, 1, 1, 1, ''), /* Verdant Plate Belt */ 
(@ENTRY, 109839, 0, 1, 1, 1, 1, ''), /* Goldsteel Belt */ 
(@ENTRY, 109840, 0, 1, 1, 1, 1, ''), /* Gutcrusher Greatbelt */ 
(@ENTRY, 109841, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Waistplate */ 
(@ENTRY, 109842, 0, 1, 1, 1, 1, ''), /* Blackwater Belt */ 
(@ENTRY, 109843, 0, 1, 1, 1, 1, ''), /* Morningscale Waistguard */ 
(@ENTRY, 109970, 0, 1, 1, 1, 1, ''), /* Hood of Arcane Mystery */ 
(@ENTRY, 109971, 0, 1, 1, 1, 1, ''), /* Frost-Touched Hood */ 
(@ENTRY, 109972, 0, 1, 1, 1, 1, ''), /* Hood of Swirling Light */ 
(@ENTRY, 109973, 0, 1, 1, 1, 1, ''), /* Lightbinder Cover */ 
(@ENTRY, 109974, 0, 1, 1, 1, 1, ''), /* Felflame Hood */ 
(@ENTRY, 109975, 0, 1, 1, 1, 1, ''), /* Hood of Burning Focus */ 
(@ENTRY, 109976, 0, 1, 1, 1, 1, ''), /* Bloodfeather Cowl */ 
(@ENTRY, 109977, 0, 1, 1, 1, 1, ''), /* Crystalbinder Helm */ 
(@ENTRY, 109978, 0, 1, 1, 1, 1, ''), /* Leafmender Hood */ 
(@ENTRY, 109979, 0, 1, 1, 1, 1, ''), /* Blackwater Helm */ 
(@ENTRY, 109980, 0, 1, 1, 1, 1, ''), /* Streamslither Helm */ 
(@ENTRY, 109981, 0, 1, 1, 1, 1, ''), /* Sharpeye Gleam */ 
(@ENTRY, 109982, 0, 1, 1, 1, 1, ''), /* Rockhide Casque */ 
(@ENTRY, 109983, 0, 1, 1, 1, 1, ''), /* Lavalink Helm */ 
(@ENTRY, 109984, 0, 1, 1, 1, 1, ''), /* Morningscale Cowl */ 
(@ENTRY, 109985, 0, 1, 1, 1, 1, ''), /* Incarnadine Greathelm */ 
(@ENTRY, 109986, 0, 1, 1, 1, 1, ''), /* Verdant Plate Crown */ 
(@ENTRY, 109987, 0, 1, 1, 1, 1, ''), /* Goldsteel Greathelm */ 
(@ENTRY, 109988, 0, 1, 1, 1, 1, ''), /* Gutcrusher Coronet */ 
(@ENTRY, 109989, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Casque */ 
(@ENTRY, 110030, 0, 1, 1, 1, 1, ''); /* Chakram-Breaker Greatsword */


-- Normal loots.
SET @ENTRY = 76141;
UPDATE creature_template SET lootid = @ENTRY WHERE entry=@ENTRY;
DELETE from `creature_loot_template` WHERE entry=@ENTRY;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@ENTRY, 1, 100, 1, 0, -@ENTRY, 2, ''); 
DELETE FROM `reference_loot_template` where entry=@ENTRY;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(@ENTRY, 109784, 0, 1, 1, 1, 1, ''), /* Sandals of Arcane Mystery */ 
(@ENTRY, 109785, 0, 1, 1, 1, 1, ''), /* Frost-Touched Boots */ 
(@ENTRY, 109786, 0, 1, 1, 1, 1, ''), /* Sandals of Swirling Light */ 
(@ENTRY, 109787, 0, 1, 1, 1, 1, ''), /* Boots of Burning Focus */ 
(@ENTRY, 109788, 0, 1, 1, 1, 1, ''), /* Bloodfeather Treads */ 
(@ENTRY, 109789, 0, 1, 1, 1, 1, ''), /* Crystalbinder Sandals */ 
(@ENTRY, 109790, 0, 1, 1, 1, 1, ''), /* Streamslither Boots */ 
(@ENTRY, 109791, 0, 1, 1, 1, 1, ''), /* Sharpeye Greaves */ 
(@ENTRY, 109792, 0, 1, 1, 1, 1, ''), /* Rockhide Treads */ 
(@ENTRY, 109793, 0, 1, 1, 1, 1, ''), /* Incarnadine Greaves */ 
(@ENTRY, 109794, 0, 1, 1, 1, 1, ''), /* Verdant Plate Treads */ 
(@ENTRY, 109795, 0, 1, 1, 1, 1, ''), /* Goldsteel Sabatons */ 
(@ENTRY, 109796, 0, 1, 1, 1, 1, ''), /* Lightbinder Treads */ 
(@ENTRY, 109797, 0, 1, 1, 1, 1, ''), /* Felflame Sandals */ 
(@ENTRY, 109798, 0, 1, 1, 1, 1, ''), /* Leafmender Sandals */ 
(@ENTRY, 109799, 0, 1, 1, 1, 1, ''), /* Blackwater Boots */ 
(@ENTRY, 109800, 0, 1, 1, 1, 1, ''), /* Lavalink Stompers */ 
(@ENTRY, 109801, 0, 1, 1, 1, 1, ''), /* Morningscale Treads */ 
(@ENTRY, 109802, 0, 1, 1, 1, 1, ''), /* Gutcrusher Stompers */ 
(@ENTRY, 109803, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Treads */ 
(@ENTRY, 109950, 0, 1, 1, 1, 1, ''), /* Stormshot Choker */ 
(@ENTRY, 109951, 0, 1, 1, 1, 1, ''), /* Fireblade Collar */ 
(@ENTRY, 109952, 0, 1, 1, 1, 1, ''), /* Skulltooth Chain */ 
(@ENTRY, 109953, 0, 1, 1, 1, 1, ''), /* Windseal Necklace */ 
(@ENTRY, 109954, 0, 1, 1, 1, 1, ''), /* Magister's Chain */ 
(@ENTRY, 109955, 0, 1, 1, 1, 1, ''), /* Demonbinder Cabochon */ 
(@ENTRY, 109956, 0, 1, 1, 1, 1, ''), /* Necklace of Endless Shadow */ 
(@ENTRY, 109957, 0, 1, 1, 1, 1, ''), /* Alc's Pendant of Fiery Dreams */ 
(@ENTRY, 109958, 0, 1, 1, 1, 1, ''), /* Healing Leaf Necklace */ 
(@ENTRY, 109959, 0, 1, 1, 1, 1, ''), /* Chain of Soothing Light */ 
(@ENTRY, 109960, 0, 1, 1, 1, 1, ''), /* Wavesurge Choker */ 
(@ENTRY, 109961, 0, 1, 1, 1, 1, ''), /* Pendant of Purifying Mists */ 
(@ENTRY, 109962, 0, 1, 1, 1, 1, ''), /* Bloodmist Pendant */ 
(@ENTRY, 109963, 0, 1, 1, 1, 1, ''), /* Goreclasp Choker */ 
(@ENTRY, 109964, 0, 1, 1, 1, 1, ''), /* Necklace of Furious Zeal */ 
(@ENTRY, 109965, 0, 1, 1, 1, 1, ''), /* Fistbreak Choker */ 
(@ENTRY, 109966, 0, 1, 1, 1, 1, ''), /* Reinforced Bloodsteel Gorget */ 
(@ENTRY, 109967, 0, 1, 1, 1, 1, ''), /* Necklace of Holy Deflection */ 
(@ENTRY, 109968, 0, 1, 1, 1, 1, ''), /* Flesh Beetle Brooch */ 
(@ENTRY, 109969, 0, 1, 1, 1, 1, ''), /* Choker of Weeping Viscera */ 
(@ENTRY, 110016, 0, 1, 1, 1, 1, ''), /* Solar Containment Unit */ 
(@ENTRY, 110031, 0, 1, 1, 1, 1, ''); /* Spire of the Furious Construct */ 

-- Normal loots.
SET @ENTRY = 76266;
UPDATE creature_template SET lootid = @ENTRY WHERE entry=@ENTRY;
DELETE FROM `creature_loot_template` WHERE entry=@ENTRY;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@ENTRY, 1, 100, 1, 0, -@ENTRY, 2, ''); 
DELETE FROM `reference_loot_template` WHERE entry=@ENTRY;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(@ENTRY, 107393, 0, 1, 1, 1, 1, ''), /* Piston Rod */ 
(@ENTRY, 109759, 0, 1, 1, 1, 1, ''), /* Ro-Ger's Brown Diamond Seal */ 
(@ENTRY, 109760, 0, 1, 1, 1, 1, ''), /* Ced's Chiming Circle */ 
(@ENTRY, 109761, 0, 1, 1, 1, 1, ''), /* Bloodthorn Band */ 
(@ENTRY, 109762, 0, 1, 1, 1, 1, ''), /* Signet of Radiant Leaves */ 
(@ENTRY, 109763, 0, 1, 1, 1, 1, ''), /* Diamondglow Circle */ 
(@ENTRY, 109764, 0, 1, 1, 1, 1, ''), /* Mark of Ice */ 
(@ENTRY, 109765, 0, 1, 1, 1, 1, ''), /* Golem's Gleaming Eye */ 
(@ENTRY, 109766, 0, 1, 1, 1, 1, ''), /* Darkflame Loop */ 
(@ENTRY, 109767, 0, 1, 1, 1, 1, ''), /* Ring of Purified Light */ 
(@ENTRY, 109768, 0, 1, 1, 1, 1, ''), /* Band of Growing Leaves */ 
(@ENTRY, 109769, 0, 1, 1, 1, 1, ''), /* Slicebinder Loop */ 
(@ENTRY, 109770, 0, 1, 1, 1, 1, ''), /* Signet of Crashing Waves */ 
(@ENTRY, 109771, 0, 1, 1, 1, 1, ''), /* Bloodied Ring of Mytosis */ 
(@ENTRY, 109772, 0, 1, 1, 1, 1, ''), /* Knucklebone of Lo'Dronar */ 
(@ENTRY, 109773, 0, 1, 1, 1, 1, ''), /* Band of Iron Scale */ 
(@ENTRY, 109774, 0, 1, 1, 1, 1, ''), /* Ring of Ripped Flesh */ 
(@ENTRY, 109775, 0, 1, 1, 1, 1, ''), /* Bladebinder Ring */ 
(@ENTRY, 109776, 0, 1, 1, 1, 1, ''), /* Seal of Resilient Fortitude */ 
(@ENTRY, 109777, 0, 1, 1, 1, 1, ''), /* Unsullied Signet */ 
(@ENTRY, 109778, 0, 1, 1, 1, 1, ''), /* Signet of the Glorious Protector */ 
(@ENTRY, 109779, 0, 1, 1, 1, 1, ''), /* Ancient Draenic Loop */ 
(@ENTRY, 109780, 0, 1, 1, 1, 1, ''), /* Signet of Shifting Magics */ 
(@ENTRY, 109781, 0, 1, 1, 1, 1, ''), /* Seal of Vindication */ 
(@ENTRY, 109782, 0, 1, 1, 1, 1, ''), /* Disease-Binder Seal */ 
(@ENTRY, 109783, 0, 1, 1, 1, 1, ''), /* Band of the Stalwart Stanchion */ 
(@ENTRY, 109804, 0, 1, 1, 1, 1, ''), /* Trousers of Arcane Mystery */ 
(@ENTRY, 109805, 0, 1, 1, 1, 1, ''), /* Frost-Touched Legwraps */ 
(@ENTRY, 109806, 0, 1, 1, 1, 1, ''), /* Leggings of Swirling Light */ 
(@ENTRY, 109807, 0, 1, 1, 1, 1, ''), /* Lightbinder Leggings */ 
(@ENTRY, 109808, 0, 1, 1, 1, 1, ''), /* Felflame Legwraps */ 
(@ENTRY, 109809, 0, 1, 1, 1, 1, ''), /* Legguards of Burning Focus */ 
(@ENTRY, 109810, 0, 1, 1, 1, 1, ''), /* Bloodfeather Leggings */ 
(@ENTRY, 109811, 0, 1, 1, 1, 1, ''), /* Crystalbinder Legwraps */ 
(@ENTRY, 109812, 0, 1, 1, 1, 1, ''), /* Leafmender Legwraps */ 
(@ENTRY, 109813, 0, 1, 1, 1, 1, ''), /* Streamslither Legguards */ 
(@ENTRY, 109814, 0, 1, 1, 1, 1, ''), /* Sharpeye Legguards */ 
(@ENTRY, 109815, 0, 1, 1, 1, 1, ''), /* Rockhide Leggings */ 
(@ENTRY, 109816, 0, 1, 1, 1, 1, ''), /* Lavalink Kilt */ 
(@ENTRY, 109817, 0, 1, 1, 1, 1, ''), /* Morningscale Leggings */ 
(@ENTRY, 109818, 0, 1, 1, 1, 1, ''), /* Incarnadine Legplates */ 
(@ENTRY, 109819, 0, 1, 1, 1, 1, ''), /* Verdant Plate Legguards */ 
(@ENTRY, 109820, 0, 1, 1, 1, 1, ''), /* Goldsteel Legplates */ 
(@ENTRY, 109821, 0, 1, 1, 1, 1, ''), /* Gutcrusher Legplates */ 
(@ENTRY, 109822, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Legplates */ 
(@ENTRY, 109823, 0, 1, 1, 1, 1, ''), /* Blackwater Leggings */ 
(@ENTRY, 109930, 0, 1, 1, 1, 1, ''), /* Mantle of Arcane Mystery */ 
(@ENTRY, 109931, 0, 1, 1, 1, 1, ''), /* Frost-Touched Shoulderpads */ 
(@ENTRY, 109932, 0, 1, 1, 1, 1, ''), /* Mantle of Swirling Light */ 
(@ENTRY, 109933, 0, 1, 1, 1, 1, ''), /* Lightbinder Shoulderpads */ 
(@ENTRY, 109934, 0, 1, 1, 1, 1, ''), /* Spaulders of Burning Focus */ 
(@ENTRY, 109935, 0, 1, 1, 1, 1, ''), /* Bloodfeather Spaulders */ 
(@ENTRY, 109936, 0, 1, 1, 1, 1, ''), /* Crystalbinder Shoulderpads */ 
(@ENTRY, 109937, 0, 1, 1, 1, 1, ''), /* Leafmender Mantle */ 
(@ENTRY, 109938, 0, 1, 1, 1, 1, ''), /* Blackwater Spaulders */ 
(@ENTRY, 109939, 0, 1, 1, 1, 1, ''), /* Streamslither Spaulders */ 
(@ENTRY, 109940, 0, 1, 1, 1, 1, ''), /* Sharpeye Shoulderguards */ 
(@ENTRY, 109941, 0, 1, 1, 1, 1, ''), /* Rockhide Shoulderguards */ 
(@ENTRY, 109942, 0, 1, 1, 1, 1, ''), /* Lavalink Spaulders */ 
(@ENTRY, 109943, 0, 1, 1, 1, 1, ''), /* Incarnadine Shoulderguard */ 
(@ENTRY, 109944, 0, 1, 1, 1, 1, ''), /* Verdant Plate Spaulders */ 
(@ENTRY, 109945, 0, 1, 1, 1, 1, ''), /* Goldsteel Shouldercaps */ 
(@ENTRY, 109946, 0, 1, 1, 1, 1, ''), /* Gutcrusher Shoulderplates */ 
(@ENTRY, 109947, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Shoulderplates */ 
(@ENTRY, 109948, 0, 1, 1, 1, 1, ''), /* Felflame Spaulders */ 
(@ENTRY, 109949, 0, 1, 1, 1, 1, ''), /* Morningscale Spaulders */ 
(@ENTRY, 110011, 0, 1, 1, 1, 1, ''), /* Fires of the Sun */ 
(@ENTRY, 110033, 0, 1, 1, 1, 1, ''), /* Arcanic of the High Sage */ 
(@ENTRY, 110034, 0, 1, 1, 1, 1, ''), /* Viryx's Indomitable Bulwark */ 
(@ENTRY, 114780, 0, 1, 1, 1, 1, ''); /* Pure Solium Band */ 

-- Normal loots.
SET @ENTRY = 234164;
UPDATE gameobject_template SET data1 = @ENTRY WHERE entry=@ENTRY;
DELETE from `gameobject_loot_template` WHERE entry=@ENTRY;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES (@ENTRY, 1, 100, 1, 0, -@ENTRY, 2, ''); 
DELETE FROM `reference_loot_template` where entry=@ENTRY;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(@ENTRY, 109844, 0, 1, 1, 1, 1, ''), /* Gloves of Arcane Mystery */
(@ENTRY, 109845, 0, 1, 1, 1, 1, ''), /* Frost-Touched Gloves */
(@ENTRY, 109846, 0, 1, 1, 1, 1, ''), /* Gloves of Swirling Light */
(@ENTRY, 109847, 0, 1, 1, 1, 1, ''), /* Lightbinder Gloves */
(@ENTRY, 109848, 0, 1, 1, 1, 1, ''), /* Gauntlets of Burning Focus */
(@ENTRY, 109849, 0, 1, 1, 1, 1, ''), /* Bloodfeather Grips */
(@ENTRY, 109850, 0, 1, 1, 1, 1, ''), /* Crystalbinder Gloves */
(@ENTRY, 109851, 0, 1, 1, 1, 1, ''), /* Leafmender Grips */
(@ENTRY, 109852, 0, 1, 1, 1, 1, ''), /* Streamslither Gauntlets */
(@ENTRY, 109853, 0, 1, 1, 1, 1, ''), /* Sharpeye Gauntlets */
(@ENTRY, 109854, 0, 1, 1, 1, 1, ''), /* Rockhide Gloves */
(@ENTRY, 109855, 0, 1, 1, 1, 1, ''), /* Lavalink Grips */
(@ENTRY, 109856, 0, 1, 1, 1, 1, ''), /* Incarnadine Gauntlets */
(@ENTRY, 109857, 0, 1, 1, 1, 1, ''), /* Verdant Plate Grips */
(@ENTRY, 109858, 0, 1, 1, 1, 1, ''), /* Goldsteel Gloves */
(@ENTRY, 109859, 0, 1, 1, 1, 1, ''), /* Gutcrusher Gauntlets */
(@ENTRY, 109860, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Crushers */
(@ENTRY, 109861, 0, 1, 1, 1, 1, ''), /* Felflame Grips */
(@ENTRY, 109862, 0, 1, 1, 1, 1, ''), /* Blackwater Grips */
(@ENTRY, 109863, 0, 1, 1, 1, 1, ''), /* Morningscale Gauntlet */
(@ENTRY, 109864, 0, 1, 1, 1, 1, ''), /* Bracers of Arcane Mystery */
(@ENTRY, 109865, 0, 1, 1, 1, 1, ''), /* Frost-Touched Wristwraps */
(@ENTRY, 109866, 0, 1, 1, 1, 1, ''), /* Bracers of Swirling Light */
(@ENTRY, 109867, 0, 1, 1, 1, 1, ''), /* Lightbinder Wristwraps */
(@ENTRY, 109868, 0, 1, 1, 1, 1, ''), /* Bracers of Burning Focus */
(@ENTRY, 109869, 0, 1, 1, 1, 1, ''), /* Bloodfeather Bracers */
(@ENTRY, 109870, 0, 1, 1, 1, 1, ''), /* Crystalbinder Wristguards */
(@ENTRY, 109871, 0, 1, 1, 1, 1, ''), /* Leafmender Wraps */
(@ENTRY, 109872, 0, 1, 1, 1, 1, ''), /* Streamslither Bracers */
(@ENTRY, 109873, 0, 1, 1, 1, 1, ''), /* Sharpeye Bracers */
(@ENTRY, 109874, 0, 1, 1, 1, 1, ''), /* Rockhide Wristguards */
(@ENTRY, 109875, 0, 1, 1, 1, 1, ''), /* Lavalink Bracers */
(@ENTRY, 109876, 0, 1, 1, 1, 1, ''), /* Incarnadine Bracers */
(@ENTRY, 109877, 0, 1, 1, 1, 1, ''), /* Verdant Plate Wristguards */
(@ENTRY, 109878, 0, 1, 1, 1, 1, ''), /* Goldsteel Bindings */
(@ENTRY, 109879, 0, 1, 1, 1, 1, ''), /* Gutcrusher Bracers */
(@ENTRY, 109880, 0, 1, 1, 1, 1, ''), /* Rivet-Sealed Bracers */
(@ENTRY, 109881, 0, 1, 1, 1, 1, ''), /* Felflame Bracers */
(@ENTRY, 109882, 0, 1, 1, 1, 1, ''), /* Blackwater Wristguards */
(@ENTRY, 109883, 0, 1, 1, 1, 1, ''), /* Morningscale Bracers */
(@ENTRY, 110006, 0, 1, 1, 1, 1, ''), /* Rukhran's Quill */
(@ENTRY, 110032, 0, 1, 1, 1, 1, ''); /* Beakbreaker Scimitar */