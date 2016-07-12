DELETE FROM spell_upgrade_item_stage WHERE ItemBonusTreeCategory IN(197, 200, 219, 204, 201, 202, 205);

INSERT INTO spell_upgrade_item_stage VALUES
(197, 2, 0, 0x20000, 	675),
(197, 4, 0, 0x1000,	685),
(197, 4, 0, 0x800000,   675),
(200, 4, 4, 0x3fa, 	685),
(219, 4, 6, 0x4000,	675),
(219, 2, 0, 0x22000, 	675),
(204, 2, 0, 0x4000000,	675),
(204, 4, 0, 0x2,	685),
(201, 4, 2, 0x103fa,	685),
(202, 4, 1, 0x103fa,	685),
(205, 4, 0, 0x804,	685);

SELECT * FROM spell_script_names WHERE ScriptName IN ("spell_skill_taladite_amplifier_upgrade", "spell_skill_burnished_essence_upgrade", "spell_skill_steelforged_essence_upgrade", "spell_skill_truesteel_essence_upgrade", "spell_skill_true_iron_trigger_upgrade", "spell_skill_linkgrease_locksprocket_upgrade", "spell_skill_weapon_crystal_upgrade", "spell_skill_hexweave_essence_upgrade", "spell_skill_upgrade_armor_or_weapon");
DELETE FROM spell_script_names WHERE ScriptName IN ("spell_skill_taladite_amplifier_upgrade", "spell_skill_burnished_essence_upgrade", "spell_skill_steelforged_essence_upgrade", "spell_skill_truesteel_essence_upgrade", "spell_skill_true_iron_trigger_upgrade", "spell_skill_linkgrease_locksprocket_upgrade", "spell_skill_weapon_crystal_upgrade", "spell_skill_hexweave_essence_upgrade", "spell_skill_upgrade_armor_or_weapon");
INSERT INTO spell_script_names VALUES
(197546, "spell_skill_upgrade_armor_or_weapon"),
(187537, "spell_skill_upgrade_armor_or_weapon"),
(187541, "spell_skill_upgrade_armor_or_weapon"),
(187539, "spell_skill_upgrade_armor_or_weapon"),
(187538, "spell_skill_upgrade_armor_or_weapon"),
(187551, "spell_skill_upgrade_armor_or_weapon"),
(187550, "spell_skill_upgrade_armor_or_weapon"),
(187552, "spell_skill_upgrade_armor_or_weapon"),
(187535, "spell_skill_upgrade_armor_or_weapon");