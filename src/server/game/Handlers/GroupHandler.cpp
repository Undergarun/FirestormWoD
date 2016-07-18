////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

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
    WorldPacket l_Data(SMSG_PARTY_COMMAND_RESULT, 4 + p_Name.size() + 1 + 4 + 4 + 8);
    l_Data.WriteBits(p_Name.length(), 9);
    l_Data.WriteBits(p_Command,       4);
    l_Data.WriteBits(p_Result,        6);

    l_Data << uint32(p_ResultData);
    l_Data.appendPackGUID(p_ResultGuid);
    l_Data.WriteString(p_Name);

    SendPacket(&l_Data);
}

void WorldSession::HandleGroupInviteOpcode(WorldPacket& p_RecvData)
{
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

    if (l_Player->GetGUID() == GetPlayer()->GetGUID())
        return;

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
    if (l_Player->GetInstanceId() != 0 && l_Player->GetDungeonDifficultyID() != GetPlayer()->GetDungeonDifficultyID())
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_IGNORING_YOU_S);
        return;
    }

#ifndef CROSS
    if (l_Player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
#else /* CROSS */
    if (l_Player->GetSocial() && l_Player->GetSocial()->HasIgnore(GetPlayer()->GetGUIDLow()))
#endif /* CROSS */
    {
        SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_IGNORING_YOU_S);
        return;
    }

    ObjectGuid l_InvitedGuid = l_Player->GetGUID(); ///< l_InvitedGuid is unused

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
#ifndef CROSS
            size_t l_RealmNameActualSize     = sWorld->GetRealmName().length();
            size_t l_NormalizedRealmNameSize = sWorld->GetNormalizedRealmName().length();
#else /* CROSS */
            size_t l_RealmNameActualSize     = GetPlayer()->GetSession()->GetServerName().length();
            size_t l_NormalizedRealmNameSize = GetPlayer()->GetSession()->GetServerName().length();
#endif /* CROSS */
            uint64 l_InviterGuid             = GetPlayer()->GetGUID();
            uint64 l_InviterBNetAccountID    = GetBNetAccountGUID();
            uint32 l_LfgCompletedMask        = 0;
            uint32 l_InviterCfgRealmID       = g_RealmID;
            uint16 l_Unk                     = 970;    ///< Always 970 in retail sniff
            std::string l_InviterName        = GetPlayer()->GetName();
#ifndef CROSS
            std::string l_InviterRealmName   = sWorld->GetRealmName();
            std::string l_NormalizeRealmName = sWorld->GetNormalizedRealmName();
#else /* CROSS */
            std::string l_InviterRealmName   = GetPlayer()->GetSession()->GetServerName();
            std::string l_NormalizeRealmName = GetPlayer()->GetSession()->GetServerName();
#endif /* CROSS */
            std::list<uint32> l_LfgSlots;

            // tell the player that they were invited but it failed as they were already in a group
            WorldPacket data(SMSG_PARTY_INVITE, 200);
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
            data.WriteBit(false);   // UnkBit
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
        if (!group->IsLeader(GetPlayer()->GetGUID()) && !group->IsAssistant(GetPlayer()->GetGUID()) && !(group->GetPartyFlags() & PARTY_FLAG_EVERYONE_IS_ASSISTANT))
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
#ifndef CROSS
    size_t l_RealmNameActualSize     = sWorld->GetRealmName().length();
    size_t l_NormalizedRealmNameSize = sWorld->GetNormalizedRealmName().length();
#else /* CROSS */
    size_t l_RealmNameActualSize     = GetPlayer()->GetSession()->GetServerName().length();
    size_t l_NormalizedRealmNameSize = GetPlayer()->GetSession()->GetServerName().length();
