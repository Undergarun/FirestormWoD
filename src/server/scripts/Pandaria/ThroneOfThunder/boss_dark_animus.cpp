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
#include "SpellScript.h"
#include "throne_of_thunder.h"

enum eSpells
{
    // Anima Orb
    SPELL_TRANSFUSION_AOE           = 138118,
    SPELL_TRANSFUSION_ENERGIZE      = 138399,

    // Anima Golem
    SPELL_EVASIVE                   = 140759,
    SPELL_ACCELERATION_LINK         = 138453,
    SPELL_ANIMA_CAPACITY_4          = 138384,
    SPELL_CRITICALLY_DAMAGED_2      = 138409,

    // Large Anima Golem
    SPELL_CRIMSON_WAKE              = 138480,
    SPELL_CRIMSON_WAKE_SLOW         = 138482,
    SPELL_CRIMSON_WAKE_MEDIUM       = 138484,
    SPELL_CRIMSON_WAKE_FAST         = 138544,
    SPELL_FIXATED                   = 138486,
    SPELL_ANIMA_CAPACITY_8          = 138385,

    // Massive Anima Golem
    SPELL_MATTER_SWAP               = 138609,
    SPELL_MATTER_SWAP_DAMAGE        = 138618,
    SPELL_TARGETED_MATTER_SWAP      = 139919,
    SPELL_EXPLOSIVE_SLAM            = 138569,
    SPELL_ANIMA_CAPACITY_36         = 138386,

    // Dark Animus
    SPELL_ACTIVATION_SEQUENCE       = 139537,   // LFR only
    SPELL_SIPHON_ANIMA              = 138644,
    SPELL_TOUCH_OF_THE_ANIMUS       = 138659,
    SPELL_EMPOWER_GOLEM             = 138780,   // Heroic only
    SPELL_ANIMA_RING                = 136954,   // 25 Anima needed
    SPELL_ANIMA_RING_1              = 136955,
    SPELL_ANIMA_RING_2              = 136956,
    SPELL_ANIMA_RING_3              = 136957,
    SPELL_ANIMA_RING_4              = 136958,
    SPELL_ANIMA_RING_5              = 136959,
    SPELL_ANIMA_RING_6              = 136960,
    SPELL_ANIMA_RING_7              = 138671,
    SPELL_ANIMA_RING_8              = 138672,
    SPELL_ANIMA_RING_9              = 138673,
    SPELL_ANIMA_RING_10             = 138674,
    SPELL_ANIMA_RING_11             = 138675,
    SPELL_ANIMA_RING_12             = 138676,
    SPELL_ANIMA_RING_DEBUFF         = 136962,
    SPELL_ANIMA_FONT                = 138691,   // 50 Anima needed
    SPELL_ANIMA_FONT_MISSILE        = 138697,
    SPELL_INTERRUPTING_JOLT         = 138763,   // 75 Anima needed
    SPELL_FULL_POWER                = 138729,   // 100 Anima neded (soft enrage)
    SPELL_FULL_POWER_MISSILE        = 138749,
    SPELL_ANIMA_CAPACITY_100        = 138387,
    SPELL_BERSERK                   = 47008,

    // Misc
    SPELL_BLOOD_ORB_VISUAL          = 138116,
    SPELL_HOVER                     = 138092,
    SPELL_FIERY_HANDS               = 34223,
    SPELL_SHIELD_INTERRUPTION       = 140021,
    SPELL_RITUAL_TRANSFORM_1        = 137618,
    SPELL_RITUAL_TRANSFORM_2        = 137898,
    SPELL_RITUAL_TRANSFORM_3        = 137899,
    SPELL_RITUAL_TRANSFORM_4        = 137900,
    SPELL_RITUAL_TRANSFORM_5        = 137902,
    SPELL_RITUAL_TRANSFORM_6        = 137903,
    SPELL_RITUAL_TRANSFORM_7        = 137904,
    SPELL_RITUAL_TRANSFORM_8        = 137906,
    SPELL_RITUAL_TRANSFORM_9        = 137944,
    SPELL_RITUAL_TRANSFORM_10       = 137945,
    SPELL_RITUAL_TRANSFORM_11       = 137946,
    SPELL_RITUAL_TRANSFORM_12       = 137947,
    SPELL_RITUAL_TRANSFORM_13       = 137948,
    SPELL_RITUAL_TRANSFORM_14       = 137949,
    SPELL_RITUAL_TRANSFORM_15       = 137951,
    SPELL_RITUAL_TRANSFORM_16       = 137952,
    SPELL_RITUAL_TRANSFORM_17       = 137953,
    SPELL_RITUAL_TRANSFORM_18       = 137954,
    SPELL_RITUAL_TRANSFORM_19       = 137955,
    SPELL_RITUAL_TRANSFORM_20       = 137956,
    SPELL_RITUAL_TRANSFORM_21       = 137957,
    SPELL_RITUAL_TRANSFORM_22       = 137958,
    SPELL_RITUAL_TRANSFORM_23       = 137959,
    SPELL_RITUAL_TRANSFORM_24       = 137960,
    SPELL_ZERO_ENERGY               = 72242,
    SPELL_TURNED_OFF                = 138373,
    SPELL_ENERGIZE_SEARCHER         = 138378,
    SPELL_ENERGIZE_CUSTOM           = 138909,
    SPELL_FIRE_BLAST                = 138099,
    SPELL_SHADOW_NOVA               = 137998,
    SPELL_SHORT_RITUAL_LIGHTNING    = 137995,
    SPELL_LONG_RITUAL_LIGHTNING     = 137994
};

enum eEvents
{
    EVENT_BERSERK   = 1,
    EVENT_FIRE_BLAST,
    EVENT_RITUAL_LIGHTNING,
    EVENT_SHADOW_NOVA,
    EVENT_CHECK_NEAR_GOLEM,
    EVENT_CRIMSON_WAKE,
    EVENT_EXPLOSIVE_SLAM,
    EVENT_MATTER_SWAP,
    EVENT_SIPHON_ANIMA,
    EVENT_TOUCH_OF_THE_ANIMUS,
    EVENT_ANIMA_RING,
    EVENT_ANIMA_FONT,
    EVENT_INTERRUPTING_JOLT,
    EVENT_FULL_POWER,
    EVENT_EMPOWER_GOLEM
};

enum eSays
{
    TALK_INTRO,
    TALK_AGGRO,
    TALK_DEATH
};

enum eActions
{
    ACTION_FIGHT_RITUALISTS,
    ACTION_KILL_RITUALISTS,
    ACTION_ACTIVATE_GOLEM,
    ACTION_DESACTIVATE_GOLEM
};

uint32 const g_RitualistTransform[24] =
{
    SPELL_RITUAL_TRANSFORM_1,
    SPELL_RITUAL_TRANSFORM_2,
    SPELL_RITUAL_TRANSFORM_3,
    SPELL_RITUAL_TRANSFORM_4,
    SPELL_RITUAL_TRANSFORM_5,
    SPELL_RITUAL_TRANSFORM_6,
    SPELL_RITUAL_TRANSFORM_7,
    SPELL_RITUAL_TRANSFORM_8,
    SPELL_RITUAL_TRANSFORM_9,
    SPELL_RITUAL_TRANSFORM_10,
    SPELL_RITUAL_TRANSFORM_11,
    SPELL_RITUAL_TRANSFORM_12,
    SPELL_RITUAL_TRANSFORM_13,
    SPELL_RITUAL_TRANSFORM_14,
    SPELL_RITUAL_TRANSFORM_15,
    SPELL_RITUAL_TRANSFORM_16,
    SPELL_RITUAL_TRANSFORM_17,
    SPELL_RITUAL_TRANSFORM_18,
    SPELL_RITUAL_TRANSFORM_19,
    SPELL_RITUAL_TRANSFORM_20,
    SPELL_RITUAL_TRANSFORM_21,
    SPELL_RITUAL_TRANSFORM_22,
    SPELL_RITUAL_TRANSFORM_23,
    SPELL_RITUAL_TRANSFORM_24
};

uint32 const g_AnimaRingSpells[12] =
{
    SPELL_ANIMA_RING_1,
    SPELL_ANIMA_RING_2,
    SPELL_ANIMA_RING_3,
    SPELL_ANIMA_RING_4,
    SPELL_ANIMA_RING_5,
    SPELL_ANIMA_RING_6,
    SPELL_ANIMA_RING_7,
    SPELL_ANIMA_RING_8,
    SPELL_ANIMA_RING_9,
    SPELL_ANIMA_RING_10,
    SPELL_ANIMA_RING_11,
    SPELL_ANIMA_RING_12
};

