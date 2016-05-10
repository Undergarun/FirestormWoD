////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "ObjectMgr.h"
#include "Vehicle.h"
#include "throne_of_thunder.h"
#include "MoveSplineInit.h"
#include "GameObjectAI.h"

enum eSpells
{
    /// Intro
    SPELL_FORCE_TO_JUMP                     = 138359,

    /// Ji-Kun
    SPELL_TALON_RAKE                        = 134366,
    SPELL_CAW_FIRST                         = 138923,
    SPELL_CAW_MISSILE                       = 138926,
    SPELL_QUILLS                            = 134380,
    SPELL_INFECTED_TALONS_PROC              = 140094,
    SPELL_INFECTED_TALONS_AURA              = 140092,
    SPELL_DOWN_DRAFT                        = 134370,
    SPELL_SAFETY_NET_TRIGGER                = 136524,
    SPELL_SLIMED                            = 134256,
    SPELL_PRIMAL_NUTRIMENT                  = 140741,
    /// Nests (with no effect : 138360  - 139286)
    SPELL_INCUBATE                          = 137526, ///< Red beam on activated nest
    SPELL_INCUBATED                         = 134347, ///< Incubated young egg (68194), turns into hatchling (68192) 10s after
    SPELL_LAY_EGG                           = 134367, ///< Summons an egg (68202)
    SPELL_FEED_YOUNG                        = 137528, ///< Casting 2s, triggers 2x 134385 (to script)
    SPELL_HATCH                             = 137534, ///< Spawn Juvenile (69836)
    SPELL_DROP_FEED_POOL                    = 138209, ///< Shows exploding green pool - triggers 134259 (summons 68188)
    SPELL_FEED_POOL_PERIODIC_DMG            = 138319, ///< aura, 35k/s
    SPELL_MOVE_JUMP_TO_TARGET               = 138359, ///< Jump to target
    SPELL_PULL_TARGET_TO_OWN_POSITION       = 138406, ///< Pulls target
    SPELL_FEED_SPAWN_SPELL                  = 138840, ///< Spawns green blob on caster
    SPELL_FEED_POOL_SPAWN_SPELL_GREEN       = 138854, ///< Shows giant green pool at caster's feet
    SPELL_FEED_JUMPS_DOWN_TO_HATCHLING      = 138904, ///< Jumps + triggers 139285
    SPELL_JUMP_TO_TARGET_ABOVE_HATCHLING    = 138907, ///< Jumps to selected target
    SPELL_BEAM_TO_JUMP_TARGET_NON_HATCHLING = 138914, ///< Green
    SPELL_BEAM_TO_JUMP_TARGET_HATCHLING     = 138915, ///< Yellow
    SPELL_FEED_APPEARANCE_SPELL             = 138918, ///< Exploding yellow egg visual
    SPELL_FEED_POOL_SPAWN                   = 139020, ///< Shows exploding green pool + spawns feed pool (68188)
    SPELL_NEST_GUARDIAN_SUMMON              = 139090, ///< Spawns Nest Guardian
    SPELL_SPAWN_JI_KUN_HATCHLING            = 139148, ///< Spawns Ji-Kun Hatchling (70144)
    SPELL_FEED_POOL_SPAWN_SPELL_YELLOW      = 139284, ///< Shows yellow pool at caster's feet
    SPELL_SUMMON_FEED_POOL                  = 140578, ///< Shows exploding yellow pool
    SPELL_SUMMON_FEED                       = 134385, ///< Summons Feed (68178)
    SPELL_CHEEP_HATCHLING                   = 139296, ///< Replace MeleeAttack for Hatchling
    SPELL_CHEEP_FLEDGLING                   = 140570,
    SPELL_CHEEP_JUVENILE                    = 140129,
    SPELL_EAT                               = 134321, ///< Cast by Hatchling when Feed pool is near
    SPELL_MORPH                             = 134322, ///< If hatchling succeeds to eat (is alive at SPELL_EAT's end), it triggers Morph to turn into Fledgling

    /// Feathers
    SPELL_DROP_FEATHERS                     = 134338, ///< Create areatrigger + visual for spawning feathers
    SPELL_DROP_FEATHERS_GOB                 = 140016, ///< Spawn feater GOB
        /// Using feathers in combat (by killing hatchlings) : 4 stacks of Daedalian wings, each allows player to fly for 10 secs, and there's lesson of Icarus debuff
        /// which prevent player to take another feather for 1 minute (also, players can't have more than 4 stacks of Daedalian wings, even if debuff is gone).
    SPELL_DAEDALIAN_WINGS                   = 134339, ///< Give the ability to fly to player - 10 secs
    SPELL_FLIGHT                            = 133755, ///< Flight spell for players - 10 secs
    SPELL_LESSON_OF_ICARUS                  = 140571, ///< Prevent players from catching a new feather
        /// Using feathers when Ji-Kun's dead (on Ji-Kun's platform) : 1 stack of Daedalian wings, which allows player to fly for 1 minute, without any debuff (but
        /// player can't pick a new feather while he's flying or having Daedalian wings)
    SPELL_DAEDALIAN_WINGS_LONG              = 140014, ///< Give the ability to fly to player - 1 minute
    SPELL_FLIGHT_LONG                       = 140013, ///< Flight spell for players - 1 min

    /// Falling
    SPELL_CATCH_FALL                        = 85282,  ///< Falling player ride vehicle (69839 - Fall Catcher)
    SPELL_EJECT_ALL_PASSENGERS              = 68576,  ///< Fall catcher eject passengers
    /// Exit
    SPELL_EXIT_CHAMBER                      = 141014,
    /// Heroic mode
    SPELL_TALON_STRIKE                      = 139100,
    SPELL_SCREECH                           = 140640,
    SPELL_SCREECH_REDUCE_CASTING_SPEED      = 134372
};

enum eEvents
{
    EVENT_TALON_RAKE = 1,
    EVENT_CAW,
    EVENT_QUILLS,
    EVENT_DOWN_DRAFT,
    EVENT_FEED_YOUNG,
    EVENT_TURN_INTO_FLEDGLING,
    EVENT_ACTIVATE_NEST,
    EVENT_HATCH,
    EVENT_CHEEP,
    EVENT_LAYING,
    EVENT_FLIGHT,
    EVENT_FEED_ACTIVATE,
    EVENT_FEEDPOOL_CHECK_PLAYERS,
    EVENT_EVOLVE,
    EVENT_TALON_STRIKE,
    EVENT_SCREECH,
    EVENT_REACTIVE_FALL_CATCHER
};

enum eActions
{
    ACTION_SUMMON_FEED = 1,
    ACTION_ACTIVATE_NEST,
    ACTION_INCUBATE_EGG,
    ACTION_HATCH,
    ACTION_FEED_EATEN,
    ACTION_FEEDPOOL_DESPAWN,
    ACTION_TAKE_FEED,
    ACTION_INCUBATER_WIPE
};

enum eTypes
{
    TYPE_GET_NEST_ID,
    TYPE_IS_HATCHLING,
    TYPE_SET_FEED_STATUS,
    TYPE_GET_FEEDPOOL_STATUS,
    TYPE_SET_FEEDPOOL_STATUS,
    TYPE_SET_FALL_CATCHER_PLAYER_GUID,
    TYPE_GET_FALL_CATCHER_TYPE
};

enum eFallCatcherTypes
{
    FALL_CATCHER_MAIN,
    FALL_CATCHER_TRANSPORTER
};

