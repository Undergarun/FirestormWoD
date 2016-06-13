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

enum eTwinSpells
{
    // Lu'lin
        // Invisible
    SPELL_DISSIPATE_LULIN       = 137187,
    SPELL_INVIS_LULIN           = 137189,
        // Moon Lotus
    SPELL_BLOOM_MOON_LOTUS      = 137118,   // Launch a beam on a target to create moon lotus
    SPELL_MOON_LOTUS_DEACTIVATE = 137191,   // Lotus deactivated during day phases
    SPELL_MOON_LOTUS_VISUAL     = 137192,   // Lotus flower visual
    SPELL_MOON_LOTUS            = 136714,   // Summon a Moon Lotus Spore (69340)
    SPELL_SLUMBER_SPORE_VISUAL  = 136721,   // Green cloud
    SPELL_SLUMBER_SPORE         = 136722,   // Make the target sleeps for 5 secs
        // Cosmic Barrage
    SPELL_COSMIC_BARRAGE        = 137749,   // Triggers 136752, which triggers 136752
    SPELL_COSMIC_BARRAGE_SUMMON = 136768,   // Triggered by 136752 for each players, which summons NPC 69365
        // Beast of Nightmares
    SPELL_BEAST_OF_NIGHTMARES   = 137375,   // Triggers 137332 which summons 69479 - Beast of Nightmares
    SPELL_BEAST_OF_NIGHTMARES_D = 137341,   // Aura the beast puts on his target to allow him to fight him, and which will triggered 137360 on healing
    SPELL_CORRUPTED_HEALING     = 137360,
        // Ice Comet
    SPELL_ICE_COMET_LULIN_BUMP  = 137418,
    SPELL_ICE_COMET             = 137419,   // Summons 69596
    SPELL_ICE_COMET_LULIN_VIS   = 138797,   // Lu'lin casting visual (phase 2)
    SPELL_ICE_COMET_BUMP        = 137495,
    SPELL_ICE_COMET_BUMP_DELAY  = 138798,   // Idem 137495, instead there's a delay before the bump (waiting for the end of 138797?)
    SPELL_ICE_COMET_DOWN        = 137420,
    SPELL_ICY_SHADOWS           = 137439,
    SPELL_ICY_SHADOWS_AURA      = 137440,
        // Tidal Force
    SPELL_TIDAL_FORCE           = 137531,
    SPELL_TIDAL_FORCE_WAVE      = 140863,
    SPELL_TIDAL_FORCE_DAMAGE    = 137716,

    // Suen
        // Invisible
    SPELL_DISSIPATE_SUEN        = 137105,
    SPELL_INVIS_SUEN            = 137106,
        // Tears of the sun
    SPELL_TEARS_OF_THE_SUN      = 137404,   // Periodically triggers 137405 to damage players
    SPELL_TEARS_OF_THE_SUN_PER  = 137405,
        // Light of the day
    SPELL_LIGHT_OF_DAY_ASSIST   = 137401,
    SPELL_LIGHT_OF_DAY          = 137493,
    SPELL_LIGHT_OF_DAY_DMG      = 137494,
    SPELL_LIGHT_OF_DAY_COMET    = 137432,
        // Fan of Flames
    SPELL_FAN_OF_FLAMES         = 137408,
        // Flames of passion
    SPELL_FLAMES_OF_PASSION     = 137412,   // Charge outward
    SPELL_FLAMES_OF_PASSION_BMP = 137414,   // Bump
    SPELL_FLAMES_OF_PASSION_BCK = 137415,   // Way back to the main target
    SPELL_FLAMES_OF_PASSION_AT  = 137416,   // Areatrigger (137417 for damages)
    SPELL_FLAMES_OF_PASSION_DMG = 137417,   // Aura put on players who walk in areatriggers created by 137416
        // Blazing Radiance
    SPELL_BLAZING_RADIANCE_DMG  = 137410,
    SPELL_BLAZING_RADIANCE      = 137411,
    SPELL_BLAZING_RADIANCE_ST   = 139412,

    // Celestials
        // Visual
    SPELL_BLUE_SHADOWY          = 138059,   // Xuen
    SPELL_RED_SHADOWY           = 138065,   // Chi-Ji
    SPELL_YELLOW_SHADOWY        = 138069,   // Niuzao
    SPELL_GREEN_SHADOWY         = 138071,   // Yu'lon
        // Buffs
    SPELL_FORTITUDE_OF_THE_OX   = 138300,   // Niuzao
    SPELL_CRANE_RUSH            = 142716,   // Chi-Ji
    SPELL_CRANE_RUSH_DAMAGE     = 138318,
    SPELL_XUEN_ALACRITY         = 138656,   // Xuen
    SPELL_SERPENTS_VITALITY     = 138306,   // Yu'lon
        // Drawings aura
    SPELL_INVOKE_TIGER_SPIRIT   = 138264,   // From Xuen   - 69771
    SPELL_INVOKE_CRANE_SPIRIT   = 138189,   // From Chi-Ji - 69774
    SPELL_INVOKE_OX_SPIRIT      = 138254,   // From Niuzao - 69777
    SPELL_INVOKE_SERPENT_SPIRIT = 138267,   // From Yu'lon - 69779
        // Astral Stars
    SPELL_STARDUST_BEAM         = 138206,   // Beam linking stars
    SPELL_FIRST_STAR            = 138140,
    SPELL_FIRST_STAR_HALO       = 149149,
    SPELL_STAR_STATE_ASTRAL     = 138039,
        // Buff Visuals
    SPELL_OX_BUFF_VISUAL        = 138298,
    SPELL_CRANE_BUFF_VISUAL     = 138314,
        // Siphonned Essences
    SPELL_SIPHONNED_YULON       = 139369,
    SPELL_SIPHONNED_XUEN        = 139372,
    SPELL_SIPHONNED_NIUZAO      = 139373,
    SPELL_SIPHONNED_CHI_JI      = 139374,

    // Misc
    SPELL_STARRY_SKY            = 137193,
    SPELL_STARRY_SKY_BLUE_AURA  = 137141,
    SPELL_SUNNY_SKY             = 137183,
    SPELL_LIGHT_OF_DAY_CTRL     = 137738,
    SPELL_INACTIVE_PLATFORM     = 139767,
    SPELL_TWIN_BERSERK          =  26662
};

enum eTwinEvents
{
    // Lu'lin
    EVENT_MOON_LOTUS = 1,
    EVENT_COSMIC_BARRAGE,
    EVENT_COSMIC_STARS,
    EVENT_BEAST_OF_NIGHTMARES,
    EVENT_ICE_COMET,
    EVENT_TIDAL_FORCE,
    EVENT_LURKERS_IN_THE_NIGHT, // Heroic
    EVENT_TALK_AGGRO,
    EVENT_TALK_BREAK_OF_DUSK,
    // Suen
    EVENT_TEARS_OF_THE_SUN,
    EVENT_LIGHT_OF_DAY,
    EVENT_FAN_OF_FLAMES,
    EVENT_FLAMES_OF_PASSION,
    EVENT_FLAMES_BACK,
    EVENT_FLAMES_AT,
    EVENT_BLAZING_RADIANCE,
    EVENT_NUCLEAR_INFERNO,      // Heroic
    EVENT_TALK_DAYLIGHT,
    // Moon Lotus
    EVENT_LOTUS_SPORE,
    EVENT_SPORE_CHECK_PLAYERS,
    EVENT_SPORE_MOVE,
    // Ice Comet
    EVENT_CHECK_SUEN,
    EVENT_ICY_SHADOWS,
    // Astral
    EVENT_CHECK_PLAYER,
    EVENT_END_PATTERN,
    // Celestials
    EVENT_CRANE_CHECK_BOSS,
    EVENT_SUMMON_RED_CRANE,
    EVENT_CRANE_MOVE,
    // Misc
    EVENT_TALK_LAST_PHASE
};

enum eSuenTalks
{
    TALK_SUEN_AGGRO,
    TALK_SUEN_PHASE_DAYLIGHT,
    TALK_SUEN_FLAMES,
    TALK_SUEN_SLAY,
    TALK_SUEN_WIPE,
    TALK_SUEN_DEATH,
    TALK_SUEN_FINAL_PHASE,
    TALK_SUEN_BREAK_OF_DUSK
};

enum eLulinTalks
{
    TALK_LULIN_AGGRO,
    TALK_LULIN_PHASE_DAYLIGHT,
    TALK_LULIN_BREAK_OF_DUSK,
    TALK_LULIN_SLAY,
    TALK_LULIN_WIPE,
    TALK_LULIN_FINAL_PHASE,
    TALK_LULIN_DEATH
};

enum eCelestialTalk
{
    TALK_CELESTIAL
};

enum eTwinPhases
{
    PHASE_NO_PHASE,         // Combat not started
    PHASE_DARKNESS,         // Lasts for 3 mins
    PHASE_DAYLIGHT,         // Lasts for 3 mins
    PHASE_BREAK_OF_DUSK,    // Lasts until one of the twins is dead
    PHASE_LAST_PHASE,       // Lasts until the fight is over
};

enum eTwinAdds
{
    NPC_FADED_IMAGE_OF_XUEN     = 69771,
    NPC_FADED_IMAGE_OF_CHI_JI   = 69774,
    NPC_FADED_IMAGE_OF_NIUZAO   = 69777,
    NPC_FADED_IMAGE_OF_YU_LON   = 69779,
    NPC_BEAST_OF_NIGHTMARES     = 69479,
    NPC_LURKER_IN_THE_NIGHT     = 69591, // Heroic
    NPC_ICE_COMET               = 69596,
    NPC_MOON_LOTUS              = 69456,
    NPC_MOON_LOTUS_CLOUD        = 69340,
    NPC_DEEP_SUBMERGE           = 69629,
    NPC_STAR                    = 69365,
    NPC_ASTRAL_STAR             = 64677,
    NPC_IMAGE_OF_CHI_JI         = 71441
};

enum eTwinEquip
{
    EQUIP_SUEN_MAIN             = 93774,
    EQUIP_LULIN_MAIN            = 93776,
    EQUIP_SUEN_OFF              = 93778,
    EQUIP_LULIN_OFF             = 93780
};

enum eTwinTypes
{
    TYPE_CHANGE_PHASE = 1,
    TYPE_ENTER_COMBAT,
    TYPE_GET_PHASE,
    TYPE_SET_STAR_ID,
    TYPE_GET_STAR_ID,
    TYPE_SET_DRAW,
    TYPE_ADD_STAR,
    TYPE_TOGGLE_CELESTIALS
};

enum eTwinValues
{
    VALUE_ENABLED   = 1,
    VALUE_DISABLED
};

