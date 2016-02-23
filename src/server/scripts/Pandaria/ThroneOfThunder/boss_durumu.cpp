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
    SPELL_HARD_STARE                     = 133765,
    SPELL_FORCE_OF_WILL                  = 136413, // May be too much powerful, to check with players
    SPELL_LINGERING_GAZE_MAIN            = 138467, // Script Effect
    SPELL_LINGERING_GAZE_MISSILE         = 133792,
    SPELL_LINGERING_GAZE_AT              = 133793,
    SPELL_LIFE_DRAIN_DUMMY               = 133795,
    SPELL_LIFE_DRAIN_AURA                = 133796,
    SPELL_LIFE_DRAIN_DMG                 = 133798,

    // Second Phase
    SPELL_BRIGHT_LIGHT                   = 134124, // Yellow link between caster/target
    SPELL_INFRARED_LIGHT                 = 134123, // Red link between caster/target
    SPELL_BLUE_RAYS                      = 134122, // Blue link between caster/target
    SPELL_BRIGHT_LIGHT_MAIN              = 133740,
    SPELL_BLUE_RAYS_MAIN                 = 133672,
    SPELL_INFRARED_LIGHT_MAIN            = 133734,
    SPELL_RED_FOG_INVISIBILITY           = 136116,
    SPELL_YELLOW_FOG_INVISIBILITY        = 136117,
    SPELL_BLUE_FOG_INVISIBILITY          = 136118,

    // Color adds spells
    SPELL_AMBER_ARMOR                    = 136174, // Amber Fog
    SPELL_BURST_OF_AMBER                 = 136123, // Amber Fog
    SPELL_AMBER_RETAILIATION             = 136175, // Amber Fog
    SPELL_CAUSTIC_SPIKE                  = 136154, // Crimson Fog
    SPELL_CRIMSON_BLOOM                  = 136122, // Crimson Fog
    SPELL_ICY_GRASP                      = 136177, // Azure Fog
    SPELL_FLASH_FREEZE                   = 136124,  // Azure Fog

    // Third Phase
    SPELL_SUMMON_EYEBEAM_TARGET          = 133775, // Target of 134169
    SPELL_DISINTEGRATION_BEAM_PRECAST    = 134169
};

enum eEvents
{
    EVENT_HARD_STARE                     = 1,
    EVENT_FORCE_OF_WILL                  = 2,
    EVENT_LINGERING_GAZE                 = 3,
    EVENT_DRAIN_LIFE                     = 4,
    EVENT_COLORBLIND_PHASE               = 5,  // Initialize second phase
    EVENT_RAY_SECOND_PHASE               = 6,  // Launch targets rays
    EVENT_SUMMON_RED_EYE                 = 7,
    EVENT_SUMMON_BLUE_EYE                = 8,
    EVENT_SUMMON_YELLOW_EYE              = 9,
    EVENT_ADD_ROTATION                   = 10, // Make the free ray rotating with the caster orientation
    EVENT_AMBER_RETAILIATION             = 11,
    EVENT_DISINTEGRATION_BEAM_PHASE      = 12, // Initialize third phase
    EVENT_STOP_DISINTEGRATION_BEAM_PHASE = 13
};

enum eActions
{
    ACTION_TURN_AROUND                   = 1,
    ACTION_ADD_REVEALED                  = 2,
    ACTION_SCHEDULE_ADD_SPELL            = 3
};

enum ePhases
{
    PHASE_NORMAL_PHASE                   = 1,
    PHASE_COLORBLIND_PHASE               = 2,
    PHASE_DISINTEGRATION_BEAM_PHASE      = 3
};

bool g_AddActivated[3] = { false, false, false };

uint32 g_AddEntries[3] = { NPC_AZURE_FOG, NPC_AMBER_FOG, NPC_CRIMSON_FOG };

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

                summons.DespawnAll();

                me->GetMotionMaster()->MoveTargetedHome();
                me->ReenableEvadeMode();
                // Make the boss static at his home position
                me->ClearUnitState(UNIT_STATE_ROOT);
                me->AddUnitState(UNIT_STATE_ROOT);
                me->GetAreaTriggerList(l_AreatriggerList, SPELL_LINGERING_GAZE_AT);

                if (!l_AreatriggerList.empty())
                    for (AreaTrigger* l_AreaTrigger : l_AreatriggerList)
                        l_AreaTrigger->Remove(0);

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

