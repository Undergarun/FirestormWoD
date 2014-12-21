#include "ScriptPCH.h"
#include "end_time.h"
#include "GameObjectAI.h"

enum Spells
{
    SPELL_TELEPORT_TO_START                 = 102564, // Start
    SPELL_TELEPORT_TO_RUBY_DRAGONSHIRE      = 102579, // Sylvanas
    SPELL_TELEPORT_TO_EMERALD_DRAGONSHIRE   = 104761, // Tyrande
    SPELL_TELEPORT_TO_BLUE_DRAGONSHIRE      = 102126, // Jaina
    SPELL_TELEPORT_TO_OBSIDIAN_DRAGONSHIRE  = 103868, // Baine
    SPELL_TELEPORT_TO_BRONZE_DRAGONSHIRE    = 104764, // Murozond
};

enum InstanceTeleporter
{
    START_TELEPORT          = 1,
    JAINA_TELEPORT          = 2,
	SYLVANAS_TELEPORT       = 3,
	TYRANDE_TELEPORT        = 4,
	BAINE_TELEPORT          = 5,
	MUROZOND_TELEPORT       = 6,
};

class go_end_time_teleport : public GameObjectScript
{
    public:
        go_end_time_teleport() : GameObjectScript("go_end_time_teleport") { }

        bool OnGossipHello(Player* pPlayer, GameObject* pGo)
        {
            if (pPlayer->isInCombat())
                return true;

            if (InstanceScript* pInstance = pGo->GetInstanceScript())
            {
                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Start.", GOSSIP_SENDER_MAIN, START_TELEPORT);

                if (pPlayer->isGameMaster())
                {
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Tyrande.", GOSSIP_SENDER_MAIN, TYRANDE_TELEPORT);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Jaina.", GOSSIP_SENDER_MAIN, JAINA_TELEPORT);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Baine.", GOSSIP_SENDER_MAIN, BAINE_TELEPORT);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Sylvanas.", GOSSIP_SENDER_MAIN, SYLVANAS_TELEPORT);
                    pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Murozond.", GOSSIP_SENDER_MAIN, MUROZOND_TELEPORT);
                }
                else
                {
                    std::list<uint32> echo_list;
                    uint32 echo1 = pInstance->GetData(DATA_ECHO_1);
                    uint32 echo2 = pInstance->GetData(DATA_ECHO_2);

                    switch (echo1)
                    {
                        case DATA_ECHO_OF_JAINA:
                            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Jaina.", GOSSIP_SENDER_MAIN, JAINA_TELEPORT);
                            break;
                        case DATA_ECHO_OF_BAINE:
                            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Baine.", GOSSIP_SENDER_MAIN, BAINE_TELEPORT);
                            break;
                        case DATA_ECHO_OF_TYRANDE:
                            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Tyrande.", GOSSIP_SENDER_MAIN, TYRANDE_TELEPORT);
                            break;
                        case DATA_ECHO_OF_SYLVANAS:
                            pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Sylvanas.", GOSSIP_SENDER_MAIN, SYLVANAS_TELEPORT);
                            break;
                    }

                    if (pInstance->GetData(DATA_FIRST_ENCOUNTER) == DONE)
                    {
                        switch (echo2)
                        {
                            case DATA_ECHO_OF_JAINA:
                                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Jaina.", GOSSIP_SENDER_MAIN, JAINA_TELEPORT);
                                break;
                            case DATA_ECHO_OF_BAINE:
                                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Baine.", GOSSIP_SENDER_MAIN, BAINE_TELEPORT);
                                break;
                            case DATA_ECHO_OF_TYRANDE:
                                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Tyrande.", GOSSIP_SENDER_MAIN, TYRANDE_TELEPORT);
                                break;
                            case DATA_ECHO_OF_SYLVANAS:
                                pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Sylvanas.", GOSSIP_SENDER_MAIN, SYLVANAS_TELEPORT);
                                break;
                        }
                    }

                    if (pInstance->GetData(DATA_SECOND_ENCOUNTER) == DONE)
                        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, "Teleport to Murozond.", GOSSIP_SENDER_MAIN, MUROZOND_TELEPORT);
                
                }
            }
        
            pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pGo), pGo->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* go, uint32 sender, uint32 action) 
		{
            //player->PlayerTalkClass->ClearMenus();
            if (player->isInCombat())
                return true;

            InstanceScript* pInstance = player->GetInstanceScript();
            if (!pInstance)
                return true;
            
            switch (action) 
		    {
                case START_TELEPORT:
                    player->CastSpell(player, SPELL_TELEPORT_TO_START, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case JAINA_TELEPORT:
                    pInstance->SetData(DATA_JAINA_EVENT, IN_PROGRESS);
                    if (pInstance->GetData(DATA_NOZDORMU_3) != DONE)
                        pInstance->SetData(DATA_NOZDORMU_3, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_BLUE_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case SYLVANAS_TELEPORT:
                    if (pInstance->GetData(DATA_NOZDORMU_4) != DONE)
                        pInstance->SetData(DATA_NOZDORMU_4, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_RUBY_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case TYRANDE_TELEPORT:
                    if (pInstance->GetData(DATA_NOZDORMU_1) != DONE)
                        pInstance->SetData(DATA_NOZDORMU_1, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_EMERALD_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case BAINE_TELEPORT:
                    if (pInstance->GetData(DATA_NOZDORMU_2) != DONE)
                        pInstance->SetData(DATA_NOZDORMU_2, IN_PROGRESS);
                    player->CastSpell(player, SPELL_TELEPORT_TO_OBSIDIAN_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
                case MUROZOND_TELEPORT:
                    player->CastSpell(player, SPELL_TELEPORT_TO_BRONZE_DRAGONSHIRE, true);
                    player->CLOSE_GOSSIP_MENU();
                    break;
            }
            
            return true;
        }    
};

void AddSC_end_time_teleport()
{
    new go_end_time_teleport();
}