Position const waypointPos[52] =
{
    { 6211.530f, 4352.063f, -41.23960f, 0.0f },
    { 6219.918f, 4323.225f, -50.82288f, 0.0f },
    { 6200.393f, 4290.322f, -58.99046f, 0.0f },
    { 6167.124f, 4255.000f, -71.33135f, 0.0f },
    { 6130.888f, 4247.688f, -77.82609f, 0.0f },
    { 6094.024f, 4258.713f, -85.37042f, 0.0f },
    { 6080.453f, 4294.052f, -87.51177f, 0.0f },
    { 6083.603f, 4316.073f, -83.81752f, 0.0f },
    { 6091.601f, 4337.876f, -79.24355f, 0.0f },
    { 6115.493f, 4360.595f, -71.60667f, 0.0f },
    { 6143.307f, 4364.279f, -64.42132f, 0.0f },
    { 6170.492f, 4353.606f, -61.10223f, 0.0f },
    { 6189.739f, 4341.566f, -57.74512f, 0.0f },
    { 6203.971f, 4317.918f, -49.33831f, 0.0f },
    { 6206.109f, 4289.632f, -44.60070f, 0.0f },
    { 6189.743f, 4265.107f, -39.40984f, 0.0f },
    { 6163.300f, 4250.018f, -32.56945f, 0.0f },
    { 6136.179f, 4247.880f, -25.26046f, 0.0f },
    { 6093.636f, 4266.221f, -8.476698f, 0.0f },
    { 6075.295f, 4302.147f, 9.5958010f, 0.0f },
    { 6086.486f, 4356.149f, 34.423870f, 0.0f },
    { 6135.314f, 4386.748f, 58.916970f, 0.0f },
    { 6174.528f, 4380.014f, 65.794550f, 0.0f },
    { 6208.828f, 4352.507f, 70.634090f, 0.0f },
    { 6215.530f, 4317.869f, 71.754340f, 0.0f },
    { 6198.188f, 4279.812f, 75.545790f, 0.0f },
    { 6149.543f, 4285.440f, 75.545790f, 0.0f },
    { 6132.934f, 4319.959f, 75.545790f, 0.0f },
    { 6164.056f, 4341.441f, 74.958320f, 0.0f },
    { 6192.802f, 4302.389f, 72.754180f, 0.0f },
    { 6180.481f, 4254.817f, 67.915810f, 0.0f },
    { 6135.318f, 4246.200f, 61.224780f, 0.0f },
    { 6086.058f, 4267.385f, 53.709340f, 0.0f },
    { 6075.467f, 4313.128f, 51.114910f, 0.0f },
    { 6105.487f, 4349.319f, 43.703910f, 0.0f },
    { 6141.576f, 4359.033f, 34.561810f, 0.0f },
    { 6167.522f, 4340.946f, 27.807100f, 0.0f },
    { 6165.942f, 4305.924f, 20.013400f, 0.0f },
    { 6142.460f, 4289.088f, 9.9207350f, 0.0f },
    { 6121.972f, 4307.348f, -4.077662f, 0.0f },
    { 6130.896f, 4333.039f, -10.56661f, 0.0f },
    { 6155.038f, 4336.327f, -13.13866f, 0.0f },
    { 6169.749f, 4320.901f, -14.18135f, 0.0f },
    { 6174.592f, 4306.330f, -15.45293f, 0.0f },
    { 6172.712f, 4289.999f, -17.64817f, 0.0f },
    { 6162.471f, 4278.408f, -20.11584f, 0.0f },
    { 6151.760f, 4278.437f, -21.28869f, 0.0f },
    { 6144.905f, 4284.731f, -22.05752f, 0.0f },
    { 6142.729f, 4294.742f, -24.24618f, 0.0f },
    { 6145.100f, 4315.314f, -26.73017f, 0.0f },
    { 6148.108f, 4324.893f, -25.96644f, 0.0f },
    { 6160.587f, 4354.137f, -24.45477f, 0.0f }
};

Position const g_BossPlatformPos = { 6112.219f, 4285.634f, -30.04051f, 0.0f };
Position const g_BossPos = { 6146.1f, 4318.6f, -31.7793f, 6.16672f };

/// Activation order - change if 10 players or 25 players. 2 is for Heroic Mode, if the activation summons a Nest Guardian (70134)
int const g_ActivationOrder10[11][2] =
{
    { 1, 0 }, ///< Activations 0-2 activate a lower nest each
    { 2, 0 },
    { 1, 0 },
    { 0, 2 }, ///< Activation 3-5 activate an upper nest each
    { 0, 1 },
    { 1, 0 },
    { 2, 0 }, ///< Activation 6-7 activate a lower nest each
    { 1, 0 },
    { 1, 2 }, ///< Activation 8 activates both a lower and an upper nest
    { 0, 1 }, // Activation 9-10 activate a upper nest each
    { 0, 2 }
};

int const g_ActivationOrder25[12][2] =
{
    { 1, 0 }, ///< Activations 0-3 activate a lower nest each
    { 2, 0 },
    { 1, 0 },
    { 1, 0 },
    { 1, 2 }, ///< Activation 4 activates both a lower and an upper nest
    { 0, 1 }, ///< Activations 5-6 activate an upper nest each
    { 0, 1 },
    { 1, 1 }, ///< Activations 7-8 activate both a lower and an upper nest each
    { 1, 2 },
    { 1, 0 }, ///< Activations 9-10 activate a lower nest each
    { 1, 0 },
    { 1, 1 }, ///< Activations 11-12 activate both a lower and an upper nest each
};

enum eActivationLengths
{
    ACTIVATION_LENGTH_10 = 11,
    ACTIVATION_LENGTH_25 = 12
};

Position const g_LowerNestPos[5] =
{
    { 6192.77f, 4267.67f,  -70.79f, 0.0f },
    { 6070.82f, 4284.69f, -101.62f, 0.0f },
    { 6096.32f, 4338.76f,  -93.88f, 0.0f },
    { 6159.26f, 4371.19f,  -70.80f, 0.0f },
    { 6219.60f, 4333.81f,  -59.09f, 0.0f }
};

Position const g_UpperNestPos[5] =
{
    { 6173.00f, 4238.90f, 41.07f, 0.0f },
    { 6077.58f, 4270.47f, 37.64f, 0.0f },
    { 6081.75f, 4372.00f, 43.45f, 0.0f },
    { 6152.44f, 4330.51f, 69.82f, 0.0f },
    { 6217.60f, 4353.01f, 66.18f, 0.0f }
};

Position const g_FeatherPos[5] =
{
    { 6125.65f, 4355.60f, -31.8626f, 5.25605f },
    { 6110.11f, 4296.95f, -31.8626f, 0.49317f },
    { 6169.00f, 4281.05f, -31.8626f, 2.10022f },
    { 6184.19f, 4339.61f, -31.8627f, 3.66889f },
    { 6084.16f, 4428.31f, -119.633f, 5.20972f }
};

void DespawnJiKunAdds(Creature* me)
{
    uint8 const l_MaxEntryToDespawn = 11;
    uint32 l_addEntries[l_MaxEntryToDespawn] =
    {
        NPC_HATCHLING,
        NPC_FEED,
        NPC_FEED_POOL,
        NPC_JUVENILE,
        NPC_YOUNG_EGG_OF_JI_KUN,
        NPC_MATURE_EGG_OF_JI_KUN,
        NPC_FLEGLING_JUVENILE,
        NPC_INCUBATER,
        NPC_JI_KUN_FLEDGLING_S_EGG,
        NPC_NEST_GUARDIAN,
        NPC_JI_KUN_HATCHLING
    };

    std::list<Creature*> l_addList;
    for (uint8 l_Idx = 0; l_Idx < l_MaxEntryToDespawn; ++l_Idx)
    {
        l_addList.clear();
        GetCreatureListWithEntryInGrid(l_addList, me, l_addEntries[l_Idx], 200.0f);

        for (Creature* l_Add : l_addList)
        {
            if (l_Add->GetEntry() != NPC_INCUBATER)
                l_Add->DespawnOrUnsummon();
            else
                l_Add->AI()->DoAction(ACTION_INCUBATER_WIPE);
        }
    }
}

/// Ji-Kun - 69712
class boss_ji_kun : public CreatureScript
{
    public:
        boss_ji_kun() : CreatureScript("boss_ji_kun") { }

        enum eMisc
        {
            LFRToTSecondPart = 611
        };

        struct boss_ji_kunAI : public BossAI
        {
            boss_ji_kunAI(Creature* p_Creature) : BossAI(p_Creature, DATA_JI_KUN)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Activated = false;
                m_ActualWaypoint = 0;
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            bool m_Activated;
            uint32 m_ActualWaypoint;
            uint32 m_LowerNest;
            uint32 m_UpperNest;
            uint32 m_NestActivationCount;       ///< For cycle
            uint32 m_TotalNestActivationCount;  ///< Total count for HM mode
            uint32 m_NestTimer;

            void InitializeAI()
            {
                if (!me->isAlive())
                    SpawnExitFeathers();
                else
                    Reset();
            }

            void SpawnExitFeathers()
            {
                for (uint8 l_Idx = 0; l_Idx < 5; ++l_Idx)
                    me->SummonCreature(NPC_EXIT_CHAMBER, g_FeatherPos[l_Idx].GetPositionX(), g_FeatherPos[l_Idx].GetPositionY(), g_FeatherPos[l_Idx].GetPositionZ());
            }

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                m_UpperNest = 0;  ///< Index of first lower nest to be activated
                m_LowerNest = 0;  ///< Index of first upper nest to be activated
                m_NestActivationCount = 0;
                m_TotalNestActivationCount = 0;
                m_NestTimer = 0;

