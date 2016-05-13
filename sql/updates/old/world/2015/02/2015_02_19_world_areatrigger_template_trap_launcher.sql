DELETE FROM areatrigger_template WHERE spell_id IN (60202, 82938, 82940);
INSERT INTO areatrigger_template (spell_id, entry, scale_x, scale_y, flags, data0, data1, data2, data3, data4, data5, ScriptName) VALUES
(60202, 6092, 1, 1, 8192, 5, 5, 5, 5, 1, 1, 'AreaTrigger_freezing_trap'),
(82938, 6094, 1, 1, 8192, 5, 5, 5, 5, 1, 1, 'AreaTrigger_explosive_trap'),
(82940, 6096, 1, 1, 8192, 5, 5, 5, 5, 1, 1, 'AreaTrigger_ice_trap');