////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "siege_of_orgrimmar.h"

enum eSpells
{
    SPELL_ANCIENT_MIASMA           = 142861,
    SPELL_ARCING_SMASH             = 143805,
    SPELL_BLOOD_RAGE               = 142879,
    SPELL_BREATH_OF_Y_SHAARJ       = 142842,
    SPELL_DISPLACED_ENERGY         = 142913,
    SPELL_ERADICATE                = 143916,
    SPELL_EXPEL_MIASMA             = 143199,
    SPELL_FATAL_STRIKE             = 142990,
    SPELL_RELENTLESS_ASSAULT       = 143261,
    SPELL_SIESMIC_SLAM             = 142851
};

enum eEvents
{
};

enum eSays
{
    TALK_INTRO,
    TALK_AGGRO,
    TALK_ARCING_SMASH,
    TALK_BREATH_OF_YSHAARJ,
    TALK_BLOOD_RAGE,
    TALK_BERSERK,
    TALK_WIPE,
    TALK_DEATH
};

class boss_malkorok : public CreatureScript
{
    public:
        boss_malkorok() : CreatureScript("boss_malkorok") { }

        struct boss_malkorokAI : public BossAI
        {
            boss_malkorokAI(Creature* creature) : BossAI(creature, DATA_MALKOROK)
            {
                pInstance = creature->GetInstanceScript();
            }
            
            EventMap events;
            InstanceScript* pInstance;
            
            void Reset()
            {
                _Reset();
                
                events.Reset();
                
                summons.DespawnAll();
                
                if (pInstance)
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
            }
            
            void JustReachedHome()
            {
                _JustReachedHome();

                if (pInstance)
                    pInstance->SetBossState(DATA_MALKOROK, FAIL);
            }
            
            void EnterCombat(Unit* /*p_Attacker*/)
            {
                // @TODO: Set in combat for other protectors
                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                    pInstance->SetBossState(DATA_MALKOROK, IN_PROGRESS);
                }
            }
            
            void JustSummoned(Creature* summon)
            {
                summons.Summon(summon);
            }

            void SummonedCreatureDespawn(Creature* summon)
            {
                summons.Despawn(summon);
            }
            
            void KilledUnit(Unit* /*p_Who*/)
            {
            }
            
            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                if (pInstance)
                {
                    pInstance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    pInstance->SetBossState(DATA_MALKOROK, DONE);
                }
            }
            
            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
            }
        };

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new boss_malkorokAI(pCreature);
        }
};
#ifndef __clang_analyzer__
void AddSC_malkorok()
{
    new boss_malkorok();
};
#endif
/*
INSERT INTO creature_text VALUES
(71454, 0, 0, "Nazgrim has failed us. Kor'kron, destroy these intruders!", 14, 0, 100, 0, 0, 'MALKOROK_INTRO'),
(71454, 1, 0, "I will never fail the Warchief!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_AGGRO'),
(71454, 2, 0, "Weaklings!", 14, 0, 100, soundId, 'MALKOROK_ARCING_SMASH_1'),
(71454, 2, 1, "You dare challenge the Warchief?", 14, 0, 100, 0, 0, soundId, 'MALKOROK_ARCING_SMASH_2'),
(71454, 2, 2, "Garrosh gives me strength!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_ARCING_SMASH_3'),
(71454, 3, 0, "Witness the might of the True Horde!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_BREATH_OF_YSHAARJ_1'),
(71454, 3, 1, "The power of the Warchief will overwhelm you!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_BREATH_OF_YSHAARJ_2'),
(71454, 4, 0, "The True Horde cannot be STOPPED!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_BLOOD_RAGE_1'),
(71454, 4, 1, "Stand and FACE ME!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_BLOOD_RAGE_2'),
(71454, 5, 0, "The overwhelming power!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_BERSERK'),
(71454, 6, 0, "The Warchief demands it!", 14, 0, 100, 0, 0, soundId, 'MALKOROK_WIPE'),
(71454, 7, 0, "To die... for the warchief... is... an... honor...", 14, 0, 100, 0, 0, soundId, 'MALKOROK_DEATH');
*/
