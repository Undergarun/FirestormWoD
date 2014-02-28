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

enum eSpells
{
    SPELL_SPIRIT_BOLT               = 121224,
    SPELL_GROUND_SLAM               = 121087,
    SPELL_PETRIFICATION             = 125090,
    SPELL_PETRIFIED                 = 125092,
    SPELL_FULLY_PETRIFIED           = 115877,
    SPELL_MONSTROUS_BITE            = 125096,
    SPELL_SUNDERING_BITE            = 116970,
    SPELL_PROTECTIVE_FENZY          = 116982,
    SPELL_SHATTERING_STONE          = 116977,
    SPELL_FOCUSED_ASSAULT           = 116990,
    SPELL_POSE_1                    = 120650,
    SPELL_POSE_2                    = 120613,
    SPELL_STONE                     = 120663,
    SPELL_BRONZE                    = 120661,
    SPELL_GHOST_ESSENCE             = 120764,
    SPELL_INACTIVE                  = 118205,
    SPELL_ACTIVATION_VISUAL         = 118212,
    SPELL_CHARGED_SHADOWS           = 117685,
    SPELL_SHADOW_BLAST              = 119365,
    SPELL_ANNIHILATE                = 119521,
    SPELL_COWARDICE                 = 119635,
    SPELL_VOLLEY                    = 119554,
    SPELL_TROLL_RUSH                = 116006,
    SPELL_CRUSHING_ATTACKS          = 119514,
    SPELL_SHOOT                     = 119524,
    SPELL_ENRAGE                    = 119629,
    SPELL_REFLECTIVE_SHIELDS        = 119630,
    SPELL_FLESH_TO_STONE            = 118552,
    SPELL_STONE_BLOCK               = 118529,
    SPELL_FORCEFUL_SWING            = 126955,
    SPELL_TOUCH_OF_NALAK_SHA        = 126958,
    SPELL_WARDEN_S_FURY             = 126970,
    SPELL_ENHANCED_RECONSTRUCTION   = 126980,
    SPELL_RECONSTRUCTING            = 126985
};

enum eEvents
{
    EVENT_CURSED_MOGU_SPIRIT_BOLT,
    EVENT_CURSED_MOGU_GROUND_SLAM,
    EVENT_ENORMOUS_QUILEN_BITE,
    EVENT_QUILEN_SUNDERING_BITE,
    EVENT_QUILEN_SHATTERING_STONE,
    EVENT_QUILEN_FOCUSED_ASSAULT,
    EVENT_ZANDALARI_TROLL_RUSH,
    EVENT_ZIAN_CHARGED_SHADOWS,
    EVENT_SORCERER_SHADOW_BLAST,
    EVENT_QIANG_ANNIHILATE,
    EVENT_QIANG_START_SECOND_FIGHT,
    EVENT_MOUNTED_MOGU_CRUSHING_ATTACKS,
    EVENT_SUBETAI_VOLLEY,
    EVENT_SUBETAI_START_THIRD_COMBAT,
    EVENT_MOGU_ARCHER_SHOOT,
    EVENT_MENG_START_FOURTH_COMBAT,
    EVENT_MENG_COWARDICE,
    EVENT_KINGS_GUARD_ENRAGE,
    EVENT_KINGS_GUARD_REFLECTIVE_SHIELDS,
    EVENT_SECRET_FLESH_TO_STONE,
    EVENT_SECRET_STONE_BLOCK,
    EVENT_WARDEN_FORCEFUL_SWING,
    EVENT_WARDEN_TOUCH_OF_NALAK_SHA,
    EVENT_WARDEN_WARDEN_S_FURY,
    EVENT_KEEPER_ENHANCED_RECONSTRUCTION,
    EVENT_KEEPER_RECONSTRUCTING
};

enum eEquipIds
{
    EQUIP_ZIAN_STAFF            = 76369,
    EQUIP_QIANG_POLEARM         = 86777,
    EQUIP_MOUNTED_MOGU_WEAPON   = 80132,
    EQUIP_SUBETAI_SWORD         = 80283,
    EQUIP_SUBETAI_BOW           = 60790
};

enum eTrashsActions
{
    ACTION_CURSED_MOGU_ATTACK_PLAYER = 1
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
            bool activationDone;
            uint64 playerActivate;

            Creature* GetCho()
            {
                if (pInstance)
                    return me->GetMap()->GetCreature(pInstance->GetData64(NPC_LOREWALKER_CHO));
                else
                    return NULL;
            }

