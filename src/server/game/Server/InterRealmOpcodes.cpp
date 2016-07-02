#ifndef CROSS
#include "InterRealmOpcodes.h"

IROpcodeTable IRopcodeTable;

void IROpcodeTable::SetIROpcode(uint16 opcode, char const* name, pIROpcodeHandler handler)
{
    _internalTable[opcode] = new IROpcodeHandler(name, handler);
}

void IROpcodeTable::Initialize()
{
#define DEFINE_IR_OPCODE_HANDLER(opcode, handler) \
    SetIROpcode(opcode, #opcode, handler);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_HELLO, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_HELLO, &InterRealmSession::Handle_Hello);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_WHO_AM_I, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_WHO_AM_I, &InterRealmSession::Handle_WhoAmI);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_TUNNEL_PACKET, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_TUNNEL_PACKET, &InterRealmSession::Handle_TunneledPacket);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_REGISTER_PLAYER, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_REGISTER_PLAYER_RESP, &InterRealmSession::Handle_RegisterPlayerResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_REGISTER_RATED, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_REGISTER_RATED_RESP, &InterRealmSession::Handle_RegisterRatedResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_UNREGISTER_PLAYER, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_UNREGISTER_PLAYER_RESP, &InterRealmSession::Handle_UnRegisterPlayerResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_PLAYER_LOGOUT, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_PLAYER_LOGOUT_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BATTLEGROUND_LIST_QUERY, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BATTLEFIELD_PORT, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BATTLEFIELD_PORT_RESP, &InterRealmSession::Handle_BattlefieldPortResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BATTLEFIELD_ENTER, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BATTLEFIELD_STATUS_QUERY, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BATTLEFIELD_LEAVE, &InterRealmSession::Handle_BattlefieldLeave);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_CHECK_PLAYERS, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_CHECK_PLAYERS, &InterRealmSession::Handle_CheckPlayers);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_CREATE, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_ROSTER, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_ROSTER_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_QUERY, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_QUERY_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_LOAD, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_LOAD_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_INVITE, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_INVITE_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_ACCEPT, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_LEAVE, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_DISBAND, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_SEND_ERROR, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_EVENT, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_UPDATE_FIELDS, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_TEAM_UPDATE_FIELDS_TWO, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_INSPECT, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ARENA_TEAM_INSPECT_RESP, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_REGISTER_GROUP, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_REGISTER_GROUP_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_REGISTER_ARENA, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_REGISTER_ARENA_RESP, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_DEBUG_BG, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_DEBUG_BG_RESP, &InterRealmSession::Handle_DebugBGResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_DEBUG_ARENA, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_DEBUG_ARENA_RESP, &InterRealmSession::Handle_DebugArenaResp);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_CURRENCY_CAP_LOAD, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_CURRENCY_CAP_LOAD_RESP, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BG_QUEUE_ANNOUNCER, &InterRealmSession::Handle_BGQueueAnnouncer);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_QUEUE_ANNOUNCER, &InterRealmSession::Handle_ArenaQueueAnnouncer);

    DEFINE_IR_OPCODE_HANDLER(IR_MSG_WHISPER, &InterRealmSession::Handle_Whisper);
    DEFINE_IR_OPCODE_HANDLER(IR_MSG_SOCIAL_LIST, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_TUNNELED_PACKET_TO_CLIENT, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_PLAYER_BG_ZONE_ID, &InterRealmSession::Handle_PlayerZoneID);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_PLAYER_PINFO, &InterRealmSession::Handle_PlayerPInfo);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_PLAYER_PINFO, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BAN, &InterRealmSession::Handle_Ban);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BAN_CHARACTER, &InterRealmSession::Handle_BanCharacter);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BAN, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BAN_CHARACTER, &InterRealmSession::Handle_Null);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_UNBAN, &InterRealmSession::Handle_Unban);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_UNBAN_CHARACTER, &InterRealmSession::Handle_UnbanCharacter);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_UNBAN_CHARACTER, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_SERVER_ANNOUNCE, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_KICK_CHARACTER, &InterRealmSession::Handle_KickPlayer);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_TELEPORT_PLAYER, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_MSG_GUILD_CHAT, &InterRealmSession::Handle_GuildChat);
    DEFINE_IR_OPCODE_HANDLER(IR_SUMMON_PLAYER, &InterRealmSession::Handle_SummonPlayer);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_REGISTER_RATED, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_REGISTER_RATED_RESP, &InterRealmSession::Handle_Null);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ARENA_LIST, &InterRealmSession::Handle_SpectatorData);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BATTLEGROUND_HOLIDAY, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BATTLEGROUND_HOLIDAY_RESP, &InterRealmSession::Handle_BattlegroundHolidayResp);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_BG_START_ANNOUNCER, &InterRealmSession::Handle_BattlegroundStartAnnoucer);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_REGISTER_SPECTATOR, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_REGISTER_SPECTATOR_RESP, &InterRealmSession::Handle_RegisterSpectatorResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_SPECTATOR_PORT, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_CURRENCY_CAP_RESET, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ACHIEVEMENT_REWARD, &InterRealmSession::Handle_AchievementReward);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_SUMMON_PLAYER, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_APPEAR_REQUEST, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_APPEAR_REQUEST_RESP, &InterRealmSession::Handle_AppearRequestResp);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_APPEAR, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ADDITIONAL_INFO, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ADDITIONAL_INFO, &InterRealmSession::Handle_AdditionalInfo);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_ACCOUNT_SPELLS, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_ANTICHEAT_REPORT, &InterRealmSession::Handle_AnticheatReport);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_CHARACTER_SKILLS, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_GUILD, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_GUILD_QUERY, &InterRealmSession::Handle_GuildQuery);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_BATTLEFIELD_LEAVE, &InterRealmSession::Handle_ClientSide);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_RESERVE_LOCAL_GUID, &InterRealmSession::Handle_ReserveLocalGuid);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_RESERVE_LOCAL_GUID, &InterRealmSession::Handle_ReserveLocalGuid);

    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_CROSS_PARTY_INFO, &InterRealmSession::Handle_CrossPartyInfo);

    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_PLAYER_RECONNECT, &InterRealmSession::Handle_ClientSide);
    DEFINE_IR_OPCODE_HANDLER(IR_SMSG_PLAYER_RECONNECT_RESULT, &InterRealmSession::Handle_PlayerReconnectResult);
    DEFINE_IR_OPCODE_HANDLER(IR_CMSG_PLAYER_RECONNECT_READY_TO_LOAD, &InterRealmSession::Handle_ClientSide);


#undef DEFINE_IR_OPCODE_HANDLER
};
#endif