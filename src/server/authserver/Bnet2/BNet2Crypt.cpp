/// Credit merydwin@gmail.com
#include "BNet2Crypt.hpp"
#include "Cryptography/HMACSHA1.h"
#include "Cryptography/BigNumber.h"

namespace BNet2 {

    /// Constructor
    BNet2Crypt::BNet2Crypt()
        : m_ClientDecrypt(SHA256_DIGEST_LENGTH), m_ServerEncrypt(SHA256_DIGEST_LENGTH), m_Initialized(false)
    {

    }

    //////////////////////////////////////////////////////////////////////////

    /// Init
    void BNet2Crypt::Init(BigNumber * p_SessionKey)
    {
        uint8_t ServerEncryptionKey[SEED_KEY_SIZE] = { 0x68, 0xE0, 0xC7, 0x2E, 0xDD, 0xD6, 0xD2, 0xF3, 0x1E, 0x5A, 0xB1, 0x55, 0xB1, 0x8B, 0x63, 0x1E };
        uint8_t ServerDecryptionKey[SEED_KEY_SIZE] = { 0xDE, 0xA9, 0x65, 0xAE, 0x54, 0x3A, 0x1E, 0x93, 0x9E, 0x69, 0x0C, 0xAA, 0x68, 0xDE, 0x78, 0x39 };

        HmacHash256 l_ServerEncryptHmac(2 * SHA256_DIGEST_LENGTH, (uint8_t*)p_SessionKey->AsByteArray(2 * SHA256_DIGEST_LENGTH));
        HmacHash256 l_ClientDecryptHmac(2 * SHA256_DIGEST_LENGTH, (uint8_t*)p_SessionKey->AsByteArray(2 * SHA256_DIGEST_LENGTH));

        uint8_t * l_EncryptHash   = l_ServerEncryptHmac.ComputeHash(ServerEncryptionKey, SEED_KEY_SIZE);
        uint8_t * l_DecryptHash   = l_ClientDecryptHmac.ComputeHash(ServerDecryptionKey, SEED_KEY_SIZE);

        m_ServerEncrypt.Init(l_EncryptHash);
        m_ClientDecrypt.Init(l_DecryptHash);

        m_Initialized = true;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Decrypt data
    void BNet2Crypt::Decrypt(uint8_t * p_Data, size_t p_Size)
    {
        if (!m_Initialized)
            return;

        m_ClientDecrypt.UpdateData(p_Size, p_Data);
    }
    /// Encrypt data
    void BNet2Crypt::Encrypt(uint8_t * p_Data, size_t p_Size)
    {
        if (!m_Initialized)
            return;

        m_ServerEncrypt.UpdateData(p_Size, p_Data);
    }

    //////////////////////////////////////////////////////////////////////////

    /// Is initialized
    bool BNet2Crypt::IsInitialized() const
    {
        return m_Initialized;
    }

}