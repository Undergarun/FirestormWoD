/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2006-2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

class mob_yenniku : public CreatureScript
{
    public:
        mob_yenniku() : CreatureScript("mob_yenniku") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_yennikuAI (creature);
        }

        struct mob_yennikuAI : public ScriptedAI
        {
            mob_yennikuAI(Creature* creature) : ScriptedAI(creature)
            {
                bReset = false;
            }

            uint32 Reset_Timer;
            bool bReset;

            void Reset()
            {
                Reset_Timer = 0;
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
            }

            void SpellHit(Unit* caster, const SpellInfo* spell)
            {
                if (caster->GetTypeId() == TYPEID_PLAYER)
                {
                    if (!bReset && CAST_PLR(caster)->GetQuestStatus(592) == QUEST_STATUS_INCOMPLETE && spell->Id == 3607)
                    {
                        me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);
                        me->CombatStop();                   //stop combat
                        me->DeleteThreatList();             //unsure of this
                        me->setFaction(83);                 //horde generic

                        bReset = true;
                        Reset_Timer = 60000;
                    }
                }
                return;
            }

            void EnterCombat(Unit* /*who*/) { }

            void UpdateAI(const uint32 diff)
            {
                if (bReset)
                {
                    if (Reset_Timer <= diff)
                    {
                        EnterEvadeMode();
                        bReset = false;
                        me->setFaction(28);                     //troll, bloodscalp
                        return;
                    }
                    else Reset_Timer -= diff;

                    if (me->isInCombat() && me->getVictim())
                    {
                        if (me->getVictim()->GetTypeId() == TYPEID_PLAYER)
                        {
                            Unit* victim = me->getVictim();
                            if (CAST_PLR(victim)->GetTeam() == HORDE)
                            {
                                me->CombatStop();
                                me->DeleteThreatList();
                            }
                        }
                    }
                 }

                //Return since we have no target
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();
            }
        };
};

#define QUEST_TAUREN_PIRATE     26630
#define GOSSIP_CHOICE_1 "Bossy, I need to ask something great of you. I need to cut off your head to prove my loyalty to the Bloodsail Buccaneers, so they will allow me to infiltrate their ranks and hopefully discover their true intentions. Booty Bay needs you, Bossy... now more than ever."
#define GOSSIP_CHOICE_2 "Moo."
#define GOSSIP_CHOICE_3 "<Do the deed.>"
#define GOSSIP_CHOICE_4 "<Chicken out.>"

class mob_bossy : public CreatureScript
{
    public:
        mob_bossy() : CreatureScript("mob_bossy") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(2, creature->GetGUID());
            }

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_3, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 3);
                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_4, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 4);
                        player->SEND_GOSSIP_MENU(2, creature->GetGUID());
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF +3:
                    player->CLOSE_GOSSIP_MENU();
                    if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
                        creature->CastSpell(player, 81311, true);

                    break;
                default:
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }

            return true;
        }
};

#define GOSSIP_CHOICE_5 "I need an extra-large pirate hat. Seahorn's orders."

class mob_narkk : public CreatureScript
{
    public:
        mob_narkk() : CreatureScript("mob_narkk") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_5, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(2, creature->GetGUID());
            }
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case GOSSIP_ACTION_INFO_DEF + 1:
                    if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
                        creature->CastSpell(player, 81315, true);

                    break;
                default:
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }

            return true;
        }
};

#define GOSSIP_CHOICE_6 "Do you have the items I asked you ?"

class mob_fleet_master_seahorn : public CreatureScript
{
    public:
        mob_fleet_master_seahorn() : CreatureScript("mob_fleet_master_seahorn") { }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch (action)
            {
                case 1:
                    if (player->HasItemCount(59148, 1) && player->HasItemCount(59147, 1))
                        if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(2487, 0);

                    break;
                default:
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }

            return true;
        }
};

enum eCaptainEvents
{
    DEMORALIZING_SHOUT_EVENT = 13730,
    EXPLODING_SHOUT_EVENT    =  7896,
    SHOOT_EVENT              =  6660
};

enum eCaptainSpells
{
    DEMORALIZING_SHOUT_SPELL = 1,
    EXPLODING_SHOUT_SPELL    = 2,
    SHOOT_SPELL              = 3
};

class npc_captain_keelhaul : public CreatureScript
{
    public:
        npc_captain_keelhaul() : CreatureScript("npc_captain_keelhaul") { }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_captain_keelhaulAI (creature);
        }

        struct npc_captain_keelhaulAI : public ScriptedAI
        {
            npc_captain_keelhaulAI(Creature* creature) : ScriptedAI(creature) { }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(DEMORALIZING_SHOUT_EVENT, 4000);
                events.ScheduleEvent(EXPLODING_SHOUT_EVENT, 9000);
                events.ScheduleEvent(SHOOT_EVENT, 4000);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Player* plr = who->ToPlayer();
                if (plr->GetQuestStatus(26631) == QUEST_STATUS_COMPLETE || plr->GetQuestStatus(26633) == QUEST_STATUS_COMPLETE || plr->GetQuestStatus(26633) == QUEST_STATUS_INCOMPLETE)
                    me->setFaction(35);
                else
                    me->setFaction(83);
            }

            void UpdateAI(const uint32 diff)
            {
                std::list<Player*> playerList;
                GetPlayerListInGrid(playerList, me, 10.0f);
                for (auto player: playerList)
                {
                    if (player->GetQuestStatus(26631) != QUEST_STATUS_COMPLETE || player->GetQuestStatus(26633) != QUEST_STATUS_COMPLETE || player->GetQuestStatus(26633) != QUEST_STATUS_INCOMPLETE)
                    {
                        //Return since we have no target
                        if (!UpdateVictim())
                            return;

                        if (me->HasUnitState(UNIT_STATE_CASTING))
                            return;

                            events.Update(diff);

                        while (uint32 eventId = events.ExecuteEvent())
                        {
                            switch (eventId)
                            {
                                case DEMORALIZING_SHOUT_EVENT:
                                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                        me->CastSpell(target, DEMORALIZING_SHOUT_SPELL, false);
                                    events.ScheduleEvent(DEMORALIZING_SHOUT_EVENT, 20000);
                                    break;
                                case EXPLODING_SHOUT_EVENT:
                                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                        me->CastSpell(me, EXPLODING_SHOUT_SPELL, false);
                                    events.ScheduleEvent(EXPLODING_SHOUT_EVENT, 20000);
                                    break;
                                case SHOOT_EVENT:
                                    if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                        me->CastSpell(target, SHOOT_SPELL, false);
                                    events.ScheduleEvent(SHOOT_EVENT, 20000);
                                    break;
                                default:
                                    break;
                            }
                        }

                        DoMeleeAttackIfReady();
                    }
                }
            }
        };
};

void AddSC_stranglethorn_vale()
{
    new mob_yenniku();
    new mob_bossy();
    new mob_narkk();
    new mob_fleet_master_seahorn();
    new npc_captain_keelhaul();
}