            void Reset()
            {
                events.Reset();

                playerActivate = 0;
                activationDone = false;

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->AI()->SetCanSeeEvenInPassiveMode(true);
                me->RemoveAurasDueToSpell(SPELL_GHOST_ESSENCE);

                switch (me->GetEntry())
                {
                    case NPC_CURSED_MOGU_SCULPTURE_1:
                        me->AddAura(SPELL_POSE_1, me);
                        me->AddAura(SPELL_STONE, me);
                        break;
                    case NPC_CURSED_MOGU_SCULPTURE_2:
                        me->AddAura(SPELL_POSE_2, me);
                        me->AddAura(SPELL_BRONZE, me);
                        break;
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* attacker)
            {
                events.Reset();
                events.ScheduleEvent(EVENT_CURSED_MOGU_SPIRIT_BOLT, 15000);
                events.ScheduleEvent(EVENT_CURSED_MOGU_GROUND_SLAM, 25000);

                me->AI()->AttackStart(attacker);
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (!pInstance)
                    return;

                if (!who->IsWithinDist(me, 15.0f))
                    return;

                if (who->GetTypeId() == TYPEID_PLAYER)
                {
                    switch (me->GetEntry())
                    {
                        case NPC_CURSED_MOGU_SCULPTURE_2:
                        {
                            if (activationDone)
                                return;

                            if (Creature* ghostEssence = pInstance->instance->GetCreature(pInstance->GetData64(NPC_GHOST_ESSENCE)))
                                ghostEssence->CastSpell(ghostEssence, SPELL_GHOST_ESSENCE, false);
                            break;
                        }
                        case NPC_CURSED_MOGU_SCULPTURE_1:
                        {
                            me->SetReactState(REACT_AGGRESSIVE);
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                            me->RemoveAurasDueToSpell(SPELL_BRONZE);
                            me->RemoveAurasDueToSpell(SPELL_POSE_2);
                            me->RemoveAurasDueToSpell(SPELL_POSE_1);
                            me->RemoveAurasDueToSpell(SPELL_STONE);
                            DoAction(ACTION_CURSED_MOGU_ATTACK_PLAYER);
                            break;
                        }
                    default:
                        break;
                    }

                    playerActivate = who->GetGUID();
                    activationDone = true;
                }
            }

            void JustDied(Unit* killer)
            {
                if (!GetClosestCreatureWithEntry(me, me->GetEntry(), 50.0f, true))
                {
                    Creature* Feng = GetClosestCreatureWithEntry(me, NPC_FENG, 100.0f, true);
                    if (Feng && Feng->AI())
                        Feng->AI()->Talk(7);
                }

                if (me->GetEntry() == NPC_CURSED_MOGU_SCULPTURE_1)
                {
                    if (Creature* lorewalkerCho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 100.0f, true))
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

            void DoAction(const int32 action)
            {
                if (action == ACTION_CURSED_MOGU_ATTACK_PLAYER)
                    if (Player* plr = ObjectAccessor::FindPlayer(playerActivate))
                        me->AI()->AttackStart(plr);
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 id = events.ExecuteEvent())
                {
                    switch (id)
                    {
                        case EVENT_CURSED_MOGU_SPIRIT_BOLT:
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                if (!target->IsFriendlyTo(me))
                                    me->CastSpell(target, SPELL_SPIRIT_BOLT, false);

                            events.ScheduleEvent(EVENT_CURSED_MOGU_SPIRIT_BOLT, 15000);
                            break;
                        case EVENT_CURSED_MOGU_GROUND_SLAM:
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                if (!target->IsFriendlyTo(me))
                                    me->CastSpell(target, SPELL_GROUND_SLAM, false);

                            events.ScheduleEvent(EVENT_CURSED_MOGU_GROUND_SLAM, 25000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_cursed_mogu_sculptureAI(creature);
        }
};

// Ghost Essence - 120764
class spell_ghost_essence : public SpellScriptLoader
{
    public:
        spell_ghost_essence() : SpellScriptLoader("spell_ghost_essence") { }

        class spell_ghost_essence_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ghost_essence_SpellScript);

            void HandleOnHit()
            {
                if (Unit* target = GetHitUnit())
                {
                    target->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                    target->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE|UNIT_FLAG_NOT_SELECTABLE);
                    target->RemoveAurasDueToSpell(SPELL_BRONZE);
                    target->RemoveAurasDueToSpell(SPELL_POSE_2);
                    target->RemoveAurasDueToSpell(SPELL_POSE_1);
                    target->RemoveAurasDueToSpell(SPELL_STONE);
                    target->GetAI()->DoAction(ACTION_CURSED_MOGU_ATTACK_PLAYER);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_ghost_essence_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ghost_essence_SpellScript();
        }
};

float quilenNewY[2] = { 1170.0f, 1240.0f };

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
                events.ScheduleEvent(EVENT_ENORMOUS_QUILEN_BITE, urand (3000, 5000));
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
                        case EVENT_ENORMOUS_QUILEN_BITE:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                            {
                                if (!target->IsFriendlyTo(me))
                                    me->CastSpell(target, SPELL_MONSTROUS_BITE, true);
                            }
                            events.ScheduleEvent(EVENT_ENORMOUS_QUILEN_BITE, urand(6000, 8000));
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

                events.ScheduleEvent(EVENT_QUILEN_SUNDERING_BITE,   urand (5000, 6000));
                events.ScheduleEvent(EVENT_QUILEN_SHATTERING_STONE, urand (10000, 12000));
                events.ScheduleEvent(EVENT_QUILEN_FOCUSED_ASSAULT,  urand (500, 5000));
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
                        case EVENT_QUILEN_SUNDERING_BITE:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->CastSpell(target, SPELL_SUNDERING_BITE, true);

                            events.ScheduleEvent(EVENT_QUILEN_SUNDERING_BITE,   urand (5000, 6000));
                            break;
                        }
                        case EVENT_QUILEN_SHATTERING_STONE:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->CastSpell(target, SPELL_SHATTERING_STONE, true);

                            events.ScheduleEvent(EVENT_QUILEN_SHATTERING_STONE, urand (10000, 12000));
                            break;
                        }
                        case EVENT_QUILEN_FOCUSED_ASSAULT:
                        {
                            if (Unit* target = me->SelectNearestTarget(5.0f))
                                me->AddAura(SPELL_FOCUSED_ASSAULT, target);

                            events.ScheduleEvent(EVENT_QUILEN_FOCUSED_ASSAULT,  urand (500, 5000));
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

                events.ScheduleEvent(EVENT_ZANDALARI_TROLL_RUSH, urand (5000, 6000));
            }

