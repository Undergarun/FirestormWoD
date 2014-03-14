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

#include "Common.h"
#include "DatabaseEnv.h"
#include "Opcodes.h"
#include "Log.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "World.h"
#include "ObjectMgr.h"
#include "GroupMgr.h"
#include "Player.h"
#include "Group.h"
#include "SocialMgr.h"
#include "Util.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "DB2Structure.h"
#include "DB2Stores.h"
#include "SpellAuraEffects.h"

class Aura;

/* differeces from off:
-you can uninvite yourself - is is useful
-you can accept invitation even if leader went offline
*/
/* todo:
-group_destroyed msg is sent but not shown
-reduce xp gaining when in raid group
-quest sharing has to be corrected
-FIX sending PartyMemberStats
*/

void WorldSession::SendPartyResult(PartyOperation operation, const std::string& member, PartyResult res, uint32 val /* = 0 */)
{
    WorldPacket data(SMSG_PARTY_COMMAND_RESULT, 4 + member.size() + 1 + 4 + 4 + 8);
    data << uint32(operation);
    data << member;
    data << uint32(res);
    data << uint32(val);                                    // LFD cooldown related (used with ERR_PARTY_LFG_BOOT_COOLDOWN_S and ERR_PARTY_LFG_BOOT_NOT_ELIGIBLE_S)
    data << uint64(0); // player who caused error (in some cases).

    SendPacket(&data);
}

void WorldSession::HandleGroupInviteOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_INVITE");

    time_t now = time(NULL);
    if (now - timeLastGroupInviteCommand < 5)
        return;
    else
       timeLastGroupInviteCommand = now;

    ObjectGuid crossRealmGuid; // unused

    recvData.read_skip<uint32>(); // Non-zero in cross realm invites
    recvData.read_skip<uint32>(); // Always 0
    recvData.read_skip<uint8>();

    std::string realmName, memberName;
    realmName = "";
    memberName = "";

    crossRealmGuid[7] = recvData.ReadBit();
    crossRealmGuid[0] = recvData.ReadBit();
    crossRealmGuid[4] = recvData.ReadBit();
    crossRealmGuid[3] = recvData.ReadBit();
    crossRealmGuid[6] = recvData.ReadBit();

    uint8 realmLen = recvData.ReadBits(9);
    recvData.ReadBit(); // unk

    crossRealmGuid[2] = recvData.ReadBit();
    crossRealmGuid[5] = recvData.ReadBit();
    crossRealmGuid[1] = recvData.ReadBit();

    uint8 nameLen = recvData.ReadBits(9);
    recvData.ReadBit(); // unk

    recvData.FlushBits();

    recvData.ReadByteSeq(crossRealmGuid[0]);
    recvData.ReadByteSeq(crossRealmGuid[4]);
    recvData.ReadByteSeq(crossRealmGuid[5]);
    recvData.ReadByteSeq(crossRealmGuid[7]);
    recvData.ReadByteSeq(crossRealmGuid[1]);

    if (realmLen > 0)
        realmName = recvData.ReadString((realmLen/2)); // unused

    recvData.ReadByteSeq(crossRealmGuid[3]);

    if (nameLen > 0)
        memberName = recvData.ReadString((nameLen/2));

    recvData.ReadByteSeq(crossRealmGuid[6]);
    recvData.ReadByteSeq(crossRealmGuid[2]);

    // attempt add selected player

    // cheating
    if (!normalizePlayerName(memberName))
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    Player* player = sObjectAccessor->FindPlayerByName(memberName.c_str());

    // no player
    if (!player)
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // restrict invite to GMs
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_GM_GROUP) && !GetPlayer()->isGameMaster() && player->isGameMaster())
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // can't group with
    if (!GetPlayer()->isGameMaster() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && GetPlayer()->GetTeam() != player->GetTeam())
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_PLAYER_WRONG_FACTION);
        return;
    }
    if (GetPlayer()->GetInstanceId() != 0 && player->GetInstanceId() != 0 && GetPlayer()->GetInstanceId() != player->GetInstanceId() && GetPlayer()->GetMapId() == player->GetMapId())
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_TARGET_NOT_IN_INSTANCE_S);
        return;
    }
    // just ignore us
    if (player->GetInstanceId() != 0 && player->GetDungeonDifficulty() != GetPlayer()->GetDungeonDifficulty())
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_IGNORING_YOU_S);
        return;
    }

    if (player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_IGNORING_YOU_S);
        return;
    }

    ObjectGuid invitedGuid = player->GetGUID();

    Group* group = GetPlayer()->GetGroup();
    if (group && group->isBGGroup())
        group = GetPlayer()->GetOriginalGroup();

    Group* group2 = player->GetGroup();
    if (group2 && group2->isBGGroup())
        group2 = player->GetOriginalGroup();
    // player already in another group or invited
    if (group2 || player->GetGroupInvite())
    {
        SendPartyResult(PARTY_OP_INVITE, memberName, ERR_ALREADY_IN_GROUP_S);

        if (group2)
        {
            // tell the player that they were invited but it failed as they were already in a group
            WorldPacket data(SMSG_GROUP_INVITE, 45);

            data.WriteBit(0);                                   // Show RealmName
            data.WriteBit(0);                                   // Unk
            data.WriteBit(invitedGuid[4]);
            data.WriteBits(strlen(GetPlayer()->GetName()), 6);  // Inviter name length
            data.WriteBit(invitedGuid[2]);
            data.WriteBit(0);                                   // Unk
            data.WriteBit(0);                                   // Unk - This force sending invite pannel
            data.WriteBit(invitedGuid[3]);
            data.WriteBit(invitedGuid[7]);
            data.WriteBits(0, 8);                               // Realm name
            data.WriteBit(0);                                   // Pair
            data.WriteBit(invitedGuid[0]);
            data.WriteBit(invitedGuid[5]);
            data.WriteBits(0, 22);                              // Count 2
            data.WriteBit(invitedGuid[6]);
            data.WriteBit(invitedGuid[1]);

            data << uint32(0);                                  // Unk - This send choose role on invite
            data << uint64(0);                                  // Unk
            data.WriteByteSeq(invitedGuid[3]);
            data.WriteByteSeq(invitedGuid[5]);
            data << uint32(0);

            if (strlen(GetPlayer()->GetName()))
                data.append(GetPlayer()->GetName(), strlen(GetPlayer()->GetName()));

            data << uint32(0);
            data.WriteByteSeq(invitedGuid[7]);
            data.WriteByteSeq(invitedGuid[4]);
            data.WriteByteSeq(invitedGuid[1]);
            data.WriteByteSeq(invitedGuid[2]);
            data.WriteByteSeq(invitedGuid[6]);
            data.WriteByteSeq(invitedGuid[0]);

            player->GetSession()->SendPacket(&data);
        }

        return;
    }

    if (group)
    {
        // not have permissions for invite
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_NOT_LEADER);
            return;
        }
        // not have place
        if (group->IsFull())
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_GROUP_FULL);
            return;
        }
    }

    // ok, but group not exist, start a new group
    // but don't create and save the group to the DB until
    // at least one person joins
    if (!group)
    {
        group = new Group;

        // new group: if can't add then delete
        if (!group->AddLeaderInvite(GetPlayer()))
        {
            delete group;
            return;
        }
        if (!group->AddInvite(player))
        {
            delete group;
            return;
        }

        group->Create(GetPlayer());
        sGroupMgr->AddGroup(group);
    }
    else
    {
        // already existed group: if can't add then just leave
        if (!group->AddInvite(player))
        {
            return;
        }
    }

    // ok, we do it

    WorldPacket data(SMSG_GROUP_INVITE, 45);

    data.WriteBit(0);                                   // Show RealmName
    data.WriteBit(0);
    data.WriteBit(invitedGuid[4]);
    data.WriteBits(strlen(GetPlayer()->GetName()), 6);  // Inviter name length
    data.WriteBit(invitedGuid[2]);
    data.WriteBit(0);
    data.WriteBit(1);
    data.WriteBit(invitedGuid[3]);
    data.WriteBit(invitedGuid[7]);
    data.WriteBits(0, 8);                               // Realm name
    data.WriteBit(0);
    data.WriteBit(invitedGuid[0]);
    data.WriteBit(invitedGuid[5]);
    data.WriteBits(0, 22);                              // Count 2
    data.WriteBit(invitedGuid[6]);
    data.WriteBit(invitedGuid[1]);

    data << uint32(0);                                  // Unk - This send choose role on invite
    data << uint64(0);                                  // Unk
    data.WriteByteSeq(invitedGuid[3]);
    data.WriteByteSeq(invitedGuid[5]);
    data << uint32(0);                                  // Unk

    if (strlen(GetPlayer()->GetName()))
        data.append(GetPlayer()->GetName(), strlen(GetPlayer()->GetName()));

    data << uint32(0);                                  // Unk
    data.WriteByteSeq(invitedGuid[7]);
    data.WriteByteSeq(invitedGuid[4]);
    data.WriteByteSeq(invitedGuid[1]);
    data.WriteByteSeq(invitedGuid[2]);
    data.WriteByteSeq(invitedGuid[6]);
    data.WriteByteSeq(invitedGuid[0]);

    player->GetSession()->SendPacket(&data);

    SendPartyResult(PARTY_OP_INVITE, memberName, ERR_PARTY_RESULT_OK);
}

void WorldSession::HandleGroupInviteResponseOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_INVITE_RESPONSE");

    recvData.read_skip<uint8>(); // unk
    bool accept = recvData.ReadBit();
    recvData.ReadBit();

    Group* group = GetPlayer()->GetGroupInvite();
    if (!group)
        return;

    if (accept)
    {
        // Remove player from invitees in any case
        group->RemoveInvite(GetPlayer());

        if (group->GetLeaderGUID() == GetPlayer()->GetGUID())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "HandleGroupAcceptOpcode: player %s(%d) tried to accept an invite to his own group", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
            return;
        }

        // Group is full
        if (group->IsFull())
        {
            SendPartyResult(PARTY_OP_INVITE, "", ERR_GROUP_FULL);
            return;
        }

        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

        // Forming a new group, create it
        if (!group->IsCreated())
        {
            // This can happen if the leader is zoning. To be removed once delayed actions for zoning are implemented
            if (!leader)
            {
                group->RemoveAllInvites();
                return;
            }

            // If we're about to create a group there really should be a leader present
            ASSERT(leader);
            group->RemoveInvite(leader);
            group->Create(leader);
            sGroupMgr->AddGroup(group);
        }

        // Everything is fine, do it, PLAYER'S GROUP IS SET IN ADDMEMBER!!!
        if (!group->AddMember(GetPlayer()))
            return;

        group->BroadcastGroupUpdate();
    }
    else
    {
        // Remember leader if online (group pointer will be invalid if group gets disbanded)
        Player* leader = ObjectAccessor::FindPlayer(group->GetLeaderGUID());

        // uninvite, group can be deleted
        GetPlayer()->UninviteFromGroup();

        if (!leader || !leader->GetSession())
            return;

        // report
        std::string name = std::string(GetPlayer()->GetName());
        WorldPacket data(SMSG_GROUP_DECLINE, name.length());
        data << name.c_str();
        leader->GetSession()->SendPacket(&data);
    }
}

void WorldSession::HandleGroupUninviteGuidOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_UNINVITE_GUID");

    ObjectGuid guid;
    std::string unkstring;

    recvData.read_skip<uint8>(); // unk 0x00

    guid[5] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[0] = recvData.ReadBit();

    uint8 stringSize = recvData.ReadBits(8);

    guid[7] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 4, 0, 2, 7, 1, 5, 6, 3 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    unkstring = recvData.ReadString(stringSize);

    // Can't uninvite yourself
    if (guid == GetPlayer()->GetGUID())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleGroupUninviteGuidOpcode: leader %s(%d) tried to uninvite himself from the group.", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != ERR_PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (grp->IsLeader(guid))
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", ERR_NOT_LEADER);
        return;
    }

    if (grp->IsMember(guid))
    {
        Player::RemoveFromGroup(grp, guid, GROUP_REMOVEMETHOD_KICK, GetPlayer()->GetGUID(), unkstring.c_str());
        return;
    }

    if (Player* player = grp->GetInvited(guid))
    {
        player->UninviteFromGroup();
        return;
    }

    SendPartyResult(PARTY_OP_UNINVITE, "", ERR_TARGET_NOT_IN_GROUP_S);
}

void WorldSession::HandleGroupUninviteOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_UNINVITE");

    std::string membername;
    recvData >> membername;

    // player not found
    if (!normalizePlayerName(membername))
        return;

    // can't uninvite yourself
    if (GetPlayer()->GetName() == membername)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandleGroupUninviteOpcode: leader %s(%d) tried to uninvite himself from the group.", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    PartyResult res = GetPlayer()->CanUninviteFromGroup();
    if (res != ERR_PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_OP_UNINVITE, "", res);
        return;
    }

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (uint64 guid = grp->GetMemberGUID(membername))
    {
        Player::RemoveFromGroup(grp, guid, GROUP_REMOVEMETHOD_KICK, GetPlayer()->GetGUID());
        return;
    }

    if (Player* player = grp->GetInvited(membername))
    {
        player->UninviteFromGroup();
        return;
    }

    SendPartyResult(PARTY_OP_UNINVITE, membername, ERR_TARGET_NOT_IN_GROUP_S);
}

void WorldSession::HandleGroupSetLeaderOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_SET_LEADER");

    ObjectGuid guid;
    recvData.read_skip<uint8>();

    uint8 bitOrder[8] = { 5, 2, 6, 7, 1, 0, 3, 4 };
    recvData.ReadBitInOrder(guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 6, 0, 5, 4, 3, 1, 2, 7 };
    recvData.ReadBytesSeq(guid, byteOrder);

    Player* player = ObjectAccessor::FindPlayer(guid);
    Group* group = GetPlayer()->GetGroup();

    if (!group || !player)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) || player->GetGroup() != group)
        return;

    // @TODO: find a better way to fix exploit, we must have possibility to change leader while group is in raid/instance
    // Prevent exploits with instance saves
    for (GroupReference *itr = group->GetFirstMember(); itr != NULL; itr = itr->next())
        if (Player* plr = itr->getSource())
            if (plr->GetMap() && plr->GetMap()->Instanceable())
                return;

    // Everything's fine, accepted.
    group->ChangeLeader(guid);
    group->SendUpdate();
}

void WorldSession::HandleGroupSetRolesOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_SET_ROLES");

    uint32 newRole = 0;
    uint8 unk = 0;
    ObjectGuid assignerGuid = GetPlayer()->GetGUID();   // Assigner GUID
    ObjectGuid targetGuid;                              // Target GUID

    Group* group = GetPlayer()->GetGroup();

    recvData >> unk;
    recvData >> newRole;

    uint8 bitOrder[8] = { 7, 4, 0, 2, 6, 5, 1, 3 };
    recvData.ReadBitInOrder(targetGuid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 0, 6, 3, 7, 1, 5, 4, 2 };
    recvData.ReadBytesSeq(targetGuid, byteOrder);

    WorldPacket data(SMSG_ROLE_CHANGED_INFORM, 24);

    if (group)
        data << uint32(group->getGroupMemberRole(targetGuid)); // Old Role
    else
        data << uint32(0);

    data << uint8(unk);
    data << uint32(newRole); // New Role

    data.WriteBit(targetGuid[0]);
    data.WriteBit(targetGuid[3]);
    data.WriteBit(assignerGuid[1]);
    data.WriteBit(assignerGuid[7]);
    data.WriteBit(targetGuid[5]);
    data.WriteBit(assignerGuid[4]);
    data.WriteBit(assignerGuid[3]);
    data.WriteBit(targetGuid[2]);
    data.WriteBit(targetGuid[7]);
    data.WriteBit(targetGuid[6]);
    data.WriteBit(assignerGuid[6]);
    data.WriteBit(targetGuid[4]);
    data.WriteBit(assignerGuid[0]);
    data.WriteBit(targetGuid[1]);
    data.WriteBit(assignerGuid[5]);
    data.WriteBit(assignerGuid[2]);

    data.WriteByteSeq(assignerGuid[3]);
    data.WriteByteSeq(targetGuid[2]);
    data.WriteByteSeq(targetGuid[6]);
    data.WriteByteSeq(assignerGuid[1]);
    data.WriteByteSeq(targetGuid[4]);
    data.WriteByteSeq(assignerGuid[0]);
    data.WriteByteSeq(targetGuid[1]);
    data.WriteByteSeq(assignerGuid[6]);
    data.WriteByteSeq(assignerGuid[2]);
    data.WriteByteSeq(targetGuid[7]);
    data.WriteByteSeq(targetGuid[5]);
    data.WriteByteSeq(targetGuid[3]);
    data.WriteByteSeq(assignerGuid[4]);
    data.WriteByteSeq(assignerGuid[7]);
    data.WriteByteSeq(targetGuid[0]);
    data.WriteByteSeq(targetGuid[5]);

    if (group)
    {
        group->setGroupMemberRole(targetGuid, newRole);
        group->SendUpdate();
        group->BroadcastPacket(&data, false);
    }
    else
        SendPacket(&data);
}

