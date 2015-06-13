UPDATE creature_template SET ScriptName = "boss_wind_lord_meljarak", minlevel = 93, maxlevel = 93, faction_A = 16, faction_H = 16, mindmg = 25000, maxdmg = 35000, attackpower = 70000, dmg_multiplier = 5 WHERE entry = 65501;
UPDATE creature_template SET ScriptName = "npc_wind_bomb_meljarak" WHERE entry = 67053;
UPDATE creature_template SET ScriptName = "npc_srathik_amber_trapper", minlevel = 92, maxlevel = 92, faction_H = 16, faction_A = 16, mindmg = 10000, maxdmg = 22000, attackpower = 45000, dmg_multiplier = 2.5 WHERE entry = 62405;
UPDATE creature_template SET ScriptName = "npc_corrosive_resin_pool" WHERE entry = 67046;
UPDATE creature_template SET ScriptName = "npc_amber_prison_meljarak", faction_A = 16, faction_H = 16, minlevel = 92, maxlevel = 92 WHERE entry = 62531; -- faction, level
UPDATE creature_template SET ScriptName = "npc_zarthik_battle_mender", minlevel = 92, maxlevel = 92, faction_H = 16, faction_A = 16, mindmg = 10000, maxdmg = 22000, attackpower = 45000, dmg_multiplier = 2.5 WHERE entry = 62408;
UPDATE creature_template SET  minlevel = 92, maxlevel = 92, faction_H = 16, faction_A = 16, mindmg = 10000, maxdmg = 22000, attackpower = 45000, dmg_multiplier = 2.5 WHERE entry = 62402;

INSERT INTO spell_script_names (spell_id, ScriptName) VALUES (122064, "spell_meljarak_corrosive_resin");
insert into spell_linked_spell values (121885, 129078, 1, 'Summon Amber Prison when hit by aura');
