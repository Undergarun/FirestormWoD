////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

class go_ashran_portal : public GameObjectScript
{
public:
    go_ashran_portal() : GameObjectScript("go_ashran_portal") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Go)
    {
        float l_PositionX = 0.f;
        float l_PositionY = 0.f;
        float l_PositionZ = 0.f;
        float l_Orientation = 0.f;

        if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
        {
            l_PositionX = 3684.13f;
            l_PositionY = -3839.93f;
            l_PositionZ = 45.49f;
            l_Orientation = 3.4f;
        }
        else
        {
            l_PositionX = 5357.f;
            l_PositionY = -3943.61f;
            l_PositionZ = 33.29f;
            l_Orientation = 3.97f;
        }

        p_Player->TeleportTo(1116, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);

        return true;
    }
};

void AddSC_GameObjectsFun()
{
    new go_ashran_portal();
}