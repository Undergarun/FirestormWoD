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
    SPELL_ANNIHILAT                     = 144969,
    SPELL_BERSERK_RAID_FINDER           = 64238,
    SPELL_CALL_BOMBARDEMENT_25          = 147120,
    SPELL_CRUSHING_FEAR_10              = 147324,
    SPELL_DESECRATE                     = 144748,
    SPELL_EMPOWERED_DESECRATE           = 145829,
    SPELL_EMPOWERED_GRIPPING_DESPAIR    = 145195,
    SPELL_EMPOWERED_TOUCH_OF_YSHAARJ    = 145171,
    SPELL_EMPOWERED_WHIRLING_CORRUPTION = 145037,
    SPELL_GRIPPING_DESPAIR              = 145183,
    SPELL_HELLSCREAMS_WARSONG           = 144821,
    SPELL_MALEFICE_25                   = 147209,
    SPELL_MANIFEST_RAGE_25              = 147011,
    SPELL_REALM_OF_YSHAARJ              = 144954,
    SPELL_SUMMON_FADING_BREATH          = 147296,
    SPELL_TOUCH_OH_YSHAARJ              = 145065,
    SPELL_WEAK_MINDED                   = 145331,
    SPELL_WHIRLING_CORRUPTION           = 144985,
    SPELL_SHAARJS_PROTECTION            = 144945
};

class boss_garrosh_hellscream : public CreatureScript
{
    public:
        boss_garrosh_hellscream() : CreatureScript("boss_garrosh_hellscream") { }

        struct boss_garrosh_hellscreamAI : public BossAI
        {
            boss_garrosh_hellscreamAI(Creature* creature) : BossAI(creature, DATA_GARROSH_HELLSCREAM)
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
            return new boss_garrosh_hellscreamAI(creature);
        }
};

class mob_siege_engineer : public CreatureScript
{
    public:
        mob_siege_engineer() : CreatureScript("mob_siege_engineer") { }

        struct mob_siege_engineerAI : public ScriptedAI
        {
            mob_siege_engineerAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_siege_engineerAI(creature);
        }
};

class mob_korkron_warbringer : public CreatureScript
{
    public:
        mob_korkron_warbringer() : CreatureScript("mob_korkron_warbringer") { }

        struct mob_korkron_warbringerAI : public ScriptedAI
        {
            mob_korkron_warbringerAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_korkron_warbringerAI(creature);
        }
};

class mob_wolf_rider_farseer : public CreatureScript
{
    public:
        mob_wolf_rider_farseer() : CreatureScript("mob_wolf_rider_farseer") { }

        struct mob_wolf_rider_farseerAI : public ScriptedAI
        {
            mob_wolf_rider_farseerAI(Creature* creature) : ScriptedAI(creature)
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
            return new mob_wolf_rider_farseerAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_Garrosh_Hellscream()
{
    new boss_garrosh_hellscream();
    new mob_siege_engineer();
    new mob_korkron_warbringer();
    new mob_wolf_rider_farseer();
}
#endif
