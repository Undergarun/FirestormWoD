////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BuildingScripts.hpp"
#include "GarrisonMgr.hpp"
#include "../GarrisonScriptData.hpp"
#include "../Sites/GarrisonSiteBase.hpp"

namespace MS { namespace Garrison
{
    /// Constructor
    GarrisonBuildingAuraPlayerScript::GarrisonBuildingAuraPlayerScript()
        : PlayerScript("GarrisonBuildingAuraPlayerScript")
    {
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    void GarrisonBuildingAuraPlayerScript::OnUpdateZone(Player* p_Player, uint32 /*p_NewZoneId*/, uint32 /*p_OldZoneID*/, uint32 /*p_NewAreaId*/)
    {
        MS::Garrison::Manager* l_GarrisonMgr = p_Player->GetGarrison();

        if (l_GarrisonMgr == nullptr)
            return;

        if (l_GarrisonMgr->HasActiveBuilding(MS::Garrison::Buildings::MageTower_SpiritLodge_Level1) && !p_Player->HasAura(Spells::SpellAuraRunesOfPower))
            p_Player->AddAura(Spells::SpellAuraRunesOfPower, p_Player);
    }
}   ///< namespace Garrison
}   ///< namespace MS