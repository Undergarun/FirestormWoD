////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
{
    std::list<Creature*> creatures;
    GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
    if (creatures.empty())
        return;

    for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

Position const g_PositionTuulaniPreMovement01st          = { 1538.54f, 2952.99f, 35.239f, 0.041050f };
Position const g_PositionTuulaniPreMovement02nd          = { 1591.58f, 2953.37f, 35.239f, 0.021415f };
Position const g_PositionTuulaniPreMovement03rd          = { 1646.196f, 2956.79f, 35.060f, 0.975673f };
Position const g_PositionTuulaniMovement2nd              = { 1644.27f, 2962.42f, 35.185f, 1.269892f };
Position const g_PositionGuardPositionTalker             = { 1645.61f, 2967.25f, 35.167f, 0.024081f };
Position const g_PositionTuulaniMovement3rd              = { 1660.10f, 2999.02f, 34.763f, 1.168017f };
Position const g_PositionTuulaniMovement4th              = { 1685.99f, 3065.55f, 35.139f, 1.204807f };
Position const g_PositionTuulaniMovement5th              = { 1722.35f, 3121.52f, 35.139f, 0.914209f };
Position const g_PositionTuulaniMovement6th              = { 1756.30f, 3154.63f, 35.139f, 0.772838f };
Position const g_PositionTuulaniMovement7th              = { 1806.26f, 3178.98f, 35.127f, 0.454751f };
Position const g_PositionTuulaniMovement8th              = { 1835.91f, 3183.27f, 29.423f, 0.129596f };
Position const g_PositionTuulaniMovement9th              = { 1854.28f, 3199.26f, 29.424f, 5.918766f };
Position const g_PositionTuulaniMovement10th             = { 1867.89f, 3197.28f, 30.970f, 6.138678f };
Position const g_PositionDefenderWhoReadsNearTwoHopilite = { 1691.51f, 3070.65f, 35.223f, 5.132760f };
Position const g_PositionDefenderWhoTalksToSoldiers      = { 1709.31f, 3099.06f, 35.139f, 5.945590f };
Position const g_PositionSoulPriestWhoMeditates          = { 1701.68f, 3113.02f, 35.223f, 0.0f };
Position const g_PositionMagusWhoCastArcane              = { 1710.42f, 3113.87f, 35.223f, 3.386260f };
Position const g_PositionMagusWhoTalksToDefender         = { 1728.73f, 3139.19f, 35.223f, 2.261400f };
Position const g_PositionSoulPriestTalksToTwoDefender    = { 1745.93f, 3138.90f, 35.223f, 0.293292f };
Position const g_PositionKaatharCrystaconst              = { 1909.75f, 3188.70f, 66.786f, 5.401960f };
Position const g_PositionGuardTalker                     = { 1645.61f, 2967.25f, 35.167f, 0.024081f };
Position const g_PositionAuchenaiReader1st               = { 1673.96f, 3074.10f, 36.132f, 5.834950f };
Position const g_PositionWarden1st                       = { 1510.29f, 2954.78f, 35.238f, 0.091797f };
Position const g_PositionKaatharCrystalPosition          = { 1909.75f, 3188.70f, 66.786f, 5.401960f };

// Floating Triggers
Position const g_PositionLightFloatingTrigger[3] =
{
    { 1664.70f, 3011.64f, 41.503f, 4.432930f },
    { 1757.79f, 2952.85f, 44.559f, 6.247188f },
    { 1665.26f, 2893.94f, 41.317f, 5.010184f }
};
// Combat guards
Position const g_PositionGuards[6] =
{
    { 1497.06f, 2948.16f, 35.239f, 2.452730f },
    { 1495.23f, 2945.86f, 35.696f, 2.554010f },
    { 1491.63f, 2944.18f, 36.147f, 2.033919f },
    { 1495.93f, 2956.85f, 35.239f, 3.644247f },
    { 1493.42f, 2958.79f, 35.239f, 3.926992f },
    { 1490.35f, 2959.78f, 35.614f, 3.875940f }
};
// Clapping guards
Position const g_PositionGuards2nd[2] =
{
    { 1506.64f, 2958.66f, 35.238f, 4.793027f },
    { 1504.08f, 2958.45f, 35.238f, 4.793027f }
};
// Warden Patrol, and Warden position to be spawned at
Position const g_PositionWardenPatrol[2] =
{
    { 1609.98f, 2955.21f, 35.238f },
    { 1508.82f, 2954.96f, 35.238f }
};
// Middle
Position const g_PositionCircularMages[3] =
{
    { 1662.40f, 2939.76f, 35.143f, 1.624580f },
    { 1668.03f, 2941.39f, 35.143f, 2.242950f },
    { 1657.34f, 2965.32f, 35.061f, 5.090852f }
};
Position const g_PositionCircularPriests[2] =
{
    { 1648.36f, 2950.41f, 35.06f, 0.288927f },
    { 1650.16f, 2947.54f, 34.822f, 0.397321f }
};
Position const g_PositionCircularHolies[2] =
{
    { 1653.17f, 2962.95f, 35.061f, 5.514967f },
    { 1650.16f, 2947.54f, 34.822f, 0.397321f }
};
// Left Hallway
Position const g_PositionWardenPatrol2nd[2] =
{
    { 1672.66f, 3022.84f, 35.140f },
    { 1673.99f, 3033.03f, 35.236f }
};
Position const g_PositionAuchenaiMagus2nd[2] =
{
    { 1675.69f, 3068.34f, 35.223f, 2.696660f },
    { 1679.35f, 3075.59f, 35.223f, 2.604600f }
};
Position const g_PositionAuchenaiVigilant[4] =
{
    { 1667.18f, 3077.60f, 36.679f, 5.796140f },
    { 1692.63f, 3118.17f, 36.679f, 5.632550f },
    { 1724.72f, 3153.71f, 36.679f, 5.423370f },
    { 1762.43f, 3183.01f, 36.679f, 5.268510f }
};
Position const g_PositionHopilliteGuardState[2] =
{
    { 1691.04f, 3062.77f, 35.223f, 2.610810f },
    { 1693.10f, 3066.61f, 35.140f, 2.546201f }
};
Position const g_PositionNep_WhomePosition = { 1908.63f, 3163.94f, 30.799f, 1.498386f };
Position const g_PositionKaatharCombatJump = { 1911.47f, 3152.13f, 30.972f, 1.550403f };
Position const g_PositionMageSpawning = { 1857.07f, 3167.93f, 29.422f, 0.275066f };
Position const g_PositionTuulaniGobjectPortalSpawn = { 1912.11f, 3183.67f, 30.799f, 3.173182f };
Position const g_PositionNyamiSpawn = { 1660.17f, 2953.36f, 34.291f, 1.413091f };
Position const g_PositionWardenAndGuards = { 1666.69f, 3015.78f, 35.145f, 1.302555f };
Position const g_PositionMagusp_WhoControlFootmans = { 1708.01f, 3107.09f, 35.139f, 6.217793f };
Position const g_PositionWallInMiddleFromNyami = { 1689.59f, 2953.11f, 35.613f, 3.139696f };
Position const g_PositionWallInBackFromNyami = { 1628.76f, 2952.63f, 35.613f, 6.231408f };
Position const g_PositionNyamiEscape05 = { 1801.23f, 3178.36f, 35.139f };
Position const g_PositionNyamiEscape04 = { 1912.99f, 3165.06f, 48.221f };
Position const g_PositionNyamiEscape03 = { 1891.66f, 3184.09f, 49.976f };
Position const g_PositionNyamiEscape02 = { 1911.14f, 3211.44f, 51.915f };
Position const g_PositionNyamiEscape01 = { 1935.88f, 3185.43f, 44.528f };
Position const g_PositionMageMoveTo = { 1895.09f, 3183.77f, 30.799f };

Position const g_PositionThreeCorpses[2] =
{
    { 1749.40f, 3145.85f, 35.140f, 3.690412f },
    { 1751.31f, 3141.03f, 35.140f, 3.278078f },
};
Position const g_PositionMagusAndDefenderHostile[2] =
{
    { 1735.47f, 3139.28f, 35.140f, 0.185946f },
    { 1738.86f, 3145.71f, 35.140f, 5.495237f },
};
Position const g_PositionSargereiDefenders[2] =
{
    { 1713.76f, 3104.75f, 35.140f, 0.902229f },
    { 1710.27f, 3108.19f, 35.140f, 0.815836f },
};
Position const g_PositionThreePrisoners[3] =
{
    { 1700.59f, 3097.08f, 40.908f, 0.701154f },
    { 1707.98f, 3103.10f, 42.982f, 1.499199f },
    { 1714.57f, 3111.26f, 43.196f, 3.066774f },
};
Position const g_PositionThreeHostileArbitrerMagusSoulPriest[3] =
{
    { 1679.36f, 3072.41f, 35.139f, 0.420260f },
    { 1691.86f, 3065.93f, 35.139f, 1.459343f },
    { 1688.90f, 3070.54f, 35.139f, 1.441278f },
};
Position const g_PositionCorpsesNearNyomi[12] =
{
    { 1673.98f, 2971.02f, 35.151f, 4.399558f },
    { 1671.73f, 2971.23f, 35.200f, 4.517365f },
    { 1674.24f, 2967.03f, 35.194f, 4.327302f },
    { 1648.88f, 2967.87f, 35.211f, 4.877079f },
    { 1651.50f, 2968.30f, 35.219f, 4.888861f },
    { 1649.75f, 2937.92f, 35.215f, 1.334930f },
    { 1644.28f, 2937.78f, 35.173f, 1.460592f },
    { 1672.23f, 2938.41f, 35.208f, 1.825676f },
    { 1674.24f, 2933.18f, 35.131f, 1.916782f },
    { 1669.27f, 2931.85f, 34.764f, 1.751064f },
};
Position const g_PositionFourMagesThatSpawnAfterKaatharIsKaaput[5] =
{
    { 1846.22f, 3198.23f, 29.424f, 6.066411f },
    { 1849.56f, 3203.73f, 29.424f, 6.023211f },
    { 1844.11f, 3170.28f, 29.410f, 6.176361f },
    { 1848.42f, 3162.75f, 29.423f, 0.003129f },
};
Position const g_PositionAfterKaatharDeathForFourMages[4] =
{
    { 1879.55f, 3201.58f, 34.418f },
    { 1876.59f, 3191.82f, 31.418f },
    { 1876.75f, 3171.40f, 31.418f },
    { 1877.25f, 3165.11f, 31.418f },
};
Position const g_PositionGuardsAndWardens[2] =
{
    { 1672.54f, 3012.73f, 35.139f, 1.318899f },
    { 1659.39f, 3016.55f, 35.165f, 1.212869f },
};
Position const g_PositionDefenderBehindMiddleWallOfNyami[4] =
{
    { 1696.44f, 2957.14f, 35.613f, 3.265362f },
    { 2952.62f, 2952.80f, 35.613f, 3.120063f },
    { 1695.18f, 2947.33f, 35.613f, 3.040738f },
    { 1692.70f, 2953.55f, 35.613f, 3.140484f },
};
Position const g_PositionDefenderBehindBackWallOfNyami[2] =
{
    { 1621.08f, 2949.12f, 35.193f, 0.044038f },
    { 1621.33f, 2958.54f, 35.612f, 6.217269f },
};

Position const g_PositionFirstPlatformFirstMove = { 1910.91f, 2810.94f, 119.795f }; // DURAG
Position const g_PositionFirstPlatormSecondMove = { 1959.16f, 2907.14f, 41.030f };
Position const g_PositionFirstPlatformThirdMove = { 2003.36f, 2863.18f, 35.212f };
Position const g_PositionSecondPlatformFirstMove = { 1981.34f, 2877.99f, 53.373f }; // GULKOSH
Position const g_PositionSecondPlatformSecondMove = { 1959.05f, 2999.68f, 75.242f };
Position const g_PositionSecondPlatformThirdMove = { 2003.70f, 3043.89f, 35.212f };
Position const g_PositionThirdPlatformFirstMove = { 2003.70f, 3043.89f, 56.912f }; // GROMTASH
Position const g_PositionThirdPlatformsSecondMove = { 1914.66f, 2991.45f, 77.863f };
Position const g_PositionThirdPlatformThirdMove = { 1817.49f, 3046.65f, 35.212f };
Position const g_PositionFourthMovement = { 1846.60f, 2950.43f, 15.170f };
Position const g_PositionPlatform_3 = { 1838.42f, 3027.05f, 35.283f, 2.149401f }; // CREATURE_GROMTASH_THE_DESTRUCTOR
Position const g_PositionDrain = { 1960.20f, 3000.69f, 16.236f, 0.847815f };
Position const g_PositionTeronogorStartingPoint[1] =
{
    { 1923.63f, 2974.06f, 16.844f, 1.104046f }
};
Position const g_PositionPlatform_01[2] =
{
    { 1985.33f, 2876.17f, 35.282f, 5.431586f }, // CREATURE_ZIPTEQ 
    { 1987.57f, 2880.12f, 35.282f, 5.169260f }, //CREATURE_DURAG_THE_DOMINATOR
};
Position const g_PositionPlatform_02[2] =
{
    { 1983.61f, 3030.35f, 35.280f, 0.613946f }, // CREATURE_SHAADUM  
    { 1987.36f, 3023.54f, 35.282f, 0.747463f }, //CREATURE_GULKOSH
};
Position const g_Positionsoul_move[4] =
{
    { 2010.29f, 2855.26f, 35.213f, 2.397908f },
    { 2011.11f, 3051.70f, 35.212f, 4.003247f },
    { 1812.13f, 3052.10f, 35.212f, 5.536366f },
    { 1812.37f, 2854.15f, 35.212f, 0.796488f },
};
Position const g_Positiondead_souls[3] =
{
    { 1922.36f, 2988.91f, 27.743f, 4.405081f },
    { 1946.59f, 2973.00f, 30.193f, 3.678247f },
    { 1963.11f, 3003.42f, 22.359f, 3.904775f },
};

Position const g_PositionSafeZoneVessel = { 1658.81f, 2957.50f, 34.291f, 5.099889f };
Position const g_PositionBubble = { 1660.65f, 2953.06f, 34.291f, 1.542292f };
Position const g_PositionWardenSpawnPoint = { 1660.96f, 2918.35f, 48.036f, 1.672848f };
Position const g_PositionWardenPosition1st = { 1686.05f, 2840.95f, 35.140f };
Position const g_PositionDeadGuards[4] =
{
    { 1645.85f, 2937.17f, 35.166f, 1.454027f },
    { 1645.02f, 2971.30f, 35.140f, 6.229243f },
    { 1675.58f, 2972.18f, 35.139f, 3.197360f },
    { 1674.51f, 2932.11f, 35.119f, 3.219360f }
};

enum eAuchindonInformation
{
    InformationAuchindonMapId  = 1182,
    InformationFriendlyFaction = 35,
    InformationHostileFaction  = 16,
    InformationDisplayIdInvis  = 11686
};

enum eDataAuchindonDatas
{
    DataBossKathaar = 1,
    DataBossNyami,
    DataBossAzzakael,
    DataBossTeronogor,
    DataMaxBosses,
    DataNyami,
    DataTuulani,
    DataTuulani02,
    DataWarden,
    DataGulkosh,
    DataGuard,
    DataDurag,
    DataIruun,
    DataJorra,
    DataElum,
    DataGromtash,
    DataTriggerMiddleNyamiFightBubble,
    DataTriggerAzzakelController,
    DataLeftCrystal,
    DataRightCrystal,
    DataHolyBarrier,
    DataAuchindonWindow,
    DataFelBarrier,
    DataFelPortal,
    DataSoulTransportStart,
    DataSoulTransport1,
    DataSoulTransport2,
    DataSoulTransport3,
    DataSoulTransport4,
};

enum eAuchindonBosses
{
    BossKaathar   = 86217,
    BossNyami     = 76177,
    BossAzaakel   = 75927,
    BossTeronogor = 77734
};

enum eAuchindonCreatures
{
    CreatureSoulBinderTuulani01    = 322521,
    CreatureAuchenaiDefenderUnique = 76632,
    CreatureSoulBinderNyami        = 77810,
    CreatureSoulBinderTuulani      = 79248,
    CreatureAucheniDefender        = 77693,
    CreatureAucheniMagus2          = 77694,
    CreatureAucheniCleric          = 77947,
    CreatureAucheniSoulPriest      = 77696,
    CreatureAucheniArbiter         = 77695,
    CreatureAucheniVigiliant       = 77819,
    CreatureAucheniSoultender      = 77131,
    CreatureAucheniHoplite         = 77948,
    CreatureAucheniRitualist       = 77949,
    CreatureAucheniWarden          = 77704,
    CreatureAucheniMagus           = 87075,
    CreatureAucheniZealot          = 77950,
    CreatureSargeiHoplite          = 77133,
    CreatureSargereiMagus          = 76263,
    CreatureSargeriRitualist       = 77130,
    CreatureSargeriMagus           = 77694,
    CreatureSargeriZealot          = 77950,
    CreatureSargeriWarden          = 77935,
    CreatureSargereiDefender       = 77042,
    CreatureSargeriSoulPriest      = 76595,
    CreatureSargeriSoulBinder      = 77812,
    CreatureSpitefulArbitrerTrash  = 76284,
    CreatureFelguard               = 76259,
    CreatureCacklingPyromaniac     = 76260,
    CreatureFelborneAbyssal        = 77905,
    CreatureBlazingTrickster       = 79511,
    CreatureZipteq                 = 78734,
    CreatureDuragTheDominator      = 77890,
    CreatureShaadum                = 78728,
    CreatureGulkosh                = 78437,
    CreatureGromtashTheDestructor  = 77889,
    CreatureZashoo                 = 78735,
    CreatureIruun                  = 77762,
    CreatureDurem                  = 77743,
    CreatureJoraa                  = 77756,
    CreatureShieldStalker          = 82681,
    CreatureWardenAzzakael         = 76572,
    CreatureLightWall              = 953642,
    CreatureLightWallTargets       = 79417,
    CreatureShadowBeam             = 100055,
    CreatureShieldVisual           = 82681,
    CreatureArcaneBomb             = 324156,
    CreatureCorpsesNyamiFight      = 456234,
    CreatureShieldSpot             = 464574,
    CreatureArcaneBombExplosionNpc = 341634,
    CreatureWardensHammer          = 76655,
    CreatureDemonsSummoner         = 76216
};

enum eAuchindonMovementInform
{
    MovementInformTuulani01 = 0,
    MovementInformTuulani02,
    MovementInformTuulani03,
    MovementInformTuulani04,
    MovementInformTuulani05,
    MovementInformTuulani06,
    MovementInformTuulani07,
    MovementInformTuulani08,
    MovementInformTuulani09,
    MovementInformTuulani10,
    MovementInformNyamiEscape05,
    MovementInformNyamiEscape06,
    MovementInformNyamiEscape07,
    MovementInformNyamiEscape08
};

enum eAuchindonGeneralActions
{
    ActionCountPre1StBossKill = 1,
    ActionCountPre3StBossKill,
    ActionDeactivateFixation,
    ActionCountPortals,
    ActionSoulMove1,
    ActionSoulMove2,
    ActionSoulMove3,
    ActionSoulMove4,
    ActionConfirmTuulaniEvent,
    ActionDemonSoulsAchievement,
};

enum eAuchindonTalk
{
    AUCHENAIDEFENDERTALK1 = 3,  ///< Nyami awaits you in the western ritual chamber, champions.()
    TUULANITALK1          = 0,  ///< Good! We must have arrived before Teron'gor and Gul'den! (44644)
    TUULANITALK2          = 1,  ///< Nyami is working to bolster the defenses against their assault - follow me,we will report to her. (44654)
    TUULANITALK3          = 2,	///< Beautiful, isn't it? Just think - you're one of the first outsiders to see inside the Auchindoum. (44655) 
    TUULANITALK4          = 4,  ///< Strange... why the door is sealed? No matter... (44656)
    TUULANITALK5          = 5,  ///< The Vindicators and Exrachs of the past. Heroes all - their souls still remain with us here. (44658)
    TUULANITALK6          = 6,  ///< Nyami! We are here - (44659)
    TUULANITALK7          = 7,  ///< What is this? Oh.. no.. no!
    TUULANITALK8          = 8,  ///< You have to stop them! (44661)
    TUULANITALK9          = 9,  ///< The protector still answers her call, you'll have to defeat it! (44645)
    TUULANITALK10         = 10, ///< 44646
    TUULANITALK11         = 11, ///< She got away.. the defences! (44647)
    TUULANITALK12         = 12, ///< She had help.. so many of our order were corrupt! (44648)
    TUULANITALK13         = 13, ///< I will gather any who remain loyal here, but you.. please.. go stop this madness (44649)
    TUULANITALK14         = 14, ///< You.. you did it! (44650)
    TUULANITALK15         = 15, ///< Here.. this is not enough to thank you for your actions, but it is what we have to give. (44651)
    TUULANITALK16         = 16, ///< Now we can rebuild, we must rebuild. We'll never allow our souls to fall to the legion. (44652)
    TUULANITALK17         = 17, ///< You have done more then you know, our people can never thank you enough. (44653)
    TUULANITALK18 = 18, ///< Let us proceed (44657)

    NYAMITALK1            = 21, ///< Yes Tuulani, now you see. (43638)
    NYAMITALK2            = 22, ///< Your efforts were valiant, I did not think the Encorai could powered the defences with only one of the Crystals. (43639)
    NYAMITALK3            = 23, ///< But nothing will stop my plan, nor that of my master's.. and the only thing that's on our way.. (43630)
    NYAMITALK4            = 24, ///< Are YOU Tuulani, and your outsider friends! (43641)
    NYAMITALK5            = 25, ///< Try as you might, this place is doomed.. it is only a matter of time. (43642)
    NYAMITALK6            = 26, ///< Yes... yes! the Crystal harms, just a bit of focus and.. (43643)
    NYAMITALK7            = 27, ///< It is done! as for you.. I think it is fitting that one of my tools dispatches the others! (43644)
    NYAMITALK8            = 28  ///< Protector! destroy these intruders! (43645)
};

enum eAuchindonSpells
{
    SpellArcaneChanneling               = 161383,
    SpellArcaneBombTriggerMissile       = 157791,
    SpellWardenChainRoot                = 154263,
    SpellWardenChainJump                = 154639,
    SpellWardenChainDot                 = 154831,
    SpellApplaud                        = 106301,
    SpellGuard                          = 156893,
    SpellShadowBeam                     = 156862,
    SpellKneel                          = 142479,
    SpellTuulaniUnlock                  = 160415,
    SpellBarrierVisual                  = 160993,
    SpellLevitateNyami                  = 136383,
    SpellNyamiEscape                    = 153992,
    SpellNyamiExplodeCrystal            = 153991,
    SpellDarkFire                       = 156955,
    SpellVoidFormTriggerBuff            = 163715,
    SpellDispersionVisualNyami          = 156868,
    SpellHolyBeam                       = 158565,
    SpellTuulaniCapturedVoidPrison      = 154840,
    SpellCrystalEarthquake              = 150209,
    SpellVoidChannel                    = 170677,
    SpellPrisonAura                     = 167081,
    SpellStrangulate                    = 78037,
    SpellDrainSoulVisual                = 156862,
    SpellAbyssalVisual                  = 159610,
    SpellMindSpike                      = 157043,
    SpellSeverTendonDummy               = 157167,
    SpellSeverTendonAura                = 157165,
    SpellVoidMendingDummy               = 154623,
    SpellVoidMendingAura                = 165031,
    SpellVoidShiftDummy                 = 155524,
    SpellVoidShiftDamage                = 157037,
    SpellShieldBash                     = 157159,
    SpellVoidStrikes                    = 166749,
    SpellVoidShell                      = 160312,
    SpellMindShear                      = 157170,
    SpellBendWill                       = 154527,
    SpellArcaneBombAreaTrigger          = 154174,
    SpellArcaneBombDmg                  = 154232,
    SpellArcaneBombDummy                = 157652,
    SpellAvengersShield                 = 165715,
    SpellWardenHammerDummy              = 154729,
    SpellWardenThrowHammer              = 154730,
    SpellWardenHammerLightning          = 154740,
    SpellWardenHammerDamage             = 154773,
    SpellWardenHammerLightningTempo     = 154775,
    SpellFelblast                       = 167092,
    SpellFelStomp                       = 157173,
    SpellCrusaderStirke                 = 176931,
    SpellFixate                         = 157168,
    SpellConfligirate                   = 154981,
    SpellEmoteTalk                      = 1,
    SpellEmoteHover                     = 533,
    SpellEmoteRead                      = 483,
    SpellVoidShiftHeal                  = 157036,
    SpellVoidShiftDmg                   = 157037,
    SpellShadowWordPainPriest                 = 176518,
    SpellPsychicTerrorDummy             = 154356,
    SpellPsychicTerrorFear              = 154359,
};

enum eAuchindonEvents
{
    EventMindSpike = 1,
    EventSeverTendom,
    EventVoidMending,
    EventHeroicVoidMending,
    EventVoidShift,
    EventShieldBash,
    EventVoidStrikes,
    EventVoidShell,
    EventMindShear,
    EventBendWill,
    EventArcaneBomb,
    EventCrusaderStirke,
    EventFelblast,
    EventFelStomp,
    EventAvengersShield,
    EventFixate,
    EventRandomMove,
    EventWardenHammer,
    EventWardenChain,
    EventConfligrate,
    EventBeginRp,
    EventShadowWordPainSoulPriest,
    EventPsychicTerrors,
};

enum eAuchindonObjects
{
    GameobjectGhostObject        = 180322,
    GameobjectHolyBarrier        = 230399,
    GameobjectHolyWall           = 231680,
    GameobjectFelBarrier         = 230400,
    GameobjectTaladorPortal      = 236689,
    GameobjectChestAucheni       = 231241,
    GameobjectNightmareBell      = 237479,
    GameobjectDemonicPortal      = 543242,
    GameobjectPuddle             = 237542,
    GameobjectAuchindonWindow    = 233009,
    GameobjectSoulTransportStart = 231736,
    GameobjectSoulTransport1     = 345366,
    GameobjectSoulTransport2     = 345367,
    GameobjectSoulTransport3     = 345368
};

enum eAuchindonAchievements
{
    AchievementAuchindonNormal = 9039,
    AchievementAuchindonHeroic = 9049,
    AchievementAuchindonMythic = 10080,
    AchievementDemonSouls      = 9551,
    AchievementNoTagBacks      = 9552,
};

enum eAuchindounChallengeDatas
{
    DailyChallengeQuestID       = 37259,
    DailyChallengeKillCredit    = 79117,

    ChallengeModeDoor           = 211989,
    ChallengeModeDoorSecond     = 212972,

    /// Scenario Datas
    NormalScenarioID            = 678,
    ChallengeScenarioID         = 427,
    EnnemiesCount               = 48,
    /// Scenario Criteria IDs
    KaatharCriteriaID           = 25102,
    NyamiCriteriaID             = 25103,
    AzaakelCriteriaID           = 25104,
    TerongorCriteriaID          = 24991,
    EnnemiesCriteriaID          = 26335
};
