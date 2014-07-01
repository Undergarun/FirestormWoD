/*
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

#ifndef WORLDPACKET_H
#define WORLDPACKET_H

#include "Common.h"
#include "Opcodes.h"
#include "ByteBuffer.h"


struct Guid
{
    public:

        bool IsValid()
        {
            return (GetHi() >> 58) & 0x3F;
        }

        uint64 GetHi()
        {
            #if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
                return m_Data[1];
            #else
                return m_Data[0];
            #endif
        }
        uint64 GetLow()
        {
            #if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
                return m_Data[0];
            #else
                return m_Data[1];
            #endif
        }

        void Make(uint8 p_Type, uint32 p_RealmID, uint32 p_Entry, uint64 p_ID = 0)
        {
            #if TRINITY_ENDIAN == TRINITY_LITTLEENDIAN
                m_Data[1] = (p_Type << 58) | (p_RealmID << 32) | (p_Entry << 6);
                m_Data[0] = p_ID;
            #else
                m_Data[1] = p_ID;
                m_Data[0] = (p_Type << 58) | (p_RealmID << 32) | (p_Entry << 6);
            #endif
        }

        bool operator==(const Guid & p_Other)
        {
            return m_Data[0] == p_Other.m_Data[0] && m_Data[1] == p_Other.m_Data[1];
        }
        bool operator!=(const Guid & p_Other)
        {
            return m_Data[0] != p_Other.m_Data[0] || m_Data[1] != p_Other.m_Data[1];
        }

    private:
        uint64 m_Data[2];

};


struct z_stream_s;

class WorldPacket : public ByteBuffer
{
    public:
                                                            // just container for later use
        WorldPacket() : ByteBuffer(0), m_opcode(UNKNOWN_OPCODE)
        {
        }

        WorldPacket(Opcodes opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode)
        {
        }
                                                            // copy constructor
        WorldPacket(WorldPacket const& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode)
        {
        }

        void Initialize(Opcodes opcode, size_t newres = 200)
        {
            clear();
            _storage.reserve(newres);
            m_opcode = opcode;
        }

        Opcodes GetOpcode() const { return m_opcode; }
        void SetOpcode(Opcodes opcode) { m_opcode = opcode; }
        void Compress(z_stream_s* compressionStream);
        void Compress(z_stream_s* compressionStream, WorldPacket const* source);

    protected:
        Opcodes m_opcode;
        void Compress(void* dst, uint32 *dst_size, const void* src, int src_size);
        z_stream_s* _compressionStream;
};
#endif
