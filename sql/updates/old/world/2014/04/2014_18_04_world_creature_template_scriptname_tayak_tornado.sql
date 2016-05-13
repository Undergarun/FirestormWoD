-- Removing script on unsused NPC for Tempest Slash
UPDATE creature_template SET ScriptName = '' WHERE entry IN (62897, 64372, 64373);
-- Assigning the script to the NPC used by Tempest Slash
UPDATE creature_template SET scriptname = 'npc_tempest_slash_tornado' WHERE entry = 62908;