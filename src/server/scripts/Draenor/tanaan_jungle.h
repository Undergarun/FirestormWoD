#ifndef TANAAN_JUNGLE_H
#define TANAAN_JUNGLE_H

struct Hut
{
    float X, Y, Z;
    bool Fired;
};

namespace TanaanQuests
{
    enum
    {
        QuestStartDraenor                    = 36881,
        QuestAzerothLastStand                = 35933,
        QuestOnslaughtEnd                    = 34392,
        QuestThePortalPower                  = 34393,
        QuestBledDryAlly                     = 35240,
        QuestBledDryHorde                    = 34421,
        QuestCostOfWar                       = 34420,
        QuestBlazeOfGlory                    = 34422,
        QuestAltarAltercation                = 34423,
        QuestAzerothsLastStand               = 35933,
        QuestKargatharProvingGrounds         = 34425,
        QuestAPotentialAlly                  = 34478,
        QuestAPotentialAllyHorde             = 34427,
        QuestKillYourHundred                 = 34429,
        QuestMastersOfShadowAlly             = 34431,
        QuestMastersOfShadowHorde            = 34737,
        QuestKeliDanTheBreakerHorde          = 34741,
        QuestKeliDanTheBreakerAlly           = 34436,
        QuestTheGunpowderPlot                = 34987,
        QuestATasteOfIron                    = 34445,
        QuestTheHomeStretchHorde             = 34446,
        QuestTheHomeStretchAlly              = 35884,
        QuestYrelHorde                       = 34740,
        QuestYrelTanaan                      = 34434,
        QuestTheBattleOfTheForge             = 34439,
        QuestTakingATripToTheTopOfTheTank    = 35747,
        QuestGaNarOfTheFrostwolf             = 34442,
        QuestShadowOfTheWorldbreaker         = 34958
    };
}

namespace TanaanSceneObjects
{
    enum
    {
        SceneCostOfWarEscort                 = 806,
        SceneGulDanFreedom                   = 808,
        SceneSouthernCage                    = 812,
        SceneEasterCage                      = 813,
        SceneRingOfFire                      = 838,
        SceneKargathFight                    = 839,
        SceneShootingGallery                 = 871,
        SceneBlackHandReveal                 = 896,
        SceneBuildingExplosion               = 902,
        SceneBridgeDestruction               = 903,
        SceneDamExplosion                    = 910,
        SceneFinaleIronBastion               = 912,
        SceneFromCaveToRidge                 = 922,
        SceneGulDanReavel                    = 925,
        SceneWaterPortal                     = 928,
        SceneKilRoggRevealed                 = 940,
        SceneEnterKarGathArena               = 945,
        SceneEscapingTheArena                = 946,
        SceneLiadrinAndOlinEnter             = 948,
        SceneKargathYells                    = 951,
        SceneNerZhulReveal                   = 952,
        SceneHordeBoat                       = 953,
        SceneCaveIn                          = 956,
        SceneChoGallsFreedom                 = 961,
        SceneTeronGorsFreedom                = 962,
        SceneAllianceBoat                    = 968,
        SceneSoulTrain                       = 1018
    };
}

