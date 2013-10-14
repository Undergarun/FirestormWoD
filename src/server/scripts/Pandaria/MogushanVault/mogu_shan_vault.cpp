/*
 * Copyright (C) 2012-2013 JadeCore <http://www.pandashan.com/>
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

#include "ScriptedCreature.h"
#include "mogu_shan_vault.h"
#include "ScriptMgr.h"
#include "ScriptedGossip.h"
#include "ScriptedEscortAI.h"
#include "CreatureAI.h"
#include "MoveSplineInit.h"

enum spells
{
    SPELL_SPIRIT_BOLT           = 121224,
    SPELL_GROUND_SLAM           = 121087,
    SPELL_PETRIFICATION         = 125090,
    SPELL_PETRIFIED             = 125092,
    SPELL_FULLY_PETRIFIED       = 115877,
    SPELL_MONSTROUS_BITE        = 125096,
    SPELL_SUNDERING_BITE        = 116970,
    SPELL_PROTECTIVE_FENZY      = 116982,
    SPELL_SHATTERING_STONE      = 116977,
    SPELL_FOCUSED_ASSAULT       = 116990
};

class mob_cursed_mogu_sculpture : public CreatureScript
{
    public:
        mob_cursed_mogu_sculpture() : CreatureScript("mob_cursed_mogu_sculture") {}

        struct mob_cursed_mogu_sculptureAI : public ScriptedAI
        {
            mob_cursed_mogu_sculptureAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;
            uint32 spiritBoltTimer;
            uint32 groundSlamTimer;

            Creature* GetCho()
            {
                if (pInstance)
                    return me->GetMap()->GetCreature(pInstance->GetData64(NPC_LOREWALKER_CHO));
                else
                    return NULL;
            }

            void Reset()
            {
                spiritBoltTimer = urand(10000, 40000);
                groundSlamTimer = urand(40000, 60000);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);

                if (me->GetEntry() == 61334)
                {
                    me->AddAura(120613, me); // Pose
                    me->AddAura(120661, me); // Bronze
                }
                else if (me->GetEntry() == 61989)
                {
                    me->AddAura(120650, me); // Pose
                    me->AddAura(120663, me); // Pierre
                }
            }

            void EnterCombat(Unit* attacker)
            {
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                me->RemoveAurasDueToSpell(120661);
                me->RemoveAurasDueToSpell(120613);
                me->RemoveAurasDueToSpell(120650);
                me->RemoveAurasDueToSpell(120663);
            }

            void JustDied(Unit* killer)
            {
                if (!GetClosestCreatureWithEntry(me, me->GetEntry(), 50.0f, true))
                {
                    Creature* Feng = GetClosestCreatureWithEntry(me, 60009, 100.0f, true);
                    if (Feng && Feng->AI())
                        Feng->AI()->Talk(7);
                }

                if (me->GetEntry() == 61989)
                {
                    if (Creature* lorewalkerCho = GetClosestCreatureWithEntry(me, 61348, 100.0f, true))
                    {
                        if (lorewalkerCho->AI())
                        {
                            if (lorewalkerCho->GetPositionX() >= 3963.0f && lorewalkerCho->GetPositionX() <= 3966.0f &&
                                lorewalkerCho->GetPositionY() >= 1340.5f && lorewalkerCho->GetPositionY() <= 1342.5f &&
                                lorewalkerCho->GetPositionZ() >= 460.0f && lorewalkerCho->GetPositionZ() <= 463.0f)
                            {
                                lorewalkerCho->AI()->DoAction(ACTION_CONTINUE_ESCORT);
                                lorewalkerCho->AI()->Talk(7);
                            }
                        }
                    }
                }
                else
                    return;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (spiritBoltTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                        if (!target->IsFriendlyTo(me))
                            me->CastSpell(target, SPELL_SPIRIT_BOLT, true);
                    spiritBoltTimer = urand(10000, 30000);
                }
                else
                    spiritBoltTimer -= diff;

                if (groundSlamTimer <= diff)
                {
                    if (Unit* target = me->SelectNearestTarget(5.0f))
                        if (!target->IsFriendlyTo(me))
                            me->CastSpell(target, SPELL_GROUND_SLAM, true);
                    groundSlamTimer = urand(40000, 60000);
                }
                else
                    groundSlamTimer -= diff;

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_cursed_mogu_sculptureAI(creature);
        }
};

float quilenNewY[2] = {1170.0f, 1240.0f};

enum enormousQuilenEvent
{
    EVENT_MONSTROUS_BITE = 1,
    EVENT_NEXT_MOVEMENT  = 2
};

class mob_enormous_stone_quilen : public CreatureScript
{
    public:
        mob_enormous_stone_quilen() : CreatureScript("mob_enormous_stone_quilen") {}

        struct mob_enormous_stone_quilenAI : public ScriptedAI
        {
            mob_enormous_stone_quilenAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
                prevPosition = 1;

                if (me->GetPositionX() > 3900)
                    prevPosition = 2;

                nextMovementTimer = 0;
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(prevPosition, me->GetPositionX(), quilenNewY[prevPosition - 1], me->GetPositionZ());
            }

            InstanceScript* pInstance;
            EventMap events;
            uint32 nextMovementTimer;
            uint8 prevPosition;

            void Reset()
            {
                events.ScheduleEvent(EVENT_MONSTROUS_BITE, urand (3000, 5000));
            }

            void JustReachedHome()
            {
                prevPosition = 1;

                if (me->GetPositionX() > 3900)
                    prevPosition = 2;

                nextMovementTimer = 0;
                me->SetWalk(true);
                me->GetMotionMaster()->MovePoint(prevPosition, me->GetPositionX(), quilenNewY[prevPosition - 1], me->GetPositionZ());
            }

            void MovementInform(uint32 typeId, uint32 pointId)
            {
                if (typeId != POINT_MOTION_TYPE)
                    return;

                if (me->isInCombat())
                    return;

                prevPosition = pointId;
                nextMovementTimer = 500;
            }

            void EnterCombat(Unit* attacker)
            {
                me->SetWalk(false);
                //me->AddAura(SPELL_PETRIFICATION, me);
            }

            void UpdateAI(const uint32 diff)
            {
                if (nextMovementTimer)
                {
                    if (nextMovementTimer <= diff)
                    {
                        me->GetMotionMaster()->MovePoint(prevPosition == 2 ? 1: 2, me->GetPositionX(), quilenNewY[prevPosition == 2 ? 0: 1], me->GetPositionZ());
                        nextMovementTimer = 0;
                    }
                    else
                        nextMovementTimer -= diff;
                }

                if (!UpdateVictim())
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MONSTROUS_BITE:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                            {
                                if (!target->IsFriendlyTo(me))
                                    me->CastSpell(target, SPELL_MONSTROUS_BITE, true);
                            }
                            events.ScheduleEvent(EVENT_MONSTROUS_BITE, urand(6000, 8000));
                            break;
                        }
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_enormous_stone_quilenAI(creature);
        }
};

enum eQuilenEvents
{
    EVENT_SUNDERING_BITE    = 1,
    EVENT_SHATTERING_STONE  = 2,
    EVENT_fOCUSED_ASSAULT   = 3
};

class mob_stone_quilen : public CreatureScript
{
    public:
        mob_stone_quilen() : CreatureScript("mob_stone_quilen") {}

        struct mob_stone_quilenAI : public ScriptedAI
        {
            mob_stone_quilenAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_SUNDERING_BITE,   urand (5000, 6000));
                events.ScheduleEvent(EVENT_SHATTERING_STONE, urand (10000, 12000));
                events.ScheduleEvent(EVENT_fOCUSED_ASSAULT,  urand (500, 5000));
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (!me->HasAura(SPELL_PROTECTIVE_FENZY) && me->HealthBelowPct(10))
                    me->CastSpell(me, SPELL_PROTECTIVE_FENZY, true);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUNDERING_BITE:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->CastSpell(target, SPELL_SUNDERING_BITE, true);

                            events.ScheduleEvent(EVENT_SUNDERING_BITE,   urand (5000, 6000));
                            break;
                        }
                        case EVENT_SHATTERING_STONE:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->CastSpell(target, SPELL_SHATTERING_STONE, true);

                            events.ScheduleEvent(EVENT_SHATTERING_STONE, urand (10000, 12000));
                            break;
                        }
                        case EVENT_fOCUSED_ASSAULT:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->AddAura(SPELL_FOCUSED_ASSAULT, target);

                            events.ScheduleEvent(EVENT_fOCUSED_ASSAULT,  urand (500, 5000));
                            break;
                        }
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_stone_quilenAI(creature);
        }
};

enum eSkullCharger
{
    SPELL_TROLL_RUSH    = 116006,
    EVENT_TROLL_RUSH    = 1,
};

class mob_zandalari_skullcharger : public CreatureScript
{
    public:
        mob_zandalari_skullcharger() : CreatureScript("mob_zandalari_skullcharger") {}

        struct mob_zandalari_skullchargerAI : public ScriptedAI
        {
            mob_zandalari_skullchargerAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();

                events.ScheduleEvent(EVENT_TROLL_RUSH, urand (5000, 6000));
            }

            void EnterCombat(Unit* attacker)
            {
                if (Creature* lorewalkerCho = GetClosestCreatureWithEntry(me, 61348, 150.0f, true))
                {
                    if (lorewalkerCho->AI())
                    {
                        lorewalkerCho->AI()->DoAction(ACTION_SAY_ZANDALARI_BEGIN);
                    }
                }
            }

            void JustDied(Unit* killer)
            {
                if (Creature* lorewalkerCho = GetClosestCreatureWithEntry(me, 61348, 150.0f, true))
                {
                    if (lorewalkerCho->AI())
                    {
                        if (lorewalkerCho->GetPositionX() == 4128.93f && lorewalkerCho->GetPositionY() == 1351.51f && lorewalkerCho->GetPositionZ() == 461.886f)
                        {
                            lorewalkerCho->AI()->DoAction(ACTION_SAY_ZANDALARI_DEATH);
                            lorewalkerCho->AI()->DoAction(ACTION_CONTINUE_ESCORT);
                        }
                    }
                }
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
                        case EVENT_TROLL_RUSH:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                            {
                                me->CastSpell(target, SPELL_TROLL_RUSH, true);
                                me->GetMotionMaster()->MoveChase(target);
                            }
                            events.ScheduleEvent(EVENT_TROLL_RUSH,   urand (5000, 6000));
                            break;
                        }

                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zandalari_skullchargerAI(creature);
        }
};

class spell_mogu_petrification : public SpellScriptLoader
{
    public:
        spell_mogu_petrification() : SpellScriptLoader("spell_mogu_petrification") { }

        class spell_mogu_petrification_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_mogu_petrification_AuraScript);

            uint32 stack;

            void OnApply(constAuraEffectPtr aurEff, AuraEffectHandleModes /*mode*/)
            {
                if (Unit* caster = GetCaster())
                {
                    if (Unit* target = GetTarget())
                    {
                        if (target->HasAura(SPELL_PETRIFIED))
                        {
                            stack = GetTarget()->GetAura(SPELL_PETRIFIED)->GetStackAmount();

                            if (stack >= 100)
                                target->AddAura(SPELL_FULLY_PETRIFIED, target);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectApply += AuraEffectApplyFn(spell_mogu_petrification_AuraScript::OnApply, EFFECT_0, SPELL_AURA_MOD_DECREASE_SPEED, AURA_EFFECT_HANDLE_REAPPLY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_mogu_petrification_AuraScript();
        }
};

#define GOSSIP_CHOICE_1  "What are you waiting for ? Let's go investigate !"

enum eChoTexts
{
    INTRO_01           =  0,
    INTRO_02           =  1,
    INTRO_03           =  2,
    INTRO_04           =  3,
    INTRO_05           =  4,
    INTRO_06           =  5,
    INTRO_07           =  6,
    INTRO_08           =  7,
    FENG_01            =  8,
    FENG_02            =  9,
    FENG_03            = 10,
    ZANDALARI_01       = 11,
    ZANDALARI_02       = 12,
    SPIRIT_KINGS_01    = 13,
};

class npc_lorewalker_cho : public CreatureScript
{
public:
    npc_lorewalker_cho() : CreatureScript("npc_lorewalker_cho")
    {
    }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
            player->SEND_GOSSIP_MENU(30003, creature->GetGUID());

            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*sender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            if (action == GOSSIP_ACTION_INFO_DEF + 1)
            {
                player->CLOSE_GOSSIP_MENU();
                creature->AI()->DoAction(ACTION_CONTINUE_ESCORT);
            }

            return true;
        }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_lorewalker_choAI(creature);
    }

    struct npc_lorewalker_choAI : public npc_escortAI
    {
        npc_lorewalker_choAI(Creature* creature) : npc_escortAI(creature)
        {
            pInstance = creature->GetInstanceScript();
        }

        bool hasSaidIntro;
        bool hasSaidStoneguardDeath;
        bool hasSaidZandalariBegin;
        bool hasSaidZandalariDeath;
        InstanceScript* pInstance;

        void Reset()
        {
            SetDespawnAtFar(false);
            SetDespawnAtEnd(false);
            hasSaidIntro = false;
            hasSaidStoneguardDeath = false;
            hasSaidZandalariBegin = false;
            hasSaidZandalariDeath = false;
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (hasSaidIntro || who->GetTypeId() != TYPEID_PLAYER)
                return;

            if (me->IsWithinDistInMap(who, 50.0f))
            {
                Talk(INTRO_01);
                hasSaidIntro = true;
                Start(false, false, who->GetGUID());
            }
        }

        void WaypointReached(uint32 waypointId)
        {
            Player* player = GetPlayerForEscort();
            if (!player)
                return;

            switch(waypointId)
            {
                case 0:
                    Talk(INTRO_02);
                    break;
                case 3:
                    Talk(INTRO_03);
                    SetEscortPaused(true);
                    SetRun(true);
                    break;
                case 6:
                    Talk(INTRO_06);
                    break;
                case 9:
                    SetRun(false);
                    break;
                case 10:
                    Talk(INTRO_07);
                    break;
                case 12:
                    SetEscortPaused(true);
                    break;
                case 13:
                    Talk(FENG_01);
                    SetEscortPaused(true);
                    break;
                case 19:
                    Talk(FENG_03);
                    SetEscortPaused(true);
                case 21:
                    SetEscortPaused(true);
                    break;
                case 36:
                    SetRun(false);
                    break;
                case 38:
                    me->SetOrientation(4.68f);
                    me->SetFacingTo(4.68f);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    SetEscortPaused(true);
                    break;
                case 39:
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    break;
                case 42:
                    me->SetOrientation(1.51f);
                    me->SetFacingTo(1.51f);
                    SetEscortPaused(true);
                    Talk(SPIRIT_KINGS_01);
                default:
                    break;
            }
        }



        void DoAction(const int32 action)
        {
            switch (action)
            {
                case ACTION_CONTINUE_ESCORT:
                    SetEscortPaused(false);
                    break;
                case ACTION_RUN:
                    SetRun(true);
                    break;
                case ACTION_SAY_ZANDALARI_BEGIN:
                    if (!hasSaidZandalariBegin)
                    {
                        Talk(ZANDALARI_01);
                        hasSaidZandalariBegin = true;
                    }
                    break;
                case ACTION_SAY_ZANDALARI_DEATH:
                    if (!hasSaidZandalariDeath)
                    {
                        Talk(ZANDALARI_02);
                        hasSaidZandalariDeath = true;
                    }
                    break;
                default:
                    break;
            }
        }

        void JustSummoned(Creature* summoned)
        {
        }

        void UpdateAI(const uint32 diff)
        {
            npc_escortAI::UpdateAI(diff);
            if (GameObject* pDoor = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_STONE_GUARD_DOOR_EXIT)))
            {
                if (pDoor->GetGoState() == GO_STATE_ACTIVE)
                {
                    if (me->GetPositionX() == 3869.48f && me->GetPositionY() == 1123.95f && me->GetPositionZ() >= 481.24f)
                    {
                        if (hasSaidStoneguardDeath == false)
                        {
                            DoAction(ACTION_CONTINUE_ESCORT);
                            Talk(INTRO_05);
                            hasSaidStoneguardDeath = true;
                        }
                        else
                            return;
                    }
                }
            }
        }
    };

};

void AddSC_mogu_shan_vault()
{
    new mob_cursed_mogu_sculpture();
    new mob_enormous_stone_quilen();
    new mob_stone_quilen();
    new mob_zandalari_skullcharger();
    new spell_mogu_petrification();
    new npc_lorewalker_cho();
}