//                 // First phase
//                 m_Events.ScheduleEvent(EVENT_HARD_STARE, 8000); // Boss first standard spell
//                 m_Events.ScheduleEvent(EVENT_FORCE_OF_WILL, 13000); // Boss second standard spell
//                 m_Events.ScheduleEvent(EVENT_LINGERING_GAZE, 19000); // Bos third standard spell
//                 m_Events.ScheduleEvent(EVENT_DRAIN_LIFE, 25000); // Boss fourth standard spell

                // Second Phase
               // m_Events.ScheduleEvent(EVENT_COLORBLIND_PHASE, 12000); // Starts second phase
                m_Events.ScheduleEvent(EVENT_DISINTEGRATION_BEAM_PHASE, 12000);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();

                summons.DespawnAll();

                std::list<AreaTrigger*> l_AreatriggerList;
                me->GetAreaTriggerList(l_AreatriggerList, SPELL_LINGERING_GAZE_AT);

                if (!l_AreatriggerList.empty())
                    for (AreaTrigger* l_AreaTrigger : l_AreatriggerList)
                        l_AreaTrigger->Remove(0);

                if (m_Instance)
                {
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    m_Instance->SetBossState(DATA_DURUMU_THE_FORGOTTEN, DONE);
                }

                // LFR Loots
                if (me->GetMap()->IsLFR())
                {
                    me->SetLootRecipient(NULL);
                    Player* l_Player = me->GetMap()->GetPlayers().begin()->getSource();
                    if (l_Player && l_Player->GetGroup())
                        sLFGMgr->AutomaticLootDistribution(me, l_Player->GetGroup());
                }
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
                        float l_Angle = 0.0f;

                        m_Phase = PHASE_COLORBLIND_PHASE;

                        // Schedule every Eye summon
                        m_Events.ScheduleEvent(EVENT_SUMMON_RED_EYE, 500);
                        m_Events.ScheduleEvent(EVENT_SUMMON_BLUE_EYE, 900);
                        m_Events.ScheduleEvent(EVENT_SUMMON_YELLOW_EYE, 1300);

                        // 0 - Azure, 1 - Amber, 2 - Crimson
                        uint8 l_SpawnCounts[3] = { 0, 0, 0 };
                        uint8 l_TotalCount = 0;

                        if (IsHeroic())
                        {
                            l_SpawnCounts[0] = 3;
                            l_SpawnCounts[1] = 1;
                            l_SpawnCounts[2] = 1;
                            l_TotalCount = 5;
                        }
                        else
                        {
                            l_SpawnCounts[0] = 3;
                            l_SpawnCounts[2] = 1;
                            l_TotalCount = 4;
                        }

                        // Spawns invisble adds at almost random locations on the plateform.
                        for (uint8 i = 0; (i <= 2 && l_TotalCount); i++)
                        {
                            while (l_SpawnCounts[i])
                            {
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

                                --l_SpawnCounts[i];
                                --l_TotalCount;
                                l_Angle += (2 * M_PI) / 3 + frand(-20.0f, 20.0f);
                            }
                        }

                        m_Events.ScheduleEvent(EVENT_COLORBLIND_PHASE, 180000);
                        break;
                    }
                    case EVENT_SUMMON_RED_EYE:
                        me->SummonCreature(NPC_RED_EYE, 5894.78f, 4513.95f, -0.4788f);
                        break;
                    case EVENT_SUMMON_BLUE_EYE:
                        me->SummonCreature(NPC_BLUE_EYE, 5894.78f, 4513.95f, -0.4788f);
                        break;
                    case EVENT_SUMMON_YELLOW_EYE:
                        me->SummonCreature(NPC_YELLOW_EYE, 5894.78f, 4513.95f, -0.4788f);
                        break;
                    case EVENT_DISINTEGRATION_BEAM_PHASE:
                        m_Phase = PHASE_DISINTEGRATION_BEAM_PHASE;
                        //m_Events.ScheduleEvent(EVENT_DISINTEGRATION_BEAM_PHASE, 180000);
                        break;
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