#endif /* CROSS */
    uint64 l_InviterGuid             = GetPlayer()->GetGUID();
    uint64 l_InviterBNetAccountID    = GetBNetAccountGUID();
    uint32 l_LfgCompletedMask        = 0;
    uint32 l_InviterCfgRealmID       = g_RealmID;
    uint16 l_Unk                     = 970;    ///< Always 970 in retail sniff
    std::string l_InviterName        = GetPlayer()->GetName();
#ifndef CROSS
    std::string l_InviterRealmName   = sWorld->GetRealmName();
    std::string l_NormalizeRealmName = sWorld->GetNormalizedRealmName();
#else /* CROSS */
    std::string l_InviterRealmName   = GetPlayer()->GetSession()->GetServerName();
    std::string l_NormalizeRealmName = GetPlayer()->GetSession()->GetServerName();
#endif /* CROSS */
    std::list<uint32> l_LfgSlots;

    // tell the player that they were invited but it failed as they were already in a group
    WorldPacket l_Data(SMSG_PARTY_INVITE, 200);
    l_Data.WriteBit(l_CanAccept);
    l_Data.WriteBit(l_MightCRZYou);
    l_Data.WriteBit(l_IsXRealm);
    l_Data.WriteBit(l_MustBeBNetFriend);
    l_Data.WriteBit(l_AllowMultipleRoles);
    l_Data.WriteBits(l_NameLenght, 6);
    l_Data.FlushBits();
    l_Data.appendPackGUID(l_InviterGuid);
    l_Data.appendPackGUID(l_InviterBNetAccountID);
    l_Data << uint16(l_Unk);                        ///< InviterVirtualRealmAddress
    l_Data << uint32(l_InviterCfgRealmID);
    l_Data.WriteBit(l_IsLocal);
    l_Data.WriteBit(false);   // UnkBit
    l_Data.WriteBits(l_RealmNameActualSize, 8);
    l_Data.WriteBits(l_NormalizedRealmNameSize, 8);
    l_Data.FlushBits();
    l_Data.WriteString(l_InviterRealmName);
    l_Data.WriteString(l_NormalizeRealmName);
    l_Data << uint32(l_LfgCompletedMask);
    l_Data << uint32(l_LfgSlots.size());
    l_Data << uint32(l_ProposedRoles);    ///< from CMSG_PARTY_INVITE
    l_Data.WriteString(l_InviterName);

    for (auto l_LfgSlot : l_LfgSlots)
        l_Data << uint32(l_LfgSlot);

    l_Player->GetSession()->SendPacket(&l_Data);

    SendPartyResult(PARTY_CMD_INVITE, l_TargetName, ERR_PARTY_RESULT_OK);
}

