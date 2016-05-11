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
#include "ScriptMgr.h"
#include "SpellScript.h"
#include "throne_of_thunder.h"
#include "AreaTrigger.h"

enum eSpells
{
    /// Thunder Trap
    SPELL_THUNDER_TRAP              = 138172,

    /// Lei Shen

    /// --- Intro ---
    SPELL_COSMETIC_TELEPORT_N       = 139103,
    SPELL_COSMETIC_TELEPORT_E       = 139102,
    SPELL_COSMETIC_TELEPORT_S       = 139105,
    SPELL_COSMETIC_TELEPORT_W       = 139104,
    SPELL_COSMETIC_VISUAL           = 138275,

    /// --- Generic Spells/Pillars ---
    /// Away from Pillars
    SPELL_DISCHARGED_ENERGY_AT      = 134820,
    SPELL_DISCHARGED_ENERGY         = 134821,
    SPELL_PILLAR_IN_CHARGE_VISUAL   = 134803,
    /// Static Shock Pillar
    SPELL_STATIC_SHOCK_ACTIVE       = 135680,
    SPELL_STATIC_SHOCK              = 135695,
    /// Diffusion Chain Pillar
    SPELL_DIFFUSION_CHAIN_ACTIVE    = 135681,
    SPELL_DIFFUSION_CHAIN_DAMAGE    = 135991,
    SPELL_DIFFUSION_CHAIN_SUMMON_1  = 135992,   ///< Summon for Pillar level 1 - http://www.wowhead.com/npc=69012
    SPELL_CHAIN_LIGHTNING_SUMMON_1  = 136017,   ///< Chain Lightning spell for Summon level 1
    SPELL_CHAIN_LIGHTNING_DAMAGE_1  = 136018,   ///< Chain Lightning spell for damage level 1
    SPELL_DIFFUSION_CHAIN_SUMMON_2  = 135993,   ///< Summon for Pillar level 2 - http://www.wowhead.com/npc=69013
    SPELL_CHAIN_LIGHTNING_SUMMON_2  = 136020,   ///< Chain Lightning spell for Summon level 2
    SPELL_CHAIN_LIGHTNING_DAMAGE_2  = 136019,   ///< Chain Lightning spell for damage level 2
    SPELL_DIFFUSION_CHAIN_SUMMON_3  = 135994,   ///< Summon for Pillar level 3 - http://www.wowhead.com/npc=69014
    SPELL_CHAIN_LIGHTNING_SUMMON_3  = 136022,   ///< Chain Lightning spell for Summon level 3
    SPELL_CHAIN_LIGHTNING_DAMAGE_3  = 136021,   ///< Chain Lightning spell for damage level 3
    /// Overcharge Pillar
    SPELL_OVERCHARGE_ACTIVE         = 135682,
    SPELL_OVERCHARGE_AURA           = 136295,
    SPELL_OVERCHARGE_DAMAGE_STUN    = 136326,
    /// Bouncing Bolt Pillar
    SPELL_BOUNCING_BOLT_ACTIVE      = 135683,
    SPELL_BOUNCING_BOLT_MISSILE     = 136361,
    SPELL_BOUNCING_BOLT_BOUNCE      = 138706,

    /// --- Phase 1 ---
    SPELL_DECAPITATE_AURA           = 134912,
    SPELL_DECAPITATE_MISSILE        = 134990,
    SPELL_DECAPITATE_DAMAGE         = 134916,
    SPELL_THUNDERSTRUCK_SEARCH      = 135091,
    SPELL_THUNDERSTRUCK_SPAWN       = 135098,
    SPELL_THUNDERSTRUCK_MISSILE     = 135095,
    SPELL_CRASHING_THUNDER_AURA     = 135143,
    SPELL_CRASHING_THUNDER_AT      = 135150,
    SPELL_CRASHING_THUNDER_DMG     = 135153,

    /// --- Transition phase ---
    SPELL_DISPLACE                  = 136948,   ///< Teleport to the Console
    SPELL_SUPERCHARGE_CONDUITS      = 137146,   ///< Cast by Conduit
    SPELL_SUPERCHARGE_AURA          = 137045,   ///< On Conduits; trigers 137073
    SPELL_SUPERCHARGE_POWER         = 137150,   ///< Reset conduit power ?
    /// Overloaded
    SPELL_OVERLOAD_NORTH            = 137153,
    SPELL_OVERLOAD_EAST             = 137169,
    SPELL_OVERLOAD_SOUTH            = 137170,
    SPELL_OVERLOAD_WEST             = 137171,
    SPELL_OVERLOAD_AURA             = 137176,   ///< Periodically triggers 140210

    /// --- Phase 2 ---
    SPELL_FUSION_SLASH              = 136478,
    SPELL_SUMMON_BALL_LIGHTNING     = 136543,   ///< Summons 69232
    SPELL_BALL_LIGHTNING_AURA       = 136534,   ///< Visual aura
    SPELL_BALL_LIGHTNING_DAMAGE     = 136464,
    SPELL_BALL_LIGHTNING_AT         = 136470,
    SPELL_BALL_LIGHTNING            = 139621,   ///< Periodically triggers 136539
    SPELL_BALL_LIGHTNING_JUMP       = 136548,   ///< Triggers 136620
    SPELL_LIGHTNING_WHIP            = 136845,   ///< Triggers 136846, which summons 69385
    SPELL_LIGHTNING_WHIP_HIT        = 136850,   ///< Apply periodic aura 137499

    /// --- Phase 3 ---
    SPELL_OVERWHELMING_POWER_START  = 136913,   ///< Triggers 136914 and 137553
    SPELL_OVERWHELMING_POWER_AURA   = 136914,   ///< Aura put on target
    SPELL_OVERWHELMING_POWER_NPC    = 137553,   ///< Triggered on NPC_OVERWHELMING_POWER - 69645
    SPELL_OVERWHELMING_POWER_VISUAL = 137557,   ///< Cast by 69645
    SPELL_VIOLENT_GALE_WINDS        = 136869,
    SPELL_VIOLENT_GALE_WINDS_AT_N   = 136867,
    SPELL_VIOLENT_GALE_WINDS_AT_E   = 136876,
    SPELL_VIOLENT_GALE_WINDS_AT_S   = 136877,
    SPELL_VIOLENT_GALE_WINDS_AT_W   = 136879,
    SPELL_VIOLENT_GALE_WINDS_AURA   = 136889,
    SPELL_WINDBURN                  = 140208,

    /// End fight
    SPELL_BERSERK                   = 47008,

    /// Misc
    SPELL_IN_QUADRANT_NORTH         = 139006,
    SPELL_IN_QUADRANT_EAST          = 139007,
    SPELL_IN_QUADRANT_SOUTH         = 139008,
    SPELL_IN_QUADRANT_WEST          = 139009,
    SPELL_CONDUIT_LEVEL_NORTH       = 139271,
    SPELL_CONDUIT_LEVEL_EAST        = 139273,
    SPELL_CONDUIT_LEVEL_SOUTH       = 139272,
    SPELL_CONDUIT_LEVEL_WEST        = 137581,
    SPELL_CONDUIT_LEVEL_VISUAL      = 137606,   ///< Display alt power for players
    SPELL_ALTERNATE_FOR_PLAYERS     = 137607,
    SPELL_METER_VISUAL_ENERGY       = 134843,
    SPELL_METER_VISUAL_LEVEL_2      = 136034,
    SPELL_METER_VISUAL_LEVEL_3      = 136035
};

enum eEvents
{
    /// Pre-fight
    EVENT_THUNDER_TRAP = 1,
    /// Generic
    EVENT_CHECK_NEAR_CONDUIT,
    EVENT_BERSERKER,
    EVENT_PILLAR_ABILITY,
    EVENT_ENERGIZE_PILLAR,
    EVENT_DISCHARGED_ENERGY,
    EVENT_CHECK_LEI_SHEN_PILLAR,
    /// Phase 1
    EVENT_CRASHING_THUNDER,
    EVENT_DECAPITATE,
    EVENT_THUNDERSTRUCK,                ///< Also used in P3
    /// Transition phases
    EVENT_TRANSITION_PHASE_END,
    EVENT_ACTIVATING_CENTRAL_CONSOLE,
    /// Phase 2
    EVENT_FUSION_SLASH,
    EVENT_SUMMON_BALL_LIGHTNING,        ///< Also used in P3
    EVENT_LIGHTNING_WHIP,               ///< Also used in P3
    /// Phase 3
    EVENT_OVERWHELMING_POWER,
    EVENT_VIOLENT_GALE_WINDS
};

enum eSays
{
    TALK_INTRO,
    TALK_AGGRO,
    TALK_SLAY,
    TALK_BERSERK,
    TALK_DEATH,
    TALK_CHARGE,
    TALK_PHASE_2A,
    TALK_PHASE_2B,
    TALK_PHASE_3A,
    TALK_PHASE_3B,
    TALK_SPELL_1,
    TALK_SPELL_2,
    TALK_SPELL_3
};

enum eActions
{
    ACTION_ACTIVATE_TRAP,
    ACTION_TELEPORT_TO_NEXT_PILLAR,
    ACTION_REMOVE_EQUIPMENT,
    ACTION_SET_EQUIPMENT,
    ACTION_START_ENERGIZE,
    ACTION_STOP_ENERGIZE,
    ACTION_DEACTIVATE_PILLAR,
    ACTION_SET_TO_TRANSITION_PHASE,
    ACTION_RESET_PILLAR_ENERGY,
    ACTION_RESTART_PILLAR,
    /// -- Ergizing Pillar --
    ACTION_ENERGIZING_NORTH,
    ACTION_ENERGIZING_EAST,
    ACTION_ENERGIZING_SOUTH,
    ACTION_ENERGIZING_WEST,
    /// -- Pillar ability --
    ACTION_STATIC_SHOCK                 = 100, ///< Allocate specific action number
    ACTION_DIFFUSION_CHAIN,
    ACTION_OVERCHARGE,
    ACTION_BOUNCING_BOLT,
    ACTION_WIND_END
};

enum eDatas
{
    DATA_STATE_NORTH,
    DATA_STATE_EAST,
    DATA_STATE_SOUTH,
    DATA_STATE_WEST,
    DATA_GET_INACTIVE_PILLARS
};

enum eTypes
{
    TYPE_GET_PILLAR_POWER_LEVEL
};

enum eActivePillar
{
    PILLAR_NONE     = 0x00,
    PILLAR_NORTH    = 0x01,
    PILLAR_EAST     = 0x02,
    PILLAR_SOUTH    = 0x04,
    PILLAR_WEST     = 0x08
};

enum ePhases : uint8
{
    PHASE_INTRO,
    PHASE_1,
    PHASE_1_TO_2,
    PHASE_2,
    PHASE_2_TO_3,
    PHASE_3
};

enum eAreaTriggers
{
    QUARTER_AREA_NORTH  = 8910,
    QUARTER_AREA_EAST   = 8911,
    QUARTER_AREA_SOUTH  = 8912,
    QUARTER_AREA_WEST   = 8913
};

enum eWindows
{
    WINDOW_NW,
    WINDOW_NE,
    WINDOW_SE,
    WINDOW_SW,
    WINDOW_NO
};

enum eLeiShenWeapons
{
    WEAPON_THUNDER      = 94906, ///< Axe
    WEAPON_LIGHTNING    = 93236, ///< Polearm
};
#define LEI_SHEN_WEAPON 94906

