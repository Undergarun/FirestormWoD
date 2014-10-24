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
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Log.h"
#include "Opcodes.h"
#include "UpdateData.h"
#include "Player.h"
#include "SocialMgr.h"
#include "ScriptMgr.h"
#include "GameObject.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"

void WorldSession::HandleSendDuelRequest(WorldPacket& p_Packet)
{
    uint64 l_TargetGUID;

    p_Packet.readPackGUID(l_TargetGUID);

    Player* l_Caster   = GetPlayer();
    Unit* l_UnitTarget = nullptr;

    l_UnitTarget = sObjectAccessor->GetUnit(*l_Caster, l_TargetGUID);
    if (!l_UnitTarget || l_Caster->GetTypeId() != TYPEID_PLAYER || l_UnitTarget->GetTypeId() != TYPEID_PLAYER)
        return;

    Player* l_Target = l_UnitTarget->ToPlayer();
    // caster or target already have requested duel
    if (l_Caster->duel || l_Target->duel || !l_Target->GetSocial() || l_Target->GetSocial()->HasIgnore(l_Caster->GetGUIDLow()))
        return;

    l_Caster->CastSpell(l_UnitTarget, 7266, false);
}

void WorldSession::HandleDuelResponseOpcode(WorldPacket& p_Packet)
{
    uint64 l_TargetGUID;
    bool l_Result;

    p_Packet.readPackGUID(l_TargetGUID);
    l_Result = p_Packet.ReadBit();

    Player* l_Player = GetPlayer();
    if (l_Player->duel == nullptr)                        ///< ignore accept from duel-sender
        return;

    if (l_Result)
    {
        Player* l_Target = l_Player->duel->opponent;
        if (l_Player == l_Player->duel->initiator || !l_Target || l_Player == l_Target || l_Player->duel->startTime != 0 || l_Target->duel->startTime != 0)
            return;

        time_t l_Now = time(NULL);
        l_Player->duel->startTimer = l_Now;
        l_Target->duel->startTimer = l_Now;

        l_Player->SendDuelCountdown(3000);
        l_Target->SendDuelCountdown(3000);
    }
    else
    {
        // player surrendered in a duel using /forfeit
        if (l_Player->duel->startTime != 0)
        {
            l_Player->CombatStopWithPets(true);
            if (l_Player->duel->opponent)
                l_Player->duel->opponent->CombatStopWithPets(true);

            l_Player->CastSpell(l_Player, 7267, true);    ///< beg
            l_Player->DuelComplete(DUEL_WON);
            return;
        }
        l_Player->DuelComplete(DUEL_INTERRUPTED);
    }
}
