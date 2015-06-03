////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

class go_tarren_mill_portal : public GameObjectScript
{
public:
    go_tarren_mill_portal() : GameObjectScript("go_tarren_mill_portal") { }

    bool OnGossipHello(Player* p_Player, GameObject* p_Go)
    {
        float l_PositionX = 0.f;
        float l_PositionY = 0.f;
        float l_PositionZ = 0.f;
        float l_Orientation = 0.f;

        if (p_Player->GetTeamId() == TeamId::TEAM_ALLIANCE)
        {
            l_PositionX = 1752.77f;
            l_PositionY = 1063.11f;
            l_PositionZ = 6.9f;
        }
        else
        {
            l_PositionX = 2621.11f;
            l_PositionY = 636.17;
            l_PositionZ = 55.97f;
            l_Orientation = 4.31f;
        }

        p_Player->TeleportTo(1280, l_PositionX, l_PositionY, l_PositionZ, l_Orientation);

        return true;
    }
};

void AddSC_GameObjectsFun()
{
    new go_tarren_mill_portal();
}