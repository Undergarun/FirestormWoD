/// Credit merydwin@gmail.com

#include "Session.hpp"
#include "Common.h"
#include "Database/DatabaseEnv.h"
#include "Configuration/Config.h"
#include "Log.h"
#include "Module.hpp"
#include "WoWModules/PasswordAuth.hpp"
#include "WoWModules/RiskFingerprintAuth.hpp"
#include "WoWModules/ThumbprintAuth.hpp"
#include "RealmList.h"
#include "AuthCodes.h"
#include "Cryptography/HMACSHA1.h"
#include "Cryptography/BigNumber.h"

#include <algorithm>

namespace BNet2 {

    typedef struct AuthHandler
    {
        uint32_t Opcode;
        uint32_t Channel;
        bool (Session::*Handler)(BNet2::Packet*);
    } AuthHandler;

    const AuthHandler OpcodeTable[] =
    {
        { OPCODE_ID(CMSG_INFORMATION_REQUEST),  OPCODE_CHANNEL(CMSG_INFORMATION_REQUEST),   &Session::None_Handle_InformationRequest    },
        { OPCODE_ID(CMSG_PROOF_RESPONSE),       OPCODE_CHANNEL(CMSG_PROOF_RESPONSE),        &Session::None_Handle_ProofResponse         },
        { OPCODE_ID(CMSG_PING),                 OPCODE_CHANNEL(CMSG_PING),                  &Session::Creep_Handle_Ping                 },
        { OPCODE_ID(CMSG_REALM_UPDATE),         OPCODE_CHANNEL(CMSG_REALM_UPDATE),          &Session::WoW_Handle_RealmUpdate            },
        { OPCODE_ID(CMSG_JOIN_REQUEST),         OPCODE_CHANNEL(CMSG_JOIN_REQUEST),          &Session::WoW_Handle_JoinRequest            },
    };

    #define AUTH_TOTAL_COMMANDS 5

    //////////////////////////////////////////////////////////////////////////

    /// Constructor
    Session::Session(RealmSocket& p_Socket)
        : m_SRP(0), m_Platform(BNet2::BATTLENET2_PLATFORM_BASE), m_Socket(p_Socket), m_CurrentPacket(NULL), m_State(BATTLENET2_SESSION_STATE_NONE)
    {

    }
    /// Destructor
    Session::~Session()
    {
        if (m_SRP)
            delete m_SRP;
    }

    //////////////////////////////////////////////////////////////////////////

    /// On read
    void Session::OnRead(void)
    {
        while (1)
        {
            uint32_t l_Size = GetSocket().recv_len();
            
            if (l_Size < 2)
                return;
            
            uint8_t * l_Buffer = new uint8_t[l_Size];

            if (!GetSocket().recv((char *)l_Buffer, l_Size))
            {
                delete[] l_Buffer;
                return;
            }

            if (m_SRP && !m_BNet2Crypt.IsInitialized() && l_Buffer[0] == 0x45 && l_Buffer[1] == 0x01)
            {
                m_BNet2Crypt.Init(&m_SRP->SessionKey);

                if (l_Size > 2)
                {
                    uint8_t * l_SecondBuffer = new uint8_t[l_Size - 2];
                    memcpy(l_SecondBuffer, l_Buffer + 2, l_Size - 2);

                    delete l_Buffer;
                    l_Buffer = l_SecondBuffer;
                }
                else
                {
                    delete l_Buffer;
                    return;
                }
            }

            if (m_BNet2Crypt.IsInitialized())
                m_BNet2Crypt.Decrypt((uint8_t*)l_Buffer, l_Size);

            Packet l_Packet((char*)l_Buffer, l_Size);

            delete[] l_Buffer;

            uint32_t l_Opcode   = l_Packet.GetOpcode();
            uint32_t l_Channel  = l_Packet.GetChannel();

            uint32_t l_I;

            for (l_I = 0; l_I < AUTH_TOTAL_COMMANDS; ++l_I)
            {
                if ((uint8)OpcodeTable[l_I].Opcode == l_Opcode && (uint8)OpcodeTable[l_I].Channel == l_Channel)
                {
                    sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::OnRead Got data for opcode %u channel %u recv length %u", l_Opcode, l_Channel, l_Size);

                    m_CurrentPacket = &l_Packet;

                    if (!(*this.*OpcodeTable[l_I].Handler)(&l_Packet))
                    {
                        sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::OnRead Command handler failed for opcode %u channel %u recv length %u", l_Opcode, l_Channel, l_Size);
                        GetSocket().shutdown();
                        m_CurrentPacket = NULL;
                        return;
                    }
                    break;
                }
            }

            // Report unknown packets in the error log
            if (l_I == AUTH_TOTAL_COMMANDS)
            {
                sLog->outError(LOG_FILTER_AUTHSERVER, "BNet2::Session::OnRead Got unknown packet from '%s' opcode %u channel %u size %u", GetSocket().getRemoteAddress().c_str(), l_Opcode, l_Channel, l_Size);
                m_CurrentPacket = NULL;
                return;
            }

            m_CurrentPacket = NULL;
        }
    }
    /// On accept
    void Session::OnAccept(void)
    {
        sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::OnAccept '%s:%d' Accepting connection", GetSocket().getRemoteAddress().c_str(), GetSocket().getRemotePort());
    }
    /// On close
    void Session::OnClose(void)
    {
        sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::OnClose");
    }
    