// Hungry Eye - 67859 (Add for the spell DRAIN LIFE, coming from first phase)
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

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_Events.Reset();

                m_Events.ScheduleEvent(EVENT_DRAIN_LIFE, 5000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (me->HasAura(SPELL_LIFE_DRAIN_DUMMY))
                {
                    // When the add is summoned, this aura is added to himself 5 seconds later.

                    std::list<Player*> l_PlayerList;
                    Player* l_AuraPlayer = nullptr;

                    GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                    // If some player has the aura LIFE DRAIN, he's selected.
                    for (Player* l_Player : l_PlayerList)
                    {
                        if (!l_Player->HasAura(SPELL_LIFE_DRAIN_AURA))
                            continue;
                        else
                            l_AuraPlayer = l_Player;
                    }

                    // If some player not getting the aura runs in front of one player who has it, this first player gets the aura applied on him.
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
            float m_Orientation;
            bool m_RedSpellCasted; // Verifies if the add CRIMSON FOG has casted the spell CRIMSON BLOOD, which should be casted when the add runs out of the lightning cone.
            bool m_YellowSpellCasted; // Verifies if the add AMBER FOG has casted the spell BURST OF AMBER, which should be casted when the add runs out of the lightning cone.
            bool m_BlueSpellCasted;
            uint32 m_TableIndex;

            // For every add, direction is set to the boss, and invisibility aura is added. RED FOG spell is set as not casted.
            void Reset()
            {
                m_Orientation = 0;
                m_RedSpellCasted = false;
                m_YellowSpellCasted = false;
                m_BlueSpellCasted = false;
                me->AddUnitState(UNIT_STATE_ROOT);

                if (Creature* l_Boss = GetClosestCreatureWithEntry(me, NPC_DURUMU_THE_FORGOTTEN, 200.0f, true))
                {
                    m_Orientation = me->GetAngle(l_Boss->GetPositionX(), l_Boss->GetPositionY());

                    me->SetFacingTo(m_Orientation);
                    me->SetOrientation(m_Orientation);
                }

                switch (me->GetEntry())
                {
                    case NPC_CRIMSON_FOG:
                        me->CastSpell(me, SPELL_RED_FOG_INVISIBILITY, false);
                        m_TableIndex = 2;
                        break;
                    case NPC_AMBER_FOG:
                        me->CastSpell(me, SPELL_YELLOW_FOG_INVISIBILITY, false);
                        m_TableIndex = 1;
                        break;
                    case NPC_AZURE_FOG:
                        me->CastSpell(me, SPELL_BLUE_FOG_INVISIBILITY, false);
                        m_TableIndex = 0;
                        break;
                    default:
                        break;
                }
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                m_Events.Reset();
            }

            // When AMBER FOG enters in combat, his armor aura which "triggers" another spell is added. The event casting the triggered spell is scheduled.
            void EnterCombat(Unit* /*victim*/)
            {
                switch (me->GetEntry())
                {
                    case NPC_CRIMSON_FOG:
                        break;
                    case NPC_AMBER_FOG:
                        me->AddAura(SPELL_AMBER_ARMOR, me);
                        break;
                    case NPC_AZURE_FOG:
                        break;
                }
            }

            void DoAction(const int32 p_Action)
            {
                // This action is called when the add is in the right light cone. About the CRIMSON FOG, it makes him casting a spell.
                // For every add, invisibility is removed and template is set to make him attackable.
                if (p_Action == ACTION_ADD_REVEALED)
                {
                    switch (me->GetEntry())
                    {
                        case NPC_CRIMSON_FOG:
                            me->RemoveAurasDueToSpell(SPELL_RED_FOG_INVISIBILITY);
                            me->CastSpell(me, SPELL_CAUSTIC_SPIKE, false);
                            break;
                        case NPC_AMBER_FOG:
                            me->RemoveAurasDueToSpell(SPELL_YELLOW_FOG_INVISIBILITY);
                            break;
                        case NPC_AZURE_FOG:
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
                // This action is called when CRIMSON FOG runs out of the cone. The boolean check is added to avoid infinite loop.
                else if (p_Action == ACTION_SCHEDULE_ADD_SPELL)
                {
                    switch (me->GetEntry())
                    {
                        case NPC_CRIMSON_FOG:
                        {
                            if (g_AddActivated[m_TableIndex])
                            {
                                if (!m_RedSpellCasted)
                                {
                                    me->CastSpell(me, SPELL_CRIMSON_BLOOM, false);
                                    m_RedSpellCasted = true;
                                }
                            }
                            break;
                        }
                        case NPC_AMBER_FOG:
                        {
                            if (g_AddActivated[m_TableIndex])
                            {
                                if (!m_YellowSpellCasted)
                                {
                                    me->CastSpell(me, SPELL_BURST_OF_AMBER, false);
                                    m_YellowSpellCasted = true;
                                }
                            }
                            break;
                        }
                        case NPC_AZURE_FOG:
                        {
                            if (g_AddActivated[m_TableIndex])
                            {
                                if (!m_BlueSpellCasted)
                                {
                                    me->CastSpell(me, SPELL_FLASH_FREEZE, false);
                                    m_BlueSpellCasted = true;
                                }
                            }
                            break;
                        }
                            break;
                        default:
                            break;
                    }
                }
            }

            void JustDied (Unit* p_Killer)
            {
                bool l_AddsAllDead = true;

                for (uint8 l_I = 0; l_I < 3; l_I++)
                    if (g_AddEntries[l_I] != me->GetEntry())
                        if (GetClosestCreatureWithEntry(me, g_AddEntries[l_I], 200.0f, true))
                            l_AddsAllDead = false;

                if (l_AddsAllDead)
                {

                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (!UpdateVictim())
                    return;

                DoMeleeAttackIfReady();

                m_Events.Update(p_Diff);

                // When AMBER FOG is in combat, every 10% of life reached, is scheduled some event which makes him casting a spell (AMBER RETAILIATION).
                if (me->isInCombat())
                {
                    if (me->HasAura(SPELL_AMBER_ARMOR))
                    {
                        if (me->GetEntry() == NPC_AMBER_FOG)
                        {
                            float l_HealthPct = me->GetHealthPct();
                            int8 l_HealthPctInt = G3D::iRound(l_HealthPct / 10) * 10;

                            switch (l_HealthPctInt)
                            {
                                case 90:
                                case 80:
                                case 70:
                                case 60:
                                case 50:
                                case 40:
                                case 30:
                                case 20:
                                case 10:
                                    m_Events.ScheduleEvent(EVENT_AMBER_RETAILIATION, 300);
                                    break;
                                default:
                                    break;
                            }
                        }
                    }
                }

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_AMBER_RETAILIATION:
                        me->CastSpell(me, SPELL_AMBER_RETAILIATION, false);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_durumu_second_phase_addAI(p_Creature);
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
            uint64 m_RedTargetGuid;
            uint64 m_BlueTargetGuid;

            // Set color eyes not moving, and set the boolean who checks if adds are activated to false.
            void Reset()
            {
                me->setFaction(35);
                me->SetReactState(REACT_PASSIVE);
                me->AddUnitState(UNIT_STATE_ROOT);

                m_RedTargetGuid  = 0;
                m_BlueTargetGuid = 0;

                for (uint8 l_I = 0; l_I < 3; l_I++)
                    g_AddActivated[l_I] = false;
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
                    m_Events.ScheduleEvent(EVENT_ADD_ROTATION, 150);
            }

            void SetGUID(uint64 guid, int32 id)
            {
                if (id == 1)
                    m_RedTargetGuid = guid;
                else if (id == 2)
                    m_BlueTargetGuid = guid;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                // Awesome part...
                // This part of the script is here to verify if one of the invisble adds is targeted by a cone.
                // If so, an action is called, which makes them visible and attackable.
                if ((me->GetEntry() == NPC_YELLOW_EYE && me->HasAura(SPELL_BRIGHT_LIGHT_MAIN)) ||
                    (me->GetEntry() == NPC_BLUE_EYE && me->HasAura(SPELL_BLUE_RAYS_MAIN)) ||
                    (me->GetEntry() == NPC_RED_EYE && me->HasAura(SPELL_INFRARED_LIGHT_MAIN)))
                {
                    switch (me->GetEntry())
                    {
                        case NPC_BLUE_EYE:
                        {
                            if (Creature* l_Add = GetClosestCreatureWithEntry(me, NPC_AZURE_FOG, 200.0f))
                            {
                                if (l_Add->HasAura(SPELL_BLUE_FOG_INVISIBILITY))
                                {
                                    if (l_Add->GetAI())
                                    {
                                        if (me->isInFront(l_Add, (M_PI / 6)))
                                        {
                                            l_Add->AI()->DoAction(ACTION_ADD_REVEALED);
                                            g_AddActivated[0] = true;
                                        }
                                    }
                                }
                                else
                                {
                                    if (l_Add->GetAI())
                                    {
                                        if (!me->isInFront(l_Add, (M_PI / 6)) && g_AddActivated[0])
                                        {
                                            CAST_AI(mob_durumu_second_phase_add::mob_durumu_second_phase_addAI, l_Add->AI())->m_BlueSpellCasted = false;
                                        }
                                        else
                                        {
                                            if (g_AddActivated[0])
                                                l_Add->AI()->DoAction(ACTION_SCHEDULE_ADD_SPELL);
                                        }
                                    }
                                }

                            }
                            break;
                        }
                        case NPC_YELLOW_EYE:
                        {
                            if (Creature* l_Add = GetClosestCreatureWithEntry(me, NPC_AMBER_FOG, 200.0f))
                            {
                                if (l_Add->HasAura(SPELL_YELLOW_FOG_INVISIBILITY))
                                {
                                    if (l_Add->GetAI())
                                    {
                                        if (me->isInFront(l_Add, (M_PI / 6)))
                                        {
                                            l_Add->AI()->DoAction(ACTION_ADD_REVEALED);
                                            g_AddActivated[1] = true;
                                        }
                                    }
                                }
                                else
                                {
                                    if (l_Add->GetAI())
                                    {
                                        if (!me->isInFront(l_Add, (M_PI / 6)) && g_AddActivated[1])
                                        {
                                            CAST_AI(mob_durumu_second_phase_add::mob_durumu_second_phase_addAI, l_Add->AI())->m_YellowSpellCasted = false;
                                        }
                                        else
                                        {
                                            if (g_AddActivated[1])
                                                l_Add->AI()->DoAction(ACTION_SCHEDULE_ADD_SPELL);
                                        }
                                    }
                                }

                            }
                            break;
                        }
                        // In this case, more particular, the boolean who checks if the spell has been casted is set to false, into the add script directly.
                        // In the case that he's no more targeted by the cone, the spell will be casted, and the boolean will be set back to true.
                        case NPC_RED_EYE:
                        {
                            if (Creature* l_Add = GetClosestCreatureWithEntry(me, NPC_CRIMSON_FOG, 200.0f))
                            {
                                if (l_Add->HasAura(SPELL_RED_FOG_INVISIBILITY))
                                {
                                    if (l_Add->GetAI())
                                    {
                                        if (me->isInFront(l_Add, (M_PI / 6)))
                                        {
                                            l_Add->AI()->DoAction(ACTION_ADD_REVEALED);
                                            g_AddActivated[2] = true;
                                        }
                                    }
                                }
                                else
                                {
                                    if (l_Add->GetAI())
                                    {
                                        if (!me->isInFront(l_Add, (M_PI / 6)) && g_AddActivated[2])
                                        {
                                            CAST_AI(mob_durumu_second_phase_add::mob_durumu_second_phase_addAI, l_Add->AI())->m_RedSpellCasted = false;
                                        }
                                        else
                                        {
                                            if (g_AddActivated[2])
                                                l_Add->AI()->DoAction(ACTION_SCHEDULE_ADD_SPELL);
                                        }
                                    }
                                }

                            }
                            break;
                        }
                        default:
                            break;
                    }
                }

                switch (m_Events.ExecuteEvent())
                {
                    // This event consists in targeting players and casting the linking light aura cone on them.
                    // For every eye but the yellow one, target GUID is saved, to channel the aura applied next to it.
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
                                            SetGUID(l_Player->GetGUID(), 1);
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
                                            SetGUID(l_Player->GetGUID(), 2);
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
                    // Some rotation is applied for the free cone (with no target), and the other ones update their orientation to the players for who the GUID has been saved earlier.
                    case EVENT_ADD_ROTATION:
                    {
                        switch (me->GetEntry())
                        {
                            case NPC_YELLOW_EYE:
                                me->SetFacingTo(me->GetOrientation() + 0.04f);
                                me->SetOrientation(me->GetOrientation() + 0.04f);
                                break;
                            case NPC_BLUE_EYE:
                            {
                                float l_Orientation = 0;

                                if (Player* l_Target = me->GetPlayer(*me, m_BlueTargetGuid))
                                {
                                    l_Orientation = me->GetAngle(l_Target->GetPositionX(), l_Target->GetPositionY());
                                    me->SetOrientation(l_Orientation);
                                    me->SetFacingTo(l_Orientation);
                                }
                                break;
                            }
                            case NPC_RED_EYE:
                            {
                                float l_Orientation = 0;

                                if (Player* l_Target = me->GetPlayer(*me, m_RedTargetGuid))
                                {
                                    l_Orientation = me->GetAngle(l_Target->GetPositionX(), l_Target->GetPositionY());
                                    me->SetOrientation(l_Orientation);
                                    me->SetFacingTo(l_Orientation);
                                }
                                break;
                            }
                            default:
                                break;
                        }

                        m_Events.ScheduleEvent(EVENT_ADD_ROTATION, 250);

                        if (Creature* l_RedEye = GetClosestCreatureWithEntry(me, NPC_RED_EYE, 5.0f))
                            if (l_RedEye->GetAI())
                                l_RedEye->AI()->DoAction(ACTION_TURN_AROUND);

                        if (Creature* l_RedEye = GetClosestCreatureWithEntry(me, NPC_BLUE_EYE, 5.0f))
                            if (l_RedEye->GetAI())
                                l_RedEye->AI()->DoAction(ACTION_TURN_AROUND);
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

// Arterial Cut (aura) - 133768
class spell_arterial_cut: public SpellScriptLoader
{
    public:
        spell_arterial_cut() : SpellScriptLoader("spell_arterial_cut") { }

        class spell_arterial_cut_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_arterial_cut_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
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
class spell_lingering_gaze_main: public SpellScriptLoader
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
class spell_life_drain_main: public SpellScriptLoader
{
    public:
        spell_life_drain_main() : SpellScriptLoader("spell_life_drain_main") { }

        class spell_life_drain_main_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_life_drain_main_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
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
class spell_life_drain_dummy: public SpellScriptLoader
{
    public:
        spell_life_drain_dummy() : SpellScriptLoader("spell_life_drain_dummy") { }

        class spell_life_drain_dummy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_life_drain_dummy_AuraScript);

            void OnTick(AuraEffect const* /*aurEff*/)
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
class spell_colorblind_phase_link_visual: public SpellScriptLoader
{
    public:
        spell_colorblind_phase_link_visual() : SpellScriptLoader("spell_colorblind_phase_link_visual") { }

        class spell_colorblind_phase_link_visual_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_colorblind_phase_link_visual_AuraScript);

            void OnRemove(AuraEffect const* /*aurEff*/, AuraEffectHandleModes /*mode*/)
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetTarget();

                if (!l_Caster || !l_Target)
                    return;

                switch (this->GetSpellInfo()->Id)
                {
                    case SPELL_BRIGHT_LIGHT:
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
