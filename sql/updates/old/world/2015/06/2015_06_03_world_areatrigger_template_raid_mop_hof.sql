-- Areatrigger templates for Heart of Fear
DELETE FROM areatrigger_template WHERE spell_id IN (123811, 122731);
INSERT INTO areatrigger_template (spell_id, eff_index, entry) VALUES
(123811, 0, 3242),
(122731, 0, 3198);