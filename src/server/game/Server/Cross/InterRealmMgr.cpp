#ifdef CROSS
#include "InterRealmMgr.h"
#include "InterRealmOpcodes.h"
#include "World.h"
#include "Config.h"

InterRealmMgr::InterRealmMgr()
{
    //int num_threads(sWorld->getIntConfig(CONFIG_NUMTHREADS));

    //if (num_threads > 0 && m_updater.activate(num_threads) == -1)
    //    abort();
}

bool InterRealmMgr::LoadConfig(bool reload)
{
    if (reload)
    {
        ConfigMgr::Load();
        configs.clear();
    }

    std::string realm_numbers = ConfigMgr::GetStringDefault("InterRealmServer.Realms", "");
    Tokenizer tokens(realm_numbers, ',');
    for (Tokenizer::const_iterator iter = tokens.begin(); iter != tokens.end(); ++iter)
    {
        uint32 realm_number = atoi(*iter);

        configs[realm_number] = InterRealmDatabaseConfig();
        InterRealmDatabaseConfig &config = configs[realm_number];

        config.realm_nubmer = realm_number;

        std::string realm_number_string = "InterRealmServer";
        realm_number_string.append((*iter));

        config.enabled = ConfigMgr::GetIntDefault(std::string(realm_number_string + ".Enabled").c_str(), 0);
        if (config.enabled == 0)
            continue;

        config.name                  = ConfigMgr::GetStringDefault(std::string(realm_number_string + ".Name").c_str(), "");
        config.connection_info       = ConfigMgr::GetStringDefault(std::string(realm_number_string + ".CharacterDatabaseInfo").c_str(), "");
        config.groupRealmMask        = ConfigMgr::GetIntDefault(std::string(realm_number_string + ".AccountBind.GroupRealmMask").c_str(), 1);
        config.allowedGroupRealmMask = ConfigMgr::GetIntDefault(std::string(realm_number_string + ".AccountBind.AllowedGroupRealmMask").c_str(), 7);

        if (config.connection_info.empty())
        {
            sLog->outError(LOG_FILTER_INTERREALM, "InterRealm connection info is empty for %s realm, disabling it", config.name.c_str());
            config.enabled = 0;
        }
    }
    uint32 counter = 0;
    for (ConfigMap::iterator itr = configs.begin();  itr != configs.end(); ++itr)
    {
        if ((*itr).second.enabled != 0)
            ++counter;
    }

    if (reload)
    {
        for (ClientMap::const_iterator itr = clients.begin();  itr != clients.end(); ++itr)
        {
            delete (*itr);
        }
        clients.clear();
    }

    sLog->outInfo(LOG_FILTER_INTERREALM, "InterRealm: Loaded %u InterRealm configs.", counter);
    return true;
}

const InterRealmDatabaseConfig* InterRealmMgr::GetConfig(uint32 realmId) const
{
    ConfigMap::const_iterator itr = configs.find(realmId);
    if (itr != configs.end())
        if (itr->second.enabled)
            return &itr->second;
    
    return NULL;
}

void InterRealmMgr::RegisterClient(InterRealmClient* client)
{
    Lock();
    clients.insert(client);
    Unlock();
}

InterRealmClient* InterRealmMgr::GetClientByRealmNumber(uint32 realmNumber)
{
    for (ClientMap::const_iterator itr = clients.begin(); itr != clients.end(); ++itr)
    {
        if ((*itr)->GetRealmId() == realmNumber)
            return (*itr);
    }

    return NULL;
}

void InterRealmMgr::Update(const uint32 diff)
{
    /*i_timer.Update(diff);
    if (i_timer.Passed())
    {
        if (!clients.empty())
        {
            for (ClientMap::const_iterator itr = clients.begin(); itr != clients.end(); ++itr)
            {
                if (InterRealmClient* client = (*itr))
                {
                    if (m_updater.activated())
                        m_updater.schedule_update(*client, uint32(i_timer.GetCurrent()));
                    else
                        client->Update(uint32(i_timer.GetCurrent()));
                }
            }
        }

        i_timer.SetCurrent(0);
        if (m_updater.activated())
            m_updater.wait();
    }*/

    //Lock();
    if (!clients.empty())
    {
        for (ClientMap::const_iterator itr = clients.begin(); itr != clients.end();)
        {
            if (InterRealmClient* client = (*itr))
            {
                if (client->IsNeedClose())
                {
                    client->RemoveAllPlayers();
                    delete client;
                    itr = clients.erase(itr);
                }
                else
                {
                    client->Update(diff);
                    ++itr;
                }
            }
        }
    }
    //Unlock();
}

void InterRealmMgr::SendPacketToAll(WorldPacket* packet)
{
    for (ClientMap::const_iterator itr = clients.begin(); itr != clients.end(); ++itr)
    {
        (*itr)->SendPacket(packet);
    }
}

void InterRealmMgr::SendBGStartAnnouncer(const char* bgName, uint32 min_level, uint32 max_level)
{
    WorldPacket packet(IR_SMSG_BG_START_ANNOUNCER, strlen(bgName) + 4 + 4);
    packet << std::string(bgName);
    packet << min_level;
    packet << max_level;
    SendPacketToAll(&packet);
}

InterRealmDatabaseClient::~InterRealmDatabaseClient()
{
    if (connection)
    {
        connection->Close();
        delete connection;
    }
    if (client)
    {
        // @TODO: rewrite closing connection
        //client->DoClose();
        delete client;
    }

}

InterRealmDatabaseClient::InterRealmDatabaseClient( const InterRealmDatabaseConfig& config, InterRealmClient* _client): client(0)
{
    connection = new InterRealmDatabasePool;
    if (!connection->Open(config.connection_info, 1, 1))
    {
        delete connection;
        connection = NULL;
        std::stringstream execption;
        execption << "[INTERREALM] InterRealm: Failed to open database for ";
        execption << config.name;
        execption << " realm";
        throw execption.str();
    }
    realm_nubmer = config.realm_nubmer;
    client = _client;
    sLog->outInfo(LOG_FILTER_INTERREALM, "InterRealm: Successfully connected to %s database", config.name.c_str());
}

InterRealmDatabaseClient::InterRealmDatabaseClient():
    client(0), connection(0), realm_nubmer(0)
{
}
#endif