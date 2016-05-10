////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "draenor_shadowmoon_valley.hpp"
#include "GameObjectAI.h"
#include "NPCHandler.h"
#include "Vehicle.h"
#include "PhaseMgr.h"
#include "Common.h"
/*
/// 79206 - Prophet Velen
class shadowmoon_prophet_velen_eventide_questgiver : public CreatureScript
{
    public:
        shadowmoon_prophet_velen_eventide_questgiver() : CreatureScript("shadowmoon_prophet_velen_eventide_questgiver")
        {
        }

        bool OnQuestAccept(Player* p_Player, Creature* p_Creature, const Quest* p_Quest) override
        {
            if (p_Quest->GetQuestId() == Shadowmoon::eQuests::QuestFindingAFoothold)
            {
                uint32 l_PhaseMask = p_Player->GetPhaseMask();
                Position l_Pos;

                p_Creature->GetPosition(&l_Pos);
                l_PhaseMask &= ~Shadowmoon::ePhases::PhaseBeforeFirstScene;
                p_Player->SummonCreature(Shadowmoon::eCreature::NpcProphetVelen, l_Pos);

                if (Creature* l_Yrel = GetClosestCreatureWithEntry(p_Creature, Shadowmoon::eCreature::NpcYrelSpawned, 20.0f))
                {
                    Position l_Pos = *l_Yrel;
                    p_Player->SummonCreature(Shadowmoon::eCreature::NpcYrelSummon, l_Pos);
                }

                p_Player->SetPhaseMask(l_PhaseMask, true);
            }

            return false;
        }
};


/// 89319 - Prophet Velen 
class shadowmoon_prophet_velen_eventide_escort : public CreatureScript
{
    public:
        shadowmoon_prophet_velen_eventide_escort() : CreatureScript("shadowmoon_prophet_velen_eventide_escort")
        {
        }

        struct shadowmoon_prophet_velen_eventide_escortAI : public ScriptedAI
        {
            shadowmoon_prophet_velen_eventide_escortAI(Creature* creature) : ScriptedAI(creature)
            {
                m_IsSummoned = false;
                m_PlayerGuid = 0;
            }

            enum eEvents
            {
                EventCheckStopFollow = 1,
                EventContinueRun     = 2
            };

            bool m_IsSummoned;
            EventMap m_Events;
            uint64 m_PlayerGuid;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
            }

            void IsSummonedBy(Unit* p_Summoner) override
            {
                m_IsSummoned = true;

                me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                m_PlayerGuid = p_Summoner->GetGUID();

                me->GetMotionMaster()->MoveFollow(p_Summoner, 1.0f, 1.0f);

                m_Events.ScheduleEvent(eEvents::EventCheckStopFollow, 6000);
            }

        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new shadowmoon_prophet_velen_eventide_escortAI(creature);
        }
};
*/

/*void AddSC_draenor_shadowmoon_valley()
{
    new shadowmoon_prophet_velen_eventide_questgiver();
    new shadowmoon_prophet_velen_eventide_escort();
    new shadowmoon_yrel_eventide_questgiver();
    new shadowmoon_yrel_eventide_escort();
}*/
