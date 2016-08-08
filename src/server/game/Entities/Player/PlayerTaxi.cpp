////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Player.h"
#include "ObjectMgr.h"
#include "Common.h"

/// String to unsigned long
/// @p_Str : String input
inline unsigned long atoul(char const* str)
{
    return strtoul(str, nullptr, 10);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Constructor
PlayerTaxi::PlayerTaxi()
{
    m_TaxiMask.fill(0);
}
/// Destructor
PlayerTaxi::~PlayerTaxi()
{

}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Init taxi nodes for level (learn destinations)
/// @p_Race : Player race
/// @p_Class : Player class
/// @p_Level : Player level
void PlayerTaxi::InitTaxiNodesForLevel(uint32 p_Race, uint32 p_Class, uint8 p_Level)
{
    /// Class specific initial known nodes
    TaxiMask const& l_FactionMask = Player::TeamForRace(p_Race) == HORDE ? sHordeTaxiNodesMask : sAllianceTaxiNodesMask;

    switch (p_Class)
    {
        case CLASS_DEATH_KNIGHT:
        {
            /// @TODO this code is from trinity and it's bugged, he learn all continent taxi masters
            ///for (uint8 i = 0; i < TaxiMaskSize; ++i)
            ///    m_taximask[i] |= sOldContinentsNodesMask[i] & factionMask[i];

            SetTaximaskNode(315);   ///< Archerus
            break;
        }
    }

    /// Race specific initial known nodes: capital and taxi hub masks
    switch (p_Race)
    {
        case RACE_HUMAN:
        case RACE_DWARF:
        case RACE_NIGHTELF:
        case RACE_GNOME:
        case RACE_DRAENEI:
        case RACE_WORGEN:
        case RACE_PANDAREN_ALLI:
            SetTaximaskNode(2);     ///< Stormwind, Elwynn
            SetTaximaskNode(6);     ///< Ironforge, Dun Morogh
            SetTaximaskNode(26);    ///< Lor'danel, Darkshore
            SetTaximaskNode(27);    ///< Rut'theran Village, Teldrassil
            SetTaximaskNode(49);    ///< Moonglade (Alliance)
            SetTaximaskNode(94);    ///< The Exodar
            SetTaximaskNode(456);   ///< Dolanaar, Teldrassil
            SetTaximaskNode(457);   ///< Darnassus, Teldrassil
            SetTaximaskNode(582);   ///< Goldshire, Elwynn
            SetTaximaskNode(589);   ///< Eastvale Logging Camp, Elwynn
            SetTaximaskNode(619);   ///< Kharanos, Dun Morogh
            SetTaximaskNode(620);   ///< Gol'Bolar Quarry, Dun Morogh
            SetTaximaskNode(624);   ///< Azure Watch, Azuremyst Isle
            break;

        case RACE_ORC:
        case RACE_UNDEAD_PLAYER:
        case RACE_TAUREN:
        case RACE_TROLL:
        case RACE_BLOODELF:
        case RACE_GOBLIN:
        case RACE_PANDAREN_HORDE:
            SetTaximaskNode(11);    ///< Undercity, Tirisfal
            SetTaximaskNode(22);    ///< Thunder Bluff, Mulgore
            SetTaximaskNode(23);    ///< Orgrimmar, Durotar
            SetTaximaskNode(69);    ///< Moonglade (Horde)
            SetTaximaskNode(82);    ///< Silvermoon City
            SetTaximaskNode(384);   ///< The Bulwark, Tirisfal
            SetTaximaskNode(402);   ///< Bloodhoof Village, Mulgore
            SetTaximaskNode(460);   ///< Brill, Tirisfal Glades
            SetTaximaskNode(536);   ///< Sen'jin Village, Durotar
            SetTaximaskNode(537);   ///< Razor Hill, Durotar
            SetTaximaskNode(625);   ///< Fairbreeze Village, Eversong Woods
            SetTaximaskNode(631);   ///< Falconwing Square, Eversong Woods
            break;
    }

    /// BlastedLands continent starting masks (It will be accessible only at new map)
    switch (Player::TeamForRace(p_Race))
    {
        case ALLIANCE:
            SetTaximaskNode(100);
            break;
        case HORDE:
            SetTaximaskNode(99);
            break;
    }

    /// Level dependent taxi hubs
    if (p_Level >= 68)
        SetTaximaskNode(213);       ///< Shattered Sun Staging Area
}

/// Load player saved taxi mask
/// @p_Data : Raw mask data
void PlayerTaxi::LoadTaxiMask(std::string const &p_Data)
{
    Tokenizer l_Tokens(p_Data, ' ');

    uint8 l_Index = 0;
    for (Tokenizer::const_iterator l_It = l_Tokens.begin(); l_Index < TaxiMaskSize && l_It != l_Tokens.end(); ++l_It, ++l_Index)
    {
        /// Load and set bits only for existing taxi nodes
        m_TaxiMask[l_Index] = sTaxiNodesMask[l_Index] & atoul(*l_It);
    }
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Is a taxi destination known for the player
/// @p_NodeID : Destination node ID
bool PlayerTaxi::IsTaximaskNodeKnown(uint32 p_NodeID) const
{
    uint8  l_Field      = uint8((p_NodeID - 1) / 8);
    uint32 l_SubMask    = 1 << ((p_NodeID - 1) % 8);

    return (m_TaxiMask[l_Field] & l_SubMask) == l_SubMask;
}

/// Add a taxi destination for the player
/// @p_NodeID : Destination node ID to add
bool PlayerTaxi::SetTaximaskNode(uint32 p_NodeID)
{
    uint8  l_Field = uint8((p_NodeID - 1) / 8);
    uint32 l_SubMask = 1 << ((p_NodeID - 1) % 8);

    if ((m_TaxiMask[l_Field] & l_SubMask) != l_SubMask)
    {
        m_TaxiMask[l_Field] |= l_SubMask;
        return true;
    }
    else
        return false;
}

bool PlayerTaxi::RemoveTaximaskNode(uint32 p_NodeID)
{
    uint8  l_Field = uint8((p_NodeID - 1) / 8);
    uint32 l_SubMask = 1 << ((p_NodeID - 1) % 8);

    if (m_TaxiMask[l_Field] & l_SubMask)
    {
        m_TaxiMask[l_Field] &= ~l_SubMask;
        return true;
    }
    else
        return false;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Append taxi mask to a byte buffer
/// @p_DataDest : Destination buffer
/// @p_All      : Append all possible taxi node
void PlayerTaxi::AppendTaximaskTo(ByteBuffer & p_DataDest, bool p_All)
{
    if (p_All)
    {
        for (uint8 l_I = 0; l_I < TaxiMaskSize; ++l_I)
            p_DataDest << uint8(sTaxiNodesMask[l_I]);
    }
    else    ///< All existed nodes
    {
        for (uint8 l_I = 0; l_I < TaxiMaskSize; ++l_I)
            p_DataDest << uint8(m_TaxiMask[l_I]);
    }
}

/// Get player taxi mask
TaxiMask const& PlayerTaxi::GetTaxiMask() const
{
    return m_TaxiMask;
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Load serialized taxi destinations
/// @p_Values : Input string
/// @p_Team   : Player team / faction
bool PlayerTaxi::LoadTaxiDestinationsFromString(std::string const& p_Values, uint32 p_Team)
{
    ClearTaxiDestinations();

    Tokenizer l_Tokens(p_Values, ' ');

    for (Tokenizer::const_iterator l_Iter = l_Tokens.begin(); l_Iter != l_Tokens.end(); ++l_Iter)
    {
        uint32 l_Node = atoul(*l_Iter);
        AddTaxiDestination(l_Node);
    }

    if (m_TaxiDestinations.empty())
        return true;

    /// Check integrity
    if (m_TaxiDestinations.size() < 2)
        return false;

    for (size_t l_I = 1; l_I < m_TaxiDestinations.size(); ++l_I)
    {
        uint32 l_Cost;
        uint32 l_Path;

        sObjectMgr->GetTaxiPath(m_TaxiDestinations[l_I - 1], m_TaxiDestinations[l_I], l_Path, l_Cost);

        if (!l_Path)
            return false;
    }

    /// Can't load taxi path without mount set (quest taxi path?)
    if (!sObjectMgr->GetTaxiMountDisplayId(GetTaxiSource(), p_Team, true))
        return false;

    return true;
}

/// Serialize taxi destinations
std::string PlayerTaxi::SaveTaxiDestinationsToString()
{
    if (m_TaxiDestinations.empty())
        return "";

    std::ostringstream l_Stream;

    for (size_t l_I = 0; l_I < m_TaxiDestinations.size(); ++l_I)
        l_Stream << m_TaxiDestinations[l_I] << ' ';

    return l_Stream.str();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Clear current taxi destinations
void PlayerTaxi::ClearTaxiDestinations()
{
    m_TaxiDestinations.clear();
}

/// Add a destination to the path
/// @p_Dest : Destination
void PlayerTaxi::AddTaxiDestination(uint32 p_Dest)
{
    m_TaxiDestinations.push_back(p_Dest);
}

/// Replace the whole taxi destination
/// @p_Nodes : New destinations
void PlayerTaxi::SetTaxiDestination(std::vector<uint32>& p_Nodes)
{
    m_TaxiDestinations.clear();
    m_TaxiDestinations.insert(m_TaxiDestinations.begin(), p_Nodes.begin(), p_Nodes.end());
}

/// Get destination origin point
uint32 PlayerTaxi::GetTaxiSource() const
{
    return m_TaxiDestinations.empty() ? 0 : m_TaxiDestinations.front();
}

/// Get destination final point
uint32 PlayerTaxi::GetTaxiDestination() const
{
    return m_TaxiDestinations.size() < 2 ? 0 : m_TaxiDestinations[1];
}

/// Get current position in the path
uint32 PlayerTaxi::GetCurrentTaxiPath() const
{
    if (m_TaxiDestinations.size() < 2)
        return 0;

    uint32 l_Path;
    uint32 l_Cost;

    sObjectMgr->GetTaxiPath(m_TaxiDestinations[0], m_TaxiDestinations[1], l_Path, l_Cost);

    return l_Path;
}

/// Get the next position in the destination
uint32 PlayerTaxi::NextTaxiDestination()
{
    m_TaxiDestinations.pop_front();
    return GetTaxiDestination();
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

/// Request to stop the path a fast a possible
bool PlayerTaxi::RequestEarlyLanding()
{
    if (m_TaxiDestinations.size() <= 2)
        return false;

    /// Start from first destination - m_TaxiDestinations[0] is the current starting node
    for (std::deque<uint32>::iterator l_It = ++m_TaxiDestinations.begin(); l_It != m_TaxiDestinations.end(); ++l_It)
    {
        if (IsTaximaskNodeKnown(*l_It))
        {
            if (++l_It == m_TaxiDestinations.end())
                return false;   ///< If we are left with only 1 known node on the path don't change the spline, its our final destination anyway

            m_TaxiDestinations.erase(l_It, m_TaxiDestinations.end());
            return true;
        }
    }

    return false;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Get destination path
std::deque<uint32> const& PlayerTaxi::GetPath() const
{
    return m_TaxiDestinations;
}

/// Is the current destination empty ?
bool PlayerTaxi::IsPathEmpty() const
{
    return m_TaxiDestinations.empty();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

/// Serialize operator
/// @p_Stream     : Destination stream
/// @p_PlayerTaxi : Player taxi instance
std::ostringstream& operator<< (std::ostringstream& p_Stream, PlayerTaxi const& p_PlayerTaxi)
{
    for (uint8 l_I = 0; l_I < TaxiMaskSize; ++l_I)
        p_Stream << uint32(p_PlayerTaxi.m_TaxiMask[l_I]) << ' ';

    return p_Stream;
}
