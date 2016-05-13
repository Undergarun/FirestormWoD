DELETE FROM spell_loot_template WHERE entry=171690;
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(171690, 113261, 25, 1, 0, 1, 1, '');
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(171690, 113263, 25, 1, 0, 1, 1, '');

DELETE FROM spell_script_names WHERE spell_id=171690 AND ScriptName="spell_Skill_BlackSmithing_TruesteelIngot";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES (171690, "spell_Skill_BlackSmithing_TruesteelIngot");