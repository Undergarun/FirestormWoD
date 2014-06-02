/// Credit merydwin@gmail.com
#pragma once

#include "../Module.hpp"

#define WOW_THUMBPRINT_AUTH_MODULE_HASH_WIN32 "36B27CD911B33C61730A8B82C8B2495FD16E8024FC3B2DDE08861C77A852941C"
#define WOW_THUMBPRINT_AUTH_MODULE_HASH_WIN64 "C3A1AC0694979E709C3B5486927E558AF1E2BE02CA96E5615C5A65AACC829226"
#define WOW_THUMBPRINT_AUTH_MODULE_HASH_MAC64 "B37136B39ADD83CFDBAFA81857DE3DD8F15B34E0135EC6CD9C3131D3A578D8C2"
#define WOW_THUMBPRINT_AUTH_MODULE_SIZE 512
#define WOW_THUMBPRINT_AUTH_MODULE_ID 0x574F5703

namespace BNet2 { namespace WoWModules {

    /// ThumbprintAuth module
    class IThumbprintAuth : public BNet2::Module
    {
        public:
            /// Constructor
            IThumbprintAuth(const std::string & p_Hash, BNet2::Platforms p_Platform);

            /// Get module data
            uint8_t * GetData();
            /// Get data size
            uint32_t GetSize(Session * p_Session);
            /// Write module
            void Write(Session * p_Session, Packet * p_Packet);

    };

    /// ThumbprintAuth module
    class ThumbprintAuthWin32 : public IThumbprintAuth
    {
        public:
            /// Constructor
            ThumbprintAuthWin32();

    };

    /// ThumbprintAuth module
    class ThumbprintAuthWin64 : public IThumbprintAuth
    {
        public:
            /// Constructor
            ThumbprintAuthWin64();

    };

    /// ThumbprintAuth module
    class ThumbprintAuthMac64: public IThumbprintAuth
    {
        public:
            /// Constructor
            ThumbprintAuthMac64();

    };

}
}
