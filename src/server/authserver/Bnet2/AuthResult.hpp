/// Credit merydwin@gmail.com
#pragma once

namespace BNet2 {

    /// Server authentication result
    enum AuthResult
    {
        BATTLENET2_AUTH_OK                                  = 0x00,
        BATTLENET2_AUTH_INVALID_GAME_SERVER                 = 0x67,
        BATTLENET2_AUTH_BAD_INFOS                           = 0x68,
        BATTLENET2_AUTH_INVALID_REGION_OR_NO_GAME_ACCOUNT   = 0x6B,
        BATTLENET2_AUTH_INVALID_PROGRAM                     = 0x6D,
        BATTLENET2_AUTH_INVALID_PLATFORM                    = 0x6E,
        BATTLENET2_AUTH_INVALID_LOCALE                      = 0x6F,
        BATTLENET2_AUTH_INVALID_GAME_VERSION                = 0x70,
        BATTLENET2_AUTH_SERVER_BUSY                         = 0x71,
        BATTLENET2_AUTH_CONNECT_LIMIT_REACHED               = 0x7B,
        BATTLENET2_AUTH_OLD_GRUNT_ACCOUNT                   = 0x7C,
        BATTLENET2_AUTH_CONNECT_METHOD_CHANGED              = 0x8D,
        BATTLENET2_AUTH_ACCOUNT_CLOSED                      = 0xCA,
        BATTLENET2_AUTH_ACCOUNT_TEMP_BANNED                 = 0xCB,
        BATTLENET2_AUTH_ACCOUNT_TEMP_BANNED_2               = 0xCC,
        BATTLENET2_AUTH_ALREADY_LOGGED                      = 0xCD,
        BATTLENET2_AUTH_ACCOUNT_TIME_LIMIT_REACHED          = 0xCE,
    };

}