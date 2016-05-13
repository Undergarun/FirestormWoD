DELETE FROM spell_script_names WHERE scriptName IN
("spell_monk_chi_explosion_mistweaver",
"spell_monk_chi_explosion_heal",
"spell_monk_chi_explosion_mistweaver_crane",
"spell_monk_chi_explosion_windwalker",
"spell_monk_chi_explosion_brewmaster");

INSERT INTO spell_script_names VALUES
(157675, "spell_monk_chi_explosion_mistweaver"),
(182078, "spell_monk_chi_explosion_heal"),
(159620, "spell_monk_chi_explosion_mistweaver_crane"),
(152174, "spell_monk_chi_explosion_windwalker"),
(157676, "spell_monk_chi_explosion_brewmaster");

DELETE FROM areatrigger_template WHERE spell_id <= 157689 AND spell_id >= 157682;
INSERT INTO `areatrigger_template` (`spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `move_curve_id`, `scale_curve_id`, `morph_curve_id`, `facing_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`, `creature_visual`) VALUES
('157682','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157683','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157684','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157685','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157686','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157687','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157688','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0'),
('157689','0','3254','0','3','3','16384','0','0','0','0','0','0','0','0','0','0','0','0','at_healing_sphere','0');
