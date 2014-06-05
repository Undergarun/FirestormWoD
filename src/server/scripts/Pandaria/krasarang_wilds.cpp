#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "krasarang_wilds.h"

class mob_arness_the_scale : public CreatureScript
{
    public:
        mob_arness_the_scale() : CreatureScript("mob_arness_the_scale")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_arness_the_scaleAI(creature);
        }

        struct mob_arness_the_scaleAI : public ScriptedAI
        {
            mob_arness_the_scaleAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 40000);
                events.ScheduleEvent(EVENT_VANISH, 20000);
                events.ScheduleEvent(EVENT_VICIOUS_REND, 15000);
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
                        case EVENT_GRAPPLING_HOOK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GRAPPLING_HOOK, false);
                            events.ScheduleEvent(EVENT_GRAPPLING_HOOK, 40000);
                            break;
                        case EVENT_VANISH:
                            me->CastSpell(me, SPELL_VANISH, false);
                            events.ScheduleEvent(EVENT_VANISH, 40000);
                            break;
                        case EVENT_VICIOUS_REND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VICIOUS_REND, false);
                            events.ScheduleEvent(EVENT_VICIOUS_REND, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_gaarn_the_toxic : public CreatureScript
{
    public:
        mob_gaarn_the_toxic() : CreatureScript("mob_gaarn_the_toxic")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_gaarn_the_toxicAI(creature);
        }

        struct mob_gaarn_the_toxicAI : public ScriptedAI
        {
            mob_gaarn_the_toxicAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOWBOLT, 15000);
                events.ScheduleEvent(EVENT_VOIDCLOUD, 70000);
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
                        case EVENT_SHADOWBOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOWBOLT, false);
                            events.ScheduleEvent(EVENT_SHADOWBOLT, 15000);
                            break;
                        case EVENT_VOIDCLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VOIDCLOUD, false);
                            events.ScheduleEvent(EVENT_VOIDCLOUD, 70000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_qu_nas : public CreatureScript
{
    public:
        mob_qu_nas() : CreatureScript("mob_qu_nas")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_qu_nasAI(creature);
        }

        struct mob_qu_nasAI : public ScriptedAI
        {
            mob_qu_nasAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_DEVASTATING_ARC, 25000);
                events.ScheduleEvent(EVENT_SUMMON_QUILEN, 15000);
                events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 70000);
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
                            events.ScheduleEvent(EVENT_DEVASTATING_ARC, 30000);
                            break;
                        case EVENT_SUMMON_QUILEN:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SUMMON_QUILEN, false);
                            events.ScheduleEvent(EVENT_SUMMON_QUILEN, 80000);
                            break;
                        case EVENT_TITANIC_STRENGTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TITANIC_STRENGTH, false);
                            events.ScheduleEvent(EVENT_TITANIC_STRENGTH, 55000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_torik_ethis : public CreatureScript
{
    public:
        mob_torik_ethis() : CreatureScript("mob_torik_ethis")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_torik_ethisAI(creature);
        }

        struct mob_torik_ethisAI : public ScriptedAI
        {
            mob_torik_ethisAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLADE_FURY, 8000);
                events.ScheduleEvent(EVENT_TORNADO, 40000);
                events.ScheduleEvent(EVENT_WINDSONG, 32000);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == MOB_TORNADO)
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
                            events.ScheduleEvent(EVENT_BLADE_FURY, 8000);
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

