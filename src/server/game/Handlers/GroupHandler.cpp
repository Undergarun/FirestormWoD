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

void WorldSession::SendPartyResult(PartyCommand p_Command, const std::string& p_Name, PartyResult p_Result, uint32 p_ResultData /* = 0 */, uint64 p_ResultGuid /* = 0 */)
{
    uint64 l_ResultGuid = 0;    ///< player who caused error (in some cases).

    WorldPacket l_Data(SMSG_PARTY_COMMAND_RESULT, 4 + p_Name.size() + 1 + 4 + 4 + 8);
    l_Data.WriteBits(p_Name.length(), 9);
    l_Data.WriteBits(p_Command,       4);
    l_Data.WriteBits(p_Result,        6);

    l_Data << uint32(p_ResultData);
    l_Data.appendPackGUID(l_ResultGuid);
    l_Data.WriteString(p_Name);

    SendPacket(&l_Data);
}

void WorldSession::HandleGroupInviteOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PARTY_INVITE");

    time_t l_Now = time(NULL);
    if (l_Now - m_TimeLastGroupInviteCommand < 5)
        return;
    else
       m_TimeLastGroupInviteCommand = l_Now;

    uint8  l_PartyIndex;
    uint32 l_ProposedRoles;
    uint64 l_TargetGuid;
    uint32 l_TargetCfgRealmID;
    size_t l_TargetNameSize;
    size_t l_TargetRealmSize;
    std::string l_TargetName;
    std::string l_TargetRealm;

    p_RecvData >> l_PartyIndex;
    p_RecvData >> l_ProposedRoles;
    p_RecvData.readPackGUID(l_TargetGuid);
    p_RecvData >> l_TargetCfgRealmID;

    l_TargetNameSize  = p_RecvData.ReadBits(9);
    l_TargetRealmSize = p_RecvData.ReadBits(9);

    l_TargetName  = p_RecvData.ReadString(l_TargetNameSize);
    l_TargetRealm = p_RecvData.ReadString(l_TargetRealmSize);

    // attempt add selected player

    // cheating
    if (!normalizePlayerName(l_TargetName))
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    Player* l_Player = sObjectAccessor->FindPlayerByName(l_TargetName.c_str());

    // no player
    if (!l_Player)
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // restrict invite to GMs
    if (!sWorld->getBoolConfig(CONFIG_ALLOW_GM_GROUP) && !GetPlayer()->isGameMaster() && l_Player->isGameMaster())
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_BAD_PLAYER_NAME_S);
        return;
    }

    // can't group with
    if (!GetPlayer()->isGameMaster() && !sWorld->getBoolConfig(CONFIG_ALLOW_TWO_SIDE_INTERACTION_GROUP) && GetPlayer()->GetTeam() != l_Player->GetTeam())
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_PLAYER_WRONG_FACTION);
        return;
    }
    if (GetPlayer()->GetInstanceId() != 0 && l_Player->GetInstanceId() != 0 && GetPlayer()->GetInstanceId() != l_Player->GetInstanceId() && GetPlayer()->GetMapId() == l_Player->GetMapId())
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_TARGET_NOT_IN_INSTANCE_S);
        return;
    }
    // just ignore us
    if (l_Player->GetInstanceId() != 0 && l_Player->GetDungeonDifficulty() != GetPlayer()->GetDungeonDifficulty())
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    if (l_Player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    ObjectGuid l_InvitedGuid = l_Player->GetGUID();

    Group* group = GetPlayer()->GetGroup();
    if (group && group->isBGGroup())
        group = GetPlayer()->GetOriginalGroup();

    Group* group2 = l_Player->GetGroup();
    if (group2 && group2->isBGGroup())
        group2 = l_Player->GetOriginalGroup();
    // player already in another group or invited
    if (group2 || l_Player->GetGroupInvite())
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_ALREADY_IN_GROUP_S);

        if (group2)
        {
            bool l_CanAccept                 = false;
            bool l_MightCRZYou               = false;
            bool l_IsXRealm                  = false;
            bool l_MustBeBNetFriend          = false;
            bool l_AllowMultipleRoles        = false;
            bool l_IsLocal                   = true;
            size_t l_NameLenght              = strlen(GetPlayer()->GetName());
            size_t l_RealmNameActualSize     = sWorld->GetRealmName().length();
            size_t l_NormalizedRealmNameSize = sWorld->GetNormalizedRealmName().length();
            uint64 l_InviterGuid             = GetPlayer()->GetGUID();
            uint64 l_InviterBNetAccountID    = GetBNetAccountGUID();
            uint32 l_LfgCompletedMask        = 0;
            uint32 l_InviterCfgRealmID       = realmID;
            uint16 l_Unk                     = 970;    ///< Always 970 in retail sniff
            std::string l_InviterName        = GetPlayer()->GetName();
            std::string l_InviterRealmName   = sWorld->GetRealmName();
            std::string l_NormalizeRealmName = sWorld->GetNormalizedRealmName();
            std::list<uint32> l_LfgSlots;  

            // tell the player that they were invited but it failed as they were already in a group
            WorldPacket data(SMSG_PARTY_INVITE, 45);
            data.WriteBit(l_CanAccept);
            data.WriteBit(l_MightCRZYou);
            data.WriteBit(l_IsXRealm);
            data.WriteBit(l_MustBeBNetFriend);
            data.WriteBit(l_AllowMultipleRoles);
            data.WriteBits(l_NameLenght, 6);
            data.FlushBits();
            data.appendPackGUID(l_InviterGuid);
            data.appendPackGUID(l_InviterBNetAccountID);
            data << uint16(l_Unk);
            data << uint32(l_InviterCfgRealmID);
            data.WriteBit(l_IsLocal);
            data.WriteBits(l_RealmNameActualSize, 8);
            data.WriteBits(l_NormalizedRealmNameSize, 8);
            data.FlushBits();
            data.WriteString(l_InviterRealmName);
            data.WriteString(l_NormalizeRealmName);
            data << uint32(l_LfgCompletedMask);
            data << uint32(l_LfgSlots.size());
            data << uint32(l_ProposedRoles);    ///< from CMSG_PARTY_INVITE
            data.WriteString(l_InviterName);

            for (auto l_LfgSlot : l_LfgSlots)
                data << uint32(l_LfgSlot);

            l_Player->GetSession()->SendPacket(&data);
        }

        return;
    }

    if (group)
    {
        // not have permissions for invite
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetGroupType() & GROUPTYPE_EVERYONE_IS_ASSISTANT))
        {
            SendPartyResult(PARTY_CMD_INVITE, "", ERR_NOT_LEADER);
            return;
        }
        // not have place
        if (group->IsFull())
        {
            SendPartyResult(PARTY_CMD_INVITE, "", ERR_GROUP_FULL);
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

        if (!group->AddInvite(l_Player))
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
        if (!group->AddInvite(l_Player))
        {
            return;
        }
    }

    // ok, we do it
    bool l_CanAccept                 = true;
    bool l_MightCRZYou               = false;
    bool l_IsXRealm                  = false;
    bool l_MustBeBNetFriend          = false;
    bool l_AllowMultipleRoles        = false;
    bool l_IsLocal                   = true;
    size_t l_NameLenght              = strlen(GetPlayer()->GetName());
    size_t l_RealmNameActualSize     = sWorld->GetRealmName().length();
    size_t l_NormalizedRealmNameSize = sWorld->GetNormalizedRealmName().length();
    uint64 l_InviterGuid             = GetPlayer()->GetGUID();
    uint64 l_InviterBNetAccountID    = GetBNetAccountGUID();
    uint32 l_LfgCompletedMask        = 0;
    uint32 l_InviterCfgRealmID       = realmID;
    uint16 l_Unk                     = 970;    ///< Always 970 in retail sniff
    std::string l_InviterName        = GetPlayer()->GetName();
    std::string l_InviterRealmName   = sWorld->GetRealmName();
    std::string l_NormalizeRealmName = sWorld->GetNormalizedRealmName();
    std::list<uint32> l_LfgSlots; 

    // tell the player that they were invited but it failed as they were already in a group
    WorldPacket data(SMSG_PARTY_INVITE, 45);
    data.WriteBit(l_CanAccept);
    data.WriteBit(l_MightCRZYou);
    data.WriteBit(l_IsXRealm);
    data.WriteBit(l_MustBeBNetFriend);
    data.WriteBit(l_AllowMultipleRoles);
    data.WriteBits(l_NameLenght, 6);
    data.FlushBits();
    data.appendPackGUID(l_InviterGuid);
    data.appendPackGUID(l_InviterBNetAccountID);
    data << uint16(l_Unk);
    data << uint32(l_InviterCfgRealmID);
    data.WriteBit(l_IsLocal);
    data.WriteBits(l_RealmNameActualSize, 8);
    data.WriteBits(l_NormalizedRealmNameSize, 8);
    data.FlushBits();
    data.WriteString(l_InviterRealmName);
    data.WriteString(l_NormalizeRealmName);
    data << uint32(l_LfgCompletedMask);
    data << uint32(l_LfgSlots.size());
    data << uint32(l_ProposedRoles);    ///< from CMSG_PARTY_INVITE
    data.WriteString(l_InviterName);

    for (auto l_LfgSlot : l_LfgSlots)
        data << uint32(l_LfgSlot);

    l_Player->GetSession()->SendPacket(&data);

    SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_PARTY_RESULT_OK);
}

void WorldSession::HandleGroupInviteResponseOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PARTY_INVITE_RESPONSE");

    uint32 l_RolesDesired;
    uint8  l_PartyIndex;
    bool   l_Accept;
    bool   l_HasRolesDesired;

    l_PartyIndex      = p_RecvData.read<uint8>();
    l_Accept          = p_RecvData.ReadBit();
    l_HasRolesDesired = p_RecvData.ReadBit();

    if (l_HasRolesDesired)
        l_RolesDesired = p_RecvData.read<uint32>();

    uint32 groupGUID = GetPlayer()->GetGroupInvite();
    if (!groupGUID)
        return;

    Group* group = sGroupMgr->GetGroupByGUID(groupGUID);
    if (!group)
        return;

    if (l_Accept)
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
            SendPartyResult(PARTY_CMD_INVITE, "", ERR_GROUP_FULL);
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
        std::string l_Name = GetPlayer()->GetName();
        WorldPacket data(SMSG_GROUP_DECLINE, l_Name.length());
        data.WriteBits(l_Name.length(), 6);
        data.FlushBits();
        data.WriteString(l_Name);
        leader->GetSession()->SendPacket(&data);
    }
}

void WorldSession::HandlePartyUninviteOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_PARTY_UNINVITE");

    std::string l_Reason;
    uint8       l_PartyIndex;
    size_t      l_ReasonSize;
    uint64      l_TargetGuid;

    p_RecvData >> l_PartyIndex;
    p_RecvData.readPackGUID(l_TargetGuid);
    l_ReasonSize = p_RecvData.ReadBits(8);
    l_Reason     = p_RecvData.ReadString(l_ReasonSize);

    // can't uninvite yourself
    if (GetPlayer()->GetGUID() == l_TargetGuid)
    {
        sLog->outError(LOG_FILTER_NETWORKIO, "WorldSession::HandlePartyUninviteOpcode: leader %s(%d) tried to uninvite himself from the group.", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
        return;
    }

    PartyResult l_PartyResult = GetPlayer()->CanUninviteFromGroup();
    if (l_PartyResult != ERR_PARTY_RESULT_OK)
    {
        SendPartyResult(PARTY_CMD_UNINVITE, "", l_PartyResult);
        return;
    }

    Group* l_Group = GetPlayer()->GetGroup();
    if (!l_Group)
        return;

    if (l_Group->IsMember(l_TargetGuid))
    {
        Player::RemoveFromGroup(l_Group, l_TargetGuid, GROUP_REMOVEMETHOD_KICK, GetPlayer()->GetGUID());
        return;
    }

    if (Player* player = l_Group->GetInvited(l_TargetGuid))
    {
        player->UninviteFromGroup();
        return;
    }

    SendPartyResult(PARTY_CMD_UNINVITE, "", ERR_TARGET_NOT_IN_GROUP_S, 0, l_TargetGuid);
}

void WorldSession::HandleSetPartyLeaderOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_PARTY_LEADER");

    uint64 l_Target;
    uint8  l_PartyIndex;

    l_PartyIndex = p_RecvData.read<uint8>();
    p_RecvData.readPackGUID(l_Target);

    Player* player = ObjectAccessor::FindPlayer(l_Target);
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
    group->ChangeLeader(l_Target);
    group->SendUpdate();
}

void WorldSession::HandleSetRoleOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_SET_ROLE");

    uint64 l_ChangedUnit;
    uint32 l_Role;
    uint8  l_PartyIndex;

    p_RecvData >> l_PartyIndex;
    p_RecvData.readPackGUID(l_ChangedUnit);
    p_RecvData >> l_Role;

    Group* l_Group = GetPlayer()->GetGroup();

    if (l_Group                                                 ///< Group Exist
        && l_ChangedUnit != GetPlayer()->GetGUID()              ///< Player change another player (not himself)
        && l_Group->GetLeaderGUID() != GetPlayer()->GetGUID())  ///< Player isn't group leader
        return;

    //uint8 l_PartyIndex         ///< Already exist in CMSG_SET_ROLE reading
    //uint64 l_ChangedUnit       ///< Already exist in CMSG_SET_ROLE reading
    uint32 l_NewRole = l_Role;   ///< From CMSG_SET_ROLE
    uint64 l_From    = GetPlayer()->GetGUID();
    uint32 l_OldRole = l_Group ? l_Group->getGroupMemberRole(l_ChangedUnit) : 0;

    WorldPacket l_Data(SMSG_ROLE_CHANGED_INFORM, 24);
    l_Data << uint8(l_PartyIndex);  ///< From CMSG_SET_ROLE
    l_Data.appendPackGUID(l_From);
    l_Data.appendPackGUID(l_ChangedUnit);
    l_Data << uint32(l_OldRole);
    l_Data << uint32(l_NewRole);

    if (l_Group)
    {
        l_Group->setGroupMemberRole(l_ChangedUnit, l_NewRole);
        l_Group->SendUpdate();
        l_Group->BroadcastPacket(&l_Data, false);
    }
    else
        SendPacket(&l_Data);
}