enum eTwinActions
{
    ACTION_PHASE_DARKNESS,
    ACTION_PHASE_DAYLIGHT,
    ACTION_PHASE_DUSKBREAK,
    ACTION_LOTUS_ACTIVATE,
    ACTION_LOTUS_DEACTIVATE,
    ACTION_STOP_FIGHT,
    ACTION_COSMIC_STARS,
    ACTION_FLAMES_BACK,
    ACTION_STOP_FLAMES_AT,
    ACTION_ACTIVATE_COMET,
    ACTION_COMET_IN_PLACE,
    ACTION_ACTIVATE_NEXT_STAR,
    ACTION_CELESTIAL_BUFF
};

enum eTwinDisplays
{
    DISPLAY_INVISIBLE_CELESTIALS    = 15435,    // Invisible display for all celestials except Yu'lon
    DISPLAY_INVISIBLE_YULON         = 48529,    // Invisible display for Yu'lon
    DISPLAY_XUEN                    = 47672,
    DISPLAY_CHI_JI                  = 47674,
    DISPLAY_NIUZAO                  = 47675,
    DISPLAY_YULON                   = 47676
};

uint32 const g_ConsortsBossEntries[2] = { NPC_SUEN, NPC_LU_LIN };
uint32 const g_ConsortsAddEntries[4] = { NPC_MOON_LOTUS_CLOUD, NPC_MOON_LOTUS, NPC_BEAST_OF_NIGHTMARES, NPC_ICE_COMET };
uint32 const g_CelestialEntries[4] = { NPC_FADED_IMAGE_OF_YU_LON, NPC_FADED_IMAGE_OF_XUEN, NPC_FADED_IMAGE_OF_NIUZAO, NPC_FADED_IMAGE_OF_CHI_JI };

Position const g_RoomCenter = { 6172.97f, 4239.63f, 146.55f, 0.0f };
Position const g_CelestialPos = { 6150.80f, 4266.85f, 146.55f, 5.4978f };

Position const g_StarPos[7] =
{
    { 6169.70f, 4264.08f, 146.55f, 0.0f },  // SW
    { 6148.47f, 4243.03f, 146.55f, 0.0f },  // SE
    { 6188.16f, 4255.12f, 146.55f, 0.0f },  // W
      g_RoomCenter,
    { 6157.78f, 4224.61f, 146.55f, 0.0f },  // E
    { 6197.18f, 4236.80f, 146.55f, 0.0f },  // NW
    { 6175.29f, 4215.75f, 146.55f, 0.0f }   // NE
};

uint8 const g_XuenDraw[7] = { 2, 7, 4, 1, 6, 4, 2 };
uint8 const g_NiuzaoDraw[6] = { 6, 4, 2, 1, 4, 7 };
uint8 const g_YulonDraw[5] = { 3, 1, 4, 2, 5 };
uint8 const g_ChijiDraw[6] = { 7, 2, 1, 4, 7, 6 };

uint32 const g_PatternSpells[4] = { SPELL_INVOKE_CRANE_SPIRIT, SPELL_INVOKE_OX_SPIRIT, SPELL_INVOKE_TIGER_SPIRIT, SPELL_INVOKE_SERPENT_SPIRIT };

class IsSuenVictim
{
    public:
        bool operator()(Player* p_Player) const
        {
            InstanceScript* l_Instance = p_Player->GetInstanceScript();
            if (!l_Instance)
                return false;

            if (Creature* l_Suen = l_Instance->instance->GetCreature(l_Instance->GetData64(NPC_SUEN)))
            {
                if (Unit* l_Victim = l_Suen->getVictim())
                    return l_Victim == p_Player;
            }

            return false;
        }
};

bool IsInRoomCenter(Creature* p_Creature)
{
    if (!p_Creature || p_Creature->GetMapId() != 1098)
        return false;

    bool l_IsValidX = p_Creature->GetPositionX() > 6170.0f && p_Creature->GetPositionX() < 6180.0f;
    bool l_IsValidY = p_Creature->GetPositionY() > 4235.0f && p_Creature->GetPositionY() < 4240.0f;

    return l_IsValidX && l_IsValidY;
}

void TwinDespawnAll(Creature* p_Creature)
{
    std::list<Creature*> l_AddList;
    for(uint32 l_Idx = 0; l_Idx < 4; ++l_Idx)
    {
        l_AddList.clear();
        GetCreatureListWithEntryInGrid(l_AddList, p_Creature, g_ConsortsAddEntries[l_Idx], 200.0f);
        for(Creature* l_Add : l_AddList)
            l_Add->DespawnOrUnsummon();
    }
}

Creature* GetOtherBoss(Creature* p_Boss)
{
    // Get InstanceScript
    InstanceScript* l_Instance = p_Boss->GetInstanceScript();
    if (!l_Instance)
        return nullptr;

    // Get Other Boss
    uint32 l_OtherBossEntry = p_Boss->GetEntry() == NPC_SUEN ? NPC_LU_LIN : NPC_SUEN;
    if (Creature* l_OtherBoss = l_Instance->instance->GetCreature(l_Instance->GetData64(l_OtherBossEntry)))
        return l_OtherBoss;
    else
        return nullptr;
}

void ToggleLotus(bool p_Active, InstanceScript* p_Instance)
{
    if (!p_Instance)
        return;

    Creature* l_Boss = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_LU_LIN));
    if (!l_Boss)
        l_Boss = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_SUEN));

    if (!l_Boss)
        return;

    std::list<Creature*> l_LotusList;
    GetCreatureListWithEntryInGrid(l_LotusList, l_Boss, NPC_MOON_LOTUS, 200.0f);

    if (!l_LotusList.empty())
    {
        for (Creature* l_Lotus : l_LotusList)
            l_Lotus->AI()->DoAction(p_Active ? ACTION_LOTUS_ACTIVATE : ACTION_LOTUS_DEACTIVATE);
    }

    // If deactivate, despawn spores
    if (!p_Active)
    {
        l_LotusList.clear();
        GetCreatureListWithEntryInGrid(l_LotusList, l_Boss, NPC_MOON_LOTUS_CLOUD, 100.0f);

        if (!l_LotusList.empty())
        {
            for (Creature* l_Spore : l_LotusList)
                l_Spore->DespawnOrUnsummon();
        }
    }
}

void ToggleCelestials(Creature* p_Creature, bool p_Apply)
{
    std::list<Creature*> l_CelestialList;
    for (uint8 l_Idx = 0; l_Idx < 4; ++l_Idx)
    {
        l_CelestialList.clear();
        GetCreatureListWithEntryInGrid(l_CelestialList, p_Creature, g_CelestialEntries[l_Idx], 200.0f);
        for (Creature* l_Celestial : l_CelestialList)
            l_Celestial->AI()->SetData(TYPE_TOGGLE_CELESTIALS, p_Apply ? VALUE_ENABLED : VALUE_DISABLED);
    }
}

// Changing phase ; makes that when a boss change his phase, the other boss also enters the new phase
void ChangePhase(uint8 p_Phase, InstanceScript* p_Instance)
{
    if (Creature* l_Suen = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_SUEN)))
    {
        l_Suen->AI()->SetData(TYPE_CHANGE_PHASE, p_Phase);
        if (p_Phase == PHASE_BREAK_OF_DUSK)
            ToggleCelestials(l_Suen, true);
    }

    if (Creature* l_Lulin = p_Instance->instance->GetCreature(p_Instance->GetData64(NPC_LU_LIN)))
        l_Lulin->AI()->SetData(TYPE_CHANGE_PHASE, p_Phase);

    // Deactivate lotus in Daylight Phase (P2) and reactivate lotus in Break of Dusk phase (P3)
    if (p_Phase == PHASE_DAYLIGHT || p_Phase == PHASE_BREAK_OF_DUSK)
        ToggleLotus(false, p_Instance);
}

//  When Twin consorts enter EvadeMode
void TwinEvadeMode(InstanceScript* p_Instance)
{
    if (!p_Instance)
        return;

    for (uint32 l_Idx = 0; l_Idx < 2; ++l_Idx)
    {
        if (Creature* l_Boss = p_Instance->instance->GetCreature(p_Instance->GetData64(g_ConsortsBossEntries[l_Idx])))
        {
            if (l_Boss->isDead())
                l_Boss->Respawn(true);
            l_Boss->SetReactState(REACT_PASSIVE);
            l_Boss->AI()->DoAction(ACTION_STOP_FIGHT);
            l_Boss->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            l_Boss->SetFullHealth();
            l_Boss->RemoveAllAuras();
            l_Boss->RemoveAllAreasTrigger();
            l_Boss->RemoveAllDynObjects();
            l_Boss->GetMotionMaster()->MoveTargetedHome();

            if (l_Boss->GetEntry() == NPC_SUEN)
            {
                TwinDespawnAll(l_Boss);
                if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(l_Boss, NPC_WORLD_TRIGGER, 100.0f))
                    l_WorldTrigger->RemoveAllAuras();

                ToggleCelestials(l_Boss, false);
            }
        }
    }

    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FLAMES_OF_PASSION_DMG);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BEAST_OF_NIGHTMARES_D);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FAN_OF_FLAMES);
    p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICY_SHADOWS);
    p_Instance->SetBossState(DATA_TWIN_CONSORTS, FAIL);
}

// When Twin consorts enter combat
void TwinEnterCombat(Unit* p_Attacker, Creature* p_FirstBoss)
{
    if (!p_FirstBoss)
        return;

    InstanceScript* l_Instance = p_FirstBoss->GetInstanceScript();
    if (!l_Instance || l_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS || !l_Instance->CheckRequiredBosses(DATA_TWIN_CONSORTS))
    {
        TwinEvadeMode(p_Attacker->GetInstanceScript());
        return;
    }


    p_FirstBoss->SetReactState(REACT_AGGRESSIVE);
    p_FirstBoss->AI()->SetData(TYPE_ENTER_COMBAT, 1);

    if (p_FirstBoss->GetEntry() == NPC_LU_LIN)
    {
        p_FirstBoss->SetInCombatWith(p_Attacker);
        p_FirstBoss->AI()->AttackStart(p_Attacker);
    }

    Creature* l_OtherBoss = GetOtherBoss(p_FirstBoss);

    if (l_OtherBoss)
    {
        l_OtherBoss->SetReactState(REACT_AGGRESSIVE);
        l_OtherBoss->AI()->SetData(TYPE_ENTER_COMBAT, 1);
        l_OtherBoss->AI()->EnterCombat(p_Attacker);

        if (l_OtherBoss->GetEntry() == NPC_LU_LIN)
        {
            l_OtherBoss->SetInCombatWith(p_Attacker);
            l_OtherBoss->AI()->AttackStart(p_Attacker);
        }
    }

    l_Instance->SetBossState(DATA_TWIN_CONSORTS, IN_PROGRESS);
    ChangePhase(PHASE_DARKNESS, l_Instance);
    ToggleCelestials(p_FirstBoss, true);
}