            void EnterCombat(Unit* attacker)
            {
                if (Creature* lorewalkerCho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 150.0f, true))
                    if (lorewalkerCho->AI())
                        lorewalkerCho->AI()->DoAction(ACTION_SAY_ZANDALARI_BEGIN);
            }

            void JustDied(Unit* killer)
            {
                if (Creature* lorewalkerCho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 150.0f, true))
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
                        case EVENT_ZANDALARI_TROLL_RUSH:
                        {
                            if (Unit* target = SelectTarget(SELECT_TARGET_FARTHEST))
                            {
                                me->CastSpell(target, SPELL_TROLL_RUSH, true);
                                me->GetMotionMaster()->MoveChase(target);
                            }

                            events.ScheduleEvent(EVENT_ZANDALARI_TROLL_RUSH,   urand (5000, 6000));
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

#define GOSSIP_CHOICE_1 "What are you waiting for ? Let's go investigate !"
#define GOSSIP_CHOICE_2 "Go ahead and turn this thing on."

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
    SPIRIT_KINGS_01    = 13
};

enum eChoEvents
{
    EVENT_START_FIRST_COMBAT = 1,
    EVENT_TALK_1             = 2,
    EVENT_TALK_2             = 3,
    EVENT_TALK_3             = 4,
    EVENT_TALK_4             = 5,
    EVENT_TALK_5             = 6,
    EVENT_TALK_6             = 7,
    EVENT_TALK_7             = 8,
    EVENT_TALK_8             = 9,
    EVENT_TALK_9             = 10,
    EVENT_TALK_10            = 11,
    EVENT_TALK_11            = 12,
    EVENT_TALK_12            = 13
};

