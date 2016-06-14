////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

/* ScriptData
SDName: Arathi Highlands
SD%Complete: 100
SDComment: Quest support: 665
SDCategory: Arathi Highlands
EndScriptData */

/* ContentData
npc_professor_phizzlethorpe
EndContentData */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedEscortAI.h"

/*######
## npc_professor_phizzlethorpe
######*/

enum eEnums
{
    SAY_PROGRESS_1      = 0,
    SAY_PROGRESS_2      = 1,
    SAY_PROGRESS_3      = 2,
    EMOTE_PROGRESS_4    = 3,
    SAY_AGGRO           = 4,
    SAY_PROGRESS_5      = 5,
    SAY_PROGRESS_6      = 6,
    SAY_PROGRESS_7      = 7,
    EMOTE_PROGRESS_8    = 8,
    SAY_PROGRESS_9      = 9,

    EVENT_SAY_3         = 1,
    EVENT_SAY_6         = 2,
    EVENT_SAY_8         = 3,

    QUEST_GOGGLE_BOGGLE     = 26050,
    MOB_VENGEFUL_SURGE      = 2776
};

class npc_professor_phizzlethorpe : public CreatureScript
{
    public:

        npc_professor_phizzlethorpe()
            : CreatureScript("npc_professor_phizzlethorpe")
        {
        }

        struct npc_professor_phizzlethorpeAI : public npc_escortAI
        {
            npc_professor_phizzlethorpeAI(Creature* creature) : npc_escortAI(creature) {}

            void WaypointReached(uint32 waypointId)
            {
                Player* player = GetPlayerForEscort();
                if (!player)
                    return;

                switch (waypointId)
                {
                    case 6:
                        Talk(SAY_PROGRESS_2, player->GetGUID());
                        events.ScheduleEvent(EVENT_SAY_3, 3000);
                        break;
                    case 8:
                        Talk(EMOTE_PROGRESS_4);
                        me->SummonCreature(MOB_VENGEFUL_SURGE, -2065.505f, -2136.88f, 22.20362f, 1.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                        me->SummonCreature(MOB_VENGEFUL_SURGE, -2059.249f, -2134.88f, 21.51582f, 1.0f, TEMPSUMMON_CORPSE_DESPAWN, 0);
                        break;
                    case 11:
                        Talk(SAY_PROGRESS_5, player->GetGUID());
                        events.ScheduleEvent(EVENT_SAY_6, 11000);
                        break;
                    case 17:
                        Talk(SAY_PROGRESS_7, player->GetGUID());
                        events.ScheduleEvent(EVENT_SAY_8, 6000);
                        break;
                }
            }

            void JustSummoned(Creature* summoned)
            {
                summoned->AI()->AttackStart(me);
            }

            void EnterCombat(Unit* /*who*/)
            {
                Talk(SAY_AGGRO);
            }

            void UpdateAI(const uint32 diff)
            {
                Player* player = GetPlayerForEscort();
                if (!player)
                    return;

                events.Update(diff);

                while (uint32 event = events.ExecuteEvent())
                {
                    switch (event)
                    {
                        case EVENT_SAY_3:
                            Talk(SAY_PROGRESS_3, player->GetGUID());
                            break;
                        case EVENT_SAY_6:
                            Talk(SAY_PROGRESS_6, player->GetGUID());
                            SetRun();
                            break;
                        case EVENT_SAY_8:
                            Talk(EMOTE_PROGRESS_8);
                            Talk(SAY_PROGRESS_9, player->GetGUID());
                            player->GroupEventHappens(QUEST_GOGGLE_BOGGLE, me);
                            break;
                    }
                }
                npc_escortAI::UpdateAI(diff);
            }

            EventMap events;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_professor_phizzlethorpeAI(creature);
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
        {
            if (quest->GetQuestId() == QUEST_GOGGLE_BOGGLE)
            {
                creature->AI()->Talk(SAY_PROGRESS_1, player->GetGUID());
                if (npc_escortAI* pEscortAI = CAST_AI(npc_professor_phizzlethorpeAI, (creature->AI())))
                    pEscortAI->Start(false, false, player->GetGUID(), quest);

                creature->setFaction(42);
            }
            return true;
        }
};
#ifndef __clang_analyzer__
void AddSC_arathi_highlands()
{
    new npc_professor_phizzlethorpe();
}
#endif
