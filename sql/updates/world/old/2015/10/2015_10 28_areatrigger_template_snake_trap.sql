DELETE FROM areatrigger_template WHERE spell_id=34600 OR spell_id=82949;
INSERT INTO areatrigger_template (spell_id,scale_x,scale_y,flags,data0,data1,data2,data3,data4,data5,data6,data7,ScriptName,creature_visual)
VALUES (34600, 1, 1, 8192, 5, 5, 5, 5, 1, 1, 0, 0, 'at_snake_trap', 0);
INSERT INTO areatrigger_template (spell_id,scale_x,scale_y,flags,data0,data1,data2,data3,data4,data5,data6,data7,ScriptName,creature_visual)
VALUES (82949, 1, 1, 8192, 5, 5, 5, 5, 1, 1, 0, 0, 'at_snake_trap', 0);