class npc_lorewalker_cho : public CreatureScript
{
    public:
        npc_lorewalker_cho() : CreatureScript("npc_lorewalker_cho") { }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            if (creature->GetPositionX() >= 4292.0f && creature->GetPositionX() <= 4294.0f && creature->GetPositionY() >= 1533.0f && creature->GetPositionY() <= 1533.1f)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_1, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 1);
                player->SEND_GOSSIP_MENU(30003, creature->GetGUID());
            }

            if (creature->GetPositionX() >= 4022.0f && creature->GetPositionX() <= 4023.0f && creature->GetPositionY() >= 1859.0f && creature->GetPositionY() <= 1860.0f)
            {
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_CHOICE_2, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF + 2);
                player->SEND_GOSSIP_MENU(30008, creature->GetGUID());
            }

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

            if (action == GOSSIP_ACTION_INFO_DEF + 2)
            {
                player->CLOSE_GOSSIP_MENU();
                creature->AI()->DoAction(ACTION_ELEGON_GOB_ACTIVATION);
            }

            return true;
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
            bool hasSaidSecretsKeeperCombat;
            bool hasSaidKeeperDied;
            InstanceScript* pInstance;
            EventMap events;

            void Reset()
            {
                events.Reset();
                SetDespawnAtFar(false);
                SetDespawnAtEnd(false);
                hasSaidIntro = false;
                hasSaidStoneguardDeath = false;
                hasSaidZandalariBegin = false;
                hasSaidZandalariDeath = false;
                hasSaidSecretsKeeperCombat = false;
                hasSaidKeeperDied = false;
            }

            void MoveInLineOfSight(Unit* who)
            {
                if (hasSaidIntro || who->GetTypeId() != TYPEID_PLAYER)
                    return;

                if (me->IsWithinDistInMap(who, 50.0f))
                {
                    Talk(INTRO_01);
                    hasSaidIntro = true;
                    Start(false, true, who->GetGUID());
                }
            }

            void WaypointReached(uint32 waypointId)
            {
                Player* player = GetPlayerForEscort();
                if (!player)
                    return;

                switch (waypointId)
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
                    {
                        me->SetOrientation(1.51f);
                        me->SetFacingTo(1.51f);
                        SetEscortPaused(true);
                        Talk(SPIRIT_KINGS_01);

                        std::list<Creature*> sorcererMoguList;
                        std::list<Creature*> mountedMoguList;
                        std::list<Creature*> moguArcherList;
                        std::list<Creature*> kingsGuardList;

                        me->GetCreatureListWithEntryInGrid(sorcererMoguList, NPC_SORCERER_MOGU, 100.0f);
                        me->GetCreatureListWithEntryInGrid(mountedMoguList, NPC_MOUNTED_MOGU, 200.0f);
                        me->GetCreatureListWithEntryInGrid(moguArcherList, NPC_MOGU_ARCHER, 100.0f);
                        me->GetCreatureListWithEntryInGrid(kingsGuardList, NPC_KINGSGUARD, 200.0f);

                        for (auto itr : sorcererMoguList)
                        {
                            if (Creature* sorcererMogu = itr)
                            {
                                sorcererMogu->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                                sorcererMogu->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                                sorcererMogu->AI()->DoAction(ACTION_BEFORE_COMBAT);
                                events.ScheduleEvent(EVENT_START_FIRST_COMBAT,	4000);
                            }
                        }

                        for (auto itr : mountedMoguList)
                        {
                            if (Creature* mountedMogu = itr)
                            {
                                mountedMogu->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                                mountedMogu->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                            }
                        }

                        for (auto itr : moguArcherList)
                        {
                            if (Creature* moguArcher = itr)
                            {
                                moguArcher->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                                moguArcher->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                            }
                        }

                        for (auto itr : kingsGuardList)
                        {
                            if (Creature* kingsGuard = itr)
                            {
                                kingsGuard->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                                kingsGuard->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                            }
                        }

                        if (Creature* zian = GetClosestCreatureWithEntry(me, MOB_ZIAN, 200.0f))
                        {
                            if (zian->AI())
                            {
                                zian->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                                zian->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                                zian->AI()->DoAction(ACTION_BEFORE_COMBAT);
                                events.ScheduleEvent(EVENT_START_FIRST_COMBAT,	5000);
                            }
                        }

                        if (Creature* meng = GetClosestCreatureWithEntry(me, MOB_MENG, 200.0f))
                        {
                            if (meng->AI())
                            {
                                meng->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                                meng->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                            }
                        }

                        if (Creature* qiang = GetClosestCreatureWithEntry(me, MOB_QIANG, 200.0f))
                        {
                            if (qiang->AI())
                            {
                                qiang->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                                qiang->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                            }
                        }

                        if (Creature* subetai = GetClosestCreatureWithEntry(me, MOB_SUBETAI, 200.0f))
                        {
                            if (subetai->AI())
                            {
                                subetai->AI()->DoAction(ACTION_SET_GHOST_VISUAL);
                                subetai->AI()->DoAction(ACTION_SET_NATIVE_DISPLAYID);
                            }
                        }

                        break;
                    }
                    case 43:
                        Talk(18);
                        break;
                    case 48:
                        Talk(19);
                        break;
                    case 53:
                        SetEscortPaused(true);
                        Talk(20);
                        break;
                    case 60:
                        if (GameObject* door = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_ELEGON_DOOR_ENTRANCE)))
                            if (door->GetGoState() == GO_STATE_READY)
                                door->SetGoState(GO_STATE_ACTIVE);
                        break;
                    case 62:
                        SetEscortPaused(true);
                        break;
                    case 67:
                        SetEscortPaused(true);
                        Talk(23);
                        break;
                    case 69:
                        Talk(24);
                        break;
                    case 70:
                        me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        SetEscortPaused(true);
                        break;
                    case 71:
                        me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                        SetEscortPaused(true);
                        break;
                    case 72:
                        events.ScheduleEvent(EVENT_TALK_1, 1000);
                        break;
                    case 79:
                    case 83:
                        if (GameObject* door = pInstance->instance->GetGameObject(pInstance->GetData64(waypointId == 79 ? GOB_ELEGON_DOOR_ENTRANCE : GOB_WILL_OF_EMPEROR_ENTRANCE)))
                            if (door->GetGoState() == GO_STATE_READY)
                                door->SetGoState(GO_STATE_ACTIVE);
                        break;
                    /*case 83:
                        if (GameObject* door = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_WILL_OF_EMPEROR_ENTRANCE)))
                            if (door->GetGoState() == GO_STATE_READY)
                                door->SetGoState(GO_STATE_ACTIVE);
                                // door->Use(me);
                        break;*/
                    case 86:
                        SetEscortPaused(true);
                        break;
                    case 88:
                        events.ScheduleEvent(EVENT_TALK_6, 1000);
                        break;
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
                    case ACTION_KEEPER_ENTER_COMBAT:
                        if (!hasSaidSecretsKeeperCombat)
                        {
                            Talk(21);
                            hasSaidSecretsKeeperCombat = true;
                        }
                        break;
                    case ACTION_KEEPER_DIED:
                        if (!hasSaidKeeperDied)
                        {
                            Talk(22);
                            SetEscortPaused(false);
                            hasSaidKeeperDied = true;
                        }
                            break;
                    case ACTION_ELEGON_GOB_ACTIVATION:
                    {
                        if (!pInstance->CheckRequiredBosses(DATA_ELEGON))
                            break;

                        Talk(25);
                        if (GameObject* button = pInstance->instance->GetGameObject(pInstance->GetData64(GOB_CELESTIAL_COMMAND)))
                            if (button->GetGoState() == GO_STATE_READY)
                                button->Use(me);
                        break;
                    }
                    case ACTION_TALK_WILL_OF_EMPEROR:
                        Talk(39);
                        break;
                    case ACTION_EMPERORS_DEATH:
                        events.ScheduleEvent(EVENT_TALK_11, 1000);
                        break;
                    default:
                        break;
                }
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

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_START_FIRST_COMBAT:
                        {
                            if (Creature* zian = GetClosestCreatureWithEntry(me, MOB_ZIAN, 200.0f))
                                if (zian->AI())
                                    zian->AI()->DoAction(ACTION_START_FIRST_COMBAT);

                            std::list<Creature*> sorcererMoguList;
                            me->GetCreatureListWithEntryInGrid(sorcererMoguList, NPC_SORCERER_MOGU, 200.0f);

                            for (auto itr : sorcererMoguList)
                                if (Creature* sorcererMogu = itr)
                                    sorcererMogu->AI()->DoAction(ACTION_START_FIRST_COMBAT);

                            break;
                        }
                        case EVENT_TALK_1:
                            Talk(29);
                            events.ScheduleEvent(EVENT_TALK_2, 4000);
                            break;
                        case EVENT_TALK_2:
                            Talk(30);
                            events.ScheduleEvent(EVENT_TALK_3, 5000);
                            break;
                        case EVENT_TALK_3:
                            Talk(31);
                            events.ScheduleEvent(EVENT_TALK_4, 4000);
                            break;
                        case EVENT_TALK_4:
                            Talk(32);
                            events.ScheduleEvent(EVENT_TALK_5, 7000);
                            break;
                        case EVENT_TALK_5:
                            Talk(33);
                            break;
                        case EVENT_TALK_6:
                            Talk(34);
                            events.ScheduleEvent(EVENT_TALK_7, 10000);
                            break;
                        case EVENT_TALK_7:
                            Talk(35);
                            events.ScheduleEvent(EVENT_TALK_8, 10000);
                            break;
                        case EVENT_TALK_8:
                            Talk(36);
                            events.ScheduleEvent(EVENT_TALK_9, 10000);
                            break;
                        case EVENT_TALK_9:
                            Talk(37);
                            events.ScheduleEvent(EVENT_TALK_10, 10000);
                            break;
                        case EVENT_TALK_10:
                            Talk(38);
                            break;
                        case EVENT_TALK_11:
                            Talk(40);
                            events.ScheduleEvent(EVENT_TALK_12, 10000);
                            break;
                        case EVENT_TALK_12:
                            Talk(41);
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_lorewalker_choAI(creature);
        }
};

