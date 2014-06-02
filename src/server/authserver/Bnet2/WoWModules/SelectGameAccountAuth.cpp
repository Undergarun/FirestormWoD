/// Credit merydwin@gmail.com
#include "SelectGameAccountAuth.hpp"

namespace BNet2 { namespace WoWModules {

    /// Constructor
    ISelectGameAccountAuth::ISelectGameAccountAuth(const std::string & p_Hash, BNet2::Platforms p_Platform)
        : Module(p_Hash, BNet2::BATTLENET2_MODULE_TYPE_AUTH, p_Platform, WOW_SELECTGAMEACCOUNT_AUTH_MODULE_ID)
    {

    }

    //////////////////////////////////////////////////////////////////////////

    /// Get module data
    uint8_t * ISelectGameAccountAuth::GetData()
    {
        return 0;
    }
    /// Get data size
    uint32_t ISelectGameAccountAuth::GetSize(Session * p_Session)
    {
        return WOW_SELECTGAMEACCOUNT_AUTH_MODULE_SIZE;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Write module
    void ISelectGameAccountAuth::Write(Session * p_Session, Packet * p_Packet)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    SelectGameAccountAuthWin32::SelectGameAccountAuthWin32()
        : ISelectGameAccountAuth(WOW_SELECTGAMEACCOUNT_AUTH_MODULE_HASH_WIN32, BNet2::BATTLENET2_PLATFORM_WIN)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    SelectGameAccountAuthWin64::SelectGameAccountAuthWin64()
        : ISelectGameAccountAuth(WOW_SELECTGAMEACCOUNT_AUTH_MODULE_HASH_WIN64, BNet2::BATTLENET2_PLATFORM_WIN64)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    SelectGameAccountAuthMac64::SelectGameAccountAuthMac64()
        : ISelectGameAccountAuth(WOW_SELECTGAMEACCOUNT_AUTH_MODULE_HASH_MAC64, BNet2::BATTLENET2_PLATFORM_MAC64)
    {

    }

}
}