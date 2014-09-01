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
#include "DBCEnums.h"
#include "ObjectMgr.h"
#include "GuildMgr.h"
#include "World.h"
#include "WorldPacket.h"
#include "DatabaseEnv.h"
#include "AchievementMgr.h"
#include "Arena.h"
#include "CellImpl.h"
#include "GameEventMgr.h"
#include "GridNotifiersImpl.h"
#include "Guild.h"
#include "Language.h"
#include "Player.h"
#include "SpellMgr.h"
#include "DisableMgr.h"
#include "ScriptMgr.h"
#include "MapManager.h"
#include "Battleground.h"
#include "BattlegroundAB.h"
#include "Map.h"
#include "InstanceScript.h"
#include "Group.h"
#include "BattlePetMgr.h"

BattlePetMgr::BattlePetMgr(Player* owner) : m_player(owner)
{
}

void BattlePetMgr::GetBattlePetList(PetBattleDataList &battlePetList) const
{

}

void BattlePetMgr::BuildBattlePetJournal(WorldPacket *data)
{
    PetBattleDataList petList;
    GetBattlePetList(petList);

    ByteBuffer dataBuffer;

    data->Initialize(SMSG_BATTLEPET_JOURNAL);
    data->WriteBits(petList.size(), 19);

    // bits part
    for (auto pet : petList)
    {
        // Not sent for the moment, pig pig
        ObjectGuid petGuid = uint64(pet.m_summonSpellID);

        data->WriteBit(petGuid[7]);
        data->WriteBit(true);                   // hasQuality, inverse
        data->WriteBit(petGuid[3]);
        data->WriteBit(petGuid[5]);
        data->WriteBit(petGuid[1]);
        data->WriteBit(petGuid[6]);
        data->WriteBit(false);                  // unk bit
        data->WriteBit(true);                   // hasBreed, inverse
        data->WriteBit(true);                   // hasUnk, inverse
        data->WriteBit(false);                  // hasFirstOwnerGuid
        data->WriteBits(0, 7);                  // name lenght
        data->WriteBit(petGuid[0]);
        data->WriteBit(petGuid[2]);
        data->WriteBit(petGuid[4]);

        dataBuffer << uint16(1);                // Level
        dataBuffer.WriteByteSeq(petGuid[7]);
        dataBuffer << uint32(1);                // Health or MaxHealth
        dataBuffer << uint32(pet.m_speciesID);  // Species
        dataBuffer << uint32(1);                // Speed
        dataBuffer << uint32(1);                // Attack
        dataBuffer << uint32(1);                // MaxHealth or Health
        dataBuffer.WriteByteSeq(petGuid[6]);
        dataBuffer << uint32(pet.m_entry);      // PetEntry
        dataBuffer.WriteByteSeq(petGuid[4]);
        dataBuffer.WriteByteSeq(petGuid[2]);
        dataBuffer.WriteByteSeq(petGuid[3]);
        dataBuffer.WriteByteSeq(petGuid[0]);
        dataBuffer << uint32(0);                // Power ?
        dataBuffer << uint16(0);                // Experience
        dataBuffer.WriteByteSeq(petGuid[1]);
        dataBuffer.WriteByteSeq(petGuid[5]);
    }

    data->WriteBit(1);                          // Unk
    data->WriteBits(0, 25);                     // Battle Team
    data->FlushBits();

    if (dataBuffer.size())
        data->append(dataBuffer);

    *data << uint16(0);                         // Unk
}
