UPDATE creature_template SET ScriptName = '' WHERE entry IN (10363, 10339, 10220, 9196, 10596, 9568, 9816, 9736, 10429, 9236, 10430, 9237);
DELETE FROM instance_template WHERE script = 'instance_blackrock_spire';
DELETE FROM creature WHERE map = 229;
DELETE FROM gameobject WHERE map = 229;