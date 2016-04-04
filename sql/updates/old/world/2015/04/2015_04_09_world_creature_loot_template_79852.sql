-- Boss OSHIR
SET @REF_OSHIR    = 79852;

UPDATE `creature_template` SET `lootid`=@REF_OSHIR WHERE `entry`=@REF_OSHIR;
DELETE FROM `creature_loot_template` WHERE `entry`=@REF_OSHIR;
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(@REF_OSHIR, 1, 100, 1, 0, -@REF_OSHIR, 2, ''),
(@REF_OSHIR, 113681, 7, 1, 0, 2, 10, '') /* Iron Horde Scraps */;

DELETE FROM `reference_loot_template` WHERE entry = @REF_OSHIR;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`, `itemBonuses`) VALUES
(@REF_OSHIR, 109824, 0, 1, 1, 1, 1, '') /* Cord of Arcane Mystery */, 
(@REF_OSHIR, 109825, 0, 1, 1, 1, 1, '') /* Frost-Touched Cord */, 
(@REF_OSHIR, 109826, 0, 1, 1, 1, 1, '') /* Cord of Swirling Light */, 
(@REF_OSHIR, 109827, 0, 1, 1, 1, 1, '') /* Lightbinder Girdle */, 
(@REF_OSHIR, 109828, 0, 1, 1, 1, 1, '') /* Felflame Belt */, 
(@REF_OSHIR, 109829, 0, 1, 1, 1, 1, '') /* Belt of Burning Focus */, 
(@REF_OSHIR, 109830, 0, 1, 1, 1, 1, '') /* Bloodfeather Girdle */, 
(@REF_OSHIR, 109831, 0, 1, 1, 1, 1, '') /* Crystalbinder Belt */, 
(@REF_OSHIR, 109832, 0, 1, 1, 1, 1, '') /* Leafmender Girdle */, 
(@REF_OSHIR, 109833, 0, 1, 1, 1, 1, '') /* Streamslither Belt */, 
(@REF_OSHIR, 109834, 0, 1, 1, 1, 1, '') /* Sharpeye Belt */, 
(@REF_OSHIR, 109835, 0, 1, 1, 1, 1, '') /* Rockhide Links */, 
(@REF_OSHIR, 109836, 0, 1, 1, 1, 1, '') /* Lavalink Girdle */, 
(@REF_OSHIR, 109837, 0, 1, 1, 1, 1, '') /* Incarnadine Girdle */, 
(@REF_OSHIR, 109838, 0, 1, 1, 1, 1, '') /* Verdant Plate Belt */, 
(@REF_OSHIR, 109839, 0, 1, 1, 1, 1, '') /* Goldsteel Belt */, 
(@REF_OSHIR, 109840, 0, 1, 1, 1, 1, '') /* Gutcrusher Greatbelt */, 
(@REF_OSHIR, 109841, 0, 1, 1, 1, 1, '') /* Rivet-Sealed Waistplate */, 
(@REF_OSHIR, 109842, 0, 1, 1, 1, 1, '') /* Blackwater Belt */, 
(@REF_OSHIR, 109843, 0, 1, 1, 1, 1, '') /* Morningscale Waistguard */, 
(@REF_OSHIR, 109864, 0, 1, 1, 1, 1, '') /* Bracers of Arcane Mystery */, 
(@REF_OSHIR, 109865, 0, 1, 1, 1, 1, '') /* Frost-Touched Wristwraps */, 
(@REF_OSHIR, 109866, 0, 1, 1, 1, 1, '') /* Bracers of Swirling Light */, 
(@REF_OSHIR, 109867, 0, 1, 1, 1, 1, '') /* Lightbinder Wristwraps */, 
(@REF_OSHIR, 109868, 0, 1, 1, 1, 1, '') /* Bracers of Burning Focus */, 
(@REF_OSHIR, 109869, 0, 1, 1, 1, 1, '') /* Bloodfeather Bracers */, 
(@REF_OSHIR, 109870, 0, 1, 1, 1, 1, '') /* Crystalbinder Wristguards */, 
(@REF_OSHIR, 109871, 0, 1, 1, 1, 1, '') /* Leafmender Wraps */, 
(@REF_OSHIR, 109872, 0, 1, 1, 1, 1, '') /* Streamslither Bracers */, 
(@REF_OSHIR, 109873, 0, 1, 1, 1, 1, '') /* Sharpeye Bracers */, 
(@REF_OSHIR, 109874, 0, 1, 1, 1, 1, '') /* Rockhide Wristguards */, 
(@REF_OSHIR, 109875, 0, 1, 1, 1, 1, '') /* Lavalink Bracers */, 
(@REF_OSHIR, 109876, 0, 1, 1, 1, 1, '') /* Incarnadine Bracers */, 
(@REF_OSHIR, 109877, 0, 1, 1, 1, 1, '') /* Verdant Plate Wristguards */, 
(@REF_OSHIR, 109878, 0, 1, 1, 1, 1, '') /* Goldsteel Bindings */, 
(@REF_OSHIR, 109879, 0, 1, 1, 1, 1, '') /* Gutcrusher Bracers */, 
(@REF_OSHIR, 109880, 0, 1, 1, 1, 1, '') /* Rivet-Sealed Bracers */, 
(@REF_OSHIR, 109881, 0, 1, 1, 1, 1, '') /* Felflame Bracers */, 
(@REF_OSHIR, 109882, 0, 1, 1, 1, 1, '') /* Blackwater Wristguards */, 
(@REF_OSHIR, 109883, 0, 1, 1, 1, 1, '') /* Morningscale Bracers */, 
(@REF_OSHIR, 109997, 0, 1, 1, 1, 1, '') /* Kihra's Adrenaline Injector */, 
(@REF_OSHIR, 110057, 0, 1, 1, 1, 1, '') /* Mindbreaker Scepter */; 
