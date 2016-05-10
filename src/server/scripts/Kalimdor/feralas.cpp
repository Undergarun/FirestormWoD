////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Feralas
SD%Complete: 0
SDComment: Quest support:
SDCategory: Feralas
EndScriptData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"
#include "ScriptedGossip.h"

class npc_homing_robot_oox_22_fe : public CreatureScript
{
    public:
        npc_homing_robot_oox_22_fe() : CreatureScript("npc_homing_robot_oox_22_fe")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_homing_robot_oox_22_feAI(creature);
        }

        struct npc_homing_robot_oox_22_feAI : public ScriptedAI
        {
            npc_homing_robot_oox_22_feAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
            }

            void UpdateAI(const uint32 /*p_Diff*/)
            {
            }
        };
};

#ifndef __clang_analyzer__
void AddSC_feralas()
{
    new npc_homing_robot_oox_22_fe();
}
#endif