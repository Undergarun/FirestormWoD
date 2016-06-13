////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef VIGNETTE_MGR_HPP
# define VIGNETTE_MGR_HPP
  
# include "Common.h"
# include "Vignette.hpp"

class WorldObject;
class GameObject;
class Creature;
class Player;

namespace Vignette
{
    using VignetteContainer = std::map<uint64, Vignette::Entity*>;

    class Manager
    {
        public:

            /**
            * Constructor of the vignette manager
            * @param p_Player : Owner of the manager
            */
            Manager(Player const* p_Player);

            /**
            * Destructor of the vignette manager
            */
            ~Manager();

            /**
            * Create the vignette and add it to the manager
            * @param p_VignetteEntry : the db2 entry of the vignette to create
            * @param p_MapId : The map id where the vignette is
            * @param p_VignetteType : Type of the vignette, @see Vignette::Type
            * @param p_Position : Position of the vignette
            * @param p_SourceGuid : Source of the vignette if any (can be gameobject or creature), the vignette will follow the position of the source
            */
            Vignette::Entity* CreateAndAddVignette(VignetteEntry const* p_VignetteEntry, uint32 const p_MapId, Vignette::Type const p_VignetteType, G3D::Vector3 const p_Position, uint64 const p_SourceGuid = 0);

            /*
            * Remove the vignettes of the manager if they are of the same ID as specified, and destroy them
            * @param p_VignetteEntry: DB2 entry of the vignette to remove and destroy
            */
            void DestroyAndRemoveVignetteByEntry(VignetteEntry const* p_VignetteEntry);

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
            * Call by Player::UpdateVisibilityOf
            * Hook to handle vignettes linked to WorldObjects
            * @param p_Target: The worldobject who appear
            */
            template<class T>
            inline void OnWorldObjectAppear(T const* p_Target); ///< Inline function 'Vignette::Manager::OnWorldObjectAppear<WorldObject>' is not defined

            /**
            * Call by Player::UpdateVisibilityOf
            * Hook to handle vignettes linked to WorldObjects
            * @param p_Target: The worldobject who disappear
            */
            template<class T>
            inline void OnWorldObjectDisappear(T const* p_Target); ///< Inline function 'Vignette::Manager::OnWorldObjectDisappear<WorldObject>' is not defined

        private:

            /**
            * Send to the client creation, update & remove of vignette
            */
            void SendVignetteUpdateToClient();

            Player const*                m_Owner;                      ///< Player for who we handle the vignettes
            VignetteContainer            m_Vignettes;                  ///< Contains all the vignette the player can see
            std::set<uint64>             m_RemovedVignette;            ///< Contains all the removed vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
            std::set<uint64>             m_AddedVignette;              ///< Contains all the added vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
            std::set<uint64>             m_UpdatedVignette;            ///< Contains all the updated vignettes to send to client at the next SMSG_VIGNETTE_UPDATE
    };
}

#endif ///< VIGNETTE_MGR_HPP