namespace TanaanPhases
{
    enum
    {
        PhaseBase                            = 0x00000001,
        PhasePortal                          = 0x00000002, ///< Area 1 (7037)
        PhaseChoGall                         = 0x00000004, ///< Area 1 (7037)
        PhaseTeronGor                        = 0x00000008, ///< Area 1 (7037) 0x0000000F
        PhaseGulDan                          = 0x00000010, ///< Area 1 (7037)
        PhaseEscortGroup                     = 0x00000020, ///< Area 1 (7037)
        PhaseGroupHouse                      = 0x00000040, ///< Area 2 (7041)
        PhaseEasternCageAlly                 = 0x00000080, ///< Area 2 (7041) 0x000000F0
        PhaseSouthernCageAlly                = 0x00000100, ///< Area 2 (7041)
        PhaseEasternCageHorde                = 0x00000200, ///< Area 2 (7041)
        PhaseSouthernCageHorde               = 0x00000400, ///< Area 2 (7041)
        PhaseAltarGroup                      = 0x00000800, ///< Area 2 (7041) 0x00000F00
        PhaseAriokAltar                      = 0x00001000, ///< Area 2 (7041)
        PhaseBridge                          = 0x00002000, ///< Area 3 (7129)
        PhaseKhadgarBridge                   = 0x00004000, ///< Area 4 (7040) 0x0000F000
        PhaseGroupKillYourHundred            = 0x00008000, ///< Area 4 (7040)
        PhaseExarchMalaadar                  = 0x00010000, ///< Area 4 (7040)
        PhaseArenaOrcs                       = 0x00020000, ///< Area 4 (7040)
        PhaseArenaPrisonnersAlly             = 0x00040000, ///< Area 4 (7040) 0x000F0000
        PhaseArenaPrisonnersHorde            = 0x00080000, ///< Area 4 (7040)
        PhaseAfterArena                      = 0x00100000, ///< Area 4 (7040)
        PhaseRocks                           = 0x00200000, ///< Area 5 (7042)
        PhaseOlinLiadrin                     = 0x00400000, ///< Area 5 (7042) 0x00F00000
        PhaseYrelQuestGiver                  = 0x00800000, ///< Area 5 (7042)
        PhaseYrelQuestTaker                  = 0x01000000, ///< Area 5 (7042)
        PhaseGroupPool                       = 0x02000000, ///< Area 6 (7043)
        PhaseLiadrinCordana                  = 0x04000000, ///< Area 6 (7043) 0x0F000000
        PhaseThaelinQuestGiver               = 0x08000000, ///< Area 6 (7043)
        PhaseThaelinTurret                   = 0x10000000, ///< Area 7 (7044)
        PhasePortalDestroyed                 = 0x20000000, ///< Area 7 (7044)
        PhaseCannonTurret                    = 0x40000000, ///< Area 8 (7045)
        PhaseQuestGiversHorde                = 0x00000200, ///< Area 2 (7041) ( Also PhaseEasternCageHorde )
        PhaseQuestGiversAlly                 = 0x00000080, ///< Area 2 (7041) ( Also PhaseEasternCageAlly  )
        PhaseCaveGroup                       = 0x00200000, ///< Area 5 (7042) ( Also PhaseRocks            )
        PhaseCaveLastRock                    = 0x00000004, ///< Area 6 (7043) ( Also PhaseChoGall          )
        PhasePoolMobs                        = 0x02000000, ///< Area 6 (7043) ( Also PhaseGroupPool        )
        PhaseFinalePhase                     = 0x00000008, ///< Area 8 (7045) ( Also PhaseTeronGor         )
    };
}

namespace TanaanKillCredits
{
    enum
    {
        CreditDarkPortalKill                 = 78419,
        CreditStatisRuneDestroyed            = 78333,
        CreditEnterGuldanPrison              = 82573,
        CreditAriokEscort                    = 78975,
        CreditFindKhadgarAtBridge            = 84456,
        CreditFollowKhadgar                  = 80244,
        CreditBloodOrb                       = 78966,
        CreditEasternCageOpened              = 85142,
        CreditSouthernCageOpened             = 82871,
        CreditNorthernSpireDisabled          = 82606,
        CreditSouthernSpireDisabled          = 82607,
        CreditAriokGossip                    = 78556,
        CreditEnterTheArena                  = 82139,
        CreditCombattantSlainInArena         = 82066,
        CreditEscapeKargathArena             = 82140,
        CreditIronHordeSlain                 = 80016,
        CreditCombattantSlainAdd             = 82142,
        CreditEscortYrel                     = 79794,
        CreditBlackrockGrunt                 = 80786,
        CreditOgronWarcrusher                = 80775,
        CreditSpeakWithThaelin               = 80880,
        CreditEscortThaelin                  = 80887,
        CreditBoatsReached                   = 81024,
        CreditEnterWorldbreakerTurret        = 80929,
        CreditMakeShiftPlunger               = 80520,
        CreditMainCannonTrigger              = 232538
    };
}

namespace TanaanQuestObjectives
{
    enum
    {
        ObjBloodRitualOrbDestroyed           = 273678,
        ObjFollowKhadgar                     = 273737,
        ObjBurningBladeDestroyed             = 273438,
        ObjShatteredHandDestroyed            = 273556,
        ObjBlackrockMarkDestroyed            = 273557,
        ObjEasternCageOpened                 = 273640,
        ObjSouthernCageOpened                = 273639,
        ObjNorthernSpireDisabled             = 272621,
        ObjSouthernSpireDisabled             = 273946,
        ObjAriokGossip                       = 273075,
        ObjEnterGulDanPrison                 = 273930,
        ObjEnterTheArena                     = 273758,
        ObjCombattantSlainInArena            = 273713,
        ObjCombattantSlainAdd                = 273763,
        ObjUngraMastersOfShadowHorde         = 272982,
        ObjUngraMastersOfShadowAlly          = 272926,
        ObjGurranMastersOfShadowHorde        = 272983,
        ObjGurranMastersOfShadowAlly         = 272927,
        ObjAnkovaMastersOfShadowsHorde       = 272984,
        ObjAnkovaMastersOfShadowsAlly        = 272929,
        ObjIronHordeSlain                    = 273041,
        ObjTopOfTheTank                      = 274029
    };
}

