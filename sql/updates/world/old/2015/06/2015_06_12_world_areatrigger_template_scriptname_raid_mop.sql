DELETE FROM areatrigger_template WHERE spell_id IN (116546, 123461);
INSERT INTO areatrigger_template (spell_id, eff_index, ScriptName) VALUES
(116235, 0, 'at_amethyst_pool'),	/* Amethyst Pool - MSV - Stone Guard */
(115817, 0, 'at_nullification_barrier'),/* Nullification Barrier - MSV - Feng */	
(116546, 0, 'at_draw_power'),		/* Draw Power - MSV - Elegon */
(122731, 0, 'at_cancelling_noise'),	/* Create Noise Cancelling Area Trigger  - HOF - Zor'lok */
(123461, 0, 'at_get_away');		/* Get Away! - ToES - Lei Shi */