                me->CastSpell(me, SPELL_INFECTED_TALONS_PROC, true);
                me->ReenableEvadeMode();
                me->AddUnitState(UNIT_STATE_ROOT);

                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SLIMED);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEED_POOL_PERIODIC_DMG);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCREECH);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LESSON_OF_ICARUS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFECTED_TALONS_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TALON_RAKE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_QUILLS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCREECH_REDUCE_CASTING_SPEED);

                    if (m_Instance->GetBossState(DATA_JI_KUN) !=  DONE)
                    {
                        m_Instance->SetBossState(DATA_JI_KUN, NOT_STARTED);
                        me->SetReactState(REACT_DEFENSIVE);
                    }
                }

//                 if (!m_Activated)
//                 {
//                     me->GetMotionMaster()->Clear();
//                     me->GetMotionMaster()->MovePoint(m_ActualWaypoint, waypointPos[0]);
//                 }

                std::list<Creature*> l_HatchlingsList;
                GetCreatureListWithEntryInGrid(l_HatchlingsList, me, 68192, 200.0f);

                for (Creature* l_Hatchling : l_HatchlingsList)
                    l_Hatchling->DespawnOrUnsummon();
            }

            Creature* GetIncubater(std::list<Creature*> p_IncubaterList, uint32 p_NestOrder)
            {
                for (Creature* l_Incubater : p_IncubaterList)
                {
                    if (l_Incubater)
                    {
                        if (l_Incubater->AI()->GetData(TYPE_GET_NEST_ID) == p_NestOrder)
                            return l_Incubater;
                    }
                }

                return 0;
            }

            void ActivateNest()
            {
                /// Retrieving nest;
                std::list<Creature*> l_IncubaterList;
                GetCreatureListWithEntryInGrid(l_IncubaterList, me, NPC_INCUBATER, 200.0f);

                /// 10 Man cycle
                if (!Is25ManRaid())
                {
                    /// lower nest
                    if (g_ActivationOrder10[m_NestActivationCount][0])
                    {
                        if (Creature* l_Incubater = GetIncubater(l_IncubaterList, m_LowerNest))
                        {
                            l_Incubater->AI()->DoAction(ACTION_ACTIVATE_NEST);
                            if (IsHeroic() && g_ActivationOrder10[m_NestActivationCount][0] == 2)
                                l_Incubater->CastSpell(l_Incubater, SPELL_NEST_GUARDIAN_SUMMON, false);
                        }
                    }
    
                    /// upper nest (id = order + 10)
                    if (g_ActivationOrder10[m_NestActivationCount][1])
                    {
                        if (Creature* l_Incubater = GetIncubater(l_IncubaterList, m_UpperNest + 10))
                        {
                            l_Incubater->AI()->DoAction(ACTION_ACTIVATE_NEST);
                            if (IsHeroic() && g_ActivationOrder10[m_NestActivationCount][1] == 2)
                                l_Incubater->CastSpell(l_Incubater, SPELL_NEST_GUARDIAN_SUMMON, false);
                        }
                    }

                    /// Increase activation count until the cycle ends, then loop
                    m_NestActivationCount = ++m_NestActivationCount % ACTIVATION_LENGTH_10;
                }
                /// 25 Man cycle (including LFR)
                else
                {
                    /// lower nest
                    if (g_ActivationOrder25[m_NestActivationCount][0])
                    {
                        if (Creature* l_Incubater = GetIncubater(l_IncubaterList, m_LowerNest))
                        {
                            l_Incubater->AI()->DoAction(ACTION_ACTIVATE_NEST);
                            if (IsHeroic() && g_ActivationOrder25[m_NestActivationCount][0] == 2)
                                l_Incubater->CastSpell(l_Incubater, SPELL_NEST_GUARDIAN_SUMMON, false);
                        }
                    }

                    /// upper nest (id = order + 10)
                    if (g_ActivationOrder25[m_NestActivationCount][1])
                    {
                        if (Creature* l_Incubater = GetIncubater(l_IncubaterList, m_UpperNest + 10))
                        {
                            l_Incubater->AI()->DoAction(ACTION_ACTIVATE_NEST);
                            if (IsHeroic() && g_ActivationOrder25[m_NestActivationCount][1] == 2)
                                l_Incubater->CastSpell(l_Incubater, SPELL_NEST_GUARDIAN_SUMMON, false);
                        }
                    }

                    /// Increase activation count until the cycle ends, then loop
                    m_NestActivationCount = ++m_NestActivationCount % ACTIVATION_LENGTH_25;
                }

                ///s Set the next lower & upper nests that'll be activated (loop if required)
                m_LowerNest = ++m_LowerNest % 4;
                m_UpperNest = ++m_UpperNest % 4;
            }

            void EnterCombat(Unit* /*p_Attacker*/)
            {
                if (m_Instance)
                {
                    if (!m_Instance->CheckRequiredBosses(DATA_JI_KUN))
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                m_Events.ScheduleEvent(EVENT_TALON_RAKE, 20000);
                m_Events.ScheduleEvent(EVENT_CAW, urand(18000, 50000));
                m_Events.ScheduleEvent(EVENT_QUILLS, 10000);
                m_Events.ScheduleEvent(EVENT_DOWN_DRAFT, 10000);
                m_Events.ScheduleEvent(EVENT_FEED_YOUNG, 30000);
                m_NestTimer = 1000;
            }

            void DamageDealt(Unit* p_Victim, uint32& /*p_Damage*/, DamageEffectType p_Type)
            {
                if (p_Type == DIRECT_DAMAGE)
                {
                    if (Unit* l_Victim = me->getVictim())
                    {
                        if (l_Victim == p_Victim)
                            me->CastSpell(p_Victim, SPELL_INFECTED_TALONS_AURA, true);
                    }
                }
            }

//             void MovementInform(uint32 p_Type, uint32 p_Id)
//             {
//                 if (p_Type != POINT_MOTION_TYPE)
//                     return;
// 
//                 uint32 l_NextId = p_Id < 51 ? p_Id++ : 0;
//                 me->GetMotionMaster()->MovePoint(l_NextId, waypointPos[l_NextId]);
//             }

            void JustDied(Unit* /*p_Killer*/)
            {
                m_Events.Reset();
                summons.DespawnAll();
                _JustDied();

                me->RemoveAllAuras();
                me->RemoveAllDynObjects();
                me->RemoveAllAreasTrigger();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_JI_KUN, DONE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SLIMED);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEED_POOL_PERIODIC_DMG);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCREECH);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LESSON_OF_ICARUS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFECTED_TALONS_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TALON_RAKE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_QUILLS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCREECH_REDUCE_CASTING_SPEED);
                }

                DespawnJiKunAdds(me);

                /// Removing Lesson of Icarus debuff and turning Flight & Daedalian Wings combat auras into Flight & Daeadalian Wings won encounter auras
                Map::PlayerList const &l_PlayerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                    {
                        if (!l_Player->HasAura(SPELL_DAEDALIAN_WINGS) && !l_Player->HasAura(SPELL_FLIGHT) && !l_Player->HasAura(SPELL_LESSON_OF_ICARUS))
                            continue;

                        /// Removing Lesson of Icarus
                        if (l_Player->HasAura(SPELL_LESSON_OF_ICARUS))
                            l_Player->RemoveAura(SPELL_LESSON_OF_ICARUS);

                        /// If player could fly 1 time or more
                        if (l_Player->HasAura(SPELL_DAEDALIAN_WINGS))
                        {
                            /// Removing Daedalian wings
                            l_Player->RemoveAura(SPELL_DAEDALIAN_WINGS);

                            /// If player is currently flying, replacing flying aura, but no need to replace Daedalian wings aura
                            if (l_Player->HasAura(SPELL_FLIGHT))
                            {
                                l_Player->RemoveAura(SPELL_FLIGHT);
                                l_Player->AddAura(SPELL_FLIGHT_LONG, l_Player);
                            }
                            // Player not flying : Daedalian wings need to be replace so player can still fly, but with the new aura
                            else
                                l_Player->AddAura(SPELL_DAEDALIAN_WINGS_LONG, l_Player);
                        }
                        /// Player can't fly more
                        else
                        {
                            /// Player is already flying (ie. the last stack of Daedalian Wings) : replacing flying aura
                            if (l_Player->HasAura(SPELL_FLIGHT))
                            {
                                l_Player->RemoveAura(SPELL_FLIGHT);
                                l_Player->AddAura(SPELL_FLIGHT_LONG, l_Player);
                            }
                        }
                    }
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
                            if (l_DungeonID != eMisc::LFRToTSecondPart)
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
                else
                    SpawnExitFeathers();
            }

            void DoAction(const int32 p_Action)
            {
                if (p_Action == ACTION_SUMMON_FEED)
                {
                    std::list<Creature*> l_HatchlingList;
                    GetCreatureListWithEntryInGrid(l_HatchlingList, me, NPC_HATCHLING, 300.0f);

                    /*
                     * Note: Ji-Kun will summon NPC_FEED. Here, we just check if boss summons Feed for hatchling (and so, how many)
                     * or not (if so, we summon between 3 and 6 Feeds). Then, it's the Feed' script itself which will decides of its
                     * behaviour (coming to hatchling or to platform).
                     */

                    /// Looking for hatchlings to feed
                    uint32 l_FeedCount = 0;
                    for (std::list<Creature*>::iterator itr = l_HatchlingList.begin(); itr != l_HatchlingList.end(); ++itr)
                    {
                        /// We're not feeding dead hatchling or hatchling which has evolved into fledgling or already eating fledgling
                        if (!(*itr)->isAlive() || !(*itr)->AI()->GetData(TYPE_IS_HATCHLING) || (*itr)->HasAura(SPELL_EAT))
                            continue;

                        (*itr)->AI()->DoAction(ACTION_TAKE_FEED);
                        ++l_FeedCount;
                    }
                    /// No Hatchling to feed : feed to random position on boss platform
                    if (!l_FeedCount)
                    {
                        uint8 l_MaxFeed = urand(3, 6);
                        for (uint8 l_Idx = 0; l_Idx < l_MaxFeed; ++l_Idx)
                        {
                            if (Creature* l_Feed = me->SummonCreature(NPC_FEED, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
                                l_Feed->AI()->SetData(TYPE_SET_FEED_STATUS, 0);
                        }
                    }
                }
            }

            void EnterEvadeMode()
            {
                me->RemoveAllAuras();
                me->RemoveAllDynObjects();
                me->RemoveAllAreasTrigger();

                m_Events.Reset();
                summons.DespawnAll();

                DespawnJiKunAdds(me);

                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SLIMED);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FEED_POOL_PERIODIC_DMG);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCREECH);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_LESSON_OF_ICARUS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_INFECTED_TALONS_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_TALON_RAKE);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_QUILLS);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_SCREECH_REDUCE_CASTING_SPEED);
                }

                m_NestActivationCount = 0;
                m_NestTimer = 0;
                m_LowerNest = 0;
                m_UpperNest = 0;

                _EnterEvadeMode();
            }

            void UpdateAI(const uint32 p_Diff)
            {
                /// Catch falling players
                Map::PlayerList const& l_playerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_playerList.begin(); l_Itr != l_playerList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                    {
                        if (!l_Player->HasAura(SPELL_PREVENT_FALL_DAMAGE) && l_Player->IsFalling())
                            l_Player->CastSpell(l_Player, SPELL_PREVENT_FALL_DAMAGE, false);
                    }
                }

                /// Nest activation
                if (m_NestTimer)
                {
                    if (m_NestTimer <= p_Diff)
                    {
                        ActivateNest();
                        m_NestTimer = 30000;
                    }
                    else
                        m_NestTimer -= p_Diff;
                }

                if (!UpdateVictim())
                    return;

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_TALON_RAKE:
                    {
                        DoCastVictim(SPELL_TALON_RAKE);
                        m_Events.ScheduleEvent(EVENT_TALON_RAKE, 60000);
                        break;
                    }
                    case EVENT_CAW:
                    {
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_CAW_FIRST, false);
                        m_Events.ScheduleEvent(EVENT_CAW, 30000);
                        break;
                    }
                    case EVENT_QUILLS:
                    {
                        me->CastSpell(me, SPELL_QUILLS, true);
                        m_Events.ScheduleEvent(EVENT_QUILLS, urand(62500, 67000));
                        break;
                    }
                    case EVENT_DOWN_DRAFT:
                    {
                        me->CastSpell(g_BossPlatformPos.m_positionX, g_BossPlatformPos.m_positionY, g_BossPlatformPos.m_positionZ, SPELL_DOWN_DRAFT, false);
                        m_Events.ScheduleEvent(EVENT_DOWN_DRAFT, 97000);
                        break;
                    }
                    case EVENT_FEED_YOUNG:
                    {
                        me->CastSpell(me, SPELL_SUMMON_FEED, false);
                        m_Events.ScheduleEvent(EVENT_FEED_YOUNG, 30000);
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
            return new boss_ji_kunAI(p_Creature);
        }
};

