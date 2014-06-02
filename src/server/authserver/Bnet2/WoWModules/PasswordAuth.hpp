/// Credit merydwin@gmail.com
#pragma once

#include "../Module.hpp"

#define WOW_PASSWORD_AUTH_MODULE_HASH_WIN32 "2E6D53ADAB37A41542B38E01F62CD365EAB8805ED0DE73C307CC6D9D1DFE478C"
#define WOW_PASSWORD_AUTH_MODULE_HASH_WIN64 "851C1D2EF926E9B9A345A460874E65517195129B9E3BDEC7CC77710FA0B1FAD6"
#define WOW_PASSWORD_AUTH_MODULE_HASH_MAC64 "19C91B68752B7826DF498BF73ACA1103C86962A9A55A0A7033E5AD895F4D927C"
#define WOW_PASSWORD_AUTH_MODULE_ID 0x574F5701

namespace BNet2 { namespace WoWModules {

    /// PasswordAuth module
    class IPasswordAuth : public BNet2::Module
    {
        public:
            /// Constructor
            IPasswordAuth(const std::string & p_Hash, BNet2::Platforms p_Platform);

            /// Get module data
            uint8_t * GetData();
            /// Get data size
            uint32_t GetSize(Session * p_Session);
            /// Write module
            void Write(Session * p_Session, Packet * p_Packet);

    };

    /// PasswordAuth module
    class PasswordAuthWin32 : public IPasswordAuth
    {
        public:
            /// Constructor
            PasswordAuthWin32();

    };

    /// PasswordAuth module
    class PasswordAuthWin64 : public IPasswordAuth
    {
        public:
            /// Constructor
            PasswordAuthWin64();

    };

    /// PasswordAuth module
    class PasswordAuthMac64 : public IPasswordAuth
    {
        public:
            /// Constructor
            PasswordAuthMac64();

    };

}
}
