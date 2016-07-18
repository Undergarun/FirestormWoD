#ifndef INTERREALM_OPCODES_H
#define INTERREALM_OPCODES_H

#include "WorldPacket.h"
#ifndef CROSS
#include "InterRealmSession.h"
#else /* CROSS */
#include "InterRealmClient.h"
#endif /* CROSS */

enum IROpcodes
{
    IR_CMSG_HELLO                                   = 0x01,
    IR_SMSG_HELLO                                   = 0x02,
    IR_CMSG_WHO_AM_I                                = 0x03,
    IR_SMSG_WHO_AM_I                                = 0x04,
    IR_CMSG_TUNNEL_PACKET                           = 0x05,
    IR_SMSG_TUNNEL_PACKET                           = 0x06,
    IR_CMSG_REGISTER_PLAYER                         = 0x07,
    IR_SMSG_REGISTER_PLAYER_RESP                    = 0x08,
    IR_CMSG_UNREGISTER_PLAYER                       = 0x09,
    IR_SMSG_UNREGISTER_PLAYER_RESP                  = 0x0A,
    IR_CMSG_PLAYER_LOGOUT                           = 0x0B,
    IR_SMSG_PLAYER_LOGOUT_RESP                      = 0x0C,
    IR_CMSG_BATTLEGROUND_LIST_QUERY                 = 0x0D,
    IR_CMSG_BATTLEFIELD_PORT                        = 0x0E,
    IR_SMSG_BATTLEFIELD_PORT_RESP                   = 0x0F,
    IR_CMSG_BATTLEFIELD_ENTER                       = 0x10,
    IR_CMSG_BATTLEFIELD_STATUS_QUERY                = 0x11,
    IR_SMSG_BATTLEFIELD_LEAVE                       = 0x12,
    IR_CMSG_CHECK_PLAYERS                           = 0x13,
    IR_SMSG_CHECK_PLAYERS                           = 0x14,

    IR_CMSG_ARENA_TEAM_CREATE                       = 0x15,
    IR_CMSG_ARENA_TEAM_ROSTER                       = 0x16,
    IR_SMSG_ARENA_TEAM_ROSTER_RESP                  = 0x17,
    IR_CMSG_ARENA_TEAM_QUERY                        = 0x18,
    IR_SMSG_ARENA_TEAM_QUERY_RESP                   = 0x19,
    IR_CMSG_ARENA_TEAM_LOAD                         = 0x1A,
    IR_SMSG_ARENA_TEAM_LOAD_RESP                    = 0x1B,
    IR_CMSG_ARENA_TEAM_INVITE                       = 0x1C,
    IR_SMSG_ARENA_TEAM_INVITE_RESP                  = 0x1D,
    IR_CMSG_ARENA_TEAM_ACCEPT                       = 0x1E,
    IR_CMSG_ARENA_TEAM_LEAVE                        = 0x1F,
    IR_CMSG_ARENA_TEAM_DISBAND                      = 0x20,
    IR_CMSG_ARENA_TEAM_REMOVE                       = 0x21,
    IR_CMSG_ARENA_TEAM_LEADER                       = 0x22,
    IR_SMSG_ARENA_TEAM_SEND_ERROR                   = 0x23,
    IR_SMSG_ARENA_TEAM_EVENT                        = 0x24,
    IR_SMSG_ARENA_TEAM_UPDATE_FIELDS                = 0x25,
    IR_SMSG_ARENA_TEAM_UPDATE_FIELDS_TWO            = 0x26,
    IR_CMSG_ARENA_TEAM_INSPECT                      = 0x27,
    IR_CMSG_ARENA_TEAM_INSPECT_RESP                 = 0x28,

    IR_CMSG_REGISTER_GROUP                          = 0x29,
    IR_SMSG_REGISTER_GROUP_RESP                     = 0x2A,
    IR_CMSG_REGISTER_ARENA                          = 0x2B,
    IR_SMSG_REGISTER_ARENA_RESP                     = 0x2C,