Position const g_TeleportPosition = { 5709.973f, 4093.968f, 156.463f, 0.004256f };

uint32 const g_PillarState[4] =
{
    PILLAR_NORTH,
    PILLAR_EAST,
    PILLAR_SOUTH,
    PILLAR_WEST
};

uint32 const g_PillarEntries[4] =
{
    GOB_NORTH_PILLAR,
    GOB_EAST_PILLAR,
    GOB_SOUTH_PILLAR,
    GOB_WEST_PILLAR
};

uint32 const g_CosmeticTeleportSpells[4] =
{
    SPELL_COSMETIC_TELEPORT_N,
    SPELL_COSMETIC_TELEPORT_E,
    SPELL_COSMETIC_TELEPORT_S,
    SPELL_COSMETIC_TELEPORT_W
};

uint32 const g_PillarControllerEntries[4] =
{
    NPC_STATIC_SHOCK_CONDUIT,
    NPC_DIFFUSION_CHAIN_CONDUIT,
    NPC_OVERCHARGE_CONDUIT,
    NPC_BOUNCING_BOLT_CONDUIT
};

uint32 const g_PillarAltPowerEnabler[4] =
{
    SPELL_CONDUIT_LEVEL_NORTH,
    SPELL_CONDUIT_LEVEL_EAST,
    SPELL_CONDUIT_LEVEL_SOUTH,
    SPELL_CONDUIT_LEVEL_WEST
};

uint32 const g_PillarBonusSpells[4] =
{
    SPELL_STATIC_SHOCK_ACTIVE,
    SPELL_DIFFUSION_CHAIN_ACTIVE,
    SPELL_OVERCHARGE_ACTIVE,
    SPELL_BOUNCING_BOLT_ACTIVE
};

uint32 const g_PillarOverloadSpells[4] =
{
    SPELL_OVERLOAD_NORTH,
    SPELL_OVERLOAD_EAST,
    SPELL_OVERLOAD_SOUTH,
    SPELL_OVERLOAD_WEST
};

uint32 const g_PillarPowerLevel[3] =
{
    SPELL_METER_VISUAL_ENERGY,
    SPELL_METER_VISUAL_LEVEL_2,
    SPELL_METER_VISUAL_LEVEL_3
};

uint32 const g_ViolentGaleWindsSpells[4] =
{
    SPELL_VIOLENT_GALE_WINDS_AT_N,
    SPELL_VIOLENT_GALE_WINDS_AT_E,
    SPELL_VIOLENT_GALE_WINDS_AT_S,
    SPELL_VIOLENT_GALE_WINDS_AT_W
};

int32 const g_PillarActions[4] =
{
    ACTION_ENERGIZING_NORTH,
    ACTION_ENERGIZING_EAST,
    ACTION_ENERGIZING_SOUTH,
    ACTION_ENERGIZING_WEST
};

uint32 const g_FloorGobEntries[4] =
{
    GOB_FLOOR_NORTH,
    GOB_FLOOR_EAST,
    GOB_FLOOR_SOUTH,
    GOB_FLOOR_WEST
};

uint32 const g_Windows[4] =
{
    GOB_NW_WINDOW,
    GOB_NE_WINDOW,
    GOB_SE_WINDOW,
    GOB_SW_WINDOW
};

uint32 const g_WindFX[4] =
{
    GOB_WIND_NW_FX,
    GOB_WIND_NE_FX,
    GOB_WIND_SE_FX,
    GOB_WIND_SW_FX
};

uint32 const g_DespawnEntries[2] =
{
    NPC_OVERWHELMING_POWER,
    NPC_UNHARNESSED_POWER
};

#define MIN_DIST_TO_CONDUIT 30.0f

Position const g_OverwhelmingPowerSpawnPos[21] =
{
    { 5756.424f, 4051.331f, 139.0409f, 0.0f },
    { 5754.281f, 4138.183f, 138.6690f, 0.0f },
    { 5663.266f, 4136.861f, 139.0478f, 0.0f },
    { 5644.216f, 4055.576f, 139.5109f, 0.0f },
    { 5676.894f, 4023.767f, 140.5970f, 0.0f },
    { 5778.203f, 4130.346f, 140.7002f, 0.0f },
    { 5743.726f, 4023.006f, 140.0055f, 0.0f },
    { 5782.804f, 4062.145f, 140.0808f, 0.0f },
    { 5637.465f, 4125.247f, 140.5260f, 0.0f },
    { 5742.257f, 4166.458f, 140.4937f, 0.0f },
    { 5685.958f, 4174.120f, 140.0497f, 0.0f },
    { 5729.863f, 4008.781f, 139.5508f, 0.0f },
    { 5795.587f, 4113.360f, 140.2041f, 0.0f },
    { 5624.827f, 4075.135f, 139.7232f, 0.0f },
    { 5797.854f, 4077.292f, 140.2016f, 0.0f },
    { 5724.964f, 4183.833f, 140.3905f, 0.0f },
    { 5698.932f, 4001.160f, 139.8845f, 0.0f },
    { 5697.327f, 4185.682f, 139.8094f, 0.0f },
    { 5812.788f, 4092.969f, 141.1292f, 0.0f },
    { 5719.229f, 4079.484f, 279.6558f, 0.0f },
    { 5696.360f, 4085.275f, 279.6558f, 0.0f }
};

Position const g_QuadrantStalkerPositions[4] =
{
    { 5758.838f, 4093.563f, 156.5461f, 0.00000f },
    { 5709.310f, 4139.850f, 156.5461f, 0.00000f },
    { 5710.230f, 4047.400f, 156.5461f, 4.61843f },
    { 5663.930f, 4093.189f, 156.5461f, 0.00000f }
};

Position const g_PillarTeleportPositions[4] =
{
    { 5771.47f, 4093.66f, 156.463f, 0.01682f }, // North pillar
    { 5710.64f, 4031.69f, 156.463f, 4.70486f }, // East pillar
    { 5649.86f, 4094.47f, 156.463f, 3.15527f }, // South pillar
    { 5709.40f, 4152.05f, 156.463f, 1.48474f }  // West pillar
};

Position const g_WindsPositions[4] =
{
    { 5788.93f, 4172.87f, 156.46f, 5.497787f },  ///< NW Window
    { 5790.02f, 4016.60f, 156.46f, 0.785398f },  ///< NE Window
    { 5653.37f, 4012.12f, 156.46f, 2.356194f },  ///< SE Window
    { 5630.43f, 4171.33f, 156.46f, 3.926990f }   ///< SW Window
};

float const g_PillarLevelZ[2] =
{
    162.72f,
    167.00f
};

struct Line
{
    float m_Slope;
    float m_Intercept;
};

/// Descending lines
Line const m_LineA1 = { 1.0f, -1709.64f }; ///< NE edge for North and East squares (at the vertex of the main area)
Line const m_LineA2 = { 1.0f, -1697.25f }; ///< NE edge for Main Area square
Line const m_LineA3 = { 1.0f, -1649.31f }; ///< SW edge for North and East squares
Line const m_LineA4 = { 1.0f, -1589.05f }; ///< NE edge for South and West squares
Line const m_LineA5 = { 1.0f, -1541.40f }; ///< SW edge for Main Area square
Line const m_LineA6 = { 1.0f, -1528.71f }; ///< SW edge for South and West squares

/// Ascending lines
Line const m_LineB1 = { -1.0f, 9898.32f }; ///< NW edge for North and West squares
Line const m_LineB2 = { -1.0f, 9885.93f }; ///< NW edge for the Main Area square
Line const m_LineB3 = { -1.0f, 9837.99f }; ///< SE edge for the North and West squares
Line const m_LineB4 = { -1.0f, 9777.73f }; ///< NW edge for the South and East squares
Line const m_LineB5 = { -1.0f, 9730.08f }; ///< SE edge for the Main Area square
Line const m_LineB6 = { -1.0f, 9717.39f }; ///< SE edge for South and East squares

class OnPlatform
{
    /// WARNING: according to the "north" direction of the room, x and y axis are inverted. That is, if you go from "south" to "north",
    /// x will increase, and if you go from "east" to "west", y will increase.
    private:
        /// Return true if the point (p_X, P_Y) is above p_Line
        /// Reminder: a point is above a line if its y value > the y value of the point on the line with the same x value.
        bool static IsAbove(float p_X, float p_Y, Line p_Line) { return p_Y > GetPosYOnLine(p_X, p_Line); }
        bool static IsUnder(float p_X, float p_Y, Line p_Line) { return !IsAbove(p_X, p_Y, p_Line); }

        /// Return the y position of the p_Line's point with p_X as abscissa of this point
        /// Reminder: y = ax + b, where a is the slope and b, the intercept.
        float static GetPosYOnLine(float p_X, Line p_Line) { return p_Line.m_Slope * p_X + p_Line.m_Intercept; }

        bool static IsInWestQuarter(float p_PosX, float p_PosY)
        {
            /// Must be under A6 and B1, and above A4 and B3
            return IsUnder(p_PosX, p_PosY, m_LineA6) && IsUnder(p_PosX, p_PosY, m_LineB1) &&
                   IsAbove(p_PosX, p_PosY, m_LineA4) && IsAbove(p_PosX, p_PosY, m_LineB3);
        }

        bool static IsInSouthQuarter(float p_PosX, float p_PosY)
        {
            /// Must be under A6 and B4, and above A4 and B6
            return IsUnder(p_PosX, p_PosY, m_LineA6) && IsUnder(p_PosX, p_PosY, m_LineB4) &&
                   IsAbove(p_PosX, p_PosY, m_LineA4) && IsAbove(p_PosX, p_PosY, m_LineB6);
        }

        bool static IsInEastQuarter(float p_PosX, float p_PosY)
        {
            /// Must be under A3 and B4, and above A1 and B6
            return IsUnder(p_PosX, p_PosY, m_LineA3) && IsUnder(p_PosX, p_PosY, m_LineB4) &&
                   IsAbove(p_PosX, p_PosY, m_LineA1) && IsAbove(p_PosX, p_PosY, m_LineB6);
        }

        bool static IsInNorthQuarter(float p_PosX, float p_PosY)
        {
            /// Must be under A3 and B1, and above A1 and B3
            return IsUnder(p_PosX, p_PosY, m_LineA3) && IsUnder(p_PosX, p_PosY, m_LineB1) &&
                   IsAbove(p_PosX, p_PosY, m_LineA1) && IsAbove(p_PosX, p_PosY, m_LineB3);
        }

        bool static IsInMainArea(float p_PosX, float p_PosY)
        {
            /// Must be under A5 and B2, and above A2 and B5
            return IsUnder(p_PosX, p_PosY, m_LineA5) && IsUnder(p_PosX, p_PosY, m_LineB2) &&
                   IsAbove(p_PosX, p_PosY, m_LineA2) && IsAbove(p_PosX, p_PosY, m_LineB5);
        }

    public:
        bool static IsOnPlatform(Unit* p_Unit)
        {
            if (p_Unit->GetPositionZ() < 156.0f)
                return false;

            float l_PosX = p_Unit->GetPositionX();
            float l_PosY = p_Unit->GetPositionY();
            return (IsInMainArea(l_PosX, l_PosY) ||
                    IsInWestQuarter(l_PosX, l_PosY) || IsInSouthQuarter(l_PosX, l_PosY) ||
                    IsInEastQuarter(l_PosX, l_PosY) || IsInNorthQuarter(l_PosX, l_PosY));
        }
};