#define MAX_GOLEM_TYPE 3
uint32 const g_AnimaGolems[MAX_GOLEM_TYPE] =
{
    NPC_ANIMA_GOLEM,
    NPC_LARGE_ANIMA_GOLEM,
    NPC_MASSIVE_ANIMA_GOLEM
};

uint32 const g_AnimaSpellPower[MAX_GOLEM_TYPE] =
{
    SPELL_ANIMA_CAPACITY_4,
    SPELL_ANIMA_CAPACITY_8,
    SPELL_ANIMA_CAPACITY_36
};

Position const g_MassiveGolemPos[2] =
{
    { 5779.6f, 4772.56f, 77.6523f, 3.10299f },
    { 5779.15f, 4840.23f, 77.6523f, 3.32945f }
};

Position const g_LargeGolemPos[8] =
{
    { 5758.77f, 4753.87f, 77.6523f, 1.5708f },
    { 5749.37f, 4753.82f, 77.6523f, 1.5708f },
    { 5714.85f, 4859.34f, 77.6523f, 4.71239f },
    { 5749.67f, 4859.12f, 77.6523f, 4.71239f },
    { 5758.92f, 4858.97f, 77.6523f, 4.71239f },
    { 5706.99f, 4754.17f, 77.6523f, 1.5708f },
    { 5715.76f, 4753.81f, 77.6523f, 1.5708f },
    { 5705.67f, 4859.f, 77.6523f, 4.71239f }
};

Position const g_AnimaGolemPos[25] =
{
    { 5763.82f, 4765.f, 77.6523f, 1.5708f },
    { 5756.32f, 4765.f, 77.6523f, 1.5708f },
    { 5707.95f, 4848.f, 77.6523f, 4.71239f },
    { 5715.45f, 4848.f, 77.6523f, 4.71239f },
    { 5724.62f, 4837.5f, 75.3576f, 4.71239f },
    { 5732.12f, 4837.5f, 75.3577f, 4.71239f },
    { 5748.82f, 4848.f, 77.6523f, 4.71239f },
    { 5739.62f, 4837.5f, 75.3576f, 4.71239f },
    { 5756.32f, 4848.f, 77.6523f, 4.71239f },
    { 5763.82f, 4848.f, 77.6523f, 4.71239f },
    { 5700.45f, 4765.f, 77.6523f, 1.5708f },
    { 5707.95f, 4765.f, 77.6523f, 1.5708f },
    { 5777.f, 4828.23f, 77.6523f, 3.14159f },
    { 5765.f, 4813.98f, 75.3576f, 3.14159f },
    { 5724.62f, 4775.5f, 75.3576f, 1.5708f },
    { 5715.45f, 4765.f, 77.6523f, 1.5708f },
    { 5777.f, 4820.73f, 77.6523f, 3.14159f },
    { 5765.f, 4806.48f, 75.3576f, 3.14159f },
    { 5732.12f, 4775.5f, 75.3577f, 1.5708f },
    { 5739.62f, 4775.5f, 75.3576f, 1.5708f },
    { 5765.f, 4798.98f, 75.3577f, 3.14159f },
    { 5777.f, 4792.23f, 77.6523f, 3.14159f },
    { 5748.82f, 4765.f, 77.6523f, 1.5708f },
    { 5777.f, 4784.73f, 77.6523f, 3.14159f },
    { 5700.45f, 4848.f, 77.6523f, 4.71239f }
};

enum eNumGolems
{
    NUM_ANIMA_GOLEM = 25,
    NUM_LARGE_GOLEM = 8,
    NUM_MASSIVE_GOLEM = 2
};

#define MAX_ANIMA_CAPACITY 4

// Dark Animus - 69427
class boss_dark_animus : public CreatureScript
{
    public:
        boss_dark_animus() : CreatureScript("boss_dark_animus") { }

        enum eMisc
        {
            LFRToTThirdPart = 612
        };

