
/////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
/////////////////////////////////////////////////////////////////////////////////

#include "InstanceScript.h"
#include "ScriptedCosmeticAI.hpp"
#include "ScriptedCreature.h"
#include "ScriptMgr.h"
#include "ScriptUtils.h"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Vehicle.h"
#include "GameObjectAI.h"
#include "Group.h"
#include "MoveSplineInit.h"
#include "CreatureTextMgr.h"

#define InvisibleDisplay 11686
#define FriendlyFaction 35
#define HostileFaction 16
#define AttackableYetNotHostileFaction 7

static void DespawnCreaturesInArea(uint32 p_Entry, WorldObject* p_Object)
{
    std::list<Creature*> l_CreaturesList;
    GetCreatureListWithEntryInGrid(l_CreaturesList, p_Object, p_Entry, 5000.0f);
    if (l_CreaturesList.empty())
        return;

    for (std::list<Creature*>::iterator l_Iter = l_CreaturesList.begin(); l_Iter != l_CreaturesList.end(); ++l_Iter)
        (*l_Iter)->DespawnOrUnsummon();
}

enum eShadowmoonBurialGroundsDatas
{
    DataBossSadana = 1,
    DataSadanaEclipseTrigger,
    DataBossNhallish,
    DataBossBonemaw,
    DataBossNerzul,
    DataWandererFirstTalk,
    DataWandererSecondTalk,
    DataWandererThirdTalk,
    DataWandererTalksValues,
    DataBonemawMouth,
    DataStoneDoorSadana,
    DataSadanaFightDoor,
    DataNerzulDoor,
    DataNhalishDoorEntrance,
    DataNhalishDoorsExit,
    DataBonemawDoorEntrance,
    DataNerzulProp,
    DataPortalToTheShadowlandReturn
};

enum eShadowmoonBurialGroundsBosses
{
    BossBoneMaw  = 75452,
    BossNerzul   = 76407,
    BossNhallish = 75829,
    BossSadana   = 75509
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
    CreatureCrypts                  = 75977,
    CreatureDisturbedSoul           = 76400,
    CreatureNerzulVisual            = 79497,
    CreatureWorldTriggerAltar       = 83816,
    CreatureWorldTriggerSadanaRp    = 838166,
    CreatureWeepingSpirit           = 89003,
    CreatureBonemawMouth            = 81881,
    CreaturePortalToTheShadowland   = 85441,
    CreatureEclipseTrigger          = 76052,
    CreatureNewDarkEclipse          = 760522
};

enum eShadowmoonBurialGroundsGameObjects
{
    GameObjectSadanaDoor           = 233919,
    GameObjectSadanaFightDoor      = 233921,
    GameObjectNerzulDoor           = 233920,
    GameObjectNhalishDoorsExit     = 227851,
    GameObjectNhalishDoorEntrance  = 227852,
    GameObjectBonemawDoorEntrance  = 233990,
    GameObjectPortalToShadowland   = 85541
};

enum eShadowmoonBurialGroundsSpells
{
    /// Defiled Spirit: 75966
    SpellDarkCommunionAuraDummy             = 153153,
    SpellDarkCommunionTick                  = 153164,
    SpellChains                             = 152309,
    SpellVortexVisual                       = 152311,
    /// Monstrous Corpse Spider: 76104
    SpellDeathVenoumDamage                  = 156717,
    SpellNecroticBurstDamage                = 156718,
    /// Plagued Bat: 75459
    SpellPlagueSpitDamage                   = 153524,
    /// Possessed Soul: 75899
    SpellReturnedSoulBuff                   = 153033,
    SpellSoulShredDebuff                    = 152979,
    /// Reanimated Ritual Bones: 75715
    SpellVoidCleaveDamage                   = 164907,
    SpellImprisonNerbySpirits               = 164705,
    /// Shadowmoon Bone-mender
    SpellRattlingBones                      = 152816,
    SpellShadowBolt                         = 152814,
    SpellShadowMend                         = 152818,
    SpellShadowWordFrality                  = 152819,
    /// Shadowmoon Ensalve: 76446
    SpellDomination                         = 154327,
    SpellRendingVoidLash                    = 156776,
    /// Shadowmoon Exhumer: 77700
    SpellExhumeTheCryptsScriptEffect        = 153268,
    SpellExhumeTheCryptsSummon              = 153275,
    SpellExhumeTheCryptsAura                = 153276,
    SpellVoidBolt                           = 156722,
    /// Shadowmoon Loyalist: 75506
    /// SpellShadowBolt = 152814,
    SpellPrisonAura                         = 152309,
    /// Void Spawn = 75652
    SpellVoidPulseDamage                    = 151086,
    SpellVoidPulseDamageFive                = 151254,
    SpellVoidPulseFullDamage                = 152964,
    SpellRagingSpiritVisual                 = 69198,
    /// Roleplaying
    SpellKneel                              = 139276,
    SpellCry                                = 175902,
    SpellStrangulate                        = 78037,
    SpellVioletLook                         = 154002,
    SpellVoidRay                            = 156920,
    SpellShadowRitualBonesAuraCosmetic      = 160530,
    SpellGhostlyVisual                      = 144465,
    /// Water Trigger
    SpellWaterSprout                        = 156682
};

enum eShadowmoonBurialGroundsEvents
{
    /// Monstrous Corpse Spider: 76104
    EventDeathVenom = 1,
    EventNecroticBurst,
    EventNecroticBurstExplosion,
    /// Plagued Bat: 75459
    EventPlagueSpit,
    /// Possessed Soul: 75899
    EventReturnedSoul,
    EventSoulShred,
    EventUnortodoxExistance,
    /// Reanimated Ritual Bones: 75715
    EventVoidCleave,
    /// Shadowmoon Bone-Mender: 75713
    EventRattlingBones,
    EventFratlity,
    /// + loyalist 75506
    EventShadowBolt,
    EventShadowMend,
    /// Shadowmoon Ensalve: 76446
    EventDomination,
    EventRendingVoidLash,
    /// Shadowmoon Exhumer: 77700
    EventExhumeTheCrypts,
    EventVoidBolt,
    /// Void Spawn = 75652
    EventVoidPulse,
    EventSizeGrowth,
    /// Roleplaying
    EventWander,
    EventKneel,
    EventCry,
    EventTalk01,
    EventTalk02,
    EventTalk03,
    EventTalk04,
    EventTalk05,
    EventTalk06
};

enum eShadowmoonBurialGroundsTalks
{
    TalkRestlessWanderer01 = 1, ///< You walk into a river of souls, Stranger.. it is best to turn back. (42012)
    TalkRestlessWanderer02,     ///< Eternity and darkness.. Cannot escape.. (42013)
    TalkRestlessWanderer03,     ///< This was once a place of peace, a place for eternal reset (42014)
    TalkUnknown01               ///< Nerzul will not hear us.. he hears only darkness now.. (43611)
};

enum eShadowmoonBurialGroundsActions
{
    ActionCountVoidSpawnNearNerzulGate = 1,
    ActionCrypts,
    ActionSizeGrowth,
    ActionChooseTalker
};

enum eShadowmoonBurialGroundsAchivement
{
    /// Heroic:
    AchievementShadowmoonBurialGroundsHeroic = 9054,
    AchievementIckyIchors                    = 9025,
    AchievementSoulsOfTheLosts               = 9026,
    AchievementWhatsYourSign                 = 9018,
    /// Normal:
    AchievementShadowmonBurialGroundsNormal  = 9041
};