class mob_go_kan : public CreatureScript
{
    public:
        mob_go_kan() : CreatureScript("mob_go_kan")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_go_kanAI(creature);
        }

        struct mob_go_kanAI : public ScriptedAI
        {
            mob_go_kanAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_SIT);
                events.Reset();
                events.ScheduleEvent(EVENT_BELLOWING_RAGE, 8000);
                events.ScheduleEvent(EVENT_RUSHING_CHARGE, 17000);
                events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 25000);
            }

            void EnterCombat(Unit* who)
            {
                me->RemoveFlag(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_SIT);
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
                        case EVENT_BELLOWING_RAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BELLOWING_RAGE, false);
                            events.ScheduleEvent(EVENT_BELLOWING_RAGE, 35000);
                            break;
                        case EVENT_RUSHING_CHARGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RUSHING_CHARGE, false);
                            events.ScheduleEvent(EVENT_RUSHING_CHARGE, 35000);
                            break;
                        case EVENT_YAUNGOL_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_YAUNGOL_STOMP, false);
                            events.ScheduleEvent(EVENT_YAUNGOL_STOMP, 35000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_spirit_of_the_crane : public CreatureScript
{
    public:
        mob_spirit_of_the_crane() : CreatureScript("mob_spirit_of_the_crane")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(30716) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(60487);
                creature->DespawnOrUnsummon();
            }

            return true;
        }
};

class mob_champion_of_chi_ji : public CreatureScript
{
    public:
        mob_champion_of_chi_ji() : CreatureScript("mob_champion_of_chi_ji")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_champion_of_chi_jiAI(creature);
        }

        struct mob_champion_of_chi_jiAI : public ScriptedAI
        {
            mob_champion_of_chi_jiAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_CHI_TORPEDO, 4000);
                events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 17000);
                events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 27000);
                events.ScheduleEvent(EVENT_UPPERCUT, 35000);
            }

            void MoveInLineOfSight(Unit* who) // Dynamic Aggro !
            {
                if (who->GetTypeId() == TYPEID_PLAYER)
                {
                    if (CAST_PLR(who)->GetQuestStatus(30740) == QUEST_STATUS_INCOMPLETE)
                    {
                        if (me->GetDistance(who) >= 10.0f && me->GetDistance(who) <= 40.0f)
                            me->CastSpell(who, SPELL_FLYING_SERPENT_KICK, false);
                    }
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
                        case EVENT_CHI_TORPEDO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CHI_TORPEDO, false);
                            events.ScheduleEvent(EVENT_CHI_TORPEDO, 40000);
                            break;
                        case EVENT_HUNDRED_HAND_SLAP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HUNDRED_HAND_SLAP, false);
                            events.ScheduleEvent(EVENT_HUNDRED_HAND_SLAP, 40000);
                            break;
                        case EVENT_SPINNING_CRANE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SPINNING_CRANE_KICK, false);
                            events.ScheduleEvent(EVENT_SPINNING_CRANE_KICK, 40000);
                            break;
                        case EVENT_UPPERCUT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_UPPERCUT, false);
                            events.ScheduleEvent(EVENT_UPPERCUT, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

// Chi Torpedo - 119539
class spell_chi_torpedo_periodic : public SpellScriptLoader
{
    public:
        spell_chi_torpedo_periodic() : SpellScriptLoader("spell_chi_torpedo_periodic") { }

        class spell_chi_torpedo_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_chi_torpedo_periodic_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = caster->getVictim())
                    {
                        caster->CastSpell(target, 119520, true);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_chi_torpedo_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_chi_torpedo_periodic_AuraScript();
        }
};

class mob_anduin_wrynn : public CreatureScript
{
    public:
        mob_anduin_wrynn() : CreatureScript("mob_anduin_wrynn")
        {
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
        {
            if (quest->GetQuestId() == 30273)
            {
                player->SummonCreature(66975, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
            }

            return true;
        }
};

class mob_anduin_wrynn_escort : public CreatureScript
{
    public:
        mob_anduin_wrynn_escort() : CreatureScript("mob_anduin_wrynn_escort")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_anduin_wrynn_escortAI(creature);
        }

        struct mob_anduin_wrynn_escortAI : public ScriptedAI
        {
            mob_anduin_wrynn_escortAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            uint64 playerGuid;

            void Reset()
            {
            }

            void IsSummonedBy(Unit* summoner)
            {
                if (CAST_PLR(summoner)->GetQuestStatus(30273) == QUEST_STATUS_INCOMPLETE)
                {
                    me->GetMotionMaster()->MoveFollow(summoner, 2.0f, 2.0f, MOTION_SLOT_ACTIVE);
                    playerGuid = summoner->GetGUID();
                }
            }

            void UpdateAI(const uint32 diff)
            {
                Player* summoner = sObjectAccessor->FindPlayer(playerGuid);
                if (!summoner)
                    return;

                if (Unit* target = summoner->getVictim())
                {
                    if (CAST_CRE(target)->GetEntry() == 59651)
                    {
                        me->Attack(target, true);
                        DoMeleeAttackIfReady();
                    }
                }
            }
        };
};

#define GOSSIP_CHOICE "<Reach out to touch Chi-Ji.>"

class npc_chi_ji : public CreatureScript
{
    public:
        npc_chi_ji() : CreatureScript("npc_chi_ji")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(30273) == QUEST_STATUS_INCOMPLETE)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(30003, creature->GetGUID());
            }

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();
                creature->DespawnOrUnsummon();
                player->SummonCreature(59651, creature->GetPositionX(), creature->GetPositionY(), creature->GetPositionZ()); // , 0.0f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
            }

            return true;
        }
};