/// Jump to Boss Platform - 69885
class mob_jump_to_boss_platform : public CreatureScript
{
    public:
        mob_jump_to_boss_platform() : CreatureScript("mob_jump_to_boss_platform") { }

        struct mob_jump_to_boss_platformAI : public ScriptedAI
        {
            mob_jump_to_boss_platformAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_CheckTimer;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);

                m_CheckTimer = 500;
            }

            void UpdateAI(const uint32 diff)
            {
                if (m_CheckTimer)
                {
                    if (m_CheckTimer <= diff)
                    {
                        std::list<Player*> l_PlayerList;
                        me->GetPlayerListInGrid(l_PlayerList, 13.0f);

                        for (Player* l_Player : l_PlayerList)
                            l_Player->CastSpell(g_BossPlatformPos.m_positionX, g_BossPlatformPos.m_positionY, g_BossPlatformPos.m_positionZ, SPELL_FORCE_TO_JUMP, true);

                        m_CheckTimer = 500;
                    }
                    else
                        m_CheckTimer -= diff;   
                }
            }
        };

        CreatureAI* GetAI(Creature* l_Creature) const
        {
            return new mob_jump_to_boss_platformAI(l_Creature);
        }
};

/// Fall Catcher - 69839
class mob_fall_catcher : public CreatureScript
{
    public:
        mob_fall_catcher() : CreatureScript("mob_fall_catcher") { }

        struct mob_fall_catcherAI : public ScriptedAI
        {
            mob_fall_catcherAI(Creature* p_Creature) : ScriptedAI(p_Creature), m_Vehicle(p_Creature->GetVehicleKit())
            {
                ASSERT(m_Vehicle)
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            Vehicle* m_Vehicle;
            uint8 m_Type;
            bool m_IsTransporting;
            float m_LandPosX;
            float m_LandPosY;
            float m_LandOri;

            void Reset()
            {
                m_Events.Reset();
                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);
                me->SetDisableGravity(false);
                m_IsTransporting = false;
                m_Type = FALL_CATCHER_MAIN;

                m_LandPosX = me->GetPositionX() + 20.0f * cos(me->GetOrientation());
                m_LandPosY = me->GetPositionY() + 20.0f * sin(me->GetOrientation());
                m_LandOri  = me->GetOrientation();
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                /// Spawned Fall Catcher is checking for players to bring up,
                /// Summoned Fall Catcher will just bring up player the Spawn one has find
                m_Type = FALL_CATCHER_TRANSPORTER;
                me->SetCanFly(true);
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != EFFECT_MOTION_TYPE)
                    return;

                /// Takeoff
                if (p_Id == 1)
                {
                    Position l_LandPos = { m_LandPosX, m_LandPosY, -28.0f, m_LandOri };

                    me->GetMotionMaster()->Clear();
                    me->GetMotionMaster()->MoveLand(2, l_LandPos);
                    return;
                }

                /// Landing
                if (p_Id == 2)
                {
                    /// Releasing passenger
                    if (m_Vehicle)
                    {
                        if (Unit* l_Player = m_Vehicle->GetPassenger(0))
                        {
                            l_Player->RemoveAura(SPELL_SAFETY_NET_TRIGGER);
                            l_Player->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);
                            me->CastSpell(l_Player, SPELL_EJECT_ALL_PASSENGERS, false);
                        }
                    }

