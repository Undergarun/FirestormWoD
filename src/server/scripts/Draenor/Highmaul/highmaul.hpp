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

#ifndef HIGHMAUL_HPP
# define HIGHMAUL_HPP

enum eHighmaulDatas
{
    BossKargathBladefist,
    BossTheButcher,
    BossBrackenspore,
    BossTectus,
    BossTwinOgron,
    BossKoragh,
    BossImperatorMargok,
    MaxHighmaulBosses,

    ElevatorActivated   = 0
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
    KargathTrigger      = 78846,
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
    WindDoor        = 236703
};

#endif