/// Lei Shen - 68397
class boss_lei_shen : public CreatureScript
{
    public:
        boss_lei_shen() : CreatureScript("boss_lei_shen") { }

        struct boss_lei_shenAI : public BossAI
        {
            boss_lei_shenAI(Creature* p_Creature) : BossAI(p_Creature, DATA_LEI_SHEN)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;

            uint32 m_TeleportTimer;     ///< Timer for teleport (intro)
            uint32 m_CosmeticTimer;     ///< Timer for lightning on pillar after teleport (intro)
            uint32 m_PillarSpell;       ///< Current Pillar Ability to use
            uint8  m_PillarState;       ///< Pillar Lei Shen is on (intro)
            uint8  m_ActivePillar;      ///< Current pillar energized by Lei Shen; if none (0), Lei shen cast Discharged Energy
            uint8  m_Phase;             ///< Fight phase
            uint8  m_InactivePillars;   ///< Pillar which have been disabled and which zone is impassable
            uint8  m_OpenedWindow;      ///< Opened Window on Violent Gale Winds
            float  m_NextHealthPct;     ///< Next health level for transition phase
            bool   m_HasConduit;        ///< True if Lei Shen is near a pillar

            void Reset()
            {
                m_Events.Reset();

                _Reset();

                SetEquipmentSlots(false, WEAPON_THUNDER, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                me->SetReactState(REACT_PASSIVE);

                me->ReenableEvadeMode();
                me->RemoveAllAreasTrigger();

                m_TeleportTimer   = 2000;
                m_CosmeticTimer   = 0;
                m_PillarState     = DATA_STATE_NORTH;
                m_ActivePillar    = 0;
                m_Phase           = PHASE_INTRO;
                m_InactivePillars = PILLAR_NONE;
                m_NextHealthPct   = 65.0f;
                m_HasConduit      = false;
                m_OpenedWindow    = WINDOW_NO;

                if (m_Instance)
                {
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DISCHARGED_ENERGY);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERCHARGE_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERLOAD_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_POWER_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WINDBURN);
                }
            }

            void KilledUnit(Unit* p_Who)
            {
                if (p_Who->GetTypeId() != TYPEID_PLAYER)
                    return;

                Talk(TALK_SLAY);
            }

