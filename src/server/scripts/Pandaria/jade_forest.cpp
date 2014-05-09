#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "jade_forest.h"

class mob_shadow_of_doubt : public CreatureScript
{
    public:
        mob_shadow_of_doubt() : CreatureScript("mob_shadow_of_doubt") 
		{ 
		}

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_shadow_of_doubtAI(creature);
        }

        struct mob_shadow_of_doubtAI : public ScriptedAI
        {
            mob_shadow_of_doubtAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                
                events.ScheduleEvent(EVENT_DEAFENED, 30000);
                events.ScheduleEvent(EVENT_GROWING_DOUBT,  9000);
                events.ScheduleEvent(EVENT_SHA_BOLT, 15000);
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
                        case EVENT_DEAFENED:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_DEAFENED, false);
                            events.ScheduleEvent(EVENT_DEAFENED, 30000);
                            break;
                        case EVENT_GROWING_DOUBT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWING_DOUBT, false);
                            events.ScheduleEvent(EVENT_GROWING_DOUBT, 9000);
                            break;
                        case EVENT_SHA_BOLT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHA_BOLT, false);
                            events.ScheduleEvent(EVENT_SHA_BOLT, 15000);
                            break;

                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_kor_nas_nightsavage : public CreatureScript
{
    public:
        mob_kor_nas_nightsavage() : CreatureScript("mob_kor_nas_nightsavage")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kor_nas_nightsavageAI(creature);
        }

        struct mob_kor_nas_nightsavageAI : public ScriptedAI
        {
            mob_kor_nas_nightsavageAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOWBOLT, 50000);
                events.ScheduleEvent(EVENT_VOIDCLOUD, 15000);
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
                            events.ScheduleEvent(EVENT_SHADOWBOLT, 70000);
                            break;
                        case EVENT_VOIDCLOUD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_VOIDCLOUD, false);
                            events.ScheduleEvent(EVENT_VOIDCLOUD, 35000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_krax_ik : public CreatureScript
{
    public:
        mob_krax_ik() : CreatureScript("mob_krax_ik")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_krax_ikAI(creature);
        }

        struct mob_krax_ikAI : public ScriptedAI
        {
            mob_krax_ikAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->CastSpell(me, SPELL_STEALTH, true);

                events.ScheduleEvent(EVENT_TORNADO, 5000);
                events.ScheduleEvent(EVENT_BLADE_FURY, 25000);
                events.ScheduleEvent(EVENT_WINDSONG, 30000);
            }

            void EnterCombat(Unit* attacker)
            {
                me->RemoveAurasDueToSpell(SPELL_STEALTH);
            }

            void JustSummoned(Creature* summon)
            {
                if (summon->GetEntry() == 64267)
                {
                    summon->DespawnOrUnsummon(15000);
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
                        case EVENT_TORNADO:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TORNADO, false);
                            events.ScheduleEvent(EVENT_TORNADO, 70000);
                            break;
                        case EVENT_BLADE_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLADE_FURY, false);
                            events.ScheduleEvent(EVENT_BLADE_FURY, 20000);
                            break;
                        case EVENT_WINDSONG:
                            me->CastSpell(me, SPELL_WINDSONG, false);
                            events.ScheduleEvent(EVENT_WINDSONG, 30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_mister_ferocious : public CreatureScript
{
    public:
        mob_mister_ferocious() : CreatureScript("mob_mister_ferocious")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mister_ferociousAI(creature);
        }

        struct mob_mister_ferociousAI : public ScriptedAI
        {
            mob_mister_ferociousAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_GOING_BANANAS, 12000);
                events.ScheduleEvent(EVENT_BANANARANG, 8000);
                events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
            }

            void JustSummoned(Creature* summon)
            {
                summon->DespawnOrUnsummon(12000);
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
                        case EVENT_GOING_BANANAS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GOING_BANANAS, false);
                            events.ScheduleEvent(EVENT_GOING_BANANAS, 10000);
                            break;
                        case EVENT_BANANARANG:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BANANARANG, false);
                            events.ScheduleEvent(EVENT_BANANARANG, 20000);
                            break;
                        case EVENT_TOSS_FILTH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TOSS_FILTH, false);
                            events.ScheduleEvent(EVENT_TOSS_FILTH, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_akkalar : public CreatureScript
{
    public:
        mob_akkalar() : CreatureScript("mob_akkalar")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_akkalarAI(creature);
        }

        struct mob_akkalarAI : public ScriptedAI
        {
            mob_akkalarAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BOUCLIER_D_EAU, 10000);
                events.ScheduleEvent(EVENT_BOUCLIER_D_EAU_2, 20000);
                events.ScheduleEvent(EVENT_BOUCLIER_DE_TERRE, 30000);
                events.ScheduleEvent(EVENT_CARAPACE_BOUCLIER, 40000);
                events.ScheduleEvent(EVENT_MOT_DE_POUVOIR, 50000);
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
                        case EVENT_BOUCLIER_D_EAU:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_D_EAU, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_D_EAU, 62000);
                            break;
                        case EVENT_BOUCLIER_D_EAU_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_D_EAU_2, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_D_EAU_2, 70000);
                            break;
                        case EVENT_BOUCLIER_DE_TERRE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BOUCLIER_DE_TERRE, false);
                            events.ScheduleEvent(EVENT_BOUCLIER_DE_TERRE, 80000);
                            break;
                        case EVENT_CARAPACE_BOUCLIER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CARAPACE_BOUCLIER, false);
                            events.ScheduleEvent(EVENT_CARAPACE_BOUCLIER, 90000);
                            break;
                        case EVENT_MOT_DE_POUVOIR:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_MOT_DE_POUVOIR, false);
                            events.ScheduleEvent(EVENT_MOT_DE_POUVOIR, 100000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_grookin_outrunner : public CreatureScript
{
    public:
        mob_grookin_outrunner() : CreatureScript("mob_grookin_outrunner")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_grookin_outrunnerAI(creature);
        }

        struct mob_grookin_outrunnerAI : public ScriptedAI
        {
            mob_grookin_outrunnerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHOOT, 7000);
                events.ScheduleEvent(EVENT_CLEAVE, 12000);
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
                        case EVENT_SHOOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHOOT, false);
                            events.ScheduleEvent(EVENT_SHOOT, 10000);
                            break;
                        case EVENT_CLEAVE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLEAVE, false);
                            events.ScheduleEvent(EVENT_CLEAVE, 10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_bamboo_python : public CreatureScript
{
    public:
        mob_bamboo_python() : CreatureScript("mob_bamboo_python")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_bamboo_pythonAI(creature);
        }

        struct mob_bamboo_pythonAI : public ScriptedAI
        {
            mob_bamboo_pythonAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SWAMP_FEVER,	7000);
            }

            void EnterCombat(Unit* attacker)
            {
                me->RemoveAurasDueToSpell(SPELL_STEALTH);
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
                    if (eventId == EVENT_SWAMP_FEVER)
                    {
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SWAMP_FEVER, false);
                            events.ScheduleEvent(EVENT_SWAMP_FEVER, 15000);
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_rakira : public CreatureScript
{
    public:
        mob_rakira() : CreatureScript("mob_rakira")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_rakiraAI(creature);
        }

        struct mob_rakiraAI : public ScriptedAI
        {
            mob_rakiraAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLOODY_PREY,	8000);
                events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                events.ScheduleEvent(EVENT_GROWL, 22000);
                events.ScheduleEvent(EVENT_HOWL_OF_THE_DENMOTHER, 30000);
                events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 40000);
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
                        case EVENT_BLOODY_PREY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODY_PREY, false);
                            events.ScheduleEvent(EVENT_BLOODY_PREY, 10000);
                            break;
                        case EVENT_CLAW_RIP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_RIP, false);
                            events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                            break;
                        case EVENT_GROWL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWL, false);
                            events.ScheduleEvent(EVENT_GROWL, 22000);
                            break;
                        case EVENT_HOWL_OF_THE_DENMOTHER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_HOWL_OF_THE_DENMOTHER, false);
                            events.ScheduleEvent(EVENT_HOWL_OF_THE_DENMOTHER, 30000);
                            break;
                        case EVENT_ROAR_OF_COURAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROAR_OF_COURAGE, false);
                            events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 47000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_ro_shen : public CreatureScript
{
    public:
        mob_ro_shen() : CreatureScript("mob_ro_shen")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_ro_shenAI(creature);
        }

        struct mob_ro_shenAI : public ScriptedAI
        {
            mob_ro_shenAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_BLOODY_PREY, 8000);
                events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                events.ScheduleEvent(EVENT_GROWL, 22000);
                events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 30000);
                events.ScheduleEvent(EVENT_BLOOD_CRAZED, 40000);
                events.ScheduleEvent(EVENT_BLOODTHIRST, 50000);
                events.ScheduleEvent(EVENT_CLAW_SLASH, 60000);
                events.ScheduleEvent(EVENT_FRENZY, 70000);
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
                        case EVENT_BLOODY_PREY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODY_PREY, false);
                            events.ScheduleEvent(EVENT_BLOODY_PREY, 10000);
                            break;
                        case EVENT_CLAW_RIP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_RIP, false);
                            events.ScheduleEvent(EVENT_CLAW_RIP, 15000);
                            break;
                        case EVENT_GROWL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_GROWL, false);
                            events.ScheduleEvent(EVENT_GROWL, 28000);
                            break;
                        case EVENT_ROAR_OF_COURAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROAR_OF_COURAGE, false);
                            events.ScheduleEvent(EVENT_ROAR_OF_COURAGE, 70000);
                            break;
                        case EVENT_BLOOD_CRAZED:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOOD_CRAZED, false);
                            events.ScheduleEvent(EVENT_BLOOD_CRAZED, 31000);
                            break;
                        case EVENT_BLOODTHIRST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_BLOODTHIRST, false);
                            events.ScheduleEvent(EVENT_BLOODTHIRST, 40000);
                            break;
                        case EVENT_CLAW_SLASH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CLAW_SLASH, false);
                            events.ScheduleEvent(EVENT_CLAW_SLASH, 25000);
                            break;
                        case EVENT_FRENZY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FRENZY, false);
                            events.ScheduleEvent(EVENT_FRENZY, 50000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_sha_reminant : public CreatureScript
{
    public:
        mob_sha_reminant() : CreatureScript("mob_sha_reminant")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sha_reminantAI(creature);
        }

        struct mob_sha_reminantAI : public ScriptedAI
        {
            mob_sha_reminantAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SHADOW_CLAW, 10000);
                events.ScheduleEvent(EVENT_OVERWHELMING_GUILT, 15000);
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
                        case EVENT_SHADOW_CLAW:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOW_CLAW, false);
                            events.ScheduleEvent(EVENT_SHADOW_CLAW, 10000);
                            break;
                        case EVENT_OVERWHELMING_GUILT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_OVERWHELMING_GUILT, false);
                            events.ScheduleEvent(EVENT_OVERWHELMING_GUILT, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

#define GOSSIP_CHOICE_1 "Challenge the Patriarch."

class mob_pandriarch_windfur : public CreatureScript
{
    public:
        mob_pandriarch_windfur() : CreatureScript("mob_pandriarch_windfur")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75009, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(QUEST_GETTING_PERMISSION) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }

                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_pandriarch_windfurAI(creature);
        }

        struct mob_pandriarch_windfurAI : public ScriptedAI
        {
            mob_pandriarch_windfurAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_THUNDERING_PALM, urand (3000, 7000));
                events.ScheduleEvent(EVENT_WIND_SWORD, urand (8000, 10000));
