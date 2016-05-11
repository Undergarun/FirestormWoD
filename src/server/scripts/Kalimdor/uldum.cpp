////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptPCH.h"
#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"

enum Spells
{
    // Akma'hat
    SPELL_STONE_MANTLE              = 93561,
    SPELL_STONE_MANTLE_BUFF         = 94965,
    SPELL_SHOCKWAVE                 = 94968,
    SPELL_FURY_OF_THE_SANDS         = 94946,
    SPELL_FURY_OF_THE_SANDS_DUMMY   = 94941,
    SPELL_FURY_OF_THE_SANDS_SUMMON  = 93574,
    SPELL_SANDS_OF_TIME             = 93578
};

enum Events
{
    // Akma'hat
    EVENT_SHOCKWAVE         = 1,
    EVENT_FURY_OF_THE_SANDS = 2,
    EVENT_STONE_MANTLE      = 3,
    EVENT_SANDS_OF_TIME     = 4
};

enum CreatureIds
{
    NPC_AKMAHAT             = 50063,
    NPC_FURY_OF_THE_SANDS   = 51293
};

// 50063
class boss_akmahat : public CreatureScript
{
public:
    boss_akmahat() : CreatureScript("boss_akmahat") { }
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new boss_akmahatAI(pCreature);
    }
    
    struct boss_akmahatAI : public ScriptedAI
    {
        boss_akmahatAI(Creature* pCreature) : ScriptedAI(pCreature), summons(me)
        {
            me->ApplySpellImmune(0, IMMUNITY_EFFECT, SPELL_EFFECT_KNOCK_BACK, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_GRIP, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_STUN, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FEAR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_ROOT, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_FREEZE, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_POLYMORPH, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_HORROR, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_SAPPED, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_CHARM, true);
            me->ApplySpellImmune(0, IMMUNITY_MECHANIC, MECHANIC_DISORIENTED, true);
            me->ApplySpellImmune(0, IMMUNITY_STATE, SPELL_AURA_MOD_CONFUSE, true);
            me->setActive(true);
        }
        
        void Reset()
        {
            events.Reset();
            summons.DespawnAll();
        }
        
        void EnterCombat(Unit* /*p_Attacker*/)
        {
            events.ScheduleEvent(EVENT_SHOCKWAVE, 16000);
            events.ScheduleEvent(EVENT_FURY_OF_THE_SANDS, 9000);
            events.ScheduleEvent(EVENT_SANDS_OF_TIME, urand(20000, 30000));
            events.ScheduleEvent(EVENT_STONE_MANTLE, 22000);
        }
        
        void JustSummoned(Creature* summon)
        {
            summons.Summon(summon);
        }
        
        void SummonedCreatureDespawn(Creature* summon)
        {
            summons.Despawn(summon);
        }
        
        void JustDied(Unit* /*killer*/)
        {
            events.Reset();
            summons.DespawnAll();
        }
        
        void UpdateAI(const uint32 diff)
        {
            if (!UpdateVictim())
                return;
            
            events.Update(diff);
            
            if (me->HasUnitState(UNIT_STATE_CASTING))
                return;
            
            if (uint32 eventId = events.ExecuteEvent())
            {
                switch (eventId)
                {
                    case EVENT_STONE_MANTLE:
                        if (!me->HasAura(SPELL_STONE_MANTLE))
                        {
                            DoCast(me, SPELL_STONE_MANTLE);
                            events.ScheduleEvent(EVENT_STONE_MANTLE, urand(43000, 45000));
                        }
                        else
                            events.ScheduleEvent(EVENT_STONE_MANTLE, urand(10000, 15000));
                        break;
                    case EVENT_SHOCKWAVE:
                        DoCastVictim(SPELL_SHOCKWAVE);
                        events.ScheduleEvent(EVENT_SHOCKWAVE, 16000);
                        break;
                    case EVENT_FURY_OF_THE_SANDS:
                        for (uint8 i = 0; i < 9; ++i)
                            DoCast(me, SPELL_FURY_OF_THE_SANDS_SUMMON, true);
                        DoCast(me, SPELL_FURY_OF_THE_SANDS);
                        events.ScheduleEvent(EVENT_FURY_OF_THE_SANDS, 16000);
                        break;
                    case EVENT_SANDS_OF_TIME:
                        DoCastAOE(SPELL_SANDS_OF_TIME);
                        events.ScheduleEvent(EVENT_SANDS_OF_TIME, 25000);
                        break;
                    default:
                        break;
                }
            }
            
            DoMeleeAttackIfReady();
        }
    private:
        EventMap events;
        SummonList summons;
    };
};

