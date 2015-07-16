////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#include "ScriptMgr.h"
#include "ScriptedCreature.h"
#include "SpellScript.h"
#include "Vehicle.h"
#include "InstanceScript.h"
#include "GameObjectAI.h"

#ifndef IRON_DOCKS_HPP
# define IRON_DOCKS_HPP

enum eIronDocksDatas
{
    DataMountWolf       = 600,
    DataNokgar          = 0,
    DataGrimrail        = 1,
    DataOshir           = 2,
    DataSkulloc         = 3,
    DataGrimrailMakogg  = 19,
    DataGrimrailNoxx    = 20,
    DataGrimrailDuguru  = 21,
    DataTurret          = 22,
    /// Mini bosses
    DataDaruna          = 4,
    DataGwarnok         = 5,
    DataOlugar          = 6,
    /// Off topic boss
    DataZuggosh         = 7,
    DataKoramar         = 8,
    /// Datas
    DataSecondEvent     = 9,
    DataThirdEvent      = 10
};

enum eIronDocksCreatures
{
    /// Bosses and Etc
    BossFleshrenderNokgar           = 81305,  ///< http://www.wowhead.com/npc=81305/fleshrender-nokgar
    BossDreadfang                   = 81297,  ///< http://www.wowhead.com/npc=81297/dreadfang
    BossOshir                       = 79852,  ///< http://www.wowhead.com/npc=79852/oshir
    BossSkulloc                     = 83612,  ///< http://www.wowhead.com/npc=83612/skulloc
    BossTurret                      = 84215,
    NpcZoggosh                      = 83616,  ///< http://www.wowhead.com/npc=83616/zoggosh
    NpcKoramar                      = 83613,  ///< http://www.wowhead.com/npc=83613/koramar
    NpcMakogg                       = 80805,  ///< http://www.wowhead.com/npc=80805/makogg-emberblade
    NpcNox                          = 80808,  ///< http://www.wowhead.com/npc=80808/neesa-nox
    NpcDuguru                       = 80816,

    /// Boss Adds
    NpcGromkarFlameslinger          = 81279,  ///< http://www.wowhead.com/npc=81279/gromkar-flameslinger
    /// Mini bosses
    NpcChampionDruna                = 81603,  ///< http://www.wowhead.com/npc=81603/champion-druna
    NpcSiegemasterOlugar            = 83026,  ///< http://www.wowhead.com/npc=83026/siegemaster-olugar
    NpcPitwardenGwarnok             = 84520,  ///< http://www.wowhead.com/npc=84520/pitwarden-gwarnok
    /// Trash
    NpcGromkarFootSoldier           = 85997,  ///< http://www.wowhead.com/npc=85997/gromkar-footsoldier
    NpcGromkarDeckhand              = 83762,  ///< http://www.wowhead.com/npc=83762/gromkar-deckhand
    /// Triggers
    TriggerArcheryTarget            = 79423,
    TriggerCannonBarrageTarget      = 99657,
    TriggerCannonBarrageTarget2     = 100981,
    TriggerCannonBarrageTarget3     = 100982,
    TriggerCannonBarrageInBossFight = 354315,
    /// Custom
    NpcGromkarFootSoldier2          = 859977, ///< http://www.wowhead.com/npc=85997/gromkar-footsoldier
    /// MISV
    NpcIronStar                     = 81247
};

enum eIronDocksAchievements
{
    MilitaristicExpansionist    = 9083,
    HeroicIronDocks             = 9047,
    NormalIronDocks             = 9038
};

enum eIronDocksTalks
{
    TalkZoggosh03_2 = 12, ///< Sir.. they're messing with the Iron Stars (44050)
    TalkKoramar010  = 25, ///< Hah! will you look on that, maybe I understimated them.. Zoggosh, prepare Skuloc let them have a taste of what he has to offer. (46901)
    TalkZoggosh04   = 13  ///< Igniting the cannon sir! (44051)
};

enum eIronDocksSpell
{
    SpellCannonBarrageAura = 168537
};

enum eIronDocksScenario
{
    /// Normal/Heroic Mode
    /// ScenarioID
    IronDocksScenarioID     = 685,
    /// Bosses
    IronDocksNokgar         = 25108,
    IronDocksGrimrail       = 25109,
    IronDocksOshir          = 25110,
    IronDocksSkulloc        = 25111,
    /// Challenge Mode
    /// ScenarioID
    IronDocksChallengeID    = 426,
    IronDocksKillCount      = 44,
    IronDocksEnnemies       = 26362
};

enum eIronDocksGameObject
{
    ChallengeModeDoor = 211989
};

#endif ///< IRON_DOCKS_HPP