void TwinJustDied(InstanceScript* p_Instance, Creature* p_DeadBoss, bool p_LastPhase)
{
    p_DeadBoss->AI()->DoAction(ACTION_STOP_FIGHT);

    if (!p_Instance)
        return;

    // All bosses are done
    if (p_LastPhase)
    {
        p_DeadBoss->RemoveAllAreasTrigger();
        p_DeadBoss->RemoveAllAuras();
        p_DeadBoss->RemoveAllDynObjects();

        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FLAMES_OF_PASSION_DMG);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_BEAST_OF_NIGHTMARES_D);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_FAN_OF_FLAMES);
        p_Instance->DoRemoveAurasDueToSpellOnPlayers(SPELL_ICY_SHADOWS);
        p_Instance->SetBossState(DATA_TWIN_CONSORTS, DONE);

        TwinDespawnAll(p_DeadBoss);

        if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(p_DeadBoss, NPC_WORLD_TRIGGER, 100.0f))
            l_WorldTrigger->RemoveAllAuras();

        ToggleCelestials(p_DeadBoss, false);
    }
    // Change to phase 4
    else
    {
        // The first killed boss won't loose
        p_DeadBoss->SetLootRecipient(NULL);

        if (Creature* l_OtherBoss = GetOtherBoss(p_DeadBoss))
            l_OtherBoss->AI()->SetData(TYPE_CHANGE_PHASE, PHASE_LAST_PHASE);

        // If dead boss is Lu'lin, last phase will be a daylight phase, so lotus are deactivated
        if (p_DeadBoss->GetEntry() == NPC_LU_LIN)
            ToggleLotus(false, p_Instance);

        // Ambiant aura change to night or day phase
        if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(p_DeadBoss, NPC_WORLD_TRIGGER, 100.0f))
        {
            if (p_DeadBoss->GetEntry() == NPC_LU_LIN)
                l_WorldTrigger->RemoveAura(SPELL_STARRY_SKY);
            else
            {
                l_WorldTrigger->RemoveAura(SPELL_SUNNY_SKY);
                l_WorldTrigger->AddAura(SPELL_STARRY_SKY_BLUE_AURA, l_WorldTrigger);
            }
        }
    }
}

// 68905 - Lu'lin <Mistress of Solitude>
class boss_lulin : public CreatureScript
{
    public:
        boss_lulin() : CreatureScript("boss_lulin") { }

        struct boss_lulinAI : public BossAI
        {
            boss_lulinAI(Creature* p_Creature) : BossAI(p_Creature, DATA_TWIN_CONSORTS)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint8 m_Phase;
            uint32 m_PhaseTimer;
            bool m_InCombat;

            void Reset()
            {
                m_Phase = 0;
                m_PhaseTimer = 0;
                m_InCombat = false;
                m_Events.Reset();
                ResetPhases();

                me->SetReactState(REACT_PASSIVE);

                SetEquipmentSlots(false, EQUIP_LULIN_MAIN, EQUIP_LULIN_OFF);
            }

            void ResetPhases()
            {
                Map::PlayerList const& l_PlrList = me->GetMap()->GetPlayers();
                for (Map::PlayerList::const_iterator l_Itr = l_PlrList.begin(); l_Itr != l_PlrList.end(); ++l_Itr)
                {
                    if (Player* l_Plr = l_Itr->getSource())
                        l_Plr->SetPhaseMask(1, true);
                }
            }

            void DamageTaken(Unit* p_Attacker, uint32& /*p_Damage*/, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS || p_Attacker->GetTypeId() != TYPEID_PLAYER)
                    return;

                EnterCombat(p_Attacker);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (m_InCombat)
                    return;

                TwinEnterCombat(p_Attacker, me);
            }

            void EnterEvadeMode()
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS)
                    {
                        m_Events.Reset();
                        summons.DespawnAll();
                        TwinEvadeMode(m_Instance);
                        Talk(TALK_LULIN_WIPE);
                    }
                }
            }

            void JustReachedHome()
            {
                Reset();
            }

            void KilledUnit(Unit* p_Victim)
            {
                if (p_Victim->IsPlayer())
                    Talk(TALK_LULIN_SLAY);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                Talk(TALK_LULIN_DEATH);

                m_Events.Reset();

                if (m_Phase == PHASE_LAST_PHASE)
                {
                    summons.DespawnAll();
                    _JustDied();
                }

                TwinJustDied(m_Instance, me, m_Phase == PHASE_LAST_PHASE);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_STOP_FIGHT:
                    {
                        m_Events.Reset();
                        m_InCombat = false;

                        if (Creature* l_Suen = GetOtherBoss(me))
                        {
                            // if Suen is dead, encounter is done, if Lu'lin is alive, it's a wipe
                            if (!l_Suen->isAlive() || me->isAlive())
                                summons.DespawnAll();
                        }
                        break;
                    }
                    default:
                        break;
                }
            }

            void CheckHitResult(MeleeHitOutcome &p_MeleeHitResult, SpellMissInfo &p_SpellResult, Unit* /*p_Attacker*/)
            {
                if (me->HasAura(SPELL_TIDAL_FORCE))
                {
                    p_MeleeHitResult = MELEE_HIT_DODGE;
                    p_SpellResult = SPELL_MISS_DEFLECT;
                }
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                switch (p_Type)
                {
                    case TYPE_CHANGE_PHASE:
                    {
                        m_Phase = p_Value;

                        // New ability according the phase
                        switch (m_Phase)
                        {
                            case PHASE_DARKNESS:
                            {
                                if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                {
                                    me->AddAura(SPELL_STARRY_SKY_BLUE_AURA, l_WorldTrigger);
                                    me->AddAura(SPELL_STARRY_SKY, l_WorldTrigger);
                                }
                                m_Events.ScheduleEvent(EVENT_MOON_LOTUS, 30000);
                                m_Events.ScheduleEvent(EVENT_COSMIC_BARRAGE, 22000);
                                m_Events.ScheduleEvent(EVENT_BEAST_OF_NIGHTMARES, 51000);
                                m_PhaseTimer = 180000;
                                break;
                            }
                            case PHASE_DAYLIGHT:
                            {
                                me->AttackStop();
                                m_Events.Reset();
                                me->CastSpell(me, SPELL_DISSIPATE_LULIN, true);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_SELECTABLE);
                                m_Events.ScheduleEvent(EVENT_ICE_COMET, 20000);
                                Talk(TALK_LULIN_PHASE_DAYLIGHT);
                                break;
                            }
                            case PHASE_BREAK_OF_DUSK:
                            {
                                me->SetReactState(REACT_AGGRESSIVE);
                                m_Events.ScheduleEvent(EVENT_COSMIC_BARRAGE, 22000);
                                m_Events.ScheduleEvent(EVENT_TIDAL_FORCE, 18000);
                                m_Events.ScheduleEvent(EVENT_TALK_BREAK_OF_DUSK, 5000);
                                me->RemoveAura(SPELL_INVIS_LULIN);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NOT_SELECTABLE);
                                break;
                            }
                            case PHASE_LAST_PHASE:
                            {
                                m_Events.ScheduleEvent(EVENT_TALK_LAST_PHASE, 3000);
                                m_Events.ScheduleEvent(EVENT_MOON_LOTUS, 30000);
                                m_Events.ScheduleEvent(EVENT_BEAST_OF_NIGHTMARES, 22000);
                                m_Events.CancelEvent(EVENT_ICE_COMET);
                                m_Events.CancelEvent(EVENT_TIDAL_FORCE);
                                break;
                            }
                            default:
                                break;
                        }

                        break;
                    }
                    case TYPE_ENTER_COMBAT:
                    {
                        m_InCombat = true;
                        m_Events.ScheduleEvent(EVENT_TALK_AGGRO, 3000);
                        break;
                    }
                    default:
                        break;
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_GET_PHASE)
                    return m_Phase;
                return 0;
            }

            void UpdateAI(uint32 const p_Diff)
            {
                // On Wipe
                if (m_Instance)
                {
                    if (m_Instance->IsWipe() && m_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                // Phase change: not in events
                if (m_Phase == PHASE_DARKNESS)
                {
                    if (m_PhaseTimer <= p_Diff)
                    {
                        // Can change only if not casting AND other boss isn't casting
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            if (Creature* l_OtherBoss = GetOtherBoss(me))
                            {
                                if (!l_OtherBoss->HasUnitState(UNIT_STATE_CASTING))
                                {
                                    ChangePhase(PHASE_DAYLIGHT, m_Instance);
                                    m_PhaseTimer = 0;
                                }
                            }
                        }
                    }
                    else
                        m_PhaseTimer -= p_Diff;
                }

                if (me->HasUnitState(UNIT_STATE_CASTING) || !m_InCombat)
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_TALK_AGGRO:
                    {
                        Talk(TALK_LULIN_AGGRO);
                        break;
                    }
                    case EVENT_MOON_LOTUS:
                    {
                        // Choose a random point
                        float l_Dist  = frand(20.0f, 40.0f);
                        float l_Angle = frand(0.0f, M_PI * 2);
                        float l_PosX  = g_RoomCenter.m_positionX + l_Dist * cos(l_Angle);
                        float l_PosY  = g_RoomCenter.m_positionY + l_Dist * sin(l_Angle);

                        if (Creature* l_Lotus = me->SummonCreature(NPC_MOON_LOTUS, l_PosX, l_PosY, me->GetPositionZ()))
                            me->CastSpell(l_Lotus, SPELL_BLOOM_MOON_LOTUS, true);

                        m_Events.ScheduleEvent(EVENT_MOON_LOTUS, 30000);
                        break;
                    }
                    case EVENT_COSMIC_BARRAGE:
                    {
                        me->SetReactState(REACT_PASSIVE);
                        me->CastSpell(me, SPELL_COSMIC_BARRAGE, true);
                        m_Events.ScheduleEvent(EVENT_COSMIC_BARRAGE, 22000);
                        break;
                    }
                    case EVENT_BEAST_OF_NIGHTMARES:
                    {
                        // Assuming current victim of Lu'lin has a Tank role
                        uint64 l_TargetGuid = 0;
                        if (Unit* l_Victim = me->getVictim())
                            l_TargetGuid = l_Victim->GetGUID();

                        // Retreiving player list
                        Map::PlayerList const& l_PlayerList = me->GetMap()->GetPlayers();
                        uint8 l_Chance = 0; // Chance to be chosen for a tank : 50% for the 1st one, 100% for the second one

                        // Picking a tank or the current victim to be target of the Beast
                        for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                        {
                            if (Player* l_Plr = l_Itr->getSource())
                            {
                                if (l_Plr->GetRoleForGroup() == ROLE_TANK || (l_TargetGuid && l_Plr->GetGUID() == l_TargetGuid))
                                {
                                    if (urand(l_Chance, 1))
                                    {
                                        me->CastSpell(l_Itr->getSource(), SPELL_BEAST_OF_NIGHTMARES, true);
                                        break;
                                    }
                                    else
                                        ++l_Chance;
                                }
                            }
                        }
                        m_Events.ScheduleEvent(EVENT_BEAST_OF_NIGHTMARES, 51000);
                        break;
                    }
                    case EVENT_ICE_COMET:
                    {
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                        if (!l_PlayerList.empty())
                        {
                            JadeCore::RandomResizeList(l_PlayerList, 1);
                            if (Player* l_Target = l_PlayerList.front())
                            {
                                if (m_Phase == PHASE_DAYLIGHT)
                                    me->CastSpell(l_Target, SPELL_ICE_COMET_LULIN_VIS, true);
                                if (Creature* l_Comet = me->SummonCreature(NPC_ICE_COMET, l_Target->GetPositionX(), l_Target->GetPositionY(), l_Target->GetPositionZ(), frand(0.0f, M_PI * 2)))
                                    l_Comet->AI()->DoAction(ACTION_ACTIVATE_COMET);
                            }
                        }
                        m_Events.ScheduleEvent(EVENT_ICE_COMET, 20000);
                        break;
                    }
                    case EVENT_TIDAL_FORCE:
                    {
                        me->SetReactState(REACT_PASSIVE);
                        me->CastSpell(me, SPELL_TIDAL_FORCE, true);
                        m_Events.ScheduleEvent(EVENT_TIDAL_FORCE, 71000);
                        break;
                    }
                    case EVENT_TALK_BREAK_OF_DUSK:
                    {
                        Talk(TALK_LULIN_BREAK_OF_DUSK);
                        break;
                    }
                    case EVENT_TALK_LAST_PHASE:
                    {
                        Talk(TALK_LULIN_FINAL_PHASE);
                        break;
                    }
                    default:
                        break;
                }
                if (m_Phase != PHASE_DAYLIGHT)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_lulinAI(p_Creature);
        }
};

