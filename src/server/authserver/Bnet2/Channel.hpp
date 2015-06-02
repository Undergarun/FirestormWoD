/// Credit merydwin@gmail.com
#pragma once

namespace BNet2 {

    /// Packets channel
    enum Channels
    {
        BATTLENET2_CHANNEL_AUTHENTICATION   = 0x00,
        BATTLENET2_CHANNEL_CONNECTION       = 0x01,
        BATTLENET2_CHANNEL_WOWREALM         = 0x02,
        BATTLENET2_CHANNEL_FRIENDS          = 0x03,
        BATTLENET2_CHANNEL_PRESENCE         = 0x04,
        BATTLENET2_CHANNEL_CHAT             = 0x05,
        BATTLENET2_CHANNEL_SUPPORT          = 0x07,
        BATTLENET2_CHANNEL_ACHIEVEMENT      = 0x08,
        BATTLENET2_CHANNEL_CACHE            = 0x0B,
        BATTLENET2_CHANNEL_PROFILE          = 0x0E
    };

}