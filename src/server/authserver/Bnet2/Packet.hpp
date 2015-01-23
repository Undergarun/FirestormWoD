/// Credit merydwin@gmail.com
#pragma once

#include <vector>
#include <inttypes.h>

#include "Opcode.hpp"
#include "ByteBuffer.h"

namespace BNet2 {

    /// Battle net 2 packet helper
    class Packet
    {
        public:
            /// Constructor
            Packet(char * p_Data, uint32_t p_Size);
            /// Constructor
            Packet(uint32 p_Opcode);

            /// Get packet opcode
            uint32_t GetOpcode();
            /// Get packet channel
            uint32_t GetChannel();
            /// Get data
            uint8_t * GetData();
            /// Get Size
            uint32_t GetSize();

            /// Append data to storage
            void AppendToStorage(char * p_Buffer, uint32_t p_Size);
            /// Append bytes array
            void AppendByteArray(uint8_t * p_Data, uint32_t p_Size);

            /// Flush bits
            void FlushBits();

            /// Read standard type
            template<typename T> T Read()
            {
                if (m_ReadPosition + sizeof(T) > m_Data.size())
                    throw ByteBufferPositionException(false, m_ReadPosition, sizeof(T), m_Data.size());

                T l_Value = *((T const*)&m_Data[m_ReadPosition]);
                EndianConvert(l_Value);

                m_ReadPosition += sizeof(T);

                return l_Value;
            }
            /// Read bytes array
            void ReadBytes(uint8_t * p_Dest, uint32_t p_Count);

            /// Write data
            template<typename T> void Write(T p_Value)
            {
                EndianConvert(p_Value);
                AppendToStorage((char *)&p_Value, sizeof(T));
            }

            /// Read specified num of bits
            template<typename T> T ReadBits(uint32_t p_Bits)
            {
                uint64_t l_Value = 0;
                uint32_t l_BisToRead = 0;

                while (p_Bits != 0)
                {
                    if ((m_BitsCount % 8) == 0)
                        m_ByteValue = Read<uint8_t>();

                    uint32_t l_ShiftedBits = m_BitsCount & 7;
                    l_BisToRead = 8 - l_ShiftedBits;

                    if (l_BisToRead >= p_Bits)
                        l_BisToRead = p_Bits;

                    p_Bits -= l_BisToRead;

                    l_Value |= (uint64_t)((m_ByteValue >> l_ShiftedBits) & ((uint8_t)(1 << l_BisToRead) - 1)) << p_Bits;
                    m_BitsCount += l_BisToRead;
                }

                return (T)l_Value;
            }
            /// Write specified num of bits
            template<typename T> void WriteBits(T p_Value, uint32_t p_Bits)
            {
                uint32_t l_BitsToWrite = 0;
                uint32_t l_ShiftedBits = 0;

                uint64_t l_Value        = p_Value;
                uint8_t l_PackedByte    = 0;

                while (p_Bits != 0)
                {
                    l_ShiftedBits = m_BitsCount & 7;

                    if (l_ShiftedBits != 0 && m_WritePosition > 0)
                        m_WritePosition -= 1;

                    l_BitsToWrite = 8 - l_ShiftedBits;

                    if (l_BitsToWrite >= p_Bits)
                        l_BitsToWrite = p_Bits;

                    l_PackedByte = (uint8_t)(m_PreByte & ~(uint64_t)(((1 << l_BitsToWrite) - 1) << l_ShiftedBits) | (((l_Value >> (p_Bits - l_BitsToWrite)) & (uint64_t)((1 << l_BitsToWrite) - 1)) << l_ShiftedBits));

                    m_BitsCount += l_BitsToWrite;
                    p_Bits -= l_BitsToWrite;

                    if (l_ShiftedBits != 0)
                        m_PreByte = 0;

                    Write<uint8_t>(l_PackedByte);
                }

                m_PreByte = l_PackedByte;
            }

            /// Read 4 character
            std::string ReadFourCC();
            /// Read string
            std::string ReadString(uint32_t p_Count);

            /// Write 4 character
            void WriteFourCC(const std::string & p_Str);
            /// Write 4 character
            void WriteFourCC_BattleGroup(const std::string & p_Str);
            /// Write string
            void WriteString(const std::string & p_Str, int p_Bits, bool p_IsCString = true, int p_AdditionalCount = 0);


        private:
            std::vector<uint8_t>    m_Data;             ///< Packet Data
            uint32_t                m_Opcode;           ///< Packet message ID
            uint32_t                m_Channel;          ///< Packet channel ID
            uint8_t                 m_ByteValue;        ///< Current byte value for bits reading
            uint8_t                 m_PreByte;          ///< Bits writting buffer
            uint32_t                m_BitsCount;        ///< Num bits readed
            uint32_t                m_ReadPosition;     ///< Data read position
            uint32_t                m_WritePosition;    ///< Data write position


    };

}