void WorldSession::HandleGroupInviteResponseOpcode(WorldPacket& p_RecvData)
{
    uint32 l_RolesDesired;
    uint8  l_PartyIndex;
    bool   l_Accept;
    bool   l_HasRolesDesired;

    l_PartyIndex      = p_RecvData.read<uint8>(); ///< l_Partyindex is never read 01/18/16
    l_Accept          = p_RecvData.ReadBit();
    l_HasRolesDesired = p_RecvData.ReadBit();

    if (l_HasRolesDesired)
        l_RolesDesired = p_RecvData.read<uint32>(); ///< l_RolesDesired is never read 01/18/16

    uint32 l_GroupGUID = GetPlayer()->GetGroupInvite();

    if (!l_GroupGUID)
        return;

    Group * l_Group = sGroupMgr->GetGroupByGUID(l_GroupGUID);

    if (!l_Group)
        return;

    if (l_Accept)
    {
        // Remove player from invitees in any case
        l_Group->RemoveInvite(GetPlayer());

        if (l_Group->GetLeaderGUID() == GetPlayer()->GetGUID())
        {
            sLog->outError(LOG_FILTER_NETWORKIO, "HandleGroupAcceptOpcode: player %s(%d) tried to accept an invite to his own group", GetPlayer()->GetName(), GetPlayer()->GetGUIDLow());
            return;
        }

        // Group is full
        if (l_Group->IsFull())
        {
            SendPartyResult(PARTY_CMD_INVITE, "", ERR_GROUP_FULL);
            return;
        }

        Player * l_Leader = ObjectAccessor::FindPlayer(l_Group->GetLeaderGUID());

        /// Forming a new group, create it
        if (!l_Group->IsCreated())
        {
            /// This can happen if the leader is zoning. To be removed once delayed actions for zoning are implemented
            if (!l_Leader)
            {
                l_Group->RemoveAllInvites();
                return;
            }

            /// If we're about to create a group there really should be a leader present
            ASSERT(l_Leader);

            l_Group->RemoveInvite(l_Leader);
            l_Group->Create(l_Leader);

            sGroupMgr->AddGroup(l_Group);
        }

        /// Everything is fine, do it, PLAYER'S GROUP IS SET IN ADDMEMBER!!!
        if (!l_Group->AddMember(GetPlayer()))
            return;

        l_Group->BroadcastGroupUpdate();
    }
    else
    {
        /// Remember leader if online (group pointer will be invalid if group gets disbanded)
        Player * l_Leader = ObjectAccessor::FindPlayer(l_Group->GetLeaderGUID());

        /// Uninvite, group can be deleted
        GetPlayer()->UninviteFromGroup();

        if (!l_Leader || !l_Leader->GetSession())
            return;

        /// Report
        std::string l_Name = GetPlayer()->GetName();

        WorldPacket l_Data(SMSG_GROUP_DECLINE, l_Name.length() + 1);
        l_Data.WriteBits(l_Name.length(), 6);
        l_Data.FlushBits();

        l_Data.WriteString(l_Name);

        l_Leader->GetSession()->SendPacket(&l_Data);
    }
}

