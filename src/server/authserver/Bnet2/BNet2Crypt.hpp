/// Credit merydwin@gmail.com
#pragma once

#include <inttypes.h>
#include "Cryptography/ARC4.h"
#include "Cryptography/BigNumber.h"

namespace BNet2
{
    /// BNet2 ARC4 crypt
    class BNet2Crypt
    {
        public:
            /// Constructor
            BNet2Crypt();

            /// Init
            void Init(BigNumber * p_SessionKey);

            /// Decrypt data
            void Decrypt(uint8_t * p_Data, size_t p_Size);
            /// Encrypt data
            void Encrypt(uint8_t * p_Data, size_t p_Size);

            /// Is initialized
            bool IsInitialized() const;

        private:
            ARC4 m_ClientDecrypt;   ///< ARC4 client decrypter
            ARC4 m_ServerEncrypt;   ///< ARC4 server encrypter
            bool m_Initialized;     ///< Is ready
    };
}