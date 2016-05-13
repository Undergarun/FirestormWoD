DELETE FROM spell_loot_template WHERE entry=170700;
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(170700, 113263, 25, 1, 0, 1, 1, '');
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(170700, 113264, 25, 1, 0, 1, 1, '');

DELETE FROM spell_script_names WHERE spell_id=170700 AND ScriptName="spell_Skill_Jewelcrafting_TaladiteCrystal";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES (170700, "spell_Skill_Jewelcrafting_TaladiteCrystal");