//                events.ScheduleEvent(EVENT_WINDFUR_PUNCH, urand (13000, 17000));
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage))
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1996.76001f, -2216.780029f, 247.725006f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(NPC_PANDRIARCH_WINDFUR);
                    }

                    if (damage > me->GetHealth())
                        damage = 0;
                }
            }

            void EnterCombat(Unit* who)
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(QUEST_GETTING_PERMISSION) == QUEST_STATUS_INCOMPLETE)
                        return;

                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
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
                        case EVENT_THUNDERING_PALM:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_THUNDERING_PALM, false);
                            events.ScheduleEvent(EVENT_THUNDERING_PALM, 10000);
                            break;
                        case EVENT_WIND_SWORD:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WIND_SWORD, false);
                            events.ScheduleEvent(EVENT_WIND_SWORD, 15000);
                            break;
                        case EVENT_WINDFUR_PUNCH:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WINDFUR_PUNCH, false);
                            events.ScheduleEvent(EVENT_WINDFUR_PUNCH, 15000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_pandriarch_bramblestaff : public CreatureScript
{
    public:
        mob_pandriarch_bramblestaff() : CreatureScript("mob_pandriarch_bramblestaff")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75010, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(QUEST_GETTING_PERMISSION) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }

                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_pandriarch_bramblestaffAI(creature);
        }

        struct mob_pandriarch_bramblestaffAI : public ScriptedAI
        {
            mob_pandriarch_bramblestaffAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_ROLL, urand (3000, 7000));
                events.ScheduleEvent(EVENT_STAFF_STRIKE, urand (8000, 10000));
                events.ScheduleEvent(EVENT_THROW_BRAMBLESTAFF, urand (13000, 17000));