                    /// Despawn
                    me->DespawnOrUnsummon();
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_GET_FALL_CATCHER_TYPE)
                    return m_Type;
                return 0;
            }

            void SetGUID(uint64 p_Guid, int32 p_Type)
            {
                if (p_Type == TYPE_SET_FALL_CATCHER_PLAYER_GUID)
                {
                    /// Start transport
                    m_IsTransporting = true;
                    if (Player* l_Player = me->GetPlayer(*me, p_Guid))
                    {
                        l_Player->RemoveAura(SPELL_PREVENT_FALL_DAMAGE);
                        l_Player->AddAura(SPELL_SAFETY_NET_TRIGGER, l_Player);
                        l_Player->CastSpell(me, SPELL_CATCH_FALL, false);
                        l_Player->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_SILENCED);

                        me->SetDisableGravity(true);
                        me->GetMotionMaster()->MoveTakeoff(1, me->GetPositionX(), me->GetPositionY(), -25.0f);
                    }
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                /// Transporter doesn't need to check in UpdateAI
                if (m_Type == FALL_CATCHER_TRANSPORTER)
                    return;

                /// Waiting for a player to take back
                if (!m_IsTransporting && m_Instance)
                {
                    Creature* l_JiKun = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_JI_KUN));
                    if (!l_JiKun)
                        return;

                    /// Check players prevent fall damage aura, cause Ji-Kun can't handle it if dead
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                    for (Player* l_Player : l_PlayerList)
                    {
                        /// Don't bring back dead players
                        if (!l_Player->isAlive())
                            continue;

                        /// If player is supposed to have the aura
                        if (IsPlayerInPreventDamageArea(l_Player))
                        {
                            if (!l_Player->HasAura(SPELL_PREVENT_FALL_DAMAGE))
                                me->CastSpell(l_Player, SPELL_PREVENT_FALL_DAMAGE, true);
                        }
                        /// Else, remove the aura if player has it
                        else if (l_Player->HasAura(SPELL_PREVENT_FALL_DAMAGE))
                        {
                            l_Player->RemoveAura(SPELL_PREVENT_FALL_DAMAGE);
                            continue;
                        }

                        /// If player isn't at the bottom of the room, he doesn't need to be bring back to Ji-Kun's platform
                        if (!IsTransportablePlayer(l_Player) || (!IsNearestTransport(l_Player)))
                            continue;

                        /// Summons another "me" who will bring up the player
                        if (Creature* l_Transporter = l_JiKun->SummonCreature(NPC_FALL_CATCHER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), me->GetOrientation()))
                            l_Transporter->AI()->SetGUID(l_Player->GetGUID(), TYPE_SET_FALL_CATCHER_PLAYER_GUID);

                        m_IsTransporting = true;
                        m_Events.ScheduleEvent(EVENT_REACTIVE_FALL_CATCHER, 1000);
                    }
                }

                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_REACTIVE_FALL_CATCHER)
                    m_IsTransporting = false;
            }

            bool IsPlayerInPreventDamageArea(Player* p_Player)
            {
                if (p_Player->isAlive())
                {
                    float l_PlayerX = p_Player->GetPositionX();
                    float l_PlayerY = p_Player->GetPositionY();

                    if (l_PlayerX > 6045.0f && l_PlayerX < 6247.0f && l_PlayerY > 4220.0f && l_PlayerY < 4446.0f)
                        return true;
                }

                return false;
            }

            bool IsTransportablePlayer(Player* p_Player)
            {
                if (!IsPlayerInPreventDamageArea(p_Player) || !p_Player->isAlive() || p_Player->HasAura(SPELL_SAFETY_NET_TRIGGER) || p_Player->IsOnVehicle() || p_Player->GetPositionZ() > -183.0f)
                    return false;
                return true;
            }

            bool IsNearestTransport(Player* p_Player)
            {
                float l_Dist = me->GetDistance(p_Player);

                /// Comparing distance to player with other fall catchers
                std::list<Creature*> l_FallCatcherList;
                GetCreatureListWithEntryInGrid(l_FallCatcherList, me, NPC_FALL_CATCHER, 200.0f);

                /// Only keep Main Fall catchers
                l_FallCatcherList.remove_if(CheckFallCatcherType());

                for (Creature* l_FallCatcher : l_FallCatcherList)
                {
                    if (l_FallCatcher->AI()->GetData(TYPE_GET_FALL_CATCHER_TYPE) == FALL_CATCHER_TRANSPORTER)
                        continue;

                    /// If any fall catcher is closest
                    if (l_Dist > l_FallCatcher->GetDistance(p_Player))
                        return false;
                }

                /// Still here? So our fall catcher is the closest
                return true;
            }

            class CheckFallCatcherType
            {
                public:
                    bool operator()(Creature* p_Creature) const
                    {
                        if (p_Creature->GetEntry() == NPC_FALL_CATCHER)
                        {
                            if (p_Creature->AI()->GetData(TYPE_GET_FALL_CATCHER_TYPE) == FALL_CATCHER_MAIN)
                                return true;
                        }

                        return false;
                    }
            };

        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_fall_catcherAI(p_Creature);
        }
};

/// Incubater - 69626
class mob_incubater : public CreatureScript
{
    public:
        mob_incubater() : CreatureScript("mob_incubater") { }

        struct mob_incubaterAI : public ScriptedAI
        {
            mob_incubaterAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint32 m_NestOrder;
            bool m_IsLowerNest;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                /* 
                 * --- Setting Id order to allow boss to retrieve the incubater in the nest ---
                 * As the nest are triggered clockwise, we need to reorder them according to their position
                 */
                m_NestOrder = 0;

                /// In a lower nest
                if (me->GetPositionZ() < 0.0f)
                {
                    if (me->GetPositionX() > 6215.0f)
                        m_NestOrder = 4;
                    else if (me->GetPositionX() > 6190.0f)
                        m_NestOrder = 0;
                    else if (me->GetPositionX() > 6150.0f)
                        m_NestOrder = 3;
                    else if (me->GetPositionX() > 6090.0f)
                        m_NestOrder = 2;
                    else if (me->GetPositionX() > 6050.0f)
                        m_NestOrder = 1;
                    m_IsLowerNest = true;
                }
                /// In a upper nest
                else
                {
                    if (me->GetPositionX() > 6215.0f)
                        m_NestOrder = 14;
                    else if (me->GetPositionX() > 6170.0f)
                        m_NestOrder = 10;
                    else if (me->GetPositionX() > 6150.0f)
                        m_NestOrder = 13;
                    else if (me->GetPositionX() > 6080.0f)
                        m_NestOrder = 12;
                    else if (me->GetPositionX() > 6075.0f)
                        m_NestOrder = 11;
                    m_IsLowerNest = false;
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_GET_NEST_ID)
                    return m_NestOrder;

                return 0;
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ACTIVATE_NEST:
                    {
                        me->CastSpell(me, SPELL_INCUBATE, true);

                        uint8 l_MaxEggs = 0;

                        /// Lower nests : 4 eggs in 10 players, 5 in 25 players
                        if (m_NestOrder < 10)
                            l_MaxEggs = Is25ManRaid() ? 5 : 4;
                        /// Upper nests : 2 eggs in 10 players, 3 in 25 players
                        else
                            l_MaxEggs = Is25ManRaid() ? 3 : 2;

                        for (uint8 l_Idx = 0; l_Idx < l_MaxEggs; ++l_Idx)
                        {
                            float l_Dist = frand(1.0f, 3.5f);
                            float l_Ori  = frand(0.0f, 2 * M_PI);
                            float l_PosX = me->GetPositionX() + l_Dist * cos(l_Ori);
                            float l_PosY = me->GetPositionY() + l_Dist * sin(l_Ori);

                            me->SummonCreature(m_NestOrder < 10 ? NPC_YOUNG_EGG_OF_JI_KUN : NPC_MATURE_EGG_OF_JI_KUN, l_PosX, l_PosY, me->GetPositionZ());
                        }

                        break;
                    }
                    case ACTION_INCUBATER_WIPE:
                    {
                        summons.DespawnAll();
                        me->RemoveAura(SPELL_INCUBATE);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_incubaterAI(p_Creature);
        }
};

/// Young egg of Ki-Kun - 68194
class mob_young_egg_of_jikun : public CreatureScript
{
    public:
        mob_young_egg_of_jikun() : CreatureScript("mob_young_egg_of_jikun") { }

        struct mob_young_egg_of_jikunAI : public ScriptedAI
        {
            mob_young_egg_of_jikunAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
                me->SetReactState(REACT_PASSIVE);

                me->CastSpell(me, SPELL_INCUBATED, false);
                m_Events.ScheduleEvent(EVENT_HATCH, 10000);
            }

            void DamageTaken(Unit* /*p_Attacker*/, uint32 &/*damage*/, SpellInfo const*  /*p_SpellInfo*/)
            {
                m_Events.CancelEvent(EVENT_HATCH);
                DoAction(ACTION_HATCH);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_HATCH)
                {
                    Position l_Pos;
                    me->GetPosition(&l_Pos);
                    if (Creature* l_HatchedMe = me->SummonCreature(NPC_HATCHLING, l_Pos))
                        l_HatchedMe->SetHealth(l_HatchedMe->GetMaxHealth() * me->GetHealth() / me->GetMaxHealth());
                    me->Kill(me);
                    me->DespawnOrUnsummon(1000);
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_HATCH)
                    DoAction(ACTION_HATCH);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_young_egg_of_jikunAI(p_Creature);
        }
};

/// Hatchling - 68192
class mob_hatchling : public CreatureScript
{
    public:
        mob_hatchling() : CreatureScript("mob_hatchling") { }

        struct mob_hatchlingAI : public ScriptedAI
        {
            mob_hatchlingAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            bool m_IsHatchling;
            bool m_IsEating;
            uint32 m_FeedGuid;

            void Reset()
            {
                m_IsEating = false;
                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_CHEEP, 1000);
                m_IsHatchling = true;
                m_FeedGuid = 0;
                me->m_SightDistance  = 5.0f;
                me->m_CombatDistance = 5.0f;
            }

