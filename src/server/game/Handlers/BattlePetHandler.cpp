////////////////////////////////////////////////////////////////////////////////
//
//  MILLENIUM-STUDIO
//  Copyright 2014-2015 Millenium-studio SARL
//  All Rights Reserved.
//
////////////////////////////////////////////////////////////////////////////////

#include "Common.h"
#include "ObjectAccessor.h"
#include "ObjectMgr.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include "Player.h"
#include "PetBattle.h"
#include "WildBattlePet.h"
#include "AchievementMgr.h"
#include "CellImpl.h"
#include "GridNotifiers.h"
#include "GridNotifiersImpl.h"

void WorldSession::SendBattlePetUpdates(bool p_AddedPet)
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    std::vector<BattlePet::Ptr> l_Pets = m_Player->GetBattlePets();

    WorldPacket l_Packet(SMSG_BATTLE_PET_UPDATES, 15 * 1024);
    l_Packet << uint32(l_Pets.size());                                                                      ///< Pets count

    for (std::vector<BattlePet::Ptr>::iterator l_It = l_Pets.begin(); l_It != l_Pets.end(); ++l_It)
    {
        BattlePet::Ptr l_Pet = (*l_It);

        uint64 l_Guid = l_Pet->JournalID;
        bool l_HasOwnerInfo = false;
        BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(l_Pet->Species);

        l_Pet->UpdateStats();

        l_Packet.appendPackGUID(l_Guid);                                                                    ///< BattlePetGUID
        l_Packet << uint32(l_Pet->Species);                                                                 ///< SpeciesID
        l_Packet << uint32(l_SpeciesInfo ? l_SpeciesInfo->entry : 0);                                       ///< CreatureID
        l_Packet << uint32(l_Pet->DisplayModelID);                                                          ///< DisplayID
        l_Packet << uint16(l_Pet->Breed);                                                                   ///< BreedID
        l_Packet << uint16(l_Pet->Level);                                                                   ///< Level
        l_Packet << uint16(l_Pet->XP);                                                                      ///< Xp
        l_Packet << uint16(l_Pet->Flags);                                                                   ///< BattlePetDBFlags
        l_Packet << int32(l_Pet->InfoPower);                                                                ///< Power
        l_Packet << int32(l_Pet->Health > l_Pet->InfoMaxHealth ? l_Pet->InfoMaxHealth : l_Pet->Health);     ///< Health
        l_Packet << int32(l_Pet->InfoMaxHealth);                                                            ///< MaxHealth
        l_Packet << int32(l_Pet->InfoSpeed);                                                                ///< Speed
        l_Packet << uint8(l_Pet->Quality);                                                                  ///< BreedQuality

        l_Packet.WriteBits(l_Pet->Name.length(), 7);                                                        ///< CustomName
        l_Packet.WriteBit(l_HasOwnerInfo);                                                                  ///< HasOwnerInfo
        l_Packet.WriteBit(l_Pet->Name.empty());                                                             ///< NoRename
        l_Packet.FlushBits();

        l_Packet.WriteString(l_Pet->Name);

        if (l_HasOwnerInfo)
        {
            uint64 l_OwnerGUID = 0;

            l_Packet.appendPackGUID(l_OwnerGUID);                                                           ///< Guid
            l_Packet << uint32(g_RealmID);                                                                  ///< PlayerVirtualRealm
            l_Packet << uint32(g_RealmID);                                                                  ///< PlayerNativeRealm
        }
    }

    l_Packet.WriteBit(p_AddedPet);                                                                          ///< HasJournalLock
    l_Packet.FlushBits();

    SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetTrapLevel()
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    WorldPacket l_Packet(SMSG_BATTLE_PET_TRAP_LEVEL, 2);
    l_Packet << uint32(m_Player->GetBattlePetTrapLevel());

    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetJournalLockAcquired()
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_JOURNAL_LOCK_ACQUIRED, 0);
    m_Player->GetSession()->SendPacket(&l_Packet);

    m_IsPetBattleJournalLocked = true;
}

