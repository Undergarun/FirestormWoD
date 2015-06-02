/// Credit merydwin@gmail.com
#pragma once

#include "Channel.hpp"

namespace BNet2 {

    /// Packets opcode
    enum Opcodes
    {
        /// BATTLENET2_CHANNEL_AUTHENTICATION
        CMSG_LOGON_REQUEST                              = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x00,   ///< Depreciated
        CMSG_RESUME_REQUEST                             = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x01,
        CMSG_PROOF_RESPONSE                             = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x02,
        CMSG_GENERATE_SINGLE_SIGN_ON_TOKEN_REQUEST_2    = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x08,   ///< Not implemented
        CMSG_LOGON_REQUEST_3                            = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x09,
        CMSG_SINGLE_SIGN_ON_REQUEST_3                   = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x0A,   ///< Not implemented

        SMSG_LOGON_RESPONSE                             = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x00,
        SMSG_RESUME_RESPONSE                            = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x01,   ///< Not implemented
        SMSG_PROOF_REQUEST                              = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x02,
        SMSG_PATCH                                      = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x03,   ///< Not implemented
        SMSG_AUTHORIZED_LICENSES                        = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x04,   ///< Not implemented
        SMSG_GENERATE_SINGLE_SIGN_ON_TOKEN_REQUEST_2    = (BNet2::BATTLENET2_CHANNEL_AUTHENTICATION << 8) | 0x08,   ///< Not implemented

        /// BATTLENET2_CHANNEL_CONNECTION
        CMSG_PING                                       = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x00,
        CMSG_ENABLE_ENCRYPTION                          = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x05,
        CMSG_LOGOUT_REQUEST                             = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x06,
        CMSG_DISCONNECT_REQUEST                         = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x07,       ///< Not implemented
        CMSG_CONNECTION_CLOSING                         = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x09,

        SMSG_PONG                                       = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x00,
        SMSG_BOOM                                       = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x01,       ///< Not implemented
        SMSG_REGULATOR_UPDATE                           = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x02,       ///< Not implemented
        SMSG_SERVER_VERSION                             = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x03,       ///< Not implemented
        SMSG_STUN_SERVERS                               = (BNet2::BATTLENET2_CHANNEL_CONNECTION << 8) | 0x04,       ///< Not implemented

        /// BATTLENET2_CHANNEL_WOWREALM
        CMSG_LIST_SUBSCRIBE_REQUEST                     = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x00,
        CMSG_LIST_UNSUBSCRIBE                           = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x01,
        CMSG_JOIN_REQUEST_V2                            = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x08,
        CMSG_MULTI_LOGON_REQUEST_V2                     = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x09,         ///< Not implemented

        SMSG_LIST_SUBSCRIBE_RESPONSE                    = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x00,
        SMSG_LIST_UPDATE                                = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x02,
        SMSG_LIST_COMPLETE                              = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x03,
        SMSG_TOON_READY                                 = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x06,         ///< Not implemented
        SMSG_TOON_LOGGED_OUT                            = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x07,         ///< Not implemented
        SMSG_JOIN_RESPONSE                              = (BNet2::BATTLENET2_CHANNEL_WOWREALM << 8) | 0x08,

        /// BATTLENET2_CHANNEL_FRIENDS

        /// BATTLENET2_CHANNEL_PRESENCE

        /// BATTLENET2_CHANNEL_CHAT

        /// BATTLENET2_CHANNEL_SUPPORT

        /// BATTLENET2_CHANNEL_ACHIEVEMENT

        /// BATTLENET2_CHANNEL_CACHE
        CMSG_GATEWAY_LOOKUP_REQUEST                     = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x02,            ///< Not implemented
        CMSG_CONNECT_REQUEST                            = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x04,            ///< Not implemented
        CMSG_DATA_CHUNK                                 = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x07,            ///< Not implemented
        CMSG_GET_STREAM_ITEMS_REQUEST                   = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x09,

        SMSG_GATEWAY_LOOKUP_RESPONSE                    = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x03,            ///< Not implemented
        SMSG_CONNECT_RESPONSE                           = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x04,            ///< Not implemented
        SMSG_PUBLISH_LIST_RESPONSE                      = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x07,            ///< Not implemented
        SMSG_RESULT                                     = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x08,            ///< Not implemented
        SMSG_GET_STREAM_ITEMS_RESPONSE                  = (BNet2::BATTLENET2_CHANNEL_CACHE << 8) | 0x09










        /// BATTLENET2_CHANNEL_PROFILE

    };

    #define OPCODE_ID(x)        (x & 0xFF)
    #define OPCODE_CHANNEL(x)   ((x & 0xFF00) >> 8)

}