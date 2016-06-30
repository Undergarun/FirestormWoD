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
    Ukurogg             = 78341,
    RapidFireStalker    = 77636,
    Uktar               = 78351,
    BattleMedicRogg     = 78352,
    Gorak               = 78343,
    IronEviscerator     = 78347
};

enum eIronMaidensActions
{
    ActionIntro,
    ActionAfterTrashesIntro,
    ActionEnteredZipline,
    ActionZiplineArrived,
    ActionJumpToShip,
    ActionSabotageShip
};

enum eIronMaidensDatas
{
    /// Counters
    MaxRandomEmotes         = 3,
    MaxLoadingChains        = 6,
    MaxDockworkerPos        = 2,
    MaxIronCleaverMoves     = 4,
    MaxIntroMovesCount      = 4,
    MaxHeartseekerTargets   = 3,
    MaxHealthForIronWill    = 20,
    MaxBoatBossFlyingMoves  = 4,
    MaxZiplineFlyingMoves   = 14,
    /// Misc Getters
    LoadingChainID          = 0,
    LoadingChainAvailable   = 1,
    /// Boss Getters
    IsAvailableForShip      = 0,
    /// Event schedulers
    FirstIronFuryAbility    = 30,
    SecondIronFuryAbility   = 100
};

enum eIronMaidensSpells
{
    ZeroPowerZeroRegen  = 118357,
    IronWill            = 159337,
    PermanentFeignDeath = 70628,
    WarmingUpAura       = 158849,
    OnABoatPeriodic     = 158726
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

    p_Instance->SetBossState(eFoundryDatas::DataIronMaidens, EncounterState::FAIL);

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = p_Instance->instance->GetCreature(p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            if (l_Maiden->IsAIEnabled)
                l_Maiden->AI()->EnterEvadeMode();

            p_Instance->SendEncounterUnit(EncounterFrameType::ENCOUNTER_FRAME_DISENGAGE, l_Maiden);
        }
    }
}

static void TriggerIronWill(InstanceScript* p_Instance)
{
    if (p_Instance == nullptr)
        return;

    for (uint8 l_I = 0; l_I < 3; ++l_I)
    {
        if (Creature* l_Maiden = p_Instance->instance->GetCreature(p_Instance->GetData64(g_IronMaidensEntries[l_I])))
        {
            if (l_Maiden->HasAura(eIronMaidensSpells::IronWill))
                continue;

            l_Maiden->CastSpell(l_Maiden, eIronMaidensSpells::IronWill, true);
        }
    }
}

static std::map<uint32, std::vector<Position>> g_ShipSpawnPos =
{
    {
        eIronMaidensCreatures::Uktar,
        {
            { 446.5365f, 3268.295f, 141.4713f, 6.040603f }
        }
    },
    {
        eIronMaidensCreatures::BattleMedicRogg,
        {
            { 458.5816f, 3267.925f, 141.4716f, 3.357880f }
        }
    },
    {
        eIronMaidensCreatures::Ukurogg,
        {
            { 441.8681f, 3136.766f, 135.3021f, 1.677542f }
        }
    },
    {
        eIronMaidensCreatures::Gorak,
        {
            { 419.5139f, 3267.728f, 141.4713f, 0.963534f }
        }
    },
    {
        eIronMaidensCreatures::IronEviscerator,
        {
            { 420.4531f, 3273.922f, 141.4713f, 4.597013f },
            { 425.0642f, 3267.917f, 141.4713f, 2.954990f }
        }
    }
};

Position const g_MarakHomePos = { 442.363f, 3136.75f, 135.219f, 1.710423f };
Position const g_GaranHomePos = { 434.845f, 3141.18f, 135.219f, 1.640610f };
Position const g_SorkaHomePos = { 425.965f, 3138.43f, 135.219f, 1.753770f };

/// For Gar'an and Marak
Position const g_BoatBossFirstJumpPos = { 506.106f, 3250.79f, 170.375f, 0.0f };

Position const g_ShipExplosionScenePos = { 494.2526f, 3282.585f, 141.3935f, 0.06420326f };
Position const g_AfterShipPlayerJumpPos = { 433.3941f, 3176.948f, 135.2187f, 4.199047f };

static std::array<G3D::Vector3, eIronMaidensDatas::MaxBoatBossFlyingMoves> g_BoatBossFlyingMoves =
{
    {
        { 504.8629f, 3248.760f, 172.3238f },
        { 495.1823f, 3234.664f, 172.8863f },
        { 472.6910f, 3202.702f, 175.0203f },
        { 463.2691f, 3186.117f, 176.3029f }
    }
};