void WorldSession::SendBattlePetJournalLockDenied()
{
    m_IsPetBattleJournalLocked = false;

    WorldPacket l_Packet(SMSG_BATTLE_PET_JOURNAL_LOCK_DENIED, 0);
    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetJournal()
{
    if (!m_Player || !m_Player->IsInWorld())
        return;

    std::vector<BattlePet::Ptr>     l_Pets              = m_Player->GetBattlePets();
    uint32                          l_UnlockedSlotCount = m_Player->GetUnlockedPetBattleSlot();
    BattlePet::Ptr                * l_PetSlots          = m_Player->GetBattlePetCombatTeam();

    if (l_UnlockedSlotCount > 0)
        m_Player->SetFlag(PLAYER_FIELD_PLAYER_FLAGS, PLAYER_FLAGS_HAS_BATTLE_PET_TRAINING);

    WorldPacket l_Packet(SMSG_BATTLE_PET_JOURNAL, 15 * 1024);
    l_Packet << uint16(m_Player->GetBattlePetTrapLevel());                                                  ///< Trap level
    l_Packet << uint32(MAX_PETBATTLE_SLOTS);                                                                                  ///< Slots count
    l_Packet << uint32(l_Pets.size());                                                                      ///< Pets count

    for (uint32 l_I = 0; l_I < MAX_PETBATTLE_SLOTS; l_I++)
    {
        uint64 l_Guid = 0;

        bool l_IsLocked = false;

        //if (m_Player->HasBattlePetTraining() && (l_I + 1) <= l_UnlockedSlotCount)
        //    l_IsLocked = false;

        if (l_PetSlots[l_I])
            l_Guid = l_PetSlots[l_I]->JournalID;

        l_Packet.appendPackGUID(l_Guid);                                                                    ///< BattlePetGUID
        l_Packet << uint32(0);                                                                              ///< CollarID
        l_Packet << uint8(l_I);                                                                             ///< SlotIndex
        l_Packet.WriteBit(!((l_I + 1) <= l_UnlockedSlotCount));                                             ///< Locked
        l_Packet.FlushBits();
    }

    for (std::vector<BattlePet::Ptr>::iterator l_It = l_Pets.begin(); l_It != l_Pets.end(); ++l_It)
    {
        BattlePet::Ptr l_Pet = (*l_It);

        uint64 l_Guid = l_Pet->JournalID;
        bool l_HasOwnerInfo = false;
        BattlePetSpeciesEntry const* l_SpeciesInfo = sBattlePetSpeciesStore.LookupEntry(l_Pet->Species);

        l_Pet->UpdateStats();

        l_Packet.appendPackGUID(l_Guid);                                                                    ///< BattlePetGUID
        l_Packet << uint32(l_Pet->Species);                                                                 ///< SpeciesID
        l_Packet << uint32(l_SpeciesInfo ? l_SpeciesInfo->entry : 0);                                       ///< CreatureID
        l_Packet << uint32(l_Pet->DisplayModelID);                                                          ///< DisplayID
        l_Packet << uint16(l_Pet->Breed);                                                                   ///< BreedID
        l_Packet << uint16(l_Pet->Level);                                                                   ///< Level
        l_Packet << uint16(l_Pet->XP);                                                                      ///< Xp
        l_Packet << uint16(l_Pet->Flags);                                                                   ///< BattlePetDBFlags
        l_Packet << int32(l_Pet->InfoPower);                                                                ///< Power
        l_Packet << int32(l_Pet->Health > l_Pet->InfoMaxHealth ? l_Pet->InfoMaxHealth : l_Pet->Health);     ///< Health
        l_Packet << int32(l_Pet->InfoMaxHealth);                                                            ///< MaxHealth
        l_Packet << int32(l_Pet->InfoSpeed);                                                                ///< Speed
        l_Packet << uint8(l_Pet->Quality);                                                                  ///< BreedQuality

        l_Packet.WriteBits(l_Pet->Name.length(), 7);                                                        ///< CustomName
        l_Packet.WriteBit(l_HasOwnerInfo);                                                                  ///< HasOwnerInfo
        l_Packet.WriteBit(l_Pet->Name.empty());                                                             ///< NoRename
        l_Packet.FlushBits();

        l_Packet.WriteString(l_Pet->Name);

        if (l_HasOwnerInfo)
        {
            uint64 l_OwnerGUID = 0;

            l_Packet.appendPackGUID(l_OwnerGUID);                                                           ///< Guid
            l_Packet << uint32(g_RealmID);                                                                  ///< PlayerVirtualRealm
            l_Packet << uint32(g_RealmID);                                                                  ///< PlayerNativeRealm
        }
    }

    l_Packet.WriteBit(true);                                                                                ///< HasJournalLock
    l_Packet.FlushBits();

    SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetDeleted(uint64 p_BattlePetGUID)
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_DELETED, 2 + 16);
    l_Packet.appendPackGUID(p_BattlePetGUID);

    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetRevoked(uint64 p_BattlePetGUID)
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_REVOKED, 2 + 16);
    l_Packet.appendPackGUID(p_BattlePetGUID);

    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetRestored(uint64 p_BattlePetGUID)
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_RESTORED, 2 + 16);
    l_Packet.appendPackGUID(p_BattlePetGUID);

    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetsHealed()
{
    WorldPacket l_Packet(SMSG_BATTLE_PETS_HEALED, 0);
    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetLicenseChanged()
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_LICENSE_CHANGED, 0);
    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetError(uint32 p_Result, uint32 p_CreatureID)
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_ERROR, 1 + 4);
    l_Packet.WriteBits(p_Result, 4);
    l_Packet.FlushBits();
    l_Packet << uint32(p_CreatureID);

    m_Player->GetSession()->SendPacket(&l_Packet);
}

