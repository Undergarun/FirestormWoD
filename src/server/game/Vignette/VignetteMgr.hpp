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
#include "Vignette.hpp"

class Player;
class WorldObject;
class GameObject;
class Creature;

namespace Vignette
{
    typedef std::map<uint64, Vignette::Entity*> VignetteContainer;

    class Manager
    {
        public:

            Manager(Player const* p_Player);
            ~Manager();

            /**
            * Add the vignette
            * @param Vignette pointer, the vignette to added
            */
            void AddVignette(Vignette::Entity* p_Vignette);

            /**
            * Remove the vignettes of the manager and destroy them
            * @param p_Lamba: Function to call for check if the vignette can be remove & destroy
            */
            void DestroyAndRemoveVignettes(std::function < bool(Vignette::Entity* const)> p_Lamba);

            /**
            * Update the vignette manager, send vignette update to client if needed
            */
            void Update();

            /**
            * Send to the client creation, update & remove of vignette
            */
            void SendVignetteUpdateToClient();

            /**
            * Call by Player::UpdateVisibilityOf
            * Hook to handle vignettes linked to WorldObjects
            * @param p_Target: The worldobject who appear
            */
            template<class T> inline void OnWorldObjectAppear(T const* p_Target)
            {
                VignetteEntry const* l_VignetteEntry = GetVignetteEntryFromWorldObject(p_Target);
                if (l_VignetteEntry == nullptr)
                    return;

                auto l_Type = GetDefaultVignetteTypeFromWorldObject(p_Target);
                auto l_TrackingQuest = GetTrackingQuestIdFromWorldObject(p_Target);

                if (l_TrackingQuest)
                {
                    if (m_Owner->GetCompletedQuests().GetBit(GetQuestUniqueBitFlag(l_TrackingQuest) - 1))
                        return;

                    l_Type = GetTrackingVignetteTypeFromWorldObject(p_Target);
                }

                Vignette::Entity* l_Vignette = new Vignette::Entity(l_VignetteEntry, p_Target->GetMapId());
                l_Vignette->Create(l_Type, G3D::Vector3(p_Target->GetPositionX(), p_Target->GetPositionY(), p_Target->GetPositionZ()), p_Target->GetGUID());

                AddVignette(l_Vignette);
            }

            /**
            * Call by Player::UpdateVisibilityOf
            * Hook to handle vignettes linked to WorldObjects
            * @param p_Target: The worldobject who disappear
            */
            template<class T> inline void OnWorldObjectDisappear(T const* p_Target)
            {
                auto l_VignetteEntry = GetVignetteEntryFromWorldObject(p_Target);
                if (l_VignetteEntry == nullptr)
                    return;

                DestroyAndRemoveVignettes([p_Target](Vignette::Entity const* p_Vignette) -> bool
                {
                    if (p_Vignette->GeSourceGuid() == p_Target->GetGUID()
                        && p_Vignette->GetVignetteType() != Vignette::Type::SourceScript)
                        return true;

                    return false;
                });
            }

        private:

            Player const*                m_Owner;                      ///< Player for who we handle the vignettes
            VignetteContainer            m_Vignettes;                  ///< Contains all the vignette the player can see
            std::set<uint64>             m_RemovedVignette;            ///< Contains all the removed vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
            std::set<uint64>             m_AddedVignette;              ///< Contains all the added vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
            std::set<uint64>             m_UpdatedVignette;            ///< Contains all the updated vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
    };
};

#endif