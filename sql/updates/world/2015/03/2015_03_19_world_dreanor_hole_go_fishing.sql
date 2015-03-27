-- Abyssal Gulper School
SET @LootEntry=60132;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111659,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111664,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111671,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,110291,-100,1,1,1, ''); -- Abyssal Gulper Lunker
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116818,-100,1,1,1, ''); -- Abyssal Gulper Lunker

-- Blackwater Whiptail School
SET @LootEntry=52521;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111662,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111663,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111670,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116749,-100,1,1,1, ''); -- Blackwater Whiptail Lunker
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116817,-100,1,1,1, ''); -- Blackwater Whiptail Lunker

-- Blind Lake Sturgeon School
SET @LootEntry=52517;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111652,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111667,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111674,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116820,-100,1,1,1, ''); -- Blind Lake Lunker
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116750,-100,1,1,1, ''); -- Blind Lake Sturgeon Lunker

-- Fat Sleeper School
SET @LootEntry=52516;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111651,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111668,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111675,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116753,-100,1,1,1, ''); -- Fat Sleeper Lunker
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116821,-100,1,1,1, ''); -- Fat Sleeper Lunker

-- Fire Ammonite School
SET @LootEntry=52518;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111656,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111666,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111673,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,110291,50,1,1,1, '');   -- Fire Ammonite Bait
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116748,-100,1,1,1, ''); -- Fire Ammonite Lunker
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116819,-100,1,1,1, ''); -- Fire Ammonite Lunker

-- Jawless Skulker School
SET @LootEntry=52515;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111650,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111669,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111676,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116822,-100,1,1,1, ''); -- Jawless Skulker Lunker
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,116752,-100,1,1,1, ''); -- Jawless Skulker Lunker

-- Oily Sea Scorpion School
SET @LootEntry=56672;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111658,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111665,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111672,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,118046,0.13,1,1,1, ''); -- Rubber Duck

-- Savage Piranha Pool
SET @LootEntry=56684;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,118564,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,118565,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,118566,100,1,1,1, '');  -- Big fish

-- Sea Scorpion School
SET @LootEntry=60125;
DELETE FROM gameobject_loot_template WHERE entry=@LootEntry;
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111658,100,1,1,1, '');  -- Small fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111665,100,1,1,1, '');  -- Medium fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,111672,100,1,1,1, '');  -- Big fish
INSERT INTO gameobject_loot_template(entry,item,ChanceOrQuestChance,lootmode,mincountOrRef,maxcount,itemBonuses) VALUES (@LootEntry,122696,0.13,1,1,1, ''); -- Sea Scorpion Lunker