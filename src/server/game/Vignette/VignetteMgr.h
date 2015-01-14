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

#ifndef VIGNETTEMGR_H
#define VIGNETTEMGR_H

#include "Common.h"
#include "Vignette.h"
#include "Player.h"

typedef std::map<uint64, Vignette*> VignetteContainer;
typedef uint64                      VignetteGuid;

class VignetteMgr
{
    public:
        VignetteMgr(Player const* p_Player);
        ~VignetteMgr();

        /**
        * Add the vignette
        * @param Vignette pointer, the vignette to added
        */
        void AddVignette(Vignette const* p_Vignette);

        /**
        * Remove the vignette
        * @param VignetteGuid, the guid of the vignette to remove
        */
        void RemoveVignette(VignetteGuid const p_VignetteLowGuid);

        /**
        * Update the vignette manager, send vignette update to client if needed
        */
        void Update();

    private:
        Player const*                m_Owner;                      ///< Player for who we handle the vignettes
        VignetteContainer            m_Vignettes;                  ///< Contains all the vignette the player can see
        std::queue<VignetteGuid>     m_RemovedVignette;            ///< Contains all the removed vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
        std::queue<VignetteGuid>     m_AddedVignette;              ///< Contains all the added vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
        std::set<VignetteGuid>       m_UpdatedVignette;            ///< Contains all the updated vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
};

#endif