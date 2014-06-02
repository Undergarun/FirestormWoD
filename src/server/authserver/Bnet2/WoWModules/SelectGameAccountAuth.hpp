/// Credit merydwin@gmail.com
#pragma once

#include "../Module.hpp"

#define WOW_SELECTGAMEACCOUNT_AUTH_MODULE_HASH_WIN32 "ABC6BB719A73EC1055296001910E26AFA561F701AD9995B1ECD7F55F9D3CA37C"
#define WOW_SELECTGAMEACCOUNT_AUTH_MODULE_HASH_WIN64 "894D25D3219D97D085EA5A8B98E66DF5BD9F460EC6F104455246A12B8921409D"
#define WOW_SELECTGAMEACCOUNT_AUTH_MODULE_HASH_MAC64 "52E2978DB6468DFADE7C61DA89513F443C9225692B5085FBE956749870993703"
#define WOW_SELECTGAMEACCOUNT_AUTH_MODULE_SIZE 0
#define WOW_SELECTGAMEACCOUNT_AUTH_MODULE_ID 0x574F5704

namespace BNet2 { namespace WoWModules {

    /// SelectGameAccount module
    class ISelectGameAccountAuth : public BNet2::Module
    {
        public:
            /// Constructor
            ISelectGameAccountAuth(const std::string & p_Hash, BNet2::Platforms p_Platform);

            /// Get module data
            uint8_t * GetData();
            /// Get data size
            uint32_t GetSize(Session * p_Session);
            /// Write module
            void Write(Session * p_Session, Packet * p_Packet);

    };

    /// RiskFingerprint module
    class SelectGameAccountAuthWin32 : public ISelectGameAccountAuth
    {
        public:
            /// Constructor
            SelectGameAccountAuthWin32();

    };

    /// RiskFingerprint module
    class SelectGameAccountAuthWin64 : public ISelectGameAccountAuth
    {
        public:
            /// Constructor
            SelectGameAccountAuthWin64();

    };

    /// RiskFingerprint module
    class SelectGameAccountAuthMac64 : public ISelectGameAccountAuth
    {
        public:
            /// Constructor
            SelectGameAccountAuthMac64();

    };

}
}