void WorldSession::HandleGroupDisbandOpcode(WorldPacket& /*recvData*/)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_DISBAND");

    Group* grp = GetPlayer()->GetGroup();
    if (!grp)
        return;

    if (_player->InBattleground())
    {
        SendPartyResult(PARTY_OP_INVITE, "", ERR_INVITE_RESTRICTED);
        return;
    }

    /** error handling **/
    /********************/

    // everything's fine, do it
    SendPartyResult(PARTY_OP_LEAVE, GetPlayer()->GetName(), ERR_PARTY_RESULT_OK);

    GetPlayer()->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
}

void WorldSession::HandleLootMethodOpcode(WorldPacket & recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LOOT_METHOD");

    uint8 lootMethod;
    ObjectGuid lootMaster;
    uint32 lootThreshold;

    recvData >> lootMethod;

    recvData.read_skip<uint8>();

    recvData >> lootThreshold;

    uint8 bitOrder[8] = { 6, 4, 7, 2, 5, 0, 1, 3 };
    recvData.ReadBitInOrder(lootMaster, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 4, 3, 0, 7, 6, 2, 1, 5 };
    recvData.ReadBytesSeq(lootMaster, byteOrder);

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    /** error handling **/
    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;
    /********************/

    // everything's fine, do it
    group->SetLootMethod((LootMethod)lootMethod);
    group->SetLooterGuid(lootMaster);
    group->SetLootThreshold((ItemQualities)lootThreshold);
    group->SendUpdate();
}

void WorldSession::HandleLootRoll(WorldPacket& recvData)
{
    ObjectGuid guid;
    uint8 itemSlot;
    uint8  rollType;

    recvData >> itemSlot; //always 0
    recvData >> rollType;              // 0: pass, 1: need, 2: greed

    uint8 bitOrder[8] = {5, 7, 2, 3, 4, 0, 6, 7};
    recvData.ReadBitInOrder(guid, bitOrder);

    uint8 byteOrder[8] = {2, 3, 7, 0, 6, 5, 1, 4};
    recvData.ReadBytesSeq(guid, byteOrder);

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->CountRollVote(GetPlayer()->GetGUID(), itemSlot, rollType);

    switch (rollType)
    {
    case ROLL_NEED:
        GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED, 1);
        break;
    case ROLL_GREED:
        GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED, 1);
        break;
    }
}

void WorldSession::HandleMinimapPingOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received MSG_MINIMAP_PING");

    if (!GetPlayer()->GetGroup())
        return;

    float x, y;
    uint8 unk;

    recvData >> y;
    recvData >> x;
    recvData >> unk;

    // everything's fine, do it
    ObjectGuid plrGuid = GetPlayer()->GetGUID();
    WorldPacket data(SMSG_MINIMAP_PING, (8+4+4));

    uint8 bits[8] = { 6, 5, 1, 2, 4, 0, 3, 7 };
    data.WriteBitInOrder(plrGuid, bits);

    data.WriteByteSeq(plrGuid[0]);
    data.WriteByteSeq(plrGuid[5]);
    data.WriteByteSeq(plrGuid[2]);
    data << float(x);
    data.WriteByteSeq(plrGuid[4]);
    data.WriteByteSeq(plrGuid[1]);
    data.WriteByteSeq(plrGuid[7]);
    data.WriteByteSeq(plrGuid[3]);
    data << float(y);
    data.WriteByteSeq(plrGuid[6]);

    GetPlayer()->GetGroup()->BroadcastPacket(&data, true, -1, GetPlayer()->GetGUID());
}

void WorldSession::HandleRandomRollOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RANDOM_ROLL");

    uint32 minimum, maximum, roll;
    uint8 unk;
    recvData >> minimum;
    recvData >> maximum;
    recvData >> unk;

    /** error handling **/
    if (minimum > maximum || maximum > 10000)                // < 32768 for urand call
        return;
    /********************/

    // everything's fine, do it
    roll = urand(minimum, maximum);

    WorldPacket data(SMSG_RANDOM_ROLL, 4+4+4+8);
    ObjectGuid guid = GetPlayer()->GetGUID();
    data << uint32(roll);
    data << uint32(maximum);
    data << uint32(minimum);

    uint8 bitsOrder[8] = { 4, 5, 2, 6, 0, 3, 1, 7 };
    data.WriteBitInOrder(guid, bitsOrder);

    uint8 bytesOrder[8] = { 2, 6, 1, 3, 4, 7, 0, 5 };
    data.WriteBytesSeq(guid, bytesOrder);

    if (GetPlayer()->GetGroup())
        GetPlayer()->GetGroup()->BroadcastPacket(&data, false);
    else
        SendPacket(&data);
}

void WorldSession::HandleRaidTargetUpdateOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RAID_TARGET_UPDATE");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint8 x, unk;
    recvData >> unk;
    recvData >> x;

    /** error handling **/
    /********************/

    // everything's fine, do it
    if (x == 0xFF)                                           // target icon request
        group->SendTargetIconList(this);
    else                                                    // target icon update
    {
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
            return;

        ObjectGuid guid;

        uint8 bitOrder[8] = { 2, 1, 6, 4, 5, 0, 7, 3 };
        recvData.ReadBitInOrder(guid, bitOrder);

        recvData.FlushBits();

        uint8 byteOrder[8] = { 5, 4, 6, 0, 1, 2, 3, 7 };
        recvData.ReadBytesSeq(guid, byteOrder);

        group->SetTargetIcon(x, _player->GetGUID(), guid);
    }
}

void WorldSession::HandleGroupRaidConvertOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_RAID_CONVERT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (_player->InBattleground())
        return;

    // Error handling
    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->GetMembersCount() < 2)
        return;

    // Everything's fine, do it (is it 0 (PARTY_OP_INVITE) correct code)
    SendPartyResult(PARTY_OP_INVITE, "", ERR_PARTY_RESULT_OK);

    // New 4.x: it is now possible to convert a raid to a group if member count is 5 or less

    bool unk;
    recvData >> unk;

    if (group->isRaidGroup())
        group->ConvertToGroup();
    else
        group->ConvertToRaid();
}

void WorldSession::HandleGroupChangeSubGroupOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_CHANGE_SUB_GROUP");

    // we will get correct pointer for group here, so we don't have to check if group is BG raid
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    time_t now = time(NULL);
    if (now - timeLastChangeSubGroupCommand < 2)
        return;
    else
       timeLastChangeSubGroupCommand = now;

    ObjectGuid guid;
    uint8 groupNr, unk;

    recvData >> unk >> groupNr;

    uint8 bitsOrder[8] = { 1, 3, 7, 2, 0, 5, 4, 6 };
    recvData.ReadBitInOrder(guid, bitsOrder);

    recvData.FlushBits();

    uint8 bytesOrder[8] = { 7, 0, 2, 4, 5, 3, 6, 1 };
    recvData.ReadBytesSeq(guid, bytesOrder);

    if (groupNr >= MAX_RAID_SUBGROUPS)
        return;

    uint64 senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        return;

    if (!group->HasFreeSlotSubGroup(groupNr))
        return;

    if (Player* movedPlayer = sObjectAccessor->FindPlayer(guid))
        group->ChangeMembersGroup(guid, groupNr);
}

void WorldSession::HandleGroupSwapSubGroupOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_SWAP_SUB_GROUP");
    uint8 unk1;
    ObjectGuid guid1;
    ObjectGuid guid2;
    uint8 unk2;

    recvData >> unk1;

    guid1[4] = recvData.ReadBit();
    guid1[6] = recvData.ReadBit();
    guid1[5] = recvData.ReadBit();
    guid1[0] = recvData.ReadBit();
    guid2[3] = recvData.ReadBit();
    guid2[4] = recvData.ReadBit();
    guid1[7] = recvData.ReadBit();
    guid1[2] = recvData.ReadBit();

    guid2[7] = recvData.ReadBit();
    guid2[1] = recvData.ReadBit();
    guid2[5] = recvData.ReadBit();
    guid2[6] = recvData.ReadBit();
    guid2[0] = recvData.ReadBit();
    guid1[3] = recvData.ReadBit();
    guid2[2] = recvData.ReadBit();
    guid1[1] = recvData.ReadBit();

    recvData.ReadByteSeq(guid2[0]);
    recvData.ReadByteSeq(guid1[5]);
    recvData.ReadByteSeq(guid1[0]);
    recvData.ReadByteSeq(guid2[7]);
    recvData.ReadByteSeq(guid1[6]);
    recvData.ReadByteSeq(guid2[1]);
    recvData.ReadByteSeq(guid2[5]);
    recvData.ReadByteSeq(guid1[7]);

    recvData.ReadByteSeq(guid1[4]);
    recvData.ReadByteSeq(guid1[3]);
    recvData.ReadByteSeq(guid2[3]);
    recvData.ReadByteSeq(guid1[1]);
    recvData.ReadByteSeq(guid1[4]);
    recvData.ReadByteSeq(guid2[6]);
    recvData.ReadByteSeq(guid2[2]);
    recvData.ReadByteSeq(guid2[2]);

    recvData >> unk2;
}

void WorldSession::HandleGroupEveryoneIsAssistantOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_EVERYONE_IS_ASSISTANT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;
    recvData.read_skip<uint8>();
    bool apply = recvData.ReadBit();
    recvData.FlushBits();

    group->ChangeFlagEveryoneAssistant(apply);
}

void WorldSession::HandleGroupAssistantLeaderOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_ASSISTANT_LEADER");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    ObjectGuid guid;
    bool apply;
    uint8 unk = 0;
    recvData >> unk;
    guid[0] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    apply = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();

    recvData.FlushBits();

    uint8 byteOrder[8] = { 6, 3, 2, 5, 7, 1, 0, 4 };
    recvData.ReadBytesSeq(guid, byteOrder);

    group->SetGroupMemberFlag(guid, apply, MEMBER_FLAG_ASSISTANT);

    group->SendUpdate();
}

void WorldSession::HandlePartyAssignmentOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_ASSIGNMENT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint64 senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        return;

    uint8 assignment, unk;
    bool apply;
    ObjectGuid guid;

    recvData >> assignment >> unk;

    guid[0] = recvData.ReadBit();
    apply = recvData.ReadBit();
    guid[4] = recvData.ReadBit();
    guid[2] = recvData.ReadBit();
    guid[1] = recvData.ReadBit();
    guid[3] = recvData.ReadBit();
    guid[6] = recvData.ReadBit();
    guid[5] = recvData.ReadBit();
    guid[7] = recvData.ReadBit();

    recvData.FlushBits();

    uint8 byteOrder[8] = { 5, 4, 7, 6, 3, 0, 1, 2 };
    recvData.ReadBytesSeq(guid, byteOrder);

    switch (assignment)
    {
        case GROUP_ASSIGN_MAINASSIST:
            group->RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINASSIST);
            group->SetGroupMemberFlag(guid, apply, MEMBER_FLAG_MAINASSIST);
            break;
        case GROUP_ASSIGN_MAINTANK:
            group->RemoveUniqueGroupMemberFlag(MEMBER_FLAG_MAINTANK);           // Remove main assist flag from current if any.
            group->SetGroupMemberFlag(guid, apply, MEMBER_FLAG_MAINTANK);
        default:
            break;
    }

    group->SendUpdate();
}

