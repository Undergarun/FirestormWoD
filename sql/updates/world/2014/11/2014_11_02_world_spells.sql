DELETE FROM spell_script_names WHERE ScriptName IN
(
"spell_sha_spirit_hunt",
"spell_sha_lava_lash_spread",
"spell_sha_windfury",
"spell_sha_flametongue",
"spell_sha_improoved_flame_shock"
);

INSERT INTO spell_script_names VALUES
(33757, "spell_sha_windfury"),
(105792, "spell_sha_lava_lash_spread"),
(58877, "spell_sha_spirit_hunt"),
(10400, "spell_sha_flametongue"),
(157804, "spell_sha_improoved_flame_shock");

DELETE FROM spell_linked_spell WHERE spell_trigger IN (165339, 165341, 165344, 73680, 60103);
INSERT INTO spell_linked_spell VALUES
(165339, 114050, 0, "Ascendance - Elemental"),
(165341, 114051, 0, "Ascendance - Enhancement"),
(165344, 114052, 0, "Ascendance - Restoration"),
(73680, 73683, 0, "Unleashs Elements"),
(73680, 73681, 0, "Unleashs Elements"),
(60103, 105792, 0, "Lava Lash - Spread");

REPLACE INTO spell_proc_event (entry, SpellFamilyName, SpellFamilyMask0, SpellFamilyMask1, SpellFamilyMask2, SpellFamilyMask3, ProcFlags, CustomChance, Cooldown) VALUES
(157804, 11, 0x10000000, 0, 0, 0, 0, 0, 0),
(10400, 0, 0, 0, 0, 0, 0x00800000, 0, 0),
(33757, 0, 0, 0, 0, 0, 0, 20, 4),
(157784, 11, 0, 0, 0x04240400, 0, 0, 0, 0);



REPLACE INTO creature_template_addon VALUES (29264, 0, 0, 0, 0, 0, "58877");
UPDATE creature_template SET spell1 = 5672 WHERE entry = 3402;
UPDATE creature_template SET spell1 = 114941 WHERE entry = 59764;
UPDATE creature_template SET spell1 = 8188 WHERE entry = 5929;

DELETE FROM spell_proc_event WHERE entry = 10400;
DELETE FROM spell_bonus_data WHERE entry IN (10444);
INSERT INTO spell_bonus_data VALUES
(10444, 0, 0, 0.149, 0, "Flametongue");


-- Ascendance
-- Spirit Hint
-- Unleash Weapons
-- Lava Lash - Spread flame shock on target -> proper fix
-- Healing Stream
-- Healing Tide
-- Enhanced Unleash
-- Enhanced Weapons - Windfury
-- Enhanced Weapons - Flametongue -- TODO: fix proc from mainhand
-- Improved Flame Shock


