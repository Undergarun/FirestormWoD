
////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptedCreature.h"

enum eShadowmoonBurialGroundsDatas
{
    DataBossSadana = 1,
    DataBossNhallish,
    DataBossBonemaw,
    DataBossNerzul,
    DataWandererTalk,

    // Doors
    DataStoneDoorSadana,
    DataSadanaFightDoor,
    DataNerzulDoor,

    // Props
    DataNerzulProp,
};

enum eShadowmoonBurialGroundsBosses
{
    BossBoneMaw  = 75452,
    BossNerzul   = 76407,
    BossNhallish = 75829,
    BossSadana   = 75509,
};

enum eShadowmoonBurialGroundsCreatures
{
    CreatureMonstrousCorpseSpider   = 76104,
    CreatureExhumeSpirit            = 75979,
    CreatureDefliedSpirit01         = 75966,
    CreatureDefiledSpirit02         = 75451,
    CreaturePlaguedBat              = 75459,
    CreaturePossessedSoul           = 75899,
    CreatureReinmatedRitualOfBones  = 75715,
    CreatureShadowmoonBoneMender    = 75713,
    CreatureShadowmoonEnslaver      = 76446,
    CreatureShadowmoonExhumer       = 77700,
    CreatureShadowmoonLoyalist      = 75506,
    CreatureSubjugatedSoul          = 76444,
    CreatureVoidSpawn               = 75652,
    CreatureLiberatedSoul           = 76190,
    CreatureCarrionWorm01           = 88769,
    CreatureCarrionWorm02           = 76057,
    CreatureCorpseSkitterling       = 77006,
    CreatureRestlessWanderer        = 75729,
    CreatureCustomizedAltarTrigger  = 852144,
    CreatureCrypts = 75977,
    CreatureDisturbedSoul = 76400,
    CreatureNerzulVisual = 79497,
};

enum eShadowmoonBurialGroundsGameObjects
{
    GameObjectSadanaDoor      = 233919,
    GameObjectSadanaFightDoor = 233921,
    GameObjectNerzulDoor      = 233920,
};

enum eShadowmoonBurialGroundsSpells
{
    //Defiled Spirit: 75966
    SpellDarkCommunionAuraDummy = 153153,
    SpellDarkCommunionTick = 153164,
    SpellChains = 152309,
    SpellVortexVisual = 152311,

    // Monstrous Corpse Spider: 76104
    SpellDeathVenoumDamage = 156717,
    SpellNecroticBurstDamage = 156718,

    // Plagued Bat: 75459
    SpellPlagueSpitDamage = 153524,

    // Possessed Soul: 75899
    SpellReturnedSoulBuff = 153033,
    SpellSoulShredDebuff = 152979,
    SpellUnortodoxExistanceAuraDummy = 152976,

    // Reanimated Ritual Bones: 75715
    SpellVoidCleaveDamage           = 164907,

    // Shadowmoon Bone-mender
    SpellRattlingBones = 152816,
    SpellShadowBolt = 152814,
    SpellShadowMend = 152818,
    SpellShadowWordFrality = 152819,

    // Shadowmoon Ensalve: 76446
    SpellDomination = 154327,
    SpellRendingVoidLash = 156776,

    // Shadowmoon Exhumer: 77700
    SpellExhumeTheCryptsScriptEffect = 153268,
    SpellExhumeTheCryptsSummon = 153275,
    SpellExhumeTheCryptsAura = 153276,
    SpellVoidBolt = 156722,

    // Shadowmoon Loyalist: 75506
    // SpellShadowBolt = 152814,

    // Void Spawn = 75652
    SpellVoidPulseDamageOneK = 151086,
    SpellVoidPulseDamageFiveH = 151254,
    SpellVoidPulseFullDamage = 152964,
    SpellRagingSpiritVisual = 69198,

    // Roleplaying
    SpellKneel = 139276,
    SpellCry = 175902,
    SpellStrangulate = 78037,
    SpellVioletLook = 154002,
    SpellVoidRay = 156920,
};

enum eShadowmoonBurialGroundsEvents
{
    // Monstrous Corpse Spider: 76104
    EventDeathVenom,
    EventNecroticBurst,

    // Plagued Bat: 75459
    EventPlagueSpit,

    // Possessed Soul: 75899
    EventReturnedSoul,
    EventSoulShred,
    EventUnortodoxExistance,

    // Reanimated Ritual Bones: 75715
    EventVoidCleave,

    // Shadowmoon Bone-Mender: 75713
    EventRattlingBones,
    EventFratlity,
    // + loyalist 75506
    EventShadowBolt,
    EventShadowMend,

    // Shadowmoon Ensalve: 76446
    EventDomination,
    EventRendingVoidLash,

    // Shadowmoon Exhumer: 77700
    EventExhumeTheCrypts,
    EventVoidBolt,

    // Void Spawn = 75652
    EventVoidPulse,

    // Roleplaying
    EventWander,
    EventKneel,
    EventCry,

    EventTalk01,
    EventTalk02,
    EventTalk03,
    EventTalk04,
    EventTalk05,
    EventTalk06,
};

enum eShadowmoonBurialGroundsTalks
{
    TalkRestlessWanderer01 = 1, // You walk into a river of souls, Stranger.. it is best to turn back. (42012)
    TalkRestlessWanderer02,     // Eternity and darkness.. Cannot escape.. (42013)
    TalkRestlessWanderer03,     // This was once a place of peace, a place for eternal reset (42014)

    TalkUnknown01 // Nerzul will not hear us.. he hears only darkness now.. (43611)
};

enum eShadowmoonBurialGroundsActions
{
    ActionCountVoidSpawnNearNerzulGate = 1,
};

enum eShadowmoonBurialGroundsAchivement
{
    // Heroic: 
    AchievementShadowmoonBurialGroundsHeroic = 9054,
    AchievementIckyIchors = 9025,
    AchievementSoulsOfTheLosts = 9026,
    AchievementWhatsYourSign = 9018,
    // Normal: 
    AchievementShadowmonBurialGroundsNormal = 9041,
};