// 68905 - Suen <Mistress of Anger>
class boss_suen : public CreatureScript
{
    public:
        boss_suen() : CreatureScript("boss_suen") { }

        struct boss_suenAI : public BossAI
        {
            boss_suenAI(Creature* p_Creature) : BossAI(p_Creature, DATA_TWIN_CONSORTS)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            uint8 m_Phase;
            uint32 m_PhaseTimer;
            uint64 m_CurrentTankGuid;
            uint64 m_FlamesTargetGuid;
            bool m_InCombat;

            void Reset()
            {
                m_Events.Reset();
                m_Phase = 0;
                m_PhaseTimer = 0;
                m_CurrentTankGuid = 0;
                m_FlamesTargetGuid = 0;
                m_InCombat = false;

                SetEquipmentSlots(false, EQUIP_SUEN_MAIN, EQUIP_SUEN_OFF);

                me->SetReactState(REACT_PASSIVE);
            }

            void DamageTaken(Unit* p_Attacker, uint32& /*p_Damage*/, SpellInfo const*  /*p_SpellInfo*/)
            {
                if (!m_Instance)
                    return;

                if (m_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS || p_Attacker->GetTypeId() != TYPEID_PLAYER)
                    return;

                EnterCombat(p_Attacker);
            }

            void EnterCombat(Unit* p_Attacker)
            {
                if (m_InCombat)
                    return;

                TwinEnterCombat(p_Attacker, me);
            }

            void EnterEvadeMode()
            {
                if (m_Instance)
                {
                    if (m_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS)
                    {
                        m_Events.Reset();
                        summons.DespawnAll();
                        TwinEvadeMode(m_Instance);
                        Talk(TALK_SUEN_WIPE);
                    }
                }
            }

            void JustReachedHome()
            {
                Reset();
            }

            void KilledUnit(Unit* p_Victim)
            {
                if (p_Victim->IsPlayer())
                    Talk(TALK_SUEN_SLAY);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                Talk(TALK_SUEN_DEATH);

                m_Events.Reset();

                if (m_Phase == PHASE_LAST_PHASE)
                {
                    summons.DespawnAll();
                    _JustDied();
                }

                TwinJustDied(m_Instance, me, m_Phase == PHASE_LAST_PHASE);
            }

            void DoAction(int32 const p_Action)
            {
                switch(p_Action)
                {
                    case ACTION_STOP_FIGHT:
                    {
                        m_Events.Reset();
                        m_InCombat = false;

                        if (Creature* l_Lulin = GetOtherBoss(me))
                        {
                            // if Lu'lin is dead, encounter is done, if Suen is alive, it's a wipe
                            if (!l_Lulin->isAlive() || me->isAlive())
                                summons.DespawnAll();
                        }
                        break;
                    }
                    case ACTION_FLAMES_BACK:
                    {
                        m_Events.ScheduleEvent(EVENT_FLAMES_BACK, 500);
                        break;
                    }
                    case ACTION_STOP_FLAMES_AT:
                    {
                        m_Events.CancelEvent(EVENT_FLAMES_AT);
                        break;
                    }
                    default:
                        break;
                }
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                switch (p_Type)
                {
                    case TYPE_CHANGE_PHASE:
                    {
                        m_Phase = p_Value;

                        // New ability according the phase
                        switch (m_Phase)
                        {
                            case PHASE_DARKNESS:
                            {
                                me->AttackStop();
                                me->CastSpell(me, SPELL_DISSIPATE_SUEN, true);
                                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                                // Events
                                m_Events.ScheduleEvent(EVENT_TEARS_OF_THE_SUN, 10000);
                                m_Events.ScheduleEvent(EVENT_LIGHT_OF_DAY, 6000);
                                break;
                            }
                            case PHASE_DAYLIGHT:
                            {
                                m_Events.CancelEvent(EVENT_TEARS_OF_THE_SUN);

                                if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                {
                                    l_WorldTrigger->RemoveAura(SPELL_STARRY_SKY);
                                    l_WorldTrigger->RemoveAura(SPELL_STARRY_SKY_BLUE_AURA);
                                    me->AddAura(SPELL_SUNNY_SKY, l_WorldTrigger);
                                }
                                me->RemoveAura(SPELL_INVIS_SUEN);
                                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                                me->SetReactState(REACT_AGGRESSIVE);

                                if (Creature* l_Lulin = GetOtherBoss(me))
                                {
                                    if (Unit* l_Victim = l_Lulin->getVictim())
                                    {
                                        me->SetInCombatWith(l_Victim);
                                        AttackStart(l_Victim);
                                    }
                                }

                                me->CastSpell(me, SPELL_BLAZING_RADIANCE, true);

                                // Events
                                m_PhaseTimer = 180000;
                                m_Events.ScheduleEvent(EVENT_FAN_OF_FLAMES, 6000);
                                m_Events.ScheduleEvent(EVENT_LIGHT_OF_DAY, 12000);
                                m_Events.ScheduleEvent(EVENT_FLAMES_OF_PASSION, 30000);
                                m_Events.ScheduleEvent(EVENT_TALK_DAYLIGHT, 5000);
                                break;
                            }
                            case PHASE_BREAK_OF_DUSK:
                            {
                                m_Events.CancelEvent(EVENT_FAN_OF_FLAMES);

                                if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                {
                                    l_WorldTrigger->RemoveAura(SPELL_SUNNY_SKY);
                                    me->AddAura(SPELL_STARRY_SKY, l_WorldTrigger);
                                }

                                Talk(TALK_SUEN_BREAK_OF_DUSK);
                                break;
                            }
                            case PHASE_LAST_PHASE:
                            {
                                m_Events.ScheduleEvent(EVENT_TALK_LAST_PHASE, 3000);
                                m_Events.ScheduleEvent(EVENT_FAN_OF_FLAMES, 6000);
                                break;
                            }
                            default:
                                break;
                        }
                        break;
                    }
                    case  TYPE_ENTER_COMBAT:
                    {
                        m_InCombat = true;
                        Talk(TALK_SUEN_AGGRO);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                // On Wipe
                if (m_Instance)
                {
                    if (m_Instance->IsWipe() && m_Instance->GetBossState(DATA_TWIN_CONSORTS) == IN_PROGRESS)
                    {
                        EnterEvadeMode();
                        return;
                    }
                }

                // Phase change: not in events
                if (m_Phase == PHASE_DAYLIGHT)
                {
                    if (m_PhaseTimer <= p_Diff)
                    {
                        // Can change only if not casting AND other boss isn't casting
                        if (!me->HasUnitState(UNIT_STATE_CASTING))
                        {
                            if (Creature* l_OtherBoss = GetOtherBoss(me))
                            {
                                if (!l_OtherBoss->HasUnitState(UNIT_STATE_CASTING))
                                {
                                    ChangePhase(PHASE_BREAK_OF_DUSK, m_Instance);
                                    m_PhaseTimer = 0;
                                }
                            }
                        }
                    }
                    else
                        m_PhaseTimer -= p_Diff;
                }

                m_Events.Update(p_Diff);

                if (me->HasUnitState(UNIT_STATE_CASTING) || !m_InCombat)
                    return;

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_TEARS_OF_THE_SUN:
                    {
                        if (m_Phase == PHASE_DARKNESS)
                            me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);

                        me->CastSpell(me, SPELL_TEARS_OF_THE_SUN, true);
                        m_Events.ScheduleEvent(EVENT_TEARS_OF_THE_SUN, 41000);
                        break;
                    }
                    case EVENT_LIGHT_OF_DAY:
                    {
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);
                        if (!l_PlayerList.empty())
                        {
                            JadeCore::RandomResizeList(l_PlayerList, 1);
                            me->CastSpell(l_PlayerList.front(), me->HasAura(SPELL_INVIS_SUEN) ? SPELL_LIGHT_OF_DAY_ASSIST : SPELL_LIGHT_OF_DAY, true);
                            me->CastSpell(l_PlayerList.front(), SPELL_LIGHT_OF_DAY_COMET, true);
                        }

                        m_Events.ScheduleEvent(EVENT_LIGHT_OF_DAY, 10000);
                        break;
                    }
                    case EVENT_TALK_DAYLIGHT:
                    {
                        Talk(TALK_SUEN_PHASE_DAYLIGHT);
                        break;
                    }
                    case EVENT_FAN_OF_FLAMES:
                    {
                        if (Unit* l_Victim = me->getVictim())
                            me->CastSpell(l_Victim, SPELL_FAN_OF_FLAMES, true);
                        m_Events.ScheduleEvent(EVENT_FAN_OF_FLAMES, 12000);
                        break;
                    }
                    case EVENT_FLAMES_OF_PASSION:
                    {
                        // Getting the player to charge
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                        if (!l_PlayerList.empty())
                        {
                            // Removing the current target from the list
                            if (Unit* l_CurrentVictim = me->getVictim())
                            {
                                // Remove it only if there're other players to target
                                if (l_PlayerList.size() > 1)
                                    l_PlayerList.remove_if(IsSuenVictim());
                                m_CurrentTankGuid = l_CurrentVictim->GetGUID();
                            }

                            // Selecting a random target
                            if (!l_PlayerList.empty())
                            {
                                JadeCore::RandomResizeList(l_PlayerList, 1);
                                if (Player* l_Target = l_PlayerList.front())
                                {
                                    me->SetReactState(REACT_PASSIVE);
                                    m_FlamesTargetGuid = l_Target->GetGUID();
                                    Talk(TALK_SUEN_FLAMES);
                                    me->CastSpell(l_Target, SPELL_FLAMES_OF_PASSION, true);
                                }
                            }

                            m_Events.ScheduleEvent(EVENT_FLAMES_BACK, 500);
                        }

                        m_Events.ScheduleEvent(EVENT_FLAMES_OF_PASSION, urand(30000, 50000));
                        break;
                    }
                    case EVENT_FLAMES_BACK:
                    {
                        // If not close enough from the target, wait a bit
                        if (Player* l_FlamesTarget = Player::GetPlayer(*me, m_FlamesTargetGuid))
                        {
                            if (me->GetDistance2d(l_FlamesTarget) < MIN_MELEE_REACH)
                            {
                                m_Events.ScheduleEvent(EVENT_FLAMES_BACK, 500);
                                break;
                            }
                        }

                        // Close enough: bump and way back to tank
                        if (Player* l_Tank = Player::GetPlayer(*me, m_CurrentTankGuid))
                        {
                            me->CastSpell(me, SPELL_FLAMES_OF_PASSION_BMP, true);
                            me->CastSpell(l_Tank, SPELL_FLAMES_OF_PASSION_BCK, true);
                            me->CastSpell(me, SPELL_FLAMES_OF_PASSION_AT, true);
                            m_Events.ScheduleEvent(EVENT_FLAMES_AT, 500);
                        }
                        break;
                    }
                    case EVENT_FLAMES_AT:
                    {
                        me->CastSpell(me, SPELL_FLAMES_OF_PASSION_AT, true);
                        if (Player* l_Tank = Player::GetPlayer(*me, m_CurrentTankGuid))
                        {
                            if (me->GetDistance2d(l_Tank) > MIN_MELEE_REACH * 2)
                                m_Events.ScheduleEvent(EVENT_FLAMES_AT, 500);
                            else
                                me->SetReactState(REACT_AGGRESSIVE);
                        }
                        break;
                    }
                    case EVENT_TALK_LAST_PHASE:
                    {
                        Talk(TALK_SUEN_FINAL_PHASE);
                        break;
                    }
                    default:
                        break;
                }