            void DamageTaken(Unit* p_Attacker, uint32& p_Damage, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_LEI_SHEN) != IN_PROGRESS)
                    {
                        EnterCombat(p_Attacker);
                        /// EnterCombat should have turn BossState to In_Progress if all is correct
                        if (m_Instance->GetBossState(DATA_LEI_SHEN) != IN_PROGRESS)
                        {
                            p_Damage = 0;
                            if (Creature* l_Pillar = Creature::GetCreature(*me, g_PillarControllerEntries[m_ActivePillar]))
                                me->CastSpell(l_Pillar, SPELL_PILLAR_IN_CHARGE_VISUAL, true);
                            return;
                        }
                    }
                    else
                        return;
                }

                if (m_Phase == PHASE_1 && me->HealthBelowPctDamaged(m_NextHealthPct, p_Damage))
                {
                    m_NextHealthPct = 30.0f;
                    m_Phase = PHASE_1_TO_2;
                    DoAction(ACTION_SET_TO_TRANSITION_PHASE);
                }
                else if (m_Phase == PHASE_2 && me->HealthBelowPctDamaged(m_NextHealthPct, p_Damage))
                {
                    m_NextHealthPct = 0.0f;
                    m_Phase = PHASE_2_TO_3;
                    DoAction(ACTION_SET_TO_TRANSITION_PHASE);
                }
            }

            void DamageDealt(Unit* p_Victim, uint32& /*p_Damage*/, DamageEffectType p_DamageType)
            {
                if (m_Phase != PHASE_3 || p_DamageType != DIRECT_DAMAGE)
                    return;

                me->CastSpell(p_Victim, SPELL_OVERWHELMING_POWER_START, true);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_LEI_SHEN) == IN_PROGRESS)
                    return;

                if (!m_Instance->CheckRequiredBosses(DATA_LEI_SHEN))
                {
                    EnterEvadeMode();
                    return;
                }

                Talk(TALK_AGGRO);

                m_TeleportTimer = 0;
                m_Phase = PHASE_1;

                m_Instance->SetBossState(DATA_LEI_SHEN, IN_PROGRESS);
                m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, me);
                me->SetReactState(REACT_AGGRESSIVE);

                for (uint8 l_Iter = DATA_STATE_NORTH; l_Iter <= DATA_STATE_WEST; ++l_Iter)
                {
                    if (Creature* l_PillarControler = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[l_Iter])))
                    {
                        m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_ENGAGE, l_PillarControler, 3);
                        l_PillarControler->AI()->DoAction(ACTION_START_ENERGIZE);
                    }
                }

                SpawnOverwhelmingPowers();
                SpawnQuadrantStalkers();

                m_Events.ScheduleEvent(EVENT_CHECK_NEAR_CONDUIT, 1000);
                m_Events.ScheduleEvent(EVENT_DECAPITATE, 40000);
                m_Events.ScheduleEvent(EVENT_THUNDERSTRUCK, 25000);
                m_Events.ScheduleEvent(EVENT_BERSERKER, 900000);
                m_Events.ScheduleEvent(EVENT_CRASHING_THUNDER, 6000);

                me->SetInCombatWith(p_Attacker);
                AttackStart(p_Attacker);
            }

            void EnterEvadeMode()
            {
                if (!m_Instance)
                    return;

                me->SetReactState(REACT_PASSIVE);

                if (me->isInCombat())
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

                if (m_Instance->GetBossState(DATA_LEI_SHEN) != IN_PROGRESS)
                    return;

                m_Events.Reset();
                summons.DespawnAll();
                DespawnAtEnd();

                m_Instance->SetBossState(DATA_LEI_SHEN, FAIL);
                m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DISCHARGED_ENERGY);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERCHARGE_AURA);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERLOAD_AURA);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_POWER_AURA);
                m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WINDBURN);

                /// Disabling Pillars
                for (uint8 l_Iter = DATA_STATE_NORTH; l_Iter <= DATA_STATE_WEST; ++l_Iter)
                {
                    if (Creature* l_PillarControler = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[l_Iter])))
                    {
                        m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, l_PillarControler, 0);
                        l_PillarControler->AI()->DoAction(ACTION_RESTART_PILLAR);
                        l_PillarControler->RemoveAura(SPELL_CONDUIT_LEVEL_VISUAL);
                    }
                }

                DisableFloors();
                CloseChargingStation();

                me->SetReactState(REACT_PASSIVE);
                me->SetFullHealth();
                me->RemoveAllAuras();
                me->RemoveAllAreasTrigger();
                me->RemoveAllDynObjects();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);
                me->ClearUnitState(UNIT_STATE_ROOT);

                SealWindow();
                Reset();
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                _JustDied();
                m_Events.Reset();
                summons.DespawnAll();
                DespawnAtEnd();

                Talk(TALK_DEATH);

                me->RemoveAllAreasTrigger();

                if (m_Instance)
                {
                    m_Instance->SetBossState(DATA_LEI_SHEN, DONE);
                    m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, me);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DISCHARGED_ENERGY);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERCHARGE_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERLOAD_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_OVERWHELMING_POWER_AURA);
                    m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_WINDBURN);

                    for (uint8 l_Iter = DATA_STATE_NORTH; l_Iter <= DATA_STATE_WEST; ++l_Iter)
                    {
                        if (Creature* l_PillarControler = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[l_Iter])))
                        {
                            m_Instance->SendEncounterUnit(ENCOUNTER_FRAME_DISENGAGE, l_PillarControler, 0);
                            l_PillarControler->AI()->DoAction(ACTION_RESTART_PILLAR);
                            l_PillarControler->RemoveAura(SPELL_CONDUIT_LEVEL_VISUAL);
                        }
                    }
                }

                DisableFloors();
                CloseChargingStation();
                SealWindow();
                m_InactivePillars = 0; ///< Prevent Inactive quarters to still damage players once the boss is dead

                me->RemoveAllAuras();
                me->RemoveAllAreasTrigger();
                me->RemoveAllDynObjects();
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IN_COMBAT);

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
                            if (l_DungeonID != 613) ///< Fourth Part
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
                    case ACTION_TELEPORT_TO_NEXT_PILLAR:
                    {
                        me->CastSpell(me, g_CosmeticTeleportSpells[m_PillarState], false);
                        m_CosmeticTimer = 2000;
                        break;
                    }
                    case ACTION_REMOVE_EQUIPMENT:
                        SetEquipmentSlots(false, EQUIP_UNEQUIP, EQUIP_NO_CHANGE, EQUIP_NO_CHANGE);
                        break;
                    case ACTION_SET_EQUIPMENT:
                        /// Phase 1: Thunder, Phase 2: Lightning, Phase 3: Thunder + Lightning
                        SetEquipmentSlots(false, m_Phase == PHASE_2 ? (int32)WEAPON_LIGHTNING : (int32)WEAPON_THUNDER,
                                                 m_Phase == PHASE_3 ? (int32)WEAPON_LIGHTNING : (int32)EQUIP_NO_CHANGE,
                                                 EQUIP_NO_CHANGE);
                        break;
                    case ACTION_SET_TO_TRANSITION_PHASE:
                    {
                        Talk(TALK_PHASE_2A);
                        m_Events.Reset();
                        me->AttackStop();
                        me->DeleteThreatList();
                        me->SetReactState(REACT_PASSIVE);
                        me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->CastSpell(me, SPELL_DISPLACE, true);
                        me->AddUnitState(UNIT_STATE_ROOT);
                        m_Events.ScheduleEvent(EVENT_ACTIVATING_CENTRAL_CONSOLE, 1000);
                        m_Events.ScheduleEvent(EVENT_TRANSITION_PHASE_END, 45000);
                        break;
                    }
                    case ACTION_STATIC_SHOCK:
                    {
                        uint32 l_PillarLevel = GetPillarLevel(DATA_STATE_NORTH);

                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                        if (l_PlayerList.size() > l_PillarLevel)
                            JadeCore::RandomResizeList(l_PlayerList, l_PillarLevel);

                        for (Player* l_Target : l_PlayerList)
                            me->AddAura(SPELL_STATIC_SHOCK, l_Target);
                        break;
                    }
                    case ACTION_DIFFUSION_CHAIN:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_RANDOM))
                            me->CastSpell(l_Target, SPELL_DIFFUSION_CHAIN_DAMAGE, true);
                        break;
                    case ACTION_OVERCHARGE:
                    {
                        uint32 l_PillarLevel = GetPillarLevel(DATA_STATE_SOUTH);

                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                        if (l_PlayerList.size() > l_PillarLevel)
                            JadeCore::RandomResizeList(l_PlayerList, l_PillarLevel);

                        for (Player* l_Target : l_PlayerList)
                            me->AddAura(SPELL_OVERCHARGE_AURA, l_Target);
                        break;
                    }
                    case ACTION_BOUNCING_BOLT:
                    {
                        uint32 l_PillarLevel = GetPillarLevel(DATA_STATE_WEST);

                        std::list<Creature*> l_QuadrantList;
                        GetCreatureListWithEntryInGrid(l_QuadrantList, me, NPC_QUADRANT_STALKER, 100.0f);

                        if (l_QuadrantList.size() > l_PillarLevel)
                            JadeCore::RandomResizeList(l_QuadrantList, l_PillarLevel);

                        for (Creature* l_Target : l_QuadrantList)
                            me->CastSpell(l_Target, SPELL_BOUNCING_BOLT_MISSILE, true);
                        break;
                    }
                    /// -- Activating Pillar --
                    case ACTION_ENERGIZING_NORTH:
                    case ACTION_ENERGIZING_EAST:
                    case ACTION_ENERGIZING_SOUTH:
                    case ACTION_ENERGIZING_WEST:
                        me->RemoveAura(SPELL_DISCHARGED_ENERGY_AT);
                        me->RemoveAreaTrigger(SPELL_DISCHARGED_ENERGY_AT);
                        if (m_Instance)
                            m_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_DISCHARGED_ENERGY);
                        m_ActivePillar = p_Action - ACTION_ENERGIZING_NORTH + 1;
                        if (!m_HasConduit)
                            m_HasConduit = true;
                        m_Events.ScheduleEvent(EVENT_PILLAR_ABILITY, 11000);
                        break;
                    /// -- Deactivating pillar --
                    case -ACTION_ENERGIZING_NORTH:
                    case -ACTION_ENERGIZING_EAST:
                    case -ACTION_ENERGIZING_SOUTH:
                    case -ACTION_ENERGIZING_WEST:
                    {
                        /// Only trigger Discharged Energy if action is triggered by current conduit, which means that Lei Shen
                        /// has left it and isn't on a pillar anymore
                        if (int32(m_ActivePillar + ACTION_ENERGIZING_NORTH + 1) != -p_Action)
                            break;

                        me->CastSpell(me, SPELL_DISCHARGED_ENERGY_AT, true);
                        me->RemoveAura(SPELL_PILLAR_IN_CHARGE_VISUAL);
                        me->CastStop();

                        if (m_Instance)
                        {
                            if (GameObject* l_Floor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_FloorGobEntries[m_ActivePillar - 1])))
                                l_Floor->SetGoState(GO_STATE_READY);
                        }

                        m_HasConduit = false;
                        m_ActivePillar = 0;

                        m_Events.CancelEvent(EVENT_PILLAR_ABILITY);
                        break;
                    }
                    case ACTION_WIND_END:
                    {
                        if (!m_Instance)
                            break;

                        /// Close the window
                        if (GameObject* l_Window = GameObject::GetGameObject(*me, m_Instance->GetData64(g_Windows[m_OpenedWindow])))
                            l_Window->SetGoState(GO_STATE_READY);

                        /// Stop the wind FX on floor
                        if (GameObject* l_WindFX = GameObject::GetGameObject(*me, m_Instance->GetData64(g_WindFX[m_OpenedWindow])))
                            l_WindFX->SetGoState(GO_STATE_READY);

                        m_OpenedWindow = WINDOW_NO;
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Data)
            {
                if (p_Data == DATA_GET_INACTIVE_PILLARS)
                    return m_InactivePillars;

                return 0;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                /// Wipe
                if (m_Instance)
                {
                    if (m_Instance->IsWipe() && m_Instance->GetBossState(DATA_LEI_SHEN) == IN_PROGRESS)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                /// Intro: Teleport
                if (m_TeleportTimer)
                {
                    if (m_TeleportTimer <= p_Diff)
                    {
                        m_TeleportTimer = 14000;

                        DoAction(ACTION_TELEPORT_TO_NEXT_PILLAR);
                    }
                    else
                        m_TeleportTimer -= p_Diff;
                }

                /// Intro: Cosmetic
                if (m_CosmeticTimer && m_Instance)
                {
                    if (m_CosmeticTimer <= p_Diff)
                    {
                        m_CosmeticTimer = 0;

                        if (Creature* l_PillarControler = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[m_PillarState])))
                            me->CastSpell(l_PillarControler, SPELL_COSMETIC_VISUAL, false);

                        m_PillarState = ++m_PillarState % (DATA_STATE_WEST + 1);
                    }
                    else
                        m_CosmeticTimer -= p_Diff;
                }

                UpdateVictim();
                CheckPositionOnPlatform();

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    /// --- Generic events ---
                    case EVENT_PILLAR_ABILITY:
                        if (!m_HasConduit)
                            break;
                        DoAction(ACTION_STATIC_SHOCK + m_PillarState);
                        m_Events.ScheduleEvent(EVENT_PILLAR_ABILITY, 40000);
                        break;
                    case EVENT_BERSERKER:
                        me->CastSpell(me, SPELL_BERSERK, true);
                        Talk(TALK_BERSERK);
                        break;
                    /// --- Phase 1 ---
                    case EVENT_DECAPITATE:
                        if (Unit* l_Target = SelectTarget(SELECT_TARGET_TOPAGGRO))
                            me->CastSpell(l_Target, SPELL_DECAPITATE_AURA, true);
                        m_Events.ScheduleEvent(EVENT_DECAPITATE, 50000);
                        break;
                    case EVENT_THUNDERSTRUCK:
                        me->CastSpell(me, SPELL_THUNDERSTRUCK_SEARCH, true);
                        me->MonsterTextEmote("Lei Shen begins to cast |cFFF00000|Hspell:135095|h[Thunderstruck]|h|r! Run!", 0, true);
                        Talk(TALK_SPELL_1);
                        m_Events.ScheduleEvent(EVENT_THUNDERSTRUCK, 46000);
                        break;
                    case EVENT_CRASHING_THUNDER:
                        me->CastSpell(me, SPELL_CRASHING_THUNDER_AURA, true);
                        m_Events.ScheduleEvent(EVENT_CRASHING_THUNDER, 30000);
                        break;
                    /// --- Transition Phases ---
                    case EVENT_ACTIVATING_CENTRAL_CONSOLE:
                    {
                        me->CastSpell(me, SPELL_SUPERCHARGE_AURA, true);
                        /// Triggering all four pillar abilities
                        for (uint8 l_Data = DATA_STATE_NORTH; l_Data <= DATA_STATE_WEST; ++l_Data)
                            DoAction(ACTION_STATIC_SHOCK + l_Data);

                        if (m_Instance)
                        {
                            if (GameObject* l_Station = m_Instance->instance->GetGameObject(m_Instance->GetData64(GOB_CHARGING_STATION)))
                                l_Station->SetGoState(GO_STATE_ACTIVE);
                        }
                        break;
                    }
                    case EVENT_TRANSITION_PHASE_END:
                    {
                        if (!m_Instance)
                            return;

                        ++m_Phase;
                        Creature* l_PillarToDeactivate = nullptr;
                        uint32 l_MaxPower = 0;

                        /// Picking pillar with max power
                        uint8 l_Data = DATA_STATE_NORTH;
                        for (uint8 l_Itr = 0; l_Itr <= DATA_STATE_WEST; ++l_Itr)
                        {
                            uint64 l_PillarGuid = m_Instance->GetData64(g_PillarControllerEntries[l_Itr]);
                            if (Creature* l_Pillar = Creature::GetCreature(*me, l_PillarGuid))
                            {
                                uint32 l_PowerLevel = (l_Pillar->AI()->GetData(TYPE_GET_PILLAR_POWER_LEVEL) * 100) + l_Pillar->GetPower(POWER_ENERGY);
                                if (l_PowerLevel > l_MaxPower)
                                {
                                    l_PillarToDeactivate = l_Pillar;
                                    l_MaxPower = l_PowerLevel;
                                    l_Data = l_Itr;
                                }
                            }
                        }

                        if (!l_PillarToDeactivate)
                            break;

                        /// Deactivating pillar and floor associated to 
                        l_PillarToDeactivate->AI()->DoAction(ACTION_DEACTIVATE_PILLAR);
                        DisableFloors(l_Data);
                        m_InactivePillars |= g_PillarState[l_Data];

                        /// Abilities in P2
                        if (m_Phase == PHASE_2)
                        {
                            m_Events.ScheduleEvent(EVENT_FUSION_SLASH, 30000);
                            m_Events.ScheduleEvent(EVENT_SUMMON_BALL_LIGHTNING, 40000);
                            m_Events.ScheduleEvent(EVENT_LIGHTNING_WHIP, 4000);
                        }
                        /// Abilities in P3
                        else
                        {
                            m_Events.ScheduleEvent(EVENT_THUNDERSTRUCK, 23000);
                            m_Events.ScheduleEvent(EVENT_LIGHTNING_WHIP, 4000);
                            m_Events.ScheduleEvent(EVENT_SUMMON_BALL_LIGHTNING, 45500);
                            m_Events.ScheduleEvent(EVENT_VIOLENT_GALE_WINDS, 18000);
                            m_Events.ScheduleEvent(EVENT_CRASHING_THUNDER, 30000);

                            for (uint8 l_Idx = 0; l_Idx < 35; ++l_Idx)
                            {
                                float l_Ori  = frand(0.0f, 6.28f);
                                float l_Dist = frand(2.0f, 15.0f);
                                float l_PosX = me->GetPositionX() + l_Dist * cos(l_Ori);
                                float l_PosY = me->GetPositionY() + l_Dist * sin(l_Ori);

                                me->SummonCreature(NPC_OVERWHELMING_POWER, l_PosX, l_PosY, me->GetPositionZ());
                            }

                            me->CastSpell(me, SPELL_OVERWHELMING_POWER_START, true);
                        }

                        if (Player* l_Target = me->FindNearestPlayer(50.0f))
                        {
                            AttackStart(l_Target);
                            me->SetInCombatWith(l_Target);
                        }

                        me->SetReactState(REACT_AGGRESSIVE);
                        me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        me->ClearUnitState(UNIT_STATE_ROOT);

                        if (GameObject* l_Station = m_Instance->instance->GetGameObject(m_Instance->GetData64(GOB_CHARGING_STATION)))
                            l_Station->SetGoState(GO_STATE_READY);

                        break;
                    }
                    /// --- Phase 2 ---
                    case EVENT_FUSION_SLASH:
                        if (Unit* l_Target = me->getVictim())
                            me->CastSpell(l_Target, SPELL_FUSION_SLASH, true);
                        m_Events.ScheduleEvent(EVENT_FUSION_SLASH, 30000);
                        break;
                    case EVENT_SUMMON_BALL_LIGHTNING:
                    {
                        Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();
                        for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                        {
                            if (roll_chance_i(75))
                                me->CastSpell(l_Itr->getSource(), SPELL_SUMMON_BALL_LIGHTNING, false);
                        }

                        m_Events.ScheduleEvent(EVENT_SUMMON_BALL_LIGHTNING, 45500);
                        break;
                    }
                    case EVENT_LIGHTNING_WHIP:
                    {
                        float l_Ori = frand(0.0f, M_PI * 2);
                        me->SetFacingTo(l_Ori);
                        me->SetOrientation(l_Ori);
                        me->CastSpell(me, SPELL_LIGHTNING_WHIP_HIT, false);
                        m_Events.ScheduleEvent(EVENT_LIGHTNING_WHIP, 45500);
                        break;
                    }
                    /// --- Phase 3 ---
                    case EVENT_VIOLENT_GALE_WINDS:
                    {
                        m_OpenedWindow = urand(DATA_STATE_NORTH, DATA_STATE_WEST);

                        std::ostringstream l_DoorText;
                        l_DoorText << "The |cFFFF0000|h[";
                        switch (m_OpenedWindow)
                        {
                            case DATA_STATE_NORTH:
                                l_DoorText << "north-west";
                                break;
                            case DATA_STATE_EAST:
                                l_DoorText << "north-east";
                                break;
                            case DATA_STATE_SOUTH:
                                l_DoorText << "south-east";
                                break;
                            case DATA_STATE_WEST:
                                l_DoorText << "south-west";
                                break;
                            default:
                                break;
                        }

                        l_DoorText << "]|h|rbegin to open, causing |cFFFF0000|Hspell:136889|h[Violent Gale Winds]|h|r!";
                        
                        me->MonsterTextEmote(l_DoorText.str().c_str(), 0, true);
                        me->CastSpell(me, g_ViolentGaleWindsSpells[m_OpenedWindow], true);

                        if (m_Instance)
                        {
                            if (GameObject* l_DoorObject = GameObject::GetGameObject(*me, m_Instance->GetData64(g_Windows[m_OpenedWindow])))
                                l_DoorObject->SetGoState(GO_STATE_ACTIVE);

                            if (GameObject* l_WindFX = GameObject::GetGameObject(*me, m_Instance->GetData64(g_WindFX[m_OpenedWindow])))
                                l_WindFX->SetGoState(GO_STATE_ACTIVE);
                        }

                        m_Events.ScheduleEvent(EVENT_VIOLENT_GALE_WINDS, 30500);

                        break;
                    }
                    default:
                        break;
                }

                DoMeleeAttackIfReady();
            }

            uint32 GetPillarLevel(uint8 p_Data)
            {
                if (m_Instance)
                {
                    if (Creature* l_Pillar = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[p_Data])))
                        return l_Pillar->AI()->GetData(TYPE_GET_PILLAR_POWER_LEVEL);
                }

                return 1;
            }

            void SpawnOverwhelmingPowers()
            {
                for (uint8 l_Iter = 0; l_Iter < 21; ++l_Iter)
                    me->SummonCreature(NPC_OVERWHELMING_POWER, g_OverwhelmingPowerSpawnPos[l_Iter]);
            }

            void SpawnQuadrantStalkers()
            {
                for (uint8 l_Iter = 0; l_Iter < 4; ++l_Iter)
                    me->SummonCreature(NPC_QUADRANT_STALKER, g_QuadrantStalkerPositions[l_Iter]);
            }

            void DisableFloors(uint8 p_Data = 4)
            {
                /// Disabling only one floor
                if (m_Instance)
                {
                    if (p_Data <= DATA_STATE_WEST)
                    {
                        if (GameObject* l_Floor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_FloorGobEntries[p_Data])))
                            l_Floor->SetGoState(GO_STATE_ACTIVE);

                        if (GameObject* l_Pillar = GameObject::GetGameObject(*me, m_Instance->GetData64(g_PillarEntries[p_Data])))
                            l_Pillar->SetGoState(GO_STATE_READY);

                        if (Creature* l_Pillar = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[p_Data])))
                            l_Pillar->CastSpell(l_Pillar, g_PillarOverloadSpells[p_Data], true);

                        return;
                    }

                    /// Disabling all floors
                    for (uint8 l_Iter = DATA_STATE_NORTH; l_Iter <= DATA_STATE_WEST; ++l_Iter)
                    {
                        if (GameObject* l_Floor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_FloorGobEntries[l_Iter])))
                            l_Floor->SetGoState(GO_STATE_READY);
                    }
                }
            }

            void DeactivatingPillars()
            {
                uint8 l_PillarState[4] = { PILLAR_NORTH, PILLAR_EAST, PILLAR_SOUTH, PILLAR_WEST };
                /// if already disabled
                for (uint8 l_Data = 0; l_Data < 4; ++l_Data)
                {
                    /// Pillar already disabled
                    if (m_InactivePillars & l_PillarState[l_Data])
                        continue;
                    /// Active pillar to disable
                    else
                    {
                        if (m_Instance)
                        {
                            if (Creature* l_Pillar = Creature::GetCreature(*me, m_Instance->GetData64(g_PillarControllerEntries[l_Data])))
                                l_Pillar->AI()->DoAction(ACTION_DEACTIVATE_PILLAR);

                            if (GameObject* l_PillarObj = GameObject::GetGameObject(*me, m_Instance->GetData64(g_PillarEntries[l_Data])))
                                l_PillarObj->SetGoState(GO_STATE_READY);
                        }
                    }
                }
            }

            void DespawnAtEnd()
            {
                for (uint8 l_Idx = 0; l_Idx < 2; ++l_Idx)
                {
                    std::list<Creature*> l_AddList;
                    GetCreatureListWithEntryInGrid(l_AddList, me, g_DespawnEntries[l_Idx], 200.0f);

                    for (Creature* l_Add : l_AddList)
                        l_Add->DespawnOrUnsummon();
                }
            }

            void CloseChargingStation()
            {
                if (m_Instance)
                {
                    if (GameObject* l_Station = m_Instance->instance->GetGameObject(m_Instance->GetData64(GOB_CHARGING_STATION)))
                        l_Station->SetGoState(GO_STATE_READY);
                }
            }

            void SealWindow()
            {
                if (m_OpenedWindow == WINDOW_NO)
                    return;

                if (m_Instance)
                {
                    if (GameObject* l_Window = GameObject::GetGameObject(*me, m_Instance->GetData64(g_Windows[m_OpenedWindow])))
                        l_Window->SetGoState(GO_STATE_READY);
                }
            }

            void CheckPositionOnPlatform()
            {
                /// Boss out of platform
                if (!OnPlatform::IsOnPlatform(me))
                {
                    /// Trying to teleport to nearest pillar
                    if (Creature* l_Stalker = GetClosestCreatureWithEntry(me, NPC_QUADRANT_STALKER, 200.0f))
                        me->NearTeleportTo(l_Stalker->m_positionX, l_Stalker->m_positionY, l_Stalker->m_positionZ, l_Stalker->m_orientation);
                    /// No pillar found (WTF?!), teleporting to the center of the room.
                    else
                        me->NearTeleportTo(g_TeleportPosition);
                }

                /// If current target falls from platform, remove him from threat list to avoid Lei Shen follows him.
                if (Unit* l_Target = me->getVictim())
                {
                    if (!OnPlatform::IsOnPlatform(l_Target))
                    {
                        //me->getThreatManager().modifyThreatPercent(l_Target, -100);
                        me->getThreatManager().getCurrentVictim()->removeReference();
                        /// Picking a new target
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 50.0f);
                        l_PlayerList.remove_if([this](Player* p_Player) -> bool
                        {
                            if (!OnPlatform::IsOnPlatform(p_Player))
                                return true;
                            return false;
                        });

                        if (!l_PlayerList.empty())
                        {
                            l_PlayerList.sort(JadeCore::DistanceCompareOrderPred(me));
                            AttackStart(l_PlayerList.front());
                        }
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_lei_shenAI(p_Creature);
        }
};

