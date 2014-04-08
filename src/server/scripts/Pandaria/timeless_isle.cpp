#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "timeless_isle.h"

class npc_prince_anduin : public CreatureScript
{
    public:
        npc_prince_anduin() : CreatureScript("npc_prince_anduin") 
		{ 
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_prince_anduinAI(creature);
        }

        struct npc_prince_anduinAI : public ScriptedAI
        {
            npc_prince_anduinAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_PRINCE_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_PRINCE_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_PRINCE_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_PRINCE_TALK_4:
                        Talk(3);
                        break;
                    case ACTION_PRINCE_TALK_5:
                        Talk(4);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
            }
        };
};

class npc_kairoz : public CreatureScript
{
    public:
        npc_kairoz() : CreatureScript("npc_kairoz") 
		{
		}

        bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 opt)
        {
            if (quest->GetQuestId() == QUEST_TIME_KEEPER_KAIROZ_H || quest->GetQuestId() == QUEST_TIME_KEEPER_KAIROZ_A)
            {
                if (creature->isQuestGiver() && creature->AI())
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->AI()->DoAction(ACTION_CONVERSATION);
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_kairozAI(creature);
        }

        struct npc_kairozAI : public ScriptedAI
        {
            npc_kairozAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;
            uint64 playerGuid;
            bool conversationEngaged;

            void Reset()
            {
                playerGuid = 0;
                conversationEngaged = false;
            }

            void SetGUID(uint64 guid, int32 bab /*= 0*/)
            {
                playerGuid = guid;
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_KAIROZ_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_KAIROZ_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_KAIROZ_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_KAIROZ_TALK_4:
                        Talk(3);
                        break;
                    case ACTION_KAIROZ_TALK_5:
                        Talk(4);
                        break;
                    case ACTION_CONVERSATION:
                        if (!conversationEngaged)
                        {
                            conversationEngaged = true;
                            events.ScheduleEvent(EVENT_PRINCE_TALK_1, 3000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_1, 6000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_2, 11000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_2, 18000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_3, 25000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_3, 32000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_4, 39000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_4, 46000);
                            events.ScheduleEvent(EVENT_KAIROZ_TALK_5, 54000);
                            events.ScheduleEvent(EVENT_PRINCE_TALK_5, 60000);
                        }
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                if (Player* player = Player::GetPlayer(*me, playerGuid))
                {
                    std::list<Creature*> princeAnduinList;
                    GetCreatureListWithEntryInGrid(princeAnduinList, me, NPC_PRINCE_ANDUIN, 7.0f);

                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                            case EVENT_PRINCE_TALK_1:
                                if (!princeAnduinList.empty())
                                    for (auto princeAnduin: princeAnduinList)
                                        princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_1);
                                break;
                            case EVENT_KAIROZ_TALK_1:
                                DoAction(ACTION_KAIROZ_TALK_1);
                                break;
                            case EVENT_PRINCE_TALK_2:
                                if (!princeAnduinList.empty())
                                    for (auto princeAnduin: princeAnduinList)
                                        princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_2);
                                break;
                            case EVENT_KAIROZ_TALK_2:
                                DoAction(ACTION_KAIROZ_TALK_2);
                                break;
                            case EVENT_PRINCE_TALK_3:
                                if (!princeAnduinList.empty())
                                    for (auto princeAnduin: princeAnduinList)
                                        princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_3);
                                break;
                            case EVENT_KAIROZ_TALK_3:
                                DoAction(ACTION_KAIROZ_TALK_3);
                                break;
                            case EVENT_PRINCE_TALK_4:
                                if (!princeAnduinList.empty())
                                    for (auto princeAnduin: princeAnduinList)
                                        DoAction(ACTION_PRINCE_TALK_4);
                                break;
                            case EVENT_KAIROZ_TALK_4:
                                DoAction(ACTION_KAIROZ_TALK_4);
                                break;
                            case EVENT_KAIROZ_TALK_5:
                                DoAction(ACTION_KAIROZ_TALK_5);
                                break;
                            case EVENT_PRINCE_TALK_5:
                                if (!princeAnduinList.empty())
                                    for (auto princeAnduin: princeAnduinList)
                                    {
                                        princeAnduin->AI()->DoAction(ACTION_PRINCE_TALK_5);
                                        conversationEngaged = false;
                                    }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        };
};

void AddSC_timeless_isle()
{
    new npc_prince_anduin();
    new npc_kairoz();
}