    IR_CMSG_DEBUG_BG                                = 0x2D,
    IR_SMSG_DEBUG_BG_RESP                           = 0x2E,
    IR_CMSG_DEBUG_ARENA                             = 0x2F,
    IR_SMSG_DEBUG_ARENA_RESP                        = 0x30,

    IR_CMSG_CURRENCY_CAP_LOAD                       = 0x31,
    IR_SMSG_CURRENCY_CAP_LOAD_RESP                  = 0x32,

    IR_SMSG_BG_QUEUE_ANNOUNCER                      = 0x33,
    IR_SMSG_ARENA_QUEUE_ANNOUNCER                   = 0x34,

    IR_MSG_WHISPER                                  = 0x35,
    IR_MSG_SOCIAL_LIST                              = 0x36,

    IR_TUNNELED_PACKET_TO_CLIENT                    = 0x37,

    IR_SMSG_PLAYER_BG_ZONE_ID                       = 0x38,

    IR_CMSG_PLAYER_PINFO                            = 0x39,
    IR_SMSG_PLAYER_PINFO                            = 0x3A,

    IR_CMSG_BAN_CHARACTER                           = 0x3B,
    IR_CMSG_BAN                                     = 0x3C,
    IR_SMSG_BAN_CHARACTER                           = 0x3D,
    IR_SMSG_BAN                                     = 0x3E,
    IR_CMSG_UNBAN_CHARACTER                         = 0x3F,
    IR_CMSG_UNBAN                                   = 0x40,
    IR_SMSG_UNBAN_CHARACTER                         = 0x41,
    IR_SMSG_SERVER_ANNOUNCE                         = 0x42,
    IR_CMSG_KICK_CHARACTER                          = 0x43,

    IR_CMSG_TELEPORT_PLAYER                         = 0x44,

    IR_MSG_GUILD_CHAT                               = 0x45,

    IR_SUMMON_PLAYER                                = 0x46,

    IR_CMSG_REGISTER_RATED                          = 0x47,
    IR_SMSG_REGISTER_RATED_RESP                     = 0x48,

    IR_CMSG_RATED_BATTLEGROUND_STATS                = 0x49,
    IR_SMSG_RATED_BATTLEGROUND_STATS_RESP           = 0x4A,

    IR_SMSG_ARENA_LIST                              = 0x4B,

    IR_CMSG_BATTLEGROUND_HOLIDAY                    = 0x4C,
    IR_SMSG_BATTLEGROUND_HOLIDAY_RESP               = 0x4E,

    IR_SMSG_BG_START_ANNOUNCER                      = 0x4F,

    IR_CMSG_REGISTER_SPECTATOR                      = 0x50,
    IR_SMSG_REGISTER_SPECTATOR_RESP                 = 0x51,
    IR_CMSG_SPECTATOR_PORT                          = 0x52,

    IR_CMSG_PING                                    = 0x53,

    IR_CMSG_PLAYER_INFO                             = 0x54,
    IR_CMSG_ACHIEVEMENT_INFO                        = 0x55,
    IR_CMSG_CURRENCY_INFO                           = 0x56,
    IR_CMSG_SKILLS_INFO                             = 0x57,
    IR_CMSG_TALENTS_INFO                            = 0x58,
    IR_CMSG_SPELLS_INFO                             = 0x59,
    IR_CMSG_GLYPHS_INFO                             = 0x5A,
    IR_CMSG_AURAS_INFO                              = 0x5B,
    IR_CMSG_QUEST_STATUS_INFO                       = 0x5C,
    IR_CMSG_QUEST_STATUS_REWARDED_INFO              = 0x5D,
    IR_CMSG_DAILY_QUEST_STATUS_INFO                 = 0x5E,
    IR_CMSG_WEEKLY_QUEST_STATUS_INFO                = 0x5F,
    IR_CMSG_SEASONAL_QUEST_STATUS_INFO              = 0x60,
    IR_CMSG_REPUTATION_INFO                         = 0x61,
    IR_CMSG_INVENTORY_INFO                          = 0x62,

