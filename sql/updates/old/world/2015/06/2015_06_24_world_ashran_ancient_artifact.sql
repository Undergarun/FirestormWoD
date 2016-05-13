DELETE FROM creature_text WHERE entry = 84113 AND groupid > 3;
INSERT INTO creature_text VALUES
(84113, 4, 0, '$n (Horde) has looted the |c00FFFF00|Hspell:168506|h[Ancient Artifact]|h|r!', 16, 0, 100, 0, 0, 0, 'AncientArtifactLootedByHorde'),
(84113, 5, 0, '$n (Alliance) has looted the |c00FFFF00|Hspell:168506|h[Ancient Artifact]|h|r!', 16, 0, 100, 0, 0, 0, 'AncientArtifactLootedByAlliance');

DELETE FROM locales_creature_text WHERE entry = 84113 AND textGroup > 3;
--                                                       French     German     Spanish    Russian
INSERT INTO locales_creature_text (entry, textGroup, id, text_loc2, text_loc3, text_loc6, text_loc8) VALUES
(
    84113, 4, 0,
    '$n (Horde) a ramassé l''|c00FFFF00|Hspell:168506|h[Artéfact ancien]|h|r !',
    '$n (Horde) hat das |c00FFFF00|Hspell:168506|h[uralte Artefakt]|h|r erbeutet!',
    '¡$n (Horda) se robó el |c00FFFF00|Hspell:168506|h[Artefacto antiguo]|h|r!',
    '$n (Орда) забирает |c00FFFF00|Hspell:168506|h[Древний артефакт]|h|r!'
),
(
    84113, 5, 0,
    '$n (Alliance) a ramassé l''|c00FFFF00|Hspell:168506|h[Artéfact ancien]|h|r !',
    '$n (Allianz) hat das |c00FFFF00|Hspell:168506|h[uralte Artefakt]|h|r erbeutet!',
    '¡$n (Alianza) se robó el |c00FFFF00|Hspell:168506|h[Artefacto antiguo]|h|r!',
    '$n (Альянс) забирает |c00FFFF00|Hspell:168506|h[Древний артефакт]|h|r!'
);

DELETE FROM spell_script_names WHERE ScriptName = 'spell_ashran_ancient_artifact';
INSERT INTO spell_script_names VALUE
(168506, 'spell_ashran_ancient_artifact');