    //////////////////////////////////////////////////////////////////////////

    /// Send packet
    void Session::Send(BNet2::Packet * p_Packet)
    {
        if (!p_Packet || !p_Packet->GetSize())
            return;

        uint8_t * l_Data = p_Packet->GetData();

        if (m_BNet2Crypt.IsInitialized())
            m_BNet2Crypt.Encrypt(l_Data, p_Packet->GetSize());

         GetSocket().send((char const*)l_Data, p_Packet->GetSize());

         sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::Send opcode %u channel %u size %u", p_Packet->GetOpcode(), p_Packet->GetChannel(), p_Packet->GetSize());
    }

    //////////////////////////////////////////////////////////////////////////

    /// Set client platform
    void Session::SetClientPlatform(const std::string & p_Str)
    {
        if (p_Str == "Win")
            m_Platform = BNet2::BATTLENET2_PLATFORM_WIN;
        else if (p_Str == "Wn64")
            m_Platform = BNet2::BATTLENET2_PLATFORM_WIN64;
        else if (p_Str == "Mc64")
            m_Platform = BNet2::BATTLENET2_PLATFORM_MAC64;
    }
    /// Set SRP params
    void Session::SetSRPParams(const std::string & p_Salt, const std::string & p_AccountName, const std::string & p_PasswordHash)
    {
        if (m_SRP)
            delete m_SRP;

        m_SRP = new SRP6a(p_Salt, p_AccountName, p_PasswordHash);
    }

    //////////////////////////////////////////////////////////////////////////

