#ifndef TANAAN_JUNGLE_H
#define TANAAN_JUNGLE_H

enum eSpells
{
    SpellKnockthrough  = 166131,
    SpellArcaneBarrage = 133123,
    SpellMeteorShower  = 165864
};

enum eEvents
{
    EventAggro     = 1
};

enum eFactions
{
    FactionAggressive = 14,
    FactionFriendly   = 35,
    FactionNeutral    = 7
};

enum eQuests
{
    QuestStartDraenor            = 36881,
    QuestDarkPortal              = 34398,
    QuestOnslaughtEnd            = 34392,
    QuestThePortalPower          = 34393,
    QuestBledDry                 = 35240,
    QuestCostOfWar               = 34420,
    QuestBlazeOfGlory            = 34422,
    QuestAltarAltercation        = 34423,
    QuestAzerothsLastStand       = 35933,
    QuestKargatharProvingGrounds = 34425
};

enum eActions
{
};

enum eSceneObjectIds
{
    SceneCostOfWarEscort   = 806,
    SceneBridgeDestruction = 903,
    SceneKilRoggRevealed   = 940,
    SceneGulDanFreedom     = 808,
    SceneEasterCage        = 813,
    SceneSouthernCage      = 812,
    SceneChoGallsFreedom   = 961,
    SceneTeronGorsFreedom  = 962,
    SceneGulDanReavel      = 925
};

enum eCreatures
{
    NpcIronGrunt                 = 78883,
    NpcMumper                    = 82188,
    NpcMoriccalas                = 81990,
    NpcBleedingHollowBloodchosen = 81895,
    NpcArchmageKhadgarSum        = 80244,
    NpcBlazeOfGloryTrigger       = 300004
};

enum eKillCredits
{
    CreditDarkPortalKill        = 78419,
    CreditStatisRuneDestroyed   = 78333,
    CreditEnterGuldanPrison     = 82573,
    CreditAriokEscort           = 78975,
    CreditFindKhadgarAtBridge   = 84456,
    CreditFollowKhadgar         = 80244,
    CreditBloodOrb              = 78966,
    CreditEasternCageOpened     = 85142,
    CreditSouthernCageOpened    = 82871,
    CreditNorthernSpireDisabled = 82606,
    CreditSouthernSpireDisabled = 82607,
    CreditAriokGossip           = 78556
};

enum eGameObjects
{
    GobWholeBridge          = 231137,
    GobDestroyedBridge      = 231136,
    GobEasternCage          = 229350,
    GobSouthernCage         = 229325,
    GobMarkOfShadowmoon     = 233056,
    GobMarkOfBleedingHollow = 233057,
};

enum eObjectiveIds
{
    ObjBloodRitualOrbDestroyed = 273678,
    ObjFollowKhadgar           = 273737,
    ObjBurningBladeDestroyed   = 273438,
    ObjShatteredHandDestroyed  = 273556,
    ObjBlackrockMarkDestroyed  = 273557,
    ObjEasternCageOpened       = 273640,
    ObjSouthernCageOpened      = 273639,
    ObjNorthernSpireDisabled   = 272621,
    ObjSouthernSpireDisabled   = 273946,
    ObjAriokGossip             = 273075,
    ObjEnterGulDanPrison       = 273930
};

enum eZoneIds
{
    MapTanaan              = 1265,
    ZoneTanaanJungle       = 7025,
    AreaTheDarkPortal      = 7037,
    AreaBlackRockQuarry    = 7043
};


enum ePhases
{
    PhaseDarkPortalBase         = 0x0001,
    PhaseGulDan                 = 0x0002,
    PhaseEscortSecondZone       = 0x0004, // SCENE OBJECT 806
    PhaseCostOfWarEnded         = 0x0008,
    PhaseBlazeOfGlory           = 0x0010,
    PhaseAltarAltercation       = 0x0020
};

#endif