/// Thunder Trap - 69825
class mob_thunder_trap : public CreatureScript
{
    public:
        mob_thunder_trap() : CreatureScript("mob_thunder_trap") { }

        struct mob_thunder_trapAI : public ScriptedAI
        {
            mob_thunder_trapAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Events.Reset();
            }

            EventMap m_Events;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ACTIVATE_TRAP:
                        m_Events.ScheduleEvent(EVENT_THUNDER_TRAP, 100);
                        break;
                    default:
                        break;
                }
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING))
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_THUNDER_TRAP:
                        me->CastSpell(me, SPELL_THUNDER_TRAP, false);
                        m_Events.ScheduleEvent(EVENT_THUNDER_TRAP, 100);
                        break;
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_thunder_trapAI(p_Creature);
        }
};

/// Static Shock Conduit - 68398 - North
/// Diffusion Chain Conduit - 68696 - East
/// Overcharge Conduit - 68697 - South
/// Bouncing Bolt Conduit - 68698 - West
class mob_pillar_controler : public CreatureScript
{
    public:
        mob_pillar_controler() : CreatureScript("mob_pillar_controler") { }

        struct mob_pillar_controlerAI : public ScriptedAI
        {
            mob_pillar_controlerAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
                m_Rising = 0.07f;
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint8 m_PowerLevel;
            uint8 m_Data;
            float m_Rising;
            float m_PosZ;
            bool m_IsEnergized;
            bool m_IsActive;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE);
                me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK6 | UNIT_FLAG2_REGENERATE_POWER);
                me->AddUnitMovementFlag(MOVEMENTFLAG_DISABLE_GRAVITY);

                me->SetFloatValue(UNIT_FIELD_POWER_REGEN_FLAT_MODIFIER, -10.0f);
                me->SetFloatValue(UNIT_FIELD_POWER_REGEN_INTERRUPTED_FLAT_MODIFIER, -10.0f);
                me->SetUInt32Value(UNIT_FIELD_MOUNT_DISPLAY_ID, 50331648);

                me->SetReactState(REACT_PASSIVE);
                me->AddUnitState(UNIT_STATE_ROOT);
                me->ReenableEvadeMode();

                me->setPowerType(POWER_ENERGY);
                me->SetPower(POWER_ENERGY, 0);
                me->SetMaxPower(POWER_ENERGY, 100);

                m_PowerLevel = 0;
                m_Data = 0;
                m_IsEnergized = false;
                m_IsActive = false;
                m_PosZ = me->GetHomePosition().m_positionZ;

                for (uint8 l_Iter = DATA_STATE_NORTH; l_Iter <= DATA_STATE_WEST; ++l_Iter)
                {
                    if (me->GetEntry() == g_PillarControllerEntries[l_Iter])
                    {
                        m_Data = l_Iter;
                        break;
                    }
                }

                me->CastSpell(me, g_PillarAltPowerEnabler[m_Data], true);
                me->CastSpell(me, SPELL_METER_VISUAL_ENERGY, true);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_START_ENERGIZE:
                    {
                        m_Events.ScheduleEvent(EVENT_CHECK_LEI_SHEN_PILLAR, 1000);
                        m_IsActive = true;
                        break;
                    }
                    /// On Resetting encounter (wipe/just died)
                    case ACTION_RESTART_PILLAR:
                    {
                        m_Events.Reset();
                        me->RemoveAllAuras();

                        if (m_Instance)
                        {
                            if (Creature* l_LeiShen = Creature::GetCreature(*me, m_Instance->GetData64(NPC_LEI_SHEN)))
                                l_LeiShen->RemoveAura(SPELL_PILLAR_IN_CHARGE_VISUAL);

                            if (GameObject* l_Floor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_FloorGobEntries[m_Data])))
                                l_Floor->SetGoState(GO_STATE_READY);
                        }

                        me->RemoveDynObject(SPELL_METER_VISUAL_LEVEL_2);
                        me->RemoveDynObject(SPELL_METER_VISUAL_LEVEL_3);
                        me->SetFlag(UNIT_FIELD_FLAGS_2, UNIT_FLAG2_UNK6 | UNIT_FLAG2_REGENERATE_POWER);

                        /// Need to TP (as the mob is root, MoveTargetedHome won't work here)
                        me->NearTeleportTo(me->GetHomePosition());

                        me->SetPower(POWER_ENERGY, 0);
                        m_PowerLevel = 0;
                        m_IsEnergized = false;
                        m_IsActive = false;
                        // Visual
                        me->CastSpell(me, g_PillarAltPowerEnabler[m_Data], true);
                        me->CastSpell(me, SPELL_METER_VISUAL_ENERGY, true);
                        break;
                    }
                    /// When pillar is deactivated at transition's end
                    case ACTION_DEACTIVATE_PILLAR:
                    {
                        me->RemoveAllAuras();
                        me->RemoveDynObject(SPELL_METER_VISUAL_LEVEL_2);
                        me->RemoveDynObject(SPELL_METER_VISUAL_LEVEL_3);
                        m_PowerLevel = 0;
                        me->SetPower(POWER_ENERGY, 0);
                        m_Events.Reset();
                        m_IsEnergized = false;
                        m_IsActive = false;
                        break;
                    }
                    /// When a pillar reach a new power level, its power is reset to 0
                    case ACTION_RESET_PILLAR_ENERGY:
                    {
                        me->SetPower(POWER_ENERGY, 0);
                        break;
                    }
                    /// When Lei Shen is away from the pillar, it's not energized anymore
                    case ACTION_STOP_ENERGIZE:
                    {
                        m_IsEnergized = false;
                        if (m_Instance)
                        {
                            if (GameObject* l_Floor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_FloorGobEntries[m_Data])))
                                l_Floor->SetGoState(GO_STATE_READY);
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void PowerModified(Powers p_Power, int32 p_Value)
            {
                if (p_Power != POWER_ENERGY || !m_IsEnergized)
                    return;

                // Getting higher to make the energy rise
                m_PosZ += m_Rising;
                me->NearTeleportTo(me->GetPositionX(), me->GetPositionY(), m_PosZ, me->GetOrientation());

                if (p_Value >= me->GetMaxPower(POWER_ENERGY))
                {
                    /// When a conduit reaches 100 Energy, it levels up.
                    /// Its Energy is reset to 0, and each level that it gains causes the ability granted to Lei Shen ...
                    /// ... to be more dangerous in some way.
                    if (m_PowerLevel < 2)
                    {
                        me->CastSpell(me->GetPositionX(), me->GetPositionY(), g_PillarLevelZ[m_PowerLevel], SPELL_METER_VISUAL_LEVEL_2 + m_PowerLevel, true);
                        DoAction(ACTION_RESET_PILLAR_ENERGY);
                        ++m_PowerLevel;

                        if (Aura* l_PowerAura = me->GetAura(g_PillarAltPowerEnabler[m_Data]))
                            l_PowerAura->SetStackAmount(l_PowerAura->GetStackAmount() + 1);
                    }

                    me->SetPower(POWER_ENERGY, 0);
                }

                Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator l_Iter = l_PlayerList.begin(); l_Iter != l_PlayerList.end(); ++l_Iter)
                {
                    if (Player* l_Player = l_Iter->getSource())
                        l_Player->SetPower(POWER_ALTERNATE_POWER, p_Value);
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_GET_PILLAR_POWER_LEVEL)
                    return m_PowerLevel;
                return 0;
            }

            void UpdateAI(const uint32 p_Diff)
            {
                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_LEI_SHEN_PILLAR:
                    {
                        if (!m_IsActive)
                            return;

                        if (m_Instance)
                        {
                            if (Creature* l_LeiShen = Creature::GetCreature(*me, m_Instance->GetData64(NPC_LEI_SHEN)))
                            {
                                /// Check if Lei Shen is close enough
                                if (me->GetDistance2d(l_LeiShen) < MIN_DIST_TO_CONDUIT)
                                {
                                    // Only increase power if not at max power level
                                    if (m_PowerLevel < 2)
                                        me->ModifyPower(POWER_ENERGY, 2);

                                    // Visual effects
                                    me->CastSpell(l_LeiShen, SPELL_PILLAR_IN_CHARGE_VISUAL, false);

                                    // If not still energized, Lei Shen must schedule the new ability and activate the floor
                                    if (!m_IsEnergized)
                                    {
                                        m_IsEnergized = true;
                                        me->CastSpell(me, SPELL_CONDUIT_LEVEL_VISUAL, true);
                                        l_LeiShen->AI()->DoAction(g_PillarActions[m_Data]);
                                        l_LeiShen->CastSpell(l_LeiShen, g_PillarBonusSpells[m_Data], true);

                                        if (GameObject* l_Floor = GameObject::GetGameObject(*me, m_Instance->GetData64(g_FloorGobEntries[m_Data])))
                                            l_Floor->SetGoState(GO_STATE_ACTIVE);
                                    }
                                }
                                /// Too far away
                                else
                                {
                                    if (m_IsEnergized)
                                    {
                                        DoAction(ACTION_STOP_ENERGIZE);
                                        l_LeiShen->AI()->DoAction(-g_PillarActions[m_Data]);
                                        l_LeiShen->RemoveAura(g_PillarBonusSpells[m_Data]);
                                        l_LeiShen->RemoveAura(SPELL_PILLAR_IN_CHARGE_VISUAL);
                                        me->RemoveAura(SPELL_CONDUIT_LEVEL_VISUAL);
                                    }
                                }
                            }
                        }
                        m_Events.ScheduleEvent(EVENT_CHECK_LEI_SHEN_PILLAR, 1000);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_pillar_controlerAI(p_Creature);
        }
};