namespace TanaanCreatures
{
    enum
    {
        NpcIronGrunt                         = 78883,
        NpcMumper                            = 82188,
        NpcMoriccalas                        = 81990,
        NpcBleedingHollowBloodchosen         = 81895,
        NpcArchmageKhadgarSum                = 80244,
        NpcBlazeOfGloryTrigger               = 300004,
        NpcKargath                           = 79097,
        NpcShatteredHandBrawler              = 82057,
        NpcHanselHeavyHands                  = 78569,
        NpcAriok                             = 78556,
        NpcYrel                              = 78994,
        NpcExarchMaladaar                    = 79537,
        NpcLadyLiadrin                       = 79675,
        NpcCordanaFelsong                    = 78430,
        NpcThaelinDarkanvil                  = 80521,
        NpcGogluk                            = 86039,
        NpcCannonTurret                      = 86690,
        NpcGunTurret                         = 86691,
        NpcThaelinDarkanvilSecond            = 78568,
        NpcBlackRockTrigger                  = 300005
    };
}

namespace TanaanGameObjects
{
    enum
    {
        GobWholeBridge                       = 231137,
        GobDestroyedBridge                   = 231136,
        GobEasternCage                       = 229350,
        GobSouthernCage                      = 229325,
        GobMarkOfShadowmoon                  = 233056,
        GobMarkOfBleedingHollow              = 233057,
        GobRingOfFire                        = 500003,
        GobIronCageDoor                      = 233197
    };
}

namespace TanaanZones
{
    enum
    {
        MapTanaan                            = 1265,
        MapDraenor                          = 1116,
        ZoneTanaanJungle                     = 7025,
        AreaTheDarkPortal                    = 7037,
        AreaHearthBlood                      = 7041,
        AreaTarThogBridge                    = 7129,
        AreaKargatharProvingGrounds          = 7040,
        AreaUmbralHalls                      = 7042,
        AreaBlackRockQuarry                  = 7043,
        AreaPathOfGlory                      = 7044,
        TerrainSwapID                        = 1307
    };
}

enum eFactions
{
    FactionAggressive                        = 14,
    FactionFriendly                          = 35,
    FactionNeutral                           = 7
};

namespace TanaanActions
{
    enum
    {
        ActionTimerSummon                    = 1,
        ActionEventSceneArena                = 2,
        ActionSummonHandBrawler              = 3
    };
}

namespace TanaanSpells
{
    enum
    {
        SpellTasteOfIronGameAura             = 164042,
        SpellHornOfWinter                    = 165762,
        SpellAuraTormentedSoul               = 166452,
        SpellKnockthrough                    = 166131,
        SpellArcaneBarrage                   = 133123,
        SpellMeteorShower                    = 165864,
        SpellCoverOfElune                    = 165900,
        SpellCrushingStomp                   = 166032,
        SpellCannonBarrage                   = 173479,
        SpellMachineGun                      = 173502,
        SpellIronBastionProgressA            = 163524,
        SpellIronBastionProgressB            = 163525,
        SpellIronBastionProgressC            = 163526,
        SpellIronBastionProgressD            = 163527,
        SpellIronBastionProgressE            = 163528,
        SpellIronBastionProgressF            = 163529,
        SpellIronBastionProgressG            = 163530,
        SpellDissociateFaction               = 149211 ///< Originally called "[UNUSED]Talk with Cordana" (unlimited aura dummy)
    };
}

namespace TanaanMovies
{
    enum
    {
        MovieDoorDestruction                 = 187,
        MovieEnterPortal                     = 199
    };
}

namespace BlazeOfGloryData
{
    enum
    {
        SceneId                              = 934,
        HutsPath                             = 10568,
        SpellTrailOfFlameVisual              = 165991,
        HutProxDist                          = 20
    };
}

#endif