    /// Get session state
    SessionState Session::GetState()
    {
        return m_State;
    }
    /// Get client platform
    Platforms Session::GetClientPlatform()
    {
        return m_Platform;
    }
    /// Get secure remote password computation helper
    SRP6a * Session::GetSRP()
    {
        return m_SRP;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Wait N bytes and append it to the current packet
    void Session::WaitBytes(uint32_t p_Count)
    {
        char * l_Buffer = new char[p_Count];

        GetSocket().recv(l_Buffer, p_Count);

        if (m_BNet2Crypt.IsInitialized())
            m_BNet2Crypt.Decrypt((uint8_t*)l_Buffer, p_Count);

        if (m_CurrentPacket)
            m_CurrentPacket->AppendToStorage(l_Buffer, p_Count);

        delete[] l_Buffer;
    }

    //////////////////////////////////////////////////////////////////////////

    RealmSocket & Session::GetSocket(void)
    {
        return m_Socket;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Send auth result
    void Session::SendAuthResult(BNet2::AuthResult p_Result, bool p_Failed)
    {
        BNet2::Packet l_Result(BNet2::SMSG_AUTH_COMPLETE);

        l_Result.WriteBits(p_Failed, 1);

        if (p_Failed)
        {
            l_Result.WriteBits(false, 1);       ///< false - disable optional modules
            l_Result.WriteBits(1, 2);           ///< 1 - enable AuthResults
            l_Result.WriteBits(p_Result, 16);   ///< AuthResults (Error codes)
            l_Result.WriteBits(0x80000000, 32); ///< Unknown*
        }
        else
        {
            bool l_HaveOptData = true;

            l_Result.WriteBits(0, 3);
            l_Result.WriteBits(0x80005000, 32);         ///< Ping request, ~10 secs
            l_Result.WriteBits(l_HaveOptData, 1);

            if (l_HaveOptData)
            {
                bool l_HasConnectionInfo = true;

                l_Result.WriteBits(l_HasConnectionInfo, 1);

                if (l_HasConnectionInfo)                ///< Battlenet::Regulator::LeakyBucketParams
                {
                    l_Result.WriteBits(25000000, 32);   ///< Threshold
                    l_Result.WriteBits(1000, 32);       ///< Rate
                }
            }

            l_Result.WriteBits(false, 1);
            l_Result.WriteString("", 8, false);         ///< First Name
            l_Result.WriteString("", 8, false);         ///< Last Name
            l_Result.WriteBits(m_AccountID, 32);
            l_Result.WriteBits(0, 8);
            l_Result.WriteBits(0, 64);
            l_Result.WriteBits(0, 8);
            l_Result.WriteString(m_AccountName, 5, false, -1);
            l_Result.WriteBits(0, 64);
            l_Result.WriteBits(0, 32);
            l_Result.WriteBits(0, 8);
        }

        Send(&l_Result);
    }

    //////////////////////////////////////////////////////////////////////////

    /// Authentication informations client request
    bool Session::None_Handle_InformationRequest(BNet2::Packet * p_Packet)
    {
        std::string l_Program   = p_Packet->ReadFourCC();
        std::string l_Platform  = p_Packet->ReadFourCC();
        std::string l_Locale    = p_Packet->ReadFourCC();

        SetClientPlatform(l_Platform);

        uint32_t l_ComponentCount = p_Packet->ReadBits<uint32_t>(6);

        for (uint32_t l_I = 0; l_I < l_ComponentCount; ++l_I)
        {
            AuthComponent l_Component;
            l_Component.Program     = p_Packet->ReadFourCC();
            l_Component.Platform    = p_Packet->ReadFourCC();
            l_Component.Build       = p_Packet->ReadBits<uint32>(32);

            BNet2::AuthResult l_Result = AuthComponentManager::GetSingleton()->Check(l_Component);

            if (l_Result != BNet2::BATTLENET2_AUTH_OK)
            {
                SendAuthResult(l_Result);
                sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::None_Handle_InformationRequest Component(%s %s %u) not allowed, error code => %u", l_Component.Program.c_str(), l_Component.Platform.c_str(), l_Component.Build, l_Result);

                return true;
            }
        }
        
        /// Have login
        if (p_Packet->ReadBits<bool>(1))
        {
            std::string const & l_IPAddress = GetSocket().getRemoteAddress();

            PreparedStatement* l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_IP_BANNED);
            l_Stmt->setString(0, l_IPAddress);

            PreparedQueryResult l_Result = LoginDatabase.Query(l_Stmt);

            if (l_Result)
            {
                SendAuthResult(BNet2::BATTLENET2_AUTH_ACCOUNT_TEMP_BANNED);
                sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::None_Handle_InformationRequest '%s:%d' Banned ip tries to login!", GetSocket().getRemoteAddress().c_str(), GetSocket().getRemotePort());
                return true;
            }

            std::string l_AccountName = p_Packet->ReadString(p_Packet->ReadBits<uint32_t>(9) + 3);

            l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_LOGONCHALLENGE);
            l_Stmt->setString(0, l_AccountName);

            PreparedQueryResult l_Result_2 = LoginDatabase.Query(l_Stmt);

            if (l_Result_2)
            {
                Field* l_Fields = l_Result_2->Fetch();

                // If the IP is 'locked', check that the player comes indeed from the correct IP address
                bool l_Locked = false;
                if (l_Fields[2].GetUInt8() == 1)                  // if ip is locked
                {
                    sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::None_Handle_InformationRequest Account '%s' is locked to IP - '%s'", l_AccountName.c_str(), l_Fields[3].GetCString());
                    sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::None_Handle_InformationRequest Player address is '%s'", l_IPAddress.c_str());

                    if (strcmp(l_Fields[4].GetCString(), l_IPAddress.c_str()) != 0)
                    {
                        sLog->outDebug(LOG_FILTER_AUTHSERVER, "[AuthChallenge] Account IP differs");
                        SendAuthResult(BNet2::BATTLENET2_AUTH_CONNECT_METHOD_CHANGED);
                        l_Locked = true;
                    }
                    else
                        sLog->outDebug(LOG_FILTER_AUTHSERVER, "BNet2::Session::None_Handle_InformationRequest Account IP matches");
                }

                if (!l_Locked)
                {
                    //set expired bans to inactive
                    LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(LOGIN_UPD_EXPIRED_ACCOUNT_BANS));

                    // If the account is banned, reject the logon attempt
                    l_Stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_ACCOUNT_BANNED);
                    l_Stmt->setUInt32(0, l_Fields[1].GetUInt32());

                    PreparedQueryResult l_BanResult = LoginDatabase.Query(l_Stmt);

                    if (l_BanResult)
                    {
                        if ((*l_BanResult)[0].GetUInt32() == (*l_BanResult)[1].GetUInt32())
                        {
                            SendAuthResult(BNet2::BATTLENET2_AUTH_ACCOUNT_TEMP_BANNED);
                            sLog->outDebug(LOG_FILTER_AUTHSERVER, "'%s:%d' BNet2::Session::None_Handle_InformationRequest Banned account %s tried to login!", GetSocket().getRemoteAddress().c_str(), GetSocket().getRemotePort(), l_AccountName.c_str());
                        }
                        else
                        {
                            SendAuthResult(BNet2::BATTLENET2_AUTH_ACCOUNT_TEMP_BANNED);
                            sLog->outDebug(LOG_FILTER_AUTHSERVER, "'%s:%d' BNet2::Session::None_Handle_InformationRequest Temporarily banned account %s tried to login!", GetSocket().getRemoteAddress().c_str(), GetSocket().getRemotePort(), l_AccountName.c_str());
                        }
                    }
                    else
                    {
                        SetSRPParams(l_Fields[10].GetString(), l_AccountName, l_Fields[9].GetString());
                        GetSRP()->ComputePublicB();

                        std::list<BNet2::Module::Ptr> l_Modules = BNet2::ModuleManager::GetSingleton()->GetPlatformModules(GetClientPlatform());

                        BNet2::Packet l_ProofRequest(BNet2::SMSG_PROOF_REQUEST);

                        l_ProofRequest.WriteBits(2, 3); ///< Modules count

                        for (std::list<BNet2::Module::Ptr>::iterator l_It = l_Modules.begin(); l_It != l_Modules.end(); l_It++)
                        {
                            switch ((*l_It)->GetID())
                            {
                                case WOW_PASSWORD_AUTH_MODULE_ID:
                                case WOW_THUMBPRINT_AUTH_MODULE_ID:
                                    l_ProofRequest.WriteFourCC((*l_It)->GetTypeStr());
                                    l_ProofRequest.WriteFourCC("\0\0XX");
                                    l_ProofRequest.AppendByteArray((*l_It)->GetHashData(), (*l_It)->GetHashDataSize());
                                    l_ProofRequest.WriteBits((*l_It)->GetSize(this), 10);

                                    (*l_It)->Write(this, &l_ProofRequest);
                                    break;

                                default:
                                    break;
                            }
                        }

                        Send(&l_ProofRequest);

                        m_AccountName           = l_AccountName;
                        m_AccountID             = l_Fields[1].GetUInt32();
                        m_AccountSecurityLevel  = l_Fields[5].GetUInt8() <= SEC_ADMINISTRATOR ? AccountTypes(l_Fields[5].GetUInt8()) : SEC_ADMINISTRATOR;
                        m_Locale                = l_Locale;

                        sLog->outDebug(LOG_FILTER_AUTHSERVER, "'%s:%d' BNet2::Session::None_Handle_InformationRequest account %s is using '%s' locale (%u)", GetSocket().getRemoteAddress().c_str(), GetSocket().getRemotePort(),
                            l_AccountName.c_str(), l_Locale.c_str(), GetLocaleByName(l_Locale));
                    }
                }
            }
            else
            {
                SendAuthResult(BNet2::BATTLENET2_AUTH_BAD_INFOS);
            }
        }

        return true;
    }
    /// Authentication client request
    bool Session::None_Handle_ProofResponse(BNet2::Packet * p_Packet)
    {
        uint32_t l_ModuleCount = p_Packet->ReadBits<uint32_t>(3);

        for (uint32_t l_I = 0; l_I < l_ModuleCount; ++l_I)
        {
            uint32_t l_DataSize = p_Packet->ReadBits<uint32_t>(10);
            uint32_t l_StateID  = p_Packet->Read<uint8_t>();

            switch (l_StateID)
            {
                case BNet2::BATTLENET2_SESSION_STATE_AUTHED:
                    SendAuthResult(BNet2::BATTLENET2_AUTH_OK, false);
                    break;

                case BNet2::BATTLENET2_SESSION_STATE_WAIT_PROOF_VERIFICATION:
                {
                    if (l_DataSize != 0x121)
                        return false;

                    uint8_t l_A[4 * SHA256_DIGEST_LENGTH];
                    uint8_t l_M1[SHA256_DIGEST_LENGTH];
                    uint8_t l_ClientChallenge[4 * SHA256_DIGEST_LENGTH];

                    p_Packet->ReadBytes(l_A,                4 * SHA256_DIGEST_LENGTH);
                    p_Packet->ReadBytes(l_M1,                   SHA256_DIGEST_LENGTH);
                    p_Packet->ReadBytes(l_ClientChallenge,  4 * SHA256_DIGEST_LENGTH);

                    GetSRP()->ComputeU(         l_A,    4 * SHA256_DIGEST_LENGTH);
                    GetSRP()->ComputeClientM(   l_A,    4 * SHA256_DIGEST_LENGTH);

                    if (GetSRP()->Compare(GetSRP()->ClientM, l_M1, SHA256_DIGEST_LENGTH))
                    {
                        GetSRP()->ComputeServerM(l_M1, SHA256_DIGEST_LENGTH);

                        m_State = BATTLENET2_SESSION_STATE_PROOF_VERIFICATION;

                        std::list<BNet2::Module::Ptr> l_Modules = BNet2::ModuleManager::GetSingleton()->GetPlatformModules(GetClientPlatform());

                        BNet2::Packet l_ProofVerification(BNet2::SMSG_PROOF_REQUEST);

                        l_ProofVerification.WriteBits(2, 3); ///< Modules count

                        for (std::list<BNet2::Module::Ptr>::iterator l_It = l_Modules.begin(); l_It != l_Modules.end(); l_It++)
                        {
                            switch ((*l_It)->GetID())
                            {
                                case WOW_PASSWORD_AUTH_MODULE_ID:
                                case WOW_RISKFINGERPRINT_AUTH_MODULE_ID:
                                    l_ProofVerification.WriteFourCC((*l_It)->GetTypeStr());
                                    l_ProofVerification.WriteFourCC("\0\0XX");
                                    l_ProofVerification.AppendByteArray((*l_It)->GetHashData(), (*l_It)->GetHashDataSize());
                                    l_ProofVerification.WriteBits((*l_It)->GetSize(this), 10);

                                    (*l_It)->Write(this, &l_ProofVerification);
                                    break;

                                default:
                                    break;
                            }
                        }

                        Send(&l_ProofVerification);
                    }
                    else
                    {
                        SendAuthResult(BNet2::BATTLENET2_AUTH_BAD_INFOS);
                        return false;
                    }

                    break;
                }

                default:
                    break;
            }

        }

        return true;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Ping request
    bool Session::Creep_Handle_Ping(BNet2::Packet * p_Packet)
    {
        BNet2::Packet l_Pong(BNet2::SMSG_PONG);
        Send(&l_Pong);

        return true;
    }
    /// Disconnect notify
    bool Session::Creep_Handle_Disconnect(BNet2::Packet * p_Packet)
    {
        return false;
    }

    //////////////////////////////////////////////////////////////////////////

    /// Realm list client request
    bool Session::WoW_Handle_RealmUpdate(BNet2::Packet * p_Packet)
    {
        // Update realm list if need
        sRealmList->UpdateIfNeed();

        BNet2::Packet l_Packet(BNet2::SMSG_REALM_AUTH_OK);

        l_Packet.FlushBits();
        l_Packet.WriteBits(0, 8);

        ACE_INET_Addr l_ClientAddress;
        GetSocket().peer().get_remote_addr(l_ClientAddress);

         uint32_t realmCounter = 1;
         for (RealmList::RealmMap::const_iterator i = sRealmList->begin(); i != sRealmList->end(); ++i)
         {
             const Realm &realm = i->second;
             uint8 lock = (realm.allowedSecurityLevel > m_AccountSecurityLevel) ? 1 : 0;
 
             //Not visible if can't access realm
             if (lock)
                 continue;
 
             //// don't work with realms which not compatible with the client
             //bool okBuild = ((_expversion & POST_BC_EXP_FLAG) && realm.gamebuild == _build) || ((_expversion & PRE_BC_EXP_FLAG) && !AuthHelper::IsPreBCAcceptedClientBuild(realm.gamebuild));
 
             uint32 flag = realm.flag;
             //RealmBuildInfo const* buildInfo = AuthHelper::GetBuildInfo(realm.gamebuild);
             //if (!okBuild)
             //{
             //    if (!buildInfo)
             //        continue;
 
             //    flag |= REALM_FLAG_OFFLINE | REALM_FLAG_SPECIFYBUILD;   // tell the client what build the realm is for
             //}
 
             //if (!buildInfo)
             //    flag &= ~REALM_FLAG_SPECIFYBUILD;
 
             std::string name = i->first;
             //if (_expversion & PRE_BC_EXP_FLAG && flag & REALM_FLAG_SPECIFYBUILD)
             //{
             //    std::ostringstream ss;
             //    ss << name << " (" << buildInfo->MajorVersion << '.' << buildInfo->MinorVersion << '.' << buildInfo->BugfixVersion << ')';
             //    name = ss.str();
             //}
 
             BNet2::Packet l_Buffer(BNet2::SMSG_REALM_UPDATE);
 
             l_Buffer.WriteBits(true, 1);
             l_Buffer.WriteBits(1, 32);
             l_Buffer.WriteBits<float>(0, 32);
             l_Buffer.WriteBits(0, 8);
             l_Buffer.WriteBits(realm.m_ID, 19);
             l_Buffer.WriteBits(0x80000000 + realm.icon, 32);
             l_Buffer.WriteString(name, 10, false);
             l_Buffer.WriteBits(false, 1);
             l_Buffer.WriteBits(0, 8);
             l_Buffer.WriteBits(0, 12);
             l_Buffer.WriteBits(0, 8);
             l_Buffer.WriteBits(0, 32);
             l_Buffer.WriteBits(realmCounter, 8);
 
             l_Buffer.FlushBits();
             l_Buffer.Write<uint8_t>(0x43);
             l_Buffer.Write<uint8_t>(0x02);

             l_Packet.AppendByteArray(l_Buffer.GetData(), l_Buffer.GetSize());
             realmCounter++;
         }

        Send(&l_Packet);

        return true;
    }

    void ToByteArray(const std::string & p_Str, uint8_t * p_Dest)
    {
        char * l_Str = const_cast<char*>(p_Str.c_str());

        for (unsigned int l_I = 0; l_I < p_Str.size(); l_I += 2)
        {
            char l_Buffer[3] = { *(l_Str + l_I), *(l_Str + l_I + 1), '\0' };
            p_Dest[l_I / 2] = strtol(l_Buffer, NULL, 16);
        }
    }
    BigNumber MakeBigNumber(const std::string & p_Str)
    {
        uint8_t * l_Buffer = new uint8_t[(p_Str.size() / 2) + 1];

        ToByteArray(p_Str, l_Buffer);
        l_Buffer[p_Str.size() / 2] = 0;

        BigNumber l_Result;
        l_Result.SetBinary(l_Buffer, (p_Str.size() / 2) + 1);

        delete[] l_Buffer;

        return l_Result;
    }

    /// Realm connection client request
    bool Session::WoW_Handle_JoinRequest(BNet2::Packet * p_Packet)
    {
        uint8_t clientSalt[4];
        uint8_t serverSalt[4];

        *(uint32_t*)clientSalt = p_Packet->ReadBits<uint32_t>(32);
        *(int32_t*)serverSalt = rand();

        uint8_t sessionKey[0x28];
        memset(sessionKey, 0, sizeof(sessionKey));

        HmacHash l_Hmac(64, (uint8_t*)GetSRP()->SessionKey.AsByteArray(64));
        l_Hmac.UpdateData((const uint8_t*)"WoW", 4);
        l_Hmac.UpdateData(clientSalt, sizeof(clientSalt));
        l_Hmac.UpdateData(serverSalt, sizeof(serverSalt));
        l_Hmac.Finalize();
        
        memcpy(sessionKey, l_Hmac.GetDigest(), l_Hmac.GetLength());
        
        HmacHash l_Hmac2(64, (uint8_t*)GetSRP()->SessionKey.AsByteArray(64));
        l_Hmac2.UpdateData((const uint8_t*)"WoW", 4);
        l_Hmac2.UpdateData(serverSalt, sizeof(serverSalt));
        l_Hmac2.UpdateData(clientSalt, sizeof(clientSalt));
        l_Hmac2.Finalize();

        ASSERT(l_Hmac.GetLength() + l_Hmac2.GetLength() == sizeof(sessionKey));
        memcpy(sessionKey + l_Hmac.GetLength(), l_Hmac2.GetDigest(), l_Hmac2.GetLength());

        char sSessionKey[sizeof(sessionKey) * 3];
        for (uint32_t i = 0; i < sizeof(sessionKey); ++i)
        {
            sprintf(sSessionKey + i * 2, "%02X", sessionKey[i]);
        }

        sSessionKey[sizeof(sSessionKey) - 1] = 0;

        BigNumber l_K = MakeBigNumber(sSessionKey);

        PreparedStatement *stmt = LoginDatabase.GetPreparedStatement(LOGIN_UPD_LOGONPROOF);
        stmt->setString(0, l_K.AsHexStr());
        stmt->setString(1, GetSocket().getRemoteAddress().c_str());
        stmt->setUInt32(2, GetLocaleByName(m_Locale));
        
        switch (GetClientPlatform())
        {
            case BATTLENET2_PLATFORM_WIN:
                stmt->setString(3, "Win");
                break;
            case BATTLENET2_PLATFORM_WIN64:
                stmt->setString(3, "Wn64");
                break;
            case BATTLENET2_PLATFORM_MAC64:
                stmt->setString(3, "Mc64");
                break;

            default:
                stmt->setString(3, "unk");
                break;
        }

        stmt->setString(4, m_AccountName);

        LoginDatabase.Query(stmt);

        uint32_t l_RealmCount = 0;
        for (RealmList::RealmMap::const_iterator i = sRealmList->begin(); i != sRealmList->end(); ++i)
        {
            const Realm &realm = i->second;
            uint8 lock = (realm.allowedSecurityLevel > m_AccountSecurityLevel) ? 1 : 0;

            //Not visible if can't access realm
            if (lock)
                continue;

            l_RealmCount++;
        }

        BNet2::Packet l_Buffer(BNet2::SMSG_JOIN_RESPONSE);

        l_Buffer.WriteBits(l_RealmCount == 0, 1);
        l_Buffer.WriteBits(*(uint32_t*)serverSalt, 32);
        l_Buffer.WriteBits(l_RealmCount, 5);
        l_Buffer.FlushBits();

        for (RealmList::RealmMap::const_iterator i = sRealmList->begin(); i != sRealmList->end(); ++i)
        {
            const Realm &realm = i->second;
            uint8 lock = (realm.allowedSecurityLevel > m_AccountSecurityLevel) ? 1 : 0;

            //Not visible if can't access realm
            if (lock)
                continue;

            ACE_INET_Addr l_Address;
            l_Address.string_to_addr(realm.address.c_str());

            uint8_t port[2];
            *(uint16_t*)port = l_Address.get_port_number();
            std::reverse(port, port + sizeof(port));
            
            uint32_t l_IpAddress = l_Address.get_ip_address();
            EndianConvertReverse(l_IpAddress);

            l_Buffer.Write(l_IpAddress);
            l_Buffer.AppendByteArray(port, sizeof(port));
        }

        l_Buffer.FlushBits();
        l_Buffer.WriteBits(0, 5);

        Send(&l_Buffer);

        return true;
    }

}