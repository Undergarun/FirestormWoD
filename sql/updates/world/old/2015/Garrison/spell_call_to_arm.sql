UPDATE `creature_template` SET `spell1`='82707',`spell2`='18651',`spell3`='6660',`equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='87518';
UPDATE `creature_template` SET `spell1`='78622',`spell2`='83015',`spell3`='8242',`equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='87519';
UPDATE `creature_template` SET `spell1`='15284', `equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='80258';

REPLACE INTO creature_equip_template (entry, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(87518, 1, 1899, 0, 111783);
REPLACE INTO creature_equip_template (entry, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(87519, 1, 1899, 7188, 0);
REPLACE INTO creature_equip_template (entry, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(80258, 1, 2877, 0, 111783);

UPDATE `creature_template` SET `spell1`='82707',`spell2`='18651',`spell3`='6660',`equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='88393';
UPDATE `creature_template` SET `spell1`='78622',`spell2`='83015',`spell3`='8242',`equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='88380';
UPDATE `creature_template` SET `spell1`='15284', `equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='88391';

INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(88380, 1, 110164, 110159, 0) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(88393, 1, 107737, 0, 111783) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);
INSERT INTO creature_equip_template (`entry`, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(88391, 1, 19900, 0, 111783) ON DUPLICATE KEY UPDATE `entry` = VALUES(`entry`), `id` = VALUES(`id`), `itemEntry1` = VALUES(`itemEntry1`), `itemEntry2` = VALUES(`itemEntry2`), `itemEntry3` = VALUES(`itemEntry3`);


