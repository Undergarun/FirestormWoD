/// Copyright Ashran 2014-2015

#ifndef BITSET_HPP_UTILITIES
#define BITSET_HPP_UTILITIES

#include "Common.h"

namespace MS { namespace Utilities
{
    /// Bit set manipulation helper
    class BitSet
    {
        public:
            /// Constructor
            BitSet();
            /// Destructor
            ~BitSet();

            /// Set bit set size in octet
            /// @p_Size : New size in octet
            void SetSize(uint32 p_Size);

            /// Enable bit a position
            /// @p_Index : Bit position
            void SetBit(uint32 p_Index);
            /// Disable bit a position
            /// @p_Index : Bit position
            void UnsetBit(uint32 p_Index);

            /// get bit at position
            /// @p_Index : Bit position
            bool GetBit(uint32 p_Index) const;

            /// Append this bit set to a packet
            /// @p_Buffer : Destination buffer
            template<class T> void AppendToByteBuffer(T * p_Buffer)
            {
                p_Buffer->append(m_Bits, m_Size);
            }

        private:
            uint8 * m_Bits; ///< Bits array
            uint32  m_Size; ///< Bit set size

    };

}   ///< namespace Utilities
}   ///< namespace MS

#endif  ///< BITSET_HPP_UTILITIES