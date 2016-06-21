////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

# include "blackrock_foundry.hpp"

enum eIronMaidensCreatures
{
    AquaticTechnician   = 85711,
    IronDockworker      = 84841,
    IronEarthbinder     = 84860,
    IronMauler          = 85748,
    IronCleaver         = 84859,
    LoadingChain        = 78767,
    Ukurogg             = 78341
};

enum eIronMaidensActions
{
    ActionIntro,
    ActionAfterTrashesIntro
};

enum eIronMaidensDatas
{
    MaxRandomEmotes     = 3,
    MaxLoadingChains    = 6,
    MaxDockworkerPos    = 2
};

static void RespawnMaidens(InstanceScript* p_Instance, Creature* p_Source)
{
    if (p_Instance == nullptr || p_Source == nullptr)
        return;

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = Creature::GetCreature(*p_Source, p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            l_Maiden->Respawn();
            l_Maiden->GetMotionMaster()->MoveTargetedHome();
        }
    }
}

static bool StartMaidens(InstanceScript* p_Instance, Creature* p_Source, Unit* p_Target)
{
    if (p_Instance == nullptr || p_Source == nullptr || p_Target == nullptr)
        return false;

    if (p_Instance->GetBossState(eFoundryDatas::DataIronMaidens) == EncounterState::IN_PROGRESS)
        return false; ///< Prevent recursive calls

    p_Instance->SetBossState(eFoundryDatas::DataIronMaidens, EncounterState::IN_PROGRESS);

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = Creature::GetCreature(*p_Source, p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_ENGAGE, l_Maiden);

            if (l_Maiden->IsAIEnabled)
                l_Maiden->AI()->AttackStart(p_Target);

            l_Maiden->SetInCombatWith(p_Target);
        }
    }

    return true;
}

static void WipeMaidens(InstanceScript* p_Instance)
{
    if (p_Instance == nullptr)
        return;

    if (p_Instance->GetBossState(eFoundryDatas::DataIronMaidens) != EncounterState::IN_PROGRESS)
        return; ///< Prevent recursive calls

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = p_Instance->instance->GetCreature(p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            if (l_Maiden->IsAIEnabled)
                l_Maiden->AI()->EnterEvadeMode();

            p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, l_Maiden);
        }
    }

    p_Instance->SetBossState(eFoundryDatas::DataIronMaidens, EncounterState::FAIL);
}

Position const g_MarakHomePos = { 441.868f, 3136.77f, 135.302f, 1.67754f };
Position const g_GaranHomePos = { 434.845f, 3141.18f, 135.302f, 1.39335f };
Position const g_SorkaHomePos = { 426.464f, 3137.16f, 135.302f, 1.75377f };

Position const g_CosmeticCleaverPos = { 446.648f, 3170.30f, 135.302f, 1.66107f };

static std::array<Emote, eIronMaidensDatas::MaxRandomEmotes> g_DockworkerEmotes =
{
    Emote::EMOTE_ONESHOT_CHEER_NOSHEATHE,
    Emote::EMOTE_ONESHOT_SALUTE_NOSHEATH,
    Emote::EMOTE_ONESHOT_ROAR
};

/// From west to east
static std::array<Position const, eIronMaidensDatas::MaxLoadingChains> g_LoadingChainsSpawnPos =
{
    {
        { 405.786f, 3194.20f, 145.749f, 1.5708f },
        { 410.354f, 3194.36f, 145.749f, 1.5708f },
        { 415.681f, 3194.32f, 145.749f, 1.5708f },
        { 421.218f, 3194.32f, 145.749f, 1.5708f },
        { 426.156f, 3194.77f, 145.749f, 1.5708f },
        { 432.026f, 3194.75f, 145.749f, 1.5708f }
    }
};

/// From west to east
static std::array<Position const, eIronMaidensDatas::MaxLoadingChains> g_LoadingChainsMoveBoatPos =
{
    {
        { 405.8764f, 3264.216f, 149.6323f, 1.5708f },
        { 410.4441f, 3264.378f, 149.6323f, 1.5708f },
        { 415.6806f, 3194.325f, 145.7494f, 1.5708f },
        { 421.3713f, 3264.340f, 149.6323f, 1.5708f },
        { 426.2462f, 3264.793f, 149.6323f, 1.5708f },
        { 432.1160f, 3264.775f, 149.6323f, 1.5708f }
    }
};

static std::array<Position const, eIronMaidensDatas::MaxDockworkerPos> g_IronDockworkerCarryCratePos =
{
    {
        { 366.79f, 3179.286f, 133.625f, 3.1741f },
        { 389.50f, 3138.120f, 134.123f, 4.7841f }
    }
};
