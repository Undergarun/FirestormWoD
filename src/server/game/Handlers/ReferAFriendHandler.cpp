////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2016 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "WorldSession.h"
#include "Player.h"
#include "ObjectMgr.h"
#include "Opcodes.h"
#include "Log.h"

void WorldSession::HandleGrantLevel(WorldPacket& recvData)
{
    uint64 guid;
    recvData.readPackGUID(guid);

    Player* target = ObjectAccessor::GetObjectInWorld(guid, m_Player);

    // check cheating
    uint8 levels = m_Player->GetGrantableLevels();
    uint8 error = 0;
    if (!target)
        error = ERR_REFER_A_FRIEND_NO_TARGET;
    else if (levels == 0)
        error = ERR_REFER_A_FRIEND_INSUFFICIENT_GRANTABLE_LEVELS;
    else if (GetRecruiterId() != target->GetSession()->GetAccountId())
        error = ERR_REFER_A_FRIEND_NOT_REFERRED_BY;
    else if (target->GetTeamId() != m_Player->GetTeamId())
        error = ERR_REFER_A_FRIEND_DIFFERENT_FACTION;
    else if (target->getLevel() >= m_Player->getLevel())
        error = ERR_REFER_A_FRIEND_TARGET_TOO_HIGH;
    else if (target->getLevel() >= sWorld->getIntConfig(CONFIG_MAX_RECRUIT_A_FRIEND_BONUS_PLAYER_LEVEL))
        error = ERR_REFER_A_FRIEND_GRANT_LEVEL_MAX_I;
    else if (target->GetGroup() != m_Player->GetGroup())
        error = ERR_REFER_A_FRIEND_NOT_IN_GROUP;

    if (error) {
        WorldPacket data(SMSG_REFER_A_FRIEND_FAILURE, 24);
        data << uint32(error);
        if (error == ERR_REFER_A_FRIEND_NOT_IN_GROUP)
            data << target->GetName();

        SendPacket(&data);
        return;
    }

    WorldPacket data2(SMSG_PROPOSE_LEVEL_GRANT, 8);
    data2.append(m_Player->GetPackGUID());
    target->GetSession()->SendPacket(&data2);
}

void WorldSession::HandleAcceptGrantLevel(WorldPacket& recvData)
{
    uint64 guid;
    recvData.readPackGUID(guid);

    Player* other = ObjectAccessor::GetObjectInWorld(guid, m_Player);
    if (!(other && other->GetSession()))
        return;

    if (GetAccountId() != other->GetSession()->GetRecruiterId())
        return;

    if (other->GetGrantableLevels())
        other->SetGrantableLevels(other->GetGrantableLevels() - 1);
    else
        return;

    m_Player->GiveLevel(m_Player->getLevel() + 1);
}
