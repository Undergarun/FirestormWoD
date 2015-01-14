/*
* Copyright (C) 2012-2015 Ashran <http://www.ashran.com>
* Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
* Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef VIGNETTE_H
#define VIGNETTE_H

#include "Common.h"
#include "DB2Stores.h"

class Vignette
{
    public:
        Vignette();
        ~Vignette();

        void Create(uint32 p_VignetteId, uint32 p_MapId, G3D::Vector3 p_Position);
        void UpdatePosition(G3D::Vector3 p_NewPosition);

    private:

        uint64               m_Guid;                    ///< Guid of the current vignette (GUID_TYPE_VIGNETTE)
        uint32 const         m_Map;                     ///< Map Id of the current vignette (we only send vignette which are on the same map the player is)
        VignetteEntry const* m_VignetteEntry;           ///< Vignette data (db2)
        G3D::Vector3         m_Position;                ///< World coordinates of the current vignette, client compute 2D minimap coord itself
};
#endif