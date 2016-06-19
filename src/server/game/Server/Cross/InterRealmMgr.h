#ifdef CROSS
#ifndef __INTERREALM_MGR_H__
#define __INTERREALM_MGR_H__

#include <ace/Singleton.h>
#include "InterRealmDb.h"
#include "InterRealmClient.h"

enum CheckRealmStatus
{
    CHECKREALM_VALID = 0,
    CHECKREALM_INVALID = 1,
};

struct InterRealmDatabaseConfig
{
    InterRealmDatabaseConfig():
        realm_nubmer(0), name(""), connection_info(""), enabled(0) {}
    
    uint32 realm_nubmer;
    std::string name;
    std::string connection_info;
    uint32 groupRealmMask;
    uint32 allowedGroupRealmMask;
    uint8 enabled;
};

struct InterRealmDatabaseClient
{
    InterRealmDatabaseClient();
    InterRealmDatabaseClient(const InterRealmDatabaseConfig& config, InterRealmClient* _client);
    ~InterRealmDatabaseClient();

    InterRealmClient* client;
    InterRealmDatabasePool* connection;
    uint32 realm_nubmer;
};

typedef std::unordered_map<uint32, InterRealmDatabaseConfig> ConfigMap;
typedef std::set<InterRealmClient*> ClientMap;


class InterRealmMgr
{
    friend class ACE_Singleton<InterRealmMgr, ACE_Null_Mutex>;
    private:
        InterRealmMgr();
        ~InterRealmMgr() {}
    public:

        bool LoadConfig(bool reload = false);
        const InterRealmDatabaseConfig* GetConfig(uint32 realmId) const;
        //bool TryToRegisterConnection(InterRealmClient* client, uint32 realm);
        //InterRealmDatabasePool* GetConnection(uint32 realm) const;
        //InterRealmDatabasePool* GetConnection(InterRealmClient* client) const;

        void RegisterClient(InterRealmClient* client);
        ClientMap const& GetClients() const { return clients; }
        InterRealmClient* GetClientByRealmNumber(uint32 realmNumber);

        void Update(const uint32 diff);

        void SendBGStartAnnouncer(const char* bgName, uint32 min_level, uint32 max_level);

        void SendPacketToAll(WorldPacket* packet);

    private:

        void Lock() { m_Mutex.lock(); }
        void Unlock() { m_Mutex.release(); }

    private:

        ConfigMap configs;
        ClientMap clients;
        ACE_Thread_Mutex m_Mutex;
};

#define sInterRealmMgr ACE_Singleton<InterRealmMgr, ACE_Null_Mutex>::instance()
#endif
#endif
