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

enum eSays
{
};

enum eSpells
{
    SPEll_ASHEN_WALL          = 144070,
    SPEll_ASHFLAR_TOTEM       = 144290,
    SPEll_BERSERK             = 47008,
    SPEll_BLOODLUST           = 144302,
    SPEll_FOUL_STREAM         = 144090,
    SPEll_FOULSTREAM_TOTEM    = 144289,
    SPEll_FROSTSTORM_STRIKE   = 144215,
    SPEll_IRON_TOMB           = 144328,
    SPEll_POISONMIST_TOTEM    = 144288,
    SPEll_RUSTED_IRON_TOTEM   = 144291,
    SPEll_TOXIC_MIST          = 144089
};

enum eEvents
{
};

class boss_earthbreaker_haromm : public CreatureScript
{
    public:
        boss_earthbreaker_haromm() : CreatureScript("boss_earthbreaker_haromm") { }

        struct boss_earthbreaker_harommAI : public BossAI
        {
            boss_earthbreaker_harommAI(Creature* creature) : BossAI(creature, DATA_KORKRON_DARK_SHAMANS)
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
            return new boss_earthbreaker_harommAI(creature);
        }
};

class boss_bloodclaw : public CreatureScript
{
    public:
        boss_bloodclaw() : CreatureScript("boss_bloodclaw") { }

        struct boss_bloodclawAI : public BossAI
        {
            boss_bloodclawAI(Creature* creature) : BossAI(creature, DATA_KORKRON_DARK_SHAMANS)
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
            return new boss_bloodclawAI(creature);
        }
};

class mob_darkfang : public CreatureScript
{
    public:
        mob_darkfang() : CreatureScript("mob_darkfang") { }

        struct mob_darkfangAI : public ScriptedAI
        {
            mob_darkfangAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_darkfangAI(creature);
        }
};

class mob_foul_slime : public CreatureScript
{
    public:
        mob_foul_slime() : CreatureScript("mob_foul_slime") { }

        struct mob_foul_slimeAI : public ScriptedAI
        {
            mob_foul_slimeAI(Creature* pCreature) : ScriptedAI(pCreature)
            {
                pInstance = pCreature->GetInstanceScript();
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

        CreatureAI* GetAI(Creature* pCreature) const
        {
            return new mob_foul_slimeAI(pCreature);
        }
};

#ifndef __clang_analyzer__
void AddSC_korkron_dark_shamans()
{
    new boss_earthbreaker_haromm();
    new boss_bloodclaw();
    new mob_darkfang();
    new mob_foul_slime();
}
#endif