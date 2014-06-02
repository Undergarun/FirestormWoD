/// Credit merydwin@gmail.com
#pragma once

#include "AuthComponent.hpp"
#include "SRP6a.hpp"
#include "Packet.hpp"
#include "BNet2Crypt.hpp"
#include "../Server/RealmSocket.h"

namespace BNet2 {

    enum SessionState
    {
        BATTLENET2_SESSION_STATE_NONE                       = 0,
        BATTLENET2_SESSION_STATE_AUTHED                     = 1,
        BATTLENET2_SESSION_STATE_WAIT_PROOF_VERIFICATION    = 2,
        BATTLENET2_SESSION_STATE_PROOF_VERIFICATION         = 3,
    };

    /// Battle net 2 session
    class Session : public RealmSocket::Session
    {
        public:
            /// Constructor
            Session(RealmSocket& p_Socket);
            /// Destructor
            ~Session();

            /// On read
            virtual void OnRead(void);
            /// On accept
            virtual void OnAccept(void);
            /// On close
            virtual void OnClose(void);

            /// Send packet
            void Send(BNet2::Packet * p_Packet);

            /// Set client platform
            void SetClientPlatform(const std::string & p_Str);
            /// Set SRP params
            void SetSRPParams(const std::string & p_Salt, const std::string & p_AccountName, const std::string & p_PasswordHash);

            /// Get session state
            SessionState GetState();
            /// Get client platform
            Platforms GetClientPlatform();
            /// Get secure remote password computation helper
            SRP6a * GetSRP();

        private:
            /// Wait N bytes and append it to the current packet
            void WaitBytes(uint32_t p_Count);

            /// Get socket
            RealmSocket & GetSocket(void);

            /// Send auth result
            void SendAuthResult(BNet2::AuthResult p_Result, bool p_Failed = true);

        public:
            /// Authentication informations client request
            bool None_Handle_InformationRequest(BNet2::Packet * p_Packet);
            /// Authentication client request
            bool None_Handle_ProofResponse(BNet2::Packet * p_Packet);

            /// Ping request
            bool Creep_Handle_Ping(BNet2::Packet * p_Packet);
            /// Disconnect notify
            bool Creep_Handle_Disconnect(BNet2::Packet * p_Packet);

            /// Realm list client request
            bool WoW_Handle_RealmUpdate(BNet2::Packet * p_Packet);
            /// Realm connection client request
            bool WoW_Handle_JoinRequest(BNet2::Packet * p_Packet);

        private:
            SessionState    m_State;            ///< Session state
            SRP6a       *   m_SRP;              ///< Secure remote password
            Platforms       m_Platform;         ///< Client platform
            BNet2Crypt      m_BNet2Crypt;       ///< Battle Net 2 crypt system

            BNet2::Packet * m_CurrentPacket;    ///< Current read packet

            RealmSocket   & m_Socket;           ///< Session socket

            std::string m_AccountName;
            uint32_t m_AccountID;
            int32_t m_AccountSecurityLevel;
            std::string m_Locale;

    };

}