/// Thunderous Throw - 68574
class mob_thunderous_throw : public CreatureScript
{
    public:
        mob_thunderous_throw() : CreatureScript("mob_thunderous_throw") { }

        struct mob_thunderous_throwAI : public ScriptedAI
        {
            mob_thunderous_throwAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;

            void Reset()
            {
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_IMMUNE_TO_PC | UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                if (!m_Instance)
                    return;

                if (Creature* l_LeiShen = Creature::GetCreature(*me, m_Instance->GetData64(NPC_LEI_SHEN)))
                {
                    Position l_Position;
                    me->GetPosition(&l_Position);

                    l_LeiShen->SendPlaySpellVisual(29519, me, 4.0f, me->GetOrientation(), l_Position, false, true);
                    l_LeiShen->CastSpell(me, SPELL_THUNDERSTRUCK_MISSILE, false);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_thunderous_throwAI(p_Creature);
        }
};

/// Lesser Diffused Lightning - 69012
class mob_lesser_diffused_lightning : public CreatureScript
{
    public:
        mob_lesser_diffused_lightning() : CreatureScript("mob_lesser_diffused_lightning") { }

        struct mob_lesser_diffused_lightningAI : public ScriptedAI
        {
            mob_lesser_diffused_lightningAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_CHAIN_LIGHTNING_SUMMON_1, true);
            }

            void DamageDealt(Unit* p_Victim, uint32& /*p_Damage*/, DamageEffectType p_DamageType)
            {
                if (p_DamageType != DIRECT_DAMAGE)
                    return;

                me->CastSpell(p_Victim, SPELL_CHAIN_LIGHTNING_DAMAGE_1, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_lesser_diffused_lightningAI(p_Creature);
        }
};

/// Diffused Lightning - 69013
class mob_diffused_lightning : public CreatureScript
{
    public:
        mob_diffused_lightning() : CreatureScript("mob_diffused_lightning") { }

        struct mob_diffused_lightningAI : public ScriptedAI
        {
            mob_diffused_lightningAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_CHAIN_LIGHTNING_SUMMON_2, true);
            }

            void DamageDealt(Unit* p_Victim, uint32& /*p_Damage*/, DamageEffectType p_DamageType)
            {
                if (p_DamageType != DIRECT_DAMAGE)
                    return;

                me->CastSpell(p_Victim, SPELL_CHAIN_LIGHTNING_DAMAGE_2, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_diffused_lightningAI(p_Creature);
        }
};

/// Diffused Lightning - 69014
class mob_greater_diffused_lightning : public CreatureScript
{
    public:
        mob_greater_diffused_lightning() : CreatureScript("mob_greater_diffused_lightning") { }

        struct mob_greater_diffused_lightningAI : public ScriptedAI
        {
            mob_greater_diffused_lightningAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->CastSpell(me, SPELL_CHAIN_LIGHTNING_SUMMON_3, true);
            }

            void DamageDealt(Unit* p_Victim, uint32& /*p_Damage*/, DamageEffectType p_DamageType)
            {
                if (p_DamageType != DIRECT_DAMAGE)
                    return;

                me->CastSpell(p_Victim, SPELL_CHAIN_LIGHTNING_DAMAGE_3, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_greater_diffused_lightningAI(p_Creature);
        }
};

/// 69232 - Ball lightning
class mob_ball_lightning : public CreatureScript
{
    public:
        mob_ball_lightning() : CreatureScript("mob_ball_lightning") { }

        struct mob_ball_lightningAI : public ScriptedAI
        {
            mob_ball_lightningAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
            }

            void IsSummonedBy(Unit* /*p_Summoner*/)
            {
                std::list<Player*> l_PlayerList;
                GetPlayerListInGrid(l_PlayerList, me, 100.0f);

                if (!l_PlayerList.empty())
                {
                    JadeCore::RandomResizeList(l_PlayerList, 1);
                    if (Player* l_Target = l_PlayerList.front())
                        me->CastSpell(l_Target, SPELL_BALL_LIGHTNING_JUMP, true);
                }

                me->AddAura(SPELL_BALL_LIGHTNING_AURA, me);
                me->AddAura(SPELL_BALL_LIGHTNING, me);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_ball_lightningAI(p_Creature);
        }
};

/// Overwhelming Power - 69645
class mob_overwhelming_power : public CreatureScript
{
    public:
        mob_overwhelming_power() : CreatureScript("mob_overwhelming_power")  { }

        struct mob_overwhelming_powerAI : public ScriptedAI
        {
            mob_overwhelming_powerAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void SpellHit(Unit* /*p_Caster*/, SpellInfo const* p_SpellInfo)
            {
                if (p_SpellInfo->Id != SPELL_OVERWHELMING_POWER_NPC)
                    return;

                me->CastSpell(me, SPELL_OVERWHELMING_POWER_VISUAL, true);
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_overwhelming_powerAI(p_Creature);
        }
};

/// Discharged Energy - 134820
class spell_discharged_energy : public SpellScriptLoader
{
    public:
        spell_discharged_energy() : SpellScriptLoader("spell_discharged_energy") { }

        class spell_discharged_energy_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_discharged_energy_AuraScript);

            void OnUpdate(uint32 /*p_Diff*/, AuraEffect* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    l_Caster->GetPlayerListInGrid(l_PlayerList, 200.0f);

                    for (Player* l_Player : l_PlayerList)
                        l_Player->CastSpell(l_Player, SPELL_DISCHARGED_ENERGY, true);
                }
            }

            void Register()
            {
                OnEffectUpdate += AuraEffectUpdateFn(spell_discharged_energy_AuraScript::OnUpdate, EFFECT_0, SPELL_AURA_AREATRIGGER);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_discharged_energy_AuraScript();
        }
};

/// Decapitate - 134912
class spell_decapitate : public SpellScriptLoader
{
    public:
        spell_decapitate() : SpellScriptLoader("spell_decapitate") { }

        class spell_decapitate_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_decapitate_AuraScript);