        struct boss_dark_animusAI : public BossAI
        {
            boss_dark_animusAI(Creature* p_Creature) : BossAI(p_Creature, DATA_DARK_ANIMUS)
            {
                m_Instance = p_Creature->GetInstanceScript();
                p_Creature->CastSpell(p_Creature, SPELL_TURNED_OFF, true);
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            uint32 m_CheckPowerTimer;

            int32 m_PowerCaps[MAX_ANIMA_CAPACITY];
            bool m_PowerCapsReached[MAX_ANIMA_CAPACITY];

            void Reset()
            {
                m_Events.Reset();

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);

                me->CastSpell(me, SPELL_ANIMA_CAPACITY_100, true);
                me->CastSpell(me, SPELL_ZERO_ENERGY, true);

                me->ReenableEvadeMode();
                me->SetReactState(REACT_PASSIVE);

                me->RemoveAura(SPELL_ACTIVATION_SEQUENCE);

                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_DARK_ANIMUS) != DONE)
                    {
                        m_Instance->SetBossState(DATA_DARK_ANIMUS, NOT_STARTED);
                        SummonGolems();
                    }
                }

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                _Reset();

                for (uint8 l_Iter = 0; l_Iter < MAX_ANIMA_CAPACITY; ++l_Iter)
                {
                    m_PowerCapsReached[l_Iter] = false;
                    m_PowerCaps[l_Iter] = (l_Iter + 1) * 25;
                }

                m_CheckPowerTimer = 1000;
            }

            void SummonGolems()
            {
                if (!m_Instance)
                    return;

                /// If encounter already done, don't spawn
                if (m_Instance->GetBossState(DATA_DARK_ANIMUS) == DONE)
                    return;

                /// Check if golems are already here or not
                if (GetClosestCreatureWithEntry(me, NPC_ANIMA_GOLEM, 200.f))
                    return;

                std::list<Creature*> l_AnimaGolemList;
                for (uint8 l_Itr = 0; l_Itr < NUM_ANIMA_GOLEM; ++l_Itr)
                {
                    if (Creature* l_Golem = me->SummonCreature(NPC_ANIMA_GOLEM, g_AnimaGolemPos[l_Itr]))
                        l_AnimaGolemList.push_back(l_Golem);
                }

                if (!Is25ManRaid())
                {
                    /// Should always be true, just prevent potential crash
                    if (l_AnimaGolemList.size() > 13)
                    {
                        JadeCore::RandomResizeList(l_AnimaGolemList, 13);
                        for (Creature* l_Golem : l_AnimaGolemList)
                        {
                            if (l_Golem)
                                l_Golem->AI()->SetData(DATA_GOLEM_DAMAGED, 1);
                        }
                    }
                }

                std::list<Creature*> l_LargeGolemList;
                for (uint8 l_Itr = 0; l_Itr < NUM_LARGE_GOLEM; ++l_Itr)
                {
                    if (Creature* l_Golem = me->SummonCreature(NPC_LARGE_ANIMA_GOLEM, g_LargeGolemPos[l_Itr]))
                        l_LargeGolemList.push_back(l_Golem);
                }

                if (!Is25ManRaid())
                {
                    /// Should always be true, just prevent potential crash
                    if (l_LargeGolemList.size() > 3)
                    {
                        JadeCore::RandomResizeList(l_LargeGolemList, 3);
                        for (Creature* l_Golem : l_LargeGolemList)
                        {
                            if (l_Golem)
                                l_Golem->AI()->SetData(DATA_GOLEM_DAMAGED, 1);
                        }
                    }
                }

                /// No Critically Damaged Massive Golem...
                for (uint8 l_Itr = 0; l_Itr < NUM_MASSIVE_GOLEM; ++l_Itr)
                    me->SummonCreature(NPC_MASSIVE_ANIMA_GOLEM, g_MassiveGolemPos[l_Itr]);
            }

            void JustReachedHome()
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_DARK_ANIMUS) != FAIL)
                    return;

                me->CastSpell(me, SPELL_TURNED_OFF, true);
            }

            void KilledUnit(Unit* p_Who)
            {
            }

            void CheckHitResult(MeleeHitOutcome& p_MeleeHit, SpellMissInfo& p_SpellMissInfo, Unit* p_Attacker, SpellInfo const* const p_SpellInfo)
            {
                if (!p_SpellInfo)
                    return;

                uint32 l_SpellId = p_SpellInfo->Id;
                if (l_SpellId == SPELL_ENERGIZE_CUSTOM || l_SpellId == SPELL_ACTIVATION_SEQUENCE || l_SpellId == SPELL_ANIMA_CAPACITY_100 ||
                    l_SpellId == SPELL_TRANSFUSION_ENERGIZE || l_SpellId == SPELL_ZERO_ENERGY)
                    return;

                if (me->HasAura(SPELL_TURNED_OFF))
                {
                    p_MeleeHit = MELEE_HIT_MISS;
                    p_SpellMissInfo = SPELL_MISS_MISS;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_DARK_ANIMUS) == IN_PROGRESS)
                        return;
                }

                if (me->HasAura(SPELL_TURNED_OFF) && me->GetPower(POWER_ENERGY) <= 0)
                {
                    p_Damage = 0;
                    return;
                }
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (!m_Instance)
                    return;

                if (!me->HasFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT))
                    return;

                if (me->HasAura(SPELL_TURNED_OFF) && me->GetPower(POWER_ENERGY) <= 0)
                    return;

                me->RemoveAura(SPELL_TURNED_OFF);

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                m_Events.ScheduleEvent(EVENT_BERSERK, 600000);  // 10 min
                m_Events.ScheduleEvent(EVENT_SIPHON_ANIMA, 6000);
                m_Events.ScheduleEvent(EVENT_TOUCH_OF_THE_ANIMUS, 11000);

                if (IsHeroic())
                    m_Events.ScheduleEvent(EVENT_EMPOWER_GOLEM, 16000);
            }

            void EnterEvadeMode()
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_DARK_ANIMUS) == DONE)
                    return;

                if (m_Instance->GetBossState(DATA_DARK_ANIMUS) != IN_PROGRESS && me->HasAura(SPELL_TURNED_OFF))
                    return;

                m_Instance->SetBossState(DATA_DARK_ANIMUS, FAIL);
                m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);

                m_Events.Reset();
                summons.DespawnAll();
                me->RemoveAllAuras();
                me->SetFullHealth();
                me->RemoveAllDynObjects();
                me->RemoveAllAreasTrigger();
                me->SetReactState(REACT_PASSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                for (uint8 l_Iter = 0; l_Iter < MAX_ANIMA_CAPACITY; ++l_Iter)
                {
                    m_PowerCapsReached[l_Iter] = false;
                    m_PowerCaps[l_Iter] = (l_Iter + 1) * 25;
                }

                me->GetMotionMaster()->MoveTargetedHome();

                SummonGolems();
            }

            void JustDied(Unit* p_Killer)
            {
                _JustDied();

                if (m_Instance != nullptr)
                {
                    m_Instance->SetBossState(DATA_DARK_ANIMUS, DONE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SPELL_TOUCH_OF_THE_ANIMUS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(eSpells::SPELL_ANIMA_FONT);
                }

                if (IsLFR())
                {
                    me->SetLootRecipient(nullptr);

                    bool l_Assigned = false;
                    Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                    for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                    {
                        if (Player* l_Player = l_Itr->getSource())
                        {
                            uint32 l_DungeonID = l_Player->GetGroup() ? sLFGMgr->GetDungeon(l_Player->GetGroup()->GetGUID()) : 0;
                            if (l_DungeonID != eMisc::LFRToTThirdPart)
                                continue;

                            if (!me || l_Player->IsAtGroupRewardDistance(me))
                                sLFGMgr->RewardDungeonDoneFor(l_DungeonID, l_Player);

                            if (l_Assigned)
                                continue;

                            if (Group* l_Group = l_Player->GetGroup())
                            {
                                l_Assigned = true;
                                sLFGMgr->AutomaticLootAssignation(me, l_Group);
                            }
                        }
                    }
                }
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ACTIVATE_GOLEM:
                    {
                        m_CheckPowerTimer = 0;

                        if (Player* l_Target = me->FindNearestPlayer(50.f))
                            EnterCombat(l_Target);
                        break;
                    }
                    case ACTION_SUMMON_GOLEMS:
                    {
                        SummonGolems();
                        break;
                    }
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                p_Value = 0;
            }

            void PowerModified(Powers p_Power, int32 p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                if (p_Value > 0 && me->HasAura(SPELL_TURNED_OFF))
                {
                    if (Player* l_Target = me->FindNearestPlayer(50.0f))
                        EnterCombat(l_Target);
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                // When the Dark Animus receives any amount of Anima (as little as 4), he activates.
                if (m_Instance && m_Instance->GetBossState(DATA_DARK_ANIMUS) == IN_PROGRESS && m_CheckPowerTimer)
                {
                    if (m_CheckPowerTimer <= p_Diff)
                    {
                        if (me->GetPower(POWER_ENERGY) <= 0)
                            m_CheckPowerTimer = 1000;
                        else
                        {
                            m_CheckPowerTimer = 0;

                            if (Player* l_Target = me->FindNearestPlayer(100.f))
                                AttackStart(l_Target);
                        }
                    }
                    else
                        m_CheckPowerTimer -= p_Diff;
                }

                for (uint8 l_Iter = 0; l_Iter < MAX_ANIMA_CAPACITY; ++l_Iter)
                {
                    if (m_PowerCapsReached[l_Iter])
                        continue;

                    if (me->GetPower(POWER_ENERGY) >= m_PowerCaps[l_Iter])
                    {
                        m_PowerCapsReached[l_Iter] = true;

                        switch (EVENT_ANIMA_RING + l_Iter)
                        {
                            case EVENT_ANIMA_RING:
                                m_Events.ScheduleEvent(EVENT_ANIMA_RING, 23000);
                                break;
                            case EVENT_ANIMA_FONT:
                                m_Events.ScheduleEvent(EVENT_ANIMA_FONT, 14000);
                                break;
                            case EVENT_INTERRUPTING_JOLT:
                                m_Events.ScheduleEvent(EVENT_INTERRUPTING_JOLT, 23000);
                                break;
                            case EVENT_FULL_POWER:
                                me->CastSpell(me, SPELL_FULL_POWER, false);
                                break;
                            default:
                                break;
                        }
                    }
                }

                if (!UpdateVictim() || me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_BERSERK:
                        me->CastSpell(me, SPELL_BERSERK, true);
                        break;
                    case EVENT_SIPHON_ANIMA:
                    {
                        uint8 l_Rand = urand(0, 10);
                        me->CastSpell(me, SPELL_SIPHON_ANIMA, true);
                        if (l_Rand)
                            me->MonsterTextEmote("Dark Animus drains power from the other golems with |cFFF00000|Hspell:138644|h[Siphon Anima]|h|r!", 0, true);
                        m_Events.ScheduleEvent(EVENT_SIPHON_ANIMA, 6000);
                        break;
                    }
                    case EVENT_TOUCH_OF_THE_ANIMUS:
                        me->CastSpell(me, SPELL_TOUCH_OF_THE_ANIMUS, true);
                        m_Events.ScheduleEvent(EVENT_TOUCH_OF_THE_ANIMUS, 11000);
                        break;
                    case EVENT_ANIMA_RING:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_ANIMA_RING, false);
                        m_Events.ScheduleEvent(EVENT_ANIMA_RING, 24200);
                        break;
                    case EVENT_ANIMA_FONT:
                        me->CastSpell(me, SPELL_ANIMA_FONT, false);
                        m_Events.ScheduleEvent(EVENT_ANIMA_FONT, 25000);
                        break;
                    case EVENT_INTERRUPTING_JOLT:
                        me->CastSpell(me, SPELL_INTERRUPTING_JOLT, false);
                        me->MonsterTextEmote("Stop casting! Dark Animus is generating an |cFFF00000|Hspell:138763|h[Interrupting Jolt]|h|r.", 0, true);
                        m_Events.ScheduleEvent(EVENT_INTERRUPTING_JOLT, 21500);
                        break;
                    case EVENT_EMPOWER_GOLEM:
                        me->CastSpell(me, SPELL_EMPOWER_GOLEM, true);
                        m_Events.ScheduleEvent(EVENT_EMPOWER_GOLEM, 16000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_dark_animusAI(p_Creature);
        }
};

// Anima Orb - 69756
class mob_anima_orb : public CreatureScript
{
    public:
        mob_anima_orb() : CreatureScript("mob_anima_orb") { }

        struct mob_anima_orbAI : public ScriptedAI
        {
            mob_anima_orbAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_IntroDone = false;
                m_Instance = p_Creature->GetInstanceScript();
                m_LinkedDarkRitualist = 0;
            }

            bool m_IntroDone;
            bool m_FightStarted;
            bool m_RitualistKilled;

            InstanceScript* m_Instance;
            uint64 m_LinkedDarkRitualist;

            void Reset()
            {
                me->CastSpell(me, SPELL_BLOOD_ORB_VISUAL, true);
                me->AddUnitState(UNIT_STATE_ROOT);
                me->SetReactState(REACT_PASSIVE);

                m_FightStarted = CheckRitualists();
                m_RitualistKilled = false;
                m_LinkedDarkRitualist = 0;

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_MATTER_SWAP);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_EXPLOSIVE_SLAM);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TOUCH_OF_THE_ANIMUS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ANIMA_FONT);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ANIMA_RING_DEBUFF);
                }
            }

            void EnterEvadeMode()
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_DARK_ANIMUS) == DONE)
                    return;

                me->SetFullHealth();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
                Reset();

                if (Creature* l_DarkAnimus = Creature::GetCreature(*me, m_Instance->GetData64(NPC_DARK_ANIMUS)))
                    l_DarkAnimus->AI()->EnterEvadeMode();
            }

            void MoveInLineOfSight(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER || m_IntroDone || !m_Instance)
                    return;

                if (p_Who->GetDistance(me) >= 50.f)
                    return;

                m_IntroDone = true;

                if (Creature* l_DarkAnimus = Creature::GetCreature(*me, m_Instance->GetData64(NPC_DARK_ANIMUS)))
                    l_DarkAnimus->AI()->DoAction(ACTION_SUMMON_GOLEMS);

                if (Creature* l_DarkRitualist = me->FindNearestCreature(NPC_DARK_RITUALIST, 20.f))
                {
                    m_LinkedDarkRitualist = l_DarkRitualist->GetGUID();
                    l_DarkRitualist->AI()->Talk(TALK_INTRO);
                }
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_FIGHT_RITUALISTS:
                    {
                        if (m_FightStarted || !m_LinkedDarkRitualist)
                            break;

                        if (Creature* l_DarkRitualist = Creature::GetCreature(*me, m_LinkedDarkRitualist))
                            l_DarkRitualist->AI()->Talk(TALK_AGGRO);

                        m_FightStarted = true;
                        break;
                    }
                    case ACTION_KILL_RITUALISTS:
                    {
                        if (m_RitualistKilled || !m_LinkedDarkRitualist)
                            break;

                        if (Creature* l_DarkRitualist = Creature::GetCreature(*me, m_LinkedDarkRitualist))
                            l_DarkRitualist->AI()->Talk(TALK_DEATH);

                        m_RitualistKilled = true;
                        break;
                    }
                    default:
                        break;
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                p_Damage = 0;

                if (!m_Instance)
                    return;

                if (!m_Instance->CheckRequiredBosses(DATA_DARK_ANIMUS) || p_Attacker->GetPositionX() < 5655.f)
                    return;

                if (m_Instance->GetBossState(DATA_DARK_ANIMUS) != DONE)
                {
                    if (!GetClosestCreatureWithEntry(me, NPC_ANIMA_GOLEM, 200.0f))
                    {
                        if (Creature* l_DarkAnimus = Creature::GetCreature(*me, m_Instance->GetData64(NPC_DARK_ANIMUS)))
                            l_DarkAnimus->AI()->DoAction(ACTION_SUMMON_GOLEMS);
                    }
                }

                me->CastSpell(me, SPELL_TRANSFUSION_AOE, true);

                m_Instance->SetBossState(DATA_DARK_ANIMUS, IN_PROGRESS);
                DoZoneInCombat();

                if (IsLFR())
                {
                    if (Creature* l_DarkAnimus = Creature::GetCreature(*me, m_Instance->GetData64(NPC_DARK_ANIMUS)))
                        l_DarkAnimus->CastSpell(l_DarkAnimus, SPELL_ACTIVATION_SEQUENCE, true);
                }

                me->RemoveAllAuras();
                me->MonsterTextEmote("The orb explodes", 0, true);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            bool CheckRitualists()
            {
                if (GetClosestCreatureWithEntry(me, NPC_DARK_RITUALIST, 150.0f))
                    return false;

                return true;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_anima_orbAI(p_Creature);
        }
};