class mob_sha_of_despair : public CreatureScript
{
    public:
        mob_sha_of_despair() : CreatureScript("mob_sha_of_despair")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_of_despairAI(creature);
        }

        struct mob_sha_of_despairAI : public ScriptedAI
        {
            mob_sha_of_despairAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            void JustDied(Unit* killer)
            {
                me->DespawnOrUnsummon();
                me->SummonCreature(59653, -1813.46f, 1052.33f, -31.115f, 0.0f, TEMPSUMMON_MANUAL_DESPAWN);

                std::list<Creature*> creatureList;
                GetCreatureListWithEntryInGrid(creatureList, me, 66975, 40.0f);

                for (auto creature: creatureList)
                    creature->DespawnOrUnsummon();
            }
        };
};

class npc_thelonius : public CreatureScript
{
    public:
        npc_thelonius() : CreatureScript("npc_thelonius")
        {
        }

        bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
        {
            switch (quest->GetQuestId())
            {
                case 30725:
                    player->SummonCreature(60530, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30739:
                    player->SummonCreature(60545, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30727:
                    player->SummonCreature(60533, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                case 30732:
                    player->SummonCreature(60538, -2318.079f, 1449.463f, 29.617f, 0.539766f, TEMPSUMMON_MANUAL_DESPAWN, 0, player->GetGUID());
                    break;
                default:
                    break;
            }

            return true;
        }
};

#define GOSSIP_CHOICE_1 "Let's fight !"

class mob_ellia_ravenmane : public CreatureScript
{
    public:
        mob_ellia_ravenmane() : CreatureScript("mob_ellia_ravenmane")
        {
        }

#define CHECK_STATUS(a) (player->GetQuestStatus(a) == QUEST_STATUS_INCOMPLETE)

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(69970, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();

                if (CHECK_STATUS(30725) || CHECK_STATUS(30739) || CHECK_STATUS(30727) || CHECK_STATUS(30732))
                {
                    creature->AI()->SetGUID(player ? player->GetGUID() : 0);
                    creature->setFaction(14);

                    if (creature->GetAI())
                    {
                        creature->AI()->Reset();
                        creature->AI()->DoAction(ACTION_REMOVE_FLAG);
                    }
                }
            }
            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ellia_ravenmaneAI(creature);
        }

        struct mob_ellia_ravenmaneAI : public ScriptedAI
        {
            mob_ellia_ravenmaneAI(Creature* creature) : ScriptedAI(creature)
            {
                playerGuid = 0;
            }

            EventMap events;
            uint64 playerGuid;

            void Reset()
            {
                events.Reset();

                switch (me->GetEntry())
                {
                    case 60530:
                        events.ScheduleEvent(EVENT_JAB, 3000);
                        events.ScheduleEvent(EVENT_KICK, 6000);
                        break;
                    case 60545:
                        events.ScheduleEvent(EVENT_CRY_OUT_IN_DESPAIR, 3000);
                        events.ScheduleEvent(EVENT_SHA_BLAST, 8000);
                        events.ScheduleEvent(EVENT_SHA_SPIKE, 13000);
                        break;
                    case 60533:
                        events.ScheduleEvent(EVENT_FIERCE_JAB, 3000);
                        events.ScheduleEvent(EVENT_FIERCE_KICK, 6000);
                        break;
                    case 60538:
                        events.ScheduleEvent(EVENT_FIERCE_JAB_2, 3000);
                        events.ScheduleEvent(EVENT_FISTS_OF_FURY, 8000);
                        events.ScheduleEvent(EVENT_VICIOUS_KICK, 13000);
                        break;
                    default:
                        break;
                }
            }


            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    switch (me->GetEntry())
                    {
                        case 60530:
                            if (CHECK_STATUS(30725))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60530);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60545:
                            if (CHECK_STATUS(30739))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60545);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60533:
                            if (CHECK_STATUS(30727))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60533);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        case 60538:
                            if (CHECK_STATUS(30732))
                            {
                                if (damage > me->GetHealth())
                                {
                                    damage = 0;
                                    me->SetFullHealth();
                                    DoAction(ACTION_REINITIALIZE);
                                    player->KilledMonsterCredit(60538);
                                    me->DespawnOrUnsummon();
                                }
                            }
                            break;
                        default:
                            break;
                        
                    }
                }
            }

            void SetGUID(uint64 guid, int32 index)
            {
                if (index == 0)
                    playerGuid = guid;
            }

            void DoAction(int32 const action)
            {
                if (action == ACTION_REMOVE_FLAG)
                {
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }

                else if (action == ACTION_REINITIALIZE)
                {
                    me->setFaction(35);
                    me->CombatStop();
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (Player* player = ObjectAccessor::FindPlayer(playerGuid))
                {
                    if (!player->isAlive())
                    {
                        DoAction(ACTION_REINITIALIZE);
                        return;
                    }
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // Entry 60530
                        case EVENT_JAB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_JAB, false);
                            events.ScheduleEvent(EVENT_JAB, 7000);
                            break;
                        case EVENT_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_KICK, false);
                            events.ScheduleEvent(EVENT_KICK, 7000);
                            break;
                            // Entry 60545
                        case EVENT_CRY_OUT_IN_DESPAIR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_CRY_OUT_IN_DESPAIR, false);
                            events.ScheduleEvent(EVENT_CRY_OUT_IN_DESPAIR, 15000);
                            break;
                        case EVENT_SHA_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_BLAST, false);
                            events.ScheduleEvent(EVENT_SHA_BLAST, 15000);
                            break;
                        case EVENT_SHA_SPIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_SHA_SPIKE, false);
                            events.ScheduleEvent(EVENT_SHA_SPIKE, 15000);
                            break;
                            // Entry 60533
                        case EVENT_FIERCE_JAB:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FIERCE_JAB, false);
                            events.ScheduleEvent(EVENT_FIERCE_JAB, 7000);
                            break;
                        case EVENT_FIERCE_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FIERCE_KICK, false);
                            events.ScheduleEvent(EVENT_FIERCE_KICK, 7000);
                            break;
                            // Entry 60538
                        case EVENT_FIERCE_JAB_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FIERCE_JAB, false);
                            events.ScheduleEvent(EVENT_FIERCE_JAB_2, 15000);
                            break;
                        case EVENT_FISTS_OF_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_FISTS_OF_FURY, false);
                            events.ScheduleEvent(EVENT_FISTS_OF_FURY, 15000);
                            break;
                        case EVENT_VICIOUS_KICK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_VICIOUS_KICK, false);
                            events.ScheduleEvent(EVENT_VICIOUS_KICK, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

void AddSC_krasarang_wilds()
{
    new mob_gaarn_the_toxic();
    new mob_arness_the_scale();
    new mob_qu_nas();
    new mob_torik_ethis();
    new mob_go_kan();
    new mob_spirit_of_the_crane();
    new mob_champion_of_chi_ji();
    new spell_chi_torpedo_periodic();
    new mob_anduin_wrynn();
    new mob_anduin_wrynn_escort();
    new npc_chi_ji();
    new mob_sha_of_despair();
    new npc_thelonius();
    new mob_ellia_ravenmane();
}