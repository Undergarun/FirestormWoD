
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2016 Millenium-studio SARL
///  All Rights Reserved.
///  Coded by Davethebrave
////////////////////////////////////////////////////////////////////////////////

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
{
    std::list<Creature*> l_CreaturesList;
    GetCreatureListWithEntryInGrid(l_CreaturesList, p_Object, p_Entry, 2000.0f);
    if (l_CreaturesList.empty())
        return;

    for (std::list<Creature*>::iterator l_Iter = l_CreaturesList.begin(); l_Iter != l_CreaturesList.end(); ++l_Iter)
        (*l_Iter)->DespawnOrUnsummon();
}

#define InvisibleDisplay 11686
#define FriendlyFaction 35
#define HostileFaction 16
#define AttackableYetNotHostileFaction 7

enum eEverbloomData
{
    DataWitherbark       = 1,
    DataLifeWardenGola,
    DataEarthshaperTelu,
    DataDulhu,
    DataXeritac,
    DataArchmageSol,
    DataRpYalnu,
    DataYalnu,
    DataRpMage,
    DataObjectVineWall,
    DataObjectWebDoor
};

enum eEverbloomBosses
{
    BossWitherbark       = 81522,
    BossLifeWardenGola   = 83892,
    BossEarthshaperTelu  = 83893,
    BossDulhu            = 83894,
    BossXeritac          = 84550,
    BossArchmageSol      = 82682,
    BossYalnu            = 83846
};

enum eEverbloomGameObjects
{
    ObjectVineWall       = 231970,
    ObjectWebDoor        = 195485
};

enum eEverbloomCreature
{
    CreatureDreadpetalToxin         = 81864,
    CreatureEverbloomNaturalist     = 81819,
    CreatureEverbloomTender         = 81985,
    CreatureMeldedBerserker         = 86372,
    CreatureGnarlroot               = 81984,
    CreatureVerdantMandragora       = 81983,
    CreatureEverbloomMender         = 81820,
    CreatureTwistedAbomination      = 84767,
    CreatureInfestedIcecaller       = 84989,
    CreatureAddledArcanomancer      = 84990,
    CreaturePutridPyromancer        = 84957,
    CreatureInfestedVenomfang       = 85232,
    CreatureVenomSprayer            = 86547,
    CreatureVenomCrazedPaleOne      = 84554,
    CreatureToxicSpiderling         = 84552,
    CreatureGorgendBusters          = 86552,
    CreatureEnchanctedWater         = 88862,
    CreatureGlobuleWater            = 81821,
    CreatureRpYalnu                 = 84336,
    CreatureTriggerFire             = 324234,
    CreatureKirinTorBattleMage      = 84329,
    CreatureUndermageKeasel         = 85496,
    CreatureTriggerTeleportToYalnu  = 324251,
    CreatureLadyBayeu               = 84358,
    CreatureTenderBeamTrigger       = 84677,
    CreatureLivingLeaves            = 324266,
    CreatureFrozenSnap              = 321432
};

/*
enum eEverbloomEvents
{
    EventDreadpetalToxin    = 1,
    EventDancingThorns,
    EventEnragedGrowth,
    EventVileBreath,
    EventEnrage,
    EventBoundingWhirl,
    EventGasp,
    EventVirulendGasp,
    EventChokingVines,
    EventHealingWaters,
    EventInfectedWounds,
    EventNoxiousEruption,
    EventSporeBreath,
    EventToxicBlood,
    EventToxicity,
    EventVenomSpray,
    EventFrostbolt,
    EventFrozenSnap,
    EvertAraceBlast,
    EventDragonsBreath,
    EventFireBall,
    EventTendonRip,
    EventLasherVenom,
    EventArcaneBlast,
    EventArcaneOrb,
    EventFireball,
    EventFlamestike,
    EventIceComet,
    EventPoisonousClaws,
    EventBoundingWhirl2,
    EventBarrageOfLeaves
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
    SpellTendonRip               = 169876,
    SpellLasherVenom             = 173563,
    SpellBlinkMageUponSummon     = 142193,

    SpellLumberingSwipe          = 169929,
    SpellEntanglementVisual      = 169192,
    SpellEntanglementStun        = 169240,
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
    SpellNaturesChanneling       = 164850
};
*/

enum eEverbloomActions
{
    ActionCountPre1StBossKill = 10,
    ActionCounting,
    ActionYalnuEvent,
    ActionBoundingWhirlAura
};

enum eEverbloomAchievements
{
    AchievementTheEverbloomNormal = 9044,
    AchivementTheEverbloomHeroic  = 9053,
    AchivementWeedWhacker         = 9223,
    AchivementWaterManagement     = 9017
};