// Dark Ritualist - 69702
class mob_dark_ritualist : public CreatureScript
{
    public:
        mob_dark_ritualist() : CreatureScript("mob_dark_ritualist") { }

        struct mob_dark_ritualistAI : public ScriptedAI
        {
            mob_dark_ritualistAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset()
            {
                me->ReenableEvadeMode();

                m_Events.Reset();

                me->CastSpell(me, g_RitualistTransform[urand(0, 23)], true);
                me->CastSpell(me, SPELL_HOVER, true);
                me->CastSpell(me, SPELL_SHIELD_INTERRUPTION, true);

                if (urand(0, 1))
                    me->AddAura(SPELL_FIERY_HANDS, me);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (m_Instance)
                {
                    if (Creature* l_AnimaOrb = Creature::GetCreature(*me, m_Instance->GetData64(NPC_ANIMA_ORB)))
                        l_AnimaOrb->AI()->DoAction(ACTION_FIGHT_RITUALISTS);
                }

                me->RemoveAura(SPELL_HOVER);
                me->RemoveAura(SPELL_SHIELD_INTERRUPTION);

                m_Events.ScheduleEvent(EVENT_FIRE_BLAST, urand(2000, 5000));
                m_Events.ScheduleEvent(EVENT_SHADOW_NOVA, urand(3000, 6000));
                m_Events.ScheduleEvent(EVENT_RITUAL_LIGHTNING, urand(5000, 10000));
            }

            void JustDied(Unit* p_Killer)
            {
                if (m_Instance)
                {
                    if (Creature* l_AnimaOrb = Creature::GetCreature(*me, m_Instance->GetData64(NPC_ANIMA_ORB)))
                        l_AnimaOrb->AI()->DoAction(ACTION_KILL_RITUALISTS);
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_FIRE_BLAST:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_FIRE_BLAST, true);
                        m_Events.ScheduleEvent(EVENT_FIRE_BLAST, urand(12000, 15000));
                        break;
                    case EVENT_SHADOW_NOVA:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_SHADOW_NOVA, true);
                        m_Events.ScheduleEvent(EVENT_SHADOW_NOVA, urand(13000, 16000));
                        break;
                    case EVENT_RITUAL_LIGHTNING:
                        if (me->HasAura(SPELL_FIERY_HANDS))
                            me->CastSpell(me, SPELL_LONG_RITUAL_LIGHTNING, false);
                        else
                            me->CastSpell(me, SPELL_SHORT_RITUAL_LIGHTNING, false);
                        m_Events.ScheduleEvent(EVENT_RITUAL_LIGHTNING, urand(15000, 20000));
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_dark_ritualistAI(p_Creature);
        }
};

// Anima Golem - 69701
class mob_anima_golem : public CreatureScript
{
    public:
        mob_anima_golem() : CreatureScript("mob_anima_golem") { }

        struct mob_anima_golemAI : public ScriptedAI
        {
            mob_anima_golemAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                p_Creature->CastSpell(p_Creature, SPELL_TURNED_OFF, true);
            }

            EventMap m_Events;
            bool m_Activated;
            bool m_Damaged;
            int32 m_Anima;

            void Reset()
            {
                m_Damaged = false;
                m_Anima = 0;

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);

                me->CastSpell(me, SPELL_ANIMA_CAPACITY_4, true);
                me->CastSpell(me, SPELL_ZERO_ENERGY, true);
                me->CastSpell(me, SPELL_EVASIVE, true);

                me->RemoveAura(SPELL_EMPOWER_GOLEM);

                me->ReenableEvadeMode();
                me->SetReactState(REACT_PASSIVE);

                m_Activated = false;

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                m_Events.Reset();
            }

            void SetData(uint32 p_Data, uint32 p_Value)
            {
                if (p_Data == DATA_GOLEM_DAMAGED)
                {
                    m_Damaged = true;
                    DoAction(ACTION_DESACTIVATE_GOLEM);
                }
            }

            void EnterCombat(Unit* p_Attacker)
            {
                me->RemoveAura(SPELL_TURNED_OFF);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                m_Events.ScheduleEvent(EVENT_CHECK_NEAR_GOLEM, 1000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                // Disabled golems cannot be killed
                if (me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                {
                    p_Damage = 0;
                    return;
                }
            }

            void JustDied(Unit* p_Killer)
            {
                me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), SPELL_ENERGIZE_SEARCHER, true);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ACTIVATE_GOLEM:
                    {
                        if (me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                            break;

                        me->RemoveAura(SPELL_TURNED_OFF);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                        me->SetReactState(REACT_AGGRESSIVE);

                        if (Player* l_Target = me->FindNearestPlayer(150.f))
                            AttackStart(l_Target);

                        m_Activated = true;
                        break;
                    }
                    case ACTION_DESACTIVATE_GOLEM:
                    {
                        me->RemoveAura(SPELL_TURNED_OFF);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->CastSpell(me, SPELL_CRITICALLY_DAMAGED_1, true);
                        me->CastSpell(me, SPELL_CRITICALLY_DAMAGED_2, true);
                        me->DisableHealthRegen();
                        me->SetHealth(1);
                        me->SetReactState(REACT_PASSIVE);
                        me->AddUnitState(UNIT_STATE_ROOT);
                        me->CastSpell(me, SPELL_ZERO_ENERGY, true);
                        me->CastSpell(me, SPELL_ANIMA_CAPACITY_4, true);
                        me->SetPower(POWER_ENERGY, 0);
                        break;
                    }
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers p_Power, int32 &p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                p_Value = 0;
            }

            void PowerModified(Powers p_Power, int32 p_Value)
            {
                if (p_Power != POWER_ENERGY || p_Value < 0)
                    return;

                if (me->isAlive())
                    m_Anima = me->GetPower(POWER_ENERGY);
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == DATA_ANIMA_LEVEL)
                    return m_Anima;
                if (p_Type == DATA_WAITING_ANIMA)
                    return 4;
                return 0;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_Damaged && !me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                    DoAction(ACTION_DESACTIVATE_GOLEM);

                if (!m_Activated || !UpdateVictim() || m_Damaged)
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_NEAR_GOLEM:
                    {
                        std::list<Creature*> l_GolemList;
                        me->GetCreatureListWithEntryInGrid(l_GolemList, NPC_ANIMA_GOLEM, 2.f);

                        // Remove before reapply to prevent overstacking
                        me->RemoveAura(SPELL_ACCELERATION_LINK);

                        if (l_GolemList.empty())
                        {
                            m_Events.ScheduleEvent(EVENT_CHECK_NEAR_GOLEM, 1000);
                            break;
                        }

                        for (Creature* l_Golem : l_GolemList)
                        {
                            if (l_Golem->GetGUID() == me->GetGUID())
                                continue;

                            me->CastSpell(me, SPELL_ACCELERATION_LINK, true);
                        }

                        m_Events.ScheduleEvent(EVENT_CHECK_NEAR_GOLEM, 1000);
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
            return new mob_anima_golemAI(p_Creature);
        }
};

