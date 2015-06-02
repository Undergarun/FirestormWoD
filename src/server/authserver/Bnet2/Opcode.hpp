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
        CMSG_REALM_UPDATE           = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x00,  ///< CMSG_LIST_SUBSCRIBE_REQUEST
                                                                                    ///< CMSG_LIST_UNSUBSCRIBE
        SMSG_REALM_AUTH_OK          = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x00,  ///< SMSG_LIST_SUBSCRIBE_RESPONSE
        SMSG_REALM_UPDATE           = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x02,  ///< SMSG_LIST_UPDATE
                                                                                    ///< SMSG_LIST_COMPLETE
        CMSG_JOIN_REQUEST           = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x08,  ///< CMSG_JOIN_REQUEST_V2
        CMSG_MULTI_LOGON_REQUEST_V2 = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x09,  ///< CMSG_MULTI_LOGON_REQUEST_V2
        SMSG_JOIN_RESPONSE          = (BNet2::BATTLENET2_CHANNEL_WOW << 8) | 0x08   ///< SMSG_JOIN_RESPONSE_V2

    };

    #define OPCODE_ID(x)        (x & 0xFF)
    #define OPCODE_CHANNEL(x)   ((x & 0xFF00) >> 8)

}