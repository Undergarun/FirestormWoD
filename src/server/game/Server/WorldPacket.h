////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#ifndef WORLDPACKET_H
#define WORLDPACKET_H

#include "Common.h"
#include "Opcodes.h"
#include "ByteBuffer.h"

struct z_stream_s;

extern std::mutex gPacketProfilerMutex;
extern std::map<uint32, uint32> gPacketProfilerData;

class WorldPacket : public ByteBuffer
{
    public:
                                                            // just container for later use
        WorldPacket() : ByteBuffer(0), m_opcode((uint16)UNKNOWN_OPCODE)
        {
        }

        WorldPacket(uint16 opcode, size_t res = 200) : ByteBuffer(res), m_opcode(opcode)
        {
        }
                                                            // copy constructor
        WorldPacket(WorldPacket const& packet) : ByteBuffer(packet), m_opcode(packet.m_opcode)
        {
        }

        void Initialize(uint16 opcode, size_t newres = 200)
        {
            clear();
            _storage.reserve(newres);
            m_opcode = opcode;
            m_BaseSize = newres;
        }

        void OnSend();

        uint16 GetOpcode() const { return m_opcode; }
        void SetOpcode(uint16 opcode) { m_opcode = opcode; }
        void Compress(z_stream_s* compressionStream);
        void Compress(z_stream_s* compressionStream, WorldPacket const* source);

    protected:
        uint16 m_opcode;
        void Compress(void* dst, uint32 *dst_size, const void* src, int src_size);
        z_stream_s* _compressionStream;
};
#endif