// Large Anima Golem - 69700
class mob_large_anima_golem : public CreatureScript
{
    public:
        mob_large_anima_golem() : CreatureScript("mob_large_anima_golem") { }

        struct mob_large_anima_golemAI : public ScriptedAI
        {
            mob_large_anima_golemAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                p_Creature->CastSpell(p_Creature, SPELL_TURNED_OFF, true);
                m_Instance = p_Creature->GetInstanceScript();
            }

            uint32 m_CheckPowerTimer;
            uint32 m_Anima;
            uint32 m_WaitingAnima;
            InstanceScript* m_Instance;
            EventMap m_Events;
            bool m_Damaged;

            void Reset()
            {
                m_Damaged = false;
                m_Anima = 0;
                m_WaitingAnima = 0;

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);

                me->CastSpell(me, SPELL_ANIMA_CAPACITY_8, true);
                me->CastSpell(me, SPELL_ZERO_ENERGY, true);

                me->RemoveAura(SPELL_EMPOWER_GOLEM);

                me->ReenableEvadeMode();
                me->SetReactState(REACT_PASSIVE);

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                m_Events.Reset();
                DespawnCrimsonWakes();

                m_CheckPowerTimer = 1000;
            }

            void SetData(uint32 p_Data, uint32 p_Value)
            {
                switch (p_Data)
                {
                    case DATA_GOLEM_DAMAGED:
                        m_Damaged = true;
                        DoAction(ACTION_DESACTIVATE_GOLEM);
                        break;
                    case DATA_WAITING_ANIMA:
                        m_WaitingAnima += p_Value;
                        break;
                    case DATA_ANIMA_RECEIVE:
                        m_WaitingAnima -= p_Value > m_WaitingAnima ? m_WaitingAnima : p_Value;
                        break;
                    default:
                        break;
                }
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                p_Value = 0;
            }

            void PowerModified(Powers p_Power, int32 p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                if (me->GetPower(POWER_ENERGY) >= me->GetMaxPower(POWER_ENERGY) && me->HasAura(SPELL_TURNED_OFF))
                    DoAction(ACTION_ACTIVATE_GOLEM);

                if (me->isAlive())
                    m_Anima = me->GetPower(POWER_ENERGY);
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == DATA_ANIMA_LEVEL)
                    return m_Anima;
                if (p_Type == DATA_WAITING_ANIMA)
                    return m_WaitingAnima;
                return 0;
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const* p_SpellInfo)
            {
                // Disabled golems cannot be killed
                if (me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                {
                    p_Damage = 0;
                    return;
                }
            }

            void JustDied(Unit* p_Killer)
            {
                me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), SPELL_ENERGIZE_SEARCHER, true);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ACTIVATE_GOLEM:
                    {
                        if (me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                            break;

                        me->RemoveAura(SPELL_TURNED_OFF);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                        me->SetReactState(REACT_AGGRESSIVE);

                        if (Player* l_Target = me->FindNearestPlayer(150.f))
                            AttackStart(l_Target);
                        break;
                    }
                    case ACTION_DESACTIVATE_GOLEM:
                    {
                        me->RemoveAura(SPELL_TURNED_OFF);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);
                        me->CastSpell(me, SPELL_CRITICALLY_DAMAGED_1, true);
                        me->CastSpell(me, SPELL_CRITICALLY_DAMAGED_2, true);
                        me->DisableHealthRegen();
                        me->SetHealth(1);
                        me->SetReactState(REACT_PASSIVE);
                        me->AddUnitState(UNIT_STATE_ROOT);
                        me->CastSpell(me, SPELL_ZERO_ENERGY, true);
                        me->CastSpell(me, SPELL_ANIMA_CAPACITY_8, true);
                        me->SetPower(POWER_ENERGY, 0);

                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_Damaged && !me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                    DoAction(ACTION_DESACTIVATE_GOLEM);

                if (me->HasAura(SPELL_CRITICALLY_DAMAGED_1))
                    return;

                if (m_Instance && m_Instance->GetBossState(DATA_DARK_ANIMUS) == IN_PROGRESS && m_CheckPowerTimer)
                {
                    if (m_CheckPowerTimer <= p_Diff)
                    {
                        if (me->GetPower(POWER_ENERGY) < me->GetMaxPower(POWER_ENERGY))
                            m_CheckPowerTimer = 1000;
                        else
                        {
                            m_CheckPowerTimer = 0;
                            DoAction(ACTION_ACTIVATE_GOLEM);
                        }
                    }
                    else
                        m_CheckPowerTimer -= p_Diff;

                    return;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CRIMSON_WAKE:
                        me->CastSpell(me, SPELL_CRIMSON_WAKE, false);
                        m_Events.ScheduleEvent(EVENT_CRIMSON_WAKE, 30000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void DespawnCrimsonWakes()
            {
                std::list<Creature*> l_CrimsonWakesList;
                me->GetCreatureListWithEntryInGrid(l_CrimsonWakesList, NPC_CRIMSON_WAKE, 150.f);

                for (Creature* l_CrimsonWake : l_CrimsonWakesList)
                    l_CrimsonWake->DespawnOrUnsummon();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_large_anima_golemAI(p_Creature);
        }
};

// Massive Anima Golem = 69699
class mob_massive_anima_golem : public CreatureScript
{
    public:
        mob_massive_anima_golem() : CreatureScript("mob_massive_anima_golem") { }

        struct mob_massive_anima_golemAI : public ScriptedAI
        {
            mob_massive_anima_golemAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                p_Creature->CastSpell(p_Creature, SPELL_TURNED_OFF, true);
                m_Instance = p_Creature->GetInstanceScript();
            }

            uint32 m_CheckPowerTimer;
            uint32 m_Anima;
            uint32 m_WaitingAnima;
            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset()
            {
                m_Anima = 0;
                m_WaitingAnima = 0;

                me->setPowerType(POWER_ENERGY);
                me->SetMaxPower(POWER_ENERGY, 100);
                me->SetPower(POWER_ENERGY, 0);

                me->CastSpell(me, SPELL_ANIMA_CAPACITY_36, true);
                me->CastSpell(me, SPELL_ZERO_ENERGY, true);

                me->RemoveAura(SPELL_EMPOWER_GOLEM);

                me->ReenableEvadeMode();
                me->SetReactState(REACT_PASSIVE);

                me->RemoveFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_REGENERATE_POWER);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_UNK_6); // Sniffed value
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK5); // Sniffed value

                m_Events.Reset();

                m_CheckPowerTimer = 1000;
            }
            
            void EnterCombat(Unit* p_Attacker)
            {
                me->RemoveAura(SPELL_TURNED_OFF);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                m_Events.ScheduleEvent(EVENT_EXPLOSIVE_SLAM, urand(4000, 8000));
                m_Events.ScheduleEvent(EVENT_MATTER_SWAP, urand(8000, 12000));

                AttackStart(p_Attacker);
            }

            void RegeneratePower(Powers p_Power, int32& p_Value)
            {
                if (p_Power != POWER_ENERGY)
                    return;

                p_Value = 0;
            }

            void PowerModified(Powers p_Power, int32 p_Value)
            {
                if (p_Power != POWER_ENERGY || p_Value < 0)
                    return;

                if (p_Value > 0 && me->HasAura(SPELL_TURNED_OFF))
                {
                    if (Player* l_Target = me->FindNearestPlayer(150.0f))
                        EnterCombat(l_Target);
                }

                if (me->isAlive())
                    m_Anima = me->GetPower(POWER_ENERGY);
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == DATA_ANIMA_LEVEL)
                    return m_Anima;
                if (p_Type == DATA_WAITING_ANIMA)
                    return m_WaitingAnima;
                return 0;
            }

            void SetData(uint32 p_Data, uint32 p_Value)
            {
                switch (p_Data)
                {
                    case DATA_WAITING_ANIMA:
                        m_WaitingAnima += p_Value;
                        break;
                    case DATA_ANIMA_RECEIVE:
                        m_WaitingAnima -= p_Value > m_WaitingAnima ? m_WaitingAnima : p_Value;
                        break;
                    default:
                        break;
                }
            }

            void JustDied(Unit* p_Killer)
            {
                me->CastSpell(me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), SPELL_ENERGIZE_SEARCHER, true);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                // When a Massive Anima Golem receives any amount of Anima
                // (so, as little as 4 Anima, from the death of an Anima Golem), it becomes active.
                if (m_Instance && m_Instance->GetBossState(DATA_DARK_ANIMUS) == IN_PROGRESS && m_CheckPowerTimer)
                {
                    if (m_CheckPowerTimer <= p_Diff)
                    {
                        if (me->GetPower(POWER_ENERGY) <= 0)
                            m_CheckPowerTimer = 1000;
                        else
                        {
                            m_CheckPowerTimer = 0;

                            if (Player* l_Target = me->FindNearestPlayer(50.f))
                                AttackStart(l_Target);
                        }
                    }
                    else
                        m_CheckPowerTimer -= p_Diff;

                    return;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_EXPLOSIVE_SLAM:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_EXPLOSIVE_SLAM, false);
                        m_Events.ScheduleEvent(EVENT_EXPLOSIVE_SLAM, 15000);
                        break;
                    case EVENT_MATTER_SWAP:
                        me->CastSpell(me, SPELL_MATTER_SWAP, false);
                        m_Events.ScheduleEvent(EVENT_MATTER_SWAP, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_massive_anima_golemAI(p_Creature);
        }
};

