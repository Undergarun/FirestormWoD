DELETE FROM disables WHERE sourceType = 0 AND entry = 110744;
DELETE FROM disables WHERE sourceType = 0 AND entry = 122121;
DELETE FROM areatrigger_template WHERE spell_id IN (110744, 122121);
INSERT INTO `areatrigger_template` (`spell_id`, `eff_index`, `entry`, `type`, `scale_x`, `scale_y`, `flags`, `move_curve_id`, `scale_curve_id`, `morph_curve_id`, `facing_curve_id`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `ScriptName`) VALUES
('110744','0','6700','4','4','4','16384','0','0','0','0','2500','0','0','0','0','0','0','0','at_pri_divine_star'),
('122121','0','7262','4','4','4','16384','0','0','0','0','2500','0','0','0','0','0','0','0','at_pri_divine_star');