void WorldSession::HandlePartyUninviteOpcode(WorldPacket& p_RecvData)
{
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
    uint64 l_Target;
    uint8  l_PartyIndex;

    l_PartyIndex = p_RecvData.read<uint8>(); ///< l_PartyIndex is never read 01/18/16
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

void WorldSession::HandleSetLootMethodOpcode(WorldPacket& p_RecvData)
{
    uint64 l_Master;
    uint32 l_Threshold;
    uint8 l_PartyIndex;
    uint8 l_Method;

    p_RecvData >> l_PartyIndex;
    p_RecvData >> l_Method;
    p_RecvData.readPackGUID(l_Master);
    p_RecvData >> l_Threshold;

    Group * l_Group = GetPlayer()->GetGroup();

    if (!l_Group || !l_Group->IsLeader(m_Player->GetGUID()) || (l_Master != 0 && !l_Group->IsMember(l_Master)) || GetPlayer()->GetBattleground() != nullptr)
    {
        WorldPacket l_Response(SMSG_SET_LOOT_METHOD_FAILED, 0);
        SendPacket(&l_Response);

        return;
    }

    l_Group->SetLootThreshold((ItemQualities)l_Threshold);
    l_Group->SetLootMethod((LootMethod)l_Method);
    l_Group->SetLooterGuid(l_Master);
    l_Group->SendUpdate();
}

void WorldSession::HandleLeaveGroupOpcode(WorldPacket& p_RecvData)
{
    uint8 l_PartyIndex = p_RecvData.read<uint8>(); ///< L_partyIndex is never read 01/18/16

    Group* l_Group = m_Player->GetGroup();
    if (!l_Group)
        return;

    if (m_Player->InBattleground())
    {
        SendPartyResult(PARTY_CMD_INVITE, "", ERR_LFG_PENDING);
        return;
    }

    /// Everything's fine, do it
    SendPartyResult(PARTY_CMD_LEAVE, m_Player->GetName(), ERR_PARTY_RESULT_OK);

    m_Player->RemoveFromGroup(GROUP_REMOVEMETHOD_LEAVE);
}

void WorldSession::HandleLootMethodOpcode(WorldPacket & recvData)
{
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

    if (GetPlayer()->GetBattleground() != nullptr)
        return;
    /********************/

    // everything's fine, do it
    group->SetLootMethod((LootMethod)lootMethod);
    group->SetLooterGuid(lootMaster);
    group->SetLootThreshold((ItemQualities)lootThreshold);
    group->SendUpdate();
}

void WorldSession::HandleLootRoll(WorldPacket& p_RecvData)
{
    uint64 l_LootObj;
    uint8  l_LootListID;
    uint8  l_RollType;

    p_RecvData.readPackGUID(l_LootObj);
    p_RecvData >> l_LootListID;
    p_RecvData >> l_RollType;

    Group* l_Group = GetPlayer()->GetGroup();

    if (!l_Group)
        return;

    l_Group->CountRollVote(GetPlayer()->GetGUID(), l_LootListID, l_RollType);

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

void WorldSession::HandleRandomRollOpcode(WorldPacket& p_RecvData)
{
    int32 l_Min;
    int32 l_Max;
    uint8 l_PartyIndex;

    p_RecvData >> l_Min;
    p_RecvData >> l_Max;
    p_RecvData >> l_PartyIndex;

    if (l_Min > l_Max || l_Min > 10000)                ///< < 32768 for urand call
        return;

    uint64 l_Roller     = GetPlayer()->GetGUID();
    uint64 l_WowAccount = GetWoWAccountGUID();
    int32 l_Result  = urand(l_Min, l_Max);
    //int32 l_Max;  ///< From CMSG_RANDOM_ROLL
    //int32 l_Min;  ///< From CMSG_RANDOM_ROLL

    WorldPacket l_Data(SMSG_RANDOM_ROLL, 4 + 4 + 4 + 16);
    l_Data.appendPackGUID(l_Roller);
    l_Data.appendPackGUID(l_WowAccount);
    l_Data << int32(l_Min);
    l_Data << int32(l_Max);
    l_Data << int32(l_Result);

    if (GetPlayer()->GetGroup())
        GetPlayer()->GetGroup()->BroadcastPacket(&l_Data, false);
    else
        SendPacket(&l_Data);
}

void WorldSession::HandleUpdateRaidTargetOpcode(WorldPacket& p_RecvData)
{
    Group* l_Group = GetPlayer()->GetGroup();
    if (!l_Group)
        return;

    uint64 l_Target;
    uint8  l_PartyIndex;
    uint8  l_Symbol;

    p_RecvData >> l_Symbol;
    p_RecvData.readPackGUID(l_Target);
    p_RecvData >> l_PartyIndex;

    if (!l_Group->IsLeader(GetPlayer()->GetGUID()) && !l_Group->IsAssistant(GetPlayer()->GetGUID()) && !(l_Group->GetPartyFlags() & PARTY_FLAG_EVERYONE_IS_ASSISTANT))
        return;

    l_Group->SetTargetIcon(l_Symbol, m_Player->GetGUID(), l_Target, l_PartyIndex);
}

void WorldSession::HandleGroupRaidConvertOpcode(WorldPacket& p_RecvData)
{
    Group* l_Group = GetPlayer()->GetGroup();

    if (!l_Group)
        return;

    if (m_Player->InBattleground())
        return;

    // Error handling
    if (!l_Group->IsLeader(GetPlayer()->GetGUID()) || l_Group->GetMembersCount() < 2)
        return;

    // Everything's fine, do it (is it 0 (PARTY_OP_INVITE) correct code)
    SendPartyResult(PARTY_CMD_INVITE, "", ERR_PARTY_RESULT_OK);

    // New 4.x: it is now possible to convert a raid to a group if member count is 5 or less
    bool l_Raid;
    p_RecvData >> l_Raid;

    if (l_Group->isRaidGroup())
        l_Group->ConvertToGroup();
    else
        l_Group->ConvertToRaid();
}

void WorldSession::HandleGroupChangeSubGroupOpcode(WorldPacket& recvData)
{
    // we will get correct pointer for group here, so we don't have to check if group is BG raid
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    time_t now = time(NULL);
    if (now - timeLastChangeSubGroupCommand < 2)
        return;
    else
       timeLastChangeSubGroupCommand = now;

    uint64 guid;
    uint8 groupNr, unk;

    recvData.readPackGUID(guid);
    recvData >> unk >> groupNr;

    if (groupNr >= MAX_RAID_SUBGROUPS)
        return;

    uint64 senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetPartyFlags() & PARTY_FLAG_EVERYONE_IS_ASSISTANT))
        return;

    if (!group->HasFreeSlotSubGroup(groupNr))
        return;

    if (Player* movedPlayer = sObjectAccessor->FindPlayer(guid)) ///< is unused movedPlayer
        group->ChangeMembersGroup(guid, groupNr);
}

