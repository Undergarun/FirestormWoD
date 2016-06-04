////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "BitSet.hpp"

namespace MS { namespace Utilities
{
    /// Constructor
    BitSet::BitSet()
        : m_Bits(nullptr), m_Size(0)
    {

    }
    /// Destructor
    BitSet::~BitSet()
    {
        if (m_Bits)
        {
            delete[] m_Bits;
            m_Bits = nullptr;
        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Set bit set size in octet
    /// @p_Size : New size in octet
    void BitSet::SetSize(uint32 p_Size)
    {
        if (m_Bits)
        {
            delete[] m_Bits;
            m_Bits = nullptr;
        }

        m_Bits = new uint8_t[p_Size];
        memset(m_Bits, 0, p_Size);

        m_Size = p_Size;
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Enable bit a position
    /// @p_Index : Bit position
    void BitSet::SetBit(uint32 p_Index)
    {
        ASSERT(p_Index < (8 * m_Size));
        m_Bits[p_Index / 8] |= (1 << (p_Index % 8));
    }
    /// Disable bit a position
    /// @p_Index : Bit position
    void BitSet::UnsetBit(uint32 p_Index)
    {
        ASSERT(p_Index < (8 * m_Size));
        m_Bits[p_Index / 8] &= 0xFF ^ (1 << (p_Index % 8));
    }

    bool BitSet::GetBit(uint32 p_Index) const
    {
        ASSERT(p_Index < (8 * m_Size));
        return m_Bits[p_Index / 8] & (1 << (p_Index % 8));
    }

}   ///< namespace Utilities
}   ///< namespace MS
