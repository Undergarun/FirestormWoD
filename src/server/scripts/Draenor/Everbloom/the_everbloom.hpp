
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

enum eEverbloomData
{
    DataWitherbark       = 500,
    DataLifeWardenGola   = 501,
    DataEarthshaperTelu  = 502,
    DataDulhu            = 503,
    DataXeritac          = 504,
    DataArchmageSol      = 505,
    DataRpYalnu          = 506,
    DataYalnu            = 507,
    DataRpMage           = 508,
    DataObjectVineWall   = 509,
    DataObjectWebDoor    = 510,
};

enum eEverbloomBosses
{
    BossWitherbark       = 81522,
    BossLifeWardenGola   = 83892,
    BossEarthshaperTelu  = 83893,
    BossDulhu            = 83894,
    BossXeritac          = 84550,
    BossArchmageSol      = 82682,
    BossYalnu            = 83846,
};

enum eEverbloomGameObjects
{
    ObjectVineWall       = 235363,
    ObjectWebDoor        = 195485,
};

enum eEverbloomCreature
{
    // Everbloom General:
    CreatureDreadpetalToxin        = 81864,
    CreatureEverbloomNaturalist    = 81819,
    CreatureEverbloomTender        = 81985,
    CreatureMeldedBerserker        = 86372,
    CreatureGnarlroot = 81984,
    CreatureVerdantMandragora      = 81983,
    CreatureEverbloomMender        = 81820,
    CreatureTwistedAbomination     = 84767, 
    
    // Pre ArchmageSol:
    CreatureInfestedIcecaller      = 84989,
    CreatureAddledArcanomancer     = 84990,
    CreaturePutridPyromancer       = 84957,

    // Pre Xeritac
    CreatureInfestedVenomfang      = 85232,
    CreatureVenomSprayer           = 86547,
    CreatureVenomCrazedPaleOne     = 84554,
    CreatureToxicSpiderling        = 84552,
    CreatureGorgendBusters         = 86552,
  
    // Witherbark
    CreatureEnchanctedWater        = 88862,
    CreatureGlobuleWater = 81821,

    // Role Playing
    CreatureRpYalnu                = 84336,
    CreatureTriggerFire            = 324234,
    CreatureKirinTorBattleMage     = 84329,
    CreatureUndermageKeasel        = 85496,

    // Hacked Areatriggers
    CreatureTriggerTeleportToYalnu = 324251,
    CreatureLadyBayeu              = 84358,
    TriggerTenderBeamTrigger       = 84677,
    TriggerLivingLeaves            = 324266,
    TriggerFrozenSnap              = 321432,
};

enum eEverbloomEvents
{
    EventDreadpetalToxin    = 1,
    EventBarrageOfLeaves    = 2,
    EventDancingThorns      = 3,
    EventEnragedGrowth      = 4,
    EventVileBreath         = 5,
    EventEnrage             = 6,
    EventBoundingWhirl      = 7,
    EventGasp               = 8,
    EventVirulendGasp       = 9,
    EventChokingVines       = 10,
    EventHealingWaters      = 11,
    EventInfectedWounds     = 12,
    EventNoxiousEruption    = 13,
    EventSporeBreath        = 14,
    EventToxicBlood         = 15,
    EventToxicity           = 16,
    EventVenomSpray         = 17,
    EventFrostbolt          = 18,
    EventFrozenSnap         = 19,
    EvertAraceBlast         = 20,
    EventDragonsBreath      = 21,
    EventFireBall           = 22,
    EventTendonRip          = 23,
    EventLasherVenom        = 24,
    EventArcaneBlast        = 25,
    EventArcaneOrb          = 26,
    EventFireball           = 27,
    EventFlamestike         = 28,
    EventIceComet           = 29,
    EventPoisonousClaws     = 30,
    EventBoundingWhirl2     = 31,
};

enum eEverbloomSpells
{
    SpellDreadpetalToxin         = 164886,
    SepllBarrageOfLeaves         = 164834,
    SpellDancingThorns           = 164973,
    SpellEnragedGrowth           = 165213,
    SpellVileBreath              = 172588,
    SpellEnrage                  = 38166,
    SpellBoundingWhirlAura       = 172578,
    SpellBoundingWhirlJump       = 172577,
    SpellBoundingWhirlDummy      = 172576,
    SpellGasp                    = 169714,
    SpellVirulendGasp            = 165093,
    SpellChokingVines            = 164965, 
    SpellHealingWaters           = 164887, 
    SpellSporeBreath             = 170411,
    SpellSporeBreathInfect       = 169854,
    SpellToxicBloodAura          = 169218,
    SpellToxicity                = 169219, 
    SpellVenomSprayDummy         = 173052,
    SpellVenomSprayDamage        = 172727,
    SpellFrostbolt               = 169840,
    SpellFrostbolt4Th            = 170028,
    SpellFlamestrikeAura         = 169100,
    SpellFlamestikeAreatriger    = 169094,
    SpellIceComet                = 170032,
    SpellArcaneBlast             = 169825,
    SpellArcaneBlast4Th          = 170035,
    SpellArcaneOrbDummy          = 167018,
    SpellArcaneOrbAreatriger     = 170040,
    SpellArcaneOrbDamage         = 170077,
    SpellFireball                = 168666,
    SpellDragonsBreath           = 169843,
    SpellFireBall                = 169839,
    SpellTendonRip               = 169876, 
    SpellLasherVenom             = 173563, 
    SpellBlinkMageUponSummon     = 142193,
    SpellNoxiousBreath           = 169878,
    SpellLumberingSwipe          = 169929,
    SpellEntanglementVisual      = 169192,
    SpellEntanglementStun        = 169240,
    SpellLivingLeavesDummy       = 169494,
    SpellLivingLeavesEffect      = 169495,
    SpellLivingLeavesAreaTrigger = 169497,
    SpellPoisonousClaws          = 169657,
    SpellInfectedWounds          = 169658,
    SpellAbominationNoxiousErupt = 169445,

    // Cosmetic
    SpellBeamCosmetic            = 164850,
    SpellWitherbarkVisPreFight   = 164713,
    SpellSolarChannel            = 170594,
    SpellCosmeticRoot            = 164715,
    SpellVisualSolArcaneBeam     = 170146,
    SpellSubmerge                = 175123,
    SpellNaturesChanneling       = 164850,
};

enum eEverbloomActions
{
    ActionCountPre1StBossKill = 1,
    ActionCounting            = 2,
    ActionYalnuEvent          = 3,
    ActionBoundingWhirlAura   = 4,
};

enum eEverbloomAchievements
{
    AchievementTheEverbloomNormal = 9044,
    AchivementTheEverbloomHeroic  = 9053,
    AchivementWeedWhacker         = 9223,
    AchivementWaterManagement     = 9017,
};