#113285/Spell - Dit Da Jow

	DELETE FROM spell_script_names WHERE spell_id = 113285;
	INSERT INTO spell_script_names VALUES (113285, "spell_item_dit_da_jow");
	
	UPDATE conditions SET ConditionTarget = 1 WHERE SourceEntry = 113285;
	