void WorldSession::HandleGroupSwapSubGroupOpcode(WorldPacket& recvData)
{
    uint8 unk1;
    uint64 guid1;
    uint64 guid2;

    recvData >> unk1;
    recvData.readPackGUID(guid2);
    recvData.readPackGUID(guid1);
}

void WorldSession::HandleGroupEveryoneIsAssistantOpcode(WorldPacket & p_Packet)
{
    Group * l_Group = GetPlayer()->GetGroup();

    if (!l_Group)
        return;

    if (!l_Group->IsLeader(GetPlayer()->GetGUID()))
        return;

    p_Packet.read_skip<uint8>();        ///< Party Index

    bool l_Apply = p_Packet.ReadBit();
    p_Packet.FlushBits();

    l_Group->ChangeFlagEveryoneAssistant(l_Apply);
}

void WorldSession::HandleGroupAssistantLeaderOpcode(WorldPacket& recvData)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    if (!group->IsLeader(GetPlayer()->GetGUID()))
        return;

    uint64 guid;
    uint8 unk = 0;

    recvData >> unk;
    recvData.readPackGUID(guid);
    bool apply = recvData.ReadBit();

    group->SetGroupMemberFlag(guid, apply, MEMBER_FLAG_ASSISTANT);
    group->SendUpdate();
}

