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
        QuestDarkPortal                      = 34398,
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
        QuestTakingATripToTheTopOfTheTank    = 35747
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
        SceneFromCaveToRidge                 = 922,
        SceneGulDanReavel                    = 925,
        SceneWaterPortal                     = 928,
        SceneKilRoggRevealed                 = 940,
        SceneEnterKarGathArena               = 945,
        SceneEscapingTheArena                = 946,
        SceneLiadrinAndOlinEnter             = 948,
        SceneKargathYells                    = 951,
        SceneNerZhulReveal                   = 952,
        SceneCaveIn                          = 956,
        SceneChoGallsFreedom                 = 961,
        SceneTeronGorsFreedom                = 962,
        SceneSoulTrain                       = 1018
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
        CreditBoatsReached                   = 81024
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
        ObjIronHordeSlain                    = 273041
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
        ZoneTanaanJungle                     = 7025,
        AreaTheDarkPortal                    = 7037,
        AreaBlackRockQuarry                  = 7043,
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
    };
}


namespace TanaanPhases
{
    enum
    {
        PhaseBase                            = 0x000001,
        PhasePortal                          = 0x000002,
        PhaseChoGall                         = 0x000004,
        PhaseTeronGor                        = 0x000008,
        PhaseGulDan                          = 0x000010,
        PhaseEscortGroup                     = 0x000020,
        PhaseHouseGroup                      = 0x000040,
        PhaseEastCage                        = 0x000080,
        PhaseSouthCage                       = 0x000100,
        PhaseAltarGroup                      = 0x000200,
//         Phaseunk                            = 0x000400,
//         Phaseunk                            = 0x000800,
//         Phaseunk                            = 0x001000,
//         Phaseunk                            = 0x002000,
//         Phaseunk                            = 0x004000,
//         Phaseunk                            = 0x008000,
//         Phaseunk                            = 0x010000,
//         Phaseunk                            = 0x020000,
//         Phaseunk                            = 0x040000,
//         Phaseunk                            = 0x080000,
//         Phaseunk                            = 0x100000,
//         Phaseunk                            = 0x200000,
//         Phaseunk                            = 0x400000,
//         Phaseunk                            = 0x800000,
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