static std::array<G3D::Vector3, eIronMaidensDatas::MaxZiplineFlyingMoves> g_ZiplineFlyingMoves =
{
    {
        { 463.9948f, 3190.459f, 180.3531f },
        { 468.0139f, 3195.972f, 179.9773f },
        { 471.1076f, 3200.292f, 179.7583f },
        { 475.1285f, 3206.103f, 179.2168f },
        { 478.3837f, 3210.953f, 179.0443f },
        { 482.4601f, 3216.684f, 178.6539f },
        { 485.8767f, 3221.579f, 178.3048f },
        { 489.3160f, 3226.421f, 177.9685f },
        { 492.8403f, 3231.653f, 177.6089f },
        { 498.5035f, 3239.773f, 177.0412f },
        { 503.0677f, 3246.305f, 176.6152f },
        { 507.5764f, 3252.608f, 176.2642f },
        { 515.6563f, 3264.310f, 175.4255f },
        { 520.7656f, 3271.337f, 174.8700f }
    }
};

static std::array<G3D::Vector3, eIronMaidensDatas::MaxZiplineFlyingMoves> g_ReverseZiplineFlyingMoves =
{
    {
        { 520.7656f, 3271.337f, 174.8700f },
        { 515.6563f, 3264.310f, 175.4255f },
        { 507.5764f, 3252.608f, 176.2642f },
        { 503.0677f, 3246.305f, 176.6152f },
        { 498.5035f, 3239.773f, 177.0412f },
        { 492.8403f, 3231.653f, 177.6089f },
        { 489.3160f, 3226.421f, 177.9685f },
        { 485.8767f, 3221.579f, 178.3048f },
        { 482.4601f, 3216.684f, 178.6539f },
        { 478.3837f, 3210.953f, 179.0443f },
        { 475.1285f, 3206.103f, 179.2168f },
        { 471.1076f, 3200.292f, 179.7583f },
        { 468.0139f, 3195.972f, 179.9773f },
        { 463.9948f, 3190.459f, 180.3531f }
    }
};

Position const g_ZiplineBossPos = { 461.8629f, 3187.325f, 180.6749f, 0.0f };
Position const g_ZiplineBoatPos = { 520.7656f, 3271.337f, 174.8700f, 0.0f };

Position const g_EnterZiplinePos = { 451.2829f, 3175.634f, 135.4688f, 0.0f };
Position const g_ExitZiplinePos = { 520.7656f, 3271.337f, 174.87f, 1.605687f };

Position const g_RoomCenterPos = { 434.86f, 3165.41f, 135.22f, 0.0f };

float const g_MarakFinalFacing = 1.710423f;
float const g_GaranFinalFacing = 1.64061f;

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
        { 405.786f, 3194.32f, 145.749f, 1.5708f },
        { 410.354f, 3194.32f, 145.749f, 1.5708f },
        { 415.681f, 3194.32f, 145.749f, 1.5708f },
        { 421.218f, 3194.32f, 145.749f, 1.5708f },
        { 426.156f, 3194.32f, 145.749f, 1.5708f },
        { 432.026f, 3194.32f, 145.749f, 1.5708f }
    }
};

/// From west to east
static std::array<Position const, eIronMaidensDatas::MaxLoadingChains> g_LoadingChainsMoveBoatPos =
{
    {
        { 405.8764f, 3264.378f, 149.6323f, 1.5708f },
        { 410.4441f, 3264.378f, 149.6323f, 1.5708f },
        { 415.6806f, 3264.378f, 149.6323f, 1.5708f },
        { 421.3713f, 3264.378f, 149.6323f, 1.5708f },
        { 426.2462f, 3264.378f, 149.6323f, 1.5708f },
        { 432.1160f, 3264.378f, 149.6323f, 1.5708f }
    }
};

static std::array<Position const, eIronMaidensDatas::MaxDockworkerPos> g_IronDockworkerCarryCratePos =
{
    {
        { 367.4045f, 3174.203f, 133.7108f, 3.1741f },
        { 390.8281f, 3142.027f, 134.2045f, 4.7841f }
    }
};

static std::array<Position const, eIronMaidensDatas::MaxIronCleaverMoves> g_IronCleaverMovesPos =
{
    {
        { 413.35f, 3143.22f, 135.22f, 4.72f },
        { 457.10f, 3142.93f, 135.22f, 6.28f },
        { 457.23f, 3187.47f, 135.22f, 1.56f },
        { 413.77f, 3187.07f, 135.22f, 3.16f }
    }
};