// Crimson Wake - 69951
class mob_crimson_wake : public CreatureScript
{
    public:
        mob_crimson_wake() : CreatureScript("mob_crimson_wake") { }

        struct mob_crimson_wakeAI : public ScriptedAI
        {
            mob_crimson_wakeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint64 m_FollowedPlayerGUID;

            void Reset()
            {
                me->CastSpell(me, SPELL_CRIMSON_WAKE_SLOW, true);
                me->SetReactState(REACT_PASSIVE);

                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                if (!p_Summoner || !p_Summoner->ToCreature() || !p_Summoner->ToCreature()->AI())
                    return;

                if (Unit* l_Target = p_Summoner->ToCreature()->AI()->SelectTarget(SELECT_TARGET_RANDOM, 0, 50.f, true, -SPELL_FIXATED))
                {
                    m_FollowedPlayerGUID = l_Target->GetGUID();
                    AttackStart(l_Target);
                    me->CastSpell(l_Target, SPELL_FIXATED, true);
                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveChase(l_Target, 0.5f);
                }
            }

            void UpdateAI(const uint32 p_Diff) { }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_crimson_wakeAI(p_Creature);
        }
};

// Long Ritual Lightning - 137994
class spell_long_ritual_lightning : public SpellScriptLoader
{
    public:
        spell_long_ritual_lightning() : SpellScriptLoader("spell_long_ritual_lightning") { }

        class spell_long_ritual_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_long_ritual_lightning_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (!p_Object)
                        return true;

                    if (p_Object->GetDistance(GetCaster()) > 30.f)
                        return false;

                    return true;
                });
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_long_ritual_lightning_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_long_ritual_lightning_SpellScript();
        }
};

// Transfusion (AoE) - 138118
class spell_transfusion_aoe : public SpellScriptLoader
{
    public:
        spell_transfusion_aoe() : SpellScriptLoader("spell_transfusion_aoe") { }

        class spell_transfusion_aoe_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_transfusion_aoe_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    p_Targets.clear();
                    std::list<Creature*> l_AnimaGolemList;
                    GetCreatureListWithEntryInGrid(l_AnimaGolemList, l_Caster, NPC_ANIMA_GOLEM, 200.0f);

                    for (Creature* l_AnimaGolem : l_AnimaGolemList)
                        p_Targets.push_back(l_AnimaGolem);
                }
            }

            void HandleOnHit()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (l_Target->GetEntry() != NPC_ANIMA_GOLEM)
                        return;

                    l_Target->CastSpell(l_Target, SPELL_TRANSFUSION_ENERGIZE, true);

                    if (l_Target->ToCreature() && l_Target->ToCreature()->AI())
                        l_Target->ToCreature()->AI()->DoAction(ACTION_ACTIVATE_GOLEM);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_transfusion_aoe_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnHit += SpellHitFn(spell_transfusion_aoe_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_transfusion_aoe_SpellScript();
        }
};

// Transfusion (Searcher) - 138378
class spell_transfusion_searcher : public SpellScriptLoader
{
    public:
        spell_transfusion_searcher() : SpellScriptLoader("spell_transfusion_searcher") { }

        class spell_transfusion_searcher_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_transfusion_searcher_SpellScript);

            int32 m_CasterPowerLevel;

            bool Load()
            {
                m_CasterPowerLevel = 0;
                return true;
            }

            void SetCasterPower()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->ToCreature())
                        m_CasterPowerLevel = l_Caster->GetAI()->GetData(DATA_ANIMA_LEVEL);
                }
            }

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    p_Targets.clear();
                    
                    /// Adding Large and Massive Anima Golems
                    uint32 l_TargetEntries[2] = { NPC_LARGE_ANIMA_GOLEM, NPC_MASSIVE_ANIMA_GOLEM };
                    for (uint8 l_Idx = 0; l_Idx < 2; ++l_Idx)
                    {
                        std::list<Creature*> l_GolemList;
                        GetCreatureListWithEntryInGrid(l_GolemList, l_Caster, l_TargetEntries[l_Idx], 200.0f);

                        for (Creature* l_Golem : l_GolemList)
                        {
                            if (!l_Golem->isAlive() || l_Golem->HasAura(SPELL_CRITICALLY_DAMAGED_1) || l_Golem->HasAura(SPELL_CRITICALLY_DAMAGED_2))
                                continue;

                            /// Anima already sent to the golem but not received yet; prevent golem to receive more anima he can handle
                            /// (this prevents a major bug in the encounter: if a golem receives more anim he can, the residue is lost.
                            /// As there's exactly 100 anima which matches to Dark Animus max power, if some anima is lost, the boss can
                            /// never reach his max power, which triggers FULL POWER, which will almost wipes the raid).
                            uint32 l_WaitingAnima = l_Golem->AI()->GetData(DATA_WAITING_ANIMA);

                            if ((l_Golem->GetPower(POWER_ENERGY) + m_CasterPowerLevel + int32(l_WaitingAnima)) <= l_Golem->GetMaxPower(POWER_ENERGY))
                                p_Targets.push_back(l_Golem);
                        }
                    }

                    /// Adding Dark Animus
                    if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                    {
                        if (Creature* l_DarkAnimus = Creature::GetCreature(*l_Caster, l_Instance->GetData64(NPC_DARK_ANIMUS)))
                            p_Targets.push_back(l_DarkAnimus);
                    }

                    /// Should not happen...
                    if (p_Targets.empty())
                        return;

                    // In LFR difficulty, a random golem is chosen.
                    if (l_Caster->GetMap()->IsLFR())
                        JadeCore::RandomResizeList(p_Targets, 1);
                    // In other difficulties, the nearest golem is chosen
                    else
                    {
                        p_Targets.sort(JadeCore::WorldObjectDistanceCompareOrderPred(l_Caster));
                        WorldObject* l_WorldObject = (*p_Targets.begin());
                        p_Targets.clear();
                        p_Targets.push_back(l_WorldObject);
                    }

                    if (WorldObject* l_TargetObject = p_Targets.front())
                    {
                        if (Creature* l_Target = l_TargetObject->ToCreature())
                            l_Target->AI()->SetData(DATA_WAITING_ANIMA, m_CasterPowerLevel);
                    }
                }
            }

            void HandleOnHit()
            {
                if (Unit* l_Target = GetHitUnit())
                {
                    if (l_Target->GetEntry() == NPC_DARK_ANIMUS)
                        l_Target->RemoveAura(SPELL_TURNED_OFF);

                    l_Target->CastCustomSpell(l_Target, SPELL_ENERGIZE_CUSTOM, &m_CasterPowerLevel, NULL, NULL, true);
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_transfusion_searcher_SpellScript::SetCasterPower);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_transfusion_searcher_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnHit += SpellHitFn(spell_transfusion_searcher_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_transfusion_searcher_SpellScript();
        }
};

