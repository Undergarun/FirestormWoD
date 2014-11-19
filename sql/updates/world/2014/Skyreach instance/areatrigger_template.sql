ALTER TABLE areatrigger_template
	ADD (ScriptName varchar(50));

insert into areatrigger_template values ('153589', '0', '6038', '257', '0', '2', '17026', '514', '0', '504', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'at_spinning_blade');
insert into areatrigger_template values ('153536', '0', '6038', '257', '0', '2', '17026', '515', '0', '504', '0', '0', '0', '0', '0', '0', '0', '0', '0', 'at_spinning_blade');

insert into areatrigger_template values ('160935', '0', '6743', '257', '5', '5', '16384', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '');


update areatrigger_template set eff_index=1 where spell_id=153905;