void WorldSession::HandleRaidLeaderReadyCheck(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RAID_LEADER_READY_CHECK");

    recvData.read_skip<uint8>(); // unk, 0x00

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        return;

    ObjectGuid groupGUID = group->GetGUID();
    ObjectGuid leaderGUID = GetPlayer()->GetGUID();

    group->SetReadyCheckCount(1);

    WorldPacket data(SMSG_RAID_READY_CHECK_STARTED);

    data.WriteBit(groupGUID[5]);
    data.WriteBit(groupGUID[3]);
    data.WriteBit(groupGUID[2]);
    data.WriteBit(leaderGUID[1]);
    data.WriteBit(leaderGUID[3]);
    data.WriteBit(leaderGUID[2]);
    data.WriteBit(groupGUID[4]);
    data.WriteBit(groupGUID[0]);
    data.WriteBit(groupGUID[1]);
    data.WriteBit(leaderGUID[5]);
    data.WriteBit(leaderGUID[4]);
    data.WriteBit(leaderGUID[0]);
    data.WriteBit(leaderGUID[7]);
    data.WriteBit(groupGUID[6]);
    data.WriteBit(leaderGUID[6]);
    data.WriteBit(groupGUID[7]);

    data.WriteByteSeq(leaderGUID[7]);
    data.WriteByteSeq(groupGUID[7]);
    data.WriteByteSeq(leaderGUID[3]);
    data.WriteByteSeq(groupGUID[2]);
    data.WriteByteSeq(groupGUID[1]);
    data.WriteByteSeq(leaderGUID[5]);
    data.WriteByteSeq(groupGUID[5]);
    data.WriteByteSeq(groupGUID[6]);
    data.WriteByteSeq(leaderGUID[2]);
    data.WriteByteSeq(groupGUID[0]);
    data.WriteByteSeq(groupGUID[3]);

    data << uint8(0x00);    // unk 5.0.5

    data.WriteByteSeq(leaderGUID[0]);
    data.WriteByteSeq(leaderGUID[4]);
    data.WriteByteSeq(groupGUID[4]);
    data.WriteByteSeq(leaderGUID[1]);
    data.WriteByteSeq(leaderGUID[6]);

    data << uint32(0x88B8); // unk 5.0.5

    group->BroadcastPacket(&data, false, -1);

    group->OfflineReadyCheck();
}

void WorldSession::HandleRaidConfirmReadyCheck(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_RAID_CONFIRM_READY_CHECK");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    recvData.read_skip<uint8>(); // unk, 0x00
    bool ready = recvData.ReadBit();
    recvData.ReadBit();
    recvData.ReadBit();

    ObjectGuid plGUID = GetPlayer()->GetGUID();
    ObjectGuid grpGUID = group->GetGUID();

    group->SetReadyCheckCount(group->GetReadyCheckCount() + 1);

    WorldPacket data(SMSG_RAID_READY_CHECK_RESPONSE);

    data.WriteBit(plGUID[1]);
    data.WriteBit(plGUID[3]);
    data.WriteBit(plGUID[7]);
    data.WriteBit(plGUID[0]);
    data.WriteBit(grpGUID[4]);
    data.WriteBit(grpGUID[7]);
    data.WriteBit(plGUID[2]);
    data.WriteBit(ready);
    data.WriteBit(grpGUID[2]);
    data.WriteBit(grpGUID[6]);
    data.WriteBit(plGUID[4]);
    data.WriteBit(plGUID[5]);
    data.WriteBit(grpGUID[1]);
    data.WriteBit(grpGUID[0]);
    data.WriteBit(grpGUID[5]);
    data.WriteBit(grpGUID[3]);
    data.WriteBit(plGUID[6]);

    data.WriteByteSeq(plGUID[2]);
    data.WriteByteSeq(plGUID[3]);
    data.WriteByteSeq(plGUID[7]);
    data.WriteByteSeq(grpGUID[1]);
    data.WriteByteSeq(grpGUID[7]);
    data.WriteByteSeq(plGUID[1]);
    data.WriteByteSeq(plGUID[0]);
    data.WriteByteSeq(grpGUID[2]);
    data.WriteByteSeq(grpGUID[3]);
    data.WriteByteSeq(plGUID[6]);
    data.WriteByteSeq(grpGUID[0]);
    data.WriteByteSeq(plGUID[5]);
    data.WriteByteSeq(plGUID[4]);
    data.WriteByteSeq(grpGUID[4]);
    data.WriteByteSeq(grpGUID[5]);
    data.WriteByteSeq(grpGUID[6]);

    group->BroadcastPacket(&data, true);

    // Send SMSG_RAID_READY_CHECK_COMPLETED
    if (group->GetReadyCheckCount() >= group->GetMembersCount())
    {
        ObjectGuid grpGUID = group->GetGUID();

        data.Initialize(SMSG_RAID_READY_CHECK_COMPLETED);

        uint8 bitOrder[8] = { 3, 2, 6, 1, 0, 7, 5, 4 };
        data.WriteBitInOrder(grpGUID, bitOrder);

        data.WriteByteSeq(grpGUID[0]);
        data.WriteByteSeq(grpGUID[6]);
        data.WriteByteSeq(grpGUID[2]);
        data.WriteByteSeq(grpGUID[4]);
        data.WriteByteSeq(grpGUID[3]);
        data.WriteByteSeq(grpGUID[5]);

        data << uint8(1);

        data.WriteByteSeq(grpGUID[7]);
        data.WriteByteSeq(grpGUID[1]);

        group->BroadcastPacket(&data, true);

    }
}