            void JustDied(Unit* /*killer*/)
            {
                me->CastSpell(me, SPELL_DROP_FEATHERS_GOB, false);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_TAKE_FEED:
                    {
                        me->SummonCreature(NPC_FEED, g_BossPos.GetPositionX(), g_BossPos.GetPositionY(), g_BossPos.GetPositionZ());
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_IS_HATCHLING)
                    return m_IsHatchling ? 1 : 0;
                return 0;
            }

            void UpdateAI(const uint32 diff)
            {
                if (!m_IsHatchling && me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(diff);

                if (m_IsHatchling)
                {
                    if (!m_IsEating)
                    {
                        std::list<Creature*> l_FeedPoolList;
                        GetCreatureListWithEntryInGrid(l_FeedPoolList, me, NPC_FEED_POOL, 5.0f);

                        for (Creature* l_Feed : l_FeedPoolList)
                        {
                            if (!l_Feed->AI()->GetData(TYPE_GET_FEEDPOOL_STATUS))
                            {
                                m_IsEating = true;
                                m_Events.Reset();
                                m_Events.ScheduleEvent(EVENT_EVOLVE, 10000);
                                m_FeedGuid = l_Feed->GetGUID();
                                l_Feed->AI()->SetData(TYPE_SET_FEEDPOOL_STATUS, 1);
                                me->CastSpell(me, SPELL_EAT, true);
                            }
                        }
                    }
                }

                switch (m_Events.ExecuteEvent())
                {
                    /// Replace Melee attack?
                    case EVENT_CHEEP:
                    {
                        Player* l_Victim = me->SelectNearestPlayer();
                        me->CastSpell(l_Victim, m_IsHatchling ? SPELL_CHEEP_HATCHLING : SPELL_CHEEP_FLEDGLING, true);
                        m_Events.ScheduleEvent(EVENT_CHEEP, 5000);
                        break;
                    }
                    case EVENT_LAYING:
                    {
                        DoCast(SPELL_LAY_EGG);
                        m_Events.ScheduleEvent(EVENT_LAYING, 30000);
                        break;
                    }
                    case EVENT_EVOLVE:
                    {
                        me->CastSpell(me, SPELL_MORPH, false);

                        m_IsEating = false;
                        m_IsHatchling = false;
                        m_Events.Reset();

                        m_Events.ScheduleEvent(EVENT_LAYING, urand(10000, 20000));
                        m_Events.ScheduleEvent(EVENT_CHEEP,  1000);

                        if (Creature* l_Feed = ObjectAccessor::GetCreature(*me, m_FeedGuid))
                            l_Feed->DespawnOrUnsummon();

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_hatchlingAI(p_Creature);
        }
};

/// Ji-Kun Fledgling's Egg - 68202
/// Mature Egg of Ji-Kun - 69628
class mob_fledgling_egg : public CreatureScript
{
    public:
        mob_fledgling_egg() : CreatureScript("mob_fledgling_egg") { }
        
        struct mob_fledgling_eggAI : public ScriptedAI
        {
            mob_fledgling_eggAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            bool m_Drop;

            void Reset()
            {
                m_Events.Reset();
                m_Drop = true;
                me->SetReactState(REACT_PASSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC);

                if (me->GetEntry() == NPC_MATURE_EGG_OF_JI_KUN)
                    me->CastSpell(me, SPELL_INCUBATED, false);
                /// Upper nests hatch after 20 secs, lower nests, after 10 secs
                m_Events.ScheduleEvent(EVENT_HATCH, me->GetPositionZ() > 0.0f ? 20000 : 10000);
            }

            void JustDied(Unit* /*killer*/)
            {
                if (m_Drop)
                    me->CastSpell(me, SPELL_DROP_FEATHERS_GOB, false);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_HATCH)
                {
                    if (Creature* l_Juvenile = me->SummonCreature(NPC_FLEGLING_JUVENILE, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
                    {
                        m_Drop = false;
                        l_Juvenile->Kill(me);
                        me->DespawnOrUnsummon(2000);
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_fledgling_eggAI(p_Creature);
        }
};

/// Juvenile - 70095 (born from 68202 - Ji_Kun Fledgling's Egg)
/// Juvenile - 69836 (born from 69628 - Mature egg of Ji-Kun)
class mob_juvenile : public CreatureScript
{
    public:
        mob_juvenile() : CreatureScript("mob_juvenile") { }

        struct mob_juvenileAI : public ScriptedAI
        {
            mob_juvenileAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();

                /// Set Flying
                me->SetDisableGravity(true);
                me->SetCanFly(true);
                me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);

                m_Events.ScheduleEvent(EVENT_CHEEP, 1000);
                m_Events.ScheduleEvent(EVENT_FLIGHT, 5000);
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE || p_Id != 1)
                    return;

                m_Events.ScheduleEvent(EVENT_FEED_YOUNG, 1000);
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_SUMMON_FEED)
                {
                    if (Creature* l_Feed = me->SummonCreature(NPC_FEED, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()))
                        l_Feed->AI()->SetData(TYPE_SET_FEED_STATUS, 0);
                }
            }

            void EnterEvadeMode()
            {
                me->DespawnOrUnsummon();
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                while (uint32 l_EventId = m_Events.ExecuteEvent())
                {
                    switch (l_EventId)
                    {
                        case EVENT_CHEEP:
                        {
                            if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM, 0, 200.0f, true))
                                me->CastSpell(l_Target, SPELL_CHEEP_JUVENILE, false);
                            m_Events.ScheduleEvent(EVENT_CHEEP, 5000);
                            break;
                        }
                        case EVENT_FLIGHT:
                        {
                            /// Shouldn't attack player in melee anymore
                            me->SetReactState(REACT_PASSIVE);

                            float l_Ori  = frand(0.0f, 2 * M_PI);
                            float l_Dist = frand(5.0f, 20.0f);
                            float l_PosX = g_BossPos.GetPositionX() + l_Dist * cos(l_Ori);
                            float l_PosY = g_BossPos.GetPositionY() + l_Dist * sin(l_Ori);
                            float l_PosZ = 5.0f;

                            me->GetMotionMaster()->MovePoint(1, l_PosX, l_PosY, l_PosZ);

                            break;
                        }
                        case EVENT_FEED_YOUNG:
                        {
                            me->CastSpell(me, SPELL_SUMMON_FEED, false);
                            m_Events.ScheduleEvent(EVENT_FEED_YOUNG, 50000);
                            break;
                        }
                        default:
                            break;
                    }
                    DoMeleeAttackIfReady();
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_juvenileAI(p_Creature);
        }
};

/// Feed - 68178
class mob_feed : public CreatureScript
{
    public:
        mob_feed() : CreatureScript("mob_feed") { }

        struct mob_feedAI : public ScriptedAI
        {
            mob_feedAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_ToHatchling;
            uint32 m_SummonerEntry;
            EventMap m_Events;
            uint64 m_HatchlingGuid;

            void Reset()
            {
                m_Events.Reset();
                m_ToHatchling = false;
                m_SummonerEntry = 0;
                m_HatchlingGuid = 0;
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_SummonerEntry = p_Summoner->GetEntry();
                if (m_SummonerEntry == NPC_HATCHLING)
                {
                    m_ToHatchling = true;
                    m_HatchlingGuid = p_Summoner->GetGUID();
                }
                m_Events.ScheduleEvent(EVENT_FEED_ACTIVATE, 100);
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                if (p_Type == TYPE_SET_FEED_STATUS)
                    m_ToHatchling = p_Value ? true : false;
            }

            void MovementInform(uint32 p_Type, uint32 p_Id)
            {
                if (p_Type != POINT_MOTION_TYPE && p_Type != EFFECT_MOTION_TYPE)
                    return;

                if (p_Id == 1)
                {
                    me->CastSpell(me, SPELL_SUMMON_FEED_POOL, false);
                    me->DespawnOrUnsummon();
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                /// In case a player catch a feed...
                if (Player* l_Player = me->SelectNearestPlayer(0.2f))
                {
                    me->AddAura(SPELL_SLIMED, l_Player);
                    me->AddAura(SPELL_PRIMAL_NUTRIMENT, l_Player);
                    me->GetMotionMaster()->Clear();
                    me->DespawnOrUnsummon();
                }

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_FEED_ACTIVATE:
                    {
                        if (m_ToHatchling)
                        {
                            if (Creature* l_Summoner = Creature::GetCreature(*me, m_HatchlingGuid))
                                me->GetMotionMaster()->MoveJump(l_Summoner->GetPositionX(), l_Summoner->GetPositionY(), l_Summoner->GetPositionZ(), 10.0f, 50.0f, 0.0f, 1);
                            break;
                        }

                        float l_Dist = frand(10.0f, 40.0f);
                        float l_Ori  = frand(0.0f, 2 * M_PI);
                        float l_PosX = g_BossPos.GetPositionX() + l_Dist * cos(l_Ori);
                        float l_PosY = g_BossPos.GetPositionY() + l_Dist * sin(l_Ori);

                        if (m_SummonerEntry == NPC_JI_KUN)
                            me->GetMotionMaster()->MoveJump(l_PosX, l_PosY, g_BossPos.GetPositionZ(), 10.0f, 30.0f, 0.0f, 1);
                        else
                        {
                            me->SetCanFly(true);
                            me->SetDisableGravity(true);
                            me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);

                            me->GetMotionMaster()->MovePoint(1, l_PosX, l_PosY, g_BossPos.GetPositionZ());
                        }
                        
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_feedAI(p_Creature);
        }
};

/// Feed Pool - 68188
class mob_feed_pool : public CreatureScript
{
    public:
        mob_feed_pool() : CreatureScript("mob_feed_pool") { }

        struct mob_feed_poolAI : public ScriptedAI
        {
            mob_feed_poolAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            bool m_IsEaten;
            bool m_IsAbsorbed;
            uint64 m_AbsorbingPlayer;
            std::map<uint64, uint8> m_PlayerTick;
            EventMap m_Events;

            void Reset()
            {
                me->CastSpell(me, SPELL_FEED_POOL_SPAWN_SPELL_GREEN, false);
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                m_IsEaten = false;
                m_IsAbsorbed = false;
                m_AbsorbingPlayer = 0;
                m_PlayerTick.clear();
                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_FEEDPOOL_CHECK_PLAYERS, 500);
            }

            void DoAction(const int32 p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_FEED_EATEN:
                    {
                        m_IsEaten = true;
                        break;
                    }
                    case ACTION_FEEDPOOL_DESPAWN:
                    {
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 3.0f);
                        me->RemoveAura(SPELL_FEED_POOL_SPAWN_SPELL_GREEN);
                        m_Events.Reset();

                        for (Player* l_Player : l_PlayerList)
                        {
                            if (l_Player->HasAura(SPELL_FEED_POOL_PERIODIC_DMG))
                                l_Player->RemoveAura(SPELL_FEED_POOL_PERIODIC_DMG);
                        }

                        me->DespawnOrUnsummon();
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_GET_FEEDPOOL_STATUS)
                    return m_IsEaten ? 1 : 0;
                return 0;
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                if (p_Type == TYPE_SET_FEED_STATUS)
                    m_IsEaten = p_Value;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_FEEDPOOL_CHECK_PLAYERS)
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, me, 8.0f);

                    std::map<uint64, uint8>::iterator l_Itr = m_PlayerTick.begin();
                    std::list<uint64> l_PlayerGuidList;
                    l_PlayerGuidList.clear();

                    for (Player* l_Player : l_PlayerList)
                    {
                        /// Removing damage aura if player is outside of the pool
                        if (l_Player->GetDistance2d(me) > 3.0f && l_Player->HasAura(SPELL_FEED_POOL_PERIODIC_DMG))
                        {
                            l_Player->RemoveAura(SPELL_FEED_POOL_PERIODIC_DMG);
                            continue;
                        }

                        /// Player in the pool
                        /// Adding Slimed aura if player hasn't it
                        if (!l_Player->HasAura(SPELL_SLIMED))
                            me->AddAura(SPELL_SLIMED, l_Player);

                        /// Checking damage aura
                        if (l_Player->HasAura(SPELL_FEED_POOL_PERIODIC_DMG))
                        {
                            /// Checking how long players have standed in the pool
                           l_Itr = m_PlayerTick.find(l_Player->GetGUID());
                           if (l_Itr != m_PlayerTick.end())
                           {
                               /// Increasing tick
                               ++(l_Itr->second);

                               /// If player has standed for 6 ticks in the zone, that is 3 second, the pool despawn
                               if (l_Itr->second >= 6)
                                   DoAction(ACTION_FEEDPOOL_DESPAWN);

                               /// Remaining player has been found
                               l_PlayerGuidList.push_back(l_Player->GetGUID());
                           }
                           /// Player not in map : we add him
                           else
                           {
                               m_PlayerTick.insert(std::make_pair(l_Player->GetGUID(), 1));
                               l_PlayerGuidList.push_back(l_Player->GetGUID());
                           }
                        }
                        /// Player hasn't damage aura : we add aura, and put its guid in the map
                        else
                        {
                            me->AddAura(SPELL_FEED_POOL_PERIODIC_DMG, l_Player);
                            m_PlayerTick.insert(std::make_pair(l_Player->GetGUID(), 1));
                            l_PlayerGuidList.push_back(l_Player->GetGUID());
                        }
                    }

                    /// Now we must remove players in the map which haven't been found
                    std::map<uint64, uint8>::iterator l_Next;
                    for (l_Itr = m_PlayerTick.begin(); l_Itr != m_PlayerTick.end(); l_Itr = l_Next)
                    {
                        /// Avoid crash with if l_Itr is erased
                        l_Next = l_Itr;
                        ++l_Next;

                        /// If player has been found in l_PlayerGuidList, we keep him
                        bool l_Found = false;
                        for (uint64 l_Guid : l_PlayerGuidList)
                        {
                            if (l_Guid == l_Itr->first)
                                l_Found = true;
                        }

                        /// Else, we remove him from the list, so he needs to enter the void zone again to despawn it
                        if (!l_Found)
                            m_PlayerTick.erase(l_Itr->first);
                    }

                    m_Events.ScheduleEvent(EVENT_FEEDPOOL_CHECK_PLAYERS, 500);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_feed_poolAI(p_Creature);
        }
};

/// Exit Chamber [DNT] - 70734
class mob_jikun_exit_chamber : public CreatureScript
{
    public:
        mob_jikun_exit_chamber() : CreatureScript("mob_jikun_exit_chamber") { }

        struct mob_jikun_exit_chamberAI : public ScriptedAI
        {
            mob_jikun_exit_chamberAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_EXIT_CHAMBER, false);
                me->CastSpell(me, SPELL_DROP_FEATHERS_GOB, false);
            }

            void UpdateAI(uint32 const /*p_Diff*/)
            {
                if (!GetClosestGameObjectWithEntry(me, GOB_FEATHER_OF_JI_KUN, 10.0f))
                    me->CastSpell(me, SPELL_DROP_FEATHERS_GOB, false);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_jikun_exit_chamberAI(p_Creature);
        }
};

/// Nest Guardian - 70134
class mob_nest_guardian : public CreatureScript
{
    public:
        mob_nest_guardian() : CreatureScript("mob_nest_guardian") { }

