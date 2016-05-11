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
    SPELL_BERSERK             = 26662,
    SPELL_EXPLOSIVE_TAR       = 144492,
    SPELL_FLAME_VENTS         = 144464,
    SPELL_IGNITE_ARMOR        = 144467,
    SPELL_SEISMIC_ACTIVITY    = 144483,
    SPELL_SHOCK_PULSE         = 144485
};

enum eEvents
{
};

class boss_iron_juggernaut : public CreatureScript
{
    public:
        boss_iron_juggernaut() : CreatureScript("boss_iron_juggernaut") { }

        struct boss_iron_juggernautAI : public BossAI
        {
            boss_iron_juggernautAI(Creature* creature) : BossAI(creature, DATA_IRON_JUGGERNAUT)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_iron_juggernautAI(creature);
        }
};

class mob_crawler_mine : public CreatureScript
{
    public:
        mob_crawler_mine() : CreatureScript("mob_crawler_mine") { }

        struct mob_crawler_mineAI : public ScriptedAI
        {
            mob_crawler_mineAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
                if (!UpdateVictim())
                    return;
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_crawler_mineAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_iron_juggernaut()
{
    new boss_iron_juggernaut();
    new mob_crawler_mine();
}
#endif
