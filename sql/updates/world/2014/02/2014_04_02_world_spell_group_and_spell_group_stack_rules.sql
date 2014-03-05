DELETE FROM `spell_group` WHERE `id` = 1121;
INSERT INTO `spell_group` ( `id`, `spell_id`) VALUES
(1121, 77761),
(1121, 77764),
(1121, 106898),
(1121, 1850),
(1121, 68992),
(1121, 113073);

DELETE FROM `spell_group_stack_rules` WHERE `group_id` = 1121;
INSERT INTO `spell_group_stack_rules` ( `stack_rule`, `group_id`) VALUES (1, 1121);