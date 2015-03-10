////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include <ScriptPCH.h>
#include <ScriptMgr.h>

namespace MS { namespace UserReporting
{

    enum State
    {
        CreateChar = 6,
        LoadScreen = 7,
        InWorld    = 8
    };

    /*
     * Update user step in user_reporting table
     * It's only update if the step provided is higher than previous step
     * @p_Player : Instance of player to update
     * @p_State  : User state (see @UserState enum)
     */
    void UpdateUserStep(WorldSession* p_Session, State p_State)
    {
        std::string l_Query = "UPDATE user_reporting SET step = %u, last_ip = '%s' WHERE account_id = %u AND step < %u";
        LoginDatabase.PExecute
        (
            l_Query.c_str(),
            p_State,                                   ///< step
            p_Session->GetRemoteAddress().c_str(),     ///< last_ip
            p_Session->GetAccountId(),                 ///< account_id
            p_State                                    ///< step
        );
    }

    class PlayerUserReporting : public PlayerScript
    {
        public:
            PlayerUserReporting() : PlayerScript("PlayerUserReporting") {}

            void OnCreate(Player* p_Player) override
            {
                UpdateUserStep(p_Player->GetSession(), State::CreateChar);
            }

            void OnLogin(Player* p_Player) override
            {
                UpdateUserStep(p_Player->GetSession(), State::InWorld);
            }
    };

    class ServerUserReporting : public ServerScript
    {
        public :
            ServerUserReporting() : ServerScript("ServerUserReporting") {}

            void OnPacketReceive(WorldSocket* p_Socket, WorldPacket& p_Packet, WorldSession* p_Session) override
            {
                if (p_Session != nullptr && p_Packet.GetOpcode() == CMSG_LOAD_SCREEN)
                    UpdateUserStep(p_Session, State::LoadScreen);
            }
    };
} ///< UserReporting
} ///< MS

void AddSC_user_reporting()
{
    new MS::UserReporting::PlayerUserReporting();
    new MS::UserReporting::ServerUserReporting();
};

