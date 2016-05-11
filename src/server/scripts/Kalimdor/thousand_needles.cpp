////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Thousand Needles
SD%Complete: 0
SDComment: Support for Quest:
SDCategory: Thousand Needles
EndScriptData */

/* ContentData
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"

class npc_pao_ka_swiftmountain : public CreatureScript
{
    public:
        npc_pao_ka_swiftmountain() : CreatureScript("npc_pao_ka_swiftmountain")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_pao_ka_swiftmountainAI(creature);
        }

        struct npc_pao_ka_swiftmountainAI : public ScriptedAI
        {
            npc_pao_ka_swiftmountainAI(Creature* creature) : ScriptedAI(creature)
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

class npc_kanati_greycloud : public CreatureScript
{
    public:
        npc_kanati_greycloud() : CreatureScript("npc_kanati_greycloud")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kanati_greycloudAI(creature);
        }

        struct npc_kanati_greycloudAI : public ScriptedAI
        {
            npc_kanati_greycloudAI(Creature* creature) : ScriptedAI(creature)
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

class npc_lakota_windsong : public CreatureScript
{
    public:
        npc_lakota_windsong() : CreatureScript("npc_lakota_windsong")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_lakota_windsongAI(creature);
        }

        struct npc_lakota_windsongAI : public ScriptedAI
        {
            npc_lakota_windsongAI(Creature* creature) : ScriptedAI(creature)
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
void AddSC_thousand_needles()
{
    new npc_lakota_windsong();
    new npc_kanati_greycloud();
    new npc_pao_ka_swiftmountain();
}
#endif