void WorldSession::HandleLeaveGroupOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_LEAVE_GROUP");

    uint8 l_PartyIndex = p_RecvData.read<uint8>();

    Group* l_Group = GetPlayer()->GetGroup();
    if (!l_Group)
        return;

    if (m_Player->InBattleground())
    {
        SendPartyResult(PARTY_CMD_INVITE, "", ERR_INVITE_RESTRICTED);
        return;
    }

    /** error handling **/
    /********************/

    // everything's fine, do it
    SendPartyResult(PARTY_CMD_LEAVE, GetPlayer()->GetName(), ERR_PARTY_RESULT_OK);

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

// @TODO: Verify order, l_RollType & l_LootListID are maybe inversed
void WorldSession::HandleLootRoll(WorldPacket& p_RecvData)
{
    uint64 l_LootObj;
    uint8  l_LootListID;
    uint8  l_RollType;

    p_RecvData.readPackGUID(l_LootObj);
    p_RecvData >> l_RollType;
    p_RecvData >> l_LootListID;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->CountRollVote(GetPlayer()->GetGUID(), l_LootListID, l_RollType);

    switch (l_RollType)
    {
        case ROLL_NEED:
            GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_NEED, 1);
            break;
        case ROLL_GREED:
            GetPlayer()->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_ROLL_GREED, 1);
            break;
    }
}

void WorldSession::HandleMinimapPingOpcode(WorldPacket& p_RecvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_MINIMAP_PING");

    if (!GetPlayer()->GetGroup())
        return;

    uint8        l_PartyIndex;
    G3D::Vector2 l_Position;

    p_RecvData.ReadVector2(l_Position);
    p_RecvData >> l_PartyIndex;

    uint64 l_Sender = GetPlayer()->GetGUID();
    //G3D::Vector2 l_Position;  ///< Already in CMSG_MINIMAP_PING

    WorldPacket l_Data(SMSG_MINIMAP_PING, (16 + 4 + 4));
    l_Data.appendPackGUID(l_Sender);
    l_Data.WriteVector2(l_Position);

    GetPlayer()->GetGroup()->BroadcastPacket(&l_Data, true, -1, l_Sender);
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

        group->SetTargetIcon(x, m_Player->GetGUID(), guid);
    }
}

