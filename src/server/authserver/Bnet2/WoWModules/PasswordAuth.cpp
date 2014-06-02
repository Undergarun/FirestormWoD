/// Credit merydwin@gmail.com
#include "PasswordAuth.hpp"
#include "../Session.hpp"

namespace BNet2 { namespace WoWModules {

    /// Constructor
    IPasswordAuth::IPasswordAuth(const std::string & p_Hash, BNet2::Platforms p_Platform)
        : Module(p_Hash, BNet2::BATTLENET2_MODULE_TYPE_AUTH, p_Platform, WOW_PASSWORD_AUTH_MODULE_ID)
    {

    }

    //////////////////////////////////////////////////////////////////////////

    /// Get module data
    uint8_t * IPasswordAuth::GetData()
    {
        return 0;
    }
    /// Get data size
    uint32_t IPasswordAuth::GetSize(Session * p_Session)
    {
        switch (p_Session->GetState())
        {
            case BNet2::BATTLENET2_SESSION_STATE_NONE:
                return 321;

            case BNet2::BATTLENET2_SESSION_STATE_PROOF_VERIFICATION:
                return 161;

        }

        return 0;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Write module
    void IPasswordAuth::Write(Session * p_Session, Packet * p_Packet)
    {
        if (!p_Session || !p_Packet)
            return;

        if (!p_Session->GetSRP())
            return;

        uint8 * l_PrivateBBytes = p_Session->GetSRP()->PrivateB.AsByteArray(4 * SHA256_DIGEST_LENGTH);

        p_Packet->FlushBits();
        p_Packet->WriteBits(p_Session->GetState(), 8);

        switch (p_Session->GetState())
        {
            case BNet2::BATTLENET2_SESSION_STATE_NONE:
                p_Packet->AppendByteArray(p_Session->GetSRP()->I,           SHA256_DIGEST_LENGTH);
                p_Packet->AppendByteArray(p_Session->GetSRP()->Salt,        SHA256_DIGEST_LENGTH);
                p_Packet->AppendByteArray(p_Session->GetSRP()->PublicB, 4 * SHA256_DIGEST_LENGTH);
                p_Packet->AppendByteArray(l_PrivateBBytes,              4 * SHA256_DIGEST_LENGTH);
                break;

            case BNet2::BATTLENET2_SESSION_STATE_PROOF_VERIFICATION:
                p_Packet->AppendByteArray(p_Session->GetSRP()->ServerM,     SHA256_DIGEST_LENGTH);
                p_Packet->AppendByteArray(l_PrivateBBytes,              4 * SHA256_DIGEST_LENGTH);
                break;

        }
    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    PasswordAuthWin32::PasswordAuthWin32()
        : IPasswordAuth(WOW_PASSWORD_AUTH_MODULE_HASH_WIN32, BNet2::BATTLENET2_PLATFORM_WIN)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    PasswordAuthWin64::PasswordAuthWin64()
        : IPasswordAuth(WOW_PASSWORD_AUTH_MODULE_HASH_WIN64, BNet2::BATTLENET2_PLATFORM_WIN64)
    {

    }

    //////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    PasswordAuthMac64::PasswordAuthMac64()
        : IPasswordAuth(WOW_PASSWORD_AUTH_MODULE_HASH_MAC64, BNet2::BATTLENET2_PLATFORM_MAC64)
    {

    }

}
}