class mob_zian : public CreatureScript
{
    public:
        mob_zian() : CreatureScript("mob_zian") { }

        struct mob_zianAI : public ScriptedAI
        {
            mob_zianAI(Creature* creature) : ScriptedAI(creature)
            {
                pInstance = creature->GetInstanceScript();
            }

            EventMap events;
            InstanceScript* pInstance;

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_ZIAN_CHARGED_SHADOWS, urand(5000, 15000));
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(41566);
                        SetEquipmentSlots(false, EQUIP_ZIAN_STAFF, 0, EQUIP_NO_CHANGE);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_FIRST_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        break;
                    case ACTION_END_FIRST_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_ZIAN_CHARGED_SHADOWS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_CHARGED_SHADOWS, false);
                            events.ScheduleEvent(EVENT_ZIAN_CHARGED_SHADOWS, urand(5000, 12000));
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_zianAI(creature);
        }
};

class mob_sorcerer_mogu : public CreatureScript
{
    public:
        mob_sorcerer_mogu() : CreatureScript("mob_sorcerer_mogu") { }

        struct mob_sorcerer_moguAI : public ScriptedAI
        {
            mob_sorcerer_moguAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;
            bool allKilled;

            void Reset()
            {
                allKilled = false;

                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_SORCERER_SHADOW_BLAST, 5000);
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(40138);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_FIRST_COMBAT:
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        break;
                    case ACTION_END_FIRST_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                events.Update(diff);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SORCERER_SHADOW_BLAST:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHADOW_BLAST, false);
                            events.ScheduleEvent(EVENT_SORCERER_SHADOW_BLAST, urand(8000, 17000));
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_sorcerer_moguAI(creature);
        }
};

class mob_qiang : public CreatureScript
{
    public:
        mob_qiang() : CreatureScript("mob_qiang") { }

