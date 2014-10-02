/*
 * Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com/>
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
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
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "throne_of_thunder.h"
#include "MoveSplineInit.h"

enum eSpells
{
    SPELL_HARD_STARE                = 133765,
    SPELL_FORCE_OF_WILL             = 136413, // May be too much powerful, to check with players
    SPELL_LINGERING_GAZE_MAIN       = 138467, // Script Effect
    SPELL_LINGERING_GAZE_MISSILE    = 133792,
    SPELL_LINGERING_GAZE_AT         = 133793,
    SPELL_LIFE_DRAIN_DUMMY          = 133795,
    SPELL_LIFE_DRAIN_AURA           = 133796,
    SPELL_LIFE_DRAIN_DMG            = 133798,

    // Second Phase
    SPELL_BRIGHT_LIGHT              = 134124, // Yellow link between caster/target
    SPELL_INFRARED_LIGHT            = 134123, // Red link between caster/target
    SPELL_BLUE_RAYS                 = 134122, // Blue link between caster/target
    SPELL_BRIGHT_LIGHT_MAIN         = 133740,
    SPELL_BLUE_RAYS_MAIN            = 133672,
    SPELL_INFRARED_LIGHT_MAIN       = 133734,
    SPELL_RED_FOG_INVISIBILITY      = 136116,
    SPELL_YELLOW_FOG_INVISIBILITY   = 136117,
    SPELL_BLUE_FOG_INVISIBILITY     = 136118,
};

enum eEvents
{
    EVENT_HARD_STARE                = 1,
    EVENT_FORCE_OF_WILL             = 2,
    EVENT_LINGERING_GAZE            = 3,
    EVENT_DRAIN_LIFE                = 4,
    EVENT_COLORBLIND_PHASE          = 5,  // Initialize second phase
    EVENT_RAY_SECOND_PHASE          = 6,  // Launch targets rays
    EVENT_SUMMON_RED_EYE            = 7,
    EVENT_SUMMON_BLUE_EYE           = 8,
    EVENT_SUMMON_YELLOW_EYE         = 9,
    EVENT_ADD_ROTATION              = 10, // Make the free ray rotating with the caster orientation
    EVENT_DISINTEGRATION_BEAM_PHASE = 11  // Initialize third phase
};

enum eActions
{
    ACTION_TURN_AROUND              = 1,
    ACTION_ADD_REVEALED             = 2
};

enum ePhases
{
    PHASE_NORMAL_PHASE              = 1,
    PHASE_COLORBLIND_PHASE          = 2,
    PHASE_DISINTEGRATION_BEAM_PHASE = 3
};

uint32 g_ColorEyeEntries[3] =
{
    { NPC_YELLOW_EYE },
    { NPC_RED_EYE },
    { NPC_BLUE_EYE }
};

uint32 g_ColorAddEntries[3] =
{
    { NPC_AMBER_FOG },
    { NPC_CRIMSON_FOG },
    { NPC_AZURE_FOG }
};

// Durumu the forgotten - 68036
class boss_durumu : public CreatureScript
{
    public:
        boss_durumu() : CreatureScript("boss_durumu") { }

        struct boss_durumuAI : public BossAI
        {
            boss_durumuAI(Creature* p_Creature) : BossAI(p_Creature, DATA_DURUMU_THE_FORGOTTEN)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            uint8 m_Phase;

            void Reset()
            {
                std::list<AreaTrigger*> l_AreatriggerList;
                m_Events.Reset();

                m_Phase = 0;

                _Reset();

                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                me->ClearUnitState(UNIT_STATE_ROOT);
                me->AddUnitState(UNIT_STATE_ROOT);

                summons.DespawnAll();

                me->GetAreaTriggerList(l_AreatriggerList, 133793);

                if (!l_AreatriggerList.empty())
                    for (AreaTrigger* l_AreaTrigger : l_AreatriggerList)
                        l_AreaTrigger->Remove();

                if (m_Instance)
                {
                    if (m_Instance->GetBossState(FAIL))
                        m_Instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, NOT_STARTED);
                }
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                m_Phase = PHASE_NORMAL_PHASE;

                m_Events.Reset();

                // First phase
/*                m_Events.ScheduleEvent(EVENT_HARD_STARE, 8000);
                m_Events.ScheduleEvent(EVENT_FORCE_OF_WILL, 13000);
                m_Events.ScheduleEvent(EVENT_LINGERING_GAZE, 19000);
                m_Events.ScheduleEvent(EVENT_DRAIN_LIFE, 25000);*/

                // Second Phase
                m_Events.ScheduleEvent(EVENT_COLORBLIND_PHASE, 12000);
                //m_Events.ScheduleEvent(EVENT_DISINTEGRATION_BEAM_PHASE, 30000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                summons.DespawnAll();

                std::list<AreaTrigger*> l_AreatriggerList;
                me->GetAreaTriggerList(l_AreatriggerList, 133793);

                if (!l_AreatriggerList.empty())
                    for (AreaTrigger* l_AreaTrigger : l_AreatriggerList)
                        l_AreaTrigger->Remove();

                if (m_Instance)
                {
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    m_Instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, DONE);
                }

                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootAssignation(me, l_Player->GetGroup());
                }
            }

            void DoAction(const int32 /*p_Action*/)
            {
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_HARD_STARE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_HARD_STARE, false);
                        m_Events.ScheduleEvent(EVENT_HARD_STARE, 12000);
                        break;
                    case EVENT_FORCE_OF_WILL:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_FORCE_OF_WILL, false);
                        m_Events.ScheduleEvent(EVENT_FORCE_OF_WILL, 6000);
                        break;
                    case EVENT_LINGERING_GAZE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_LINGERING_GAZE_MAIN, false);
                        m_Events.ScheduleEvent(EVENT_LINGERING_GAZE, 12000);
                        break;
                    case EVENT_DRAIN_LIFE:
                        me->SummonCreature(NPC_HUNGRY_EYE, me->GetPositionX() + 1.0f, me->GetPositionY() + 1.0f, me->GetPositionZ() + 1.0f);
                        break;
                    case EVENT_COLORBLIND_PHASE:
                    {
                        uint8 l_TargetCounter = 0;
                        uint8 l_SummonCounter = 0;
                        // float l_Angle = 0.0f;

                        m_Phase = PHASE_COLORBLIND_PHASE;

                        m_Events.ScheduleEvent(EVENT_SUMMON_RED_EYE, 500);
                        m_Events.ScheduleEvent(EVENT_SUMMON_BLUE_EYE, 900);
                        m_Events.ScheduleEvent(EVENT_SUMMON_YELLOW_EYE, 1300);

                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);

