DELETE FROM spell_loot_template WHERE entry=169081;
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(169081, 113263, 25, 1, 0, 1, 1, '');
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(169081, 113264, 25, 1, 0, 1, 1, '');

DELETE FROM spell_script_names WHERE spell_id=169081 AND ScriptName="spell_Skill_Inscription_WarPaints";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES (169081, "spell_Skill_Inscription_WarPaints");