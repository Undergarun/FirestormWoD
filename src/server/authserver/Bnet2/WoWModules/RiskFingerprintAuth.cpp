/// Credit merydwin@gmail.com
#include "RiskFingerprintAuth.hpp"

namespace BNet2 { namespace WoWModules {

    /// Constructor
    IRiskFingerprintAuth::IRiskFingerprintAuth(const std::string & p_Hash, BNet2::Platforms p_Platform)
        : Module(p_Hash, BNet2::BATTLENET2_MODULE_TYPE_AUTH, p_Platform, WOW_RISKFINGERPRINT_AUTH_MODULE_ID)
    {

    }

    //////////////////////////////////////////////////////////////////////////

    /// Get module data
    uint8_t * IRiskFingerprintAuth::GetData()
    {
        return 0;
    }
    /// Get data size
    uint32_t IRiskFingerprintAuth::GetSize(Session * p_Session)
    {
        return WOW_RISKFINGERPRINT_AUTH_MODULE_SIZE;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Write module
    void IRiskFingerprintAuth::Write(Session * p_Session, Packet * p_Packet)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    RiskFingerprintAuthWin32::RiskFingerprintAuthWin32()
        : IRiskFingerprintAuth(WOW_RISKFINGERPRINT_AUTH_MODULE_HASH_WIN32, BNet2::BATTLENET2_PLATFORM_WIN)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    RiskFingerprintAuthWin64::RiskFingerprintAuthWin64()
        : IRiskFingerprintAuth(WOW_RISKFINGERPRINT_AUTH_MODULE_HASH_WIN64, BNet2::BATTLENET2_PLATFORM_WIN64)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    RiskFingerprintAuthMac64::RiskFingerprintAuthMac64()
        : IRiskFingerprintAuth(WOW_RISKFINGERPRINT_AUTH_MODULE_HASH_MAC64, BNet2::BATTLENET2_PLATFORM_MAC64)
    {

    }

}
}