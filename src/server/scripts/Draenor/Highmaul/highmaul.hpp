////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCosmeticAI.hpp"
#include "Cell.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"
#include "Vehicle.h"

#ifndef HIGHMAUL_HPP
# define HIGHMAUL_HPP

enum eHighmaulDatas
{
    /// Bosses
    BossKargathBladefist,
    BossTheButcher,
    BossBrackenspore,
    BossTectus,
    BossTwinOgron,
    BossKoragh,
    BossImperatorMargok,
    MaxHighmaulBosses,

    /// Instance datas
    ElevatorActivated   = 0,

    /// Misc values
    RaidGrate001        = 0,
    RaidGrate002,
    RaidGrate003,
    RaidGrate004,
    MaxRaidGrates
};

enum eHighmaulCreatures
{
    /// Walled City
    /// The Coliseum
    KargathBladefist    = 78714,
    JhornTheMad         = 83377,
    ThoktarIronskull    = 83378,
    Vulgor              = 80048,
    BladespireSorcerer  = 80071,
    CrowdAreatrigger    = 79260,
    MargokCosmetic      = 83268,
    /// The Underbelly
    TheButcher          = 77404,
    /// Gorian Strand
    Brackenspore        = 78491
};

enum eHighmaulGameobjects
{
    ArenaElevator   = 233098,
    CollisionWall   = 234299,
    /// Kargath's Doors
    GateArenaExit   = 231781,
    GateArenaInner  = 231780,
    /// The Butcher's Doors
    EarthenPillar   = 239110,
    /// Brackenspore's Doors
    FungalGiantDoor = 239124,
    WindDoor        = 236703,

    /// Kargath Bladefist
    RaidGrate1      = 232368,
    RaidGrate2      = 232369,
    RaidGrate3      = 232370,
    RaidGrate4      = 232371
};

enum eHighmaulWorldStates
{
    IronBomberEnable            = 9722,
    IronBomberRemaining         = 9723,
    DrunkenBileslingerEnable    = 9724,
    DrunkenBileslingerRemaining = 9725
};

#endif