////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PLAYERTAXI_H_INCLUDED
# define PLAYERTAXI_H_INCLUDED

# include "DB2Stores.h"
# include "Define.h"
# include "WorldSession.h"
# include <map>

/// Player taxi manager class
class PlayerTaxi
{
    public:
        /// Constructor
        PlayerTaxi();
        /// Destructor
        ~PlayerTaxi();

        /// Init taxi nodes for level (learn destinations)
        /// @p_Race : Player race
        /// @p_Class : Player class
        /// @p_Level : Player level
        void InitTaxiNodesForLevel(uint32 p_Race, uint32 p_Class, uint8 p_Level);
        /// Load player saved taxi mask
        /// @p_Data : Raw mask data
        void LoadTaxiMask(std::string const& p_Data);

        /// Is a taxi destination known for the player
        /// @p_NodeID : Destination node ID
        bool IsTaximaskNodeKnown(uint32 p_NodeID) const;
        /// Add a taxi destination for the player
        /// @p_NodeID : Destination node ID to add
        bool SetTaximaskNode(uint32 p_NodeID);

        /// Append taxi mask to a byte buffer
        /// @p_DataDest : Destination buffer
        /// @p_All      : Append all possible taxi node
        void AppendTaximaskTo(ByteBuffer & p_DataDest, bool p_All);
        /// Get player taxi mask
        TaxiMask const& GetTaxiMask() const;

        /// Load serialized taxi destinations
        /// @p_Values : Input string
        /// @p_Team   : Player team / faction
        bool LoadTaxiDestinationsFromString(std::string const& p_Values, uint32 p_Team);
        /// Serialize taxi destinations
        std::string SaveTaxiDestinationsToString();

        /// Clear current taxi destinations
        void ClearTaxiDestinations();
        /// Add a destination to the path
        /// @p_Dest : Destination
        void AddTaxiDestination(uint32 p_Dest);
        /// Replace the whole taxi destination
        /// @p_Nodes : New destinations
        void SetTaxiDestination(std::vector<uint32>& p_Nodes);
        /// Get destination origin point
        uint32 GetTaxiSource() const;
        /// Get destination final point
        uint32 GetTaxiDestination() const;
        /// Get current position in the path
        uint32 GetCurrentTaxiPath() const;
        /// Get the next position in the destination
        uint32 NextTaxiDestination();

        /// Request to stop the path a fast a possible
        bool RequestEarlyLanding();

        /// Get destination path
        std::deque<uint32> const& GetPath() const;
        /// Is the current destination empty ?
        bool IsPathEmpty() const;

        /// Serialize operator
        /// @p_Stream     : Destination stream
        /// @p_PlayerTaxi : Player taxi instance
        friend std::ostringstream& operator<< (std::ostringstream& p_Stream, PlayerTaxi const& p_PlayerTaxi);

    private:
        /// Player known taxi mask
        TaxiMask m_TaxiMask;
        /// List of taxi destinations
        std::deque<uint32> m_TaxiDestinations;

};

/// Serialize operator
/// @p_Stream     : Destination stream
/// @p_PlayerTaxi : Player taxi instance
std::ostringstream& operator << (std::ostringstream& p_Stream, PlayerTaxi const& p_PlayerTaxi);

#endif  ///< PLAYERTAXI_H_INCLUDED
