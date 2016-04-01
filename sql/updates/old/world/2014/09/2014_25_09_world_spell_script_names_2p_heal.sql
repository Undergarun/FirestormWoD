DELETE FROM spell_script_names WHERE ScriptName = 'spell_pri_item_s12_2p_heal' OR ScriptName = 'spell_pri_item_s12_4p_heal';
INSERT INTO `spell_script_names`
            (`spell_id`,
             `ScriptName`)
VALUES ('33076',
        'spell_pri_item_s12_2p_heal');