                if (m_Phase != PHASE_DARKNESS)
                    DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new boss_suenAI(p_Creature);
        }
};

// 69456 - Moon Lotus (flower)
class mob_moon_lotus : public CreatureScript
{
    public:
        mob_moon_lotus() : CreatureScript("mob_moon_lotus") { }

        struct mob_moon_lotusAI : public ScriptedAI
        {
            mob_moon_lotusAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            bool m_IsActive;

            void Reset()
            {
                m_Events.Reset();
                m_IsActive = false;
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                me->SetReactState(REACT_PASSIVE);
                me->AddUnitState(UNIT_STATE_ROOT);
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_LOTUS_ACTIVATE:
                    {
                        m_IsActive = true;
                        me->AddAura(SPELL_MOON_LOTUS_VISUAL, me);
                        m_Events.ScheduleEvent(EVENT_LOTUS_SPORE, 4000);
                        break;
                    }
                    case ACTION_LOTUS_DEACTIVATE:
                    {
                        m_IsActive = false;
                        me->AddAura(SPELL_MOON_LOTUS_DEACTIVATE, me);
                        m_Events.Reset();
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_LOTUS_SPORE)
                {
                    me->SummonCreature(NPC_MOON_LOTUS_CLOUD, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ(), frand(0.0f, 2 * M_PI));
                    m_Events.ScheduleEvent(EVENT_LOTUS_SPORE, 4000);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_moon_lotusAI(p_Creature);
        }
};

// 69340 - Moon Lotus (spore)
class mob_moon_lotus_spore : public CreatureScript
{
    public:
        mob_moon_lotus_spore() : CreatureScript("mob_moon_lotus_spore") { }

        struct mob_moon_lotus_sporeAI : public ScriptedAI
        {
            mob_moon_lotus_sporeAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;

            void Reset()
            {
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_IMMUNE_TO_NPC | UNIT_FLAG_IMMUNE_TO_PC);
                me->AddAura(SPELL_SLUMBER_SPORE_VISUAL, me);
                me->DespawnOrUnsummon(15000);

                me->SetSpeed(MOVE_RUN, 0.2f);

                m_Events.ScheduleEvent(EVENT_SPORE_MOVE, 100);
                m_Events.ScheduleEvent(EVENT_SPORE_CHECK_PLAYERS, 500);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                m_Events.Update(p_Diff);

                uint32 l_EventId = m_Events.ExecuteEvent();

                if (l_EventId == EVENT_SPORE_MOVE)
                {
                    float l_PosX = me->GetPositionX() + 50.0f * cos(me->GetOrientation());
                    float l_PosY = me->GetPositionY() + 50.0f * sin(me->GetOrientation());

                    me->GetMotionMaster()->MovePoint(0, l_PosX, l_PosY, me->GetPositionZ());
                }

                if (l_EventId == EVENT_SPORE_CHECK_PLAYERS)
                {
                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, me, 0.1f);

                    if (!l_PlayerList.empty())
                    {
                        for (Player* l_Player : l_PlayerList)
                        {
                            if (!l_Player->HasAura(SPELL_SLUMBER_SPORE))
                                me->AddAura(SPELL_SLUMBER_SPORE, l_Player);
                        }
                    }

                    m_Events.ScheduleEvent(EVENT_SPORE_CHECK_PLAYERS, 500);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_moon_lotus_sporeAI(p_Creature);
        }
};

// Celestials
// 69771 - Faded image of Xuen
// 69774 - Faded image of Chi-Ji
// 69777 - Faded image of Niuzao
// 69779 - Faded image of Yu'lon
class mob_celestial : public CreatureScript
{
    public:
        mob_celestial() : CreatureScript("mob_celestial") { }

        struct mob_celestialAI : public ScriptedAI
        {
            mob_celestialAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            EventMap m_Events;
            uint32 m_DrawSpell;
            uint8 m_PhaseMask;
            uint8 m_CraneSummoned;
            bool m_InCenter;
            bool m_Enabled;
            bool m_ActivateBuff;

            void Reset()
            {
                m_Events.Reset();
                m_Enabled = false;
                m_ActivateBuff = false;
                m_DrawSpell = 0;
                m_PhaseMask = 0;
                m_CraneSummoned = 0;

                if (!(m_InCenter = IsInRoomCenter(me)))
                {
                    me->SetDisableGravity(true);
                    me->SetCanFly(true);
                    me->SetByteFlag(UNIT_FIELD_ANIM_TIER, 3, UNIT_BYTE1_FLAG_HOVER);

                    uint32 l_CelestialAura = me->GetEntry() == NPC_FADED_IMAGE_OF_NIUZAO ? SPELL_YELLOW_SHADOWY :
                                             me->GetEntry() == NPC_FADED_IMAGE_OF_CHI_JI ? SPELL_RED_SHADOWY :
                                             me->GetEntry() == NPC_FADED_IMAGE_OF_XUEN   ? SPELL_BLUE_SHADOWY : SPELL_GREEN_SHADOWY;

                    m_DrawSpell = me->GetEntry() == NPC_FADED_IMAGE_OF_NIUZAO ? SPELL_INVOKE_OX_SPIRIT :
                                  me->GetEntry() == NPC_FADED_IMAGE_OF_CHI_JI ? SPELL_INVOKE_CRANE_SPIRIT :
                                  me->GetEntry() == NPC_FADED_IMAGE_OF_XUEN   ? SPELL_INVOKE_TIGER_SPIRIT : SPELL_INVOKE_SERPENT_SPIRIT;

                    // Each spirit must use a different phaseMask to avoid several players make a mess if they activate many celestials at the same time
                    m_PhaseMask = me->GetEntry() == NPC_FADED_IMAGE_OF_NIUZAO ? 2 :
                                  me->GetEntry() == NPC_FADED_IMAGE_OF_CHI_JI ? 4 :
                                  me->GetEntry() == NPC_FADED_IMAGE_OF_XUEN   ? 8 : 16;

                    me->AddAura(l_CelestialAura, me);
                }
                else
                {
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
                    me->SetDisplayId(me->GetEntry() == NPC_FADED_IMAGE_OF_YU_LON ? DISPLAY_INVISIBLE_YULON : DISPLAY_INVISIBLE_CELESTIALS);
                }
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_CELESTIAL_BUFF)
                {
                    m_ActivateBuff = true;
                    Talk(TALK_CELESTIAL);

                    switch (m_DrawSpell)
                    {
                        case SPELL_INVOKE_CRANE_SPIRIT:
                        {
                            if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                l_WorldTrigger->CastSpell(l_WorldTrigger, SPELL_CRANE_BUFF_VISUAL, true);
                            m_Events.ScheduleEvent(EVENT_SUMMON_RED_CRANE, 100);
                            break;
                        }
                        case SPELL_INVOKE_OX_SPIRIT:
                        {
                            if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                l_WorldTrigger->CastSpell(l_WorldTrigger, SPELL_OX_BUFF_VISUAL, true);

                            Map::PlayerList const &l_PlayerList = me->GetMap()->GetPlayers();
                            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
                                me->CastSpell(l_Itr->getSource(), SPELL_FORTITUDE_OF_THE_OX, true);
                            break;
                        }
                        case SPELL_INVOKE_SERPENT_SPIRIT:
                        {
                            if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                l_WorldTrigger->CastSpell(l_WorldTrigger, SPELL_SERPENTS_VITALITY, true);
                            break;
                        }
                        case SPELL_INVOKE_TIGER_SPIRIT:
                        {
                            if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                                l_WorldTrigger->CastSpell(l_WorldTrigger, SPELL_XUEN_ALACRITY, true);
                            break;
                        }
                        default:
                            break;
                    }
                }
            }

            void OnSpellClick(Unit* p_Player)
            {
                if (p_Player->GetTypeId() != TYPEID_PLAYER || m_InCenter || !m_Enabled)
                    return;

                m_Enabled = false;

                p_Player->NearTeleportTo(g_CelestialPos, false);
                p_Player->SetPhaseMask(2, true);
                me->AddAura(m_DrawSpell, p_Player);

                for (uint8 l_Idx = 0; l_Idx < 7; ++l_Idx)
                {
                    if (Creature* l_Star = me->SummonCreature(NPC_ASTRAL_STAR, g_StarPos[l_Idx], TEMPSUMMON_TIMED_DESPAWN, 30000))
                    {
                        l_Star->AI()->SetData(TYPE_SET_STAR_ID, l_Idx + 1);
                        l_Star->AI()->SetData(TYPE_SET_DRAW, me->GetEntry());
                    }
                }
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                if (p_Type == TYPE_TOGGLE_CELESTIALS && !m_InCenter)
                {
                    m_Enabled = (p_Value == VALUE_ENABLED);
                    if (m_Enabled)
                    {
                        m_ActivateBuff = false;
                        m_CraneSummoned = 0;
                    }
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_InCenter || !m_ActivateBuff)
                    return;

                m_Events.Update(p_Diff);

                if (m_Events.ExecuteEvent() == EVENT_SUMMON_RED_CRANE)
                {
                    if (Creature* l_WorldTrigger = GetClosestCreatureWithEntry(me, NPC_WORLD_TRIGGER, 200.0f))
                    {
                        uint8 l_CraneSummoned = urand(2, 4);
                        for (uint8 l_Idx = 0; l_Idx < l_CraneSummoned; ++l_Idx)
                        {
                            float l_Arc  = frand(0.0f, M_PI);
                            float l_Dist = frand(30.0f, 60.0f);
                            float l_PosX = l_WorldTrigger->GetPositionX() + l_Dist * cos(l_Arc);
                            float l_PosY = l_WorldTrigger->GetPositionY() + l_Dist * sin(l_Arc);

                            me->SummonCreature(NPC_IMAGE_OF_CHI_JI, l_PosX, l_PosY, g_RoomCenter.GetPositionZ(), M_PI * 1.5f);
                        }

                        ++m_CraneSummoned;
                        if (m_CraneSummoned < 20)
                            m_Events.ScheduleEvent(EVENT_SUMMON_RED_CRANE, 1000);
                        else
                            m_ActivateBuff = false;
                    }
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_celestialAI(p_Creature);
        }
};

// 69479 - Beast of nightmares
class mob_beast_of_nightmares : public CreatureScript
{
    public:
        mob_beast_of_nightmares() : CreatureScript("mob_beast_of_nightmares") { }

