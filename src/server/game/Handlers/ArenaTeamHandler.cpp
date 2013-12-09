/*
 * Copyright (C) 2008-2012 TrinityCore <http://www.trinitycore.org/>
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "Player.h"
#include "World.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "DatabaseEnv.h"

#include "Arena.h"
#include "Log.h"
#include "ObjectMgr.h"
#include "SocialMgr.h"

void WorldSession::HandleInspectArenaTeamsOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "MSG_INSPECT_ARENA_TEAMS");
}

void WorldSession::HandleArenaTeamQueryOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ARENA_TEAM_QUERY");
}

void WorldSession::HandleArenaTeamRosterOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ARENA_TEAM_ROSTER");
}

void WorldSession::HandleArenaTeamInviteOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_INVITE");
}

void WorldSession::HandleArenaTeamAcceptOpcode(WorldPacket & /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_ACCEPT");                // empty opcode
}

void WorldSession::HandleArenaTeamCreateOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_CREATE");                // empty opcode
}

void WorldSession::HandleArenaTeamDeclineOpcode(WorldPacket & /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_DECLINE");               // empty opcode
}

void WorldSession::HandleArenaTeamLeaveOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_LEAVE");
}

void WorldSession::HandleArenaTeamDisbandOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_DISBAND");
}

void WorldSession::HandleArenaTeamRemoveOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_REMOVE");
}

void WorldSession::HandleArenaTeamLeaderOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "CMSG_ARENA_TEAM_LEADER");
}

/*
+ERR_ARENA_NO_TEAM_II "You are not in a %dv%d arena team"

+ERR_ARENA_TEAM_CREATE_S "%s created.  To disband, use /teamdisband [2v2, 3v3, 5v5]."
+ERR_ARENA_TEAM_INVITE_SS "You have invited %s to join %s"
+ERR_ARENA_TEAM_QUIT_S "You are no longer a member of %s"
ERR_ARENA_TEAM_FOUNDER_S "Congratulations, you are a founding member of %s!  To leave, use /teamquit [2v2, 3v3, 5v5]."

+ERR_ARENA_TEAM_INTERNAL "Internal arena team error"
+ERR_ALREADY_IN_ARENA_TEAM "You are already in an arena team of that size"
+ERR_ALREADY_IN_ARENA_TEAM_S "%s is already in an arena team of that size"
+ERR_INVITED_TO_ARENA_TEAM "You have already been invited into an arena team"
+ERR_ALREADY_INVITED_TO_ARENA_TEAM_S "%s has already been invited to an arena team"
+ERR_ARENA_TEAM_NAME_INVALID "That name contains invalid characters, please enter a new name"
+ERR_ARENA_TEAM_NAME_EXISTS_S "There is already an arena team named \"%s\""
+ERR_ARENA_TEAM_LEADER_LEAVE_S "You must promote a new team captain using /teamcaptain before leaving the team"
+ERR_ARENA_TEAM_PERMISSIONS "You don't have permission to do that"
+ERR_ARENA_TEAM_PLAYER_NOT_IN_TEAM "You are not in an arena team of that size"
+ERR_ARENA_TEAM_PLAYER_NOT_IN_TEAM_SS "%s is not in %s"
+ERR_ARENA_TEAM_PLAYER_NOT_FOUND_S "\"%s\" not found"
+ERR_ARENA_TEAM_NOT_ALLIED "You cannot invite players from the opposing alliance"

+ERR_ARENA_TEAM_JOIN_SS "%s has joined %s"
+ERR_ARENA_TEAM_YOU_JOIN_S "You have joined %s.  To leave, use /teamquit [2v2, 3v3, 5v5]."

+ERR_ARENA_TEAM_LEAVE_SS "%s has left %s"

+ERR_ARENA_TEAM_LEADER_IS_SS "%s is the captain of %s"
+ERR_ARENA_TEAM_LEADER_CHANGED_SSS "%s has made %s the new captain of %s"

+ERR_ARENA_TEAM_REMOVE_SSS "%s has been kicked out of %s by %s"

+ERR_ARENA_TEAM_DISBANDED_S "%s has disbanded %s"

ERR_ARENA_TEAM_TARGET_TOO_LOW_S "%s is not high enough level to join your team"

ERR_ARENA_TEAM_TOO_MANY_MEMBERS_S "%s is full"

ERR_ARENA_TEAM_LEVEL_TOO_LOW_I "You must be level %d to form an arena team"
*/
