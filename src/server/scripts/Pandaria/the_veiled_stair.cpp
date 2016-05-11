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
#include "the_veiled_stair.h"


// 64616 - Wrathion
class npc_wrathion : public CreatureScript
{
    public:
        npc_wrathion() : CreatureScript("npc_wrathion")
        {
        }

        bool OnQuestAccept(Player* player, Creature* /*p_Creature*/, Quest const* quest)
        {
            if (quest->GetQuestId() == QUEST_A_LEGEND_IN_THE_MAKING)
            {
                if (player->GetTeam() == ALLIANCE)
                {
                    player->PlayScene(208, player);
                    player->KilledMonsterCredit(LEGEND_MAKING_KILL_CREDIT);
                }

                else
                {
                    player->PlayScene(234, player);
                    player->KilledMonsterCredit(LEGEND_MAKING_KILL_CREDIT);
                }
                    return true;
            }

            return true;
        }
};

// 64822 - Wrathion
class npc_wrathion_mason : public CreatureScript
{
    public:
        npc_wrathion_mason() : CreatureScript("npc_wrathion_mason")
        {
        }

        bool OnQuestReward(Player* player, Creature* creature, Quest const* quest, uint32 /*opt*/)
        {
            if (quest->GetQuestId() == 31482)
            {
                if (creature->isQuestGiver() && creature->AI())
                {
                    creature->DespawnOrUnsummon();

                    if (AchievementEntry const* AchievWC = sAchievementStore.LookupEntry(7533)) // Chapter I: Trial of the Black Prince
                        player->CompletedAchievement(AchievWC);
                }
            }
            return true;
        }

        struct npc_wrathion_masonAI : public npc_escortAI
        {
            npc_wrathion_masonAI(Creature* creature) : npc_escortAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (Player* player = summoner->ToPlayer())
                {
                    if (player->GetQuestStatus(31482) == QUEST_STATUS_INCOMPLETE)
                    {
                        SetDespawnAtFar(false);
                        SetDespawnAtEnd(false);
                        me->RemoveFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                        events.ScheduleEvent(EVENT_FIRST_MOVE, 2000);
                        playerGuid = player->GetGUID();
                    }
                }
            }

            void WaypointReached(uint32 waypointId)
            {
                if (Player* player = Player::GetPlayer(*me, playerGuid))
                {
                    switch (waypointId)
                    {
                        case 1:
                            Talk(1);
                            break;
                        case 3:
                            SetEscortPaused(true);
                            me->SetFacingTo(4.719869f);
                            me->SetOrientation(4.719869f);
                            Talk(2);
                            events.ScheduleEvent(EVENT_THIRD_TALK, 5000);
                            events.ScheduleEvent(EVENT_SECOND_MOVE, 10000);
                            break;
                        case 4:
                            me->SetFacingTo(5.442434f);
                            me->SetOrientation(5.442434f);
                            break;
                        default:
                            break;
                    }
                }
            }

            void UpdateAI(const uint32 diff)
            {
                npc_escortAI::UpdateAI(diff);
                events.Update(diff);

                if (Player* player = Player::GetPlayer(*me, playerGuid))
                {
                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                            case EVENT_FIRST_MOVE:
                                Talk(0);
                                Start(false, false, player->GetGUID());
                                break;
                            case EVENT_THIRD_TALK:
                                Talk(3);
                                events.ScheduleEvent(EVENT_PLAY_SCENE, 4000);
                                events.ScheduleEvent(EVENT_FOURTH_TALK, 7000);
                                break;
                            case EVENT_SECOND_MOVE:
                                SetEscortPaused(false);
                                break;
                            case EVENT_PLAY_SCENE:
                                player->PlayScene(245, player);
                                break;
                            case EVENT_FOURTH_TALK:
                                Talk(4);
                                events.ScheduleEvent(EVENT_FIFTH_TALK, 6000);
                                break;
                            case EVENT_FIFTH_TALK:
                                Talk(5);
                                events.ScheduleEvent(EVENT_SIXTH_TALK, 5000);
                                break;
                            case EVENT_SIXTH_TALK:
                                Talk(6);
                                events.ScheduleEvent(EVENT_SEVENTH_TALK, 10000);
                                break;
                            case EVENT_SEVENTH_TALK:
                                Talk(7);
                                events.ScheduleEvent(EVENT_EIGHTH_TALK, 8000);
                                break;
                            case EVENT_EIGHTH_TALK:
                                Talk(8);
                                events.ScheduleEvent(EVENT_FINAL, 10000);
                                break;
                            case EVENT_FINAL:
                                player->KilledMonsterCredit(64664);
                                me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        };
        
        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_wrathion_masonAI(creature);
        }
};

#ifndef __clang_analyzer__
void AddSC_the_veiled_stair()
{
    new npc_wrathion();
    new npc_wrathion_mason();
}
#endif