        struct mob_beast_of_nightmaresAI : public ScriptedAI
        {
            mob_beast_of_nightmaresAI(Creature* p_Creature) : ScriptedAI(p_Creature) { }

            uint64 m_TargetGuid;

            void Reset()
            {
                m_TargetGuid = 0;
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

                Player* l_Victim = me->SelectNearestPlayer(5.0f);

                if (l_Victim)
                {
                    m_TargetGuid = l_Victim->GetGUID();
                    me->SetInCombatWith(l_Victim);
                    AttackStart(l_Victim);
                    me->AddAura(SPELL_BEAST_OF_NIGHTMARES_D, l_Victim);
                    me->AddThreat(l_Victim, 1000000.0f);
                }
                else
                    me->DespawnOrUnsummon();
            }

            void JustDied(Unit* p_Killer)
            {
                if (Player* p_Player = p_Killer->ToPlayer())
                {
                    if (p_Player->HasAura(SPELL_BEAST_OF_NIGHTMARES_D))
                        p_Player->RemoveAura(SPELL_BEAST_OF_NIGHTMARES_D);
                }
            }

            void CheckHitResult(MeleeHitOutcome& p_MeleeResult, SpellMissInfo& p_SpellResult, Unit* p_Attacker)
            {
                if (p_Attacker->GetGUID() == m_TargetGuid && p_Attacker->HasAura(SPELL_BEAST_OF_NIGHTMARES_D))
                {
                    p_MeleeResult = MELEE_HIT_NORMAL;
                    p_SpellResult = SPELL_MISS_NONE;
                }
                else
                {
                    p_MeleeResult = MELEE_HIT_MISS;
                    p_SpellResult = SPELL_MISS_MISS;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_beast_of_nightmaresAI(p_Creature);
        }
};

// 69596 - Ice comet
class mob_ice_comet : public CreatureScript
{
    public:
        mob_ice_comet() : CreatureScript("mob_ice_comet") { }

        struct mob_ice_cometAI : public ScriptedAI
        {
            mob_ice_cometAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;

            void Reset()
            {
                m_Events.Reset();
                me->SetReactState(REACT_PASSIVE);
                me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE | UNIT_FLAG_NOT_SELECTABLE);
            }

            void JustDied(Unit* /*p_Killer*/)
            {
                // Prevent dead comet to remove blazing radiance stack
                me->DespawnOrUnsummon();
            }

            void DoAction(int32 const p_Action)
            {
                switch (p_Action)
                {
                    case ACTION_ACTIVATE_COMET:
                    {
                        if (m_Instance)
                        {
                            if (Creature* l_Lulin = m_Instance->instance->GetCreature(m_Instance->GetData64(NPC_LU_LIN)))
                            {
                                if (l_Lulin->AI()->GetData(TYPE_GET_PHASE) == PHASE_DAYLIGHT)
                                    l_Lulin->CastSpell(me, SPELL_ICE_COMET_BUMP_DELAY, true);
                                else
                                    l_Lulin->CastSpell(me, SPELL_ICE_COMET_BUMP, true);
                            }
                        }
                        break;
                    }
                    case ACTION_COMET_IN_PLACE:
                    {
                        me->CastSpell(me, SPELL_ICE_COMET_DOWN, true);
                        me->CastSpell(me, SPELL_ICY_SHADOWS, true);
                        m_Events.ScheduleEvent(EVENT_CHECK_SUEN, 1000);
                        m_Events.ScheduleEvent(EVENT_ICY_SHADOWS, 1500);
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!me->HasAura(SPELL_ICE_COMET_DOWN))
                    return;

                m_Events.Update(p_Diff);

                switch(m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_SUEN:
                    {
                        if (Creature* l_Lulin = GetClosestCreatureWithEntry(me, NPC_SUEN, 7.0f))
                            l_Lulin->DealDamage(me, me->GetMaxHealth() * (0.2f / me->GetDistance2d(l_Lulin)));
                        m_Events.ScheduleEvent(EVENT_CHECK_SUEN, 1000);
                        break;
                    }
                    case EVENT_ICY_SHADOWS:
                    {
                        me->CastSpell(me, SPELL_ICY_SHADOWS, true);
                        m_Events.ScheduleEvent(EVENT_ICY_SHADOWS, 1500);
                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_ice_cometAI(p_Creature);
        }
};

// 64677 - Astral Star
class mob_astral_star : public CreatureScript
{
    public:
        mob_astral_star() : CreatureScript("mob_astral_star") { }

        struct mob_astral_starAI : public ScriptedAI
        {
            mob_astral_starAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            InstanceScript* m_Instance;
            EventMap m_Events;
            std::list<uint8> m_PreviousStarsList;
            uint8 m_StarId;
            uint8 m_StarCount;
            uint8 m_StarMax;
            uint32 m_DrawToPerform;
            uint64 m_SummonerGuid;
            bool m_ShouldCheck;

            void Reset()
            {
                m_Events.Reset();
                m_PreviousStarsList.clear();
                m_StarId = 0;
                m_StarCount = 0;
                m_StarMax = 0;
                m_DrawToPerform = 0;
                m_SummonerGuid = 0;
                m_ShouldCheck = false;
                me->SetPhaseMask(2, true);
            }

            void IsSummonedBy(Unit* p_Summoner)
            {
                m_SummonerGuid = p_Summoner->GetGUID();
            }

            void DoAction(int32 const p_Action)
            {
                if (p_Action == ACTION_ACTIVATE_NEXT_STAR)
                {
                    m_ShouldCheck = true;
                    m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                }
            }

            uint32 GetData(uint32 p_Type)
            {
                if (p_Type == TYPE_GET_STAR_ID)
                    return m_StarId;
                return 0;
            }

            void SetData(uint32 p_Type, uint32 p_Value)
            {
                switch (p_Type)
                {
                    case TYPE_SET_STAR_ID:
                    {
                        m_StarId = p_Value;
                        break;
                    }
                    case TYPE_SET_DRAW:
                    {
                        m_DrawToPerform = p_Value;

                        if (!m_StarId)
                            return;

                        uint8 l_FirstStar = 0;

                        switch (m_DrawToPerform)
                        {
                            case NPC_FADED_IMAGE_OF_YU_LON:
                            {
                                m_StarMax = 5;
                                l_FirstStar = 3;
                                break;
                            }
                            case NPC_FADED_IMAGE_OF_XUEN:
                            {
                                m_StarMax = 7;
                                l_FirstStar = 2;
                                break;
                            }
                            case NPC_FADED_IMAGE_OF_CHI_JI:
                            {
                                m_StarMax = 6;
                                l_FirstStar = 7;
                                break;
                            }
                            case NPC_FADED_IMAGE_OF_NIUZAO:
                            {
                                m_StarMax = 6;
                                l_FirstStar = 6;
                                break;
                            }
                            default:
                                break;
                        }

                        if (m_StarId == l_FirstStar)
                        {
                            me->AddAura(SPELL_FIRST_STAR, me);
                            me->AddAura(SPELL_FIRST_STAR_HALO, me);
                            m_ShouldCheck = true;
                            m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                        }
                        break;
                    }
                    case TYPE_ADD_STAR:
                    {
                        m_PreviousStarsList.push_back(p_Value);
                        ++m_StarCount;
                        break;
                    }
                    default:
                        break;
                }
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (!m_StarId || !m_DrawToPerform || !m_ShouldCheck)
                    return;

                m_Events.Update(p_Diff);

                switch (m_Events.ExecuteEvent())
                {
                    case EVENT_CHECK_PLAYER:
                    {
                        // Get list of near players
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 1.0f);

                        // No player around, check later
                        if (l_PlayerList.empty())
                        {
                            m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                            return;
                        }

                        // There's at least 1 player around
                        Player* l_Player = nullptr;
                        while (!l_PlayerList.empty() && !l_Player)
                        {
                            // Get only player which has the right phase
                            if (l_PlayerList.front()->GetPhaseMask() == 2)
                                l_Player = l_PlayerList.front();
                            // else, remove it from the list (and so next player in the list become the first one)
                            else
                                l_PlayerList.remove(l_PlayerList.front());
                        }

                        // We got a player
                        if (l_Player)
                        {
                            bool l_Check = false;
                            uint8 l_NextStar = 0;
                            // Checking if the next star matches with the star Id
                            switch (m_DrawToPerform)
                            {
                                case NPC_FADED_IMAGE_OF_NIUZAO:
                                {
                                    if (g_NiuzaoDraw[m_StarCount] == m_StarId)
                                    {
                                        l_Check = true;
                                        l_NextStar = m_StarMax == m_StarCount + 1 ? 0 : g_NiuzaoDraw[m_StarCount + 1];
                                    }
                                    break;
                                }
                                case NPC_FADED_IMAGE_OF_XUEN:
                                {
                                    if (g_XuenDraw[m_StarCount] == m_StarId)
                                    {
                                        l_Check = true;
                                        l_NextStar = m_StarMax == m_StarCount + 1 ? 0 : g_XuenDraw[m_StarCount + 1];
                                    }
                                    break;
                                }
                                case NPC_FADED_IMAGE_OF_YU_LON:
                                {
                                    if (g_YulonDraw[m_StarCount] == m_StarId)
                                    {
                                        l_Check = true;
                                        l_NextStar = m_StarMax == m_StarCount + 1 ? 0 : g_YulonDraw[m_StarCount + 1];
                                    }
                                    break;
                                }
                                case NPC_FADED_IMAGE_OF_CHI_JI:
                                {
                                    if (g_ChijiDraw[m_StarCount] == m_StarId)
                                    {
                                        l_Check = true;
                                        l_NextStar = m_StarMax == m_StarCount + 1 ? 0 : g_ChijiDraw[m_StarCount + 1];
                                    }
                                    break;
                                }
                                default:
                                    break;
                            }

                            // The star is the next to be activated
                            if (l_Check)
                            {
                                ++m_StarCount;
                                me->CastSpell(me, SPELL_STAR_STATE_ASTRAL, true);
                                std::list<Creature*> l_StarList;
                                GetCreatureListWithEntryInGrid(l_StarList, me, me->GetEntry(), 200.0f);
                                uint8 l_PreviousStar = m_PreviousStarsList.empty() ? 0 : *m_PreviousStarsList.rbegin();
                                m_PreviousStarsList.push_back(m_StarId);
                                m_ShouldCheck = false;

                                // Check if pattern is ended
                                if (m_StarCount == m_StarMax)
                                {
                                    m_Events.ScheduleEvent(EVENT_END_PATTERN, 1000);
                                    m_ShouldCheck = true;
                                }

                                // Update the other creature and create a beam to the previous star
                                for (Creature* l_Star : l_StarList)
                                {
                                    if (l_Star == me)
                                        continue;

                                    l_Star->AI()->SetData(TYPE_ADD_STAR, m_StarId);

                                    // Cast beam on previous star
                                    if (l_PreviousStar)
                                    {
                                        if (l_Star->AI()->GetData(TYPE_GET_STAR_ID) == l_PreviousStar)
                                            me->CastSpell(l_Star, SPELL_STARDUST_BEAM, true);
                                    }

                                    // Activate next star
                                    if (l_NextStar)
                                    {
                                        if (l_Star->AI()->GetData(TYPE_GET_STAR_ID) == l_NextStar)
                                            l_Star->AI()->DoAction(ACTION_ACTIVATE_NEXT_STAR);
                                    }
                                }
                            }
                            else
                                m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                        }
                        else
                            m_Events.ScheduleEvent(EVENT_CHECK_PLAYER, 500);
                        break;
                    }
                    case EVENT_END_PATTERN:
                    {
                        std::list<Player*> l_PlayerList;
                        GetPlayerListInGrid(l_PlayerList, me, 200.0f);

                        // Resetting the player to the right phase
                        for (Player* l_Player : l_PlayerList)
                        {
                            if (l_Player->GetPhaseMask() == 2)
                            {
                                l_Player->SetPhaseMask(1, true);
                                for (uint8 l_Idx = 0; l_Idx < 4; ++l_Idx)
                                    l_Player->RemoveAura(g_PatternSpells[l_Idx]);
                            }
                        }

                        // Activating buff - Reminder : m_DrawToPerform == invoked celestial's entry
                        if (Creature* l_Celestial = Creature::GetCreature(*me, m_SummonerGuid))
                            l_Celestial->AI()->DoAction(ACTION_CELESTIAL_BUFF);

                        break;
                    }
                    default:
                        break;
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_astral_starAI(p_Creature);
        }
};

// 71441 - Image of Chi-Ji
class mob_image_of_chi_ji : public CreatureScript
{
    public:
        mob_image_of_chi_ji() : CreatureScript("mob_image_of_chi_ji") { }

        struct mob_image_of_chi_jiAI : public ScriptedAI
        {
            mob_image_of_chi_jiAI(Creature* p_Creature) : ScriptedAI(p_Creature)
            {
                m_Instance = p_Creature->GetInstanceScript();
            }

            EventMap m_Events;
            InstanceScript* m_Instance;
            bool m_HitLulin;
            bool m_HitSuen;

            void Reset()
            {
                m_Events.Reset();
                m_HitLulin = false;
                m_HitSuen  = false;

                me->SetReactState(REACT_PASSIVE);
                me->SetObjectScale(1.5f);

                m_Events.ScheduleEvent(EVENT_CRANE_MOVE, 100);
                m_Events.ScheduleEvent(EVENT_CRANE_CHECK_BOSS, 500);
            }

            void UpdateAI(uint32 const p_Diff)
            {
                if (m_HitLulin && m_HitSuen)
                    return;

                m_Events.Update(p_Diff);

                uint32 l_EventId = m_Events.ExecuteEvent();

                if (l_EventId == EVENT_CRANE_MOVE)
                {
                    float l_DestX = me->GetPositionX() + 300.0f * cos(me->GetOrientation());
                    float l_DestY = me->GetPositionY() + 300.0f * sin(me->GetOrientation());

                    me->GetMotionMaster()->MovePoint(0, l_DestX, l_DestY, me->GetPositionZ());
                }

                if (l_EventId == EVENT_CRANE_CHECK_BOSS)
                {
                    if (!m_HitLulin)
                    {
                        if (Creature* l_Lulin = GetClosestCreatureWithEntry(me, NPC_LU_LIN, MIN_MELEE_REACH, true))
                        {
                            me->CastSpell(l_Lulin, SPELL_CRANE_RUSH_DAMAGE, true);
                            m_HitLulin = true;
                        }
                    }

                    if (!m_HitSuen)
                    {
                        if (Creature* l_Suen = GetClosestCreatureWithEntry(me, NPC_SUEN, MIN_MELEE_REACH, true))
                        {
                            me->CastSpell(l_Suen, SPELL_CRANE_RUSH_DAMAGE, true);
                            m_HitSuen = true;
                        }
                    }

                    if (!m_HitLulin || !m_HitSuen)
                        m_Events.ScheduleEvent(EVENT_CRANE_CHECK_BOSS, 500);
                }
            }
        };

        CreatureAI* GetAI(Creature* p_Creature) const
        {
            return new mob_image_of_chi_jiAI(p_Creature);
        }
};

// 137118 - Bloom Moon Lotus
class spell_bloom_moon_lotus : public SpellScriptLoader
{
    public:
        spell_bloom_moon_lotus() : SpellScriptLoader("spell_bloom_moon_lotus") { }

        class spell_bloom_moon_lotus_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_bloom_moon_lotus_SpellScript);

            void ActivateLotus()
            {
                if (Creature* l_Lotus = GetHitCreature())
                    l_Lotus->AI()->DoAction(ACTION_LOTUS_ACTIVATE);
            }

            void Register()
            {
                OnHit += SpellHitFn(spell_bloom_moon_lotus_SpellScript::ActivateLotus);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_bloom_moon_lotus_SpellScript();
        }
};

// 137187 / 137105 - Dissipate
class spell_dissipate : public SpellScriptLoader
{
    public:
        spell_dissipate() : SpellScriptLoader("spell_dissipate") { }

        class spell_dissipate_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_dissipate_SpellScript);

            void Invisible()
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->AddAura(l_Caster->GetEntry() == NPC_LU_LIN ? SPELL_INVIS_LULIN : SPELL_INVIS_SUEN, l_Caster);
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_dissipate_SpellScript::Invisible);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_dissipate_SpellScript();
        }
};

// 136752 - Cosmic Barrage
class spell_cosmic_barrage : public SpellScriptLoader
{
    public:
        spell_cosmic_barrage() : SpellScriptLoader("spell_cosmic_barrage") { }

