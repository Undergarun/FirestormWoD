DELETE FROM `spell_group` WHERE id = 1083 AND spell_id = 6673;
INSERT INTO `spell_group` (id, spell_id) VALUES (1083, 6673);
DELETE FROM `spell_group_stack_rules` WHERE group_id = 1083 AND stack_rule = 1;
INSERT INTO `spell_group_stack_rules` (group_id, stack_rule) VALUES (1083, 1);