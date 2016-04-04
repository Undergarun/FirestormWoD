DELETE FROM spell_loot_template WHERE entry=169092;
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(169092, 113261, 25, 1, 0, 1, 1, '');
INSERT INTO spell_loot_template(entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount, itemBonuses) VALUES(169092, 113264, 25, 1, 0, 1, 1, '');

DELETE FROM spell_script_names WHERE spell_id=169092 AND ScriptName="spell_Skill_Enchantment_TemporalCrystal";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES (169092, "spell_Skill_Enchantment_TemporalCrystal");