/// 138909 - Transfusion - SPELL_ENERGIZE_CUSTOM
class spell_custom_transfusion : public SpellScriptLoader
{
    public:
        spell_custom_transfusion() : SpellScriptLoader("spell_custom_transfusion") { }

        class spell_custom_transfusion_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_custom_transfusion_SpellScript);

            void TransferAnima()
            {
                Creature* l_Target = GetHitCreature();

                if (!l_Target)
                    return;

                int32 l_TransferedAnima = GetSpellValue()->EffectBasePoints[EFFECT_0];
                l_Target->AI()->SetData(DATA_ANIMA_RECEIVE, uint32(l_TransferedAnima));
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_custom_transfusion_SpellScript::TransferAnima);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_custom_transfusion_SpellScript();
        }
};

// Crimson Wake (Slow) - 138482
class spell_crimson_wake_slow : public SpellScriptLoader
{
    public:
        spell_crimson_wake_slow() : SpellScriptLoader("spell_crimson_wake_slow") {}

        class spell_crimson_wake_slow_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_crimson_wake_slow_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_CRIMSON_WAKE_MEDIUM, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_crimson_wake_slow_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_crimson_wake_slow_AuraScript();
        }
};

// Crimson Wake (Medium) - 138484
class spell_crimson_wake_medium : public SpellScriptLoader
{
    public:
        spell_crimson_wake_medium() : SpellScriptLoader("spell_crimson_wake_medium") {}

        class spell_crimson_wake_medium_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_crimson_wake_medium_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_CRIMSON_WAKE_FAST, true);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_crimson_wake_medium_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_crimson_wake_medium_AuraScript();
        }
};

/// Fixated - 138486
class spell_fixated : public SpellScriptLoader
{
    public:
        spell_fixated() : SpellScriptLoader("spell_fixated") {}

        class spell_fixated_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_fixated_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->ToCreature()->DespawnOrUnsummon();
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_fixated_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_MOD_POSSESS_PET, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_fixated_AuraScript();
        }
};

// Matter Swap - 138609
class spell_matter_swap : public SpellScriptLoader
{
    public:
        spell_matter_swap() : SpellScriptLoader("spell_matter_swap") { }

        class spell_matter_swap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_matter_swap_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if(JadeCore::UnitAuraCheck(true, SPELL_MATTER_SWAP));

                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_matter_swap_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_matter_swap_SpellScript();
        }

        class spell_matter_swap_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_matter_swap_AuraScript);

            void HandleEffectUpdate(uint32 p_Diff, AuraEffect* p_AurEff)
            {
                if (Aura* l_MatterSwap = p_AurEff->GetBase())
                {
                    if (AuraEffect* l_Effect0 = l_MatterSwap->GetEffect(EFFECT_0))
                    {
                        if (Unit* l_Target = GetUnitOwner())
                        {
                            uint32 l_ElapsedTime = l_MatterSwap->GetMaxDuration() - l_MatterSwap->GetDuration();

                            // See http://fr.wowhead.com/npc=69427#comments:id=1847317
                            if (l_ElapsedTime < 6000)
                            {
                                p_AurEff->ChangeAmount(25);
                                l_Effect0->ChangeAmount(75);
                            }
                            else if (l_ElapsedTime < 9000)
                            {
                                p_AurEff->ChangeAmount(50);
                                l_Effect0->ChangeAmount(50);
                            }
                            else if (l_ElapsedTime < 12000)
                            {
                                p_AurEff->ChangeAmount(75);
                                l_Effect0->ChangeAmount(25);
                            }
                            else
                            {
                                p_AurEff->ChangeAmount(100);
                                l_Effect0->ChangeAmount(0);
                            }
                        }
                    }
                }
            }

            void HandleDispel(DispelInfo* p_DispelData)
            {
                if (Unit* l_MassiveGolem = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        std::list<Player*> l_PlayerList;
                        l_Target->GetPlayerListInGrid(l_PlayerList, 100.f);

                        if (l_PlayerList.empty())
                            return;

                        uint64 l_CasterGuid = l_MassiveGolem->GetGUID();
                        l_PlayerList.remove_if([this, l_CasterGuid](Player* p_Player) -> bool
                        {
                            if (!p_Player || !p_Player->HasAura(SPELL_TARGETED_MATTER_SWAP))
                                return true;

                            if (p_Player->GetAura(SPELL_TARGETED_MATTER_SWAP, l_CasterGuid) == nullptr)
                                return true;

                            return false;
                        });

                        if (l_PlayerList.empty())
                            return;

                        if (Player* l_Player = (*l_PlayerList.begin()))
                        {
                            int32 l_TargetDamage = l_Target->CountPctFromMaxHealth(GetEffect(EFFECT_0)->GetAmount());
                            int32 l_PlayerDamage = l_Player->CountPctFromMaxHealth(GetEffect(EFFECT_1)->GetAmount());

                            if (l_TargetDamage)
                                l_MassiveGolem->CastCustomSpell(l_Target, SPELL_MATTER_SWAP_DAMAGE, &l_TargetDamage, NULL, NULL, true);
                            if (l_PlayerDamage)
                                l_MassiveGolem->CastCustomSpell(l_Player, SPELL_MATTER_SWAP_DAMAGE, &l_PlayerDamage, NULL, NULL, true);

                            Position l_TargetPos, l_PlayerPos;
                            l_Player->GetPosition(&l_PlayerPos);
                            l_Target->GetPosition(&l_TargetPos);

                            l_Player->NearTeleportTo(l_TargetPos.m_positionX, l_TargetPos.m_positionY, l_TargetPos.m_positionZ, l_TargetPos.m_orientation);
                            l_Target->NearTeleportTo(l_PlayerPos.m_positionX, l_PlayerPos.m_positionY, l_PlayerPos.m_positionZ, l_PlayerPos.m_orientation);
                        }
                    }
                }
            }

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes p_Mode)
            {
                AuraRemoveMode l_RemoveMode = GetTargetApplication()->GetRemoveMode();
                if (l_RemoveMode != AURA_REMOVE_BY_EXPIRE)
                    return;

                if (Unit* l_MassiveGolem = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        std::list<Player*> l_PlayerList;
                        l_Target->GetPlayerListInGrid(l_PlayerList, 100.f);

                        if (l_PlayerList.empty())
                            return;

                        uint64 l_CasterGuid = l_MassiveGolem->GetGUID();
                        l_PlayerList.remove_if([this, l_CasterGuid](Player* p_Player) -> bool
                        {
                            if (!p_Player || !p_Player->HasAura(SPELL_TARGETED_MATTER_SWAP))
                                return true;

                            if (p_Player->GetAura(SPELL_TARGETED_MATTER_SWAP, l_CasterGuid) == nullptr)
                                return true;

                            return false;
                        });

                        if (l_PlayerList.empty())
                            return;

                        if (Player* l_Player = (*l_PlayerList.begin()))
                        {
                            int32 l_TargetDamage = l_Target->CountPctFromMaxHealth(p_AurEff->GetBase()->GetEffect(EFFECT_0)->GetAmount());
                            int32 l_PlayerDamage = l_Target->CountPctFromMaxHealth(p_AurEff->GetAmount());

                            if (l_TargetDamage)
                                l_MassiveGolem->CastCustomSpell(l_Target, SPELL_MATTER_SWAP_DAMAGE, &l_TargetDamage, NULL, NULL, true);
                            if (l_PlayerDamage)
                                l_MassiveGolem->CastCustomSpell(l_Player, SPELL_MATTER_SWAP_DAMAGE, &l_PlayerDamage, NULL, NULL, true);

                            Position l_TargetPos, l_PlayerPos;
                            l_Player->GetPosition(&l_PlayerPos);
                            l_Target->GetPosition(&l_TargetPos);

                            l_Player->NearTeleportTo(l_TargetPos.m_positionX, l_TargetPos.m_positionY, l_TargetPos.m_positionZ, l_TargetPos.m_orientation);
                            l_Target->NearTeleportTo(l_PlayerPos.m_positionX, l_PlayerPos.m_positionY, l_PlayerPos.m_positionZ, l_PlayerPos.m_orientation);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_matter_swap_AuraScript::HandleEffectUpdate, EFFECT_1, SPELL_AURA_DUMMY);
                OnDispel += AuraDispelFn(spell_matter_swap_AuraScript::HandleDispel);
                OnEffectRemove += AuraEffectRemoveFn(spell_matter_swap_AuraScript::OnRemove, EFFECT_1, SPELL_AURA_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_matter_swap_AuraScript();
        }
};

// Targeted: Matter Swap - 139919
class spell_targeted_matter_swap : public SpellScriptLoader
{
    public:
        spell_targeted_matter_swap() : SpellScriptLoader("spell_targeted_matter_swap") { }

        class spell_targeted_matter_swap_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_targeted_matter_swap_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;
                
                p_Targets.sort(JadeCore::WorldObjectDistanceCompareOrderPred(GetCaster(), true));
                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_targeted_matter_swap_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_targeted_matter_swap_SpellScript();
        }
};

