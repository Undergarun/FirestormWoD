DELETE FROM spell_loot_template WHERE entry=171391;
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(171391, 113263, 25, 1, 0, 1, 1, '');
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(171391, 113262, 25, 1, 0, 1, 1, '');

DELETE FROM spell_script_names WHERE spell_id=171391 AND ScriptName="spell_Skill_Leatherworking_BurnishedLeather";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES (171391, "spell_Skill_Leatherworking_BurnishedLeather");