//                         for (uint8 i = 0; i < 3; i++)
//                         {
//                             float l_X = me->GetPositionX() + (frand(44.70f, 62.78f) * cos(l_Angle));
//                             float l_Y = me->GetPositionY() + (frand(44.70f, 62.78f) * sin(l_Angle));
//
//                            switch (i)
//                             {
//                                 case 0:
//                                     me->SummonCreature(NPC_AZURE_FOG, l_X, l_Y, me->GetPositionZ());
//                                         break;
//                                 case 1:
//                                     me->SummonCreature(NPC_AMBER_FOG, l_X, l_Y, me->GetPositionZ());
//                                         break;
//                                 case 2:
//                                     me->SummonCreature(NPC_CRIMSON_FOG, l_X, l_Y, me->GetPositionZ());
//                                         break;
//                                 default:
//                                     break;
//                             }
//
//                             l_Angle += (2 * M_PI) / 3 + frand(-20.0f, 20.0f);
//                         }

                        for (uint8 i = 0; i < 3; i++)
                        {
                            float l_Angle = frand(0, 2 * M_PI);
                            float l_X = me->GetPositionX() + (frand(44.70f, 62.78f) * cos(l_Angle));
                            float l_Y = me->GetPositionY() + (frand(44.70f, 62.78f) * sin(l_Angle));

                            switch (i)
                            {
                            case 0:
                                me->SummonCreature(NPC_AZURE_FOG, l_X, l_Y, me->GetPositionZ());
                                break;
                            case 1:
                                me->SummonCreature(NPC_AMBER_FOG, l_X, l_Y, me->GetPositionZ());
                                break;
                            case 2:
                                me->SummonCreature(NPC_CRIMSON_FOG, l_X, l_Y, me->GetPositionZ());
                                break;
                            default:
                                break;
                            }
                        }

                        //m_Events.ScheduleEvent(EVENT_COLORBLIND_PHASE, 180000);
                        break;
                    }
                    case EVENT_SUMMON_RED_EYE:
                    {
                        me->SummonCreature(NPC_RED_EYE, 5894.78f, 4513.95f, -0.4788f);
                        break;
                    }
                    case EVENT_SUMMON_BLUE_EYE:
                    {
                        me->SummonCreature(NPC_BLUE_EYE, 5894.78f, 4513.95f, -0.4788f);
                        break;
                    }
                    case EVENT_SUMMON_YELLOW_EYE:
                    {
                        me->SummonCreature(NPC_YELLOW_EYE, 5894.78f, 4513.95f, -0.4788f);
                        break;
                    }
                    case EVENT_DISINTEGRATION_BEAM_PHASE:
                    {
                        m_Phase = PHASE_DISINTEGRATION_BEAM_PHASE;

                        m_Events.ScheduleEvent(EVENT_DISINTEGRATION_BEAM_PHASE, 180000);
                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_durumuAI(p_Creature);
        }
};