void WorldSession::HandleGroupRaidConvertOpcode(WorldPacket& recvData)
{
    sLog->outDebug(LOG_FILTER_NETWORKIO, "WORLD: Received CMSG_GROUP_RAID_CONVERT");

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (m_Player->InBattleground())
        return;

    // Error handling
    if (!group->IsLeader(GetPlayer()->GetGUID()) || group->GetMembersCount() < 2)
        return;

    // Everything's fine, do it (is it 0 (PARTY_OP_INVITE) correct code)
    SendPartyResult(PARTY_CMD_INVITE, "", ERR_PARTY_RESULT_OK);

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

void WorldSession::BuildPartyMemberStatsChangedPacket(Player* p_Player, WorldPacket* p_Data, uint16 p_Mask, bool p_FullUpdate)
{
    if (p_FullUpdate)
    {
        uint16 l_PlayerStatus = MEMBER_STATUS_OFFLINE;

        if (p_Player)
        {
            l_PlayerStatus |= MEMBER_STATUS_ONLINE;

            if (p_Player->IsPvP())
                l_PlayerStatus |= MEMBER_STATUS_PVP;

            if (p_Player->isDead())
                l_PlayerStatus |= MEMBER_STATUS_DEAD;

            if (p_Player->HasFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_GHOST))
                l_PlayerStatus |= MEMBER_STATUS_GHOST;

            if (p_Player->isAFK())
                l_PlayerStatus |= MEMBER_STATUS_AFK;

            if (p_Player->isDND())
                l_PlayerStatus |= MEMBER_STATUS_DND;
        }

        p_Data->Initialize(SMSG_PARTY_MEMBER_STATE_FULL, 200);
        
        p_Data->WriteBit(false);                    ///< ForEnemy
        p_Data->FlushBits();
        p_Data->appendPackGUID(p_Player->GetGUID());
        *p_Data << uint8(1);                        ///< Same realms ?
        *p_Data << uint8(0);                        ///< Unk, maybe "instance" status
        *p_Data << uint16(l_PlayerStatus);
        *p_Data << uint8(p_Player->getPowerType());
        *p_Data << uint16(0);
        *p_Data << uint32(p_Player->GetHealth());
        *p_Data << uint32(p_Player->GetMaxHealth());
        *p_Data << uint16(p_Player->GetPower(p_Player->getPowerType()));
        *p_Data << uint16(p_Player->GetMaxPower(p_Player->getPowerType()));
        *p_Data << uint16(p_Player->getLevel());
        *p_Data << uint16(p_Player->GetSpecializationId(p_Player->GetActiveSpec()));
        *p_Data << uint16(p_Player->GetZoneId());
        *p_Data << uint16(0);
        *p_Data << uint32(0);
        *p_Data << uint16(p_Player->GetPositionX());
        *p_Data << uint16(p_Player->GetPositionY());
        *p_Data << uint16(p_Player->GetPositionZ());
        *p_Data << uint32(0);
        *p_Data << uint32(p_Player->GetVisibleAuras()->size() > MAX_AURAS ? MAX_AURAS : p_Player->GetVisibleAuras()->size());

        *p_Data << uint32(0);
        *p_Data << uint32(0);
        p_Data->appendPackGUID(0);

        if (p_Player->GetVisibleAuras()->size())
        {
            uint64 l_AuraMask = p_Player->GetAuraUpdateMaskForRaid();

            for (uint32 l_AuraIT = 0; l_AuraIT < MAX_AURAS; ++l_AuraIT)
            {
                if (l_AuraMask & (uint64(1) << l_AuraIT))
                {
                    AuraApplication const* l_AuraApplication = p_Player->GetVisibleAura(l_AuraIT);

                    if (!l_AuraApplication)
                    {
                        *p_Data << uint32(0);
                        *p_Data << uint8(0);
                        *p_Data << uint32(0);
                        *p_Data << uint32(0);
                        continue;
                    }

                    uint32 l_EffectCount = 0;

                    if (l_AuraApplication->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                    {
                        for (uint32 l_Y = 0; l_Y < MAX_SPELL_EFFECTS; ++l_Y)
                            if (constAuraEffectPtr l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                                l_EffectCount++;
                    }

                    *p_Data << uint32(l_AuraApplication->GetBase()->GetId());
                    *p_Data << uint8(l_AuraApplication->GetFlags());
                    *p_Data << uint32(0);
                    *p_Data << uint32(l_EffectCount);

                    if (l_AuraApplication->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                    {
                        for (uint32 l_Y = 0; l_Y < MAX_SPELL_EFFECTS; ++l_Y)
                            if (constAuraEffectPtr l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                                *p_Data << float(l_Effect->GetAmount());
                    }
                }
            }
        }

        if (p_Data->WriteBit(p_Player->GetPet() != 0))
        {
            p_Data->FlushBits();

            Pet * l_Pet = p_Player->GetPet();

            p_Data->appendPackGUID(l_Pet->GetGUID());
            *p_Data << uint16(l_Pet->GetDisplayId());
            *p_Data << uint32(l_Pet->GetHealth());
            *p_Data << uint32(l_Pet->GetMaxHealth());

            *p_Data << uint32(l_Pet->GetVisibleAuras()->size() > MAX_AURAS ? MAX_AURAS : l_Pet->GetVisibleAuras()->size());

            uint64 l_AuraMask = l_Pet->GetAuraUpdateMaskForRaid();

            for (uint32 l_AuraIT = 0; l_AuraIT < MAX_AURAS; ++l_AuraIT)
            {
                if (l_AuraMask & (uint64(1) << l_AuraIT))
                {
                    AuraApplication const* l_AuraApplication = l_Pet->GetVisibleAura(l_AuraIT);

                    if (!l_AuraApplication)
                    {
                        *p_Data << uint32(0);
                        *p_Data << uint8(0);
                        *p_Data << uint32(0);
                        *p_Data << uint32(0);
                        continue;
                    }

                    uint32 l_EffectCount = 0;

                    if (l_AuraApplication->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                    {
                        for (uint32 l_Y = 0; l_Y < MAX_SPELL_EFFECTS; ++l_Y)
                            if (constAuraEffectPtr l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                                l_EffectCount++;
                    }

                    *p_Data << uint32(l_AuraApplication->GetBase()->GetId());
                    *p_Data << uint8(l_AuraApplication->GetFlags());
                    *p_Data << uint32(0);
                    *p_Data << uint32(l_EffectCount);

                    if (l_AuraApplication->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                    {
                        for (uint32 l_Y = 0; l_Y < MAX_SPELL_EFFECTS; ++l_Y)
                            if (constAuraEffectPtr l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                                *p_Data << float(l_Effect->GetAmount());
                    }
                }
            }

            p_Data->WriteBits(l_Pet->GetName() ? strlen(l_Pet->GetName()) : 0, 8);
            p_Data->FlushBits();

            if (l_Pet->GetName())
                p_Data->WriteString(l_Pet->GetName());
        }
        else
            p_Data->FlushBits();
    }
    else
    {
        bool l_PetInfo = p_Mask & (GROUP_UPDATE_FLAG_PET_GUID | GROUP_UPDATE_FLAG_PET_NAME | GROUP_UPDATE_FLAG_PET_MODEL_ID | GROUP_UPDATE_FLAG_PET_CUR_HP | GROUP_UPDATE_FLAG_PET_MAX_HP | GROUP_UPDATE_FLAG_PET_AURAS);


    }
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
    BuildPartyMemberStatsChangedPacket(player, &data, mask, true);
    SendPacket(&data);
}

void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket& /*recvData*/)
{
    // every time the player checks the character screen
    m_Player->SendRaidInfo();
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

    uint8 unk = 0;
    recvData >> unk;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->SendUpdate();
}

void WorldSession::HandleClearRaidMarkerOpcode(WorldPacket& recvData)
{
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