        struct mob_qiangAI : public ScriptedAI
        {
            mob_qiangAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_QIANG_ANNIHILATE, urand(5000, 15000));
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(41812);
                        SetEquipmentSlots(false, EQUIP_QIANG_POLEARM, 0, EQUIP_NO_CHANGE);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_SECOND_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        break;
                    case ACTION_END_SECOND_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);

                std::list<Creature*> sorcererMoguList;
                me->GetCreatureListWithEntryInGrid(sorcererMoguList, NPC_SORCERER_MOGU, 200.0f);

                std::list<Creature*> mountedMoguList;
                me->GetCreatureListWithEntryInGrid(mountedMoguList, NPC_MOUNTED_MOGU, 200.0f);

                for (auto sorcererMogu : sorcererMoguList)
                {
                    if (sorcererMogu->isAlive() && sorcererMogu != me)
                        return;
                    else
                    {
                        sorcererMogu->AI()->DoAction(ACTION_END_FIRST_COMBAT);

                        if (Creature* zian = GetClosestCreatureWithEntry(me, MOB_ZIAN, 200.0f))
                            if (zian->AI())
                                zian->AI()->DoAction(ACTION_END_FIRST_COMBAT);
                    }
                }

                if (Creature* qiang = GetClosestCreatureWithEntry(me, MOB_QIANG, 200.0f))
                    if (qiang->AI())
                        qiang->AI()->DoAction(ACTION_BEFORE_COMBAT);

                for (auto mountedMogu : mountedMoguList)
                    mountedMogu->AI()->DoAction(ACTION_BEFORE_COMBAT);
                    
                events.ScheduleEvent(EVENT_QIANG_START_SECOND_FIGHT, 5000);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_QIANG_ANNIHILATE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_ANNIHILATE, false);
                            events.ScheduleEvent(EVENT_QIANG_ANNIHILATE,       urand(10000, 20000));
                            break;
                        case EVENT_QIANG_START_SECOND_FIGHT:
                            if (Creature* qiang = GetClosestCreatureWithEntry(me, MOB_QIANG, 200.0f))
                                if (qiang->AI())
                                    qiang->AI()->DoAction(ACTION_START_SECOND_COMBAT);
                            for (auto itr : mountedMoguList)
                                itr->AI()->DoAction(ACTION_START_SECOND_COMBAT);
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_qiangAI(creature);
        }
};

class mob_mounted_mogu : public CreatureScript
{
    public:
        mob_mounted_mogu() : CreatureScript("mob_mounted_mogu") { }

        struct mob_mounted_moguAI : public ScriptedAI
        {
            mob_mounted_moguAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_MOUNTED_MOGU_CRUSHING_ATTACKS, urand(5000, 12000));
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(42555);
                        me->SetUInt32Value(UNIT_FIELD_MOUNTDISPLAYID, 41441);
                        SetEquipmentSlots(false, EQUIP_MOUNTED_MOGU_WEAPON, 0, EQUIP_NO_CHANGE);
                        break;
                   case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                   case ACTION_START_SECOND_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                   case ACTION_END_SECOND_COMBAT:
                       me->DespawnOrUnsummon();
                       break;
                    default:
                        break;
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
                        case EVENT_MOUNTED_MOGU_CRUSHING_ATTACKS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_CRUSHING_ATTACKS, false);
                            events.ScheduleEvent(EVENT_MOUNTED_MOGU_CRUSHING_ATTACKS, 25000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mounted_moguAI(creature);
        }
};

class mob_subetai : public CreatureScript
{
    public:
        mob_subetai() : CreatureScript("mob_subetai") { }

        struct mob_subetaiAI : public ScriptedAI
        {
            mob_subetaiAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_SUBETAI_VOLLEY, urand(5000, 15000));
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(41565);
                        SetEquipmentSlots(false, EQUIP_SUBETAI_SWORD, EQUIP_SUBETAI_SWORD, EQUIP_SUBETAI_BOW);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_THIRD_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        break;
                    case ACTION_END_THIRD_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
                
                std::list<Creature*> mountedMoguList;
                me->GetCreatureListWithEntryInGrid(mountedMoguList, NPC_MOUNTED_MOGU, 200.0f);
                
                std::list<Creature*> moguArcherList;
                me->GetCreatureListWithEntryInGrid(moguArcherList, NPC_MOGU_ARCHER, 100.0f);

                for (auto mountedMogu : mountedMoguList)
                {
                    if (mountedMogu->isAlive() && mountedMogu != me)
                        return;
                    else
                    {
                        mountedMogu->AI()->DoAction(ACTION_END_SECOND_COMBAT);

                        if (Creature* qiang = GetClosestCreatureWithEntry(me, MOB_QIANG, 200.0f))
                            if (qiang->AI())
                                qiang->AI()->DoAction(ACTION_END_SECOND_COMBAT);
                    }
                }

                if (Creature* subetai = GetClosestCreatureWithEntry(me, MOB_SUBETAI, 200.0f))
                    if (subetai->AI())
                        subetai->AI()->DoAction(ACTION_BEFORE_COMBAT);

                for (auto moguArcher : moguArcherList)
                    moguArcher->AI()->DoAction(ACTION_BEFORE_COMBAT);
                    
                events.ScheduleEvent(EVENT_SUBETAI_START_THIRD_COMBAT, 5000);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_SUBETAI_VOLLEY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM))
                                me->CastSpell(target, SPELL_VOLLEY, false);
                            events.ScheduleEvent(EVENT_SUBETAI_VOLLEY,       urand(10000, 20000));
                            break;
                        case EVENT_SUBETAI_START_THIRD_COMBAT:
                            if (Creature* subetai = GetClosestCreatureWithEntry(me, MOB_SUBETAI, 200.0f))
                                if (subetai->AI())
                                    subetai->AI()->DoAction(ACTION_START_THIRD_COMBAT);
                            for (auto itr : moguArcherList)
                                itr->AI()->DoAction(ACTION_START_THIRD_COMBAT);
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_subetaiAI(creature);
        }
};

class mob_mogu_archer : public CreatureScript
{
    public:
        mob_mogu_archer() : CreatureScript("mob_mogu_archer") { }