void WorldSession::BuildPartyMemberStatsChangedPacket(Player* player, WorldPacket* data, uint16 mask, uint64 guid, bool full /*= false*/)
{
    ObjectGuid playerGuid = guid;
    ByteBuffer dataBuffer;

    if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)                // if update power type, update current/max power also
        mask |= (GROUP_UPDATE_FLAG_CUR_POWER | GROUP_UPDATE_FLAG_MAX_POWER | GROUP_UPDATE_FLAG_UNK_80);

    if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)            // same for pets
        mask |= (GROUP_UPDATE_FLAG_PET_CUR_POWER | GROUP_UPDATE_FLAG_PET_MAX_POWER);

    Pet* pet = NULL;
    if (!player)
        mask &= ~GROUP_UPDATE_FULL;
    else if (!(pet = player->GetPet()))
        mask &= ~GROUP_UPDATE_PET ;

    data->Initialize(SMSG_PARTY_MEMBER_STATS, 200);         // average value
    *data << uint32(mask);

    if (mask & GROUP_UPDATE_FLAG_STATUS)
    {
        uint16 status = MEMBER_STATUS_OFFLINE;

        if (player)
        {
            status |= MEMBER_STATUS_ONLINE;

            if (player->IsPvP())
                status |= MEMBER_STATUS_PVP;

            if (player->isDead())
                status |= MEMBER_STATUS_DEAD;

            if (player->HasFlag(PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
                status |= MEMBER_STATUS_GHOST;

            if (player->isAFK())
                status |= MEMBER_STATUS_AFK;

            if (player->isDND())
                status |= MEMBER_STATUS_DND;
        }
        
        dataBuffer << uint16(status);
    }

    if (mask & GROUP_UPDATE_FLAG_MOP_UNK)
    {
        dataBuffer << uint8(1); // Same realms ?
        dataBuffer << uint8(0); // Unk, maybe "instance" status
    }

    if (mask & GROUP_UPDATE_FLAG_CUR_HP)
        dataBuffer << uint32(player->GetHealth());

    if (mask & GROUP_UPDATE_FLAG_MAX_HP)
        dataBuffer << uint32(player->GetMaxHealth());

    Powers powerType = POWER_MANA;
    if (mask & GROUP_UPDATE_FLAG_POWER_TYPE)
        dataBuffer << uint8(powerType);

    if (mask & GROUP_UPDATE_FLAG_CUR_POWER)
        dataBuffer << uint16(player->GetPower(powerType));

    // Now current power ?
    if (mask & GROUP_UPDATE_FLAG_MAX_POWER)
        dataBuffer << uint16(player->GetPower(powerType));

    // Now max power ?
    if (mask & GROUP_UPDATE_FLAG_UNK_80)
        dataBuffer << uint16(player->GetMaxPower(powerType));

    if (mask & GROUP_UPDATE_FLAG_LEVEL)
        dataBuffer << uint16(player->getLevel());

    if (mask & GROUP_UPDATE_FLAG_ZONE)
        dataBuffer << uint16(player->GetZoneId());

    if (mask & GROUP_UPDATE_FLAG_UNK400)
        dataBuffer << uint16(0);

    if (mask & GROUP_UPDATE_FLAG_POSITION)
        dataBuffer << uint16(player->GetPositionX()) << uint16(player->GetPositionY()) << uint16(player->GetPositionZ());

    if (mask & GROUP_UPDATE_FLAG_AURAS)
    {
        dataBuffer << uint8(1);
        uint64 auramask = player->GetAuraUpdateMaskForRaid();
        dataBuffer << uint64(auramask);
        uint32 count = player->GetVisibleAuras()->size();
        dataBuffer << uint32(count > MAX_AURAS ? MAX_AURAS : count);
        for (uint32 i = 0; i < MAX_AURAS; ++i)
        {
            if (auramask & (uint64(1) << i))
            {
                AuraApplication const* aurApp = player->GetVisibleAura(i);
                if (!aurApp)
                {
                    dataBuffer << uint32(0);
                    dataBuffer << uint8(0);
                    dataBuffer << uint32(0);
                    continue;
                }

                dataBuffer << uint32(aurApp->GetBase()->GetId());
                dataBuffer << uint8(aurApp->GetFlags());
                dataBuffer << uint32(0); // Unk 5.4.0

                if (aurApp->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                {
                    size_t pos = dataBuffer.wpos();
                    uint8 count = 0;

                    dataBuffer << uint8(0);
                    for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                    {
                        if (constAuraEffectPtr eff = aurApp->GetBase()->GetEffect(i)) // NULL if effect flag not set
                        {
                            dataBuffer << float(eff->GetAmount());
                            count++;
                        }
                    }
                    dataBuffer.put(pos, count);
                }
            }
        }
    }

    if (mask & GROUP_UPDATE_FLAG_PET_GUID)
    {
        if (pet)
            dataBuffer << uint64(pet->GetGUID());
        else
            dataBuffer << uint64(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_NAME)
    {
        if (pet)
            dataBuffer << pet->GetName();
        else
            dataBuffer << uint8(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MODEL_ID)
    {
        if (pet)
            dataBuffer << uint16(pet->GetDisplayId());
        else
            dataBuffer << uint16(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_CUR_HP)
    {
        if (pet)
            dataBuffer << uint32(pet->GetHealth());
        else
            dataBuffer << uint32(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MAX_HP)
    {
        if (pet)
            dataBuffer << uint32(pet->GetMaxHealth());
        else
            dataBuffer << uint32(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_POWER_TYPE)
    {
        if (pet)
            dataBuffer << uint8(pet->getPowerType());
        else
            dataBuffer << uint8(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_CUR_POWER)
    {
        if (pet)
            dataBuffer << uint16(pet->GetPower(pet->getPowerType()));
        else
            dataBuffer << uint16(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PET_MAX_POWER)
    {
        if (pet)
            dataBuffer << uint16(pet->GetMaxPower(pet->getPowerType()));
        else
            dataBuffer << uint16(0);
    }

    if (mask & GROUP_UPDATE_FLAG_MOP_UNK_2)
        dataBuffer << uint16(0); // Unk

    if (mask & GROUP_UPDATE_FLAG_PET_AURAS)
    {
        if (pet)
        {
            dataBuffer << uint8(0);
            uint64 auramask = pet->GetAuraUpdateMaskForRaid();
            dataBuffer << uint64(auramask);
            uint32 count = pet->GetVisibleAuras()->size();
            dataBuffer << uint32(count > MAX_AURAS ? MAX_AURAS : count);
            for (uint32 i = 0; i < MAX_AURAS; ++i)
            {
                if (auramask & (uint64(1) << i))
                {
                    AuraApplication const* aurApp = pet->GetVisibleAura(i);
                    if (!aurApp)
                    {
                        dataBuffer << uint32(0);
                        dataBuffer << uint8(0);
                        dataBuffer << uint32(0);
                        continue;
                    }

                    dataBuffer << uint32(aurApp->GetBase()->GetId());
                    dataBuffer << uint8(aurApp->GetFlags());
                    dataBuffer << uint32(0); // Unk 5.4.0

                    if (aurApp->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                    {
                        size_t pos = dataBuffer.wpos();
                        uint8 count = 0;

                        dataBuffer << uint8(0);
                        for (uint32 i = 0; i < MAX_SPELL_EFFECTS; ++i)
                        {
                            if (constAuraEffectPtr eff = aurApp->GetBase()->GetEffect(i)) // NULL if effect flag not set
                            {
                                dataBuffer << float(eff->GetAmount());
                                count++;
                            }
                        }
                        dataBuffer.put(pos, count);
                    }
                }
            }
        }
        else
        {
            dataBuffer << uint8(0);
            dataBuffer << uint64(0);
            dataBuffer << uint32(0);
        }
    }

    if (mask & GROUP_UPDATE_FLAG_VEHICLE_SEAT)
    {
        if (Vehicle* veh = player->GetVehicle())
            dataBuffer << uint32(veh->GetVehicleInfo()->m_seatID[player->m_movementInfo.t_seat]);
        else
            dataBuffer << uint32(0);
    }

    if (mask & GROUP_UPDATE_FLAG_PHASE)
    {
        dataBuffer << uint8(8); // either 0 or 8, same unk found in SMSG_PHASESHIFT
        dataBuffer.WriteBits(0, 23); // count
        // for (count) *data << uint16(phaseId)
    }

    *data << uint32(dataBuffer.size());

    dataBuffer.WriteBit(playerGuid[1]);
    dataBuffer.WriteBit(false);
    dataBuffer.WriteBit(playerGuid[7]);
    dataBuffer.WriteBit(playerGuid[2]);
    dataBuffer.WriteBit(playerGuid[6]);
    dataBuffer.WriteBit(playerGuid[3]);
    dataBuffer.WriteBit(playerGuid[4]);
    dataBuffer.WriteBit(playerGuid[5]);
    dataBuffer.WriteBit(playerGuid[0]);
    dataBuffer.WriteBit(true);

    uint8 bytes[8] = { 6, 1, 4, 2, 5, 0, 3, 7 };
    dataBuffer.WriteBytesSeq(playerGuid, bytes);

    data->append(dataBuffer);
}

/*this procedure handles clients CMSG_REQUEST_PARTY_MEMBER_STATS request*/
void WorldSession::HandleRequestPartyMemberStatsOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_REQUEST_PARTY_MEMBER_STATS");

    ObjectGuid Guid;
    recvData.read_skip<uint8>();

    uint8 bitOrder[8] = { 7, 1, 4, 3, 6, 2, 5, 0 };
    recvData.ReadBitInOrder(Guid, bitOrder);

    recvData.FlushBits();

    uint8 byteOrder[8] = { 7, 0, 4, 2, 1, 6, 5, 3 };
    recvData.ReadBytesSeq(Guid, byteOrder);

    Player* player = HashMapHolder<Player>::Find(Guid);
    if (player && player->GetGroup() != GetPlayer()->GetGroup())
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "Player %u (%s) sent CMSG_REQUEST_PARTY_MEMBER_STATS for player %u (%s) whos is not in the same group!",
            GetPlayer()->GetGUIDLow(), GetPlayer()->GetName(), player->GetGUIDLow(), player->GetName());
        return;
    }

    uint16 mask = GROUP_UPDATE_FLAG_STATUS;
    if (player)
    {
        mask |= GROUP_UPDATE_PLAYER;

        if (player->GetPet())
            mask |= GROUP_UPDATE_PET;
    }

    WorldPacket data;
    BuildPartyMemberStatsChangedPacket(player, &data, mask, Guid, true);
    SendPacket(&data);
}

void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket& /*recvData*/)
{
    // every time the player checks the character screen
    _player->SendRaidInfo();
}

void WorldSession::HandleOptOutOfLootOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_OPT_OUT_OF_LOOT");

    bool passOnLoot = recvData.ReadBit();
    recvData.FlushBits();

    // ignore if player not loaded
    if (!GetPlayer())                                        // needed because STATUS_AUTHED
    {
        if (passOnLoot)
            sLog->outError(LOG_FILTER_NETWORKIO, "CMSG_OPT_OUT_OF_LOOT value<>0 for not-loaded character!");
        return;
    }

    GetPlayer()->SetPassOnGroupLoot(passOnLoot);
}

void WorldSession::HandleRolePollBegin(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_ROLE_POLL_BEGIN");

    uint8 unk = 0;
    recvData >> unk;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    ObjectGuid guid = GetPlayer()->GetGUID();

    WorldPacket data(SMSG_ROLL_POLL_INFORM);

    uint8 bitsOrder[8] = { 0, 5, 7, 6, 1, 2, 4, 3 };
    data.WriteBitInOrder(guid, bitsOrder);

    data.WriteByteSeq(guid[5]);
    data.WriteByteSeq(guid[0]);
    data.WriteByteSeq(guid[3]);
    data.WriteByteSeq(guid[4]);
    data.WriteByteSeq(guid[7]);
    data.WriteByteSeq(guid[2]);

    data << uint8(unk);

    data.WriteByteSeq(guid[6]);
    data.WriteByteSeq(guid[1]);

    group->BroadcastPacket(&data, false, -1);
}

void WorldSession::HandleRequestJoinUpdates(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_REQUEST_JOIN_UPDATES");

    uint8 unk;

    recvData >> unk;
}

void WorldSession::HandleClearRaidMarkerOpcode(WorldPacket& recvData)
{
    // Not needed
    uint8 markerId = recvData.read<uint8>();

    Player* plr = GetPlayer();
    if (!plr)
        return;

    Group* group = plr->GetGroup();
    if (!group)
        return;

    if (markerId < 5)
        group->RemoveRaidMarker(markerId);
    else
        group->RemoveAllRaidMarkers();
}
