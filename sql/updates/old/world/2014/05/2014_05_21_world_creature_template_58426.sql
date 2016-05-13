#58426/NPC - Cloud Ring

	UPDATE `creature_template` SET `InhabitType` = 4, `ScriptName` = 'npc_cloud_ring' WHERE `entry` = 58426;
	INSERT INTO `creature_template_addon` (`entry`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES('58426','0','0','0','0','0','129282');
	UPDATE `creature_template` SET `modelid1` = 11686, `modelid2` = 7029 WHERE `entry` = 58426;
	
