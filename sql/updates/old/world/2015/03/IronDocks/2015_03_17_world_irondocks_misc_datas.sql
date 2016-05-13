-- Spell Script Names
DELETE FROM spell_script_names WHERE spell_id IN (178412, 167299, 167239, 164370, 172810, 172885, 168537, 168227, 169132, 163665, 98928, 163668, 178154, 156637, 163689, 163740, 168965, 172810, 164504);
INSERT INTO spell_script_names VALUES
(178412, 'iron_docks_spell_flurry_periodic'),
(167299, 'iron_docks_spell_charge_forward'),
(167239, 'iron_docks_spell_shoot_damage_decrease_on_dummies'),
(164370, 'iron_docks_spell_barbed_arrow_aura'),
(172810, 'iron_docks_spell_barbed_arrow_dummy'),
(172885, 'iron_docks_spell_charging_slash_effect'),
(168537, 'spell_barrage_targets'),
(168227, 'spell_gronn_smash'),
(169132, 'spell_back_draft'),
(163665, 'spell_flaming_slash_damage_targets'),
(98928, 'iron_docks_flame_wave_restriction'),
(163668, 'spell_flaming_slash_damage_target_change'),
(178154, 'spell_acid_spew'),
(156637, 'spell_rapid_fire_damage'),
(163689, 'spell_sanguine_sphere'),
(163740, 'spell_tainted_blood_damage_target_change'),
(168965, 'koramar_berserker_jump'),
(172810, 'iron_docks_spell_burning_arrow_aura'),
(164504, 'iron_docks_intimidated');

-- instance_template
DELETE FROM instance_template WHERE map = 1195;
INSERT INTO instance_template (map, parent, script, allowMount) VALUE
(1195, 0, 'instance_iron_docks', 0);

-- areatrigger teleport
DELETE FROM areatrigger_teleport WHERE id IN (10098, 10099, 10284);
INSERT INTO `areatrigger_teleport` (`id`, `name`, `target_map`, `target_position_x`, `target_position_y`, `target_position_z`, `target_orientation`) VALUES
(10098, 'Iron Docks Entrance', 1195, 6751.96, -557.81, 4.88767, 0),
(10099, 'Iron Docks Exit', 1116, 8852.49, 1364.35, 97, 1.581),
(10284, 'Iron Docks Exit', 1116, 8852.49, 1364.35, 97, 1.581);

-- areatrigger template
DELETE FROM areatrigger_template WHERE spell_id IN (164628, 164278, 173484);
INSERT INTO areatrigger_template (spell_id, eff_index, entry, scale_x, scale_y, flags, ScriptName) VALUES
(173484, 0, 7898, 5, 5, 16384, 'iron_docks_area_lava_barrage_effect'),
(164278, 0, 7198, 7.5, 7.5, 16384, 'iron_docks_area_trigger_barbed_arrow'),
(164628, 0, 7224, 3, 3, 16384, 'iron_docks_area_trigger_burning_arrow');