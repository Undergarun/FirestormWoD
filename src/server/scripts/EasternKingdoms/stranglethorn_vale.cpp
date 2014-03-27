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

/* ScriptData
SDName: Stranglethorn_Vale
SD%Complete: 100
SDComment: Quest support: 592
SDCategory: Stranglethorn Vale
EndScriptData */


#include "ScriptMgr.h"
#include "ScriptedCreature.h"


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
                                                                //Yenniku's Release
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

        void EnterCombat(Unit* /*who*/) {}

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
                {
                    creature->CastSpell(player, 81311, true);
                }
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
                {
                    creature->CastSpell(player, 81315, true);
                }
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
                {
                    if (player->GetQuestStatus(QUEST_TAUREN_PIRATE) == QUEST_STATUS_INCOMPLETE)
                    {
                        player->KilledMonsterCredit(2487, 0);
                    }
                }
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
        npc_captain_keelhaulAI(Creature* creature) : ScriptedAI(creature)
        {
        }

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
            {
                me->setFaction(35);
            }

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

// The 4 following scripts are for quest 26597 "Stranglethorn Fever"

#define QUEST_STRANGLETHORN_FEVER 26597
#define GOSSIP_CHOICE_7 "Continue."
#define NPC_WITCH_DOCTOR_UNBAGWA 1449
#define NPC_ENRAGED_SILVER_GORILLA 1511
#define NPC_KONDA 1516
#define NPC_MOKK_THE_SAVAGE 1514
#define FACTION_NEUTRAL 7
#define FACTION_FRIENDLY 14

enum eEvents
{
    EVENT_ENRAGE,
    EVENT_ENRAGE_2,
    EVENT_FLING_FILTH,
    EVENT_BATTLE_SHOOT,
    EVENT_STOMP
};

enum eSpells
{
    SPELL_ENRAGE       = 63227,
    SPELL_ENRAGE_2     = 8599,
    SPELL_FLING_FILTH  = 83012,
    SPELL_BATTLE_SHOOT = 81219,
    SPELL_STOMP        = 83378
};

class mob_witch_doctor_unbagwa : public CreatureScript
{
    public:
        mob_witch_doctor_unbagwa() : CreatureScript("mob_witch_doctor_unbagwa") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(QUEST_STRANGLETHORN_FEVER) == QUEST_STATUS_INCOMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_7, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(75013, creature->GetGUID());
            }

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                std::list<Creature*> firstGorilla;
                GetCreatureListWithEntryInGrid(firstGorilla, creature, NPC_ENRAGED_SILVER_GORILLA, 30.0f);

                if (firstGorilla.empty())
                    creature->SummonCreature(NPC_ENRAGED_SILVER_GORILLA, -13771.110352f, -6.243329f, 45.640976f, creature->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_witch_doctor_unbagwaAI (creature);
        }

        struct mob_witch_doctor_unbagwaAI : public ScriptedAI
        {
            mob_witch_doctor_unbagwaAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            uint64 playerGuid;

            void MoveInLineOfSight(Unit* who)
            {
                if (who->ToPlayer())
                    playerGuid = who->GetGUID();
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = sObjectAccessor->GetPlayer(*me, playerGuid))
                {
                    if (Unit* target = player->getVictim())
                    {
                        me->AI()->AttackStart(target);
                        DoMeleeAttackIfReady();
                    }
                }
            }
        };
};

class mob_enraged_silverback_gorilla : public CreatureScript
{
    public:
        mob_enraged_silverback_gorilla() : CreatureScript("mob_enraged_silverback_gorilla")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_enraged_silverback_gorillaAI (creature);
        }

        struct mob_enraged_silverback_gorillaAI : public ScriptedAI
        {
            mob_enraged_silverback_gorillaAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                me->GetMotionMaster()->MovePoint(1, -13739.563477f, -28.298729f, 44.522377f);
                events.ScheduleEvent(EVENT_ENRAGE, 10000);
                events.ScheduleEvent(EVENT_ENRAGE_2, 20000);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->SummonCreature(NPC_KONDA, -13771.110352f, -6.243329f, 45.640976f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                            return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                        return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ENRAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ENRAGE, false);
                            break;
                        case EVENT_ENRAGE_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(me, SPELL_ENRAGE_2, false);
                            break;
                        default:
                            break;
                    }

                DoMeleeAttackIfReady();
                }
            }
        };
};

class mob_konda : public CreatureScript
{
    public:
        mob_konda() : CreatureScript("mob_konda")
        {
        }

        EventMap events;

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kondaAI (creature);
        }

        struct mob_kondaAI : public ScriptedAI
        {
            mob_kondaAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_ENRAGE_2, 10000);
                events.ScheduleEvent(EVENT_FLING_FILTH, 20000);
                me->GetMotionMaster()->MovePoint(1, -13739.563477f, -28.298729f, 44.522377f);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->SummonCreature(NPC_MOKK_THE_SAVAGE, -13771.110352f, -6.243329f, 45.640976f, me->GetOrientation(), TEMPSUMMON_MANUAL_DESPAWN, 0);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ENRAGE_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ENRAGE, false);
                            break;
                        case EVENT_FLING_FILTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(me, SPELL_FLING_FILTH, false);
                            events.ScheduleEvent(EVENT_FLING_FILTH, 15000);
                            break;
                        default:
                            break;
                    }

                DoMeleeAttackIfReady();
                }
            }
        };
};

class mob_mokk_the_savage : public CreatureScript
{
    public:
        mob_mokk_the_savage() : CreatureScript("mob_mokk_the_savage")
        {
        }

        EventMap events;

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mokk_the_savageAI (creature);
        }

        struct mob_mokk_the_savageAI : public ScriptedAI
        {
            mob_mokk_the_savageAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void Reset()
            {
                events.Reset();
                me->GetMotionMaster()->MovePoint(1, -13739.563477f, -28.298729f, 44.522377f);
                events.ScheduleEvent(EVENT_ENRAGE_2, 10000);
                events.ScheduleEvent(EVENT_BATTLE_SHOOT, 25000);
                events.ScheduleEvent(EVENT_STOMP, 32000);
            }

            void JustDied(Unit* /*killer*/)
            {
                std::list<Creature*> unbagwaList;
                GetCreatureListWithEntryInGrid(unbagwaList, me, NPC_WITCH_DOCTOR_UNBAGWA, 20.0f);

                for (auto unbagwa: unbagwaList)
                    unbagwa->CombatStop();
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ENRAGE_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ENRAGE_2, false);
                            break;
                        case EVENT_BATTLE_SHOOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(me, SPELL_BATTLE_SHOOT, false);
                            events.ScheduleEvent(EVENT_BATTLE_SHOOT, 30000);
                            break;
                        case EVENT_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(me, SPELL_STOMP, false);
                            events.ScheduleEvent(EVENT_STOMP, 30000);
                            break;
                        default:
                            break;
                    }

                DoMeleeAttackIfReady();
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
    new mob_witch_doctor_unbagwa();
    new mob_enraged_silverback_gorilla();
    new mob_konda();
    new mob_mokk_the_savage();
}
