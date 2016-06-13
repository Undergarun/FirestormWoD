#ifndef SPIRES_OR_ARAK_H
#define SPIRES_OR_ARAK_H

namespace SpiresOfArakSpells
{
    enum
    {
        /// World Boss Rukhmar
        SpellLooseQuillsLauncher    = 167647, ///< 30 sec aura to drop quills each 3 sec (spell 167648) + create AT
        SpellLooseQuillsDummy       = 167648, ///< dummy
        SpellLooseQuillsMissile     = 167649, ///< Triggers missile + trigger spell 167650
        SpellLooseQuillsDamage      = 167650, ///< School Damage
        SpellSharpBeak              = 167614, ///< (Charge + damages) + debuff target damage +50% (aura 167615)
        SpellPiercedArmor           = 167615, ///< Debuff damage +50%
        SpellSolarBreath            = 167679, ///< Aura periodic dummy (triggers 167687 periodically)
        SpellSolarBreathDamage      = 167687, ///< Fire damages, TARGET_UNIT_CONE_ENEMY_54
        SpellBlazeOfGloryDummy      = 167629, ///< Fire damages TARGET_DEST_DEST + turn Piles of Ash in the area into phoenixes
        SpellBlazeOfGloryDamage     = 167630,
        SpellBloodFeatherDummy      = 167625, ///< Throws quills which explode, casting 167630 and creating "Fixated Phoenix" (as wowhead said, but named Energized Phoenix)
        SpellBloodFeatherMissile    = 167626,
        SpellSolarRadiationAura     = 167708, ///< timeless aura, triggers 167710 each 15s
        SpellSolarRadiationDamage   = 167710, ///< Fire damages to ennemies in area
        SpellRukhmarBonus           = 178851,
        /// Phoenixes
        SpellFixate                 = 167757,
        SpellFixateSkyReach         = 176544, ///< skyreach spell, may be useful
        SpellSunburst               = 169810, ///< Explodes when killed by players (unused yet, maybe simply related to skyreach)
        SpellSunStrike              = 153828, ///< Explodes when reaches a player (unused yet, maybe simply related to skyreach)
        SouthshoreMobScalingAura    = 169704,

        Rope                        = 171935
    };
}

namespace SpiresOfArakEvents
{
    enum
    {
        EventLooseQuills      = 1,
        EventSharpBeak        = 2,
        EventSolarBreath      = 3,
        EventBlazeOfGlory     = 4,
        EventBloodFeather     = 5,
        EventSolarRadiation   = 6,
        EventMoveToPlayer     = 7,
        EventTurnPhoenixToAsh = 8,
        EventPhoenixFixatePlr = 9,
        EventFlyDestReached   = 10,
        EventFlyOver          = 11
    };
}

/*namespace SpiresOfArakQuests
{
    enum
    {
    };
}*/

namespace SpiresOfArakDatas
{
    enum
    {
        HealthScalingCoeff = 10,
        DamageScalingCoeff = 1
    };
}

namespace SpiresOfArakActions
{
    enum
    {
        ActionMoveDownToward = 1
    };
}

namespace SpiresOfArakCreatures
{
    enum
    {
        CreatureEnergizedPhoenix = 83769,
        CreaturePileOfAsh        = 83768,
        CreatureDepletedPhoenix  = 83783, ///< Seems to be used, but didn't find anything about its abilities, needs reports/precisions
        RavenspeakerInitiate     = 86161,
        PrisonerPost             = 86159,
        IronGrenad               = 86524
    };
}

#endif
