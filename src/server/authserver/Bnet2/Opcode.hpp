/// Credit merydwin@gmail.com
#pragma once

#include "Channel.hpp"

namespace BNet2 {

    /// Packets opcode
    enum Opcodes
    {
        /// Base bnet2 authentication opcodes
        CMSG_INFORMATION_REQUEST    = (BNet2::BATTLENET2_CHANNEL_NONE << 8) | 0x09,
        SMSG_PROOF_REQUEST          = (BNet2::BATTLENET2_CHANNEL_NONE << 8) | 0x02,
        CMSG_PROOF_RESPONSE         = (BNet2::BATTLENET2_CHANNEL_NONE << 8) | 0x02,
        SMSG_AUTH_COMPLETE          = (BNet2::BATTLENET2_CHANNEL_NONE << 8) | 0x00,

        /// Protocol misc
        CMSG_PING                   = (BNet2::BATTLENET2_CHANNEL_CREEP << 8) | 0x00,
        SMSG_PONG                   = (BNet2::BATTLENET2_CHANNEL_CREEP << 8) | 0x00,
        CMSG_DISCONNECT             = (BNet2::BATTLENET2_CHANNEL_CREEP << 8) | 0x06,

        /// WoW related opcodes
        CMSG_REALM_UPDATE           = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x00,
        SMSG_REALM_AUTH_OK          = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x00,
        SMSG_REALM_UPDATE           = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x02,
        CMSG_JOIN_REQUEST           = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x08,
        SMSG_JOIN_RESPONSE          = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x08

    };

    #define OPCODE_ID(x)        (x & 0xFF)
    #define OPCODE_CHANNEL(x)   ((x & 0xFF00) >> 8)

}