void WorldSession::HandlePartyAssignmentOpcode(WorldPacket& recvData)
{
    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    uint64 senderGuid = GetPlayer()->GetGUID();
    if (!group->IsLeader(senderGuid) && !group->IsAssistant(senderGuid) && !(group->GetPartyFlags() & PARTY_FLAG_EVERYONE_IS_ASSISTANT))
        return;

    uint8 assignment, unk;
    uint64 guid;

    recvData >> unk >> assignment;
    recvData.readPackGUID(guid);
    bool apply = recvData.ReadBit();

    recvData.FlushBits();

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

void WorldSession::HandleRaidLeaderReadyCheck(WorldPacket& p_RecvData)
{
    uint8 l_PartyIndex;
    p_RecvData >> l_PartyIndex;

    Group* l_Group = GetPlayer()->GetGroup();
    if (!l_Group)
        return;

    if (!l_Group->IsLeader(GetPlayer()->GetGUID()) && !l_Group->IsAssistant(GetPlayer()->GetGUID()) && !(l_Group->GetPartyFlags() & PARTY_FLAG_EVERYONE_IS_ASSISTANT))
        return;

    l_Group->SetReadyCheckCount(1);

    WorldPacket l_Response(SMSG_READY_CHECK_STARTED);

    l_Response << uint8(l_PartyIndex);
    l_Response.appendPackGUID(l_Group->GetGUID());
    l_Response.appendPackGUID(GetPlayer()->GetGUID());
    l_Response << uint32(35000);          ///< Duration

    l_Group->BroadcastPacket(&l_Response, false, -1);
    l_Group->OfflineReadyCheck();
}

void WorldSession::HandleRaidConfirmReadyCheck(WorldPacket& p_RecvData)
{
    uint8 l_PartyIndex;
    bool l_IsIsReady;

    p_RecvData >> l_PartyIndex;
    l_IsIsReady = p_RecvData.ReadBit();

    Group* l_Group = GetPlayer()->GetGroup();

    if (!l_Group)
        return;

    l_Group->SetReadyCheckCount(l_Group->GetReadyCheckCount() + 1);

    WorldPacket l_Response(SMSG_READY_CHECK_RESPONSE);

    l_Response.appendPackGUID(l_Group->GetGUID());
    l_Response.appendPackGUID(GetPlayer()->GetGUID());
    l_Response.WriteBit(l_IsIsReady);
    l_Response.FlushBits();

    l_Group->BroadcastPacket(&l_Response, true);

    // Send SMSG_READY_CHECK_COMPLETED
    if (l_Group->GetReadyCheckCount() >= l_Group->GetMembersCount())
    {
        l_Response.Initialize(SMSG_READY_CHECK_COMPLETED);

        l_Response << uint8(l_PartyIndex);
        l_Response.appendPackGUID(l_Group->GetGUID());

        l_Group->BroadcastPacket(&l_Response, true);
    }
}

void WorldSession::BuildPartyMemberStatsChangedPacket(Player* p_Player, WorldPacket* p_Data, uint32 /*p_Mask*/, bool p_Ennemy /*= false*/)
{
    assert(p_Player && p_Data);

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

    p_Data->Initialize(SMSG_PARTY_MEMBER_STATE_FULL, 1024);

    p_Data->WriteBit(p_Ennemy);                  ///< ForEnemy
    p_Data->FlushBits();
    p_Data->appendPackGUID(p_Player->GetGUID());
    *p_Data << uint8(1);                        ///< Unk, maybe "instance" status
    *p_Data << uint8(0);                        ///< MEMBER_STATUS_*
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
    *p_Data << uint16(0);                     ///< On Trinity it's GetZoneId
    *p_Data << uint32(0);
    *p_Data << uint16(p_Player->GetPositionX());
    *p_Data << uint16(p_Player->GetPositionY());
    *p_Data << uint16(p_Player->GetPositionZ());
    *p_Data << uint32();                       ///< VehicleSeat

    uint8 l_AuraCount = 0;
    size_t l_AuraPos = p_Data->wpos();
    *p_Data << uint32(l_AuraCount);

    *p_Data << uint32(p_Player->GetPhaseMask());
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
                l_AuraCount++;

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
                    for (uint8 l_Y = 0; l_Y < l_AuraApplication->GetEffectCount(); ++l_Y)
                    {
                        if (AuraEffect const* l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                            l_EffectCount++;
                    }
                }

                *p_Data << uint32(l_AuraApplication->GetBase()->GetId());
                *p_Data << uint8(l_AuraApplication->GetFlags());
                *p_Data << uint32(0);
                *p_Data << uint32(l_EffectCount);

                if (l_AuraApplication->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                {
                    for (uint8 l_Y = 0; l_Y < l_AuraApplication->GetEffectCount(); ++l_Y)
                    {
                        if (AuraEffect const* l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                            *p_Data << float(l_Effect->GetAmount());
                    }
                }
            }
        }

        p_Data->put(l_AuraPos, l_AuraCount);
    }

    if (p_Data->WriteBit(p_Player->GetPet() != 0))
    {
        Pet* l_Pet = p_Player->GetPet();

        p_Data->FlushBits();
        p_Data->appendPackGUID(l_Pet->GetGUID());
        *p_Data << uint16(l_Pet->GetDisplayId());
        *p_Data << uint32(l_Pet->GetHealth());
        *p_Data << uint32(l_Pet->GetMaxHealth());

        uint8 l_PetAuraCount = 0;
        size_t l_PetAuraPos = p_Data->wpos();
        *p_Data << uint32(l_PetAuraCount);

        uint64 l_AuraMask = l_Pet->GetAuraUpdateMaskForRaid();

        for (uint32 l_AuraIT = 0; l_AuraIT < MAX_AURAS; ++l_AuraIT)
        {
            if (l_AuraMask & (uint64(1) << l_AuraIT))
            {
                AuraApplication const* l_AuraApplication = l_Pet->GetVisibleAura(l_AuraIT);
                l_PetAuraCount++;

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
                    for (uint8 l_Y = 0; l_Y < l_AuraApplication->GetEffectCount(); ++l_Y)
                    {
                        if (AuraEffect const* l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                            l_EffectCount++;
                    }
                }

                *p_Data << uint32(l_AuraApplication->GetBase()->GetId());
                *p_Data << uint8(l_AuraApplication->GetFlags());
                *p_Data << uint32(0);
                *p_Data << uint32(l_EffectCount);

                if (l_AuraApplication->GetFlags() & AFLAG_ANY_EFFECT_AMOUNT_SENT)
                {
                    for (uint8 l_Y = 0; l_Y < l_AuraApplication->GetEffectCount(); ++l_Y)
                    {
                        if (AuraEffect const* l_Effect = l_AuraApplication->GetBase()->GetEffect(l_Y))
                            *p_Data << float(l_Effect->GetAmount());
                    }
                }
            }
        }

        p_Data->put(l_PetAuraPos, l_PetAuraCount);
        p_Data->WriteBits(l_Pet->GetName() ? strlen(l_Pet->GetName()) : 0, 8);

        if (l_Pet->GetName())
            p_Data->WriteString(l_Pet->GetName());
    }
    else
        p_Data->FlushBits();
}

