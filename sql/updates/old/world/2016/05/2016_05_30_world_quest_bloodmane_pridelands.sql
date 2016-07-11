-- Quest 36660 - Bonus Objective: Bloodmane Pridelands
DELETE FROM `npc_spellclick_spells` WHERE npc_entry=86159;
INSERT INTO `npc_spellclick_spells` (npc_entry, spell_id, cast_flags, user_type) VALUES(86159, 172113, 1, 0);
UPDATE `creature_template` SET npcflag= `npcflag` | 0x01000000 WHERE entry= 86159;

DELETE FROM `spell_script_names` WHERE `spell_id` = 172113;
INSERT INTO `spell_script_names`(spell_id, ScriptName) VALUES (172113, "spell_quest_spires_of_arak_free_prisoners");
 
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=86161;
INSERT INTO `smart_scripts` (
  `entryorguid`,
  `source_type`,
  `id`,
  `link`,
  `event_type`,
  `event_phase_mask`,
  `event_chance`,
  `event_flags`,
  `event_param1`,
  `event_param2`,
  `event_param3`,
  `event_param4`,
  `action_type`,
  `action_param1`,
  `action_param2`,
  `action_param3`,
  `action_param4`,
  `action_param5`,
  `action_param6`,
  `target_type`,
  `target_param1`,
  `target_param2`,
  `target_param3`,
  `target_x`,
  `target_y`,
  `target_z`,
  `target_o`,
  `comment`
) 
VALUES
  (
    '86161',
    '0',
    '0',
    '0',
    '25',
    '0',
    '100',
    '0',
    '0',
    '0',
    '0',
    '0',
    '11',
    '171935',
    '2',
    '0',
    '0',
    '0',
    '0',
    '19',
    '86159',
    '2',
    '0',
    '0',
    '0',
    '0',
    '0',
    'RavenspeakerInitiate cast rope to PrisonerPost'
  ) ;

