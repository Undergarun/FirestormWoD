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
   SPELL_AURA_OF_PRIDE            = 146817,
   SPELL_BANISHMENT               = 146823,
   SPELL_CORRUPTED_PRISON         = 144574,
   SPELL_IMPRISON                 = 144563,
   SPELL_MARK_OF_ARROGANCE        = 144351,
   SPELL_NATURES_BARRIER          = 145379,
   SPELL_PROJECTION               = 146822,
   SPELL_REACHING_ATTACK          = 144774,
   SPELL_SELF_REFLECTION          = 144800,
   SPELL_SWELLING_PRIDE           = 144400,
   SPELL_UNLEASHED                = 144832,
   SPELL_WOUNDED_PRIDE            = 144358
};

class boss_sha_of_pride : public CreatureScript
{
    public:
        boss_sha_of_pride() : CreatureScript("boss_sha_of_pride") { }

        struct boss_sha_of_prideAI : public BossAI
        {
            boss_sha_of_prideAI(Creature* creature) : BossAI(creature, DATA_SHA_OF_PRIDE)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_sha_of_prideAI(creature);
        }
};

class mob_reflection : public CreatureScript
{
    public:
        mob_reflection() : CreatureScript("mob_reflection") { }

        struct mob_reflectionAI : public ScriptedAI
        {
            mob_reflectionAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_reflectionAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_sha_of_pride()
{
    new boss_sha_of_pride();
    new mob_reflection();
}
#endif
