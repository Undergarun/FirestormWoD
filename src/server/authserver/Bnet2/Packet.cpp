/// Credit merydwin@gmail.com

#include "Packet.hpp"

namespace BNet2 {

    /// Constructor
    Packet::Packet(char * p_Data, uint32_t p_Size)
        : m_BitsCount(0), m_ByteValue(0), m_ReadPosition(0), m_WritePosition(0), m_PreByte(0)
    {
        AppendToStorage(p_Data, p_Size);

        m_Opcode = ReadBits<uint8_t>(6);

        if (ReadBits<bool>(1))
            m_Channel = ReadBits<uint8_t>(4);
        else
            m_Channel = BNet2::BATTLENET2_CHANNEL_NONE;
    }
    /// Constructor
    Packet::Packet(uint32 p_Opcode)
        : m_BitsCount(0), m_ByteValue(0), m_ReadPosition(0), m_WritePosition(0), m_PreByte(0)
    {
        m_Opcode    = OPCODE_ID(p_Opcode);
        m_Channel   = OPCODE_CHANNEL(p_Opcode);

        WriteBits(m_Opcode, 6);
        WriteBits(m_Channel != BNet2::BATTLENET2_CHANNEL_NONE, 1);

        if (m_Channel != BNet2::BATTLENET2_CHANNEL_NONE)
            WriteBits(m_Channel, 4);
    }

    //////////////////////////////////////////////////////////////////////////

    /// Get packet opcode
    uint32_t Packet::GetOpcode()
    {
        return m_Opcode;
    }
    /// Get packet channel
    uint32_t Packet::GetChannel()
    {
        return m_Channel;
    }
    /// Get data
    uint8_t * Packet::GetData()
    {
        return (uint8_t*)&m_Data[0];
    }
    /// Get Size
    uint32_t Packet::GetSize()
    {
        return m_Data.size();
    }

    //////////////////////////////////////////////////////////////////////////

    /// Append data to storage
    void Packet::AppendToStorage(char * p_Buffer, uint32_t p_Size)
    {
        if (m_Data.size() < m_WritePosition + p_Size)
            m_Data.resize(m_WritePosition + p_Size);

        memcpy(&m_Data[m_WritePosition], p_Buffer, p_Size);

        m_WritePosition += p_Size;
    }
    /// Append bytes array
    void Packet::AppendByteArray(uint8_t * p_Data, uint32_t p_Size)
    {
        FlushBits();
        AppendToStorage((char*)p_Data, p_Size);
    }

    //////////////////////////////////////////////////////////////////////////

    /// Flush bits
    void Packet::FlushBits()
    {
        uint32_t l_RemainingBits = 8 - (m_BitsCount & 7);

        if (l_RemainingBits < 8)
            WriteBits<uint32_t>(0, l_RemainingBits);

        m_PreByte = 0;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Read bytes array
    void Packet::ReadBytes(uint8_t * p_Dest, uint32_t p_Count)
    {
        for (uint32_t l_I = 0; l_I < p_Count; ++l_I)
            p_Dest[l_I] = Read<uint8_t>();
    }

    //////////////////////////////////////////////////////////////////////////

    /// Read 4 character
    std::string Packet::ReadFourCC()
    {
        union
        {
            uint32_t Value;
            uint8_t Bytes[4];
        } Converter;

        Converter.Value = ReadBits<uint32_t>(32);

        std::reverse(Converter.Bytes, Converter.Bytes + 4);

        char l_Buffer[5] = { 0, 0, 0, 0, 0 };

        uint32_t l_Y = 0;
        for (uint32 l_I = 0; l_I < 4; l_I++)
            if (Converter.Bytes[l_I])
                l_Buffer[l_Y++] = Converter.Bytes[l_I];

        return std::string(l_Buffer);
    }
    /// Read string
    std::string Packet::ReadString(uint32_t p_Count)
    {
        std::string l_Str;

        for (uint32_t l_I = 0; l_I < p_Count; l_I++)
        {
            char c = Read<char>();
            if (c == 0)
                break;

            l_Str += c;
        }
        return l_Str;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Write 4 character
    void Packet::WriteFourCC(const std::string & p_Str)
    {
        Write<uint8_t>(p_Str[0]);
        Write<uint8_t>(p_Str[1]);
        Write<uint8_t>(p_Str[2]);
        Write<uint8_t>(p_Str[3]);
    }

    /// Write 4 character
    void Packet::WriteFourCC_BattleGroup(const std::string & p_Str)
    {
        Write<uint8_t>(0);
        Write<uint8_t>(0);
        Write<uint8_t>(p_Str[0]);
        Write<uint8_t>(p_Str[1]);
    }
    /// Write string
    void Packet::WriteString(const std::string & p_Str, int p_Bits, bool p_IsCString, int p_AdditionalCount)
    {
        WriteBits(((int)p_Str.size()) + p_AdditionalCount, p_Bits);

        FlushBits();

        for (uint32_t l_I = 0; l_I < p_Str.size(); l_I++)
            Write<uint8_t>(p_Str[l_I]);

        if (p_IsCString)
            Write<uint8_t>(0);

        FlushBits();
    }

}