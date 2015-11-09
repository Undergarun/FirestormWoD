DELETE FROM spell_script_names WHERE spell_id IN (178210, 178218, 178221, 178228, 178209, 178217, 178225, 178227, 178212, 178216, 178224, 178226, 178213, 178220, 178223, 178230, 178211, 178219, 178222, 178229);
INSERT INTO spell_script_names VALUES
(178210, 'spell_item_legs_of_iron'),
(178218, 'spell_item_legs_of_iron'),
(178221, 'spell_item_legs_of_iron'),
(178228, 'spell_item_legs_of_iron'),
(178209, 'spell_item_chest_of_iron'),
(178217, 'spell_item_chest_of_iron'),
(178225, 'spell_item_chest_of_iron'),
(178227, 'spell_item_chest_of_iron'),
(178212, 'spell_item_helm_of_iron'),
(178216, 'spell_item_helm_of_iron'),
(178224, 'spell_item_helm_of_iron'),
(178226, 'spell_item_helm_of_iron'),
(178213, 'spell_item_shoulders_of_iron'),
(178220, 'spell_item_shoulders_of_iron'),
(178223, 'spell_item_shoulders_of_iron'),
(178230, 'spell_item_shoulders_of_iron'),
(178211, 'spell_item_gauntlets_of_iron'),
(178219, 'spell_item_gauntlets_of_iron'),
(178222, 'spell_item_gauntlets_of_iron'),
(178229, 'spell_item_gauntlets_of_iron');

# Lady Liadrin <Blood Knight Matriarch>
SET @REF_LADY_LIADRIN = 76804;
DELETE FROM creature WHERE id = @REF_LADY_LIADRIN;
INSERT INTO creature (id, map, zoneId, areaId, phaseMask, spawnMask, position_x, position_y, position_z, orientation, npcflag) VALUE
(@REF_LADY_LIADRIN, 1116, 6662, 7269, 1, 1, 2496.939, 2851.589, 242.8111, 6.261039, 0x02);

# Essence of the Iron Conqueror/Protector/Vanquisher (Horde)
DELETE FROM creature_questender WHERE id = @REF_LADY_LIADRIN AND quest IN (37540, 37551, 37552, 37543, 37547, 37548, 37541, 37549, 37550);
INSERT INTO creature_questender VALUES
(@REF_LADY_LIADRIN, 37540), -- Conqueror Normal
(@REF_LADY_LIADRIN, 37551), -- Conqueror Heroic
(@REF_LADY_LIADRIN, 37552), -- Conqueror Mythic
(@REF_LADY_LIADRIN, 37543), -- Protector Normal
(@REF_LADY_LIADRIN, 37547), -- Protector Heroic
(@REF_LADY_LIADRIN, 37548), -- Protector Mythic
(@REF_LADY_LIADRIN, 37541), -- Vanquisher Normal
(@REF_LADY_LIADRIN, 37549), -- Vanquisher Heroic
(@REF_LADY_LIADRIN, 37550); -- Vanquisher Mythic

UPDATE quest_template SET SpecialFlags = SpecialFlags | 0x01 WHERE Id IN (37540, 37551, 37552, 37543, 37547, 37548, 37541, 37549, 37550);
UPDATE quest_template SET EndText = '$n, your courage and dedication has made Draenor a safer place. Take these armaments and go in the Light.' WHERE Id IN (37540, 37551, 37552, 37543, 37547, 37548, 37541, 37549, 37550);

# Exarch Maladaar <Speaker for the Dead>
SET @REF_EXARCH_MALADAAR = 75028;
DELETE FROM creature WHERE id = @REF_EXARCH_MALADAAR;
INSERT INTO creature (id, map, zoneId, areaId, phaseMask, spawnMask, position_x, position_y, position_z, orientation, npcflag) VALUE
(@REF_EXARCH_MALADAAR, 1116, 6662, 7269, 1, 1, 2495.125, 2864.642, 242.8111, 4.82859, 0x02);

# Essence of the Iron Conqueror/Protector/Vanquisher (Alliance)
DELETE FROM creature_questender WHERE id = @REF_EXARCH_MALADAAR AND quest IN (37553, 37557, 37560, 37555, 37559, 37562, 37554, 37558, 37561);
INSERT INTO creature_questender VALUES
(@REF_EXARCH_MALADAAR, 37553), -- Conqueror Normal
(@REF_EXARCH_MALADAAR, 37557), -- Conqueror Heroic
(@REF_EXARCH_MALADAAR, 37560), -- Conqueror Mythic
(@REF_EXARCH_MALADAAR, 37555), -- Protector Normal
(@REF_EXARCH_MALADAAR, 37559), -- Protector Heroic
(@REF_EXARCH_MALADAAR, 37562), -- Protector Mythic
(@REF_EXARCH_MALADAAR, 37554), -- Vanquisher Normal
(@REF_EXARCH_MALADAAR, 37558), -- Vanquisher Heroic
(@REF_EXARCH_MALADAAR, 37561); -- Vanquisher Mythic

UPDATE quest_template SET SpecialFlags = SpecialFlags | 0x01 WHERE Id IN (37553, 37557, 37560, 37555, 37559, 37562, 37554, 37558, 37561);
UPDATE quest_template SET EndText = '$n, your courage and dedication has made Draenor a safer place. Take these armaments, and may the Light protect you on your journey.' WHERE Id IN (37553, 37557, 37560, 37555, 37559, 37562, 37554, 37558, 37561);

UPDATE creature_template SET npcflag = npcflag | 0x02 WHERE entry IN (@REF_LADY_LIADRIN, @REF_EXARCH_MALADAAR);