        struct mob_nest_guardianAI : public ScriptedAI
        {
            mob_nest_guardianAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->AddUnitState(UNIT_STATE_ROOT);
                m_Events.Reset();
                m_Events.ScheduleEvent(EVENT_TALON_STRIKE, 10000);
                m_Events.ScheduleEvent(EVENT_SCREECH, 500);
            }

            void JustDied(Unit* /*killer*/)
            {
                me->DespawnOrUnsummon(2000);
            }

            void UpdateAI(const uint32 p_Diff)
            {
                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                while (uint32 l_EventId = m_Events.ExecuteEvent())
                {
                    switch (l_EventId)
                    {
                        case EVENT_TALON_STRIKE:
                        {
                            Unit* l_Victim = me->getVictim();
                            if (!l_Victim)
                                l_Victim = SelectTarget(SELECT_TARGET_TOPAGGRO, 0, 5.0f, true);

                            if (l_Victim)
                                me->CastSpell(l_Victim, SPELL_TALON_STRIKE, true);

                            m_Events.ScheduleEvent(EVENT_TALON_STRIKE, 10000);
                            break;
                        }
                        case EVENT_SCREECH:
                        {
                            if (!me->SelectNearbyTarget() && (!me->getVictim() || me->getVictim()->GetDistance2d(me) > NOMINAL_MELEE_RANGE))
                                me->CastSpell(me, SPELL_SCREECH, true);
                            m_Events.ScheduleEvent(EVENT_SCREECH, 2000);
                            break;
                        }
                        default:
                            break;
                    }
                }
                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_nest_guardianAI(p_Creature);
        }
};


/// Caw - 138923
class spell_caw : public SpellScriptLoader
{
    public:
        spell_caw() : SpellScriptLoader("spell_caw") { }

        class spell_caw_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_caw_SpellScript);

            void HandleOnHit()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                        l_Caster->CastSpell(l_Target, SPELL_CAW_MISSILE, true);
                }
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_caw_SpellScript::HandleOnHit);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_caw_SpellScript();
        }
};

