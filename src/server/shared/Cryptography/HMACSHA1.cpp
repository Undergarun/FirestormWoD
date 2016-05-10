////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "HMACSHA1.h"
#include "BigNumber.h"
#include "Common.h"
#include "BigNumber.h"

HmacHash::HmacHash(uint32 len, uint8 *seed)
{
    HMAC_CTX_init(&m_ctx);
    HMAC_Init_ex(&m_ctx, seed, len, EVP_sha1(), NULL);
}

HmacHash::~HmacHash()
{
    HMAC_CTX_cleanup(&m_ctx);
}

void HmacHash::UpdateData(const std::string &str)
{
    HMAC_Update(&m_ctx, (uint8 const*)str.c_str(), str.length());
}

void HmacHash::UpdateData(const uint8* data, size_t len)
{
    HMAC_Update(&m_ctx, data, len);
}

void HmacHash::Finalize()
{
    uint32 length = 0;
    HMAC_Final(&m_ctx, (uint8*)m_digest, &length);
    ASSERT(length == SHA_DIGEST_LENGTH);
}

uint8 *HmacHash::ComputeHash(BigNumber* bn)
{
    HMAC_Update(&m_ctx, bn->AsByteArray(), bn->GetNumBytes());
    Finalize();
    return (uint8*)m_digest;
}

//////////////////////////////////////////////////////////////////////////

HmacHash256::HmacHash256(uint32 len, uint8 *seed)
{
    HMAC_CTX_init(&m_ctx);
    HMAC_Init_ex(&m_ctx, seed, len, EVP_sha256(), NULL);
}

HmacHash256::~HmacHash256()
{
    HMAC_CTX_cleanup(&m_ctx);
}

void HmacHash256::UpdateData(const std::string &str)
{
    HMAC_Update(&m_ctx, (uint8 const*)str.c_str(), str.length());
}

void HmacHash256::UpdateData(const uint8* data, size_t len)
{
    HMAC_Update(&m_ctx, data, len);
}

void HmacHash256::Finalize()
{
    uint32 length = 0;
    HMAC_Final(&m_ctx, (uint8*)m_digest, &length);
    ASSERT(length == SHA256_DIGEST_LENGTH);
}

uint8 *HmacHash256::ComputeHash(BigNumber* bn)
{
    HMAC_Update(&m_ctx, bn->AsByteArray(), bn->GetNumBytes());
    Finalize();
    return (uint8*)m_digest;
}

uint8 *HmacHash256::ComputeHash(uint8 *seed, uint32 len)
{
    HMAC_Update(&m_ctx, seed, len);
    Finalize();
    return (uint8*)m_digest;
}