// 51293
class npc_akmahat_fury_of_the_sands : public CreatureScript
{
public:
    npc_akmahat_fury_of_the_sands() : CreatureScript("npc_akmahat_fury_of_the_sands") { }
    
    CreatureAI* GetAI(Creature* pCreature) const
    {
        return new npc_akmahat_fury_of_the_sandsAI(pCreature);
    }
    
    struct npc_akmahat_fury_of_the_sandsAI : public Scripted_NoMovementAI
    {
        npc_akmahat_fury_of_the_sandsAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
        {
        }
        
        void IsSummonedBy(Unit* /*owner*/)
        {
            DoCast(me, SPELL_FURY_OF_THE_SANDS_DUMMY, true);
        }
        
        void SpellHit(Unit* /*caster*/, const SpellInfo* spellInfo)
        {
            if (spellInfo->Id == SPELL_FURY_OF_THE_SANDS)
                me->DespawnOrUnsummon(1000);
        }
    };
};

#define GOSSIP_CHOICE_1  "I'm ready, Doctor Jones !"

enum eHarrisonJonesActions
{
    ACTION_FIRST_MOVE        = 1,
    ACTION_FIRST_JUMP        = 2,
    ACTION_SECOND_JUMP       = 3,
    ACTION_THIRD_JUMP        = 4,
    ACTION_SAY_01            = 5,
    ACTION_SAY_02            = 6
};

enum eHarrisonJonesTexts
{
    SAY_01           = 0,
    SAY_02           = 1,
    SAY_03           = 2
};

#define ACTION_SECOND_MOVE 2
#define ACTION_THIRD_MOVE 3
#define ACTION_FOURTH_MOVE 4
#define ACTION_FIFTH_MOVE 5

class mob_harrison_jones : public CreatureScript
{
public:
    mob_harrison_jones() : CreatureScript("mob_harrison_jones")
    {
    }
    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(27196) == QUEST_STATUS_INCOMPLETE)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(16786, creature->GetGUID());
        }

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
    {
        player->PlayerTalkClass->ClearMenus();

        if (action == GOSSIP_ACTION_INFO_DEF + 1)
        {
            player->CLOSE_GOSSIP_MENU();
            creature->AI()->DoAction(ACTION_FIRST_MOVE);
        }

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_harrison_jonesAI(creature);
    }

    struct mob_harrison_jonesAI : public ScriptedAI
    {
        mob_harrison_jonesAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        bool hasSaid1;
        bool hasSaid2;

        void Reset()
        {
            hasSaid1 = false;
            hasSaid2 = false;
        }

        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_FIRST_MOVE:
                    me->GetMotionMaster()->MovePoint(0, -9153.513f, -1534.094f, 71.019f);
                    Talk(SAY_01);
                    break;
                case ACTION_FIRST_JUMP:
                    me->GetMotionMaster()->MoveJump(-9148.565f, -1536.0601f, 73.8644f, 2.0f, 2.0f);
                    break;
                case ACTION_SECOND_JUMP:
                    me->GetMotionMaster()->MoveJump(-9145.193f, -1538.517f, 75.033f, 2.0f, 2.0f);
                    break;
                case ACTION_THIRD_JUMP:
                    break;
                case ACTION_SAY_01:
                    if (hasSaid1 == true)
                        Talk(SAY_02);
                    break;
                case ACTION_SAY_02:
                    if (hasSaid2 == true)
                        Talk(SAY_03);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 /*p_Diff*/)
        {

            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 15.0f);

            if (me->GetPositionX() == -9153.513f && me->GetPositionY() == -1534.094f && me->GetPositionZ() == 71.019f)
            {
                DoAction(ACTION_SAY_01);
                DoAction(ACTION_FIRST_JUMP);
                hasSaid1 = true;
            }

            if (me->GetPositionX() == -9148.565f && me->GetPositionY() == -1536.0601f && me->GetPositionZ() == 73.8644f)
            {
                DoAction(ACTION_SECOND_JUMP);
            }

            if (me->GetPositionX() == -9145.193f && me->GetPositionY() == -1538.517f && me->GetPositionZ() == 75.033f)
            {
                DoAction(ACTION_SAY_02);
                DoAction(ACTION_THIRD_JUMP);
                hasSaid2 = true;

                for (auto player: playerList)
                    if (player->GetQuestStatus(27196) == QUEST_STATUS_INCOMPLETE)
                        player->KilledMonsterCredit(45238);
            }
        }
    };
};

#define NPC_ULDUM_WATCHER 45316

