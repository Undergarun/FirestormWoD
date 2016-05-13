DELETE FROM spell_loot_template WHERE entry=172539;

DELETE FROM spell_script_names WHERE spell_id=172539 AND ScriptName="spell_Skill_FirstAid_AntisepticBandage";
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES (172539, "spell_Skill_FirstAid_AntisepticBandage");