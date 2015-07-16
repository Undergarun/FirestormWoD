-- Get Away!
DELETE FROM areatrigger_template WHERE spell_id = 123461;
INSERT INTO `areatrigger_template` (`spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `move_curve_id`, `scale_curve_id`, `morph_curve_id`, `facing_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`, `creature_visual`) VALUES
('123461','1','3385','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','at_get_away','0');