// Anima Ring - 136954
class spell_anima_ring : public SpellScriptLoader
{
    public:
        spell_anima_ring() : SpellScriptLoader("spell_anima_ring") { }

        class spell_anima_ring_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_anima_ring_SpellScript);

            float m_Angle;

            bool Load()
            {
                m_Angle = 0.f;
                return true;
            }

            void HandleOnHit(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        float l_X = l_Target->GetPositionX() + ((6.f) * cos(m_Angle));
                        float l_Y = l_Target->GetPositionY() + ((6.f) * sin(m_Angle));

                        l_Target->CastSpell(l_X, l_Y, l_Target->GetPositionZ(), g_AnimaRingSpells[p_EffIndex], true, NULL, nullptr, l_Caster->GetGUID());

                        m_Angle += (M_PI * 2.f) / 12.f;
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_1, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_2, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_3, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_4, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_5, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_6, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_7, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_8, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_9, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_10, SPELL_EFFECT_TRIGGER_SPELL);
                OnEffectHitTarget += SpellEffectFn(spell_anima_ring_SpellScript::HandleOnHit, EFFECT_11, SPELL_EFFECT_TRIGGER_SPELL);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_anima_ring_SpellScript();
        }
};

// Activation Sequence - 139537
class spell_activation_sequence : public SpellScriptLoader
{
    public:
        spell_activation_sequence() : SpellScriptLoader("spell_activation_sequence") {}

        class spell_activation_sequence_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_activation_sequence_AuraScript);

            void OnRemove(AuraEffect const* p_AurEff, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Creature* l_DarkAnimus = GetTarget()->ToCreature())
                    l_DarkAnimus->AI()->DoAction(ACTION_ACTIVATE_GOLEM);
            }

            void Register()
            {
                AfterEffectRemove += AuraEffectRemoveFn(spell_activation_sequence_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_activation_sequence_AuraScript();
        }
};

// Siphon Anima - 138644
class spell_siphon_anima : public SpellScriptLoader
{
    public:
        spell_siphon_anima() : SpellScriptLoader("spell_siphon_anima") { }

        class spell_siphon_anima_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_siphon_anima_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (!p_Object || !p_Object->ToCreature())
                        return true;

                    if (!p_Object->ToCreature()->isAlive())
                        return true;

                    if (p_Object->ToUnit()->GetPower(POWER_ENERGY) <= 0)
                        return true;

                    if (p_Object->GetEntry() == GetCaster()->GetEntry())
                        return true;

                    return false;
                });

                Unit* l_Caster = GetCaster();
                if (!l_Caster)
                    return;

                for (WorldObject* l_Target : p_Targets)
                {
                    if (Creature* l_Golem = l_Target->ToCreature())
                    {
                        if (l_Golem->GetPower(POWER_ENERGY) == 1)
                            l_Caster->Kill(l_Golem);
                    }
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_siphon_anima_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_siphon_anima_SpellScript();
        }
};

// Touch of the Animus - 138659
class spell_touch_of_the_animus : public SpellScriptLoader
{
    public:
        spell_touch_of_the_animus() : SpellScriptLoader("spell_touch_of_the_animus") { }

        class spell_touch_of_the_animus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_touch_of_the_animus_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_touch_of_the_animus_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_touch_of_the_animus_SpellScript();
        }
};

// Anima Font - 138691
class spell_anima_font : public SpellScriptLoader
{
    public:
        spell_anima_font() : SpellScriptLoader("spell_anima_font") { }

        class spell_anima_font_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_anima_font_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_anima_font_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_anima_font_SpellScript();
        }
};

// Anima Font (Dummy - trigger missile) - 138694
class spell_anima_font_dummy : public SpellScriptLoader
{
    public:
        spell_anima_font_dummy() : SpellScriptLoader("spell_anima_font_dummy") { }

        class spell_anima_font_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_anima_font_dummy_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                JadeCore::RandomResizeList(p_Targets, 1);
            }

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SPELL_ANIMA_FONT_MISSILE, true);
                }
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_anima_font_dummy_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ALLY);
                OnEffectHitTarget += SpellEffectFn(spell_anima_font_dummy_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_anima_font_dummy_SpellScript();
        }
};

// FULL POWER - 138734
class spell_full_power : public SpellScriptLoader
{
    public:
        spell_full_power() : SpellScriptLoader("spell_full_power") { }

        class spell_full_power_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_full_power_SpellScript);

            void HandleDummy(SpellEffIndex p_EffIndex)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SPELL_FULL_POWER_MISSILE, true);
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_full_power_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_full_power_SpellScript();
        }
};

// Empower Golem - 138780
class spell_empower_golem : public SpellScriptLoader
{
    public:
        spell_empower_golem() : SpellScriptLoader("spell_empower_golem") { }

        class spell_empower_golem_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_empower_golem_SpellScript);

            void CorrectTargets(std::list<WorldObject*>& p_Targets)
            {
                if (p_Targets.empty())
                    return;

                p_Targets.remove_if([this](WorldObject* p_Object) -> bool
                {
                    if (!p_Object || !p_Object->ToCreature())
                        return true;

                    if (!p_Object->ToCreature()->isAlive())
                        return true;

                    if (p_Object->GetEntry() == NPC_ANIMA_GOLEM ||
                        p_Object->GetEntry() == NPC_LARGE_ANIMA_GOLEM ||
                        p_Object->GetEntry() == NPC_MASSIVE_ANIMA_GOLEM)
                        return false;

                    return false;
                });

                if (p_Targets.empty())
                    return;

                std::list<Unit*> l_TempList;
                for (WorldObject* l_Object : p_Targets)
                    l_TempList.push_back(l_Object->ToUnit());

                l_TempList.sort(JadeCore::HealthPctOrderPred());
                WorldObject* l_Object = (*l_TempList.begin());
                p_Targets.clear();
                p_Targets.push_back(l_Object);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_empower_golem_SpellScript::CorrectTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_empower_golem_SpellScript::CorrectTargets, EFFECT_1, TARGET_UNIT_SRC_AREA_ENTRY);
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_empower_golem_SpellScript::CorrectTargets, EFFECT_2, TARGET_UNIT_SRC_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_empower_golem_SpellScript();
        }
};

/// Created by spells from 136955 to 136960 and from 138671 to 138675
class at_anima_ring : public AreaTriggerEntityScript
{
    public:
        at_anima_ring() : AreaTriggerEntityScript("at_anima_ring") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 p_Time)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            std::list<Unit*> l_TargetList;
            float l_Radius = 0.9f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            for (Unit* l_Unit : l_TargetList)
            {
                if (l_Unit->GetExactDist2d(p_AreaTrigger) > l_Radius)
                    continue;

                l_Caster->CastSpell(l_Unit, SPELL_ANIMA_RING_DEBUFF, true);
                p_AreaTrigger->SetDuration(0);
                return;
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_anima_ring();
        }
};

void AddSC_boss_dark_animus()
{
    new boss_dark_animus();             ///< 69427
    new mob_anima_orb();                ///< 69756
    new mob_dark_ritualist();           ///< 69702
    new mob_anima_golem();              ///< 69701
    new mob_large_anima_golem();        ///< 69700
    new mob_massive_anima_golem();      ///< 69699
    new mob_crimson_wake();             ///< 69951
    new spell_long_ritual_lightning();  ///< 137994
    new spell_transfusion_aoe();        ///< 138118
    new spell_transfusion_searcher();   ///< 138378
    new spell_custom_transfusion();     ///< 138909
    new spell_crimson_wake_slow();      ///< 138482
    new spell_crimson_wake_medium();    ///< 138484
    new spell_fixated();                ///< 138486
    new spell_matter_swap();            ///< 138609
    new spell_targeted_matter_swap();   ///< 139919
    new spell_anima_ring();             ///< 136954
    new spell_activation_sequence();    ///< 139537
    new spell_siphon_anima();           ///< 138644
    new spell_touch_of_the_animus();    ///< 138659
    new spell_anima_font();             ///< 138691
    new spell_anima_font_dummy();       ///< 138694
    new spell_full_power();             ///< 138734
    new spell_empower_golem();          ///< 138780
    new at_anima_ring();                ///< from 136955 to 136960 and from 138671 to 138675
}