        struct mob_mogu_archerAI : public ScriptedAI
        {
            mob_mogu_archerAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_MOGU_ARCHER_SHOOT, urand(5000, 12000));
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(41498);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_THIRD_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    case ACTION_END_THIRD_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
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
                        case EVENT_MOGU_ARCHER_SHOOT:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_SHOOT, false);
                            events.ScheduleEvent(EVENT_MOGU_ARCHER_SHOOT,       25000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mogu_archerAI(creature);
        }
};

class mob_meng : public CreatureScript
{
    public:
        mob_meng() : CreatureScript("mob_meng") { }

        struct mob_mengAI : public ScriptedAI
        {
            mob_mengAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(41813);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_FOURTH_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        break;
                    case ACTION_END_FOURTH_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                events.Update(diff);
                
                std::list<Creature*> kingsGuardList;
                me->GetCreatureListWithEntryInGrid(kingsGuardList, NPC_KINGSGUARD, 200.0f);
                
                std::list<Creature*> moguArcherList;
                me->GetCreatureListWithEntryInGrid(moguArcherList, NPC_MOGU_ARCHER, 100.0f);

                for (auto moguArcher : moguArcherList)
                {
                    if (moguArcher->isAlive() && moguArcher != me)
                        return;
                    else
                    {
                        moguArcher->AI()->DoAction(ACTION_END_THIRD_COMBAT);

                        if (Creature* subetai = GetClosestCreatureWithEntry(me, MOB_SUBETAI, 200.0f))
                            if (subetai->AI())
                                subetai->AI()->DoAction(ACTION_END_THIRD_COMBAT);
                    }
                }

                if (Creature* meng = GetClosestCreatureWithEntry(me, MOB_MENG, 200.0f))
                    if (meng->AI())
                        meng->AI()->DoAction(ACTION_BEFORE_COMBAT);

                for (auto kingsGuard : kingsGuardList)
                    kingsGuard->AI()->DoAction(ACTION_BEFORE_COMBAT);
                    
                events.ScheduleEvent(EVENT_MENG_START_FOURTH_COMBAT, 5000);

                while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        case EVENT_MENG_START_FOURTH_COMBAT:
                            if (Creature* meng = GetClosestCreatureWithEntry(me, MOB_MENG, 200.0f))
                                if (meng->AI())
                                    meng->AI()->DoAction(ACTION_START_FOURTH_COMBAT);
                            for (auto itr : kingsGuardList)
                                itr->AI()->DoAction(ACTION_START_FOURTH_COMBAT);
                            break;
                        case EVENT_MENG_COWARDICE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_COWARDICE, false);
                            events.ScheduleEvent(EVENT_MENG_COWARDICE,       25000);
                            break;
                        default:
                            break;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mengAI(creature);
        }
};

class mob_kingsguard : public CreatureScript
{
    public:
        mob_kingsguard() : CreatureScript("mob_kingsguard") { }

        struct mob_kingsguardAI : public ScriptedAI
        {
            mob_kingsguardAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void JustDied(Unit* killer)
            {
                DoAction(ACTION_END_FOURTH_COMBAT);
                if (Creature* meng = GetClosestCreatureWithEntry(me, MOB_MENG, 200.0f))
                    if (meng->AI())
                        meng->AI()->DoAction(ACTION_END_FOURTH_COMBAT);
            }

            void Reset()
            {
                me->SetDisplayId(11686);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                events.Reset();
                events.ScheduleEvent(EVENT_KINGS_GUARD_ENRAGE, urand(5000, 12000));
                events.ScheduleEvent(EVENT_KINGS_GUARD_REFLECTIVE_SHIELDS, urand(15000, 20000));
            }

            void DoAction(const int32 action)
            {
                switch (action)
                {
                    case ACTION_SET_GHOST_VISUAL:
                        me->AddAura(SPELL_INACTIVE, me);
                        break;
                    case ACTION_SET_NATIVE_DISPLAYID:
                        me->SetDisplayId(41498);
                        break;
                    case ACTION_BEFORE_COMBAT:
                        me->AddAura(SPELL_ACTIVATION_VISUAL, me);
                        break;
                    case ACTION_START_FOURTH_COMBAT:
                        me->RemoveAurasDueToSpell(SPELL_ACTIVATION_VISUAL);
                        me->setFaction(14);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        break;
                    case ACTION_END_FOURTH_COMBAT:
                        me->DespawnOrUnsummon();
                        break;
                    default:
                        break;
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
                        case EVENT_KINGS_GUARD_ENRAGE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ENRAGE, false);
                            events.ScheduleEvent(EVENT_KINGS_GUARD_ENRAGE, 25000);
                            break;
                        case EVENT_KINGS_GUARD_REFLECTIVE_SHIELDS:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_REFLECTIVE_SHIELDS, false);
                            events.ScheduleEvent(EVENT_KINGS_GUARD_REFLECTIVE_SHIELDS, 40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_kingsguardAI(creature);
        }
};

class mob_mogu_secret_keeper : public CreatureScript
{
    public:
        mob_mogu_secret_keeper() : CreatureScript("mob_mogu_secret_keeper") { }