// Hungry Eye - 67859
class mob_hungry_eye : public CreatureScript
{
    public:
        mob_hungry_eye() : CreatureScript("mob_hungry_eye") { }

        struct mob_hungry_eyeAI : public ScriptedAI
        {
            mob_hungry_eyeAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(EVENT_DRAIN_LIFE, 5000);
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (me->HasAura(SPELL_LIFE_DRAIN_DUMMY))
                {
                    std::list<Player*> l_PlayerList;

                    Player* l_AuraPlayer = nullptr;

                    GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        if (!l_Player->HasAura(SPELL_LIFE_DRAIN_AURA))
                            continue;
                        else
                            l_AuraPlayer = l_Player;
                    }

                    if (l_AuraPlayer)
                    {
                        for (Player* l_Player : l_PlayerList)
                        {
                            if (l_Player == l_AuraPlayer)
                                continue;

                            if (l_Player->IsInBetween(l_AuraPlayer, me, 3.0f))
                            {
                                me->AddAura(SPELL_LIFE_DRAIN_AURA, l_Player);
                                l_AuraPlayer->RemoveAura(SPELL_LIFE_DRAIN_AURA);
                                l_Player = l_AuraPlayer;
                            }
                        }
                    }
                }

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_DRAIN_LIFE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_LIFE_DRAIN_DUMMY, false);
                        m_Events.ScheduleEvent(EVENT_DRAIN_LIFE, 25000);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_hungry_eyeAI(p_Creature);
        }
};

// Color Eyes - 67854/67855/67856
class mob_second_phase_eye : public CreatureScript
{
    public:
        mob_second_phase_eye() : CreatureScript("mob_second_phase_eye") { }

        struct mob_second_phase_eyeAI : public ScriptedAI
        {
            mob_second_phase_eyeAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
                me->setFaction(35);
                me->SetReactState(REACT_PASSIVE);
                me->AddUnitState(UNIT_STATE_ROOT);
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(EVENT_RAY_SECOND_PHASE, 5000);
            }