            void OnRemove(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetTarget())
                    {
                        l_Caster->CastSpell(l_Target, SPELL_DECAPITATE_MISSILE, true);
                        l_Caster->ToCreature()->AI()->DoAction(ACTION_REMOVE_EQUIPMENT);
                    }
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_decapitate_AuraScript::OnRemove, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_decapitate_AuraScript();
        }
};

/// Decapitate (missile) - 134916
/// Thunderstruck (damage) - 135096
class spell_damage_by_distance : public SpellScriptLoader
{
    public:
        spell_damage_by_distance() : SpellScriptLoader("spell_damage_by_distance") { }

        class spell_damage_by_distance_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_damage_by_distance_SpellScript);

            void HandleOnHit(SpellEffIndex /*p_EffIndex*/)
            {
                if (GetSpellInfo()->Id == SPELL_DECAPITATE_DAMAGE)
                {
                    if (Creature* l_LeiShen = GetCaster()->ToCreature())
                    {
                        l_LeiShen->AI()->DoAction(ACTION_SET_EQUIPMENT);

                        if (Unit* l_Target = GetHitUnit())
                        {
                            float l_Dist = std::max(1.0f, l_LeiShen->GetDistance(l_Target));
                            SetHitDamage(int32(ceil(GetHitDamage() / l_Dist)));
                        }
                    }
                }
                else
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        if (Creature* l_ThunderousThrow = l_Target->FindNearestCreature(NPC_THUNDEROUS_THROW, 100.0f))
                        {
                            float l_Dist = std::max(1.0f, l_ThunderousThrow->GetDistance(l_Target));
                            SetHitDamage(int32(ceil(GetHitDamage() / l_Dist)));
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_damage_by_distance_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_damage_by_distance_SpellScript();
        }
};

/// Crashing Thunder - 135143
class spell_crashing_thunder_periodic : public SpellScriptLoader
{
    public:
        spell_crashing_thunder_periodic() : SpellScriptLoader("spell_crashing_thunder_periodic") { }

        class spell_crashing_thunder_periodic_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_crashing_thunder_periodic_AuraScript);

            void OnTick(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (!l_Caster->ToCreature())
                        return;

                    if (Unit* l_Target = l_Caster->ToCreature()->AI()->SelectTarget(SELECT_TARGET_RANDOM))
                        l_Caster->CastSpell(l_Target, SPELL_CRASHING_THUNDER_AT, true);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_crashing_thunder_periodic_AuraScript::OnTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_crashing_thunder_periodic_AuraScript();
        }
};

/// Diffusion Chain - 135991
class spell_diffusion_chain : public SpellScriptLoader
{
    public:
        spell_diffusion_chain() : SpellScriptLoader("spell_diffusion_chain") { }

        class spell_diffusion_chain_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_diffusion_chain_SpellScript);

            void HandleOnHit(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        uint32 l_Spell = SPELL_DIFFUSION_CHAIN_SUMMON_1;

                        if (l_Caster->GetEntry() == NPC_DIFFUSION_CHAIN_CONDUIT)
                        {
                            uint32 l_PowerLevel = l_Caster->GetAI()->GetData(TYPE_GET_PILLAR_POWER_LEVEL);
                            l_Spell = l_PowerLevel == 2 ? SPELL_DIFFUSION_CHAIN_SUMMON_2 : l_PowerLevel == 3 ? SPELL_DIFFUSION_CHAIN_SUMMON_3 : l_Spell;
                        }

                        l_Target->CastSpell(l_Target, l_Spell, true, NULL, nullptr, l_Caster->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_diffusion_chain_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_SCHOOL_DAMAGE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_diffusion_chain_SpellScript();
        }
};

/// Overcharged (Dummy) - 137252
class spell_overcharged_dummy : public SpellScriptLoader
{
    public:
        spell_overcharged_dummy() : SpellScriptLoader("spell_overcharged_dummy") { }

        class spell_overcharged_dummy_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_overcharged_dummy_SpellScript);

            void HandleOnHit(SpellEffIndex /*p_EffIndex*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Target = GetHitUnit())
                    {
                        Creature* l_LeiShen = NULL;
                        if (InstanceScript* l_Instance = l_Caster->GetInstanceScript())
                            l_LeiShen = Creature::GetCreature(*l_Caster, l_Instance->GetData64(NPC_LEI_SHEN));
                        if (!l_LeiShen)
                            return;

                        // @TODO: Handle things with AreaTrigger for safety area
                        l_Caster->CastSpell(l_Target, SPELL_OVERCHARGE_DAMAGE_STUN, true, NULL, nullptr, l_LeiShen->GetGUID());
                    }
                }
            }

            void Register()
            {
                OnEffectHitTarget += SpellEffectFn(spell_overcharged_dummy_SpellScript::HandleOnHit, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_overcharged_dummy_SpellScript();
        }
};

/// Bouncing Bolt - 136361
/// Bouncing Bolt (Bounce) - 138706
class spell_bouncing_bolt : public SpellScriptLoader
{
    public:
        spell_bouncing_bolt() : SpellScriptLoader("spell_bouncing_bolt") { }

        class spell_bouncing_bolt_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bouncing_bolt_SpellScript);

            void HandleMissile(SpellEffIndex /*p_EffIndex*/)
            {
                if (Position const* l_Pos = GetExplTargetDest())
                {
                    if (Unit* l_Caster = GetCaster())
                    {
                        if (!l_Caster->isInCombat() || !l_Caster->isAlive())
                            return;

                        std::list<WorldObject*> l_TargetList;
                        JadeCore::WorldObjectSpellAreaTargetCheck l_Checker(6.0f, l_Pos, l_Caster, l_Caster, GetSpellInfo(), TARGET_CHECK_DEFAULT, NULL);
                        JadeCore::WorldObjectListSearcher<JadeCore::WorldObjectSpellAreaTargetCheck> l_Searcher(l_Caster, l_TargetList, l_Checker);
                        l_Caster->GetMap()->VisitAll(l_Pos->m_positionX, l_Pos->m_positionY, 6.0f, l_Searcher);

                        if (!l_TargetList.empty())
                        {
                            l_TargetList.remove_if([this](WorldObject* p_Object) -> bool
                            {
                                if (!p_Object)
                                    return true;

                                if (p_Object->IsPlayer())
                                    return false;

                                if (p_Object->ToUnit() && p_Object->ToUnit()->GetOwner() && p_Object->ToUnit()->GetOwner()->ToPlayer())
                                    return false;

                                return true;
                            });
                        }

                        if (l_TargetList.empty())
                        {
                            float l_X, l_Y, l_Z;
                            l_Pos->GetPosition(l_X, l_Y, l_Z);
                            // If the Bouncing Bolt does not impact with a player, an Unharnessed Power forms ...
                            // ... and the Bouncing Bolt bounces again.
                            if (Creature* l_Summon = l_Caster->SummonCreature(NPC_UNHARNESSED_POWER, l_X, l_Y, l_Z))
                                l_Summon->CastSpell(l_Summon, SPELL_BOUNCING_BOLT_BOUNCE, true);
                        }
                    }
                }
            }

            void Register()
            {
                OnEffectHit += SpellEffectFn(spell_bouncing_bolt_SpellScript::HandleMissile, EFFECT_0, SPELL_EFFECT_TRIGGER_MISSILE);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_bouncing_bolt_SpellScript();
        }
};

/// 136539 - Ball lightning
class spell_ball_lightning : public SpellScriptLoader
{
    public:
        spell_ball_lightning() : SpellScriptLoader("spell_ball_lightning") { }

        class spell_ball_lightning_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ball_lightning_SpellScript);

            void HandleDamage()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, l_Caster, 6.0f);

                    for (Player* l_Player : l_PlayerList)
                        l_Caster->CastSpell(l_Player, SPELL_BALL_LIGHTNING_DAMAGE, true);
                }
            }

            void Register()
            {
                OnCast += SpellCastFn(spell_ball_lightning_SpellScript::HandleDamage);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ball_lightning_SpellScript();
        }
};

/// Overwhelming Power - 136913
class spell_overwhelming_power : public SpellScriptLoader
{
    public:
        spell_overwhelming_power() : SpellScriptLoader("spell_overwhelming_power") { }

        class spell_overwhelming_power_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_overwhelming_power_AuraScript);

            void OnProc(AuraEffect const* /*p_AurEff*/, ProcEventInfo& /*p_ProcInfo*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Unit* l_Victim = l_Caster->getVictim())
                    {
                        if (l_Victim->IsPlayer())
                            l_Caster->CastSpell(l_Victim, SPELL_OVERWHELMING_POWER_AURA, true);
                    }

                    std::list<Creature*> l_OverwhelmingList;
                    GetCreatureListWithEntryInGrid(l_OverwhelmingList, l_Caster, NPC_OVERWHELMING_POWER, 20.0f);
                    for (Creature* l_OverWhelming : l_OverwhelmingList)
                    {
                        if (urand(0, 1))
                            l_Caster->CastSpell(l_OverWhelming, SPELL_OVERWHELMING_POWER_NPC, true);
                    }
                }
            }

            void Register()
            {
                OnEffectProc += AuraEffectProcFn(spell_overwhelming_power_AuraScript::OnProc, EFFECT_0, SPELL_AURA_PROC_TRIGGER_SPELL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_overwhelming_power_AuraScript();
        }
};

/// Thunder Trap (activator) - 8896
class areatrigger_thunder_trap_activator : public AreaTriggerScript
{
    public:
        areatrigger_thunder_trap_activator() : AreaTriggerScript("areatrigger_thunder_trap_activator") { }

        bool OnTrigger(Player* p_Player, AreaTriggerEntry const* /*p_Trigger*/, bool /*p_Enter*/)
        {
            if (!p_Player)
                return false;

            if (InstanceScript* l_Instance = p_Player->GetInstanceScript())
            {
                if (l_Instance->GetData(DATA_THUNDER_CLAP))
                    return false;

                l_Instance->SetData(DATA_THUNDER_CLAP, true);

                std::list<Creature*> l_ThunderTraps;
                p_Player->GetCreatureListWithEntryInGrid(l_ThunderTraps, NPC_THUNDER_TRAP, 200.0f);

                for (Creature* l_ThunderTrap : l_ThunderTraps)
                    l_ThunderTrap->AI()->DoAction(ACTION_ACTIVATE_TRAP);
            }

            return true;
        }
};

/// Quarter Area (North) - 8910
/// Quarter Area (East) - 8911
/// Quarter Area (South) - 8912
/// Quarter Area (West) - 8913
class areatrigger_quarter_area : public AreaTriggerScript
{
    public:
        areatrigger_quarter_area() : AreaTriggerScript("areatrigger_quarter_area") { }