        struct mob_mogu_secret_keeperAI : public ScriptedAI
        {
            mob_mogu_secret_keeperAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_SECRET_FLESH_TO_STONE, urand(5000, 12000));
                events.ScheduleEvent(EVENT_SECRET_STONE_BLOCK, urand(15000, 20000));
            }
            
            void JustDied(Unit* killer)
            {
                if (Creature* cho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 60.0f, true))
                {
                    if (cho->GetPositionX() >= 3985.0f && cho->GetPositionX() <= 3986.0f && cho->GetPositionY() >= 1550.0f && cho->GetPositionY() <= 1551.0f)
                        cho->AI()->DoAction(ACTION_CONTINUE_ESCORT);
                    else
                        cho->AI()->DoAction(ACTION_KEEPER_DIED);
                }
            }

            void EnterCombat(Unit* attacker)
            {
                if (Creature* cho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 60.0f, true))
                    cho->AI()->DoAction(ACTION_KEEPER_ENTER_COMBAT);
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
                        case EVENT_SECRET_FLESH_TO_STONE:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FLESH_TO_STONE, false);
                            events.ScheduleEvent(EVENT_SECRET_FLESH_TO_STONE,       25000);
                            break;
                        case EVENT_SECRET_STONE_BLOCK:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_STONE_BLOCK, false);
                            events.ScheduleEvent(EVENT_SECRET_STONE_BLOCK,       40000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mogu_secret_keeperAI(creature);
        }
};

class mob_mogu_warden : public CreatureScript
{
    public:
        mob_mogu_warden() : CreatureScript("mob_mogu_warden") { }

        struct mob_mogu_wardenAI : public ScriptedAI
        {
            mob_mogu_wardenAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_WARDEN_FORCEFUL_SWING, urand(5000, 12000));
                events.ScheduleEvent(EVENT_WARDEN_TOUCH_OF_NALAK_SHA, urand(15000, 20000));
                events.ScheduleEvent(EVENT_WARDEN_WARDEN_S_FURY, urand(22000, 28000));
            }
            
            void JustDied(Unit* killer)
            {
                if (me->GetEntry() == NPC_MOGUSHAN_WARDEN)
                    if (Creature* cho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 60.0f, true))
                        cho->AI()->DoAction(ACTION_CONTINUE_ESCORT);
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
                        case EVENT_WARDEN_FORCEFUL_SWING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_FORCEFUL_SWING, false);
                            events.ScheduleEvent(EVENT_WARDEN_FORCEFUL_SWING,       30000);
                            break;
                        case EVENT_WARDEN_TOUCH_OF_NALAK_SHA:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_TOUCH_OF_NALAK_SHA, false);
                            events.ScheduleEvent(EVENT_WARDEN_TOUCH_OF_NALAK_SHA,       30000);
                            break;
                        case EVENT_WARDEN_WARDEN_S_FURY:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_WARDEN_S_FURY, false);
                            events.ScheduleEvent(EVENT_WARDEN_WARDEN_S_FURY,       30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mogu_wardenAI(creature);
        }
};

class mob_mogu_engine_keeper : public CreatureScript
{
    public:
        mob_mogu_engine_keeper() : CreatureScript("mob_mogu_engine_keeper") { }

        struct mob_mogu_engine_keeperAI : public ScriptedAI
        {
            mob_mogu_engine_keeperAI(Creature* creature) : ScriptedAI(creature)
            {
            }

            EventMap events;

            void Reset()
            {
                events.Reset();
                events.ScheduleEvent(EVENT_KEEPER_ENHANCED_RECONSTRUCTION, urand(5000, 12000));
                events.ScheduleEvent(EVENT_KEEPER_RECONSTRUCTING, urand(15000, 20000));
            }
            
            void JustDied(Unit* killer)
            {
                if (Creature* cho = GetClosestCreatureWithEntry(me, NPC_LOREWALKER_CHO, 120.0f, true))
                    cho->AI()->DoAction(ACTION_CONTINUE_ESCORT);
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
                        case EVENT_KEEPER_ENHANCED_RECONSTRUCTION:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_ENHANCED_RECONSTRUCTION, false);
                            events.ScheduleEvent(EVENT_KEEPER_ENHANCED_RECONSTRUCTION,       30000);
                            break;
                        case EVENT_KEEPER_RECONSTRUCTING:
                            if (Unit* target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                                me->CastSpell(target, SPELL_RECONSTRUCTING, false);
                            events.ScheduleEvent(EVENT_KEEPER_RECONSTRUCTING,       30000);
                            break;
                        default:
                            break;
                    }
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new mob_mogu_engine_keeperAI(creature);
        }
};

void AddSC_mogu_shan_vault()
{
    new mob_cursed_mogu_sculpture();
    new spell_ghost_essence();
    new mob_enormous_stone_quilen();
    new mob_stone_quilen();
    new mob_zandalari_skullcharger();
    new spell_mogu_petrification();
    new npc_lorewalker_cho();
    new mob_sorcerer_mogu();
    new mob_mogu_archer();
    new mob_mounted_mogu();
    new mob_kingsguard();
    new mob_subetai();
    new mob_meng();
    new mob_zian();
    new mob_qiang();
    new mob_mogu_secret_keeper();
    new mob_mogu_warden();
    new mob_mogu_engine_keeper();
}
