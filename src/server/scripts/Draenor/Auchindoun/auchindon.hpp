////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef DEFAUCHINDONH
#define DEFAUCHINDONH


static void DespawnCreaturesInArea(uint32 entry, WorldObject* object)
{
    std::list<Creature*> creatures;
    GetCreatureListWithEntryInGrid(creatures, object, entry, 5000.0f);
    if (creatures.empty())
        return;

    for (std::list<Creature*>::iterator iter = creatures.begin(); iter != creatures.end(); ++iter)
        (*iter)->DespawnOrUnsummon();
}

Position g_PositionTuulaniMovement000              = { 1644.27f, 2962.42f, 35.185f, 1.269892f };
Position g_PositionTuulaniPreMovement01            = { 1538.54f, 2952.99f, 35.239f, 0.041050f };
Position g_PositionTuulaniPreMovement02            = { 1591.58f, 2953.37f, 35.239f, 0.021415f };
Position g_PositionTuulaniPreMovement03            = { 1646.19f, 2956.79f, 35.060f, 0.975673f };
Position g_PositionTuulaniMovement01               = { 1660.10f, 2999.02f, 34.763f, 1.168017f };
Position g_PositionTuulaniMovement02               = { 1685.99f, 3065.55f, 35.139f, 1.204807f };
Position g_PositionTuulaniMovement03               = { 1722.35f, 3121.52f, 35.139f, 0.914209f };
Position g_PositionTuulaniMovement04               = { 1756.30f, 3154.63f, 35.139f, 0.772838f };
Position g_PositionTuulaniMovement05               = { 1806.26f, 3178.98f, 35.127f, 0.454751f };
Position g_PositionTuulaniMovement06               = { 1835.91f, 3183.27f, 29.423f, 0.129596f };
Position g_PositionTuulaniMovement07               = { 1854.28f, 3199.26f, 29.424f, 5.918766f };
Position g_PositionTuulaniMovement08               = { 1867.89f, 3197.28f, 30.970f, 6.138678f };
Position g_PositionDefenderWhoReadsNearTwoHopilite = { 1691.51f, 3070.65f, 35.223f, 5.132760f };
Position g_PositionDefenderWhoTalksToSoldiers      = { 1709.31f, 3099.06f, 35.139f, 5.945590f };
Position g_PositionSoulPriestWhoMeditates          = { 1701.68f, 3113.02f, 35.223f, 0.0f };
Position g_PositionMagusWhoCastArcane              = { 1710.42f, 3113.87f, 35.223f, 3.386260f };
Position g_PositionMagusWhoTalksToDefender         = { 1728.73f, 3139.19f, 35.223f, 2.261400f };
Position g_PositionSoulPriestTalksToTwoDefender    = { 1745.93f, 3138.90f, 35.223f, 0.293292f };
Position g_PositionKaatharCrystag_Position          = { 1909.75f, 3188.70f, 66.786f, 5.401960f };
Position g_PositionGuardTalker                     = { 1645.61f, 2967.25f, 35.167f, 0.024081f };
Position g_PositionAuchenaiReader1st               = { 1673.96f, 3074.10f, 36.132f, 5.834950f };
Position g_PositionWarden1st                       = { 1510.29f, 2954.78f, 35.238f, 0.091797f };

// Floating Triggers
Position g_PositionLightFloatingTrigger[3] =
{
    { 1664.70f, 3011.64f, 41.503f, 4.432930f },
    { 1757.79f, 2952.85f, 44.559f, 6.247188f },
    { 1665.26f, 2893.94f, 41.317f, 5.010184f }
};
// Combat guards
Position g_PositionGuards[6] =
{
    { 1497.06f, 2948.16f, 35.239f, 2.452730f },
    { 1495.23f, 2945.86f, 35.696f, 2.554010f },
    { 1491.63f, 2944.18f, 36.147f, 2.033919f },
    { 1495.93f, 2956.85f, 35.239f, 3.644247f },
    { 1493.42f, 2958.79f, 35.239f, 3.926992f },
    { 1490.35f, 2959.78f, 35.614f, 3.875940f }
};
// Clapping guards
Position g_PositionGuards2nd[2] =
{
    { 1506.64f, 2958.66f, 35.238f, 4.793027f },
    { 1504.08f, 2958.45f, 35.238f, 4.793027f }
};
// Warden Patrol, and Warden position to be spawned at
Position g_PositionWardenPatrol[2] =
{
    { 1609.98f, 2955.21f, 35.238f },
    { 1508.82f, 2954.96f, 35.238f }
};
// Middle
Position g_PositionCircularMages[3] =
{
    { 1662.40f, 2939.76f, 35.143f, 1.624580f },
    { 1668.03f, 2941.39f, 35.143f, 2.242950f },
    { 1657.34f, 2965.32f, 35.061f, 5.090852f }
};
Position g_PositionCircularPriests[2] =
{
    { 1648.36f, 2950.41f, 35.06f, 0.288927f },
    { 1650.16f, 2947.54f, 34.822f, 0.397321f }
};
Position g_PositionCircularHolies[2] =
{
    { 1653.17f, 2962.95f, 35.061f, 5.514967f },
    { 1650.16f, 2947.54f, 34.822f, 0.397321f }
};
// Left Hallway
Position g_PositionWardenPatrol2nd[2] =
{
    { 1672.66f, 3022.84f, 35.140f },
    { 1673.99f, 3033.03f, 35.236f }
};
Position g_PositionAuchenaiMagus2nd[2] =
{
    { 1675.69f, 3068.34f, 35.223f, 2.696660f },
    { 1679.35f, 3075.59f, 35.223f, 2.604600f }
};
Position g_PositionAuchenaiVigilant[4] =
{
    { 1667.18f, 3077.60f, 36.679f, 5.796140f },
    { 1692.63f, 3118.17f, 36.679f, 5.632550f },
    { 1724.72f, 3153.71f, 36.679f, 5.423370f },
    { 1762.43f, 3183.01f, 36.679f, 5.268510f }
};
Position g_PositionHopilliteGuardState[2] =
{
    { 1691.04f, 3062.77f, 35.223f, 2.610810f },
    { 1693.10f, 3066.61f, 35.140f, 2.546201f }
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
    DataNyami,
    DataTuulani,
    DataWarden,
    DataGulkosh,
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
    DataSoulTransport4
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
    MovementInformTuulani01 = 1,
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
    ActionConfirmTuulaniEvent
};

enum eAuchindonTalk
{
    AUCHENAIDEFENDERTALK1 = 3,  ///< Nyami awaits you in the western ritual chamber, champions.()
    TUULANITALK1          = 0,  ///< Good! We must have arrived before Teron'gor and Gul'den! (44644)
    TUULANITALK2          = 1,  ///< Nyami is working to bolster the defenses against their assault - follow me,we will report to her. (44654)
    TUULANITALK3          = 2,	///< Beautiful, isn't it? Just think - you're one of the first outsiders to see inside the Auchindoum. (44655) 
    TUULANITALK4          = 4,  ///< Strange... why the door is sealed? No matter... (44656)
    TUULANITALK2          = 18, ///< Let us proceed (44657)
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
    SpellArcaneChanneling               = 77855,
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
    SpellVoidShiftDmg                   = 157037
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
    EventBeginRp
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
};
#endif