    IR_CMSG_CURRENCY_CAP_RESET                      = 0x63,

    IR_SMSG_ACHIEVEMENT_REWARD                      = 0x64,

    IR_CMSG_SUMMON_PLAYER                           = 0x65,

    IR_CMSG_APPEAR_REQUEST                          = 0x66,
    IR_SMSG_APPEAR_REQUEST_RESP                     = 0x67,
    IR_CMSG_APPEAR                                  = 0x68,

    IR_CMSG_ADDITIONAL_INFO                         = 0x69,
    IR_SMSG_ADDITIONAL_INFO                         = 0x6A,

    IR_CMSG_ACCOUNT_SPELLS                          = 0x6B,

    IR_SMSG_ANTICHEAT_REPORT                        = 0x6C,

    IR_CMSG_CHARACTER_SKILLS                        = 0x6D,

    IR_CMSG_GUILD                                   = 0x6E,
    IR_SMSG_GUILD_QUERY                             = 0x6F,

    IR_CMSG_BATTLEFIELD_LEAVE                       = 0x70,

    IR_SMSG_RESERVE_LOCAL_GUID                      = 0x71,
    IR_CMSG_RESERVE_LOCAL_GUID                      = 0x72,

    IR_SMSG_CROSS_PARTY_INFO                        = 0x73,

    IR_CMSG_PLAYER_RECONNECT                        = 0x74,
    IR_SMSG_PLAYER_RECONNECT_RESULT                 = 0x75,
    IR_CMSG_PLAYER_RECONNECT_READY_TO_LOAD          = 0x76,

    IR_NUM_MSG_TYPES,
};

#ifndef CROSS
typedef void (InterRealmSession::*pIROpcodeHandler)(WorldPacket& recvPacket);
#else /* CROSS */
typedef void(InterRealmClient::*pIROpcodeHandler)(WorldPacket& recvPacket);
#endif /* CROSS */

struct IROpcodeHandler
{
    IROpcodeHandler() {}
    IROpcodeHandler(char const* _name, pIROpcodeHandler _handler) : name(_name), IRhandler(_handler) {}

    char const* name;
    pIROpcodeHandler IRhandler;
};

class IROpcodeTable
{
    public:
        IROpcodeTable()
        {
            memset(_internalTable, 0, sizeof(_internalTable));
        }

        ~IROpcodeTable()
        {
            for (uint32 i = 0; i < IR_NUM_MSG_TYPES; ++i)
                delete _internalTable[i];
        }

        void Initialize();

        IROpcodeHandler* operator[] (uint32 index) const
        {
            return _internalTable[index];
        }

    private:
        
        void SetIROpcode(uint16 opcode, char const* name, pIROpcodeHandler handler);

        IROpcodeTable(IROpcodeTable const&);
        IROpcodeTable& operator=(IROpcodeTable const&);

        IROpcodeHandler* _internalTable[IR_NUM_MSG_TYPES];
};

extern IROpcodeTable IRopcodeTable;

inline std::string GetIROpcodeNameForLogging(uint16 id)
{
    uint32 opcode = uint32(id);
    std::ostringstream ss;
    ss << '[';

    if (id < UNKNOWN_OPCODE)
    {
        if (IROpcodeHandler const* handler = IRopcodeTable[uint32(id) & 0x7FFF])
            ss << handler->name;
        else
            ss << "UNKNOWN OPCODE";
    }
    else
        ss << "UNKNOWN OPCODE";

    ss << " 0x" << std::hex << std::uppercase << opcode << std::nouppercase << " (" << std::dec << opcode << ")]";
    return ss.str();
}

#endif
