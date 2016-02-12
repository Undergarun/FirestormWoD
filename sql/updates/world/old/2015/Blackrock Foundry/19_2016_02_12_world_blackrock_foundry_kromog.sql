-- Templates to update:
-- 77844 (Rune de Terre broyeuse
-- 77929 (Réverbération)
-- 78055 (Choc ondoyant)
-- 77692 (Kromog)

/*
Aggro
    Oggum dror undro dukkar.
    
Abilities
    DESTRUCTION!
    LANDSLIDE!
    Bury you!
    
Thundering Blows
    MOGGOK DOOM AGURR!
    
Killing a player
    Into... nothing...
    To dust....
    
Frenzy
    Smash you... into mountain.
    
Death
    RRRRRRRRRrrooaaaarrr...
    
Blackhand yells: That beast was irreplaceable. Where are the attackers now? I want them dead. DEAD!

Kromog begins to cast [Rippling Smash]!
Kromog begins to channel [Rune of Grasping Earth]!
*/

DELETE FROM spell_script_names WHERE spell_id IN (156704, 162348);
INSERT INTO spell_script_names VALUES
(156704, "spell_foundry_slam"),
(162348, "spell_foundry_fists_of_stone");