//                events.ScheduleEvent(EVENT_WHIRLWIND, urand (20000, 23000));
//                events.ScheduleEvent(EVENT_WHIRLWIND_2, urand (24000, 27000));
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1862.300049f, -2325.060059f, 257.062012f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(NPC_PANDRIARCH_BRAMBLESTAFF);
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(QUEST_GETTING_PERMISSION) == QUEST_STATUS_INCOMPLETE)
                        return;

                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
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
                        case EVENT_ROLL:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ROLL, false);
                            events.ScheduleEvent(EVENT_ROLL,      10000);
                            break;
                        case EVENT_STAFF_STRIKE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_STAFF_STRIKE, false);
                            events.ScheduleEvent(EVENT_STAFF_STRIKE,      10000);
                            break;
                        case EVENT_THROW_BRAMBLESTAFF:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_THROW_BRAMBLESTAFF, false);
                            events.ScheduleEvent(EVENT_THROW_BRAMBLESTAFF,      10000);
                            break;
                        case EVENT_WHIRLWIND:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WHIRLWIND, false);
                            events.ScheduleEvent(EVENT_WHIRLWIND,      10000);
                            break;
                        case EVENT_WHIRLWIND_2:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WHIRLWIND_2, false);
                            events.ScheduleEvent(EVENT_WHIRLWIND_2,      10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_pandriarch_goldendraft : public CreatureScript
{
    public:
        mob_pandriarch_goldendraft() : CreatureScript("mob_pandriarch_goldendraft")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75010, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(QUEST_GETTING_PERMISSION) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                    creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }

                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_pandriarch_goldendraftAI(creature);
        }

        struct mob_pandriarch_goldendraftAI : public ScriptedAI
        {
            mob_pandriarch_goldendraftAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_EXPLOSIVE_LAGER, urand (3000, 7000));
                events.ScheduleEvent(EVENT_FIRE_ALE, urand (8000, 10000));
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1942.630005f, -2290.530029f, 240.429001f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_NPC);
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(NPC_PANDRIARCH_GOLDENDRAFT);
                    }
                }
            }

            void EnterCombat(Unit* who)
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(QUEST_GETTING_PERMISSION) == QUEST_STATUS_INCOMPLETE)
                        return;

                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
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
                        case EVENT_EXPLOSIVE_LAGER:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_EXPLOSIVE_LAGER, false);
                            events.ScheduleEvent(EVENT_EXPLOSIVE_LAGER,      10000);
                            break;
                        case EVENT_FIRE_ALE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FIRE_ALE, false);
                            events.ScheduleEvent(EVENT_FIRE_ALE,      10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

#define GOSSIP_CHOICE_2 "Let's see which one of us is the better student. I challenge you to a duel !"

class mob_big_bao : public CreatureScript
{
    public:
        mob_big_bao() : CreatureScript("mob_big_bao")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(75012, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                if (player->GetQuestStatus(QUEST_TRAINER_S_CHALLENGE) == QUEST_STATUS_INCOMPLETE)
                {
                    creature->setFaction(14);
                    creature->SetReactState(REACT_DEFENSIVE);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    creature->AI()->Reset();
                    creature->CombatStart(player, true);
                }
                player->CLOSE_GOSSIP_MENU();
            }

            return true;
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_big_baoAI(creature);
        }

        struct mob_big_baoAI : public ScriptedAI
        {
            mob_big_baoAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_FORCE_BURST, 3000);
                events.ScheduleEvent(EVENT_KNOCK_AWAY, 8000);
                events.ScheduleEvent(EVENT_PULL_CLOSE, 13000);
                events.ScheduleEvent(EVENT_WAR_STOMP, 18000);
            }

            void EnterCombat(Unit* who)
            {
                if (Player* player = who->ToPlayer())
                {
                    if (player->GetQuestStatus(QUEST_TRAINER_S_CHALLENGE) == QUEST_STATUS_INCOMPLETE)
                        player->CastSpell(player, SPELL_RACE_AGAINST_THE_CLOCK);
                    else
                    {
                        me->CombatStop();
                        me->setFaction(35);
                    }
                }
            }

            void DamageTaken(Unit* attacker, uint32& damage)
            {
                if (Player* player = attacker->ToPlayer())
                {
                    if (me->HealthBelowPctDamaged(10, damage) || damage > me->GetHealth())
                    {
                        damage = 0;
                        me->CombatStop();
                        me->GetMotionMaster()->MovePoint(0, 1604.75f, -2562.139893f, 153.134003f);
                        me->setFaction(35);
                        me->SetFullHealth();
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        player->KilledMonsterCredit(MOB_BIG_BAO);
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
                        case EVENT_FORCE_BURST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCE_BURST, false);
                            events.ScheduleEvent(EVENT_FORCE_BURST, 10000);
                            break;
                        case EVENT_KNOCK_AWAY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_KNOCK_AWAY, false);
                            events.ScheduleEvent(EVENT_KNOCK_AWAY, 10000);
                            break;
                        case EVENT_PULL_CLOSE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_PULL_CLOSE, false);
                            events.ScheduleEvent(EVENT_PULL_CLOSE, 10000);
                            break;
                        case EVENT_WAR_STOMP:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FIRE_ALE, false);
                            events.ScheduleEvent(SPELL_WAR_STOMP, 10000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

class mob_hutia : public CreatureScript
{
    public:
        mob_hutia() : CreatureScript("mob_hutia")
        {
        }

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_hutiatAI(creature);
        }

        struct mob_hutiatAI : public ScriptedAI
        {
            mob_hutiatAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();

                me->CastSpell(me, SPELL_SPIRIT_MEND, false);
                me->CastSpell(me, SPELL_STRONG_WILL, false);

                events.ScheduleEvent(EVENT_FRENZY_2, 7000);
                events.ScheduleEvent(EVENT_SPIRIT_BEAST_BLESSING, 12000);
                events.ScheduleEvent(EVENT_SPIRIT_HEAL, 19000);
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
                        case EVENT_FRENZY_2:
                            me->CastSpell(me, SPELL_FRENZY_2, false);
                            events.ScheduleEvent(EVENT_FRENZY_2, 30000);
                            break;
                        case EVENT_SPIRIT_BEAST_BLESSING:
                            me->CastSpell(me, SPELL_SPIRIT_BEAST_BLESSING, false);
                            events.ScheduleEvent(EVENT_SPIRIT_BEAST_BLESSING, 40000);
                            break;
                        case EVENT_SPIRIT_HEAL:
                            me->CastSpell(me, SPELL_SPIRIT_HEAL_2, false);
                            events.ScheduleEvent(EVENT_SPIRIT_HEAL, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };
};

#define REPUTATION_ORDER_OF_THE_CLOUD_SERPENT 1271

class npc_elder_anli : public CreatureScript
{
    public:
        npc_elder_anli() : CreatureScript("npc_elder_anli")
        {
        }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (player->GetQuestStatus(QUEST_RIDING_THE_SKIES_FIRST) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RIDING_THE_SKIES_SECOND) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RIDING_THE_SKIES_THIRD) == QUEST_STATUS_INCOMPLETE)
                if (player->GetReputationRank(REPUTATION_ORDER_OF_THE_CLOUD_SERPENT) == REP_EXALTED)
                    player->KilledMonsterCredit(NPC_INSTRUCTOR_WINDBLADE);

            return true;
        }
};

void AddSC_jade_forest()
{
    //Rare mobs
    new mob_kor_nas_nightsavage();
    new mob_krax_ik();
    new mob_mister_ferocious();
    //Elite mobs
    new mob_shadow_of_doubt();
    new mob_akkalar();
    new mob_grookin_outrunner();
    new mob_bamboo_python();
    new mob_rakira();
    new mob_ro_shen();
    new mob_sha_reminant();
    new mob_hutia();
    //Standard Mobs
    new mob_pandriarch_windfur();
    new mob_pandriarch_bramblestaff();
    new mob_pandriarch_goldendraft();
    new mob_big_bao();
    new npc_elder_anli();
}