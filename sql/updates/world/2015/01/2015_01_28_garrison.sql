UPDATE creature_loot_template SET ChanceOrQuestChance = ChanceOrQuestChance * -1 WHERE item=113681;

UPDATE `creature_template` SET `spell1`='82707',`spell2`='18651',`spell3`='6660',`equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='87518';
UPDATE `creature_template` SET `spell1`='78622',`spell2`='83015',`spell3`='8242',`equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='87519';
UPDATE `creature_template` SET `spell1`='15284', `equipment_id`='1', ScriptName="npc_CallToArms_Garr" WHERE `entry`='80258';

REPLACE INTO creature_equip_template (entry, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(87518, 1, 1899, 0, 111783);
REPLACE INTO creature_equip_template (entry, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(87519, 1, 1899, 7188, 0);
REPLACE INTO creature_equip_template (entry, `id`, `itemEntry1`, `itemEntry2`, `itemEntry3`) VALUES(80258, 1, 2877, 0, 111783);