            void MovementInform(uint32 type, uint32 id)
            {
                if (type != POINT_MOTION_TYPE)
                    return;

                if (id == 1)
                {
                    me->ClearUnitState(UNIT_STATE_ROOT);
                    me->AddUnitState(UNIT_STATE_ROOT);
                }
            }

            void DoAction(const int32 p_Action)
            {
                if (p_Action == ACTION_TURN_AROUND)
                {
                    m_Events.ScheduleEvent(EVENT_ADD_ROTATION, 150);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (me->GetEntry())
                {
                    case NPC_BLUE_EYE:
                    {
                        if (Creature* l_Add = GetClosestCreatureWithEntry(me, NPC_AZURE_FOG, 200.0f))
                            if (l_Add->HasAura(SPELL_YELLOW_FOG_INVISIBILITY) || l_Add->HasAura(SPELL_RED_FOG_INVISIBILITY) || l_Add->HasAura(SPELL_BLUE_FOG_INVISIBILITY))
                                if (l_Add->isInFront(me, 0.593412f))
                                    if (l_Add->GetAI())
                                        l_Add->AI()->DoAction(ACTION_ADD_REVEALED);
                        break;
                    }
                    case NPC_YELLOW_EYE:
                    {
                        if (Creature* l_Add = GetClosestCreatureWithEntry(me, NPC_AMBER_FOG, 200.0f))
                            if (l_Add->HasAura(SPELL_YELLOW_FOG_INVISIBILITY) || l_Add->HasAura(SPELL_RED_FOG_INVISIBILITY) || l_Add->HasAura(SPELL_BLUE_FOG_INVISIBILITY))
                                if (l_Add->isInFront(me, 0.593412f))
                                    if (l_Add->GetAI())
                                        l_Add->AI()->DoAction(ACTION_ADD_REVEALED);

                        break;
                    }
                    case NPC_RED_EYE:
                    {
                        if (Creature* l_Add = GetClosestCreatureWithEntry(me, NPC_CRIMSON_FOG, 200.0f))
                            if (l_Add->HasAura(SPELL_YELLOW_FOG_INVISIBILITY) || l_Add->HasAura(SPELL_RED_FOG_INVISIBILITY) || l_Add->HasAura(SPELL_BLUE_FOG_INVISIBILITY))
                                if (l_Add->isInFront(me, 0.593412f))
                                    if (l_Add->GetAI())
                                        l_Add->AI()->DoAction(ACTION_ADD_REVEALED);
                        break;
                    }
                    default:
                        break;
                }

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_RAY_SECOND_PHASE:
                    {
                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 200.0f);

                        switch (me->GetEntry())
                        {
                            case NPC_RED_EYE:
                            {
                                for (Player* l_Player : l_PlayerList)
                                {
                                    if (l_Player->GetRoleForGroup() != ROLE_TANK)
                                    {
                                        if (!l_Player->HasAura(SPELL_INFRARED_LIGHT) && !l_Player->HasAura(SPELL_BRIGHT_LIGHT) && !l_Player->HasAura(SPELL_BLUE_RAYS))
                                        {
                                            me->CastSpell(l_Player, SPELL_INFRARED_LIGHT, false);
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                            case NPC_YELLOW_EYE:
                            {
                                for (Player* l_Player : l_PlayerList)
                                {
                                    if (l_Player->GetRoleForGroup() != ROLE_TANK)
                                    {
                                        if (!l_Player->HasAura(SPELL_INFRARED_LIGHT) && !l_Player->HasAura(SPELL_BRIGHT_LIGHT) && !l_Player->HasAura(SPELL_BLUE_RAYS))
                                        {
                                            me->CastSpell(l_Player, SPELL_BRIGHT_LIGHT, false);
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                            case NPC_BLUE_EYE:
                            {
                                for (Player* l_Player : l_PlayerList)
                                {
                                    if (l_Player->GetRoleForGroup() != ROLE_TANK)
                                    {
                                        if (!l_Player->HasAura(SPELL_INFRARED_LIGHT) && !l_Player->HasAura(SPELL_BRIGHT_LIGHT) && !l_Player->HasAura(SPELL_BLUE_RAYS))
                                        {
                                            me->CastSpell(l_Player, SPELL_BLUE_RAYS, false);
                                            break;
                                        }
                                    }
                                }
                                break;
                            }
                                default:
                                    break;
                        }

                        Position l_Pos;

                        if (Creature* l_Durumu = GetClosestCreatureWithEntry(me, NPC_DURUMU_THE_FORGOTTEN, 150.0f))
                        {
                            l_Durumu->GetPosition(&l_Pos);
                            me->GetMotionMaster()->MovePoint(1, l_Pos);
                        }

                        break;
                    }
                    case EVENT_ADD_ROTATION:
                    {
                        me->SetFacingTo(me->GetOrientation() + 0.04f);
                        me->SetOrientation(me->GetOrientation() + 0.04f);
                        m_Events.ScheduleEvent(EVENT_ADD_ROTATION, 150);
                    }
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_second_phase_eyeAI(p_Creature);
        }
};

// Color adds - 69050/69051/69052
class mob_durumu_second_phase_add : public CreatureScript
{
    public:
        mob_durumu_second_phase_add() : CreatureScript("mob_durumu_second_phase_add") { }

        struct mob_durumu_second_phase_addAI : public ScriptedAI
        {
            mob_durumu_second_phase_addAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
            }

            EventMap m_Events;

            void Reset()
            {
                me->SetDisplayId(11686);

                switch (me->GetEntry())
                {
                    case NPC_CRIMSON_FOG:
                        me->CastSpell(me, SPELL_RED_FOG_INVISIBILITY, false);
                        break;
                    case NPC_AMBER_FOG:
                        me->CastSpell(me, SPELL_YELLOW_FOG_INVISIBILITY, false);
                        break;
                    case NPC_AZURE_FOG:
                        me->CastSpell(me, SPELL_BLUE_FOG_INVISIBILITY, false);
                        break;
                    default:
                        break;
                }
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_Events.Reset();
                me->AddUnitState(UNIT_STATE_ROOT);
            }

            void DoAction(const int32 p_Action)
            {
                if (p_Action == ACTION_ADD_REVEALED)
                {
                    switch (me->GetEntry())
                    {
                        case NPC_CRIMSON_FOG:
                            me->SetDisplayId(47140);
                            me->RemoveAurasDueToSpell(SPELL_RED_FOG_INVISIBILITY);
                            break;
                        case NPC_AMBER_FOG:
                            me->SetDisplayId(47141);
                            me->RemoveAurasDueToSpell(SPELL_YELLOW_FOG_INVISIBILITY);
                            break;
                        case NPC_AZURE_FOG:
                            me->SetDisplayId(47142);
                            me->RemoveAurasDueToSpell(SPELL_BLUE_FOG_INVISIBILITY);
                            break;
                        default:
                            break;
                    }

                    me->ClearUnitState(UNIT_STATE_ROOT);
                    me->setFaction(14);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISABLE_MOVE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                    if (Player* l_Player = me->SelectNearestPlayer(200.0f))
                        me->Attack(l_Player, true);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_durumu_second_phase_addAI(p_Creature);
        }
};

// Arterial Cut (aura) - 133768
class spell_arterial_cut : public SpellScriptLoader
{
    public:
        spell_arterial_cut() : SpellScriptLoader("spell_arterial_cut") { }

        class spell_arterial_cut_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arterial_cut_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                if (Player* l_Player = l_Caster->ToPlayer())
                    if (l_Player->GetHealth() == l_Player->GetMaxHealth())
                        this->Remove();
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_arterial_cut_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DAMAGE);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_arterial_cut_AuraScript();
        }
};

// Lingering Gaze - 138467
class spell_lingering_gaze_main : public SpellScriptLoader
{
    public:
        spell_lingering_gaze_main() : SpellScriptLoader("spell_lingering_gaze_main") { }

        class spell_lingering_gaze_main_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_lingering_gaze_main_SpellScript);

            void HandleOnCast()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, l_Caster, 100.0f);

                if (l_PlayerList.empty())
                    return;

                if (l_Caster->GetMap()->IsHeroic())
                    JadeCore::RandomResizeList(l_PlayerList, 5);
                else
                    JadeCore::RandomResizeList(l_PlayerList, 2);

                for (Player* l_Player: l_PlayerList)
                    l_Caster->CastSpell(l_Player, SPELL_LINGERING_GAZE_MISSILE, true);
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_lingering_gaze_main_SpellScript::HandleOnCast);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_lingering_gaze_main_SpellScript();
        }
};

// Life drain (main) - 133795
class spell_life_drain_main : public SpellScriptLoader
{
    public:
        spell_life_drain_main() : SpellScriptLoader("spell_life_drain_main") { }

        class spell_life_drain_main_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_life_drain_main_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                std::list<Player*> l_PlayerList;
                l_Caster->GetPlayerListInGrid(l_PlayerList, 200.0f);

                for (Player* l_Player : l_PlayerList)
                    l_Player->RemoveAura(SPELL_LIFE_DRAIN_DUMMY);

            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_life_drain_main_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_life_drain_main_AuraScript();
        }
};

// Life drain (aura) - 133796
class spell_life_drain_dummy : public SpellScriptLoader
{
    public:
        spell_life_drain_dummy() : SpellScriptLoader("spell_life_drain_dummy") { }

        class spell_life_drain_dummy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_life_drain_dummy_AuraScript);

            void OnTick(constAuraEffectPtr /*aurEff*/)
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                if (!l_Caster->GetInstanceScript())
                    return;

                if (Creature* l_Boss = l_Caster->GetCreature(*l_Caster, l_Caster->GetInstanceScript()->GetData64(NPC_DURUMU_THE_FORGOTTEN)))
                    l_Boss->ModifyHealth(urand(47500, 52500));
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_life_drain_dummy_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_life_drain_dummy_AuraScript();
        }
};

