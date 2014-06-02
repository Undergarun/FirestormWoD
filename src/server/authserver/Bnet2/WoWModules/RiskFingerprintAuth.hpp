/// Credit merydwin@gmail.com
#pragma once

#include "../Module.hpp"

#define WOW_RISKFINGERPRINT_AUTH_MODULE_HASH_WIN32 "5E298E530698AF905E1247E51EF0B109B352AC310CE7802A1F63613DB980ED17"
#define WOW_RISKFINGERPRINT_AUTH_MODULE_HASH_WIN64 "8C43BDA10BE33A32ABBC09FB2279126C7F5953336391276CFF588565332FCD40"
#define WOW_RISKFINGERPRINT_AUTH_MODULE_HASH_MAC64 "1AF5418A448F8AD05451E3F7DBB2D9AF9CB13458EEA2368EBFC539476B954F1C"
#define WOW_RISKFINGERPRINT_AUTH_MODULE_SIZE 0
#define WOW_RISKFINGERPRINT_AUTH_MODULE_ID 0x574F5702

namespace BNet2 { namespace WoWModules {

    /// RiskFingerprint module
    class IRiskFingerprintAuth : public BNet2::Module
    {
        public:
            /// Constructor
            IRiskFingerprintAuth(const std::string & p_Hash, BNet2::Platforms p_Platform);

            /// Get module data
            uint8_t * GetData();
            /// Get data size
            uint32_t GetSize(Session * p_Session);
            /// Write module
            void Write(Session * p_Session, Packet * p_Packet);

    };

    /// RiskFingerprint module
    class RiskFingerprintAuthWin32 : public IRiskFingerprintAuth
    {
        public:
            /// Constructor
            RiskFingerprintAuthWin32();

    };

    /// RiskFingerprint module
    class RiskFingerprintAuthWin64 : public IRiskFingerprintAuth
    {
        public:
            /// Constructor
            RiskFingerprintAuthWin64();

    };

    /// RiskFingerprint module
    class RiskFingerprintAuthMac64 : public IRiskFingerprintAuth
    {
        public:
            /// Constructor
            RiskFingerprintAuthMac64();

    };

}
}