        class spell_cosmic_barrage_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_cosmic_barrage_AuraScript);

            void SummonStar(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_COSMIC_BARRAGE_SUMMON, true);
            }

            void LaunchStar(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    l_Caster->CastSpell(l_Caster, SPELL_LAUNCH_STAR, true);

                    std::list<Creature*> l_StarList;
                    GetCreatureListWithEntryInGrid(l_StarList, l_Caster, NPC_STAR, 10.0f);

                    std::list<Player*> l_PlayerList;
                    GetPlayerListInGrid(l_PlayerList, l_Caster, 100.0f);

                    for (Creature* l_Star : l_StarList)
                    {
                        std::list<Player*> l_StarTargetList = l_PlayerList;
                        JadeCore::RandomResizeList(l_StarTargetList, 1);
                        l_Star->AI()->SetGUID(l_StarTargetList.front()->GetGUID(), DATA_COSMIC_TARGET);
                        l_Star->AI()->DoAction(ACTION_COSMIC_STARS);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_cosmic_barrage_AuraScript::SummonStar, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove   += AuraEffectRemoveFn(spell_cosmic_barrage_AuraScript::LaunchStar, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_cosmic_barrage_AuraScript();
        }
};

// 137139 - Launch star
class spell_launch_star : public SpellScriptLoader
{
    public:
        spell_launch_star() : SpellScriptLoader("spell_launch_star") { }

        class spell_launch_star_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_launch_star_AuraScript);

            void ReturnToNormal(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Creature* l_Lulin = l_Caster->ToCreature())
                        l_Lulin->SetReactState(REACT_AGGRESSIVE);

                    std::list<Creature*> l_StarList;
                    GetCreatureListWithEntryInGrid(l_StarList, l_Caster, NPC_STAR, 10.0f);

                    for (Creature* l_Star : l_StarList)
                        l_Star->DespawnOrUnsummon();
                }
            }

            void Register()
            {
                OnEffectRemove += AuraEffectRemoveFn(spell_launch_star_AuraScript::ReturnToNormal, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_launch_star_AuraScript();
        }
};

// 137129 - Crashing star
class spell_crashing_star : public SpellScriptLoader
{
    public:
        spell_crashing_star() : SpellScriptLoader("spell_crashing_star") { }

        class spell_crashing_star_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_crashing_star_SpellScript);

            void Despawn()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() == NPC_STAR)
                        l_Caster->ToCreature()->DespawnOrUnsummon();
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_crashing_star_SpellScript::Despawn);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_crashing_star_SpellScript();
        }
};

// 137404 - Tears of the sun
class spell_tears_of_the_sun : public SpellScriptLoader
{
    public:
        spell_tears_of_the_sun() : SpellScriptLoader("spell_tears_of_the_sun") { }

