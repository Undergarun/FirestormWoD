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
        Position l_Position;

        if (p_Player->GetTeam() == ALLIANCE)
        {
            l_Position.m_positionX = 3684.13f;
            l_Position.m_positionY = -3839.93f;
            l_Position.m_positionZ = 45.49f;
            l_Position.m_orientation = 3.4f;
        }
        else
        {
            l_Position.m_positionX = 5357.f;
            l_Position.m_positionY = -3943.61f;
            l_Position.m_positionZ = 33.29f;
            l_Position.m_orientation = 3.97f;
        }

        p_Player->TeleportTo(1116, l_Position.m_positionX, l_Position.m_positionY, l_Position.m_positionZ, l_Position.m_orientation);

        return true;
    }
};

void AddSC_GameObjectsFun()
{
    new go_ashran_portal();
}