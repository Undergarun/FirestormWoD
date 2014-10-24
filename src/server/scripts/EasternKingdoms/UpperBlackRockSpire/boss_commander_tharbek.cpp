/*
* Copyright (C) 2012-2014 JadeCore <http://www.pandashan.com>
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

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "upper_blackrock_spire.h"
#include "Vehicle.h"

enum eSpells
{
    // Ironbarb Skyreaver
    SPELL_INCINERATING_BREATH_DUMMY = 161882,
    SPELL_INCINERATING_BREATH       = 161883,
    SPELL_NOXIUS_SPIT_SEARCHER      = 161811,
    SPELL_NOXIUS_SPIT_MISSILE       = 161824
};

enum eEvents
{
    // Ironbarb Skyreaver
    EVENT_INCINERATING_BREATH   = 1,
    EVENT_NOXIOUS_SPIT
};

enum eSays
{
    TALK_WAVE1_01,
    TALK_WAVE1_02,
    TALK_WAVE2_01,
    TALK_WAVE2_02,
    TALK_WAVE3_01,
    TALK_WAVE3_02,
    TALK_WAVE3_03,
    TALK_AGGRO,
    TALK_DEATH,
    TALK_SLAY
};

enum eActions
{
    ACTION_THARBEK_TALK_1,
    ACTION_ZAELA_TALK_1,
    ACTION_ZAELA_TALK_2,
    ACTION_THARBEK_TALK_2,
    ACTION_ZAELA_TALK_3,
    ACTION_THARBEK_TALK_3,
    ACTION_ZAELA_TALK_4,
    ACTION_THARBEK_TALK_4,
    ACTION_ACTIVATE_SPELLS
};

enum eMoves
{
    MOVE_NONE,
    MOVE_LAND,
    MOVE_TO_BOSS,
    MOVE_FLY_SECOND,
    MOVE_FLY_THIRD,
    MOVE_LAST_POS
};

enum eSpawnCounts
{
    MAX_RAGEMAW_SPAWN       = 2,
    MAX_LEADBELCHER_SPAWN   = 2,
    MAX_BERSERKER_SPAWN     = 2,
    MAX_DREADWEAVER_SPAWN   = 2,
    MAX_VETERAN_SPAWN       = 2,
    MAX_VILEMAW_SPAWN       = 24
};

enum ePhases
{
    PHASE_NONE,
    PHASE_WAVE_1,
    PHASE_WAVE_2,
    PHASE_WAVE_3,
    PHASE_BOSS
};

Position const g_RagemawWorgSpawnPos[MAX_RAGEMAW_SPAWN] =
{
    { 201.9718f, -420.6500f, 110.9819f, 2.982781f },
    { 201.5918f, -416.9412f, 110.9783f, 2.982781f }
};

Position const g_BlackIronLeadbelcherSpawnPos[MAX_LEADBELCHER_SPAWN] =
{
    { 206.8586f, -422.6804f, 111.0060f, 2.982781f },
    { 207.1411f, -419.0410f, 111.0102f, 2.982781f }
};

Position const g_BlackIronBerserkerSpawnPos[MAX_BERSERKER_SPAWN] =
{
    { 200.9668f, -420.4635f, 110.9768f, 3.286658f },
    { 200.0462f, -417.1895f, 110.9742f, 3.286658f }
};

Position const g_BlackIronDreadweaverSpawnPos[MAX_DREADWEAVER_SPAWN] =
{
    { 202.0608f, -422.3512f, 110.9811f, 3.038545f },
    { 202.2822f, -418.6975f, 110.9838f, 3.038545f }
};

Position const g_BlackIronEngineerSpawnPos = { 202.2992f, -420.4620f, 110.9838f, 3.205957f };

Position const g_BlackIronVeteranSpawnPos[MAX_VETERAN_SPAWN] =
{
    { 200.8219f, -419.0781f, 110.9755f, 3.205957f },
    { 201.1120f, -421.8300f, 110.9765f, 3.205957f }
};

Position const g_VilemawHatchlingSpawnPos[MAX_VILEMAW_SPAWN] =
{
    { 207.0501f, -412.1474f, 111.0085f, 3.398512f },
    { 202.1562f, -414.1769f, 110.9797f, 3.655612f },
    { 205.6995f, -415.4170f, 111.0026f, 3.398512f },
    { 206.8414f, -408.8590f, 111.0048f, 3.398512f },
    { 202.0319f, -410.5206f, 110.9762f, 3.788048f },
    { 205.7464f, -411.0979f, 110.9997f, 3.398512f },
    { 202.5679f, -414.3772f, 110.9824f, 3.655612f },
    { 206.8433f, -409.9876f, 111.0057f, 3.398512f },
    { 201.5118f, -410.1655f, 110.9728f, 3.788043f },
    { 202.4219f, -409.7433f, 110.9781f, 3.415119f },
    { 206.8244f, -413.6733f, 111.0083f, 3.398512f },
    { 200.7200f, -414.1158f, 110.9719f, 3.655612f },
    { 206.9559f, -432.8058f, 110.9710f, 2.982781f },
    { 199.3375f, -431.8492f, 110.9684f, 2.535856f },
    { 201.6699f, -430.6247f, 110.9719f, 2.535856f },
    { 207.4523f, -427.6798f, 110.9906f, 3.097996f },
    { 203.3738f, -429.3423f, 110.9792f, 2.982781f },
    { 201.4162f, -426.4257f, 110.9747f, 2.982781f },
    { 202.1310f, -429.8545f, 110.9759f, 2.535856f },
    { 199.9797f, -425.3489f, 110.9728f, 2.982781f },
    { 205.9484f, -425.1725f, 111.0003f, 2.878894f },
    { 202.5716f, -424.8698f, 110.9819f, 2.906152f },
    { 204.8136f, -424.6309f, 110.9938f, 2.993758f },
    { 205.7831f, -429.7979f, 110.9796f, 2.982781f }
};

Position const g_IronbarbSpawnPos       = { 202.2760f, -420.2011f, 110.0100f, 3.139839f };
Position const g_IronbarbFlyPos         = { 153.7290f, -435.7477f, 122.3390f, 3.139839f };
Position const g_IronbarbSecondFlyPos   = { 135.4544f, -420.9265f, 123.8489f, 1.710034f };
Position const g_IronbarbThirdFlyPos    = { 177.4848f, -408.3633f, 116.7175f, 6.123974f };
Position const g_IronbarbLastFlyPos     = { 188.2879f, -420.0663f, 110.8817f, 3.145754f };

// Commander Tharbek - 79912
class boss_commander_tharbek : public CreatureScript
{
    public:
        boss_commander_tharbek() : CreatureScript("boss_commander_tharbek") { }

        struct boss_commander_tharbekAI : public BossAI
        {
            boss_commander_tharbekAI(Creature* p_Creature) : BossAI(p_Creature, DATA_COMMANDER_THARBEK)
            {
                m_Instance = p_Creature->GetInstanceScript();

                if (m_Instance)
                    m_Instance->SetBossState(DATA_COMMANDER_THARBEK, TO_BE_DECIDED);

                m_FirstTalkTimer        = 0;
                m_SecondTalkTimer       = 0;
                m_ThirdTalkTimer        = 0;
                m_FourthTalkTimer       = 0;
                m_FifthTalkTimer        = 0;
                m_SixthTalkTimer        = 0;
                m_SeventhTalkTimer      = 0;
                m_Phase                 = PHASE_NONE;
                m_IronbarbSkyreaverGuid = 0;
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            uint32 m_FirstTalkTimer;
            uint32 m_SecondTalkTimer;
            uint32 m_ThirdTalkTimer;
            uint32 m_FourthTalkTimer;
            uint32 m_FifthTalkTimer;
            uint32 m_SixthTalkTimer;
            uint32 m_SeventhTalkTimer;

            uint8 m_Phase;
            std::vector<uint64> m_WaveGuidsContainer;

            uint64 m_IronbarbSkyreaverGuid;

            void Reset()
            {
                _Reset();

                me->ReenableEvadeMode();

                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_DISARMED);

                if (m_Instance)
                    m_Instance->SetBossState(DATA_COMMANDER_THARBEK, NOT_STARTED);

                m_Events.Reset();

                m_Phase = PHASE_BOSS;
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() == TYPEID_PLAYER)
                    Talk(TALK_SLAY);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                _EnterCombat();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_COMMANDER_THARBEK, IN_PROGRESS);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                }

                Talk(TALK_AGGRO);
            }

            void JustDied(Unit* p_Killer)
            {
                _JustDied();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_COMMANDER_THARBEK, DONE);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }

                Talk(TALK_DEATH);
            }

            void JustReachedHome()
            {
                _JustReachedHome();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_COMMANDER_THARBEK, FAIL);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                }
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_THARBEK_TALK_1:
                        m_FirstTalkTimer = 8000;
                        break;
                    case ACTION_THARBEK_TALK_2:
                        m_ThirdTalkTimer = 6000;
                        break;
                    case ACTION_THARBEK_TALK_3:
                        m_FifthTalkTimer = 6000;
                        break;
                    case ACTION_THARBEK_TALK_4:
                        SummonThirdWave();
                        m_SixthTalkTimer = 13000;
                        break;
                    case ACTION_ACTIVATE_SPELLS:
                        // Schedule events here
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                ScheduleFirstTalk(p_Diff);
                ScheduleSecondTalk(p_Diff);
                ScheduleThirdTalk(p_Diff);
                ScheduleFourthTalk(p_Diff);
                ScheduleFifthTalk(p_Diff);
                ScheduleSixthTalk(p_Diff);
                ScheduleSeventhTalk(p_Diff);

                switch (m_Phase)
                {
                    case PHASE_WAVE_1:
                    {
                        if (IsWaveCompleted(p_Diff))
                        {
                            m_Phase = PHASE_NONE;

                            if (m_Instance)
                            {
                                if (Creature* l_Zaela = Creature::GetCreature(*me, m_Instance->GetData64(NPC_WARLORD_ZAELA)))
                                    l_Zaela->AI()->DoAction(ACTION_ZAELA_TALK_2);
                            }
                        }
                        break;
                    }
                    case PHASE_WAVE_2:
                    {
                        if (IsWaveCompleted(p_Diff))
                        {
                            m_Phase = PHASE_NONE;

                            if (m_Instance)
                            {
                                if (Creature* l_Zaela = Creature::GetCreature(*me, m_Instance->GetData64(NPC_WARLORD_ZAELA)))
                                    l_Zaela->AI()->DoAction(ACTION_ZAELA_TALK_3);
                            }
                        }
                        break;
                    }
                    case PHASE_WAVE_3:
                        break;
                    default:
                        break;
                }

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    default:
                        break;
                }
            }

            void ScheduleFirstTalk(const uint32 p_Diff)
            {
                if (!m_FirstTalkTimer)
                    return;

                if (m_FirstTalkTimer <= p_Diff)
                {
                    m_FirstTalkTimer = 0;
                    Talk(TALK_WAVE1_01);
                    m_SecondTalkTimer = 8000;
                }
                else
                    m_FirstTalkTimer -= p_Diff;
            }

            void ScheduleSecondTalk(const uint32 p_Diff)
            {
                if (!m_SecondTalkTimer)
                    return;

                if (m_SecondTalkTimer <= p_Diff)
                {
                    m_SecondTalkTimer = 0;
                    Talk(TALK_WAVE1_02);
                    SummonFirstWave();

                    if (m_Instance)
                    {
                        if (Creature* l_Zaela = Creature::GetCreature(*me, m_Instance->GetData64(NPC_WARLORD_ZAELA)))
                            l_Zaela->AI()->DoAction(ACTION_ZAELA_TALK_1);
                    }
                }
                else
                    m_SecondTalkTimer -= p_Diff;
            }

            void ScheduleThirdTalk(const uint32 p_Diff)
            {
                if (!m_ThirdTalkTimer)
                    return;

                if (m_ThirdTalkTimer <= p_Diff)
                {
                    m_ThirdTalkTimer  = 0;
                    m_FourthTalkTimer = 3000;
                    Talk(TALK_WAVE2_01);
                    SummonSecondWave();
                }
                else
                    m_ThirdTalkTimer -= p_Diff;
            }

            void ScheduleFourthTalk(const uint32 p_Diff)
            {
                if (!m_FourthTalkTimer)
                    return;

                if (m_FourthTalkTimer <= p_Diff)
                {
                    m_FourthTalkTimer = 0;
                    Talk(TALK_WAVE2_02);
                }
                else
                    m_FourthTalkTimer -= p_Diff;
            }

            void ScheduleFifthTalk(const uint32 p_Diff)
            {
                if (!m_FifthTalkTimer)
                    return;

                if (m_FifthTalkTimer <= p_Diff)
                {
                    m_FifthTalkTimer = 0;
                    Talk(TALK_WAVE3_01);

                    if (m_Instance)
                    {
                        if (Creature* l_Zaela = Creature::GetCreature(*me, m_Instance->GetData64(NPC_WARLORD_ZAELA)))
                            l_Zaela->AI()->DoAction(ACTION_ZAELA_TALK_4);
                    }
                }
                else
                    m_FifthTalkTimer -= p_Diff;
            }

            void ScheduleSixthTalk(const uint32 p_Diff)
            {
                if (!m_SixthTalkTimer)
                    return;

                if (m_SixthTalkTimer <= p_Diff)
                {
                    m_SixthTalkTimer = 0;
                    Talk(TALK_WAVE3_02);
                    m_SeventhTalkTimer = 4500;
                }
                else
                    m_SixthTalkTimer -= p_Diff;
            }

            void ScheduleSeventhTalk(const uint32 p_Diff)
            {
                if (!m_SeventhTalkTimer)
                    return;

                if (m_SeventhTalkTimer <= p_Diff)
                {
                    m_SeventhTalkTimer = 0;
                    Talk(TALK_WAVE3_03);

                    if (Creature* l_Ironbarb = me->SummonCreature(NPC_IRONBARB_SKYREAVER, g_IronbarbSpawnPos))
                        m_IronbarbSkyreaverGuid = l_Ironbarb->GetGUID();
                }
                else
                    m_SeventhTalkTimer -= p_Diff;
            }

            bool IsWaveCompleted(const uint32 p_Diff)
            {
                for (uint64 l_Guid : m_WaveGuidsContainer)
                {
                    if (Creature* l_Summon = Creature::GetCreature(*me, l_Guid))
                    {
                        if (l_Summon->isAlive())
                            return false;
                    }
                }

                return true;
            }

            void SummonFirstWave()
            {
                if (Player* l_Player = me->FindNearestPlayer(50.f))
                {
                    if (m_Instance)
                    {
                        if (GameObject* l_SpawnDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(GOB_THARBEK_SPAWN_DOOR)))
                            l_SpawnDoor->Use(me);
                    }

                    Creature* l_Summon = NULL;
                    m_Phase = PHASE_WAVE_1;
                    m_WaveGuidsContainer.resize(9);

                    // First wave : 2 Worgs, 2 Leadbelcher and 5 Hatchlings
                    for (uint8 l_Index = 0; l_Index < MAX_RAGEMAW_SPAWN; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_RAGEMAW_WORG, g_RagemawWorgSpawnPos[l_Index]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }

                    for (uint8 l_Index = 0; l_Index < MAX_LEADBELCHER_SPAWN; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_BLACK_IRON_LEADBELCHER, g_BlackIronLeadbelcherSpawnPos[l_Index]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }

                    for (uint8 l_Index = 0; l_Index < 5; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_VILEMAW_HATCHLING, g_VilemawHatchlingSpawnPos[urand(0, MAX_VILEMAW_SPAWN - 1)]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }
                }
            }

            void SummonSecondWave()
            {
                if (Player* l_Player = me->FindNearestPlayer(50.f))
                {
                    if (m_Instance)
                    {
                        if (GameObject* l_SpawnDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(GOB_THARBEK_SPAWN_DOOR)))
                            l_SpawnDoor->Use(me);
                    }

                    Creature* l_Summon = NULL;
                    m_Phase = PHASE_WAVE_2;
                    m_WaveGuidsContainer.resize(9);

                    // First wave : 2 Berserker, 2 Dreadweaver and 5 Hatchlings
                    for (uint8 l_Index = 0; l_Index < MAX_BERSERKER_SPAWN; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_BLACK_IRON_BERSERKER, g_BlackIronBerserkerSpawnPos[l_Index]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }

                    for (uint8 l_Index = 0; l_Index < MAX_DREADWEAVER_SPAWN; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_BLACK_IRON_DREADWEAVER, g_BlackIronDreadweaverSpawnPos[l_Index]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }

                    for (uint8 l_Index = 0; l_Index < 5; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_VILEMAW_HATCHLING, g_VilemawHatchlingSpawnPos[urand(0, MAX_VILEMAW_SPAWN - 1)]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }
                }
            }

            void SummonThirdWave()
            {
                if (Player* l_Player = me->FindNearestPlayer(50.f))
                {
                    if (m_Instance)
                    {
                        if (GameObject* l_SpawnDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(GOB_THARBEK_SPAWN_DOOR)))
                            l_SpawnDoor->Use(me);
                    }

                    Creature* l_Summon = NULL;
                    m_Phase = PHASE_WAVE_3;
                    m_WaveGuidsContainer.resize(8);

                    // First wave : 1 Engineer, 2 Veterans and 5 Hatchlings
                    if (l_Summon = me->SummonCreature(NPC_BLACK_IRON_ENGINEER, g_BlackIronEngineerSpawnPos))
                    {
                        l_Summon->AI()->AttackStart(l_Player);
                        m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                    }

                    for (uint8 l_Index = 0; l_Index < MAX_VETERAN_SPAWN; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_BLACK_IRON_VETERAN, g_BlackIronVeteranSpawnPos[l_Index]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }

                    for (uint8 l_Index = 0; l_Index < 5; ++l_Index)
                    {
                        if (l_Summon = me->SummonCreature(NPC_VILEMAW_HATCHLING, g_VilemawHatchlingSpawnPos[urand(0, MAX_VILEMAW_SPAWN - 1)]))
                        {
                            l_Summon->AI()->AttackStart(l_Player);
                            m_WaveGuidsContainer.push_back(l_Summon->GetGUID());
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_commander_tharbekAI(p_Creature);
        }
};

// Ironbarb Skyreaver - 80098
class mob_ironbarb_skyreaver : public CreatureScript
{
    public:
        mob_ironbarb_skyreaver() : CreatureScript("mob_ironbarb_skyreaver") { }

        struct mob_ironbarb_skyreaverAI : public ScriptedAI
        {
            mob_ironbarb_skyreaverAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                m_Instance   = p_Creature->GetInstanceScript();
                m_MoveTimer  = 0;
                m_ActualMove = MOVE_NONE;
            }

            InstanceScript* m_Instance;
            Vehicle* m_Vehicle;
            EventMap m_Events;

            bool m_BossOut;

            uint32 m_MoveTimer;
            uint8  m_ActualMove;

            void Reset()
            {
                me->RemoveAllAreasTrigger();

                m_BossOut = false;

                m_Events.Reset();
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_MoveTimer  = 1000;
                m_ActualMove = MOVE_NONE;

                me->SetReactState(REACT_PASSIVE);

                if (m_Instance)
                {
                    if (GameObject* l_SpawnDoor = GameObject::GetGameObject(*me, m_Instance->GetData64(GOB_THARBEK_SPAWN_DOOR)))
                        l_SpawnDoor->Use(me);
                }
            }

            void MovementInform(uint32 p_Type, uint32 p_ID)
            {
                if (p_Type != POINT_MOTION_TYPE)
                    return;

                switch (p_ID)
                {
                    case MOVE_LAND:
                    case MOVE_FLY_SECOND:
                    case MOVE_FLY_THIRD:
                        m_MoveTimer = 300;
                        break;
                    case MOVE_TO_BOSS:
                    {
                        if (m_Instance)
                        {
                            if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(NPC_COMMANDER_THARBEK)))
                            {
                                l_Tharbek->EnterVehicle(me);
                                l_Tharbek->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                            }
                        }

                        m_MoveTimer = 300;
                        break;
                    }
                    case MOVE_LAST_POS:
                    {
                        me->SetCanFly(false);
                        me->RemoveUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
                        me->SetOrientation(M_PI);

                        Position l_Pos;
                        me->GetPosition(&l_Pos);
                        me->SetHomePosition(l_Pos);
                        m_Events.Reset();
                        me->SetReactState(REACT_AGGRESSIVE);
                        break;
                    }
                    default:
                        break;
                }
            }

            void EnterCombat(Unit* p_Attacker)
            {
                m_Events.ScheduleEvent(EVENT_INCINERATING_BREATH, 15000);
                m_Events.ScheduleEvent(EVENT_NOXIOUS_SPIT, 5000);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage)
            {
                if (!m_BossOut && me->HealthBelowPctDamaged(25, p_Damage))
                {
                    m_BossOut = true;
                    m_Vehicle->RemoveAllPassengers();

                    if (Creature* l_Tharbek = Creature::GetCreature(*me, m_Instance->GetData64(NPC_COMMANDER_THARBEK)))
                    {
                        l_Tharbek->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                        l_Tharbek->AI()->AttackStart(p_Attacker);
                        l_Tharbek->AI()->DoAction(ACTION_ACTIVATE_SPELLS);
                    }
                }
            }

            void SpellHitTarget(Unit* p_Target, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id == SPELL_NOXIUS_SPIT_SEARCHER)
                    me->CastSpell(p_Target, SPELL_NOXIUS_SPIT_MISSILE, true);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                ScheduleMovement(p_Diff);

                if (!UpdateVictim())
                    return;

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_INCINERATING_BREATH:
                        me->CastSpell(me, SPELL_INCINERATING_BREATH_DUMMY, true);
                        me->CastSpell(me, SPELL_INCINERATING_BREATH, false);
                        m_Events.ScheduleEvent(EVENT_INCINERATING_BREATH, 30000);
                        break;
                    case EVENT_NOXIOUS_SPIT:
                        me->CastSpell(me, SPELL_NOXIUS_SPIT_SEARCHER, true);
                        m_Events.ScheduleEvent(EVENT_INCINERATING_BREATH, 20000);
                        break;
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            void ScheduleMovement(const uint32 p_Diff)
            {
                if (!m_MoveTimer)
                    return;

                if (m_MoveTimer <= p_Diff)
                {
                    m_MoveTimer = 0;

                    switch (m_ActualMove)
                    {
                        case MOVE_NONE:
                        {
                            float l_X = me->GetPositionX() + (25.f * cos(me->GetOrientation()));
                            float l_Y = me->GetPositionY() + (25.f * sin(me->GetOrientation()));

                            m_ActualMove = MOVE_LAND;
                            me->GetMotionMaster()->MovePoint(m_ActualMove, l_X, l_Y, me->GetPositionZ());
                            break;
                        }
                        case MOVE_LAND:
                        {
                            me->SetCanFly(true);
                            me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);
                            m_ActualMove = MOVE_TO_BOSS;
                            me->GetMotionMaster()->MovePoint(m_ActualMove, g_IronbarbFlyPos);
                            break;
                        }
                        case MOVE_TO_BOSS:
                        {
                            m_ActualMove = MOVE_FLY_SECOND;
                            me->GetMotionMaster()->MovePoint(m_ActualMove, g_IronbarbSecondFlyPos);
                            break;
                        }
                        case MOVE_FLY_SECOND:
                        {
                            m_ActualMove = MOVE_FLY_THIRD;
                            me->GetMotionMaster()->MovePoint(m_ActualMove, g_IronbarbThirdFlyPos);
                            break;
                        }
                        case MOVE_FLY_THIRD:
                        {
                            m_ActualMove = MOVE_LAST_POS;
                            me->GetMotionMaster()->MovePoint(m_ActualMove, g_IronbarbLastFlyPos);
                            break;
                        }
                        default:
                            break;
                    }
                }
                else
                    m_MoveTimer -= p_Diff;
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_ironbarb_skyreaverAI(p_Creature);
        }
};

void AddSC_boss_commander_tharbek()
{
    new boss_commander_tharbek();
    new mob_ironbarb_skyreaver();
}