        class spell_tears_of_the_sun_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_tears_of_the_sun_AuraScript);

            void PeriodicDamage(AuraEffect const* /*aurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                    l_Caster->CastSpell(l_Caster, SPELL_TEARS_OF_THE_SUN_PER, true);
            }

            void Invisible(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_mod*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() == NPC_SUEN)
                    {
                        l_Caster->AddAura(SPELL_INVIS_SUEN, l_Caster);
                        l_Caster->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE | UNIT_FLAG_NON_ATTACKABLE);
                    }
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_tears_of_the_sun_AuraScript::PeriodicDamage, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
                OnEffectRemove   += AuraEffectRemoveFn(spell_tears_of_the_sun_AuraScript::Invisible, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tears_of_the_sun_AuraScript();
        }
};

/// 137405 - Tears of the sun - Damaging players
class spell_tears_of_the_sun_dmg : public SpellScriptLoader
{
    public:
        spell_tears_of_the_sun_dmg() : SpellScriptLoader("spell_tears_of_the_sun_dmg") { }

        class spell_tears_of_the_sun_dmg_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_tears_of_the_sun_dmg_SpellScript);

            void PickTarget(std::list<WorldObject*> &p_Targets)
            {
                /// Removing non player targets
                p_Targets.remove_if([this](WorldObject* p_Obj) -> bool
                {
                    Player* l_Player = p_Obj->ToPlayer();
                    if (!l_Player)
                        return true;

                    return false;
                });

                /// Picking a random player
                if (p_Targets.size() > 1)
                    JadeCore::RandomResizeList(p_Targets, 1);
            }

            void Register()
            {
                OnObjectAreaTargetSelect += SpellObjectAreaTargetSelectFn(spell_tears_of_the_sun_dmg_SpellScript::PickTarget, EFFECT_0, TARGET_UNIT_SRC_AREA_ENEMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_tears_of_the_sun_dmg_SpellScript();
        }
};

// 138804 - Light of the day
class spell_suen_light_of_the_day : SpellScriptLoader
{
    public:
        spell_suen_light_of_the_day() : SpellScriptLoader("spell_suen_light_of_the_day") { }

        class spell_suen_light_of_the_day_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_suen_light_of_the_day_SpellScript);

            void Invisible()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() == NPC_SUEN)
                        l_Caster->AddAura(SPELL_INVIS_SUEN, l_Caster);
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_suen_light_of_the_day_SpellScript::Invisible);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_suen_light_of_the_day_SpellScript();
        }
};

// 137493 - Light of day (despawn near ice comets)
class spell_suen_light_of_day_ice : public SpellScriptLoader
{
    public:
        spell_suen_light_of_day_ice() : SpellScriptLoader("spell_suen_light_of_day_ice") { }

        class spell_suen_light_of_day_ice_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_suen_light_of_day_ice_SpellScript);

            void MeltComets()
            {
                Unit* l_Caster = GetCaster();
                Unit* l_Target = GetHitUnit();

                if (l_Caster && l_Target)
                {
                    std::list<Creature*> l_CometList;
                    GetCreatureListWithEntryInGrid(l_CometList, l_Target, NPC_ICE_COMET, 8.0f);

                    for (Creature* l_Comet : l_CometList)
                        l_Comet->DespawnOrUnsummon();
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_suen_light_of_day_ice_SpellScript::MeltComets);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_suen_light_of_day_ice_SpellScript();
        }
};

// 137411 - Blazing radiance
class spell_blazing_radiance : public SpellScriptLoader
{
    public:
        spell_blazing_radiance() : SpellScriptLoader("spell_blazing_radiance") { }

        class spell_blazing_radiance_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_blazing_radiance_AuraScript);

            void PeriodicStack(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (Creature* l_Comet = GetClosestCreatureWithEntry(l_Caster, NPC_ICE_COMET, 8.0f))
                    {
                        if (Aura* l_Blazing = l_Caster->GetAura(SPELL_BLAZING_RADIANCE_ST))
                        {
                            if (l_Blazing->GetStackAmount())
                                l_Blazing->SetStackAmount(l_Blazing->GetStackAmount() - 1);
                            else
                                l_Blazing->Remove();
                        }
                    }
                    else
                        l_Caster->CastSpell(l_Caster, SPELL_BLAZING_RADIANCE_ST, false);
                    l_Caster->CastSpell(l_Caster, SPELL_BLAZING_RADIANCE_DMG, false);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_blazing_radiance_AuraScript::PeriodicStack, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_blazing_radiance_AuraScript();
        }
};

// 138798 - 137495 - Ice Comet
class spell_ice_comet : public SpellScriptLoader
{
    public:
        spell_ice_comet() : SpellScriptLoader("spell_ice_comet") { }

        class spell_ice_comet_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_ice_comet_SpellScript);

            void Comet()
            {
                if (Unit* l_Caster = GetCaster())
                {
                    std::list<Creature*> l_CometList;
                    GetCreatureListWithEntryInGrid(l_CometList, l_Caster, NPC_ICE_COMET, 200.0f);

                    for (Creature* l_Comet : l_CometList)
                    {
                        if (!l_Comet->HasAura(SPELL_ICE_COMET_DOWN))
                        {
                            l_Comet->AI()->DoAction(ACTION_COMET_IN_PLACE);
                            return;
                        }
                    }
                }
            }

            void Register()
            {
                AfterCast += SpellCastFn(spell_ice_comet_SpellScript::Comet);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_ice_comet_SpellScript();
        }
};

// 137531 - Tidal Force
class spell_tidal_force : public SpellScriptLoader
{
    public:
        spell_tidal_force() : SpellScriptLoader("spell_tidal_force") { }

        class spell_tidal_force_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_tidal_force_AuraScript);

            void Waves(AuraEffect const* /*p_AurEff*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    float l_IniOri = l_Caster->GetOrientation();
                    l_Caster->SetOrientation(frand(0.0f, M_PI * 2));
                    l_Caster->CastSpell(l_Caster, SPELL_TIDAL_FORCE_WAVE, true);
                    l_Caster->CastSpell(l_Caster, SPELL_TIDAL_FORCE_DAMAGE, true);
                    l_Caster->SetOrientation(l_IniOri);
                }
            }

            void TidalEnd(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mod*/)
            {
                if (Unit* l_Caster = GetCaster())
                {
                    if (l_Caster->GetEntry() == NPC_LU_LIN)
                        l_Caster->ToCreature()->SetReactState(REACT_AGGRESSIVE);
                }
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_tidal_force_AuraScript::Waves, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL);
                OnEffectRemove   += AuraEffectRemoveFn(spell_tidal_force_AuraScript::TidalEnd, EFFECT_0, SPELL_AURA_PERIODIC_TRIGGER_SPELL, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_tidal_force_AuraScript();
        }
};

// 138189 - Invoke Crane Spirit
// 138254 - Invoke Ox Spirit
// 138264 - Invoke Tiger Spirit
// 138267 - Invoke Serpent Spirit
class spell_invoke_celestial_spirit : public SpellScriptLoader
{
    public:
        spell_invoke_celestial_spirit() : SpellScriptLoader("spell_invoke_celestial_spirit") { }

        class spell_invoke_celestial_spirit_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_invoke_celestial_spirit_AuraScript);

            void ResetPhase(AuraEffect const* /*p_AurEff*/, AuraEffectHandleModes /*p_Mode*/)
            {
                if (Unit* l_Player = GetUnitOwner())
                    l_Player->SetPhaseMask(1, true);
            }

            void Register()
            {
                OnEffectRemove   += AuraEffectRemoveFn(spell_invoke_celestial_spirit_AuraScript::ResetPhase, EFFECT_0, SPELL_AURA_SCREEN_EFFECT, AURA_EFFECT_HANDLE_REAL);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_invoke_celestial_spirit_AuraScript();
        }
};

/// Created by spell 137416
class at_flames_of_passion : public AreaTriggerEntityScript
{
    public:
        at_flames_of_passion() : AreaTriggerEntityScript("at_flames_of_passion") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            std::list<Player*> l_TargetList;
            GetPlayerListInGrid(l_TargetList, p_AreaTrigger, 10.0f);

            for (Player* l_Player : l_TargetList)
            {
                /// Applying damage aura on players in range
                if (l_Player->GetDistance2d(p_AreaTrigger) < .75f)
                {
                    if (!l_Player->HasAura(SPELL_FLAMES_OF_PASSION_DMG))
                        l_Player->AddAura(SPELL_FLAMES_OF_PASSION_DMG, l_Player);
                }
                /// Removing aura on players outside the range
                else
                {
                    if (l_Player->HasAura(SPELL_FLAMES_OF_PASSION_DMG))
                        l_Player->RemoveAura(SPELL_FLAMES_OF_PASSION_DMG);
                }
            }
        }

        void OnRemove(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            Map::PlayerList const& l_PlayerList = l_Caster->GetMap()->GetPlayers();
            for (Map::PlayerList::const_iterator l_Itr = l_PlayerList.begin(); l_Itr != l_PlayerList.end(); ++l_Itr)
            {
                if (Player* l_Player = l_Itr->getSource())
                    l_Player->RemoveAura(SPELL_FLAMES_OF_PASSION_DMG);
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_flames_of_passion();
        }
};

/// Created by spell 137439
class at_icy_shadows : public AreaTriggerEntityScript
{
    public:
        at_icy_shadows() : AreaTriggerEntityScript("at_icy_shadows") { }

        void OnUpdate(AreaTrigger* p_AreaTrigger, uint32 /*p_Time*/)
        {
            Unit* l_Caster = p_AreaTrigger->GetCaster();
            if (!l_Caster)
                return;

            std::list<Unit*> l_TargetList;
            float l_Radius = 10.0f;

            JadeCore::NearestAttackableUnitInObjectRangeCheck l_Check(p_AreaTrigger, l_Caster, l_Radius);
            JadeCore::UnitListSearcher<JadeCore::NearestAttackableUnitInObjectRangeCheck> l_Searcher(p_AreaTrigger, l_TargetList, l_Check);
            p_AreaTrigger->VisitNearbyObject(l_Radius, l_Searcher);

            if (!l_TargetList.empty())
            {
                for (Unit* l_Unit : l_TargetList)
                {
                    if (l_Caster->isInBack(l_Unit, M_PI / 4) && l_Caster->GetDistance2d(l_Unit) < 6.0f)
                    {
                        if (!l_Unit->HasAura(SPELL_ICY_SHADOWS_AURA))
                            l_Caster->AddAura(SPELL_ICY_SHADOWS_AURA, l_Unit);
                    }
                    else
                    {
                        if (l_Unit->HasAura(SPELL_ICY_SHADOWS_AURA))
                            l_Unit->RemoveAura(SPELL_ICY_SHADOWS_AURA);
                    }
                }
            }
        }

        AreaTriggerEntityScript* GetAI() const
        {
            return new at_icy_shadows();
        }
};

#ifndef __clang_analyzer__
void AddSC_boss_twin_consorts()
{
    new boss_suen();                        ///< 68904
    new boss_lulin();                       ///< 68905
    new mob_moon_lotus();                   ///< 69456
    new mob_moon_lotus_spore();             ///< 69340
    new mob_celestial();                    ///< 69771, 69774, 69777, 69779
    new mob_beast_of_nightmares();          ///< 69479
    new mob_ice_comet();                    ///< 69596
    new mob_astral_star();                  ///< 64677
    new mob_image_of_chi_ji();              ///< 71441
    new spell_bloom_moon_lotus();           ///< 137118
    new spell_dissipate();                  ///< 137187 - 137105
    new spell_cosmic_barrage();             ///< 136752
    new spell_launch_star();                ///< 137139
    new spell_crashing_star();              ///< 137129
    new spell_tears_of_the_sun();           ///< 137404
    new spell_tears_of_the_sun_dmg();       ///< 137405
    new spell_suen_light_of_the_day();      ///< 138804
    new spell_suen_light_of_day_ice();      ///< 137493
    new spell_blazing_radiance();           ///< 137411
    new spell_ice_comet();                  ///< 138798 - 137495
    new spell_tidal_force();                ///< 137531
    new spell_invoke_celestial_spirit();    ///< 138189 - 138254 - 138264 - 138267
    new at_flames_of_passion();             ///< 137416
    new at_icy_shadows();                   ///< 137439
}
#endif