class mob_harrison_jones_2 : public CreatureScript
{
public:
    mob_harrison_jones_2() : CreatureScript("mob_harrison_jones_2")
    {
    }

    bool OnQuestAccept(Player* /*player*/, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == 27431)
            creature->AI()->DoAction(ACTION_FIRST_MOVE);

        return true;
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_harrison_jones_2AI(creature);
    }

    struct mob_harrison_jones_2AI : public ScriptedAI
    {
        mob_harrison_jones_2AI(Creature* creature) : ScriptedAI(creature)
        {
        }

        void DoAction(const int32 action)
        {
            std::list<Player*> playerList;
            GetPlayerListInGrid(playerList, me, 30.0f);

            switch (action)
            {
                case ACTION_FIRST_MOVE:
                    me->GetMotionMaster()->MovePoint(0, -9177.980f, -1532.866f, -172.559f);
                    break;
                case ACTION_SECOND_MOVE:
                    me->GetMotionMaster()->MovePoint(0, -9170.007f, -1497.185f, -172.558f);
                    break;
                case ACTION_THIRD_MOVE:
                    me->GetMotionMaster()->MovePoint(0, -9169.322f, -1475.245f, -172.559f);
                    break;
                case ACTION_FOURTH_MOVE:
                    me->GetMotionMaster()->MovePoint(0, -9161.517f, -1473.894f, -170.909f);
                    break;
                case ACTION_FIFTH_MOVE:
                    me->GetMotionMaster()->MovePoint(0, -9145.245f, -1470.735f, -170.909f);
                    for (auto player: playerList)
                        if (player->GetQuestStatus(27431) == QUEST_STATUS_INCOMPLETE)
                            player->KilledMonsterCredit(45742);
                    break;
                default:
                    break;
            }
        }

        void UpdateAI(const uint32 /*p_Diff*/)
        {
            std::list<Creature*> uldumWatcherList;
            me->GetCreatureListWithEntryInGrid(uldumWatcherList, NPC_ULDUM_WATCHER, 100.0f);

            if (me->GetPositionX() <= -9176.0f && me->GetPositionY() <= -1531.0f && me->GetPositionZ() == -172.559f
                && me->GetPositionX() >= -9178.0f && me->GetPositionY() >= -1533.0f)
                me->AI()->DoAction(ACTION_SECOND_MOVE);

            if (me->GetPositionX() == -9170.007f && me->GetPositionY() == -1497.185f && me->GetPositionZ() == -172.558f)
                me->AI()->DoAction(ACTION_THIRD_MOVE);

            if (me->GetPositionX() == -9169.322f && me->GetPositionY() == -1475.245f && me->GetPositionZ() == -172.559f)
                me->AI()->DoAction(ACTION_FOURTH_MOVE);

            if (me->GetPositionX() == -9161.517f && me->GetPositionY() == -1473.894f && me->GetPositionZ() == -170.909f)
                me->AI()->DoAction(ACTION_FIFTH_MOVE);

// TODO : Camera Systems

        }
    };
};

enum eLurkingTempestSpells
{
    SPELL_LIGHTNING_BOLT    = 89105,
    SPELL_LURK              = 85467
};

enum eLurkingTempestEvents
{
    EVENT_LIGHTNING_BOLT    = 1,
    EVENT_LURK              = 2
};


class mob_lurking_tempest : public CreatureScript
{
public:
    mob_lurking_tempest() : CreatureScript("mob_lurking_tempest")
    {
    }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new mob_lurking_tempestAI(creature);
    }

    struct mob_lurking_tempestAI : public ScriptedAI
    {
        mob_lurking_tempestAI(Creature* creature) : ScriptedAI(creature)
        {
        }

        EventMap events;

        void Reset()
        {
            events.Reset();

            events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 8000);
            events.ScheduleEvent(EVENT_LURK, 14000);
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
                    case EVENT_LIGHTNING_BOLT:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_LIGHTNING_BOLT, false);
                        events.ScheduleEvent(EVENT_LIGHTNING_BOLT, 12000);
                        break;
                    case EVENT_LURK:
                        if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(target, SPELL_LURK, false);
                        events.ScheduleEvent(EVENT_LURK, 12000);
                        break;
                    default:
                        break;
                }
            }

            DoMeleeAttackIfReady();
        }
    };
};

#ifndef __clang_analyzer__
void AddSC_uldum()
{
    new mob_harrison_jones();
    new mob_harrison_jones_2();
    new boss_akmahat();
    new npc_akmahat_fury_of_the_sands();
    new mob_lurking_tempest();
}
#endif
