-- Echoes of attenuation and of force and verve
UPDATE 
  creature_template 
SET
  minlevel = 93,
  maxlevel = 93,
  faction_A = 14,
  faction_H = 14,
  mindmg = 5174,
  maxdmg = 12574,
  attackpower = 62114,
  dmg_multiplier = 7,
  baseattacktime = 2000,
  rangeattacktime = 2000,
  ScriptName = "boss_zorlok" 
WHERE entry IN (65174, 65173) ;

-- Sonic Pulse
UPDATE 
  creature_template 
SET
  modelid1 = 11686,
  minlevel = 93, maxlevel = 93,
  baseattacktime = 2000, rangeattacktime = 2000,
  unit_flags = 33554432, unit_flags2 = 67110912,
  ScriptName = "mob_sonic_pulse"
WHERE entry = 63837 ;