////////////////////////////////////////////////////////////////////////////////
///
///  MILLENIUM-STUDIO
///  Copyright 2015 Millenium-studio SARL
///  All Rights Reserved.
///
////////////////////////////////////////////////////////////////////////////////

#ifndef BLACKROCK_FOUNDRY_HPP
# define BLACKROCK_FOUNDRY_HPP

# include "InstanceScript.h"
# include "ScriptedCosmeticAI.hpp"
# include "ScriptedCreature.h"
# include "ScriptMgr.h"
# include "ScriptUtils.h"
# include "Cell.h"
# include "CellImpl.h"
# include "GridNotifiers.h"
# include "GridNotifiersImpl.h"
# include "Vehicle.h"
# include "GameObjectAI.h"
# include "Group.h"
# include "MoveSplineInit.h"

static void CastSpellToPlayers(Map* p_Map, Unit* p_Caster, uint32 p_SpellID, bool p_Triggered)
{
    if (p_Map == nullptr)
        return;

    Map::PlayerList const& l_Players = p_Map->GetPlayers();
    for (Map::PlayerList::const_iterator l_Iter = l_Players.begin(); l_Iter != l_Players.end(); ++l_Iter)
    {
        if (Player* l_Player = l_Iter->getSource())
        {
            if (p_Caster != nullptr)
                p_Caster->CastSpell(l_Player, p_SpellID, p_Triggered);
            else
                l_Player->CastSpell(l_Player, p_SpellID, p_Triggered);
        }
    }
}

enum eFoundryCreatures
{
    /// Slagworks - Part 1
    /// Gruul
    BossGruul               = 76877,
    /// Oregorger
    BossOregorger           = 77182,
    /// Blast Furnace
    HeartOfTheMountain      = 76806,
    /// The Black Forge - Part 2
    /// Hans'gar & Franzok
    BossHansgar             = 76973,
    BossFranzok             = 76974,
    /// Flamebender Ka'graz
    BossFlamebenderKagraz   = 76814,
    /// Kromog
    BossKromog              = 77692,
    /// Iron Assembly - Part 3
    /// Beastlord Darmac
    BossBeastlordDarmac     = 76865,
    /// Operator Thogar
    BossOperatorThogar      = 76906,
    /// Iron Maidens
    BossAdmiralGaran        = 77557,
    BossEnforcerSorka       = 77231,
    BossMarakTheBlooded     = 77477,
    /// Blackhand's Crucible - Part 4
    /// Blackhand
    BossBlackhand           = 77325
};

enum eFoundryGameObjects
{
    /// Slagworks - Part 1
    SlagworksDoor               = 238835,
    GruulSpikeDoor              = 230930,
    BKFoundrySpikeTrapGate      = 230931,
    FurnacePortcullis           = 237224,
    BlastFurnaceEncounterDoor   = 230759,
    /// The Black Forge - Part 2
    BlackForgePortcullis        = 238836,
    /// Iron Assembly - Part 3
    IronAssembleyGate           = 238837,
    /// Blackhand's Crucible - Part 4
    SpikeGate                   = 232556,
    CrucibleDoor                = 233006
};

enum eFoundryDatas
{
    /// Bosses
    DataGruul,
    DataOregorger,
    DataBlastFurnace,
    DataHansgarAndFranzok,
    DataFlamebenderKagraz,
    DataKromog,
    DataBeastlordDarmac,
    DataOperatorThogar,
    DataIronMaidens,
    DataBlackhand,
    MaxBossData,

    /// Misc
    PristineTrueIronOres    = 0,

    /// Counters
    MaxPristineTrueIronOres = 3
};

enum eFoundrySpells
{
    Berserker = 26662
};

enum eFoundryAchievements
{
    TheIronPrince = 8978
};

#endif