/// Regurgitate - 134385
class spell_regurgitate : public SpellScriptLoader
{
    public:
        spell_regurgitate() : SpellScriptLoader("spell_regurgitate") { }

        class spell_regurgitate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_regurgitate_SpellScript);

            void HandleBeforeCast()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->ToCreature())
                        l_Caster->GetAI()->DoAction(ACTION_SUMMON_FEED);
                }
            }

            void Register()
            {
                BeforeCast += SpellCastFn(spell_regurgitate_SpellScript::HandleBeforeCast);
            }
        };

    private:
        class CheckTargets
        {
        public:
            bool operator()(Creature* p_Creature) const
            {
                if (!p_Creature->isAlive())
                {
                    if (p_Creature->AI()->GetData(TYPE_IS_HATCHLING))
                        return true;
                }

                return false;
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_regurgitate_SpellScript();
        }
};

/// 134339 - Daedalian Wings
class spell_daedalian_wings : public SpellScriptLoader
{
    public:
        spell_daedalian_wings() : SpellScriptLoader("spell_daedalian_wings") { }

        class spell_daedalian_wings_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_daedalian_wings_SpellScript);

            SpellCastResult CheckDeadalian()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_LESSON_OF_ICARUS))
                        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
                    return SPELL_CAST_OK;
                }

                return SPELL_FAILED_CASTER_DEAD;
            }

            void SetStackEnd()
            {
                Unit* l_Caster = GetCaster();

                if (!l_Caster)
                    return;

                Aura* l_DaedalianAura = l_Caster->GetAura(SPELL_DAEDALIAN_WINGS);
                if (!l_DaedalianAura)
                    l_DaedalianAura = l_Caster->AddAura(SPELL_DAEDALIAN_WINGS, l_Caster);

                if (!l_DaedalianAura)
                    return;

                l_DaedalianAura->SetStackAmount(4);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_daedalian_wings_SpellScript::CheckDeadalian);
                AfterCast   += SpellCastFn(spell_daedalian_wings_SpellScript::SetStackEnd);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_daedalian_wings_SpellScript();
        }
};

/// 133755 - Flight
class spell_flight : public SpellScriptLoader
{
    public:
        spell_flight() : SpellScriptLoader("spell_flight") { }

        class spell_flight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_flight_SpellScript);

            SpellCastResult AllowFly()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetMapId() != 1098)
                        return SPELL_FAILED_INCORRECT_AREA;

                    return SPELL_CAST_OK;
                }

                return SPELL_FAILED_CASTER_DEAD;
            }

            void DaedalianStacks()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Aura* l_DaedalianAura = l_Caster->GetAura(SPELL_DAEDALIAN_WINGS))
                    {
                        if (l_DaedalianAura->GetStackAmount() == 1)
                            l_Caster->RemoveAura(SPELL_DAEDALIAN_WINGS);
                        else
                            l_DaedalianAura->SetStackAmount(l_DaedalianAura->GetStackAmount() - 1);
                    }
                }
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_flight_SpellScript::AllowFly);
                AfterCast   += SpellCastFn(spell_flight_SpellScript::DaedalianStacks);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_flight_SpellScript();
        }
};

/// Screech - 140640
class spell_screech : public SpellScriptLoader
{
    public:
        spell_screech() : SpellScriptLoader("spell_screech") { }

        class spell_screech_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_screech_SpellScript);

            void ReduceSpeed()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Victim = GetHitPlayer();

                if (!l_Victim || !l_Caster)
                    return;

                l_Caster->AddAura(SPELL_SCREECH_REDUCE_CASTING_SPEED, l_Victim);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_screech_SpellScript::ReduceSpeed);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_screech_SpellScript();
        }
};

/// Daedalian Wings - 140014
class spell_daedalian_end : public SpellScriptLoader
{
    public:
        spell_daedalian_end() : SpellScriptLoader("spell_daedalian_end") { }

        class spell_daedalian_end_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_daedalian_end_SpellScript);

            SpellCastResult CheckFlightAura()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->HasAura(SPELL_FLIGHT_LONG))
                        return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;

                    return SPELL_CAST_OK;
                }

                return SPELL_FAILED_CASTER_DEAD;
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_daedalian_end_SpellScript::CheckFlightAura);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_daedalian_end_SpellScript();
        }
};

/// Flight - 140013
class spell_jikun_flight : public SpellScriptLoader
{
    public:
        spell_jikun_flight() : SpellScriptLoader("spell_jikun_flight") { }

        class spell_jikun_flight_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_jikun_flight_SpellScript);

            SpellCastResult CheckArea()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetMapId() != 1098)
                        return SPELL_FAILED_INCORRECT_AREA;

                    return SPELL_CAST_OK;
                }

                return SPELL_FAILED_CASTER_DEAD;
            }

            void DaedalianAura()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->RemoveAura(SPELL_DAEDALIAN_WINGS_LONG);
            }

            void Register()
            {
                OnCheckCast += SpellCheckCastFn(spell_jikun_flight_SpellScript::CheckArea);
                AfterCast   += SpellCastFn(spell_jikun_flight_SpellScript::DaedalianAura);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_jikun_flight_SpellScript();
        }
};

/// Feather of Ji-Kun - 218543
class go_feather_of_jikun : public GameObjectScript
{
    public:
        go_feather_of_jikun() : GameObjectScript("go_feather_of_jikun") { }

        struct go_feather_of_jikunAI : public GameObjectAI
        {
            go_feather_of_jikunAI(GameObject* p_Go) : GameObjectAI(p_Go) { }

            bool GossipHello(Player* p_Player)
            {
                InstanceScript* l_Instance = p_Player->GetInstanceScript();
                if (!l_Instance)
                    return false;

                if (l_Instance->GetBossState(DATA_JI_KUN) == DONE)
                {
                    if (p_Player->HasAura(SPELL_FLIGHT_LONG) || p_Player->HasAura(SPELL_DAEDALIAN_WINGS_LONG))
                        return false;

                    go->CastSpell(p_Player, SPELL_DAEDALIAN_WINGS_LONG);
                }
                else
                {
                    if (p_Player->HasAura(SPELL_LESSON_OF_ICARUS))
                        return false;

                    go->CastSpell(p_Player, SPELL_DAEDALIAN_WINGS);
                    go->Delete();
                }

                return true;
            }
        };

        GameObjectAI* GetAI(GameObject* p_Go) const
        {
            return new go_feather_of_jikunAI(p_Go);
        }
};

class at_down_draft : public AreaTriggerEntityScript
{
    public:
        at_down_draft() : AreaTriggerEntityScript("at_down_draft") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            std::list<Player*> l_PlayerList;
            GetPlayerListInGrid(l_PlayerList, p_AreaTrigger, 40.0f);

            Position l_Pos;
            p_AreaTrigger->GetPosition(&l_Pos);

            for (Player* l_Player : l_PlayerList)
            {
                if (l_Player->IsWithinDist(l_Caster, 30.0f, false))
                {
                    if (l_Player->isAlive() && !l_Player->HasMovementForce(p_AreaTrigger->GetGUID()))
                        l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), true, l_Pos, -7.0f, 1);
                    else if (!l_Player->isAlive() && l_Player->HasMovementForce(p_AreaTrigger->GetGUID()))
                        l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, l_Pos);
                }
                else if (l_Player->HasMovementForce(p_AreaTrigger->GetGUID()))
                    l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, l_Pos);
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Position l_Pos;
            p_AreaTrigger->GetPosition(&l_Pos);

            Map::PlayerList const &l_PlayerList = p_AreaTrigger->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
            {
                for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                {
                    if (Player* l_Player = l_Itr->getSource())
                        l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, l_Pos);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_down_draft();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_ji_kun()
{
    new boss_ji_kun();                  ///< 69712
    new mob_jump_to_boss_platform();    ///< 69885
    new mob_fall_catcher();             ///< 69839
    new mob_incubater();                ///< 69626
    new mob_young_egg_of_jikun();       ///< 68194
    new mob_hatchling();                ///< 68192
    new mob_fledgling_egg();            ///< 68202 - 69628
    new mob_juvenile();                 ///< 70095 - 69836
    new mob_feed();                     ///< 68178
    new mob_feed_pool();                ///< 68188
    new mob_jikun_exit_chamber();       ///< 70734
    new mob_nest_guardian();            ///< 70134
    new spell_caw();                    ///< 138923
    new spell_regurgitate();            ///< 134385
    new spell_daedalian_wings();        ///< 134339
    new spell_flight();                 ///< 133755
    new spell_screech();                ///< 140640
    new spell_daedalian_end();          ///< 140014
    new spell_jikun_flight();           ///< 140013
    new go_feather_of_jikun();          ///< 218543
    new at_down_draft();                ///< 134370
}
#endif