////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#include "GarrisonSiteBase.hpp"
#include "Group.h"

namespace MS { namespace Garrison { namespace Sites
{
    /// Constructor
    /// @p_Map : Target map
    GarrisonSiteBase::GarrisonSiteBase(Map* p_Map)
        : InstanceScript(p_Map), m_OwnerGUID(0)
    {

    }
    /// Destructor
    GarrisonSiteBase::~GarrisonSiteBase()
    {
        if (Player * l_Player = sObjectAccessor->FindPlayer(m_OwnerGUID))
        {
            if (l_Player->GetGarrison())
                l_Player->GetGarrison()->_SetGarrisonScript(nullptr);
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Get UInt32 value by ID
    /// @p_ID : Value ID
    uint32 GarrisonSiteBase::GetData(uint32 p_ID)
    {
        return m_Data32[p_ID];
    }
    /// Set UInt32 value
    /// @p_ID    : Value ID
    /// @p_Value : Value
    void GarrisonSiteBase::SetData(uint32 p_ID, uint32 p_Value)
    {
        m_Data32[p_ID] = p_Value;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Called when a player successfully enters the instance.
    /// @p_Player : Entered player instance
    void GarrisonSiteBase::OnPlayerEnter(Player * p_Player)
    {
        if (!p_Player->IsInWorld())
            return;

        InstanceScript::OnPlayerEnter(p_Player);

        m_Players.emplace(p_Player->GetGUID());

        Group * l_Group = p_Player->GetGroup();
        if (!l_Group || l_Group->IsLeader(p_Player->GetGUID()))
        {
            if (p_Player->GetGarrison())
            {
                p_Player->GetGarrison()->_SetGarrisonScript(this);

                m_OwnerGUID = p_Player->GetGUID();
                OnOwnerEnter(p_Player);
            }
        }
    }

}   ///< namespace Sites
}   ///< namespace Garrison
}   ///< namespace MS
