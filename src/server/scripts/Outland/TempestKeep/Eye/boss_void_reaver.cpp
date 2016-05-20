////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Boss_Void_Reaver
SD%Complete: 90
SDComment: Should reset if raid are out of room.
SDCategory: Tempest Keep, The Eye
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "the_eye.h"

enum eEnums
{
    SAY_AGGRO                   = -1550000,
    SAY_SLAY1                   = -1550001,
    SAY_SLAY2                   = -1550002,
    SAY_SLAY3                   = -1550003,
    SAY_DEATH                   = -1550004,
    SAY_POUNDING1               = -1550005,
    SAY_POUNDING2               = -1550006,

    SPELL_POUNDING              = 34162,
    SPELL_ARCANE_ORB            = 34172,
    SPELL_KNOCK_AWAY            = 25778,
    SPELL_BERSERK               = 27680
};

class boss_void_reaver : public CreatureScript
{
    public:

        boss_void_reaver()
            : CreatureScript("boss_void_reaver")
        {
        }

        struct boss_void_reaverAI : public ScriptedAI
        {
            boss_void_reaverAI(Creature* creature) : ScriptedAI(creature)
            {
                instance = creature->GetInstanceScript();
            }

            InstanceScript* instance;

            uint32 Pounding_Timer;
            uint32 ArcaneOrb_Timer;
            uint32 KnockAway_Timer;
            uint32 Berserk_Timer;

            bool Enraged;

            void Reset()
            {
                Pounding_Timer = 15000;
                ArcaneOrb_Timer = 3000;
                KnockAway_Timer = 30000;
                Berserk_Timer = 600000;

                Enraged = false;

                if (instance && me->isAlive())
                    instance->SetData(DATA_VOIDREAVEREVENT, NOT_STARTED);
            }

            void KilledUnit(Unit* /*victim*/)
            {
                DoScriptText(RAND(SAY_SLAY1, SAY_SLAY2, SAY_SLAY3), me);
            }

            void JustDied(Unit* /*killer*/)
            {
                DoScriptText(SAY_DEATH, me);
                DoZoneInCombat();

                if (instance)
                    instance->SetData(DATA_VOIDREAVEREVENT, DONE);
            }

            void EnterCombat(Unit* /*who*/)
            {
                DoScriptText(SAY_AGGRO, me);

                if (instance)
                    instance->SetData(DATA_VOIDREAVEREVENT, IN_PROGRESS);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;
                // Pounding
                if (Pounding_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_POUNDING);
                    DoScriptText(RAND(SAY_POUNDING1, SAY_POUNDING2), me);
                    Pounding_Timer = 15000; //cast time(3000) + cooldown time(12000)
                }
                else
                    Pounding_Timer -= diff;
                // Arcane Orb
                if (ArcaneOrb_Timer <= diff)
                {
                    Unit* target = NULL;
                    std::list<HostileReference*> t_list = me->getThreatManager().getThreatList();
                    std::vector<Unit*> target_list;
                    for (std::list<HostileReference*>::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                    {
                        target = Unit::GetUnit(*me, (*itr)->getUnitGuid());
                        if (!target)
                            continue;
                        // exclude pets & totems, 18 yard radius minimum
                        if (target->IsPlayer() && target->isAlive() && !target->IsWithinDist(me, 18, false))
                            target_list.push_back(target);
                        target = NULL;
                    }

                    if (!target_list.empty())
                        target = *(target_list.begin()+rand()%target_list.size());
                    else
                        target = me->getVictim();

                    if (target)
                        me->CastSpell(target, SPELL_ARCANE_ORB, false, NULL, nullptr, 0);
                    ArcaneOrb_Timer = 3000;
                }
                else
                    ArcaneOrb_Timer -= diff;
                // Single Target knock back, reduces aggro
                if (KnockAway_Timer <= diff)
                {
                    DoCast(me->getVictim(), SPELL_KNOCK_AWAY);
                    //Drop 25% aggro
                    if (DoGetThreat(me->getVictim()))
                        DoModifyThreatPercent(me->getVictim(), -25);
                    KnockAway_Timer = 30000;
                }
                else
                    KnockAway_Timer -= diff;
                //Berserk
                if (Berserk_Timer < diff && !Enraged)
                {
                    DoCast(me, SPELL_BERSERK);
                    Enraged = true;
                }
                else
                    Berserk_Timer -= diff;

                DoMeleeAttackIfReady();

                EnterEvadeIfOutOfCombatArea(diff);
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_void_reaverAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_void_reaver()
{
    new boss_void_reaver();
}
#endif