// Life drain (aura) - 133796
class spell_colorblind_phase_link_visual : public SpellScriptLoader
{
    public:
        spell_colorblind_phase_link_visual() : SpellScriptLoader("spell_colorblind_phase_link_visual") { }

        class spell_colorblind_phase_link_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_colorblind_phase_link_visual_AuraScript);

            void OnRemove(constAuraEffectPtr /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (!l_Caster || !l_Target)
                    return;

                switch (this->GetSpellInfo()->Id)
                {
                    case SPELL_BRIGHT_LIGHT:
                        //l_Caster->CastSpell(l_Caster, SPELL_BRIGHT_LIGHT_MAIN, true);
                        l_Caster->AddAura(SPELL_BRIGHT_LIGHT_MAIN, l_Caster);

                        if (Creature* l_Creature = l_Caster->ToCreature())
                            if (l_Creature->GetAI())
                                l_Creature->AI()->DoAction(ACTION_TURN_AROUND);
                        break;
                    case SPELL_BLUE_RAYS:
                        l_Caster->CastSpell(l_Target, SPELL_BLUE_RAYS_MAIN, true);
                        break;
                    case SPELL_INFRARED_LIGHT:
                        l_Caster->CastSpell(l_Target, SPELL_INFRARED_LIGHT_MAIN, true);
                        break;
                    default:
                        break;
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_colorblind_phase_link_visual_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL_OR_REAPPLY_MASK);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_colorblind_phase_link_visual_AuraScript();
        }
};


void AddSC_boss_durumu()
{
    new boss_durumu();
    new mob_hungry_eye();
    new mob_second_phase_eye();
    new mob_durumu_second_phase_add();
    new spell_arterial_cut();
    new spell_lingering_gaze_main();
    new spell_life_drain_dummy();
    new spell_colorblind_phase_link_visual();
}
