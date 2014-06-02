/// Credit merydwin@gmail.com
#pragma once

#include "Cryptography/BigNumber.h"
#include "Cryptography/SHA1.h"
#include <inttypes.h>
#include <string>

namespace BNet2 {

    /// N modulus
    static uint8_t SRP6a_N[128] = {
        0xAB, 0x24, 0x43, 0x63, 0xA9, 0xC2, 0xA6, 0xC3, 0x3B, 0x37, 0xE4, 0x61, 0x84, 0x25, 0x9F, 0x8B,
        0x3F, 0xCB, 0x8A, 0x85, 0x27, 0xFC, 0x3D, 0x87, 0xBE, 0xA0, 0x54, 0xD2, 0x38, 0x5D, 0x12, 0xB7,
        0x61, 0x44, 0x2E, 0x83, 0xFA, 0xC2, 0x21, 0xD9, 0x10, 0x9F, 0xC1, 0x9F, 0xEA, 0x50, 0xE3, 0x09,
        0xA6, 0xE5, 0x5E, 0x23, 0xA7, 0x77, 0xEB, 0x00, 0xC7, 0xBA, 0xBF, 0xF8, 0x55, 0x8A, 0x0E, 0x80,
        0x2B, 0x14, 0x1A, 0xA2, 0xD4, 0x43, 0xA9, 0xD4, 0xAF, 0xAD, 0xB5, 0xE1, 0xF5, 0xAC, 0xA6, 0x13,
        0x1C, 0x69, 0x78, 0x64, 0x0B, 0x7B, 0xAF, 0x9C, 0xC5, 0x50, 0x31, 0x8A, 0x23, 0x08, 0x01, 0xA1,
        0xF5, 0xFE, 0x31, 0x32, 0x7F, 0xE2, 0x05, 0x82, 0xD6, 0x0B, 0xED, 0x4D, 0x55, 0x32, 0x41, 0x94,
        0x29, 0x6F, 0x55, 0x7D, 0xE3, 0x0F, 0x77, 0x19, 0xE5, 0x6C, 0x30, 0xEB, 0xDE, 0xF6, 0xA7, 0x86
    };

    /// G seed
    static uint8_t SRP6a_G[1] = {
        0x02
    };

    /// SRP6a encryption helper (RSA based)
    class SRP6a
    {
        public:
            /// Constructor
            SRP6a(const std::string & p_Salt, const std::string & p_AccountName, const std::string p_PasswordVerifier);

            /// Compute public b key
            void ComputePublicB();
            /// Compute U
            void ComputeU(uint8_t * p_A, uint32_t p_ASize);
            /// Compute S
            void ComputeS(BigNumber & p_U);
            /// Compute session key
            void ComputeSessionKey();
            /// Compute client M
            void ComputeClientM(uint8_t * p_A, uint32_t p_ASize);
            /// Compute server M
            void ComputeServerM(uint8_t * p_ClientM, uint32_t p_ClientMSize);

            /// Compare two bytes array
            bool Compare(uint8_t * l_Left, uint8_t * l_Right, uint32_t p_Size);

        private:
            /// String to big number
            BigNumber MakeBigNumber(const std::string & p_Str);
            /// Str to byte array
            void ToByteArray(const std::string & p_Str, uint8_t * p_Dest);
            /// SHA256 of string
            void Sha256(const std::string & p_Str, uint8_t * p_Dest);
            /// SHA256 of data
            void Sha256(uint8_t * p_Data, uint32_t p_DataSize, uint8_t * p_Dest);

        public:
            BigNumber N;
            BigNumber G;
            BigNumber V;
            BigNumber K;
            BigNumber A;
            BigNumber S;
            BigNumber PrivateB;
            BigNumber SessionKey;

            uint8_t I[SHA256_DIGEST_LENGTH];
            uint8_t Salt[SHA256_DIGEST_LENGTH];
            uint8_t PublicB[4 * SHA256_DIGEST_LENGTH];
            uint8_t ClientM[SHA256_DIGEST_LENGTH];
            uint8_t ServerM[SHA256_DIGEST_LENGTH];

    };

}