        bool OnTrigger(Player* p_Player, AreaTriggerEntry const* p_Trigger, bool p_Enter)
        {
            if (!p_Player)
                return false;

            uint32 l_InactivePillars = 0;

            if (InstanceScript* l_Instance = p_Player->GetInstanceScript())
            {
                if (Creature* l_LeiShen = Creature::GetCreature(*p_Player, l_Instance->GetData64(NPC_LEI_SHEN)))
                    l_InactivePillars = l_LeiShen->AI()->GetData(DATA_GET_INACTIVE_PILLARS);
            }

            switch (p_Trigger->ID)
            {
                case QUARTER_AREA_NORTH:
                {
                    if (p_Enter)
                    {
                        p_Player->CastSpell(p_Player, SPELL_IN_QUADRANT_NORTH, true);

                        if ((l_InactivePillars & PILLAR_NORTH) && !p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->AddAura(SPELL_OVERLOAD_AURA, p_Player);
                    }
                    else
                    {
                        p_Player->RemoveAura(SPELL_IN_QUADRANT_NORTH);

                        if ((l_InactivePillars & PILLAR_NORTH) && p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->RemoveAura(SPELL_OVERLOAD_AURA);
                    }
                    break;
                }
                case QUARTER_AREA_EAST:
                {
                    if (p_Enter)
                    {
                        p_Player->CastSpell(p_Player, SPELL_IN_QUADRANT_EAST, true);

                        if ((l_InactivePillars & PILLAR_EAST) && !p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->AddAura(SPELL_OVERLOAD_AURA, p_Player);
                    }
                    else
                    {
                        p_Player->RemoveAura(SPELL_IN_QUADRANT_EAST);

                        if ((l_InactivePillars & PILLAR_EAST) && p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->RemoveAura(SPELL_OVERLOAD_AURA);
                    }
                    break;
                }
                case QUARTER_AREA_SOUTH:
                {
                    if (p_Enter)
                    {
                        p_Player->CastSpell(p_Player, SPELL_IN_QUADRANT_SOUTH, true);

                        if ((l_InactivePillars & PILLAR_SOUTH) && !p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->AddAura(SPELL_OVERLOAD_AURA, p_Player);
                    }
                    else
                    {
                        p_Player->RemoveAura(SPELL_IN_QUADRANT_SOUTH);

                        if ((l_InactivePillars & PILLAR_SOUTH) && p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->RemoveAura(SPELL_OVERLOAD_AURA);
                    }
                    break;
                }
                case QUARTER_AREA_WEST:
                {
                    if (p_Enter)
                    {
                        p_Player->CastSpell(p_Player, SPELL_IN_QUADRANT_WEST, true);

                        if ((l_InactivePillars & PILLAR_WEST) && !p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->AddAura(SPELL_OVERLOAD_AURA, p_Player);
                    }
                    else
                    {
                        p_Player->RemoveAura(SPELL_IN_QUADRANT_WEST);

                        if ((l_InactivePillars & PILLAR_WEST) && p_Player->HasAura(SPELL_OVERLOAD_AURA))
                            p_Player->RemoveAura(SPELL_OVERLOAD_AURA);
                    }
                    break;
                }
                default:
                    break;
            }

            return true;
        }
};

/// Violent Gale Winds - 3981 (spells 136867, 136876, 136877, 136879)
class areatrigger_violent_gale_winds : public AreaTriggerEntityScript
{
    public:
        areatrigger_violent_gale_winds() : AreaTriggerEntityScript("areatrigger_violent_gale_winds") { }

        uint32 m_Window;
        InstanceScript* m_Instance;

        void OnCreate(AreaTrigger* p_AreaTrigger)
        {
            m_Instance = p_AreaTrigger->GetInstanceScript();

            Unit* l_Caster = p_AreaTrigger->GetCaster();

            if (!l_Caster)
                return;

            uint32 l_Spell = p_AreaTrigger->GetSpellId();
            m_Window = l_Spell == SPELL_VIOLENT_GALE_WINDS_AT_N ? 0 : l_Spell == SPELL_VIOLENT_GALE_WINDS_AT_E ? 1 :
                       l_Spell == SPELL_VIOLENT_GALE_WINDS_AT_S ? 2 : 3;

            /// For dev player test purpose
            if (l_Caster->GetEntry() != NPC_LEI_SHEN)
            {
                if (m_Instance)
                {
                    if (GameObject* l_DoorObject = GameObject::GetGameObject(*l_Caster, m_Instance->GetData64(g_Windows[m_Window])))
                        l_DoorObject->SetGoState(GO_STATE_ACTIVE);

                    if (GameObject* l_WindFX = GameObject::GetGameObject(*l_Caster, m_Instance->GetData64(g_WindFX[m_Window])))
                        l_WindFX->SetGoState(GO_STATE_ACTIVE);
                }
            }

            Map::PlayerList const& l_PlayerList = p_AreaTrigger->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
            {
                if (Player* l_Player = l_Itr->getSource())
                {
                    if (l_Player->isAlive())
                    {
                        l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), true, g_WindsPositions[m_Window], -3.0f, 1);
                        l_Player->CastSpell(l_Player, SPELL_VIOLENT_GALE_WINDS_AURA, true);
                    }
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Map::PlayerList const& l_PlayerList = p_AreaTrigger->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
            {
                if (Player* l_Player = l_Itr->getSource())
                {
                    l_Player->SendApplyMovementForce(p_AreaTrigger->GetGUID(), false, g_WindsPositions[m_Window]);
                    l_Player->RemoveAura(SPELL_VIOLENT_GALE_WINDS_AURA); ///< Aura WindBurn (140208) not removed, as it stacks with all violent gale winds
                }
            }

            if (Unit* l_Caster = p_AreaTrigger->GetCaster())
            {
                if (l_Caster->GetEntry() == NPC_LEI_SHEN)
                    l_Caster->GetAI()->DoAction(ACTION_WIND_END);
                else
                {
                    if (m_Instance)
                    {
                        /// Close the window
                        if (GameObject* l_Window = GameObject::GetGameObject(*p_AreaTrigger, m_Instance->GetData64(g_Windows[m_Window])))
                            l_Window->SetGoState(GO_STATE_READY);

                        /// Stop the wind FX on floor
                        if (GameObject* l_WindFX = GameObject::GetGameObject(*p_AreaTrigger, m_Instance->GetData64(g_WindFX[m_Window])))
                            l_WindFX->SetGoState(GO_STATE_READY);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new areatrigger_violent_gale_winds();
        }
};

/// Displacement Pad - 218417
class go_displacement_pad : public GameObjectScript
{
    public:
        go_displacement_pad() : GameObjectScript("go_displacement_pad") { }

        bool OnGossipHello(Player* p_Player, GameObject* /*p_GameObject*/)
        {
            if (p_Player)
            {
                p_Player->NearTeleportTo(g_TeleportPosition.m_positionX,
                    g_TeleportPosition.m_positionY,
                    g_TeleportPosition.m_positionZ,
                    g_TeleportPosition.m_orientation);
            }

            return true;
        }
};

/// Created by spell 135150
class at_crashing_thunder : public AreaTriggerEntityScript
{
    public:
        at_crashing_thunder() : AreaTriggerEntityScript("at_crashing_thunder") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 4.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            if (!l_TargetList.empty())
            {
                for (auto l_Unit : l_TargetList)
                {
                    if (!l_Unit->ToPlayer())
                        continue;

                    if (l_Unit->GetDistance(p_AreaTrigger) > l_Radius && l_Unit->HasAura(SPELL_CRASHING_THUNDER_DMG))
                        l_Unit->RemoveAura(SPELL_CRASHING_THUNDER_DMG);
                    else if (l_Unit->GetDistance(p_AreaTrigger) <= l_Radius && !l_Unit->HasAura(SPELL_CRASHING_THUNDER_DMG))
                        l_Caster->CastSpell(l_Unit, SPELL_CRASHING_THUNDER_DMG, true);
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_crashing_thunder();
        }
};

/// Created by spell 137499
class at_lightning_whip : public AreaTriggerEntityScript
{
    public:
        at_lightning_whip() : AreaTriggerEntityScript("at_lightning_whip") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            AreaTriggerTemplateList const& l_Templates = p_AreaTrigger->GetTemplates();
            std::vector<G3D::Vector2> l_CheckPoints;

            std::list<Unit*> l_TargetList;
            float l_Radius = 20.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            float l_X = p_AreaTrigger->GetPositionX();
            float l_Y = p_AreaTrigger->GetPositionY();
            float l_Z = p_AreaTrigger->GetPositionZ();
            float l_O = p_AreaTrigger->GetOrientation();

            Position l_Pos = { l_X, l_Y, l_Z, 0.0f };

            for (AreaTriggerTemplate l_Template : l_Templates)
            {
                /// Absolute position according to the current position and the template
                float l_AbsX = l_X + l_Template.m_PolygonDatas.m_Vertices[0];
                float l_AbsY = l_Y + l_Template.m_PolygonDatas.m_Vertices[1];

                /// Calculating the distance between the origin and the absolute position, and the angle between them
                Position l_VirtualPos = { l_AbsX, l_AbsY, l_Z, 0.0f };
                float l_Dist  = l_Pos.GetExactDist2d(&l_VirtualPos);
                float l_Angle = l_Pos.GetAngle(&l_VirtualPos);

                /// Reporting the distance and the angle according to the current orientation
                float l_NewX = l_X + l_Dist * cos(l_O - l_Angle + M_PI / 2);
                float l_NewY = l_Y + l_Dist * sin(l_O - l_Angle + M_PI / 2);

                /// Adding the point to the vector
                G3D::Vector2 l_Point = G3D::Vector2(l_NewX, l_NewY);
                l_CheckPoints.push_back(l_Point);
            }

            for (Unit* l_Unit : l_TargetList)
            {
                /// Player above the ground
                if (l_Unit->GetPositionZ() > p_AreaTrigger->GetPositionZ() + 0.5f)
                {
                    if (l_Unit->HasAura(SPELL_LIGHTNING_WHIP_HIT))
                    {
                        l_Unit->RemoveAura(SPELL_LIGHTNING_WHIP_HIT);
                        continue;
                    }
                }

                /// Target not in lightning lines
                if (!p_AreaTrigger->IsInAreaTriggerPolygon(l_CheckPoints, G3D::Vector2(l_Unit->m_positionX, l_Unit->m_positionY)))
                {
                    if (l_Unit->HasAura(SPELL_LIGHTNING_WHIP_HIT))
                    {
                        l_Unit->RemoveAura(SPELL_LIGHTNING_WHIP_HIT);
                        continue;
                    }
                }

                l_Unit->AddAura(SPELL_LIGHTNING_WHIP_HIT, l_Unit);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_lightning_whip();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_lei_shen()
{
    new boss_lei_shen();                        ///< 68397
    new mob_thunder_trap();                     ///< 69825
    new mob_pillar_controler();                 ///< 68398 - 68696 - 68697 - 68698
    new mob_thunderous_throw();                 ///< 68574
    new mob_lesser_diffused_lightning();        ///< 69012
    new mob_diffused_lightning();               ///< 69013
    new mob_greater_diffused_lightning();       ///< 69014
    new mob_ball_lightning();                   ///< 69232
    new mob_overwhelming_power();               ///< 69645
    new spell_discharged_energy();              ///< 134820
    new spell_decapitate();                     ///< 134912
    new spell_damage_by_distance();             ///< 134916 - 135096
    new spell_crashing_thunder_periodic();      ///< 135143
    new spell_diffusion_chain();                ///< 135991
    new spell_overcharged_dummy();              ///< 137252
    new spell_bouncing_bolt();                  ///< 136361 - 138706
    new spell_ball_lightning();                 ///< 136539
    new spell_overwhelming_power();             ///< 136913
    new areatrigger_thunder_trap_activator();   ///< 8896
    new areatrigger_quarter_area();             ///< 8910 - 8911 - 8912 - 8913
    new areatrigger_violent_gale_winds();       ///< 3981
    new go_displacement_pad();                  ///< 218417
    new at_crashing_thunder();                  ///< 135150
    new at_lightning_whip();                    ///< 137499
}
#endif
