#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "kun_lai_summit.h"


class mob_nessos_the_oracle : public CreatureScript
{
    public:
        mob_nessos_the_oracle() : CreatureScript("mob_nessos_the_oracle")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_nessos_the_oracleAI(creature);
        }

        struct mob_nessos_the_oracleAI : public ScriptedAI
        {
            mob_nessos_the_oracleAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_VICIOUS_REND,      7000);
                events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 17000);
                events.ScheduleEvent(EVENT_VANISH, 12000);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_VICIOUS_REND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VICIOUS_REND, false);
                            events.ScheduleEvent(EVENT_VICIOUS_REND,      7000);
                            break;
                        case EVENT_GRAPPLING_HOOK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GRAPPLING_HOOK, false);
                            events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 17000);
                            break;
                        case EVENT_VANISH:
                            me->CastSpell(me, SPELL_VANISH, false);
                            events.ScheduleEvent(EVENT_VANISH, 20000);
                            events.ScheduleEvent(EVENT_SMOKED_BLADE, urand(0, 8000));
                            break;
                        case EVENT_SMOKED_BLADE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SMOKED_BLADE, false);

                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_ski_thik : public CreatureScript
{
    public:
        mob_ski_thik() : CreatureScript("mob_ski_thik")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ski_thikAI(creature);
        }

        struct mob_ski_thikAI : public ScriptedAI
        {
            mob_ski_thikAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLADE_FURY,       8000);
                events.ScheduleEvent(EVENT_TORNADO,         40000);
                events.ScheduleEvent(EVENT_WINDSONG,        32000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == 64267)
                {
                    summon->AddAura(SPELL_TORNADO_DMG, summon);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->GetMotionMaster()->MoveRandom(20.0f);
                }
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
                        case EVENT_BLADE_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLADE_FURY, false);
                            events.ScheduleEvent(EVENT_BLADE_FURY,      8000);
                            break;
                        case EVENT_TORNADO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORNADO, false);
                            events.ScheduleEvent(EVENT_TORNADO, 40000);
                            break;
                        case EVENT_WINDSONG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WINDSONG, false);
                            events.ScheduleEvent(EVENT_WINDSONG, 32000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_havak : public CreatureScript
{
    public:
        mob_havak() : CreatureScript("mob_havak")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_havakAI(creature);
        }

        struct mob_havakAI : public ScriptedAI
        {
            mob_havakAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEVASTATING_ARC,   50000);
                events.ScheduleEvent(EVENT_SUMMON_QUILEN,     15000);
                events.ScheduleEvent(EVENT_TITANIC_STRENGH,   32000);
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
                        case EVENT_DEVASTATING_ARC:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEVASTATING_ARC, false);
                            events.ScheduleEvent(EVENT_DEVASTATING_ARC,      50000);
                            break;
                        case EVENT_SUMMON_QUILEN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SUMMON_QUILEN, false);
                            events.ScheduleEvent(EVENT_SUMMON_QUILEN, 80000);
                            break;
                        case EVENT_TITANIC_STRENGH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_TITANIC_STRENGH, false);
                            events.ScheduleEvent(EVENT_TITANIC_STRENGH, 40000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_mogujia_soul_caller : public CreatureScript
{
    public:
        mob_mogujia_soul_caller() : CreatureScript("mob_mogujia_soul_caller")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mogujia_soul_callerAI(creature);
        }

        struct mob_mogujia_soul_callerAI : public ScriptedAI
        {
            mob_mogujia_soul_callerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DRAIN_LIFE,      20000);
                events.ScheduleEvent(EVENT_SHADOW_BOLT,     15000);
                events.ScheduleEvent(EVENT_SHADOW_CRASH,    32000);
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
                        case EVENT_DRAIN_LIFE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DRAIN_LIFE, false);
                            events.ScheduleEvent(EVENT_DRAIN_LIFE,      20000);
                            break;
                        case EVENT_SHADOW_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOW_BOLT, false);
                            events.ScheduleEvent(EVENT_SHADOW_BOLT, 15000);
                            break;
                        case EVENT_SHADOW_CRASH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHADOW_CRASH, false);
                            events.ScheduleEvent(EVENT_SHADOW_CRASH, 32000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_quilen_stonemaw : public CreatureScript
{
    public:
        mob_quilen_stonemaw() : CreatureScript("mob_quilen_stonemaw")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_quilen_stonemawAI(creature);
        }

        struct mob_quilen_stonemawAI : public ScriptedAI
        {
            mob_quilen_stonemawAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_UNSTABLE_SERUM,      5000);
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
                        case EVENT_UNSTABLE_SERUM:
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->CastSpell(target, SPELL_UNSTABLE_SERUM, false);
                            events.ScheduleEvent(EVENT_UNSTABLE_SERUM,      5000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_zai_the_outcast : public CreatureScript
{
    public:
        mob_zai_the_outcast() : CreatureScript("mob_zai_the_outcast")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zai_the_outcastAI(creature);
        }

        struct mob_zai_the_outcastAI : public ScriptedAI
        {
            mob_zai_the_outcastAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_RAIN_DANCE,       8000);
                events.ScheduleEvent(EVENT_TORRENT,         40000);
                events.ScheduleEvent(EVENT_WATER_BOLT,      32000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == 64267)
                {
                    summon->AddAura(SPELL_TORNADO_DMG, summon);
                    summon->SetReactState(REACT_PASSIVE);
                    summon->GetMotionMaster()->MoveRandom(20.0f);
                }
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
                        case EVENT_RAIN_DANCE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RAIN_DANCE, false);
                            events.ScheduleEvent(EVENT_RAIN_DANCE,      8000);
                            break;
                        case EVENT_TORRENT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORRENT, false);
                            events.ScheduleEvent(EVENT_TORRENT, 40000);
                            break;
                        case EVENT_WATER_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WATER_BOLT, false);
                            events.ScheduleEvent(EVENT_WATER_BOLT, 32000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_zhi_the_harmonious : public CreatureScript
{
    public:
        mob_zhi_the_harmonious() : CreatureScript("mob_zhi_the_harmonious")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zhi_the_harmoniousAI(creature);
        }

        struct mob_zhi_the_harmoniousAI : public ScriptedAI
        {
            mob_zhi_the_harmoniousAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ZHI_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_ZHI_TALK_2:
                        Talk(1);
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

class mob_andruin_wrynn : public CreatureScript
{
    public:
        mob_andruin_wrynn() : CreatureScript("mob_andruin_wrynn")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_andruin_wrynnAI(creature);
        }

        struct mob_andruin_wrynnAI : public ScriptedAI
        {
            mob_andruin_wrynnAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_ANDRUIN_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_ANDRUIN_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_ANDRUIN_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_ANDRUIN_TALK_4:
                        Talk(3);
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

class mob_taran_zhu_white_tiger : public CreatureScript
{
    public:
        mob_taran_zhu_white_tiger() : CreatureScript("mob_taran_zhu_white_tiger")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_taran_zhu_white_tigerAI(creature);
        }

        struct mob_taran_zhu_white_tigerAI : public ScriptedAI
        {
            mob_taran_zhu_white_tigerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_TARAN_ZHU_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_TARAN_ZHU_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_TARAN_ZHU_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_TARAN_ZHU_TALK_4:
                        Talk(3);
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

class npc_xuen : public CreatureScript
{
    public:
        npc_xuen() : CreatureScript("npc_xuen")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (!creature->AI())
                return false;

            if (player->GetQuestStatus(QUEST_A_CELESTIAL_EXPERIENCE_H) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_A_CELESTIAL_EXPERIENCE_A) == QUEST_STATUS_INCOMPLETE)
            {
                if (!creature->AI()->GetData(XUEN_PHASE_1))
                {
                    creature->AI()->DoAction(ACTION_CONVERSATION);
                    player->KilledMonsterCredit(KILLED_CREDIT_FIRST_PHASE);
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->AI()->SetData(XUEN_PHASE_1, DONE);
                }

                if (creature->AI()->GetData(XUEN_PHASE_2))
                {
                    creature->AI()->DoAction(ACTION_SECOND_CONVERSATION);
                    player->KilledMonsterCredit(KILLED_CREDIT_SECOND_PHASE);
                    creature->AI()->SetData(XUEN_PHASE_2, DONE);
                }

                if (creature->AI()->GetData(XUEN_PHASE_3))
                {
                    player->KilledMonsterCredit(KILLED_CREDIT_THIRD_PHASE);
                    creature->AI()->DoAction(ACTION_THIRD_CONVERSATION);
                    creature->AI()->SetData(XUEN_PHASE_3, DONE);
                }
            }

            return true;
        }


        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_xuenAI(creature);
        }

        struct npc_xuenAI : public ScriptedAI
        {
            npc_xuenAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;
            uint64 playerGuid;
            uint32 phaseDone[XUEN_PHASE_MAX];

            void Reset()
            {
                events.Reset();
                playerGuid = 0;
                for (int i = 0; i < XUEN_PHASE_MAX; i++)
                    phaseDone[i] = NOT_STARTED;
            }

            void SetGUID(uint64 guid, int32 id /*= 0*/)
            {
                playerGuid = guid;
            }

            uint32 GetData(uint32 index)
            {
                if (index >= XUEN_PHASE_MAX)
                    return 0;

                return phaseDone[index];
            }

            void SetData(uint32 index, uint32 value)
            {
                if (index >= XUEN_PHASE_MAX)
                    return;

                phaseDone[index] = value;
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_CONVERSATION:
                        events.ScheduleEvent(EVENT_XUEN_TALK_1, 2000);
                        events.ScheduleEvent(EVENT_TARAN_ZHU_TALK_1, 7000);
                        events.ScheduleEvent(EVENT_ANDRUIN_TALK_1, 13000);
                        events.ScheduleEvent(EVENT_TARAN_ZHU_TALK_2, 20000);
                        events.ScheduleEvent(EVENT_XUEN_TALK_2, 30000);
                        events.ScheduleEvent(EVENT_PLAYER_TELEPORT, 31000);
                        events.ScheduleEvent(EVENT_SPAWN_SPIRIT_OF_VIOLENCE, 34000);
                        events.ScheduleEvent(EVENT_TARAN_ZHU_TALK_3, 45000);
                        events.ScheduleEvent(EVENT_ANDRUIN_TALK_2, 51000);
                        events.ScheduleEvent(EVENT_SPAWN_SPIRIT_OF_VIOLENCE, 54000);
                        events.ScheduleEvent(EVENT_ZHI_TALK_1, 63000);
                        break;
                    case ACTION_SECOND_CONVERSATION:
                        events.ScheduleEvent(EVENT_XUEN_TALK_3, 2000);
                        events.ScheduleEvent(EVENT_PLAYER_TELEPORT, 6000);
                        events.ScheduleEvent(EVENT_SPAWN_SPIRIT_OF_ANGER, 9000);
                        events.ScheduleEvent(EVENT_ANDRUIN_TALK_3, 15000);
                        events.ScheduleEvent(EVENT_TARAN_ZHU_TALK_4, 28000);
                        events.ScheduleEvent(EVENT_SPAWN_SPIRIT_OF_VIOLENCE, 30000);
                        events.ScheduleEvent(EVENT_ZHI_TALK_2, 36000);
                        break;
                    case ACTION_THIRD_CONVERSATION:
                        events.ScheduleEvent(EVENT_XUEN_TALK_4, 2000);
                        events.ScheduleEvent(EVENT_PLAYER_TELEPORT, 6000);
                        events.ScheduleEvent(EVENT_SPAWN_SPIRIT_OF_HATRED, 8000);
                        events.ScheduleEvent(EVENT_ANDRUIN_TALK_4, 20000);
                        break;
                    case ACTION_XUEN_TALK_1:
                        Talk(0);
                        break;
                    case ACTION_XUEN_TALK_2:
                        Talk(1);
                        break;
                    case ACTION_XUEN_TALK_3:
                        Talk(2);
                        break;
                    case ACTION_XUEN_TALK_4:
                        Talk(3);
                        break;
                    case ACTION_FINAL_KILL_CREDIT:
                        events.ScheduleEvent(EVENT_FINAL_KILL_CREDIT, 1000);
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
                    std::list<Creature*> TaranZhuList;
                    GetCreatureListWithEntryInGrid(TaranZhuList, me, NPC_TARAN_ZHU, 15.0f);

                    std::list<Creature*> andruinWrynnList;
                    GetCreatureListWithEntryInGrid(andruinWrynnList, me, NPC_ANDRUIN_WRYNN, 15.0f);

                    std::list<Creature*> zhiList;
                    GetCreatureListWithEntryInGrid(zhiList, me, NPC_ZHI_THE_HARMONIOUS, 15.0f);

                    while (uint32 eventId = events.ExecuteEvent())
                    {
                        switch (eventId)
                        {
                            case EVENT_XUEN_TALK_1:
                                DoAction(ACTION_XUEN_TALK_1);
                                break;
                            case EVENT_TARAN_ZHU_TALK_1:
                                if (!TaranZhuList.empty())
                                    for (auto TaranZhu: TaranZhuList)
                                        TaranZhu->AI()->DoAction(ACTION_TARAN_ZHU_TALK_1);
                                break;
                            case EVENT_ANDRUIN_TALK_1:
                                if (!andruinWrynnList.empty())
                                    for (auto andruinWrynn: andruinWrynnList)
                                        andruinWrynn->AI()->DoAction(ACTION_ANDRUIN_TALK_1);
                                break;
                            case EVENT_TARAN_ZHU_TALK_2:
                                if (!TaranZhuList.empty())
                                    for (auto TaranZhu: TaranZhuList)
                                        TaranZhu->AI()->DoAction(ACTION_TARAN_ZHU_TALK_2);
                                break;
                            case EVENT_XUEN_TALK_2:
                                DoAction(ACTION_XUEN_TALK_2);
                                break;
                            case EVENT_PLAYER_TELEPORT:
                                player->TeleportTo(870, 3770.889f, 538.036f, 639.046f, 0.0f);
                                break;
                            case EVENT_SPAWN_SPIRIT_OF_VIOLENCE:
                                me->SummonCreature(MOB_SPIRIT_OF_VIOLENCE, 3778.698f, 539.101f, 639.007f);
                                break;
                            case EVENT_TARAN_ZHU_TALK_3:
                                if (!TaranZhuList.empty())
                                    for (auto TaranZhu: TaranZhuList)
                                        TaranZhu->AI()->DoAction(ACTION_TARAN_ZHU_TALK_3);
                                break;
                            case EVENT_ANDRUIN_TALK_2:
                                if (!andruinWrynnList.empty())
                                    for (auto andruinWrynn: andruinWrynnList)
                                        andruinWrynn->AI()->DoAction(ACTION_ANDRUIN_TALK_2);
                                break;
                            case EVENT_ZHI_TALK_1:
                                if (!zhiList.empty())
                                    for (auto zhi: zhiList)
                                        zhi->AI()->DoAction(ACTION_ZHI_TALK_1);
                                break;
                            case EVENT_XUEN_TALK_3:
                                DoAction(ACTION_XUEN_TALK_3);
                                break;
                            case EVENT_SPAWN_SPIRIT_OF_ANGER:
                                me->SummonCreature(MOB_SPIRIT_OF_ANGER, 3778.698f, 539.101f, 639.007f);
                                break;
                            case EVENT_ANDRUIN_TALK_3:
                                if (!andruinWrynnList.empty())
                                    for (auto andruinWrynn: andruinWrynnList)
                                        andruinWrynn->AI()->DoAction(ACTION_ANDRUIN_TALK_3);
                                break;
                            case EVENT_TARAN_ZHU_TALK_4:
                                if (!TaranZhuList.empty())
                                    for (auto TaranZhu: TaranZhuList)
                                        TaranZhu->AI()->DoAction(ACTION_TARAN_ZHU_TALK_4);
                                break;
                            case EVENT_ZHI_TALK_2:
                                if (!zhiList.empty())
                                    for (auto zhi: zhiList)
                                        zhi->AI()->DoAction(ACTION_ZHI_TALK_2);
                                break;
                            case EVENT_XUEN_TALK_4:
                                DoAction(ACTION_XUEN_TALK_4);
                                break;
                            case EVENT_SPAWN_SPIRIT_OF_HATRED:
                                if (player->GetQuestStatus(QUEST_A_CELESTIAL_EXPERIENCE_H) == QUEST_STATUS_INCOMPLETE)
                                    me->SummonCreature(MOB_SPIRIT_OF_HATRED_1, 3778.698f, 539.101f, 639.007f);

                                if (player->GetQuestStatus(QUEST_A_CELESTIAL_EXPERIENCE_A) == QUEST_STATUS_INCOMPLETE)
                                    me->SummonCreature(MOB_SPIRIT_OF_HATRED_2, 3778.698f, 539.101f, 639.007f);
                                break;
                            case EVENT_ANDRUIN_TALK_4:
                                if (!andruinWrynnList.empty())
                                    for (auto andruinWrynn: andruinWrynnList)
                                        andruinWrynn->AI()->DoAction(ACTION_ANDRUIN_TALK_4);
                                break;
                            case EVENT_FINAL_KILL_CREDIT:
                                player->KilledMonsterCredit(KILLED_CREDIT_FOURTH_PHASE);
                                Reset();
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        };
};

class mob_spirit_of_violence : public CreatureScript
{
    public:
        mob_spirit_of_violence() : CreatureScript("mob_spirit_of_violence")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_spirit_of_violenceAI(creature);
        }

        struct mob_spirit_of_violenceAI : public ScriptedAI
        {
            mob_spirit_of_violenceAI(Creature* creature) : ScriptedAI(creature)
            {
                xuenGUID = 0;
            }

            EventMap events;
            uint64 xuenGUID;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_UNTAMED_FURY, 3000);
                events.ScheduleEvent(EVENT_HATED_BLAST, 12000);
                events.ScheduleEvent(EVENT_SHA_CORRUPTION, 23000);
            }

            void JustDied(Unit* killer)
            {
                Creature* xuen = me->GetMap()->GetCreature(xuenGUID);
                if (!xuen || !xuen->GetAI())
                    return;

                xuen->AI()->SetData(XUEN_PHASE_2, DONE);
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (!summoner)
                    return;

                xuenGUID = summoner->GetGUID();
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
                        case EVENT_UNTAMED_FURY:
                            me->CastSpell(me, SPELL_UNTAMED_FURY, false);
                            events.ScheduleEvent(EVENT_UNTAMED_FURY,      20000);
                            break;
                        case EVENT_HATED_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HATED_BLAST, false);
                            events.ScheduleEvent(EVENT_HATED_BLAST, 20000);
                            break;
                        case EVENT_SHA_CORRUPTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_CORRUPTION, false);
                            events.ScheduleEvent(EVENT_SHA_CORRUPTION, 20000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_spirit_of_anger : public CreatureScript
{
    public:
        mob_spirit_of_anger() : CreatureScript("mob_spirit_of_anger")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_spirit_of_angerAI(creature);
        }

        struct mob_spirit_of_angerAI : public ScriptedAI
        {
            mob_spirit_of_angerAI(Creature* creature) : ScriptedAI(creature)
            {
                xuenGUID = 0;
            }

            EventMap events;
            uint64 xuenGUID;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEVASTATION, 3000);
                events.ScheduleEvent(EVENT_HATED_BLAST, 12000);
                events.ScheduleEvent(EVENT_SHA_CORRUPTION, 23000);
            }

            void JustDied(Unit* killer)
            {
                Creature* xuen = me->GetMap()->GetCreature(xuenGUID);
                if (!xuen || !xuen->GetAI())
                    return;

                xuen->AI()->SetData(XUEN_PHASE_3, DONE);
            }

            void DamageTaken(Unit* attacker, uint32 &damage)
            {
                if(me->HealthBelowPctDamaged(20, damage))
                {
                    me->SummonCreature(MOB_SPIRIT_OF_VIOLENCE, 3778.698f, 539.101f, 639.007f);
                }
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (!summoner)
                    return;

                xuenGUID = summoner->GetGUID();
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
                        case EVENT_DEVASTATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEVASTATION, false);
                            events.ScheduleEvent(EVENT_DEVASTATION,      20000);
                            break;
                        case EVENT_HATED_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HATED_BLAST, false);
                            events.ScheduleEvent(EVENT_HATED_BLAST, 20000);
                            break;
                        case EVENT_SHA_CORRUPTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_CORRUPTION, false);
                            events.ScheduleEvent(EVENT_SHA_CORRUPTION, 20000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_spirit_of_hatred : public CreatureScript
{
    public:
        mob_spirit_of_hatred() : CreatureScript("mob_spirit_of_hatred")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_spirit_of_hatredAI(creature);
        }

        struct mob_spirit_of_hatredAI : public ScriptedAI
        {
            mob_spirit_of_hatredAI(Creature* creature) : ScriptedAI(creature)
            {
                xuenGUID = 0;
            }

            EventMap events;
            uint64 xuenGUID;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_UNTAMED_FURY, 3000);
                events.ScheduleEvent(EVENT_HATED_BLAST, 12000);
                events.ScheduleEvent(EVENT_SHA_CORRUPTION, 23000);
                events.ScheduleEvent(EVENT_WHIRLWIND_OF_ANGER, 32000);
            }

            void JustDied(Unit* killer)
            {
                Creature* xuen = me->GetMap()->GetCreature(xuenGUID);
                if (!xuen || !xuen->GetAI())
                    return;

                xuen->AI()->SetData(XUEN_PHASE_4, DONE);
                xuen->AI()->DoAction(ACTION_FINAL_KILL_CREDIT);
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (!summoner)
                    return;

                xuenGUID = summoner->GetGUID();
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
                        case EVENT_DEVASTATION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEVASTATION, false);
                            events.ScheduleEvent(EVENT_DEVASTATION,      20000);
                            break;
                        case EVENT_HATED_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HATED_BLAST, false);
                            events.ScheduleEvent(EVENT_HATED_BLAST, 20000);
                            break;
                        case EVENT_SHA_CORRUPTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_CORRUPTION, false);
                            events.ScheduleEvent(EVENT_SHA_CORRUPTION, 20000);
                            break;
                        case EVENT_WHIRLWIND_OF_ANGER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_WHIRLWIND_OF_ANGER, false);
                            events.ScheduleEvent(EVENT_WHIRLWIND_OF_ANGER, 20000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

void AddSC_kun_lai_summit()
{
    new mob_nessos_the_oracle();
    new mob_ski_thik();
    new mob_havak();
    new mob_mogujia_soul_caller();
    new mob_quilen_stonemaw();
    new mob_zai_the_outcast();
    new mob_zhi_the_harmonious();
    new mob_andruin_wrynn();
    new mob_taran_zhu_white_tiger();
    new npc_xuen();
    new mob_spirit_of_violence();
    new mob_spirit_of_anger();
    new mob_spirit_of_hatred();
}
