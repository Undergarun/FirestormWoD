/// Credit merydwin@gmail.com
#include "SRP6a.hpp"
#include <openssl/sha.h>
#include <algorithm>

namespace BNet2 {

    /// Constructor
    SRP6a::SRP6a(const std::string & p_Salt, const std::string & p_AccountName, const std::string p_PasswordVerifier)
    {
        Sha256(p_AccountName, I);
        ToByteArray(p_Salt, Salt);

        N.SetBinary(BNet2::SRP6a_N, sizeof(BNet2::SRP6a_N));
        G.SetBinary(BNet2::SRP6a_G, sizeof(BNet2::SRP6a_G));

        V = MakeBigNumber(p_PasswordVerifier);

        //////////////////////////////////////////////////////////////////////////

        uint8_t * l_Buffer = new uint8_t[sizeof(BNet2::SRP6a_N) + sizeof(BNet2::SRP6a_G)];

        memcpy(l_Buffer,                            BNet2::SRP6a_N, sizeof(BNet2::SRP6a_N));
        memcpy(l_Buffer + sizeof(BNet2::SRP6a_N),   BNet2::SRP6a_G, sizeof(BNet2::SRP6a_G));

        uint8_t l_KHash[SHA256_DIGEST_LENGTH];

        Sha256(l_Buffer, sizeof(BNet2::SRP6a_N) + sizeof(BNet2::SRP6a_G), l_KHash);
        K.SetBinary(l_KHash, SHA256_DIGEST_LENGTH);

        delete[] l_Buffer;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Compute public b key
    void SRP6a::ComputePublicB()
    {
        uint8_t l_Buffer[4 * SHA256_DIGEST_LENGTH];

        for (uint32_t l_I = 0; l_I < (4 * SHA256_DIGEST_LENGTH); ++l_I)
            l_Buffer[l_I] = rand() % 255;

        PrivateB.SetBinary(l_Buffer, 4 * SHA256_DIGEST_LENGTH);

        BigNumber l_Result = ((K * V) + G.ModExp(PrivateB, N)) % N;
        
        uint8 * l_BinResult = l_Result.AsByteArray(4 * SHA256_DIGEST_LENGTH);

        for (uint32_t l_I = 0; l_I < sizeof(PublicB); ++l_I)
            PublicB[l_I] = l_BinResult[l_I];
    }
    /// Compute U
    void SRP6a::ComputeU(uint8_t * p_A, uint32_t p_ASize)
    {
        A.SetBinary(p_A, p_ASize);

        uint8_t * l_Buffer = new uint8_t[p_ASize + sizeof(PublicB)];

        memcpy(l_Buffer, p_A, p_ASize);
        memcpy(l_Buffer + p_ASize, PublicB, sizeof(PublicB));

        uint8_t l_Hash[SHA256_DIGEST_LENGTH];

        Sha256(l_Buffer, p_ASize + sizeof(PublicB), l_Hash);

        BigNumber l_U;
        l_U.SetBinary(l_Hash, SHA256_DIGEST_LENGTH);

        ComputeS(l_U);

        delete[] l_Buffer;
    }
    /// Compute S
    void SRP6a::ComputeS(BigNumber & p_U)
    {
        S = ((A * V.ModExp(p_U, N)) % N).ModExp(PrivateB, N);

        ComputeSessionKey();
    }
    /// Compute session key
    void SRP6a::ComputeSessionKey()
    {
        uint32_t l_Size = S.GetNumBytes(); 

        uint8 * l_SBytes = S.AsByteArray(l_Size);

        uint8_t * l_Part1 = new uint8_t[l_Size / 2];
        uint8_t * l_Part2 = new uint8_t[l_Size / 2];

        for (uint32_t l_I = 0; l_I < (l_Size / 2); l_I++)
        {
            l_Part1[l_I] = l_SBytes[l_I * 2];
            l_Part2[l_I] = l_SBytes[l_I * 2 + 1];
        }

        uint8_t l_Part1_Hash[SHA256_DIGEST_LENGTH];
        uint8_t l_Part2_Hash[SHA256_DIGEST_LENGTH];

        Sha256(l_Part1, (l_Size / 2), l_Part1_Hash);
        Sha256(l_Part2, (l_Size / 2), l_Part2_Hash);

        delete[] l_Part1;
        delete[] l_Part2;

        uint8_t l_SessionKey[2 * SHA256_DIGEST_LENGTH];

        for (uint32_t l_I = 0; l_I < SHA256_DIGEST_LENGTH; l_I++)
        {
            l_SessionKey[l_I * 2 + 0] = l_Part1_Hash[l_I];
            l_SessionKey[l_I * 2 + 1] = l_Part2_Hash[l_I];
        }

        SessionKey.SetBinary(l_SessionKey, 2 * SHA256_DIGEST_LENGTH);
    }
    /// Compute client M
    void SRP6a::ComputeClientM(uint8_t * p_A, uint32_t p_ASize)
    {
        uint8_t l_N_Hash[SHA256_DIGEST_LENGTH];
        uint8_t l_G_Hash[SHA256_DIGEST_LENGTH];

        Sha256(BNet2::SRP6a_N, sizeof(BNet2::SRP6a_N), l_N_Hash);
        Sha256(BNet2::SRP6a_G, sizeof(BNet2::SRP6a_G), l_G_Hash);

        for (uint32_t l_I = 0; l_I < SHA256_DIGEST_LENGTH; l_I++)
            l_N_Hash[l_I] ^= l_G_Hash[l_I];

        uint32_t l_BufferSize = SHA256_DIGEST_LENGTH + SHA256_DIGEST_LENGTH +  SHA256_DIGEST_LENGTH + p_ASize + (4 * SHA256_DIGEST_LENGTH) + (2 * SHA256_DIGEST_LENGTH);
        uint8_t * l_Buffer = new uint8_t[l_BufferSize];
        uint32_t l_PtrPos = 0;

        
        std::string l_IHexString;
        for (uint32_t l_I = 0; l_I < SHA256_DIGEST_LENGTH; l_I++)
        {
            l_IHexString += "  ";

            sprintf(&(l_IHexString[l_I * 2]), "%02X", (unsigned char)I[l_I]);
        }
        uint8_t l_I_Hash[SHA256_DIGEST_LENGTH];
        Sha256(l_IHexString, l_I_Hash);

        memcpy(l_Buffer + l_PtrPos, l_N_Hash,                                                   SHA256_DIGEST_LENGTH);      l_PtrPos += SHA256_DIGEST_LENGTH;
        memcpy(l_Buffer + l_PtrPos, l_I_Hash,                                                   SHA256_DIGEST_LENGTH);      l_PtrPos += SHA256_DIGEST_LENGTH;
        memcpy(l_Buffer + l_PtrPos, Salt,                                                       SHA256_DIGEST_LENGTH);      l_PtrPos += SHA256_DIGEST_LENGTH;
        memcpy(l_Buffer + l_PtrPos, p_A,                                                        p_ASize);                   l_PtrPos += p_ASize;
        memcpy(l_Buffer + l_PtrPos, PublicB,                                                    4 * SHA256_DIGEST_LENGTH);  l_PtrPos += 4 * SHA256_DIGEST_LENGTH;
        memcpy(l_Buffer + l_PtrPos, SessionKey.AsByteArray(2 * SHA256_DIGEST_LENGTH),           2 * SHA256_DIGEST_LENGTH);  l_PtrPos += 2 * SHA256_DIGEST_LENGTH;

        Sha256(l_Buffer, l_BufferSize, ClientM);

        delete[] l_Buffer;
    }
    /// Compute server M
    void SRP6a::ComputeServerM(uint8_t * p_ClientM, uint32_t p_ClientMSize)
    {
        uint32_t l_Size     = A.GetNumBytes() > 0x80 ? 0x80 : (A.GetNumBytes() != 0 ? A.GetNumBytes() : 1);
        uint8 *  l_ABytes   = A.AsByteArray(0x80);

        uint32_t    l_BufferSize    = l_Size + p_ClientMSize + SessionKey.GetNumBytes();
        uint8_t *   l_Buffer        = new uint8_t[l_BufferSize];
        uint32_t    l_PtrPos        = 0;

        memcpy(l_Buffer + l_PtrPos, l_ABytes,                           l_Size);                    l_PtrPos += l_Size;
        memcpy(l_Buffer + l_PtrPos, p_ClientM,                          p_ClientMSize);             l_PtrPos += p_ClientMSize;
        memcpy(l_Buffer + l_PtrPos, SessionKey.AsByteArray(),           SessionKey.GetNumBytes());  l_PtrPos += SessionKey.GetNumBytes();

        Sha256(l_Buffer, l_BufferSize, ServerM);

        delete[] l_Buffer;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Compare two bytes array
    bool SRP6a::Compare(uint8_t * l_Left, uint8_t * l_Right, uint32_t p_Size)
    {
        for (uint32_t l_I = 0; l_I < p_Size; l_I++)
            if (l_Left[l_I] != l_Right[l_I])
                return false;

        return true;
    }

    //////////////////////////////////////////////////////////////////////////

    /// String to big number
    BigNumber SRP6a::MakeBigNumber(const std::string & p_Str)
    {
        uint8_t * l_Buffer = new uint8_t[(p_Str.size() / 2) + 1];

        ToByteArray(p_Str, l_Buffer);
        l_Buffer[p_Str.size() / 2] = 0;

        BigNumber l_Result;
        l_Result.SetBinary(l_Buffer, (p_Str.size() / 2) + 1);

        delete[] l_Buffer;

        return l_Result;
    }
    /// Str to byte array
    void SRP6a::ToByteArray(const std::string & p_Str, uint8_t * p_Dest)
    {
        char * l_Str = const_cast<char*>(p_Str.c_str());

        for (unsigned int l_I = 0; l_I < p_Str.size(); l_I += 2)
        {
            char l_Buffer[3] = { *(l_Str + l_I), *(l_Str + l_I + 1), '\0' };
            p_Dest[l_I / 2] = strtol(l_Buffer, NULL, 16);
        }
    }
    /// SHA256 of string
    void SRP6a::Sha256(const std::string & p_Str, uint8_t * p_Dest)
    {
        Sha256((uint8_t*)const_cast<char *>(p_Str.c_str()), p_Str.size(), p_Dest);
    }
    /// SHA256 of data
    void SRP6a::Sha256(uint8_t * p_Data, uint32_t p_DataSize, uint8_t * p_Dest)
    {
        memset(p_Dest, 0, SHA256_DIGEST_LENGTH * sizeof(uint8));

        SHA256_CTX l_Context;

        SHA256_Init(&l_Context);
        SHA256_Update(&l_Context, p_Data, p_DataSize);
        SHA256_Final(p_Dest, &l_Context);
    }
}