/*this procedure handles clients CMSG_REQUEST_PARTY_MEMBER_STATS request*/
void WorldSession::HandleRequestPartyMemberStatsOpcode(WorldPacket& recvData)
{
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
    BuildPartyMemberStatsChangedPacket(player, &data, mask);
    SendPacket(&data);
}

void WorldSession::HandleRequestRaidInfoOpcode(WorldPacket& /*recvData*/)
{
    // every time the player checks the character screen
    m_Player->SendRaidInfo();
}

void WorldSession::HandleOptOutOfLootOpcode(WorldPacket& p_RecvData)
{
    bool l_OptOutOfLoot = p_RecvData.ReadBit();

    // ignore if player not loaded
    if (!GetPlayer())                                        // needed because STATUS_AUTHED
    {
        if (l_OptOutOfLoot)
            sLog->outError(LOG_FILTER_NETWORKIO, "CMSG_OPT_OUT_OF_LOOT value<>0 for not-loaded character!");

        return;
    }

    GetPlayer()->SetPassOnGroupLoot(l_OptOutOfLoot);
}

void WorldSession::HandleRolePollBegin(WorldPacket& p_RecvData)
{
    uint8 l_PartyIndex = 0;

    p_RecvData >> l_PartyIndex;

    Group* l_Group = GetPlayer()->GetGroup();

    if (!l_Group)
        return;

    WorldPacket l_Response(SMSG_ROLL_POLL_INFORM);
    l_Response << uint8(l_PartyIndex);
    l_Response.appendPackGUID(GetPlayer()->GetGUID());

    l_Group->BroadcastPacket(&l_Response, false, -1);
}

void WorldSession::HandleRequestJoinUpdates(WorldPacket& recvData)
{
    uint8 unk = 0;
    recvData >> unk;

    Group* group = GetPlayer()->GetGroup();
    if (!group)
        return;

    group->SendUpdate();
}

void WorldSession::HandleClearRaidMarkerOpcode(WorldPacket& p_RecvData)
{
    uint8 l_MarkerID = p_RecvData.read<uint8>();

    Group* l_Group = m_Player->GetGroup();
    if (!l_Group)
        return;

    if (l_MarkerID < eRaidMarkersMisc::MaxRaidMarkers)
        l_Group->RemoveRaidMarker(l_MarkerID);
    else
        l_Group->RemoveAllRaidMarkers();
}
