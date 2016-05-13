UPDATE creature_template SET dmg_multiplier = 10, ScriptName = 'npc_ashran_warspear_headhunter' WHERE entry = 88691;
UPDATE creature_template SET dmg_multiplier = 10, ScriptName = 'npc_ashran_stormshield_sentinel' WHERE entry = 86767;
UPDATE creature_template SET faction = 122 WHERE entry = 79947;

DELETE FROM creature WHERE id = 85811 AND map = 1191;
DELETE FROM creature WHERE id = 85812 AND map = 1191;