void WorldSession::SendBattlePetCageDateError(uint32 p_SecondsUntilCanCage)
{
    WorldPacket l_Packet(SMSG_BATTLE_PET_CAGE_DATE_ERROR, 4);
    l_Packet << uint32(p_SecondsUntilCanCage);

    m_Player->GetSession()->SendPacket(&l_Packet);
}

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

void WorldSession::HandleBattlePetQueryName(WorldPacket& p_RecvData)
{
    uint64 l_UnitGuid;
    uint64 l_JournalGuid;

    p_RecvData.readPackGUID(l_JournalGuid);
    p_RecvData.readPackGUID(l_UnitGuid);

    Creature* l_Creature = Unit::GetCreature(*m_Player, l_UnitGuid);

    if (!l_Creature)
        return;

    BattlePet::Ptr l_BattlePet = nullptr;

    if (l_Creature->GetOwner() && l_Creature->GetOwner()->IsPlayer())
        l_BattlePet = l_Creature->GetOwner()->ToPlayer()->GetBattlePet(l_JournalGuid);

    if (!l_BattlePet)
        return;

    bool l_HaveCustomName = l_Creature->GetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP) != 0;
    bool l_HaveDeclinedNames = false;

    if (l_BattlePet)
    {
        for (int l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
        {
            if (!l_BattlePet->DeclinedNames[l_I].empty())
                l_HaveDeclinedNames = true;
        }
    }

    WorldPacket l_Packet(SMSG_QUERY_BATTLE_PET_NAME_RESPONSE, 0x40);

    l_Packet.appendPackGUID(l_JournalGuid);
    l_Packet << uint32(l_Creature->GetEntry());
    l_Packet << uint32(l_Creature->GetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP));
    l_Packet.WriteBit(l_HaveCustomName);

    if (l_HaveCustomName)
    {
        l_Packet.WriteBits(l_HaveCustomName ? strlen(l_Creature->GetName()) : 0, 8);
        l_Packet.WriteBit(l_HaveDeclinedNames);

        for (uint32 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_Packet.WriteBits(l_BattlePet ? l_BattlePet->DeclinedNames[l_I].size() : 0, 7);
    }

    l_Packet.FlushBits();

    if (l_HaveCustomName)
    {
        l_Packet.WriteString(l_Creature->GetName());

        for (uint32 l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_Packet.WriteString(l_BattlePet ? l_BattlePet->DeclinedNames[l_I] : "");
    }

    m_Player->GetSession()->SendPacket(&l_Packet);
}

/// [INTERNAL]
void WorldSession::HandleBattlePetsReconvert(WorldPacket& p_RecvData)
{
    /// Internal handler
}

void WorldSession::HandleBattlePetUpdateNotify(WorldPacket& p_RecvData)
{
    uint64 l_BattlePetGUID;
    p_RecvData.readPackGUID(l_BattlePetGUID);

    /// Nothing todo the client received a battle pet update
}

void WorldSession::HandleBattlePetRequestJournalLock(WorldPacket& /*p_RecvData*/)
{
    if (m_IsPetBattleJournalLocked)
        SendBattlePetJournalLockAcquired();
    else
        SendBattlePetJournalLockDenied();
}

void WorldSession::HandleBattlePetRequestJournal(WorldPacket& /*p_RecvData*/)
{
    SendBattlePetJournal();
}

void WorldSession::HandleBattlePetDeletePet(WorldPacket& p_RecvData)
{
    uint64 l_BattlePetGUID;
    p_RecvData.readPackGUID(l_BattlePetGUID);

    /// @TODO
}

void WorldSession::HandleBattlePetDeletePetCheat(WorldPacket& p_RecvData)
{
    uint64 l_BattlePetGUID;
    p_RecvData.readPackGUID(l_BattlePetGUID);

    /// @TODO
}

/// [INTERNAL]
void WorldSession::HandleBattlePetDeleteJournal(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

void WorldSession::HandleBattlePetModifyName(WorldPacket& p_RecvData)
{
    DeclinedName    l_DeclinedNames;
    uint64          l_PetJournalID;
    bool            l_HaveDeclinedNames = false;
    uint32          l_NameLenght        = 0;
    std::string     l_Name;

    uint32 l_DeclinedNameLens[MAX_DECLINED_NAME_CASES];

    p_RecvData.readPackGUID(l_PetJournalID);
    l_NameLenght        = p_RecvData.ReadBits(7);
    l_HaveDeclinedNames = p_RecvData.ReadBit();

    l_Name = p_RecvData.ReadString(l_NameLenght);

    if (l_HaveDeclinedNames)
    {
        p_RecvData.ResetBitReading();

        for (size_t l_I = 0 ; l_I < MAX_DECLINED_NAME_CASES ; ++l_I)
            l_DeclinedNameLens[l_I] = p_RecvData.ReadBits(7);

        for (size_t l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
            l_DeclinedNames.name[l_I] = p_RecvData.ReadString(l_DeclinedNameLens[l_I]);
    }

    PetNameInvalidReason l_NameInvalidReason = sObjectMgr->CheckPetName(l_Name);
    if (l_NameInvalidReason != PET_NAME_SUCCESS)
    {
        SendPetNameInvalid(l_NameInvalidReason, l_Name, (l_HaveDeclinedNames) ? &l_DeclinedNames : NULL);
        return;
    }

    uint32 l_TimeStamp = l_Name.empty() ? 0 : time(0);

    BattlePet::Ptr l_BattlePet = m_Player->GetBattlePet(l_PetJournalID);

    if (l_BattlePet)
    {
        l_BattlePet->Name           = l_Name;
        l_BattlePet->NameTimeStamp  = l_TimeStamp;

        if (l_HaveDeclinedNames)
        {
            for (size_t l_I = 0; l_I < MAX_DECLINED_NAME_CASES; ++l_I)
                l_BattlePet->DeclinedNames[l_I] = l_DeclinedNames.name[l_I];
        }
    }

    m_Player->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, l_TimeStamp);

    Creature* l_Creature = m_Player->GetSummonedBattlePet();

    if (!l_Creature)
        return;

    if (l_Creature->GetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_GUID) == l_PetJournalID)
    {
        l_Creature->SetName(l_Name);
        l_Creature->SetUInt32Value(UNIT_FIELD_BATTLE_PET_COMPANION_NAME_TIMESTAMP, l_TimeStamp);
    }
}

void WorldSession::HandleBattlePetSummon(WorldPacket& recvData)
{
    uint64 l_JournalID;

    recvData.readPackGUID(l_JournalID);

    if (m_Player->GetSummonedBattlePet() && m_Player->GetSummonedBattlePet()->GetGuidValue(UNIT_FIELD_BATTLE_PET_COMPANION_GUID) == l_JournalID)
        m_Player->UnsummonCurrentBattlePetIfAny(false);
    else
    {
        m_Player->UnsummonCurrentBattlePetIfAny(false);
        m_Player->SummonBattlePet(l_JournalID);
    }
}

/// [INTERNAL]
void WorldSession::HandleBattlePetSetLevel(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

void WorldSession::HandleBattlePetSetBattleSlot(WorldPacket& p_RecvData)
{
    if (m_IsPetBattleJournalLocked)
        return;

    uint64  l_PetJournalID;
    uint8   l_DestSlot = 0;

    p_RecvData.readPackGUID(l_PetJournalID);
    p_RecvData >> l_DestSlot;

    if (l_DestSlot >= MAX_PETBATTLE_SLOTS)
        return;

    BattlePet::Ptr   l_BattlePet = m_Player->GetBattlePet(l_PetJournalID);
    BattlePet::Ptr * l_PetSlots = m_Player->GetBattlePetCombatTeam();

    if (l_BattlePet)
    {
        for (uint8 l_I = 0; l_I < MAX_PETBATTLE_SLOTS; ++l_I)
        {
            if (l_PetSlots[l_I] && l_PetSlots[l_I]->Slot == l_DestSlot)
                l_PetSlots[l_I]->Slot = l_BattlePet->Slot;
        }

        l_BattlePet->Slot = l_DestSlot;
    }

    m_Player->UpdateBattlePetCombatTeam();
    SendPetBattleSlotUpdates(false);
}

/// [INTERNAL]
void WorldSession::HandleBattlePetSetCollar(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

void WorldSession::HandleBattlePetSetFlags(WorldPacket& p_RecvData)
{
    uint64 l_PetJournalID;
    uint32 l_Flag = 0;
    uint8 l_Action = 0;

    p_RecvData.readPackGUID(l_PetJournalID);
    p_RecvData >> l_Flag;

    l_Action = p_RecvData.ReadBits(2);  ///< 0 add flag, 2 remove it ///< l_Action is never read 01/18/16

    BattlePet::Ptr l_BattlePet = m_Player->GetBattlePet(l_PetJournalID);

    if (l_BattlePet)
    {
        if (l_BattlePet->Flags & l_Flag)
            l_BattlePet->Flags = l_BattlePet->Flags & ~l_Flag;
        else
            l_BattlePet->Flags |= l_Flag;
    }
}

/// [INTERNAL]
void WorldSession::HandleBattlePetsRestoreHealth(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

/// [INTERNAL]
void WorldSession::HandleBattlePetAdd(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

/// [INTERNAL]
void WorldSession::HandleBattlePetSetQualityCheat(WorldPacket& /*p_RecvData*/)
{
    /// Internal handler
}

void WorldSession::HandleBattlePetCage(WorldPacket& p_RecvData)
{
    /// @TODO
}